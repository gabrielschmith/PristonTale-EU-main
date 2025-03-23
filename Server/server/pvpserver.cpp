#include "stdafx.h"
#include "pvpserver.h"
#include "ini.h"


PvPServer::PvPServer()
{

}


PvPServer::~PvPServer()
{
}

BOOL PvPServer::SQLGetTopPvP( PvPClassData & s )
{
	BOOL bRet = FALSE;

	ZeroMemory( &s, sizeof( PvPClassData ) );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 100 CharacterName FROM CharacterPvP WHERE (Kills>0) ORDER BY Kills DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				char szCharacterName[32] = { 0 };

				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szCharacterName, _countof( szCharacterName ) );
					for ( int i = 0; i < 10; i++ )
					{
						if ( s.szCharacterName[i][0] == 0 )
						{
							if ( CHARACTERSERVER->SQLGetCharacterClass( szCharacterName ) == (i + 1) )
							{
								STRINGCOPY( s.szCharacterName[i], szCharacterName );
								bRet = TRUE;
								break;
							}
						}
					}
				}
			}
		}

		pcDB->Close();
	}
	return bRet;
}

BOOL PvPServer::SQLSetTopPvP( PvPClassData & s )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		for ( int i = 0; i < 10; i++ )
		{
			if ( s.szCharacterName[i][0] != 0 )
			{
				if ( pcDB->Prepare( "UPDATE CharacterPvP SET TopPvP=1 WHERE CharacterName=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, s.szCharacterName[i], STRLEN( s.szCharacterName[i] ) );
					pcDB->ExecuteUpdate();
				}
			}

			pcDB->Clear();
		}
		pcDB->Close();
	}

	return bRet;
}

void PvPServer::ResetPvPBuffer()
{
	INFO ( "ResetPvPBuffer" );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = USERSDATA + i;

		if ( ud->pcSocketData && ud->iID )
		{
			User * pcUser = USERDATATOUSER( ud );
			if ( pcUser )
			{
				pcUser->iPVPKills = 0;
				pcUser->iPVPDeaths = 0;
				pcUser->iPVPKillStreak = 0;

				OnUnLoadUser( pcUser );

				ProcessPvPData( pcUser, 0, FALSE );
			}
		}
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterPvP SET Kills=0, Deaths=0, KillStreak=0, TopPvP=0" ) )
		{
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}


BOOL PvPServer::SQLSelectCharacterPvP( User * pcUser )
{
	BOOL bRet = FALSE;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM CharacterPvP WHERE CharacterName=?" ) )
		{
			char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharacterName );

			if ( pcDB->Execute() && pcDB->Fetch() )
				bRet = TRUE;
		}
		pcDB->Close();
	}
	return bRet;
}

void PvPServer::ProcessPvPData( User * pcUser, int iID, BOOL bKill )
{
	PacketPvPData sPacket;
	sPacket.iLength			= sizeof( PacketPvPData );
	sPacket.iHeader			= PKTHDR_PVPDataUpdate;
	sPacket.iPVPKills		= pcUser->iPVPKills;
	sPacket.iPVPDeaths		= pcUser->iPVPDeaths;
	sPacket.iPVPKillStreak	= pcUser->iPVPKillStreak;
	sPacket.bKill			= bKill;
	sPacket.iID				= iID;
	SENDPACKET( pcUser, &sPacket, TRUE );

	// Old BP
	pcUser->pcUserData->sBlessCastle_Kills = (short)pcUser->iPVPKills;
}

void PvPServer::ProcessPvPKill( User * pcKiller, User * pcVictim, int iType )
{
	int iaMaps[] =
	{
		MAPID_BlessCastle, MAPID_BattleTown,
	};

	int iMapID = pcKiller->GetMapID();
	BOOL bSend = FALSE;

	for ( int i : iaMaps )
	{
		if ( i == iMapID )
		{
			bSend = TRUE;
			break;
		}
	}

	if ( bSend == FALSE )
		return;

	PacketPvPKillType sPacket;
	sPacket.iLength			= sizeof( PacketPvPKillType );
	sPacket.iHeader			= PKTHDR_PvPKill;
	sPacket.iKillType		= iType;
	sPacket.iKillerClass	= pcKiller->pcUserData->sCharacterData.iClass;
	sPacket.iVictimClass	= pcVictim->pcUserData->sCharacterData.iClass;
	STRINGCOPY( sPacket.szKillerName, CHARACTERSERVER->GetCharacterName( pcKiller->pcUserData ) );
	STRINGCOPY( sPacket.szVictimName, CHARACTERSERVER->GetCharacterName( pcVictim->pcUserData ) );
	SENDPACKETSTAGE( &sPacket, pcKiller->pcUserData->iMapID );
}

#define MAX_TIME_KILLSAFE		80

void PvPServer::OnKilled( User * pcKiller, User * pcVictim )
{
	BOOL bBlockKill = FALSE;
	if ( pcKiller->pcUserData->pcSocketData->lIP == pcVictim->pcUserData->pcSocketData->lIP )
		bBlockKill = TRUE;

	if ( pcKiller->dwLastKillIP == pcVictim->pcUserData->pcSocketData->lIP )
		bBlockKill = TRUE;
	else
		pcKiller->dwLastKillIP = pcVictim->pcUserData->pcSocketData->lIP;

	if ( pcKiller->pcUserData->iClanID != 0 && pcVictim->pcUserData->iClanID != 0 && (pcKiller->pcUserData->iClanID == pcVictim->pcUserData->iClanID) )
	{
		bBlockKill = TRUE;
	}

	if ( bBlockKill == FALSE )
	{
		pcKiller->iPVPKills++;
		pcKiller->iPVPKillStreak++;

		if ( pcKiller->iLastKillSafeID != pcVictim->GetID() )
		{
			pcKiller->iKillSafe++;
			pcKiller->iLastKillSafeID = pcVictim->GetID();
		}
		else
		{
			bBlockKill = TRUE;
		}

		if ( pcKiller->bResetKillStreak )
		{
			pcKiller->iPVPKillStreak = 1;
			pcKiller->bResetKillStreak = FALSE;
		}

		if ( pcKiller->dwKillSafeTimeOut < TICKCOUNT )
		{
			pcKiller->iKillSafe = 1;

			pcKiller->dwKillSafeTimeOut = TICKCOUNT + (MAX_TIME_KILLSAFE * 1000);
		}
	}

	//Kill Show
	if ( (pcKiller->dwKillSafeTimeOut > TICKCOUNT) && (bBlockKill == FALSE) )
	{
		int iType = PVPKILLTYPE_None;

		if ( pcKiller->iKillSafe == 2 )
			iType = PVPKILLTYPE_DoubleKill;
		else if ( pcKiller->iKillSafe == 3 )
			iType = PVPKILLTYPE_TripleKill;
		else if ( pcKiller->iKillSafe == 4 )
			iType = PVPKILLTYPE_QuadraKill;
		else if ( pcKiller->iKillSafe == 5 )
			iType = PVPKILLTYPE_PentaKill;
		else if ( pcKiller->iKillSafe == 6 )
			iType = PVPKILLTYPE_Rampage;
		else
		{
			if ( pcKiller->iKillSafe > 6 )
				iType = PVPKILLTYPE_Unstoppable;
		}

		if ( iType != PVPKILLTYPE_None )
			ProcessPvPKill( pcKiller, pcVictim, iType );

		//Check if Victim have Kills before die
		if ( (pcVictim->dwKillSafeTimeOut > TICKCOUNT) && (pcVictim->iKillSafe >= 2) )
		{
			//Send Shutdown
			ProcessPvPKill( pcKiller, pcVictim, PVPKILLTYPE_Shutdown );
		}
	}

	pcVictim->iPVPDeaths++;
	pcVictim->bResetKillStreak = TRUE;
	pcVictim->iKillSafe = 0;
	pcVictim->dwKillSafeTimeOut = 0;

	if( pcKiller->GetID() != pcVictim->GetID() )
	{
		CHATSERVER->SendChatEx( pcKiller, CHATCOLOR_Notice, "> You killed %s!", CHARACTERSERVER->GetCharacterName( pcVictim->pcUserData ) );
		CHATSERVER->SendChatEx( pcVictim, CHATCOLOR_Notice, "> %s killed you!", CHARACTERSERVER->GetCharacterName( pcKiller->pcUserData ) );
	}

	if ( bBlockKill == FALSE )
	{
		ProcessPvPData( pcKiller, pcVictim->pcUserData->iID, TRUE );
	}

	ProcessPvPData( pcVictim, pcKiller->pcUserData->iID, FALSE );
}

BOOL PvPServer::OnLoadUser( User * pcUser )
{
	if ( !GAME_SERVER )
	{
		BOOL bPvP = FALSE;
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT Kills, Deaths, TopPvP FROM CharacterPvP WHERE CharacterName=?" ) )
			{
				char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharacterName );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iPVPKills );
					pcDB->GetData( 2, PARAMTYPE_Integer, &pcUser->iPVPDeaths );
					pcDB->GetData( 3, PARAMTYPE_Integer, &bPvP );
				}
			}
			pcDB->Close();
		}

		if ( bPvP )
		{
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE CharacterPvP SET TopPvp=0 WHERE CharacterName=?" ) )
				{
					char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharacterName );

					pcDB->ExecuteUpdate();
				}
				pcDB->Close();
			}

			ITEMSERVER->CreatePremium( pcUser, ITEMID_BCBuff, ITEMTIMERTYPE_BCBuff, ITEMTIMERTIME_72H, true);
		}

		ProcessPvPData( pcUser, 0, FALSE );
	}
	return TRUE;
}

BOOL PvPServer::OnUnLoadUser( User * pcUser )
{
	if ( !GAME_SERVER )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

		if ( SQLSelectCharacterPvP( pcUser ) )
		{
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE CharacterPvP SET Kills=?, Deaths=?, KillStreak=? WHERE CharacterName=?" ) )
				{
					char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iPVPKills );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iPVPDeaths );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iPVPKillStreak );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, pszCharacterName );
					pcDB->ExecuteUpdate();
				}
				pcDB->Close();
			}
		}
		else
		{
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO CharacterPvP([CharacterName],[Kills],[Deaths],[KillStreak],[TopPvP]) VALUES (?,?,?,?,0)" ) )
				{
					char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharacterName );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iPVPKills );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iPVPDeaths );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iPVPKillStreak );
					pcDB->Execute();
				}
				pcDB->Close();
			}
		}
	}
	return TRUE;
}


void PvPServer::Update()
{
	return;

	if ( GAME_SERVER )
		return;

	if ( dwTick < TICKCOUNT )
	{
		SYSTEMTIME * ps = GetServerTime();

		if ( ps->wDayOfWeek == 6 && sLastDay != ps->wDay )
		{
			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->iID )
				{
					User * pcUser = USERDATATOUSER( ud );
					if ( pcUser )
					{
						OnUnLoadUser( pcUser );
					}
				}
			}

			PvPClassData s;
			if ( SQLGetTopPvP( s ) )
			{
				for ( int i = 0; i < 10; i++ )
				{
					int iClass = (i + 1);

					if ( s.szCharacterName[i][0] != 0 )
					{
						char * pszClass = "Fighter";

						if ( iClass == CHARACTERCLASS_Mechanician )
							pszClass = "Mechanician";
						if ( iClass == CHARACTERCLASS_Archer )
							pszClass = "Archer";
						if ( iClass == CHARACTERCLASS_Pikeman )
							pszClass = "Pikeman";
						if ( iClass == CHARACTERCLASS_Atalanta )
							pszClass = "Atalanta";
						if ( iClass == CHARACTERCLASS_Knight )
							pszClass = "Knight";
						if ( iClass == CHARACTERCLASS_Magician )
							pszClass = "Magician";
						if ( iClass == CHARACTERCLASS_Priestess )
							pszClass = "Priestess";
						if ( iClass == CHARACTERCLASS_Assassin )
							pszClass = "Assassin";
						if ( iClass == CHARACTERCLASS_Shaman )
							pszClass = "Shaman";

						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "PvP> Top Rank %s: %s", pszClass, s.szCharacterName[i] );
					}
				}

				ResetPvPBuffer();

				CHATSERVER->SendChatAll( CHATCOLOR_Global, "PvP> Rank Resetted!" );

				SQLSetTopPvP( s );

				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * ud = USERSDATA + i;

					if ( ud->pcSocketData && ud->iID )
					{
						for ( int i = 0; i < 10; i++ )
						{
							if ( s.szCharacterName[i][0] != 0 )
							{
								if ( STRINGCOMPARE( CHARACTERSERVER->GetCharacterName( ud ), s.szCharacterName[i] ) )
								{
									ITEMSERVER->CreatePremium( USERDATATOUSER(ud), ITEMID_BCBuff, ITEMTIMERTYPE_BCBuff, ITEMTIMERTIME_72H, true);
								}
							}
						}
					}
				}
			}

			sLastDay = ps->wDay;
		}

		dwTick = TICKCOUNT + (120 * 1000);
	}
}