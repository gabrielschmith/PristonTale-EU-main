#include "stdafx.h"
#include "mapserver.h"
#include "picosha2.h"
#include "Logger.h"
#include "HNSSkill.h"

EMapID * MapServer::piBabelStormMapID = (EMapID *)0x006E2C30;
DWORD * MapServer::pdwBabelStormEndTime = (DWORD *)0x07AC9D54;

typedef Map * ( __cdecl *t_GetStageByXZ) ( int X, int Z );
t_GetStageByXZ GetStageByXZ = ( t_GetStageByXZ)0x0054F700;

typedef int( __thiscall *t_SetNewSpawn ) ( int point, int X, int Z, int v );
t_SetNewSpawn SetNewSpawn = ( t_SetNewSpawn )0x0054F280;

typedef void( __thiscall *t_DeleteFlagPoint )(int iPoint, int iX, int iZ);
t_DeleteFlagPoint fnDeleteFlagPoint = (t_DeleteFlagPoint)0x0054F300;

typedef void( __thiscall *t_SendFlagPoint )(int iPoint);
t_SendFlagPoint SendFlagPoint = (t_SendFlagPoint)0x0054F090;


typedef void(__thiscall* t_LoadStage)(Map * map, LPCSTR lpString);
t_LoadStage LoadStage = (t_LoadStage)0x0054F0F0;


BOOL * pbIsCrystalMonster		= (BOOL*)(0x08B81830);
BOOL * pbIsSummonedGMMonster	= (BOOL*)(0x08B81834);
BOOL * pbIsSummonedMonster		= (BOOL*)(0x08B81838);



#define ITEM_TRANS_LIMIT_TIME	16000

MapServer::MapServer()
{
	iGameTimeShift = 0;
	pcaBaseMap = (BaseMap*)0x0075B038;

	//not sure why +1  is added here. Let's just leave it
	for ( int i = 0; i < (NUM_MAPS + 1); i++ )
		vSpawnMapsInfo.push_back( new SpawnDataInfo( i ) );
}

MapServer::~MapServer()
{
}

BOOL MapServer::IsMapSpawnPositionExists( int iMapID, int iX, int iZ )
{
	int dX, dZ;

	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		for ( SpawnDataInfo::SpawnData * ps : vSpawnMapsInfo[iMapID]->vSpawnData )
		{
			dX = abs(ps->iX - iX);
			dZ = abs(ps->iZ - iZ);

			if ( ps && dX <= 5 && dZ <= 5 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void MapServer::SendMapIndicators( User * pcUser, int iMapID, BOOL bForceSend )
{
	if ( LOGIN_SERVER )
		return;

	if ( iMapID < 0 && iMapID >= NUM_MAPS )
		return;

	auto iter = mMapIndicators.find( iMapID );
	if ( iter == mMapIndicators.end() )
	{
		if ( bForceSend )
		{
			PacketMapIndicators mPacket;
			ZeroMemory( &mPacket, sizeof( PacketMapIndicators ) );
			mPacket.iHeader = PKTHDR_MapIndicators;
			mPacket.iLength = sizeof( PacketMapIndicators ) - ( ( MAX_MAP_INDICATORS ) * sizeof( MapIndicator ) );
			mPacket.iMapID = iMapID;
			mPacket.iCount = 0;

			SENDPACKET( pcUser, &mPacket );
		}

		return;
	}

	SENDPACKET( pcUser, &iter->second );
}

BOOL MapServer::AddMapIndicator( User * pcUser, int iMapID, MapIndicator::Type eMapIndicatorType, int iValue, int iCompactPosX, int iCompactPosZ, int iAngleY )
{
	if ( LOGIN_SERVER )
		return FALSE;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return FALSE;

	auto pcDB = SQLCONNECTION( EDatabaseID::DATABASEID_GameDB, 10 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO MapIndicator (MapID, Type, Value, PosX, PosZ, AngleY) VALUES ( ?, ?, ?, ?, ?, ? )" ) )
		{
			pcDB->BindParameterInput( 1, EDatabaseDataType::PARAMTYPE_Integer, &iMapID );
			pcDB->BindParameterInput( 2, EDatabaseDataType::PARAMTYPE_Integer, &eMapIndicatorType );
			pcDB->BindParameterInput( 3, EDatabaseDataType::PARAMTYPE_Integer, &iValue );
			pcDB->BindParameterInput( 4, EDatabaseDataType::PARAMTYPE_Integer, &iCompactPosX );
			pcDB->BindParameterInput( 5, EDatabaseDataType::PARAMTYPE_Integer, &iCompactPosZ );
			pcDB->BindParameterInput( 6, EDatabaseDataType::PARAMTYPE_Integer, &iAngleY );

			pcDB->Execute();
		}

		pcDB->Close();
	}

	SQLBuildMapIndicators();
	SendMapIndicators( pcUser, iMapID, TRUE );

	return TRUE;
}

BOOL MapServer::RemoveMapIndicatorNearMe( User * pcUser )
{
	if ( LOGIN_SERVER )
		return FALSE;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return FALSE;

	int iMapId = (int)pcUser->pcUserData->iMapID;
	auto iter = mMapIndicators.find( iMapId );
	if ( iter == mMapIndicators.end() )
		return FALSE;

	int iDistXY = 999999;
	int iIndex = -1;

	for ( int i = 0; i < iter->second.iCount; i++ )
	{
		int dX = 0, dZ = 0;
		dX = ( pcUser->pcUserData->sPosition.iX >> 8 ) - iter->second.mapIndicators[i].iCompactPosX;
		dZ = ( pcUser->pcUserData->sPosition.iZ >> 8 ) - iter->second.mapIndicators[i].iCompactPosZ;

		int iDist = dX*dX + dZ*dZ;
		if ( iDist < iDistXY && iDist < DISTANCE_XY_20_meters )
		{
			iIndex = i;
			iDistXY = iDist;
		}
	}

	if ( iIndex == -1 )
		return FALSE;

	BOOL bResult = FALSE;
	auto pcDB = SQLCONNECTION( EDatabaseID::DATABASEID_GameDB, 9 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM MapIndicator WHERE MapID = ? AND PosX = ? AND PosZ = ?" ) )
		{
			pcDB->BindParameterInput( 1, EDatabaseDataType::PARAMTYPE_Integer, &iMapId );
			pcDB->BindParameterInput( 2, EDatabaseDataType::PARAMTYPE_Integer, &iter->second.mapIndicators[iIndex].iCompactPosX );
			pcDB->BindParameterInput( 3, EDatabaseDataType::PARAMTYPE_Integer, &iter->second.mapIndicators[iIndex].iCompactPosZ );

			bResult = pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}

	if ( bResult )
	{
		SQLBuildMapIndicators(); //rebuild
		SendMapIndicators( pcUser, iMapId, TRUE );
	}

	return bResult;
}

void MapServer::SQLBuildMapIndicators( )
{
	if ( LOGIN_SERVER )
		return;

	mMapIndicators.clear();

	auto pcDB = SQLCONNECTION( EDatabaseID::DATABASEID_GameDB, 8 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MapID, Type, Value, PosX, PosZ, AngleY FROM MapIndicator ORDER BY MapID" ) )
		{
			if ( pcDB->Execute() )
			{
				int iPrevMapID = -1;

				while ( pcDB->Fetch() )
				{
					int iMapID;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iMapID );

					if ( iPrevMapID != iMapID )
					{
						PacketMapIndicators mPacket;
						ZeroMemory( &mPacket, sizeof( PacketMapIndicators ) );
						mMapIndicators.insert( { iMapID, mPacket } );

						mMapIndicators[iMapID].iMapID = iMapID;

						iPrevMapID = iMapID;
					}

					if ( mMapIndicators[iMapID].iCount >= MAX_MAP_INDICATORS )
					{
						WARN( "Too many indicators for map id: %d", iMapID );
						continue;
					}

					auto instance = &mMapIndicators[iMapID].mapIndicators[mMapIndicators[iMapID].iCount];


					int iIndicatorType;

					pcDB->GetData( 2, PARAMTYPE_Integer, &iIndicatorType );
					pcDB->GetData( 3, PARAMTYPE_Integer, &instance->iValue );
					pcDB->GetData( 4, PARAMTYPE_Integer, &instance->iCompactPosX );
					pcDB->GetData( 5, PARAMTYPE_Integer, &instance->iCompactPosZ );
					pcDB->GetData( 6, PARAMTYPE_Integer, &instance->iAngleY );

					instance->eType = (MapIndicator::Type)iIndicatorType;

					mMapIndicators[iMapID].iCount++;
				}
			}
		}

		pcDB->Close();
	}

	for ( auto & kvp : mMapIndicators )
	{
		kvp.second.iHeader = PKTHDR_MapIndicators;
		kvp.second.iLength = sizeof( PacketMapIndicators ) - ( ( MAX_MAP_INDICATORS - kvp.second.iCount ) * sizeof( MapIndicator ) );
	}
}

void MapServer::AddMapSpawnData( int iMapID, const char * pszName, int iID, int iX, int iZ )
{
	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		if ( IsMapSpawnPositionExists( iMapID, iX, iZ ) )
		{
			INFO( "MapServer::AddMapSpawnData Skipped. Spawn already exists for Map %d at X = %d, Z = %d", iMapID, iX, iZ );
			return;
		}

		SpawnDataInfo::SpawnData * psData = new SpawnDataInfo::SpawnData;
		psData->iID = iID;
		psData->iX	= iX;
		psData->iZ	= iZ;
		STRINGCOPY( psData->szName, pszName );
		vSpawnMapsInfo[iMapID]->vSpawnData.push_back( psData );
	}
	else
	{
		WARN ( "MapServer::AddMapSpawnData FAILED! iMapID = %d, vSpawnMapsInfo.size () = %d", iMapID, vSpawnMapsInfo.size () );
	}
}

void MapServer::DelMapSpawnData( int iMapID, int iX, int iZ )
{
	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		for ( std::vector<SpawnDataInfo::SpawnData*>::iterator it = vSpawnMapsInfo[iMapID]->vSpawnData.begin(); it != vSpawnMapsInfo[iMapID]->vSpawnData.end(); it++ )
		{
			SpawnDataInfo::SpawnData * ps = (*it);

			if ( ps && (ps->iX == iX) && (ps->iZ == iZ) )
			{
				SAFE_DELETE( ps );
				vSpawnMapsInfo[iMapID]->vSpawnData.erase( it );
				break;
			}
		}
	}
	else
	{
		WARN ( "MapServer::DelMapSpawnData FAILED! iMapID = %d, vSpawnMapsInfo.size () = %d", iMapID, vSpawnMapsInfo.size () );
	}
}

SpawnDataInfo::SpawnData * MapServer::GetMapSpawnData( int iMapID, int iX, int iZ )
{
	SpawnDataInfo::SpawnData * psRet = NULL;

	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		for ( std::vector<SpawnDataInfo::SpawnData*>::iterator it = vSpawnMapsInfo[iMapID]->vSpawnData.begin(); it != vSpawnMapsInfo[iMapID]->vSpawnData.end(); it++ )
		{
			SpawnDataInfo::SpawnData * ps = (*it);

			if ( ps && (ps->iX == iX) && (ps->iZ == iZ) )
			{
				psRet = ps;
				break;
			}
		}
	}

	return psRet;
}

NAKED UINT MapServer::GetMinuteOfDay()
{
	JMP( pfnGetMinuteOfDay );
}

NAKED BOOL MapServer::HandleUnitQuestMap( void * pcMap, UnitData * pcUnitData )
{
	JMP( pfnQuestUnitMapHandler );
}

void MapServer::SetFlagPoint( UserData * pcUserData, const char * pszDescription )
{
	if ( pcUserData && lstrlenA( pszDescription ) )
	{
		int iX = pcUserData->sPosition.iX;
		int iZ = pcUserData->sPosition.iZ;
		int iStageID = pcUserData->iMapID;

		int iPoint = (int)GetStageByXZ( iX, iZ );

		int ishrX = iX >> 8;
		int ishrZ = iZ >> 8;

		INFO ( "MapServer::SetFlagPoint: iPoint = %d, iX = %d, iZ = %d, iStageID = %d", iPoint, ishrX, ishrZ, iStageID );

		if ( iPoint != 0 )
		{
			SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_Monsters, 19);

			int iSpawnCount = 0;
			if (pcDB->Open())
			{
				if (pcDB->Prepare("SELECT COUNT(*) FROM MapSpawnPoint WHERE Stage=? AND X=? AND Z=?"))
				{
					pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iStageID);
					pcDB->BindParameterInput(2, PARAMTYPE_Integer, &ishrX);
					pcDB->BindParameterInput(3, PARAMTYPE_Integer, &ishrZ);

					if (pcDB->Execute() && pcDB->Fetch())
					{
						pcDB->GetData(1, PARAMTYPE_Integer, &iSpawnCount);
					}
				}

				pcDB->Close();

				if (iSpawnCount >= 1)
				{
					CHATSERVER->SendChat(USERDATATOUSER(pcUserData), CHATCOLOR_Error, "> Flag Point not added. Duplicate.");
					return;
				}
			}

			// Get Last ID
			int iID = 0;
			if (pcDB->Open())
			{
				//get the last id
				if (pcDB->Prepare("SELECT TOP 1 ID FROM MapSpawnPoint ORDER BY ID DESC"))
				{
					if (pcDB->Execute() && pcDB->Fetch())
						pcDB->GetData(1, PARAMTYPE_Integer, &iID);
				}
				pcDB->Close();
			}


			iID++;
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO MapSpawnPoint([ID],[Stage],[X],[Z],[Description]) VALUES(?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iStageID );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &ishrX );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &ishrZ );
					pcDB->BindParameterInput( 5, PARAMTYPE_String, (void*)pszDescription );
					pcDB->Execute();
				}
				pcDB->Close();
			}

			SetNewSpawn( iPoint, ishrX, ishrZ, 1 );
			SendFlagPoint( iPoint );

			AddMapSpawnData( iStageID, pszDescription, iID, ishrX, ishrZ );

			CHATSERVER->SendChatAllGM( "GM> Flag '%s' added [Map: %d, X: %d Z: %d]", pszDescription, iStageID, ishrX, ishrZ );
		}
		else
		{
			CHATSERVER->SendChat(USERDATATOUSER(pcUserData), CHATCOLOR_Error, "> Map not found!");
		}
	}
}

void MapServer::DeleteFlagPoint( UserData * pcUserData, PacketDeleteFlagPoint * psPacket )
{
	INFO ( "MapServer::DeleteFlagPoint" );

	if ( pcUserData && pcUserData->iGameLevel == GAMELEVEL_Four)
	{
		Map * pcMap = GetStageByXZ( psPacket->iX, psPacket->iZ );

		if ( pcMap )
		{
			if ( pcMap->pcBaseMap )
			{
				std::cout << "pcMap " << pcMap->pcBaseMap->iMapID << std::endl;

				fnDeleteFlagPoint( (int)pcMap, psPacket->iX >> 8, psPacket->iZ >> 8 );
				SendFlagPoint( (int)pcMap );

				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 3 );

				int iX = psPacket->iX >> 8;
				int iZ = psPacket->iZ >> 8;

				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "DELETE FROM MapSpawnPoint WHERE (X=?) AND (Z=?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iX );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iZ );
						pcDB->Execute();
					}
					pcDB->Close();
				}

				DelMapSpawnData( pcMap->pcBaseMap->GetID(), iX, iZ );
			}
		}
		else
		{
			WARN ( "MapServer::DeleteFlagPoint: Base map not found" );
		}
	}
	else
	{
		WARN ( "MapServer::DeleteFlagPoint: Map not found" );
	}
}

void MapServer::SendHealedAmountToNearbyPlayers( User * pcHealer, User * pcTarget, int iHealValue )
{
	PacketDebugDamageInfo sPacket;
	sPacket.iLength		= sizeof( PacketDebugDamageInfo );
	sPacket.iHeader		= PKTHDR_DamageDebugInfo;
	sPacket.dwObjectID = pcTarget->GetID();
	sPacket.dwAttackerID = pcHealer->GetID();
	sPacket.sTypeAction = EDamageTextType::Healing;
	sPacket.iValue = iHealValue;

	MAPSERVER->SendUserReceivedDamageInfoTextToNearbyPlayers( pcTarget->pcUserData, &sPacket );
}

void MapServer::SendUserReceivedDamageInfoTextToNearbyPlayers( UserData * pcUserData, PacketDebugDamageInfo * psPacket )
{
	int iBuffPos;

	if ( pcUserData )
	{
		psPacket->dwObjectID = pcUserData->iID;
		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
				continue;

			//skip this user
			if ( pcUser->pcUserData == pcUserData )
				continue;

			//dist check
			if ( pcUserData->sPosition.WithinXZYDistance( &pcUser->pcUserData->sPosition, DISTANCE_XY_15_meters, 100 ) == FALSE )
				continue;

			//In BC, send it instant
			if ( pcUserData->iMapID == EMapID::MAPID_BlessCastle )
			{
				SENDPACKET( pcUser, psPacket );
				continue;
			}

			if ( pcUser->sDamageInfoContainer.sAmount < DAMAGEDEBUGCONTAINER_MAXCOUNT )
			{
				iBuffPos = pcUser->sDamageInfoContainer.sAmount * sizeof( PacketDebugDamageInfo );
				CopyMemory( pcUser->sDamageInfoContainer.baBuffer + iBuffPos, psPacket, sizeof( PacketDebugDamageInfo ) );
				pcUser->sDamageInfoContainer.sAmount++;
			}
		}
	}
}

void MapServer::SendUnitDamageInfoTextToNearbyPlayers( UserData * pcUserData, UnitData * pcUnitData, int iDamage, BOOL bCrit, short sParameter, EDamageTextType sTypeActionOverride )
{
	DEBUG ( "SendUnitDamageInfoTextToNearbyPlayers: DMG = %d, CRIT = %d", iDamage, bCrit );

	if ( pcUnitData && pcUnitData->sCharacterData.iType != 0 && (iDamage > 0 || sTypeActionOverride != EDamageTextType::Undefined) )
	{
		PacketDebugDamageInfo sPacket;
		sPacket.iLength		= sizeof( PacketDebugDamageInfo );
		sPacket.iHeader		= PKTHDR_DamageDebugInfo;
		sPacket.dwObjectID	= pcUnitData->iID;
		sPacket.dwAttackerID = pcUserData == NULL ? 0 : pcUserData->iID;

		if ( sTypeActionOverride != EDamageTextType::Undefined )
		{
			sPacket.sTypeAction = sTypeActionOverride;
		}
		else if ( bCrit )
		{
			sPacket.sTypeAction = EDamageTextType::Critical;
		}
		else
		{
			sPacket.sTypeAction = EDamageTextType::Normal;
		}

		sPacket.iValue		= iDamage;
		sPacket.sParameter  = sParameter;

		int iBuffPos;

		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			User * pcUser = USERDATATOUSER ( &USERSDATA[i] );

			if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
				continue;

			if ( pcUser->GetGameLevel() <= GAMELEVEL_Two || pcUser->bDebugChat == FALSE )
			{
				//disable for bellatra
				if ( pcUser->GetMapID() == EMapID::MAPID_Bellatra )
					continue;

				//disable flinch text (debug only)
				if ( sPacket.sTypeAction == EDamageTextType::Flinch )
					continue;
			}

			//dist check
			if ( pcUnitData->sPosition.WithinXZYDistance( &USERSDATA[i].sPosition, DISTANCE_XY_15_meters, 100 ) == FALSE )
				continue;

			if ( pcUser->sDamageInfoContainer.sAmount < DAMAGEDEBUGCONTAINER_MAXCOUNT )
			{
				iBuffPos = pcUser->sDamageInfoContainer.sAmount * sizeof( PacketDebugDamageInfo );
				CopyMemory( pcUser->sDamageInfoContainer.baBuffer + iBuffPos, &sPacket, sizeof( PacketDebugDamageInfo ) );
				pcUser->sDamageInfoContainer.sAmount++;
			}
		}
	}

	//Iron: these are not used anywhere?
	*(int *)( ESkillsVarsASM::ATTACK_DAMAGE ) = 0;
	*(BOOL *)( ESkillsVarsASM::CRITICAL_HIT ) = FALSE;
}

BOOL MapServer::AddMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	//Get Index of empyt Unit in Stage
    int iIndex = GetEmptyIndexUnitData( pcMap );

	if ( iIndex >= 0 )
	{
		//Set Unit to stage
		pcMap->pcaUnitData[iIndex] = pcUnitData;

		//Set Pointer of stage to Unit
		pcUnitData->pcMapSpawn = (void*)pcMap;

		//Not a	Special Unit?
        if ( (*pbIsCrystalMonster) == FALSE &&
			 (*pbIsSummonedGMMonster) == FALSE &&
			 (*pbIsSummonedMonster) == FALSE )
        {
			//Count It in Spawn.
            pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]++;
        }
		else
		{
			//Otherwise Set No Counter Flag
			pcUnitData->bNoCountInSpawn = TRUE;
		}

		//Is Special Unit?
		if ( pcUnitData->bNoCountInSpawn )
		{
			//Clear all flags
			if ( (*pbIsSummonedMonster) )
				(*pbIsSummonedMonster)		= FALSE;

			if ( (*pbIsSummonedGMMonster) )
				(*pbIsSummonedGMMonster)	= FALSE;

			if ( (*pbIsCrystalMonster) )
				(*pbIsCrystalMonster)		= FALSE;
		}


		//Set last Unit Pointer at Stage
		for ( int i = iIndex; i < MAX_ALIVEMONSTERS; i++ )
		{
			if ( pcMap->pcaUnitData[i] != NULL )
				pcMap->iNumUsingUnitData = i + 1;
		}

		//Success
        return TRUE;
    }

	//Fail
    return FALSE;
}

BOOL MapServer::DelMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	//Not a Special Unit? then no need to decrase Counter on Spawn Flag
	if ( !pcUnitData->bNoCountInSpawn && pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex] > 0 )
		pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]--;
	else
		bRet = TRUE;

	//Off Flag
	pcUnitData->bNoCountInSpawn = FALSE;

	//Clear Unit Pointer on Stage (killed or closed Unit)
	for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
	{
		if ( pcMap->pcaUnitData[i] == pcUnitData )
		{
			pcMap->pcaUnitData[i] = NULL;

			//Success
			bRet					= TRUE;
			break;
		}
	}
	return bRet;
}

BOOL MapServer::DisableMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	if (pcMap && pcUnitData)
	{
		//Not a Special Unit? then no need to decrase Counter on Spawn Flag
		if ((pcUnitData->iSpawnPointIndex >= 0) && (pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex] > 0))
		{
			pcUnitData->bNoCountInSpawn = TRUE;
			pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]--;

			//Debug: if (pcUnitData->pcOwner)
			//	CHATSERVER->SendChatEx(pcUnitData->pcOwner->pcSocketData->u, CHATCOLOR_Error, "> Monster `%s` disabled from spawn!", pcUnitData->sCharacterData.szName);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL MapServer::DelItem( Map * pcMap, Loot * psItem )
{
	if( psItem )
	{
		psItem->bInUse = FALSE;

		PacketGetItem sPacket;
		sPacket.iHeader = PKTHDR_DeleteItemFromMap;
		sPacket.iLength = sizeof(PacketGetItem);
		sPacket.sPosition = Point3D( psItem->sPosition.iX << 8, psItem->sPosition.iY << 8, psItem->sPosition.iZ << 8 );
		sPacket.pcMap = pcMap;

		for( size_t i = 0; i < PLAYERS_MAX; i++ )
		{
			UserData * pcUserData = USERSDATA + i;
			if( pcUserData->pcSocketData && pcUserData->iID && pcUserData->iMapID == pcMap->pcBaseMap->iMapID )
			{
				int iX = psItem->sPosition.iX - ( pcUserData->sPosition.iX >> 8 );
				int iZ = psItem->sPosition.iZ - ( pcUserData->sPosition.iZ >> 8 );
				int iDistance = iX * iX + iZ * iZ;

				if ( iDistance < DISTANCE_XY_33_meters )
				{
					SENDPACKET ( USERDATATOUSER ( pcUserData ), &sPacket );
				}
			}
		}
	}

	return TRUE;
}

//See int srReadStage() in leaked code
void MapServer::CustomReadStageData()
{
	CALL(0x43C620); //InitStageLoader
	MAPSERVER->Load();

	INFO("Loading map data..");

	Map* pMap = nullptr;

	//Up to secret lab
	for (int i = 0; i < NUM_MAPS; i++)
	{
		pMap = MAPSDATA + i;

		INFO ( "Loading stage: Map id: %d, model: %s", i, pcaBaseMap[i].szStageFilePath );

		LoadStage(pMap, pcaBaseMap[i].szStageFilePath); //"Data\\Maps\\fore3.dat";
		pMap->pcBaseMap = &pcaBaseMap[i];


		if ( mMapSkipNextEventSpawn.find( i ) == mMapSkipNextEventSpawn.end() )
			mMapSkipNextEventSpawn.insert( { i, FALSE } );
	}

	pMap = MAPSDATA + 0;

	//ASM
	//dword_7AABF6C = *((_DWORD *)MAPSDATA + 1);
	//dword_7AABF70 = *((_DWORD*)MAPSDATA + 1);
	//Leaked
	//lpsmStage[0] = StageArea[0].lpStage;
    //lpsmStage[1] = StageArea[0].lpStage;
	//Called code
	WRITEDWORD(0x07AABF6C, pMap);
	WRITEDWORD(0x07AABF70, pMap);
}

signed int CustomReadStageData()
{
	MAPSERVER->CustomReadStageData();
	return 1;
}

void MapServer::Load()
{
	INFO("Loading map data from database..");

	ZeroMemory( pcaBaseMap, sizeof( BaseMap ) * NUM_MAPS );

	for( int i = 0; i < NUM_MAPS; i++ )
		pcaBaseMap[i].SetID( (EMapID)i );

	//Open SQL Connection
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB, 7 );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "SELECT * FROM MapList ORDER BY ID ASC" ) )
		{
			int iMapIndex = 0;

			if( pcDB->Execute())
			{
				while (pcDB->Fetch())
				{
					//std::cout << "iMapIndex = " << iMapIndex << std::endl;

					//Get Parameters
					char szShortName[256] = { 0 };
					int iLevelMap = 0;
					char szFilePath[256] = { 0 };

					pcDB->GetData( 3, PARAMTYPE_String, szShortName, 256 );
					pcDB->GetData( 5, PARAMTYPE_Integer, &iLevelMap );
					pcDB->GetData( 7, PARAMTYPE_String, szFilePath, 256 );

					BaseMap * pc = ( pcaBaseMap + iMapIndex);
					pc->SetLevelRequirement( MAPLEVEL(iMapIndex) );
					pc->SetFileName( szFilePath, szShortName );

					//Navisko Town?
					if(iMapIndex == MAPID_NaviskoTown )
					{
						pc->SetType( MAPTYPE_Desert );
						pc->SetLevelRequirement( MAPLEVEL( MAPID_NaviskoTown ) );
						pc->SetNightSpawn( TRUE );
						pc->SetCenter( 21928, -2335 );
						pc->AddSpawn( 21928, -2335 );
					}

					//Bless Castle?
					if(iMapIndex == MAPID_BlessCastle )
					{
						pc->SetType( MAPTYPE_BlessCastle );
						pc->SetLevelRequirement( MAPLEVEL( MAPID_BlessCastle ) );
						pc->SetCenter( 35225, -23847 );
						pc->AddSpawn( 33920, -23479 );
					}

					*(UINT*)(0x08B80008 + (iMapIndex * 4)) = MAPLEVEL(iMapIndex);

					iMapIndex++;

				}
			}
		}
		pcDB->Close();
	}
}

BOOL MapServer::IsPVPMap( int iMapID )
{
	if ( iMapID == MAPID_BlessCastle )
		return TRUE;

	if ( (*(int*)0x0084831C) != (-1) && (*(int*)0x0084831C) == iMapID )
		return TRUE;

	return FALSE;
}

BOOL MapServer::IsEXPMap( int iMapID )
{
	BOOL bResult = FALSE;
	switch ( iMapID )
	{
		case MAPID_MysteryForest1:
		case MAPID_MysteryForest2:
		case MAPID_MysteryForest3:
		case MAPID_MysteryDesert1:
		case MAPID_MysteryDesert2:
		case MAPID_MysteryDesert3:
		case MAPID_ForgottenTempleF1:
		case MAPID_ForgottenTempleF2:
		case MAPID_AncientDungeonF1:
		case MAPID_AncientDungeonF2:
		case MAPID_AncientDungeonF3:
			bResult = TRUE;
			break;
		default:
			break;
	}
	return bResult;
}

BOOL MapServer::IsBellatraMapArea( int iX, int iY, int iZ )
{
	BOOL bRes = FALSE;
	__asm
	{
		PUSH iZ;
		PUSH iY;
		PUSH iX;
		MOV EAX, 0x0054DF10;
		CALL EAX;
		ADD ESP, 0x0C;
		MOV bRes, EAX;
	}
	return bRes;
}

BOOL MapServer::IsSummonMap( int iMapID )
{
	if ( iMapID == MAPID_T5QuestArena || iMapID == MAPID_Bellatra )
		return FALSE;

	return TRUE;
}

BOOL MapServer::IsValidMap( Map * pcMap )
{
	for( int i = 0; i < NUM_MAPS; i++ )
		if( pcMap == MAPSDATA + i )
			return TRUE;

	return FALSE;
}

int MapServer::GetEmptyIndexUnitData( Map * pcMap )
{
	for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
	{
		//Empty Pointer? Found.
		if ( pcMap->pcaUnitData[i] == NULL )
			return i;
	}

	//Not Found
	return (-1);
}

/// <summary>
/// Send all items within 20 meters to the user
/// This is done every 8 seconds.
/// </summary>
/// <param name="pcMap"></param>
/// <param name="pcUserData"></param>
void MapServer::SendStageItem( Map * pcMap, User * pcUser )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );

	std::vector<Loot *> nearbyItems;

	int dX = 0, dZ = 0, iDistance = 0;

	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		Loot * p = pcMap->saLoot + i;

		if ( p == NULL )
			continue;

		if ( p->bInUse == FALSE )
			continue;

		if ( p->dwDropTime > TICKCOUNT )
		{
			//DEBUG( "Skipping. Drop time is not reached yet for item %s [%d][%d]. Drop time = %d, Time remaining = %d", p->sLootItem.sItem.szItemName, p->sLootItem.sItem.iChk1, p->sLootItem.sItem.iChk2, p->dwDropTime, ( p->dwDropTime - TICKCOUNT ) );
			continue;
		}

		if ( TICKCOUNT > ( p->dwEndTime - ITEM_TRANS_LIMIT_TIME )  )
		{
			//DEBUG( "Skipping. Drop end time reached for item %s [%d][%d]", p->sLootItem.sItem.szItemName, p->sLootItem.sItem.iChk1, p->sLootItem.sItem.iChk2 );
			continue;
		}

		if ( p->eLootType == ELootType::Exclusive &&
			( p->lExclusiveOwnerID != pcUser->pcUserData->iID ) )
		{
			DEBUG( "Skipping due to item not exclusive to user (%d): %s", pcUser->pcUserData->iID, p->sLootItem.sItem.szItemName );
			continue;
		}

		if ( ITEMSERVER->CanViewDropItem( pcUser, p ) == FALSE )
		{
			DEBUG( "Skipping. Cannot view drop item" );
			continue;
		}

		dX = p->sPosition.iX - ( pcUser->pcUserData->sPosition.iX >> 8 );
		dZ = p->sPosition.iZ - ( pcUser->pcUserData->sPosition.iZ >> 8 );

		iDistance = ( dX * dX + dZ * dZ );

		if ( iDistance < DISTANCE_XY_20_meters && abs( dX ) < 4096 && abs( dZ ) < 4096 )
		{
			nearbyItems.push_back( p );
		}
	}

	//sort drops by latest (by timestamp)
	std::sort( nearbyItems.begin (),
		nearbyItems.end (),
		[](const Loot * lhs, const Loot * rhs)
		{
			return lhs->dwDropTime > rhs->dwDropTime;
		});

	DWORD dwCode;

	for ( Loot * p : nearbyItems )
	{
		dwCode = p->sLootItem.sItem.sItemID.ToItemID ();

		sPacket.saItems[sPacket.iCount].bActive = p->bInUse;
		sPacket.saItems[sPacket.iCount].eItemFlags = p->sLootItem.sItem.eItemFlags;
		sPacket.saItems[sPacket.iCount].eRarity = p->sLootItem.sItem.eRarity;

		sPacket.saItems[sPacket.iCount].iX = p->sPosition.iX << 8;
		sPacket.saItems[sPacket.iCount].iY = p->sPosition.iY << 8;
		sPacket.saItems[sPacket.iCount].iZ = p->sPosition.iZ << 8;
		sPacket.saItems[sPacket.iCount].iItemID = dwCode;
		sPacket.saItems[sPacket.iCount].iClass = ClassFlagToCharacterClass ( (EClassFlag)p->sLootItem.sItem.eSpecialization );

		dwCode &= 0xFFFF0000;

		if ( p->sLootItem.sItem.iPotionCount > 1 && ( dwCode & 0xFF000000 ) == ITEMBASE_Potion )
			STRINGFORMAT ( sPacket.saItems[sPacket.iCount].szName, "%sx%d", p->sLootItem.sItem.szItemName, p->sLootItem.sItem.iPotionCount );
		else
			STRINGCOPY ( sPacket.saItems[sPacket.iCount].szName, p->sLootItem.sItem.szItemName );

		sPacket.iCount++;

		//max 50 items (see packet struct)
		if ( sPacket.iCount == 50 )
		{
			break;
		}
	}

	if ( sPacket.iCount > 0 )
	{
		//std::cout << TIME << ": MapServer::SendStageItem" << std::endl;
		//std::cout << " sPacket.iCount:" << sPacket.iCount << std::endl;

		sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
		sPacket.iHeader = PKTHDR_PlayItem;
		sPacket.bActive = TRUE;
		SENDPACKET( pcUser, &sPacket, TRUE );
	}

	nearbyItems.clear ();
}

void MapServer::SendStageSpawn( Map * pcMap, UserData * pcUserData )
{
	if ( !pcUserData )
		return;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );

	pcMap->iNumActiveSpawnFlag = 0;
	pcMap->iNumSpawnFlag = 0;

	int dX = 0, dZ = 0, iDistance = 0;
	for ( int i = 0; i < 200; i++ )
	{
		if ( sPacket.iCount == 50 && pcUserData->iGameLevel >= GAMELEVEL_Three )
		{
			sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
			sPacket.iHeader = PKTHDR_PlayItem;
			sPacket.bActive = TRUE;
			SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
			ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
		}

		Map::SpawnFlag * p = pcMap->saSpawnFlag + i;
		if ( p->bActive )
		{
			dX = p->iX - (pcUserData->sPosition.iX >> 8);
			dZ = p->iZ - (pcUserData->sPosition.iZ >> 8);

			iDistance = (dX*dX+dZ*dZ);

			if ( iDistance < DISTANCE_XY_33_meters && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				if ( pcUserData->iGameLevel >= GAMELEVEL_Three && (pcMap->uCounter & 0x1FF) == 0x80 )
				{
					sPacket.saItems[sPacket.iCount].bActive = p->bActive;
					sPacket.saItems[sPacket.iCount].iX = p->iX << 8;
					sPacket.saItems[sPacket.iCount].iY = 0;
					sPacket.saItems[sPacket.iCount].iZ = p->iZ << 8;
					sPacket.saItems[sPacket.iCount].iItemID = 0;
					sPacket.saItems[sPacket.iCount].iClass = 0;

					if ( pcMap->pcBaseMap )
						UNITINFODATA->OnDescriptionSpawn( pcMap->pcBaseMap->GetID(), p->iX, p->iZ, sPacket.saItems[sPacket.iCount].szName );

					sPacket.iCount++;
				}
			}

			if ( iDistance < 0x1C2000 && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				pcMap->baSpawnFlagActive[i]++;
			}

			if ( pcMap->baSpawnFlagActive[i] )
				pcMap->iNumActiveSpawnFlag++;
			pcMap->iNumSpawnFlag++;
		}
	}
	if ( sPacket.iCount > 0 && pcUserData->iGameLevel >= GAMELEVEL_Three )
	{
		sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
		sPacket.iHeader = PKTHDR_PlayItem;
		sPacket.bActive = TRUE;
		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
	}
}


#define DIST_AROUND_NEAR	(256*256)
#define DIST_AROUND_MIDDLE	(16*64 * 16*64)

/// <summary>
/// 2012 src: int STG_AREA::SetStartPosNearChar(smCHAR *lpChar, int cx, int cy, int cz)
/// Install character positions around specific coordinates
/// </summary>
BOOL MapServer::SetStartPosNearChar( Map * pcMap, UnitData * lpChar, int cx, int cy, int cz )
{
	BOOL fromFixedPos = FALSE;
	if ( ( *(Point3D **)0x07AC9E24 ) )
	{
		fromFixedPos = TRUE;
		cx = ( *(Point3D **)0x07AC9E24 )->iX;
		cz = ( *(Point3D **)0x07AC9E24 )->iZ;
		cy = 0;
	}

	if (pcMap->sIsBossMinions)
		lpChar->PartyFlag = 5; //boss minion

	if ( cy == 0 )
	{
		cy = pcMap->pcStage->GetHighestPoint( cx, cz );

		if ( cy == CLIP_OUT )
		{
			WARN( "Invalid boss spawn position: X: %d, Z: %d for Map ID: %d", cx, cz, pcMap->pcBaseMap->iMapID );
			return FALSE;
		}
	}

	int height;
	int dx, dy, dz;
	int x, y, z;
	int deltaHeight;

	BOOL bFound = FALSE;

	int i_X = 0; //mid
	int i_Z = 0; //top
	int i_Count = 0;
	int i_Multiplier = 1;
	BOOL bRandomise = FALSE;

	for ( int i = 0; i < 20; i++ )
	{
		if ( pcMap->sBossSubsSpawnCount >= 8 )
		{
			i_Multiplier = 2;
			if ( pcMap->sBossSubsSpawnCount >= 14 )
			{
				bRandomise = TRUE;
			}
		}

		if ( bRandomise )
		{
			i_X = 0;
			i_Z = 0;
			dx = cx + Dice::RandomI( -( MONSPAWN_SET_DIST * 256 ), ( MONSPAWN_SET_DIST * 256 ) );
			dz = cz + Dice::RandomI( -( MONSPAWN_SET_DIST * 256 ), ( MONSPAWN_SET_DIST * 256 ) );
			dy = cy;
		}
		else
		{
			i_X = ptItemSettingMatrix[pcMap->sBossSubsSpawnCount & 7].x;
			i_Z = ptItemSettingMatrix[pcMap->sBossSubsSpawnCount & 7].y;
			pcMap->sBossSubsSpawnCount++;

			dx = cx + ( ( i_X * ( MONSPAWN_SET_DIST / i_Multiplier ) ) * 256 );
			dz = cz + ( ( i_Z * ( MONSPAWN_SET_DIST / i_Multiplier ) ) * 256 );
			dy = cy;
		}

		//Find the floor height at the current location
		height = pcMap->pcStage->GetHighestPoint( dx, dz );
		if ( height != CLIP_OUT )
		{
			deltaHeight = height - dy;

			if ( abs(deltaHeight) < 5120 )
			{
				x = dx;
				z = dz;
				y = height;
				bFound = TRUE;

				//if ( fromFixedPos )
				//{
				//	DEBUG( "Map id = %d, Spawm x = %d, y = %d, z = %d,  i_X = %d, i_Z = %d, iMulitply = %d, bRandom = %d, sub count = %d, attempt = %d", pcMap->pcBaseMap->iMapID, x >> 8, y >> 8, z >> 8, i_X, i_Z, i_Multiplier, bRandomise, pcMap->iBossSubsSpawnCount, i);
				//}

				break;
			}
		}
	}


	if ( bFound == FALSE )
	{
		WARN( "SetStartPosNearChar - No suitable nearby position found for spawn after 20 attempts. X = %d, Y = %d, Z = %d, MapId = %d", cx, cy, cz, pcMap->pcBaseMap->iMapID );
		x = cx;
		y = cy;
		z = cz;
	}
	__asm
	{
		MOV ECX, lpChar;
		MOV EAX, 0x004163E0;  //void smCHAR::Init()
		CALL EAX;
	}

	lpChar->pcStage = pcMap->pcStage;

	__asm
	{
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH z;
		PUSH 0;
		PUSH x;
		MOV ECX, lpChar;
		MOV EAX, 0x00416970; //int smCHAR::SetPosi( int x, int y, int z , int angX, int angY, int angZ )
		CALL EAX;
	}

	lpChar->sSpawnPosition.iX = x;
	lpChar->sSpawnPosition.iY = y;
	lpChar->sSpawnPosition.iZ = z;
	lpChar->iLureDistance = DIST_AROUND_MIDDLE;
	lpChar->iSpawnPointIndex = pcMap->iLastActiveSpawn;

	return TRUE;
}

/// <summary>
/// 2012 Src: int STG_AREA::SetStartPosChar(smCHAR *lpChar)
/// //Find and set new spawn start location
/// </summary>
BOOL MapServer::SetSpawnMonster( Map * pcMap, UnitData * pcUnitData )
{
	if (!pcMap)
		return FALSE;

	int iFlag = 0, iX = 0, iZ = 0, iY = 0;

	for (int i = pcMap->iNumActiveSpawns; i < MAX_SPAWNFLAGS; i++)
	{
		Map::SpawnFlag* p = pcMap->saSpawnFlag + i;

		//if (p->bActive &&
		//	pcMap->baSpawnFlagActive[i] &&
		//	pcMap->iaSpawnMonsterActive[i] < pcMap->sSpawnSetting.iSpawnMonsters &&	pcMap->dwaTimeLastMonsterSpawned[i] < TICKCOUNT)
		//{
		//	iX = p->iX << 8;
		//	iZ = p->iZ << 8;
		//	pcMap->iNumActiveSpawns = i + 1;
		//	pcMap->iLastActiveSpawn = i;
		//	iFlag++;
		//	break;
		//}
	}

	if (iFlag == 0)
	{
		for (int i = 0; i < MAX_SPAWNFLAGS; i++)
		{
			Map::SpawnFlag* p = pcMap->saSpawnFlag + i;

			if (p->bActive &&
				pcMap->baSpawnFlagActive[i] &&
				pcMap->iaSpawnMonsterActive[i] < pcMap->sSpawnSetting.iSpawnMonsters &&
				pcMap->dwaTimeLastMonsterSpawned[i] < TICKCOUNT)
			{
				iX = p->iX << 8;
				iZ = p->iZ << 8;
				pcMap->iNumActiveSpawns = i + 1;
				pcMap->iLastActiveSpawn = i;
				iFlag++;
				break;
			}
		}
	}

	//Boss / SoD
	if ( iFlag == 0 && (pcMap->bForceCreateMonster || pcUnitData->sCharacterData.sMonsterClass == EMonsterClass::Boss) )
	{
		pcMap->bForceCreateMonster = FALSE;

		for ( int i = pcMap->iNumActiveSpawns; i < MAX_SPAWNFLAGS; i++ )
		{
			Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

			if ( p->bActive && pcMap->baSpawnFlagActive[i] )
			{
				iX = p->iX << 8;
				iZ = p->iZ << 8;
				iFlag++;
				break;
			}
		}

		if ( iFlag == 0 && pcMap->iNumActiveSpawns < MAX_SPAWNFLAGS )
		{
			for ( int i = 0; i < pcMap->iNumActiveSpawns; i++ )
			{
				Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

				if ( p->bActive && pcMap->baSpawnFlagActive[i] )
				{
					iX = p->iX << 8;
					iZ = p->iZ << 8;
					iFlag++;
					break;
				}
			}
		}

		if ( iFlag == 0 )
		{
			for ( int i = 0; i < MAX_SPAWNFLAGS; i++ )
			{
				Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

				if ( p->bActive )
				{
					iX = p->iX << 8;
					iZ = p->iZ << 8;
					iFlag++;
					break;
				}
			}
		}
	}

	if ( iFlag > 0 )
	{
		if ( ( *(DWORD *)0x07AC9E24 ) )
		{
			DWORD p = ( *(DWORD *)0x07AC9E24 );
			iX = ( *(DWORD *)( p ) );
			iZ = ( *(DWORD *)( p + 8 ) );
		}

		__asm
		{
			MOV ECX, pcUnitData;
			MOV EAX, 0x004163E0; //void smCHAR::Init()
			CALL EAX;
		}
		pcUnitData->pcStage = pcMap->pcStage;

		int iXRand, iZRand, heightCheck;
		BOOL bFound = FALSE;
		int iTargetHeight = pcMap->pcStage->GetHighestPoint ( iX, iZ  );
		int iDeltaHeight;

		//max 20 retries
		for ( int i = 0; i < 20; i++ )
		{
			iXRand = Dice::RandomI( -(96 << 8), (96 << 8) );
			iZRand = Dice::RandomI( -(96 << 8), (96 << 8) );

			heightCheck = pcMap->pcStage->GetHighestPoint ( iX + iXRand, iZ + iZRand  );

			if ( heightCheck != CLIP_OUT )
			{
				iDeltaHeight = heightCheck - iTargetHeight;

				//above - allov up to 5 diff (1280)
				//below - allow up to 20 diff (-5120)
				if ( iDeltaHeight >= -5120 && iDeltaHeight < 1280 )
				{
					iX += iXRand;
					iZ += iZRand;
					iY = heightCheck;
					bFound = TRUE;
					break;
				}
				else
				{
					//DEBUG( "Height delta (%d) is too large. Trying again..", heightDelta );
				}
			}
		}

		if ( bFound == FALSE )
		{
			WARN( "SetSpawnMonster - No suitable position found for spawn after 20 attempts. X = %d, Z = %d, MapId = %d", iX, iZ, pcMap->pcBaseMap->iMapID );
			return FALSE;
		}


		__asm
		{
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH iZ;
			PUSH 0;
			PUSH iX;
			MOV ECX, pcUnitData;
			MOV EAX, 0x00416970; //int smCHAR::SetPosi( int x, int y, int z , int angX, int angY, int angZ )
			CALL EAX;
		}

		pcUnitData->sSpawnPosition.iX = iX;
		pcUnitData->sSpawnPosition.iY = bFound ? iY : pcUnitData->sPosition.iY;
		pcUnitData->sSpawnPosition.iZ = iZ;
		pcUnitData->iLureDistance = ( 256 * 256 );

		pcUnitData->iSpawnPointIndex = pcMap->iLastActiveSpawn;

		pcMap->dwaTimeLastMonsterSpawned[pcMap->iLastActiveSpawn] = TICKCOUNT + pcMap->sSpawnSetting.iSpawnIntervalMax;
		return TRUE;
	}
	return FALSE;
}


int MapServer::CollectAllNearbyGold ( Map * pcMap, Point3D * psGoldPosition )
{
	//std::cout << "CollectAllNearbyGold" << std::endl;

	Loot * ps = NULL;
	int iTotalGoldNearby = 0;

	int dX = 0, dZ = 0, iDistance = 0;

	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		Loot * p = pcMap->saLoot + i;

		if ( p == NULL )
			continue;

		if ( p->bInUse &&
			p->dwEndTime > TICKCOUNT &&
			p->sLootItem.sItem.sItemID.ToItemID () == ITEMID_Gold &&
			p->sPosition != *psGoldPosition)
		{
			//note - the positions here are short version (not the full ones)
			dX = p->sPosition.iX - psGoldPosition->iX;
			dZ = p->sPosition.iZ - psGoldPosition->iZ;
			iDistance = ( dX * dX ) + ( dZ * dZ );

			if ( iDistance < DISTANCE_XY_3_meters && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				iTotalGoldNearby += p->sLootItem.sItem.iGold;

				//std::cout << "iTotalGoldNearby: " << p->sLootItem.sItem.iGold << std::endl;

				MAPSERVER->DelItem( pcMap, p );
			}
		}
	}

	return iTotalGoldNearby;
}

/// <summary>
/// This gets called whenever a player throws away an item
/// from player inventory. However it does not retain the information about who
/// threw away the item :/
///
/// Also gets called on unit killed and the drop is 'public'
/// </summary>
BOOL MapServer::SendItemStageNearUsers( Map * pcMap, Loot * psItemStage )
{
	//std::cout << TICKCOUNT << " : " << "MapServer::SendItemStageNearUsers" << std::endl;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
	sPacket.iLength = sizeof( PacketStageItemData::StageItemData ) + 16;
	sPacket.iHeader = PKTHDR_PlayItem;
	sPacket.bActive = TRUE;

	DWORD dwCode = psItemStage->sLootItem.sItem.sItemID.ToItemID();

	sPacket.saItems[sPacket.iCount].bActive = psItemStage->bInUse;
	sPacket.saItems[sPacket.iCount].eItemFlags = psItemStage->sLootItem.sItem.eItemFlags;
	sPacket.saItems[sPacket.iCount].eRarity = psItemStage->sLootItem.sItem.eRarity;
	sPacket.saItems[sPacket.iCount].iX = psItemStage->sPosition.iX << 8;
	sPacket.saItems[sPacket.iCount].iY = psItemStage->sPosition.iY << 8;
	sPacket.saItems[sPacket.iCount].iZ = psItemStage->sPosition.iZ << 8;
	sPacket.saItems[sPacket.iCount].iItemID = dwCode;
	sPacket.saItems[sPacket.iCount].iClass = ClassFlagToCharacterClass( (EClassFlag)psItemStage->sLootItem.sItem.eSpecialization );

	dwCode &= 0xFFFF0000;

	if ( psItemStage->sLootItem.sItem.iPotionCount > 1 && (dwCode & 0xFF000000) == 0x04000000 )
		STRINGFORMAT( sPacket.saItems[sPacket.iCount].szName, "%sx%d", psItemStage->sLootItem.sItem.szItemName, psItemStage->sLootItem.sItem.iPotionCount );
	else
		STRINGCOPY( sPacket.saItems[sPacket.iCount].szName, psItemStage->sLootItem.sItem.szItemName );

	sPacket.iCount++;

	Point3D sPosition = psItemStage->sPosition;
	sPosition.iX <<= 8;
	sPosition.iY <<= 8;
	sPosition.iZ <<= 8;


	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];

		if (pcUser && pcUser->pcUserData && pcUser->IsConnected())
		{
			if (sPosition.WithinXZDistance( &pcUser->pcUserData->sPosition, DISTANCE_XY_19_meters, DISTANCE_MISC_Y_EX ))
			{
				//bypass loot filter check if this user had threw away the item
				//this in turn will allow the item to remain visible on client side for 16 seconds
				//then it will disappear.
				if (pcUser->LastDroppedItem.iItemID == psItemStage->sLootItem.sItem.sItemID.ToInt() &&
					pcUser->LastDroppedItem.iChk1 == psItemStage->sLootItem.sItem.iChk1 &&
					pcUser->LastDroppedItem.iChk2 == psItemStage->sLootItem.sItem.iChk2)
				{
					ZeroMemory( &pcUser->LastDroppedItem, sizeof( DropItemData ) );
					SENDPACKET( pcUser, &sPacket );
					continue;
				}

				if (ITEMSERVER->CanViewDropItem( pcUser, psItemStage ) == FALSE)
				{
					DEBUG( "Loot %s is filtered out by user (%s)", psItemStage->sLootItem.sItem.szItemName, pcUser->pcUserData->szCharacterName );
					continue;
				}

				SENDPACKET( pcUser, &sPacket );
			}
		}
	}

	return TRUE;
}

void MapServer::SendItemStageUser( Map * pcMap, User * pcUser, Loot * psItemStage )
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	if (ITEMSERVER->CanViewDropItem( pcUser, psItemStage ) == FALSE)
		return;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
	sPacket.iLength = sizeof( PacketStageItemData::StageItemData ) + 16;
	sPacket.iHeader = PKTHDR_PlayItem;
	sPacket.bActive = TRUE;

	DWORD dwCode = psItemStage->sLootItem.sItem.sItemID.ToItemID();

	sPacket.saItems[sPacket.iCount].bActive = psItemStage->bInUse;
	sPacket.saItems[sPacket.iCount].eItemFlags = psItemStage->sLootItem.sItem.eItemFlags;
	sPacket.saItems[sPacket.iCount].eRarity = psItemStage->sLootItem.sItem.eRarity;
	sPacket.saItems[sPacket.iCount].iX = psItemStage->sPosition.iX << 8;
	sPacket.saItems[sPacket.iCount].iY = psItemStage->sPosition.iY << 8;
	sPacket.saItems[sPacket.iCount].iZ = psItemStage->sPosition.iZ << 8;
	sPacket.saItems[sPacket.iCount].iItemID = dwCode;
	sPacket.saItems[sPacket.iCount].iClass = ClassFlagToCharacterClass( (EClassFlag)psItemStage->sLootItem.sItem.eSpecialization );

	dwCode &= 0xFFFF0000;

	if ( psItemStage->sLootItem.sItem.iPotionCount > 1 && (dwCode & 0xFF000000) == 0x04000000 )
		STRINGFORMAT( sPacket.saItems[sPacket.iCount].szName, "%sx%d", psItemStage->sLootItem.sItem.szItemName, psItemStage->sLootItem.sItem.iPotionCount );
	else
		STRINGCOPY( sPacket.saItems[sPacket.iCount].szName, psItemStage->sLootItem.sItem.szItemName );

	sPacket.iCount++;


	SENDPACKET( pcUser, &sPacket );
}

BOOL MapServer::OnThrowItem( User * pcUser, struct PacketThrowItem * psPacket )
{
	DEBUG( "MapServer::OnThrowItem" );

	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return TRUE; //need to be true to bypass assembly

	UserData * pcUserData = pcUser->pcUserData;

	if ( psPacket->sItem.sItemID.ToInt() == 0 )
	{

		//this occurs on force orb usage
#ifdef DEV_MODE
		WARN ( "OnThrowItem - Item ID is zero for item: %s [%d][%d]", psPacket->sItem.szItemName, psPacket->sItem.iChk1, psPacket->sItem.iChk2 );
#endif
		return TRUE;
	}

	if ( (psPacket->sItem.sItemID.ToItemID() & 0xFFFF0000) == 0x08020000 )
	{
		// XMas Event Crystal?
		if ( psPacket->sItem.sItemID.ToItemID() == ITEMID_SnowManCrystal )
		{
			UNITSERVER->CreateUnitEnemy( "Snowman", psPacket->sPosition.iX, pcUserData->sPosition.iY, psPacket->sPosition.iZ, pcUserData );

			// User Warp Effect summon crystal
			PacketUserWarpEffect sPacketEffect;
			ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
			sPacketEffect.iID = 0x100;
			sPacketEffect.iX = psPacket->sPosition.iX;
			sPacketEffect.iY = psPacket->sPosition.iY;
			sPacketEffect.iZ = psPacket->sPosition.iZ;
			CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );
			return TRUE;
		}
	}

	//Check here if force orb was used
	if ( ( psPacket->sItem.sItemID.ToItemID () & 0xFFFF0000 ) == ITEMTYPE_ForceOrb )
	{
		//std::cout << "psPacket->sItem.iPotionCount  = " << psPacket->sItem.iPotionCount << std::endl;
		//std::cout << "psPacket->sItem.iChk1  = " << psPacket->sItem.iChk1 << std::endl;
		//std::cout << "psPacket->sItem.iChk2  = " << psPacket->sItem.iChk2 << std::endl;

		int WParam = psPacket->sItem.sItemID.ToInt();
		int LParam = psPacket->sItem.iChk1;
		int SParam = psPacket->sItem.iChk2;
		int EParam = psPacket->sItem.iPotionCount ^ SParam ^ PKTHDR_SaveThrowItem2;

		//TransCommand.WParam = lpItemInfo->CODE;
		//TransCommand.LParam = lpItemInfo->ItemHeader.Head;
		//TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
		//TransCommand.EParam = lpItemInfo->PotionCount ^ TransCommand.SParam ^ TransCommand.code;
		//
		//TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
		//TransCommand.LxParam = EncodeParam ( TransCommand.WxParam );
		//TransCommand.SxParam = GetTotalMoney () ^ TransCommand.LxParam;
		//TransCommand.ExParam = lpItemInfo->Money;


		int cnt = ( EParam ^ SParam ^ PKTHDR_SaveThrowItem2 );

		//std::cout << "Force orb used: " << cnt << std::endl;

		//if (cnt) is 100 then force was used
		if ( cnt == 100 )
		{

			//Force use
			cnt = ( WParam & 0x0000FFFF ) >> 8; // 1 For Lucidy Force, 13 for Bellum, 14 for Oredo Force

			//Not used for standard force
			if ( ( WParam & 0x0000FFFF ) >= 0x00002A00 )
			{
				cnt -= 16;
			}

			cnt--;


			ITEMSERVER->OnForceOrbActivated( pcUser, cnt, (EItemID)WParam );

			//std::cout << "cnt = " << cnt << std::endl;

			/*else if ( cnt >= 20 && cnt < 32 ) {
				lpPlayInfo->dwForceOrb_SaveCode = lpTransCommandEx->WParam;
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime + MagicForceOrbUseTime[cnt - 20] * 1000;
				lpPlayInfo->dwForceOrb_SaveDamage = MagicForceOrbDamage[cnt - 20];
			}
			else if ( cnt >= 34 && cnt < 37 ) {
				lpPlayInfo->dwForceOrb_SaveCode = lpTransCommandEx->WParam;
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime + BillingMagicForceOrbUseTime[cnt - 34] * 1000;
				lpPlayInfo->dwForceOrb_SaveDamage = BillingMagicForceOrbDamage[cnt - 34];
			}
			len = 100;*/

			return TRUE;
		}
	}

	bool l_IsDropableItem = ITEMSERVER->IsDropableItem(psPacket->sItem.sItemID.ToItemID());
	if (l_IsDropableItem == FALSE)
		return TRUE;

	if (l_IsDropableItem &&
		psPacket->sItem.sItemID.ToItemID() != ITEMID_Gold &&
		psPacket->sItem.sItemID.ToItemType() != ITEMTYPE_ManaPotion &&
		psPacket->sItem.sItemID.ToItemType() != ITEMTYPE_HealthPotion &&
		psPacket->sItem.sItemID.ToItemType() != ITEMTYPE_StaminaPotion &&
		(psPacket->sItem.iPotionCount == 0 || psPacket->sItem.iPotionCount == 1 ))
	{
		if (!ITEMSERVER->CheckItemForm(&psPacket->sItem))
		{
			WARN ( "OnThrowItem - CheckValidItem failed for item: %s (%d)", psPacket->sItem.szItemName, psPacket->sItem.sItemID.ToItemID() );
			return TRUE;
		}
		//std::cout << "Looking for item " << psPacket->sItem.sItemID.ToItemID() << " | " << psPacket->sItem.iChk1 << " | " << psPacket->sItem.iChk2  << std::endl;

		//also check last inventory snapshot because PKTHR_ThrowItem gets called AFTER PKTHR_NetInventory
		bool l_ItemFound = ITEMSERVER->HasItemInInventory ( pcUserData, (EItemID)psPacket->sItem.sItemID.ToItemID (), psPacket->sItem.iChk1, psPacket->sItem.iChk2, TRUE );

		if (GAME_SERVER && l_ItemFound)
		{
			int l_Colision = std::count_if( pcUserData->pcSocketData->u->LastDroppedItemsThrowMapHack.begin(),
											pcUserData->pcSocketData->u->LastDroppedItemsThrowMapHack.end(),
											[psPacket](const DropItemData & p_Data) -> bool
			{
				return p_Data.iItemID == psPacket->sItem.sItemID.ToItemID() && (p_Data.iChk1 == psPacket->sItem.iChk1) && (p_Data.iChk2 == psPacket->sItem.iChk2);
			});

			if ( l_Colision )
			{
				//WARN ( "THROW ITEM - Duplicate found %s [%d] [%d] [%d]", psPacket->sItem.szItemName, psPacket->sItem.sItemID.ToItemID (), psPacket->sItem.iChk1, psPacket->sItem.iChk2 );
				l_ItemFound = false;
			}
		}

		if (!l_ItemFound)
		{
			//TODO Clean up?
			//WARN ( "THROW ITEM %s [%d] [%d] [%d] NOT FOUND", psPacket->sItem.szItemName, psPacket->sItem.sItemID.ToItemID(), psPacket->sItem.iChk1, psPacket->sItem.iChk2);

			//LOGSERVER->OnLogItemEx(pcUserData->pcSocketData->u, ITEMLOGID_SellShop, "[%s](%s) Throw not owned item %d",
			//	pcUserData->szAccountName, pcUserData->szCharacterName, psPacket->sItem.sItemID.ToItemID());

			/*SOCKETSERVER->AddBanIP(pcUserData->pcSocketData->lIP, "THROW NOT OWNED ITEM");

			SENDPACKETBLANK(USERDATATOUSER(pcUserData), PKTHDR_Disconnect);
			WNDPROC_SOCKETCLOSE(SOCKETSERVER->GetHWND(), pcUserData->pcSocketData);

			NETSERVER->DisconnectUser(pcUserData);

			return TRUE;*/
		}
		//else
		{
//             if (pcUserData->iMapID == 3 || pcUserData->iMapID == 21)
//             {
//                 /// Restore the item
//                 /// Call rsPutItem2
//                 reinterpret_cast<int(__cdecl *)(UserData * a1, const Item * a2)>(0x552B70)(pcUserData, &psPacket->sItem);
//                 return TRUE;
//             }

			/// Destroy the item until next sync
			if ( GAME_SERVER )
			{
				if ( pcUser->GetGameLevel() >= GAMELEVEL_One &&
					 pcUser->bTeasureHuntingMode == TRUE)
				{
					DEBUG( "Teasure Hunting mode activated for: %s", psPacket->sItem.szItemName );
					psPacket->sItem.eItemFlags |= EItemFlag::TreasureHuntingMode;
				}

				DropItemData data;
				data.iItemID = psPacket->sItem.sItemID.ToItemID ();
				data.iChk1 = psPacket->sItem.iChk1;
				data.iChk2 = psPacket->sItem.iChk2;

				pcUser->LastDroppedItemsThrowMapHack.push_back ( data );

				DEBUG( "Last dropped item: %s [%d] [%d] [%d]", psPacket->sItem.szItemName, data.iItemID, data.iChk1, data.iChk2 );

				CopyMemory( &pcUser->LastDroppedItem, &data, sizeof( DropItemData ) );
			}
		}

		//return !l_ItemFound;
		return FALSE; // forward event to server exe
	}

	if (psPacket->sItem.sItemID.ToItemID() != ITEMID_Gold && psPacket->sItem.iGold)
		return TRUE;

    /// Disable gold dropping
	if (psPacket->sItem.iGold && psPacket->sItem.iGold < 0 || psPacket->sItem.iGold > 0/*MAX_DROP_GOLD*/)
	{
		PacketSetCharacterGold sPacket;
		sPacket.iHeader = PKTHDR_SetGold;
		sPacket.iLength = sizeof(PacketSetCharacterGold);
		sPacket.dwGold	= pcUserData->GetGold();

		PACKETSERVER->Send(pcUserData, &sPacket);

		return TRUE;
	}

	if (psPacket->sItem.iGold > pcUserData->GetGold())
	{
		PacketLogCheat sPacket;
		sPacket.iCheatID	= CHEATLOGID_GenerateGoldHack;
		sPacket.LParam		= pcUserData->GetGold();
		sPacket.SParam		= psPacket->sItem.iGold;
		sPacket.WxParam		= 6;
		LOGSERVER->OnLogCheat(pcUserData, &sPacket);

		pcUserData->iSaveGold = pcUserData->GetGold();

		if (pcUserData->GetGold() < 0)
			pcUserData->iInventoryGold = 0;

		return TRUE;
	}
	else
	{
		/// Substract gold until next sync
		if (GAME_SERVER)
            USERSERVER->SubServerUserGold(pcUserData, psPacket->sItem.iGold, WHEREID_DropItem);
	}

	if (psPacket->sItem.iGold)
	{
		auto & l_Data = pcUserData->pcSocketData->u->m_GoldThrowHistory;

		if ((l_Data.LastTime + MAX_THROW_GOLD_TIME_PERIOD) < TICKCOUNT)
			l_Data.Count = 0;

		l_Data.Count++;
		l_Data.LastTime = TICKCOUNT;

		if (l_Data.Count > MAX_THROW_GOLD_TIME_PER_PERIOD)
		{
			LOGERROR( "THROW GOLD LIMIT REACHED - BANNED" );

			if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
			{
				SOCKETSERVER->AddBanIP( pcUserData->pcSocketData->lIP, "THROW GOLD LIMIT REACHED" );

				SENDPACKETBLANK( USERDATATOUSER( pcUserData ), PKTHDR_Disconnect );
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );

				NETSERVER->DisconnectUser( pcUserData );
			}

			return TRUE;
		}
	}


	return FALSE; //returning false here will mean the thrown away item will get processed in assembly
}

BOOL MapServer::UseUnionCore( UserData * pcUserData, struct PacketUseUnionCore * psPacket )
{
	// Online?
	if ( pcUserData && pcUserData->pcSocketData )
	{
		// Get User
		User * pcUser = USERDATATOUSER( pcUserData );
		if ( pcUser )
		{
			// Item Fake? dont teleport
			if ( !ITEMSERVER->CheckItemForm( &psPacket->sItem ) )
				return FALSE;

			psPacket->sItem.szItemName[31] = 0;

			// Get Character name to teleport to him
			int iLen		= lstrlenA( psPacket->sItem.szItemName );
			char * pszName	= &psPacket->sItem.szItemName[iLen + 1];

			// Don't have name? don't teleport
			if ( pszName[0] == 0 )
				return FALSE;

			// Get Player
			UserData * pcUserDataTarget = USERDATABYNAME( pszName );

			if ( !pcUserDataTarget || !pcUserDataTarget->iID || pcUserData->dwExclusiveNum || pcUserDataTarget->dwExclusiveNum ||
				IsBellatraMapArea( pcUserDataTarget->sPosition.iX, pcUserDataTarget->sPosition.iY, pcUserDataTarget->sPosition.iZ ) ||
				FURYARENAHANDLER->InsideArena( &pcUserDataTarget->sPosition ) || // Inside Fury Arena?
				pcUserDataTarget->iMapID == MAPID_QuestArena || pcUserDataTarget->iMapID == MAPID_T5QuestArena || pcUserDataTarget->iMapID == MAPID_BlessCastle )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You cannot teleport to player" );
				return FALSE;
			}
			// Mismatch clan?
		/*	if ( pcUserData->dwClanID != pcUserDataTarget->dwClanID )
			{
				// Don't teleport
				CHATSERVER->SendChatMessageBox( pcUserData, "You need to be on the same clan" );
				return FALSE;
			}
			*/
			Map * pcMap = GetStageByXZ( pcUserDataTarget->sPosition.iX, pcUserDataTarget->sPosition.iZ );
			if ( pcMap )
			{
				if ( pcMap->pcBaseMap )
				{
					// Check level player and level map
					int iLevelMap = (*(int*)(0x08B80008 + (pcMap->pcBaseMap->GetID() * 4)));
					if ( pcUserData->sCharacterData.iLevel < iLevelMap )
					{
						CHATSERVER->SendTitleBox( pcUserData, "Level does not match" );
						return FALSE;
					}

					// Success, Send coordinates to teleport to player
					PacketSendUseUnionCore sPacket;
					sPacket.iLength			= sizeof( PacketSendUseUnionCore );
					sPacket.iHeader			= PKTHDR_UseUnionCore;
					sPacket.iMapID			= pcMap->pcBaseMap->GetID();
					sPacket.iX				= pcUserDataTarget->sPosition.iX + (rand() % 4096);
					sPacket.iY				= pcUserDataTarget->sPosition.iY;
					sPacket.iZ				= pcUserDataTarget->sPosition.iZ + (rand() % 4096);
					sPacket.dwItemID		= psPacket->sItem.sItemID.ToItemID();
					sPacket.dwItemHead		= psPacket->sItem.iChk1;
					sPacket.dwItemChecksum	= psPacket->sItem.iChk2;
					sPacket.dwUnk			= 0;
					SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL MapServer::SetBossPosition( Map * pcMap, CharacterData * psCharacterData, Point3D * psPosition )
{
	BOOL bUpdate = FALSE;

	if ( pcMap && pcMap->pcBaseMap && psCharacterData && psPosition )
	{
		int iMapID			= pcMap->pcBaseMap->GetID();

		DWORD dwEffectID	= psCharacterData->iMonsterEffectID;

		switch ( iMapID )
		{
			case MAPID_GallubiaValley:
				if ( dwEffectID == MONSTEREFFECTID_CHAOSCARA )
				{
					psPosition->iX = 8733873;
					psPosition->iY = 0;
					psPosition->iZ = 9408604;
					bUpdate = TRUE;
				}
				break;

			case MAPID_FrozenSanctuary:
				if ( dwEffectID == MONSTEREFFECTID_DEATHKNIGHT )
				{
					psPosition->iX = 9722024;
					psPosition->iY = 0;
					psPosition->iZ = 12570546;
					bUpdate = TRUE;
				}
				break;

			case MAPID_KelvezuLair:
				if ( dwEffectID == MONSTEREFFECTID_KELVEZU )
				{
					psPosition->iX = 0x0080FB3D;
					psPosition->iY = 0;
					psPosition->iZ = 0x00CD88D7;
					bUpdate = TRUE;
				}
				break;

			case MAPID_LostTemple:
				if ( dwEffectID == MONSTEREFFECTID_MOKOVA )
				{
					psPosition->iX = -2997436;
					psPosition->iY = 0;
					psPosition->iZ = 3346561;
					bUpdate = TRUE;
				}
				break;

			/*case MAPID_EndlessTowerF1:
				if ( dwEffectID == MONSTEREFFECTID_GOBLINSHAMAN )
				{
					psPosition->iX = 3646068;
					psPosition->iY = 0;
					psPosition->iZ = -10323850;
					bUpdate = TRUE;
				}
				break;

			case MAPID_EndlessTowerF2:
				if ( dwEffectID == MONSTEREFFECTID_GORGON )
				{
					psPosition->iX = 1348259;
					psPosition->iY = 0;
					psPosition->iZ = -9876309;
					bUpdate = TRUE;
				}
				break;



				*/


			case MAPID_EndlessTowerF3:
				if ( dwEffectID == MONSTEREFFECTID_DevilShy )
				{
					psPosition->iX = 1446577;
					psPosition->iY = 0;
					psPosition->iZ = -12316221;
					bUpdate = TRUE;
				}
				break;

			case MAPID_IceMineF1:
				if ( dwEffectID == MONSTEREFFECTID_TULLA )
				{
					psPosition->iX = 6456813;
					psPosition->iY = 0;
					psPosition->iZ = -9239019;
					bUpdate = TRUE;
				}
				break;

			case MAPID_SecretLaboratory:
				if ( dwEffectID == MONSTEREFFECTID_Draxos )
				{
					psPosition->iX = -3072497;
					psPosition->iY = 0;
					psPosition->iZ = -14107881;
					bUpdate = TRUE;
				}
				break;

			case MAPID_AncientWeapon:
				if (dwEffectID == MONSTEREFFECTID_Greedy)
				{
					psPosition->iX = 3275477;
					psPosition->iY = 0;
					psPosition->iZ = -17017423;
					bUpdate = TRUE;
				}
				break;

			//case MAPID_CastleOfTheLost:
			//	if (dwEffectID == MONSTEREFFECTID_S_F_ELEMENTAL)
			//	{
			//		psPosition->iX = 710897;
			//		psPosition->iY = 0;
			//		psPosition->iZ = 2010591;
			//		bUpdate = TRUE;
			//	}
			//	break;

			case MAPID_SeaOfAbyss1:
				if (dwEffectID == MONSTEREFFECTID_Yagditha)
				{
					psPosition->iX = -5814228;
					psPosition->iY = 0;
					psPosition->iZ = 998017;
					bUpdate = TRUE;
					//psPosition->iX = -5821332;
					//psPosition->iY = 0;
					//psPosition->iZ = 1068535;
					//bUpdate = TRUE;
				}
				break;

			case MAPID_HearOfFire:
				if (dwEffectID == MONSTEREFFECTID_IGNIS)
				{
					psPosition->iX = 8795345;
					psPosition->iY = 0;
					psPosition->iZ = -16216073;
					bUpdate = TRUE;
				}
				break;

			case MAPID_BambooForest:
				if (dwEffectID == MONSTEREFFECTID_MRGHOST)
				{
					psPosition->iX = -1114263;
					psPosition->iY = 0;
					psPosition->iZ = -2748554;
					bUpdate = TRUE;
				}
				break;

			case MAPID_MushroomCave:
				if (dwEffectID == MONSTEREFFECTID_SCORPION)
				{
					psPosition->iX = 31670117;
					psPosition->iY = 0;
					psPosition->iZ = 6201469;
					bUpdate = TRUE;
				}
				break;

			case MAPID_AcasiaForest:
				if (dwEffectID == MONSTEREFFECTID_HOBGOBLIN)
				{
					psPosition->iX = -3031926;
					psPosition->iY = 0;
					psPosition->iZ = -2666519;
					bUpdate = TRUE;
				}
				break;

			case MAPID_ValleyOfTranquility:
				if (dwEffectID == MONSTEREFFECTID_EGAN)
				{
					psPosition->iX = -9053;
					psPosition->iY = 0;
					psPosition->iZ = 13837242;
					bUpdate = TRUE;
				}
				break;

			case MAPID_RoadToTheWind:
				if (dwEffectID == MONSTEREFFECTID_RABIE)
				{
					psPosition->iX = -363064;
					psPosition->iY = 0;
					psPosition->iZ = 16206218;
					bUpdate = TRUE;
				}
				break;

			case MAPID_LandOfDusk:
				if (dwEffectID == MONSTEREFFECTID_MUTANTRABIE)
				{
					psPosition->iX = -390672;
					psPosition->iY = 0;
					psPosition->iZ = 11059563;
					bUpdate = TRUE;
				}
				break;

			case MAPID_ForestOfSpirits:
				if (dwEffectID == MONSTEREFFECTID_HAUNTINGPLANT)
				{
					psPosition->iX = -548152;
					psPosition->iY = 0;
					psPosition->iZ = 8630622;
					bUpdate = TRUE;
				}
				break;

			case MAPID_RuinenVillage:
				if (dwEffectID == MONSTEREFFECTID_CYCLOPS)
				{
					psPosition->iX = 816391;
					psPosition->iY = 0;
					psPosition->iZ = 5777188;
					bUpdate = TRUE;
				}
				break;

			case MAPID_CastleOfTheLost:
				if (dwEffectID == MONSTEREFFECTID_SKELETON)
				{
					psPosition->iX = 1286267;
					psPosition->iY = 0;
					psPosition->iZ = 2860347;
					bUpdate = TRUE;
				}
				break;

			case MAPID_RefugeOfTheAncients:
				if (dwEffectID == MONSTEREFFECTID_IMP)
				{
					psPosition->iX = 367217;
					psPosition->iY = 0;
					psPosition->iZ = 270939;
					bUpdate = TRUE;
				}
				break;

			case MAPID_GardenOfFreedom:
				if (dwEffectID == MONSTEREFFECTID_HOPT)
				{
					psPosition->iX = 1224832;
					psPosition->iY = 0;
					psPosition->iZ = -2794471;
					bUpdate = TRUE;
				}
				break;

			case MAPID_AncientsBattlefield:
				if (dwEffectID == MONSTEREFFECTID_FIGON)
				{
					psPosition->iX = 8093243;
					psPosition->iY = 0;
					psPosition->iZ = -111757;
					bUpdate = TRUE;
				}
				break;

			case MAPID_DarkSanctuary:
				if (dwEffectID == MONSTEREFFECTID_DARKKNIGHT)
				{
					psPosition->iX = 41543521;
					psPosition->iY = 0;
					psPosition->iZ = 4990031;
					bUpdate = TRUE;
				}
				break;

			case MAPID_CursedTempleF1:
				if (dwEffectID == MONSTEREFFECTID_STYGIANLORD)
				{
					psPosition->iX = -3098969;
					psPosition->iY = 0;
					psPosition->iZ = -9842318;
					bUpdate = TRUE;
				}
				break;

			case MAPID_Oasis:
				if (dwEffectID == MONSTEREFFECTID_DAWLIN)
				{
					psPosition->iX = 3565703;
					psPosition->iY = 0;
					psPosition->iZ = -3435929;
					bUpdate = TRUE;
				}
				break;

			case MAPID_RailwayOfChaos:
				if (dwEffectID == MONSTEREFFECTID_TEMPLEGUARD)
				{
					psPosition->iX = 12860301;
					psPosition->iY = 0;
					psPosition->iZ = 3382601;
					bUpdate = TRUE;
				}
				break;

			case MAPID_HeartOfPerum:
				if (dwEffectID == MONSTEREFFECTID_BABEL)
				{
					psPosition->iX = 9484234;
					psPosition->iY = 0;
					psPosition->iZ = 6755276;
					bUpdate = TRUE;
				}
				break;

			case MAPID_CursedTempleF2:
				if (dwEffectID == MONSTEREFFECTID_RATOO)
				{
					psPosition->iX = -914115;
					psPosition->iY = 0;
					psPosition->iZ = -10498488;
					bUpdate = TRUE;
				}
				break;

			case MAPID_ForbiddenLand:
				if (dwEffectID == MONSTEREFFECTID_STONEGIANT)
				{
					psPosition->iX = 8518943;
					psPosition->iY = 0;
					psPosition->iZ = 2584567;
					bUpdate = TRUE;
				}
				break;

			case MAPID_AncientPrisonF1:
				if (dwEffectID == MONSTEREFFECTID_SKELETONMELEE)
				{
					psPosition->iX = -2935329;
					psPosition->iY = 0;
					psPosition->iZ = -7034511;
					bUpdate = TRUE;
				}
				break;

			case MAPID_AncientPrisonF2:
				if (dwEffectID == MONSTEREFFECTID_MUMMY)
				{
					psPosition->iX = -1168834;
					psPosition->iY = 0;
					psPosition->iZ = -6370413;
					bUpdate = TRUE;
				}
				break;

			case MAPID_CursedLand:
				if (dwEffectID == MONSTEREFFECTID_BARGON)
				{
					psPosition->iX = 4174414;
					psPosition->iY = 0;
					psPosition->iZ = 3209236;
					bUpdate = TRUE;
				}
				break;

			case MAPID_ForgottenLand:
				if (dwEffectID == MONSTEREFFECTID_XETAN)
				{
					psPosition->iX = 3722645;
					psPosition->iY = 0;
					psPosition->iZ = 238954;
					bUpdate = TRUE;
				}
				break;

			case MAPID_EndlessTowerF2:
				if (dwEffectID == MONSTEREFFECTID_DevilShy)
				{
					psPosition->iX = 1348255;
					psPosition->iY = 0;
					psPosition->iZ = -9876894;
					bUpdate = TRUE;
				}
				break;

			case MAPID_EndlessTowerF1:
				if (dwEffectID == MONSTEREFFECTID_GORGON)
				{
					psPosition->iX = 3646501;
					psPosition->iY = 0;
					psPosition->iZ = -10321278;
					bUpdate = TRUE;
				}
				break;

			case MAPID_GreedyLake:
				if (dwEffectID == MONSTEREFFECTID_SPIDER)
				{
					psPosition->iX = 4768778;
					psPosition->iY = 0;
					psPosition->iZ = 5999428;
					bUpdate = TRUE;
				}
				break;

			case MAPID_LandOfChaos:
				if (dwEffectID == MONSTEREFFECTID_BLOODYKNIGHT)
				{
					psPosition->iX = -3524681;
					psPosition->iY = 0;
					psPosition->iZ = -304295;
					bUpdate = TRUE;
				}
				break;

			default:
				break;
		}
	}

	return bUpdate;
}

/**
OnServer.cpp
int rsOpenBossMonster(STG_AREA *lpStgArea, sBOSS_MONSTER *lpBossMonster)
*/
BOOL MapServer::SpawnMonsterBoss( Map * pcMap, Map::SpawnSetting::Boss * psBoss )
{
	MonsterMapSpawnHandle sCharacter;

	UnitData * pcUnitData = NULL;

	if ( psBoss->cdBoss->iMonsterEffectID == MONSTEREFFECTID_BABEL )
	{
		if ( READDWORD( 0x07AC9D54 ) > TICKCOUNT )
			return FALSE;

		WRITEDWORD( 0x006E2C30, pcMap->pcBaseMap->iMapID );

		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pc = pcMap->pcaUnitData[i];

			if ( pc )
			{
				if ( pc->bActive && pc->GetAnimation() != ANIMATIONTYPE_Die )
				{
					pc->Animate( ANIMATIONTYPE_Flinch );
					pc->iStunTimeLeft = 16 * 8;
				}
			}
		}
	}

	static Point3D sPosition;

	if( psBoss->cdBoss->bExclusiveBoss = SetBossPosition( pcMap, psBoss->cdBoss, &sPosition ) )
		(*(Point3D**)0x07AC9E24 ) = &sPosition;

	if ( psBoss->cdBoss )
	{
		CopyMemory( &sCharacter.sCharacterData, psBoss->cdBoss, sizeof( CharacterData ) );
		pcMap->bForceCreateMonster = TRUE;
		pcUnitData = CreateUnit( pcMap, &sCharacter, 0 );

		//can be null if there's no spawn position
		if ( pcUnitData )
		{
			pcMap->iNumActiveSpawns = pcMap->iLastActiveSpawn;

			if ( ( *(Point3D **)0x07AC9E24 ) )
			{
				pcUnitData->sSpawnPosition = pcUnitData->sPosition;
			}
			else
			{
				if ( pcMap->pcBaseMap->iMapID != EMapID::MAPID_Bellatra )
				{
					//for minions
					( *(Point3D **)0x07AC9E24 ) = &pcUnitData->sSpawnPosition;
					pcMap->sIsBossMinions = 1;
					pcMap->sBossSubsSpawnCount = 0; //reset counter
				}
			}

			if ( psBoss->cdMinion )
			{
				CopyMemory( &sCharacter.sCharacterData, psBoss->cdMinion, sizeof( CharacterData ) );
				pcMap->bForceCreateMonster = TRUE;
				CreateUnit( pcMap, &sCharacter, psBoss->iMinions );
			}

			//For each boss we wanna record each user attack damage data
			//See also CustomRecordCharacterDamage
			if (pcUnitData->pcMap->pcBaseMap->iMapID != MAPID_Bellatra &&
				pcUnitData->pcMap->pcBaseMap->iMapID != MAPID_BlessCastle)
			{
				//DEBUG("BOSS: %s AttackDamageData initialized (mapserver)", pcUnitData->GetName());

				pcUnitData->psaDamageUsersData = new AttackDamageData[ATTACK_DAMAGE_MAX];
				if (pcUnitData->psaDamageUsersData)
				{
					ZeroMemory(pcUnitData->psaDamageUsersData, sizeof(AttackDamageData) * ATTACK_DAMAGE_MAX);
				}
			}
		}
	}

	pcMap->sIsBossMinions = 0;
	pcMap->sBossSubsSpawnCount = 0;

	if ( (*(Point3D**)0x07AC9E24) ) //lprsFixedPoint
	{
		(*(Point3D**)0x07AC9E24) = NULL;
	}

	return TRUE;
}

BOOL MapServer::UpdateUnitPosition( UnitData * pcUnitData, int iMapID, int iX, int iY, int iZ )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Npc, 2 );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE MapNPC SET X=?, Y=?, Z=? WHERE (IDNPC=(SELECT ID FROM NPCList WHERE Name=?)) AND (Stage=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iX );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iY );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iZ );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUnitData->sCharacterData.szName, 32 );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iMapID );

					pcDB->ExecuteUpdate();
				}
				pcDB->Close();
			}
		}

		pcUnitData->sPosition.iX = iX << 8;
		pcUnitData->sPosition.iY = iY << 8;
		pcUnitData->sPosition.iZ = iZ << 8;

		return TRUE;
	}

	return FALSE;
}

BOOL MapServer::UpdateUnitAngle( UnitData * pcUnitData, int iMapID, int iAngle )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Npc, 3 );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE MapNPC SET Angle=? WHERE (IDNPC=(SELECT ID FROM NPCList WHERE Name=?)) AND (Stage=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAngle );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUnitData->sCharacterData.szName, 32 );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iMapID );

					pcDB->ExecuteUpdate();
				}
				pcDB->Close();
			}
		}

		pcUnitData->sAngle.iY = iAngle;

		return TRUE;
	}

	return FALSE;
}

UnitData * MapServer::CreateUnit( Map * pcStage, MonsterMapSpawnHandle * pcMonster, int iGroup )
{
	typedef UnitData * (__thiscall *tfnCreateUnit)(Map * pcStage, MonsterMapSpawnHandle * pcMonster, int iGroup);
	tfnCreateUnit fnCreateUnit = (tfnCreateUnit)0x08B8100C;
	return fnCreateUnit( pcStage, pcMonster, iGroup );
}


void MapServer::UpdateMap( Map * pcMap )
{ //0x00566220
	DWORD dwTickCount = TICKCOUNT;

	if ( pcMap->pcStage != NULL )
	{
		pcMap->uCounter++;

		//Spawn a Monster
		if ( (pcMap->uCounter % pcMap->sSpawnSetting.iSpawnIntervalMin) == 0 )
		{
			// Map contains less 'alive' units compared to max spawn for map
			// so let's spawn another.
			if ( pcMap->iNumAliveUnitData < pcMap->sSpawnSetting.iSpawnMaxMonsters )
			{
				if ( (pcMap->pcBaseMap->bNightSpawn == FALSE) || (pcMap->bNight == TRUE) )
				{
					if ( pcMap->sSpawnSetting.iTotalMonsters > 0 )
					{
						MonsterMapSpawnHandle sMonster;
						sMonster.sCharacterData.szName[0] = 0;

						int iSpawnRate = 0;
						int iRandomSpawnRate = Dice::RandomI( 0, pcMap->sSpawnSetting.iTotalMonsterSpawnRate );

						//Pick a monster to spawn from a list of monsters for the map. The chances
						// are controlled in MapMonster table.
						for ( UINT i = 0; i < pcMap->sSpawnSetting.iTotalMonsters; i++ )
						{
							iSpawnRate += pcMap->sSpawnSetting.saMonster[i].iSpawnRate;

							if ( iSpawnRate > iRandomSpawnRate )
							{
								CopyMemory( &sMonster.sCharacterData, pcMap->sSpawnSetting.saMonster[i].cd, sizeof( CharacterData ) );
								break;
							}
						}

						//Found a monster to spawn.
						if ( sMonster.sCharacterData.szName[0] != 0 )
						{
							UnitInfo * psNewUnitInfo = sMonster.sCharacterData.psUnitInfo;

							int iSpawnCount = Dice::RandomI( psNewUnitInfo->iGroupLimitMin, psNewUnitInfo->iGroupLimitMax );

							BOOL bEventMob = FALSE;

							if ( iSpawnCount <= 0 )
								iSpawnCount = 1;

							//check if we need to skip event spawn after
							//bForceCreateNewSpawn was set to TRUE to prevent
							//another event monster to spawn following a event spawn
							if ( mMapSkipNextEventSpawn[pcMap->pcBaseMap->iMapID] == FALSE )
							{
								if ( EVENTSERVER->TryGetEventMonsterSpawn( &sMonster.sCharacterData, pcMap ) )
								{
									bEventMob = TRUE;
								}
								else if ( CHRISTMASHANDLER->OnSpawnMonsterEvent( &sMonster.sCharacterData, pcMap ) )
								{
									bEventMob = TRUE;
								}
								else if ( GENERALEVENTHANDLER->OnSpawnMonsterEvent( &sMonster.sCharacterData, pcMap ) )
								{
									bEventMob = TRUE;
								}
								else if ( HALLOWEENHANDLER->OnSpawnEventMonster( &sMonster.sCharacterData ) )
								{
									bEventMob = TRUE;
								}
							}
							else
							{
								mMapSkipNextEventSpawn[pcMap->pcBaseMap->iMapID] = FALSE;
							}

							if ( bEventMob == TRUE )
								iSpawnCount = 1;

							//Important note, this function will return null UnitData if there is NO
							//nearby players.
							UnitData * pcUnitData = pcMap->CreateMonster( &sMonster.sCharacterData, iSpawnCount );
							if ( pcUnitData != nullptr )
							{
								//With the Closure of SoA2 it's no longer used
								/*
								if (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Arhdyra || pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MINEGOLEM)
								{
									pcUnitData->iLureDistance = DISTANCE_XY_33_meters;
									//printf("Lure distance 5 meters");
								}
								*/

								//Reduced Luring Distance in Hunting Maps
								if (pcMap->pcBaseMap->iMapID == MAPID_HearOfFire) 
								{
									pcUnitData->iLureDistance = DISTANCE_XY_24_meters;
								}

								//Reduced Luring Distance Endgame Monsters
								if (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_Yagditha || pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_IGNIS)
								{
									pcUnitData->iLureDistance = DISTANCE_XY_27_meters;
								}

								if ( bEventMob )
								{
									if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MIMIC )
									{
										EVENTSERVER->OnEventMimicSpawned( pcUnitData, pcMap );
									}

									mMapSkipNextEventSpawn[pcMap->pcBaseMap->iMapID] = TRUE;
									MAPSERVER->DisableMonsterSpawn( pcMap, pcUnitData );
								}
							}
						}
					}
				}
			}
		}

		//Update Spawn Activity based on Users
		//~ every 8 seconds
		switch ( pcMap->uCounter % 512 )
		{
			case 128:
			{
				//Reset Active Spawn

				ZeroMemory( pcMap->baSpawnFlagActive, sizeof( pcMap->baSpawnFlagActive ) );

				//Update Spawns to Active (And Show Spawn Flags)
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User ** pcaUserInGame = USERSERVER->pcaUserInGame;
				for ( UINT i = 0; i < uUsersInGame; i++ )
				{
					User * pcUser = pcaUserInGame[i];
					if (pcUser && pcUser->pcUserData && pcUser->IsConnected())
					{
						SendStageSpawn(pcMap, pcUser->pcUserData);
					}
				}
			}
			break;
			case 192:
			{
				//Show Loot
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User ** pcaUserInGame = USERSERVER->pcaUserInGame;
				for ( UINT i = 0; i < uUsersInGame; i++ )
				{
					User * pcUser = pcaUserInGame[i];

					if ( pcUser && pcUser->pcUserData && pcUser->IsConnected() && ( ( dwTickCount - pcUser->pcUserData->dwTimeLastPacket ) < 5000 ) )
					{
						SendStageItem( pcMap, pcUser );
					}
				}
			}
			break;
		}

		//Remove Timed Out Loot
		for ( UINT i = 0; i < MAX_LOOT; i++ )
		{
			Loot * pLoot = pcMap->saLoot + i;

			if ( (pLoot->bInUse) && (pLoot->dwEndTime < TICKCOUNT) )
				pLoot->bInUse = FALSE;
		}

		//Bellatra Map Update
		if ( pcMap->pcBaseMap->iMapID == MAPID_Bellatra )
		{
			if ( pcMap->sSpawnSetting.iTotalMonsters > 0 )
			{
				CharacterData sNewCharacterData;
				CopyMemory( &sNewCharacterData, pcMap->sSpawnSetting.saMonster[0].cd, sizeof( CharacterData ) );

				MonsterMapSpawnHandle s;
				CopyMemory( &s.sCharacterData, &sNewCharacterData, sizeof( CharacterData ) );

				BELLATRASERVER->UpdateServer( pcMap, &s );
			}
		}

		//Bless Castle Map Update
		if ( pcMap->pcBaseMap->iMapID == MAPID_BlessCastle )
		{
			//Update BC
			BLESSCASTLEHANDLER->UpdateSiegeWar();
		}

		//Swifts in Map from Night to Day or vice versa
		if ( pcMap->bNight != IsNight() )
		{
			//DayNight swift in maps with Night Spawn on (Navisko Town)
			if ( pcMap->pcBaseMap->bNightSpawn )
			{
				if ( IsNight() )
				{
					//Curse NPCs into Monsters (except Bellatra NPCs) // sod
					CharacterData * psZombie = *(CharacterData **)0x07AC9D5C;

					UINT uUnitsInGame = UNITSERVER->uUnitsInGame;
					Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;
					for ( UINT i = 0; i < uUnitsInGame; i++ )
					{
						Unit * pcUnit = pcaUnitInGame[i];
						UnitData * pcUnitData = pcUnit->pcUnitData;

						//In Use AND Same Map AND NPC AND Not Bellatra NPC?
						if ( pcUnitData->bActive == TRUE &&
							pcUnitData->pcMap == pcMap &&
							pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC &&
							pcUnitData->sUnitInfo.EventNPC == 0 &&							   //Is not Bellatra NPCs?
							pcUnitData->sUnitInfo.NPCEventCode !=  NPCEVENTCODE_SoloRankList ) //and isn't Marcus Sol?
						{
							//Reset Unit Data
							pcUnitData->Free();

							//Set in Use again
							pcUnitData->bActive = TRUE;

							//Copy Zombie Data
							CopyMemory( &pcUnitData->sCharacterData, psZombie, sizeof( CharacterData ) );
							CopyMemory( &pcUnitData->sUnitInfo, psZombie->psUnitInfo, sizeof( UnitInfo ) );

							//Set Mode
							pcUnitData->eActionPattern = ACTIONMODE_ServerMonster;

							//Update Num Alive
							pcMap->iNumAliveUnitData++;

							//Load Model INI
							pcUnitData->LoadModel( pcUnitData->sCharacterData.Monster.szBodyModel );

							//Set Lure Distance
							pcUnitData->iLureDistance = UNIT_DEFAULT_LURE_DISTANCE_FAR;

							//Add Unit Data to Map
							pcMap->AddUnitData( pcUnitData );

							//Set Death Timer
							pcUnitData->iDeathCounter = 256;

							//Animate to Idle
							pcUnitData->Animate( ANIMATIONTYPE_Idle );

							//Make Unit appear as New Load
							pcUnitData->sCharacterData.cNewLoad++;

							//Load Unit
							pcUnit->Load();
						}
					}
				}
				else
				{
					//Kill Alive Monsters
					UINT uUsersInGame = UNITSERVER->uUnitsInGame;
					Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;
					for ( UINT i = 0; i < uUsersInGame; i++ )
					{
						Unit * pcUnit = pcaUnitInGame[i];
						UnitData * pcUnitData = pcUnit->pcUnitData;

						//InUse AND Same Map AND Monster?
						if ( pcUnitData->bActive == TRUE &&
							pcUnitData->pcMap == pcMap &&
							pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
						{
							DEBUG( " Kill alive monsters" );

							//Kill Unit
							pcUnitData->SetCurrentHealth( 0 );
							pcUnitData->Animate( EAnimationType::ANIMATIONTYPE_Die );
						}
					}

					//Spawn NPCs
					for ( UINT i = 0; i < MAX_NPCINMAP; i++ )
					{
						PacketUnitInfo * psNPC = pcMap->saNPC + i;

						if ( psNPC && psNPC->iHeader != 0 )
						{
							if ( psNPC->sCharacterData.iMonsterType == MONSTERTYPE_Summon &&
								psNPC->sCharacterData.psUnitInfo != nullptr &&
								( psNPC->sCharacterData.psUnitInfo->EventNPC > 0 ||
									psNPC->sCharacterData.psUnitInfo->NPCEventCode == NPCEVENTCODE_SoloRankList ) )
							{
								//std::cout << "skipping: " << psNPC->sCharacterData.szName << std::endl;
								continue;
							}

							SpawnNPC( pcMap, psNPC );
						}
					}
				}
			}

			//DayNight swift in Ricarten
			if ( pcMap->pcBaseMap->iMapID == MAPID_RicartenTown )
			{
				//DEBUGSTATUS( "Day / night mode changed for map id: %d. Is night time: %d", pcMap->pcBaseMap->iMapID, IsNight() );

				CloseEventMonster( 100 );
				//CALL_WITH_ARG1( 0x00552060, 100 ); //Removes Wolverines from World (rsCloseEventMonster) - buggy, also removes pet wolverine??

				if ( IsNight() )
				{
					//Switching to Night.. spawn Wolverines
					//CALL_WITH_ARG4( 0x0055BB00, 100, 0, 0, 0 ); //rsOpenEventMonster
				}
				else
				{
					//Switching to Day.. spawn Wolverine NPCs in Rict
					for ( UINT i = 0; i < MAX_NPCINMAP; i++ )
					{
						PacketUnitInfo * psNPC = pcMap->saNPC + i;

						if ( ( psNPC->iHeader != 0 ) &&
							 ( psNPC->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN ) )
						{
							SpawnNPC( pcMap, psNPC );
						}
					}
				}
			}

			//Swift DayNight Time in Map
			pcMap->bNight = IsNight();
		}
	}
}

//Event monster removed
int MapServer::CloseEventMonster( int EventCode )
{
	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITSDATA + i;

		if ( pc &&
			 pc->bActive &&
			 pc->sUnitInfo.eSpecialType == SPECIALUNITTYPE_QuestWolverine &&
			 pc->pcOwner == NULL)
		{
			//DEBUGSTATUS( "CloseEventMonster - Freeing event (%d) unit %s", EventCode, pc->GetName() );

			//Not dead?
			if ( pc->GetAnimation() != EAnimationType::ANIMATIONTYPE_Die )
			{
				if ( pc->Animate( EAnimationType::ANIMATIONTYPE_Special ) == FALSE )
					pc->Free();

				if ( pc->pcMap )
				{
					if ( MAPSERVER->DelMonsterSpawn( pc->pcMap, pc ) )
						pc->pcMap->iNumAliveUnitData--;
				}
			}
			else
			{
				pc->Free();
				if ( pc->pcMap )
				{
					if ( MAPSERVER->DelMonsterSpawn( pc->pcMap, pc ) )
						pc->pcMap->iNumAliveUnitData--;
				}
			}
		}
	}

	return TRUE;
}



void MapServer::Update()
{
	//Only for game server
	if ( LOGIN_SERVER )
		return;

	//Update each Map
	for ( int i = 0; i < NUM_MAPS; i++ )
	{
		Map * pcMap = MAPSDATA + i;

		UpdateMap( pcMap );
	}

	//Update Time
	UpdateTime();


	//Boss Time Update?
	if (READWORD(0x07AC9F54) != GetServerTime()->wDay)
	{
		BOSS_TIME = Dice::RandomI(15, 45);
		WRITEWORD(0x07AC9F54, GetServerTime()->wDay);
		UpdateUsersBossTime();

		GSERVER->SqlUpdateOrInsertMetadata("boss.time.second", BOSS_TIME);
	}
}

void MapServer::ForceNightMode(BOOL bForce)
{
	INFO( "Force Night Mode: %d", bForce );

	bForceNightMode = bForce;
	bForceDayMode = FALSE;
}


void MapServer::ForceDayMode(BOOL bForce)
{
	INFO( "Force Day Mode: %d", bForce );

	bForceDayMode = bForce;
	bForceNightMode = FALSE;
}

void MapServer::UpdateTime()
{
	UINT minOfDay = GetMinuteOfDay();

	minOfDay += iGameTimeShift;
	if (minOfDay > 24)
		minOfDay -= 24;

	//Update PT time and check if it's night!
	// iGameTimeShift is used to shift game time into the future
	// to help with testing of Bellatra. See /shiftgametime
	SetTime(minOfDay);

	if ( bForceNightMode )
	{
		IsNight( TRUE );
		return;
	}

	else if ( bForceDayMode )
	{
		IsNight( FALSE );
		return;
	}

	if ( (GetTime() < 4) || (GetTime() >= 23) )
	{
		//It's night!
		IsNight( TRUE );
	}
	else
	{
		//Not night!
		IsNight( FALSE );
	}
}

void MapServer::RemoveNPC( Map * pcMap, ID iNpcID )
{
	UINT uUnitsInGame = UNITSERVER->uUnitsInGame;
	Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;
	for ( UINT i = 0; i < uUnitsInGame; i++ )
	{
		Unit * pcUnit = pcaUnitInGame[i];
		UnitData * pcUnitData = pcUnit->pcUnitData;

		//In Use AND Same Map AND NPC AND Not Bellatra NPC?
		if ( pcUnitData->bActive == TRUE &&
			 pcUnitData->pcMap == pcMap &&
			 pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC &&
			 pcUnitData->iID == iNpcID )
		{
			//Reset Unit Data
			pcUnitData->Free();

			if ( MAPSERVER->DelMonsterSpawn( pcMap, pcUnitData ) )
			{
				pcMap->iNumAliveUnitData--;
			}
		}
	}
}

void MapServer::SpawnNPC( Map * pcMap, PacketUnitInfo * psPacket )
{
	CALLT_WITH_ARG1( 0x0054E100, (DWORD)pcMap, (DWORD)psPacket );
}

BOOL MapServer::SpawnCrystal( EItemID iItemID, int iX, int iY, int iZ, UserData * pcUserData )
{
	struct CrystalData
	{
		EItemID			iItemID;

		int				iChance; //0~99%

		int				iTime;

		char			* pszName;
	};

	static const CrystalData saNormalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 5, "Hopy_Crystal" },
		//{ ITEMID_HopyCrystal, 20, 5, "Phillai Guard_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 5, "Hobgoblin_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 5, "Decoy_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Bargon_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Head Cutter_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "Figon_Crystal" },
		{ ITEMID_KingHopyCrystal, 8, 5, "King Hopy_Crystal" },
		{ ITEMID_HulkCrystal, 2, 5, "Hulk_Crystal" },
		{ ITEMID_GuardianSaintCrystal, 0, 5, "Guardian Saint_Crystal" },
		{ ITEMID_WebCrystal, 0, 5, "Web_Crystal" },
		{ ITEMID_DarkSpecterCrystal, 0, 5, "Dark Specter_Crystal" },
		{ ITEMID_IronGuardCrystal, 0, 5, "Iron Guard_Crystal" },
		{ ITEMID_RicartenMilitiaGuardCrystal, 0, 5, "Ricarten Militia_Crystal" },
		{ ITEMID_RicartenGuardCrystal, 0, 5, "Ricarten Guard_Crystal" },
		{ ITEMID_RoyalBlessGuardCrystal, 0, 5, "Royal Bless Guard_Crystal" },
	};

	static const CrystalData saDuskCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 5, "Hopy_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 5, "Hobgoblin_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 5, "Decoy_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Bargon_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Head Cutter_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "Figon_Crystal" },
		{ ITEMID_KingHopyCrystal, 8, 5, "King Hopy_Crystal" },
		{ ITEMID_HulkCrystal, 2, 5, "Hulk_Crystal" },
		/*{ ITEMID_HopyCrystal, 20, 7, "Typhoon_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 7, "Grotesque_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 7, "Stygian_Crystal" },
		{ ITEMID_BargonCrystal, 14, 7, "Dark Specter_Crystal" },*/
	};

	static const CrystalData saDoomedCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 8, "Dark Phalanx_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 8, "King Spider_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 8, "Seto_Crystal" },
		{ ITEMID_BargonCrystal, 14, 8, "Hell Hound_Crystal" },
	};

	static const CrystalData saOccultCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 9, "Atalanta Spirit_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 9, "Archer Spirit_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 9, "Pikeman Spirit_Crystal" },
		{ ITEMID_BargonCrystal, 14, 9, "Mechanician Spirit_Crystal" },
	};

	static const CrystalData saArticCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "Undead Miner_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 10, "Tarantulika_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 10, "Mine Bat_Crystal" },
		{ ITEMID_BargonCrystal, 14, 10, "Ice Serpent_Crystal" },
	};

	static const CrystalData saAbysmalCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "Dey_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 10, "Shogoth_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 10, "Koon_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Gurkob_Crystal" },
		{ ITEMID_HeadCutterCrystal, 10, 5, "Sathla_Crystal" },
		//{ ITEMID_FigonCrystal, 10, 5, "Arhdyra_Crystal" },
	};

	static const CrystalData saFieryCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "Igniculus_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 10, "Lava Insec_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 10, "Lava Archer_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Lava Brawler_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Lava Giant_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "Lava Golem_Crystal" },
	};

	static const CrystalData saMechanicCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "Mechanic_Crystal" },
		{ ITEMID_HobgoblinCrystal, 15, 10, "MechanicKing_Crystal" },
	};

	static const CrystalData saMechanicKingCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "MechanicKing_Crystal" },
		{ ITEMID_HobgoblinCrystal, 13, 10, "MechanicBoss_Crystal" },
	};

	static const CrystalData saBossData[] =
	{
		{ ITEMID_HopyCrystal, 20, 5, "Zzangpy_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 5, "Wild Goblin_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 5, "Maximum Decoy_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Bauton_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Vega Demir_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "HellSing_Crystal" },
		{ ITEMID_KingHopyCrystal, 8, 5, "Horropion_Crystal" },
		{ ITEMID_HulkCrystal, 2, 5, "Typhoon_Crystal" },
		{ ITEMID_GuardianSaintCrystal, 0, 5, "Guardian King_Crystal" },
		{ ITEMID_WebCrystal, 0, 5, "Cursed Web_Crystal" },
		{ ITEMID_DarkSpecterCrystal, 0, 5, "Dark Phantom_Crystal" },
		{ ITEMID_IronGuardCrystal, 0, 5, "Imitir Guard_Crystal" },
		{ ITEMID_RicartenMilitiaGuardCrystal, 0, 5, "Ricarten Militia_Crystal" },
		{ ITEMID_RicartenGuardCrystal, 0, 5, "Ricarten Guard_Crystal" },
		{ ITEMID_RoyalBlessGuardCrystal, 0, 5, "Royal Bless Guard_Crystal" },
	};

	BOOL bBossCrystal = (*(BOOL*)0x07AC9D50);

	BOOL bBlessCastleCrystal = FALSE;

	const CrystalData * psCrystal = NULL;

	const CrystalData * psaCrystals = saNormalData;
	int iCount						= _countof( saNormalData );

	if ( bBossCrystal )
	{
		psaCrystals					= saBossData;
		iCount						= _countof( saBossData );
	}

	if ( iItemID == ITEMID_DuskyCrystal )
	{
		psaCrystals					= saDuskCrystalData;
		iCount						= _countof( saDuskCrystalData );
	}

	if ( iItemID == ITEMID_DoomedCrystal )
	{
		psaCrystals					= saDoomedCrystalData;
		iCount						= _countof( saDoomedCrystalData );
	}

	if ( iItemID == ITEMID_OccultCrystal )
	{
		psaCrystals					= saOccultCrystalData;
		iCount						= _countof( saOccultCrystalData );
	}

	if ( iItemID == ITEMID_ArcticCrystal )
	{
		psaCrystals					= saArticCrystalData;
		iCount						= _countof( saArticCrystalData );
	}

	if ( iItemID == ITEMID_AbysmalCrystal )
	{
		psaCrystals					= saAbysmalCrystalData;
		iCount						= _countof( saAbysmalCrystalData );
	}

	if ( iItemID == ITEMID_FieryCrystal )
	{
		psaCrystals					= saFieryCrystalData;
		iCount						= _countof( saFieryCrystalData );
	}

	if (iItemID == ITEMID_MechanicCrystal)
	{
		psaCrystals					= saMechanicCrystalData;
		iCount						= _countof(saMechanicCrystalData);
	}

	if (iItemID == ITEMID_MechanicKingCrystal)
	{
		psaCrystals					= saMechanicKingCrystalData;
		iCount						= _countof(saMechanicKingCrystalData);
	}

	//Mystic Crystal
	if ( iItemID == ITEMID_MysticCrystal ||
		iItemID == ITEMID_DuskyCrystal ||
		iItemID == ITEMID_DoomedCrystal ||
		iItemID == ITEMID_OccultCrystal ||
		iItemID == ITEMID_ArcticCrystal ||
		iItemID == ITEMID_MechanicCrystal ||
		iItemID == ITEMID_MechanicKingCrystal ||
		iItemID == ITEMID_AbysmalCrystal ||
		iItemID == ITEMID_FieryCrystal)
	{
		int iRandom = Dice::RandomI( 0, 100 );

		int iRandomCount = 0;

		for ( int i = 0; i < iCount; i++ )
		{
			iRandomCount += psaCrystals[i].iChance;

			if ( iRandom < iRandomCount )
			{
				psCrystal = psaCrystals + i;
				break;
			}
		}

		if ( psCrystal == NULL )
			psCrystal = &psaCrystals[0];
	}

	//Normal Crystal
	if ( psCrystal == NULL )
	{
		for ( int i = 0; i < iCount; i++ )
		{
			if ( psaCrystals[i].iItemID == iItemID )
			{
				psCrystal = psaCrystals + i;
				break;
			}
		}
	}


	if ( psCrystal != NULL &&
		 iItemID == ITEMID_MechanicCrystal ||
		 iItemID == ITEMID_MechanicKingCrystal )
	{
		int iOwnerCount = 0;

		Map * pcMap = MAPSDATA + pcUserData->iMapID;

		if ( pcMap == NULL )
			return FALSE;

		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pcUnitData = pcMap->pcaUnitData[i];

			if ( pcUnitData && pcUnitData->sCharacterData.iOwnerID == pcUserData->iID )
			{
				if ( STRINGCOMPAREI( pcUnitData->GetName(), "Mechanic" ) ||
					 STRINGCOMPAREI( pcUnitData->GetName(), "MechanicKing" ) ||
					 STRINGCOMPAREI( pcUnitData->GetName(), "MechanicBoss" ) )
				{
					iOwnerCount++;
				}
			};
		}

		if ( iOwnerCount >= 2 )
		{
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "Spawn limit of 2 reached for Mechanic Crystal. Try again later!" );
			return FALSE;
		}
	}

	if ( psCrystal != NULL )
	{
		Map * pcMap = GetStageByXZ( iX, iZ );

		if ( pcMap )
		{
			if ( pcMap->pcBaseMap->iMapType == MAPTYPE_Town )
				return FALSE;

			//Bless Castle Crystal?
			if ( (psCrystal->iItemID == ITEMID_RicartenGuardCrystal) ||
				(psCrystal->iItemID == ITEMID_RicartenMilitiaGuardCrystal) ||
				(psCrystal->iItemID == ITEMID_RoyalBlessGuardCrystal) )
			{
				if ( (*(BOOL*)0x07AB1EE8) == FALSE || pcUserData->iClanID == 0 || pcUserData->iClanID != (*(int*)0x07AB1EEC) || pcUserData->iMapID != MAPID_BlessCastle )
				{
					CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "You can't summon a Bless Castle guard now!" );
				
					return FALSE;
				}
			
			
				int iaBCGuard[3] = { 0,0,0 };
				int iClanMonster = 0;
				int iSelfMonster = 0;
			
				for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
				{
					UnitData * pcUnitData = pcMap->pcaUnitData[i];
			
					if ( pcUnitData && pcUnitData->iClanID == pcUserData->iClanID )
					{
						iClanMonster++;
			
						if ( pcUnitData->sCharacterData.iOwnerID == pcUserData->iID )
							iSelfMonster++;
			
						switch ( pcUnitData->sCharacterData.iMonsterEffectID )
						{
							case MONSTEREFFECTID_CASTLE_SOLDER_A:
								iaBCGuard[0]++;
								break;
							case MONSTEREFFECTID_CASTLE_SOLDER_B:
								iaBCGuard[1]++;
								break;
							case MONSTEREFFECTID_CASTLE_SOLDER_C:
								iaBCGuard[2]++;
								break;
						}
					}
				}
			
				int iBlessCrystalID = 0;
			
				if ( iItemID == ITEMID_RicartenMilitiaGuardCrystal )
					iBlessCrystalID = 0;
				else if ( iItemID == ITEMID_RicartenGuardCrystal )
					iBlessCrystalID = 1;
				else if ( iItemID == ITEMID_RoyalBlessGuardCrystal )
					iBlessCrystalID = 2;
			
				if ( iClanMonster >= 20 || iSelfMonster >= 1 || iaBCGuard[iBlessCrystalID] >= 5 )
				{
					CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "You can't summon a Bless Castle guard now!" );
				
					return FALSE;
				}
			
				bBlessCastleCrystal = TRUE;
			}

			//Position
			Point2D * psaPoint = (Point2D*)0x006E2BF0;
			Point3D sNewPosition( 0, 0, 0 );

			int iPos	= 0;
			int iHeight = 0;
			for ( int i = 0; i < 8; i++ )
			{
				int j = Dice::RandomI( 0, 8 );

				Point2D * ps = psaPoint + (j & 7);

				sNewPosition.iX = iX + (ps->iX << 8);
				sNewPosition.iY = iY;
				sNewPosition.iZ = iZ + (ps->iY << 8);

				j++;

				iHeight = pcMap->pcStage->GetHighestPoint( sNewPosition.iX, sNewPosition.iY, sNewPosition.iZ, 0x2000 );

				if ( iHeight != 0xFFFF8001 )
				{
					if ( abs( iHeight - sNewPosition.iY ) < 0x2000 )
						break;
				}

				iPos++;
			}

			if ( iPos < 8 )
			{
				iX = sNewPosition.iX;
				iY = iHeight;
				iZ = sNewPosition.iZ;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = UNITSERVER->CreateUnitEnemy( psCrystal->pszName, iX, iY, iZ, pcUserData );

			if ( pcUnitData )
			{
				int iLengthName = STRLEN( pcUnitData->GetName() );
				int iLength		= STRLEN( "_Crystal" );

				ZeroMemory( &pcUnitData->sCharacterData.szName[iLengthName - iLength], iLength );

				STRINGCOPY( pcUnitData->sCharacterData.Pet.szOwnerName, CHARACTERSERVER->GetCharacterName( pcUserData ) );

				pcUnitData->sUnitInfo.dwHashCharacterName	= 0;
				pcUnitData->sCharacterData.iMonsterType		= MONSTERTYPE_Summon;
				pcUnitData->sCharacterData.iOwnerID			= pcUserData->iID;

				const int hp = UNITSERVER->GetMonsterHealthPointDefinition(pcUnitData->sUnitInfo.iUniqueMonsterID);
				pcUnitData->InitHealthPoint( hp );

				pcUnitData->pcOwner							= pcUserData;
				pcUnitData->dwUpdateCharInfoTime			= TICKCOUNT + (psCrystal->iTime * 60 * 1000);
				pcUnitData->pcFocusTarget					= NULL;
				pcUnitData->pcFocusingTarget				= NULL;
				pcUnitData->pcAttackTarget					= NULL;

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData, iItemID );

				//User Warp Effect summon crystal
				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID	= 0x100;
				sPacketEffect.iX	= iX;
				sPacketEffect.iY	= iY;
				sPacketEffect.iZ	= iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL MapServer::SpawnMonsterBC( Map * pcMap, CharacterData * psCharacterData, int iX, int iY, int iZ )
{
	return (BOOL)CALL_WITH_ARG5( 0x0055CC90, (DWORD)pcMap, (DWORD)psCharacterData, iX, iZ, iY );
}

UINT MapServer::GetWorldTimeHour()
{
	return CALL( 0x0054CBC0 );
}

int MapServer::MakeFaceArea( Stage * pcThis, int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom )
{
	if (!pcThis)
		return 0;

	int iFaceCount = 0;

	StageFace ** psaFaceList = (StageFace**)0x08B818F0;

	pcThis->dwSumCount++;
	int iCalcSum = pcThis->dwSumCount;

	int isX = (iX >> (8 + 6)) & 0xFF;
	int isZ = (iZ >> (8 + 6)) & 0xFF;

	int iwX = isX + (iWidth >> (8 + 6));
	int iwZ = isZ + (iHeight >> (8 + 6));

	for ( int cx = isX; cx < iwX; cx++ )
	{
		for ( int cz = isZ; cz < iwZ; cz++ )
		{
			int x = cx & 0xFF;
			int y = cz & 0xFF;

			if ( pcThis->pdwaArea[x][y] )
			{
				int iPos = pcThis->pdwaArea[x][y][0];

				for ( int i = 1; i < (iPos + 1); i++ )
				{
					int FaceNum = pcThis->pdwaArea[x][y][i];

					StageFace * psFace = pcThis->pFace + FaceNum;
					if ( psFace->iCalcSum != iCalcSum )
					{
						psFace->iCalcSum = iCalcSum;

						int iY1 = pcThis->pVertex[psFace->wVertex[0]].sPosition.iY;
						int iY2 = pcThis->pVertex[psFace->wVertex[1]].sPosition.iY;
						int iY3 = pcThis->pVertex[psFace->wVertex[2]].sPosition.iY;

						if ( (iY1 < iTop && iY1 > iBottom) || (iY2 < iTop && iY2 > iBottom) || (iY3 < iTop && iY3 > iBottom) )
						{
							psaFaceList[iFaceCount++]	= psFace;
							psaFaceList[iFaceCount]		= NULL;
						}
					}
				}
			}
		}
	}

	WRITEDWORD( 0x0736660C, iFaceCount );

	pcThis->iCalcSum++;

	return iFaceCount;
}

BOOL MapServer::SetupPolygonArea( Stage * pcThis )
{
	//CLEAR AREA
	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			pcThis->pdwaArea[j][i] = NULL;
		}
	}

	pcThis->psaAreaList = new Point2D[4096];
	ZeroMemory( pcThis->psaAreaList, sizeof( Point2D ) * 4096 );

	Point3D saVertexP[3];

	for ( int i = 0; i < pcThis->iFaceCount; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			saVertexP[j].iX = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iX >> 8;
			saVertexP[j].iY = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iY >> 8;
			saVertexP[j].iZ = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iZ >> 8;
		}
	}

	//TODO: GET POLY AREAS
	CALLT_WITH_ARG3( 0x00469590, (DWORD)pcThis, (DWORD)&saVertexP[0], (DWORD)&saVertexP[1], (DWORD)&saVertexP[2] );

	if ( pcThis->iAreaCount > 0 )
	{
		for ( int i = 0; i < pcThis->iAreaCount; i++ )
		{
			pcThis->pdwaArea[pcThis->psaAreaList[i].iX][pcThis->psaAreaList[i].iY]++;
		}
	}

	SAFE_DELETE_ARRAY( pcThis->psaAreaList );

	int iBufferCount = 0;

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			if ( (int)pcThis->pdwaArea[j][i] > 0 )
				iBufferCount += (int)(pcThis->pdwaArea[j][i]) + 1;
		}
	}

	pcThis->iAreaBufferSize = iBufferCount;
	pcThis->pdwAreaBuffer = new DWORD[iBufferCount];

	iBufferCount = 0;

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			if ( (int)pcThis->pdwaArea[j][i] > 0 )
			{
				pcThis->pdwaArea[j][i] = pcThis->pdwAreaBuffer + iBufferCount;
				pcThis->pdwaArea[j][i][0] = 0;

				iBufferCount += (int)(pcThis->pdwaArea[j][i]) + 1;
			}
		}
	}

	pcThis->psaAreaList = new Point2D[4096];
	ZeroMemory( pcThis->psaAreaList, sizeof( Point2D ) * 4096 );

	for ( int i = 0; i < pcThis->iFaceCount; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			saVertexP[j].iX = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iX >> 8;
			saVertexP[j].iY = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iY >> 8;
			saVertexP[j].iZ = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iZ >> 8;
		}

		//TODO: GET POLY AREAS
		CALLT_WITH_ARG3( 0x00469590, (DWORD)pcThis, (DWORD)&saVertexP[0], (DWORD)&saVertexP[1], (DWORD)&saVertexP[2] );


		if ( pcThis->iAreaCount > 0 )
		{
			for ( int j = 0; j < pcThis->iAreaCount; j++ )
			{
				pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY]++;

				int iPos = pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY][0];

				pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY][iPos] = i;
			}
		}
	}

	SAFE_DELETE_ARRAY( pcThis->psaAreaList );

	return TRUE;
}

int MapServer::GetHeightArea( Stage * pcThis, int iAX, int iAZ, int iX, int iZ )
{
	int iHeight = 0;

	int cx = iAX & 0xFF;
	int cz = iAZ & 0xFF;

	int iCalcSum = pcThis->iCalcSum;

	if ( pcThis->pdwaArea[cx][cz] )
	{
		int iPos = pcThis->pdwaArea[cx][cz][0];

		for ( int i = 1; i < (iPos + 1); i++ )
		{
			int iFaceNum = pcThis->pdwaArea[cx][cz][i];

			StageFace * psFace = pcThis->pFace + iFaceNum;

			if ( psFace->iCalcSum != iCalcSum /* MESH */ )
			{
				psFace->iCalcSum = iCalcSum;

				//TODO: GETPOLYHEIGHT
				int iH = 0;

				if ( iH != (-32767) && iH > iHeight )
					iHeight = iH;
			}
		}
	}

	return iHeight;
}

void MapServer::SendSpawnFlagMarkers( User * pcUser, int iMapID )
{
	if ( LOGIN_SERVER )
		return;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE || pcUser->GetGameLevel() <= GAMELEVEL_Two )
		return;


	DEBUG( "SendSpawnFlagMarkers - MAP ID = %d", iMapID );

	PacketMapSpawnMarkers sPacket;
	ZeroMemory ( &sPacket, sizeof ( PacketMapSpawnMarkers ) );
	sPacket.iLength = sizeof ( PacketMapSpawnMarkers );
	sPacket.iHeader = PKTHDR_MapSpawnMarkers;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 4 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT X, Z FROM MapSpawnPoint WHERE Stage = ?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iMapID );

			if ( pcDB->Execute())
			{
				Point3D * point = NULL;

				while ( pcDB->Fetch() )
				{
					point = &sPacket.saMarkers[sPacket.iCount];

					point->iY = 0;
					pcDB->GetData( 1, EDatabaseDataType::PARAMTYPE_Integer, &point->iX );
					pcDB->GetData( 2, EDatabaseDataType::PARAMTYPE_Integer, &point->iZ );

					//DEBUG( "SendSpawnFlagMarkers - MAP ID = %d, Index = %d, X = %d, Z = %d", iMapID, sPacket.iCount, point->iX, point->iZ );

					sPacket.iCount++;
				}
			}
		}
		pcDB->Close();
	}

	if ( sPacket.iCount > 0 )
	{
		sPacket.iLength = sizeof( Packet ) + 4 + ( sizeof( Point3D ) * sPacket.iCount );
		SENDPACKET( pcUser, &sPacket );
	}
}

void MapServer::UpdateUserBossTime( User * pcUser, int iMapID )
{
	if ( (iMapID >= 0) && (iMapID < NUM_MAPS) )
	{
		Map * pcMap = MAPSDATA + iMapID;
		if ( pcMap )
		{
			SYSTEMTIME sBossTime;
			CopyMemory( &sBossTime, GetServerTime(), sizeof( SYSTEMTIME ) );

			DWORD dwLessTimeBoss = 0;
			char * pszBossName = NULL;

			//Get Boss Time
			for ( int i = 0; i < pcMap->sSpawnSetting.iTotalBosses; i++ )
			{
				for ( int j = 0; j < pcMap->sSpawnSetting.saBoss[i].iBossHours; j++ )
				{
					//Get Boss Time
					sBossTime.wHour		= pcMap->sSpawnSetting.saBoss[i].caBossHour[j];
					sBossTime.wMinute	= BOSS_TIME;
					sBossTime.wSecond	= 0;

					//After the Time?
					if ( sBossTime.wMinute >= GetServerTime()->wMinute )
					{
						//If Boss was spawned at the current boss time on loop, go to the next day...
						if ( sBossTime.wHour < GetServerTime()->wHour )
							sBossTime.wDay++;
					}

					DWORD dwBossTime	= SystemTimeToDWORD( &sBossTime );
					DWORD dwServerTime	= SystemTimeToDWORD( GetServerTime() );
					if ( dwBossTime >= dwServerTime )
					{
						DWORD dwDifference = dwBossTime - dwServerTime;
						if ( (dwLessTimeBoss > dwDifference) || (dwLessTimeBoss == 0) )
						{
							pszBossName		= pcMap->sSpawnSetting.saBoss[i].cdBoss->szName;
							dwLessTimeBoss	= dwDifference;
						}
					}

					//Backup Time
					CopyMemory( &sBossTime, GetServerTime(), sizeof( SYSTEMTIME ) );
				}
			}

			PacketBossTimeUpdate sPacket;
			ZeroMemory( &sPacket, sizeof( PacketBossTimeUpdate ) );
			sPacket.iLength = sizeof( PacketBossTimeUpdate );
			sPacket.iHeader = PKTHDR_BossTimeUpdate;

			if ( pszBossName )
			{
				STRINGCOPY( sPacket.szBossName, pszBossName );
				sPacket.dwTime = dwLessTimeBoss;
			}

			SENDPACKET( pcUser, &sPacket );
		}
	}
}

void MapServer::UpdateUsersBossTime()
{
	//Update Users about Boss Time
	UINT uUsersInGame = USERSERVER->uUsersInGame;
	User ** pcaUserInGame = USERSERVER->pcaUserInGame;
	for ( UINT i = 0; i < uUsersInGame; i++ )
	{
		User * pcUser = pcaUserInGame[i];
		if (pcUser && pcUser->pcUserData && pcUser->IsConnected())
		{
			UpdateUserBossTime(pcUser, pcUser->GetMapID());
		}
	}
}

void MapServer::HandlePacket( User * pcUser, PacketBossTimeUpdate * psPacket )
{
	if ( (psPacket->iMapID >= 0) && (psPacket->iMapID <= NUM_MAPS) )
		UpdateUserBossTime( pcUser, psPacket->iMapID );
}

/// <summary>
/// ASM address: 0x46EB00
/// Leaked: int GetRadian2D(int x1 ,int y1, int x2, int y2 )
///
/// A function to find the two-dimensional angle between two points (this is also calculated by Lee Seong-su)
/// </summary>
/// <param name="iX"></param>
/// <param name="iZ"></param>
/// <param name="iTargetX"></param>
/// <param name="iTargetZ"></param>
/// <returns></returns>
NAKED int MapServer::GetAngle( int iX, int iZ, int iTargetX, int iTargetZ )
{
	JMP( pfnGetAngle );
}

/// <summary>
/// Find item at given position
/// </summary>
Loot * MapServer::FindItem( Map * pcMap, int iX, int iY, int iZ )
{
	Loot * ps = NULL;

	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		Loot * p = pcMap->saLoot + i;
		if ( p &&
			 p->bInUse &&
			 p->sPosition.iX == iX &&
			 p->sPosition.iY == iY &&
			 p->sPosition.iZ == iZ )
		{
			ps = p;
			break;
		}
	}
	return ps;
}


Loot * MapServer::AddItemV2( Map * pcMap, LootItem * psLootItem, int x, int y, int z, enum class ELootType eLootType )
{
	int cnt, cnt2;
	int height;
	int dx, dz, dy;
	int ta;

	DWORD dwItem;

	BOOL bRandomize = FALSE;
	BOOL bAdded = FALSE;
	BOOL bFound = FALSE;

	Loot * lpStgItem;
	int iBounds = 1;

	int i_X; //mid
	int i_Z; //top
	int i_Count = 0;

	//set starting pos
	i_X = ptItemSettingMatrix[pcMap->SpawnCount & 7].x;
	i_Z = ptItemSettingMatrix[pcMap->SpawnCount & 7].y;
	pcMap->SpawnCount++;

	do
	{
		//std::cout << "i_X: " << i_X << " i_Z:" << i_Z << " iBounds " << iBounds << std::endl;

		dx = x + ( i_X * ITEM_SET_DIST );
		dz = z + ( i_Z * ITEM_SET_DIST );
		dy = y;

		if ( bRandomize )
		{
			//shift towards middle of two bounds
			dx += ( i_X * ( ITEM_SET_DIST / 2 ) );
			dz += ( i_Z * ( ITEM_SET_DIST / 2 ) );

			//then randomize
			dx += Dice::RandomI ( -ITEM_SET_DIST, ITEM_SET_DIST );
			dz += Dice::RandomI ( -ITEM_SET_DIST, ITEM_SET_DIST );
		}

		//Find the floor height at the current location
		height = pcMap->pcStage->GetHighestPoint ( dx << FLOATNS, dy << FLOATNS, dz << FLOATNS, 32 * fONE );

		if ( height != CLIP_OUT )
		{
			ta = abs( height - ( dy << FLOATNS ) );
			if ( ta < 32 * fONE )
			{
				dy = height >> FLOATNS;

				//No item exists at location?
				lpStgItem = FindItem( pcMap, dx, dy, dz );
				if ( lpStgItem == NULL )
				{
					x = dx;
					z = dz;
					y = dy;

					bFound = TRUE;
					break;
				}
			}
		}


		//At top, go right till x reaches +ve bound
		if ( i_Z == -iBounds && i_X >= 0 && i_X < iBounds )
		{
			i_X++;
		}
		//At top-right corner or at right? go down until z reachs +ve bound
		else if ( ( i_Z == -iBounds && i_X == iBounds ) ||
			( i_X == iBounds && i_Z < iBounds) )
		{
			i_Z++;
		}
		//At bottom-right corner or at bottom? go down until x reachs -ve bound
		else if ( ( i_X == iBounds && i_Z == iBounds ) ||
			( i_Z == iBounds && i_X > -iBounds ))
		{
			i_X--;
		}
		//At bottom-left corner or at left? go up until z reachs +ve bound
		else if ( ( i_Z == iBounds && i_X == -iBounds ) ||
			i_X == -iBounds && i_Z > -iBounds)
		{
			i_Z--;
		}
		//At top-left corner or at top? go right until x reaches 0
		else if ( ( i_X == -iBounds && i_Z == -iBounds ) ||
			i_Z == -iBounds && i_X < 0)
		{
			i_X++;

			//at top-mid position? advance to next outer bounds
			if ( i_X == 0 )
			{
				iBounds++;
				i_Z = -iBounds;
			}
		}

		i_Count++;

		//3rd iterations, let's reset and randomize
		if ( iBounds == 3)
		{
			if ( bRandomize == FALSE )
			{
				iBounds = 1;

				i_X = ptItemSettingMatrix[pcMap->SpawnCount & 7].x;
				i_Z = ptItemSettingMatrix[pcMap->SpawnCount & 7].y;

				bRandomize = TRUE;
			}
			else
			{
				break;
			}
		}
	}
	while ( i_Count < 100 ); //max 100 attempts..

	//now let's find a spare position in the 1024 loot slots in map..
	for (cnt2 = 0; cnt2 < 2; cnt2++)
	{
		for (cnt = 0; cnt < MAX_LOOT; cnt++)
		{
			Loot * lpLoot = &pcMap->saLoot[cnt];

			BOOL bIsForceAllowDrop = ( lpLoot->sLootItem.sItem.eItemFlags & EItemFlag::ForceAllowDrop ) == EItemFlag::ForceAllowDrop;
			BOOL bisTeasureHuntingItem = ( lpLoot->sLootItem.sItem.eItemFlags & EItemFlag::TreasureHuntingMode ) == EItemFlag::TreasureHuntingMode;

			if (!lpLoot->bInUse ||									//loot not in use
				(cnt2 && lpLoot->Level == 0 &&						//max loot is reached, repeat loop again but replace anything that doesn't have '1' (gold, potion etc)
				!bIsForceAllowDrop && !bisTeasureHuntingItem))      //also avoid overriding treasure hunting items on 2nd pass
			{
				bAdded = TRUE;

				lpLoot->bInUse = TRUE;
				memcpy(&lpLoot->sLootItem, psLootItem, sizeof(LootItem));

				lpLoot->sPosition.iX = x;
				lpLoot->sPosition.iY = y;
				lpLoot->sPosition.iZ = z;

				lpLoot->dwDropTime = TICKCOUNT;
				lpLoot->eLootType = eLootType;

				dwItem = lpLoot->sLootItem.sItem.sItemID.ToItemType();

				if ( bIsForceAllowDrop || bisTeasureHuntingItem )
				{
					lpLoot->dwEndTime = lpLoot->dwDropTime + (1000 * 60 * 60 * 48);
					INFO( "Event item '%s' was dropped, and will expiry in 48 hours, eItemFlags = %d", psLootItem->sItem.szItemName, psLootItem->sItem.eItemFlags );
				}
				else
				{
					constexpr int iCommonExpiryTime			= ( 1000 * 60 * 2 );
					constexpr int iUncommonExpiryTime		= ( 1000 * 60 * 4 );
					constexpr int iRareExpiryTime			= ( 1000 * 60 * 6 );
					constexpr int iEpicExpiryTime			= ( 1000 * 60 * 8 );
					constexpr int iLegendaryExpiryTime		= ( 1000 * 60 * 10 );
					constexpr int iEverythingElseExpiryTime	= ( 1000 * 60 * 3 );

					if ( lpLoot->sLootItem.sItem.eRarity == EItemRarity::COMMON )
					{
						lpLoot->dwEndTime = lpLoot->dwDropTime + iCommonExpiryTime;
					}
					else if ( lpLoot->sLootItem.sItem.eRarity == EItemRarity::UNCOMMON )
					{
						lpLoot->dwEndTime = lpLoot->dwDropTime + iUncommonExpiryTime;
					}
					else if ( lpLoot->sLootItem.sItem.eRarity == EItemRarity::RARE )
					{
						lpLoot->dwEndTime = lpLoot->dwDropTime + iRareExpiryTime;
					}
					else if ( lpLoot->sLootItem.sItem.eRarity == EItemRarity::LEGENDARY )
					{
						lpLoot->dwEndTime = lpLoot->dwDropTime + iLegendaryExpiryTime;
					}
					else
					{
						lpLoot->dwEndTime = lpLoot->dwDropTime + iEverythingElseExpiryTime;
					}

					//for lvl 100+ map increase expiry time by 1 min
					if ( pcMap->pcBaseMap->iLevel >= 100 )
					{
						lpLoot->dwEndTime += (1000 * 60 * 1);
					}
				}

				//First item selection
				if ( dwItem != ITEMTYPE_ManaPotion &&
					 dwItem != ITEMTYPE_HealthPotion &&
					 dwItem != ITEMTYPE_StaminaPotion &&
					 dwItem != ITEMTYPE_GoldAndExp )
				{
					lpLoot->Level = 1;
				}
				else
				{
					lpLoot->Level = 0;
					dwItem = 0;
				}

				return lpLoot;
			}
		}
	}

	if ( bAdded == FALSE )
	{
		WARN( "Item %s was not added to map!", psLootItem->sItem.szItemName );
	}

	return NULL;
}


/// <summary>
/// Drop mass amount of gold around a position
/// </summary>
void MapServer::AddMassGoldDrops ( Map * pcMap, int x, int y, int z, int iGoldMin, int iGoldMax, int iDropCount, User * pcUser )
{
	int cnt2;
	int height;
	int dx, dz, dy;

	LootItem sItemLoot;
	sItemLoot.sItem.sItemID = EItemID::ITEMID_Gold;

	int iBounds = 1;
	int i_X = 0; //mid
	int i_Z = -iBounds; //top
	int i_Count = 0;

	//top row

	do
	{
		std::cout << "i_X: " << i_X << " i_Z:" << i_Z << " iBounds " << iBounds << std::endl;

		sItemLoot.sItem.iGold = Dice::RandomI(iGoldMin, iGoldMax);
		STRINGCOPY ( sItemLoot.sItem.szItemName, FormatString ( "%d Gold", sItemLoot.sItem.iGold ) );

		dx = x + ( i_X * ITEM_SET_DIST ) + Dice::RandomI ( -ITEM_SET_DIST / 2, ITEM_SET_DIST / 2 );
		dz = z + ( i_Z * ITEM_SET_DIST ) + Dice::RandomI ( -ITEM_SET_DIST / 2, ITEM_SET_DIST / 2 );
		dy = y;

		//Find the floor height at the current location
		height = pcMap->pcStage->GetHighestPoint ( dx << FLOATNS, dy << FLOATNS, dz << FLOATNS, 32 * fONE );

		dy = height >> FLOATNS;

		for ( cnt2 = 0; cnt2 < MAX_LOOT; cnt2++ )
		{
			Loot * pLoot = pcMap->saLoot + cnt2;

			//found?
			if ( !pLoot->bInUse )
			{
				pLoot->sPosition.iX = dx;
				pLoot->sPosition.iY = dy;
				pLoot->sPosition.iZ = dz;
				pLoot->bInUse = TRUE;

				memcpy ( &pLoot->sLootItem, &sItemLoot, sizeof ( LootItem ) );

				pLoot->dwDropTime = TICKCOUNT;
				pLoot->dwEndTime = pLoot->dwDropTime + STG_ITEM_WAIT_TIME;	//3 min
				pLoot->eLootType = ELootType::Initial;					//For checking whether the item is initially created

				ITEMSERVER->ReformItem ( &sItemLoot.sItem );

				MAPSERVER->SendItemStageUser( pcMap, pcUser, pLoot );
				pLoot->dwDropTime += 5000;

				break;
			}
		}


		//At top, go right till x reaches +ve bound
		if ( i_Z == -iBounds && i_X >= 0 && i_X < iBounds )
		{
			i_X++;
		}
		//At top-right corner or at right? go down until z reachs +ve bound
		else if ( ( i_Z == -iBounds && i_X == iBounds ) ||
			      ( i_X == iBounds && i_Z < iBounds) )
		{
			i_Z++;
		}
		//At bottom-right corner or at bottom? go down until x reachs -ve bound
		else if ( ( i_X == iBounds && i_Z == iBounds ) ||
				  ( i_Z == iBounds && i_X > -iBounds ))
		{
			i_X--;
		}
		//At bottom-left corner or at left? go up until z reachs +ve bound
		else if ( ( i_Z == iBounds && i_X == -iBounds ) ||
					i_X == -iBounds && i_Z > -iBounds)
		{
			i_Z--;
		}
		//At top-left corner or at top? go right until x reaches 0
		else if ( ( i_X == -iBounds && i_Z == -iBounds ) ||
					i_Z == -iBounds && i_X < 0)
		{
			i_X++;

			//at top-mid position? advance to next outer bounds
			if ( i_X == 0 )
			{
				iBounds++;
				i_Z = -iBounds;
			}
		}

		i_Count++;
	}
	while ( ( i_Count < iDropCount ) );

}