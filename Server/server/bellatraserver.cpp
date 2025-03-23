#include "stdafx.h"
#include "bellatraserver.h"
#include "Logger.h"
#include "Utilities.h"

DWORD dwClanBellatraUpdateTime = 0;

WORD wLastDayResetSOD = 0;

const int iRankingPointsMinNumOfSurvivorsReq = 3;

BellatraServer::BellatraServer()
{
	bSodIsActive = FALSE;
}


BellatraServer::~BellatraServer()
{
}

//Temp fix for now. In the future we'd want to have one clan database
//for both ASP (ideally PHP) and server code
void BellatraServer::SyncOldClanTablesWithNew()
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB_2);
	if (pcDB->Open())
	{
		std::string query = std::string(
			"UPDATE [UserDB].[dbo].[CharacterInfo]"
			" SET ClanID = CL.ID"
			" FROM[ClanDB].[dbo].[UL] UL"
			" INNER JOIN[ClanDB].[dbo].[ClanList] AS CL"
			" ON UL.ClanName = CL.ClanName"
			" INNER JOIN[UserDB].[dbo].[CharacterInfo] AS CI"
			" ON CI.Name = UL.ChName collate SQL_Latin1_General_CP1_CI_AS"
			" WHERE CL.ID <> CI.ClanID OR ClanID = 0");

		pcDB->Prepare(query.c_str()) && pcDB->Execute();
		pcDB->Clear();

		query = std::string(
			"UPDATE [ClanDB].[dbo].[ClanList]"
			" SET ClanLeader = t1.ClanZang"
			" FROM[ClanDB].[dbo].[ClanList] t2"
			" INNER JOIN[ClanDB].[dbo].[CL] t1"
			" ON t1.ClanName = t2.ClanName"
			" WHERE t2.ClanLeader <> t1.ClanZang");

		pcDB->Prepare(query.c_str()) && pcDB->Execute();
		pcDB->Clear();

		query = std::string(
			"UPDATE[ClanDB].[dbo].[ClanList]"
			" SET IconID = CL_OLD.MIconCnt"
			" FROM[ClanDB].[dbo].[ClanList] AS CL"
			" INNER JOIN[ClanDB].[dbo].[CL] AS CL_OLD"
			" ON CL_OLD.ClanName = CL.ClanName"
			" WHERE IconID <> CL_OLD.MIconCnt OR IconID = 0");

		pcDB->Prepare(query.c_str()) && pcDB->Execute();
		pcDB->Close();
	}

}

void BellatraServer::UpdateCrownClanIDs()
{
	iGoldCrownClanId = 0;
	iSilverCrownClanId = 0;
	iBronzeCrownClanId = 0;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB_2);
	if (pcDB->Open())
	{
		//in case of matching score, take the earliest score (based on date)
		if (pcDB->Prepare("SELECT TOP 3 IconID FROM ClanList WHERE BellatraPoints > 0 ORDER BY BellatraPoints DESC, BellatraDate ASC"))
		{
			if (pcDB->Execute())
			{
				int j = 0;
				while (pcDB->Fetch())
				{
					if (j == 0)
						pcDB->GetData(1, PARAMTYPE_Integer, &iGoldCrownClanId);

					else if (j == 1)
						pcDB->GetData(1, PARAMTYPE_Integer, &iSilverCrownClanId);

					else if (j == 2)
						pcDB->GetData(1, PARAMTYPE_Integer, &iBronzeCrownClanId);

					j++;
				}
			}
		}

		pcDB->Close();
	}

	DEBUGIF(bDebug, "BellatraServer::UpdateCrownClanIDs()");
	DEBUGIF(bDebug, "iGoldCrownClanId = %d", iGoldCrownClanId);
	DEBUGIF(bDebug, "iSilverCrownClanId = %d", iSilverCrownClanId);
	DEBUGIF(bDebug, "iBronzeCrownClanId = %d", iBronzeCrownClanId);
}

//This method gets called on char load
void BellatraServer::SendFullCrownDataToUser(UserData * pcUserData)
{
	DEBUGIF(bDebug, "BellatraServer::SendFullCrownDataToUser()");

	//JLM - Don't send PacketBellatraCrown here
	//because checking clan id isn't reliable
	//instead the player will get the crown index
	//from ClanMember.asp
	//When SOD gets updated, the full info will be sent via SendFullCrownDataToAllUsers
	if (pcUserData)
	{
		PacketBellatraCrownFull sPacketFull;
		sPacketFull.iLength = sizeof(PacketBellatraCrownFull);
		sPacketFull.iHeader = PKTHDR_BellatraCrownFull;
		sPacketFull.iBellatraCrownGoldClanId = iGoldCrownClanId;
		sPacketFull.iBellatraCrownSilverClanId = iSilverCrownClanId;
		sPacketFull.iBellatraCrownBronzeClanId = iBronzeCrownClanId;

		SENDPACKET(USERDATATOUSER(pcUserData), &sPacketFull, TRUE); //crown data for other users
	}
}

//store a list of top solo sod players locally
//after someone completes a solo round
//and if any ranking is changed, update all users
//also load at server start up
//then add User:Load() to get id

void BellatraServer::SendFullCrownDataToAllUsers()
{
	//UpdateClanCrown is more reliable using GAME_SERVER
	// as it will have the latest sCharacterData.iaClanID
	if (LOGIN_SERVER)
		return;

	DEBUGIF(bDebug, "BellatraServer::SendFullCrownDataToAllUsers()");

	PacketBellatraCrown sPacket;
	sPacket.iLength			= sizeof( PacketBellatraCrown );
	sPacket.iHeader			= PKTHDR_BellatraCrown;
	sPacket.iBellatraCrown	= 0;

	PacketBellatraCrownFull sPacketFull;
	sPacketFull.iLength = sizeof(PacketBellatraCrownFull);
	sPacketFull.iHeader = PKTHDR_BellatraCrownFull;
	sPacketFull.iBellatraCrownGoldClanId = iGoldCrownClanId;
	sPacketFull.iBellatraCrownSilverClanId = iSilverCrownClanId;
	sPacketFull.iBellatraCrownBronzeClanId = iBronzeCrownClanId;



	DEBUGIF(bDebug, "sPacketFull.iBellatraCrownGoldClanId = %d", iGoldCrownClanId);
	DEBUGIF(bDebug, "sPacketFull.iBellatraCrownSilverClanId = %d", iSilverCrownClanId);
	DEBUGIF(bDebug, "sPacketFull.iBellatraCrownBronzeClanId = %d", iBronzeCrownClanId);

	int j = 0;

	// Update
	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		if (!USERSDATA[i].pcSocketData)
			continue;

		UserData * pcUserData = &USERSDATA[i];
		User * pcUser = USERDATATOUSER( pcUserData );

		if ( pcUser->bNetServer )
			continue;

		sPacket.iBellatraCrown = 0;

		if (iGoldCrownClanId != 0 && pcUserData->sCharacterData.iClanID == iGoldCrownClanId)
		{
			sPacket.iBellatraCrown = 1;
		}
		else if (iSilverCrownClanId != 0 && pcUserData->sCharacterData.iClanID == iSilverCrownClanId)
		{
			sPacket.iBellatraCrown = 2;
		}
		else if (iBronzeCrownClanId != 0 && pcUserData->sCharacterData.iClanID == iBronzeCrownClanId)
		{
			sPacket.iBellatraCrown = 3;
		}

		DEBUGIF(bDebug, "sPacket.iBellatraCrown = %d %s", sPacket.iBellatraCrown, CHARACTERSERVER->GetCharacterName(&USERSDATA[i]));

		SENDPACKET(pcUser, &sPacket, TRUE);		//crown info for self
		SENDPACKET(pcUser, &sPacketFull, TRUE); //crown data for other users
	}
}

int BellatraServer::GetUserRoomCount( int iRoomID)
{
	if (iRoomID >= 0 && iRoomID < 4 )
		return iaUserRoomCount[iRoomID];

	return 0;
}

int BellatraServer::GetOriginalUserRoomCount(const int iRoomID)
{
	if (iRoomID >= 0 && iRoomID < 4)
		return iOriginalUserRoomCount[iRoomID];

	return 0;
}

void BellatraServer::SetRoomSoloMode(const int iRoomID, BOOL bSoloMode)
{
	if (iRoomID >= 0 && iRoomID < 4)
		baRoomSoloMode[iRoomID] = bSoloMode;
}

BOOL BellatraServer::GetRoomSoloMode(const int iRoomID)
{
	if (iRoomID >= 0 && iRoomID < 4)
		return baRoomSoloMode[iRoomID];

	return FALSE;
}

void BellatraServer::SetUserRoomCount(const int iRoomID, const int iCount )
{
	DEBUGIF(bDebug, "SetUserRoomCount - Room id = %d, iCount = %d", iRoomID, iCount);

	if ( iRoomID >= 0 && iRoomID < 4 )
		iaUserRoomCount[iRoomID] = iCount;
}

void BellatraServer::SetOriginalUserRoomCount(const int iRoomID, const int iCount)
{
	DEBUGIF(bDebug, "SetOriginalUserRoomCount - Room id = %d, iCount = %d", iRoomID, iCount);

	if (iRoomID >= 0 && iRoomID < 4)
		iOriginalUserRoomCount[iRoomID] = iCount;
}

int BellatraServer::GetRoomScore( int i )
{
	if ( i >= 0 && i < 4 )
	{
		int * p = (int*)pfnRoomScoreData;

		return p[i];
	}

	return 0;
}

void BellatraServer::ResetBellatra()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM BellatraTeamScore" ) )
			pcDB->Execute();

		pcDB->Clear();

		if ( pcDB->Prepare( "DELETE FROM BellatraPersonalScore" ) )
			pcDB->Execute();

		pcDB->Clear();

		if (pcDB->Prepare( "DELETE FROM BellatraPersonalScoreNew" ))
			pcDB->Execute();

		pcDB->Clear();

		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=0, BellatraDate=0" ) )
			pcDB->ExecuteUpdate();

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CL SET Cpoint=0"))
			pcDB->ExecuteUpdate();

		pcDB->Close();
	}

	iGoldCrownClanId = 0;
	iSilverCrownClanId = 0;
	iBronzeCrownClanId = 0;

	ClearAllSoloCrownRankingFromUsers();
	SendFullCrownDataToAllUsers();

	CHATSERVER->SendChatAll( CHATCOLOR_White, "Bellatra> SoD Scores Cleared! (Weekly)" );
}

/// <summary>
/// Return 1, 2 or 3 if solo player has a ranking
/// otherwise 0
/// </summary>
int BellatraServer::GetSoloCrownRankingForUser( User* pcUser )
{
	int iCrownRanking = 0;


	if (pcUser && pcUser->pcUserData && pcUser->pcUserData->pcSocketData)
	{
		iCrownRanking = GetSoloCrownRankingForUser(pcUser->iCharacterID, pcUser->pcUserData->sCharacterData.iClass);

		//std::cout << CHARACTERSERVER->GetCharacterName(pcUser->pcUserData) << " : " << pcUser->iCharacterID << " : " << iCrownRanking << std::endl;
	}

	return iCrownRanking;
}


/// <summary>
/// Reset solo sod ranking to 0 for all users
/// </summary>
void BellatraServer::ClearAllSoloCrownRankingFromUsers()
{
	INFO("BELLATRA: ClearAllSoloCrownRankingFromUsers");

	// Update
	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		UserData* ud = USERSDATA + i;

		if (!ud->pcSocketData)
			continue;

		User* u = USERDATATOUSER(ud);

		if (u && u->sBellatraSoloCrown > 0)
		{
			UpdateAndSyncUserSoloRanking(u, 0);
		}
	}
}



int BellatraServer::GetTopoSoloScoreForMe(int CharacterId)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	std::string query = " SELECT TOP (1) Score"
						" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew]"
						" WHERE CharacterId = ?"
						" ORDER BY Score DESC";

	int iScore = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &CharacterId);
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iScore);
			}
		}

		pcDB->Close();
	}

	return iScore;
}

int BellatraServer::GetTopoSoloScoreForClass(ECharacterClass eCharacterClass)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	std::string query = " SELECT TOP (1) Score"
						" FROM (SELECT t.*, row_number() over ( partition by CharacterID order by Score desc ) as seqnum"
						" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t"
						" WHERE t.CharacterClass = ?"
						" ) t"
						" WHERE seqnum <= 1"
						" ORDER BY Score DESC, Date ASC";

	int iScore = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &eCharacterClass);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iScore);
			}
		}

		pcDB->Close();
	}

	return iScore;
}

/// <summary>
/// Update solo sod ranking for character class
/// </summary>
void BellatraServer::UpdateAllSoloCrownRankingForClass(ECharacterClass eCharacterClass)
{
	INFO ("BELLATRA: UpdateAllSoloCrownRankingForClass %s", GetCharacterClassStringByClassEnum(eCharacterClass));

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	std::string query = " SELECT TOP (3) CharacterID"
						" FROM (SELECT t.*, row_number() over ( partition by CharacterID order by Score desc ) as seqnum"
						" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t"
						" WHERE t.CharacterClass = ?"
						" ) t"
						" WHERE seqnum <= 1"
						" ORDER BY Score DESC, Date ASC";


	int iRankingSoloFirstPlaceCharacterId = -1;
	int iRankingSoloSecondPlaceCharacterId = -1;
	int iRankingSoloThirdPlaceCharacterId = -1;

	if (pcDB->Open())
	{
		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &eCharacterClass);

			if (pcDB->Execute())
			{
				int iThisCharId = 0;
				int iPlacement = 0;
				while (pcDB->Fetch())
				{
					iPlacement++;
					pcDB->GetData(1, PARAMTYPE_Integer, &iThisCharId );

					if (iPlacement == 1)
					{
						iRankingSoloFirstPlaceCharacterId = iThisCharId;
					}
					else if (iPlacement == 2)
					{
						iRankingSoloSecondPlaceCharacterId = iThisCharId;
					}
					else if (iPlacement == 3)
					{
						iRankingSoloThirdPlaceCharacterId = iThisCharId;
					}
				}
			}
		}

		pcDB->Close();
	}

	DEBUG ("iRankingSoloFirstPlaceCharacterId = %d",  iRankingSoloFirstPlaceCharacterId);
	DEBUG ("iRankingSoloSecondPlaceCharacterId = %d", iRankingSoloSecondPlaceCharacterId);
	DEBUG ("iRankingSoloThirdPlaceCharacterId = %d",  iRankingSoloThirdPlaceCharacterId);

	// Update
	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		UserData* ud = USERSDATA + i;

		if (!ud->pcSocketData)
			continue;

		if (ud->sCharacterData.iClass != eCharacterClass)
			continue;

		User* u = USERDATATOUSER(ud);

		if (u)
		{
			if (u->iCharacterID == iRankingSoloFirstPlaceCharacterId)
			{
				UpdateAndSyncUserSoloRanking(u, 1);
			}
			else if (u->iCharacterID == iRankingSoloSecondPlaceCharacterId)
			{
				UpdateAndSyncUserSoloRanking(u, 2);
			}
			else if (u->iCharacterID == iRankingSoloThirdPlaceCharacterId)
			{
				UpdateAndSyncUserSoloRanking(u, 3);
			}
			else if (u->sBellatraSoloCrown != 0) //user no longer first, second, or third
			{
				UpdateAndSyncUserSoloRanking(u, 0);
			}
		}
	}
}

/// <summary>
/// Update solo sod ranking for all users
/// </summary>
void BellatraServer::UpdateAllSoloCrownRankingForAllClasses()
{
	DEBUG ("BELLATRA: UpdateAllSoloCrownRankingForAllClasses");

	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Archer);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Assassin);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Atalanta);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Fighter);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Knight);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Magician);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Mechanician);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Pikeman);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Priestess);
	UpdateAllSoloCrownRankingForClass(ECharacterClass::CHARACTERCLASS_Shaman);
}


/// <summary>
/// Return 1, 2 or 3 if solo player has a ranking
/// otherwise 0
/// </summary>
int BellatraServer::GetSoloCrownRankingForUser(int iCharacterId, ECharacterClass eCharacterClass)
{
	if (LOGIN_SERVER)
		return -1;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	/*
	select Top (3) CharacterName, Score
	from (select t.*, row_number() over (partition by CharacterID order by Score desc) as seqnum
			from [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t
			where t.CharacterClass = 2
			) t
	where seqnum <= 1
	ORDER BY Score DESC, Date Asc*/

	std::string query = "SELECT TOP (3) CharacterID"
		" FROM (SELECT t.*, row_number() over ( partition by CharacterID order by Score desc ) as seqnum"
		" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t"
		" WHERE t.CharacterClass = ?"
		" ) t"
		" WHERE seqnum <= 1"
		" ORDER BY Score DESC, Date ASC";

	int iRanking = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &eCharacterClass);

			if (pcDB->Execute())
			{
				int iThisCharId = 0;
				int iCounter = 0;
				while (pcDB->Fetch())
				{
					iCounter++;

					pcDB->GetData(1, PARAMTYPE_Integer, &iThisCharId );

					if (iThisCharId == iCharacterId)
					{
						iRanking = iCounter;
						break;
					}
				}
			}
		}

		pcDB->Close();
	}

	return iRanking;
}

//Called from EXE - 0x48478A18
//Leaders of leading SoD clan can fetch SoD gold
BOOL BellatraServer::PHGoldClanRecv( User * pcUser, struct PacketClanGoldReceive * psPacket )
{
	DEBUGIF(bDebug, "BellatraServer::PHGoldClanRecv");

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	int iGoldDB = 0;
	int iGoldDBa = 0;
	int iGoldDBmin = 0;
	int iNewGoldValue = 0;

	int iGold	= psPacket->iGold;
	BOOL bUpdate = FALSE;
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT BellatraGold FROM ClanList WHERE ClanLeader=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iGoldDB );
			}
		}

		pcDB->Clear();

		if (pcDB->Prepare("SELECT ClanMoney FROM CL WHERE ClanZang=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iGoldDBa);
			}
		}
		pcDB->Close();

	}

	iGoldDBmin = std::min(iGoldDB, iGoldDBa);


	DEBUGIF(bDebug, "BellatraServer::PHGoldClanRecv: Requested gold = %d, BellatraGold = %d, ClanMoney = %d", iGold, iGoldDB, iGoldDBa);

	if ( iGold > 0 )
	{
		if ( iGold < 0 || iGoldDBmin < iGold  )
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_BellatraGoldHackLeader;
			sPacket.LParam = iGold;
			sPacket.SParam = iGoldDBmin;
			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );

			iGold = iGoldDB;
		}
		else
		{
			USERSERVER->AddServerUserGold( pcUser->pcUserData, iGold, WHEREID_ClanMoney );
			psPacket->iGoldSend = iGold;
			psPacket->iGold		= -iGold;
			SENDPACKET( pcUser, psPacket );
			bUpdate = TRUE;

			iNewGoldValue = iGoldDBmin - iGold;
		}
	}
	else
	{
		//send packet to user and show how much gold they have
		//This is after clicking on 'amount'
		psPacket->iGold		= iGoldDBmin;
		psPacket->iGoldSend = 0;
		SENDPACKET( pcUser, psPacket );
	}


	if ( bUpdate )
	{
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE ClanList SET BellatraGold=? WHERE ClanLeader=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iNewGoldValue);
				pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
				pcDB->ExecuteUpdate();
			}

			pcDB->Clear();

			if (pcDB->Prepare("UPDATE CL SET ClanMoney=? WHERE ClanZang=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iNewGoldValue);
				pcDB->BindParameterInput(2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));
				pcDB->ExecuteUpdate();
			}

			pcDB->Close();
		}
	}
	return TRUE;
}


void BellatraServer::OnLoadUser ( User * pcUser )
{
	if (GAME_SERVER)
	{
		pcUser->sBellatraSoloCrown = GetSoloCrownRankingForUser(pcUser);

		//DEBUG( "BellatraServer::OnLoadUser: %s solo ranking:  %d", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->sBellatraSoloCrown );

		if (pcUser->sBellatraSoloCrown > 0)
		{
			UpdateAndSyncUserSoloRanking(pcUser, pcUser->sBellatraSoloCrown);
		}
	}
}

//Update client
//Update login server
void BellatraServer::UpdateAndSyncUserSoloRanking(User * pcUser, int iNewRanking, BOOL bUpdateLoginServer)
{
	std::cout << "UpdateAndSyncUserSoloRanking, iNewRanking: " << iNewRanking << " for user: " << CHARACTERSERVER->GetCharacterName(pcUser->pcUserData) <<   std::endl;

	pcUser->sBellatraSoloCrown = iNewRanking;

	if (GAME_SERVER)
	{
		DEBUG ("BELLATRA: %s: Set ranking for class: %s: %d",
			CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
			GetCharacterClassStringByClassEnum(pcUser->pcUserData->sCharacterData.iClass),
			iNewRanking);

		//send crown ranking to player so buff is applied
		PacketCommand sPacket{};
		sPacket.iLength = sizeof(PacketCommand);
		sPacket.iHeader = PKTHDR_BellatraCrownSolo;
		sPacket.p1 = pcUser->sBellatraSoloCrown;
		SENDPACKET(pcUser->pcUserData, &sPacket, TRUE);

		if (pcUser->pcUserData->iID != 0 &&
			bUpdateLoginServer)
		{
			//update login-server status so other players
			//see correct crown ranking of this player
			ZeroMemory(&sPacket, sizeof(PacketCommand));
			sPacket.iLength = sizeof(PacketCommand);
			sPacket.iHeader = PKTHDR_NetBellatraSoloRanking;
			sPacket.p1 = pcUser->pcUserData->iID;
			sPacket.p2 = pcUser->sBellatraSoloCrown;

			NetConnection* l_LoginServer = NETSERVER->GetLoginServerConnection();
			if (l_LoginServer)
			{
				NETSERVER->SendPacket(l_LoginServer->pcSocketData, &sPacket);
			}
		}
	}
	else
	{
		//update game-server status so other players
		//see correct crown ranking of this player
		PacketCommand sPacket{};
		sPacket.iLength = sizeof(PacketCommand);
		sPacket.iHeader = PKTHDR_NetBellatraSoloRanking;
		sPacket.p1 = pcUser->pcUserData->iID;
		sPacket.p2 = pcUser->sBellatraSoloCrown;

		NETSERVER->SendPacketGameServers(&sPacket);
	}
}

void BellatraServer::DeletePoint( char * pszUserID, char * pszCharName, char * pszIP, int iUnk, int iUnk2 )
{

}

int BellatraServer::AddTaxGoldToTopThreeClans( const int gold )
{
	DEBUGIF(bDebug, "BellatraServer::AddTaxGoldToTopThreeClans %d", gold);


	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	SQLConnection * pcDBa = SQLCONNECTION( DATABASEID_ClanDB_2 );

	int count = 0;
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT COUNT(ID) FROM ClanList WHERE BellatraPoints > 0"))
		{
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &count);
			}
		}
		pcDB->Close();
	}

	DEBUGIF(bDebug, "numer of clans with points more than 0 %d", count);

	int iTotalTax = 0;

	if (count == 0)
	{
		return 0; //not taxed
	}
	//only one sod clan
	else if (count == 1)
	{
		iTotalTax = EVENT_BELLATRA_FIRST_CLAN_TAX;
	}
	//only two sod clan
	else if(count == 2)
	{
		iTotalTax = EVENT_BELLATRA_FIRST_CLAN_TAX + EVENT_BELLATRA_SECOND_CLAN_TAX;
	}
	//we have three or more
	else
	{
		iTotalTax = EVENT_BELLATRA_FIRST_CLAN_TAX + EVENT_BELLATRA_SECOND_CLAN_TAX + EVENT_BELLATRA_THIRD_CLAN_TAX;
	}

	int iTotalTaxValue = static_cast<int>(gold * (iTotalTax / 100.0f));

	if (iTotalTaxValue < 0)
		iTotalTaxValue = 0;

	//safe guard
	if (iTotalTaxValue > 500'000)
		iTotalTaxValue = 500'000;

	int iFirstClanTaxValue = static_cast<int>(iTotalTaxValue * (EVENT_BELLATRA_FIRST_CLAN_TAX / (iTotalTax * 1.0f)));
	int iSecondClanTaxValue = static_cast<int>(iTotalTaxValue * (EVENT_BELLATRA_SECOND_CLAN_TAX / (iTotalTax * 1.0f)));
	int iThirdClanTaxValue = static_cast<int>(iTotalTaxValue * (EVENT_BELLATRA_THIRD_CLAN_TAX / (iTotalTax * 1.0f)));


	DEBUGIF(bDebug, "iTotalTaxValue -> %d", iTotalTaxValue);
	DEBUGIF(bDebug, "iFirstClanTaxValue -> %d", iFirstClanTaxValue);
	DEBUGIF(bDebug, "iSecondClanTaxValue -> %d", iSecondClanTaxValue);
	DEBUGIF(bDebug, "iThirdClanTaxValue -> %d", iThirdClanTaxValue);


	if ( pcDB->Open() && pcDBa->Open() )
	{
		char szClanName[32] = { 0 };
		int taxValue = 0;

		//in case of matching score, take the earliest score (based on date)
		if (pcDB->Prepare("SELECT TOP(3) ClanName FROM ClanList WHERE BellatraPoints > 0 ORDER BY BellatraPoints DESC, BellatraDate ASC"))
		{
			if (pcDB->Execute())
			{
				int clanIndex = 0;
				while ( pcDB->Fetch() )
				{
					DEBUGIF(bDebug, "Fetching..");

					pcDB->GetData (1, PARAMTYPE_String, szClanName, 32);

					if (clanIndex == 0)
						taxValue = iFirstClanTaxValue;
					else if (clanIndex == 1)
						taxValue = iSecondClanTaxValue;
					else if (clanIndex == 2)
						taxValue = iThirdClanTaxValue;
					else
						taxValue = 0;

					DEBUGIF(bDebug, "%d szClanName -> %s tax -> %d", clanIndex, szClanName, taxValue);

					if (pcDBa->Prepare("UPDATE ClanList SET BellatraGold=BellatraGold+? WHERE ClanName=?"))
					{
						pcDBa->BindParameterInput(1, PARAMTYPE_Integer, &taxValue);
						pcDBa->BindParameterInput(2, PARAMTYPE_String, szClanName);
						pcDBa->ExecuteUpdate();
					}

					pcDBa->Clear();

					if (pcDBa->Prepare("UPDATE CL SET ClanMoney=ClanMoney+? WHERE ClanName=?"))
					{
						pcDBa->BindParameterInput(1, PARAMTYPE_Integer, &taxValue);
						pcDBa->BindParameterInput(2, PARAMTYPE_String, szClanName);
						pcDBa->ExecuteUpdate();
					}

					pcDBa->Clear();

					clanIndex++;
				}

				DEBUGIF(bDebug, "FINISHED!");
			}
			else
			{
				iTotalTaxValue = 0;
			}
		}

		pcDBa->Close();
		pcDB->Close();
	}



	return iTotalTaxValue;
}

int BellatraServer::GetTeamPoint( UserData * pcUserData )
{
	int iPoints = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TotalPoint FROM BellatraTeamScore WHERE (CharacterName=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUserData ) );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iPoints );
			}
		}

		pcDB->Close();
	}

	DEBUGIF(bDebug, "BellatraServer::GetClanPoint %s, points = %d", CHARACTERSERVER->GetCharacterName(pcUserData), iPoints);


	return iPoints;
}

//used by login server only
BOOL BellatraServer::RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode )
{
	if (GAME_SERVER)
		return FALSE;

	User* pcUser = USERDATATOUSER(pcUserData);

	if ( pcUser && pcUserData && SodScore >= 0 )
	{
		DEBUG ("BellatraServer::RecordPoint. iLevel = %d, SodScore = %d, SodKillCount = %d, SodTotalScore = %d, SodTotalSucessUser = %d, SodTotalUser = %d, SodCode = %d",
			iLevel,
			SodScore,
			SodKillCount,
			SodTotalScore,
			SodTotalSucessUser,
			SodTotalUser,
			SodCode);

		int iClanID = 0;

		char * charName = CHARACTERSERVER->GetCharacterName(pcUserData);

		char szDateTime[20];
		GetDateTime( szDateTime );

		USERSERVER->SetAccountNameFromSQL( pcUserData );

		SQLConnection* userDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_CharInfo );

		if (userDB->Open())
		{
			if (userDB->Prepare("SELECT ClanID FROM CharacterInfo WHERE Name=?"))
			{
				userDB->BindParameterInput(1, PARAMTYPE_String, charName);
				if (userDB->Execute() && userDB->Fetch())
				{
					userDB->GetData(1, PARAMTYPE_Integer, &iClanID);
					DEBUGIF(bDebug, "GetClan query result = %d", iClanID);
				}
			}
			userDB->Close();
		}

		DEBUGIF(bDebug, "GetClan result | %s | %d", charName, iClanID);

		// two or more players
		if (SodTotalUser >= 2)
		{
			DEBUG ("Inserting new team score for %s", charName);

			SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

			if (pcDB->Open())
			{
				if (pcDB->Prepare("INSERT INTO BellatraTeamScore ([AccountName],[CharacterName],[JobCode],[Level],[ClanCode],[Score],[Kills],[TotalPoint],[TotalUser],[SuccessUser],[Code],[Quake],[StunSeal],[FreezeSeal],[RabieSeal],[StygianSeal],[GuardianSaintSeal],[PointBag],[Date]) VALUES (?,?,?,?,?,?,?,?,?,?,?,0,0,0,0,0,0,0,?)"))
				{
					pcDB->BindParameterInput(1, PARAMTYPE_String, pcUserData->szAccountName);
					pcDB->BindParameterInput(2, PARAMTYPE_String, charName);
					pcDB->BindParameterInput(3, PARAMTYPE_Integer, &pcUserData->sCharacterData.iClass);
					pcDB->BindParameterInput(4, PARAMTYPE_Integer, &iLevel);
					pcDB->BindParameterInput(5, PARAMTYPE_Integer, &iClanID);
					pcDB->BindParameterInput(6, PARAMTYPE_Integer, &SodScore);
					pcDB->BindParameterInput(7, PARAMTYPE_Integer, &SodKillCount);
					pcDB->BindParameterInput(8, PARAMTYPE_Integer, &SodTotalScore);
					pcDB->BindParameterInput(9, PARAMTYPE_Integer, &SodTotalUser);
					pcDB->BindParameterInput(10, PARAMTYPE_Integer, &SodTotalSucessUser);
					pcDB->BindParameterInput(11, PARAMTYPE_Integer, &SodCode);
					pcDB->BindParameterInput(12, PARAMTYPE_String, szDateTime);

					pcDB->Execute();
				}

				pcDB->Close();
			}
		}


		//Personal score
		//Requires one user for entire of sod
		else if ( SodTotalUser == 1 && SodTotalSucessUser == 1 )
		{
			DEBUG ("Inserting new solo score for %s", charName);

			int iTopScoreForMe = GetTopoSoloScoreForMe(pcUser->iCharacterID);
			int iTopScoreForClass = GetTopoSoloScoreForClass(pcUserData->sCharacterData.iClass);

			DEBUG ("Current top score for me: %d ",
				iTopScoreForMe);

			DEBUG ("Current top score for %s: %d ",
				GetCharacterClassStringByClassEnum(pcUserData->sCharacterData.iClass),
				iTopScoreForClass);

			SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);

			int iID = 0;
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT ID FROM BellatraPersonalScore WHERE CharacterName=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, charName );
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
					}
				}


				pcDB->Clear();

				if ( iID )
				{
					DEBUGIF( bDebug, "Updating personal score for %s, score = %d", charName, SodScore );

					//to be obsoleted once website is updated to use BellatraPersonalScoreNew
					if ( pcDB->Prepare( "UPDATE BellatraPersonalScore SET Kills=?, Score=?, Date=? WHERE (CharacterName=?) AND (Score < ?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &SodKillCount );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &SodScore );
						pcDB->BindParameterInput( 3, PARAMTYPE_String, szDateTime );
						pcDB->BindParameterInput( 4, PARAMTYPE_String, charName );
						pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &SodScore );
						pcDB->ExecuteUpdate();
					}
				}
				else
				{
					DEBUGIF( bDebug, "Inserting personal score for %s, score = %d", charName, SodScore );

					//to be obsoleted once website is updated to use BellatraPersonalScoreNew
					if ( pcDB->Prepare( "INSERT INTO BellatraPersonalScore ([AccountName],[CharacterName],[Kills],[Score],[Date]) VALUES (?,?,?,?,?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, charName );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &SodKillCount );
						pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &SodScore );
						pcDB->BindParameterInput( 5, PARAMTYPE_String, szDateTime );
						pcDB->Execute();
					}
				}

				pcDB->Clear();

				//new table - record everything
				if ( pcDB->Prepare( "INSERT INTO BellatraPersonalScoreNew ([CharacterID],[CharacterName],[CharacterClass],[CharacterLevel],[Kills],[Score],[UnixTime],[Date]) VALUES (?,?,?,?,?,?,?,GETDATE())" ) )
				{
					DWORD unixTime = GetUnixTime();

					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, charName );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUserData->sCharacterData.iClass );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUserData->sCharacterData.iLevel );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &SodKillCount );
					pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &SodScore );
					pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &unixTime );
					pcDB->Execute();
				}

				pcDB->Close();
			}

			if (SodScore > iTopScoreForClass &&			 //current score is better than top score for class
				(iTopScoreForClass == 0 ||				 //no existing top score for this class
				(iTopScoreForMe != iTopScoreForClass)))  //no need to re-broadcast as the top score is me
			{
				//Broadcast time!
				//New top solo score for this class
				CHATSERVER->SendChatAllEx(CHATCOLOR_Global,
					"Bellatra> New Top %s! %s with score of %s",
					GetCharacterClassStringByClassEnum(pcUserData->sCharacterData.iClass),
					CHARACTERSERVER->GetCharacterName(pcUserData),
					FormatNumber(SodScore));
			}

			//Update all solo ranking for this class
			UpdateAllSoloCrownRankingForClass(pcUserData->sCharacterData.iClass);
		}
	}
	return TRUE;
}

///
//Selects top clan
//Compares
//Record new best clan point

BOOL BellatraServer::RecordClanPoint( const char * pszClanName, int iTotalPoint, int iCurrentClanPoint)
{
	int iClanLeaderPoint	= 0;
	char szClanName[32]		= { 0 };
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB_2 );
	if ( pcDB->Open() )
	{

		//in case of matching score, take the earliest score (based on date)
		if (pcDB->Prepare("SELECT TOP 1 ClanName, BellatraPoints FROM ClanList WHERE BellatraPoints > 0 ORDER BY BellatraPoints DESC, BellatraDate ASC"))
		{
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_String, szClanName, 32);
				pcDB->GetData(2, PARAMTYPE_Integer, &iClanLeaderPoint);
			}
		}

		if (!STRINGCOMPAREI(szClanName, pszClanName) && iTotalPoint > iClanLeaderPoint)
		{
			CHATSERVER->SendChatAllEx(CHATCOLOR_White, "Bellatra> New Top Clan! %s with score of %s", pszClanName, FormatNumber(iTotalPoint));
		}

		pcDB->Clear();


		if ( pcDB->Prepare( "UPDATE ClanList SET BellatraPoints=?, BellatraDate=? WHERE ClanName=?" ) )
		{
			DWORD dwTime = (DWORD)time( NULL );
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iTotalPoint);
			pcDB->BindParameterInput( 2, PARAMTYPE_Int64, &dwTime );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, (void*)pszClanName );
			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CL SET Cpoint=?, SodDate=getdate() WHERE ClanName=?"))
		{
			DWORD dwTime = (DWORD)time(NULL);
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iTotalPoint);
			pcDB->BindParameterInput(2, PARAMTYPE_String, (void*)pszClanName);
			pcDB->ExecuteUpdate();
		}

		pcDB->Close();

		DEBUGIF(bDebug, "BellatraServer::RecordClanPoint %s, total points = %d, old points = %d, top clan = %s, top clan points = %d",
			   pszClanName, iTotalPoint, iCurrentClanPoint, &szClanName, iClanLeaderPoint);


	}

	return TRUE;
}

//Gets the current clan point for SoD
int BellatraServer::GetClanPoint( const char * pszClanName )
{
	int iClanPoint = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ClanDB_2 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT BellatraPoints FROM ClanList WHERE ClanName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszClanName );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iClanPoint );
			}
		}
		pcDB->Close();
	}
	return iClanPoint;
}

int BellatraServer::GetClanPoint( int iClanID)
{
	int iClanPoint = 0;
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB_2);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT BellatraPoints FROM ClanList WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iClanID);
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iClanPoint);
			}
		}
		pcDB->Close();
	}
	return iClanPoint;
}

BOOL BellatraServer::GetClanNameFromId(int iClanID, char* pszClanName)
{
	BOOL success = FALSE;

	pszClanName[0] = '\0';

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB_2);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT TOP(1) ClanName FROM ClanList WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iClanID);
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_String, pszClanName, 32);
				success = TRUE;
			}
		}
		pcDB->Close();
	}

	return success;
}

BOOL BellatraServer::DoWeeklyResetIfRequired()
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
	if (sTime.wDayOfWeek == 6 &&
		sTime.wHour == 0 &&
		sTime.wDay != wLastDayResetSOD &&
		bAllowSodScoreReset)
	{
		ResetBellatra();
		wLastDayResetSOD = sTime.wDay;
	}

	return TRUE;
}


BOOL BellatraServer::CheckAndUpdateClanTopScore()
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);
	if ( pcDB->Open() )
	{
		std::string query = "SELECT T.ClanCode, "
			"Max (T.SumClanTeamScore) AS MaxClanTeamScore, "
			"MAX (TeamScoreCount) AS ClanTeamScoreCount "
			"FROM ("
			"SELECT ClanCode, Code, SUM(Score) AS SumClanTeamScore, COUNT(*) TeamScoreCount "
			"FROM [ClanDB].[dbo].[BellatraTeamScore] "
			"GROUP BY ClanCode, Code) "
			"AS T "
			"GROUP BY T.ClanCode "
			"ORDER BY MaxClanTeamScore DESC";

		// Get Sod Data
		// Code - unique sod code
		// SuccessUser - number of surviving players in team
		// TotalPoint - total point for that sod
		if ( pcDB->Prepare( query.c_str() ) )
		{
			// Have data?
			if ( pcDB->Execute() )
			{
				int iClanCode = 0;
				int iMaxClanScore = 0;
				int iCurrentClanPoint = 0;
				int iTotalPlayers = 0;
				char szClanName[32] = { 0 };

				// Fetch results
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iClanCode );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iMaxClanScore );
					pcDB->GetData( 3, PARAMTYPE_Integer, &iTotalPlayers );

					// Get current clan point
					iCurrentClanPoint		= GetClanPoint ( iClanCode );

					GetClanNameFromId ( iClanCode, szClanName);

					DEBUGIF(bDebug, "UpdateClanBellatra - szClanName = %s Current points = %d, Max team points = %d, Survivors = %d", szClanName, iCurrentClanPoint, iMaxClanScore, iTotalPlayers);

					// More than 3 users in the room that is same clan and new Point is more than Old Point? Record clan point
					if ( iTotalPlayers >= iRankingPointsMinNumOfSurvivorsReq && iMaxClanScore > iCurrentClanPoint )
						RecordClanPoint( szClanName, iMaxClanScore, iCurrentClanPoint );
				}
			}
		}
		pcDB->Close();
	}

	return TRUE;
}

//This function is called whenever someone makes a kill in SoD
// The score is added to the room
BOOL BellatraServer::AddRoomScore( int iRoomID, int iScore )
{
	int * piaRoomScore = ((int*)pfnRoomScoreData);

	if ( iRoomID >= 0 && iRoomID < 8 )
	{
		piaRoomScore[iRoomID] += iScore;
		return TRUE;
	}

	return FALSE;
}

BOOL BellatraServer::UnitKilled( UnitData * pcUnitData )
{
	int * piaRoomKills = ((int*)pfnRoomScoreKill);

	piaRoomKills[pcUnitData->PartyParam]--;

	//3%
	if (false && Dice::RandomI( 0, 1000 ) < 10 ) // Chaos disabled this shit. Josh agrees xD
	{
		LootItem sLootItem;

		//Get Item Data
		DefinitionItem * ps = ITEMSERVER->FindItemDefByCode( ITEMID_BrilliantStone );

		if ( ps )
		{
			//Create Item with non-spec
			ITEMSERVER->CreateItem( &sLootItem.sItem, ps,  EItemSource::BellatraKill, 0 );

			//Get Position of Unit on Map
			int iX = pcUnitData->sPosition.iX >> 8;
			int iY = pcUnitData->sPosition.iY >> 8;
			int iZ = pcUnitData->sPosition.iZ >> 8;

			//Get Map Data from Unit
			Map * psMap = (Map*)pcUnitData->pcMap;

			if ( psMap )
			{
				//Add Item to Map
				Loot * psItemD = MAPSERVER->AddItemV2( psMap, &sLootItem, iX, iY, iZ, ELootType::Initial);

				//Added? Send It to near users...
				if ( psItemD )
					MAPSERVER->SendItemStageNearUsers( psMap, psItemD );
			}
		}
	}

	return TRUE;
}

NAKED BOOL BellatraServer::UpdateBellatraUserScore( UserData * pcUserData )
{
	JMP( pfnUpdateBellatraUserScore );
}

NAKED BOOL BellatraServer::HandleItemRoom( UnitData * pcUnitData, UserData * pcUserData )
{
	JMP( pfnHandleBellatraRoomItem );
}

NAKED void BellatraServer::HandleConfigurationNextRound()
{
	JMP( pfnHandleConfigNextRound );
}

//This in turn calls BellatraServer::RecordPoint via LOGIN_SERVER
NAKED void BellatraServer::SendScoreUser( UserData * pcUserData, int iScore, int iKills, int iTotalScore, int iUserCount, int iTotalUsers, int iCode )
{
	JMP( pfnSendScore );
}

NAKED BOOL BellatraServer::CreateBellatraItem( int iItemID, Map * pcStage, int iX, int iY, int iZ, int iQuantity )
{
	JMP( pfnCreateBellatraItem );
}

//This method gets called at ~ 60 times per second.
BOOL BellatraServer::UpdateServer( Map * pcStage, MonsterMapSpawnHandle * psMonsterData )
{
	static BOOL baBossSpawn[8];

	//Gets Bellatra data for current round
	BellatraData * psBellatraData = GetBellatraData() + (GetRound() - 1);

	PacketBellatraInformation sPacket1;

	if (TIMEHOUR == 23 && bBroadcastSoDStartingSoonAtHour23)
	{
		for (int i = 0; i < PLAYERS_MAX; i++)
		{
			UserData* ud = USERSDATA + i;

			if (ud->pcSocketData && ud->dwExclusiveNum != 0x100 && ud->sCharacterData.iLevel >= 80)
			{
				CHATSERVER->SendChat(USERDATATOUSER(ud), CHATCOLOR_Global, "Bellatra> Sod opens in 5 min!");
			}
		}

		bBroadcastSoDStartingSoonAtHour23 = FALSE;
	}
	else if (TIMEHOUR == 4 && bBroadcastSoDEntryIsOpenAtHour4)
	{
		for (int i = 0; i < PLAYERS_MAX; i++)
		{
			UserData* ud = USERSDATA + i;

			if (ud->pcSocketData &&
				ud->dwExclusiveNum != 0x100 &&
				ud->sCharacterData.iLevel >= 80 &&
				ud->iMapID == MAPID_NaviskoTown)
			{
				CHATSERVER->SendChat(USERDATATOUSER(ud), CHATCOLOR_Command, "SOD_Entry_Is_Open");
			}
		}

		bBroadcastSoDEntryIsOpenAtHour4 = FALSE;
	}




	//rsHardCoreEventTimeOver is TRUE
	if ( READDWORD( 0x07AC9E30 ) )
	{
		//rsHardCoreEventMode is TRUE
		if ( READDWORD( 0x07AC9E2C ) )
		{
			WRITEDWORD( 0x07AC9E30, FALSE ); //Set rsHardCoreEventTimeOver to FALSE
		}
		else
		{
			return TRUE;
		}
	}

	BOOL bHour = FALSE;

	//Monsters not initialized yet for THIS round?
	if (bForceInit || psBellatraData->saMonster[0].szName[0] == 0 )
	{
		//The previous SoD (if there is one) have finished
		if ( dwFinishTime <= TICKCOUNT )
		{
			DEBUGIF(bDebug, "psBellatraData->saMonster[0].szName[0] == 0");
			DEBUGIF(bDebug, "dwFinishTime <= TICKCOUNT");


			//Get SoD Map info
			Map * psSOD = MAPSDATA + MAPID_Bellatra;

			for ( int i = 0; i < MAX_NPCINMAP; i++ )
			{
				PacketUnitInfo * ps = psSOD->saNPC + i;

				if ( ps->iHeader && STRINGCOMPARE( "152", ps->sCharacterData.szName ) )
				{
					DEBUGIF(bDebug, "CALLT_WITH_ARG1 0x0054E100 for %s", ps->sCharacterData.szName);

					CALLT_WITH_ARG1( 0x0054E100, (DWORD)psSOD, (DWORD)ps );
					break;
				}
			}

			bHour = TRUE;
		}
	}

	else if (psBellatraData->iHour == TIMEHOUR)
	{
		DEBUGIF(bDebug, "psBellatraData->iHour == TIMEHOUR. %d vs %d", psBellatraData->iHour, TIMEHOUR);

		bHour = TRUE;
	}


	if (bForceHour)
	{
		DEBUGIF(bDebug, "Forced hour to be true");

		bHour = TRUE;
		bForceHour = FALSE;
	}

	//Start of a round (game 'hour' = sql start round 'hour')
	if (  bHour )
	{
		//Round index is 0 (0 + 1)
		//sod started
		if ( GetRound() == 1 )
		{
			bSodIsActive = TRUE;

			DEBUGIF(bDebug, "bHour is true and round index is 0. psBellatraData->iHour is %d and TIMEHOUR is %d", psBellatraData->iHour, TIMEHOUR);

			Map * psSOD = MAPSDATA + MAPID_Bellatra;

			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				UnitData * pc = UNITSDATA + i;

				if ( pc && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC && (DWORD)pc->pcMap == (DWORD)psSOD )
				{
					if ( STRINGCOMPARE( "Ihin", pc->GetName() ) )
					{
						//Remove Ihin
						pc->Free();
						break;
					}
				}
			}

			DEBUGIF(bDebug, "GetRound() is: %d", GetRound());


			BellatraServer::HandleConfigurationNextRound();
			psBellatraData = GetBellatraData() + (GetRound() - 1); //load next round data

			DEBUGIF(bDebug, "BellatraServer::HandleConfigurationNextRound");
			DEBUGIF(bDebug, "GetRound() is now: %d", GetRound());


			//Defines the END time of SoD

#ifdef DEV_MODE
			dwFinishTime = TICKCOUNT + (47 * 1000) + (7 * 60 * 1000); //7 minutes in length + 47 seconds
#else
			dwFinishTime = TICKCOUNT + (47 * 1000) + (15 * 60 * 1000); //15 minutes in length + 47 seconds
#endif

			//reset
			SetOriginalUserRoomCount(0, 0);
			SetOriginalUserRoomCount(1, 0);
			SetOriginalUserRoomCount(2, 0);
			SetOriginalUserRoomCount(3, 0);

			SetRoomSoloMode(0, FALSE);
			SetRoomSoloMode(1, FALSE);
			SetRoomSoloMode(2, FALSE);
			SetRoomSoloMode(3, FALSE);




			for (int i = 0; i < PLAYERS_MAX; i++)
			{
				UserData* ud = USERSDATA + i;

				if (ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
				{
					if (MAPSERVER->IsBellatraMapArea(ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ))
					{
						CHATSERVER->SendChat(USERDATATOUSER(ud), CHATCOLOR_Command, "SOD_Started");

						ud->iBellatraKills = 0;
						ud->iBellatraScore = 0;

						SetOriginalUserRoomCount(ud->sBellatraRoomID, GetOriginalUserRoomCount(ud->sBellatraRoomID) + 1);
					}
				}
			}

			for (int i = 0; i < PLAYERS_MAX; i++)
			{
				UserData* ud = USERSDATA + i;

				if (ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
				{
					if ( MAPSERVER->IsBellatraMapArea(ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ))
					{
						User* u = USERDATATOUSER(ud);
						if (u)
						{
							u->iBellatraRoomInitialPlayerCount = GetOriginalUserRoomCount(ud->sBellatraRoomID);

							DEBUG ("BELLATRA: iBellatraRoomInitialPlayerCount for %s for room id: %d = %d ",
								CHARACTERSERVER->GetCharacterName(ud),
								ud->sBellatraRoomID,
								u->iBellatraRoomInitialPlayerCount);

							PacketNumberOfInitialBellatraPlayers sPacket;
							sPacket.iLength = sizeof(PacketNumberOfInitialBellatraPlayers);
							sPacket.iHeader = PKTHDR_NumfOfInitialBellatraPlayers;
							sPacket.iCount = u->iBellatraRoomInitialPlayerCount;
							SENDPACKET(USERDATATOUSER(ud), &sPacket, TRUE);
						}
					}
				}

			}
		}

		//count surviving players at start of each round
		SetUserRoomCount(0, 0);
		SetUserRoomCount(1, 0);
		SetUserRoomCount(2, 0);
		SetUserRoomCount(3, 0);

		for (int i = 0; i < PLAYERS_MAX; i++)
		{
			UserData* ud = USERSDATA + i;
			if (ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
			{
				if (MAPSERVER->IsBellatraMapArea(ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ))
				{
					SetUserRoomCount(ud->sBellatraRoomID, GetUserRoomCount(ud->sBellatraRoomID) + 1);
				}
			}
		}


		int numOfSurvivingUsersInLeadingRoom = GetUserRoomCount(GetRankRoomID(0));

		DEBUGIF(bDebug, "ROUND NUMBER: %d", GetRound());
		DEBUGIF(bDebug, "numOfSurvivingUsersInLeadingRoom = %d", numOfSurvivingUsersInLeadingRoom);



		BOOL bDropLoots = FALSE;

		//Loop over all existing monsters in SoD (from previous round)
		// and kill them all off. If it's the final round and the monster name is null
		// then drop the loots if there are any surviving users
		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pc = (UnitData*)pcStage->pcaUnitData[i];

			if ( pc && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Monster )
			{
				if ( pc->bActive && pc->PartyFlag == 0x100 && pc->IsDead() == FALSE )
				{
					//Kill
					pc->SetCurrentHealth( 0 );
					pc->Animate( ANIMATIONTYPE_Die );

					//BellatraData no longer contain any monster info?
					//so therefore it must be at the end of the round
					//once the monsters disappeared (?)
					if (psBellatraData->saMonster[0].szName[0] == 0)
					{
						bDropLoots = TRUE;
					}
				}
			}
		}

		if (bDropLoots && numOfSurvivingUsersInLeadingRoom > 0)
		{
			int iNumOfDrops = numOfSurvivingUsersInLeadingRoom * 3;
			if (iNumOfDrops > 18) //18 is max (6 players * 3)
				iNumOfDrops = 16;
			else if (iNumOfDrops < 3)
				iNumOfDrops = 3; // 3 is minimum

			DropBellatraRewards(iNumOfDrops);
		}


		//Clear memory on the SoD monster spawn handler
		ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );


		if ( psBellatraData->iPoints > 0 )
		{
			DEBUGIF(bDebug, "psBellatraData->iPoints > 0. %d", psBellatraData->iPoints);


			int iTotalPoints = 0;

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
				{
					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						if (iTotalPoints < ud->iBellatraScore)
						{
							iTotalPoints = ud->iBellatraScore;

							DEBUGIF(bDebug, "iTotalPoints = %d", iTotalPoints);
							DEBUGIF(bDebug, "ud->iBellatraScore = %d", ud->iBellatraScore);
						}
					}
				}
			}

			if ( iTotalPoints < psBellatraData->iPoints )
			{
				DEBUGIF(bDebug, "iTotalPoints < psBellatraData->iPoints TRUE");


				//Update Round
				int * piaRoomScoreKills = ((int*)pfnRoomScoreKill);

				DEBUGIF(bDebug, "pfnRoomScoreKill = %d", pfnRoomScoreKill);

				ZeroMemory( piaRoomScoreKills, sizeof( int ) * 8 );

				SetRound( GetRound() );
				psBellatraData = GetBellatraData() + (GetRound() - 1);

				//prepare to teleporting users out of SoD
				Map * psNaviskoMap = MAPSDATA + MAPID_NaviskoTown;
				PacketWarpGateField sPacket;
				sPacket.iLength = sizeof( PacketWarpGateField );
				sPacket.iHeader = PKTHDR_WarpGateField;
				sPacket.iX = psNaviskoMap->pcBaseMap->GetCenter().iX << 8;
				sPacket.iZ = psNaviskoMap->pcBaseMap->GetCenter().iY << 8;
				sPacket.iStage = MAPID_NaviskoTown;

				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * ud = USERSDATA + i;

					if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
					{
						if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
						{
							User * u = USERDATATOUSER(ud);

							if (u)
							{
								PacketNumberOfInitialBellatraPlayers sPacket1;
								sPacket1.iLength = sizeof(PacketNumberOfInitialBellatraPlayers);
								sPacket1.iHeader = PKTHDR_NumfOfInitialBellatraPlayers;
								sPacket1.iCount = 0;
								SENDPACKET(u, &sPacket1, TRUE);

								CHATSERVER->SendChat(u, CHATCOLOR_White, "Bellatra> Your team failed to reach the points to the next round!");
								SENDPACKET(u, &sPacket, TRUE);

								u->iBellatraRoomInitialPlayerCount = 0;
							}
						}
					}
				}
			}

		}

		//szName of monster becomes empty when sod is finished
		if (bForceInit || psBellatraData->saMonster[0].szName[0] == 0 )
		{
			bSodIsActive = FALSE;
			bForceInit = FALSE;

			DEBUGIF(bDebug, "bForceInit | psBellatraData->saMonster[0].szName[0] == 0 ");


			switch ( Dice::RandomI(0, 2) )
			{
				case 0:
					SetBellatraData( (BellatraData*)0x07AC4E98 );
					break;
				case 1:
					SetBellatraData( (BellatraData*)0x07AC46DC );
					break;
				case 2:
					SetBellatraData( (BellatraData*)0x07AC3F20 );
					break;
			}
			DEBUGIF(bDebug, "SetRound( 0 )");

			//Set round to 0
			SetRound( 0 );

			int * piaRoomKills = ((int*)pfnRoomScoreKill);

			ZeroMemory( piaRoomKills, sizeof( int ) * 8 );
			ZeroMemory( baBossSpawn, sizeof( BOOL ) * 8 );

			psBellatraData = GetBellatraData() + (GetRound() - 1);

			CopyMemory( GetBellatraMonster( 0 ), &psBellatraData->saMonster[0], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 1 ), &psBellatraData->saMonster[1], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 2 ), &psBellatraData->saMonster[2], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 3 ), &psBellatraData->saMonster[3], sizeof( BellatraData::BellatraUnitData ) );
			CopyMemory( GetBellatraMonster( 4 ), &psBellatraData->sBoss, sizeof( BellatraData::BellatraUnitData ) );

			SetMaxMonsterInRooms( psBellatraData->iMaxMonsters );

			ZeroMemory( BellatraServer::GetBossSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );
			ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );
			(*(CharacterData**)0x07AC9B90) = psBellatraData->sBoss.psCharacterData;

			//Set timeout 30s for existing
			//users to be kicked out from final round
			SetTimeout( TICKCOUNT + 30000 ); //dwHardCoreEvent_OutTime

			WRITEDWORD( 0x07AB26B0, 0 );
			WRITEDWORD( 0x07AB2768, 0 );
			WRITEDWORD( 0x07AB276C, 0 );
			WRITEDWORD( 0x07AB2770, 0 );
			WRITEDWORD( 0x07AB2774, 0 );

			int iTotalScore = 0;
			int iSurvivorCount = 0;

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
				{
					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						iTotalScore += ud->iBellatraScore;
						iSurvivorCount++;
					}
				}
			}

			ZeroMemory( GetBellatraResultPacket(), sizeof( PacketBellatraResult ) );

			int iGold = 0;

			int numOfOriginalUsersInLeadingRoom = GetOriginalUserRoomCount(GetRankRoomID(0));

			DoWeeklyResetIfRequired();
			SyncOldClanTablesWithNew();

			auto clanIds = std::vector<int>();
			auto individualScores = std::vector<int>();

			for (int i = 0; i < PLAYERS_MAX; i++)
			{
				UserData* ud = USERSDATA + i;

				if (ud->pcSocketData && ud->iID && ud->dwExclusiveNum == 0x100)
				{
					//update clan id if needed
					ud->sCharacterData.iClanID = CHARACTERSERVER->SQLGetCharacterLongClanID(CHARACTERSERVER->GetCharacterName(ud));
					ud->iClanID = CHARACTERSERVER->GetClanCode(ud->sCharacterData.iClanID);

					clanIds.push_back(ud->sCharacterData.iClanID);

					if (!ud->IsDead() && MAPSERVER->IsBellatraMapArea(ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ))
					{
						individualScores.push_back(ud->iBellatraScore);
					}
					else
					{
						individualScores.push_back(0);
					}
				}
			}

			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->pcSocketData && ud->iID && ud->dwExclusiveNum == 0x100)
				{
					int userClanId = ud->sCharacterData.iClanID;

					int clanSurvivorsCount = 0;
					int clanScore = 0;
					for (UINT i = 0; i < clanIds.size(); i++)
					{
						if (userClanId == clanIds[i])
						{
							clanSurvivorsCount++;
							clanScore += individualScores[i];
						}
					}

					if (numOfOriginalUsersInLeadingRoom >= iRankingPointsMinNumOfSurvivorsReq)
					{
						if (clanSurvivorsCount < iRankingPointsMinNumOfSurvivorsReq)
						{
							CHATSERVER->SendChatEx(USERDATATOUSER(ud), CHATCOLOR_Global, "Ranking> Not enough survivors in your clan! Need %d or more", iRankingPointsMinNumOfSurvivorsReq);
						}
						else if (clanSurvivorsCount != numOfOriginalUsersInLeadingRoom)
						{
							CHATSERVER->SendChatEx(USERDATATOUSER(ud), CHATCOLOR_Global, "Ranking> Number of clan survivors = %d", clanSurvivorsCount);
							CHATSERVER->SendChatEx(USERDATATOUSER(ud), CHATCOLOR_Global, "Ranking> Adjusted room points = %s", FormatNumber(clanScore));
						}
						else
						{
							CHATSERVER->SendChatEx(USERDATATOUSER(ud), CHATCOLOR_Global, "Ranking> Everyone survived. Well done!");
							CHATSERVER->SendChatEx(USERDATATOUSER(ud), CHATCOLOR_Global, "Ranking> Total room points (100%%) = %s", FormatNumber(iTotalScore));
						}
					}


					DEBUGIF(bDebug, "BellatraServer::UpdateServer D");


					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						auto user = USERDATATOUSER(ud);

						PacketBellatraInformation s;
						s.iLength = sizeof( PacketBellatraInformation );
						s.iHeader = PKTHDR_BellatraInformation;
						s.iType = 4;
						s.iRound = GetRound() - 1;
						s.iUnk = 100;
						s.iValue = 0;
						SENDPACKET(user, &s, TRUE );

						PacketBellatraResult * ps = GetBellatraResultPacket();

						if ( ps->iResultCount < 32 && ud->bHideMode == FALSE )
						{
							STRINGCOPY( ps->saResult[ps->iResultCount].szName, CHARACTERSERVER->GetCharacterName(ud));
							ps->saResult[ps->iResultCount].pcUserData = ud;
							ps->saResult[ps->iResultCount].iClanID = ud->sCharacterData.iClanID;
							ps->saResult[ps->iResultCount].iClass = ud->sCharacterData.iClass;
							ps->saResult[ps->iResultCount].iScore = ud->iBellatraScore;
							ps->saResult[ps->iResultCount].iMonster = ud->iBellatraKills;
							ps->iResultCount++;
						}

						BellatraServer::SendScoreUser( ud, ud->iBellatraScore, ud->iBellatraKills, iTotalScore, iSurvivorCount, numOfOriginalUsersInLeadingRoom, (*(int*)0x07AB1550) );
					}
					else
						BellatraServer::SendScoreUser( ud, -1, ud->iBellatraKills, 0, 0, numOfOriginalUsersInLeadingRoom, (*(int*)0x07AB1550) );
				}
			}

			PacketBellatraResult * ps = GetBellatraResultPacket();
			ps->iLength = sizeof( PacketBellatraResult ) - ((32 - ps->iResultCount) * sizeof( PacketBellatraResult::BellatraResult ));
			ps->iUnk = (*(int*)0x07AB1550);
			ps->dwTime = READDWORD( 0x07AC9D6C );
			ps->bShow = TRUE;

			for ( int i = 0; i < ps->iResultCount; i++ )
			{
				UserData * ud = ps->saResult[i].pcUserData;

				if ( ud && ud->pcSocketData )
					SENDPACKET( USERDATATOUSER(ud), ps, TRUE );
			}

			ps->bShow = FALSE;


			return TRUE;
		}


		CopyMemory( GetBellatraMonster( 0 ), &psBellatraData->saMonster[0], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 1 ), &psBellatraData->saMonster[1], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 2 ), &psBellatraData->saMonster[2], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 3 ), &psBellatraData->saMonster[3], sizeof( BellatraData::BellatraUnitData ) );
		CopyMemory( GetBellatraMonster( 4 ), &psBellatraData->sBoss, sizeof( BellatraData::BellatraUnitData ) );

		SetMaxMonsterInRooms( psBellatraData->iMaxMonsters );

		(*(CharacterData**)0x07AC9B90) = psBellatraData->sBoss.psCharacterData;

		int * piaRoomKills = ((int*)pfnRoomScoreKill);

		ZeroMemory( BellatraServer::GetMonsterSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );

		ZeroMemory( BellatraServer::GetBossSpawnHandle(), sizeof( BellatraMonsterSpawnHandle ) );

		ZeroMemory( piaRoomKills, sizeof( int ) * 8 );
		ZeroMemory( baBossSpawn, sizeof( BOOL ) * 8 );

		if ( GetBellatraMonster( 0 )->psCharacterData )
		{
			SetDelayTime( TICKCOUNT + (psBellatraData->iDelayTime * 1000) );

			sPacket1.iLength = sizeof( PacketBellatraInformation );
			sPacket1.iHeader = PKTHDR_BellatraInformation;
			sPacket1.iType = 1;
			sPacket1.iRound = GetRound() - 1;
			sPacket1.iUnk = -1;
			sPacket1.iValue = 0;

			PacketBellatraInformation sPacket2;
			sPacket2.iLength = sizeof( PacketBellatraInformation );
			sPacket2.iHeader = PKTHDR_BellatraInformation;
			sPacket2.iType = 3;
			sPacket2.iRound = GetRound() - 1;
			sPacket2.iUnk = -1;
			sPacket2.iValue = 0;

			//Set Team Rank
			CALL( 0x0054DA30 );

			if ( GetRound() == 4 || GetRound() == 7 )
			{
				DEBUGIF(bDebug, "ROUND 4 or 7");

				if ( GetRound() == 4 )
				{
					DEBUGIF(bDebug, "ROUND 4");

					SetUserRoomCount( 0, 0 );
					SetUserRoomCount( 1, 0 );
					SetUserRoomCount( 2, 0 );
					SetUserRoomCount( 3, 0 );

					for ( int i = 0; i < PLAYERS_MAX; i++ )
					{
						UserData * ud = USERSDATA + i;

						if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 && !ud->IsDead())
						{
							if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
							{
								SetUserRoomCount( ud->sBellatraRoomID, GetUserRoomCount( ud->sBellatraRoomID ) + 1 );
							}
						}
					}
				}

				for ( int i = 0; i < PLAYERS_MAX; i++ )
				{
					UserData * ud = USERSDATA + i;

					if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 )
					{
						if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ))
						{
							//Top two teams will advance to round 5
							//GetRankRoomID( 0 ) - get first ranking room
							//GetRankRoomID( 1 ) - get second ranking room and is round 4
							if ((ud->sBellatraRoomID == GetRankRoomID( 0 ) || (ud->sBellatraRoomID == GetRankRoomID( 1 ) && GetRound() == 4)) && !ud->IsDead())
							{
								DEBUGIF(bDebug, "Advancing to next round: %s", CHARACTERSERVER->GetCharacterName(ud));

								//leading room
								if ( ud->sBellatraRoomID == GetRankRoomID( 0 ) )
									sPacket1.iUnk = iaBellatraRoundRoomTable[0][GetRound() - 1];
								else
									sPacket1.iUnk = iaBellatraRoundRoomTable[2][GetRound() - 1];

								SENDPACKET( USERDATATOUSER(ud), &sPacket1, TRUE );
							}

							//The rest of the teams will be given out gold based on their score
							// It won't be much because round 1-4 doesn't give out much gold
							else
							{

								SENDPACKET( USERDATATOUSER(ud), &sPacket2, TRUE );

								BellatraServer::SendScoreUser( ud, -1, ud->iBellatraKills, 0, 0, GetOriginalUserRoomCount(ud->sBellatraRoomID), (*(int*)0x07AB1550) );

								SendPrizeAndClearUserBellatraStats(ud, ud->sBellatraRoomID, ud->IsDead() ? 0.5f : 1.0f, true);

								ud->dwExclusiveNum = 0;
							}
						}
					}
				}
				sPacket1.iLength = 0;
			}
		}


		for ( int i = 0; i < PLAYERS_MAX; i++ )
		{
			UserData * ud = USERSDATA + i;

			if ( ud->pcSocketData && ud->dwExclusiveNum == 0x100 ) //0x100 = SOD Event flag (rsHARDCORE_EVENT_FLAG)
			{
				if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) && !ud->IsDead())
				{
					//this is sent to each sod player
					// at start of each round

					User* u = USERDATATOUSER(ud);

					if (u)
					{
						PacketNumberOfInitialBellatraPlayers sPacket;
						sPacket.iLength = sizeof(PacketNumberOfInitialBellatraPlayers);
						sPacket.iHeader = PKTHDR_NumfOfInitialBellatraPlayers;
						sPacket.iCount = u->iBellatraRoomInitialPlayerCount;
						SENDPACKET(u, &sPacket, TRUE);

						// if user is dead, don't send packet. the user will eventually
						// get kicked out
						if (sPacket1.iLength)
							SENDPACKET(u, &sPacket1, TRUE);
					}
				}
				else
				{
					BellatraServer::SendScoreUser(ud, -1, ud->iBellatraKills, 0, 0, GetOriginalUserRoomCount(ud->sBellatraRoomID), (*(int*)0x07AB1550));

					//user died but prize not given to user
					if (ud->iBellatraScore != -1)
					{
						//0.5f death penalty
						//include placement prize only if the user was in the leading sod team
						//otherwise it can be certain
						//round() == 1 is true when sod is finished. the user could be dead on the floor just before the end
						//then gets teleported out before the timeout
						SendPrizeAndClearUserBellatraStats(ud, ud->sBellatraRoomID, 0.5f, ud->sBellatraRoomID == GetRankRoomID(0) && (GetRound() >= 7 || GetRound() == 1));
					}


					ud->dwExclusiveNum = 0;
				}
			}
		}

		//set next round
		SetRound( GetRound() );

		return FALSE;
	}


	//Round index is zero
	if ( GetRound() == 1 )
	{
		//Users have 30 seconds to gather all manas
		//then they get teleported to Nav
		if ( GetTimeout() && GetTimeout() < TICKCOUNT )
		{
			DEBUGIF(bDebug, "GetRound() == 1 - removing users");
			DEBUGIF(bDebug, "GetTimeout() && GetTimeout() < TICKCOUNT");


			for ( int i = 0; i < PLAYERS_MAX + 256; i++ )
			{
				UserData * ud = USERSDATA + i;

				if ( ud->dwExclusiveNum == 0x100 ) //0x100 = SOD Event flag (rsHARDCORE_EVENT_FLAG)
				{
					if (ud->iBellatraScore != -1)
					{
						SendPrizeAndClearUserBellatraStats(ud, ud->sBellatraRoomID, 1.0f, true);
					}

					ud->dwExclusiveNum = 0;

					if ( MAPSERVER->IsBellatraMapArea( ud->sPosition.iX, ud->sPosition.iY, ud->sPosition.iZ ) )
					{
						if ( ud->pcSocketData && i < PLAYERS_MAX )
						{
							//send user to center of nav

							User* u = USERDATATOUSER(ud);

							if (u)
							{
								PacketNumberOfInitialBellatraPlayers sPacket1;
								sPacket1.iLength = sizeof(PacketNumberOfInitialBellatraPlayers);
								sPacket1.iHeader = PKTHDR_NumfOfInitialBellatraPlayers;
								sPacket1.iCount = 0;
								SENDPACKET(u, &sPacket1, TRUE);

								Map* psNaviskoMap = MAPSDATA + MAPID_NaviskoTown;
								PacketWarpGateField sPacket;
								sPacket.iLength = sizeof(PacketWarpGateField);
								sPacket.iHeader = PKTHDR_WarpGateField;
								sPacket.iX = psNaviskoMap->pcBaseMap->GetCenter().iX << 8;
								sPacket.iZ = psNaviskoMap->pcBaseMap->GetCenter().iY << 8;
								sPacket.iStage = MAPID_NaviskoTown;
								SENDPACKET(u, &sPacket);

								u->iBellatraRoomInitialPlayerCount = 0;
							}
						}
					}
				}
			}

			SyncOldClanTablesWithNew();
			CheckAndUpdateClanTopScore();
			UpdateCrownClanIDs();
			SendFullCrownDataToAllUsers();

			DEBUGIF(bDebug, "SetTotalUsers(0)");
			DEBUGIF(bDebug, "SetTimeout(0)");
			SetTotalUsers( 0 );
			SetTimeout( 0 );
			iSumEntranceFees = 0;

			if ( (*(BOOL*)0x07AC9E2C) == FALSE )
				WRITEDWORD( 0x07AC9E30, TRUE ); //rsHardCoreEventTimeOver = TRUE

			//rsSoD_StartCode = (ServerNameCode << 24) | ((tServerTime) & 0xFFFFFF);
			(*(int*)0x07AB1550) = (((*(DWORD*)0x07AC9D1C) << 24) | (((*(DWORD*)0x07AC9D6C) & 0xFFFFFF)));


			//ZeroMemory(rsSodTeamScore, sizeof(int) * 4);
			WRITEDWORD( 0x07AC9BB8, 0 );
			WRITEDWORD( 0x07AC9BBC, 0 );
			WRITEDWORD( 0x07AC9BC0, 0 );
			WRITEDWORD( 0x07AC9BC4, 0 );

			//ZeroMemory(rsSodUser, sizeof(sSOD_USER)*rsSOD_USER_MAX);
			ZeroMemory( (void*)0x07AC57F0, 0x2D0 * 4 );

			//rsSodUserCount = 0;
			WRITEDWORD( 0x07AB3074, 0 );

			bThirdPlacePrizeGivenOut = FALSE;
			bSecondPlacePrizeGivenOut = FALSE;
			bBroadcastSoDStartingSoonAtHour23 = TRUE;
			bBroadcastSoDEntryIsOpenAtHour4 = TRUE;
		}

		return FALSE;
	}

	//Update every 1 second
	if ( GetTimeUpdate() < TICKCOUNT )
	{
		CALL_WITH_ARG1( 0x0054DAB0, 0 ); //rsTransUpdateSod - sends team and individual score info to each player
		SetTimeUpdate( TICKCOUNT + 1000 );
	}

	int iRoundCount = GetRound() - 2;

	//This controls round start
	// the delay is defined in the Event > Bellatra database
	// If round index is 1, the result will be -1 therefore dont spawn any monsters yet
	// When round index is 2 = the round count will be 0
	// So it looks like round 1 (index = 0) is just for preparation?
	if ( iRoundCount < 0 || iRoundCount >= 8 || GetDelayTime() > TICKCOUNT )
		return FALSE;

	//get current monster count in room
	int * piaRoomKills = ((int*)pfnRoomScoreKill);

	for ( int i = iaBellatraRoundTable[iRoundCount][0]; i < iaBellatraRoundTable[iRoundCount][1]; i++ )
	{
		//current monster count in room
		int iMonsterCount = piaRoomKills[i];

		//we have already reached the max monster limit for sod
		// no need to spawn more
		if ( iMonsterCount > GetMaxMonsterInRooms() )
			continue;

		//Spawn boss after max number of have spawned and 5 monsters have been killed
		if ( (*(CharacterData**)0x07AC9B90) && baBossSpawn[i] == FALSE && GetMonsterSpawnHandle()->saRound[i].iCount > (GetMaxMonsterInRooms() + 5) )
		{
			//copy boss to monster spawn handler
			CopyMemory( &psMonsterData->sCharacterData, (*(CharacterData**)0x07AC9B90), sizeof( CharacterData ) );
			GetBossSpawnHandle()->saRound[i].iCount++;
			piaRoomKills[i]++;
			baBossSpawn[i] = TRUE;
		}
		else
		{
			for ( int j = 0; j < 4; j++ )
			{
				//chance for each four monster spawning
				// per tick
				int iChance = Dice::RandomI(1, 100);
				if ( GetBellatraMonster( j )->psCharacterData && iChance <= GetBellatraMonster( j )->iPercent )
				{
					CopyMemory( &psMonsterData->sCharacterData, GetBellatraMonster( j )->psCharacterData, sizeof( CharacterData ) );
					break;
				}
			}
		}

		int iX, iY, iZ;

		//rsGetHardCoreEventOpenMonsterPos
		//this determines where the monster spawns
		CALL_WITH_ARG4( 0x0054D9B0, (DWORD)&iX, (DWORD)&iY, (DWORD)&iZ, i );

		psMonsterData->iPositionState = 1;

		psMonsterData->sCharacterData.iType = ECharacterType::CHARACTERTYPE_Monster;

		pcStage->bForceCreateMonster = TRUE;

		UnitData * pcUnitData = MAPSERVER->CreateUnit( pcStage, psMonsterData, 0 );
		if ( pcUnitData )
		{
			char szName[32] = { 0 };
			STRINGCOPY( szName, pcUnitData->GetName() );

			int iLength = STRLEN( szName );

			//_SOD Fix
			if ( iLength > 4 )
				szName[iLength - 4] = 0;

			ZeroMemory( pcUnitData->sCharacterData.szName, 32 );
			STRINGCOPY( pcUnitData->sCharacterData.szName, szName );

			pcUnitData->sPosition = Point3D( iX, iY, iZ );

			pcUnitData->Animate( ANIMATIONTYPE_Idle );
			pcUnitData->iTargetMoveCount = 0;
			pcUnitData->PartyFlag = 0x100; //SoD event id
			pcUnitData->PartyParam = i;

			pcUnitData->sSpawnPosition = saBellatraSpawnPoints[i][2];
			pcUnitData->sSpawnPosition <<= 8;
			pcUnitData->iLureDistance = (12 * 64 * 12 * 64);

			pcUnitData->sCharacterData.iSight = (1300 * 1300);

			GetMonsterSpawnHandle()->saRound[i].iCount++;
			piaRoomKills[i]++;
		}
	}

	return FALSE;
}

//Send prize to user
// And set Bellatra Score to -1 and Bellatra Kills to -1
void BellatraServer::SendPrizeAndClearUserBellatraStats( UserData * userData, const short sBellatraRoomId, const float fPercentOfPrize, const bool bIncludePrizePool)
{
	DEBUGIF(bDebug, "SendPrizeAndClearUserBellatraStats %d %.1f %d", sBellatraRoomId, fPercentOfPrize, bIncludePrizePool);


	int iRoomScore = GetRoomScore(sBellatraRoomId);

	if (iRoomScore > 0)
	{
		int iPrizePool = GetSumPrizePoolFromEntranceFees();

		DEBUGIF(bDebug, "iSumEntranceFees %d", iSumEntranceFees);
		DEBUGIF(bDebug, "fPrizePool %d", iPrizePool);
		DEBUGIF(bDebug, "iRoomScore %d", iRoomScore);

		int numOfInitialUserCountInRoom = GetOriginalUserRoomCount(sBellatraRoomId);

		DEBUGIF(bDebug, "numOfInitialUserCountInRoom %d", numOfInitialUserCountInRoom);
		if (numOfInitialUserCountInRoom == 0)
			numOfInitialUserCountInRoom = 1; //set to 1. otherwise when doing 'float / 0' = it gives a crazy big number. safe-guard

		auto u = USERDATATOUSER(userData);

		CHATSERVER->SendChatEx(u, CHATCOLOR_White, "> Bellatra SoD Results ----------------");

		if (numOfInitialUserCountInRoom > 1)
		{
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Team score: %s", FormatNumber(iRoomScore));
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Your score: %s", FormatNumber(userData->iBellatraScore));
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Your kills: %d", userData->iBellatraKills);
		}
		else
		{
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Total score: %s", FormatNumber(iRoomScore));
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Total kills: %d", userData->iBellatraKills);
		}

		int iRoomScoreGold = 0;
		if (EVENT_BELLATRA_ROOM_SCORE_TO_GOLD_SCALE != 0)
		{
			//Convert score to gold based on scale
			float fRoomScoreGold = iRoomScore / (EVENT_BELLATRA_ROOM_SCORE_TO_GOLD_SCALE * 1.0f);
			iRoomScoreGold = static_cast<int>(round(fRoomScoreGold));
		}

		DEBUGIF(bDebug, "iRoomScoreGold %.0f", iRoomScoreGold);

		CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Room score prize: %s gp", FormatNumber(iRoomScoreGold));

		int iUserPlacementPrize = 0;
		if (bIncludePrizePool)
		{
			iUserPlacementPrize = GetUserPlacementPrize(iPrizePool, sBellatraRoomId);
			if (iUserPlacementPrize > 0)
			{
				CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - %s placement prize: %s gp", GetPlacementString(sBellatraRoomId), FormatNumber(iUserPlacementPrize));
			}
		}

		DEBUGIF(bDebug, "iUserPlacementPrize %d", iUserPlacementPrize);



		//how much of the score belongs to the user
		float fPercentOfScore = 0.0f;

		if (userData->iBellatraScore > 0)
		{
			fPercentOfScore = userData->iBellatraScore / (iRoomScore * 1.0f);

			if (fPercentOfScore > 1.0f)
				fPercentOfScore = 1.0f;

			DEBUGIF(bDebug, "fPercentOfScore = %.1f", fPercentOfScore);
		}

		int iUserPerformanceGold = static_cast<int>(((iRoomScore / (EVENT_BELLATRA_PERFORMANCE_GOLD_SCALE * 1.0f)) * fPercentOfScore));

		CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Your performance prize: %s gp", FormatNumber(iUserPerformanceGold));

		DEBUGIF(bDebug, "iUserPerformanceGold = %d", iUserPerformanceGold);

		int iTotalGold = static_cast<int>((iRoomScoreGold + iUserPlacementPrize + iUserPerformanceGold) * fPercentOfPrize);

		DEBUGIF(bDebug, "iTotalGold = %d", iTotalGold);

		int iTaxValue = AddTaxGoldToTopThreeClans(iTotalGold);

		if (iTaxValue > 0)
		{
			//subtract tax
			iTotalGold -= iTaxValue;

			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - Bellatra Clan(s) Tax: %s gp", FormatNumber(iTaxValue));
		}

		DEBUGIF(bDebug, "iTotalGold (After tax) = %d", iTotalGold);

		//safe-guard
		if (iTotalGold < 0)
			iTotalGold = 0;

		if (iTotalGold)
		{
			CHARACTERSERVER->GiveGOLD(u, iTotalGold, WHEREID_BellatraSOD);
		}

		if (fPercentOfPrize != 1.0f)
		{
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - You received %s gp (50%% death penalty)", FormatNumber(iTotalGold));
		}
		else
		{
			CHATSERVER->SendChatEx(u, CHATCOLOR_White, " - You received a total of %s gp!", FormatNumber(iTotalGold));
		}

		CHATSERVER->SendChatEx(u, CHATCOLOR_White, "----------------------------------------------------");

		userData->iBellatraScore = -1;
		userData->iBellatraKills = -1;


	}
}

const char * BellatraServer::GetPlacementString(const short bellatraRoomId)
{
	if (bellatraRoomId == GetRankRoomID(0))
		return "1st";

	else if (bellatraRoomId == GetRankRoomID(1))
		return "2nd";

	else if (bellatraRoomId == GetRankRoomID(2))
		return "3rd";

	return "4th";
}


int BellatraServer::GetPlacement(const short bellatraRoomId)
{
	if (bellatraRoomId == GetRankRoomID(0))
		return 1;

	else if (bellatraRoomId == GetRankRoomID(1))
		return 2;

	else if (bellatraRoomId == GetRankRoomID(2))
		return 3;

	return 4;
}

int BellatraServer::GetUserPlacementPrize(const int iPrizePool, const short bellatraRoomId)
{
	int iOriginalUserCount = GetOriginalUserRoomCount(bellatraRoomId);

	DEBUGIF(bDebug, "iOriginalUserCount = %d, room id = %d", iOriginalUserCount, bellatraRoomId);
	DEBUGIF(bDebug, "GetRound() = %d", GetRound());
	DEBUGIF(bDebug, "GetRankRoomID(0) = %d", GetRankRoomID(0));
	DEBUGIF(bDebug, "GetRankRoomID(1) = %d", GetRankRoomID(1));
	DEBUGIF(bDebug, "GetRankRoomID(2) = %d", GetRankRoomID(2));


	if (iOriginalUserCount == 0)
		iOriginalUserCount = 6;  //safegard against div by 0 in case of bug

	float fUserPlacementPrize = 0.0f;

	//Third place
	//At round 4 onrwards, there will only be two teams remaining
	if (GetRound() >= 4 && bellatraRoomId == GetRankRoomID(2))
	{
		DEBUGIF(bDebug, "GetRound() >= %d && bellatraRoomId == GetRankRoomID(2)", GetRound());

		float fPlacementPrizePool = iPrizePool * 0.10f;
		fUserPlacementPrize = fPlacementPrizePool / iOriginalUserCount;

		bThirdPlacePrizeGivenOut = TRUE;
	}

	//Second place
	//At round 7 onrwards, there will only be one team remaining
	if (GetRound() >= 4 && bellatraRoomId == GetRankRoomID(1))
	{
		DEBUGIF(bDebug, "GetRound() >= %d && bellatraRoomId == GetRankRoomID(1)", GetRound());

		float fPercentage = bThirdPlacePrizeGivenOut ? 0.25f : 0.30f;
		float fPlacementPrizePool = iPrizePool * fPercentage;
		fUserPlacementPrize = fPlacementPrizePool / iOriginalUserCount;

		bSecondPlacePrizeGivenOut = TRUE;
	}

	//First place place. For leading sod, the round index would be 0
	//by the time this gets called
	if ((GetRound() == 1 || GetRound() >= 7) && bellatraRoomId == GetRankRoomID(0))
	{
		DEBUGIF(bDebug, "GGetRound() >= 7 && bellatraRoomId == GetRankRoomID(0)");

		float fPercentage = 1.0f;

		if (bThirdPlacePrizeGivenOut)
		{
			fPercentage = 0.65f;
		}
		else if (bSecondPlacePrizeGivenOut)
		{
			fPercentage = 0.70f;
		}

		float fPlacementPrizePool = iPrizePool * fPercentage;
		fUserPlacementPrize = fPlacementPrizePool / iOriginalUserCount;
	}

	DEBUGIF(bDebug, "fUserPlacementPrize = %.0f", fUserPlacementPrize);

	return static_cast<int>(fUserPlacementPrize);
}

int BellatraServer::GetSumPrizePoolFromEntranceFees()
{
	//note - clan tax is applied on  gold received from player, not based on entrance fees

	float percent = 1.0f - (EVENT_BELLATRA_MAINTENANCE_TAX / 100.0f);

	if (percent > 1.0f)
		percent = 1.0f;
	else if (percent < 0.0f)
		percent = 0.0f;

	return static_cast<int>(iSumEntranceFees * percent);
}


void BellatraServer::SQLBellatraRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity )
{
	DEBUGIF(bDebug, "BellatraServer::SQLBellatraRewardLog");


	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 8 );
	if ( pcDB->Open() )
	{

		if ( pcDB->Prepare( "INSERT INTO BellatraRewardLog([AccountName],[Name],[ItemName],[Quantity],[Date]) VALUES (?,?,?,?,GETDATE())" ) )
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

void BellatraServer::HandlePacket( User * pcUser, PacketReceiveItemIhin * psPacket )
{
	if ( (psPacket->iCount - 80) >= 15 )
	{
		UserData * ud = pcUser->pcUserData;

		if ( ud && ud->pcSocketData )
		{
			/* JLM - disabled
			SPrizeData * ps = GetPrizeBellatraRewardItem();

			if ( ps->bActive )
			{
				char szItemName[32] = { 0 };
				if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
				{
                    if (STRINGCOMPAREI("GG101", ps->szCode))
                    {
                        USERSERVER->AddServerUserGold(ud, ps->iQuantity, WHEREID_BellatraSOD);
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_White, "Bellatra> You won %s gp!", FormatNumber( ps->iQuantity ) );
                    }
                    else
                    {
                        USERSERVER->SetUserID(ud);

                        ITEMSERVER->AddItemOpenBox(ud, ps->szCode, 0, ps->iQuantity);
                        SQLBellatraRewardLog(ud->szAccountName, CHARACTERSERVER->GetCharacterName(ud), ps->szCode, ps->iQuantity);

						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_White, "Bellatra> You won %s[%s]!", szItemName, FormatNumber( ps->iQuantity ) );
                        CHATSERVER->SendChat(pcUser, CHATCOLOR_White, "Bellatra> Check Item Distributor to get your prize!");
                    }
				}
			}*/
		}
	}
}

void BellatraServer::DropBellatraRewards(int iNumOfDrops)
{
	std::cout << "BellatraServer::DropBellatraRewards" << std::endl;

	Map* psSOD = MAPSDATA + MAPID_Bellatra;
	std::vector<std::tuple<int, int>> dropCoords;

	for (int i_DropIndex = 0; i_DropIndex < iNumOfDrops; i_DropIndex++)
	{
		auto prize = GetPrizeBellatraRewardItem();
		if (prize == nullptr)
			continue;

		auto defItem = ITEMSERVER->FindItemDefByCode(prize->dwItemCode);
		if (defItem == nullptr)
			continue;

		LootItem sLootItem;
		ITEMSERVER->CreateItem(&sLootItem.sItem, defItem, EItemSource::BellatraReward, 0);

		if (defItem->sItem.sItemID.ToItemBase() == ITEMBASE_Potion)
		{
			sLootItem.sItem.iPotionCount = prize->iQuantity;
			ITEMSERVER->ReformItem(&sLootItem.sItem);
		}

		int iX, iY, iZ;
		BOOL bFound = FALSE;

		//max 20 retries
		for (int i_Retries = 0; i_Retries < 20; i_Retries++)
		{
			//Get a random position in Bellatra
			iX = Dice::RandomI(iBellatraDropMinX, iBellatraDropMaxX) >> 8;
			iY = iBellatraDropY >> 8;
			iZ = Dice::RandomI(iBellatraDropMinZ, iBellatraDropMaxZ) >> 8;

			for (auto tuple : dropCoords)
			{
				int thisX = std::get<0>(tuple);
				int thisZ = std::get<1>(tuple);

				if (iX == thisX ||
					iZ == thisZ)
				{
					//already dropped a loot at this position. Try again..
					continue;
				}

				bFound = TRUE;
			}

			dropCoords.push_back(tuple<int, int>(iX, iZ));
		}

		if ( bFound == FALSE )
			WARN ( "Bellatra - Unable to find a drop spot" );



		//Add Item to Map
		Loot* psItemD = MAPSERVER->AddItemV2( psSOD, &sLootItem, iX, iY, iZ, ELootType::Initial );
		if (psItemD)
		{
			psItemD->dwDropTime = TICKCOUNT + 5000;
			psItemD->dwEndTime = TICKCOUNT +  (3 * 60 * 1000); //3 minutes
			MAPSERVER->SendItemStageNearUsers(psSOD, psItemD);
		}
	}

	dropCoords.clear();
}

void BellatraServer::TestBellatraReward()
{
	if (LOGIN_SERVER)
		return;

	const int iRepeatCount = 10000;

	if (CreateDirectory("TestBellatra", NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		//Check and create dir
	}
	else
	{
		return;
	}


	std::map<std::string, int> vTestResult;


	std::string logFileName = FormatString("TestBellatra\\BellatraDrops.txt");
	CLogger* logger = new CLogger(logFileName);


	//for (int i = 0; i < iRepeatCount; i++)
	//{
	//	auto reward = GetPrizeBellatraRewardItem();

	//	auto defItem = ITEMSERVER->FindItemDefByCode(reward->dwItemCode);

	//	std::string itemName = defItem->sItem.szItemName;

	//	if (vTestResult.find(itemName) == vTestResult.end())
	//		vTestResult.insert({ itemName, 0 });

	//	vTestResult[itemName]++;
	//}

	logger->WriteLine("");
	logger->WriteLine("");
	logger->WriteLine(Util::CurrentDateTime());
	logger->WriteLine("==========================================================================================");
	logger->WriteLine("==========================================================================================");
	logger->WriteLine("10000 x Bellatra loot result:");
	logger->WriteLine("");


	std::string specName = "";
	for (auto testResult : vTestResult)
	{
		specName = testResult.first;
		padTo(specName, 20);

		float fChance = static_cast<float>(testResult.second) / static_cast<float>(iRepeatCount);
		logger->WriteLine("%s = %d (%.3f%%)", specName.c_str(), testResult.second, fChance * 100.f);
	}

	logger->WriteLine("-----------------------------------------------------------------");
	logger->Flush();
	logger->Close();
}

SPrizeData * BellatraServer::GetPrizeBellatraRewardItem()
{
	int iRand = Dice::RandomI(1, iTotalPercent);
	int iTotal = 0;

	for ( int i = 0; i < 50; i++ )
	{
		SPrizeData * p = saPrizes + i;
		if ( p->bActive )
		{
			iTotal += p->iPercent;
			if (iRand <= iTotal)
				return p;
		}
	}
	return &saPrizes[0];
}

void BellatraServer::ForceStartSod()
{
	WRITEDWORD(0x07AC9E30, TRUE);
	WRITEDWORD(0x07AC9E2C, TRUE);

	int iOriginalTime = static_cast<int>(MAPSERVER->GetTime() - MAPSERVER->iGameTimeShift);

	if (iOriginalTime < 0)
		iOriginalTime += 24;

	int iDiff = 6 - iOriginalTime;

	if (iDiff < 0)
		iDiff += 24;

	MAPSERVER->iGameTimeShift = iDiff;


	SetRound(0);

	bForceInit = TRUE;
	bForceHour = TRUE;
}

void BellatraServer::ForceEndSod()
{
	dwFinishTime = 0;
}

//For prize pool
void BellatraServer::AddEntranceFee(int fee)
{
	if (fee < 0)
		fee = 0;

	DEBUGIF(bDebug, "Added fee to entrance pool: %d", fee);

	iSumEntranceFees += fee;
}

void BellatraServer::SQLReadInRewardsTable()
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ServerDB, 9);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT ItemCode, Quantity, [Percent] FROM BellatraReward ORDER BY [Percent] DESC"))
		{
			if (pcDB->Execute())
			{
				int i = 0;
				while (pcDB->Fetch())
				{
					SPrizeData* p = saPrizes + i;
					pcDB->GetData(1, PARAMTYPE_String, p->szCode, 32);
					pcDB->GetData(2, PARAMTYPE_Integer, &p->iQuantity);
					pcDB->GetData(3, PARAMTYPE_Integer, &p->iPercent);
					p->bActive = TRUE;
					p->dwItemCode = ITEMSERVER->FindItemDefByCode(p->szCode)->sItem.sItemID.ToInt();

					iTotalPercent += p->iPercent;
					i++;
				}
			}
		}
		pcDB->Close();
	}
}

void BellatraServer::Init()
{
	//if ( GAME_SERVER )
	//{
	//	INFO("Initializing Bellatra (SOD)");
	//	SQLReadInRewardsTable();
	//}

	SetOriginalUserRoomCount(0, 0);
	SetOriginalUserRoomCount(1, 0);
	SetOriginalUserRoomCount(2, 0);
	SetOriginalUserRoomCount(3, 0);

	UpdateCrownClanIDs();
}

