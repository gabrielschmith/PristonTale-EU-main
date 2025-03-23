#pragma once

struct QuestActiveMonsterKillData
{
	int						iQuestID;
	int						iPercentIncreaseEXP;
	int						iDropReduce;

	USHORT					sMonsterCount;
	int						iaMonsterID[QUEST_MONSTERITEM_MAX];
	USHORT					saMonsterKillCount[QUEST_MONSTERITEM_MAX];

	short					sMapID;
	short					sGiverNpcID;
};

enum class EQuestClassFlag : int
{
	None			= 0,

	Mechanician		= 1 << 0,
	Fighter			= 1 << 1,
	Pikeman			= 1 << 2,
	Archer			= 1 << 3,
	Assassin		= 1 << 4,

	Atalanta		= 1 << 5,
	Knight			= 1 << 6,
	Magician		= 1 << 7,
	Priestess		= 1 << 8,
	Shaman			= 1 << 9,

	Tempkson		= Mechanician | Fighter | Pikeman | Archer | Assassin,
	Morion			= Knight | Atalanta | Magician | Priestess | Shaman,
};

inline EQuestClassFlag operator | (EQuestClassFlag a, EQuestClassFlag b)
{
	return static_cast<EQuestClassFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline EQuestClassFlag operator & (EQuestClassFlag a, EQuestClassFlag b)
{
	return static_cast<EQuestClassFlag>(static_cast<int>(a) & static_cast<int>(b));
}

struct NPCQuestData
{
	enum EFlag : int
	{
		Giver       = 1 << 0,
		Progresser  = 1 << 1,
		Completer   = 1 << 2,
		Repeatable  = 1 << 3,
		QuestWindow = 1 << 4,

		All = Giver | Progresser | Completer
	};

	int iQuestID;
	EFlag eFlag;

	BOOL IsGiver() const
	{
		return ( eFlag & EFlag::Giver ) != 0;
	}

	BOOL IsProgresser() const
	{
		return ( eFlag & EFlag::Progresser ) != 0;
	}

	BOOL IsCompleter() const
	{
		return ( eFlag & EFlag::Completer ) != 0;
	}
};


struct SimpleQuestData
{
	int iQuestID;
	int iMinLevel;
	int iMaxLevel;
	int iMapID;
	int iGiverNpcID;
	INT64 lExpReward;
	BOOL bEXPPotBonus;
	BOOL bItemRewardSelection;
	BOOL iExpLEvelDiff;
	EAsmQuestBit iASMQuestBit;
	EQuestClassFlag eClassFlag;
	EQuestType eType;
	EQuestPartyType ePartyType;
	BOOL bMultiple;			//TRUE if this quest can be taken while undergoing other quests
	BOOL bPVP;				//TRUE if this quest is PVP based
	int iAutoStartQuestID;	//Auto starts quest if any
	int iMainQuestID;		//Main quest id if any (for sub-quests)

	char szQuestName[64];
	char szQuestBookName[40];
	char szQuestBookStartText[64];
	char szQuestBookTipText[64];
	char szQuestBookEndText[64];
	char szQuestBookGroupName[32];
	int iQuestBookGroupRank;

	std::vector<short> sRequiredQuestIDs;
	std::vector<short> sInclusiveQuestIDs;
	std::vector<DWORD> sReqMonsterIDs;
	std::vector<short> sReqMonsterCounts;
	std::vector<ItemID> sReqItemIDs;
	std::vector<short> sReqItemCounts;

	std::vector<ItemID> sItemRewardsIDs;
	std::vector<short> sItemRewardsCount;

	std::vector<EQuestExtraRewardType> eExtraRewardsTypes;
	std::vector<UINT> uExtraRewardsValues;
};


class QuestServer
{
public:
												QuestServer();
	virtual									   ~QuestServer();

	EQuestClassFlag								ECharacterClassToQuestClassFlag( ECharacterClass eCharacterClass );
	EQuestClassFlag								GetQuestClassFlagFromRaw( const char * szRaw );
	BOOL     									GetQuestWeapon( User * pcUser, Item * pcItem, int iRank );
	BOOL     									GetQuestWeapon( User * pcUser, Item * pcItem, const std::string itemCode, int iRank );
	BOOL     									GetItemRewardFromCode( User * pcUser, DWORD dwItemID, Item * pcItem, BOOL bViewMode, int iQuestID );
	BOOL										HandleQuestWeaponUpgrade( User * pcUser, PacketQuestWeaponMature * sPacket );
	BOOL										SetQuestWeaponMonsterIDAndCount( Item * sItem, int iRank );
	BOOL										SetQuestWeaponRank( Item * sItem, int iRank );
	//SQL Data
	void										LoadFinishedQuestsForUser( User * pcUser );

	BOOL										IsQuestExpired(  User * pcUser, int iQuestID );
	BOOL										IsQuestExpired( EQuestType eQuestType, const SYSTEMTIME & finishTime );
	void										UpdateNPCQuestIconForUser( User * pcUser, int iQuestID );
	void										UpdateNPCQuestIconForUserPostLevelUp( User * pcUser );
	BOOL										TryTakeAvaliableQuest( User * pcUser, int iQuestID, int iNPCID = -1, BOOL bSendMessage = FALSE );
	const std::string							GetQuestWaitMessage( EQuestType eQuestType );

	BOOL										SQLDeleteQuest( User * pcUser, int iQuestID );
	BOOL										SQLCancelQuest( User * pcUser, int iQuestID, BOOL bAutoStartQuest );

	BOOL										IsDoneMainQuestID( User * pcUser, SimpleQuestData * psQuestData );
	BOOL										IsCompletedRequiredQuestIDs( User * pcUser, SimpleQuestData * psQuestData );
	int											GetNpcQuestGiverIDForQuest( int iQuestID );

	BOOL										SQLFinishQuest( User * pcUser, int iQuestID );

	int											GetAutoStartQuestID( User * pcUser, int iQuestID );
	std::vector<int>							SQLGetActiveQuestsID( User * pcUser );
	BOOL										AccountHasFinishedQuest ( char * szAccountName, enum EQuestID eQuestID );
	BOOL										CheckAndAutoFinishOtherQuests( User * pcUser, int iFinishedQuestID );
	BOOL										UpdateAndSyncFinishedQuestUser( User * pcUser, int iQuestID, EQuestType eQuestType, const SYSTEMTIME & sSystemTime, BOOL bLoadStage );
	BOOL										HandleQuestCommand( User * pcUser, PacketTransCommand * psCommand );
	BOOL										OpenMonsterQuestArea( User * pcUser, DWORD dwQuestCode, int Param = 0 );
	BOOL										HandleFinishedQuestUser( User * pcUser, int iQuestID = 0, BOOL bLoginTime = FALSE, BOOL bForceFinish = FALSE );
	BOOL										SQLInsertFinisheRankUpQuest( User * pcUser, int iQuestID );
	BOOL										SQLInsertFinishedASMQuest( User * pcUser, int iQuestID );
	BOOL										CheckAndInsertFinishedWarpGateQuestUser( User * pcUser, EQuestID eQuestID, EItemID eCurrWarpWingID, EItemID eWarpWingCheckID );
	BOOL										CheckAndInsertFinishedRankupQuestUser( User * pcUser, EQuestID eQuestID, ECharacterRace eCharacterRace, int iRankUpLevel0Based );
	BOOL										CheckAndInsertFinishedASMQuestUser( User * pcUser, EQuestID eQuestID, EAsmQuestBit eAsmQuestID );
	BOOL										HandleFinishedASMQuestUser( User * pcUser);
	BOOL										HandleFinishedWarpGateQuestUser( User * pcUser );
	BOOL										HandleFinishedRankupQuestUser( User * pcUser);
	BOOL										HandleFinishedWarehouseQuestUser( User * pcUser);

	void										RemoveExpiredQuestFinishData( User * pcUser );
	void										RemoveQuestFinishData( User * pcUser, int iQuestID, BOOL bSyncWithOtherServer = FALSE );
	int											SQLGetQuestTimeLeft( User * pcUser, int iQuestID, DWORD & dwTimeLeft );
	int											SQLGetQuestTimeLeft( int iQuestID, DWORD & dwTimeLeft );

	BOOL										SQLGetAreaQuest( int iQuestID, short & iMapID, short & sGiverNpcID, int & iMinX, int & iMaxX, int & iMinZ, int & iMaxZ, int & iType, int & i
	);

	BOOL										SQLGetMonsterItemCharacter( User * pcUser, int iQuestID, char * pszMonsterCounter, char * pszItemCounter );
	BOOL										SQLGetQuestNameShortDesc( int iQuestID, char * pszName, char * pszGroupName, char * pszShortDescription, BOOL & bPvP, int & iParty, int & iType, int & iLevelMin, int& iLevelMax, BOOL & bMultiple, DWORD & dwWaitingTime, DWORD & dwTimeTotal, short & sMainQuestID, short & sGroupNum );
	BOOL										SQLGetDynamicQuestShortData( int iQuestID, char * pszName, IMinMax & sLevel );

	BOOL										SQLStartQuest( User * pcUser, int iQuestID );
	BOOL										SQLQuestSwapClass( char * pszCharacterName, int iOldQuestID, int iNewQuestID );

	BOOL										OnLoadUser( User * pcUser );
	BOOL										OnUnLoadUser( User * pcUser );

	int											GetAutoQuestRewardItemCodeIfAny( int iQuestID );
	void										OnSayTime( UnitData * pcUnitData );

	void										LoadNPCQuests();
	BOOL										IsNpcQuestGiver(int iNPCID);
	void										SendNpcQuestStatusIcon( User * pcUser, int iNPCID, BOOL bForceSendStatus = FALSE, int iQuestID = -1, BOOL bClicked = FALSE );
	void										HandleGetUnitInfoData( User * pcUser, int iUnitID );

	BOOL										OnClickDynamicNPC( User * pcUser, UnitData * pcUnitData );
	BOOL										OnClickNPC( User * pcUser, UnitData * pcUnitData );
	BOOL										OnClickNPC( User * pcUser, int iNpcID, int iQuestID = -1 );
	BOOL										IsRewardFilteredOutForUser( User * pcUser, DefinitionItem * psDef );

	void										SendNpcQuestMessage( User * pcUser, int iQuestID, int iType );

	void										SQLSaveQuestProgress( User * pcUser, PacketQuestUpdateData * psPacket );

	void										HandlePacket( User * pcUser, PacketSyncFinishedQuestData * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestNPCAccept * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestHandleCancel * psPacket );
	void										HandlePacketUnload( User * pcUser, PacketQuestUpdateData * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestUpdateData * psPacket, BOOL bUpdateNPCQuestIcon = FALSE );
	void										HandlePacket( User * pcUser, PacketNetQuestUpdateData * psPacket );
	void										HandlePacket( User * pcUser, PacketNetQuestUpdateDataPart * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestNPCFinish * psPacket );
	void										HandlePacket( User * pcUser, PacketDynamicQuestNPCOpen * psPacket );
	void										HandlePacket( User * pcUser, PacketQuestItemSwap * psPacket );
	void										HandlePacket( User* pcUser, PacketQuestNamesRequest* psPacket );
	void										HandlePacket(User* pcUser, PacketQuestDataRequest* psPacket);
	void										HandleQuestCancelOrFinish(User* pcUser, int iQuestID, BOOL bForceFinish);

	void										SendNewQuestsPostLevelUp( User * pcUser );
	void										RunTests ();
	void										HandleLoadQuests( User * pcUser );

	void										ProcessPacket( User * pcUser, PacketQuestStartData * psPacket );
	void										ProcessPacket( User * pcUser, PacketDynamicQuestOpen * psPacket );

	//T5
	void										ProcessT5Quests( User * pcUser, int iQuestID, int iValue1, int iValue2 );

	void										HandlePacket( PacketNetT5QuestData * psPacket );

	int											GetReduceQuestDrop( User * pcUser, UnitData * pcUnitData );

	void										HandleQuestMonsterNameRequest( User * pcUser, int iUniqueMonsterID );
	void										SendQuestMonsterName( User * pcUser, CharacterData * psCharData );

	INT64										GetQuestEXPOnKill( User * pcUser, UnitData * pcUnitData, INT64 iEXP );
	//Kill
	BOOL										OnUnitKilled( User * pcUser, Unit * pcUnit );

	BOOL										IsUndergoingAnyQuest( User * pcUser );
	BOOL										IsUndergoingLoneNonMultipleQuest( User * pcUser );
	BOOL										IsActiveQuest( User * pcUser, int iQuestID, PacketQuestUpdateData * psData = NULL );
	BOOL										IsCompletedQuestRequirements( User * pcUser, PacketQuestUpdateData * pQuestData );
	PacketQuestUpdateData						* GetActiveQuestData( User * pcUser, int iQuestID );

private:
	BOOL										IsT5QuestProgress( int iQuestID );

	std::vector<QuestActiveMonsterKillData*>	vQuestActiveMonsterKill;

	std::map<DWORD, std::vector<NPCQuestData>>	mNPCQuests;
	std::map<int, SimpleQuestData>				mQuests;

	std::map<DWORD, Item>						mQuestRewards;
};

