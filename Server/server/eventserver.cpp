#include "stdafx.h"
#include "eventserver.h"
#include <unordered_set>


CEventServer::CEventServer()
{
	bOnlineRewardInitialized = FALSE;

}

CEventServer::~CEventServer()
{
	for (std::vector<HuntEventMobData*>::iterator it = vHuntEventMobData.begin(); it != vHuntEventMobData.end(); it++)
	{
		SAFE_DELETE( *it );
	}

	vHuntEventMobData.clear();
	mHuntMobSpawnLocations.clear();
}


void CEventServer::Init ()
{
	SQLLoadMimicSpawnData();
	SQLLoadHuntEventData();
}

BOOL CEventServer::SQLLoadHuntEventData()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Event, 1 );

	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( "SELECT MonsterID FROM MonsterList WHERE Name = 'Wanted Moriph'", true ) )
		{
			if ( pcDB->Execute () && pcDB->Fetch () )
			{
				pcDB->GetData ( 1, EDatabaseDataType::PARAMTYPE_Integer, &iWantedMoriphMonsterID );
				//INFO ( "Wanted Moriph Monster ID = %d", iWantedMoriphMonsterID );
			}
			else
			{
				LOGERROR ( "'Wanted Moriph' not found in MonsterList" );
			}
		}

		pcDB->Clear ();

		if ( pcDB->Prepare ( "SELECT MonsterID FROM MonsterList WHERE Name = 'Wanted Wolf'", true ) )
		{
			if ( pcDB->Execute () && pcDB->Fetch () )
			{
				pcDB->GetData ( 1, EDatabaseDataType::PARAMTYPE_Integer, &iWantedWolfMonsterID );
				//INFO ( "Wanted Wolf Monster ID = %d", iWantedWolfMonsterID );
			}
			else
			{
				LOGERROR ( "'Wanted Wolf' not found in MonsterList" );
			}
		}

		pcDB->Close ();
	}

	mHuntMobSpawnLocations.clear();
	mHuntMobSpawnMapIDs.clear();

	pcDB = SQLCONNECTION( DATABASEID_GameDB_Event, 2 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [Stage], [X], [Z] FROM MapSpawnPoint WHERE Stage IN (0,1,2,4,5,6,7,8,10,17,18,19,20) ORDER BY Stage" ) )
		{
			int iPrevStageID = -1;

			if ( pcDB->Execute() )
			{
				int iStageID = 0;

				while ( pcDB->Fetch() )
				{
					SpawnPos pos{};

					pcDB->GetData( 1, PARAMTYPE_Integer, &iStageID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &pos.iPosX );
					pcDB->GetData( 3, PARAMTYPE_Integer, &pos.iPosZ );

					if ( iStageID != iPrevStageID )
					{
						mHuntMobSpawnMapIDs.push_back( iStageID );
						mHuntMobSpawnLocations.insert( { iStageID, std::vector<SpawnPos>{} } );
						iPrevStageID = iStageID;
					}

					mHuntMobSpawnLocations[iStageID].push_back( pos );
				}
			}
		}

		pcDB->Close();
	}


	return TRUE;
}

BOOL CEventServer::SQLLoadMimicSpawnData()
{
	if ( LOGIN_SERVER )
		return FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Event, 4 );

	BOOL bHasErrors = FALSE;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MapID, MimicName, SpawnChance, MinDelayBetweenSpawn FROM MimicSpawn", true ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					MimicSpawnInfo spawnInfo{};

					char szMimicName[32];

					pcDB->GetData( 1, EDatabaseDataType::PARAMTYPE_Integer, &spawnInfo.iMapID );
					pcDB->GetData( 2, EDatabaseDataType::PARAMTYPE_String, szMimicName, _countof( szMimicName ) );
					pcDB->GetData( 3, EDatabaseDataType::PARAMTYPE_Float, &spawnInfo.fSpawnChance );
					pcDB->GetData( 4, EDatabaseDataType::PARAMTYPE_Integer, &spawnInfo.iMinDelayBetweenSpawn );

					if ( spawnInfo.iMapID < 0 || spawnInfo.iMapID >= NUM_MAPS )
					{
						LOGERROR( "[SQLLoadMimicSpawnData] Invalid map id %d", spawnInfo.iMapID );
						bHasErrors = TRUE;
						continue;
					}

					auto charInfo = UNITINFODATA->FindEnemyCharInfo( szMimicName );

					if ( charInfo == NULL )
					{
						LOGERROR( "[SQLLoadMimicSpawnData] Unknown Monster name: '%s'. Ensure name matches data in MonsterList", szMimicName );
						bHasErrors = TRUE;
						continue;
					}

					DEBUG( "Loaded mimic: %s (%d) for map %s (%d)", charInfo->szName, charInfo->iUniqueMonsterID, pszaMapsName[spawnInfo.iMapID], spawnInfo.iMapID );

					spawnInfo.iMonsterID = charInfo->iUniqueMonsterID;

					auto it = mMapIdToMimicSpawnInfo.find( spawnInfo.iMapID );
					if ( it == mMapIdToMimicSpawnInfo.end() )
					{
						mMapIdToMimicSpawnInfo.insert( { spawnInfo.iMapID, spawnInfo } );
					}
					else
					{
						it->second.fSpawnChance = spawnInfo.fSpawnChance;
						it->second.iMapID = spawnInfo.iMapID;
						it->second.iMinDelayBetweenSpawn = spawnInfo.iMinDelayBetweenSpawn;
						it->second.iMonsterID = charInfo->iUniqueMonsterID;
					}
				}
			}
		}

		pcDB->Close();
	}

	return !bHasErrors;
}

BOOL CEventServer::SQLLogEventKill( enum ServerEventID eventID, User * pcUser, UnitData * pcUnitData )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB_EventKill, 1 );

	int iMapID = pcUnitData->pcMap->pcBaseMap->GetID();

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO EventKillLog (EventID, MapID, CharacterID, MonsterID, MonsterEffectID, DateTime) VALUES (?, ?, ?, ?, ?, GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, EDatabaseDataType::PARAMTYPE_Integer, &eventID );
			pcDB->BindParameterInput( 2, EDatabaseDataType::PARAMTYPE_Integer, &iMapID );
			pcDB->BindParameterInput( 3, EDatabaseDataType::PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 4, EDatabaseDataType::PARAMTYPE_Integer, &pcUnitData->sCharacterData.iUniqueMonsterID );
			pcDB->BindParameterInput( 5, EDatabaseDataType::PARAMTYPE_Integer, &pcUnitData->sCharacterData.iMonsterEffectID );

			pcDB->Execute();
		}

		pcDB->Close();
	}

	return TRUE;
}

BOOL CEventServer::TryGetEventMonsterSpawn( struct CharacterData * psCharInfo, class Map * pcMap )
{
	if ( psCharInfo == NULL )
		return FALSE;

	if ( pcMap == NULL )
		return FALSE;

	if ( pcMap->pcBaseMap->iMapID < 0 || pcMap->pcBaseMap->iMapID >= NUM_MAPS )
		return FALSE;

	if ( psCharInfo->iMonsterEffectID == MONSTEREFFECTID_MIMIC )
		return FALSE;

	//mimic event
	if ( GAME_SERVER && EVENT_MIMIC )
	{
		auto it = mMapIdToMimicSpawnInfo.find( pcMap->pcBaseMap->iMapID );
		if ( it == mMapIdToMimicSpawnInfo.end() )
			return FALSE;

		if ( it->second.fSpawnChance == 0.0f )
			return FALSE;

		const float value = Dice::RandomF( 0.0f, 1.0f );
		if ( it->second.fSpawnChance < value )
			return FALSE;

		if (it->second.tLastSpawnTime != 0 &&
			(it->second.tLastSpawnTime + it->second.iMinDelayBetweenSpawn) > TIME)
			return FALSE;

		CharacterData * psChar = UNITINFODATA->GetCharacterDataByMonsterID( it->second.iMonsterID );
		if ( psChar == NULL )
		{
			WARN( "Mimic monster doesn't exist for monster id: %d", it->second.iMonsterID );
			return FALSE;
		}

		CopyMemory( psCharInfo, psChar, sizeof( CharacterData ) );
		psCharInfo->iMonsterEffectID = MONSTEREFFECTID_MIMIC;

		return TRUE;
	}

	return FALSE;
}

BOOL CEventServer::OnEventMimicSpawned( class UnitData * pcUnitData, class Map * pcMap )
{
	INFO( "Mimic spawned: %s at map %s (%d)", pcUnitData->sCharacterData.szName, pszaMapsName[pcMap->pcBaseMap->iMapID], pcMap->pcBaseMap->iMapID );

	auto it = mMapIdToMimicSpawnInfo.find( pcMap->pcBaseMap->iMapID );
	if ( it == mMapIdToMimicSpawnInfo.end() )
		return FALSE;

	it->second.tLastSpawnTime = TIME;		//record time of last spawn

	return TRUE;
}

BOOL CEventServer::IsEventMimicMonster( UnitData * pcUnitData )
{
	return pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MIMIC
		   && pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Monster;
}

BOOL CEventServer::IsEventHuntMonster( UnitData * pcUnitData )
{
	return (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MORIF ||
		    pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MOLLYWOLF)
		    && pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Monster;
}

void CEventServer::GiveRudolphPrizes()
{
	if ( EVENT_CHRISTMAS == FALSE )
		return;

	unordered_set<std::string> userNamesSent;

	for ( std::vector<PrizeEventRudolphUser*>::iterator it = vChristmasEventRudolph.begin(); it != vChristmasEventRudolph.end(); it++ )
	{
		PrizeEventRudolphUser * psc = (*it);

		if ( psc->szAccountName[0] != 0 )
		{
			SPrizeData * ps = GetPrizeRewardItem();

			UserData* userData = NULL;
			if ((userData = USERDATABYACCOUNTNAME(psc->szAccountName)) != NULL)
			{
				char* charName = CHARACTERSERVER->GetCharacterName(userData);
				std::string charNameStr(charName);

				char szItemName[32] = { 0 };
				ITEMSERVER->AddItemOpenBox(psc->szAccountName, ps->szCode, 0, ps->iQuantity);

				if (ITEMSERVER->FindItemName(ps->szCode, szItemName))
				{
					SQLOnlineRewardLog(psc->szAccountName, charName, szItemName, ps->iQuantity);
				}

				std::unordered_set<std::string>::const_iterator got = userNamesSent.find(charNameStr);

				//not found - first time sending out message to user
				if (got == userNamesSent.end())
				{
					auto u = USERDATATOUSER(userData);
					CHATSERVER->SendChat(u, CHATCOLOR_Command, "Santa_Presents_Delivered"); //show dialog etc
					CHATSERVER->SendChat(u, CHATCOLOR_Global, "Santa Claus> Ho Ho Ho!! New presents have arrived under the Christmas Tree! Visit Item Distributor to get yours!");

					//add name to collection so the user doesn't get the message more than once
					userNamesSent.insert(userData->szCharacterName);
				}
			}
			else
			{
				ITEMSERVER->AddItemOpenBox(psc->szAccountName, ps->szCode, 0, ps->iQuantity);
			}
		}

		delete psc;
	}


	vChristmasEventRudolph.clear();


}

void CEventServer::HandlePacket( UserData * pcUserData, PacketNPCEasterEgg * psPacket )
{
	switch ( psPacket->iAction )
	{
		case PacketNPCEasterEgg::ACTION_None:

			//Welcome Message when open NPC...
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Easter Event:" );
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Welcome, let's get some prizes? ^^'" );

			break;
		case PacketNPCEasterEgg::ACTION_GetReward:
			int iCount;
			iCount = 0;

			//Check if get Items needed....
			for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
			{
				if ( psPacket->iaItemID[i] == ITEMID_RedEasterEgg || psPacket->iaItemID[i] == ITEMID_GreenEasterEgg || psPacket->iaItemID[i] == ITEMID_PinkEasterEgg )
					iCount++;
			}

			//Get all Items?
			if ( iCount == EASTER_ITEMS_MAX )
			{
				//Check for Valid Items...
				for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
				{
					//Fake?
					if (!ITEMSERVER->DeleteItemInventory( pcUserData, psPacket->iaItemID[i], psPacket->dwaHead[i], psPacket->dwaCheckSum[i] ))
					{
						//Log
						PacketLogCheat s;
						s.iCheatID	= CHEATLOGID_EasterEggFakeItem;
						s.LParam	= psPacket->iaItemID[i];
						s.SParam	= psPacket->dwaHead[i];
						s.EParam	= psPacket->dwaCheckSum[i];
						LOGSERVER->OnLogCheat( pcUserData, &s );
						return;
					}

					//Get Item Name
					char * pszItemName	= "Red Easter Egg";
					if ( psPacket->iaItemID[i] == ITEMID_GreenEasterEgg )
						pszItemName		= "Green Easter Egg";
					else if ( psPacket->iaItemID[i] == ITEMID_PinkEasterEgg )
						pszItemName		= "Pink Easter Egg";


					//Log it...
					LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardEasterGet, "Character[%s] Put Easter Egg ([%s][%08X][%d x %d])",
						CHARACTERSERVER->GetCharacterName( pcUserData ), pszItemName, psPacket->iaItemID[i], psPacket->dwaHead[i], psPacket->dwaCheckSum[i] );

				}

				//Get Reward
				SPrizeData * ps = GetPrizeRewardItem();
				if ( ps )
				{
					char szItemName[32] = { 0 };

					//Find Item Name to put on Log...
					if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
					{
						//Find Item by Code to get ID to use on Log...
						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( ps->szCode );

						if ( pcItemData )
						{
							//Send To ItemBox
							ITEMSERVER->AddItemOpenBox( pcUserData, ps->szCode, 0, ps->iQuantity );

							//Log it...
							LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardEasterGet, "Character[%s] Received Easter Reward ([%s][%08X] Count[%d])",
								CHARACTERSERVER->GetCharacterName(pcUserData), szItemName, pcItemData->sBaseItemID.ToItemID(), ps->iQuantity );

							//Send Message
							CHATSERVER->SendTitleBox( pcUserData, "Check Item Distributor to get your reward!" );
							CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), EChatColor::CHATCOLOR_Global, "> Check Item Distributor to get your reward!" );
						}
					}
				}
			}
			break;
	}
}

void CEventServer::HandlePacket( UserData * pcUserData, PacketNPCRudolphChristmas * psPacket )
{
	if ( (psPacket->iItemID == ITEMID_GiftBox1) || (psPacket->iItemID == ITEMID_GiftBox2) )
	{
		//Fake?
		if (!ITEMSERVER->DeleteItemInventory( pcUserData, psPacket->iItemID, psPacket->dwHead, psPacket->dwCheckSum ))
		{
			//Log
			PacketLogCheat s;
			s.iCheatID = CHEATLOGID_ChristmasRudolphFakeBox;
			s.LParam = psPacket->iItemID;
			s.SParam = psPacket->dwHead;
			s.EParam = psPacket->dwCheckSum;
			LOGSERVER->OnLogCheat( pcUserData, &s );
		}

		SPrizeData* ps = GetPrizeRewardItem();

		ITEMSERVER->AddItemOpenBox(pcUserData->szAccountName, ps->szCode, 0, ps->iQuantity);

		LOGSERVER->OnLogItemEx( USERDATATOUSER( pcUserData ), ITEMLOGID_RewardChristmasRudolphGet, "Character[%s] Trade Christmas Rudolph Reward", CHARACTERSERVER->GetCharacterName( pcUserData ) );

		vChristmasEventRudolph.push_back( new PrizeEventRudolphUser( pcUserData->szAccountName ) );

		CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Global, "Rudolph> Thank you, you'll receive your reward soon!" );
		CHATSERVER->SendTitleBox( pcUserData, "Santa Claus will deliver your gift to the Item Distributor soon!" );
	}
}

void CEventServer::HandlePacket( UserData * pcUserData, PacketNPCPuzzle * psPacket )
{
}


BOOL CEventServer::OnUnitKilled( UnitData * pcUnitData, User * pcKiller )
{
	if ( pcUnitData->PartyFlag != 0 )
	{
		if ( IsEventHuntMonster( pcUnitData ) )
		{
			OnHuntEventMobKilled( pcUnitData, pcKiller );

			pcUnitData->PartyFlag = 0;
			return TRUE;
		}
	}

	if (IsEventMimicMonster( pcUnitData ) && pcKiller )
	{
		AppendToKillTracker( pcKiller, pcUnitData );
		SQLLogEventKill( ServerEventID::Mimic, pcKiller, pcUnitData );
		INFO( "Event> %s killed by player %s at map id %d", pcUnitData->sCharacterData.szName, pcKiller->GetName(), pcUnitData->pcMap->pcBaseMap->iMapID );

		return TRUE;
	}

	return FALSE;
}

SPrizeData * CEventServer::GetPrizeRewardItem()
{
	int iRand = Dice::RandomI(0, iTotalPercent);
	int iTotal = 0;

	for (const auto v : vPrizesReward)
	{
		SPrizeData * p = v;
		if (p->bActive)
		{
			iTotal += p->iPercent;
			if (iRand <= iTotal)
				return p;
		}
	}
	return vPrizesReward[0];
}

void CEventServer::SQLOnlineRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 13 );
	if ( pcDB->Open() )
	{

		if ( pcDB->Prepare( "INSERT INTO OnlineRewardLog([AccountName],[Name],[ItemName],[Quantity],[Date]) VALUES (?,?,?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszName, 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszItemName, 32 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iQuantity );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

bool CEventServer::IncludeUserInPrizePool(UserData* ud)
{
	/*
	To prevent abuse of online rewards..
	 - Level 0-10 chars have 0% chance to be in the pool
	 - Level 10+ chars have 25% chances to be in the pool for prize (dice roll)
	 - Level 20+ chars have 50% chance to be in the pool for prize (dice roll)
	 - Level 30+ chars have 75% chance to be in the pool for prize (dice roll)
	 - Level 40+ chars have 100% chance to be in the pool for prize (dice roll)
	*/

	int userLevel = ud->GetLevel();


	if (userLevel < 10)
	{
		return false;
	}

	//10 - 20
	else if (userLevel < 20)
	{
		return Dice::RandomI(0, 99) <= 25;
	}

	//20 - 30
	else if (userLevel < 30)
	{
		return Dice::RandomI(0, 99) <= 50;
	}

	//30 - 40
	else if (userLevel < 40)
	{
		return Dice::RandomI(0, 99) <= 75;
	}

	//40+
	else
	{
		return true;
	}
}

void CEventServer::SetPrizeOnlineReward()
{
	static UserData* pcaUsersReward[1024];

	// Clear Memory
	ZeroMemory(pcaUsersReward, sizeof(UserData*) * 1024);

	int iCounter = 0;

	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		UserData* ud = USERSDATA + i;

		User* pcUser = USERDATATOUSER(ud);

		if (ud->pcSocketData && ud->iID != 0 &&
			ud->szAccountName[0] != 0 &&
			ud->sCharacterData.szName[0] != 0 &&
			ud->iGameLevel == 0 &&					//Exclude Game-Masters from online rewards
			pcUser &&
			pcUser->bNetServer == FALSE)
		{

			if (IncludeUserInPrizePool(ud))
				pcaUsersReward[iCounter++] = ud;
		}
	}


	char szItemName[32] = { 0 };

	if (iCounter > 0)
	{
		// Get Random and set prize
		UserData* u = pcaUsersReward[Dice::RandomI(0, iCounter - 1)];
		if (u)
		{
			SPrizeData* p = GetPrizeRewardItem();
			if (p)
			{
				ITEMSERVER->AddItemOpenBox(u, p->szCode, 0, p->iQuantity);
				if (ITEMSERVER->FindItemName(p->szCode, szItemName))
				{
					SQLOnlineRewardLog(u->szAccountName, CHARACTERSERVER->GetCharacterName(u), szItemName, p->iQuantity);
				}

				CHATSERVER->SendChat(USERDATATOUSER(u), CHATCOLOR_Command, "Online_Reward_Won");
				CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "OnlineReward> %s won this time!", CHARACTERSERVER->GetCharacterName(u));
			}
		}
	}
}

void CEventServer::StopHuntEvent()
{
	for (std::vector<HuntEventMobData*>::iterator it = vHuntEventMobData.begin(); it != vHuntEventMobData.end(); it++)
	{
		SAFE_DELETE( *it );
	}
	vHuntEventMobData.clear();
}

void CEventServer::ResetWantedWolfEvent()
{
	//clear out kill tracker
	mUserToHuntMobKilledCounter.clear();

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("DELETE FROM WantedWolfKills"))
		{
			pcDB->Execute();
		}

		pcDB->Clear();

		if (pcDB->Prepare("DELETE FROM WantedWolfDrops"))
		{
			pcDB->Execute();
		}

		pcDB->Close();
	}

	bHuntEventInitialized = FALSE;
}

BOOL CEventServer::GiveWantedMoriphTitles ()
{
	BOOL bResult = FALSE;
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_UserDB_LocalServer_TitleAura, 10 );
	if ( pcDB->Open () )
	{
		bResult = pcDB->Execute ( "EXEC GiveWantedMoriphTitlesBasedOnKills" );

		if ( bResult == FALSE )
		{
			LOGERROR ( "Execution of Stored Procedure 'GiveWantedMoriphTitlesBasedOnKills' failed!" );
		}

		pcDB->Close ();
	}

	return bResult;
}

BOOL CEventServer::GiveWantedWolfTitles ()
{
	BOOL bResult = FALSE;
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_UserDB_LocalServer_TitleAura, 11 );
	if ( pcDB->Open () )
	{
		bResult = pcDB->Execute ( "EXEC GiveWantedWolfTitlesBasedOnKills" );

		if ( bResult == FALSE )
		{
			LOGERROR ( "Execution of Stored Procedure 'GiveWantedWolfTitlesBasedOnKills' failed!" );
		}

		pcDB->Close ();
	}

	return bResult;

}

void CEventServer::ResetWantedMoriphEvent()
{
	//clear out kill tracker
	mUserToHuntMobKilledCounter.clear();


	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("DELETE FROM WantedMoriphKills"))
		{
			pcDB->Execute();
		}

		if (pcDB->Prepare("DELETE FROM WantedMoriphDrops"))
		{
			pcDB->Execute();
		}

		pcDB->Close();
	}

	bHuntEventInitialized = FALSE;
}


BOOL CEventServer::OnHuntEventMobKilled( UnitData* pcUnitData, User * pcKiller )
{
	//Moriph killed!
	for (HuntEventMobData * moriphData : vHuntEventMobData)
	{
		if (moriphData->bActive &&
			moriphData->iMonsterID == pcUnitData->iID)
		{
			ZeroMemory( moriphData, sizeof( HuntEventMobData ) );

			if ((EVENT_WANTEDMORIPH_EVENT || EVENT_WANTEDWOLF_EVENT) && pcKiller != NULL)
			{
				int iKillCount = 0;
				auto it = mUserToHuntMobKilledCounter.find(pcKiller->iCharacterID);

				//user not found. first time.
				if (it == mUserToHuntMobKilledCounter.end())
				{
					SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);
					if (pcDB->Open())
					{
						std::string query = "";

						if (EVENT_WANTEDMORIPH_EVENT)
							query = "SELECT COUNT(*) FROM WantedMoriphKills WHERE CharID=?";
						else if (EVENT_WANTEDWOLF_EVENT)
							query = "SELECT COUNT(*) FROM WantedWolfKills WHERE CharID=?";

						//Pick a random spawn flag from level 1 maps
						if (pcDB->Prepare(query.c_str()))
						{
							pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcKiller->iCharacterID);

							if (pcDB->Execute() && pcDB->Fetch())
							{
								pcDB->GetData(1, PARAMTYPE_Integer, &iKillCount);
							}
						}

						pcDB->Close();
					}

					iKillCount++;

					mUserToHuntMobKilledCounter.insert(std::make_pair(pcKiller->iCharacterID, iKillCount));
				}

				//found
				else
				{
					it->second++;
					iKillCount = it->second;


				}

				//first kill?
				if (iKillCount == 1)
				{
					if (EVENT_WANTEDMORIPH_EVENT)
						CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "%s hunted down a Wanted Moriph!", pcKiller->GetName());
					else if (EVENT_WANTEDWOLF_EVENT)
						CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "%s hunted down Molly Wolf!", pcKiller->GetName());
				}

				//every 5th kill?
				else if (it->second % 5 == 0)
				{
					if (EVENT_WANTEDMORIPH_EVENT)
						CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "%s hunted %d Wanted Moriphs!", pcKiller->GetName(), iKillCount);
					else if (EVENT_WANTEDWOLF_EVENT)
						CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "%s hunted %d Molly Wolfs!", pcKiller->GetName(), iKillCount);
				}

				//just for GM
				else
				{
					if (EVENT_WANTEDMORIPH_EVENT)
						CHATSERVER->SendChatAllGM("> [GM] Moriph hunted: %s - Map %d", pcKiller->GetName(), pcUnitData->pcMap->pcBaseMap->iMapID);
					else if (EVENT_WANTEDWOLF_EVENT)
						CHATSERVER->SendChatAllGM("> [GM] Wolf hunted: %s - Map %d", pcKiller->GetName(), pcUnitData->pcMap->pcBaseMap->iMapID);
				}

				//just for the user
				if (EVENT_WANTEDMORIPH_EVENT)
					CHATSERVER->SendChatEx(pcKiller, CHATCOLOR_Notice, "> You hunted %d Wanted Moriphs!", iKillCount);
				else if (EVENT_WANTEDWOLF_EVENT)
					CHATSERVER->SendChatEx(pcKiller, CHATCOLOR_Notice, "> You hunted %d Molly Wolfs!", iKillCount);


				//INSERT Kill Data into Database
				SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);
				if (pcDB->Open())
				{
					std::string query = "";

					if (EVENT_WANTEDMORIPH_EVENT)
						query = "INSERT INTO WantedMoriphKills ([CharID],[MapID],[UnixTime]) VALUES (?,?,?)";
					else if (EVENT_WANTEDWOLF_EVENT)
						query = "INSERT INTO WantedWolfKills ([CharID],[MapID],[UnixTime]) VALUES (?,?,?)";

					if (pcDB->Prepare(query.c_str()))
					{
						DWORD unixTime = GetUnixTime();

						pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcKiller->iCharacterID);
						pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUnitData->pcMap->pcBaseMap->iMapID);
						pcDB->BindParameterInput(3, PARAMTYPE_Integer, &unixTime);

						pcDB->Execute();
					}

					pcDB->Close();
				}


				pcDB = SQLCONNECTION ( DATABASEID_UserDB_LocalServer_KillTracker, 1 );
				if ( pcDB->Open () )
				{
					std::string query = " IF EXISTS (SELECT 0 FROM CharacterMonsterKillTracker WHERE CharacterID=? AND MonsterID=?)"
						" BEGIN"
						" UPDATE CharacterMonsterKillTracker SET KillCount=(KillCount+1) WHERE CharacterID=? AND MonsterID=?"
						" END ELSE BEGIN"
						" INSERT INTO CharacterMonsterKillTracker ([CharacterID],[MonsterID],[MonsterName],[KillCount]) VALUES (?, ?, ?, 1)"
						" END";

					int iMonsterID = 0;
					std::string szMonsterName;
					if ( EVENT_WANTEDMORIPH_EVENT )
					{
						iMonsterID = iWantedMoriphMonsterID;
						szMonsterName = "Wanted Moriph";
					}
					else if ( EVENT_WANTEDWOLF_EVENT )
					{
						iMonsterID = iWantedWolfMonsterID;
						szMonsterName = "Wanted Wolf";
					}

					if ( iMonsterID > 0 )
					{
						if ( pcDB->Prepare ( query.c_str () ) )
						{
							pcDB->BindParameterInput ( 1, PARAMTYPE_Integer, &pcKiller->iCharacterID );
							pcDB->BindParameterInput ( 2, PARAMTYPE_Integer, &iMonsterID );
							pcDB->BindParameterInput ( 3, PARAMTYPE_Integer, &pcKiller->iCharacterID );
							pcDB->BindParameterInput ( 4, PARAMTYPE_Integer, &iMonsterID );
							pcDB->BindParameterInput ( 5, PARAMTYPE_Integer, &pcKiller->iCharacterID );
							pcDB->BindParameterInput ( 6, PARAMTYPE_Integer, &iMonsterID );
							pcDB->BindParameterInput ( 7, PARAMTYPE_String, (char *)szMonsterName.c_str (), 32 );

							pcDB->Execute ();
						}
					}
					else
					{
						WARN ( "Wanted event Monster ID is 0" );
					}


					pcDB->Close ();
				}

			}

			return TRUE;
		}
	}

	WARN ( "Couldn't find matching event mob for id %d", pcUnitData->iID );

	return FALSE;
}

BOOL CEventServer::AppendToKillTracker(User * pcKiller, UnitData* pcUnitData )
{
	if ( pcKiller == NULL || pcKiller->IsValidAndInGame() == FALSE )
		return FALSE;

	auto pcDB = SQLCONNECTION ( DATABASEID_UserDB_LocalServer_KillTracker, 2 );
	if ( pcDB->Open () )
	{
		std::string query = " IF EXISTS (SELECT 0 FROM CharacterMonsterKillTracker WHERE CharacterID=? AND MonsterID=?)"
			" BEGIN"
			" UPDATE CharacterMonsterKillTracker SET KillCount=(KillCount+1) WHERE CharacterID=? AND MonsterID=?"
			" END ELSE BEGIN"
			" INSERT INTO CharacterMonsterKillTracker ([CharacterID],[MonsterID],[MonsterName],[KillCount]) VALUES (?, ?, ?, 1)"
			" END";

		if ( pcDB->Prepare ( query.c_str () ) )
		{
			pcDB->BindParameterInput ( 1, PARAMTYPE_Integer, &pcKiller->iCharacterID );
			pcDB->BindParameterInput ( 2, PARAMTYPE_Integer, &pcUnitData->sCharacterData.iUniqueMonsterID );
			pcDB->BindParameterInput ( 3, PARAMTYPE_Integer, &pcKiller->iCharacterID );
			pcDB->BindParameterInput ( 4, PARAMTYPE_Integer, &pcUnitData->sCharacterData.iUniqueMonsterID );
			pcDB->BindParameterInput ( 5, PARAMTYPE_Integer, &pcKiller->iCharacterID );
			pcDB->BindParameterInput ( 6, PARAMTYPE_Integer, &pcUnitData->sCharacterData.iUniqueMonsterID );
			pcDB->BindParameterInput( 7, PARAMTYPE_String, pcUnitData->sCharacterData.szName, 32 );

			pcDB->Execute ();
		}

		pcDB->Close ();
	}

	return TRUE;
}

BOOL CEventServer::SpawnHuntEventUnit(HuntEventMobData * pMoriphData, Map * pcMap, int iSpawnX, int iSpawnZ)
{
	CharacterData* psChar = NULL;

	if (EVENT_WANTEDMORIPH_EVENT)
		psChar = UNITSERVER->GetCharacterDataByName("Wanted Moriph");
	else if (EVENT_WANTEDWOLF_EVENT)
		psChar = UNITSERVER->GetCharacterDataByName("Wanted Wolf");

	MonsterMapSpawnHandle sMonster;
	sMonster.sCharacterData.szName[0] = 0;
	CopyMemory(&sMonster.sCharacterData, psChar, sizeof(CharacterData));

	static Point3D sPosition;
	sPosition.iX = iSpawnX << 8;
	sPosition.iY = 0;
	sPosition.iZ = iSpawnZ << 8;

	(*(Point3D**)0x07AC9E24) = &sPosition;

	pcMap->bForceCreateMonster = TRUE;

	UnitData * pcUnitData = MAPSERVER->CreateUnit(pcMap, &sMonster, 0);
	if ( pcUnitData )
	{
		pcUnitData->PartyFlag = 0x500; //hunt event id (required to avoid spam)
		pcUnitData->sSpawnPosition = pcUnitData->sPosition;
		pcUnitData->dwUpdateCharInfoTime = TICKCOUNT + (30 * 60 * 1000); //30 minute alive time

		DEBUG ( "Spawning Hunt Mob. Map = %d, Pos = %d,%d, Delay = %d", pcMap->pcBaseMap->iMapID, iSpawnX, iSpawnZ, EVENT_HUNT_SPAWN_DELAY_MIN );

		pMoriphData->iMonsterID = pcUnitData->iID;
		pMoriphData->iSpawnX = iSpawnX;
		pMoriphData->iSpawnZ = iSpawnZ;
		pMoriphData->iMapId = pcMap->pcBaseMap->iMapID;
		pMoriphData->bActive = TRUE;
		pMoriphData->dwNextSpawnTime = pcUnitData->dwUpdateCharInfoTime + ( 60 * 1000 ); //in case unit didn't die after 60s
	}

	pcMap->iNumActiveSpawns = pcMap->iLastActiveSpawn;
	(*(Point3D**)0x07AC9E24) = NULL;


	return TRUE;
}

void CEventServer::LogEventMobDropItem( UnitData* pcUnitData, UserData* pcUserData, Item * psItem)
{
	User* u = USERDATATOUSER(pcUserData);

	if (pcUserData && pcUnitData && u)
	{
		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);
		if (pcDB->Open())
		{
			std::string query;

			if (EVENT_WANTEDMORIPH_EVENT)
			{
				query = "INSERT INTO WantedMoriphDrops ([CharID],[MapID],[ItemName],[UnixTime]) VALUES (?,?,?,?)";
			}
			else
			{
				query = "INSERT INTO WantedWolfDrops ([CharID],[MapID],[ItemName],[UnixTime]) VALUES (?,?,?,?)";
			}

			//Pick a random spawn flag from level 1 maps
			if (pcDB->Prepare(query.c_str()))
			{
				DWORD unixTime = GetUnixTime();

				pcDB->BindParameterInput(1, PARAMTYPE_Integer, &u->iCharacterID);
				pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUnitData->pcMap->pcBaseMap->iMapID);
				pcDB->BindParameterInput(3, PARAMTYPE_String, &psItem->szItemName, 32);
				pcDB->BindParameterInput(4, PARAMTYPE_Integer, &unixTime);

				pcDB->Execute();
			}

			pcDB->Close();
		}
	}
}

BOOL CEventServer::SpawnHuntEventMobAtRandomSpawnFlag(HuntEventMobData * pHuntEventData)
{
	//5 attempts to find a suitable spawn.
	// Surely that's more than enough
	// Unless we have many spawns..
	int iAttempts = 10;

	if ( mHuntMobSpawnMapIDs.size() == 0 )
	{
		LOGERROR( "mHuntMobSpawnMapIDs.size() is 0" );
		return FALSE;
	}

	int iRandomMapID = mHuntMobSpawnMapIDs[Dice::RandomI( 0, mHuntMobSpawnMapIDs.size() - 1)];
	int iSpawnPosIndex = -1;

	if ( mHuntMobSpawnLocations.find( iRandomMapID ) == mHuntMobSpawnLocations.end() )
	{
		LOGERROR( "Map id %d doesn't exist for mHuntMobSpawnLocations", iRandomMapID );
		return FALSE;
	}

	if ( mHuntMobSpawnLocations[iRandomMapID].size() == 0 )
	{
		LOGERROR( "No spawn positions for map id %d for mHuntMobSpawnLocations", iRandomMapID );
		return FALSE;
	}

	while (iAttempts-- > 0 && iSpawnPosIndex == -1)
	{
		int iRandomPosIndex = Dice::RandomI(0, mHuntMobSpawnLocations[iRandomMapID].size() - 1);

		const auto & spawnPos = mHuntMobSpawnLocations[iRandomMapID][iRandomPosIndex];

		BOOL bFoundAnyClose = FALSE;

		//Check if this flag isn't used for an existing Moriph spawn, and
		// it's not too close to an existing one.
		for (HuntEventMobData * it : vHuntEventMobData)
		{
			if (it &&
				it->bActive &&
				iRandomMapID == it->iMapId)
			{
				//Relative Position
				int iDeltaX = spawnPos.iPosX - it->iSpawnX;
				int iDeltaZ = spawnPos.iPosZ - it->iSpawnZ;

				int iDistance = ( iDeltaX * iDeltaX + iDeltaZ * iDeltaZ );

				//std::cout << "Dist check: " << iDistance << " vs " << DISTANCE_UNIT_SEMIFAR << std::endl;
				if (iDistance < EVENT_HUNT_MIN_SPAWN_MIN_DIST)
				{
					//Too close. Let's pick another spawn flag
					bFoundAnyClose = TRUE;
					break;
				}
			}
		}

		if ( bFoundAnyClose == FALSE )
		{
			iSpawnPosIndex = iRandomPosIndex;
			break;
		}
	}

	if (iSpawnPosIndex != -1)
	{
		Map * pcMap = MAPSDATA + iRandomMapID;

		const auto & spawnPos = mHuntMobSpawnLocations[iRandomMapID][iSpawnPosIndex];

		SpawnHuntEventUnit( pHuntEventData, pcMap, spawnPos.iPosX, spawnPos.iPosZ );

		return TRUE;
	}

	//It's possible to get this if we don't have enough maps
	// and we have too many hunt mob spawns
	WARN ( " Unable to spawn a new event monster??" );

	return FALSE;
}

int CEventServer::GetTotalSpawnFlagsForHuntEvent()
{
	int iTotalSpawnFlags = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Event, 3 );

	if (pcDB->Open())
	{
		//Get the total number of spawns in all level 1 maps (From Garden of Freedom to Oasis). We should get about 1000 flags
		//For a maximum of 20 Hunt Mob, it's 1 in 50 flags that will be used.
		if (pcDB->Prepare("SELECT COUNT(*) FROM MapSpawnPoint WHERE Stage IN (0,1,2,4,5,6,7,8,10,17,18,19,20)"))
		{
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iTotalSpawnFlags);
			}
		}

		pcDB->Close();
	}

	return iTotalSpawnFlags;
}

BOOL CEventServer::IsHuntEventActive()
{
	return EVENT_WANTEDMORIPH_EVENT || EVENT_WANTEDWOLF_EVENT;
}

void CEventServer::CheckAndSpawnHuntEventMobs()
{
	//spawn more if needed
	for (int i = vHuntEventMobData.size(); i < EVENT_HUNT_MAX_SPAWN; i++)
	{
		HuntEventMobData * huntMobData = new HuntEventMobData();
		ZeroMemory( huntMobData, sizeof( HuntEventMobData ) );

		//spawn first 5 instantly
		if ( i < 10 )
		{
			huntMobData->dwNextSpawnTime = TICKCOUNT;
		}
		else
		{
			huntMobData->dwNextSpawnTime = TICKCOUNT + Dice::RandomI( 0, 10 ) * 60 * 1000; //random spawn between 0 to 10 minutes from now
		}

		vHuntEventMobData.push_back( huntMobData );
	}

	int iActiveCount = 0;

	for (HuntEventMobData* huntEventData : vHuntEventMobData)
	{
		//we reached the max spawn count. we're done
		if (iActiveCount >= EVENT_HUNT_MAX_SPAWN)
			break;

		if (huntEventData)
		{
			if (huntEventData->bActive)
			{
				if ( TICKCOUNT >= huntEventData->dwNextSpawnTime )
				{
					UnitData * pcUnitData = UNITSERVER->GetUnit( huntEventData->iMonsterID );
					if ( pcUnitData == NULL || !IsEventHuntMonster( pcUnitData ))
					{
						WARN( "Unit is no longer hunt event monster or is NULL??" );
						ZeroMemory( huntEventData, sizeof( HuntEventMobData ) );
						ZeroMemory( huntEventData, sizeof( HuntEventMobData ) );
					}
					else
					{
						if ( pcUnitData->GetAnimation() != EAnimationType::ANIMATIONTYPE_Die && pcUnitData->iAliveCounter < 10 )
						{
							WARN( "Killing off the active hunt event %d", pcUnitData->iID );
							pcUnitData->Animate( EAnimationType::ANIMATIONTYPE_Die );
							pcUnitData->SetCurrentHealth( 0 );
						}
					}
				}

				iActiveCount++;
			}
			else
			{
				if ( huntEventData->dwNextSpawnTime == 0 )
				{
					huntEventData->dwNextSpawnTime = TICKCOUNT + (EVENT_HUNT_SPAWN_DELAY_MIN * 60 * 1000);
				}

				//Is it time to spawn a new one?
				else if (TICKCOUNT >= huntEventData->dwNextSpawnTime)
				{
					if (SpawnHuntEventMobAtRandomSpawnFlag(huntEventData))
					{
						//std::cout << "Spawning a new Wolf after " << EVENT_HUNT_SPAWN_DELAY_MIN << " min delay.." << std::endl;
						iActiveCount++;
						break;
					}
				}
			}
		}
	}
}


void CEventServer::OnSayTime( UnitData * pcUnitData )
{
	EMonsterEffectID eEffectID = (EMonsterEffectID)pcUnitData->sCharacterData.iMonsterEffectID;

	if (EVENT_HALLOWEEN && eEffectID == MONSTEREFFECTID_BRAZILBEAR)
	{
		if (pcUnitData->dwMessageEndTime < TICKCOUNT)
		{
			char * pszaBrazilBearMessage[4] =
			{
				"Haha happy Halloween!",
				"Spooky scary ghosts will get you!",
				"Trick or treat!",
				"I'll be throwing toilet papers!"
			};
			int iRand = Dice::RandomI(0, 3);
			pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;
			CHATSERVER->SendUserBoxChatRange(pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaBrazilBearMessage[iRand]);
		}
	}

	if (EVENT_HALLOWEEN && eEffectID == MONSTEREFFECTID_BEAR)
	{
		if (pcUnitData->dwMessageEndTime < TICKCOUNT)
		{
			char * pszaBearMessage[4] =
			{
				"Haha happy Halloween!",
				"Spooky scary ghosts will get you!",
				"Trick or treat!",
				"Have you seen my Lantern?"
			};
			int iRand = Dice::RandomI(0, 3);
			pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;
			CHATSERVER->SendUserBoxChatRange(pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaBearMessage[iRand]);
		}
	}
}

void CEventServer::LoadPrizesOnlineReward()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB, 14 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare ( "SELECT ItemCode, Quantity, [Percent] FROM OnlineReward ORDER BY NEWID()" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					SPrizeData * p = new SPrizeData;
					pcDB->GetData( 1, PARAMTYPE_String, p->szCode, 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &p->iQuantity );
					pcDB->GetData( 3, PARAMTYPE_Integer, &p->iPercent );
					p->bActive = TRUE;
					iTotalPercent += p->iPercent;

					vPrizesReward.push_back( p );

					i++;
				}
			}
		}
		pcDB->Close();
	}
}



void CEventServer::Tick()
{
	if ( !GAME_SERVER )
	{
		//Initialization of online rewards
		if ( bOnlineRewardInitialized == FALSE )
		{
			INFO ( "Online rewards initialized!" );

			bOnlineRewardInitialized = TRUE;

			// Set Prizes
			LoadPrizesOnlineReward();
			wLastHourChristmas			= GetServerTime()->wDay;

			if (GetServerTime()->wMinute >= 30)
				wNextMinuteOnlineReward = 0;
			else
				wNextMinuteOnlineReward = 30;
		}

		if (bForceNextOnlineReward == TRUE)
		{
			bForceNextOnlineReward = FALSE;
			SetPrizeOnlineReward();
		}

		//Online rewards to occur every 30 minutes
		if (GetServerTime()->wMinute == wNextMinuteOnlineReward)
		{
			if (wNextMinuteOnlineReward == 0)
				wNextMinuteOnlineReward = 30;
			else
				wNextMinuteOnlineReward = 0;

			SetPrizeOnlineReward();
		}

		//give out presents every 1 hour
		if ( (wLastHourChristmas != GetServerTime()->wHour) || (bResetRudolph == TRUE) )
		{
			GiveRudolphPrizes();
			wLastHourChristmas = GetServerTime()->wHour;

			bResetRudolph = FALSE;
		}
	}
	else
	{
		if (EVENTSERVER->IsHuntEventActive())
		{
			if (bHuntEventInitialized == FALSE)
			{
				bHuntEventInitialized = TRUE;

				//Check every 10s to spawn more
				dwNextHuntMobCheckTime = TICKCOUNT + (10 * 1000);
			}

			else if (TICKCOUNT > dwNextHuntMobCheckTime)
			{
				dwNextHuntMobCheckTime = TICKCOUNT + ( 1 * 1000 );

				//std::cout << "Checking.." << std::endl;

				EVENTSERVER->CheckAndSpawnHuntEventMobs();
			}
		}
	}
}
