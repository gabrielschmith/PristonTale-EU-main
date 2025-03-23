#include "stdafx.h"
#include "UnitGame.h"
#include "gamecharacterstatus.h"
#include "Camera.h"

UnitGame * UnitGame::pcInstance = NULL;
int UnitGame::iSpeedHack = 0;
DWORD UnitGame::dwUpdatePacket = 0;
PacketServerInfo sServerInfo;

bool UnitGame::bNewStats = TRUE;


Unit  UnitGame::cViewUnit;

extern INT64 lExpEarned;

float fSZHPBar = 15.0f;

extern BOOL __cdecl AnimateMeshTreeNew( EXEMesh * pcMesh, int iFrame, int iAX, int iAY, int iAZ );

UnitGame::UnitGame()
{
	pcaUnitData = (UnitData*)0x0D16888;

	iParticleAura1st = 0;
	iParticleAura2nd = 0;
	iParticleAura3rd = 0;
	iParticleAuraOverall = 0;

	iBellatraRoomInitialPlayerCount = 0;
	iBellatraSoloCrownRanking = 0;

	pcUnitData = new UnitData();
	ZeroMemory( pcUnitData, sizeof( UnitData ) );

	pcUnit = new Unit();
	pcaUnit = new Unit[MAX_UNITS];

	cLogMutex = new std::mutex();

	for ( UINT i = 0; i < MAX_UNITS; i++ )
	{
		pcaUnit[i].uIndex = i;
		pcaUnit[i].pcUnitData = pcaUnitData + i;
	}

	WRITEDWORD( 0x00CF481C, pcUnitData );

	WRITEDWORD( CHAR_DATA_PTR, &pcUnitData->sCharacterData );

	pcUnit->Load( pcUnitData );
}

UnitGame::~UnitGame()
{
	SAFE_DELETE( pcUnitData );

	pcUnit->UnLoad();
	SAFE_DELETE( pcUnit );

	for ( UINT i = 0; i < MAX_UNITS; i++)
	{
		pcaUnit[i].UnLoad ();
	}

	SAFE_DELETE_ARRAY( pcaUnit );
	SAFE_DELETE( cLogMutex );
}

UnitData * UnitGame::GetUnitDataByID( ID lID )
{
	if ( lID == UNITDATA->iID )
		return UNITDATA;

	typedef UnitData*( __cdecl *t_GetUnitDataByID )( UINT uID );
	t_GetUnitDataByID GetUnitDataByID = (t_GetUnitDataByID)pfnGetUnitData;

	return GetUnitDataByID( lID );
}

Unit * UnitGame::UnitDataToUnit( UnitData * pcUnitData )
{
	if ( pcUnitData == NULL )
		return NULL;

	//Find matching Unit for this UnitData
	Unit * pcUnit = NULL;

	if ( UNITDATA == pcUnitData )
	{
		//UnitData is self
		pcUnit = this->pcUnit;
	}
	else if ( (UnitData*)(0x0207C888) == pcUnitData )
	{
		//The View Unit
		pcUnit = &cViewUnit;
	}
	else if ( (pcUnitData - pcaUnitData) < MAX_UNITS )
	{
		//Withing the Unit Array
		pcUnit = pcaUnit + (pcUnitData - pcaUnitData);
	}

	//Nothing? Then use a Static Unit
	if ( pcUnit == NULL )
	{
		static Unit t;
		pcUnit = &t;
	}

	//Load Unit if ID mismatch between Unit and UnitData
	if ( ( pcUnit->pcUnitData != pcUnitData ) || ( pcUnit->iID != pcUnitData->iID ) )
	{
		pcUnit->Load ( pcUnitData );
	}

	return pcUnit;
}

void UnitGame::DrawLifeBar( int iX, int iY )
{
	// Vars
	int iPercent = 0, iPercent2 = 0, iPercent3 = 0;
	DWORD dwColor = 0;

	UnitData * pc = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );

	if ( pc == NULL )
		return;

	if( pc->sCharacterData.iType == 1 )
		pc->bShowHP = TRUE;
	else
		pc->bShowHP = FALSE;

	// HP Max is 0? no need show
	if( pc->GetMaxHealth() == 0 || ( !DEBUG_GM && !pc->bShowHP ) )
		return;

	// Treepass screen? no show
	if( iX < 0 || ( iX + 100 ) >= ( *(int*)0x03A9A30C ) )
		return;

	// Treepass screen? no show
	if( iY < 0 || ( iY + 100 ) >= ( *(int*)0x03A9A310 ) )
		return;

	// HP Percent
	iPercent = ( pc->GetCurrentHealth() * 92 ) / pc->GetMaxHealth();
	iPercent2 = ( pc->GetCurrentHealth() * 511 ) / pc->GetMaxHealth();
	iPercent2 = ( iPercent2 * iPercent2 ) / 512;
	iPercent3 = iPercent2 & 0xFF;

	// Color based to percent HP
	if( iPercent2 < 256 )
		dwColor = D3DCOLOR_RGBA( 255, iPercent3, 0, 255 );
	else
		dwColor = D3DCOLOR_RGBA( 255 - iPercent3, 255, 0, 255 );

	// Calculate Bar Size relative to Resolution
	int iHeight = (RESOLUTION_WIDTH / 4) * 3;
	float fSize = 0, fSizeW = 0;
	fSizeW = (float)RESOLUTION_WIDTH * (*(float*)0x007AFD8C);

	if( iHeight == RESOLUTION_HEIGHT )
		fSize = ((float)RESOLUTION_WIDTH / 800.0f);
	else
		fSize = ((float)RESOLUTION_WIDTH / (float)( ( RESOLUTION_HEIGHT / 3 ) * 4 ));

	// Texture 0 = Bar, 1 = Color
	int * piImage = ( (int*)0x00CDFF54 );

	// Functions
	typedef void( __cdecl *tfnDrawTexImageFloat )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, int Transp );
	typedef void( __cdecl *tfnDrawTexImageFloatColor )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, DWORD dwColor );
	tfnDrawTexImageFloat fnDrawTexImageFloat = (tfnDrawTexImageFloat)0x00409F40;
	tfnDrawTexImageFloatColor fnDrawTexImageFloatColor = (tfnDrawTexImageFloatColor)0x0040A240;

	// Draw Bar model
	fnDrawTexImageFloat( piImage[0], (float)iX, (float)iY, 100 * fSizeW, DXGraphicEngine::GetWidthBackBuffer() > 1024 ? 22.5f : 16.0f*fSize, 128, 16, 0, 0, 100, 15, 255 );

	// Have HP? Draw inside Bar model
	if( pc->GetCurrentHealth() > 0 )
		fnDrawTexImageFloatColor( piImage[1], (float)iX, (float)iY, (float)(iPercent + 3)*fSizeW, DXGraphicEngine::GetWidthBackBuffer() > 1024 ? 22.5f : 16.0f*fSize, 128, DXGraphicEngine::GetWidthBackBuffer() == 800 ? 18.0f : 16.0f, 0, 0, (float)iPercent + 3, 15, dwColor );
}

BOOL UnitGame::SetCharacterHeadModel( const char * pszHeadModel, BOOL bEffect )
{
	// Model not exists?
	char szModelName[64] = { 0 };

	STRINGCOPY( szModelName, pszHeadModel );
	int iLen = STRLEN( szModelName );
	if( iLen > 3 )
	{
		szModelName[iLen - 3] = 'i';
		szModelName[iLen - 2] = 'n';
		szModelName[iLen - 1] = 'x';
	}

	if( !FILEEXIST( szModelName ) && !FILEEXIST( pszHeadModel ) )
		return FALSE;

	if( STRINGCOMPARE( pszHeadModel, UNITDATA->sCharacterData.Player.szHeadModel ) )
		return TRUE;

	STRINGCOPY( UNITDATA->sCharacterData.Player.szHeadModel, pszHeadModel );
	UNITDATA->bNoMove = TRUE;

	SetPlayerPattern( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );

	UNITDATA->sCharacterData.cNewLoad++;
	CHECK_CHARACTER_CHECKSUM;
	UpdateObjectID( UNITDATA->iID );

	SAVE;

	if( bEffect )
	{
		SKILLMANAGERHANDLER->StartSkillEffect( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, 0, 0, 0, SKILL_UP1 );
		SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
	}

	return TRUE;
}

void UnitGame::LogCSV ( const char * format, ... )
{
	auto name = pcUnit->GetName ();

	if ( name == NULL || strlen ( name ) == 0 )
		return;

	if ( pcLoggerCSV == nullptr )
	{
		auto logFileName = FormatString ( "%s.csv", name );

		//remove existing file if there is one
		remove ( logFileName );

		pcLoggerCSV = CLogger::GetLogger ( logFileName );
	}

	cLogMutex->lock ();
	va_list args;
	va_start ( args, format );
	pcLoggerCSV->LogRaw ( format, args );
	va_end ( args );

	cLogMutex->unlock ();
}

void UnitGame::Log(const char* format, ...)
{
	if ( pcUnit == NULL )
		return;

	auto name = pcUnit->GetName();

	if (name == NULL || strlen(name) == 0)
		return;

	if (pcLogger == nullptr)
	{
		auto logFileName = FormatString("%s.log", name);

		//remove existing file if there is one
		remove(logFileName);

		pcLogger = CLogger::GetLogger(logFileName);
	}

	cLogMutex->lock();
	va_list args;
	va_start(args, format);
	pcLogger->Log(format, args);
	va_end(args);

	cLogMutex->unlock();
}

void UnitGame::LogCSV ( const std::string & sMessage )
{
	auto name = pcUnit->GetName ();

	if ( strlen ( name ) == 0 )
		return;

	if ( !pcLoggerCSV )
	{
		auto logFileName = FormatString ( "%s.csv", name );

		//remove existing file if there is one
		remove ( logFileName );

		pcLoggerCSV = CLogger::GetLogger ( logFileName );
	}

	pcLoggerCSV->LogRaw ( sMessage );
}

void UnitGame::Log(const std::string& sMessage)
{
	auto name = pcUnit->GetName();

	if (strlen(name) == 0)
		return;

	if (!pcLogger)
	{
		if (strlen(name) == 0)
			return;

		auto logFileName = FormatString ( "%s.log", name );

		//remove existing file if there is one
		remove ( logFileName );

		pcLogger = CLogger::GetLogger ( logFileName );
	}

	cLogMutex->lock ();
	pcLogger->Log(sMessage);
	cLogMutex->unlock ();
}

void UnitGame::FlushLog()
{
	if ( pcLogger )
	{
		cLogMutex->lock ();
		pcLogger->Flush ();
		cLogMutex->unlock ();
	}

	if ( pcLoggerCSV )
	{
		pcLoggerCSV->Flush ();
	}
}

void UnitGame::ClearFarUnits()
{
	// This Function has been rewrite to reduce lag while playing actions
	static int iUnitMonstersCount;
	static BOOL bCountNowUnitMonsters;

	BOOL bForceHideUnit;
	BOOL bDontFreeUnit;
	BOOL bIsPlayer;
	ECharacterType characterType;
	int iX = 0, iZ = 0;

	int iMapID = MAP_ID;
	int iSecondMapID = -1;

	int iSecondMapIndex = (UNITDATA->iLoadedMapIndex == 0) ? 1 : 0;
	if ( FIELDGAME[iSecondMapIndex] )
		iSecondMapID = FIELDGAME[iSecondMapIndex]->iMapID;

	int iUnitMapID = -1;

	for( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * p = pcaUnitData + i;
		// Unit Active?
		if( p->bActive )
		{
			bForceHideUnit = FALSE;
			bDontFreeUnit = FALSE;

			iUnitMapID = p->sCharacterData.sMapID;
			characterType = p->sCharacterData.iType;

			bIsPlayer = characterType == ECharacterType::CHARACTERTYPE_Player || p->PkMode_CharState == ECharacterType::CHARACTERTYPE_Player;

			if ((p->dwLastActiveTime + 16000) < TICKCOUNT)
			{
				bForceHideUnit = TRUE;
			}

			if ( characterType == CHARACTERTYPE_NPC )
			{
				if ( iUnitMapID == iMapID || iUnitMapID == iSecondMapID )
				{
					continue;
				}
				else
				{
					bForceHideUnit = TRUE;
				}
			}

			if ( ( ( bIsPlayer && bForceHideUnit == FALSE ) &&
				     iUnitMapID == iMapID ))
			{
				bDontFreeUnit = TRUE;
			}

			int iDistanceXZ = 0, iDistanceXZY = 0, iDistanceX = 0, iDistanceZ = 0, iDistanceY = 0;
			p->sPosition.GetPTDistanceXZY(&UNITDATA->sPosition, iDistanceXZY, iDistanceXZ, iDistanceX, iDistanceZ, iDistanceY);

			// Distance is far from player or can delete it?
			if( bForceHideUnit || ( p->bVisible && (iDistanceXZ > DISTANCE_MAX_UNIT_OUTOFRANGE || iDistanceX > DISTANCE_MISC || iDistanceZ > DISTANCE_MISC || iDistanceY > DISTANCE_MISC_Y)) )
			{
				//if ( bIsPlayer )
				//{
				//	CHATBOX->AddDebugMessage( "Setting unit to invisible" );
				//	UNITGAME->Log( "Setting unit to invisible" );
				//	UNITGAME->Log( "p->bVisible = %d, bForceFreeUnit = %d, iDistanceXZ = %d", p->bVisible, bForceFreeUnit, iDistanceXZ );
				//}

				//UnitData * pp = (*(UnitData**)0x00CF48D0);

				//p->bVisible controls other things such as visibility of HP, etc
				//See BOOL __cdecl RenderUnitData( UnitData * pcUnitData ) for example
				//Note = Setting visible to false won't remove the unit from mini map
				// but calling p->Free() will.
				p->bVisible = FALSE;

				if (bForceHideUnit && !bDontFreeUnit)
				{
					// Clear unit on memory (delete data about that char)
					p->Free();
				}
			}
			else if (bIsPlayer &&
				!p->bVisible && iDistanceXZ <= DISTANCE_MAX_UNIT_BASIC_VIEW && iDistanceX < DISTANCE_MISC && iDistanceZ < DISTANCE_MISC && iDistanceY < DISTANCE_MISC_Y)
			{
				p->bVisible = TRUE;
			}
		}
	}
}

void UnitGame::AddEXP( INT64 iEXP )
{
	LARGE_INTEGER s;
	s.QuadPart = iEXP;

	if( UNITDATA->sCharacterData.iLevel < *(int*)( 0x04B06F14 ) )
		CALL_WITH_ARG2( pfnAddCharacterEXP, s.LowPart, s.HighPart );

	lExpEarned += iEXP;
}

INT64 UnitGame::GetEXPFromLevel( int iLevel )
{
	if ( iLevel > 0 && iLevel <= (*(int*)(0x04B06F14) + 1) )
	{
		INT64 * iEXPTable = (INT64*)(0x0082B9E8);

		return iEXPTable[iLevel-1];
	}

	return 0;
}




BOOL UnitGame::OnLoseEXP()
{
	if (CHARACTERGAME->IsStageVillage() ||
		MAP_ID == MAPID_GardenOfFreedom ||
		MAP_ID == MAPID_CastleOfTheLost ||
		MAP_ID == MAPID_BlessCastle ||
		MAP_ID == MAPID_QuestArena ||
		MAP_ID == MAPID_T5QuestArena ||
		MAP_ID == MAPID_Bellatra ||
		GRANDFURYHANDLER->InsideArena(UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ))
	{
		return FALSE;
	}

	INT64 iEXPCharacter = CHARACTERGAME->GetEXP();

	LARGE_INTEGER liEXPLevel;
	liEXPLevel.QuadPart = GetEXPFromLevel( UNITDATA->sCharacterData.iLevel );

	INT64 iEXP = GetEXPFromLevel( UNITDATA->sCharacterData.iLevel + 1 ) - liEXPLevel.QuadPart;

	int iPercent = 3;

	BOOL bRessurectionItem = FALSE;

	if ( UNITDATA->sCharacterData.iLevel > 10 )
	{
		DWORD dwQuestFlag = PLAYER_QUESTLOG;

		if ( ((PLAYER_QUESTLOG & EAsmQuestBit::Level_85) != 0) && (UNITDATA->sCharacterData.iLevel >= 85) )
			iPercent = 2;

		//Bless of Kalia?
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) && ITEMHANDLER->GetItemInventoryByCode( ITEMID_BlessOfKalia ) )
			iPercent = 1;

		ItemData * pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_RebirthScroll );

		if ( pcItemData )
		{
			bRessurectionItem = TRUE;

			ITEMHANDLER->UseItem( &pcItemData->sItem );
			ITEMHANDLER->DeleteItem( pcItemData );
			iPercent = 0;

			PacketLoseExperience sPacket;
			ZeroMemory( &sPacket, sizeof( PacketLoseExperience ) );
			sPacket.iLength = sizeof( PacketLoseExperience );
			sPacket.iHeader = PKTHDR_LoseExp;
			sPacket.iSubExp = 0;
			sPacket.bRessurectionItem = bRessurectionItem;
			sPacket.iLevel = UNITDATA->sCharacterData.iLevel;
			sPacket.iMapID = MAP_ID;
			SENDPACKETL( &sPacket );

			WRITEDWORD( 0x0209EA4C, READDWORD( 0x0CF4750 ) + 180000 );
		}

		if ( iPercent > 0 )
		{
			INT64 iExpLose = (INT64)round( ((double)iEXP * iPercent) / (double)100 );

			iEXP = iEXPCharacter - iExpLose;

			if ( iEXP < liEXPLevel.QuadPart )
			{
				iEXP = liEXPLevel.QuadPart;
				iExpLose = iEXPCharacter - liEXPLevel.QuadPart;
			}

			if ( iExpLose > 0 )
			{
				PacketLoseExperience sPacket;
				ZeroMemory( &sPacket, sizeof( PacketLoseExperience ) );
				sPacket.iLength = sizeof( PacketLoseExperience );
				sPacket.iHeader = PKTHDR_LoseExp;
				sPacket.iSubExp = iExpLose;
				sPacket.bRessurectionItem = bRessurectionItem;
				sPacket.iLevel = UNITDATA->sCharacterData.iLevel;
				sPacket.iMapID = MAP_ID;
				SENDPACKETL( &sPacket );

				LARGE_INTEGER li;

				li.QuadPart = iEXPCharacter - iExpLose;

				CHARACTERGAME->SetEXP( li.QuadPart );
			}

			int iGold = UNITDATA->sCharacterData.iGold / 100;

			if ( CHARACTERGAME->CanUseGold( iGold ) )
			{
				UNITDATA->sCharacterData.iGold -= iGold;

				ITEMHANDLER->SendGoldUse( iGold );
				ITEMHANDLER->ProcessGold();
				PLAYMINISOUND( 18 );
			}
		}

		UPDATE_CHARACTER_CHECKSUM;

		CALL( 0x0045DE30 );
	}

	return TRUE;
}

int UnitGame::GetCharacterAbsorption()
{
	int iUnitAbsorption = UNITDATA->sCharacterData.iAbsorbRating;

	//iUnitAbsorption += SKILLMANAGERHANDLER->GetSkillAbsorptionBuffs ();

	//iUnitAbsorption += *(int*)0x035D0CDC; //??
	//iUnitAbsorption += *(int*)0x035EB608; //Disabled - Complusion (old)
	//iUnitAbsorption += *(int*)0x035D0CDC; //??

	return iUnitAbsorption;
}

/// <summary>
/// checks left slot (right hand)
/// </summary>
BOOL UnitGame::IsWeaponEquippedOnSelf(EItemType eItemType)
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex >= 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == eItemType )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL UnitGame::IsShieldEquippedOnSelf()
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex >= 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL UnitGame::IsOrbEquippedOnSelf ( )
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex >= 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Orb )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void UnitGame::ReceiveServerInformation( PacketServerInfo * psPacket )
{
	CopyMemory( &sServerInfo, psPacket, sizeof(PacketServerInfo) );
	GM_MODE = psPacket->bGameMaster;

	if( GM_MODE )
		*(int*)( 0x04B06F14 ) = 140;

	CopyMemory( &GameCore::GetInstance()->sServerTime, &sServerInfo.sServerTime, sizeof(SYSTEMTIME) );
}

#ifdef ITEMFIXES
void UnitGame::GetAffixStats( ItemFixData * eItemFixData, ItemData eItemData )
{
	eItemFixData->fAbsorb += eItemData.sItem.sPrefixData.fAbsorb;
	eItemFixData->fBlock += eItemData.sItem.sPrefixData.fBlock;
	eItemFixData->fCritical += eItemData.sItem.sPrefixData.fCritical;
	eItemFixData->fRegenerationHP += eItemData.sItem.sPrefixData.fRegenerationHP;
	eItemFixData->fRegenerationMP += eItemData.sItem.sPrefixData.fRegenerationMP;
	eItemFixData->fRunSpeed += eItemData.sItem.sPrefixData.fRunSpeed;
	eItemFixData->iAddHP += eItemData.sItem.sPrefixData.iAddHP;
	eItemFixData->iAddMP += eItemData.sItem.sPrefixData.iAddMP;
	eItemFixData->iATKPow += eItemData.sItem.sPrefixData.iATKPow;
	eItemFixData->iATKRating += eItemData.sItem.sPrefixData.iATKRating;
	eItemFixData->iDefense += eItemData.sItem.sPrefixData.iDefense;
	eItemFixData->iFire += eItemData.sItem.sPrefixData.iFire;
	eItemFixData->iFrost += eItemData.sItem.sPrefixData.iFrost;
	eItemFixData->iLightning += eItemData.sItem.sPrefixData.iLightning;
	eItemFixData->iOrganic += eItemData.sItem.sPrefixData.iOrganic;
	eItemFixData->iPoison += eItemData.sItem.sPrefixData.iPoison;
	eItemFixData->iRange += eItemData.sItem.sPrefixData.iRange;

	eItemFixData->fAbsorb += eItemData.sItem.sSuffixData.fAbsorb;
	eItemFixData->fBlock += eItemData.sItem.sSuffixData.fBlock;
	eItemFixData->fCritical += eItemData.sItem.sSuffixData.fCritical;
	eItemFixData->fRegenerationHP += eItemData.sItem.sSuffixData.fRegenerationHP;
	eItemFixData->fRegenerationMP += eItemData.sItem.sSuffixData.fRegenerationMP;
	eItemFixData->fRunSpeed += eItemData.sItem.sSuffixData.fRunSpeed;
	eItemFixData->iAddHP += eItemData.sItem.sSuffixData.iAddHP;
	eItemFixData->iAddMP += eItemData.sItem.sSuffixData.iAddMP;
	eItemFixData->iATKPow += eItemData.sItem.sSuffixData.iATKPow;
	eItemFixData->iATKRating += eItemData.sItem.sSuffixData.iATKRating;
	eItemFixData->iDefense += eItemData.sItem.sSuffixData.iDefense;
	eItemFixData->iFire += eItemData.sItem.sSuffixData.iFire;
	eItemFixData->iFrost += eItemData.sItem.sSuffixData.iFrost;
	eItemFixData->iLightning += eItemData.sItem.sSuffixData.iLightning;
	eItemFixData->iOrganic += eItemData.sItem.sSuffixData.iOrganic;
	eItemFixData->iPoison += eItemData.sItem.sSuffixData.iPoison;
	eItemFixData->iRange += eItemData.sItem.sSuffixData.iRange;

}
#endif

/// <summary>
///
/// </summary>
/// <param name="pItem"></param>
/// <param name="pInventory"></param>
/// <param name="iLevelDamage"></param>
/// <param name="iSkillDamage"></param>
/// <param name="iWeaponDamage">Weapon damage</param>
void UnitGame::SetCharacterFormula ( ItemData * pItem, InventoryItemData * pInventory, int * iLevelDamage, int * iSkillDamage, int * iWeaponDamage )
{
	int iItemIndex = pInventory[0].iItemIndex - 1;
	int iItemClass = pItem[iItemIndex].iItemWpnDmg;
	int iItemSpec = pItem[iItemIndex].sItem.eSpecialization;

	BOOL bAddSpecBonus = FALSE;

#ifdef ITEMFIXES
	ItemFixData eAllAffixData;
	for ( int i = 0; i <= 9; i++ )
	{
		ItemData CurrentItem = pItem[pInventory[i].iItemIndex - 1];
		GetAffixStats ( &eAllAffixData, CurrentItem );
	}
#endif

	CharacterData * playerData = PLAYERDATA;

	const DWORD									pdwInventory = 0x035EBB20;
	const DWORD									pdwStrength = pdwInventory + 0x39C4C;
	const DWORD									pdwSpirit = pdwInventory + 0x39C58;
	const DWORD									pdwTalent = pdwInventory + 0x39C54;
	const DWORD									pdwAgility = pdwInventory + 0x39C5C;
	const DWORD									pdwHealth = pdwInventory + 0x39C50;

	int iStrength = playerData->iStrength + ( *(int *)pdwStrength );
	int iSpirit = playerData->iSpirit + ( *(int *)pdwSpirit );
	int iTalent = playerData->iTalent + ( *(int *)pdwTalent );
	int iAgility = playerData->iAgility + ( *(int *)pdwAgility );
	int iHealth = playerData->iHealth + ( *(int *)pdwHealth );


	int iMinDamage = 1 + ( ( (iStrength)+130 ) / 130 ) + ( ( iTalent + iAgility ) / 40 );
	int iMaxDamage = 2 + ( ( iStrength + 130 ) / 130 ) + ( ( iTalent + iAgility ) / 40 );


	switch ( playerData->iClass )
	{
		case CHARACTERCLASS_Fighter:
		if ( iItemClass == WEAPONCLASS_Melee )
		{
#ifdef ITEMFIXES
			iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 520 ) / 520 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 520 ) / 520 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iStrength + 176 ) / 176 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iStrength + 176 ) / 176 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 260 ) / 260 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 400 ) / 400 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 520 ) / 520 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 260 ) / 260 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 400 ) / 400 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 520 ) / 520 ) ) - iWeaponDamage[1];
			}
#endif
			if ( iItemSpec == CLASSFLAG_Fighter )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Mechanician:
		if ( iItemClass == WEAPONCLASS_Melee || iItemClass == WEAPONCLASS_Ranged )
		{
			if ( iItemClass == WEAPONCLASS_Ranged )
			{
#ifdef ITEMFIXES
				iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 600 ) / 600 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 320 ) / 320 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
				iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 600 ) / 600 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 320 ) / 320 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
				if ( bNewStats )
				{
					iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iAgility + 202 ) / 202 ) ) - iWeaponDamage[0];
					iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iAgility + 202 ) / 202 ) ) - iWeaponDamage[1];
				}
				else
				{
					iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 560 ) / 560 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 600 ) / 600 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 560 ) / 560 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 320 ) / 320 ) ) - iWeaponDamage[0];
					iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 560 ) / 560 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 600 ) / 600 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 560 ) / 560 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 320 ) / 320 ) ) - iWeaponDamage[1];
				}
#endif
			}
			else
			{
#ifdef ITEMFIXES
				iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 320 ) / 320 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 600 ) / 600 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 560 ) / 560 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
				iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 320 ) / 320 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 600 ) / 600 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 560 ) / 560 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 560 ) / 560 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
				if ( bNewStats )
				{
					iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iStrength + 207 ) / 207 ) ) - iWeaponDamage[0];
					iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iStrength + 207 ) / 207 ) ) - iWeaponDamage[1];
				}
				else
				{
					iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 320 ) / 320 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 600 ) / 600 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 560 ) / 560 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 560 ) / 560 ) ) - iWeaponDamage[0];
					iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 320 ) / 320 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 600 ) / 600 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 560 ) / 560 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 560 ) / 560 ) ) - iWeaponDamage[1];
				}
#endif
			}

			if ( iItemSpec == CLASSFLAG_Mechanician )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Archer:
		if ( iItemClass == WEAPONCLASS_Ranged )
		{
#ifdef ITEMFIXES
			iMinDamage = 2 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 460 ) / 460 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 260 ) / 260 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 460 ) / 460 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 260 ) / 260 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				//int iEfficiency = GetStatEfficiency ( playerData, iAgility, saCharStatAlloc[playerData->iClass].iAgility );
				//int iAgilityMod = static_cast<int>( ceil ( iAgility * iEfficiency * 0.01f ) );

				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iAgility + 169 ) / 169 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iAgility + 169 ) / 169 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 2 + ( ( iWeaponDamage[0] * ( iStrength + 400 ) / 400 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 460 ) / 460 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 260 ) / 260 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 400 ) / 400 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 460 ) / 460 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 260 ) / 260 ) ) - iWeaponDamage[1];
			}
#endif



			if ( iItemSpec == CLASSFLAG_Archer )
				bAddSpecBonus = TRUE;

		}
		break;

		case CHARACTERCLASS_Pikeman:
		if ( iItemClass == WEAPONCLASS_Melee )
		{
#ifdef ITEMFIXES
			iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 420 ) / 420 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 400 ) / 400 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 420 ) / 420 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 400 ) / 400 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iStrength + 159 ) / 159 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iStrength + 159 ) / 159 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 240 ) / 240 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 480 ) / 480 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 420 ) / 420 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 400 ) / 400 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 240 ) / 240 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 480 ) / 480 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 420 ) / 420 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 400 ) / 400 ) ) - iWeaponDamage[1];
			}
#endif

			if ( iItemSpec == CLASSFLAG_Pikeman )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Atalanta:
		if ( iItemClass == WEAPONCLASS_Ranged )
		{
#ifdef ITEMFIXES
			iMinDamage = 2 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 460 ) / 460 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 260 ) / 260 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 460 ) / 460 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 260 ) / 260 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iAgility + 169 ) / 169 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iAgility + 169 ) / 169 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 2 + ( ( iWeaponDamage[0] * ( iStrength + 400 ) / 400 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 460 ) / 460 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 260 ) / 260 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 400 ) / 400 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 460 ) / 460 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 260 ) / 260 ) ) - iWeaponDamage[1];
			}
#endif

			if ( iItemSpec == CLASSFLAG_Atalanta )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Knight:
		if ( iItemClass == WEAPONCLASS_Melee )
		{
#ifdef ITEMFIXES
			iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 300 ) / 300 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 600 ) / 600 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 300 ) / 300 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 600 ) / 600 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iStrength + 166 ) / 166 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iStrength + 166 ) / 166 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 300 ) / 300 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 600 ) / 600 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 300 ) / 300 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 600 ) / 600 ) ) - iWeaponDamage[1];
			}
#endif

			if ( iItemSpec == CLASSFLAG_Knight )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Magician:
		if ( iItemClass == WEAPONCLASS_Magic )
		{
#ifdef ITEMFIXES
			iMinDamage = 2 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iSpirit + 176 ) / 176 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iSpirit + 176 ) / 176 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 2 + ( ( iWeaponDamage[0] * ( iStrength + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 260 ) / 260 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 260 ) / 260 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[1];
			}
#endif

			if ( iItemSpec == CLASSFLAG_Magician )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Priestess:
		if ( iItemClass == WEAPONCLASS_Magic )
		{
#ifdef ITEMFIXES
			iMinDamage = 2 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 400 ) / 400 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iSpirit + 162 ) / 162 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iSpirit + 162 ) / 162 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 2 + ( ( iWeaponDamage[0] * ( iStrength + 480 ) / 480 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 240 ) / 240 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 400 ) / 400 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 480 ) / 480 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 240 ) / 240 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 400 ) / 400 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[1];
			}
#endif
			if ( iItemSpec == CLASSFLAG_Priestess )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Assassin:
		if ( iItemClass == WEAPONCLASS_Melee )
		{
#ifdef ITEMFIXES
			iMinDamage = 1 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 420 ) / 420 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 400 ) / 400 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 240 ) / 240 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 480 ) / 480 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 420 ) / 420 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 400 ) / 400 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iStrength + 154 ) / 154 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iStrength + 154 ) / 154 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 1 + ( ( iWeaponDamage[0] * ( iStrength + 240 ) / 240 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 480 ) / 480 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 420 ) / 420 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 400 ) / 400 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 240 ) / 240 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 480 ) / 480 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 420 ) / 420 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 400 ) / 400 ) ) - iWeaponDamage[1];
			}
#endif
			if ( iItemSpec == CLASSFLAG_Assassin )
				bAddSpecBonus = TRUE;
		}
		break;

		case CHARACTERCLASS_Shaman:
		if ( iItemClass == WEAPONCLASS_Magic )
		{
#ifdef ITEMFIXES
			iMinDamage = 2 + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iStrength + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iSpirit + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[0] + eAllAffixData.iATKPow );
			iMaxDamage = 3 + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iStrength + 520 ) / 520 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iSpirit + 260 ) / 260 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iTalent + 440 ) / 440 ) ) + ( ( ( iWeaponDamage[1] + eAllAffixData.iATKPow ) * ( iAgility + 480 ) / 480 ) ) - ( iWeaponDamage[1] + eAllAffixData.iATKPow );
#else
			if ( bNewStats )
			{
				iMinDamage = 2 + ( iWeaponDamage[0] * 3 ) + ( ( iWeaponDamage[0] * ( iSpirit + 174 ) / 174 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( iWeaponDamage[1] * 3 ) + ( ( iWeaponDamage[1] * ( iSpirit + 174 ) / 174 ) ) - iWeaponDamage[1];
			}
			else
			{
				iMinDamage = 2 + ( ( iWeaponDamage[0] * ( iStrength + 520 ) / 520 ) ) + ( ( iWeaponDamage[0] * ( iSpirit + 260 ) / 260 ) ) + ( ( iWeaponDamage[0] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[0] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[0];
				iMaxDamage = 3 + ( ( iWeaponDamage[1] * ( iStrength + 520 ) / 520 ) ) + ( ( iWeaponDamage[1] * ( iSpirit + 260 ) / 260 ) ) + ( ( iWeaponDamage[1] * ( iTalent + 440 ) / 440 ) ) + ( ( iWeaponDamage[1] * ( iAgility + 480 ) / 480 ) ) - iWeaponDamage[1];
			}
#endif
			if ( iItemSpec == CLASSFLAG_Shaman )
				bAddSpecBonus = TRUE;
		}
		break;

		default:
		break;
	}


	if ( bAddSpecBonus )
	{
#ifdef ITEMFIXES
		iMinDamage += ( ( iWeaponDamage[0] + eAllAffixData.iATKPow ) + ( iWeaponDamage[1] + eAllAffixData.iATKPow ) ) / 16;
#else
	iMinDamage += ( iWeaponDamage[0] + iWeaponDamage[1] ) / 16;
#endif
	if ( pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv > 0 )
		iMaxDamage += playerData->iLevel / pItem[iItemIndex].sItem.sSpecData.iSpecAttackPowerDiv;
	}



	SKILLMANAGERHANDLER->SetWeaponDamageBuffFromSkill ( iSkillDamage, iWeaponDamage );

	//Add passive skill buffs
	iMinDamage += iSkillDamage[0];
	iMaxDamage += iSkillDamage[1];

	//Add extra damage if using matching spec based weapon
	iMinDamage += *iLevelDamage;
	iMaxDamage += *iLevelDamage;

#if defined (DEV_MODE) && defined (GM_MODE)
	if ( MAP_ID == MAPID_QuestArena )
	{
		int iAvg = ( iMinDamage + iMaxDamage ) / 2;
		playerData->iMinDamage = iAvg;
		playerData->iMaxDamage = iAvg;
	}
	else
	{
		playerData->iMinDamage = iMinDamage;
		playerData->iMaxDamage = iMaxDamage;
	}
#else
	playerData->iMinDamage = iMinDamage;
	playerData->iMaxDamage = iMaxDamage;
#endif

	//store base attack power
	CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin = playerData->iMinDamage;
	CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax = playerData->iMaxDamage;

	CharacterDataLocal * ps = NULL;

	__asm
	{
		PUSH EAX;
		MOV EAX, iWeaponDamage;
		SUB EAX, 0x34;
		MOV ps, EAX;
		POP EAX;
	}
#ifdef ITEMFIXES
	CGameCharacterStatus::GetInstance()->UpdateCharacterStatus( ps, eAllAffixData );
#else
	CGameCharacterStatus::GetInstance()->UpdateCharacterStatus( ps);
#endif
}

//int UnitGame::GetStatEfficiency (CharacterData* playerData, int iStat, int iCharStatAlloc)
//{
//	int iLevelCheck = playerData->iLevel;
//
//	if ( iLevelOverride > 0 )
//		iLevelCheck = iLevelOverride;
//
//	int iStatsToEfficientMax = static_cast<int>(ceil(iaCharTotalStats[iLevelCheck] * 0.01f * iStatsToMaxEfficiency));
//	int iCurrentStatMaxReq = static_cast<int>( ceil ( iCharStatAlloc * 0.01f * iaCharTotalStats[iLevelCheck]));
//	int iPeakEfficiency = iCurrentStatMaxReq + iStatsToEfficientMax;
//	int iLowestEff = iLowestEfficiency;
//	int iMaxEff = iMaxEfficiency;
//
//	int iEfficiency = 0;
//
//	if ( iStat < iPeakEfficiency ) 
//		iEfficiency = iLowestEff;
//	else
//	{
//		int iLowestPoint = iCurrentStatMaxReq + ( iStatsToEfficientMax * 2 );
//		int iEffTotal = 0;
//		int iEffCount = 0;
//
//		for ( UINT i = 0; i < iStat ; i++ )
//		{
//			if ( i < ( iCurrentStatMaxReq + iStatsToEfficientMax ) )
//				iEffTotal += iLowestEff;
//			else if ( ( i <= iPeakEfficiency ) && ( i < iLowestPoint ) )
//			{
//				int iCurrentX = i - iCurrentStatMaxReq - iStatsToEfficientMax;
//
//				iEffTotal += iLowestEff + ( 0.5f * ( cos ( ( iCurrentX + iStatsToEfficientMax ) / PI * 10 / iStatsToEfficientMax ) + 1 ) ) * ( iMaxEff - iLowestEff );
//			}
//			else
//				iEffTotal += iMaxEff;
//
//			iEffCount += 1;
//		}
//
//		iEfficiency = static_cast<int>( ceil ( iEffTotal / iEffCount ));
//	}
//
//
//	return iEfficiency;
//}

/// <summary>
/// Get attack chance
/// 10 means 90% chance to defend
/// 90 means 10% chance to defend
/// https://docs.google.com/spreadsheets/d/14BGEPwN2556vnvSMDnqgBuBqOxbb6qDXMCxARAF-9vk/edit#gid=619854393
/// </summary>
/// <param name="iDefenseRating">The defense chance of the monster</param>
/// <param name="iAttackRating">The attack rating of the player</param>
/// <param name="bApplyCaps">If true, apply min / max upper bounds (10 and 90).</param>
/// <returns></returns>
int UnitGame::PlayerAttackChance ( int iMonsterDefenceRating )
{
	const int baseline = 90; //90% accuracy is the base for players (if attack rating and defense is same)
	const int scalingAbove = 50;
	const int scalingBelow = 20;
	const int max = 100;
	const int min = 30;

	//if difference is +ve then monster's attack rating is higher
	//if difference is -ve then monster's attack rating is lower
	const int diff = UNITDATA->sCharacterData.iAttackRating - iMonsterDefenceRating;
	const int accuracy = baseline + ( diff / (diff > 0 ? scalingAbove : scalingBelow) );

	if ( accuracy > max )
		return max;

	if ( accuracy < min )
		return min;

	return accuracy;
}


void UnitGame::RenderDebugText()
{
	auto sCameraZoom = (CameraZoom *)0x00824FA4;
	int iCameraCircle =  *(int *)0x00A17630;

	const int iXDebug = 8;
	const int iYDebug = 4;
	int iYPos = 0;

	//Server Name
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Server Name: %s", (char*)( 0x038FEF40 + ( ( *(int*)0x4B06960 - 1 ) * 0x68 ) ) ), 0, -1 );
	iYPos += 15;

	//Server Version
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Server Version: %d", sServerInfo.iServerVersion ), 0, -1 );
	iYPos += 15;

	//Users Online
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Users Online: %d/%d", sServerInfo.iUsers, sServerInfo.iMaxUsers ), 0, -1 );
	iYPos += 15;

	//Boss Time
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Boss Time: xx:%02d", sServerInfo.iBossTime ), 0, -1 );
	iYPos += 30;

	//Account
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Account: %s", (char*)0x39033E8 ), 0, -1 );
	iYPos += 15;

	//Character Name
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Character Name: %s", (char*)*(UINT*)CHAR_DATA_PTR ), 0, -1 );
	iYPos += 15;

	//FPS
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "FPS: %d", *(int*)0x0A1755C ), 0, -1 );
	iYPos += 15;

	//X
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "X: %d [%d] AX(%d)", UNITDATA->sPosition.iX, UNITDATA->sPosition.iX >> 8, UNITDATA->sAngle.iX ), 0, -1 );
	iYPos += 15;

	//Y
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Y: %d [%d] AY(%d)", UNITDATA->sPosition.iY, UNITDATA->sPosition.iY >> 8, UNITDATA->sAngle.iY ), 0, -1 );
	iYPos += 15;

	//Z
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Z: %d [%d] AZ(%d)", UNITDATA->sPosition.iZ, UNITDATA->sPosition.iZ >> 8, UNITDATA->sAngle.iZ ), 0, -1 );
	iYPos += 15;

	//Stage Area
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Stage Area: %d", CHARACTERGAME->GetStageID() ), 0, -1 );
	iYPos += 30;


	//Camera turn
	FONTHANDLER->Draw ( 0, iXDebug, iYDebug + iYPos, FormatString ( "Camera turn: %d", iCameraCircle ), 0, -1 );
	iYPos += 15;
	//Camera angle
	FONTHANDLER->Draw ( 0, iXDebug, iYDebug + iYPos, FormatString ( "Camera angle: %d", sCameraZoom->iVerticalZoom ), 0, -1 );
	iYPos += 15;
	//Camera zoom
	FONTHANDLER->Draw ( 0, iXDebug, iYDebug + iYPos, FormatString ( "Camera zoom: %d", sCameraZoom->iHorizontalZoom ), 0, -1 );
	iYPos += 30;

	//Weapon
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Weapon ID: %04X (%d)", UNITDATA->eWeaponItemID, UNITDATA->bIsWeaponTwoHanded ), 0, -1 );
	iYPos += 15;

	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Weapon Equip Mode: %04X", UNITDATA->eToolEquipModeTown ), 0, -1 );
	iYPos += 30;

	//Animation
	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Animation ID: 0x%02X FrameStep(%d)", UNITDATA->iAction, UNITDATA->iFrameStep ), 0, -1 );
	iYPos += 15;

	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Animation Frame: %d", UNITDATA->iFrame ), 0, -1 );
	iYPos += 15;

	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Animation Type: 0x%02X", UNITDATA->psModelAnimation->iType ), 0, -1 );
	iYPos += 15;

	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "Enviroment Color: [%d][%d][%d]", READDWORD( 0x00CF4784 ), READDWORD( 0x00CF4788 ), READDWORD( 0x00CF478C ) ), 0, -1 );
	iYPos += 15;

	DWORD dwParticles = ((*(DWORD*)(0x039032CC + 0x0C)) - (*(DWORD*)(0x039032CC + 0x08))) / 0x08C;

	FONTHANDLER->Draw( 0, iXDebug, iYDebug + iYPos, FormatString( "# Particles: %d", dwParticles ), 0, -1 );
	iYPos += 15;

	struct GMSeeUnitInfo
	{
		char			  szName[64];
		int				  iTimes;
		BOOL			  bPlayer;
		BOOL			  bFriendly; //TRUE is blue else red
	};

	std::vector<GMSeeUnitInfo> vUnits;
	GMSeeUnitInfo * unit;
	GMSeeUnitInfo nunit;

	for( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * ud = pcaUnitData + i;

		if( ud->bActive )
		{
			bool bNew = true;
			for( std::vector<GMSeeUnitInfo>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
			{
				GMSeeUnitInfo * cur = &( *it );

				if( lstrcmpiA( ud->sCharacterData.szName, cur->szName ) == 0 )
				{
					unit = cur;
					bNew = false;
					break;
				}
			}

			if( bNew )
			{
				unit = &nunit;

				StringCchCopyA( unit->szName, _countof( unit->szName ), ud->sCharacterData.szName );
				unit->iTimes = 1;
				unit->bPlayer = FALSE;
				unit->bFriendly = FALSE;

				if( ud->sCharacterData.iType == CHARACTERTYPE_Monster )
				{
					if( ud->sCharacterData.iType == CHARACTERTYPE_Player )
						unit->bPlayer = TRUE;
				}
				else if( ud->sCharacterData.iType == CHARACTERTYPE_Player )
				{
					unit->bPlayer = TRUE;
					unit->bFriendly = TRUE;
				}
				else if( ud->sCharacterData.iType == CHARACTERTYPE_NPC )
					unit->bFriendly = TRUE;

				vUnits.push_back( *unit );
			}
			else
				unit->iTimes++;
		}
	}

	int iLeft = 0, iRight = 0;
	for( std::vector<GMSeeUnitInfo>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
	{
		GMSeeUnitInfo cur = *it;

		if( !cur.bPlayer )
		{
			if( cur.bFriendly )
				FONTHANDLER->Draw( 1, 350, 4 + ( iLeft++ * 15 ), FormatString( "%s x %d", cur.szName, cur.iTimes ), DT_WORDBREAK, D3DCOLOR_XRGB( 104, 202, 230 ) );
			else
				FONTHANDLER->Draw( 1, 350, 4 + ( iLeft++ * 15 ), FormatString( "%s x %d", cur.szName, cur.iTimes ), DT_WORDBREAK, D3DCOLOR_XRGB( 255, 128, 96 ) );
		}
		else
			FONTHANDLER->Draw( 1, 350 + 200, 4 + ( iRight++ * 15 ), FormatString( "%s x %d", cur.szName, cur.iTimes ), DT_WORDBREAK, -1 );
	}
}

void UnitGame::RenderCharacterDebugText()
{
	int iXDebug = RESOLUTION_WIDTH - 80;
	int iYDebug = *(int*)0x82BF08;

	const int iLabelPosition = 180;
	const int iValuePosition = 65;

	int iPaddingBottom = (RESOLUTION_HEIGHT >> 2) - 50;

	//Update UnitData Pointer
	if( iLastObjectID == 0 )
		pcViewUnitData = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );
	else if( *(DWORD*)( 0x207C888 + 0x10 ) != iLastObjectID )
		pcViewUnitData = UNITDATABYID( *(DWORD*)( 0x207C888 + 0x10 ) );

	if ( pcViewUnitData == NULL )
		return;

	UnitData * pcTargetView = (UnitData*)(0x0207C888);

	if ( pcTargetView->sRightHandTool.eItemID != iLastUnitDataViewItemID )
	{
		iLastUnitDataViewItemID = pcTargetView->sRightHandTool.eItemID;

		CopyMemory( pcTargetView, pcViewUnitData, sizeof( UnitData ) );

		pcTargetView->Animate( ANIMATIONTYPE_Idle );
	}

	//Level
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Level: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iLevel ), 0, -1 );
	iPaddingBottom += 15;

	//HP
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "HP: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d / %d", pcViewUnitData->GetCurrentHealth(), pcViewUnitData->GetMaxHealth() ), 0, -1 );
	iPaddingBottom += 15;

	//MP
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "MP: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d / %d", pcViewUnitData->sCharacterData.sMP.sCur, pcViewUnitData->sCharacterData.sMP.sMax ), 0, -1 );
	iPaddingBottom += 15;

	//SP
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "SP: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d / %d", pcViewUnitData->sCharacterData.sSP.sCur, pcViewUnitData->sCharacterData.sSP.sMax ), 0, -1 );
	iPaddingBottom += 15;

	//Defense
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Defense: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iDefenseRating ), 0, -1 );
	iPaddingBottom += 15;

	//fAbsorbRating
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Absorb: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iAbsorbRating ), 0, -1 );
	iPaddingBottom += 15;

	//Block
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Block: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iBlockRating ), 0, -1 );
	iPaddingBottom += 15;

	//sDamage
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Damage: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d - %d", pcViewUnitData->sCharacterData.iMinDamage, pcViewUnitData->sCharacterData.iMaxDamage ), 0, -1 );
	iPaddingBottom += 15;

	//Attack Rating
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Attack Rating: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iAttackRating ), 0, -1 );
	iPaddingBottom += 15;

	//Range
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Range: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iAttackRange / 256 ), 0, -1 );
	iPaddingBottom += 15;

	int iDistanceA = 0, iDistanceB = 0, iDistanceC = 0;
	pcViewUnitData->sPosition.GetPTDistanceXZ(&UNITDATA->sPosition, iDistanceA, iDistanceB, iDistanceC);

	//Range
	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Range 2 Mon: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("%d", (int)sqrt(iDistanceA)), 0, -1);
	iPaddingBottom += 15;

	//Attack fMovementSpeed
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Attack Speed: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iAttackSpeed ), 0, -1 );
	iPaddingBottom += 15;

	//Critical
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Critical: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iCritical ), 0, -1 );
	iPaddingBottom += 15;

	//Regen
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Regen: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%.2f - %.2f - %.2f", pcViewUnitData->sCharacterData.fHPRegen, pcViewUnitData->sCharacterData.fMPRegen, pcViewUnitData->sCharacterData.fSPRegen ), 0, -1 );
	iPaddingBottom += 15;

	//Movement fMovementSpeed
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Movement Speed: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iMovementSpeed ), 0, -1 );
	iPaddingBottom += 15;

	//iWeight
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Weight: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.sWeight ), 0, -1 );
	iPaddingBottom += 15;

	//Attributes
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Attributes: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d - %d - %d - %d - %d (%d)",
		pcViewUnitData->sCharacterData.iStrength, pcViewUnitData->sCharacterData.iSpirit, pcViewUnitData->sCharacterData.iTalent, pcViewUnitData->sCharacterData.iAgility, pcViewUnitData->sCharacterData.iHealth, pcViewUnitData->sCharacterData.iStatPoints ), 0, -1 );
	iPaddingBottom += 15;

	//Gold
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Gold: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%s gp", FormatNumber(pcViewUnitData->sCharacterData.iGold) ), 0, -1 );
	iPaddingBottom += 15;

	//EXP
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "EXP: ", 0, -1 );
	LARGE_INTEGER li;
	li.LowPart = pcViewUnitData->sCharacterData.iCurrentExpLow;
	li.HighPart = pcViewUnitData->sCharacterData.iCurrentExpHigh;
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%s", FormatNumber(li.QuadPart) ), 0, -1 );
	iPaddingBottom += 15;

	//Unique ID
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Unique ID: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iID ), 0, -1 );
	iPaddingBottom += 15;

	//Clan ID
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Clan ID: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->sCharacterData.iClanID ), 0, -1 );
	iPaddingBottom += 15;

	//Clan ID
	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Bellatra Crown: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("%d", pcViewUnitData->iBellatraCrown), 0, -1);
	iPaddingBottom += 15;

	//Type
	std::string strType;

	if( pcViewUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
		strType = "[Monster]";
	else if( pcViewUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		strType = "[NPC]";
	else
		strType = "[Player]";

	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Type: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, strType, 0, -1 );
	iPaddingBottom += 15;

	//Animation ID
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Active Skill ID: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "0x%02X", pcViewUnitData->sActiveSkill.cSkillTinyID), 0, -1 );
	iPaddingBottom += 15;

	//Animation ID
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Animation ID: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "0x%02X", pcViewUnitData->iAction), 0, -1 );
	iPaddingBottom += 15;

	//Animation Type
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Animation Type: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "0x%02X", pcViewUnitData->GetAnimation() ), 0, -1 );
	iPaddingBottom += 15;

	//FPS
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "FPS: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->iFrameStep ), 0, -1 );
	iPaddingBottom += 15;

	//Animation Frame
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Animation Frame: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "%d", pcViewUnitData->iFrame ), 0, -1 );
	iPaddingBottom += 15;


	/*FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Last Damage Time: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("%d", pcViewUnitData->dwLastDamageTime == 0 ? 0 : (pcViewUnitData->dwLastDamageTime - TICKCOUNT)), 0, -1);
	iPaddingBottom += 15;


	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Is Attacking: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("%d", pcViewUnitData->bMonsterAttacking), 0, -1);
	iPaddingBottom += 15;


	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Next Anim Type: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("0x%02X", pcViewUnitData->eNextAnimationType), 0, -1);
	iPaddingBottom += 15;

	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Active skill: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("0x%02X", pcViewUnitData->sActiveSkill.iValue), 0, -1);
	iPaddingBottom += 15;

	FONTHANDLER->Draw(0, iXDebug - iLabelPosition, iPaddingBottom, "Frame timer: ", 0, -1);
	FONTHANDLER->Draw(0, iXDebug - iValuePosition, iPaddingBottom, FormatString("%d", pcViewUnitData->bMonsterAttacking), 0, -1);
	iPaddingBottom += 15;*/

	//Position
	FONTHANDLER->Draw( 0, iXDebug - iLabelPosition, iPaddingBottom, "Position: ", 0, -1 );
	FONTHANDLER->Draw( 0, iXDebug - iValuePosition, iPaddingBottom, FormatString( "X [%d (%d)]\nY [%d(%d)]\nZ [%d(%d)]",
		pcViewUnitData->sPosition.iX, pcViewUnitData->sAngle.iX, pcViewUnitData->sPosition.iY, pcViewUnitData->sAngle.iY, pcViewUnitData->sPosition.iZ, pcViewUnitData->sAngle.iZ ), 0, -1 );
	iPaddingBottom += 45;

	iLastObjectID = *(DWORD*)( 0x207C888 + 0x10 );
}


void UnitGame::SendUnitDataEx(BOOL bForceUpdate)
{
	//Prepare Packet
	PacketPlayDataEx sPacket;
	ZeroMemory( &sPacket, sizeof( PacketPlayDataEx ) );
	sPacket.iLength = sizeof( PacketPlayDataEx );
	sPacket.iHeader = PKTHDR_PlayDataEx;
	sPacket.bGameMaster = GM_MODE;
	sPacket.iMapID = MAP_ID;

	// Skills levels
	for( int i = 0; i < SKILL_COUNT_CHARACTER; i++ )
		sPacket.iLevelSkill[i] = SKILLSCHARACTER[i + 1].iLevel;

	// Mac Address
	{
		UUID uuid;

		// Ask OS to create UUID
		UuidCreateSequential( &uuid );

		// Bytes 2 through 7 inclusive
		sprintf_s( sPacket.szMacAddr, 20, "%02X-%02X-%02X-%02X-%02X-%02X",
			uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7] );
	}

	// Player Data
	CopyMemory( &sPacket.sCharacterData, &UNITDATA->sCharacterData.iStrength, sizeof( CharacterDataPacket ) );
	sPacket.iLevelCharacter = UNITDATA->sCharacterData.iLevel;
	sPacket.iCritical = UNITDATA->sCharacterData.iCritical;

	// ServerID
	sPacket.dwServerID = ( *(DWORD*)0x04B06960 );

	// Quest Log
	sPacket.dwQuestLevelLog = PLAYER_QUESTLOG;

	sPacket.iBellatraCrown = (*(DWORD*)0x03B18BC8 );

	//Bellatra Crown
	//Log("SendUnitDataEx - sPacket.iBellatraCrown = %d", sPacket.iBellatraCrown);

	//HD Serial
	{
		char szHD[4];

		szHD[0] = 'C';
		szHD[1] = ':';
		szHD[2] = '\\';
		szHD[3] = 0;

		DWORD dw = 0;

		GetVolumeInformationA( szHD, NULL, 0, &dw, NULL, NULL, NULL, 0 );

		sPacket.uSerialHD = dw;
	}

	sPacket.userID = UNITDATA->iID;

	if( iSpeedHack <= 3 )
	{
		sPacket.dwSpeed = 0;
		iSpeedHack++;
	}
	else
		sPacket.dwSpeed = 0;

	// Gold
	sPacket.dwGold = UNITDATA->sCharacterData.iGold;

	if( dwUpdatePacket < GetCurrentTime() || CHARACTERGAME->GetStageID() == MAPID_BlessCastle || bForceUpdate )
	{
		dwUpdatePacket = GetCurrentTime() + 5000;
		SENDPACKETG( &sPacket );
	}

	auto settings = SETTINGSHANDLER->GetModel()->GetSettings();

	//Normal...
	PacketPlayData * psPacketPlay = (PacketPlayData*)UNITDATA->baUnitInfo;
	psPacketPlay->sMP.sCur = CHARACTERGAME->GetCurrentMP();
	psPacketPlay->sMP.sMax = UNITDATA->sCharacterData.sMP.sMax;
	psPacketPlay->sSP.sCur = CHARACTERGAME->GetCurrentSP();
	psPacketPlay->sSP.sMax = UNITDATA->sCharacterData.sSP.sMax;
	psPacketPlay->bPartyLeader = UNIT->bPartyLeader;
	psPacketPlay->saSpeed[0] = UNITDATA->sCharacterData.iAttackSpeed;
	psPacketPlay->saSpeed[1] = UNITDATA->sCharacterData.iMovementSpeed;
	psPacketPlay->sHPLong.iMin = CHARACTERGAME->GetCurrentHP();
	psPacketPlay->sHPLong.iMax = UNITDATA->sCharacterData.sHP.sMax;
	psPacketPlay->sAngleY = UNITDATA->sAngle.iY;
	psPacketPlay->sSize = UNITDATA->sCharacterData.sSize;
	psPacketPlay->bInvisibleInMiniMap = UNIT->bInvisibleInMiniMap;
	psPacketPlay->bLootFilterEnabled = settings.bEnableLootFilter;
	psPacketPlay->iLootFilterFlag = settings.iLootFilterFlags;
	psPacketPlay->sEquipmentMinLevel = settings.bEnableEquipmentMinLevelFilter ? static_cast<short>( settings.iMinEquipmentLevelReq ) : 0;


	int iSecondMapIndex = (UNITDATA->iLoadedMapIndex == 0) ? 1 : 0;
	psPacketPlay->sSecondMapID = -1;
	if ( FIELDGAME[iSecondMapIndex] )
		psPacketPlay->sSecondMapID = FIELDGAME[iSecondMapIndex]->iMapID;
}

NAKED void UnitGame::UpdateObjectID( ID lID )
{
	JMP( pfnUpdateObjectID );
}

#define BLURITEM_MAX 32

EXEVertex sVertexBlur[(BLURITEM_MAX * 2) * 3];
EXEFace sFaceBlur[BLURITEM_MAX * 2];

BOOL UnitGame::RenderItemMotionBlur( UnitData * pcUnitData, UnitTool * psHand )
{
	if ( !pcUnitData->bVisible || !pcUnitData->eWeaponItemID )
		return FALSE;

	if( (pcUnitData->sLeftHandTool.eItemID & 0xFFFF0000) == ITEMTYPE_Shield )
		return FALSE;

	if ( pcUnitData != UNITDATA )
	{
		if ( READDWORD( 0x0A1755C ) <= 45 )
			return FALSE;
	}

	const int iTotalFrame	= 80;

	int iCos = 0, iSin = 0;

	//Position
	Point3D sTop( 0, 0, 0 );
	Point3D sBottom( 0, 0, psHand->iSizeMax );

	//Angle
	Point3D sAngle	= pcUnitData->sAngle;
	sAngle.iY		= (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

	static BOOL bHandleFaces = FALSE;

	if ( bHandleFaces == FALSE )
	{
		USHORT iV = 0;
		int iB = 0;

		EXETextureLink * psa = ((EXETextureLink*)0x00A1BCB8);

		for ( int i = 0; i < BLURITEM_MAX; i++ )
		{
			sFaceBlur[iB].pcTextureLink = &psa[i % 128];
			sFaceBlur[iB].uVertex1	= iV;
			sFaceBlur[iB].uVertex2	= iV+1;
			sFaceBlur[iB].uVertex3	= iV+2;
			sFaceBlur[iB].uMatID	= (USHORT)READDWORD( 0x00A1CCBC );
			iB++;

			sFaceBlur[iB].pcTextureLink = &psa[i % 128];
			sFaceBlur[iB].uVertex1	= iV+2;
			sFaceBlur[iB].uVertex2	= iV+1;
			sFaceBlur[iB].uVertex3	= iV+3;
			sFaceBlur[iB].uMatID	= (USHORT)READDWORD( 0x00A1CCBC );
			iB++;

			iV += 2;
		}

		bHandleFaces = TRUE;
	}

	//Vertex and Face
	EXEVertex * psVertex	= sVertexBlur;
	EXEFace * psFace		= sFaceBlur;

	int iLevelBlurO		= BLURITEM_MAX << 1;
	int iSizeLevelBlur	= sBottom.iZ / (BLURITEM_MAX << 1);

	//Update position if weapon is Two Hand
	if ( pcUnitData->bIsWeaponTwoHanded )
	{
		int iAngle	= (-PTANGLE_45 / 5) & PTANGLE_Mask;

		iSin		= PTSEN[iAngle] >> 8;
		iCos		= PTCOS[iAngle] >> 8;

		sTop.iY		= -(sTop.iZ * iSin) >> 8;
		sTop.iZ		= (sTop.iZ * iCos) >> 8;

		sBottom.iY	= -(sBottom.iZ * iSin) >> 8;
		sBottom.iZ	= (sBottom.iZ * iCos) >> 8;
	}

	//Using Skill?
	if ( ACTIONMODE )
	{
		//Update Angle
		if ( sAngle.iY > PTANGLE_180 )
			sAngle.iY = PTANGLE_270;
		else
			sAngle.iY = PTANGLE_90;
	}

	if ( psHand->pModel && psHand->pBone )
	{
		EXEMatrixI & sMatrix = psHand->pBone->sWorld;

		for ( int i = 0; i < BLURITEM_MAX; i++ )
		{
			int iFrame = pcUnitData->iFrame - (i * iTotalFrame >> 2);

			if ( iFrame < pcUnitData->psModelAnimation->iBeginFrame )
				iFrame = pcUnitData->psModelAnimation->iBeginFrame;

			AnimateMeshTreeNew( psHand->pBone, iFrame, sAngle.iX, sAngle.iY, sAngle.iZ );

			int iVertexCount = i << 1;

			Point3D sRadian;
			Point3D sPositionOut;

			//Get Top Position
			{
				sPositionOut = sTop;

				//Radian
				sRadian.iX = sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
				sRadian.iY = sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
				sRadian.iZ = sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

				//Get Weapon Position
				psVertex[iVertexCount].sPosition.iX = (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
				psVertex[iVertexCount].sPosition.iZ = (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
				psVertex[iVertexCount].sPosition.iY = (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);
			}

			iVertexCount++;

			//Get Bottom Position
			{
				sPositionOut = sBottom;

				//Radian
				sRadian.iX = sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
				sRadian.iY = sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
				sRadian.iZ = sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

				//Get Weapon Position
				psVertex[iVertexCount].sPosition.iX = (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
				psVertex[iVertexCount].sPosition.iZ = (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
				psVertex[iVertexCount].sPosition.iY = (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);
			}

			//Update Position
			sTop.iZ		+= iSizeLevelBlur;
			sBottom.iZ	-= iSizeLevelBlur;
		}
	}

	RENDERER->Init();

	RENDERER->SetCameraPosition( UNITTARGETPOSITION_X, UNITTARGETPOSITION_Y, UNITTARGETPOSITION_Z, UNITTARGETANGLE_X, UNITTARGETANGLE_Y, UNITTARGETANGLE_Z );

	//Add Vertex to the render
	for ( int i = 0; i < iLevelBlurO; i++ )
		RENDERER->AddObjectVertex( &psVertex[i] );

	//Add Faces to the render
	for ( int i = 0; i < (iLevelBlurO - 2); i += 2 )
	{
		EXEVertex * v0 = &psVertex[i];
		EXEVertex * v1 = &psVertex[i + 1];
		EXEVertex * v2 = &psVertex[i + 2];
		EXEVertex * v3 = &psVertex[i + 3];

		if ( v0->sPosition.iX != v2->sPosition.iX || v0->sPosition.iY != v2->sPosition.iY || v0->sPosition.iZ != v2->sPosition.iZ ||
			v1->sPosition.iX != v3->sPosition.iX || v1->sPosition.iY != v3->sPosition.iY || v1->sPosition.iZ != v3->sPosition.iZ )
		{
			RENDERER->AddObjectFace( &psFace[i] );
			RENDERER->AddObjectFace( &psFace[i + 1] );
		}
	}

	int r, g, b;

	r = RENDERER->r;
	g = RENDERER->g;
	b = RENDERER->b;

	RENDERER->r -= 64;
	RENDERER->g -= 64;
	RENDERER->b += 256;

	BOOL bPass = FALSE;

	if ( pcUnitData->sActiveSkill.iValue != 0 )
	{
		switch ( pcUnitData->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Fighter:
				switch ( pcUnitData->sActiveSkill.cSkillTinyID )
				{
					case SKILLPLAYID_T11:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T12:
						RENDERER->r += 256;
						RENDERER->g += 256;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T22:
						RENDERER->r -= 64;
						RENDERER->g += 256;
						RENDERER->b += 128;
						bPass = TRUE;
						break;

					default:
						break;
				}
				break;

			case CHARACTERCLASS_Pikeman:
				switch ( pcUnitData->sActiveSkill.cSkillTinyID )
				{
					case SKILLPLAYID_T12:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b += 256;
						bPass = TRUE;
						break;

					case SKILLPLAYID_T33:
						RENDERER->r += 256;
						RENDERER->g -= 64;
						RENDERER->b -= 64;
						bPass = TRUE;
						break;

					default:
						break;
				}
				break;

			default:
				break;
		}
	}

	if ( !bPass && psHand->iBlinkSpeed )
	{
		RENDERER->r += psHand->saBlinkColor[2];
		RENDERER->g += psHand->saBlinkColor[1];
		RENDERER->b += psHand->saBlinkColor[0];
	}

	if ( RENDERER->iFaceCount )
	{
		RENDERER->SetRenderClip( 0x3F );
		RENDERER->ClipFaceRender();
		RENDERER->Vertex2DGeometry( 0 );
		RENDERER->Render();
	}

	RENDERER->r = r;
	RENDERER->g = g;
	RENDERER->b = b;

	return TRUE;
}

BOOL UnitGame::IsBlockedRangeTarget()
{
	UnitData * pcUnitData = (*(UnitData**)0x0CF48D0);

	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
			return TRUE;
	}

	return FALSE;
}


BOOL UnitGame::SetPVPArea( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	int iMapPVPID = READDWORD( 0x03A975F8 );

	int iMapType = UNITDATA->iLoadedMapIndex >= 0 ? FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType : -1;
	int iMapID = UNITDATA->iLoadedMapIndex >= 0 ? FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID : -1;

	if ( UNITDATA->iLoadedMapIndex >= 0 &&
		(iMapType == MAPTYPE_BlessCastle && (int)READDWORD( 0x008250CC ) > UNITDATA->sPosition.iZ) ||
		(iMapType == MAPTYPE_BlessCastle && BLESSCASTLEHANDLER->InSiegeWar() && BLESSCASTLEHANDLER->GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) ||
		(iMapID == iMapPVPID && READDWORD( 0x03A975F4 ) > TICKCOUNT && pcUnitData->PartyFlag == 0) )
	{
		if ( pcUnitData->PkMode_CharState == CHARACTERTYPE_None &&
			 pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player &&
			(pcUnitData->sCharacterData.iClanID == 0 || pcUnitData->sCharacterData.iClanID != UNITDATA->sCharacterData.iClanID) )
		{
			if ( iMapID != iMapPVPID && pcUnitData->PartyFlag == 0 )
			{
				pcUnitData->PkMode_CharState = pcUnitData->sCharacterData.iType;
				pcUnitData->sCharacterData.iType = CHARACTERTYPE_Monster;
			}
		}
	}
	else
	{
		//no longer in PVP, restore character start
		if ( pcUnitData->PkMode_CharState )
		{
			pcUnitData->sCharacterData.iType = pcUnitData->PkMode_CharState;
			pcUnitData->PkMode_CharState = CHARACTERTYPE_None;
		}
	}

	return bRet;
}

BOOL UnitGame::Fun()
{
	int i = (rand() % 8192) * 2;

	DWORD dw = CALL_WITH_ARG1( 0x005C1B80, i );

	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750), (dw >> 24) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 500, (dw >> 16) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 1000, (dw >> 8) & 0xFF, 0, 0, (DWORD)UNITDATA );
	CALL_WITH_ARG6( 0x0045CFB0, 0x12000010, (*(int*)0x00CF4750) + 1500, dw & 0xFF, 0, 0, (DWORD)UNITDATA );

	UNITDATA->eNextMotionCode = ANIMATIONTYPE_Yahoo;

	CALL_WITH_ARG4( 0x00623390, 0x160, i, 0, 0 );

	return TRUE;
}

void UnitGame::MoveUnit( UnitData * pcUnitData, int iSpeed )
{
	CALLT_WITH_ARG1( 0x0044FFB4, (DWORD)pcUnitData, iSpeed );
}

void UnitGame::HandlePacket( PacketSaveData * psPacket )
{
	static int iCurrentBlock = 0;

	if ( psPacket->iCount > (iCurrentBlock) )
	{
		DISCONNECT_TYPE = 0;
		DISCONNECT_TIME = TRUE;
	}
	else
		iCurrentBlock++;

}

/// <summary>
/// Received misc command about this unit from the server
/// See UserServer::SendUserMiscCommandToOtherNearbyPlayers
/// </summary>
/// <param name="pcUnitData">The unit data in which this command is for</param>
/// <param name="eCommand">The command</param>
/// <param name="iArg1">Optional arg #1</param>
/// <param name="iArg2">Optional arg #2</param>
void UnitGame::HandlePacketCommand ( PacketCommand * psPacket)
{
	if ( psPacket->iHeader == PKTHDR_MiscUnitCommand )
	{
		EUnitDataMiscCommand eCommand = (EUnitDataMiscCommand)psPacket->p1;
		UnitData * pcUnitData = UNITDATABYID ( psPacket->p2 );

		if ( pcUnitData )
		{
			//DEBUGINFO ( "PKTHDR_MiscUnitCommand received!" );

			int iArg1 = psPacket->p3;
			int iArg2 = psPacket->p4;

			switch ( eCommand )
			{
				case EUnitDataMiscCommand::COMMANDID_ShowSuccessAgingOrMixSkillAnimation:
				{
					PLAYMINISOUND (25, SOUNDHANDLER->GetDistanceVolume ( pcUnitData->sPosition ) );
					SKILLMANAGERHANDLER->StartSkillEffectOld ( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 5001 ); //5001 = EFFECTID_AGING

				}
			}
		}
		else
		{
			//DEBUGINFO ( "Unit id not found: %d", psPacket->p2 );
		}
	}
}

void UnitGame::LoadParticles ()
{
	iParticleAura1st = PARTICLE->Load( "game\\scripts\\particles\\PlayerRank1.part" );
	iParticleAura2nd = PARTICLE->Load( "game\\scripts\\particles\\PlayerRank2.part" );
	iParticleAura3rd = PARTICLE->Load( "game\\scripts\\particles\\PlayerRank3.part" );
	iParticleAuraOverall = PARTICLE->Load("game\\scripts\\particles\\PlayerRankOverall.part");
	iLegendaryDropParticle = PARTICLE->Load( "game\\scripts\\particles\\Legendary_Drop.part" );
	iEpicDropParticle = PARTICLE->Load( "game\\scripts\\particles\\Epic_Drop.part" );
	iRareDropParticle = PARTICLE->Load( "game\\scripts\\particles\\Rare_Drop.part" );
}

/// <summary>
/// Handle PacketUserEffect for self
/// </summary>
void UnitGame::HandlePacket ( PacketUserEffect * psPacket )
{
	UNIT->sTitleRarity = psPacket->sTitleRarity;
	UNIT->sPlayerAuraId = psPacket->sPlayerAuraId;
	UNIT->sDialogSkinId = psPacket->sDialogSkinId;

	//CHATBOX->AddDebugMessage ( FormatString ( "UNIT->sPlayerAuraId = %d", UNIT->sPlayerAuraId) );

	SetPlayerRankingAura ( UNIT, UNIT->sPlayerAuraId );

	if ( UNIT->sTitleRarity )
	{
		STRINGCOPY ( UNIT->szTitle, psPacket->szTitle );
	}
	else
	{
		UNIT->szTitle[0] = { 0 };
	}
}

void UnitGame::SetPlayerRankingAura ( Unit * pcUnit, short sAuraRankingId )
{
	if ( sAuraRankingId == 1 )
	{
		if ( PARTICLE->IsRunning ( pcUnit->GetID (), iParticleAura1st ) )
		{
			PARTICLE->Update ( pcUnit->GetID (), iParticleAura1st, 70.0f );
		}
		else
		{
			int iID = PARTICLE->Start( iParticleAura1st,
				pcUnit->pcUnitData->sPosition.iX,
				pcUnit->pcUnitData->sPosition.iY - (100 << 8) + pcUnit->pcUnitData->iModelHeight,
				pcUnit->pcUnitData->sPosition.iZ );

			//CHATBOX->AddMessage ( FormatString ( "1st Start ID = %d", iID ) );
			PARTICLE->AddScript( iID, iParticleAura1st, pcUnit->pcUnitData, 70.0f, TRUE );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura2nd );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura3rd );
			PARTICLE->Delete(pcUnit->GetID(), iParticleAuraOverall);
		}
	}
	else if ( sAuraRankingId == 2 )
	{
		if ( PARTICLE->IsRunning ( pcUnit->GetID (), iParticleAura2nd ) )
		{
			PARTICLE->Update ( pcUnit->GetID (), iParticleAura2nd, 70.0f );
		}
		else
		{
			int iID = PARTICLE->Start ( iParticleAura2nd,
				pcUnit->pcUnitData->sPosition.iX,
				pcUnit->pcUnitData->sPosition.iY - (100 << 8) + pcUnit->pcUnitData->iModelHeight,
				pcUnit->pcUnitData->sPosition.iZ );

			//CHATBOX->AddMessage ( FormatString ( "2nd Start ID = %d", iID ) );
			PARTICLE->AddScript ( iID, iParticleAura2nd, pcUnit->pcUnitData, 70.0f, TRUE );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura1st );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura3rd );
			PARTICLE->Delete(pcUnit->GetID(), iParticleAuraOverall);
		}
	}
	else if ( sAuraRankingId == 3 )
	{
		if ( PARTICLE->IsRunning ( pcUnit->GetID (), iParticleAura3rd ) )
		{
			PARTICLE->Update ( pcUnit->GetID (), iParticleAura3rd, 70.0f );
		}
		else
		{
			int iID = PARTICLE->Start ( iParticleAura3rd,
				pcUnit->pcUnitData->sPosition.iX,
				pcUnit->pcUnitData->sPosition.iY - (100 << 8) + pcUnit->pcUnitData->iModelHeight,
				pcUnit->pcUnitData->sPosition.iZ );

		//	CHATBOX->AddMessage ( FormatString ( "3nd Start ID = %d", iID ) );

			PARTICLE->AddScript ( iID, iParticleAura3rd, pcUnit->pcUnitData, 70.0f, TRUE );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura1st );
			PARTICLE->Delete ( pcUnit->GetID (), iParticleAura2nd );
			PARTICLE->Delete(pcUnit->GetID(), iParticleAuraOverall);
		}
	}
	else if (sAuraRankingId == 4)
	{
		if (PARTICLE->IsRunning(pcUnit->GetID(), iParticleAuraOverall))
		{
			PARTICLE->Update(pcUnit->GetID(), iParticleAuraOverall, 70.0f);
		}
		else
		{
			int iID = PARTICLE->Start(iParticleAuraOverall,
				pcUnit->pcUnitData->sPosition.iX,
				pcUnit->pcUnitData->sPosition.iY - (100 << 8) + pcUnit->pcUnitData->iModelHeight,
				pcUnit->pcUnitData->sPosition.iZ);

			//	CHATBOX->AddMessage ( FormatString ( "3nd Start ID = %d", iID ) );

			PARTICLE->AddScript(iID, iParticleAuraOverall, pcUnit->pcUnitData, 70.0f, TRUE);
			PARTICLE->Delete(pcUnit->GetID(), iParticleAura1st);
			PARTICLE->Delete(pcUnit->GetID(), iParticleAura2nd);
			PARTICLE->Delete(pcUnit->GetID(), iParticleAura3rd);
		}
	}
	else
	{
		//remove existing particle from unit if any
		PARTICLE->Delete ( pcUnit->GetID (), iParticleAura1st );
		PARTICLE->Delete ( pcUnit->GetID (), iParticleAura2nd );
		PARTICLE->Delete ( pcUnit->GetID (), iParticleAura3rd );
		PARTICLE->Delete(pcUnit->GetID(), iParticleAuraOverall);
	}
}

/// <summary>
/// Handle PacketUnitStatus for other units
/// </summary>
void UnitGame::HandlePacketUnitStatus( UnitData * pcUnitData, Packet * psPacket )
{
	switch (psPacket->iHeader)
	{
		//this update is received frequently
		case PKTHDR_Effect_User:
		{
			auto pcUserEffect = reinterpret_cast<PacketUserEffect*>(psPacket);
			Unit * pcUnit1 = UNITDATATOUNIT ( UNITDATABYID ( pcUserEffect->iUniqueUserID ) );
			if ( pcUnit1 )
			{
				pcUnit1->sTitleRarity = pcUserEffect->sTitleRarity;
				pcUnit1->sPlayerAuraId = pcUserEffect->sPlayerAuraId;
				pcUnit1->sDialogSkinId = pcUserEffect->sDialogSkinId;

				SetPlayerRankingAura ( pcUnit1, pcUnit1->sPlayerAuraId );

				if ( pcUnit1->sTitleRarity )
				{
					STRINGCOPY ( pcUnit1->szTitle, pcUserEffect->szTitle );
				}
				else
				{
					pcUnit1->szTitle[0] = { 0 };
				}
			}

			break;
		}

		case PKTHDR_UnitStatusMove:
		{
			pcUnitData->sCharacterData.sMapID = ( reinterpret_cast<PacketUnitStatusMove *>( psPacket ) )->sMapID;

			CLogger::GetLogger()->Log( "Received move data: %d, map id: %d", ( reinterpret_cast<PacketUnitStatusMove *>( psPacket ) )->lID, pcUnitData->sCharacterData.sMapID );
			CLogger::GetLogger()->Flush();
		}
		break;

		case PKTHDR_ExtraUserData:
		{
			auto pcExtraUserInfo = reinterpret_cast<PacketExtraUserInfo*>(psPacket);
			UnitData* pcUnitData1 = UNITDATABYID(pcExtraUserInfo->iUniqueUserID);
			Unit * pcUnit1 = UNITDATATOUNIT( pcUnitData1 );
			if (pcUnitData1 && pcUnit1)
			{
				pcUnit1->sBellatraSoloCrown = pcExtraUserInfo->sBellatraSoloCrown;

				if ( pcUnitData1->eForceOrbItemID != EItemID::ITEMID_None )
				{
					//disable render in town for other players (Saves FPS)
					if ( MAP_ID != MAPID_RicartenTown &&
						 MAP_ID != MAPID_PillaiTown)
					{
						DWORD forceIndex0Based = ( ( pcUnitData1->eForceOrbItemID & 0xFFFF ) >> 8 ) - 1; //0 = Lucidy force and so on

						if ( forceIndex0Based >= 0 && forceIndex0Based <= 13 )
						{
							//See AssaParticle_ShelltomWeapon in leaked code
							typedef DWORD( __cdecl * tfnAssaParticleShelltomWeapon )( UnitData * pcUnitData, int iTime, int iCode );
							tfnAssaParticleShelltomWeapon fnAssaParticleShelltomWeapon = (tfnAssaParticleShelltomWeapon)0x005C16B0;

							fnAssaParticleShelltomWeapon( pcUnitData1, 16 * 70, forceIndex0Based );
						}
					}
				}
			}
			return;
		}
	}

	PacketPlayData * psPacketPlayData = (PacketPlayData*)psPacket;

	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	switch ( psPacket->iHeader )
	{
		//this is for users and monsters
		case PKTHDR_PlayDataChar:

			//if ( pcUnitData->PkMode_CharState == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
			//{
			//	UNITGAME->Log( "PlayDataChar received for unit: %s", pcUnit->GetName() );
			//	UNITGAME->Log( "psPacketPlayData->iPlayBuffCount: %d", psPacketPlayData->iPlayBuffCount );
			//
			//	psPacketPlayData->iPlayBuffCount = 0;
			//}

			pcUnitData->dwLastActiveTime = TICKCOUNT;

			pcUnitData->sCharacterData.sMP		= psPacketPlayData->sMP;
			pcUnitData->sCharacterData.sSP		= psPacketPlayData->sSP;

			pcUnitData->sCharacterData.sMapID   = psPacketPlayData->sMapID;

			if ( pcUnitData->IsPlayer() )
			{
				pcUnitData->sCharacterData.iAttackSpeed = psPacketPlayData->saSpeed[0];
				pcUnitData->sCharacterData.iMovementSpeed = psPacketPlayData->saSpeed[1];

				//pcUnitData->sAngle.iY = psPacketPlayData->sAngleY % 4096;
				pcUnitData->sCharacterData.sSize = psPacketPlayData->sSize;
				pcUnitData->sCharacterData.iLevel = psPacketPlayData->iLevel;

				pcUnit->bInvisibleInMiniMap = psPacketPlayData->bInvisibleInMiniMap;
				pcUnit->bPartyLeader = psPacketPlayData->bPartyLeader;

				if ( psPacketPlayData->sHPLong.iMax == 0 )
				{
					pcUnit->sHPLong.iMin = psPacketPlayData->sHp[0];
					pcUnit->sHPLong.iMax = psPacketPlayData->sHp[1];
				}
				else
				{
					pcUnit->sHPLong = psPacketPlayData->sHPLong;
				}

			}
			else if ( pcUnitData->IsNPC() )
			{
				pcUnit->sHPLong.iMin = 100;
				pcUnit->sHPLong.iMax = 100;
				pcUnitData->sCharacterData.sHP.sCur = 100;
				pcUnitData->sCharacterData.sHP.sMax = 100;
			}
			else if ( pcUnitData->IsSummon() )
			{
				pcUnit->sHPLong.iMin = psPacketPlayData->sHp[0];
				pcUnit->sHPLong.iMax = psPacketPlayData->sHp[1];
				pcUnitData->sCharacterData.sHP.sCur = psPacketPlayData->sHp[0];
				pcUnitData->sCharacterData.sHP.sMax = psPacketPlayData->sHp[1];
			}
			else
			{
				pcUnit->sHPLong = psPacketPlayData->sHPLong;

				pcUnitData->sCharacterData.sHP.sCur = MONSTER_HP_OVERRIDE;
				pcUnitData->sCharacterData.sHP.sMax = MONSTER_HP_OVERRIDE;
			}


			/*UNITGAME->Log("PKTHDR_PlayData bEventInfo [%d][%d][%d][%d] updateInfo [%d][%d][%d][%d]",
				psPacketPlayData->bEventInfo[0], psPacketPlayData->bEventInfo[1], psPacketPlayData->bEventInfo[2], psPacketPlayData->bEventInfo[3],
				psPacketPlayData->bUpdateInfo[0], psPacketPlayData->bUpdateInfo[1], psPacketPlayData->bUpdateInfo[2], psPacketPlayData->bUpdateInfo[3]);
				*/

			break;

		default:

			/*UNITGAME->Log("%d bEventInfo [%d][%d][%d][%d] updateInfo [%d][%d][%d][%d]", psPacket->iHeader,
				psPacketPlayData->bEventInfo[0], psPacketPlayData->bEventInfo[1], psPacketPlayData->bEventInfo[2], psPacketPlayData->bEventInfo[3],
				psPacketPlayData->bUpdateInfo[0], psPacketPlayData->bUpdateInfo[1], psPacketPlayData->bUpdateInfo[2], psPacketPlayData->bUpdateInfo[3]);
				*/
			break;
	}

	//JLM - Rendering Experiment
	//pcUnitData->dwLastActiveTime = TICKCOUNT;
	//if (pcUnitData->sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_XY_8))
	//{
		//Old...
		CALLT_WITH_ARG1(0x00434AF0, (DWORD)pcUnitData, (DWORD)psPacket);
	//}



	//JLM - On server side it sends crown id in PKTHDR_PlayData within bEventInfo[3]
	//It was proven to be too difficult to update the crown id of each player (the code resides in EXE)
	//So instead the value from the server is overriden using the actual values here.
	//It's hacky but it does the job
	//CALLT_WITH_ARG1( 0x00434AF0, (DWORD)pcUnitData, (DWORD)psPacket ); calls the method which in turn updates
	// iBellatraCrown for sCharacterData
	if (pcUnitData->PkMode_CharState == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player)
	{
		auto bellatra = GAMECORE->GetBellatraGameHandler();
		if (bellatra->iBellatraCrownGoldClanId > 0 &&
			pcUnitData->sCharacterData.iClanID == bellatra->iBellatraCrownGoldClanId)
		{
			pcUnitData->iBellatraCrown = 1;
		}
		else if (bellatra->iBellatraCrownSilverClanId > 0 &&
			pcUnitData->sCharacterData.iClanID == bellatra->iBellatraCrownSilverClanId)
		{
			pcUnitData->iBellatraCrown = 2;
		}
		else if (bellatra->iBellatraCrownBronzeClanId > 0 &&
			pcUnitData->sCharacterData.iClanID == bellatra->iBellatraCrownBronzeClanId)
		{
			pcUnitData->iBellatraCrown = 3;
		}
		else
		{
			pcUnitData->iBellatraCrown = 0;
		}

		/*UNITGAME->Log("%s clan = %d. crown = %d (1st = %d, 2nd = %d, 3rd = %d)",
			pcUnitData->GetName(), pcUnitData->sCharacterData.iClanID, pcUnitData->iBellatraCrown,
			bellatra->iBellatraCrownGoldClanId, bellatra->iBellatraCrownSilverClanId, bellatra->iBellatraCrownBronzeClanId);*/
	}
}

/// <summary>
/// This is for other players / units
/// </summary>
void UnitGame::HandlePacketUnitBuff( UnitData * pcUnitData, PacketBuffData * psPacket )
{
	switch ( psPacket->iHeader )
	{
		case PTKHDR_Effect_ForceOrb:
		{
			//for other players
			break;
		}

		case PTKHDR_Effect_Weapon:
		{
			psPacket->iHeader = 0;

			break;
		}

		case PTKHDR_Effect_Debuff:
		{
			if ( psPacket->Debuff.bBurning )
			{
				//CHATBOX->AddDebugMessage( "Burning: %d -> %d", pcUnitData->iFireOverlay, psPacket->Debuff.bBurning );

				// If the unit's fire overlay is -ve. the unit was previously burnt
				// and the burning 'red' effect is reaching towards 0. Flip the value so that
				// it will reach the max value again. Otherwise set it to 1 to be incremented to max value.
				// See EXE.cpp ClientUnitDataUpdate()
				if ( pcUnitData->iFireOverlay < 0 )
					pcUnitData->iFireOverlay = -pcUnitData->iFireOverlay;
				else if (pcUnitData->iFireOverlay == 0 )
					pcUnitData->iFireOverlay = 1;
			}
			else
			{
				//unit is no longer burning
				//flip the value so that it will reach zero
				if (pcUnitData->iFireOverlay > 0 )
					pcUnitData->iFireOverlay = -pcUnitData->iFireOverlay;
			}

			if ( psPacket->Debuff.bPoison )
			{
				//CHATBOX->AddDebugMessage( "Poison: %d -> %d", pcUnitData->iPoisonOverlay, psPacket->Debuff.bPoison );

				// If the unit's poison overlay is -ve. the unit was previously poison
				// and the poison's 'green' effect is reaching towards 0. Flip the value so that
				// it will reach the max value again. Otherwise set it to 1 to be incremented to max value.
				// See EXE.cpp ClientUnitDataUpdate()
				if ( pcUnitData->iPoisonOverlay < 0 )
					pcUnitData->iPoisonOverlay = -pcUnitData->iPoisonOverlay;
				else if (pcUnitData->iPoisonOverlay == 0 )
					pcUnitData->iPoisonOverlay = 1;
			}
			else
			{
				//unit is no longer poisoned
				//flip the value so that it will reach zero
				if (pcUnitData->iPoisonOverlay > 0 )
					pcUnitData->iPoisonOverlay = -pcUnitData->iPoisonOverlay;
			}

			if ( psPacket->Debuff.bDistortion )
			{
				pcUnitData->bDistortion = 120;
			}
			else
			{
				pcUnitData->bDistortion = 0;
			}

			if ( psPacket->Debuff.bIceOverlay )
			{
				if ( pcUnitData->iIceOverlay <= 0 )
					pcUnitData->iIceOverlay = -200;
			}
			else
			{
				if ( pcUnitData->iIceOverlay > 0 )
					pcUnitData->iIceOverlay = ( -pcUnitData->iIceOverlay );
			}

			if ( psPacket->Debuff.bCurse )
			{
				pcUnitData->iCurseLazy = 120;

				if ( psPacket->Debuff.bCurse == 2 ) //Wisp
				{
					SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect("Wisp2", pcUnitData, 5, ESkillID::SKILLID_Wisp);
				}
			}
			else
			{
				pcUnitData->iCurseLazy = 0;
			}

			pcUnitData->bDamageReduction = psPacket->Debuff.iDamageReduction;
			pcUnitData->iSlowTimeLeft = psPacket->Debuff.iStunCount;
			break;
		}

		default:
			break;
	}
}

int UnitGame::GetFramesSendCount()
{
	int iFrames = 64;

	if (SOCKETGAME)
	{
		int iPing = SOCKETGAME->GetAveragePingGame();
		if (iPing <= 40)
			iFrames = 16;
		else if (iPing <= 150)
			iFrames = 32;
		else
			iFrames = 64;
	}

	return 64;
}