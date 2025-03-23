#pragma once

#include "userdataserver.h"
#include "character.h"



enum EWhereAction : int
{
	WHEREID_None,
	WHEREID_Trade			= 300,
	WHEREID_Distributor		= 600,
	WHEREID_Aging			= 400,
	WHEREID_AgingHalfPrice	= 401,
	WHEREID_MixItem			= 420,
	WHEREID_RespecItem		= 421,
	WHEREID_ForceOrb		= 440,
	WHEREID_LearnSkill		= 460,
	WHEREID_WarpGate		= 470,
	WHEREID_BuyItem			= 200,
	WHEREID_BuyPotion		= 210,
	WHEREID_SellItem		= 220,
	WHEREID_LoadPlayer		= 100,
	WHEREID_DropItem		= 120,
	WHEREID_GetItem			= 130,
	WHEREID_StarPoint		= 620,
	WHEREID_ClanMoney		= 630,
	WHEREID_GiveMoney		= 640,
	WHEREID_GiveMoneyShared	= 641,
	WHEREID_PersonalShop	= 320,
	WHEREID_OpenWarehouse   = 500,
	WHEREID_QuestGold		= 501,
	WHEREID_FuryArena		= 502,

    /// New
    WHEREID_RepairItem          = 10000,
    WHEREID_DropItemRestoreOk   = 10001,
    WHEREID_Wings               = 10002,
    WHEREID_BellatraSOD			= 10003,
	WHEREID_EventGirl			= 10004,
	WHEREID_NeverSinkTeleport	= 10005
};

class UserServer
{
public:

	User										**pcaUserInGame = NULL;
	UINT										uUsersInGame = 0;
	UserData									* pcaUserData = NULL;

												UserServer();
	virtual									   ~UserServer();


	UINT										GetSocket();
	UINT										GetID();

	static void									CreateData();

	static User *								GetFreeUser();
	User *										UserdatatoUser( UserData * pcUserData );
	static User *								GetUserByIndex( UINT uIndex );

	static UserData *							GetUserdata( int iEnemyID );
	static UserData *							GetUserdata( const char * pszCharacterName );
	static UserData *							GetUserdataByAccount( const char * pszAccountName );
	UserData *									GetUserdataSocket( SocketData * pcSocketData );
	int											GetTicket();

	void										CheckAndActivateGameMasterStatus ( User * pcUser );
	BOOL										SetAccountNameFromSQL( UserData * pcUserData );

	//SQL
	static int									SQLGetAccountID( const char * pszAccountName );

	void										CancelForceOrb( User * pcUser );

	void										ReconnectUser( UserData * pcUserData );
	void										UpdateUnitStatus( User * pcUser );

	void										Loop();
	void										LoopUsers( User * pcUser );

	void										SendUnitStatus( User * pcUser, Packet * psPacket, BOOL bIncrementAmount = TRUE );

	void										ClearUnitStatus( User * pcUser );

	void										SendUnitStatusM( User * pcUser, Packet * psPacket, BOOL bIncrementAmount = TRUE );

	void										SendDamageInfoAndClearBuffer ( User * pcUser );

	void										OnLeaveMap( User * pcUser, int iOldMapId );

	void										OnGetPlayerInfoData( User * pcUser, PacketSimple * psPacket );
	void										SendSkillBuffStatus( User * pcUser, User * pcOtherUser );
	void										SendSkillBuffStatusToAllUsersinRange( User * pcUser );
	BOOL										UpdateSkillBuffHash( User * pcUser );

	BOOL										CheckAndUpdateSkillStatus( User * pcUser, ESkillID eSkillID, time_t & uEndTime, BYTE & bLevel1Based, USHORT & sSyncTimeLeft, BYTE & bSyncLevel1Based, BOOL bForceExpire );
	void										UpdateSkillBuffStatus( User * pcUser, ESkillID eSkillID, time_t uEndTime, BYTE bLevel1Based, int iParameter = 0 );

	void										SendAndClearUnitStatusM( User * pcUser );

	void										LoopUnits( User * pcUser );

	void										SendUserMiscCommandToOtherNearbyPlayers ( User * pcUser, EUnitDataMiscCommand eCommand, int iArg1 = -1, int iArg2 = -1 );

	static void									OnHandlerFunctions( DWORD dwCode, DWORD * pdwOptional );

	BOOL										AddServerUserGold( UserData * pcUserData, int iGold, EWhereAction eWhere = WHEREID_None );

	BOOL										SubServerUserGold( UserData * pcUserData, int iGold, EWhereAction eWhere = WHEREID_None );

	DWORD										GetCharacterWarpWingTier( UserData * pcUserData );
	DWORD										GetCharacterTier( UserData * pcUserData );
	DWORD										GetQuestLogBit( UserData * pcUserData );
	BOOL										InventoryServerCheck( UserData * pcUserData, int iUnused );

	BOOL										AddUserOnline( UserData * pcUserData, BOOL bOffline );
	BOOL										OnTradeRequest( UserData * pcUserData, PacketSimple * psPacket );

	BOOL										OnLoadUser( User * pcUser );
	BOOL										OnUnLoadUser( User * pcUser );

	void										ResetSkills( User * pcUser );

	void										UpdateSkillStatus ( User * pcUser );
	void										UpdateSkillBuffStatus ( User * pcUser );
	void										SetSkillTime ( short & sPacketLevel, short & sPacketTime, UINT & uUserLevel, time_t & uUserTimeOut );

	void										HandlePacket( User * pcUser, PacketPlayData * psPacket );

	void										OnKilled( User * pcUser, PacketDeadCharacter * psPacket );

	void										SettingsDataSet( User * pcUser, PacketSettingsData * psPacket );

	void										UpdatePetData( User * pcUser, PacketUpdatePetData * psPacket );
	void										UpdatePetDataEx( User * pcUser, PacketUpdatePetDataEx * psPacket );

	void										DisconnectOldUser( User * pcUser );

	void										ReconnectUser(UserData* pcUser, struct PacketReconnectUser * psPacket );
	BOOL										DeleteReconnectUser( const char * pszAccountName );

	static DWORD								AllowPacket( User * pcUser );

	void										Update();
	static DWORD WINAPI							UsersUpdate();

	void										SendServerMessages ( User * pcUser );
	void										SyncUserMuteStatus( User * pcUser );
	void										UpdateCharacterIDFromSQL( User * pcUser );

	int											GetNewUserID();
};


