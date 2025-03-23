#pragma once

static int iaBellatraRoundRoomTable[4][8] = {
	{ 0,	0,	0,	4,	4,	4,	6,	6 },
	{ 1,	1,	1,	4,	4,	4,	6,	6 },
	{ 2,	2,	2,	5,	5,	5,	6,	6 },
	{ 3,	3,	3,	5,	5,	5,	6,	6 }
};

static int iaBellatraRoundTable[8][2] = {
	{ 0,	4 },		//1
	{ 0,	4 },		//2
	{ 0,	4 },		//3
	{ 4,	6 },		//4
	{ 4,	6 },		//5
	{ 4,	6 },		//6
	{ 6,	7 },		//7
	{ 6,	7 }		//8
};

static Point3D saBellatraSpawnPoints[7][3] = {
	{	
		{ 18967,  1154,	3055 },
		{ 19995,  1154,	2134 },
		{ 19485,  1154,	2615 }	
	},
	{
		{ 20838,  1154,	3024 },
		{ 21671,  1154,	2193 },
		{ 21255,  1154,	2615 }	
	},
	{
		{ 22478,  1154,	3049 },
		{ 23437,  1154,	2135 },
		{ 22948,  1154,	2615 }
	},
	{
		{ 24204,  1154,	3094 },
		{ 25173,  1154,	2135 },
		{ 24674,  1154,	2615 }
	},
	{	
		{ 20509,  1621,	5284 },
		{ 21544,  1621,	4261 },
		{ 21012,  1621,	4773 }	
	},
	{
		{ 22575,  1621,	5253 },
		{ 23610,  1621,	4230 },
		{ 23077,  1621,	4742 }	
	},
	{	
		{ 21538,  1638,	7057 },
		{ 22527,  1638,	6079 },
		{ 22018,  1638,	6569 }	
	}
};


struct BellatraData
{
	struct BellatraUnitData
	{
		char			szName[32];
		CharacterData	* psCharacterData;
		int				iPercent;
	};

	BellatraUnitData	saMonster[4];
	BellatraUnitData	sBoss;

	int					iPercent;
	int					iHour;
	BOOL				iPoints;
	int					iMaxMonsters;
	int					iDelayTime;
};


struct BellatraMonsterSpawnHandle
{
	struct SSpawn
	{
		BOOL				iCount;
	};

	SSpawn					saRound[8];
};

#define MAX_BELLATRA_REWARD_LOCATIONS 20


class BellatraServer
{
private:
	IMPFNC										pfnRoomScoreData			= 0x07AC9BB8;
	IMPFNC										pfnRoomScoreKill			= 0x08B810B0;
	IMPFNC										pfnUpdateBellatraUserScore	= 0x0054DBF0;
	IMPFNC										pfnHandleBellatraRoomItem	= 0x0054FFC0;
	IMPFNC										pfnHandleConfigNextRound	= 0x0054D750;
	IMPFNC										pfnCreateBellatraItem		= 0x005525E0;
	IMPFNC										pfnSendScore				= 0x0055B220;


	int											iBellatraDropMinX			= 5518000;
	int											iBellatraDropMaxX			= 5755000;
	int											iBellatraDropMinZ			= 1560000;
	int											iBellatraDropMaxZ			= 1800000;
	int											iBellatraDropY				= 419400;

	BOOL										bUseSoloCrownHumor			= FALSE;
	

public:
	BellatraServer();
	virtual ~BellatraServer();

	void										SendFullCrownDataToAllUsers();
	void										SyncOldClanTablesWithNew();
	void										UpdateCrownClanIDs();

	void										SetSoloCrownHumorMode(BOOL b) { bUseSoloCrownHumor = b; }
	BOOL										IsSoloCrownHumorModeOn() { return bUseSoloCrownHumor; }

	static BellatraData::BellatraUnitData		* GetBellatraMonster( int iIndex ) { return ((BellatraData::BellatraUnitData*)0x07AABE80) + iIndex; }
	static BellatraMonsterSpawnHandle			* GetBossSpawnHandle() { return (BellatraMonsterSpawnHandle*)0x07AB3054; }
	static BellatraMonsterSpawnHandle			* GetMonsterSpawnHandle() { return (BellatraMonsterSpawnHandle*)0x07AC9B98; }

	static PacketBellatraResult					* GetBellatraResultPacket() { return (PacketBellatraResult*)0x07AABFA8; }

	void										SetMaxMonsterInRooms( int i ) { (*(int*)0x07AABF48) = i; }
	int											GetMaxMonsterInRooms() { return (*(int*)0x07AABF48); }
	void										SetTimeout( DWORD dw ) { (*(DWORD*)0x07AB14F8) = dw; }
	DWORD										GetTimeout() { return (*(DWORD*)0x07AB14F8); }

	void										SetTimeUpdate( DWORD dw ) { WRITEDWORD( 0x07AC9E34, dw ); }
	DWORD										GetTimeUpdate() { return READDWORD( 0x07AC9E34 ); }

	void										SetDelayTime( DWORD dw ) { WRITEDWORD( 0x07AC9E38, dw ); }
	DWORD										GetDelayTime() { return READDWORD( 0x07AC9E38 ); }

	int											GetRankRoomID( int iRank ) { return (*(int*)(0x07AABF94 + (iRank * 4))); }

	int											GetUserRoomCount( const int iRoomID);
	void										SetUserRoomCount( const int iRoomID, const int iCount );

	int											GetOriginalUserRoomCount(const int iRoomID);
	void										SetOriginalUserRoomCount(const int iRoomID, const int iCount);


	void										SetRoomSoloMode(const int iRoomId, BOOL bSoloMode);
	BOOL										GetRoomSoloMode(const int iRoomId);

	int											GetRoomScore( int i );

												
	int											GetTotalUsers() { return (*(int*)0x07AABD38); } //All users from all rooms!!
	void										SetTotalUsers(int i) { (*(int*)0x07AABD38) = i; }

	static BellatraData							* GetBellatraData() { return *(BellatraData**)(0x006E469C); }
	static void									SetBellatraData( BellatraData * ps ) { *(BellatraData**)(0x006E469C) = ps; }
	int											GetRound() { return (*(int*)0x07AAC884) + 1; }
	void										SetRound( int i ) { (*(int*)0x07AAC884) = i; }

	void										ResetBellatra();

	void										AllowBellatraReset(BOOL allow) { bAllowSodScoreReset = allow; }
	
	void										ClearAllSoloCrownRankingFromUsers();
	int											GetTopoSoloScoreForMe(int iCharacterId);
	void										UpdateAndSyncUserSoloRanking(User* pcUser, int iRanking, BOOL bUpdateLoginServer = TRUE);
	int											GetTopoSoloScoreForClass(ECharacterClass eCharacterClass);
	void										UpdateAllSoloCrownRankingForClass(ECharacterClass eCharacterClass);
	void										UpdateAllSoloCrownRankingForAllClasses();
	int											GetSoloCrownRankingForUser(User* pcUser);
	int											GetSoloCrownRankingForUser(int iCharacterId, ECharacterClass eCharacterClass);
	
	BOOL										PHGoldClanRecv( User * pcUser, struct PacketClanGoldReceive * psPacket );

	void										OnLoadUser(User* pcUser);

	void										DeletePoint( char * pszUserID, char * pszCharName, char * pszIP, int iUnk, int iUnk2 );

	int											AddTaxGoldToTopThreeClans( const int SodScore );

	int											GetTeamPoint( UserData * pcUserData );

	BOOL										RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode );

	BOOL										RecordClanPoint( const char * pszClanName, int iPoint, int iOldPoint );
	int											GetClanPoint( const char * pszClanName );
	int											GetClanPoint( int iClanId);
	BOOL										GetClanNameFromId ( int iClanId, char* pszClanName);
	BOOL										DoWeeklyResetIfRequired();
	void										SendFullCrownDataToUser(UserData * pcUserData);
	BOOL										CheckAndUpdateClanTopScore( );

	BOOL										IsSodActive () { return bSodIsActive; }

	BOOL										AddRoomScore( int iRoomID, int iScore );
	BOOL										UnitKilled( UnitData * pcUnitData );

	static BOOL									UpdateBellatraUserScore( UserData * pcUserData );

	static BOOL									HandleItemRoom( UnitData * pcUnitData, UserData * pcUserData );

	static void									HandleConfigurationNextRound();

	static void									SendScoreUser( UserData * pcUserData, int iScore, int iKills, int iTotalScore, int iUserCount, int iTotalUsers, int iCode );

	static BOOL									CreateBellatraItem( int iItemID, Map * pcStage, int iX, int iY, int iZ, int iQuantity );

	BOOL										UpdateServer( Map * pcStage, MonsterMapSpawnHandle * psMonsterData );

	void										SQLBellatraRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity );
	void										SQLReadInRewardsTable();

	void										HandlePacket( User * pcUser, PacketReceiveItemIhin * psPacket );

	void										DropBellatraRewards(int iNumOfDrops);
	void										TestBellatraReward();
	SPrizeData									* GetPrizeBellatraRewardItem();


	void										Init();

	void										ForceStartSod();
	void										ForceEndSod();
	void										AddEntranceFee(int fee);
	int											GetSumEntranceFees() { return iSumEntranceFees; }
	int											GetFinalRoomScore();
	int											GetSumPrizePoolFromEntranceFees();
	void										SendPrizeAndClearUserBellatraStats(UserData* userData, const short sBellatraRoomId, const float fPercentOfPrize, const bool bIncludePrizePool);
	int											GetUserPlacementPrize(const int iPrizePool, const short bellatraRoomId);
	const char *								GetPlacementString(const short bellatraRoomId);
	int											GetPlacement(const short bellatraRoomId);

	BOOL										bSkipCurrentRound = FALSE;
	BOOL										bForceInit = FALSE;
	BOOL										bForceHour = FALSE;

	BOOL										bAllowSodScoreReset = TRUE;

private:
	DWORD										dwFinishTime = 0;

	int											iIhinPosition = -1;
	int											iTotalPercent = 0;

	BOOL										bSodIsActive = FALSE;

												//Sum entrance fees will be used for bonus prize (80%)
	int											iSumEntranceFees = 0;

	BOOL										bDebug = 0;

	int											iTempCounter = 0;

	int											iGoldCrownClanId = 0;
	int											iSilverCrownClanId = 0;
	int											iBronzeCrownClanId = 0;

												//num of team prize given out
												//this determines 1st, 2nd and 3rd prize percentage
												//See https://docs.google.com/spreadsheets/d/1i_L5-JUzPuEar8zmhDiUoJ_9DqX1mkA2zkztBK43aco/edit?usp=sharing
	BOOL										bThirdPlacePrizeGivenOut = FALSE;
	BOOL										bSecondPlacePrizeGivenOut = FALSE;

	BOOL										bBroadcastSoDStartingSoonAtHour23 = TRUE;
	BOOL										bBroadcastSoDEntryIsOpenAtHour4 = TRUE;

	int											iaUserRoomCount[4];
	BOOL										baRoomSoloMode[4];

												//The original number of users in each room (for prize computations)
	int											iOriginalUserRoomCount[4];

	SPrizeData									saPrizes[50];
};

