#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define IP_FORTRESS_MAIN			"127.0.0.1"		// offIcial 54.36.127.45
#define IP_FORTRESS_SEASONAL 		"127.0.0.1"		//"51.195.60.124" Seasonal box	// 37.187.180.4 Test box
#define IP_FORTRESS_DEV 			"127.0.0.1"			// Local only
//#define IP_FORTRESS_DEV 			"80.114.94.30"		// Chaos
//#define IP_FORTRESS_DEV 			"124.197.23.156"	// Josh

//#define TEST_SCREEN
#define NEW_USERINTERFACE

//#define LOG_INCOMING_OUTGOING_PACKETS

#define _NEW_DMG__

#define MAX_UNITS		1024

#define WM_UPDATE			WM_USER +   1
#define WM_WINDOWREADY		WM_USER +  51
#define WM_SOCKETACCEPT		WM_USER + 101
#define WM_SOCKETPACKET		WM_USER + 102
#define WM_SOCKETCLOSE		WM_USER + 103

//Global Functions
#include "def.h"
#include "strings.h"
#include "common.h"
#include "point.h"

#include "X3D.h"

//System
#include "CSystem.h"
#include "CApplication.h"
#include "CWindow.h"
#include "Timer.h"
#include "CTimer.h"

//File Library
#include "CReader.h"
#include "CReaderFile.h"

//Definitions
#include "sound.h"
#include "packets.h"
#include "map.h"
#include "quest.h"
#include "party.h"
#include "chat.h"
#include "debug.h"
#include "warehouse.h"
#include "user.h"
#include "account.h"

#include "sky.h"

#include "AYObfuscate.h"

#include "Game.h"
#include "CGameWindow.h"
#include "DX.h"
#include "RecvPacket.h"
#include "GameCore.h"

#include "socket.h"
#include "Coin.h"
#include "CoinShopTab.h"
#include "CoinShop.h"

#include "EXE.h"

static int iGamePort = 10009;
static int iUnitRenderDistanceMAX = DISTANCE_XY_13_meters;

static void __cdecl ToolTipBox(int iX, int iY, const char* format, ...)
{

	char szBuf[256];
	ZeroMemory(szBuf, 256);
	va_list ap;
	va_start(ap, format);
	_vsnprintf_s(szBuf, 256, 255, format, ap);
	va_end(ap);

	CTOOLTIP->SetText(iX, iY, szBuf);
}

static void __cdecl TitlePTBox(const char* format, ...)
{

	char szBuf[256];
	ZeroMemory(szBuf, 256);
	va_list ap;
	va_start(ap, format);
	_vsnprintf_s(szBuf, 256, 255, format, ap);
	va_end(ap);

	CTITLEBOX->SetText(szBuf, 3000);
}

static void __cdecl TeleportPlayer(int iStage)
{
	CALL_WITH_ARG1(0x004441D0, (DWORD)iStage);
}

static void __cdecl SetPlayerPattern(UnitData* Player, char* szModelFile, char* szModelFile2)
{
	typedef int(__cdecl* t_SetPlayerPattern) (UnitData*, char*, char*);
	t_SetPlayerPattern SetPlayerPattern = (t_SetPlayerPattern)0x0043C410;

	SetPlayerPattern(Player, szModelFile, szModelFile2);
}

static void __cdecl PlaySoundMini(int iMiniSoundCode, int iVolume = 400)
{
	CALL_WITH_ARG2(0x00543A30, iMiniSoundCode, iVolume);
}

/// <summary>
/// 2012 source: int	CharPlaySound( smCHAR *lpChar )
/// </summary>
static void __cdecl PlayUnitSound(UnitData* pcUnitData)
{
	CALL_WITH_ARG1(0x00543F80, (DWORD)pcUnitData);
}

static int __cdecl GetSpeedFrame(int iCharacterSpeed, int iSpeed)
{
	int ir = 0;
	__asm
	{
		PUSH iSpeed;
		PUSH iCharacterSpeed;
		MOV EAX, 0x0045F7D0;
		CALL EAX;
		ADD ESP, 0x08;
		MOV ir, EAX;
	}
	return ir;
}

enum
{
	CHARSTATUSCOLOR_White,
	CHARSTATUSCOLOR_Red,
	CHARSTATUSCOLOR_Yellow,
	CHARSTATUSCOLOR_Blue,
	CHARSTATUSCOLOR_Green,
	CHARSTATUSCOLOR_Gold,
	CHARSTATUSCOLOR_Pink,
	CHARSTATUSCOLOR_Orange,
};

enum
{
	CHARSTATUSTYPECOLOR_AttackRating,
	CHARSTATUSTYPECOLOR_Damage,
	CHARSTATUSTYPECOLOR_Defense,
	CHARSTATUSTYPECOLOR_Absorb,
	CHARSTATUSTYPECOLOR_Speed,
	CHARSTATUSTYPECOLOR_HP,
	CHARSTATUSTYPECOLOR_MP,
	CHARSTATUSTYPECOLOR_SP,

};

enum EMouseIcon : int
{
	EMOUSEICON_None,
	EMOUSEICON_Default,
	EMOUSEICON_Sell,
	EMOUSEICON_Buy,
	EMOUSEICON_Repair,
	EMOUSEICON_RepairAll,
	EMOUSEICON_Attack,
	EMOUSEICON_GetItem1,
	EMOUSEICON_GetItem2,
	EMOUSEICON_Talk,
};


static void SetTextStatusColor(int iStatus, int iColor)
{
	(*(int*)(0x0370713C + (iStatus * 4))) = iColor;
}

#define RENDERDISTANCE_ENABLE				(*(BOOL*)0x00CF48C4)
#define RENDERDISTANCE_MAX					iUnitRenderDistanceMAX

#define POLYCOUNT							(*(int*)0x02444BF0)

#define COLOR_R								(*(int*)0x00CF4778)
#define COLOR_G								(*(int*)0x00CF477C)
#define COLOR_B								(*(int*)0x00CF4780)

#define BRIGHTNESS							(*(int*)0x00A17560)

#define BLURTIME							(*(DWORD*)0x00A17564)

#define ANGLE_X								(*(int*)0x00824FA4)

#define GAME_DISTANCE						(*(int*)0x00824FB0)

#define ACTIONMODE							(*(int*)0x00CF4818)
#define ACTIONMODE_MOVE						(*(int*)0x009E2188)

#define MINIMIZED_GAME						(*(BOOL*)0x00A17550)

#define TEXTUREFRAME						(*(int*)0x03500CC0)
#define ISTEXTUREREADY						(*(BOOL*)0x0082C124)

#define TRACECAMERA							((Point3D*)0x00A15DE8)
#define TRACETARGET							((Point3D*)0x00A16254)
#define ISAUTOCAMERA						(*(BOOL*)0x00A1757C)
#define ISBIGMAP							(*(BOOL*)0x02E45824)
#define CAMERA_MOVE_STEP					(8<<8)
#define FIELDGAME							((BaseMap**)0x00CF4748)

#define VIEWPOINTSIGHT						(*(int*)0x0082BF34)
#define ZDISTANCEMIN						(*(int*)0x0082BF38)
#define RENDERSIGHTPOSITION					(*(int*)0x00A1768C)

#define CHARACTERMESSAGE					((UnitData**)0x00A16418)
#define CHARACTERMESSAGE_COUNT				(*(int*)0x00A16250)

#define HIDERIGHTNPCBOX						{WRITEDWORD( 0x0207CA64, 0 );}
#define SHOWRIGHTNPCBOX						{WRITEDWORD( 0x0207CA64, 1 );}

#define STAGE1								(*(Stage**)0x00CF4740)
#define STAGE2								(*(Stage**)0x00CF4744)

#define RESTARTCOUNTER						(*(int*)0x00CF4770)	//after death or on spawn, your character is flashing for some time. During this time you cannot cast skills etc
#define RESTARTFLAG						    (*(int*)0x035E10F8)	//
#define RESTARTINDEX					    (*(int*)0x035E1104)	//
#define USEETHERCORECODE					(*(int*)0x00CF47BC)	//UseEtherCoreCode

#define CASTLEMASTERID						(READDWORD(0x0392E08C))

#define DARKCOLOR_AMOUNT					(*(int*)0x00CF4774)
#define DARKLIGHTCOLOR_RANGE				(*(int*)0x00826D0C)

#define ISSHOW_MINIMAP						(*(BOOL*)0x035E11D8)
#define ISSHOW_INTERFACE					(*(BOOL*)0x00824F80)
#define FOLLOWMODE_ACTION					(*(BOOL*)0x00CF4894)
#define CANATTACK							(*(BOOL*)0x00CF48AC)
#define AUTOATTACKMODE						(*(BOOL*)0x00CF48B0)
#define SELECTED_MOUSE_BUTTON				(*(BOOL*)0x00CF4898)
#define DISP_EACH_MODE						(*(BOOL*)0x00CF48DC)
#define AUTOMOUSE_WM_COUNT					(*(BOOL*)0x00CF47DC)

#define PLAYER_BACKUP						*( UINT* )0x0CF481C
#define PLAYER_TIER							(*( UINT* )( PLAYER_BACKUP + 0x3A98 ))
#define QUESTCODE_BASE_ADDR  				0x362B5BC
#define PLAYER_QUESTLOG						*( DWORD* )0x0362BA68
#define PLAYER_SKILL_POINT					*( DWORD* )0x03673B70
#define PLAYER_STATS_POINT					*( DWORD* )(PLAYER_BACKUP + 0x3A74)
#define CHAR_DATA_PTR						0x362A3B8
#define SKILL_INITIALIZATION_FLAG			*( BOOL* )0x0362B1F4 //sinSkillPontInitButtonFlag

#define UNITDATA_HOVER						(*(UnitData**)0x00CF489C) //lpMouseHoverChar. Gets set when the mouse cursor is hovering over an character. Was UNITDATA_SELECTED
#define UNITDATA_FOCUSED					(*(UnitData**)0x00CF48A0) //lpMouseSelectedChar. Gets set when the mouse cursor is Was UNITDATA_FOCUS
#define ITEMVIEW_SELECTED					(*(DropItemView**)0x00CF48A4)

#define DEBUG_GM							(*(BOOL*)0x00A1758C)
#define DEBUG_USER							(*(BOOL*)0x00A17590)
#define MOUSE_ICON							(*(EMouseIcon*)0x00999308)

#define ISPRESSEDKEY(c)						(*(BYTE*)(0x00A16318 + c))
#define ISMOUSELEFT_CLICKED					(*(BOOL*)0x00A17568)

#define RUN_UIMODE							(*(BOOL*)0x035E11D0)

#define SETPLAYERPATTERN					SetPlayerPattern
#define CURRENT_TARGET						(*(UnitData**)0x0CF48D0)
#define	TARGET_UNITDATA						((UnitData*)(0x0207C888))
#define GM_MODE								*( int* )0x00CDE1CC
#define TITLEBOX							TitlePTBox
#define TOOLTIP								ToolTipBox
#define TICKCOUNT							GetTickCount()
#define ITEMINDEX_INVENTORY					((*(int*)0x35EB65C)-1)
#define PLAYMINISOUND						PlaySoundMini
#define PLAYUNITSOUND						PlayUnitSound

#define CAMERAPOSITION						((Point3D*)0x00A15DE8)
#define CAMERATARGETPOSITION				((Point3D*)0x00A16254)

#define UNITTARGETPOSITION_X				(*(int*)0x00A1CCD4)
#define UNITTARGETPOSITION_Y				(*(int*)0x00A186A8)
#define UNITTARGETPOSITION_Z				(*(int*)0x00A1CCC0)

#define UNITTARGETANGLE_X					(*(int*)0x00A1BCB4)
#define UNITTARGETANGLE_Y					(*(int*)0x00A180A0)
#define UNITTARGETANGLE_Z					(*(int*)0x00A18090)

#define SETSTATUSCOLOR						SetTextStatusColor

// Definitions
#define PORT_GAME							iGamePort


#define UNITPLAYER							( Unit::GetUnit(UNITDATA) )
#define GETUNIT(p)							(Unit::GetUnit(p))
#define PLAYERDATA							( *( CharacterData**)CHAR_DATA_PTR )
#define SKILLSCHARACTER						( ( struct Skill* )0x3670D78)
#define	DISCONNECT_TIME						(*(DWORD*)0x03A97670)
#define	DISCONNECT_TYPE						(*(DWORD*)0x03A97674)

// D3D
#define ANIMOBJTREE							Game::AnimObjectTree

#define GETSPEEDFRAME						GetSpeedFrame

#define PTSEN								(*(int**)(0x0082C104))
#define PTCOS								(*(int**)(0x0082C108))

#define TELEPORTPLAYER						TeleportPlayer

#define RESOLUTION_WIDTH					DX::Graphic::GetWidthBackBuffer()
#define RESOLUTION_HEIGHT					DX::Graphic::GetHeightBackBuffer()

#define WM_WINDOWREADY		WM_USER +  51

#endif