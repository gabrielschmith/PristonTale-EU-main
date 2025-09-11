#include "stdafx.h"
#include "characterserver.h"
#include <math.h>
#include "Logger.h"
#include "HNSSkill.h"

typedef UINT(__cdecl* t_GetCodebyName) (char* nick);
t_GetCodebyName GetCodebyName = (t_GetCodebyName)0x00415F80;

extern void		CheckAndAutoSyncNewSkillsDataforGMsOnly(User* pcGMUser);
extern void		CheckAndAutoSyncNewSkillsInfoforGMsOnly(User* pcGMUser);


#define DEBUG_AURA FALSE
#define DEBUG_TITLE FALSE

CharacterServer::CharacterServer()
{
}

CharacterServer::~CharacterServer()
{
}


void CharacterServer::SQLUpdateGMLevel(const char* p_CharacterName, int iGMLevel)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET GMLevel=? WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Byte, &iGMLevel);
			pcDB->BindParameterInput(2, PARAMTYPE_String, (char*)p_CharacterName);

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

std::string CharacterServer::SQLGetAccountName(const char* p_CharacterName)
{
	if (p_CharacterName == NULL || p_CharacterName[0] == 0)
		return "";

	std::string result = "";
	char szAccountName[32] = { 0 };

	BOOL bRet = FALSE;
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT TOP(1) [AccountName] FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, (char*)p_CharacterName, 32);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_String, szAccountName, _countof(szAccountName));
				result = std::string(szAccountName);
			}
		}

		pcDB->Close();
	}

	return result;
}

bool CharacterServer::CheckCharacterAccountGouvernance(const char* p_CharacterName, const char* p_AccountName)
{
	if (p_CharacterName == NULL)
	{
		WARN("CheckCharacterAccountGouvernance - Character name is null");
		return FALSE;
	}

	if (p_CharacterName[0] == 0)
	{
		WARN("CheckCharacterAccountGouvernance - Character name is empty");
		return FALSE;
	}

	if (p_AccountName == NULL)
	{
		WARN("CheckCharacterAccountGouvernance - Account name is null");
		return FALSE;
	}
	if (p_AccountName[0] == 0)
	{
		WARN("CheckCharacterAccountGouvernance - Account name is empty");
		return FALSE;
	}

	BOOL bRet = FALSE;
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT [ID],[AccountName] FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, (char*)p_CharacterName);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				char szAccountName[32];
				pcDB->GetData(2, PARAMTYPE_String, szAccountName, _countof(szAccountName));

				std::string l_DBAccountName = szAccountName;
				std::string l_TestAccountName = p_AccountName;

				std::transform(l_DBAccountName.begin(), l_DBAccountName.end(), l_DBAccountName.begin(), ::toupper);
				std::transform(l_TestAccountName.begin(), l_TestAccountName.end(), l_TestAccountName.begin(), ::toupper);

				bRet = strcmp(l_DBAccountName.c_str(), l_TestAccountName.c_str()) == 0;
			}
		}

		pcDB->Close();
	}

	return bRet;
}

BOOL CharacterServer::SQLGetCharacter(SQLConnection* pcDB, const char* pszCharacterName, SQLCharacter* ps)
{
	if (pszCharacterName == NULL || pszCharacterName[0] == 0)
		return FALSE;

	if (ps == NULL)
		return FALSE;

	BOOL bRet = FALSE;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT [ID],[AccountName],[Level],[Experience],[Gold],[JobCode],[ClanID],[ClanPermission],[LastSeenDate],[LastStage] FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, (char*)pszCharacterName, STRLEN(pszCharacterName));

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &ps->iID);
				pcDB->GetData(2, PARAMTYPE_String, ps->szAccountName, 32);
				pcDB->GetData(3, PARAMTYPE_Integer, &ps->iLevel);
				pcDB->GetData(4, PARAMTYPE_Int64, &ps->iEXP);
				pcDB->GetData(5, PARAMTYPE_Integer, &ps->iGold);
				pcDB->GetData(6, PARAMTYPE_Integer, &ps->iClass);
				pcDB->GetData(7, PARAMTYPE_Integer, &ps->iClanID);
				pcDB->GetData(8, PARAMTYPE_Integer, &ps->iClanPermission);
				pcDB->GetData(9, PARAMTYPE_Time, &ps->sLastSeenDate);
				pcDB->GetData(10, PARAMTYPE_Integer, &ps->iLastMapID);

				bRet = TRUE;

				STRINGCOPY(ps->szCharacterName, pszCharacterName);
			}
		}

		pcDB->Close();
	}

	return bRet;
}

//get short clan id, i.e. 34
int CharacterServer::SQLGetCharacterClanID(char* pszCharacterName)
{
	int iID = -1;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT ClanID FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszCharacterName, STRLEN(pszCharacterName));

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iID);
			}
		}

		pcDB->Close();
	}

	return iID;
}

// Get long clan id, i.e. 10000004, for character
int CharacterServer::SQLGetCharacterLongClanID(char* pszCharacterName)
{
	int iShortClanID = SQLGetCharacterClanID(pszCharacterName);

	//user not found, or has no clan?
	if (iShortClanID == -1 || iShortClanID == 0)
		return 0;

	int iLongClanID = 0;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT TOP(1) IconID FROM ClanList WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iShortClanID);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iLongClanID);
			}
			else
			{
				WARN("Clan not found for id = %d. Char name = %s", iShortClanID, pszCharacterName);
			}
		}
		pcDB->Close();
	}

	//std::cout << pszCharacterName << ": Long clan id: " << iLongClanID << std::endl;

	return iLongClanID;
}

void CharacterServer::SQLRenameCharacter(char* pszCharacterName, char* pszNewCharacterName)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Name=? WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CharacterItemTimer SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CharacterQuest SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CharacterPvP SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}


		pcDB->Close();
	}

	//JLM - No need to rename in other tables
	//because at the end of seasonal,
	//CharacterItemTimer and CharacterQuest can just be merged with the main server.
	if (SERVER_IS_SEASONAL)
	{
		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_PrimaryServer);

		if (pcDB->Open())
		{
			if (pcDB->Prepare("UPDATE CharacterInfo SET Name=? WHERE Name=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
				pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

				pcDB->ExecuteUpdate();
			}

			pcDB->Close();
		}
	}

	pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE ClanList SET ClanLeader=? WHERE ClanLeader=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}


		pcDB->Clear();

		if (pcDB->Prepare("UPDATE BellatraPersonalScore SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE BellatraPersonalScoreNew SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();


		if (pcDB->Prepare("UPDATE BellatraTeamScore SET CharacterName=? WHERE CharacterName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CL SET ClanZang=? WHERE ClanZang=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}


		pcDB->Clear();

		if (pcDB->Prepare("UPDATE UL SET ChName=? WHERE ChName=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszNewCharacterName, 32);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}


		pcDB->Close();
	}
}

void CharacterServer::SQLChangeCharacterLevel(char* pszCharacterName, int iLevel, INT64 iEXP)
{
	if (SERVER_IS_SEASONAL)
	{

	}
}

void CharacterServer::SQLChangeCharacterLevel(SQLConnection* pcDB, char* pszCharacterName, int iLevel, INT64 iEXP)
{
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET [Level]=?,[Experience]=? WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iLevel);
			pcDB->BindParameterInput(2, PARAMTYPE_Int64, &iEXP);
			pcDB->BindParameterInput(3, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

void CharacterServer::SQLChangeCharacterClass(char* pszCharacterName, int iClass)
{
	SQLChangeCharacterClass(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, iClass);

	if (SERVER_IS_SEASONAL)
	{
		SQLChangeCharacterClass(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), pszCharacterName, iClass);
	}
}

void CharacterServer::SQLChangeCharacterClass(SQLConnection* pcDB, char* pszCharacterName, int iClass)
{
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET [JobCode]=? WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iClass);
			pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharacterName, 32);

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

/// <summary>
/// Tick Every 1 min
/// </summary>
void CharacterServer::Tick1Min()
{
	if (LOGIN_SERVER)
		return;

	UpdateAndSyncAuraAndDisplaySkinForAllClass();
	UpdatePlayerCharacterOverallRanking(); //top 1
}




void CharacterServer::UpdateAndSyncAuraAndDisplaySkinForAllClass()
{
	//DEBUG("CharacterServer::UpdateAndSyncAuraAndDisplaySkinForAllClass");

	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Archer);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Assassin);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Atalanta);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Fighter);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Knight);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Magician);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Mechanician);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Pikeman);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Priestess);
	UpdateAndSyncAuraAndDisplaySkinForClass(ECharacterClass::CHARACTERCLASS_Shaman);
}

/// <summary>
/// Called after char selection
/// Ranking will be checked every 1 min via UpdateAndSyncAuraAndDisplaySkinForAllClass
/// </summary>
void CharacterServer::SQLUpdatePlayerCharacterClassRanking(User* pcUser)
{
	if (pcUser->IsValidAndInGame() == FALSE)
		return;

	DEBUGIF(DEBUG_AURA, "CharacterServer::SQLUpdatePlayerCharacterClassRanking ( %s )", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 1);

	int iRanking = 0;
	if (pcDB->Open())
	{
#ifdef SEASONAL_IS_ACTIVE
		//if seasonal is active, use current seasonal num for one of the above query.
		int iSeasonalNum = CURRENT_SEASONAL_NUM;
#else
		//otherwise '999' (return all chars)
		int iSeasonalNum = 999;
#endif

		std::string query;
#ifdef DEV_MODE

		if (SERVER_IS_SEASONAL)
		{
			query = "SELECT TOP (1) Ranking FROM"
				" ("
				" SELECT ID, Level, Experience, LevelUpDate, RANK() over (order by Level DESC, Experience DESC, LevelUpDate ASC) AS Ranking"
				" FROM CharacterInfo"
				" WHERE JobCode = ?"
				" AND Seasonal = ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ) t"
				" WHERE t.ID = ?";
		}
		else
		{
			query = "SELECT TOP (1) Ranking FROM"
				" ("
				" SELECT ID, Level, Experience, LevelUpDate, RANK() over (order by Level DESC, Experience DESC, LevelUpDate ASC) AS Ranking"
				" FROM CharacterInfo"
				" WHERE JobCode = ?"
				" AND Seasonal < ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ) t"
				" WHERE t.ID = ?";
		}
#else

		if (SERVER_IS_SEASONAL)
		{
			query = "SELECT TOP (1) Ranking FROM"
				" ("
				" SELECT ID, Level, Experience, LevelUpDate, RANK() over (order by Level DESC, Experience DESC) AS Ranking"
				" FROM CharacterInfo"
				" WHERE JobCode = ?"
				" AND GMLevel < 2" //ignore Admins / GMs
				" AND Seasonal = ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ) t"
				" WHERE t.ID = ?";
		}
		else
		{
			query = "SELECT TOP (1) Ranking FROM"
				" ("
				" SELECT ID, Level, Experience, LevelUpDate, RANK() over (order by Level DESC, Experience DESC) AS Ranking"
				" FROM CharacterInfo"
				" WHERE JobCode = ?"
				" AND GMLevel < 2" //ignore Admins / GMs
				" AND Seasonal < ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ) t"
				" WHERE t.ID = ?";
		}

#endif

		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->pcUserData->sCharacterData.iClass);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iSeasonalNum);
			pcDB->BindParameterInput(3, PARAMTYPE_Integer, &pcUser->iCharacterID);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iRanking);
			}
		}

		pcDB->Close();
	}

	//Only top 3 per class for now.
	if (iRanking <= 3)
	{
		pcUser->sClassRanking = iRanking;
		DEBUG("SQLUpdatePlayerCharacterClassRanking - pcUser->sClassRanking = %d", pcUser->sClassRanking);
	}
	else
	{
		pcUser->sClassRanking = 0; //everyone else should have a '0' ranking so that logic will work elsewhere - see CheckSyncUserRanking
	}
}

/// <summary>
/// Get number one character id and store this locally.
/// This ID will be used to determine if the player should have aura id of 4
/// </summary>
void CharacterServer::UpdatePlayerCharacterOverallRanking()
{
	if (LOGIN_SERVER)
		return;

	DEBUGIF(DEBUG_AURA, "CharacterServer::UpdatePlayerCharacterOverallRanking");

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 2);

	int iTopCharacterId = 0;
	if (pcDB->Open())
	{
#ifdef SEASONAL_IS_ACTIVE
		//if seasonal is active, use current seasonal num for one of the above query.
		int iSeasonalNum = CURRENT_SEASONAL_NUM;
#else
		//otherwise '999' (return all chars)
		int iSeasonalNum = 999;
#endif

		std::string query;

#ifdef DEV_MODE

		if (SERVER_IS_SEASONAL)
		{
			query = "SELECT TOP (1) ID"
				" FROM CharacterInfo"
				" WHERE Seasonal = ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ORDER by Level DESC, Experience DESC, LevelUpDate ASC";
		}
		else
		{
			query = "SELECT TOP (1) ID"
				" FROM CharacterInfo"
				" WHERE Seasonal < ?"
				" AND Banned = 0"
				" AND Level >= 10"
				" ORDER by Level DESC, Experience DESC, LevelUpDate ASC";
		}
#else

		if (SERVER_IS_SEASONAL)
		{
			query = "SELECT TOP (1) ID"
				" FROM CharacterInfo"
				" WHERE GMLevel < 2" //ignore Admins / GMs
				" AND Banned = 0"
				" AND Seasonal = ?"
				" AND Level >= 10"
				" ORDER by Level DESC, Experience DESC";
		}
		else
		{
			query = "SELECT TOP (1) ID"
				" FROM CharacterInfo"
				" WHERE GMLevel < 2" //ignore Admins / GMs
				" AND Banned = 0"
				" AND Seasonal < ?"
				" AND Level >= 10"
				" ORDER by Level DESC, Experience DESC";
		}

#endif

		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iSeasonalNum);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iTopCharacterId);
			}
		}

		pcDB->Close();
	}

	if (iTopCharacterId)
	{
		iTopRankingCharacterId = iTopCharacterId;

		//std::cout << "UpdatePlayerCharacterOverallRanking iTopRankingCharacterId: " << iTopRankingCharacterId << std::endl;
	}
}

void CharacterServer::UpdateAndSyncAuraAndDisplaySkinForClass(enum ECharacterClass eCharacterClass)
{
	if (LOGIN_SERVER)
		return;

	//DEBUG ( "CharacterServer::UpdateAndSyncAuraAndDisplaySkinForClass (%d)", (int)eCharacterClass );

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 3);

	std::string query;

#ifdef DEV_MODE
	if (SERVER_IS_SEASONAL)
	{
		query = " SELECT TOP (3) ID"
			" FROM CharacterInfo"
			" WHERE JobCode = ?"
			" AND Banned = 0"
			" AND Seasonal = ?"
			" AND Level >= 10"
			" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
	}
	else
	{
		query = " SELECT TOP (3) ID"
			" FROM CharacterInfo"
			" WHERE JobCode = ?"
			" AND Banned = 0"
			" AND Seasonal < ?"
			" AND Level >= 10"
			" ORDER BY Level DESC, Experience DESC, LevelUpDate ASC";
	}
#else


	if (SERVER_IS_SEASONAL)
	{
		query = " SELECT TOP (3) ID"
			" FROM CharacterInfo"
			" WHERE JobCode = ?"
			" AND GMLevel < 2" //ignore Admins / GMs
			" AND Banned = 0"
			" AND Seasonal = ?"
			" AND Level >= 10"
			" ORDER BY Level DESC, Experience DESC";
	}
	else
	{
		query = " SELECT TOP (3) ID"
			" FROM CharacterInfo"
			" WHERE JobCode = ?"
			" AND GMLevel < 2"  //ignore Admins / GMs
			" AND Banned = 0"
			" AND Seasonal < ?"
			" AND Level >= 10"
			" ORDER BY Level DESC, Experience DESC";
	}
#endif


	int iRankingFirstPlaceCharacterId = -1;
	int iRankingSecondPlaceCharacterId = -1;
	int iRankingThirdPlaceCharacterId = -1;

	if (pcDB->Open())
	{
#ifdef SEASONAL_IS_ACTIVE
		//if seasonal is active, use current seasonal num for one of the above query.
		int iSeasonalNum = CURRENT_SEASONAL_NUM;
#else
		//otherwise '999' (return all chars)
		int iSeasonalNum = 999;
#endif

		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &eCharacterClass);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iSeasonalNum);

			if (pcDB->Execute())
			{
				int iThisCharId = 0;
				int iPlacement = 0;
				while (pcDB->Fetch())
				{
					iPlacement++;
					pcDB->GetData(1, PARAMTYPE_Integer, &iThisCharId);

					if (iPlacement == 1)
					{
						iRankingFirstPlaceCharacterId = iThisCharId;
					}
					else if (iPlacement == 2)
					{
						iRankingSecondPlaceCharacterId = iThisCharId;
					}
					else if (iPlacement == 3)
					{
						iRankingThirdPlaceCharacterId = iThisCharId;
					}
				}
			}
		}

		pcDB->Close();
	}

	// Update
	for (int i = 0; i < PLAYERS_MAX; i++)
	{
		UserData* ud = USERSDATA + i;
		User* u = USERDATATOUSER(ud);

		if (u && u->IsValidAndInGame())
		{
			if (ud->sCharacterData.iClass != eCharacterClass)
				continue;

			if (u->iCharacterID == iRankingFirstPlaceCharacterId)
			{
				CheckSyncUserRanking(u, 1);

				/*
				*   If we want to boardcast this, we need to keep track of top player of each class at server startup then check for any changes
					//Broadcast time!
					//New top solo score for this class
					CHATSERVER->SendChatAllEx ( CHATCOLOR_Global,
					"Ranking> New Top %s: %s",
					GetCharacterClassStringByClassEnum ( pcUser->pcUserData->sCharacterData.iClass ),
					CHARACTERSERVER->GetCharacterName ( pcUser->pcUserData ) );
				*/
			}
			else if (u->iCharacterID == iRankingSecondPlaceCharacterId)
			{
				CheckSyncUserRanking(u, 2);
			}
			else if (u->iCharacterID == iRankingThirdPlaceCharacterId)
			{
				CheckSyncUserRanking(u, 3);
			}
			else //user no longer first, second, or third
			{
				CheckSyncUserRanking(u, 0);
			}

		}
	}
}

//Update client
//Update login server
void CharacterServer::CheckSyncUserRanking(User* pcUser, int iNewClassRanking)
{
	//is different? sync!
	if (iNewClassRanking != pcUser->sClassRanking)
	{
		DEBUGIF(DEBUG_AURA, "CharacterServer: CheckSyncUserRanking: iNewClassRanking: %d vs %d for user: %s", iNewClassRanking, pcUser->sClassRanking, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));

		pcUser->sClassRanking = iNewClassRanking;

		SyncUserEffectToUser(pcUser);
	}

	//no update needed
	else
	{
		return;
	}
}

char* CharacterServer::GetCharacterName(UserData* pcUserData)
{
	if (pcUserData == NULL)
		return "";

	if (!GAME_SERVER)
		return pcUserData->szCharacterName;

	return pcUserData->sCharacterData.szName;
}

char* CharacterServer::GetCharacterName(User* pcUser)
{
	if (pcUser == NULL || pcUser->IsValid() == FALSE)
		return "";

	if (!GAME_SERVER)
		return pcUser->pcUserData->szCharacterName;

	return pcUser->pcUserData->sCharacterData.szName;
}

bool CharacterServer::GetCharacterExFilePath(char* pszCharacterName, char* pszBuffer, int iBufferLen)
{
	if (StringCbPrintfA(pszBuffer, iBufferLen, "Data\\Character\\%s.chd", pszCharacterName) == S_OK)
		return true;

	return false;
}

bool CharacterServer::GetCharacterFilePath(char* pszCharacterName, char* pszBuffer, int iBufferLen)
{
	if (StringCbPrintfA(pszBuffer, iBufferLen, "Data\\Character\\%s.chr", pszCharacterName) == S_OK)
		return true;

	return false;
}

DWORD CharacterServer::GetClanCode(int iClassClan)
{
	DWORD	dwCode, dwCode1, dwCode2;

	dwCode = iClassClan % 1000000;
	dwCode1 = iClassClan / 100000000;
	dwCode2 = dwCode1 * 100000000;

	return dwCode + dwCode2;
}

void CharacterServer::SendBlessCastleSkill(UserData* pcUserData)
{
	typedef void(__cdecl* tfnSendBlessClastleSkill)(UserData* pcUserData);
	tfnSendBlessClastleSkill fnSendBlessClastleSkill = (tfnSendBlessClastleSkill)0x00557F10;
	if (!GAME_SERVER)
		fnSendBlessClastleSkill(pcUserData);
}

int CharacterServer::ChangeCharacterName(char* pszCharacterName, char* pszNewCharacterName)
{
	if (STRLEN(pszCharacterName) >= 32 || STRLEN(pszNewCharacterName) >= 32)
		return -2;

	UserData* pcUserData = USERDATABYNAME(pszCharacterName);

	if (pcUserData != NULL)
	{
		if (pcUserData->pcSocketData)
		{
			if (pcUserData->pcSocketData->bConnected)
				return -1;

			LOGSERVER->LogSocketClose(pcUserData->pcSocketData, "CharacterServer::ChangeCharacterName()", _ReturnAddress());
			WNDPROC_SOCKETCLOSE(SOCKETSERVER->GetHWND(), pcUserData->pcSocketData);
		}
	}

	SQLCharacter s;

	if (SERVER_IS_SEASONAL)
	{
		if (CHARACTERSERVER->SQLGetCharacter(SQLCONNECTION(EDatabaseID::DATABASEID_UserDB_PrimaryServer), pszNewCharacterName, &s) == TRUE)
			return 1;
	}

	if (CHARACTERSERVER->SQLGetCharacter(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszNewCharacterName, &s) == TRUE)
		return 1;

	if (CHARACTERSERVER->SQLGetCharacter(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, &s) == FALSE)
		return 1;

	if (CHARACTERSERVER->SQLGetCharacterClanID(pszNewCharacterName) != -1)
		return 2;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath(pszCharacterName, szCharacterFilePath, _countof(szCharacterFilePath));

	char szNewCharacterFilePath[MAX_PATH];
	GetCharacterFilePath(pszNewCharacterName, szNewCharacterFilePath, _countof(szNewCharacterFilePath));

	if (CopyFileA(szCharacterFilePath, szNewCharacterFilePath, FALSE) == FALSE)
		return 3;

	DeleteFileA(szCharacterFilePath);

	FILE* pFile;
	if (fopen_s(&pFile, szNewCharacterFilePath, "rb+") == 0)
	{
		fseek(pFile, 0x10, SEEK_SET);
		fwrite(pszNewCharacterName, 1, lstrlenA(pszNewCharacterName) + 1, pFile);
		fflush(pFile);
		fclose(pFile);

		SQLRenameCharacter(pszCharacterName, pszNewCharacterName);
	}

	return 0;
}

int CharacterServer::ChangeCharacterLevel(char* pszCharacterName, int iLevel)
{
	if (STRLEN(pszCharacterName) >= 32)
		return -2;

	if (iLevel < 1 || iLevel > SERVER_LEVEL_MAX)
		return -1;

	SQLCharacter s;

	if (CHARACTERSERVER->SQLGetCharacter(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, &s) == FALSE)
		return 1;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath(pszCharacterName, szCharacterFilePath, _countof(szCharacterFilePath));

	//EXP
	LARGE_INTEGER liEXP;
	liEXP.QuadPart = GetExpFromLevel(iLevel);

	User* pc = USERDATATOUSER(USERDATABYNAME(pszCharacterName));

	if (pc == NULL)
	{

		FILE* pFile;
		if (fopen_s(&pFile, szCharacterFilePath, "rb+") == 0)
		{
			fseek(pFile, 0xC8, SEEK_SET);
			fwrite(&iLevel, 1, 4, pFile);
			fseek(pFile, 0x14C, SEEK_SET);
			fwrite(&liEXP.LowPart, 1, 4, pFile);
			fseek(pFile, 0x194, SEEK_SET);
			fwrite(&liEXP.HighPart, 1, 4, pFile);
			fflush(pFile);
			fclose(pFile);

			SQLChangeCharacterLevel(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, iLevel, liEXP.QuadPart);

			if (SERVER_IS_SEASONAL)
			{
				SQLChangeCharacterLevel(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), pszCharacterName, iLevel, liEXP.QuadPart);
			}
		}
	}
	else
	{
		GiveEXP(pc, liEXP.QuadPart - GetExp(pc->pcUserData));
	}

	return 0;
}

int CharacterServer::ChangeCharacterClass(char* pszCharacterName, int iClass)
{
	if (STRLEN(pszCharacterName) >= 32)
		return -2;

	if (iClass < 1 || iClass > 12)
		return -1;

	SQLCharacter s;

	if (CHARACTERSERVER->SQLGetCharacter(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, &s) == FALSE)
		return 1;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath(pszCharacterName, szCharacterFilePath, _countof(szCharacterFilePath));

	User* pc = USERDATATOUSER(USERDATABYNAME(pszCharacterName));

	if (pc == NULL)
	{

		FILE* pFile;
		if (fopen_s(&pFile, szCharacterFilePath, "rb+") == 0)
		{
			int iRank = 0;

			fseek(pFile, 0x184, SEEK_SET);
			fread(&iRank, 1, 4, pFile);


			char szNewHeadModel[64] = { 0 };
			char cTier[2] = { iRank ? 0x60 + (char)iRank : 0, 0 };
			char cChar[2] = { iRank == 2 ? '_' : '-', 0 };

			switch (iClass)
			{
			case CHARACTERCLASS_Fighter:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Mechanician:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Archer:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Pikeman:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier);
				if (iRank == 3)
					STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf");
				break;

			case CHARACTERCLASS_Atalanta:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Knight:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Magician:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Priestess:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Assassin:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier);
				break;

			case CHARACTERCLASS_Shaman:
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier);
				break;

			default:
				break;
			}

			int iOldClass = 0;

			fseek(pFile, 0xC4, SEEK_SET);
			fread(&iOldClass, 1, 4, pFile);

			fseek(pFile, 0xC4, SEEK_SET);
			fwrite(&iClass, 1, 4, pFile);
			fflush(pFile);

			fseek(pFile, 0x70, SEEK_SET);
			fwrite(szNewHeadModel, 1, 64, pFile);
			fflush(pFile);

			fclose(pFile);

			SQLChangeCharacterClass(pszCharacterName, iClass);

			//Quest Handler
			ECharacterRace iOldRace = GetCharacterRace((ECharacterClass)iOldClass);
			ECharacterRace iNewRace = GetCharacterRace((ECharacterClass)iClass);


			if ((iOldRace == CHARACTERRACE_Tempskron) && (iNewRace == CHARACTERRACE_Morion))
			{
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1601, 1606);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1602, 1607);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1603, 1608);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1604, 1609);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1605, 1610);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1640, 1642);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1641, 1643);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1800, 1802);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1801, 1803);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1648, 1650);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1649, 1651);

				if ((iClass == CHARACTERCLASS_Magician) || (iClass == CHARACTERCLASS_Priestess) || (iClass == CHARACTERCLASS_Shaman))
					QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1621, 1622);
			}
			else if ((iOldRace == CHARACTERRACE_Morion) && (iNewRace == CHARACTERRACE_Tempskron))
			{
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1606, 1601);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1607, 1602);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1608, 1603);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1609, 1604);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1610, 1605);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1642, 1640);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1643, 1641);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1802, 1800);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1803, 1801);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1650, 1648);
				QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1651, 1649);

				if ((iOldClass == CHARACTERCLASS_Magician) || (iOldClass == CHARACTERCLASS_Priestess) || (iOldClass == CHARACTERCLASS_Shaman))
					QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1622, 1621);
			}
			else if ((iOldRace == CHARACTERRACE_Morion) && (iNewRace == CHARACTERRACE_Morion))
			{
				if ((iOldClass == CHARACTERCLASS_Magician) || (iOldClass == CHARACTERCLASS_Priestess) || (iOldClass == CHARACTERCLASS_Shaman))
				{
					if ((iClass == CHARACTERCLASS_Atalanta) || (iClass == CHARACTERCLASS_Knight))
						QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1622, 1621);
				}
				else if ((iOldClass == CHARACTERCLASS_Atalanta) || (iOldClass == CHARACTERCLASS_Knight))
				{
					if ((iClass == CHARACTERCLASS_Magician) || (iClass == CHARACTERCLASS_Priestess) || (iClass == CHARACTERCLASS_Shaman))
						QUESTSERVER->SQLQuestSwapClass(pszCharacterName, 1621, 1622);
				}
			}
		}
	}

	return 0;
}

BOOL CharacterServer::SetCharacterEXPOnDisconnect(char* pszCharacterName, INT64 iEXP)
{
	BOOL bRet = FALSE;

	char szCharacterFilePath[MAX_PATH];
	GetCharacterFilePath(pszCharacterName, szCharacterFilePath, _countof(szCharacterFilePath));

	//EXP
	LARGE_INTEGER liEXP;
	liEXP.QuadPart = iEXP;

	User* pc = USERDATATOUSER(USERDATABYNAME(pszCharacterName));

	FILE* pFile;
	if (fopen_s(&pFile, szCharacterFilePath, "rb+") == 0)
	{
		int iLevel = 0;
		fseek(pFile, 0x0C8, SEEK_SET);
		fread(&iLevel, 1, 4, pFile);
		fseek(pFile, 0x14C, SEEK_SET);
		fwrite(&liEXP.LowPart, 1, 4, pFile);
		fseek(pFile, 0x194, SEEK_SET);
		fwrite(&liEXP.HighPart, 1, 4, pFile);
		fflush(pFile);
		fclose(pFile);

		SQLChangeCharacterLevel(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, iLevel, liEXP.QuadPart);

		if (SERVER_IS_SEASONAL)
		{
			SQLChangeCharacterLevel(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), pszCharacterName, iLevel, liEXP.QuadPart);
		}

		bRet = TRUE;
	}

	return bRet;
}

void CharacterServer::SQLUpdateCharacter(char* pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold)
{
	SQLUpdateCharacter(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pszCharacterName, iCharacterClass, iCharacterLevel, iCharacterExperience, iCharacterGold);

	if (SERVER_IS_SEASONAL)
	{
		SQLUpdateCharacter(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), pszCharacterName, iCharacterClass, iCharacterLevel, iCharacterExperience, iCharacterGold);
	}
}


void CharacterServer::SQLUpdateCharacter(SQLConnection* pcDB, char* pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold)
{
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Level=?, JobCode=?, Experience=?, Gold=? WHERE Name=?"))
		{
			DEBUG("CharacterServer::SQLUpdateCharacter = %d", iCharacterExperience);

			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iCharacterLevel);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iCharacterClass);
			pcDB->BindParameterInput(3, PARAMTYPE_Int64, &iCharacterExperience);
			pcDB->BindParameterInput(4, PARAMTYPE_Integer, &iCharacterGold);
			pcDB->BindParameterInput(5, PARAMTYPE_String, pszCharacterName);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}



void CharacterServer::SQLUpdateCharacterExpAndLevel(User* pcUser)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	int iCharacterLevel = pcUser->pcUserData->GetLevel();
	ECharacterClass iClassCode = pcUser->pcUserData->sCharacterData.iClass;
	LONGLONG liCharacterExp = pcUser->pcUserData->liEXP.QuadPart;
	int iCharacterID = pcUser->iCharacterID;
	BOOL bIsNotGMCHar = pcUser->GetGameLevel() == EGameLevel::GAMELEVEL_None;


	auto pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		int iSqlTopLevelForClass = -1;

#ifdef DEV_MODE
		std::string query = "SELECT TOP(1) Level FROM CharacterInfo WHERE JobCode=? AND Banned=0 ORDER BY Level DESC, LevelUpDate ASC";
#else
		std::string query = "SELECT TOP(1) Level FROM CharacterInfo WHERE JobCode=? AND GMLevel<2 AND Banned=0 ORDER BY Level DESC";
#endif

		if (bIsNotGMCHar && pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, EDatabaseDataType::PARAMTYPE_Integer, &iClassCode);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, EDatabaseDataType::PARAMTYPE_Integer, &iSqlTopLevelForClass);
			}
		}

		BOOL bAnnounceTopPlayer = FALSE;

		if (bIsNotGMCHar &&
			iSqlTopLevelForClass != -1 &&
			iCharacterLevel >= 10 &&
			iCharacterLevel > iSqlTopLevelForClass)
		{
			if (iCharacterLevel <= 40)
			{
				bAnnounceTopPlayer = (iCharacterLevel % 10) == 0; //every 10 levels
			}
			else if (iCharacterLevel <= 80)
			{
				bAnnounceTopPlayer = (iCharacterLevel % 5) == 0;	//every 5 levels
			}
			else
			{
				bAnnounceTopPlayer = TRUE; //Announce every 1 level
			}
		}

		if (bIsNotGMCHar && bAnnounceTopPlayer)
		{
			CHATSERVER->SendChatAllEx(EChatColor::CHATCOLOR_Global, "%s is the first %s to reach level %d!", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData), GetCharacterClassStringByClassEnum(iClassCode), iCharacterLevel);
		}

		pcDB->Clear();

		if (pcDB->Prepare("UPDATE CharacterInfo SET Level=?, Experience=? WHERE ID=?"))
		{
			//DEBUG ( "CharacterServer::SQLUpdateCharacterExpAndLevel: Character %d, level = %d, exp = %d", iCharacterID, iCharacterLevel, liCharacterExp );

			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iCharacterLevel);
			pcDB->BindParameterInput(2, PARAMTYPE_Int64, &liCharacterExp);
			pcDB->BindParameterInput(3, PARAMTYPE_Integer, &iCharacterID);
			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}


int CharacterServer::SQLGetCharacterClass(char* pszCharacterName)
{
	int iClass = 0;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT JobCode FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszCharacterName, STRLEN(pszCharacterName));

			if (pcDB->Execute() && pcDB->Fetch())
				pcDB->GetData(1, PARAMTYPE_Integer, &iClass);
		}
		pcDB->Close();
	}

	return iClass;
}

/// <summary>
/// Ticks at 1s interval
/// </summary>
BOOL CharacterServer::OnPremiumTimersAndForceOrbTick1s(UserData* pcUserData)
{
	//Only used by game servers
	if (LOGIN_SERVER)
		return FALSE;

	User* pcUser = USERDATATOUSER(pcUserData);

	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return FALSE;
	PETSYSTEMHANDLER->CheckPetRespawn(pcUserData);
	for (int i = 0; i < PREMIUM_ITEM_TOTAL; i++)
	{
		ItemPremium* psItem = &pcUser->sPremiumData.saItemList[i];

		if (psItem->dwCode == 0)
			continue;

		if (psItem->eType == EItemTimerType::ITEMTIMERTYPE_None)
			continue;

		//is this item's timer frozen in this map? If so skip counting down
		if (ITEMSERVER->IsItemTimerFreezedMap(pcUserData->iMapID, psItem->eType))
			continue;

		//Timer has expired (s remaining)
		if (psItem->dwTimeLeft <= 2)
		{
			ITEMSERVER->OnPremiumItemCancelledOrExpired(pcUser, psItem, TRUE);
		}
		else
		{
			psItem->dwTimeLeft -= 1; //it's not prefect, but it's good enough. clients will get sync'd with server timers every 10s anyway
		}
	}

	return TRUE;
}


void CharacterServer::CheckGold(UserData* pcUserData)
{
	User* pcUser = USERDATATOUSER(pcUserData);
	if (!GAME_SERVER && pcUserData)
	{
		if (!pcUserData->iWarehouseStatus && !pcUser->TradeWindowOpen && pcUser->pData.dwGold != pcUserData->GetGold())
		{
			PacketSetCharacterGold sPacket;
			sPacket.iHeader = PKTHDR_SetGold;
			sPacket.iLength = sizeof(PacketSetCharacterGold);
			sPacket.dwGold = pcUserData->GetGold();

			PACKETSERVER->Send(pcUserData, &sPacket);

			pcUserData->iSaveGold = pcUserData->GetGold();
			pcUser->pData.dwGold = pcUserData->GetGold();
		}

		if (pcUserData->GetGold() == -150000000)
		{
			LOGSERVER->LogSocketClose(pcUserData->pcSocketData, "CharacterServer::CheckGold()", _ReturnAddress());
			WNDPROC_SOCKETCLOSE(SOCKETSERVER->GetHWND(), pcUserData->pcSocketData);
		}
	}
}

INT64 CharacterServer::GetExp(UserData* pcUserData)
{
	if (pcUserData)
		return pcUserData->liEXP.QuadPart;

	return 0;
}

void CharacterServer::SetExpFromSQL(UserData* pcUserData)
{

}

INT64 CharacterServer::GetExpFromLevel(int iLevel)
{
	if (iLevel <= 0)
		return 0;

	if (iLevel > SERVER_LEVEL_MAX)
		iLevel = SERVER_LEVEL_MAX;

	// Use our updated experience table
	if (iLevel <= (int)(sizeof(s_TotalExpPerLevel) / sizeof(s_TotalExpPerLevel[0])))
	{
		return static_cast<INT64>(s_TotalExpPerLevel[iLevel - 1]);
	}

	// Fallback to memory-based table for safety
	UINT uExpLow = *(UINT*)(((iLevel - 1) * 8) + 0x08B8022C);
	UINT uExpHigh = *(UINT*)(((iLevel - 1) * 8) + 0x08B80230);
	return ((INT64)uExpHigh << 32) | ((INT64)uExpLow & 0xFFFFFFFF);
}

int CharacterServer::GetLevelFromExp(INT64 iExp)
{
	if (iExp <= 0)
		return 1;

	static const int s_LevelMax = sizeof(s_TotalExpPerLevel) / sizeof(s_TotalExpPerLevel[0]);
	
	int iLevel = 1;
	for (int i = 0; i < s_LevelMax; ++i)
	{
		if (static_cast<uint64_t>(iExp) >= s_TotalExpPerLevel[i])
			iLevel = i + 1;
		else
			break;
	}

	if (iLevel > s_LevelMax)
		iLevel = s_LevelMax;

	return iLevel;
}

void CharacterServer::OnCharacterUpdate(UserData* pcUserData)
{
	char* pszCharacterName = GetCharacterName(pcUserData);

	if (!GAME_SERVER)
	{

	}
}

void CharacterServer::SQLLoadUserEffectTitleAndRanking(User* pcUser)
{
	if (LOGIN_SERVER)
		return;

	if (pcUser->IsValidAndInGame() == FALSE)
		return;

	DEBUGIF(DEBUG_AURA, "CharacterServer::SQLLoadUserEffectTitleAndRanking");


	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 4);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT Title, DialogSkin FROM CharacterInfo WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				int iTitleId;
				int iDialogSkin;

				pcDB->GetData(1, PARAMTYPE_Integer, &iTitleId);
				pcDB->GetData(2, PARAMTYPE_Integer, &iDialogSkin);

				pcUser->sDialogSkinId = static_cast<short>(iDialogSkin);

				if (iTitleId)
				{
					pcDB->Clear();

					std::string query = "SELECT TOP(1) TD.Title, TD.TitleRarity"
						" FROM CharacterTitleList AS CTL"
						" INNER JOIN TitleDef AS TD"
						" ON TD.TitleID = CTL.TitleID"
						" WHERE CTL.CharacterID = ?"
						" AND TD.TitleID = ?";

					if (pcDB->Prepare(query.c_str()))
					{
						pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);
						pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iTitleId);

						if (pcDB->Execute() && pcDB->Fetch())
						{
							pcDB->GetData(1, PARAMTYPE_String, pcUser->szTitle, 25);
							pcDB->GetData(2, PARAMTYPE_Short, &pcUser->sTitleRarity);
						}
					}
				}
			}
		}

		pcDB->Close();
	}

	CHARACTERSERVER->SQLUpdatePlayerCharacterClassRanking(pcUser);

	//send title, ranking etc to user
	CHARACTERSERVER->SyncUserEffectToUser(pcUser);
}

void CharacterServer::SyncUserEffectToUser(User* pcUser)
{
	if (LOGIN_SERVER)
		return;

	if (pcUser->IsValidAndInGame() == FALSE)
		return;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::SyncUserEffectToUser");

	//aura / dialog skin to be based on class rankng for now


	PacketUserEffect* packet = new PacketUserEffect();
	packet->iLength = sizeof(PacketUserEffect);
	packet->iHeader = PKTHDR_Effect_User;
	packet->iUniqueUserID = pcUser->GetID();
	packet->sTitleRarity = pcUser->sTitleRarity;
	packet->sDialogSkinId = 0;

	STRINGCOPY(packet->szTitle, pcUser->szTitle);

	if (pcUser->GetMapID() == EMapID::MAPID_BlessCastle)
	{
		packet->sPlayerAuraId = 0;
	}
	else
	{
		if (pcUser->iCharacterID == iTopRankingCharacterId)
		{
			packet->sPlayerAuraId = 4;
		}
		else
		{
			packet->sPlayerAuraId = pcUser->sClassRanking;
		}
	}


	SENDPACKET(pcUser, packet);
}

void CharacterServer::ClearDialogSkin(User* pcUser)
{
	pcUser->sDialogSkinId = 0;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::ClearDialogSkin");

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 5);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET DialogSkin=NULL WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}

	SyncUserEffectToUser(pcUser);
}


void CharacterServer::SetDialogSkin(User* pcUser, int iDialogSkin)
{
	pcUser->sDialogSkinId = iDialogSkin;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::SetDialogSkin");

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 6);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Aura=? WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iDialogSkin);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}

	SyncUserEffectToUser(pcUser);
}

void CharacterServer::ClearTitle(User* pcUser)
{
	if (LOGIN_SERVER)
		return;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::ClearTitle");

	pcUser->szTitle[0] = { 0 };
	pcUser->sTitleRarity = 0;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 7);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Title=NULL WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}

	SyncUserEffectToUser(pcUser);
}

void CharacterServer::SetTitle(User* pcUser, int iTitleID)
{
	if (LOGIN_SERVER)
		return;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::SetTitle");

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 8);
	if (pcDB->Open())
	{
		std::string query = "SELECT TOP(1) TD.Title, TD.TitleRarity"
			" FROM CharacterTitleList AS CTL"
			" INNER JOIN TitleDef AS TD"
			" ON TD.TitleID = CTL.TitleID"
			" WHERE CTL.CharacterID = ?"
			" AND TD.TitleID = ?";

		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iTitleID);

			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_String, pcUser->szTitle, 25);
				pcDB->GetData(2, PARAMTYPE_Short, &pcUser->sTitleRarity);

				CHATSERVER->SendChatEx(pcUser, EChatColor::CHATCOLOR_Normal, "Current Title: %s", pcUser->szTitle);

				pcDB->Clear();

				//update Title in CharacterInfo Table
				if (pcDB->Prepare("UPDATE CharacterInfo SET Title=? WHERE ID=?"))
				{
					pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iTitleID);
					pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUser->iCharacterID);

					pcDB->ExecuteUpdate();
				}

				SyncUserEffectToUser(pcUser);
			}
			else
			{
				CHATSERVER->SendChat(pcUser, EChatColor::CHATCOLOR_Error, "Title set failed!");
			}

			pcDB->Close();
		}
	}
}

void CharacterServer::SendFullTitleListIfAny(User* pcUser)
{
	if (LOGIN_SERVER)
		return;

	DEBUGIF(DEBUG_TITLE, "CharacterServer::SendFullTitleListIfAny");

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_TitleAura, 9);
	if (pcDB->Open())
	{
		std::string query = "SELECT DISTINCT(TD.Title), TD.TitleRarity, TD.TitleID"
			" FROM CharacterTitleList AS CTL"
			" INNER JOIN TitleDef AS TD"
			" ON TD.TitleID = CTL.TitleID"
			" WHERE CTL.CharacterID = ?"
			" ORDER BY TD.Title";

		PacketPlayerTitleList* packet = new PacketPlayerTitleList();
		packet->iHeader = PKTHDR_TitleList;

		if (pcDB->Prepare(query.c_str()))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);

			if (pcDB->Execute())
			{
				while (pcDB->Fetch())
				{
					PlayerTitle title{};
					ZeroMemory(&title, sizeof(PlayerTitle));

					int iTitleID = -1;

					pcDB->GetData(1, PARAMTYPE_String, title.szTitle, 20); //todo for later, increase to 25
					pcDB->GetData(2, PARAMTYPE_Short, &title.sRarity);
					pcDB->GetData(3, PARAMTYPE_Integer, &iTitleID);

					title.sTitleID = static_cast<short>(iTitleID);

					CopyMemory(packet->baData + packet->iCount * sizeof(PlayerTitle), &title, sizeof(PlayerTitle));

					packet->iCount++;
				}
			}
		}

		pcDB->Close();

		if (packet->iCount > 0)
		{
			packet->iLength = (packet->iCount * sizeof(PlayerTitle)) + 0x08; //0x08 = size of packet header

			SENDPACKET(pcUser, packet);
		}
		else
		{
			CHATSERVER->SendChat(pcUser, EChatColor::CHATCOLOR_Error, "You have not obtained any titles!");
		}
	}
}

BOOL CharacterServer::CharacterInTown(EMapID eMapID)
{
	for (int i = 0; i < _countof(aiTowns); i++)
	{
		if (eMapID == aiTowns[i])
			return TRUE;
	}

	return FALSE;
}

BOOL CharacterServer::CharacterInTownEx(EMapID eMapID)
{
	for (int i = 0; i < _countof(aiTownsEx); i++)
	{
		if (eMapID == aiTownsEx[i])
			return TRUE;
	}

	return FALSE;
}

BOOL CharacterServer::CharacterInTown(UserData* pcUserData)
{
	if (pcUserData)
	{
		if (pcUserData->iMapID < 0 || pcUserData->iMapID >= NUM_MAPS)
			return TRUE;

		for (int i = 0; i < _countof(aiTowns); i++)
		{
			if (pcUserData->iMapID == aiTowns[i])
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CharacterServer::CharacterInTownEx(UserData* pcUserData)
{
	if (pcUserData)
	{
		if (pcUserData->iMapID < 0 || pcUserData->iMapID >= NUM_MAPS)
			return TRUE;

		for (int i = 0; i < _countof(aiTownsEx); i++)
		{
			if (pcUserData->iMapID == aiTownsEx[i])
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CharacterServer::CharacterInCityStage(UserData* pcUserData)
{
	if (pcUserData)
	{
		if (pcUserData->iMapID < 0 || pcUserData->iMapID > 80)
			return TRUE;

		for (int i = 0; i < _countof(aStageCity); i++)
		{
			if (pcUserData->iMapID == aStageCity[i])
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CharacterServer::CharacterInBelltraSOD(UserData* pcUserData)
{
	if (pcUserData)
	{
		return pcUserData->iMapID == MAPID_Bellatra;
	}

	return FALSE;
}


BOOL CharacterServer::CharacterInBattleStage(UserData* pcUserData)
{
	if (pcUserData)
	{
		for (int i = 0; i < _countof(aStageBattle); i++)
		{
			if (pcUserData->iMapID == aStageBattle[i])
				return TRUE;
		}
	}
	return FALSE;
}

void CharacterServer::OnAddExp(UserData* pcUserData)
{

}

void CharacterServer::OnLoseExp(UserData* pcUserData, PacketLoseExperience* psPacket)
{
	int iLevelCap = SERVER_LEVEL_MAX;
	//if ( pcUserData->iGameLevel >= GAMELEVEL_Two )
		//iLevelCap = SERVER_LEVEL_MAX; // Level CAP

	// User alive and not in city maps and not in Bless Castle?
	if (pcUserData && pcUserData->pcSocketData && psPacket->iLevel < iLevelCap)
	{
		// Is Hack or bug?
		if (psPacket->iSubExp < 0 || psPacket->iSubExp > pcUserData->liEXP.QuadPart)
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_OnDeathCheatExp;
			sPacket.LParam = ((int32_t*)&psPacket->iSubExp)[0];
			sPacket.SParam = ((int32_t*)&psPacket->iSubExp)[1];
			LOGSERVER->OnLogCheat(pcUserData, &sPacket);

			if (pcUserData->iGameLevel != GAMELEVEL_Four)
			{
				NETSERVER->DisconnectUser(pcUserData);

				SENDPACKETBLANK(USERDATATOUSER(pcUserData), PKTHDR_Disconnect);
				WNDPROC_SOCKETCLOSE(SOCKETSERVER->GetHWND(), pcUserData->pcSocketData);
			}

			return;
		}

		INT64 iExpLevel = GetExpFromLevel(psPacket->iLevel);

		INT64 iExpCharacter = GetExp(pcUserData) - psPacket->iSubExp;

		if (iExpCharacter < iExpLevel)
			iExpCharacter = iExpLevel;

		SetExp(pcUserData, iExpCharacter);

		User* pcUser = USERDATATOUSER(pcUserData);

		//Log It
		LOGSERVER->OnLogEx(pcUser, ACCLOGID_CharacterExpLose, "Character[%s] Lose EXP[%I64d] MapID[%d] RessurectionItem[%s]",
			CHARACTERSERVER->GetCharacterName(pcUser->pcUserData), psPacket->iSubExp, psPacket->iMapID, psPacket->bRessurectionItem ? "TRUE" : "FALSE");
	}
}

void CharacterServer::SetExp(UserData* pcUserData, INT64 iExp)
{
	int iLevelCap = SERVER_LEVEL_MAX;
	//if ( pcUserData->iGameLevel >= GAMELEVEL_Two )
		//iLevelCap = 100; // Level CAP

	//DEBUG( "SetExp Level cap = %d, exp = %I64d", iLevelCap, iExp );

	if (pcUserData && !GAME_SERVER)
	{
		pcUserData->liEXP.QuadPart = iExp;

		INT64 iExpMax = GetExpFromLevel(iLevelCap);

		//DEBUG( "Curr: %I64d, Max: %I64d", pcUserData->liEXP.QuadPart, iExpMax );

		//set to max exp if over the limit (d'oh me)
		if (pcUserData->liEXP.QuadPart > iExpMax)
			pcUserData->liEXP.QuadPart = iExpMax;
	}
}

void CharacterServer::SQLSetEXP(User* pcUser, INT64 iExp)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	SQLSetEXP(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pcUser, iExp);

	if (SERVER_IS_SEASONAL)
	{
		SQLSetEXP(SQLCONNECTION(DATABASEID_UserDB_PrimaryServer), pcUser, iExp);
	}
}

void CharacterServer::SQLSetEXP(SQLConnection* pcDB, User* pcUser, INT64 iExp)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Experience=?, LastSeenDate=GETDATE() WHERE ID=?"))
		{
			DEBUG("SQLSetExp = %d, ID = %d", iExp, pcUser->iCharacterID);

			if (pcUser->iCharacterID == 0)
				WARN("CHAR ID is 0 for char name %s", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));

			pcDB->BindParameterInput(1, PARAMTYPE_Int64, &iExp);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}


void CharacterServer::SQLGiveEXP(User* pcUser, INT64 iExp)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	SQLGiveEXP(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pcUser, iExp);

	if (SERVER_IS_SEASONAL)
	{
		SQLGiveEXP(SQLCONNECTION(DATABASEID_UserDB_PrimaryServer), pcUser, iExp);
	}
}

void CharacterServer::SQLGiveEXP(SQLConnection* pcDB, User* pcUser, INT64 iExp)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET Experience=(Experience+?), LastSeenDate=GETDATE() WHERE ID=?"))
		{
			DEBUG("SQLGiveEXP = %lld, ID = %d", iExp, pcUser->iCharacterID);

			pcDB->BindParameterInput(1, PARAMTYPE_Int64, &iExp);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}

void CharacterServer::GiveEXP(User* pcUser, INT64 iExp)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	INT64 iExpCharacter = GetExp(pcUser->pcUserData) + iExp;
	INT64 iExpCharacterOld = GetExp(pcUser->pcUserData);

	if (GAME_SERVER)
		NETSERVER->GiveEXP(pcUser->pcUserData, iExp);
	else
	{
		SetExp(pcUser->pcUserData, iExpCharacter);
		PacketExperience sPacket;
		ZeroMemory(&sPacket, sizeof(PacketExperience));
		sPacket.iLength = sizeof(PacketExperience);
		sPacket.iHeader = PKTHDR_AddExp;
		sPacket.iAddExp = GetExp(pcUser->pcUserData) - iExpCharacterOld;
		SENDPACKET(pcUser, &sPacket);
	}
}

BOOL CharacterServer::GiveGOLD(User* pcUser, int iGold, EWhereAction eWhere)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return FALSE;

	if (GAME_SERVER)
	{
		DEBUG("GAME_SERVER CharacterServer::GiveGOLD IGN = %d, Gold = %d, Where = %d", pcUser->pcUserData->iID, iGold, eWhere);


		PacketNetPlayerGoldDiff l_DiffPacket;
		l_DiffPacket.iHeader = PKTHDR_NetPlayerGoldDiff;
		l_DiffPacket.iLength = sizeof(PacketNetPlayerGoldDiff);
		l_DiffPacket.ObjectID = pcUser->pcUserData->iID;
		l_DiffPacket.Diff = iGold;

		NetConnection* l_LoginServer = NETSERVER->GetLoginServerConnection();
		if (l_LoginServer)
			NETSERVER->SendPacket(l_LoginServer->pcSocketData, &l_DiffPacket);
	}
	else if (LOGIN_SERVER)
	{
		DEBUG("LOGIN_SERVER CharacterServer::GiveGOLD IGN = %s, Gold = %d, Where = %d", pcUser->pcUserData->szCharacterName, iGold, eWhere);


		if (iGold > 0)
			USERSERVER->AddServerUserGold(pcUser->pcUserData, iGold, eWhere);

		if (iGold < 0)
			USERSERVER->SubServerUserGold(pcUser->pcUserData, abs(iGold), eWhere);

		PacketSetCharacterGold sPacket;
		sPacket.iLength = sizeof(PacketSetCharacterGold);
		sPacket.iHeader = PKTHDR_SetGold;
		sPacket.dwGold = pcUser->pcUserData->GetGold();

		SENDPACKET(pcUser, &sPacket);
	}

	return TRUE;
}

/// <summary>
/// Insert new Character into CharacterInfo and return the inserted ID
/// </summary>
/// <param name="pcDB">Pointer to UserDB</param>
/// <param name="pszAccountName">Account name</param>
/// <param name="pczCharName">Character name</param>
/// <param name="iSeasonalNum">Seasonal number</param>
/// <param name="iGMLevel">Game master level</param>
/// <param name="iOverrideCharacterId">If specified it will use this ID instead of the auto-generated ID</param>
/// <returns>The inserted ID</returns>
int CharacterServer::SQLCharacterInsert(SQLConnection* pcDB, char* pszAccountName, char* pczCharName, int iSeasonalNum, int iGMLevel, int iOverrideCharacterId)
{
	int iInsertedID = 0;

	if (pcDB->Open())
	{
		std::string query = "";

		//to allow override of id
		if (iOverrideCharacterId != -1)
			query = "SET IDENTITY_INSERT CharacterInfo ON; ";

		query += "INSERT INTO CharacterInfo (";

		//ID column
		if (iOverrideCharacterId != -1)
			query += "[ID],";

		query += "[AccountName],[Name],[OldHead],[Level],[Experience],[Gold],[JobCode],[ClanID],[ClanPermission],[ClanLeaveDate],[LastSeenDate],[BlessCastleScore],[FSP],[LastStage],[IsOnline],[Seasonal],[GMLevel],[Banned],[Title],[DialogSkin]) ";

		if (iOverrideCharacterId == -1)
			query += "OUTPUT INSERTED.ID ";

		query += "VALUES (";

		//ID value
		if (iOverrideCharacterId != -1)
			query += "?,";

		query += "?, ?, ?, 1, 0, 0, 0, 0, 0, 0, GETDATE(), 0, 0, 0, 1, ?, ?, 0, NULL, NULL);";

		//set it back to auto-generation
		if (iOverrideCharacterId != -1)
			query += " SET IDENTITY_INSERT CharacterInfo OFF;";

		if (pcDB->Prepare(query.c_str()))
		{
			char szNull[2] = { 0,0 };
			int c = 0;

			if (iOverrideCharacterId != -1)
				pcDB->BindParameterInput(++c, PARAMTYPE_Integer, &iOverrideCharacterId);

			pcDB->BindParameterInput(++c, PARAMTYPE_String, pszAccountName, 32);
			pcDB->BindParameterInput(++c, PARAMTYPE_String, pczCharName, 32);
			pcDB->BindParameterInput(++c, PARAMTYPE_String, szNull);
			pcDB->BindParameterInput(++c, PARAMTYPE_Byte, &iSeasonalNum);
			pcDB->BindParameterInput(++c, PARAMTYPE_Byte, &iGMLevel);

			if (pcDB->Execute())
			{
				if (iOverrideCharacterId == -1)
				{
					if (pcDB->Fetch())
					{
						pcDB->GetData(1, EDatabaseDataType::PARAMTYPE_Integer, &iInsertedID);

						STATUS("New character '%s' inserted with ID of %d", pczCharName, iInsertedID);
					}
				}
				else
				{
					iInsertedID = iOverrideCharacterId;

					INFO("New character '%s' inserted with override ID of %d", pczCharName, iOverrideCharacterId);
				}
			}
		}
		pcDB->Close();
	}

	return iInsertedID;
}

void CharacterServer::CharacterCreate(User* pcUser, PacketCreateCharacter* psPacket)
{
	// Is Userdata?
	if (pcUser && pcUser->pcUserData)
	{
		psPacket->iCode = FALSE;

		//check allowable chars
		for (int i = 0; i < lstrlenA(psPacket->szCharname); i++)
		{
			if (psPacket->szCharname[i] >= 'a' && psPacket->szCharname[i] <= 'z')
				continue;
			if (psPacket->szCharname[i] >= 'A' && psPacket->szCharname[i] <= 'Z')
				continue;
			if (psPacket->szCharname[i] >= '0' && psPacket->szCharname[i] <= '9')
				continue;

			if (SERVER_IS_SEASONAL)
			{
				if (psPacket->szCharname[i] == '-' || psPacket->szCharname[i] == '_')
					continue;
			}

			SENDPACKET(pcUser, psPacket);
			return;
		}

		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_PrimaryServer);
		if (pcDB->Open())
		{
			int iCount = 0;
			if (pcDB->Prepare("SELECT COUNT(*) FROM CharacterInfo WHERE Name=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, psPacket->szCharname, 32);

				if (pcDB->Execute() && pcDB->Fetch())
				{
					pcDB->GetData(1, PARAMTYPE_Integer, &iCount);
				}
			}

			pcDB->Close();

			//Char already exist?
			if (iCount >= 1)
			{
				SENDPACKET(pcUser, psPacket);
				return;
			}
		}

		if (pcDB->Open())
		{
			int iCount = 0;
			if (pcDB->Prepare("SELECT COUNT(*) FROM CharacterInfo WHERE AccountName=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, psPacket->szUserID);

				if (pcDB->Execute() && pcDB->Fetch())
				{
					pcDB->GetData(1, PARAMTYPE_Integer, &iCount);
				}
			}

			pcDB->Close();

			//Account already have 6 or more chars?
			if (iCount >= 6)
			{
				SENDPACKET(pcUser, psPacket);
				return;
			}
		}

		char szDataPath[256] = { 0 };
		STRINGFORMAT(szDataPath, "Data\\Character\\%s.chr", psPacket->szCharname);

		// Character exists in local folder?
		if (FILEEXIST(szDataPath))
		{
			SENDPACKET(pcUser, psPacket);
			return;
		}

		//set to 'success'
		psPacket->iCode = TRUE;
		pcUser->pcUserData->dwHashCharacterName = GetCodebyName(psPacket->szCharname);
		STRINGCOPY(pcUser->pcUserData->szCharacterName, psPacket->szCharname);

		char szDateTime[20] = { 0 };
		GetDateTime(szDateTime);

		// Insert into database(s)
		{
			int iInsertedID = -1;

			//also insert into primary database
			if (SERVER_IS_SEASONAL)
			{
				//get primary char id from main database
				//and use the same id for seasonal database
				//this is to make merging easier
				int iPrimaryID = SQLCharacterInsert(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), psPacket->szUserID, psPacket->szCharname, CURRENT_SEASONAL_NUM, pcUser->GetGameLevel());
				SQLCharacterInsert(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), psPacket->szUserID, psPacket->szCharname, CURRENT_SEASONAL_NUM, pcUser->GetGameLevel(), iPrimaryID);

				iInsertedID = iPrimaryID;
			}
			else
			{
				iInsertedID = SQLCharacterInsert(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), psPacket->szUserID, psPacket->szCharname, 0, pcUser->GetGameLevel()); //babel database
			}

			if (iInsertedID > 0)
				pcUser->iCharacterID = iInsertedID; //save newly created ID
		}

		// Log
		{
			char szLog[256] = { 0 };
			char* pszIP = NULL;

			if (pcUser->pcUserData->pcSocketData)
				pszIP = pcUser->pcUserData->pcSocketData->szIP;

			STRINGFORMAT(szLog, CHARACTERCREATED_LOGACCTEXT, pcUser->pcUserData->szAccountName, psPacket->szCharname);

			SQLConnection* pcDB = SQLCONNECTION(DATABASEID_LogDB, 9);

			if (pcDB->Open())
			{
				if (pcDB->Prepare("INSERT INTO AccountLog VALUES (?,?,?,?,?,?)"))
				{
					int iLogID = ACCLOGID_CharacterCreated;
					pcDB->BindParameterInput(1, PARAMTYPE_String, pszIP);
					pcDB->BindParameterInput(2, PARAMTYPE_String, pcUser->pcUserData->szAccountName);
					pcDB->BindParameterInput(3, PARAMTYPE_Integer, &iLogID);
					pcDB->BindParameterInput(4, PARAMTYPE_String, szLog);
					pcDB->BindParameterInput(5, PARAMTYPE_String, szDateTime);
					pcDB->BindParameterInput(6, PARAMTYPE_Integer, &GAME_SERVER);

					pcDB->Execute();
				}
				pcDB->Close();
			}

		}



		//JLM
		// Give new players 24 hours EXP potion
		// to assist them with levelling up for the
		// first 24 hours. Have it automatically activated
		// so players can't abuse this.
		// Don't send the packet as you'd get 'An item is already in use' at start up.

		ITEMSERVER->CreatePremium(pcUser,
			ITEMID_ExpUp1H,
			ITEMTIMERTYPE_ExpUp,
			ITEMTIMERTIME_6H, false);

		ITEMSERVER->CreatePremium(pcUser,
			ITEMID_ThirdEye1H,
			ITEMTIMERTYPE_ThirdEye,
			ITEMTIMERTIME_6H, false);

		SENDPACKET(pcUser, psPacket);
	}
}

void CharacterServer::SendCharacterDataEx(User* pcUser)
{
	if (pcUser == NULL || pcUser->IsValidAndInGame() == FALSE)
		return;

	PacketCharacterDataEx s;
	ZeroMemory(&s, sizeof(PacketCharacterDataEx));
	s.iLength = sizeof(PacketCharacterDataEx);
	s.iHeader = PKTHDR_CharacterDataEx;
	s.sData.bSeasonal = SERVER_IS_SEASONAL;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT OldHead, FSP FROM CharacterInfo WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->iCharacterID);

			if (pcDB->Execute())
			{
				if (pcDB->Fetch())
				{
					pcDB->GetData(1, PARAMTYPE_String, s.sData.szOldHead, _countof(s.sData.szOldHead));
					pcDB->GetData(2, PARAMTYPE_Integer, &s.sData.iFSP);

					if (IsNull(s.sData.szOldHead))
						ZeroMemory(s.sData.szOldHead, _countof(s.sData.szOldHead));
				}
			}
		}
		pcDB->Close();
	}

	SENDPACKET(pcUser, &s);
}

//only for game servers
void CharacterServer::OnCharacterSyncData(UserData* pcUserData)
{
	if (LOGIN_SERVER)
		return;

	if (pcUserData == nullptr)
	{
		WARN("OnCharacterSyncData pcUserData is NULL!");
		return;
	}

	User* pcUser = USERDATATOUSER(pcUserData);

	if (pcUser == nullptr)
	{
		WARN("OnCharacterSyncData pcUser is NULL!");
		return;
	}

	//INFO ( "On Character Loaded: %s", CHARACTERSERVER->GetCharacterName ( pcUserData ) );
	LOGSERVER->OnLogEx(pcUser, ACCLOGID_CharacterLoad, "[%s][%s] OnCharacterSyncData", (GAME_SERVER ? "GS" : "LS"), CHARACTERSERVER->GetCharacterName(pcUserData));

	//USERSERVER->DisconnectOldUser( pcUser ); - may be causing issues with selecting chars causing D/C afterwards. This was added last year and isn't used on Regnum. Maybe it's not neccesary

	USERSERVER->SendServerMessages(pcUser);

	USERSERVER->SetAccountNameFromSQL(pcUserData);

#ifdef RECONNECT_SYSTEM
	USERSERVER->DeleteReconnectUser(pcUserData->szAccountName);
#endif

	NETSERVER->OnGetGameMaster(pcUserData);

	USERSERVER->UpdateCharacterIDFromSQL(pcUser);


	NETSERVER->LoadQuestData(pcUser);
	NETSERVER->LoadPVPData(pcUser);

	ITEMSERVER->OnLoadUser(pcUser);

	BELLATRASERVER->OnLoadUser(pcUser);

	HALLOWEENHANDLER->SendEventStatusToUser(pcUser);

	CHARACTERSERVER->SQLLoadUserEffectTitleAndRanking(pcUser);

	SendServerCommonSkillInfoToClient(pcUser);

	ARCHERSKILL->SendServerSkillInfoToClient(pcUser);
	FIGHTERSKILL->SendServerSkillInfoToClient(pcUser);
	MECHANICIANSKILL->SendServerSkillInfoToClient(pcUser);
	PIKEMANSKILL->SendServerSkillInfoToClient(pcUser);
	ASSASSINSKILL->SendServerSkillInfoToClient(pcUser);

	ATALANTASKILL->SendServerSkillInfoToClient(pcUser);
	KNIGHTSKILL->SendServerSkillInfoToClient(pcUser);
	MAGICIANSKILL->SendServerSkillInfoToClient(pcUser);
	PRIESTESSSKILL->SendServerSkillInfoToClient(pcUser);
	SHAMANSKILL->SendServerSkillInfoToClient(pcUser);


	SendServerCommonSkillDataToClient(pcUser);

	ARCHERSKILL->SendServerSkillDataToClient(pcUser);
	FIGHTERSKILL->SendServerSkillDataToClient(pcUser);
	MECHANICIANSKILL->SendServerSkillDataToClient(pcUser);
	PIKEMANSKILL->SendServerSkillDataToClient(pcUser);
	ASSASSINSKILL->SendServerSkillDataToClient(pcUser);

	ATALANTASKILL->SendServerSkillDataToClient(pcUser);
	PRIESTESSSKILL->SendServerSkillDataToClient(pcUser);
	KNIGHTSKILL->SendServerSkillDataToClient(pcUser);
	SHAMANSKILL->SendServerSkillDataToClient(pcUser);
	MAGICIANSKILL->SendServerSkillDataToClient(pcUser);

	SendSkillChainScaleDataToClient(pcUser);


	//Record time (seconds since 1970) when the user has loaded
	//We will use this to calculate time elapsed in various places.
	pcUser->uUserLoadedTime = TIME;

	SendCharacterDataEx(pcUser);

	// Online Status
	auto pcDB = SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo);

	// Clan Message
	int iClanID = 0;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET IsOnline=1, JobCode=?, LastSeenDate=GETDATE() WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUserData->sCharacterData.iClass);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcUser->iCharacterID);
			pcDB->ExecuteUpdate();
		}

		pcDB->Clear();

		if (pcDB->Prepare("SELECT ClanID FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pcUserData->sCharacterData.szName);
			if (pcDB->Execute() && pcDB->Fetch())
			{
				pcDB->GetData(1, PARAMTYPE_Integer, &iClanID);
			}
		}
		pcDB->Close();
	}


	pcDB = SQLCONNECTION(DATABASEID_ClanDB);

	if (iClanID > 0)
	{
		char szClanMessage[128];
		if (pcDB->Open())
		{
			if (pcDB->Prepare("SELECT LoginMessage FROM ClanList WHERE ID=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iClanID);
				if (pcDB->Execute() && pcDB->Fetch())
				{
					pcDB->GetData(1, PARAMTYPE_String, szClanMessage, 128);
				}
			}
			pcDB->Close();
		}

		if (!IsNull(szClanMessage))
		{
			CHATSERVER->SendChatEx(USERDATATOUSER(pcUserData), CHATCOLOR_Clan, "> %s", szClanMessage);
		}
	}

	// Get Item Box
	int iItemsBox = ITEMSERVER->SQLItemBoxCount(pcUserData->szAccountName);
	if (iItemsBox > 0)
	{
		CHATSERVER->SendChatEx(USERDATATOUSER(pcUserData), CHATCOLOR_Global, "> You've received %d item%s in the Item Distributor!", iItemsBox, iItemsBox == 1 ? "" : "s");
	}
	// Get Coin Amount
	int iCoinAmount = COINSHOPHANDLER->GetCoinAmount(pcUserData->szAccountName);
	if (iCoinAmount > 0)
	{
		CHATSERVER->SendChatEx(USERDATATOUSER(pcUserData), CHATCOLOR_Global, "> You have %d Credits!", iCoinAmount);
	}

	std::vector<std::string> vNotification = ACCOUNTSERVER->SQLGetAccountNotification(pcUserData->szAccountName);
	if (vNotification.size() > 0)
	{
		CHATSERVER->SendChatEx(USERDATATOUSER(pcUserData), CHATCOLOR_Error, ">You have %d notice%s!", vNotification.size(), vNotification.size() > 1 ? "s" : "");
		for (std::vector<std::string>::iterator it = vNotification.begin(); it != vNotification.end(); it++)
		{
			std::string str = (*it);
			CHATSERVER->SendChatEx(USERDATATOUSER(pcUserData), CHATCOLOR_Error, "> %s", str.c_str());
		}
	}

	BLESSCASTLESERVER->SendUserScore(USERDATATOUSER(pcUserData));

	BELLATRASERVER->SendFullCrownDataToUser(pcUserData);

	NETSERVER->SendCommandToLoginServer(PKTHDR_NetGetMuteSync, pcUserData->iID); //get mute status from login server
}

void CharacterServer::OnCharacterSyncDataEx(UserData* pcUserData, PacketPlayDataEx* psPacket)
{
	User* pcUser = USERDATATOUSER(pcUserData);

	if (GAME_SERVER)
	{
		psPacket->iHeader = PKTHDR_NetPlayDataEx;
		psPacket->userID = pcUser->GetID();
		NETSERVER->SendPacketToLoginServer(psPacket);
	}

	NETSERVER->SendClan(pcUserData);
	memcpy(&pcUser->pData, psPacket, sizeof(PacketPlayDataEx));
	memcpy(&pcUserData->sCharacterData.iStrength, &pcUser->pData.sCharacterData, sizeof(CharacterDataPacket));
	pcUserData->sCharacterData.iCritical = pcUser->pData.iCritical;

	// Check Gold
	CheckGold(pcUserData);

	if (pcUserData->iGameLevel == GAMELEVEL_None)
	{
		if (!GAME_SERVER && psPacket->dwSpeed >= ((((25 * 10) + 250) * 460) >> 8))
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_RunSpeedHack;
			sPacket.LParam = psPacket->dwSpeed;
			LOGSERVER->OnLogCheat(pcUserData, &sPacket);
			return;
		}
	}

	// Not Game Master?
	if (pcUser->pData.bGameMaster && !pcUserData->iGameLevel)
	{
		pcUser->pData.bGameMaster = false;
		psPacket->bGameMaster = false;

		if (pcUser->bCheckedGameMaster)
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID = CHEATLOGID_FakeGM;
			LOGSERVER->OnLogCheat(pcUserData, &sPacket);
		}
		else
			pcUser->bCheckedGameMaster = TRUE;
	}

	if (!pcUser->dwTimeSync)
	{
		pcUser->dwTimeSync = TICKCOUNT;
	}

	// Server ID
	pcUserData->bServerIndexID = (BYTE)psPacket->dwServerID;


	if (GAME_SERVER)
	{
		// Send server information to user debug
		PacketServerInfo sPacket;
		ZeroMemory(&sPacket, sizeof(PacketServerInfo));
		sPacket.iLength = sizeof(PacketServerInfo);
		sPacket.iHeader = PKTHDR_ServerInfo;
		sPacket.iUsers = pcUserData->iGameLevel >= GAMELEVEL_Two ? USERS_ONLINE : 0;
		sPacket.iMaxUsers = pcUserData->iGameLevel >= GAMELEVEL_Two ? MAX_USERS : 0;
		sPacket.iBossTime = BOSS_TIME;
		sPacket.iServerVersion = SERVER_VERSION_MIN;
		sPacket.bSiegeWarType = BLESSCASTLESERVER->GetSiegeWarMode();
		sPacket.sSiegeWarDay = GetSiegeWarDay();
		sPacket.sSiegeWarHour = GetSiegeWarHour();
		GetSystemTime(&sPacket.sServerTime);

		if (pcUserData->iGameLevel >= GAMELEVEL_Two)
			sPacket.bGameMaster = TRUE;

		SENDPACKET(USERDATATOUSER(pcUserData), &sPacket);
	}

	CHEATSERVER->CheckStatePoint(pcUserData, psPacket->dwQuestLevelLog);
}

void CharacterServer::SQLSaveCharacterDataEx(User* pcUser)
{
	CHARACTERSERVER->SQLSaveCharacterDataEx(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), pcUser);

	if (SERVER_IS_SEASONAL)
	{
		CHARACTERSERVER->SQLSaveCharacterDataEx(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), pcUser);
	}
}

void CharacterServer::SQLSaveCharacterDataEx(SQLConnection* pcDB, User* pcUser)
{
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET FSP=? WHERE Name=?"))
		{
			auto charName = GetCharacterName(pcUser->pcUserData);

			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcUser->sCharacterDataEx.iFSP);
			pcDB->BindParameterInput(2, PARAMTYPE_String, charName, 32);

			if (pcDB->ExecuteUpdate() == FALSE)
			{
				DEBUG("Update FSP failed for char name: '%s'", charName);
			}
		}
		pcDB->Close();
	}
}

void CharacterServer::SQLUpdateLevelAndJobCode(CharacterData* psCharacterData)
{
	SQLUpdateLevelAndJobCode(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), psCharacterData);

	if (SERVER_IS_SEASONAL)
	{
		SQLUpdateLevelAndJobCode(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), psCharacterData);
	}
}

void CharacterServer::SQLUpdateLevelAndJobCode(SQLConnection* pcDB, CharacterData* pcCharacterData)
{
	//std::cout << "SQLUpdateLevelAndJobCode  = " << pcCharacterData->iClass << std::endl;

	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET JobCode=?, Level=? WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &pcCharacterData->iClass);
			pcDB->BindParameterInput(2, PARAMTYPE_Integer, &pcCharacterData->iLevel);
			pcDB->BindParameterInput(3, PARAMTYPE_String, &pcCharacterData->szName);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}

void CharacterServer::SQLUpdateLevelUpDate(int iCharacterId)
{
	SQLUpdateLevelUpDate(SQLCONNECTION(DATABASEID_UserDB_LocalServer_CharInfo), iCharacterId);

	if (SERVER_IS_SEASONAL)
	{
		SQLUpdateLevelUpDate(SQLCONNECTION(DATABASEID_UserDB_PrimaryServerSyncer), iCharacterId);
	}
}


void CharacterServer::SQLUpdateLevelUpDate(SQLConnection* pcDB, int iCharacterId)
{
	if (pcDB->Open())
	{
		if (pcDB->Prepare("UPDATE CharacterInfo SET LevelUpDate=GETDATE() WHERE ID=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iCharacterId);
			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}
}

void CharacterServer::OnCharacterUpdateLevelClan(User* pcUser, PacketUpdateClanLevel* psPacket)
{
	if (pcUser->pcUserData)
	{
		int iLevelOld = pcUser->pcUserData->sCharacterData.iLevel;

		if (psPacket->dwCodeOP == 0)
		{
			pcUser->pcUserData->sCharacterData.iLevel = psPacket->dwLevel;
		}
		else if (psPacket->dwCodeOP == 1)
		{
			pcUser->pcUserData->sCharacterData.iLevel = psPacket->dwLevel;
			pcUser->pcUserData->sCharacterData.iClanID = psPacket->dwClanCode;
			pcUser->pcUserData->sCharacterData.cNewLoad = (BYTE)psPacket->dwUpdateInfo;
			pcUser->pcUserData->iClanID = GetClanCode(pcUser->pcUserData->sCharacterData.iClanID);

			BLESSCASTLESERVER->SendTopClansSync(pcUser);

			SQLUpdateLevelAndJobCode(&pcUser->pcUserData->sCharacterData);
			SendBlessCastleSkill(pcUser->pcUserData);
		}

		if (pcUser->pcUserData->sCharacterData.iLevel != iLevelOld)
		{
			SQLUpdateLevelUpDate(pcUser->iCharacterID);

			if (pcUser->bParty)
			{
				PARTYHANDLER->SendChatParty(pcUser, FormatString("> %s has leveled up to %d!", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
					pcUser->pcUserData->sCharacterData.iLevel), CHATCOLOR_Party, true);
			}

			if (pcUser->pcUserData->iClanID)
			{
				CHATSERVER->SendChatAllUsersInClan(pcUser->pcUserData, CHATCOLOR_Clan, "> %s has leveled up to %d!", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
					pcUser->pcUserData->sCharacterData.iLevel);

				CHATSERVER->SendChatEx(pcUser, CHATCOLOR_Clan, "> %s has leveled up to %d!", CHARACTERSERVER->GetCharacterName(pcUser->pcUserData),
					pcUser->pcUserData->sCharacterData.iLevel);
			}

			QUESTSERVER->UpdateNPCQuestIconForUserPostLevelUp(pcUser);
			QUESTSERVER->SendNewQuestsPostLevelUp(pcUser);
		}
	}
}


void CharacterServer::OnOutCharacterData(UserData* pcUserData)
{
	return;
}

void CharacterServer::SetPositionWarp(UserData* pcUserData, int iX, int iZ, int iStageID)
{
	if (pcUserData)
	{
		PacketWarpGateField sPacket;
		sPacket.iLength = sizeof(PacketWarpGateField);
		sPacket.iHeader = PKTHDR_WarpGateField;
		sPacket.iStage = iStageID != -1 ? iStageID : pcUserData->iMapID;
		sPacket.iX = iX;
		sPacket.iZ = iZ;
		SENDPACKET(USERDATATOUSER(pcUserData), &sPacket);

		if (iStageID != -1)
			pcUserData->iMapID = (EMapID)iStageID;

		pcUserData->sPosition.iX = iX;
		pcUserData->sPosition.iZ = iZ;
	}
}

BOOL CharacterServer::IsCharacterLeader(User* pcUser)
{
	BOOL bRet = FALSE;
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_ClanDB);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT ID FROM ClanList WHERE ClanLeader=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, GetCharacterName(pcUser->pcUserData));
			if (pcDB->Execute() && pcDB->Fetch())
				bRet = TRUE;
		}
		pcDB->Close();
	}
	return bRet;
}

char* CharacterServer::GetServerName(User* pcUser)
{
	return GSERVER->GetServerName(pcUser->pcUserData->bServerIndexID);
}

void CharacterServer::PHCharacterDataEx(User* pcUser)
{
	if (!GAME_SERVER)
	{
		char szPath[MAX_PATH];
		GetCharacterExFilePath(pcUser->pcUserData->szCharacterName, szPath, MAX_PATH);

		FILE* fp;
		fopen_s(&fp, szPath, "rb");
		if (fp)
		{
			//	fread()
			fclose(fp);
		}
	}
}

int CharacterServer::SQLGetMonsterID(char* pszMonsterName)
{
	int iID = -1;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_Monsters, 1);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT ID FROM MonsterList WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszMonsterName, 32);

			if (pcDB->Execute())
			{
				if (pcDB->Fetch())
					pcDB->GetData(1, PARAMTYPE_Integer, &iID);
			}
		}

		pcDB->Close();
	}

	return iID;
}

int CharacterServer::SQLGetNPCID(char* pszNPCName, int iMapID)
{
	int iID = -1;

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_Npc, 1);



	if (pcDB->Open() != FALSE)
	{
		if (pcDB->Prepare(iMapID == (-1) ? "SELECT ID FROM NPCList WHERE Name=?" : "SELECT IDNPC FROM MapNPC WHERE IDNPC in (SELECT ID FROM NPCList WHERE Name=?) AND (Stage=?) AND ([Enabled]=1)"))
		{
			if (iMapID != (-1))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, pszNPCName, STRLEN(pszNPCName));
				pcDB->BindParameterInput(2, PARAMTYPE_Integer, &iMapID);
			}
			else
				pcDB->BindParameterInput(1, PARAMTYPE_String, pszNPCName, 32);

			if (pcDB->Execute())
			{
				if (pcDB->Fetch())
					pcDB->GetData(1, PARAMTYPE_Integer, &iID);
			}
		}

		pcDB->Close();
	}

	return iID;
}

int CharacterServer::SQLGetCharacterID(SQLConnection* pcDB, char* pszName)
{
	int iID = -1;


	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT ID FROM CharacterInfo WHERE Name=?"))
		{
			pcDB->BindParameterInput(1, PARAMTYPE_String, pszName, STRLEN(pszName));

			if (pcDB->Execute() && pcDB->Fetch())
				pcDB->GetData(1, PARAMTYPE_Integer, &iID);
		}

		pcDB->Close();
	}

	//DEBUGSTATUS( "[SQLGetCharacterID] Char id is: %d for name: %s", iID, pszName);

	return iID;
}

void CharacterServer::HandlePacket(User* pcUser, PacketCharacterDataEx* psPacket)
{
	CopyMemory(&pcUser->sCharacterDataEx, &psPacket->sData, sizeof(CharacterDataEx));
	CHATSERVER->SendDebugChat(pcUser, CHATCOLOR_Error, "> SaveEx Success");
}

void CharacterServer::HandlePacket(User* pcUser, PacketCharacterCombatData* psPacket)
{
	pcUser->iDefenseRating = psPacket->iDefenseRating;
	pcUser->sBlockChance = psPacket->sBlockChance;
	pcUser->sEvadeChance = psPacket->sEvadeChance;

}

void CharacterServer::Debug(const char* pszFormat, ...)
{
	if (bDebug)
	{
		char buff[200];
		va_list ap;

		va_start(ap, pszFormat);
		vsnprintf_s(buff, 200, pszFormat, ap);
		va_end(ap);

		std::cout << buff << "\n";
	}
}