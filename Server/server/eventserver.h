#pragma once

/// <summary>
/// Event variables and functions in ASM
/// </summary>
enum class EEventASM
{
	EVENT_CHILD_VAR = 0x845FF4,		//Children's Event (Attack weighted when Ulken is a character).
};

struct PrizeEventRudolphUser
{
	char					szAccountName[32];

	PrizeEventRudolphUser( char szA[32] )
	{
		StringCchCopyA( szAccountName, 32, szA );
	};

	~PrizeEventRudolphUser() {};
};

struct HuntEventMobSpawnFlag
{
	int iMapId;
	int iPosX;
	int iPosZ;
	int iIndex;
};

struct SpawnPos
{
	int iPosX;
	int iPosZ;
};

struct MimicSpawnInfo
{
	int iMonsterID;
	int iMapID;

	float fSpawnChance;			//spawn chance for mimic
	time_t tLastSpawnTime;      //time when mimic was last spawned
	int iMinDelayBetweenSpawn;  //min delay in seconds between each mimic spawn. i.e. 30 minutes means if a mimic was spawned 20 minutes ago, no further mimics will be spawned for next 10 minutes
};

struct HuntEventMobData
{
	BOOL		bActive = FALSE;
	ID			iMonsterID;
	DWORD		dwNextSpawnTime;
	int			iMapId;
	int			iSpawnX;
	int			iSpawnZ;
};


class CEventServer
{


protected:
	std::vector<SPrizeData*>						vPrizesReward;
	int												iTotalPercent = 0;

private:
	SPrizeData										* GetPrizeRewardItem();

	void											SQLOnlineRewardLog( char * pszAccountName, char * pszName, char * pszItemName, int iQuantity );

	bool											IncludeUserInPrizePool( UserData* pcUserData );
	void											SetPrizeOnlineReward();

	void											LoadPrizesOnlineReward();

	BOOL											bOnlineRewardInitialized = FALSE;
	BOOL											bHuntEventInitialized = FALSE;
	BOOL											bForceNextOnlineReward = FALSE;
	WORD											wNextMinuteOnlineReward = 0;
	WORD											wLastHourChristmas = 0;

	BOOL											bResetRudolph = FALSE;

	std::vector<PrizeEventRudolphUser*>				vChristmasEventRudolph;
	std::vector<HuntEventMobData*>					vHuntEventMobData;
	std::map<int, int>								mUserToHuntMobKilledCounter;
	std::vector<int>                                mHuntMobSpawnMapIDs;
	std::map<int, std::vector<SpawnPos>>            mHuntMobSpawnLocations;

	DWORD											dwNextHuntMobCheckTime = 0;


	int												iWantedWolfMonsterID = 0;
	int												iWantedMoriphMonsterID = 0;

	std::map<int, MimicSpawnInfo>					mMapIdToMimicSpawnInfo;


public:

	enum ServerEventID
	{
		EXP,
		SiegeWar,
		Bellatra,
		Morif,
		Wolf,
		Christmas,
		Halloween,
		Drop,
		Valentine,
		OnlineReward,
		Mimic
	};


	CEventServer();
	virtual ~CEventServer();

	void											Init ();

	BOOL											SQLLoadHuntEventData( );
	BOOL											SQLLoadMimicSpawnData( );
	BOOL											IsEventMimicMonster( UnitData * pcUnitData );
	BOOL											IsEventHuntMonster( UnitData * pcUnitData );
	BOOL											OnEventMimicSpawned( class UnitData * psUnitData, class Map * pcMap );

	void											GiveRudolphPrizes();

	BOOL											SQLLogEventKill( enum ServerEventID eventID, User * pcUser, UnitData * pcUnitData );

	BOOL											TryGetEventMonsterSpawn( struct CharacterData * psCharInfo, class Map * pcMap );

	void											HandlePacket( UserData * pcUserData, PacketNPCEasterEgg * psPacket );
	void											HandlePacket( UserData * pcUserData, PacketNPCRudolphChristmas * psPacket );
	void											HandlePacket( UserData * pcUserData, PacketNPCPuzzle * psPacket );

	//void											EasterEventUnitKilled( UnitData * pcUnitData );

	BOOL											OnUnitKilled( UnitData * pcUnitData, User * pcKiller );

	void											Tick();

	void											ClearTickOnlineReward() { bForceNextOnlineReward = TRUE; }
	void											ClearTickOnlineRewardChristmas() { bResetRudolph = TRUE; }

	BOOL											OnHuntEventMobKilled( UnitData * pcUnitData, User * pcKiller );
	BOOL											SpawnHuntEventUnit( HuntEventMobData* pMoriphData, Map* pcMap, int iX, int iZ );
	BOOL											AppendToKillTracker( User * pcKiller, UnitData * pcUnitData );

	BOOL											GiveWantedMoriphTitles ( );
	BOOL											GiveWantedWolfTitles ( );
	void											StopHuntEvent( );
	void											ResetWantedMoriphEvent( );
	void											ResetWantedWolfEvent( );

	void											LogEventMobDropItem( UnitData* pcUnitData, UserData* pcUserData, Item* psItem );
	BOOL											SpawnHuntEventMobAtRandomSpawnFlag( HuntEventMobData* pMoriphData );
	int												GetTotalSpawnFlagsForHuntEvent( );
	void											CheckAndSpawnHuntEventMobs( );
	BOOL											IsHuntEventActive();


	void											OnSayTime( UnitData * pcUnitData );




};

