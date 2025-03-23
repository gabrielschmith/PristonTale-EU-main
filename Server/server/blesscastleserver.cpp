#include "stdafx.h"
#include "blesscastleserver.h"


BlessCastleServer::BlessCastleServer()
{
	psBlessCastlePVPLeague	= (BlessCastlePVPLeague*)0x07AB14FC;
	psBlessCastleStatusData = (BlessCastleStatusData*)0x07AB1EE8;

	iClanBCID1 = 0;
	iClanBCID2 = 0;
	iClanBCID3 = 0;
}


BlessCastleServer::~BlessCastleServer()
{
}

void BlessCastleServer::NetSendBlessCastleStatus( NetConnection * psNetConnection )
{
	BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

	sBlessCastleStatusShort.iClanID1			= iClanBCID1;
	sBlessCastleStatusShort.iClanID2			= iClanBCID2;
	sBlessCastleStatusShort.iClanID3			= iClanBCID3;
	sBlessCastleStatusShort.iTax				= psBlessCastleStatusData->iTax;
	sBlessCastleStatusShort.iSkill				= (int)psBlessCastleStatusData->iSkill;
	sBlessCastleStatusShort.iaTowerType[0]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[0]);
	sBlessCastleStatusShort.iaTowerType[1]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[1]);
	sBlessCastleStatusShort.iaTowerType[2]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[2]);
	sBlessCastleStatusShort.iaTowerType[3]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[3]);
	sBlessCastleStatusShort.iaTowerType[4]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[4]);
	sBlessCastleStatusShort.iaTowerType[5]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[5]);
	sBlessCastleStatusShort.iaGuardAmount[0]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[0]);
	sBlessCastleStatusShort.iaGuardAmount[1]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[1]);
	sBlessCastleStatusShort.iaGuardAmount[2]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[2]);

	PacketNetBCStatus sPacketNet;
	sPacketNet.iLength		= sizeof( PacketNetBCStatus );
	sPacketNet.iHeader		= PKTHDR_NetBCStatusSnd;
	CopyMemory( &sPacketNet.sBlessCastleStatus, &sBlessCastleStatusShort, sizeof( sPacketNet.sBlessCastleStatus ) );

	if ( psNetConnection != NULL )
	{
		NETSERVER->SendPacket( psNetConnection->pcSocketData, &sPacketNet );
	}
	else if ( LOGIN_SERVER )
	{
		NETSERVER->SendPacketGameServers( &sPacketNet );
	}
}


BOOL BlessCastleServer::SQLWriteBlessCastleStatus()
{
	//only for login server
	if ( GAME_SERVER )
		return FALSE;

	BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

	sBlessCastleStatusShort.iClanID1		    = iClanBCID1;
	sBlessCastleStatusShort.iClanID2		    = iClanBCID2;
	sBlessCastleStatusShort.iClanID3		    = iClanBCID3;
	sBlessCastleStatusShort.iTax				= psBlessCastleStatusData->iTax;
	sBlessCastleStatusShort.iSkill				= (int)psBlessCastleStatusData->iSkill;
	sBlessCastleStatusShort.iaTowerType[0]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[0]);
	sBlessCastleStatusShort.iaTowerType[1]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[1]);
	sBlessCastleStatusShort.iaTowerType[2]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[2]);
	sBlessCastleStatusShort.iaTowerType[3]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[3]);
	sBlessCastleStatusShort.iaTowerType[4]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[4]);
	sBlessCastleStatusShort.iaTowerType[5]		= (int)(psBlessCastleStatusData->sSettings.saTowerType[5]);
	sBlessCastleStatusShort.iaGuardAmount[0]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[0]);
	sBlessCastleStatusShort.iaGuardAmount[1]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[1]);
	sBlessCastleStatusShort.iaGuardAmount[2]	= (int)(psBlessCastleStatusData->sSettings.caGuardAmount[2]);

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB, 10 );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "UPDATE BlessCastleSettings SET ClanID1=?,ClanID2=?,ClanID3=?,Tax=?,Skill=?,Tower1Type=?,Tower2Type=?,Tower3Type=?,Tower4Type=?,Tower5Type=?,Tower6Type=?,Guard1Amount=?,Guard2Amount=?,Guard3Amount=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID1 );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID2 );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID3 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &sBlessCastleStatusShort.iTax );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &sBlessCastleStatusShort.iSkill );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[0] );
			pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[1] );
			pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[2] );
			pcDB->BindParameterInput( 9, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[3] );
			pcDB->BindParameterInput( 10, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[4] );
			pcDB->BindParameterInput( 11, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[5] );
			pcDB->BindParameterInput( 12, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[0] );
			pcDB->BindParameterInput( 13, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[1] );
			pcDB->BindParameterInput( 14, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[2] );

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}

	NetSendBlessCastleStatus();

	return TRUE;
}


void BlessCastleServer::SQLReadBlessCastleStatus()
{
	if( !GAME_SERVER )
	{
		BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB, 11 );

		if( pcDB->Open() )
		{
			if( pcDB->Prepare( "SELECT ClanID1,ClanID2,ClanID3,Tax,Skill,Tower1Type,Tower2Type,Tower3Type,Tower4Type,Tower5Type,Tower6Type,Guard1Amount,Guard2Amount,Guard3Amount FROM BlessCastleSettings" ) )
			{
				if( pcDB->Execute() )
				{
					if( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID1 );
						pcDB->GetData( 2, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID2 );
						pcDB->GetData( 3, PARAMTYPE_Integer, &sBlessCastleStatusShort.iClanID3 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &sBlessCastleStatusShort.iTax );
						pcDB->GetData( 5, PARAMTYPE_Integer, &sBlessCastleStatusShort.iSkill );
						pcDB->GetData( 6, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[0] );
						pcDB->GetData( 7, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[1] );
						pcDB->GetData( 8, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[2] );
						pcDB->GetData( 9, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[3] );
						pcDB->GetData( 10, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[4] );
						pcDB->GetData( 11, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaTowerType[5] );
						pcDB->GetData( 12, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[0] );
						pcDB->GetData( 13, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[1] );
						pcDB->GetData( 14, PARAMTYPE_Integer, &sBlessCastleStatusShort.iaGuardAmount[2] );
					}
				}
			}

			pcDB->Close();
		}

		psBlessCastleStatusData->iClanID					= sBlessCastleStatusShort.iClanID1;
		psBlessCastleStatusData->iTax						= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->iTaxNow					= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->iSkill						= sBlessCastleStatusShort.iSkill;
		psBlessCastleStatusData->sSettings.iTax				= sBlessCastleStatusShort.iTax;
		psBlessCastleStatusData->sSettings.saTowerType[0]	= (short)(sBlessCastleStatusShort.iaTowerType[0]);
		psBlessCastleStatusData->sSettings.saTowerType[1]	= (short)(sBlessCastleStatusShort.iaTowerType[1]);
		psBlessCastleStatusData->sSettings.saTowerType[2]	= (short)(sBlessCastleStatusShort.iaTowerType[2]);
		psBlessCastleStatusData->sSettings.saTowerType[3]	= (short)(sBlessCastleStatusShort.iaTowerType[3]);
		psBlessCastleStatusData->sSettings.saTowerType[4]	= (short)(sBlessCastleStatusShort.iaTowerType[4]);
		psBlessCastleStatusData->sSettings.saTowerType[5]	= (short)(sBlessCastleStatusShort.iaTowerType[5]);
		psBlessCastleStatusData->sSettings.caGuardAmount[0]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[0]);
		psBlessCastleStatusData->sSettings.caGuardAmount[1]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[1]);
		psBlessCastleStatusData->sSettings.caGuardAmount[2]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[2]);
		psBlessCastleStatusData->sSettings.iSkill			= sBlessCastleStatusShort.iSkill;

		//Update New ClanIDs used for 3 crowns
		iClanBCID1 = sBlessCastleStatusShort.iClanID1;
		iClanBCID2 = sBlessCastleStatusShort.iClanID2;
		iClanBCID3 = sBlessCastleStatusShort.iClanID3;

		NetSendBlessCastleStatus();
	}
	else
	{
		Packet sPacket;
		sPacket.iLength		= sizeof( sPacket );
		sPacket.iHeader		= PKTHDR_NetBCStatusReq;

		NetConnection * nc = NETSERVER->GetLoginServerConnection();
		if ( nc )
			NETSERVER->SendPacket( nc->pcSocketData, &sPacket );
	}
}

void BlessCastleServer::SQLUpdateUserScore( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [BlessCastleScore]=? WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iBlessCastleScore );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

/// <summary>
/// Get current Tax rate for Bless Castle
/// 10 = 10%, 0 = 0%
/// </summary>
int  BlessCastleServer::GetCurrentTax ()
{
	if ( psBlessCastleStatusData == NULL)
		return 0;

	return psBlessCastleStatusData->iTax;
}

void BlessCastleServer::NetRecvBlessCastleStatus( PacketNetBCStatus * psPacket )
{
	BlessCastleStatusShort sBlessCastleStatusShort = { 0 };

	memcpy( &sBlessCastleStatusShort, &psPacket->sBlessCastleStatus, sizeof( sBlessCastleStatusShort ) );

	psBlessCastleStatusData->iClanID					= sBlessCastleStatusShort.iClanID1;
	psBlessCastleStatusData->iTax						= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->iTaxNow					= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->iSkill						= sBlessCastleStatusShort.iSkill;
	psBlessCastleStatusData->sSettings.iTax				= sBlessCastleStatusShort.iTax;
	psBlessCastleStatusData->sSettings.saTowerType[0]	= (short)(sBlessCastleStatusShort.iaTowerType[0]);
	psBlessCastleStatusData->sSettings.saTowerType[1]	= (short)(sBlessCastleStatusShort.iaTowerType[1]);
	psBlessCastleStatusData->sSettings.saTowerType[2]	= (short)(sBlessCastleStatusShort.iaTowerType[2]);
	psBlessCastleStatusData->sSettings.saTowerType[3]	= (short)(sBlessCastleStatusShort.iaTowerType[3]);
	psBlessCastleStatusData->sSettings.saTowerType[4]	= (short)(sBlessCastleStatusShort.iaTowerType[4]);
	psBlessCastleStatusData->sSettings.saTowerType[5]	= (short)(sBlessCastleStatusShort.iaTowerType[5]);
	psBlessCastleStatusData->sSettings.caGuardAmount[0]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[0]);
	psBlessCastleStatusData->sSettings.caGuardAmount[1]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[1]);
	psBlessCastleStatusData->sSettings.caGuardAmount[2]	= (unsigned char)(sBlessCastleStatusShort.iaGuardAmount[2]);
	psBlessCastleStatusData->sSettings.iSkill			= sBlessCastleStatusShort.iSkill;

	iClanBCID1 = sBlessCastleStatusShort.iClanID1;
	iClanBCID2 = sBlessCastleStatusShort.iClanID2;
	iClanBCID3 = sBlessCastleStatusShort.iClanID3;

	SQLWriteBlessCastleStatus();
}

void BlessCastleServer::SetBlessCastleOwnerClanID( int iClanID1, int iClanID2, int iClanID3 )
{
	INFO( "SetBlessCastleOwnerClanID = %d", iClanID1 );

	psBlessCastleStatusData->iClanID = iClanID1;
	iClanBCID1 = iClanID1;
	iClanBCID2 = iClanID2;
	iClanBCID3 = iClanID3;

	SQLWriteBlessCastleStatus();
}

void BlessCastleServer::NetSendBlessCastleOwnerClanID( int iClanID1, int iClanID2, int iClanID3 )
{
	if( GAME_SERVER )
	{
		PacketNetBCNewOwnerClanID sPacketNet;
		sPacketNet.iLength			= sizeof( sPacketNet );
		sPacketNet.iHeader			= PKTHDR_NetBCNewOwnerClanID;
		sPacketNet.iClanID1			= iClanID1;
		sPacketNet.iClanID2			= iClanID2;
		sPacketNet.iClanID3			= iClanID3;

		NetConnection * nc = NETSERVER->GetLoginServerConnection();
		if ( nc )
			NETSERVER->SendPacket( nc->pcSocketData, &sPacketNet );
	}
}

void BlessCastleServer::UpdateIndividualScores( BOOL bReset )
{
	for ( int i = 0; i < 1024; i++ )
	{
		UserData * pc = &USERSDATA[i];
		User * pcUser = USERDATATOUSER( pc );

		if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle && pcUser->iBlessCastleScore > 0 )
		{
			if ( bReset )
				pcUser->iBlessCastleScore = 0;

			SQLUpdateUserScore( pcUser );
		}
	}
}

void BlessCastleServer::HandleIndividualScore()
{
	char szAccountName[3][32] = { 0 };
	char szCharacterName[3][32] = { 0 };
	int iCharacterScore[3] = { 0 };
	char szGold[32] = { 0 };

	UpdateIndividualScores();

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 3 [AccountName],[Name],[BlessCastleScore] FROM CharacterInfo ORDER BY BlessCastleScore DESC" ) )
		{
			int i = 0;

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szAccountName[i], 32 );
					pcDB->GetData( 2, PARAMTYPE_String, szCharacterName[i], 32 );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iCharacterScore[i] );

					i++;
				}
			}
		}

		pcDB->Close();

		//Conquest Mode?
		if ( (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) || (GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) )
		{
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> Player Top Scores:" );

			if ( szCharacterName[0] && iCharacterScore[0] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 3'000'000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szCharacterName[0] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[0] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[0], "GG101", 0, 3'000'000);
			}
			if ( szCharacterName[1] && iCharacterScore[1] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 2'000'000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szCharacterName[1] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[1] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[1], "GG101", 0, 2'000'000);
			}
			if ( szCharacterName[2] && iCharacterScore[2] > 0 )
			{
				STRINGCOPY( szGold, FormatNumber( 1'000'000 ) );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szCharacterName[2] );
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( iCharacterScore[2] ), szGold );
				ITEMSERVER->AddItemOpenBox( szAccountName[2], "GG101", 0, 1'000'000);
			}
		}
	}

	UpdateIndividualScores( TRUE );
}

void BlessCastleServer::SendIndividualScore()
{
	PacketBlessCastleTopIndividual s, s2;

	ZeroMemory( &s, sizeof( PacketBlessCastleTopIndividual ) );
	ZeroMemory( &s2, sizeof( PacketBlessCastleTopIndividual ) );

	s.iLength = sizeof( PacketBlessCastleTopIndividual );
	s.iHeader = PKTHDR_BlessCastleIndividualScore;
	s.iMode = GetSiegeWarMode();

	s2.iLength = sizeof( PacketBlessCastleTopIndividual );
	s2.iHeader = PKTHDR_BlessCastleIndividualScore;
	s2.iMode = GetSiegeWarMode();

	int i = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TOP 3 [Name],[JobCode],[BlessCastleScore] FROM CharacterInfo ORDER BY BlessCastleScore DESC" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, s.szCharacterName[i], 32 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &s.iClass[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &s.iDamage[i] );

					i++;
				}
			}
		}

		pcDB->Close();
	}

	if ( i > 0 )
	{
		if ( CanShowScore() )
		{
			SENDPACKETSTAGE( &s, MAPID_BlessCastle );
		}
		else
		{
			for ( int i = 0; i < 1024; i++ )
			{
				UserData * pc = &USERSDATA[i];

				if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle )
				{
					if ( pc->iGameLevel >= GAMELEVEL_Two )
						SENDPACKET( USERDATATOUSER(pc), &s );
					else
						SENDPACKET( USERDATATOUSER(pc), &s2 );
				}
			}
		}
	}
}

BOOL BlessCastleServer::Main()
{
	if ( LOGIN_SERVER )
		return TRUE;

	psBlessCastleStatusData->iCounter++;

	Map * pcMap = MAPSDATA + MAPID_BlessCastle;

	int iUpdate = 0;

	//Fountain Main...
	BlessCastleServer::MainFountain();

	if ( psBlessCastlePVPLeague->iMode )
	{
		if ( psBlessCastlePVPLeague->dwEndTime && (psBlessCastlePVPLeague->dwEndTime < TICKCOUNT) )
			psBlessCastlePVPLeague->iMode = 0;
		else
		{
			if ( psBlessCastleStatusData->iCastleMode == 0 )
			{
				if ( psBlessCastlePVPLeague->dwRestartTime && (psBlessCastlePVPLeague->dwRestartTime < TICKCOUNT) )
					BLESSCASTLESERVER->Start();
			}
		}
	}

	if ( psBlessCastleStatusData->iCastleMode == 0 )
	{
		psBlessCastleStatusData->saTimeSec[0] = 0;
		psBlessCastleStatusData->saTimeSec[1] = 0;
		return FALSE;
	}

	if ( psBlessCastleStatusData->iCastleMode == 3 )
	{
		if ( psBlessCastleStatusData->dwBattleOverTime < TICKCOUNT )
		{
			//Clear
			for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
			{
				UnitData * pc = pcMap->pcaUnitData[i];
				if ( pc && pc->psaSiegeWarDataList )
				{
					pc->psaSiegeWarDataList = NULL;
				}
			}

			ZeroMemory ( psBlessCastleStatusData->psaDamageData, sizeof ( AttackDamageData ) * MAX_DAMAGEDATA );
			psBlessCastleStatusData->psaDamageData = NULL;

			DEBUG ( "BC - Battle ended!" );
			DEBUG ( "BC - Castle mode: %d", psBlessCastleStatusData->iCastleMode );
			BlessCastleServer::EndBattle ();
			DEBUG ( "BC - Castle mode: %d", psBlessCastleStatusData->iCastleMode );
		}
		return TRUE;
	}

	if ( (psBlessCastleStatusData->iCounter & 0x1F) == 0 )
	{
		//DEBUG ( "BC - Castle mode: %d", psBlessCastleStatusData->iCastleMode );

		psBlessCastleStatusData->saTimeSec[0] = (short)((TICKCOUNT - psBlessCastleStatusData->dwStartTime) / 1000);
		psBlessCastleStatusData->saTimeSec[1] = (short)((psBlessCastleStatusData->dwBattleOverTime - psBlessCastleStatusData->dwStartTime) / 1000);

		psBlessCastleStatusData->baSoldierCount[0] = 0;
		psBlessCastleStatusData->baSoldierCount[1] = 0;
		psBlessCastleStatusData->baSoldierCount[2] = 0;
		psBlessCastleStatusData->baSoldierCount[3] = 0;

		psBlessCastleStatusData->baCrystalTowerCount[0] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[1] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[2] = 0;
		psBlessCastleStatusData->baCrystalTowerCount[3] = 0;

		//Tick
		if ( dwTickUpdateUsersScore < TICKCOUNT )
		{
			UpdateIndividualScores();

			SendIndividualScore();

			dwTickUpdateUsersScore = TICKCOUNT + 5000; //update every 5 seconds
		}

		BOOL bCastleDoor = FALSE;
		UnitData * pcTower = NULL;
		int iValhallaCount = 0; //Further New BC?
		int iCrystalTowerCount = 0;


		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pc = pcMap->pcaUnitData[i];

			if ( pc && pc->GetCurrentHealth() > 0 )
			{
				//BC Door Alive?
				if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_DOOR )
				{
					//Set
					bCastleDoor = TRUE;

					//Conquest Mode?
					if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
					{
						//Set Door to score on Siege War..
						if ( psBlessCastleStatusData->iDefenseLevel == 0 &&
							 psBlessCastleStatusData->psaDamageData )
						{
							if ( pc->psaSiegeWarDataList == NULL )
								pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
						}
					}

					*(UnitData**)0x08B64054 = pc;
				}
				else if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B )
				{
					pcTower = pc;
					iValhallaCount++;

					if ( psBlessCastleStatusData->psaDamageData &&
						 pc->psaSiegeWarDataList == NULL )
					{
						pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
					}
				}
				else if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_N ||
					pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R ||
					pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G ||
					pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B )
				{

					switch ( pc->sCharacterData.iMonsterEffectID )
					{
						case MONSTEREFFECTID_CASTLE_CRYSTAL_R:
							if ( psBlessCastleStatusData->baCrystalTowerCount[0] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[0]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_G:
							if ( psBlessCastleStatusData->baCrystalTowerCount[1] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[1]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_B:
							if ( psBlessCastleStatusData->baCrystalTowerCount[2] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[2]++;
							break;

						case MONSTEREFFECTID_CASTLE_CRYSTAL_N:
							if ( psBlessCastleStatusData->baCrystalTowerCount[3] < 100 )
								psBlessCastleStatusData->baCrystalTowerCount[3]++;
							break;

						default:
							break;
					}

					iCrystalTowerCount++;

					if ( psBlessCastleStatusData->psaDamageData &&
						 pc->psaSiegeWarDataList == NULL )
					{
						pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
					}
				}
				else if ( pc->sCharacterData.iOwnerID )
				{
					BOOL bSoldier = FALSE;

					switch ( pc->sCharacterData.iMonsterEffectID )
					{
						case MONSTEREFFECTID_CASTLE_SOLDER_A:
							if ( psBlessCastleStatusData->baSoldierCount[0] < 100 )
								psBlessCastleStatusData->baSoldierCount[0]++;

							bSoldier = TRUE;
							break;
						case MONSTEREFFECTID_CASTLE_SOLDER_B:
							if ( psBlessCastleStatusData->baSoldierCount[1] < 100 )
								psBlessCastleStatusData->baSoldierCount[1]++;

							bSoldier = TRUE;
							break;
						case MONSTEREFFECTID_CASTLE_SOLDER_C:
							if ( psBlessCastleStatusData->baSoldierCount[2] < 100 )
								psBlessCastleStatusData->baSoldierCount[2]++;

							bSoldier = TRUE;
							break;

						default:
							break;
					}

					if ( bSoldier )
					{
						//Conquest Mode?
						if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest || GetSiegeWarMode () == SIEGEWARMODE_Normal)
						{
							//Set Soldiers to score on Siege War..
							if ( psBlessCastleStatusData->psaDamageData &&
								 pc->psaSiegeWarDataList == NULL )
							{
								pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
							}
						}
					}
				}
				else if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_A ||
					      pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_B ||
					      pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_C )
				{
					//Conquest Mode?
					if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest || GetSiegeWarMode () == SIEGEWARMODE_Normal )
					{
						if ( psBlessCastleStatusData->psaDamageData &&
							 pc->psaSiegeWarDataList == NULL )
						{
							pc->psaSiegeWarDataList = psBlessCastleStatusData->psaDamageData;
						}
					}
				}

				pc->dwLastActiveTime	= TICKCOUNT;
				pc->iAliveCounter		= 256;
			}
		}

		//Door is still alive
		if ( bCastleDoor)
		{
			if (GetSiegeWarMode() == SIEGEWARMODE_TheConquest)
			{
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User** pcaUserInGame = USERSERVER->pcaUserInGame;
				for (UINT i = 0; i < uUsersInGame; i++)
				{
					User* pcUser = pcaUserInGame[i];

					if (pcUser->IsInGame() && (pcUser->GetMapID() == MAPID_BlessCastle) && pcUser->pcUserData) //&& (pcUser->GetGameLevel() < GAMELEVEL_Two
					{
						//if user iZ (up/down) position is less than -6553600 (inside the castle) then spawn user at start of field
						if (pcUser->pcUserData->sPosition.iZ < ((-25580) << 8))
						{
							CHARACTERSERVER->SetPositionWarp(pcUser->pcUserData, 8859282, -5931214, MAPID_BlessCastle);
						}
					}
				}
			}
			else if (GetSiegeWarMode() == SIEGEWARMODE_Normal)
			{
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User** pcaUserInGame = USERSERVER->pcaUserInGame;
				for (UINT i = 0; i < uUsersInGame; i++)
				{
					User* pcUser = pcaUserInGame[i];

					if (pcUser->IsInGame() &&
						pcUser->GetMapID() == MAPID_BlessCastle &&
						pcUser->pcUserData &&
						pcUser->pcUserData->iClanID != psBlessCastleStatusData->iClanID &&
						pcUser->GetGameLevel() <= GAMELEVEL_Two)
					{
						//if user iZ (up/down) position is less than -6553600 (inside the castle) then spawn user at start of field
						//we will only be checking for players that is not in the Blessed Castle Clan

						if (pcUser->pcUserData->sPosition.iZ < ((-25580) << 8))
						{
							CHARACTERSERVER->SetPositionWarp(pcUser->pcUserData, 8859282, -5931214, MAPID_BlessCastle);
						}
					}
				}
			}
		}

		if ( ( psBlessCastleStatusData->iDefenseLevel == 0 ) && ( bCastleDoor == FALSE ) )
		{
			psBlessCastleStatusData->iDefenseLevel++;
			INFO ( "psBlessCastleStatusData->iDefenseLevel = %d", psBlessCastleStatusData->iDefenseLevel );
		}

		BOOL bFinished = FALSE;

		if ( iValhallaCount == 0 )
		{
			BlessCastleServer::End( FALSE );

			if ( psBlessCastleStatusData->sPacketTopData.iaClanID[0] )
			{
				psBlessCastleStatusData->iClanID = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[0] );
				iClanBCID1 = psBlessCastleStatusData->iClanID;


				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[1] )
					iClanBCID2 = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[1] );

				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[2] )
					iClanBCID3 = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[2] );

				KillBuffs();

				ResetUserScore();

				BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( psBlessCastleStatusData->iClanID, iClanBCID2, iClanBCID3 );

				iUpdate = 2;

				BLESSCASTLESERVER->SQLWriteBlessCastleStatus();

				bFinished = TRUE;
			}
		}
		else if ( psBlessCastleStatusData->dwBattleOverTime < TICKCOUNT )
		{
			//Change to new Mode = TRUE
			BOOL bUnitsAlive	= FALSE;

			//Conquest Mode? Units Alive
			if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
				bUnitsAlive		= TRUE;

			BlessCastleServer::End( bUnitsAlive );

			if ( psBlessCastleStatusData->sPacketTopData.iaClanID[0] )
			{
				
				//CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "IS UPDATING!");

				//Conquest Mode?
				if ( (GetSiegeWarMode() == SIEGEWARMODE_TheConquest) )
				{
					//No Winner
					psBlessCastleStatusData->iClanID = 0;
				}
				else if ( (GetSiegeWarMode() == SIEGEWARMODE_TheMassacre) || (GetSiegeWarMode() == SIEGEWARMODE_Normal))
				{
					//Otherwise, on Normal Mode...
					//psBlessCastleStatusData->iClanID = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[0] );
					//iClanBCID1 = psBlessCastleStatusData->iClanID;
					//it should work as is wanna try
					// "normal" is siege 0, is that what happens weekly then? Or is that Mode 1 Conquest.
					//mode 1 in thiory resets the leader

					if ( psBlessCastleStatusData->sPacketTopData.iaClanID[1] )
						iClanBCID2 = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[0] );

					if ( psBlessCastleStatusData->sPacketTopData.iaClanID[2] )
						iClanBCID3 = BlessCastleServer::GetClanCodeByID( psBlessCastleStatusData->sPacketTopData.iaClanID[1] );
				}

				KillBuffs();

				ResetUserScore();

				BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( psBlessCastleStatusData->iClanID, iClanBCID2, iClanBCID3 );

				iUpdate = 2;

				BLESSCASTLESERVER->SQLWriteBlessCastleStatus();

				bFinished = TRUE;
			}
		}

		//Finished?
		if ( bFinished )
		{
			DEBUG ( "Finished" );

			//Update Crowns
			SendTopClansSyncToUsers();

			//Announce Winners
			if (GetSiegeWarMode() == SIEGEWARMODE_Normal)
			{
				char szAccountName[32] = { 0 };
				char szClanName[32] = { 0 };

				//1st
				if (GetClanName(psBlessCastleStatusData->sPacketTopData.iaClanID[0], szClanName))
				{
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> Golden Crown: %s!", szClanName);
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szClanName);
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> %s Points", FormatNumber(psBlessCastleStatusData->sPacketTopData.iaDamage[0]));
				}

				//2nd
				if (GetClanName(psBlessCastleStatusData->sPacketTopData.iaClanID[1], szClanName))
				{
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> Silver Crown: %s", szClanName);
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> %s Points", FormatNumber(psBlessCastleStatusData->sPacketTopData.iaDamage[1]));
				}

				//3rd
				if (GetClanName(psBlessCastleStatusData->sPacketTopData.iaClanID[2], szClanName))
				{
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> Bronze Crown: %s", szClanName);
					CHATSERVER->SendChatAllEx(CHATCOLOR_Global, "BlessCastle> %s Points", FormatNumber(psBlessCastleStatusData->sPacketTopData.iaDamage[2]));
				}

				HandleIndividualScore();

				//Notify Users
				CHATSERVER->SendChatAll(CHATCOLOR_Global, "BlessCastle> All crowns have been updated!");
			}

			//Handle Prizes
			else if ( GetSiegeWarMode() == SIEGEWARMODE_TheConquest )
			{
				char szAccountName[32] = { 0 };
				char szClanName[32] = { 0 };
				char szGold[32] = { 0 };

				//1st
				int iGold = 0;

				//5,000,000 every 10,000 points
				iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[0] / 10000) * 250'000);

				if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[0], szAccountName ) )
				{
					if ( iGold > MAX_GOLD_PERBOX )
					{
						int iGoldR = iGold;
						int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
						for ( int i = 0; i < iCount; i++ )
						{
							int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
							iGoldR -= MAX_GOLD_PERBOX;
						}
					}
					else
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
				}

				if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[0], szClanName ) )
				{
					STRINGCOPY( szGold, FormatNumber( iGold ) );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> Winner Clan: %s!", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[0] ), szGold );
				}

				//2nd
				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[1] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[1] / 10000) * 250'000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[1], szAccountName ) )
					{
						if ( iGold > MAX_GOLD_PERBOX )
						{
							int iGoldR = iGold;
							int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
							for ( int i = 0; i < iCount; i++ )
							{
								int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
								ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
								iGoldR -= MAX_GOLD_PERBOX;
							}
						}
						else
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
					}

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[1], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[1] ), szGold );
					}
				}

				//3rd
				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[2] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[2] / 10000) * 250'000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[2], szAccountName ) )
					{
						if ( iGold > MAX_GOLD_PERBOX )
						{
							int iGoldR = iGold;
							int iCount = (iGold / MAX_GOLD_PERBOX) + 1;
							for ( int i = 0; i < iCount; i++ )
							{
								int j = iGoldR > MAX_GOLD_PERBOX ? MAX_GOLD_PERBOX : iGoldR;
								ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, j );
								iGoldR -= MAX_GOLD_PERBOX;
							}
						}
						else
							ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );
					}

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[2], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[2] ), szGold );
					}
				}

				HandleIndividualScore();

				//Notify Users
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> All prizes delivered to the leaders!" );
			}
			else if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
			{
				char szAccountName[32] = { 0 };
				char szClanName[32] = { 0 };
				char szGold[32] = { 0 };

				//1st
				int iGold = 0;

				//5,000,000 every 10,000 points
				iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[0] / 5000) * 250'000);

				if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[0], szAccountName ) )
					ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );


				if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[0], szClanName ) )
				{
					STRINGCOPY( szGold, FormatNumber( iGold ) );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> Winner Clan: %s!", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 1st Place: %s", szClanName );
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[0] ), szGold );
				}

				//2nd
				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[1] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[1] / 5000) * 250'000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[1], szAccountName ) )
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[1], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 2nd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[1] ), szGold );
					}

				}

				//3rd
				if ( psBlessCastleStatusData->sPacketTopData.iaClanID[2] )
				{
					iGold = ((psBlessCastleStatusData->sPacketTopData.iaDamage[2] / 5000) * 250'000);

					if ( GetClanLeaderAccount( psBlessCastleStatusData->sPacketTopData.iaClanID[2], szAccountName ) )
						ITEMSERVER->AddItemOpenBox( szAccountName, "GG101", 0, iGold );

					if ( GetClanName( psBlessCastleStatusData->sPacketTopData.iaClanID[2], szClanName ) )
					{
						STRINGCOPY( szGold, FormatNumber( iGold ) );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> 3rd Place: %s", szClanName );
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "BlessCastle> %s Points, %s gp!", FormatNumber( psBlessCastleStatusData->sPacketTopData.iaDamage[2] ), szGold );
					}
				}

				HandleIndividualScore();

				//Notify Users
				CHATSERVER->SendChatAll( CHATCOLOR_Global, "BlessCastle> All prizes delivered to the leaders!" );
			}
		}

		psBlessCastleStatusData->pcUnitDataTower = pcTower;

		if ( pcTower )
		{
			if ( iCrystalTowerCount > 0 )
			{
				pcTower->SetCurrentHealthToMax();
			}
			else
			{
				//if ( pcTower->psaDamageUsersData == NULL )
				//{
				//	//if ( psBlessCastleStatusData->psaDamageData )
				//	//{
				//		//pcTower->psaDamageUsersData = psBlessCastleStatusData->psaDamageData;
				//
				//		//psBlessCastleStatusData->psaDamageData = NULL;
				//	//}
				//	//else
				//	//{
				//	//	pcTower->psaDamageUsersData = new AttackDamageData[MAX_DAMAGEDATA];
				//	//
				//	//	if ( pcTower->psaDamageUsersData )
				//	//		ZeroMemory( pcTower->psaDamageUsersData, sizeof( AttackDamageData ) * MAX_DAMAGEDATA );
				//	//}
				//}
			}


			//BlessCastleServer::GetTop10ClanDamage ( psBlessCastleStatusData->psaDamageData, &psBlessCastleStatusData->sPacketTopData );
			if ( bFinished == FALSE && bCastleDoor == FALSE && BLESSCASTLEHANDLER->GetBlessCastleStatus() )
				SelectDamageClanTop10 ( psBlessCastleStatusData );
		}
	}

	if ( psBlessCastleStatusData->sPacketTopData.iHeader )
	{
		//INFO ( "Sending psBlessCastleStatusData->sPacketTopData" );

		for ( int i = (psBlessCastleStatusData->iCounter & 0x1F); i < 1024; i += 0x20 )
		{
			UserData * pc = &USERSDATA[i];

			if ( pc->pcSocketData && pc->iID && pc->iMapID == MAPID_BlessCastle )
			{
				if ( CanShowScore() || (pc->iGameLevel >= GAMELEVEL_Two) )
				{
					SENDPACKET( USERDATATOUSER(pc), &psBlessCastleStatusData->sPacketTopData );
				}

				if ( iUpdate > 0 )
				{
					BlessCastleServer::SendBlessCastleDataToLoginServer( pc );
					iUpdate--;
				}
			}
		}
	}

	return TRUE;
}

//int	rsSelectDamageClanTop10(smCHAR *lpChar, rsUSER_LIST_TOP10 *lpUserListDamageTop10)
BOOL BlessCastleServer::SelectDamageClanTop10 ( BlessCastleStatusData * lpBlessCasterStatus )
{
	if ( psBlessCastleStatusData->psaDamageData == NULL )
		return FALSE;

	AttackDamageData lpAttackDamageList[MAX_DAMAGEDATA];
	CopyMemory ( &lpAttackDamageList, psBlessCastleStatusData->psaDamageData, sizeof ( AttackDamageData ) * MAX_DAMAGEDATA );

	PacketBlessCastleTopData * lpUserListDamageTop10 = &psBlessCastleStatusData->sPacketTopData;

	int cnt, cnt2;
	int ChkCnt;
	int	TotalDamage = 0;

	if (lpUserListDamageTop10 == NULL )
		return FALSE;

	ZeroMemory ( lpUserListDamageTop10, sizeof ( PacketBlessCastleTopData ) );

	AttackDamageData * ptr = NULL;

	for ( cnt2 = 0; cnt2 < MAX_DAMAGEDATA; cnt2++ )
	{
		TotalDamage += lpAttackDamageList[cnt2].iDamage;
	}

	for ( cnt = 0; cnt < 10; cnt++ )
	{
		ChkCnt = -1;
		for ( cnt2 = 0; cnt2 < MAX_DAMAGEDATA; cnt2++ )
		{
			if ( lpAttackDamageList[cnt2].iID && CHARACTERSERVER->GetClanCode ( lpAttackDamageList[cnt2].iID ) != lpBlessCasterStatus->iClanID &&
				 lpAttackDamageList[cnt2].iDamage > lpUserListDamageTop10->iaDamage[cnt] )
			{
				lpUserListDamageTop10->iaUserID[cnt] = lpAttackDamageList[cnt2].iID;
				lpUserListDamageTop10->iaDamage[cnt] = lpAttackDamageList[cnt2].iDamage;
				lpUserListDamageTop10->iaCounter[cnt] = lpAttackDamageList[cnt2].iCounter;

				ChkCnt = cnt2;
			}
		}

		if ( ChkCnt >= 0 )
		{
			lpAttackDamageList[ChkCnt].iID = 0;
		}
	}

	( (DWORD *)lpUserListDamageTop10->baCrystalTowerCount )[0] = ( (DWORD *)lpBlessCasterStatus->baCrystalTowerCount )[0];

	lpUserListDamageTop10->dwUpdateTime = TICKCOUNT;
	lpUserListDamageTop10->dwCharCode = 0; // lpChar->dwObjectSerial;

	lpUserListDamageTop10->iHeader = PKTHDR_BLESSCASTLE_INFO;
	lpUserListDamageTop10->iLength = sizeof ( PacketBlessCastleTopData );
	lpUserListDamageTop10->tCode = SERVER_CODE;
	lpUserListDamageTop10->dwObjectSerial = 2;
	lpUserListDamageTop10->dwTotalDamage = TotalDamage;

	if ( lpBlessCasterStatus->dwScoreLogTime && lpBlessCasterStatus->dwScoreLogTime < TICKCOUNT )
	{
		lpBlessCasterStatus->dwScoreLogTime = TICKCOUNT + 5 * 60 * 1000;
		//log stuff?
	}

	return TRUE;
}

void BlessCastleServer::SendUserScore( User * pcUser )
{
	if ( psBlessCastleStatusData->iCastleMode <= 0 )
		return;

	PacketBlessCastleUserScoreUpdate s;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [BlessCastleScore] FROM CharacterInfo WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &pcUser->iBlessCastleScore );

				s.iLength = sizeof( PacketBlessCastleUserScoreUpdate );
				s.iHeader = PKTHDR_BlessCastleUserScoreUpdate;
				s.iUserScore = pcUser->iBlessCastleScore;

				SENDPACKET( pcUser, &s );
			}
		}

		pcDB->Close();
	}
}

NAKED int BlessCastleServer::GetClanCodeByID( int iID )
{
	JMP( 0x00557FF0 );
}

ESiegeWarMode BlessCastleServer::GetSiegeWarMode()
{
	return (ESiegeWarMode)EVENT_BLESSCASTLE_MODE;
}

void BlessCastleServer::SetSiegeWarMode( ESiegeWarMode iNewMode )
{
	EVENT_BLESSCASTLE_MODE = iNewMode;
	WRITEDWORD( 0x08B86750, iNewMode );
}

BOOL BlessCastleServer::Start()
{
	//iClanBCID1 = 0;
	//iClanBCID2 = 0;
	//iClanBCID3 = 0;

	//BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( 0, 0, 0 );

	SendTopClansSyncToUsers();

	Map * pcBCMap = MAPSDATA + MAPID_BlessCastle;

	if ( EVENT_BLESSCASTLE == 2 )
	{
		psBlessCastlePVPLeague->iMode		= 1;
		psBlessCastlePVPLeague->dwBeginTime = TICKCOUNT;

		if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
			psBlessCastlePVPLeague->dwEndTime = TICKCOUNT + (30 * 60 * 1000);
		else
			psBlessCastlePVPLeague->dwEndTime = TICKCOUNT + (120 * 60 * 1000);

		EVENT_BLESSCASTLE = 1;
	}

	psBlessCastleStatusData->iCastleMode	= 1;
	psBlessCastleStatusData->iCounter		= 0;
	psBlessCastleStatusData->iDefenseLevel	= 0;

	if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
		psBlessCastleStatusData->dwBattleOverTime = TICKCOUNT + (30 * 60 * 1000);
	else
		psBlessCastleStatusData->dwBattleOverTime = TICKCOUNT + (120 * 60 * 1000);

	psBlessCastleStatusData->dwStartTime	= TICKCOUNT;
	psBlessCastleStatusData->saTimeSec[0]	= 0;
	psBlessCastleStatusData->saTimeSec[1]	= 0;
	psBlessCastleStatusData->dwScoreLogTime	= TICKCOUNT + (5 * 60 * 1000);

	if ( psBlessCastlePVPLeague->iMode )
	{
		psBlessCastlePVPLeague->dwRestartTime = 0;
	}

	if ( psBlessCastleStatusData->psaDamageData == NULL )
	{
		INFO ( "Init psBlessCastleStatusData->psaDamageData" );
		psBlessCastleStatusData->psaDamageData = new AttackDamageData[ATTACK_DAMAGE_MAX];
	}
	if ( psBlessCastleStatusData->psaDamageData )
	{
		INFO ( "Zero memory psBlessCastleStatusData->psaDamageData" );
		ZeroMemory( psBlessCastleStatusData->psaDamageData, sizeof( AttackDamageData ) * ATTACK_DAMAGE_MAX );
	}

	if ( psBlessCastleStatusData->iClanID == 0 )
	{
		psBlessCastleStatusData->sSettings.caGuardAmount[0] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[1] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[2] = 20;
		psBlessCastleStatusData->sSettings.caGuardAmount[3] = 0;

		//Towers
		for (int i = 0; i < 6; i++)
			psBlessCastleStatusData->sSettings.saTowerType[i] = Dice::RandomI(1, 3);
	}

	ZeroMemory( &psBlessCastleStatusData->sPacketTopData, sizeof( PacketBlessCastleTopData ) );

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITSDATA + i;

		if ( pc->bActive && pc->pcMap == pcBCMap )
		{
			pc->Free();

			if ( MAPSERVER->DelMonsterSpawn( pcBCMap, pc ) )
				pcBCMap->iNumAliveUnitData--;
		}
	}

	CharacterData * psaCharacterData	= *(CharacterData**)0x07AB3050;
	int iCounterCharacterData			= UNITINFODATA->GetTotalUnitEnemy ();

	CharacterData * psCastleDoor	= NULL;
	CharacterData * psValhalla		= NULL;
	CharacterData * psTowerR		= NULL;
	CharacterData * psTowerG		= NULL;
	CharacterData * psTowerB		= NULL;
	CharacterData * psTowerN		= NULL;
	CharacterData * psCastleGuardA	= NULL;
	CharacterData * psCastleGuardB	= NULL;
	CharacterData * psCastleGuardC	= NULL;
	CharacterData * psSetTower		= NULL;

	for ( int i = 0; i < iCounterCharacterData; i++ )
	{
		CharacterData * ps = psaCharacterData + i;

		if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_DOOR )
			psCastleDoor = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B )
			psValhalla = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R )
			psTowerR = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G )
			psTowerG = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B )
			psTowerB = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_N )
			psTowerN = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_A )
			psCastleGuardA = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_B )
			psCastleGuardB = ps;
		else if ( ps->iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_C )
			psCastleGuardC = ps;
	}

	if ( psCastleDoor )
	{
		MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleDoor, sCastleDoorPosition.iX << 8, sCastleDoorPosition.iY << 8, sCastleDoorPosition.iZ << 8 );
	}

	if ( psValhalla )
	{
		MAPSERVER->SpawnMonsterBC( pcBCMap, psValhalla, sCastleValhallaPosition.iX << 8, sCastleValhallaPosition.iY << 8, sCastleValhallaPosition.iZ << 8 );
	}

	for ( int i = 0; i < _countof( saBlessCastleTowerPosition ); i++ )
	{
		if ( saBlessCastleTowerPosition[i].iX == 0 )
			break;

		psSetTower = psTowerN;

		switch ( psBlessCastleStatusData->sSettings.saTowerType[i] )
		{
			case 1: //Ice
				psSetTower = psTowerB;
				break;
			case 2: //Lightning
				psSetTower = psTowerG;
				break;
			case 3: //Fire
				psSetTower = psTowerR;
				break;
			default:
				break;
		}

		if ( psSetTower )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psSetTower, saBlessCastleTowerPosition[i].iX << 8, saBlessCastleTowerPosition[i].iY << 8, saBlessCastleTowerPosition[i].iZ << 8 );
	}

	for ( int i = 0; i < 3; i++ )
	{
		if ( psBlessCastleStatusData->sSettings.caGuardAmount[i] > 20 )
			psBlessCastleStatusData->sSettings.caGuardAmount[i] = 20;
	}

	if ( psCastleGuardA )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[0]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardA, saCastleGuardPositionA[i].iX << 8, saCastleGuardPositionA[i].iY << 8, saCastleGuardPositionA[i].iZ << 8 );
	}
	if ( psCastleGuardB )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[1]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardB, saCastleGuardPositionB[i].iX << 8, saCastleGuardPositionB[i].iY << 8, saCastleGuardPositionB[i].iZ << 8 );
	}
	if ( psCastleGuardC )
	{
		for ( int i = 0; i < psBlessCastleStatusData->sSettings.caGuardAmount[2]; i++ )
			MAPSERVER->SpawnMonsterBC( pcBCMap, psCastleGuardC, saCastleGuardPositionC[i].iX << 8, saCastleGuardPositionC[i].iY << 8, saCastleGuardPositionC[i].iZ << 8 );
	}

	CHATSERVER->SendChatAllUsersInStage( MAPID_BlessCastle, CHATCOLOR_Global, "BlessCastle> Siege War started! Good Luck for all clans!" );
	STATUS ( "Bless castle mode: %d", GetSiegeWarMode () );

	NetSendBlessCastleStatus();

	return TRUE;
}

NAKED BOOL BlessCastleServer::End( BOOL bUnitsAlive )
{
	JMP( 0x00557A70 );
}

NAKED BOOL BlessCastleServer::EasySiegeWar()
{
	JMP( 0x00557CA0 );
}

NAKED void BlessCastleServer::EndBattle()
{
	JMP( 0x00557B60 );
}

NAKED void BlessCastleServer::GetTop10ClanDamage( UnitData * pcUnitData, PacketBlessCastleTopData * psPacket )
{
	JMP( 0x0055D3C0 );
}

NAKED void BlessCastleServer::SendBlessCastleDataToLoginServer( UserData * pcUserData )
{
	JMP( 0x00557DD0 );
}

NAKED void BlessCastleServer::MainFountain()
{
	JMP( 0x0055D540 );
}

void BlessCastleServer::KillBuffs()
{
	for ( int i = 0; i < 1024; i++ )
	{
		UserData * pc = &USERSDATA[i];

		if ( pc->pcSocketData && pc->iID )
		{
			pc->dwBlessSkill_Code	= 0;
			pc->dwBlessSkillTime	= 0;
			pc->dwBlessSkillParam	= 0;
		}
	}
}

void BlessCastleServer::ResetUserScore()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterInfo SET [BlessCastleScore]=0" ) )
		{
			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

int BlessCastleServer::GetClanTop( int iTopNumber )
{
	if ( iTopNumber == 1 )
		return iClanBCID1;
	else if ( iTopNumber == 2 )
		return iClanBCID2;

	return iClanBCID3;
}

void BlessCastleServer::SendTopClansSync( User * pcUser )
{
	PacketBlessCastleTopClanSync sPacket;
	sPacket.iLength = sizeof( PacketBlessCastleTopClanSync );
	sPacket.iHeader = PKTHDR_BlessCastleTopClanSync;
	sPacket.iaClanID[0] = iClanBCID1;
	sPacket.iaClanID[1] = iClanBCID2;
	sPacket.iaClanID[2] = iClanBCID3;
	SENDPACKET( pcUser, &sPacket );
}

BOOL BlessCastleServer::GetClanLeaderAccount( int iClanID, char szLeaderAccount[32] )
{
	BOOL bRet = FALSE;

	szLeaderAccount[0] = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AccountName FROM ClanList WHERE IconID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClanID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, szLeaderAccount, 32 );

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL BlessCastleServer::GetClanName( int iClanID, char szName[32] )
{
	BOOL bRet = FALSE;

	szName[0] = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ClanName FROM ClanList WHERE IconID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iClanID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, szName, 32 );

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

BOOL BlessCastleServer::CanShowScore ()
{
	BOOL bRet = TRUE;

	if ( GetSiegeWarMode () == SIEGEWARMODE_TheMassacre )
	{
		bRet = FALSE;

		DWORD dwTime = psBlessCastleStatusData->dwBattleOverTime - psBlessCastleStatusData->dwStartTime;

		//5 minutes or less than? Show score
		if ( ( 5 * 60 * 1000 ) >= dwTime )
			bRet = TRUE;
	}

	return bRet;
}

void BlessCastleServer::SendTopClansSyncToUsers()
{
	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		SendTopClansSync( USERSERVER->pcaUserInGame[i] );
}
