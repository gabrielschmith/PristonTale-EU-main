#include "stdafx.h"
#include "SocketGame.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

std::string SocketGame::m_WorldLoginToken;
std::string SocketGame::m_WorldLoginTokenPass;

SocketGame * SocketGame::pcInstance;

SocketGame::SocketGame()
{
	bActive = false;

	hWnd = NULL;

	pcaSocketData = new SocketData[MAX_CONNECTIONS];

	pcSocketDataL = (SocketData**)0x03A97624;
	pcSocketDataG = (SocketData**)0x03A97620;

	iDisconnectCode = -1;

	bReservePackets = FALSE;
	bHandlingReservedPackets = FALSE;
	AskedPlayDataEx = false;
}

SocketGame::~SocketGame()
{
	bActive = false;

	hWnd = NULL;

	SAFE_DELETE(pcaSocketData);
}

void SocketGame::Load(HWND hWnd)
{
	this->bActive = true;
	this->hWnd = hWnd;

	int iMaxSendPacket = 500;
	int iMaxReceivePacket = 500;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		sd->pcThis = sd;
		sd->bInUse = false;

		sd->Prepare(iMaxSendPacket, iMaxReceivePacket);

		sd->hReceiveThread = CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)SocketGame::Receiver, sd, STACK_SIZE_PARAM_IS_A_RESERVATION );
		sd->hSendThread = CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)SocketGame::Sender, sd, STACK_SIZE_PARAM_IS_A_RESERVATION );
	}

	sPingLogin = MinMax(0, 0);
	sPingGame = MinMax(0, 0);

	bReservePackets = FALSE;
}

void SocketGame::UnLoad()
{
	bReservePackets = FALSE;

	bActive = false;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		SOCKETCLOSE(sd);

		SetEvent(sd->hReceiveThreadSignal);
		WaitForSingleObject(sd->hReceiveThread, INFINITE);
		CloseHandle(sd->hReceiveThread);

		SetEvent(sd->hSendThreadSignal);
		SetEvent(sd->hSendEvent);
		WaitForSingleObject(sd->hSendThread, INFINITE);
		CloseHandle(sd->hSendThread);
	}
}

void SocketGame::Loop()
{
	if (Game::GetGameMode() == GAMEMODE_InGame)
	{
		static bool bFirst = false;
		if (!bFirst)
		{
			if (SOCKETL && SOCKETG)
			{
				ZeroMemory((void*)0x03A46600, 0x8E * 4);
				WRITEDWORD(0x03A975F4, 0);
				WRITEDWORD(0x03A975F8, 0);
				WRITEDWORD(0x03A97648, 0);
				WRITEDWORD(0x03A97664, 0);
				WRITEDWORD(0x03A3E3BC, READDWORD(0x00CF4824));
				WRITEDWORD(0x03A46844, 0);
				WRITEDWORD(0x00CF47C0, 0);
				WRITEDWORD(0x03A976E4, 0);
				WRITEDWORD(0x039C8710, 0);
				WRITEDWORD(0x00A1781C, 0);
				WRITEDWORD(0x03A975F8, -1);
				ZeroMemory((void*)0x039F0308, 0x80 * 4);
				ZeroMemory((void*)0x0397A708, 0x6 * 4);
				ZeroMemory((void*)0x03A46AA0, 0x6 * 4);

				UnitData * ud = UNITDATA;
				if (ud)
				{
					PacketSelectCharacter s;
					s.iLength = sizeof(PacketSelectCharacter);
					s.iHeader = PKTHDR_SelectCharacter;
					s.dwReserved1 = 0;
					s.dwReserved2 = 0;
					s.dwReserved3 = 0;
					STRINGCOPY(s.szCharName, ud->sCharacterData.szName);
					SENDPACKETL(&s);
				}
			}
			bFirst = true;
		}
		else
		{
			CHARACTERGAME->UpdateCharacterPlayData(); //Update Character... 48470013

			if (DISCONNECT_TYPE != (-2))
			{
				if (SOCKETL == NULL)
				{
					if (DISCONNECT_TYPE == -1)
					{
						DISCONNECT_TYPE = 1;
						ChatGame::AddChatBoxTextEx(CHATCOLOR_Error, "Connection lost with %s", "Login Server");
					}
				}
				else if (SOCKETG == NULL)
				{
					if (DISCONNECT_TYPE == -1)
					{
						DISCONNECT_TYPE = 2;
						ChatGame::AddChatBoxTextEx(CHATCOLOR_Error, "Connection lost with %s", "Game Server");
					}
				}
			}
		}

		CALL(0x0061ECF0);
		CALL(0x00408DF0);
#ifdef RECONNECT_TEST
		CheckConnection();
#endif
	}

}

void SocketGame::Tick(ETickType iTickType)
{
	switch (iTickType)
	{
	case TICKTYPE_100MilliSecond:
		PingConnections();
		break;
	}
}


void SocketGame::PHPing(PacketPing * psPacket, SocketData * pcSocketData)
{
	pcSocketData->SetPing(TICKCOUNT - psPacket->dwTime);

	int iPing = pcSocketData->GetPing(TICKCOUNT);

	MinMax & sPing = (pcSocketData == SOCKETL) ? PingLogin() : PingGame();

	if ((iPing < sPing.sMin) || (sPing.sMin == 0))
		sPing.sMin = iPing;

	if ((iPing > sPing.sMax) || (sPing.sMax == 0))
		sPing.sMax = iPing;
}


void SocketGame::PingConnections(bool p_Override)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		if ((sd->bInUse) && (sd->bConnected))
		{
			if (TICKCOUNT > sd->dwPingTimeCoolDown)
			{
				sd->dwPingTimeCoolDown = TICKCOUNT + 3000;

				//DEBUGINFO("PING %d, IS SOCKET = %d", TICKCOUNT, sd == SOCKETL);

				//Keep Alive every 3 seconds
				sd->Ping(TICKCOUNT);

				MinMax& sPing = (sd == SOCKETL) ? PingLogin() : PingGame();
				sPing.sMin = sPing.sMax = sd->GetPing(TICKCOUNT);
			}
		}
	}
}

char * SocketGame::GetServerTypeName(SocketData * sd, int iOptionalCode)
{
	static char szLoginServer[] = "Login Server";
	static char szGameServer[] = "Game Server";
	static char szUnknown[] = "Unknown";

	if ((sd == SOCKETL) || (iOptionalCode == 1))
		return szLoginServer;
	else if ((sd == SOCKETG) || (iOptionalCode == 2))
		return szGameServer;

	return szUnknown;
}

BOOL SocketGame::IsReservePackets()
{
	return bReservePackets;
}

void SocketGame::SetReservePackets(BOOL b)
{
	//DEBUG( "SetReservePackets( BOOL %s ) (current: %s)", b ? "true" : "false", bReservePackets ? "true" : "false" );

	bReservePackets = b;
}

BOOL SocketGame::IsHandlingReservedPackets()
{
	return bHandlingReservedPackets;
}

void SocketGame::HandlePacket(SocketData * sd, PacketReceiving * p)
{
	Packet * psPacket = (Packet*)p->baPacket;
	int len = psPacket->iLength;

	if ( len <= MAX_PKTSIZ)
	{
		PACKETHANDLER->AnalyzePacket( sd, psPacket );
		/* TEMPORARILY */
		static char * packettemp = NULL;
		if ( packettemp == NULL )
			packettemp = new char[0x202C];
		char * pointertotmp = packettemp;
		memcpy( packettemp + 0x2C, psPacket, len );
		WRITEDWORD( pointertotmp, sd );
		if ( psPacket->iHeader == PKTHDR_KeySet )
		{
			//    sd->KeyObfuscatorSet( ((PacketSendKey*)&p->sPacket)->bKey );
		}
		else
		{
			__asm
			{
				PUSHAD;
				MOV EAX, packettemp;
				PUSH EAX;
				MOV EAX, 0x00632A50;
				CALL EAX;
				ADD ESP, 4;
				POPAD;
			}
		}
		PACKETHANDLER->OnPostProcessedPacket( sd, psPacket );
	}
	sd->dwTimeLastPacketReceived = TICKCOUNT;
	sd->FreePacketReceiving(p);
}

void SocketGame::SocketPacket(SocketData * sd, PacketReceiving * p)
{
	if (bReservePackets)
	{
		//DEBUG( "Reserve Packet (%d) (%d) [%d][0x%08X] [%s]", p->bInUse, p->bDelete, ((Packet *)p->baPacket)->iLength, ((Packet *)p->baPacket)->iHeader, sd->szIP );

		//Create new Reserved Packet
		PacketReserved * pr = new PacketReserved(sd, p);

		//Add Reserved Packet to Queue
		vReservedPackets.push_back(pr);
	}
	else
	{
		//Any Reserved Packets?
		if (vReservedPackets.size() > 0)
		{
			//DEBUG( "Reserved Packets count: %d", vReservedPackets.size() );

			//Set Handling of Reserved Packets is in Progress
			bHandlingReservedPackets = TRUE;

			//Handle each Reserved Packet
			for (vector<PacketReserved*>::iterator it = vReservedPackets.begin(); it != vReservedPackets.end(); it++)
			{
				PacketReserved * pr = *it;

				//Handle Reserved Packet
				HandlePacket(pr->pSocketData, pr->pPacketReceiving);

				//Delete Reserved Packet
				delete pr;
			}

			//Clear Reserved Packet Queue
			vReservedPackets.clear();

			//Done Handling Reserved Packets
			bHandlingReservedPackets = FALSE;

			//DEBUG( "Reserved Packets cleared" );
		}

		//Handle Packet
		HandlePacket(sd, p);
	}
}

void SocketGame::SetIPPort(const char * pszIP, int iPort)
{
	char * ip = (char*)pszIP;
	if ((pszIP[0] < '0') || (pszIP[0] > '9'))
	{
		hostent * record = gethostbyname(pszIP);
		in_addr * address = (in_addr *)record->h_addr;
		ip = inet_ntoa(*address);
	}
	// IP
	STRINGCOPYLEN((char*)0x0CDE160, 16, ip);
	STRINGCOPYLEN((char*)0x0CDE13C, 16, ip);
	STRINGCOPYLEN((char*)0x0CDE1A8, 16, ip);
	STRINGCOPYLEN((char*)0x0CDE184, 16, ip);
	PORT_GAME = iPort;
	// PORT
	*(UINT*)0x0CDE180 = PORT_GAME;
	*(UINT*)0x0CDE15C = PORT_GAME;
	*(UINT*)0x0CDE1A4 = PORT_GAME;
	*(UINT*)0x0CDE1C8 = PORT_GAME;
}


void SocketGame::SocketClose(SocketData * sd)
{
	if ((sd) && (sd->bInUse))
	{
		DEBUG("Close %s:%d", sd->szIP, sd->iPort);

		if (sd == SOCKETL)
			*(DWORD*)0x03A97624 = NULL;
		if (sd == SOCKETG)
			*(DWORD*)0x03A97620 = NULL;

		while (sd->bConnecting)
			Sleep(2500);

		sd->Close();
		sd->UnInit();
	}
}

SocketData * SocketGame::GetFreeSocketData()
{
	SocketData * r = NULL;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		if (!sd->bInUse)
		{
			sd->Init();

			r = sd;
			break;
		}
	}

	return r;
}

SocketData * SocketGame::GetSocketData(const char * pszIP, int iPort)
{
	SocketData * r = NULL;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		if (sd->bInUse)
		{
			if ((sd->iPort == iPort) && (lstrcmpiA(sd->szIP, pszIP) == 0))
			{
				r = sd;
				break;
			}
		}
	}

	return r;
}

void SocketGame::ConnectServerL(const char * pszIP, int iPort)
{
	CloseServerL();

	SetSocketL(Connect(pszIP, iPort));
}

void SocketGame::ConnectServerG(const char * pszIP, int iPort)
{
	CloseServerG();

	SetSocketG(Connect(pszIP, iPort));
}

void SocketGame::CloseServerL()
{
	if (SOCKETL != NULL)
	{
		SOCKETL->Close();
		SOCKETL->UnInit();

		SetSocketL(NULL);
	}
}

void SocketGame::CloseServerG()
{
	if (SOCKETG != NULL)
	{
		SOCKETG->Close();
		SOCKETG->UnInit();

		SetSocketG(NULL);
	}
}

void SocketGame::CloseConnections()
{
	CloseServerL();
	CloseServerG();
}

int SocketGame::GetConnectionsAlive()
{
	int ret = 0;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		SocketData * sd = pcaSocketData + i;

		if (sd->bInUse)
			ret++;
	}

	return ret;
}

SocketData * SocketGame::Connect(const char * pszIP, int iPort)
{ //conn thread: 521230
	SocketData * sd = NULL;

	if ((sd = GetInstance()->GetSocketData(pszIP, iPort)) == NULL)
	{
		sd = GetInstance()->GetFreeSocketData();

		if (sd)
		{
			if (!sd->Connect(pszIP, iPort))
			{
				sd->UnInit();

				return NULL;
			}

			if (iPort == 10009)
				SocketGame::GetInstance()->SetSocketL(sd);
			else
			{
				SocketGame::GetInstance()->SetSocketG(sd);
			}
		}
	}
	else
	{
		PacketReceiving * p = new PacketReceiving;
		p->bInUse = TRUE;
		p->bDelete = TRUE;
		//Connection already Confirmed
		PacketVersion sPacket;
		sPacket.iLength = sizeof(PacketVersion);
		sPacket.iHeader = PKTHDR_Version;
		sPacket.bServerFull = FALSE;
		sPacket.iUnk2 = 0;
		sPacket.iVersion = READDWORD(0x04AF7FFC);
		CopyMemory(p->baPacket, &sPacket, sizeof(PacketVersion));
		SOCKETGAME->SocketPacket(sd, p);
	}

	return sd;
}

DWORD WINAPI SocketGame::Receiver(SocketData * sd)
{
	SocketGame * m = GetInstance();

	HWND hWnd = m->GetHWND();
	do
	{
		WaitForSingleObject(sd->hReceiveThreadSignal, INFINITE);

		if (!m->Active())
			break;

		DEBUG("[Receiver] Active (%s:%d)", sd->szIP, sd->iPort);

		PacketReceiving * p;
		while ((p = sd->ReceivePacket()) != NULL)
		{
			//Post Packet Message
			WNDPROC_SOCKETPACKET(hWnd, sd, p);
		}

		sd->bBlockSend = TRUE;

		DEBUG("[Receiver] Done");

		if (m->Active())
		{
			if (sd->bConnected)
			{
				//Post Close Message
				WNDPROC_SOCKETCLOSE(hWnd, sd);
			}
		}

		DEBUG("[Receiver] Wait (%d) (%d)", sd->iRecvRet, sd->iRecvEC);
	} while (m->Active());

	return TRUE;
}

DWORD WINAPI SocketGame::Sender(SocketData * sd)
{
	SocketGame * m = GetInstance();

	DWORD dwTime = TICKCOUNT + 1000;

	do
	{
		WaitForSingleObject(sd->hSendThreadSignal, INFINITE);

		if (!m->Active())
			break;

		DEBUG("[Sender] Active (%s:%d)", sd->szIP, sd->iPort);

		PacketSending * p;
		while ((p = sd->NextPacketSending()) != NULL)
		{
			if (sd->Send(p) == FALSE)
			{
				sd->FreePacketSending(p);
				break;
			}

			sd->FreePacketSending(p);
		}

		DEBUG("[Sender] Done");
		DEBUG("[Sender] Wait (%d) (%d)", sd->iSendRet, sd->iSendEC);
	} while (m->Active());

	return TRUE;
}

BOOL WINAPI SocketGame::CheckConnection()
{
	static DWORD dwTimeCheckL = 0;
	static DWORD dwTimeCheckG = 0;
	//Login Server
	if (SOCKETGAME)
	{
		SocketData* sd = SOCKETL;
		if (sd)
		{
			if (SOCKETGAME->szLoginIP[0] == 0)
			{
				STRINGCOPY(SOCKETGAME->szLoginIP, sd->szIP);
				SOCKETGAME->iLoginPort = sd->iPort;
			}
			if ((sd->dwTimeLastPacketReceived + 20000) < TICKCOUNT)
			{
#ifdef RECONNECT_SYSTEM
				SOCKETGAME->bLoginServerReconnect = TRUE;
#else
				DISCONNECT_TYPE = -1;
				DISCONNECT_TIME = TICKCOUNT;
#endif
			}
		}
		else
		{

#ifdef RECONNECT_SYSTEM
			SOCKETGAME->bLoginServerReconnect = TRUE;
#endif
		}
	}
	//Game Server
	if (SOCKETGAME)
	{
		SocketData* sd = SOCKETG;
		if (sd)
		{
			if (SOCKETGAME->szGameIP[0] == 0)
			{
				STRINGCOPY(SOCKETGAME->szGameIP, sd->szIP);
				SOCKETGAME->iGamePort = sd->iPort;
			}
			if ((sd->dwTimeLastPacketReceived + 20000) < TICKCOUNT)
			{
#ifdef RECONNECT_SYSTEM
				SOCKETGAME->bGameServerReconnect = TRUE;
#else 
				DISCONNECT_TYPE = -1;
				DISCONNECT_TIME = TICKCOUNT;
#endif
			}
		}
		else
		{
#ifdef RECONNECT_SYSTEM
			SOCKETGAME->bGameServerReconnect = TRUE;
#endif
		}
	}

#ifdef RECONNECT_SYSTEM
	if (SOCKETGAME->bLoginServerReconnect)
	{
		if (dwTimeCheckL < TICKCOUNT)
		{
			if (SOCKETL)
			{
				if (SOCKETL->bConnected)
					SOCKETGAME->SocketClose(SOCKETL);
				SOCKETGAME->SetSocketL(NULL);
			}
			#
			SOCKETGAME->iReconnectLCOUNT++;
			if (SOCKETGAME->iReconnectLCOUNT > 10)
			{
				DISCONNECT_TYPE = -1;
				DISCONNECT_TIME = TICKCOUNT;
			}
			else
			{
				CHATGAMEHANDLER->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Reconnecting to Login Server (%d/10)", SOCKETGAME->iReconnectLCOUNT);
				SocketData* sd = SOCKETGAME->Connect(SOCKETGAME->szLoginIP, SOCKETGAME->iLoginPort);
				SOCKETGAME->SetSocketL(sd);
				if (sd)
				{
					//Send Reconnect Packet
					PacketReconnectUser s;
					s.iLength = sizeof(PacketReconnectUser);
					s.iHeader = PKTHDR_Reconnect;
					s.dwObjectID = UNIT->iID;
					s.iUnk = 0;
					STRINGCOPY(s.szUserID, UNITDATA->sCharacterData.szName);
					SENDPACKETL(&s);
				}
			}
			dwTimeCheckL = TICKCOUNT + 5000;
		}
	}
	if (SOCKETGAME->bGameServerReconnect)
	{
		if (dwTimeCheckG < TICKCOUNT)
		{
			if (SOCKETG)
			{
				if (SOCKETG->bConnected)
					SOCKETGAME->SocketClose(SOCKETG);
				SOCKETGAME->SetSocketG(NULL);
			}
			SOCKETGAME->iReconnectGCOUNT++;
			if (SOCKETGAME->iReconnectGCOUNT > 10)
			{
				DISCONNECT_TYPE = -1;
				DISCONNECT_TIME = TICKCOUNT;
			}
			else
			{
				CHATGAMEHANDLER->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Reconnecting to Game Server (%d/10)", SOCKETGAME->iReconnectGCOUNT);
				SocketData* sd = SOCKETGAME->Connect(SOCKETGAME->szGameIP, SOCKETGAME->iGamePort);
				SOCKETGAME->SetSocketG(sd);
				if (sd)
				{
					//Send Reconnect Packet
					PacketReconnectUser s;
					s.iLength = sizeof(PacketReconnectUser);
					s.iHeader = PKTHDR_Reconnect;
					s.dwObjectID = UNIT->iID;
					s.iUnk = 0;
					STRINGCOPY(s.szUserID, UNITDATA->sCharacterData.szName);
					SENDPACKETG(&s);
				}
			}
			dwTimeCheckG = TICKCOUNT + 5000;
		}
	}
#endif

	return TRUE;
}

void SocketGame::SetDisconnectCode(int i)
{
	iDisconnectCode = i;

	BOOL bDC = (i >= 0) ? TRUE : FALSE;

	DISCONNECT_TIME = TICKCOUNT;
	DISCONNECT_TYPE = i;
}
