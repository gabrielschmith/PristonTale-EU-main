#pragma once

class Map;

struct SpawnDataInfo
{
	struct SpawnData
	{
		int					iID;
		char				szName[32];
		int					iX;
		int					iZ;
	};

	std::vector<SpawnData*>	vSpawnData;

	int						iMapID;

	SpawnDataInfo() {}
	SpawnDataInfo( int _iMapID ) { iMapID = _iMapID; }
	~SpawnDataInfo() {}
};

class MapServer
{

private:



	POINT ptItemSettingPosi[8] =
	{
		//8
		{  0             , -ITEM_SET_DIST }, //mid-top
		{  ITEM_SET_DIST , -ITEM_SET_DIST }, //right-top
		{  ITEM_SET_DIST ,  0 }, //right-mid
		{  ITEM_SET_DIST ,  ITEM_SET_DIST }, //right-bottom
		{  0             ,  ITEM_SET_DIST }, //mid-bottom
		{ -ITEM_SET_DIST ,  ITEM_SET_DIST }, //left-bottom
		{ -ITEM_SET_DIST ,  0 }, //left-mid
		{ -ITEM_SET_DIST , -ITEM_SET_DIST }, //left-right
	};

	POINT ptItemSettingMatrix[8] =
	{
		{ 0, -1 }, //mid-top
		{ 1, -1 }, //right-top
		{ 1,  0 }, //right-mid
		{ 1,  1 }, //right-bottom
		{ 0,  1 }, //mid-bottom
		{-1,  1 }, //left-bottom
		{-1,  0 }, //left-mid
		{-1, -1 }, //left-right
	};



	typedef int( __thiscall *tfnFindEmptyIndexUnitData )(Map * pcMap);

	IMPFNC									pfnAddItemStage			= 0x0054E370;
	IMPFNC									pfnFindIndexUnitData	= 0x0054EFA0;

	IMPFNC									pfnQuestUnitMapHandler  = 0x0055BA00;

	IMPFNC									pfnGetMinuteOfDay		= 0x0054CBC0;

	IMPFNC									pfnGetAngle				= 0x0046EB00;	//GetRadian2D

	BaseMap									* pcaBaseMap;

	std::vector<SpawnDataInfo*>				vSpawnMapsInfo;

	std::map<int, PacketMapIndicators>		mMapIndicators;

	BOOL									bForceNightMode = FALSE;
	BOOL									bForceDayMode = FALSE;

	std::map<int, BOOL>						mMapSkipNextEventSpawn;

public:
	//Constructor
											MapServer();
	virtual									~MapServer();

	BOOL									IsMapSpawnPositionExists( int iMapID, int iX, int iZ );
	void									SendMapIndicators( User * pcUser, int iMapID, BOOL bForceSend = FALSE );
	BOOL									RemoveMapIndicatorNearMe( User * pcUser );
	BOOL									AddMapIndicator( User * pcUser, int iMapID, MapIndicator::Type eMapIndicatorType, int iValue, int iCompactPosX, int iCompactPosY, int iAngleY );

	void									SQLBuildMapIndicators( );
	void									AddMapSpawnData( int iMapID, const char * pszName, int iID, int iX, int iZ );
	void									DelMapSpawnData( int iMapID, int iX, int iZ );
	SpawnDataInfo::SpawnData				* GetMapSpawnData( int iMapID, int iX, int iZ );

	static UINT								GetMinuteOfDay();

	static BOOL								HandleUnitQuestMap( void * pcMap, UnitData * pcUnitData );

	void									SetFlagPoint( UserData * pcUserData, const char * pszDescription );
	void									DeleteFlagPoint( UserData * pcUserData, PacketDeleteFlagPoint * psPacket );

	void									SendHealedAmountToNearbyPlayers( User * pcHealer, User * pcTarget, int iHealValue );
	void									SendUserReceivedDamageInfoTextToNearbyPlayers( UserData * pcUserData, PacketDebugDamageInfo * psPacket );
	void									SendUnitDamageInfoTextToNearbyPlayers( UserData * pcUserData, UnitData * pcUnitData, int iDamage, BOOL bCrit, short sParameter = 0, EDamageTextType iTypeActionOverride = EDamageTextType::Undefined );

	BOOL									AddMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DelMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DisableMonsterSpawn( Map * pcMap, UnitData * pcUnitData );
	BOOL									DelItem( Map * pcMap, Loot * psItem );

	void									CustomReadStageData();
	void									Load();

	BOOL									IsNight() { return (*(BOOL*)0x07AC5654); }
	void									IsNight(BOOL b) { (*(BOOL*)0x07AC5654) = b; }

	UINT									GetTime() { return (*(UINT*)0x07AAC88C); }
	void									SetTime(UINT i) { (*(UINT*)0x07AAC88C) = i; }

	BOOL									IsPVPMap( int iMapID );
	BOOL									IsEXPMap( int iMapID );
	BOOL									IsBellatraMapArea( int iX, int iY, int iZ );
	BOOL									IsSummonMap( int iMapID );
	BOOL									IsValidMap( Map * pcMap );

	int										GetEmptyIndexUnitData( Map * pcMap );



	int										CollectAllNearbyGold( Map * pcMap, Point3D * psGoldPosition );
	BOOL									SendItemStageNearUsers( Map * pcMap, Loot * psItemStage );
	void									SendItemStageUser( Map * pcMap, User * pcUser, Loot * psItemStage );

	void									SendStageItem( Map * pcMap, User * pcUser );
	void									SendStageSpawn( Map * pcMap, UserData * pcUserData );
	BOOL									SetSpawnMonster( Map * pcMap, UnitData * pcUnitData );
	BOOL									SetStartPosNearChar( Map * pcMap, UnitData * lpChar, int cx, int cy, int cz );

	BOOL									OnThrowItem( User * pcUser, struct PacketThrowItem * psPacket );

	BOOL									UseUnionCore( UserData * pcUserData, struct PacketUseUnionCore * psPacket );

	BOOL									SetBossPosition( Map * pcMap, CharacterData * pcUnitData, Point3D * psPosition );
	BOOL									SpawnMonsterBoss( Map * pcMap, Map::SpawnSetting::Boss * psBoss );

	BOOL									UpdateUnitPosition( UnitData * pcUnitData, int iMapID, int iX, int iY, int iZ );
	BOOL									UpdateUnitAngle( UnitData * pcUnitData, int iMapID, int iAngle);

	UnitData							  * CreateUnit( Map * pcStage, MonsterMapSpawnHandle * pcMonster, int iGroup );

	int										CloseEventMonster( int EventCode );

	void									UpdateMap( Map * pcMap );

	void									Update();
	void									UpdateTime();

	void									ForceDayMode( BOOL bForce );
	void									ForceNightMode( BOOL bForce );

	void									RemoveNPC( Map * pcMap, ID iNpcID );
	void									SpawnNPC( Map * pcMap, PacketUnitInfo * psPacket );
	BOOL									SpawnCrystal( EItemID iItemID, int iX, int iY, int iZ, UserData * pcUserData );
	BOOL									SpawnMonsterBC( Map * pcMap, CharacterData * psCharacterData, int iX, int iY, int iZ );

	UINT									GetWorldTimeHour();

	int										MakeFaceArea( Stage * pcThis, int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom );
	BOOL									SetupPolygonArea( Stage * pcThis );
	int										GetHeightArea( Stage * pcThis, int iAX, int iAZ, int iX, int iZ );

	void									SendSpawnFlagMarkers( User * pcUser, int iMapID );
	void									UpdateUserBossTime( User * pcUser, int iMapID );

	void									UpdateUsersBossTime();

	void									HandlePacket( User * pcUser, PacketBossTimeUpdate * psPacket );

	static int								GetAngle( int iX, int iZ, int iTargetX, int iTargetZ );

	static Loot								* FindItem( Map * pcMap, int iX, int iY, int iZ );
	Loot									* AddItemV2( Map * pcMap, LootItem * psLootItem, int iX, int iY, int iZ, enum class ELootType eLootType );
	void									AddMassGoldDrops (  Map * pcMap, int x, int y, int z, int iGoldMin, int iGoldMax, int iDropCount, User * pcUser );

	static EMapID							* piBabelStormMapID;
	static DWORD							* pdwBabelStormEndTime;

	//Shift the time of day (useful if you want to start SoD now!!)
	int										iGameTimeShift;
};

