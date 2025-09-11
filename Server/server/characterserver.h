#pragma once

struct SQLCharacter
{
	int					iID;
	char				szAccountName[32];
	char				szCharacterName[32];
	int					iLevel;
	INT64				iEXP;
	int					iGold;
	int					iClass;
	int					iClanID;
	int					iClanPermission;
	SYSTEMTIME 			sLastSeenDate;
	int					iLastMapID;
};


class CharacterServer
{
private:
	int											iSiegeWarDay = 0;
	int											iSiegeWarHour = 0;

	int											iTopRankingCharacterId = 0;

	int											bDebug = FALSE;
	void										Debug(const char* pzsFormat, ...);

												//Private SQL functions. Use the public ones below.
	void										SQLChangeCharacterLevel ( SQLConnection * pcDB, char * pszCharacterName, int iLevel, INT64 iEXP );
	void										SQLChangeCharacterClass ( SQLConnection * pcDB, char * pszCharacterName, int iClass );
	void										SQLUpdateCharacter ( SQLConnection * pcDB, char * pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold );
	void										SQLUpdateLevelUpDate ( SQLConnection * pcDB, int iCharacterId);
	void										SQLUpdateLevelAndJobCode ( SQLConnection * pcDB, CharacterData * pcCharacterData );
	void										SQLSaveCharacterDataEx ( SQLConnection * pcDB, User * pcUser );
	void										SQLGiveEXP ( SQLConnection * pcDB, User * pcUser, INT64 iEXP );
	void										SQLSetEXP ( SQLConnection * pcDB, User * pcUser, INT64 iEXP );


public:
	CharacterServer();
	virtual ~CharacterServer();

	std::string									SQLGetAccountName(const char* p_CharacterName);
	bool										CheckCharacterAccountGouvernance(const char * p_CharacterName, const char * p_AccountName);

	void										SetSiegeWarDay( int i ) { iSiegeWarDay = i; }
	void										SetSiegeWarHour( int i ) { iSiegeWarHour = i; }

	int											GetSiegeWarDay() { return iSiegeWarDay; }
	int											GetSiegeWarHour() { return iSiegeWarHour; }


	void										SQLUpdateCharacterExpAndLevel ( User * pcUser );
	void										SQLUpdateGMLevel ( const char * p_CharacterName, int iGMLevel );
	BOOL										SQLGetCharacter( SQLConnection * pcDB, const char * pszCharacterName, SQLCharacter * ps );
	int											SQLGetCharacterLongClanID (char* pszCharacterName);
	int											SQLGetCharacterClanID( char * pszCharacterName );
	int											SQLGetCharacterClass ( char * pszCharacterName );
	void										SQLRenameCharacter( char * pszCharacterName, char * pszNewCharacterName );
	void										SQLChangeCharacterLevel ( char * pszCharacterName, int iLevel, INT64 iEXP );
	void										SQLChangeCharacterClass (  char * pszCharacterName, int iClass );
	void										SQLUpdateCharacter ( char * pszCharacterName, int iCharacterClass, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold );
	void										SQLUpdateLevelUpDate ( int iCharacterId );
	void										SQLUpdateLevelAndJobCode ( CharacterData * pcCharacterData );
	void										SQLSaveCharacterDataEx ( User * pcUser );
	void										SQLGiveEXP ( User * pcUser, INT64 iEXP );
	void										SQLSetEXP ( User * pcUser, INT64 iEXP );

	void										Tick1Min ();


	char *										GetCharacterName( UserData * pcUserData );
	char *										GetCharacterName( User * pcUser );
	bool										GetCharacterExFilePath( char * pszCharacterName, char * pszBuffer, int iBufferLen );
	bool										GetCharacterFilePath( char * pszCharacterName, char * pszBuffer, int iBufferLen );
	DWORD										GetClanCode( int iClassClan );
	void										SendBlessCastleSkill( UserData * pcUserData );

	int											ChangeCharacterName( char * pszCharacterName, char * pszNewCharacterName );
	int											ChangeCharacterLevel( char * pszCharacterName, int iLevel );
	int											ChangeCharacterClass( char * pszCharacterName, int iClass );
	BOOL										SetCharacterEXPOnDisconnect( char * pszCharacterName, INT64 iEXP );



	BOOL										OnPremiumTimersAndForceOrbTick1s( UserData * pcUserData );
	void										CheckGold( UserData * pcUserData );

	INT64										GetExp( UserData * pcUserData );
	void										SetExpFromSQL( UserData * pcUserData );

	INT64										GetExpFromLevel( int iLevel );
	int											GetLevelFromExp( INT64 iExp );

	void										OnCharacterUpdate( UserData * pcUserData );

	int											GetTopRankingCharacterID () { return iTopRankingCharacterId; }

	void										SQLLoadUserEffectTitleAndRanking ( User * pcUser );
	void										SyncUserEffectToUser ( User * pcUser );
	void										ClearDialogSkin ( User * pcUser );
	void										SetDialogSkin ( User * pcUser, int iDialogSkin );
	void										ClearTitle ( User * pcUser );
	void										SetTitle ( User * pcUser, int iTitleID );

	void										SendFullTitleListIfAny ( User * pcUser );

	void										UpdateAndSyncAuraAndDisplaySkinForAllClass ();
	void										SQLUpdatePlayerCharacterClassRanking ( User * pcUser );
	void										UpdatePlayerCharacterOverallRanking ();
	void										UpdateAndSyncAuraAndDisplaySkinForClass ( enum ECharacterClass eCharacterClass );
	void										CheckSyncUserRanking ( User * pcUser, int iNewRanking );

	BOOL										CharacterInTown( EMapID eMapID );
	BOOL										CharacterInTownEx( EMapID eMapID );
	BOOL										CharacterInTown( UserData * pcUserData );
	BOOL										CharacterInTownEx( UserData * pcUserData );
	BOOL										CharacterInCityStage( UserData * pcUserData );
	BOOL										CharacterInBelltraSOD( UserData * pcUserData );
	BOOL										CharacterInBattleStage( UserData * pcUserData );

	void										OnAddExp( UserData * pcUserData );
	void										OnLoseExp( UserData * pcUserData, PacketLoseExperience * psPacket );
	void										SetExp( UserData * pcUserData, INT64 iExp );
	void										GiveEXP( User * pcUser, INT64 iExp );
	BOOL										GiveGOLD( User * pcUser, int iGold, EWhereAction eWhere = WHEREID_GiveMoney );

	int											SQLCharacterInsert ( SQLConnection * pcDB, char * pszUserID, char * pczCharName, int iSeasonalNum, int iGMLevel, int iOverrideCharacterId = -1 );
	void										CharacterCreate( User * pcUser, PacketCreateCharacter * psPacket );

	void										SendCharacterDataEx( User * pcUser );

	void										OnCharacterSyncData( UserData * pcUserData );
	void										OnCharacterSyncDataEx( UserData * pcUserData, PacketPlayDataEx * psPacket );

	void										OnCharacterUpdateLevelClan( User * pcUser, PacketUpdateClanLevel * psPacket );

	void										OnCharacterData( UserData * pcUserData );
	void										OnOutCharacterData( UserData * pcUserData );

	void										SetPositionWarp( UserData * pcUserData, int iX, int iZ, int iStageID = -1 );

	BOOL										IsCharacterLeader( User * pcUser );

	char										* GetServerName( User * pcUser );

	void										PHCharacterDataEx( User * pcUser );

	int											SQLGetMonsterID( char * pszMonsterName );
	int											SQLGetNPCID( char * pszNPCName, int iMapID = -1 );

	int											SQLGetCharacterID( SQLConnection * pcDB, char * pszName );

	void										HandlePacket( User * pcUser, PacketCharacterDataEx * psPacket );
	void										HandlePacket( User * pcUser, PacketCharacterCombatData * psPacket );

};

