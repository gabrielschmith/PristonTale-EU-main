#pragma once
class RecvPacket
{
private:

public:
	RecvPacket();
	virtual ~RecvPacket();

	BOOL										OnPostProcessedPacket ( SocketData * pcSocketData, Packet * psPacket );
	BOOL										IsPacketIgnored( Packet * psPacket );
	BOOL										AnalyzePacket( SocketData * pcSocketData, Packet * psPacket );
};

