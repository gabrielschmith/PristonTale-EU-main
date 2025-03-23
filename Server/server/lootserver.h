#pragma once

class LootServer
{
public:

	enum EDropType : int
	{
		DROPTYPE_UNKNOWN = -1,
		DROPTYPE_AIR = 1,
		DROPTYPE_GOLD = 2,
		DROPTYPE_ITEMS = 3,
	};

	struct BaseDropDefinition
	{
		int iDropChance;
		EDropType eDropType;
	};

	struct GoldDropDefinition : BaseDropDefinition
	{
		int iGoldMin;
		int iGoldMax;
	};

	struct ItemDropDefinition : BaseDropDefinition
	{
		std::vector<DWORD> vItemCodes;
	};

	//Nothing
	struct AirDropDefinition : BaseDropDefinition
	{

	};

	struct MonsterDropTable
	{
		int iTotalDropChance = 0;
		std::vector<BaseDropDefinition*> vDropDefinitions;
	};

	LootServer();
	virtual ~LootServer();


	void					SQLUpdateDropTableFromDatabase();

	BOOL					SendQuestDropItemToUser( UnitData * pcUnitData, User * pcUser );
	BOOL					GetRandomItemForMonster(UnitData * pcUnitData, User * pcUser, Item* psItem);

	BOOL					DropDefinitionExistsForMonsterID ( int iMonsterDropId );

	void					GenerateDropStats( std::string sMonsterName, const char * szSubFolder = "Test", int iRepeatCount = 10000);
	void					GenerateDropStatsMap( int iMapID, const char * szSubFolder = "Test", int iRepeatCount = 10000);
private:




	BaseDropDefinition *	GetRandomDropDefinition(int iMonsterDropId);

	std::map<int, MonsterDropTable>				      mDropTable;
	std::mutex										  mDropTableMutex;
};
