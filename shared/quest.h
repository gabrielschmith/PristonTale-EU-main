#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <vector>

struct IMinMax;

#define QUEST_MONSTERITEM_MAX				10
#define QUEST_MONSTERID_ANY					9999

#define VAMPBEE_MONSTER_ID		1107
#define QUESTWEAPON_TARGET_AGE	3
#define QUESTWEAPON_MAX_AGE		5

enum class EQuestUserStatus
{
	NotStarted,
	InProgress,
	Expired,    //Quest is finished but expired
	Completed,  //Completed quest but not yet handed in
	Done,	    //Finished quest and handed in
	Error,
};

enum EAsmQuestBit : DWORD
{
	Level_30	 = 1 << 0, //1		QUESTBIT_LEVEL_30		For Her
	Level_55	 = 1 << 1, //2		QUESTBIT_LEVEL_55		The Cave
	Level_70	 = 1 << 2, //4		QUESTBIT_LEVEL_70		Friendship Mitchell
	Level_80	 = 1 << 3, //8		QUESTBIT_LEVEL_80		Sealed Fury
	Level_85	 = 1 << 4, //16		QUESTBIT_LEVEL_85		Tears of Kalia
	Level_90	 = 1 << 5, //32		QUESTBIT_LEVEL_90		Eura Village
	Level_80_2	 = 1 << 6, //64		QUESTBIT_LEVEL_80_2		Test of Kingdom
	Level_90_2	 = 1 << 7, //128	QUESTBIT_LEVEL_90_2		Bitter Ordeal
};

enum EAsmQuestCode : DWORD
{
	ChangeJob1		= 0x0001, //1	SIN_QUEST_CODE_CHANGEJOB
	ChangeJob2_M	= 0x0002, //2	SIN_QUEST_CODE_CHANGEJOB2_NPC_M
	ChangeJob2_D	= 0x0003, //3	SIN_QUEST_CODE_CHANGEJOB2_NPC_D
	ChangeJob3		= 0x0004, //4	SIN_QUEST_CODE_CHANGEJOB3
	ChangeJob4		= 0x0010, //16	SIN_QUEST_CODE_CHANGEJOB4

	Level30			= 0x0005, //5	SIN_QUEST_CODE_LEVEL30
	Level55			= 0x0006, //6	SIN_QUEST_CODE_LEVEL55
	Level55_2		= 0x0007, //7	SIN_QUEST_CODE_LEVEL55_2
	Level70			= 0x0008, //8	SIN_QUEST_CODE_LEVEL70
	Level80			= 0x0009, //9	SIN_QUEST_CODE_LEVEL80
	Level85			= 0x000A, //10	SIN_QUEST_CODE_LEVEL85
	Level90			= 0x000B, //11	SIN_QUEST_CODE_LEVEL90 (eura village)
	Level80_2		= 0x000C, //12	SIN_QUEST_CODE_LEVEL80_2
	Level90_2		= 0x000D, //13	SIN_QUEST_CODE_LEVEL90_2 (3 extra stats per lvl)
};

enum class EQuestIcon : BYTE
{
	None        = 0,
	Avaliable   = 1, //yellow ! (quest avaliable)
	Progress    = 2, //gray ? (quest in progress)
	Complete    = 3, //yellow ? (pending complete)
	Unavaliable = 4, //gray ! (unable to do quest)
	AvaliableRepeatable = 5,  //blue ! (repeatable quest avaliable)
	CompleteRepeatable = 6,  //blue ? (repeatable quest avaliable)
};

enum class EQuestPartyType : BYTE
{
	SoloOnly		 = 0,
	PartyOnly		 = 1,
	SoloAndParty     = 2,
	PartyAndRaid     = 3,
	SoloPartyAndRaid = 4,
	RaidOnly		 = 5,
	SoloAny			 = 6,
	SoloPartyAny	 = 7,
};

enum class EQuestType : BYTE
{
	None					= 0,
	SoloOneOff				= 1,
	PartyOneOff				= 2,
	Daily					= 3,
	DailyMidNight			= 4,
	DailyWeek				= 5,
	DailyMonth				= 6,
	Repeatable				= 7,
	Weekend					= 8,
	WeekendDaily			= 9,
	WeekendDailyMidNight	= 10,
	WeekendRepeatable		= 11,
	WeekFriday				= 12,
};

enum EQuestDurationType
{
	QUESTDURATIONTYPE_None,
	QUESTDURATIONTYPE_Running,
	QUESTDURATIONTYPE_Amount,
};

enum EQuestAreaType
{
	QUESTAREATYPE_None,
	QUESTAREATYPE_Bound,
	QUESTAREATYPE_Radius,

};

/// <summary>
/// See QuestRewardList > ExtraRewardType
/// </summary>
enum class EQuestExtraRewardType : int
{
	None			= 0,
	Gold			= 1,
	WarehousePage	= 2,
	EXPPotBuff		= 3,
	ThirdEyeBuff	= 4,
	HPUpBuff		= 5,
	MPUpBuff		= 6,
	StatPoint		= 7, //TBD
	SkillPoint		= 8, //TBD
	StatPointPerLvl	= 9, //TBD
	DecreaseDeathPlenty	= 10,
	ExtraHealth		= 11,
	RankUp			= 12,
	QuestWeapon		= 13,
	SixthSenseBuff	= 14,
};

enum EQuestID
{
	QUESTID_None,

	QUESTID_General					= 1500,
	QUESTID_Warehouse2Page			= 1501,


	QUESTID_Warpgate_1_Metal				= 2100,
	QUESTID_Warpgate_2_Silver				= 2101,
	QUESTID_Warpgate_3_Gold					= 2102,
	QUESTID_Warpgate_4_Diamond				= 2103,
	QUESTID_Warpgate_5_Chaos				= 2104,
	QUESTID_Warpgate_6_Imperial				= 2105,

	QUESTID_ASM_30_ForHer					= 4000,
	QUESTID_ASM_55_TheCave					= 4010,
	QUESTID_ASM_70_FriendShipWithMichell	= 4020,
	QUESTID_ASM_80_SealedFury				= 4030,
	QUESTID_ASM_80_TestOfKingdom_1			= 4040,
	QUESTID_ASM_80_TestOfKingdom_2			= 4041,
	QUESTID_ASM_80_TestOfKingdom_3			= 4042,
	QUESTID_ASM_80_TestOfKingdom_4			= 4043,
	QUESTID_ASM_80_TestOfKingdom_5			= 4044,
	QUESTID_ASM_85_TearsOfKalia				= 4050,
	QUESTID_ASM_90_EuraVillage				= 4060,
	QUESTID_ASM_90_BitterOrdeal_1			= 4070,
	QUESTID_ASM_90_BitterOrdeal_2			= 4071,
	QUESTID_ASM_90_BitterOrdeal_3			= 4072,


	QUESTID_Rankup_2_Tempskron				= 5000,
	QUESTID_Rankup_2_Morion_2				= 5021,
	QUESTID_Rankup_2_Morion_3				= 5022,
	QUESTID_Rankup_2_Morion_4				= 5023,
	QUESTID_Rankup_3_Tempskron_Vamp			= 5040,
	QUESTID_Rankup_3_Tempskron				= 5041,
	QUESTID_Rankup_3_Morion_Vamp			= 5050,
	QUESTID_Rankup_3_Morion					= 5051,
	QUESTID_Rankup_4_Tempskron				= 5060,
	QUESTID_Rankup_4_Morion					= 5070,

	//Normal Quests
	QUESTID_BlacksmithGodenGold		= 10001,
	QUESTID_WarehouseGold			= 10002,
	QUESTID_Quest110A				= 10003,
	QUESTID_AnAncientThreat			= 10004,
	QUESTID_AnEndlessBattle			= 10005,
	QUESTID_AnEndlessSuffering		= 10006,
	QUESTID_ATestOfBravery			= 10007,
	QUESTID_GreenLiquidQuestStep1	= 10008,
	QUESTID_GreenLiquidQuestStep2	= 10009,




	//T5 Rankup
	QUESTID_AMatterOfStrenght		= 10010,
	QUESTID_CryMeARiver				= 10011,
	QUESTID_TheLastStage			= 10012,
	QUESTID_T5QuestArena			= 10013,


	//Secret Laboratory Quests
	QUESTID_TheSecretRevealed		= 10014,


	//Abyss Quests
	QUESTID_TheEliteOfFear			= 10014,
	QUESTID_TheReturnOfMutants		= 10015,
	QUESTID_TheAbyssHunter			= 10016,
	QUESTID_ATestofLoneliness		= 10017,
	QUESTID_UndertheAbyss			= 10018,

	//Ice Mine Quests
	QUESTID_MinersFearsEasy			= 10019,
	QUESTID_MinersFearsMedium		= 10020,
	QUESTID_MinersFearsHard			= 10021,
	QUESTID_TullaMenace				= 10022,
	QUESTID_MineAlone				= 10023,

	QUESTID_DailyQuestsIan			= 11001,
	QUESTID_DailyQuest80A			= 11002,
	QUESTID_DailyQuest85A			= 11003,
	QUESTID_DailyQuest90A			= 11004,
	QUESTID_DailyQuest95A			= 11005,
	QUESTID_DailyQuest100A			= 11006,
	QUESTID_DailyQuest105A			= 11007,
	QUESTID_DailyQuest110A			= 11009,
	QUESTID_DailyQuest115A			= 11010,
	QUESTID_DailyQuestLimit			= 11050,

	//CoinShop
	QUESTID_CoinShopNPC				= 15000,
	QUESTID_EasterNPC				= 15001,
	QUESTID_RudolphNPC				= 15002,
};


enum EQuestMonsterID
{
	QUESTMONSTERID_Ratoo		= 10,


	QUESTMONSTERID_BronzeWolverine		= 1191, //Tail
	QUESTMONSTERID_SilverWolverine		= 1192, //Claw
	QUESTMONSTERID_GoldenWolverine		= 1193, //Horn

	QUESTMONSTERID_MinigueSilver		= 1031, //Vamp

	QUESTMONSTERID_BeeDog				= 1029, //Honey

	QUESTMONSTERID_IronGuard	= 24,
	QUESTMONSTERID_CrystalTower	= 2105,
};

enum EQuestStatus
{
	QUESTSTATUSID_None,
	QUESTSTATUSID_Killing,
	QUESTSTATUSID_GetItem,
	QUESTSTATUSID_EndQuest,
	QUESTSTATUSID_Finished		= 2000,

	// Add From Here
	QUESTSTATUSID_T5Quest1
};

#define MAX_QUESTDATA		5

struct QuestDataNew
{
	int										iaQuestID[MAX_QUESTDATA];
	int										iaKillCount[MAX_QUESTDATA];
	int										iaItemCount[MAX_QUESTDATA];
};

struct PacketQuestFinish : Packet
{
	DWORD									dwQuestID;
	EQuestStatus							eQuestStatus;
};

struct PacketQuestRestart : Packet
{
	DWORD									dwQuestID;
};
struct PacketQuestCancel : Packet
{
	DWORD									dwQuestID;
};

struct PacketQuestNPCOpen : Packet
{
	int										iID;
	int										iType;
	int										iAutoItemRewardId;

	char									szName[64];
	char									szMessage[2048];
};

struct PacketQuestNPCAccept : Packet
{
	int										iID;

	PacketQuestNPCAccept() { iID = 0; }
	PacketQuestNPCAccept( int i ) { iID = i; }
};



struct PacketQuestDataRequest : Packet
{
	int questID;
};



struct QuestInformation
{
	char name[40];
	char group[32];
	short groupNum;
	short questID;
	BOOL finished;
	BYTE minLevel;
	BYTE maxLevel;
	short mapID;
	short giverNpcID;
	short requiredQuestId;
	short mainQuestId;
	EQuestPartyType partyMode;
	EQuestType questType;

}; //size = 92

#define MAX_QUEST_INFO 88 //max allowed. 0x2000 (max packet size) / sizeof(QuestInformation)

struct PacketQuestNamesResponse : Packet
{
	BOOL bInitial;
	int questCount;
	QuestInformation questsInformation[MAX_QUEST_INFO];
};


#define MAX_QUESTEXTRAREWARD_COUNT 3
#define MAX_QUESTREWARD_ITEM 10
#define QUESTDATA_MONSTERITEM_MAX 5
struct PacketQuestDataResponse : Packet
{
	int			iQuestID;

	char		szStartText[64];
	char		szTipText[64];
	char		szEndText[64];

	BYTE		bNumOfReqMonsters;
	USHORT		saReqMonstersCounts[QUESTDATA_MONSTERITEM_MAX];
	char		szaReqMonsterNames[QUESTDATA_MONSTERITEM_MAX][32];
	BYTE        bNumOfReqItems;
	USHORT		saReqItemsCounts[QUESTDATA_MONSTERITEM_MAX];
	int			iaReqItemIds[QUESTDATA_MONSTERITEM_MAX];
	short		sMapID;
	INT64		iExpReward;

	short		sExtraRewardCount;
	EQuestExtraRewardType sExtraRewardType[MAX_QUESTEXTRAREWARD_COUNT];
	UINT		uExtraRewardValue [MAX_QUESTEXTRAREWARD_COUNT];

	BOOL		bItemSelect;
	BYTE		bItemCount;
	Item		saRewardItem[MAX_QUESTREWARD_ITEM]; //max 10

	int			GetTotalMonsterCount() {int iTotal = 0;	for ( int i = 0; i < bNumOfReqMonsters; i++ ) iTotal += saReqMonstersCounts[i]; return iTotal;}
	int			GetTotalItemCount() {int iTotal = 0;	for ( int i = 0; i < bNumOfReqItems; i++ ) iTotal += saReqItemsCounts[i]; return iTotal;}

};


struct PacketQuestWeaponMature : Packet
{
	Item sItem;
};

struct PacketQuestNPCFinish : Packet
{
	int										iID;
	int										iItemIndex;
	short									sWeight[2];
};


#define MAX_QUESTDYNAMICMENU			50

struct PacketDynamicQuestOpen : Packet
{
	char									szImagePath[260];
	char									szMainText[2048];
	int										iQuestCount;

	struct QuestData
	{
		int									iQuestID;
		char								szQuestName[64];
		IMinMax								sRequiredLevel;
		EQuestType							eQuestType;
		EQuestUserStatus					eQuestUserStatus;
	};

	QuestData								saQuestData[MAX_QUESTDYNAMICMENU];
};


struct PacketQuestHandleCancel : Packet
{
	int										iID;
};

struct PacketHandleFinishedQuest : Packet
{
	int										iID;

	char									szQuestName[40];
	char									szQuestGroupName[32];
	EQuestPartyType							eQuestPartyType;
	EQuestType								eType;
	BYTE									iMinLevel;
	BYTE									iMaxLevel;
	short									sGiverNpcID;
	short									sMapID;
	short									sMainQuestID;

	short									sGroupNum;
	SYSTEMTIME								sStartDate;
	SYSTEMTIME								sEndDate;

	EQuestExtraRewardType					iaExtraReward[QUEST_MONSTERITEM_MAX];
	UINT									iaExtraRewardValue[QUEST_MONSTERITEM_MAX];

	EAsmQuestBit							eAsmQuestBit;

	BOOL									bLoginTime;
};

struct PacketQuestMonsterName : Packet
{
	USHORT uMonsterUniqueID;
	char szMonsterName[32];
};

struct PacketQuestUpdateData : Packet
{
	int										iID;

	DWORD									dwTimeLeft;

	USHORT									iMonsterCount;
	USHORT									saMonstersKilled[QUEST_MONSTERITEM_MAX];
	USHORT									saMonstersReq[QUEST_MONSTERITEM_MAX]; //monster requirements

	USHORT									iItemsCount;
	USHORT									saItemsCollected[QUEST_MONSTERITEM_MAX];
	USHORT									saItemsReq[QUEST_MONSTERITEM_MAX]; //items requirements

	BOOL									bFinishedOtherRequirements;

	BOOL									IsRequirementsEmpty() { return iMonsterCount == 0 && iItemsCount == 0; }
};

struct PacketNetQuestUpdateData : Packet
{
	int										iID;
	struct PacketQuestUpdateData			saPacket[MAX_QUESTPACKETDATA];
};

struct PacketNetQuestUpdateDataPart : Packet
{
	int										iUserID;
	BOOL									bUpdateNPCQuestIcon;
	struct PacketQuestUpdateData			sData;
};


struct PacketSyncFinishedQuestData : Packet
{
	ID dwUserID;
	int iQuestID;
	EQuestType eQuestType;
	SYSTEMTIME sDateFinished;
};

struct PacketQuestStartData : Packet
{
	int										iID;
	char									szName[64];
	char									szShortDescription[256];

	char									szQuestGroupName[32];
	short									sQuestGroupNum;

	int										iType;

	int										iLevelMin;
	int										iLevelMax;

	BOOL									bInitialLoad;
	BOOL									bTime;
	DWORD									dwTimeLeft;
	DWORD									dwWaitingTime;
	DWORD									dwTimeTotal;

	short									sMapID;
	short									sGiverNpcID;

	int										iAreaType;

	int										iXMin;
	int										iXMax;
	int										iZMin;
	int										iZMax;

	int										iRadius;

	short									sMainQuestID;

	int										iParty;

	BOOL									bPvP;

	BOOL									bMultiple;

	int										iaMonsterID[QUEST_MONSTERITEM_MAX];
	USHORT									saMonstersReq[QUEST_MONSTERITEM_MAX];
	USHORT									saMonstersKilled[QUEST_MONSTERITEM_MAX];
	char									szaMonsterName[QUEST_MONSTERITEM_MAX][32];

	int										iaItemID[QUEST_MONSTERITEM_MAX];
	USHORT									saItemsReq[QUEST_MONSTERITEM_MAX];
	USHORT									saItemsCollected[QUEST_MONSTERITEM_MAX];
};


struct PacketDynamicQuestNPCOpen : Packet
{
	int										iQuestID;
};

struct PacketQuestItemSwap : Packet
{
	int					iQuestID;

	int					iItemID;
	int					iChk1;
	int					iChk2;
};

struct QuestFinishedData
{
	int										iID;
	char									szQuestName[40];
	char									szQuestGroupName[32];

	EQuestPartyType							eParty;
	EQuestType								eType;
	BYTE									iMinLevel;
	BYTE									iMaxLevel;
	short									sMapID;
	short									sGiverNpcID;
	short									sMainQuestID;
	short									sGroupNum;

	SYSTEMTIME								sStartDate;
	SYSTEMTIME								sEndDate;

	EQuestExtraRewardType					iaExtraReward[QUEST_MONSTERITEM_MAX];
	int										iaExtraRewardValue[QUEST_MONSTERITEM_MAX];
};



class QuestData
{
private:
	int							iID;

	int							iaMonsterID[QUEST_MONSTERITEM_MAX];
	USHORT						saMonstersReq[QUEST_MONSTERITEM_MAX];
	USHORT						saMonstersKilled[QUEST_MONSTERITEM_MAX];

	char						szaMonsterName[QUEST_MONSTERITEM_MAX][32];

	int							iaItemID[QUEST_MONSTERITEM_MAX];
	USHORT						saItemsReq[QUEST_MONSTERITEM_MAX];
	USHORT						saItemsCollected[QUEST_MONSTERITEM_MAX];

	char						szaItemName[QUEST_MONSTERITEM_MAX][32];

	char						szName[64];
	char						szGroupName[32];
	char						szShortDescription[256];

	int							iType;

	int							iLevelMin;
	int							iLevelMax;

	BOOL						bPvP;

	short						sMapID;
	short						sGiverNpcID;
	short						sGroupNum;

	int							iAreaType;

	BOOL						bTime;
	DWORD						dwTimeLeft;
	DWORD						dwWaitingTime;
	DWORD						dwTimeTotal;

	int							iXMin, iXMax;
	int							iZMin, iZMax;

	int							iRadius;

	EQuestPartyType				iParty;
	BOOL						bMultiple;
	USHORT						sMainQuestID;

	BOOL						bForceFinished = FALSE;
	BOOL						bKillsFinished = FALSE;
	BOOL						bItemsFinished = FALSE;
	BOOL						bOtherRequirementsFinished = FALSE;

	DWORD						pfnFinished = 0;

	BOOL						bUpdate = FALSE;
	BOOL						bForceUpdate = FALSE;

	BOOL						bFinishHandled = FALSE;

	void						HandleFinish();

public:
	//Constructor
	QuestData( PacketQuestStartData * psPacket );
	virtual ~QuestData();

	void						SetName( char * pszQuestName );
	void						SetDescription( char * pszQuestDescription );

	void						SetFunctionFinishHandle( void * pFunction ) { pfnFinished = (DWORD)pFunction; }

	char * GetName() { return szName; }
	char * GetGroupName() { return szGroupName; }
	char * GetShortDescription() { return szShortDescription; }

	int							GetID() { return iID; }
	short						GetMapID() { return sMapID; }
	short						GetGroupNum() { return sGroupNum; }
	short						GetGiverNpcID() { return sGiverNpcID; }
	int							GetAreaType() { return iAreaType; }
	int							GetType() { return iType; }
	int							GetLevelMin() { return iLevelMin; }
	int							GetLevelMax() { return iLevelMax; }

	BOOL						QuestAllowsPVP() { return bPvP; }

	int							GetXMin() { return iXMin; }
	int							GetXMax() { return iXMax; }
	int							GetZMin() { return iZMin; }
	int							GetZMax() { return iZMax; }
	int							GetRadius() { return iRadius; }

#ifdef DEV_MODE
	void						SetForceFinished() { bForceFinished = TRUE; }
	void						SetKillsFinished() { bKillsFinished = TRUE; }
	void						SetItemsFinished() { bItemsFinished = TRUE; }
	void						SetOtherRequirementsFinished() { bOtherRequirementsFinished = TRUE; }
#endif

	BOOL						HasOtherRequirements() { return ( iID == QUESTID_Rankup_3_Morion || iID == QUESTID_Rankup_3_Tempskron ); };
	void						CheckOtherRequirementsFinished();
	BOOL						IsKillsFinished() { return bKillsFinished; }
	BOOL						IsItemsFinished() { return bItemsFinished; }
	BOOL						IsOtherRequirementsFinished() { return HasOtherRequirements() ? bOtherRequirementsFinished : TRUE; }

#ifdef DEV_MODE
	BOOL						IsFinished() { return ( bForceFinished || ( bKillsFinished && bItemsFinished && IsOtherRequirementsFinished() ) ); }
#else
	BOOL						IsFinished() { return (bKillsFinished && bItemsFinished && IsOtherRequirementsFinished()); }
#endif
	BOOL						CanFinishQuestWaitTime();

	BOOL						IsInArea( int iMapID, int iX, int iZ );

	int							GetMonsterID( int i );
	USHORT						GetMonstersReq( int i );

	USHORT						GetMonsterCount();
	USHORT						GetItemCount();

	USHORT						* GetMonsterKilledData() { return saMonstersKilled; }
	USHORT						* GetItemsCollectData() { return saItemsCollected; }
	USHORT						* GetMonstersReqData() { return saMonstersReq; }
	USHORT						* GetItemsReqData() { return saItemsReq; }

	int							GetTotalMonsterKilledAmount( );
	int							GetTotalItemAmount( );
	USHORT						GetMonsterKilledAmount( int i );
	char						* GetMonsterName( int i );
	char						* GetItemName( int i );
	int							GetItemID( int i );
	USHORT						GetItemsReq( int i );
	USHORT						GetItemsReqByItemID( int iItemID );
	USHORT						GetItemsCollected( int i );

	EQuestPartyType				GetQuestPartyType() { return iParty; }
	USHORT						GetMainQuestID() { return sMainQuestID; }

	BOOL						QuestAllowsSolo()	{ return (iParty == EQuestPartyType::SoloAny      || iParty == EQuestPartyType::SoloPartyAny     || iParty == EQuestPartyType::SoloOnly     || iParty == EQuestPartyType::SoloAndParty     || iParty == EQuestPartyType::SoloPartyAndRaid ) ? TRUE : FALSE; }
	BOOL						QuestAllowsParty()	{ return (iParty == EQuestPartyType::SoloAny      || iParty == EQuestPartyType::SoloPartyAny     || iParty == EQuestPartyType::PartyOnly    || iParty == EQuestPartyType::SoloAndParty     || iParty == EQuestPartyType::PartyAndRaid || iParty == EQuestPartyType::SoloPartyAndRaid) ? TRUE : FALSE; }
	BOOL						QuestAllowsRaid()	{ return (iParty == EQuestPartyType::SoloAny      || iParty == EQuestPartyType::PartyAndRaid || iParty == EQuestPartyType::SoloPartyAny     || iParty == EQuestPartyType::SoloPartyAndRaid || iParty == EQuestPartyType::RaidOnly) ? TRUE : FALSE; }


	BOOL						IsMultiple() { return bMultiple; }

	BOOL						IsTimeQuest() { return bTime; }
	DWORD						GetTimeQuest() { return dwTimeLeft; }
	DWORD						GetWaitTime() { return dwWaitingTime; }

	BOOL						AddMonsterKill( int i );
	BOOL						AddItemCollected( int i );

	void						SetID( int i ) { iID = i; }
	void						SetMapID( short i ) { sMapID = i; }

	BOOL						Update();
	void						Update( BOOL b ) { bUpdate = b; };
	void						ForceUpdate( BOOL b ) { bForceUpdate = b; };
	BOOL						IsForceUpdate() { return bForceUpdate; };

	//Functions
	BOOL						HandleUnitKill( int iTargetID, int iTargetLevel, int iKillerID, int iMapID, int iX, int iZ );

	BOOL						RemoveRequiredItemsFromInventory ();
	BOOL						CheckPlayerItems ();
	BOOL						HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ, int iTotalItem = -1 );

	void						Update( float fTime );

};

class Quest
{
public:

	Quest();
	virtual ~Quest();

	std::vector<QuestData*>		& GetData() { return vData; }
	std::vector<QuestFinishedData*>		& GetFinishData() { return vFinishedData; }
	QuestData*					GetQuest( int iID );

	BOOL						IsActiveQuest( int iID );

	BOOL						IsFinishedQuestKills( int iID );
	BOOL						IsFinishedQuestItems( int iID );

	BOOL						AddQuest( QuestData * psQuestData );

	BOOL						AddFinishedQuest( PacketHandleFinishedQuest * psPacket );

	BOOL						IsDoneQuest( int iQuestID );

#if defined (_GAME)
	BOOL						IsQuestExpired(EQuestType eQuestType, const SYSTEMTIME& finishTime);
#endif

	BOOL						IsAvailable( int iLevel, int iQuestID );

	BOOL						HandleUnitKill( int iTargetID, int iTargetLevel, int iKillerID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid, BOOL bTargetIsPlayer, BOOL bTargetIsBoss);

	BOOL						CheckAllItemRequirements( );
	BOOL						CheckAllOtherRequirements( );

	BOOL						DeleteQuest( int iID );
	BOOL						DeleteFinishQuest( int iID );
	void						DeleteAllQuests();

	int							GetRewardValue( EQuestExtraRewardType iRewardType );

private:
	std::vector<QuestData*>		vData;

	std::vector<QuestFinishedData*> vFinishedData;
};