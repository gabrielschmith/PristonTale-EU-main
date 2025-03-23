#pragma once

#define MAX_CHATLENGTH			100
#define MAX_CHATMESSAGES		320
enum EChatColor
{
	CHATCOLOR_Error				= 0,   //Red
	CHATCOLOR_Whisper			= 1,   //Blue
	CHATCOLOR_Blue				= 2,   //Blue
	CHATCOLOR_Global			= 3,   //Purple
	CHATCOLOR_Party				= 4,   //Green
	CHATCOLOR_Clan				= 5,   //Yellow (clan)
	CHATCOLOR_Trade				= 6,   //Whitish Cyan (trade)
	CHATCOLOR_Notice			= 9,   //Purple
	CHATCOLOR_ClanLeader		= 10,  //Orange (clan leader)
	CHATCOLOR_Normal			= 11,  //Yellow (normal chat)
	CHATCOLOR_Raid				= 12,  //Cyan
	CHATCOLOR_Command			= 13,  //JLM - Cheap solution to do what we want on client side
	CHATCOLOR_GameMaster        = 14,  //For sending chat to all game-masters online
	CHATCOLOR_White             = 15,
};

struct ChatBoxMessage
{
	EChatColor	  iColor;
	char		  szMessage[256];
};

struct PacketChat : Packet
{
	char			  szChat[128];
};

/// <summary>
/// struct TRANS_CHATMESSAGE
/// Sends a popup message above a unit's head
/// Use PKTHDR_ChatMessage (smTRANSCODE_WHISPERMESSAGE)
/// </summary>
struct PacketChatMessage : Packet
{
	DWORD	dwIP;
	DWORD	dwObjectSerial;

	char szMessage[256];
};

//Note - if updating size of this struct, make sure to update client side too.
struct PacketChatBoxMessage : Packet
{
	EChatColor		  iChatColor;				//Chat Type, Party, Clan, Trade, etc.
	ID				  lID;						//User ID, if 0 only shows in Chat Box, otherwise also above User
	char			  szChatBoxMessage[256];
};

class Chat
{
public:
	static unsigned int	ChatCommand( char * pszCommand, char * pszChat );
	static void			ChatParamQ( char * pszCommand, int iParamNum, char * pszOut );
	static void			ChatParam( char * pszCommand, int iParamNum, char * pszOut );
	static void			TrimSpace( char * pszIn, char * pszOut );
	static int			TrimSpaceBegin( char * pszIn, char * pszOut );
};