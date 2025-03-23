#pragma once
#include "structaccount.h"


struct SQLUser
{
	int					iID; //db id
	char				szAccountName[32];
	char				szPassword[65];
	int					iFlag;
	int					iActive;
	int					iCoins;
	int					iGold;
	BOOL				bGameMasterType;
	EGameLevel			iGameMasterLevel;
	char				szGameMasterMacAddress[20];
	EBanStatus			iBanStatus;
	SYSTEMTIME 			sUnbanDate;
	BOOL				bIsMuted;
	int					iMuteCount;
	SYSTEMTIME			sUnmuteDate;
};

class AccountServer
{
public:

	static bool								bActive;
	static int								iWaitingAccountLogins;
	static CMutex *							pcMutex;
	static AccountLogin						saAccountLogin[MAX_ACCOUNTLOGINQUEUE];


	AccountServer();
	virtual ~AccountServer();

	BOOL									IsOnlineAccount( UserData * pcUserData, const char * pszUserID );

	BOOL									UnmuteAccountByCharacterName( User * pcGMUser, const char * pszCharacterName, BOOL bReset = FALSE );
	BOOL									GetSQLUserInfoData ( const char * pszAccountName, SQLUser * psSqlUserOut );

	BOOL									IsAdminMacAddress( const char* pszAccountName, const char * pszMacAddress, UINT uSerialHD );
	BOOL									IsIPAccept( const char * pszIP );
	BOOL									IsAccountAccept( const char * pszAccountName );

	BOOL									IsMacBanned( UserData * pcUserData, const char * pszMacAddress );
	BOOL									IsSerialHDBanned( UserData * pcUserData, const char * pszSerialHD );
	BOOL									IsComputerNameBanned( UserData * pcUserData, const char * pszPCName );
	BOOL									IsHardwareIDBanned( UserData * pcUserData, const char * pszHardwareID );

	static void								SQLCharacterLog( char * pszAccountName, int iCharacterClass, char * pszCharacterName, char * pszCode, char * pszIP, char * pszServerName, int iCharacterLevel, INT64 iCharacterExperience, int iCharacterGold, int iAction );
	static void								SQLUserOnline( char * pszAccountName, char * pszCharacterName, char * pszIP, int iTicket, int iAction, int iCharacterClass, int iCharacterLevel );

	BOOL									CanAccessWarehousePage ( User * pcUser, int iPageNum );

	static void								SQLCharacterSave( UserData * pcUserData );

	int										OnLoginSuccess( UserData * pcUserData );


	static bool								AddAccountLogin( User * pcUser, AccountLogin & al );

	//SQL
	static bool								SQLAccountHasFlag( char * pszAccountName, EAccountFlag iFlag );
	static EAccountFlag						SQLAccountGetFlag( char * pszAccountName );
	static BOOL								SQLAccountIsAdmin( char * pszAccountName );

	static void								PHAccountLogin( User * pcUser, PacketLoginUser * psPacket );

	static bool								SQLUnbanAccountId ( int iAccountID );
	static bool								SQLUnbanAccount( char * pszAccountName );

	static BOOL								SQLUnmuteAccountId ( int iAccountID );
	static BOOL								SQLUnmuteAccount ( char * pszAccountName );

	void									SQLSyncBanStatusToCharacterInfo ( SQLConnection * cDB, char * pszAccountName, EBanStatus eBanStatus );

	void									SQLDeleteCharacter ( char * pszCharName );

	static void								ProcessAccountLogin( AccountLogin & al );
	static DWORD WINAPI						ProcessAccountLoginQueue();

	BOOL									SqlIsAccountMuted(std::string szAccountName, int* iMuteCount);
	std::vector<std::string>				SQLGetAccountNotification( const char * pszAccountName );

	void									SelectCharacter(UserData * pcUserData, PacketSelectCharacter * psPacket);
	void									DeleteCharacter( UserData * pcUserData, PacketDeleteCharacter * psPacket );

	void									OnReadPostBox( UserData * pcUserData );
	void									OnDeletePostBox( UINT ItemID );
	void									OnAccountSave( UserData * pcUserData );

	BOOL									AddWarehouseItem( UserData * pcUserData, DropItemData * psItem, BOOL bSeasonalWH );
	BOOL									DeleteWarehouseItem( UserData * pcUserData, DropItemData * psItem, BOOL bSeasonalWH );
	BOOL									FindItemWarehouse( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwCheckSum, BOOL bSeasonalWH );

	BOOL									OnDisconnectUser( UserData * pcUserData );
private:
	const int								iCountCharacterPlay = 6;
	void									LogAccountLogin( UserData * pcUserData, AccountLogin & al, ELogAccount eLogAccount );


	void									SQLDeleteCharacter ( SQLConnection * pcDB, char * pszCharName );
};

