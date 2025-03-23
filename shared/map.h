#pragma once

#include "character.h"
#include "unit.h"
#include "item.h"


#define NUM_MAPS							67

#define MAPLEVEL							GetMapLevel

#if defined(_GAME)
#define MAX_BORDERMAP						12
#else
#define MAX_BORDERMAP						8
#endif

#define MAX_AMBIENTMAP						80
#define MAX_OBJECTMAP						50
#define MAX_SPAWNMAP						8

#define MAX_SPAWNFLAGS			200
#define MAX_NPCINMAP			100
#define MAX_MONSTERINMAP		50
#define MAX_BOSSINMAP			16
#define MAX_BOSSHOURS			32
#define MAX_LOOT				1024 //STG_ITEM_MAX
#define MAX_ALIVEMONSTERS		1024


class BaseMap;

struct Item;

struct CharacterData;
class UnitData;





enum EMapID : int
{
	MAPID_Invalid							= -1,
	MAPID_AcasiaForest						= 0,
	MAPID_BambooForest						= 1,
	MAPID_GardenOfFreedom					= 2,
	MAPID_RicartenTown						= 3,
	MAPID_RefugeOfTheAncients				= 4,
	MAPID_CastleOfTheLost					= 5,
	MAPID_RuinenVillage						= 6,
	MAPID_CursedLand						= 7,
	MAPID_ForgottenLand						= 8,
	MAPID_NaviskoTown						= 9,
	MAPID_Oasis								= 10,
	MAPID_AncientsBattlefield				= 11,
	MAPID_ForbiddenLand						= 12,
	MAPID_AncientPrisonF1					= 13,
	MAPID_AncientPrisonF2					= 14,
	MAPID_AncientPrisonF3					= 15,
	MAPID_ChessRoom							= 16,
	MAPID_ForestOfSpirits					= 17,
	MAPID_LandOfDusk						= 18,
	MAPID_ValleyOfTranquility				= 19,
	MAPID_RoadToTheWind						= 20,
	MAPID_PillaiTown						= 21,
	MAPID_CursedTempleF1					= 22,
	MAPID_CursedTempleF2					= 23,
	MAPID_MushroomCave						= 24,
	MAPID_BeehiveCave						= 25,
	MAPID_DarkSanctuary						= 26,
	MAPID_RailwayOfChaos					= 27,
	MAPID_HeartOfPerum						= 28,
	MAPID_Eura								= 29,
	MAPID_Bellatra							= 30,
	MAPID_GallubiaValley					= 31,
	MAPID_QuestArena						= 32,
	MAPID_BlessCastle						= 33,
	MAPID_GreedyLake						= 34,
	MAPID_FrozenSanctuary					= 35,
	MAPID_KelvezuLair						= 36,
	MAPID_LandOfChaos						= 37,
	MAPID_LostTemple						= 38,
	MAPID_GhostCastle						= 39,
	MAPID_EndlessTowerF1					= 40,
	MAPID_EndlessTowerF2					= 41,
	MAPID_CursedTempleF3					= 42,
	MAPID_EndlessTowerF3					= 43,
	MAPID_IceMineF1							= 44,
	MAPID_Atlantis							= 45,
	MAPID_MysteryForest1					= 46,
	MAPID_MysteryForest2					= 47,
	MAPID_MysteryForest3					= 48,
	MAPID_BattleTown						= 49,
	MAPID_MysteryDesert3					= 50,
	MAPID_MysteryDesert2					= 51,
	MAPID_MysteryDesert1					= 52,
	MAPID_ForgottenTempleF1					= 53,
	MAPID_ForgottenTempleF2					= 54,
	MAPID_AncientDungeonF1					= 55,
	MAPID_AncientDungeonF2					= 56,
	MAPID_AncientDungeonF3					= 57,
	MAPID_AncientWeapon						= 58,
	MAPID_SeaOfAbyss1						= 59,
	MAPID_T5QuestArena						= 60,
	MAPID_SecretLaboratory					= 61,
	MAPID_SeaOfAbyss2						= 62,
	MAPID_HearOfFire						= 63,
	MAPID_WindForest1						= 64,
	MAPID_WindForest2						= 65,
	MAPID_SecretLab2						= 66,
};

//Season 4 Preperations
/*
	MAPID_WindForest1						= 63,
	MAPID_WindForest2						= 64,
	MAPID_DungeonTest						= 65,
*/

//used by quest book
static EMapID peOrderedMapIDs[NUM_MAPS + 1]
{
	MAPID_Invalid				,

	//Towns / misc
	MAPID_RicartenTown			,
	MAPID_PillaiTown			,
	MAPID_NaviskoTown			,
	MAPID_Bellatra				,
	MAPID_Eura					,
	MAPID_Atlantis				,
	MAPID_BattleTown			,
	MAPID_ChessRoom				,
	MAPID_QuestArena			,
	MAPID_T5QuestArena			,
	MAPID_BlessCastle			,
	MAPID_GhostCastle			,


	MAPID_HearOfFire			, //113
	MAPID_SeaOfAbyss2			, //113
	MAPID_SeaOfAbyss1			, //113
	MAPID_AncientWeapon			, //110
	MAPID_SecretLaboratory		, //108
	MAPID_IceMineF1				, //105
	MAPID_EndlessTowerF3		, //102
	MAPID_EndlessTowerF2		, //100
	MAPID_EndlessTowerF1		, //100
	MAPID_LostTemple			, //100
	MAPID_LandOfChaos			, //95
	MAPID_KelvezuLair			, //90
	MAPID_FrozenSanctuary		, //90
	MAPID_GallubiaValley		, //90
	MAPID_HeartOfPerum			, //85
	MAPID_RailwayOfChaos		, //80
	MAPID_CursedTempleF2		, //75
	MAPID_GreedyLake			, //70
	MAPID_CursedTempleF1		, //70
	MAPID_DarkSanctuary			, //65
	MAPID_AncientPrisonF3		, //60
	MAPID_BeehiveCave			, //55
	MAPID_MushroomCave			, //55
	MAPID_ForbiddenLand			, //55
	MAPID_AncientsBattlefield	, //50
	MAPID_AncientPrisonF2		, //45
	MAPID_AncientPrisonF1		, //40


	MAPID_Oasis					, //~35
	MAPID_ForgottenLand			, //~30
	MAPID_CursedLand			, //~25
	MAPID_RuinenVillage			, //~20

	//Tempskron path
	MAPID_CastleOfTheLost		, //~15
	MAPID_RefugeOfTheAncients	, //~15
	MAPID_AcasiaForest			, //~10
	MAPID_BambooForest			, //~5
	MAPID_GardenOfFreedom		, //~1

	//Morion path
	MAPID_ForestOfSpirits		, //~15
	MAPID_LandOfDusk			, //~10
	MAPID_ValleyOfTranquility	, //~5
	MAPID_RoadToTheWind			, //~1


	//other maps (unused)
	MAPID_MysteryForest1		,
	MAPID_MysteryForest2		,
	MAPID_MysteryForest3		,
	MAPID_MysteryDesert3		,
	MAPID_MysteryDesert2		,
	MAPID_MysteryDesert1		,
	MAPID_ForgottenTempleF1		,
	MAPID_ForgottenTempleF2		,
	MAPID_CursedTempleF3		,
	MAPID_AncientDungeonF1		,
	MAPID_AncientDungeonF2		,
	MAPID_AncientDungeonF3		,

	MAPID_WindForest1			,
	MAPID_WindForest2			,

};

static const char * pszaMapsName[NUM_MAPS]
{
	"Acasia Forest",						//0
    "Bamboo Forest",						//1
    "Garden of Freedom",					//2
    "Ricarten Town",						//3
    "Refuge of the Ancients",				//4
    "Castle of the Lost",					//5
    "Ruinen Village",						//6
    "Cursed Land",							//7
    "Forgotten Land",						//8
    "Navisko Town",							//9
    "Oasis",								//10
    "Battlefield of the Ancients",			//11
    "Forbidden Land",						//12
    "Ancient Prison 1F",					//13
    "Ancient Prison 2F",					//14
    "Ancient Prison 3F",					//15
    "Checker (GM) Room",					//16
    "Forest of Spirits",					//17
    "Land of Dusk",							//18
    "Valley of Tranquility",				//19
    "Road to the Wind",						//20
    "Pillia Town",							//21
    "Cursed Temple 1F",						//22
    "Cursed Temple 2F",						//23
    "Mushroom Cave",						//24
    "Beehive Cave",							//25
    "Sanctuary of Darkness",				//26
    "Railway of Chaos",						//27
    "Heart of Perum",						//28
    "Eura",									//29
    "Bellatra Celestial",					//30
    "Gallubia Valley",						//31
    "Tier 4 Arena",							//32
    "Bless Castle",							//33
    "Greedy Lake",							//34
    "Frozen Temple",						//35
    "Fire Map",								//36
    "Lost Island",							//37
    "Lost Temple",							//38
    "GM Test Map",							//39
	"Endless Tower 1F",						//40
	"Endless Tower 2F",						//41
	"Cursed Temple 3F",						//42
	"Endless Tower 3F",						//43
	"Ice Mine 1F",							//44
	"Atlantis Town",						//45
	"Mystery Forest 1",						//46
	"Mystery Forest 2",						//47
	"Mystery Forest 3",						//48
	"Battle Town",							//49
	"Mystery Desert 3",						//50
	"Mystery Desert 2",						//51
	"Mystery Desert 1",						//52
	"Forgotten Temple 1",					//53
	"Forgotten Temple 2",					//54
	"Ancient Dungeon 1",					//55
	"Ancient Dungeon 2",					//56
	"Ancient Dungeon 3",					//57
	"Anciient Weapon",						//58
	"Abyss of the Sea",						//59
	"T5 Quest Arena",						//60
	"Secret Laboratory",					//61
	"Sea Of Abyss 2",						//62
	"Heart of Fire",						//63
	"WindForest1",							//64
	"WindForest2",							//65
	"Secret Laboratory 2"					//66
};

//Season 4 preperations
/*
	"WindForest1",
	"WindForest2",
	"Dungeon",
	//"Coliseum",							//62
*/

static const int iaMapsLevel[NUM_MAPS]
{
	1,										//0
	1,										//1
	1,										//2
	1,										//3
	1,										//4
	1,										//5
	1,                                      //6
	1,                                      //7
	1,                                      //8
	1,                                      //9
	1,                                      //10
	50,                                     //11
	55,                                     //12
	40,                                     //13
	45,                                     //14
	60,                                     //15
	1,                                      //16q
	1,                                      //17
	1,                                      //18
	1,                                      //19
	1,                                      //20
	1,                                      //21
	70,                                     //22
	75,                                     //23
	55,                                     //24
	55,                                     //25
	60,                                     //26
	80,                                     //27 - Railway of Chaos
	84,                                     //28 - Heart of Perum
	88,                                     //29 - Ice1
	40,                                     //30 - Bellatra
	88,                                     //31 - Gallubia Valley (Ice1)
	1,										//32 - Quest Map
	39,                                     //33 - Bless Castle
	65,										//34 - Greedy Lake
	92,                                     //35 - Ice2
	92,									    //36 - Kelvezu Cave
	95,									    //37 - Land of Chaos
	98,										//38 - Lost Temple
	100,									//39 - Ghost Castle
	100,									//40 - Endless Tower 1
	102,									//41 - Endless Tower 2
	150,									//42 - Cursed Temple 3
	150,									//43 - Endless Tower 3
	105,									//44 - Ice Mine 1
	100,                                    //45 - Atlantis Town
	150,									//46 - Mystery Forest 1
	150,									//47 - Mystery Forest 2
	150,									//48 - Mystery Forest 3
	100,									//49 - Battle Town
	150,									//50 - Mystery Desert 3
	150,									//51 - Mystery Desert 2
	150,                                    //52 - Mystery Desert 1
	150,									//53 - Forgotten Temple 1
	150,									//54 - Forgotten Temple 2
	150,									//55 - Ancient Dungeon 1
	150,									//56 - Ancient Dungeon 2
	150,									//57 - Ancient Dungeon 3
	110,									//58 - Ancient Weapon
	113,									//59 - Abyss of the Sea
	150,									//60 - T5 Quest Arena
	108,									//61 - Secret Laboratory
	115,									//62 - Sea Of Abyss 2
	113,									//63 - Heart of Fire
	1,										//64
	1,										//65
	108,									//66
};

enum EMapType
{
	MAPTYPE_Town							= 0x100,
	MAPTYPE_Forest							= 0x200,
	MAPTYPE_Desert							= 0x300,
	MAPTYPE_Wasteland						= 0x400,
	MAPTYPE_Dungeon							= 0x500,
	MAPTYPE_Iron							= 0x600,
	MAPTYPE_Unused_1						= 0x700,
	MAPTYPE_ChessRoom						= 0x800,
	MAPTYPE_Ice								= 0x900,
	MAPTYPE_BlessCastle						= 0xA00,
	MAPTYPE_Distorted						= 0xB00,
	MAPTYPE_BattleTown						= 0xC00
};

enum class ELootType : int
{
	Unknown = -1,
	None = 0,
	Initial = 1,
	Exclusive = 2
};

enum ETeleportType : unsigned int
{
	TELEPORTTPE_Direct						= 0,
	TELEPORTTYPE_Warp						= 1,
	TELEPORTTYPE_WarpGate					= 2,
};

struct MapBorder
{
	int										iX;
	int										iZ;
	int										iY;
	BaseMap									* pcMap;
};

struct Teleport
{
	int										iX;
	int										iZ;
	int										iY;
	int										iHeight;
	int										iSize;
	MapBorder								saOutFieldBorder[MAX_BORDERMAP];
	int										iOutFieldCount;
	int										iLevel;
	int										iEffect;
};

struct MapPosition
{
	int										iX;
	int										iY;
	int										iZ;
	int										iRound;
	int										iNum;
};

struct FlatMap
{
	Point3D									sPosition;
	int										iLeftX;
	int										iRightX;
};

//class sFIELD
class BaseMap
{
public:
	void									* pvVoid;			//Virtual Table? [Pointer always NULL]
	char									szStageFilePath[64];
	char									szMiniMapFilePath[64];
	char									szMiniMapNameFilePath[64];

	EMapType								iMapType;
	int										iaSky[3];
	EBackgroundMusicID						iBackgroundMusicID;

	BOOL									bNightSpawn;

	int										iBorderCount;
	MapBorder								saFieldBorder[MAX_BORDERMAP];

	int										iWarpGateCount;
	int										iWarpGateCurrent;
	Teleport								saWarpGate[MAX_BORDERMAP];
	Point3D									sWarpGatePosition;

	MapPosition								saAmbientPosition[MAX_AMBIENTMAP];
	int										iAmbientCount;

	int										iLevel;
	int										iSight;

#if defined(_GAME)
	char									szaObjectName[MAX_OBJECTMAP][64];
#else
	char								  * szaObjectName[MAX_OBJECTMAP];
#endif

	DWORD									dwaObjectBip[MAX_OBJECTMAP];
	int										iObjectCount;

	Point2D									sPositionCenter;

	EMapID									iMapID;
	int										iUnused;

	Point2D									saSpawnPoint[MAX_SPAWNMAP];
	int										iSpawnCount;

#if defined(_GAME)
	FlatMap									sActionCamera;
#endif

public:
	int										GetID(){return iMapID;}

	void									SetID( EMapID i ) { iMapID = i; }
	void									SetFileName( char * pszName, char * pszTitle );
	void									SetType( EMapType i ) { iMapType = i; }
	void									SetSky( int iMorning, int iEvening, int iNight ) { iaSky[0] = iMorning; iaSky[1] = iEvening; iaSky[2] = iNight; }
	void									SetBackgroundMusicID( EBackgroundMusicID i ) { iBackgroundMusicID = i; }
	void									SetLevelRequirement( int i ) { iLevel = i; }
	void									SetNightSpawn( BOOL b ){ bNightSpawn = b; }
	void									SetCenter( int iCX, int iCZ ) { sPositionCenter.iX = iCX; sPositionCenter.iY = iCZ; }
	void									AddSpawn( int iX, int iZ ){saSpawnPoint[iSpawnCount].iX = iX; saSpawnPoint[iSpawnCount].iY = iZ; iSpawnCount++;}
	void									AddBorder( class BaseMap * pcMap, int iX, int iZ, int iY = 0 );
	void									AddTeleport1( int iX, int iZ, int iY, int iSize, int iHeight, class BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iLevel = 1, int iWarpType = 0 );
	void									AddTeleport2( int iX, int iZ, int iY, int iSize, int iHeight, class BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iXMap2, int iZMap2, int iYMap2, int iLevel = 1, int iWarpType = 0 );
	void									AddSound( int iX, int iY, int iZ, int iRound, int iCode );
	void									AddStaticModel( char * pszName, int iAnimationBip = 0 );
	void									AddModelEx( char * pszFormat, ... );
	void									SetCamera( int iX, int iY, int iZ, int iLeftX, int iRightX );

	Point2D									& GetCenter() { return sPositionCenter; }
};

static int GetMapLevel( int i ) { return iaMapsLevel[i]; };


struct StageVertex
{
	DWORD				dwSum;

	void				* pvVertexRender;

	Point3D				sPosition;

	short				saColor[4];
};

struct StageFace
{
	DWORD				dwSum;
	int					iCalcSum;

	WORD				wVertex[4];

	void				* pTexture;

	short				sNormal[4];
};

struct StageTexture
{
	float			u[3];
	float			v[3];

	void			* pTexture;

	void			* pNextTexture;
};

//struct psITEM
struct LootItem
{
	BOOL		  bUse;
	DWORD		  dwReserved1;
	Item		  sItem;
}; //Size = 0x21C

   //struct STG_ITEMS
struct Loot
{
	BOOL			  bInUse;
	Point3D			  sPosition;



	LootItem		  sLootItem;

	DWORD			  dwDropTime;		 //dwCreateTime
	DWORD			  dwEndTime;		 //dwLoseTime
	union
	{
		int			  Level;
		ID			  lExclusiveOwnerID;
	};
	ELootType		eLootType;
};
//Size = 0x23C

//class smSTAGE3D
class Stage
{
private:
#ifdef _GAME
	IMPFNC pfnConstructor = 0x00485680;
	IMPFNC pfnDestructor = 0x004887B0;
	IMPFNC pfnGetHighestPoint = 0x00486780;
	IMPFNC pfnGetHighestPointEx = 0x00487F20;
	IMPFNC pfnSaveToFile = 0x004876D0;
	IMPFNC pfnLoadFromFile = 0x004878B0;
	IMPFNC pfnDrawMap = 0x004871D0;
	IMPFNC pfnLoadStage = 0x0047DC00;
	IMPFNC pfnFindItem = 0x0000000;

#else
	IMPFNC pfnConstructor = 0x00468480;
	IMPFNC pfnDestructor = 0x0046AF50;
	IMPFNC pfnGetHighestPoint = 0x004694D0;
	IMPFNC pfnGetHighestPointEx = 0x0046A960;	//GetFloorHeight
	IMPFNC pfnSaveToFile = 0x0046A160;
	IMPFNC pfnLoadFromFile = 0x0046A320;
	IMPFNC pfnDrawMap = 0x00000000;
	IMPFNC pfnLoadStage = 0x00000000;
	IMPFNC pfnFindItem = 0x0054E310;
#endif

public:
	//Attributes
	BOOL					bState;				//TRUE if Stage is Loaded, otherwise FALSE

	DWORD					* pdwaArea[256][256];
	Point2D					* psaAreaList;
	int						iAreaCount;

	int						iUnknown001;

	DWORD					dwSumCount;
	int						iCalcSum;

	StageVertex				* pVertex;
	StageFace				* pFace;
	StageTexture			* pTexture;
	void					* pLight;

	void					* pMaterial;

	void					* pStageObject;

	void					* pMaterialData;

	int						iVertexCount;
	int						iFaceCount;
	int						iTextureCount;
	int						iLightCount;

	int						iVertexColorCount;

	int						iContrast;
	int						iBright;

	Point3D					sVectorLight;

	DWORD					* pdwAreaBuffer;
	int						iAreaBufferSize;

	int						iWest;
	int						iSouth;
	int						iEast;
	int						iNorth;

	//Operations
	Stage();
	~Stage();

	/**
	* Retrieves the highest 3D Y value in the Stage at the provided 2D coordinate
	* @param iX the 2D X coordinate
	* @param iZ the 2D Y coordinate
	* @return the highest 3D Y value at the provided 2D coordinate
	*/
	int					  GetHighestPoint( int iX, int iZ );
	int					  GetHighestPoint( int iX, int iY, int iZ, int iMeshHeight );

	Loot *				  FindItem( int iX, int iY, int iZ );

	/**
	* Saves Stage to File
	* @param pszFilePath the File Path to Save the Stage
	* @return TRUE if saved
	*/
	BOOL				  SaveToFile( const char * pszFilePath );

	/**
	* Loads Stage from File
	* @param pszFilePath the File Path to Load the Stage from
	* @return TRUE if loaded
	*/
	BOOL				  LoadFromFile( const char * pszFilePath );

	void				  Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ, EXEMatrixE * sRotationMatrix = NULL );

	static Stage		  * LoadStage( char * pszFileName, int i = 0 );

}; //Size = 0x40074


//class STG_AREA (field.h)
class Map
{
private:
	IMPFNC				pfnAddItem			= 0x0054E370;	//AddItem
	IMPFNC				pfnCreateMonster	= 0x08B8100C;
	IMPFNC				pfnAddUnitData		= 0x0054EFC0;   //will call BOOL __stdcall AddMonsterSpawn( UnitData * pcUnitData ) in DLL.cpp

public:
	void				* p;

	Stage				* pcStage;   //lpStage
	BaseMap				* pcBaseMap; //lpField

	char				szSpawnFlagFilePath[64];
	char				szNPCFilePath[64];
	char				szMonsterFilePath[64];

	struct SpawnFlag
	{
		int											bActive;
		int											iX;
		int											iZ;
	};


	SpawnFlag			saSpawnFlag[MAX_SPAWNFLAGS];
	BOOL				baSpawnFlagActive[MAX_SPAWNFLAGS];
	int					iaSpawnMonsterActive[MAX_SPAWNFLAGS];
	int					iNumActiveSpawns;
	int					iLastActiveSpawn;
	DWORD				dwaTimeLastMonsterSpawned[MAX_SPAWNFLAGS];

	int					iNumSpawnFlag;
	int					iNumActiveSpawnFlag;



	Loot				saLoot[MAX_LOOT];

	void				* pvUnused;

	PacketUnitInfo		saNPC[MAX_NPCINMAP];

	//rsSTG_MONSTER_LIST
	struct SpawnSetting
	{
		//rsSTG_MONSTER
		struct Monster
		{
			char			  szMonsterName[32];
			int				  iSpawnRate;
			CharacterData	* cd;
			int				  iSpawnRateCum;
		}; //Size = 0x2C

		Monster			  saMonster[MAX_MONSTERINMAP];	//0x000 0x898

		UINT			  iTotalMonsterSpawnRate;		//0x898
		UINT			  iTotalMonsters;				//0x89C
		int				  iSpawnMaxMonsters;			//0x8A0
		int				  iSpawnIntervalMin;			//0x8A4
		int				  iSpawnMonsters;				//0x8A8
		int				  iSpawnIntervalMax;			//0x8AC

		struct Boss
		{
			CharacterData	* cdBoss;
			CharacterData	* cdMinion;
			int				  iMinions;
			char			  caBossHour[MAX_BOSSHOURS];
			int				  iBossHours;
		}; //Size = 0x30

		Boss			  saBoss[MAX_BOSSINMAP];			//0x8B0 0x300
		int				  iTotalBosses;						//0xBB0
	}; //Size = 0x0BB4

	SpawnSetting		  sSpawnSetting;					//0x9C880 (rsSTG_MONSTER)


	int					  StgCharInfoCnt;

	int					  SpawnCount;
	short				  sBossSubsSpawnCount;				// for Boss minions
	short				  sIsBossMinions;

	UINT				  uCounter;							//0x9D440 - Counter

	BOOL				  bNight;							//0x9D444 - FieldNightDay		    - Variables to detect changes in nighttime

	UnitData			* pcaUnitData[MAX_ALIVEMONSTERS];	//0x9D448 - *lpCharMonster[STG_MONSTER_MAX]; - Advance Monster Connection Table

	int					  iNumAliveUnitData;				//0x9D848 - MonsterCount			- Monster Advent Counter
	int					  iNumUsingUnitData;				//0x9D84C - MonsterLastPoint		- Monster Table Last Point

	BOOL				  bForceCreateMonster;				//0x9D850 - EvnMonster				- Monster temporarily exceeded

	DWORD				  Event_dwMonsterTime;				//0x9D854 - Event_dwMonsterTime		- Event Monster Setup Time
	DWORD				  dwUnused385123329;				//0x9D858 - Event_lpChrMonster		- Event Monster Pointer
	DWORD				  dwUnused852931293;				//0x9D85C - Event_MonsterPercentage - Event Monster Probability

	int					  iState;							//0x9D860 - dwActiveMode

	BOOL				DeleteItem( Loot * psLootItem );

	UnitData			* CreateMonster( CharacterData * psCharacterData, int iCount );

	void				AddUnitData( UnitData * pcUnitData );

};

static int aiTowns[]
{
	MAPID_RicartenTown,
	MAPID_PillaiTown,
	MAPID_Atlantis
};

static int aiTownsEx[]
{
	MAPID_RicartenTown,
	MAPID_PillaiTown,
	MAPID_Eura,
	MAPID_NaviskoTown,
	MAPID_Atlantis
};


static int aStageCity[]
{
	MAPID_RicartenTown,
	MAPID_PillaiTown,
	MAPID_Eura,
	MAPID_NaviskoTown,
	MAPID_BattleTown,
	MAPID_Atlantis
};
static int aStageBattle[]
{
	MAPID_BlessCastle,
};


struct PacketBossTimeUpdate : Packet
{
	char				szBossName[32];
	DWORD				dwTime;
	int					iMapID;

	PacketBossTimeUpdate() {};
	PacketBossTimeUpdate( int _iMapID ) { iMapID = _iMapID; };

	~PacketBossTimeUpdate() {};
};

#define MAX_SPAWN_MARKERS		200	//Per map
struct PacketMapSpawnMarkers : Packet
{
	int iCount;
	Point3D saMarkers[MAX_SPAWN_MARKERS];
};
