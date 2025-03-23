#pragma once

#include <set>
#include <map>
#include <queue>
#include <string>
#include <mutex>

#define MAX_RECONNECTCONNECTIONS 128

#define MAX_SOCKET_ON_SAME_IP							15

#define MAX_DISCONNECT_TICK								15
#define MAX_DISCONNECT_TIME								(10*1000)

#define MAX_PACKETFLOW 400
#define TIM_PACKETFLOW 2000
#define MAX_PACKETBYTESFLOW			SocketServer::uBytePacketFlow

#define SOCKETACCEPT(s,a)			SocketServer::GetInstance()->SocketAccept(s,a);
#define SOCKETPACKET(s,p)			SocketServer::GetInstance()->SocketPacket(s,p);
#define SOCKETCLOSE(s)				SocketServer::GetInstance()->SocketClose(s);

enum class EAllowPacketResult : BYTE
{
	Allow,
	BytesRateLimitExeeded,
};

struct DisconnectedUser
{
	UINT						lIP;
	int							iDisconnectTimes;
	DWORD						dwTime;
};

struct BanIPElement
{
	DWORD IP;
	const char * Reason;
};

struct PacketStat
{
	int iCounter;
	float fTotalDuration;
	float fMaxDuration;
	float fMinDuration;
};

class SocketServer
{
protected:
	HWND												hWnd;
	WSADATA												sWsaData;
	SOCKET												sListenSocket = INVALID_SOCKET;
	int													iMaxConnections;
	BOOL												bActive = FALSE;
	BOOL												bSetFull = FALSE;
	SocketData											* pcaSocketData;
	CMutex												* pcMutex = NULL;

	HANDLE												hEventSocket;

	BYTE												bPacketObfuscatorByte = 0;

	std::mutex m_DisconnectionHistoryMutex;
	std::map<DWORD, DisconnectedUser> m_DisconnectionHistory;

	std::queue<BanIPElement> m_IPsToBan;
	std::set<DWORD> m_BanIPs;

	time_t uNextPacketLogTime;
	std::map<DWORD, PacketStat> m_PacketReceiveStat;

	std::map<DWORD, std::string> m_OpcodesNames;
	std::map<DWORD, std::string> m_OpcodesNamesUnk;
	UserData* reconnectedUsers[MAX_RECONNECTCONNECTIONS];

public:

    const std::string & GetOpcodeName(DWORD p_Opcode)
    {
        auto l_It = m_OpcodesNames.find(p_Opcode);

        if (l_It != m_OpcodesNames.end())
            return l_It->second;

        const static std::string l_NullString;

        return l_NullString;
    }

	//note - don't return a reference to a std::string here because
	//the strings are created locally here (including for unknown opcode).
    const std::string GetOpcodeNameOrDefault(DWORD p_Opcode)
    {
		std::string l_OpcodeName = GetOpcodeName(p_Opcode);
        if (l_OpcodeName.empty())
        {
            char l_HexString[16];
			_itoa_s(p_Opcode, l_HexString, 16);
            return "0x" + std::string(l_HexString);
        }

        return l_OpcodeName;
    }


	static UINT											uBytePacketFlow;

	HANDLE												hListener;
	static SocketServer									* pcInstance;
	static SocketServer									* GetInstance() { return SocketServer::pcInstance; };
	CMutex												* GetMutex() { return pcMutex; };
	static void											CreateInstance() { SocketServer::pcInstance = new SocketServer(); };
	HWND												GetHWND() { return hWnd; };
	BOOL												IsActive() { return bActive; };
	BOOL												IsFull() { return bSetFull; };
	void												SetFull(BOOL b) { bSetFull = b; };

	SocketData											* GetFreeSocketData();
	int													GetNumFreeSlots();

	SocketData											* GetSocketData(const char * pszIP, int iPort);

	int													GetConnectionCount(UINT lIP);
	void												DisconnectIP(UINT lIP);

	SocketData											* Connect(const char * pszIP, int iPort);
	void												SocketAccept(SOCKET s, sockaddr_in * addr);
	void												SocketPacket(SocketData * sd, PacketReceiving * p);
	void												SocketClose(SocketData * sd);

	void												LogPacketTimes();

	void												AddReconnectUser(UserData* pcUser);

	SocketServer();
	virtual ~SocketServer();

	static DWORD WINAPI									Sender(SocketData * sd);
	static DWORD WINAPI									Receiver(SocketData * sd);
	static DWORD WINAPI									Listener(SOCKET * ls);
	static DWORD WINAPI									DeniedSocketsHandler();

	void												HandleReconnectedUsers();
	void												HandleBanFirewall();
	void												HandleConnectionCount();

	static EAllowPacketResult							AllowPacket(SocketData * sd);

	void												AddBanIP(DWORD dwIP, const char * p_Reason);
	BOOL												IsBannedIP(UINT lIP);
	void												AddDisconnectedUser(UINT lIP);

	void												Load();

	BOOL												Init(HWND hwnd);

	void												Listen(int iPort);

	void												OnReadHandler(SOCKET s, DWORD dwParam);

	HANDLE												GetHandleEventSocket() { return hEventSocket; }

	BYTE												GetPacketObfuscatorByte() { return bPacketObfuscatorByte; }
	void												SetPacketObfuscatorByte(BYTE b) { bPacketObfuscatorByte = b; }
};

#define SOCKETMUTEX										(SocketServer::GetInstance()->GetMutex())
