#pragma once

#define MAX_COMMANDSPERTIME 10

enum ESQLServerCommand
{
	SQLSERVERCOMMAND_ChangeCharacterName	= 10,
	SQLSERVERCOMMAND_ChangeCharacterLevel	= 11,
	SQLSERVERCOMMAND_ChangeCharacterAccount = 12,
	SQLSERVERCOMMAND_ChangeCharacterClass	= 13,

	SQLSERVERCOMMAND_ChangeAccountName		= 20,

	SQLSERVERCOMMAND_KickAccountName		= 30,
	SQLSERVERCOMMAND_KickCharacterName		= 31,

	SQLSERVERCOMMAND_LoadCoinShop			= 101,
	SQLSERVERCOMMAND_LoadCheatList			= 111,
	SQLSERVERCOMMAND_LoadMixFormula			= 121,
};

struct SQLServerCommand
{
	ESQLServerCommand	iType;
	char				szOperator[32];
	char				szParameter1[64];
	char				szParameter2[64];
	char				szParameter3[64];
	SYSTEMTIME			sDateSubmitted;
	SYSTEMTIME			sDateProcessed;
};


class ServerCommand
{
private:


	void									OnActivateGameMaster( User * pcUser );

	BOOL									OnGameMasterAdminCommand( User * pcUser, const char * pszBuff );
	BOOL									OnGameMasterLevel1Command( User * pcUser, const char * pszBuff );
	BOOL									OnGameMasterLevel2Command( User * pcUser, const char * pszBuff );
	BOOL									OnGameMasterLevel3Command ( User * pcUser, const char * pszBuff );

	BOOL									OnPlayerCommand( User * pcUser, const char * pszBuff );

	void									HandleGrantTitleCommand(const std::string& charName, const std::string& title, const int titleRarity);
public:
	ServerCommand();
	virtual ~ServerCommand();

	void									SQLReadAndExecuteGMCommand ();
	static void								SQLRead();
	static BOOL								SQLReadFromCharacter( const char * pszCharacterName, ESQLServerCommand iCommand, SQLServerCommand & s );

	bool									Process( SQLServerCommand * ps );


	BOOL									OnGameMasterCommand( User * pcUser, const char * pszBuff );

	void									SetVersion( int iVersion );
	void									SetMoriphEvent(BOOL bEnable, int iSpawnCount = -1, int iSpawnDelayMin = -1);
	void									SetWolfEvent(BOOL bEnable, int iSpawnCount = -1, int iSpawnDelayMin = -1);
	void									SetHalloweenEvent( BOOL b );
	void									SetMimicEvent ( BOOL b );
	void									SetChristmasEvent ( BOOL b );
	void									SetEventGirlFree ( BOOL b );
	void									SetEasterEvent( BOOL b );
	void									SetStarWarsEvent( BOOL b );
	void									SetBeeEvent( BOOL b );
	void									SetValentineDayEvent ( BOOL b );
	void									SetAgingEvent (BOOL bFree, BOOL bNoBreak, BOOL bHalfPrice );

	BOOL									OnReceiveChat( User * pcUser, PacketChat * psPacket );

	void									CheckUserOnline( User * pcUser, struct PacketUserOnline * psPacket );

	void									AddCommand( int iCommandID, char * pszParameter1, char * pszParameter2, char * pszParameter3 );

};

struct SQLSpawnCamera
{
	int										szMapNumber;
	Point3D									szSpawn;
	int										szCamXCoord;
	int										szCamZCoord;
	int										szCamTurn;
	int										szCamAngle;
	int										szCamZoom;
	int										szMinPlayerCount;
	int										szSearchRadius;
	int										szSpawnType;
	int										iPlayersPerSpawn;
};

