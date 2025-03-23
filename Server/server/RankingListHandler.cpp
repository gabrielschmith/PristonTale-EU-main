#include "stdafx.h"
#include "RankingListHandler.h"

CRankingListHandler::CRankingListHandler()
{
}

CRankingListHandler::~CRankingListHandler()
{
}

void CRankingListHandler::SendBellatraSoloRankingData(User* pcUser, int iRankingType)
{
	//Top level, all players (0) or class type (1 - 10)
	if (iRankingType >= 0 && iRankingType <= 10)
	{
		PacketSoloRankingList s;
		ZeroMemory(&s, sizeof(PacketSoloRankingList));
		s.iRankingType = iRankingType;
		s.iLength = sizeof(PacketSoloRankingList);
		s.iHeader = PKTHDR_NPCRankListSoloSod;

		SQLConnection* pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );
		if (pcDB->Open())
		{
			std::string query = "";

			if (iRankingType == 0)
			{
				query = " SELECT TOP (30) [CharacterName], [CharacterClass], [CharacterLevel], [Kills], [Score]"
						" FROM (SELECT t.*, row_number() over ( partition by CharacterID order by Score desc ) as seqnum"
						" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t"
						" ) t"
						" WHERE seqnum <= 1"
						" ORDER BY Score DESC, Date ASC";
			}
			else
			{
				query = " SELECT TOP (30) [CharacterName], [CharacterClass], [CharacterLevel], [Kills], [Score]"
						" FROM (SELECT t.*, row_number() over ( partition by CharacterID order by Score desc ) as seqnum"
						" FROM [ClanDB].[dbo].[BellatraPersonalScoreNew] AS t"
						" WHERE t.CharacterClass = ?"
						" ) t"
						" WHERE seqnum <= 1"
						" ORDER BY Score DESC, Date ASC";
			}

			int i = 0;

			if (pcDB->Prepare(query.c_str()))
			{
				if (iRankingType != 0)
					pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iRankingType);

				if (pcDB->Execute())
				{
					while (pcDB->Fetch())
					{
						pcDB->GetData(1, PARAMTYPE_String,   s.szCharacterName[i], 32);
						pcDB->GetData(2, PARAMTYPE_Integer, &s.iClass[i]);
						pcDB->GetData(3, PARAMTYPE_Integer, &s.iLevel[i]);
						pcDB->GetData(4, PARAMTYPE_Integer, &s.iKills[i]);
						pcDB->GetData(5, PARAMTYPE_Integer, &s.iScore[i]);

						i++;
					}
				}
			}

			pcDB->Close();
		}

		SENDPACKET(pcUser, &s);
	}
}

void CRankingListHandler::SendRankingData(User * pcUser, int iRankingType)
{
	//Top level, all players (0) or class type (1 - 10)
	if (iRankingType >= 0 && iRankingType <= 10)
	{
		PacketRankingListAllPlayer s;
		ZeroMemory(&s, sizeof(PacketRankingListAllPlayer));
		s.iRankingType = iRankingType;
		s.iLength = sizeof(PacketRankingListAllPlayer);
		s.iHeader = PKTHDR_NPCRankListAllPlayers;

		SQLConnection* pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Bless );
		if (pcDB->Open())
		{
			std::string query = "";

#ifdef SEASONAL_IS_ACTIVE
			//if seasonal is active, use current seasonal num for one of the above query.
			int iSeasonalNum = CURRENT_SEASONAL_NUM;
#else
			//otherwise '999' (return all chars)
			int iSeasonalNum = 999;
#endif

			if (iRankingType == 0)
			{
				if ( SERVER_IS_SEASONAL )
				{
					query = "SELECT DISTINCT TOP 50 [Name], [Level], [JobCode], [Experience], [LevelUpDate] FROM CharacterInfo"
						" WHERE GMLevel < 2"  //ignore Admins / GMs
						" AND Banned = 0"
						" AND Seasonal = ?"
						" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
				}
				else
				{
					//this query should be used by main server only
					query = "SELECT DISTINCT TOP 50 [Name], [Level], [JobCode], [Experience], [LevelUpDate] FROM CharacterInfo"
						" WHERE GMLevel < 2" //ignore Admins / GMs
						" AND Banned = 0"
						" AND Seasonal < ?"
						" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
				}
			}
			else
			{
				if ( SERVER_IS_SEASONAL )
				{
					query = "SELECT DISTINCT TOP 50 [Name], [Level], [JobCode], [Experience], [LevelUpDate] FROM CharacterInfo"
						" WHERE GMLevel < 2"  //ignore Admins / GMs
						" AND Banned = 0"
						" AND JobCode = ?"
						" AND Seasonal = ?"
						" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
				}
				else
				{
					//this query should be used by main server only
					query = "SELECT DISTINCT TOP 50 [Name], [Level], [JobCode], [Experience], [LevelUpDate] FROM CharacterInfo"
						" WHERE GMLevel < 2"  //ignore Admins / GMs
						" AND Banned = 0"
						" AND JobCode = ?"
						" AND Seasonal < ?"
						" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
				}
			}

			int i = 0;

			if (pcDB->Prepare(query.c_str()))
			{
				if ( iRankingType != 0 )
				{
					pcDB->BindParameterInput ( 1, PARAMTYPE_Integer, &iRankingType );
					pcDB->BindParameterInput ( 2, PARAMTYPE_Integer, &iSeasonalNum );
				}
				else
				{
					pcDB->BindParameterInput ( 1, PARAMTYPE_Integer, &iSeasonalNum );
				}

				if (pcDB->Execute())
				{
					while (pcDB->Fetch())
					{
						pcDB->GetData(1, PARAMTYPE_String, s.szCharacterName[i], 32);
						pcDB->GetData(2, PARAMTYPE_Integer, &s.iLevel[i]);
						pcDB->GetData(3, PARAMTYPE_Integer, &s.iClass[i]);
						pcDB->GetData(4, PARAMTYPE_Int64, &s.iExperience[i]);

						i++;
					}
				}
			}

			pcDB->Close();
		}

		SENDPACKET(pcUser, &s);
	}

	/*char szAccountName[3][32] = { 0 };
	char szCharacterName[3][32] = { 0 };
	int iCharacterScore[3] = { 0 };
	int iClanID[3] = { 0 };

	SQLConnection* pcDB2 = SQLCONNECTION(DATABASEID_UserDB_PrimaryServer_3);
	// pegando clanID top 1 bless
	if (pcDB2->Open())
	{
		if (pcDB2->Prepare("SELECT TOP 3 [AccountName],[Name],[ClanID],[BlessCastleScore] FROM CharacterInfo ORDER BY BlessCastleScore DESC"))
		{
			int i = 0;

			if (pcDB2->Execute())
			{
				while (pcDB2->Fetch())
				{
					pcDB2->GetData(1, PARAMTYPE_String, szAccountName[i], 32);
					pcDB2->GetData(2, PARAMTYPE_String, szCharacterName[i], 32);
					pcDB2->GetData(3, PARAMTYPE_Integer, &iClanID[i]);
					pcDB2->GetData(4, PARAMTYPE_Integer, &iCharacterScore[i]);

					i++;
				}
			}
		}

		pcDB2->Close();
	}

	// pegando dados top1 clan bless
	SQLConnection* pcDB3 = SQLCONNECTION(DATABASEID_ClanDB);
	if (pcDB3->Open())
	{
		if (pcDB3->Prepare("SELECT ClanName, ClanLeader, IconID FROM ClanList WHERE ID=?"))
		{
			pcDB3->BindParameterInput(1, PARAMTYPE_Integer, &iClanID[0]);

			if (pcDB3->Execute() && pcDB3->Fetch())
			{
				pcDB3->GetData(1, PARAMTYPE_String, s.szNameClan, 32); // Clan Name
				pcDB3->GetData(2, PARAMTYPE_String, s.szClanLeader, 32); // Clan Nick Lider
				pcDB3->GetData(3, PARAMTYPE_Integer, &s.iClanBlessIconID); // Clan Icon ID

			}
		}
		pcDB3->Close();
	}

	//TOP PVP
	SQLConnection* pcDB4 = SQLCONNECTION(DATABASEID_UserDB_PrimaryServer);
	int u = 0;
	if (pcDB4->Open())
	{
		if (pcDB4->Prepare("SELECT TOP 10 [CharacterName],[Kills],[Deaths] FROM CharacterPvP ORDER BY Kills DESC"))
		{

			if (pcDB4->Execute())
			{
				while (pcDB4->Fetch())
				{
					pcDB4->GetData(1, PARAMTYPE_String, s.szPVPCharName[u], 32);
					pcDB4->GetData(2, PARAMTYPE_Integer, &s.iPVPKills[u]);
					pcDB4->GetData(3, PARAMTYPE_Integer, &s.iPVPDeaths[u]);
					u++;
				}
			}
		}

		pcDB4->Close();
	}

	//TOP CLAN SOD
	SQLConnection* pcDB5 = SQLCONNECTION(DATABASEID_ClanDB);
	char szClanName[10][32] = { 0 };
	int b = 0;
	if (pcDB5->Open())
	{
		if (pcDB5->Prepare("SELECT TOP 10 [ClanName],[BellatraPoints] FROM ClanList WHERE BellatraPoints > 0 ORDER BY BellatraPoints DESC "))
		{

			if (pcDB5->Execute())
			{
				while (pcDB5->Fetch())
				{
					pcDB5->GetData(1, PARAMTYPE_String, s.szClanSodName[b], 32);
					pcDB5->GetData(2, PARAMTYPE_Integer, &s.iClanSodPoints[b]);

					STRINGCOPY(szClanName[b], s.szClanSodName[b]);

					SQLConnection* pcDB6 = SQLCONNECTION(DATABASEID_ClanDB_2);
					if (pcDB6->Open())
					{
						if (pcDB6->Prepare("SELECT MemCnt FROM CL WHERE ClanName=?"))
						{
							pcDB6->BindParameterInput(1, PARAMTYPE_String, szClanName[b]);

							if (pcDB6->Execute() && pcDB6->Fetch())
							{
								pcDB6->GetData(1, PARAMTYPE_Integer, &s.iClanSodMemberCount[b], 32);
							}
						}
						pcDB6->Close();
					}
					b++;
				}
			}
		}
		pcDB5->Close();
	}

	//TOP CLAN SOD PLAYER
	SQLConnection* pcDB7 = SQLCONNECTION(DATABASEID_ClanDB);
	int a = 0;
	if (pcDB7->Open())
	{
		if (pcDB7->Prepare("SELECT TOP 10 [CharacterName],[Score] FROM BellatraPersonalScore WHERE BellatraPersonalScore > 0 ORDER BY Score DESC"))
		{
			if (pcDB7->Execute())
			{
				while (pcDB7->Fetch())
				{
					pcDB7->GetData(1, PARAMTYPE_String, s.szSODPlayer[a], 32);
					pcDB7->GetData(2, PARAMTYPE_Integer, &s.iSODPlayerPoints[a]);
					a++;
				}
			}
		}

		pcDB7->Close();
	}
	*/


}
