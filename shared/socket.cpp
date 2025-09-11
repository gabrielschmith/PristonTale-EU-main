#include "stdafx.h"
#include "socket.h"
#include <mutex>
#include <map>
#include <stdint.h>

const BOOL SOCKET_NODELAY = TRUE;

const int RCVWND_SIZE = 0x800000;
const int SNDWND_SIZE = 0x800000;



static std::map<SOCKET, SocketDataContext*> m_SocketDataContext;

static SocketDataContext* FindSocketDataContext(SOCKET p_SocketData)
{
	auto l_It = m_SocketDataContext.find(p_SocketData);

	if (l_It == m_SocketDataContext.end())
	{
		auto l_New = new SocketDataContext();
		m_SocketDataContext[p_SocketData] = l_New;

		return l_New;
	}

	return l_It->second;
}

SocketData::SocketData()
{
	bInUse = false;
	bConnecting = false;
	bConnected = false;
	bKeySet = false;

	auto l_Context = GetSocketDataContext();
	l_Context->InterServerAuthed = false;

	//Sending
	iDefSendPacket = 0;
	iMaxSendPacket = 0;
	iSendWheel = 0;
	iSendingWheel = 0;
	psaPacketSending = NULL;

	bBlockSend = TRUE;

	dwBulkThreadId = 0;
	sBulkPacket.bInUse = FALSE;

	//Receiving
	iDefReceivePacket = 0;
	iMaxReceivePacket = 0;
	iReceiveWheel = 0;
	psaPacketReceiving = NULL;

	hReceiveThreadSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
	pcReceiveMutex = new CMutex("Socket Receive");
	hSendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hSendThreadSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
	pcSendMutex = new CMutex("Socket Send");
}

SocketData::~SocketData()
{
	SAFE_DELETE(pcSendMutex);
	CloseHandle(hSendThreadSignal);
	CloseHandle(hSendEvent);
	SAFE_DELETE(pcReceiveMutex);
	CloseHandle(hReceiveThreadSignal);

	SAFE_DELETE_ARRAY(psaPacketReceiving);

	SAFE_DELETE_ARRAY(psaPacketSending);
}

void SocketData::Prepare(UINT iMaxSendPacket, UINT iMaxReceivePacket)
{
	SAFE_DELETE_ARRAY(psaPacketReceiving);

	SAFE_DELETE_ARRAY(psaPacketSending);

	if (this->iDefReceivePacket == 0)
	{
		//Only default the first time
		this->iDefReceivePacket = iMaxReceivePacket;
	}

	if (this->iDefSendPacket == 0)
	{
		//Only default the first time
		this->iDefSendPacket = iMaxSendPacket;
	}

	this->iMaxSendPacket = iMaxSendPacket;
	this->iSendWheel = 0;
	this->iSendingWheel = 0;
	this->psaPacketSending = new PacketSending[iMaxSendPacket];

	this->iMaxReceivePacket = iMaxReceivePacket;
	this->iReceiveWheel = 0;
	this->psaPacketReceiving = new PacketReceiving[iMaxReceivePacket];
}

void SocketData::Init()
{
	bInUse = true;
	bConnecting = false;
	bConnected = false;
	bKeySet = false;
	auto l_Context = GetSocketDataContext();
	l_Context->InterServerAuthed = false;

	//Sending
	iSendWheel = 0;
	iSendingWheel = 0;

	for (UINT i = 0; i < iMaxSendPacket; i++)
	{
		PacketSending* pp = psaPacketSending + i;

		ZeroMemory(pp, sizeof(PacketSending));

		pp->bInUse = FALSE;
	}

	dwBulkThreadId = 0;
	sBulkPacket.bInUse = FALSE;

	//Receiving
	iReceiveWheel = 0;

	for (UINT i = 0; i < iMaxReceivePacket; i++)
	{
		PacketReceiving* pp = psaPacketReceiving + i;

		ZeroMemory(pp, sizeof(PacketReceiving));

		pp->bInUse = FALSE;
	}

	iRecvRet = -10;
	iRecvEC = -10;
	iSendRet = -10;
	iSendEC = -10;

	bKeepAlive = FALSE;
}

void SocketData::UnInit()
{
	bKeySet = false;
	bConnected = false;
	bConnecting = false;
	bInUse = false;
	this->s = INVALID_SOCKET;
	auto l_Context = GetSocketDataContext();
	l_Context->InterServerAuthed = false;
}

bool SocketData::Connect(const char* pszIP, int iPort)
{
	bool ret = false;

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s != INVALID_SOCKET)
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr(pszIP);
		sin.sin_port = htons(iPort);

		StringCchCopyA(this->szIP, _countof(this->szIP), pszIP);
		this->iPort = iPort;
		bConnecting = true;

		if (connect(s, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
		{
			Open(s, &sin);

			ret = true;
		}

		bConnecting = false;
	}

	return ret;
}

void SocketData::Open(SOCKET s, sockaddr_in* addr)
{
	this->s = s;
	this->lIP = addr->sin_addr.s_addr;
	this->iPort = ntohs(addr->sin_port);

	StringCchCopyA(this->szIP, _countof(this->szIP), inet_ntoa(addr->sin_addr));

	dwTimeLastPacketReceived = 0;

	bPing = false;
	iPing = 0;
	iCountIN = 0;
	iCountOUT = 0;
	iCountOUTB = 0;
	dwTimeIN = 0;

	bConnected = true;

	iRecvRet = -10;
	iRecvEC = -10;
	iSendRet = -10;
	iSendEC = -10;

	bKeepAlive = FALSE;

	//Nagle Algorithm
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char*)&SOCKET_NODELAY, sizeof(BOOL));

	//Set Receive and Send Window sizes
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&RCVWND_SIZE, sizeof(int));
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&SNDWND_SIZE, sizeof(int));

	ResetEvent(hSendEvent);

	SetEvent(hReceiveThreadSignal);
	SetEvent(hSendThreadSignal);

	bBlockSend = FALSE;
}
bool g_CheatEngineDetected = false;

void SocketData::Ping(DWORD dwTime)
{
	bPing = true;
	dwPingTime = dwTime;

	PacketPing p;
	p.iLength = sizeof(p);
	p.iHeader = PKTHDR_Ping;
	p.dwTime = dwTime;
	p.CheatEngineDetected = g_CheatEngineDetected;

	SendPacket(&p);
}

void SocketData::Close()
{
	bConnected = false;
	bBlockSend = TRUE;
	auto l_Context = GetSocketDataContext();
	l_Context->InterServerAuthed = false;

	SetEvent(hSendEvent);

	shutdown(s, SD_BOTH);
	closesocket(s);
	s = INVALID_SOCKET;
}

void SocketData::SendKeySet(BYTE bObfuscator)
{
	for (int i = 0; i < NUM_ENCKEYS; i++)
		baKeySet[i] = Dice::RandomI(0, 256);

	PacketKeySet s;
	s.iLength = sizeof(s);
	s.iEncKeyIndex = Dice::RandomI(0, 256);
	s.iEncrypted = Dice::RandomI(2, 256);
	s.iHeader = PKTHDR_KeySet;

	bObfuscator += (s.iEncKeyIndex + s.iEncrypted);

	DEBUG("SendKeySet bObfuscator: %d [%d][%d]", bObfuscator, s.iEncKeyIndex, s.iEncrypted);

	for (int i = 0; i < NUM_ENCKEYS; i++)
		s.baKeySet[i] = (baKeySet[i] ^ bObfuscator);

	DEBUG("First Key[0]: %d", baKeySet[0]);

	SendPacket(&s);

	bKeySet = true;
}

void SocketData::ReceiveKeySet(PacketKeySet* p)
{
	BYTE bObfuscator = (BYTE)iPort;
	bObfuscator += (p->iEncKeyIndex + p->iEncrypted);

	DEBUG("ReceiveKeySet bObfuscator: %d [%d][%d]", bObfuscator, p->iEncKeyIndex, p->iEncrypted);

	for (int i = 0; i < NUM_ENCKEYS; i++)
		baKeySet[i] = (p->baKeySet[i] ^ bObfuscator);

	DEBUG("First Key[0]: %d", baKeySet[0]);

	bKeySet = true;
}

void SocketData::EncryptPacket(Packet* p)
{
	if (bKeySet)
	{
		BYTE bObf = baKeySet[bNextKey];

		p->iEncKeyIndex = bNextKey;
		p->iEncrypted = 1;

		bObf += p->iEncKeyIndex;
		bObf += p->iEncrypted;

		BYTE* buf = (BYTE*)p;
		for (PKTLEN i = PKTLENBFRHDR; i < p->iLength; i++)
		{
			BYTE* ib = (BYTE*)&i;

			bObf += ib[0];
			bObf += ib[1];

			buf[i] ^= bObf;
			buf[i] ^= (p->iEncKeyIndex + 0x2);
		}

		bNextKey += p->iLength;
	}
}

void SocketData::DecryptPacket(Packet* p)
{
	if (bKeySet)
	{
		BYTE bObf = baKeySet[p->iEncKeyIndex];

		bObf += p->iEncKeyIndex;
		bObf += p->iEncrypted;

		BYTE* buf = (BYTE*)p;
		for (PKTLEN i = PKTLENBFRHDR; i < p->iLength; i++)
		{
			BYTE* ib = (BYTE*)&i;

			bObf += ib[0];
			bObf += ib[1];

			buf[i] ^= bObf;
			buf[i] ^= (p->iEncKeyIndex + 0x2);
		}
	}

	p->iEncKeyIndex = 0;
	p->iEncrypted = 0;
}

bool SocketData::PacketEncrypted(Packet* p)
{
	if (bKeySet)
	{
		return p->iEncrypted != 0;
	}
	else
	{
		if (p->iHeader == PKTHDR_KeySet)
			ReceiveKeySet((PacketKeySet*)p);
	}

	return false;
}

PacketReceiving* SocketData::FreePacketReceiving()
{
	pcReceiveMutex->Lock();

	int iFree = iReceiveWheel % iMaxReceivePacket;

	PacketReceiving* ret = psaPacketReceiving + iFree;

	if (ret->bInUse == FALSE)
	{
		ret->bInUse = TRUE;
		ret->bDelete = FALSE;

		iReceiveWheel++;

		pcReceiveMutex->Unlock();
	}
	else
	{
		pcReceiveMutex->Unlock();

		if (bKeepAlive)
		{
			ret = new PacketReceiving;
			ret->bInUse = TRUE;
			ret->bDelete = TRUE;
		}
		else
		{
			ret = NULL;
		}
	}

	return ret;
}

void SocketData::FreePacketReceiving(PacketReceiving* pp)
{
	if (pp->bDelete == FALSE)
	{
		pcReceiveMutex->Lock();

		pp->bInUse = FALSE;

		pcReceiveMutex->Unlock();
	}
	else
	{
		delete pp;
	}
}

PacketReceiving* SocketData::ReceivePacket()
{
	PKTLEN offset = 0;

	PacketReceiving* pp = FreePacketReceiving();

	if (pp == NULL)
	{
		LOGERROR("ReceivePacket(): No more available FreePacketReceiving");

		iRecvRet = 0x0F0F0F0F;
		iRecvEC = -1;
		return NULL;
	}

	BYTE* baReceiveBuffer = pp->baPacket;

	uint8_t l_XorKey = GAME_XOR;

	int iBytesReceived = 0;
	while ((iBytesReceived = recv(s, (char*)(baReceiveBuffer + offset), sizeof(PKTLEN) - offset, 0)) > 0)
	{
		offset += iBytesReceived;

		if (offset < sizeof(PKTLEN))
			continue;

		baReceiveBuffer[0] ^= l_XorKey;
		baReceiveBuffer[1] ^= l_XorKey;

		PKTLEN sPacketLength = *(PKTLEN*)baReceiveBuffer;

		if (sPacketLength < sizeof(Packet))
		{
			iBytesReceived = 0;
			offset = 0;
			continue;
		}

		if (sPacketLength > MAX_PKTSIZ)
		{
			//Likely because of game version mismatch
			WARN("ReceivePacket(): %d Bytes Expected, Max %d! Header: %d. Ignoring", sPacketLength, MAX_PKTSIZ, baReceiveBuffer[4]);
			iRecvEC = 0x6A6A;
			FreePacketReceiving(pp);
			return NULL;
		}

		while ((iBytesReceived = recv(s, (char*)(baReceiveBuffer + offset), sPacketLength - offset, 0)) > 0)
		{
			offset += iBytesReceived;

			if (offset == sPacketLength)
			{
				for (size_t l_BI = 2; l_BI < sPacketLength; ++l_BI)
					baReceiveBuffer[l_BI] ^= l_XorKey;

				Packet* psPacket = (Packet*)baReceiveBuffer;
				if (PacketEncrypted(psPacket))
					DecryptPacket(psPacket);

				return pp;
			}
			else if (offset > sPacketLength)
			{
				LOGERROR("ReceivePacket(): offset %d > sPacketLength %d", offset, sPacketLength);
				iRecvEC = 0x6A6A; //27242
				FreePacketReceiving(pp);
				return NULL;
			}
		}
		break;
	}

	iRecvRet = iBytesReceived < 0 ? errno : 0;
	iRecvEC = WSAGetLastError();

	if (iBytesReceived > 0)
		iCountIN += iBytesReceived;

	FreePacketReceiving(pp);
	return NULL;
}

PacketSending* SocketData::NextPacketSending()
{
	while (bConnected)
	{
		pcSendMutex->Lock();

		if (iSendWheel > iSendingWheel)
		{
			PacketSending* ps = psaPacketSending + (iSendingWheel % iMaxSendPacket);

			if (ps->bInUse)
			{
				iSendingWheel++;

				pcSendMutex->Unlock();

				return ps;
			}
		}

		pcSendMutex->Unlock();

		WaitForSingleObject(hSendEvent, INFINITE);
	}

	return NULL;
}

void SocketData::FreePacketSending(PacketSending* pp)
{
	pcSendMutex->Lock();

	pp->bInUse = FALSE;

	pcSendMutex->Unlock();
}

BOOL SocketData::Send(BYTE* pbData, UINT uSize)
{
	uint8_t l_XorKey = GAME_XOR;

	for (size_t l_BI = 0; l_BI < uSize; ++l_BI)
		pbData[l_BI] ^= l_XorKey;

	UINT uDataSent = 0;
	while (uSize > uDataSent)
	{
		int iSendVal = send(s, (const char*)(pbData + uDataSent), uSize - uDataSent, 0);

		if (iSendVal < 0)
		{
			DWORD dwError = WSAGetLastError();

			iSendEC = dwError;
			WARN("send() failed (%d)", dwError);
			return FALSE;
		}

		uDataSent += iSendVal;
	}

	for (size_t l_BI = 0; l_BI < uSize; ++l_BI)
		pbData[l_BI] ^= l_XorKey;

	iCountOUT++;

	return TRUE;
}

BOOL SocketData::SendData(BYTE* pbData, UINT uSize)
{
	//DEBUG( "SocketData::SendPacket( 0x%04X )", uSize );

	if (bBlockSend)
		return FALSE;

	if (uSize == 0)
		return TRUE;

	if (uSize > MAX_PKTSIZ)
		return FALSE;

	pcSendMutex->Lock();

	//Use Automatic Bulking if possible and enabled
	PacketSending* ps = FindAutomaticBulk(uSize);

	if (ps)
	{
		CopyMemory(ps->baPacket + ps->uSize, pbData, uSize);
		ps->uSize += uSize;

		pcSendMutex->Unlock();

		return TRUE;
	}
	else
	{
		ps = psaPacketSending + (iSendWheel % iMaxSendPacket);

		//Should not be in use..
		if (!ps->bInUse)
		{
			ps->bInUse = TRUE;
			ps->uSize = uSize;
			CopyMemory(ps->baPacket, pbData, uSize);

			iSendWheel++;

			SetEvent(hSendEvent);

			pcSendMutex->Unlock();

			return TRUE;
		}
	}

	pcSendMutex->Unlock();

	return FALSE;
}

PacketSending* SocketData::FindAutomaticBulk(UINT uSizeNeeded)
{
	//Automatically Bulk to the last Waiting Packet to send, if available
	if (iSendWheel > iSendingWheel)
	{
		UINT iWaitingSend = iSendWheel - 1;

		PacketSending* ps = psaPacketSending + (iWaitingSend % iMaxSendPacket);

		//This Waiting Packet to send should be in use..
		if (ps->bInUse)
		{
			//Enough Space Available to use for Packet requesting to Bulk?
			if ((ps->uSize + uSizeNeeded) <= MAX_PKTSIZ)
				return ps;
		}
	}

	return NULL;
}

BOOL SocketData::Send(PacketSending* pp)
{


	//Encrypted all Packets contained (more than 1 if PacketSending is used in Bulking)
	UINT uPosition = 0;
	while (uPosition < pp->uSize)
	{
		Packet* psPacket = (Packet*)(pp->baPacket + uPosition);

		if (psPacket->iEncrypted == 1)
			EncryptPacket(psPacket);

		uPosition += psPacket->iLength;
		iCountOUT += psPacket->iLength;

		iCountOUTB++;
	}

	//Send all Packets contained (more than 1 if Bulked)
	Send(pp->baPacket, pp->uSize);

	return TRUE;
}

BOOL SocketData::SendPacketCommand(enum PacketsHeader header, int p1, int p2, int p3, int p4)
{
	PacketCommand sPacket;
	ZeroMemory(&sPacket, sizeof(PacketCommand));
	sPacket.iLength = sizeof(PacketCommand);
	sPacket.iHeader = header;
	sPacket.p1 = p1;
	sPacket.p2 = p2;
	sPacket.p3 = p3;
	sPacket.p4 = p4;

	return SendPacket(&sPacket, FALSE);
}

BOOL SocketData::SendPacket(Packet* p, BOOL bEncrypted)
{
	//	LOGERROR( "SocketData::SendPacket( [0x%04X][0x%08X] )", p->iLength, p->iHeader );


#ifdef _GAME

#if defined (LOG_INCOMING_OUTGOING_PACKETS)

	char hexString[16];
	_itoa_s(p->iHeader, hexString, 16);
	CLogger::GetLogger()->Log("%d,OUT,%s,%d,%d,0,0", TICKCOUNT, hexString, p->iLength);
	CLogger::GetLogger()->Flush();
#endif

#ifdef DEV_MODE
	if (p->iHeader == 0)
	{
		CHATBOX->AddMessage("ERROR> Sending packet header is 0!", EChatColor::CHATCOLOR_Error);
	}

	if (p->iLength == 0)
	{
		CHATBOX->AddMessage("ERROR> Sending packet length is 0!", EChatColor::CHATCOLOR_Error);
	}
#endif


	if (PACKETHANDLER->IsPacketIgnored(p))
		return FALSE;


#endif



	if (p->iLength > MAX_PKTSIZ)
		return FALSE;

	if (p->iHeader != PKTHDR_KeySet)
	{
		p->iEncKeyIndex = 0;
		p->iEncrypted = bEncrypted ? 1 : 0;
	}

	if (pcSendMutex == nullptr)
		return FALSE;

	pcSendMutex->Lock();

	//Is Forced Bulk Sending Activated? (not sent until flushed)
	if (sBulkPacket.uCount > 0)
	{
		//Bulk Sending Initiating Thread is same as current Thread?
		if (dwBulkThreadId == GetCurrentThreadId())
		{
			pcSendMutex->Unlock();

			UINT uNewSize = sBulkPacket.uSize + p->iLength;

			if (uNewSize > MAX_PKTSIZ)
			{
				if (FlushBulkSend() == FALSE)
					return FALSE;
			}

			UINT uPosition = sBulkPacket.uSize;

			CopyMemory(sBulkPacket.baPacket + uPosition, p, p->iLength);

			sBulkPacket.uSize = uPosition + p->iLength;

			return TRUE;
		}
	}

	//No Forced Bulk Sending.. so Send seperately or if possible Bulk automatically (sent instantly)
	BOOL bRet = SendData((BYTE*)p, p->iLength);

	pcSendMutex->Unlock();

	return bRet;
}

BOOL SocketData::SendPacketBlank(PKTHDR iHeader, BOOL bEncrypted)
{
	Packet s;
	s.iLength = sizeof(s);
	s.iHeader = iHeader;

	return SendPacket(&s, bEncrypted);
}

BOOL SocketData::StartBulkSend()
{
	pcSendMutex->Lock();

	if (sBulkPacket.uCount > 0)
	{
		if (dwBulkThreadId != GetCurrentThreadId())
		{
			pcSendMutex->Unlock();

			return FALSE;
		}

		sBulkPacket.uCount++;

		pcSendMutex->Unlock();

		return TRUE;
	}

	dwBulkThreadId = GetCurrentThreadId();

	sBulkPacket.uCount = 1;
	sBulkPacket.uSize = 0;

	pcSendMutex->Unlock();

	return TRUE;
}

BOOL SocketData::FlushBulkSend()
{
	pcSendMutex->Lock();

	if (sBulkPacket.uCount == 0)
	{
		pcSendMutex->Unlock();

		return FALSE;
	}

	if (dwBulkThreadId != GetCurrentThreadId())
	{
		pcSendMutex->Unlock();

		return FALSE;
	}

	if (sBulkPacket.uSize > 0)
	{
		if (SendData(sBulkPacket.baPacket, sBulkPacket.uSize) == FALSE)
		{
			pcSendMutex->Unlock();

			return FALSE;
		}

		sBulkPacket.uSize = 0;
	}

	pcSendMutex->Unlock();

	return TRUE;
}

BOOL SocketData::StopBulkSend()
{
	pcSendMutex->Lock();

	if (sBulkPacket.uCount == 0)
	{
		pcSendMutex->Unlock();

		return FALSE;
	}

	if (dwBulkThreadId != GetCurrentThreadId())
	{
		pcSendMutex->Unlock();

		return FALSE;
	}

	sBulkPacket.uCount--;

	if (sBulkPacket.uCount == 0)
	{
		//Flush
		if (sBulkPacket.uSize > 0)
		{
			SendData(sBulkPacket.baPacket, sBulkPacket.uSize);

			sBulkPacket.uSize = 0;
		}
	}

	pcSendMutex->Unlock();

	return TRUE;
}

int SocketData::GetPing(DWORD dwTime)
{
	if (bPing)
		return dwTime - dwPingTime;

	return iPing;
}


SocketDataContext* SocketData::GetSocketDataContext()
{
	return FindSocketDataContext(s);
}