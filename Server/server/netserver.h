#pragma once

#include <string>
#include <map>
#include <mutex>

struct NetConnection
{
	class SocketData							* pcSocketData;
	int											iServerType;
	char										szIP[32];
	int											iPort;
};


class NetServer
{

protected:
	BOOL											bInit			= FALSE;
	WORD											wTimeMinute		= 0;
	BOOL											bReceivedBack	= FALSE;
private:




	enum
	{

	};
	WSADATA										wsaData;
	SOCKET										SocketData = INVALID_SOCKET;

	std::vector<NetConnection*>					vNetConnections;

	void										OnReceiveFromGameServer( UserData * pcUserData, void * pPacket );
	void										OnReceiveClient( UserData * pcUserData, void * pPacket );

	std::mutex									m_PlayerWorldLoginTokenMutex;
	std::map<std::string, std::string>			m_PlayerWorldLoginToken;

public:



	NetServer();
	virtual ~NetServer();

	NetConnection								* GetLoginServerConnection();

	void										SendCommandToLoginServer ( enum PacketsHeader ePacketHeaderId, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0, int iValue4 = 0 );
	void										SendGMCommandToLoginServer ( User * pcUser, std::string chat );
	void										SendPacketToLoginServer ( void * packet );
	void										SendPacket( class SocketData * pcSocket, void * packet );
	void										SendCommandGameServers( enum PacketsHeader ePacketHeaderId, int iValue1 = 0, int iValue2 = 0, int iValue3 = 0, int iValue4 = 0 );
	void										SendPacketGameServers( void * packet );

	BOOL										Disconnected( class SocketData * pcSocketData );
	void										Reconnect( class SocketData * pcSocketData );
	void										Connect( NetConnection * nc );
	void										AddConnection( char * pszIP, int iPort, int iServerType );
	NetConnection *								AddNetConnection( class SocketData * pcSocketData );

	void										ConnectAll();
	void										DisconnectAll();

	void										SyncEventStatusToLoginServer ( enum PacketsHeader netHeader, BOOL enabled );
	void										SetAgingEvent(BOOL bFreeAging, BOOL bNoBreak, BOOL bHalfPrice);
	void										SendBotAdd( void * pPacket );
	void										SendExp( UserData * pcUserData, INT64 iExp );
	void										GiveEXP( UserData * pcUserData, INT64 iExp );
	void										SendExpLogin( UserData * pcUserData, INT64 iExp );
	void										SendLoginUserMiscDataToGameServers ( User * pcUser );
	void										SendForceOrbToLoginServer( UserData * pcUserData, BOOL bCity, int iForceOrbTime );
	void										SendEXPPotion( UserData * pcUserData, int iEXPPotion );
	void										SendClan( UserData * pcUserData );
	void										SendSyncChar( UserData *pcUserData );
	void										GetQuestDataFromLoginServer( UserData * pcUserData );
	void										SyncGameMaster( UserData * pcUserData );
	void										OnGetGameMaster( UserData * pcUserData );
	void										LoadQuestData( User * pcUser );
	void										LoadPVPData( User * pcUser );
	void										DisconnectUser( int iUserId );
	void										DisconnectUser( UserData * pcUserData );
	void										OnUnloadUser( UserData * pcUserData );
	void										GetExp( UserData * pcUserData );
	void										SendVersion( int iVersion );
	void										SendTradeCoin(int iID, int iReceiverID);
	void										SendQuestData( User * pcUser );
	void										SendQuestCancelOrFinish( User * pcUser, int iQuestID, BOOL bForceFinish );
	void										SendQuestUpdateDataPart( User * pcUser, PacketQuestUpdateData * data, BOOL bUpdateNPCQuestIcon );
	void										SendQuestUpdateData( User * pcUser );

	NetConnection								* GetNetConnectionBySocketData( class SocketData * sd );

	void										OnReceivePacket( User * pcUser, void * pPacket );

	void AddWorldConnectAllowance(const std::string & p_TokenHash, const std::string & p_TokenPassHash);
	bool UsePlayerWorldLoginToken(const std::string & p_TokenHash, const std::string & p_TokenPassHash);

	void										Init();


	void										Tick();
};

