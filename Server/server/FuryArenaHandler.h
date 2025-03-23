#pragma once

#define _X_KINGFURY			-928739
#define _Y_KINGFURY			124461
#define _Z_KINGFURY			-11049304

#define _X_LEAVEFURYARENA	-928749
#define _Z_LEAVEFURYARENA	-10776389


#define _BOSS_FURYARENA					"King Fury"
#define _BOSS_FURYARENALOW				"Ancient Fury"
#define _MINION_FURYARENA				"Fury Worm"
#define _MINION_FURYARENALOW			"B-Machine"
#define _MONSTEREVENT_FURYARENA			"Wanted Wolf"
#define _MONSTEREVENT_FURYARENALOW		"Wanted Wolf"
#define _MINION_FURYARENA_CNT			6
#define _METEOR_CNT						6
#define _FURYARENA_OUTER_POSITION_CNT		12


// Minions Position


class CFuryArenaHandler
{
private:

	const Point3D saMinionArenaPositions[_MINION_FURYARENA_CNT]
	{
		// X,				Y,              Z
		{-832643  ,         129672,      -11044001 },
		{-877737  ,         129672,      -11133150 },
		{-976560  ,         129672,      -11131540 },
		{-1020146 ,         129672,      -11051200 },
		{-975771  ,         129672,      -10966303 },
		{-879339  ,         129672,      -10965339 },
	};

	const Point3D saFuryArenaPositions[_FURYARENA_OUTER_POSITION_CNT]
	{
		// X,				Y,              Z
		{-826925  ,         129672,      -11049204 },
		{-839432  ,         129672,      -11099298 },
		{-876055  ,         129672,      -11138068 },
		{-926033  ,         129672,      -11152344 },
		{-981443  ,         129672,      -11140282 },
		{-1017854 ,         129672,      -11099024 },
		{-1031620 ,         129672,      -11048370 },
		{-1018571 ,         129672,      -10997418 },
		{-978479  ,         129672,      -10958574 },
		{-926704  ,         129672,      -10945471 },
		{-876089  ,         129672,      -10960102 },
		{-836435  ,         129672,      -10997825 }
	};

	//more precise positiong but causes meterorites to go out of stage?

	/*const Point3D saMeteoritePosition[_METEOR_CNT] =
	{
		{-844741  ,         0,      -11094740 },
		{-925581  ,         0,      -11144875 },
		{-1010140 ,         0,      -11098940 },
		{-1010569 ,         0,      -11001908 },
		{-928553  ,         0,      -10955289 },
		{-847235  ,         0,      -10999720 }

		{-902681 , 129672, -10958374},
		{-859710 , 129672, -10982346},
		{-835370 , 129672, -11025516},
		{-835588 , 129672, -11075310},
		{-859397 , 129672, -11118648},
		{-903482 , 129672, -11143420},
		{-952186 , 129672, -11143648},
		{-993691 , 129672, -11116756},
		{-1019404, 129672, -11074775},
		{-1020402, 129672, -11023876},
		{-998170 , 129672, -10979725},
		{-951514 , 129672, -10956815}
	};*/


	enum FuryEventID : int
	{
		KingFury  = 0,
		LordBabel = 1,
	};

	struct SPrizeData									saPrizesReward[50];
	int													iTotalPercent = 0;

	BOOL												KillUnits();

	SPrizeData											* GetRewardItem( int iID );

public:
	CFuryArenaHandler();
	virtual ~CFuryArenaHandler();

	int													GetEventID();

	DWORD												GetTimeLeft( SYSTEMTIME * ps );

	DWORD												GetTick() { return dwTickTime; }

	BOOL												InsideArena( Point3D * psPosition );

	int													GetTotalUsers();

	void												ReadConfigIniValues();

	void												OnFuryArenaTeleporterNPCClicked ( User * pcUser );
	void												OnFuryArenaNPCClicked ( User * pcUser );

	BOOL												IsStarted() { return bStarted; };
	BOOL												IsEnabled() { return bEnabled; };
	BOOL												IsBoss( UnitData * pcUnitData );
	void												SetEnable( BOOL bVal ) { bEnabled = bVal; };

	void												SQLLog( char * pszLog, ... );
	void												SQLClearFuryArenaRewardTracker();
	void												SQLCheckAndClearFuryArenaRewardTrackerAtZeroHour();
	BOOL												SQLAddUserToRewardTracker( int iEventID, int iCharacterId );
	BOOL												SQLCanReceiveEXPReward( int iCharacterId );

	BOOL												ExchangeItem( User * pcUser, PacketFuryArenaRequest * psPacket );

	BOOL												SetBoss( const char * szMonsterName );
	BOOL												SetMinion( const char * szMonsterName );
	BOOL												SetMonsterEvent( const char * szMonsterName );

	BOOL												UpdateTime();
	BOOL												SetTimeMinion( int iMinute = 1 );

	void												ForceStartEvent(int iEventIDOverride);
	void												ForceStartEventTestMode(int iEventIDOverride);
	BOOL												StartEvent();
	BOOL												EndEvent();
	BOOL												LeaveFromArena();
	BOOL												LeaveFromArena( User * pcUser );
	BOOL												OnDisconnected( User * pcUser );

	BOOL												OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData, int iKillerUserDataId );

	void												PacketHandlerData( User * pcUser, PacketFuryArenaRequest * psPacket );
	BOOL												EnterFromArena( User * pcUser );

	void												Meteorite();

	void												UpdateHPBoss();

	void												Tick1s();

	void												Init();

protected:
	FuryEventID											iEventID     = FuryEventID::KingFury;
	FuryEventID											iEventIDNow  = FuryEventID::KingFury;

	BOOL												bTestMode					 = FALSE;
	BOOL												bUsersAllLeftFuryArena		 = FALSE;
	BOOL												bRewardTrackerResetDone      = FALSE;
	BOOL												bCanEnter		= FALSE;
	BOOL												bEnabled		= FALSE;
	BOOL												bStarted		= FALSE;
	BOOL												bAlert1Start	= FALSE;
	BOOL												bAlert2Start	= FALSE;
	BOOL												bAlert3Start	= FALSE;
	BOOL												bAlert4Start	= FALSE;
	WORD												wDayEvent = 0;
	int													iHourEvent		= 21;
	int													iMinuteEvent	= 30;

	//int													iSecondsSpawnMinion = 120;  //every 120 seconds

	//DWORD												dwEventLeaveTime = 0;
	DWORD												dwForceStartTime = 0;
	DWORD												dwTickTime = 0;
	DWORD												dwStarted = 0;
	DWORD												dwMeteor = 0;

	int													iMaxExpRewardPerDay = 2;
	int													iDeathCounter = 0;
	int													iMinionsKilled = 0;
	int													iTotalUsers = 0;

	char												szBossName[2][32];
	char												szMinionName[2][32];
	char												szMonsterEventName[2][32];

	UnitData											* pcUnitDataMinions[_MINION_FURYARENA_CNT];
	BOOL											    minionKilled[_MINION_FURYARENA_CNT];
	UnitData											* pcUnitDataBoss = NULL;
};

