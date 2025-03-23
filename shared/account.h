#pragma once

#include "user.h"
#include "character.h"

#define _REQUIREACCOUNTVALIDATION

#define MAX_ACCOUNTNAMELEN			32
#define MAX_PASSWORDLEN				32
#define MAX_ACCOUNTLOGINQUEUE		100
#define MAX_CHARACTERSPERACCOUNT	5

enum class EBanStatus
{
	BANSTATUS_Error				= -1,
	BANSTATUS_NotBanned			= 0,
	BANSTATUS_Banned			= 1,
	BANSTATUS_TempBanned		= 2,
	BANSTATUS_BanExpired		= 3,
};

enum EMuteStatus : int
{
	MUTESTATUS_Error		= -1,
	MUTESTATUS_NotMuted		= 0,
	MUTESTATUS_Muted		= 1,
	MUTESTATUS_PermaMute	= 2,	//Permanent mute. Ouch.
};

enum EAccountLogin
{
	ACCOUNTLOGIN_Success			= 1,	//[Successful]
	ACCOUNTLOGIN_LoginPending		= 0,	//Login pending / Connecting failed
	ACCOUNTLOGIN_IncorrectName		= -1,	//Account does not exist in the selected world
	ACCOUNTLOGIN_IncorrectPassword	= -2,	//Incorrect Password
	ACCOUNTLOGIN_Banned				= -3,	//Account is banned
	ACCOUNTLOGIN_LoggedIn			= -4,	//Account is already logged in
	ACCOUNTLOGIN_IPBanned			= -5,	//Your IP is banned
	ACCOUNTLOGIN_YouAreBanned		= -6,	//Your IP is banned
	ACCOUNTLOGIN_TryAgain			= -7,	//[Game will automatically retry]
	ACCOUNTLOGIN_Maintenance		= -8,	//Server is in Maintenance
	ACCOUNTLOGIN_AccountNotActive	= -16,	//Account not active, see User Management Panel
	ACCOUNTLOGIN_WrongVersion		= -17,	//Version does not Match
	ACCOUNTLOGIN_TempBanned			= -18,	//Temporarily Banned
	ACCOUNTLOGIN_AccountNameNotFound = -19,	//Account name does not exist
};



enum EAccountFlag
{
	ACCOUNTFLAG_NotExist			= -1,
	ACCOUNTFLAG_Invalid				= 0,
	ACCOUNTFLAG_Mule				= 1,
	ACCOUNTFLAG_Activated			= 2,
	ACCOUNTFLAG_EventGM				= 4,
	ACCOUNTFLAG_Supporter			= 8,
	ACCOUNTFLAG_GameMaster			= 16,
	ACCOUNTFLAG_AcceptedLatestTOA	= 32,
	ACCOUNTFLAG_Approved			= 64,
	ACCOUNTFLAG_MustConfirm			= 128,
	ACCOUNTFLAG_GMIPRequired		= 256,
};

enum ECharacterLog
{
	CHARACTERLOG_Create				= 1,
	CHARACTERLOG_Delete				= 2,
	CHARACTERLOG_Update				= 3,
};

struct AccountFileData
{
	char	  szHeader[16];
	char	  szAccountName[32];
	char	  szCharacterName[MAX_CHARACTERSPERACCOUNT + 1][32];
};

struct PacketAccountLoginCode : Packet
{
	DWORD			  dwReserved;
	EAccountLogin	  iCode;
	int				  iFailCode;

	char			  szMessage[256];
};

struct PacketAccountID : Packet
{
	ID				  lAccountID;
};

struct PacketWorldLoginToken : Packet
{
	char TokenPass[65];
};
struct PacketWorldLoginAuth: Packet
{
	char Token[65];
	char TokenPass[65];
};

struct AccountLogin
{
	User				* pcUser;
	UserData			* pcUserData;
	SocketData			* pcSocketData;

	char				szAccountName[32];
	char				szPassword[65];
	char				szIP[20];
	char				szMacAddress[20];

	UINT				uSerialHD;
	char				szPCName[32];
	char				szVideoName[256];

	char				szHardwareID[40];

	UINT				uWidthScreen;
	UINT				uHeightScreen;
	int					iSystemOS;
};
