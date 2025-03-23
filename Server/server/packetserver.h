#pragma once
#include "userdataserver.h"

#define SENDPACKET PacketServer::Send
#define SENDPACKETBLANK PacketServer::SendPacketBlank
#define SENDPACKETSTAGE PacketServer::SendPacketStage
#define SENDPACKETRANGE PacketServer::SendPacketRange
#define SENDPACKETCOMMAND PacketServer::SendPacketCommand

struct PacketSyncData
{
	int											iSize;
	int											iHeader;

	struct CharacterData							CharInfo;

	DWORD	dwObjectID;

	int	x, y, z;				//최초 좌표
	int ax, ay, az;			//최초 각도
	int state;				//속성
};

class PacketServer
{
public:
	PacketServer();
	virtual ~PacketServer();

	BOOL										AnalyzePacket( User * pcUser, PacketReceiving * p );

	static BOOL									IsPacketIgnored(Packet * pPacket);
	static const std::string					GetExtraInfoForPacket(Packet * pPacket);

	static void									ShopCodeTemp();
	static void									Send(User * pcUser, void * pPacket, BOOL bEncrypted = FALSE);
	static void									Send(UserData * pcUserData, void * pPacket, BOOL bEncrypted = FALSE);
	static void									SendSkillCastCommandRange( User * pcCaster, int LParam = 0, int WParam = 0, int SParam = 0, int EParam = 0 );
	static void									SendPacketCommand ( User * pcUser, int iPacket, int LParam = 0, int WParam = 0, int SParam = 0, int EParam = 0 );
	static void									SendPacketBlank( User * pcUser, int iPacket, BOOL bEncrypted = FALSE );
	static void									SendPacketStage( void * pPacket, int iStageID, BOOL bEncrypt = FALSE );
	static void									SendPacketRange( void * pPacket, Point3D * psPos, int iStageID, int iDistance, BOOL bEncrypted = FALSE, UserData* p_Exclude = nullptr);
	static void								    WritePacketToOutput(Packet* psPacket);

private:

};