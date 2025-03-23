#include "stdafx.h"
#include "FuryArenaHandler.h"
#include "Logger.h"


CFuryArenaHandler::CFuryArenaHandler()
{
	for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
		pcUnitDataMinions[i] = NULL;

	// Boss Name
	ZeroMemory( szBossName[FuryEventID::KingFury], 32 );
	STRINGCOPY( szBossName[FuryEventID::KingFury], _BOSS_FURYARENA );

	ZeroMemory( szBossName[FuryEventID::LordBabel], 32 );
	STRINGCOPY( szBossName[FuryEventID::LordBabel], _BOSS_FURYARENALOW );

	// Minion Name
	ZeroMemory( szMinionName[FuryEventID::KingFury], 32 );
	STRINGCOPY( szMinionName[FuryEventID::KingFury], _MINION_FURYARENA );

	ZeroMemory( szMinionName[FuryEventID::LordBabel], 32 );
	STRINGCOPY( szMinionName[FuryEventID::LordBabel], _MINION_FURYARENALOW );

	// Minion Name
	ZeroMemory( szMonsterEventName[FuryEventID::KingFury], 32 );
	STRINGCOPY( szMonsterEventName[FuryEventID::KingFury], _MONSTEREVENT_FURYARENA );

	ZeroMemory( szMonsterEventName[FuryEventID::LordBabel], 32 );
	STRINGCOPY( szMonsterEventName[FuryEventID::LordBabel], _MONSTEREVENT_FURYARENALOW );
}


CFuryArenaHandler::~CFuryArenaHandler()
{
}


BOOL CFuryArenaHandler::InsideArena( Point3D * psPosition )
{
	if ( psPosition->iX >= _XMIN_FURYARENA && psPosition->iX <= _XMAX_FURYARENA )
	{
		if ( psPosition->iZ >= _ZMIN_FURYARENA && psPosition->iZ <= _ZMAX_FURYARENA )
			return TRUE;
	}
	return FALSE;
}

int CFuryArenaHandler::GetTotalUsers()
{
	int iUsers = 0;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = USERSDATA + i;
		User * u = USERDATATOUSER( ud );
		if ( ud && u )
		{
			if ( u->bInGrandFury )
				iUsers++;
		}
	}
	return iUsers;
}

void CFuryArenaHandler::ReadConfigIniValues()
{
	INI::CReader cReader("server.ini");

	//Initial values. Also used to define max value if killed in less than 3 minutes
	FURYARENA_LORDBABEL_EXP_INITIAL			= cReader.ReadInt("Event", "FuryArena.LordBabel.ExpInitial");
	FURYARENA_LORDBABEL_GOLD_INITIAL		= cReader.ReadInt("Event", "FuryArena.LordBabel.GoldInitial");
	FURYARENA_KINGFURY_EXP_INITIAL			= cReader.ReadInt("Event", "FuryArena.KingFury.ExpInitial");
	FURYARENA_KINGFURY_GOLD_INITIAL			= cReader.ReadInt("Event", "FuryArena.KingFury.GoldInitial");

	FURYARENA_LORDBABEL_EXP_MINIMUM			= cReader.ReadInt("Event", "FuryArena.LordBabel.ExpMinimum");
	FURYARENA_KINGFURY_EXP_MINIMUM			= cReader.ReadInt("Event", "FuryArena.KingFury.ExpMinimum");

	//PER TICK = Per 10 seconds starting at 3 minutes, up to 5 minutes
	FURYARENA_LORDBABEL_EXP_LOSSPERTICK		= cReader.ReadInt("Event", "FuryArena.LordBabel.ExpLossPerTick");
	FURYARENA_LORDBABEL_GOLD_LOSSPERTICK	= cReader.ReadInt("Event", "FuryArena.LordBabel.GoldLossPerTick");
	FURYARENA_KINGFURY_EXP_LOSSPERTICK		= cReader.ReadInt("Event", "FuryArena.KingFury.ExpLossPerTick");
	FURYARENA_KINGFURY_GOLD_LOSSPERTICK		= cReader.ReadInt("Event", "FuryArena.KingFury.GoldLossPerTick");

	FURYARENA_KINGFURY_GOLD_PERMINION       = cReader.ReadInt("Event", "FuryArena.KingFury.BonusGoldPerMinionKill");
	FURYARENA_LORDBABEL_GOLD_PERMINION      = cReader.ReadInt("Event", "FuryArena.KingFury.BonusGoldPerMinionKill");

	//Meteor damage min / max
	FURYARENA_LORDBABEL_METEOR_DAMAGE_MIN  = cReader.ReadInt("Event", "FuryArena.LordBabel.MeteorDamageMin");
	FURYARENA_LORDBABEL_METEOR_DAMAGE_MAX  = cReader.ReadInt("Event", "FuryArena.LordBabel.MeteorDamageMax");
	FURYARENA_KINGFURY_METEOR_DAMAGE_MIN   = cReader.ReadInt("Event", "FuryArena.KingFury.MeteorDamageMin");
	FURYARENA_KINGFURY_METEOR_DAMAGE_MAX   = cReader.ReadInt("Event", "FuryArena.KingFury.MeteorDamageMax");
}

BOOL CFuryArenaHandler::IsBoss( UnitData * pcUnitData )
{
	if ( STRINGCOMPARE( szBossName[iEventIDNow], pcUnitData->GetName() ) )
	{
		pcUnitData->sPosition.iX = _X_KINGFURY;
		pcUnitData->sPosition.iZ = _Z_KINGFURY;
	}
	return TRUE;
}

SPrizeData* CFuryArenaHandler::GetRewardItem(int iID)
{
	// Randomize Items

	int iRand = Dice::RandomI(0, iTotalPercent);
	int iTotal = 0;

	for (int i = 0; i < 50; i++)
	{
		SPrizeData* p = saPrizesReward + i;

		// Items for boss ID?
		if (p->bActive && p->iQuantity == iID)
		{
			iTotal += p->iPercent;
			if (iRand <= iTotal)
				return p;
		}
	}
	return &saPrizesReward[0];
}

BOOL CFuryArenaHandler::KillUnits()
{
	// Boss
	if ( pcUnitDataBoss )
	{
		if ( !pcUnitDataBoss->IsDead() )
			UNITSERVER->SetMotionFromCode( pcUnitDataBoss, EAnimationType::ANIMATIONTYPE_Die);

		pcUnitDataBoss = NULL;
	}

	// Minions
	for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
	{
		if ( pcUnitDataMinions[i] )
		{
			if ( !pcUnitDataMinions[i]->IsDead() )
				UNITSERVER->SetMotionFromCode( pcUnitDataMinions[i], EAnimationType::ANIMATIONTYPE_Die);
		}
		pcUnitDataMinions[i] = NULL;
	}
	return TRUE;
}

void CFuryArenaHandler::SQLLog( char * pszLog, ... )
{
	va_list vl;

	static char szString[1024];

	va_start( vl, pszLog );
	_vsnprintf_s( szString, _countof( szString ), _TRUNCATE, pszLog, vl );
	va_end( vl );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 14 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO FuryArenaLog([Description],[Date]) VALUES(?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, szString, STRLEN( szString ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CFuryArenaHandler::SQLClearFuryArenaRewardTracker()
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("DELETE FROM FuryArenaRewardTracker"))
		{
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CFuryArenaHandler::SQLCheckAndClearFuryArenaRewardTrackerAtZeroHour()
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	//Reset at 0 hour daily
	if (sTime.wHour == 0 && bRewardTrackerResetDone == FALSE)
	{
		bRewardTrackerResetDone = TRUE;
		SQLClearFuryArenaRewardTracker();
		CHATSERVER->SendChatAllGM("Fury Area Reward Tracker Cleared!");
	}
	else if (sTime.wHour != 0 && bRewardTrackerResetDone == TRUE)
	{
		bRewardTrackerResetDone = FALSE;
	}
}

BOOL CFuryArenaHandler::SQLCanReceiveEXPReward(int iCharacterID)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);

	int iCount = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT COUNT(*) FROM FuryArenaRewardTracker WHERE CharacterID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iCharacterID);
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iCount);
			}
		}

		pcDB->Close();
	}

	return iCount < iMaxExpRewardPerDay;
}

//For checking how many times the uesr have received EXP and Gold reward
BOOL CFuryArenaHandler::SQLAddUserToRewardTracker(int iEventID, int iCharacterID)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_EventDB);

	int iCount = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO FuryArenaRewardTracker (FuryEventID, CharacterID) VALUES (?, ?)"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iEventID);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iCharacterID);
			pcDB->Execute();
		}

		pcDB->Close();
	}

	return iCount < iMaxExpRewardPerDay;
}

int CFuryArenaHandler::GetEventID()
{
	return iEventID;
}

void CFuryArenaHandler::OnFuryArenaTeleporterNPCClicked ( User * pcUser )
{
	if ( SERVER_CODE == 1 )
	{
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof ( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState = FURYARENASTATEID_OpenNPC;
		sPacket.iEventID = FURYARENAHANDLER->GetEventID ();
		sPacket.dwTimeLeft = FURYARENAHANDLER->GetTimeLeft ( GetServerTime () );
		SENDPACKET ( pcUser, &sPacket, TRUE );
	}
	else
	{
		CHATSERVER->SendChat ( pcUser, CHATCOLOR_Global, "FuryArena> The Grand Fury is closed on this server!" );
	}
}

void CFuryArenaHandler::OnFuryArenaNPCClicked ( User * pcUser )
{
	if ( SERVER_CODE == 1 )
	{
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof ( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState = FURYARENASTATEID_OpenNPCEntrance;
		sPacket.iEventID = FURYARENAHANDLER->GetEventID ();
		sPacket.dwTimeLeft = FURYARENAHANDLER->GetTimeLeft ( GetServerTime () );
		SENDPACKET ( pcUser, &sPacket, TRUE );
	}
	else
	{
		CHATSERVER->SendChat ( pcUser, CHATCOLOR_Global, "FuryArena> The Grand Fury is closed on this server!" );
	}
}

DWORD CFuryArenaHandler::GetTimeLeft( SYSTEMTIME * ps )
{
	static const DWORD dw4HoursTGF = (60 * 60 * 4);

	DWORD dw = 0;

	DWORD dwTimePS = SystemTimeToDWORD( ps );

	SYSTEMTIME s;
	ZeroMemory( &s, sizeof( SYSTEMTIME ) );

	// Fury Arena Boss ID Handler CHAOS

	int iHourOffset = 0;

	//for seasonal server, shift time by 2 hrs
	if ( SERVER_IS_SEASONAL)
		iHourOffset = 2;

	s.wHour = 2 + iHourOffset;
	s.wDay = ps->wDay;
	s.wMonth = ps->wMonth;
	s.wYear = ps->wYear;
	dw = SystemTimeToDWORD( &s ) - dwTimePS;
	if ( dw <= dw4HoursTGF )
	{
		iEventID = FuryEventID::LordBabel;
		return dw;
	}

	s.wHour = 6 + iHourOffset;
	dw = SystemTimeToDWORD(&s) - dwTimePS;
	if (dw <= dw4HoursTGF)
	{
		iEventID = FuryEventID::KingFury;
		return dw;
	}

	s.wHour = 10 + iHourOffset;
	dw = SystemTimeToDWORD(&s) - dwTimePS;
	if (dw <= dw4HoursTGF)
	{
		iEventID = FuryEventID::LordBabel;
		return dw;
	}

	s.wHour = 14 + iHourOffset;
	dw = SystemTimeToDWORD(&s) - dwTimePS;
	if (dw <= dw4HoursTGF)
	{
		iEventID = FuryEventID::KingFury;
		return dw;
	}

	s.wHour = 18 + iHourOffset;
	dw = SystemTimeToDWORD(&s) - dwTimePS;
	if (dw <= dw4HoursTGF)
	{
		iEventID = FuryEventID::LordBabel;
		return dw;
	}

	s.wHour = 22 + iHourOffset;

	dw = SystemTimeToDWORD(&s) - dwTimePS;
	if (dw <= dw4HoursTGF)
	{
		iEventID = FuryEventID::KingFury;
		return dw;
	}

	return 0;
}

//Old system.
BOOL CFuryArenaHandler::ExchangeItem( User * pcUser, PacketFuryArenaRequest * psPacket )
{
	if ( psPacket->p1 != 100 )
		return FALSE;

	int iMonster = psPacket->p4;
	int iMinutes = psPacket->p5;

	INT64 iExp	= 0;

	int iID		= -1;

	if ( iMonster == ITEMCRAFTTYPE_TGFKingFury )
	{
		// EXP King Fury
		iExp = 0;

		if ( iMinutes <= 4 )
			iExp = 300'000'000;
		else if ( iMinutes > 4 && iMinutes <= 5 )
			iExp = 250'000'000;
		else if ( iMinutes > 5 && iMinutes <= 6 )
			iExp = 200'000'000;
		else if ( iMinutes > 6 && iMinutes <= 7 )
			iExp = 150'000'000;
		else if ( iMinutes > 7 )
			iExp = 100'000'000;

		iID = 0;
	}
	else
	{
		// EXP Lord Babel
		iExp = 0;

		if (iMinutes <= 2)
			iExp = 80'000'000;
		else if (iMinutes > 2 && iMinutes <= 3)
			iExp = 60'000'000;
		else if (iMinutes > 3 && iMinutes <= 4)
			iExp = 40'000'000;
		else if (iMinutes > 4)
			iExp = 20'000'000;

		iID = 1;
	}

	// Get Prize
	SPrizeData * p = GetRewardItem( iID );

	// Exp?
	if ( STRINGCOMPAREI( "GG101", p->szCode ) )
	{
		// Give EXP to user
		CHARACTERSERVER->GiveEXP( pcUser, iExp );

		// Send EXP Sound
		PacketFuryArenaRequest sPacket;
		sPacket.iLength			= sizeof( PacketFuryArenaRequest );
		sPacket.iHeader			= PKTHDR_FuryArena;
		sPacket.eState			= FURYARENASTATEID_EXP;
		SENDPACKET( pcUser, &sPacket, TRUE );

		// Log It
		SQLLog( "AccountName[%s] CharacterName[%s] Received EXP [%s]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), FormatNumber( iExp ) );

		// Send message to user to be confirmed
		CHATSERVER->SendTitleBox( pcUser->pcUserData, "You received %s EXP", FormatNumber( iExp ) );
		CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "You received %s EXP", FormatNumber( iExp ) );
		SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );
	}
	else
	{
		// Item
		ItemData * psItemData = ITEMSERVER->FindItemPointerTable( p->szCode );
		if ( psItemData )
		{
			DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( psItemData->sBaseItemID.ToItemID() );
			if ( psDef )
			{
				// Structure Item Data to send to user
				PacketFuryArenaItemData sPacket;
				sPacket.iLength = sizeof( PacketFuryArenaItemData );
				sPacket.iHeader = PKTHDR_FuryArenaItemData;
				CopyMemory( &sPacket.sItemData, psItemData, sizeof( ItemData ) );

				ITEMSERVER->CreateItem( &sPacket.sItemData.sItem, psDef, EItemSource::FuryStone );
				ITEMSERVER->SaveItemDataToDatabase( &sPacket.sItemData.sItem );
				ITEMSERVER->ReformItem( &sPacket.sItemData.sItem );
				ITEMSERVER->AddItemInventory( pcUser->pcUserData, sPacket.sItemData.sItem.sItemID.ToItemID(), sPacket.sItemData.sItem.iChk1, sPacket.sItemData.sItem.iChk2 );

				SENDPACKET( pcUser, &sPacket, TRUE );

				// Log It
				SQLLog( "AccountName[%s] CharacterName[%s] Received Item [0x%08X][%s]", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
					psItemData->sBaseItemID.ToItemID(), psItemData->szBaseName );

				// Send message to user to be confirmed
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You received %s", psItemData->szBaseName );
				CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Error, "You received %s", psItemData->szBaseName );
			}
		}
	}

	return TRUE;
}

BOOL CFuryArenaHandler::SetBoss( const char * pszMonsterName )
{
//	STRINGCOPY( szBossName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::SetMinion( const char * pszMonsterName )
{
//	STRINGCOPY( szMinionName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::SetMonsterEvent( const char * pszMonsterName )
{
//	STRINGCOPY( szMonsterEventName, pszMonsterName );
	return TRUE;
}

BOOL CFuryArenaHandler::UpdateTime()
{
	dwTickTime = GetTimeLeft( GetServerTime() );
	return TRUE;
}

BOOL CFuryArenaHandler::SetTimeMinion( int iMinute )
{
	//iSecondsSpawnMinion	= iMinute;
	return TRUE;
}

//For testing only. Low boss HP, low mob HP etc.
void CFuryArenaHandler::ForceStartEventTestMode(int iEventIDOverride)
{
	iEventIDNow = (FuryEventID)iEventIDOverride;
	dwForceStartTime = TICKCOUNT + (10 * 1000);
	bTestMode = TRUE;

	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		if (USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2)
		{
			User* pcUser = USERDATATOUSER(&USERSDATA[i]);
			if (pcUser && pcUser->GetGameLevel() >= GAMELEVEL_One)
			{
				CHATSERVER->SendChat(pcUser, EChatColor::CHATCOLOR_White, "Fury Arena Test Mode Starting in 10s!");
				EnterFromArena(pcUser);
			}
		}
	}
}

//For testing only. Low boss HP, low mob HP etc.
void CFuryArenaHandler::ForceStartEvent(int iEventIDOverride)
{
	iEventIDNow = (FuryEventID)iEventIDOverride;
	dwForceStartTime = TICKCOUNT + (10 * 1000);

	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		if (USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2)
		{
			User* pcUser = USERDATATOUSER(&USERSDATA[i]);
			if (pcUser && pcUser->GetGameLevel() >= GAMELEVEL_One)
			{
				CHATSERVER->SendChat(pcUser, EChatColor::CHATCOLOR_White, "Fury Arena Test Mode Starting in 10s!");
				EnterFromArena(pcUser);
			}
		}
	}
}

BOOL CFuryArenaHandler::StartEvent()
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;

	if ( !bStarted )
	{
		SYSTEMTIME * psServerTime = GetServerTime();

		//Update config values
		ReadConfigIniValues();

		// Kill any units that were left behind (just in case)
		KillUnits();

		// Spawn Monster
		pcUnitDataBoss = UNITSERVER->CreateUnitEnemy( szBossName[iEventIDNow], _X_KINGFURY, _Y_KINGFURY, _Z_KINGFURY );
		pcUnitDataBoss->dwUpdateCharInfoTime = TICKCOUNT + ((60 * 60 * 2) * 1000);
		pcUnitDataBoss->sCharacterData.bExclusiveBoss = TRUE;
		pcUnitDataBoss->sCharacterData.bGrandFuryBoss = TRUE;

		//std::cout << "pcUnitData->psaDamageUsersData IS SET (VIA FURY EVENT)!" << std::endl;

		//DEBUG("BOSS: %s AttackDamageData initialized (Fury Arena)", pcUnitDataBoss->GetName());
		pcUnitDataBoss->psaDamageUsersData = new AttackDamageData[ATTACK_DAMAGE_MAX];
		if (pcUnitDataBoss->psaDamageUsersData)
		{
			ZeroMemory(pcUnitDataBoss->psaDamageUsersData, sizeof(AttackDamageData) * ATTACK_DAMAGE_MAX);
		}

		//Low HP in test mode
		if (bTestMode)
		{
			pcUnitDataBoss->SetCurrentHealth( 100 );
		}

		dwForceStartTime    = 0;
		wDayEvent			= psServerTime->wDay;
		iHourEvent			= psServerTime->wHour;


		// Spawn Minions
		for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
		{
			// Spawn
			pcUnitDataMinions[i] = UNITSERVER->CreateUnitEnemy( szMinionName[iEventIDNow], saMinionArenaPositions[i].iX, _Y_KINGFURY + 3000, saMinionArenaPositions[i].iZ );

			if (bTestMode)
			{
				pcUnitDataMinions[i]->InitHealthPoint( 100 );
			}
			else
			{
				pcUnitDataMinions[i]->InitHealthPoint( 30000 );
			}

			minionKilled[i] = FALSE;
		}

		// Send start to all
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_Status;
		sPacket.p1		= TRUE;

		iTotalUsers = 0;
		iDeathCounter = 0;
		iMinionsKilled = 0;

		// Start at all
		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			if ( USERSDATA[i].pcSocketData)
			{
				User * pcUser = USERDATATOUSER( &USERSDATA[i] );

				if ( pcUser->bInGrandFury && InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					SENDPACKET( pcUser, &sPacket, TRUE );
					iTotalUsers++;
				}
			}
		}


		// Notify Users about event
		if (bTestMode == FALSE)
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "FuryArena> The Grand Fury Started!" );

		dwStarted = TICKCOUNT;

		dwMeteor = 0;

		// Event Started
		bStarted = TRUE;

		bUsersAllLeftFuryArena = FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::EndEvent()
{
	//End test mode
	bTestMode = FALSE;

	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;

	if ( bStarted )
	{
		// Set Vars
		bAlert1Start		= FALSE;
		bAlert2Start		= FALSE;
		bAlert3Start		= FALSE;
		bAlert4Start		= FALSE;

		// Leave Time
		//dwEventLeaveTime	= TICKCOUNT + (20 * 1000);

		// Notify users from event end
		if (bTestMode == FALSE)
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "FuryArena> The Grand Fury Ended!" );

		// Kill All Units in event
		KillUnits();

		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_Status;
		sPacket.p1		= FALSE;

		// End at all
		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			if ( USERSDATA[i].pcSocketData )
			{
				User * pcUser = USERDATATOUSER( &USERSDATA[i] );

				if ( pcUser->bInGrandFury && InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					SENDPACKET( pcUser, &sPacket, TRUE );

					//leave arena between 10 and 20 seconds (spread out)
					int iSeconds = Dice::RandomI(10, 20);
					pcUser->dwFuryLeaveTime = TICKCOUNT + (iSeconds * 1000);

					CHATSERVER->SendChatEx(pcUser, EChatColor::CHATCOLOR_Error, "Leaving Fury Arena in %d seconds..", iSeconds);
				}
			}
		}

		// Event Ended
		bStarted			= FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::EnterFromArena( User * pcUser )
{
	// Is Already entered is is flagged? no need again
	if ( pcUser->bInGrandFury == TRUE && InsideArena( &pcUser->pcUserData->sPosition ))
		return FALSE;

	// Teleport
	CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_KINGFURY + (Dice::RandomI(0, 4096 * 2)), _Z_KINGFURY + (Dice::RandomI(0, 4096 * 2)), MAPID_CursedTempleF2 );

	// Entered from Arena
	CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> %s has entered the Arena", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

	// Success
	pcUser->bInGrandFury = TRUE;
	pcUser->dwGrandFuryTickEnter = TICKCOUNT + (15 * 1000);
	pcUser->dwFuryLeaveTime = 0;

	// Packet Success
	PacketFuryArenaRequest sPacket;
	sPacket.iLength		= sizeof( PacketFuryArenaRequest );
	sPacket.iHeader		= PKTHDR_FuryArena;
	sPacket.eState		= FURYARENASTATEID_Enter;
	SENDPACKET( pcUser, &sPacket );

	sPacket.eState	= FURYARENASTATEID_Status;
	sPacket.p1		= bStarted;
	SENDPACKET( pcUser, &sPacket );
	return TRUE;
}

BOOL CFuryArenaHandler::LeaveFromArena()
{
	BOOL bFoundAny = FALSE;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		// Alive and in the map?
		if ( USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2 )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			// Inside Arena?
			if ( InsideArena( &pcUser->pcUserData->sPosition ) || pcUser->bInGrandFury )
			{
				if (pcUser->dwFuryLeaveTime != 0 && TICKCOUNT > pcUser->dwFuryLeaveTime)
				{
					// Leave
					CHARACTERSERVER->SetPositionWarp(pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2);
					pcUser->bInGrandFury = FALSE;
					pcUser->dwGrandFuryTickEnter = 0;
					pcUser->dwFuryLeaveTime = 0;
				}

				bFoundAny = TRUE;
			}
		}
	}

	// All left. No longer need to check.
	if (bFoundAny == FALSE)
	{
		bUsersAllLeftFuryArena = TRUE;
	}

	return TRUE;
}

BOOL CFuryArenaHandler::LeaveFromArena( User * pcUser )
{
	if ( pcUser )
	{
		// Teleport Leave
		CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2 );

		// Set State
		pcUser->bInGrandFury = FALSE;
		pcUser->dwGrandFuryTickEnter = 0;
		pcUser->dwFuryLeaveTime = 0;

		// Send Arena Status to user
		PacketFuryArenaRequest sPacket;
		sPacket.iLength = sizeof( PacketFuryArenaRequest );
		sPacket.iHeader = PKTHDR_FuryArena;
		sPacket.eState	= FURYARENASTATEID_Status;
		sPacket.p1		= FALSE;
		SENDPACKET( pcUser, &sPacket, TRUE );

		// Notify users
		CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> %s left the Arena (Kicked)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		return TRUE;
	}

	return FALSE;
}

BOOL CFuryArenaHandler::OnDisconnected( User * pcUser )
{
	CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> %s left the Arena (DC)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	pcUser->bInGrandFury			= FALSE;
	pcUser->dwGrandFuryTickEnter	= 0;
	pcUser->dwFuryLeaveTime = 0;
	return TRUE;
}

BOOL CFuryArenaHandler::OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData, int iKillerUserDataId )
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return FALSE;

	// Not Boss?
	if ( !STRINGCOMPARE( szBossName[iEventIDNow], pcUnitData->GetName() ) )
	{
		// Inside arena?
		if ( InsideArena( &pcUnitData->sPosition ) )
		{
			// Minion?
			for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
			{
				// Kill him
				if ( pcUnitData == pcUnitDataMinions[i] )
				{
					pcUnitDataMinions[i] = NULL;
					break;
				}
			}
			return FALSE;
		}
	}
	// Is on Arena?
	if ( !InsideArena( &pcUnitData->sPosition ) )
		return FALSE;

	// Set End Event
	EndEvent();



	//See https://docs.google.com/spreadsheets/d/1GNWePBhgcB1YX1n9pny5WlQk2y5lnsa2jJ8JCBAWMMA/edit#gid=1488786183
	int iStartingGold    = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_GOLD_INITIAL     : FURYARENA_LORDBABEL_GOLD_INITIAL;
	int iStartingExp     = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_EXP_INITIAL      : FURYARENA_LORDBABEL_EXP_INITIAL;
	int iMinTime         = iEventIDNow == FuryEventID::KingFury ? (3 * 60)						      : (1 * 60);
	int iMaxTime         = iEventIDNow == FuryEventID::KingFury ? (5 * 60)						      : (3 * 60);
	int iTickSecond      = 10; //1 tick = 10 seconds
	int iExpLossPerTick  = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_EXP_LOSSPERTICK  : FURYARENA_LORDBABEL_EXP_LOSSPERTICK;
	int iGoldLossPerTick = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_GOLD_LOSSPERTICK : FURYARENA_LORDBABEL_GOLD_LOSSPERTICK;
	int iGoldPerMinion   = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_GOLD_PERMINION   : FURYARENA_LORDBABEL_GOLD_PERMINION;
	int iMinExperience   = iEventIDNow == FuryEventID::KingFury ? FURYARENA_KINGFURY_EXP_MINIMUM      : FURYARENA_LORDBABEL_EXP_MINIMUM;

	// Get seconds killed TGF Boss
	int iSeconds = (TICKCOUNT - dwStarted) / 1000;
	float fMinutes = static_cast<float>(iSeconds / 60.0f);

	if (iSeconds < iMinTime)
		iSeconds = iMinTime;

	if (iSeconds > iMaxTime)
		iSeconds = iMaxTime;

	int iDuration        = iSeconds - iMinTime;			   //calculate duration in seconds since min time
	int iTicks           = iDuration / iTickSecond;        //total ticks passed since min time
	int iTotalExpLoss    = iExpLossPerTick * iTicks;	   //calculate exp loss over time
	int iTotalGoldLoss   = iGoldLossPerTick * iTicks;	   //calculate gold loss over time
	int iTotalExpReward  = iStartingExp - iTotalExpLoss;   //calculate total exp reward
	int iTotalGoldReward = iStartingGold - iTotalGoldLoss + (iMinionsKilled * iGoldPerMinion); //calculate total gold reward

	if (iTotalExpReward < iMinExperience)
		iTotalExpReward = iMinExperience;

	if (iTotalGoldReward < 0)
		iTotalGoldReward = 0;

	SQLLog("EVENT DATA = Name: %s, Duration: %.1f, Total: %d, Dead: %d", szBossName[iEventIDNow], fMinutes, iTotalUsers, iDeathCounter);

	CHATSERVER->SendChatAllUsersInRange(_X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_White, "FuryArena> %s killed in %.1f minutes!", szBossName[iEventIDNow], fMinutes);
	CHATSERVER->SendChatAllUsersInRange(_X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_White, "FuryArena> Minions killed: %d/%d",		 iMinionsKilled, _MINION_FURYARENA_CNT);

	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		// Alive and in the map?
		if (USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2)
		{
			User* pcUser = USERDATATOUSER(&USERSDATA[i]);

			// Inside Arena?
			if (pcUser && InsideArena(&pcUser->pcUserData->sPosition))
			{
				// Is in event?
				if (pcUser->bInGrandFury)
				{
					if (pcUserData != pcUser->pcUserData)
					{
						UNITSERVER->ProcessPacketKillUnitID(pcUser, pcUnitData->iID, iKillerUserDataId );
					}


					CHARACTERSERVER->GiveGOLD(pcUser, iTotalGoldReward, EWhereAction::WHEREID_FuryArena);
					CHATSERVER->SendChatEx(pcUser, CHATCOLOR_White, "FuryArena> Gold reward: %s", FormatNumber(iTotalGoldReward));

					//Limit to two times a day.
					//But don't limit gold reward
					if (SQLCanReceiveEXPReward(pcUser->iCharacterID))
					{
						CHARACTERSERVER->GiveEXP(pcUser, iTotalExpReward);
						CHATSERVER->SendChatEx(pcUser, CHATCOLOR_White, "FuryArena> EXP reward: %s", FormatNumber(iTotalExpReward));

						SQLLog("AccountName[%s] CharacterName[%s] EventID[%d] received reward: Exp[%d] Gold[%d]",
							pcUser->pcUserData->szAccountName,
							CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
							iEventIDNow,
							iTotalExpReward,
							iTotalGoldReward);
					}
					else
					{
						CHATSERVER->SendChatEx(pcUser, CHATCOLOR_Error, "FuryArena> No EXP reward (Max 2 daily). Resets at 0h server time");

						SQLLog("AccountName[%s] CharacterName[%s] EventID[%d] received reward: Exp[%d] Gold[%d]",
							pcUser->pcUserData->szAccountName,
							CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
							iEventIDNow,
							0,
							iTotalGoldReward);
					}

					SQLAddUserToRewardTracker(iEventIDNow, pcUser->iCharacterID);
				}
			}
		}
	}

	return TRUE;
}

void CFuryArenaHandler::PacketHandlerData( User * pcUser, PacketFuryArenaRequest * psPacket )
{
	EFuryArenaState eState = psPacket->eState;

	DWORD dw = GetTimeLeft( GetServerTime() );

	switch ( eState )
	{
		// Open NPC, request time
		case FURYARENASTATEID_OpenNPC:
			if ( !GAME_SERVER )
				break;

			if ( SERVER_CODE == 1 )
			{
				psPacket->dwTimeLeft = GetTimeLeft( GetServerTime() );
				psPacket->iEventID = GetEventID();
				SENDPACKET( pcUser, psPacket, TRUE );
			}
			else
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "FuryArena> The Grand Fury is closed on this server!" );
			}
			break;

		// Request Teleport
		case FURYARENASTATEID_TeleportCT2:
		{
			if (GAME_SERVER)
				break;

			int iGold = pcUser->pcUserData->GetGold();
			int iPrice = pcUser->pcUserData->GetLevel() * 1000;

			if (iPrice > 0 && iPrice <= iGold)
			{
				CHARACTERSERVER->GiveGOLD(pcUser, -iPrice, EWhereAction::WHEREID_FuryArena);
				CHARACTERSERVER->SetPositionWarp(pcUser->pcUserData, _X_LEAVEFURYARENA + (Dice::RandomI(0, 4096)), _Z_LEAVEFURYARENA + (Dice::RandomI(0, 4096)), MAPID_CursedTempleF2);
			}
			else
			{
				CHATSERVER->SendChat(pcUser, CHATCOLOR_Global, "FuryArena> Not enough gold!");
			}
			break;
		}

		// Request state
		case FURYARENASTATEID_RequestEnterArena:
			if ( !GAME_SERVER )
				break;
			// Started?
			if ( bStarted )
			{
				// Notify user
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Event was started" );
				break;
			}

			// Time to enter on arena
			if ( dw < 30 || dw > (15*60) )
			{
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You cannot enter the arena at this moment" );
				break;
			}

			// Level Level 75 - 94
			if ( iEventID == FuryEventID::LordBabel)
			{
				if ( pcUser->pcUserData->GetLevel() >= 95 )
				{
					// Hack, can't enter at arena
					CHATSERVER->SendTitleBox( pcUser->pcUserData, "You must be between level 75 and 94, inclusive" );
					break;
				}
			}

			//Level 95+
			else if ( iEventID == FuryEventID::KingFury )
			{
				if (pcUser->pcUserData->GetLevel() < 95)
				{
					// Hack, can't enter at arena
					CHATSERVER->SendTitleBox(pcUser->pcUserData, "You must be level 95 or above");
					break;
				}
			}

			// Success!
			EnterFromArena( pcUser );
			break;

		case FURYARENASTATEID_Exchange:
			// Items need to be on Login Server
			if ( GAME_SERVER )
				break;

			// Exchange Item
			if ( !ExchangeItem(pcUser, psPacket) )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Error: Cannot receive reward" );

			break;

		default:
			break;
	}
}

void CFuryArenaHandler::Meteorite()
{
	//First Meteorite to occur in 10 seconds
	if ( !dwMeteor )
	{
		dwMeteor = TICKCOUNT + 10000;
	}

	// Meteor Tick
	if ( dwMeteor < TICKCOUNT )
	{
		// Structure Packet to fire meteor effect
		PacketFireMeteorite sPacket;
		sPacket.iLength = sizeof( PacketFireMeteorite );
		sPacket.iHeader = PKTHDR_FireMeteorite;
		sPacket.bSmallMeteor = FALSE;
		sPacket.iDelay = Dice::RandomI( 50, 251 );
		sPacket.iCount = Dice::RandomI( 1, 3 );

		// Structure Packet to Quick Damage
		PacketDamageQuick sPDM;
		sPDM.iLength		= sizeof( PacketDamageQuick );
		sPDM.iHeader		= PKTHDR_DamageQuick;
		sPDM.bPercentile	= FALSE;
		sPDM.bBaseCurrentHP = FALSE;
		sPDM.iDamage		= iEventIDNow == FuryEventID::KingFury ?
			Dice::RandomI(FURYARENA_KINGFURY_METEOR_DAMAGE_MIN, FURYARENA_KINGFURY_METEOR_DAMAGE_MAX) :
			Dice::RandomI(FURYARENA_LORDBABEL_METEOR_DAMAGE_MIN, FURYARENA_LORDBABEL_METEOR_DAMAGE_MAX);


		//sPDM.iDamage		= iEventIDNow == FuryEventID::KingFury ? Dice::RandomI( 250, 350 ) : Dice::RandomI( 150, 250 );

		std::vector<int> numbers;

		for (int i = 0; i < _FURYARENA_OUTER_POSITION_CNT; i++)
		{
			numbers.push_back(i);
		}
		std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(TICKCOUNT));

		for ( int i = 0; i < _METEOR_CNT; i++ )
		{
			auto meteorPos = saFuryArenaPositions[numbers[i]];

			// Set Meteor Position and send effect to users on range
			// Come from straight down.
			sPacket.sBegin.iX   = meteorPos.iX;
			sPacket.sBegin.iY	= meteorPos.iY + 150000;
			sPacket.sBegin.iZ   = meteorPos.iZ;
			sPacket.sEnd.iX		= meteorPos.iX;
			sPacket.sEnd.iY		= meteorPos.iY;
			sPacket.sEnd.iZ		= meteorPos.iZ;

			SENDPACKETRANGE( &sPacket, (Point3D*)&meteorPos, MAPID_CursedTempleF2, DISTANCE_XY_41_meters );

			// Send Quick Damage Packet at users on range
			SENDPACKETRANGE( &sPDM, (Point3D*)&meteorPos, MAPID_CursedTempleF2, 13000); //13000 ~ 4 meters
		}

		// Then every 3 seconds
		dwMeteor = TICKCOUNT + 3000;
	}
}

void CFuryArenaHandler::UpdateHPBoss()
{
	PacketFuryArenaBossData s;
	s.iLength				= sizeof( PacketFuryArenaBossData );
	s.iHeader				= PKTHDR_FuryArenaBossData;

	// Convert to real HP
	s.sHP.iCurrentBossHP    = pcUnitDataBoss->GetCurrentHealth();
	s.sHP.iMaxBossHP		= pcUnitDataBoss->GetMaxHealth();

	// Handle HP
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		// Alive and in the map?
		if ( USERSDATA[i].pcSocketData && USERSDATA[i].iMapID == MAPID_CursedTempleF2 )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			// Inside Arena?
			if ( InsideArena( &pcUser->pcUserData->sPosition ) )
			{
				// Is in event?
				if ( pcUser->bInGrandFury )
				{
					SENDPACKET( pcUser, &s, TRUE );
				}
			}
		}
	}
}

// Game server only
// Tick = 1 second interval
void CFuryArenaHandler::Tick1s()
{
	// Is Disabled? no event
	if ( !IsEnabled() )
		return;

	//Not game server?
	if ( !GAME_SERVER )
		return;

	// Not Midgard? no event
	if ( SERVER_CODE != 1 )
		return;

	SYSTEMTIME * psServerTime = GetServerTime();

	if (dwTickTime == 0)
		dwTickTime = GetTimeLeft(psServerTime);

	dwTickTime -= 1;

	if ( !bStarted )
	{
		SQLCheckAndClearFuryArenaRewardTrackerAtZeroHour();

		if ( !bAlert1Start && dwTickTime == (30 * 60) )
		{
			// Notify Users about event
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "FuryArena> %s will start in 30 minutes!", szBossName[iEventID] );
			bAlert1Start = TRUE;
		}
		if ( !bAlert2Start && dwTickTime == (15 * 60) )
		{
			// Notify Users about event
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "FuryArena> %s will start in 15 minutes!", szBossName[iEventID] );
			bAlert2Start = TRUE;
			bCanEnter = TRUE;
		}

		if ( !bAlert3Start && dwTickTime == (10 * 60) )
		{
			// Notify Users about event
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "FuryArena> %s will start in 10 minutes!", szBossName[iEventID] );
			bAlert3Start = TRUE;
		}

		if ( !bAlert4Start && dwTickTime == (5 * 60) )
		{
			// Notify Users about event
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "FuryArena> %s will start in 5 minutes!", szBossName[iEventID] );
			bAlert4Start = TRUE;
		}

		if ( dwTickTime > 5 && dwTickTime <= (30 * 60) )
		{
			iEventIDNow = iEventID;
		}

		if (dwForceStartTime != 0 &&
			TICKCOUNT > dwForceStartTime)
		{
			StartEvent();
		}
		else
		{
			if (dwTickTime == 0)
			{
				// Start event
				StartEvent();
			}
		}



		if ( (dwTickTime % ((30 * 60) + 1)) == 0 )
			dwTickTime = GetTimeLeft( psServerTime );
	}

	if ( bStarted )
	{
		Meteorite();

		UpdateHPBoss();

		// Time to spawn minions?

		// Spawn Minions
		for ( int i = 0; i < _MINION_FURYARENA_CNT; i++ )
		{
			if (pcUnitDataMinions[i])
			{
				// Unit is still alive?
				if (pcUnitDataMinions[i]->IsDead() == FALSE)
				{
					continue;
				}

				// Unit haven't been killed yet and it has been killed
				else if (minionKilled[i] == FALSE)
				{
					//Track minion kills for bonus gold
					iMinionsKilled++;
					CHATSERVER->SendChatAllUsersInRange(_X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> Minion killed! (%d / %d)", iMinionsKilled, _MINION_FURYARENA_CNT);

					minionKilled[i] = TRUE;
				}

				/*else if (TICKCOUNT > dwNextSpawnTime[i])
				{
					// Spawn new minion
					pcUnitDataMinions[i] = UNITSERVER->CreateUnitEnemy(szMinionName[iEventIDNow], MinionsFuryArena[i].iX, _Y_KINGFURY + 3000, MinionsFuryArena[i].iZ);
					dwNextSpawnTime[i] = 0;
				}*/
			}
		}


		int iHour = iHourEvent + 2;
		if ( iHour >= 24 )
			iHour -= 24;

		// Is after 2 Hours?
		if ( psServerTime->wHour == (WORD)iHour )
		{
			// End event
			EndEvent();
		}

		// Boss was killed extern? end event
		if ( pcUnitDataBoss && (pcUnitDataBoss->bActive == FALSE || pcUnitDataBoss->IsDead()) )
			EndEvent();

		if ( GetTotalUsers() == 0 )
			EndEvent();
	}

	// Check Leave from Arena
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			User * pcUser = USERDATATOUSER( &USERSDATA[i] );

			if ( pcUser->bInGrandFury && pcUser->dwGrandFuryTickEnter < TICKCOUNT )
			{
				// Not Inside Arena?
				if ( !InsideArena( &pcUser->pcUserData->sPosition ) )
				{
					PacketFuryArenaRequest sPacket;
					sPacket.iLength = sizeof( PacketFuryArenaRequest );
					sPacket.iHeader = PKTHDR_FuryArena;
					sPacket.eState	= FURYARENASTATEID_Status;
					sPacket.p1		= FALSE;
					SENDPACKET( pcUser, &sPacket, TRUE );

					// Leave from Arena
					CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> %s left the Arena", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
					pcUser->bInGrandFury = FALSE;
					pcUser->dwGrandFuryTickEnter = 0;
					pcUser->dwFuryLeaveTime = 0;
				}

				if ( USERSDATA[i].IsDead() )
				{
					// Leave from Arena ( Killed )
					CHATSERVER->SendChatAllUsersInRange( _X_KINGFURY, _Z_KINGFURY, MAPID_CursedTempleF2, DISTANCE_XY_41_meters, CHATCOLOR_Global, "FuryArena> %s left the Arena (Killed)", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

					iDeathCounter++;

					PacketFuryArenaRequest sPacket;
					sPacket.iLength = sizeof( PacketFuryArenaRequest );
					sPacket.iHeader = PKTHDR_FuryArena;
					sPacket.eState	= FURYARENASTATEID_Status;
					sPacket.p1		= FALSE;
					SENDPACKET( pcUser, &sPacket, TRUE );

					// Teleport Leave
					CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, _X_LEAVEFURYARENA, _Z_LEAVEFURYARENA, MAPID_CursedTempleF2 );



					// Set State
					pcUser->bInGrandFury = FALSE;
					pcUser->dwGrandFuryTickEnter = 0;
					pcUser->dwFuryLeaveTime = 0;
				}
			}
		}
	}

	// Leave Time?
	if (bUsersAllLeftFuryArena == FALSE)
	{
		// Leave users from arena
		LeaveFromArena();
	}
}

void CFuryArenaHandler::Init()
{

	ReadConfigIniValues();

	//Obsolete
	ZeroMemory( saPrizesReward, sizeof( SPrizeData ) * _countof( saPrizesReward ) );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB, 15 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ItemCode, [Percent], BossID FROM FuryArena ORDER BY ID ASC" ) )
		{
			int i = 0;

			pcDB->Execute();

			while ( pcDB->Fetch() )
			{
				SPrizeData * ps = saPrizesReward + i;

				pcDB->GetData( 1, PARAMTYPE_String, ps->szCode, 32 );
				pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iPercent );
				pcDB->GetData( 3, PARAMTYPE_Integer, &ps->iQuantity ); // BossID

				iTotalPercent += ps->iPercent;

				ps->bActive = TRUE;

				i++;
			}
		}
		pcDB->Close();
	}


	//GAME SERVER
	if (GAME_SERVER)
	{
		dwTickTime = GetTimeLeft( GetServerTime() );
	}

	SetEnable( FURYARENA_ISENABLED );
}
