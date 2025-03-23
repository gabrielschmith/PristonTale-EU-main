#pragma once


#include <objbase.h>
#include <vector>
#include <comdef.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

#define MAX_PARAMS		10000
#define MAX_QUERY_SIZE	2048

enum EDatabaseID
{
	DATABASEID_None						= 0,

	DATABASEID_GameDB					= 1,
	DATABASEID_GameDB_Event			= 2,
	DATABASEID_GameDB_Aging				= 3,
	DATABASEID_GameDB_Npc				= 4,
	DATABASEID_GameDB_Monsters			= 5,
	DATABASEID_GameDB_Mixing			= 6,

	DATABASEID_GameDB_QuestList			= 10,
	DATABASEID_GameDB_QuestList_Top		= 11, //to prevent lock of QuestList
	DATABASEID_GameDB_QuestEventList	= 12,
	DATABASEID_GameDB_PetSystem			= 13,
	DATABASEID_GameDB_PetSystem_Skin	= 14,

	DATABASEID_SkillDB				= 20,
	DATABASEID_SkillDBNew			= 21,

	DATABASEID_EventDB				= 30,

	DATABASEID_ClanDB				= 40,
	DATABASEID_ClanDB_2				= 41,
	DATABASEID_ClanDB_3				= 42,
	DATABASEID_ClanDB_4				= 43,

	DATABASEID_ServerDB				= 50,

	DATABASEID_ItemDB				= 60,

	DATABASEID_ChatDB				= 70,
	DATABASEID_ChatDB_Cleaner		= 71,

	//These will always point to the 'local' sql server
	DATABASEID_UserDB_LocalServer_Misc		  = 81,
	DATABASEID_UserDB_LocalServer_CharInfo	  = 83,
	DATABASEID_UserDB_LocalServer_UserInfo	  = 84,
	DATABASEID_UserDB_LocalServer_Bless		  = 85,	//Bless castle score and pvp, and ranking
	DATABASEID_UserDB_LocalServer_Quest		  = 86,	//Quest
	DATABASEID_UserDB_LocalServer_Timers	  = 87,	//Item timers
	DATABASEID_UserDB_LocalServer_TitleAura	  = 88,	//Title and Aura
	DATABASEID_UserDB_LocalServer_KillTracker = 89,	//Kill tracker
	DATABASEID_UserDB_LocalServer_ItemBox     = 90,	//Item box
	DATABASEID_UserDB_LocalServer_Quest_Sub   = 91,	//Quest

	//These will always point to the 'primary' sql server
	DATABASEID_UserDB_PrimaryServer			= 100,
	DATABASEID_UserDB_PrimaryServer_2		= 101,		//UserDB -> CoinTrading / Coin (dedicated)

	//For syncing local database to primary (seasonal -> primary only)
	DATABASEID_UserDB_PrimaryServerSyncer   = 102,

	DATABASEID_LogDB						= 200,
	DATABASEID_LogDB_ServerLog				= 201, //For server log only, to prevent deadlock
	DATABASEID_LogDB_Cleaner				= 202, //For execution of the stored procedure to clean up old logs from database which can take a while
	DATABASEID_LogDB_PacketLog				= 203, //For logging of packet stats
	DATABASEID_LogDB_DatabaseLog			= 204, //For logging of database stats
	DATABASEID_LogDB_EventKill			    = 205,
};

enum EDatabaseDataType
{
	PARAMTYPE_Byte,		//tinyint  (0 to 255)
	PARAMTYPE_Short,	//smallint (+/- 32,768)
	PARAMTYPE_Integer,	//int	   (+/- 2,147,483,648)
	PARAMTYPE_Int64,	//bigint   (+/- 9,223,372,036,854,775,808)
	PARAMTYPE_String,
	PARAMTYPE_Float,
	PARAMTYPE_Time,
	PARAMTYPE_Binary,
	PARAMTYPE_Null = 5000,
};

struct DatabaseStructureData
{
	EDatabaseID									eDatabaseID;
	char										* pszDatabaseName;
};

struct DatabaseStat
{
	int iCounter;
	float fTotalDuration;
	float fMaxDuration;
	float fMinDuration;
};


class SQLConnection
{
private:
	DatabaseStructureData						* GetDatabaseData( EDatabaseID eDatabaseID );
	SQLHANDLE									hEnv;
	SQLHANDLE									hConnection;
	SQLHANDLE									hStatement;
	EDatabaseID									eID;
	char										szDatabaseName[ 64 ];
	int											iLastUniqueSubID;
	int											iUniqueSubID;


	std::map<int, DatabaseStat>					sDBStat;

	char										szQuery[MAX_QUERY_SIZE];

	CRITICAL_SECTION							sCriticalSection;

	BOOL										bOpen		= FALSE;
	BOOL										bPrepared	= FALSE;

	bool										baBlock[MAX_PARAMS];
	SQLINTEGER									iaBlock[MAX_PARAMS];
	SQLINTEGER									* BlockNext();
	void										BlockFree();



	time_t										uNextDBLogTime;

public:

	void										Init( EDatabaseID eDatabaseID , std::string configPrefix = std::string());

	BOOL										SQLTimeStampToSystemTime( SYSTEMTIME * psSystemTime, SQL_TIMESTAMP_STRUCT * psSQLTimeStamp );

	void										SetUniqueSubID( int iSubConnectionID );
	BOOL										Open( );
	BOOL										Prepare( const char * pszQuery, bool p_IgnoreUnsecure = false );
	BOOL										Execute(bool bSupressError = false);
	BOOL										ExecuteUpdate();
	BOOL										Execute( const char * pszQuery );

	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, BYTE * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, short * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, unsigned short * pParameter);
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, int * pParameter);
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, unsigned int * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, long * pParameter);
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, unsigned long * pParameter );

	//BOOL										GetData( int iPosition, EDatabaseDataType eDataType, const char * pParameter, int iSize );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, char * pParameter, int iSize);

	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, float * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, SYSTEMTIME * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, signed __int64 * pParameter );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, uint64_t * pParameter );

	BOOL										GetBinaryData( int iPosition, void * pParameter, int iSize );

	void										LogDatabaseStats();

	BOOL										BindParameterInput( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize = 0 );
	BOOL										GetDataInternal( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize = 0 );
	BOOL										GetColumnName( int iPosition, void * pParameter, int iSize );
	BOOL										GetColumnCount( int & iCount );
	EDatabaseDataType							GetColumnType( const char * pszTableName, const char * pszColumnName );
	BOOL										Fetch();
	BOOL										Close();
	BOOL										Clear();

	static SQLConnection						* GetConnection( EDatabaseID eDatabaseID, int iSubConnectId = 0);

	SQLConnection();
	virtual ~SQLConnection();
};

