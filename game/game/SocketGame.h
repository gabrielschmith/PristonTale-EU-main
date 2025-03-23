#pragma once

#include <string>

class SocketData;

struct PacketReceiving;
struct PacketSending;

#define SOCKETGAME					(SocketGame::GetInstance())

#define SOCKETL						SOCKETGAME->GetSocketL()
#define SOCKETG						SOCKETGAME->GetSocketG()

#define SENDPACKETL					SOCKETL->SendPacket
#define SENDPACKETG					SOCKETG->SendPacket
#define SENDCOMMANDL				SOCKETL->SendPacketCommand
#define SENDCOMMANDG				SOCKETG->SendPacketCommand
#define SENDBLANKG					SOCKETG->SendPacketBlank
#define SENDBLANKL					SOCKETL->SendPacketBlank

#define SOCKETPACKET(s,p)			SocketGame::GetInstance()->SocketPacket(s,p);
#define SOCKETCLOSE(s)				SocketGame::GetInstance()->SocketClose(s);

#define MAX_CONNECTIONS				2

#define SOCKETSEND_DEBUGCOUNT		(*(DWORD*)0x03A97600)

#define SOCKET_NETSTANDCOUNT		(*(DWORD*)0x03A97794)

#define SOCKET_RECEIVEGAMESERVER	(*(BOOL*)0x009CA9C8)

class SocketGame
{
private:
	static SocketGame				* pcInstance;

	bool							  bActive;
	HWND							  hWnd;

	SocketData						* pcaSocketData;

	SocketData						**pcSocketDataL;
	SocketData						**pcSocketDataG;

	int								  iDisconnectCode;

	BOOL							  bReservePackets;
	BOOL							  bHandlingReservedPackets;
	std::vector<struct PacketReserved*>	  vReservedPackets;

	MinMax								sPingLogin;
	MinMax								sPingGame;

	static std::string m_WorldLoginToken;
	static std::string m_WorldLoginTokenPass;

public:
	bool AskedPlayDataEx;

	static void						  CreateInstance() { pcInstance = new SocketGame(); }
	static SocketGame				* GetInstance() { return pcInstance; }
	static void						  DeleteInstance() { delete pcInstance; }

	static void SetWorldLoginToken(char * p_Data)		{ m_WorldLoginToken		= p_Data; }
	static void SetWorldLoginTokenPass(char * p_Data)	{ m_WorldLoginTokenPass = p_Data; }

	SocketGame();
	virtual							 ~SocketGame();

	void							  Load(HWND hWnd);
	void							  UnLoad();

	void							  Loop();
	void							  Tick(ETickType iTickType);
	void							  PHPing(PacketPing * psPacket, SocketData * pcSocketData);

	void							  PingConnections(bool p_Override = false);

	char							* GetServerTypeName(SocketData * sd, int iOptionalCode = 0);

	BOOL							  IsReservePackets();
	void							  SetReservePackets(BOOL b);
	BOOL							  IsHandlingReservedPackets();

	void							  HandlePacket(SocketData * sd, PacketReceiving * p);

	void							  SocketPacket(SocketData * sd, PacketReceiving * p);

	void								SetIPPort(const char * pszIP, int iPort);

	void							  SocketClose(SocketData * sd);

	SocketData						* GetFreeSocketData();
	SocketData						* GetSocketData(const char * pszIP, int iPort);

	void							  ConnectServerL(const char * pszIP, int iPort);
	void							  ConnectServerG(const char * pszIP, int iPort);
	void							  CloseServerL();
	void							  CloseServerG();
	void							  CloseConnections();

	int								  GetConnectionsAlive();
	HWND							  GetHWND() { return hWnd; }

	void							  Active(bool b) { bActive = b; }
	bool							  Active() { return bActive; }

	static SocketData				* Connect(const char * pszIP, int iPort);
	static DWORD WINAPI				  Receiver(SocketData * sd);
	static DWORD WINAPI				  Sender(SocketData * sd);
	static BOOL WINAPI				  CheckConnection();

	int								  GetDisconnectCode() { return iDisconnectCode; }
	void							  SetDisconnectCode(int i);

	inline void						  SetSocketL(SocketData * p) { *pcSocketDataL = p; }
	inline void						  SetSocketG(SocketData * p) { *pcSocketDataG = p; }
	inline SocketData				* GetSocketL() { return *pcSocketDataL; }
	inline SocketData				* GetSocketG() { return *pcSocketDataG; }

	inline MinMax					& PingLogin() { return sPingLogin; }
	inline MinMax					& PingGame() { return sPingGame; }

	inline int						  GetAveragePingLogin() { return (sPingLogin.sMin + sPingLogin.sMax) >> 1; }
	inline int						  GetAveragePingGame() { return (sPingGame.sMin + sPingGame.sMax) >> 1; }

	DWORD dwLastTickLogin = 0;
	DWORD dwLastTickGame = 0;

	int iReconnectLCOUNT = 0;
	int iReconnectGCOUNT = 0;

	BOOL bLoginServerReconnect = FALSE;
	BOOL bGameServerReconnect = FALSE;

	char szLoginIP[32] = { 0 };
	int iLoginPort = 0;

	char szGameIP[32] = { 0 };
	int iGamePort = 0;

	SocketData* pcSocketLogin = nullptr;
	SocketData* pcSocketGame = nullptr;

	int iDisconnectType = 0;
	int iDisconnectTime = 0;
};