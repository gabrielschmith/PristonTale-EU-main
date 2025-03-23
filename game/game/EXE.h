#pragma once

#include "EXEFace.h"
#include "EXEFrame.h"
#include "EXEMesh.h"
#include "EXEModel.h"
#include "EXERender.h"
#include "EXEVertex.h"
#include "EXEUglyRefactor.h"

#define GETWORDPT(decoded, line)			((char*)CALL_WITH_ARG2(0x004780A0, (DWORD)decoded, (DWORD)line))
#define GETWORDVALUEPT(buffer, param)		((char*)CALL_WITH_ARG2(0x004780F0, (DWORD)buffer, (DWORD)param))

#define DAMAGESINGLE_COUNT					(*(int*)0x00A04764) //dmAttackCount

#define PLAYTIME							(*(DWORD*)0x00CF4750)
#define BATTLETIME							(*(DWORD*)0x00CF4764)

#define CHANGEJOB4_CODE						(*(DWORD*)0x0362B5A0)

#define GETSERVER_SOCKET(id)				((SocketData*)CALL_WITH_ARG1(0x0061EB00, id))

#define PRECISIONTARGET_RANGE				(*(BOOL*)0x00A046AC)

#define SPARKSHIELD_MODEL					(*(EXEModel**)0x00CF47EC)
#define DIVINESHIELD_MODEL					(*(EXEModel**)0x00CF47F0)
#define GODLYSHIELD_MODEL					(*(EXEModel**)0x00CF47F4)
#define PROJECTILE_MODEL					(*(EXEModel**)0x00CE3F88)

#define MESSAGEBOX_SHOWALWAYS				(*(BOOL*)0x00A17598)


#define BLESSCASTLE_BLUE_CROWN_MODEL		(*(EXEModel**)0x00CF47FC)
#define BLESSCASTLE_GOLDEN_CROWN_MODEL		(*(EXEModel**)0x00CF47F8)

#define EXETEXTUREMANAGER_DEFAULT			(*(EXETextureManager**)0x03500CE8) //smMATERIAL_GROUP default


struct EXESurfaceOld
{
	int						iReferenceCount;
	EXETextureHandle		sTexture;
	void					* psCache;
};

struct ClanInfoHeader
{
	char					szClanTexture[40];	//ClanMark_Num
	DWORD					dwClanTextureNum;	//nClanMark
	char					szClanName[40];		//ClanName
	char					szClanText[90];		//ClanReport
	char					cGserver[4];		//Gserver
	char					szUniqueClanInfo[64];	//UniqueClaClanInfo_nNum
};

//struct CLANINFO
struct ClanInfo
{
	ClanInfoHeader			sHeader;

	EXESurfaceOld			* psTexture16;
	EXESurfaceOld			* psTexture32;

	DWORD					dwTime;
	int						iStatus;   //0: disabled, 1: updating info..., 2: done!, 3: Error (nStatus)
	int						iSaveNum;
};

struct EXERenderCacheData
{
	EXERender		cRender;
	EXEMesh			cMesh;
};

struct EXEStageModel
{
	EXEModel				* pcModelBone;
	EXEModel				* pcModel;

	Point3D					sPosition;
	Point3D					sAngle;

	EXEStageModel			* psParent;

	int						iSum;

	DWORD					dwTimeUpdate;
};

static const DWORD							pdwSkillBuff = 0x03693AD8;

IMPFNC										pfnSetDynamicLight		= 0x00450D60;
IMPFNC										pfnSetCameraCharacter	= 0x0041E9C0;
IMPFNC										pfnSetCastleMaster		= 0x004292B0;
IMPFNC										pfnGetDamageFromArea	= 0x0040FB40;
IMPFNC										pfnRenderDropItemView	= 0x00428FB0;
IMPFNC										pfnSetupCamera			= 0x004019C0;
IMPFNC										pfnSetupCameraMatrix	= 0x0048B540;
IMPFNC										pfnSetDynamicLights		= 0x0040C050;
IMPFNC										pfnRenderSky			= 0x005AA330;
IMPFNC										pfnRenderStage			= 0x00450EB0;
IMPFNC										pfnRender3DAlpha		= 0x00454210;
IMPFNC										pfnRenderModelShadow	= 0x004548A0;
IMPFNC										pfnUpdateUITextures		= 0x00417EF0;
IMPFNC										pfnRenderEffects		= 0x005769F0;
IMPFNC										pfnRender3DEngine		= 0x00525C00;
IMPFNC										pfnRenderModel2D		= 0x00454950;
IMPFNC										pfnRenderZoomMap		= 0x0040B780;
IMPFNC										pfnFakeRenderGameState	= 0x00417FE0;
IMPFNC										pfnRenderGameState		= 0x004181D0;
IMPFNC										pfnUpdateMovementCamera = 0x004168A0;
IMPFNC										SetDynamicLight2		= 0x0040C080; //Leaked: SetDynLight

class EXERenderCache
{
protected:
	static std::vector<EXERenderCacheData*>  vRender;


public:
	static void								FinalRender();
	static void								FinalRenderHandler( EXEMesh * pcThis );
	static void								RenderCacheData();
};

static BOOL									AddUnitQueueID( int iID );

void										RenderDropItem( DropItemView * ps );

DropItemView *								FindScItem( int x, int z );

DropItemView *								FindEmptyScItem( );

void										RenderCharacterHP( UnitData * pcUnitData, int iX, int iY );

void										RenderCharacterChangingBar( UnitData * pcUnitData, int iX, int iY, int iMin, int iMax );

BOOL										Render3D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

BOOL										Render3DAlpha();

void WINAPI									Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

BOOL										IsCreatedNewRenderTarget();

static void									SetStageDynamicLight( Stage * pcStage, int iX, int iY, int iZ );

void										SetCharacterCamera( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

static void									SetClanMaster( DWORD dwID );

static int									GetDamageFromAreaSkill( UnitData * pcUnitData, UnitData * pcTarget, int iDestX, int iDestY, int iDestZ );

int											SetupCamera( struct EXECameraTrace * psCamera, int * iX, int * iY, int * iZ );

int											SetupCameraMatrix( EXECameraTrace * psCamera, int iX, int iY, int iZ, int iDX, int iDY, int iDZ );

void										RenderSky( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

int											RenderStage( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

int											RenderModelShadow( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );
void										UpdateUITextures();

void										RenderEffects( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

void										RenderModel2D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

void										Render3DEngine( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

void										RenderZoomMap();

void										PrepareUnitDataHUD( UnitData * p );
void										RenderUnitDataHUD();

void										UpdateMovementCamera();

void										FakeRenderGameState();
BOOL										RenderGameState();

void										SetDynamicLights();
void										SetDynamicLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iPower, int iTime = 10 );

int __stdcall								RenderStageMesh( EXEStageModel * psStageModel );

void										DrawMessageMiddle( const char * pszText );
void										DrawCharacterMessage( int iX, int iY, char * pszText, int iLineSize, DWORD dwClan, char * pszClanName, DWORD dwColor, BOOL bSelected, int iBellatra );

BOOL										LoadUnitData( UnitData * pcUnitData, char * pszBodyModel, char * pszHeadModel, Point3D * psPosition, Point3D * psAngle );

int											DrawGameInterface3D();

BOOL										RenderUnitData( UnitData * pcUnitData );

BOOL										ClientUnitDataUpdate( UnitData * pcUnitData );
BOOL										ClientMonsterUnitDataUpdate( UnitData * pcUnitData );
BOOL										ClientPlayerUnitDataUpdate( UnitData* pcUnitData );

int											AdjustMonsterDamageForBellatra( int iDamage );

void __cdecl								SendNpcUnitClick( UnitData * pcUnitData, int iState );
BOOL __cdecl								SendSingleDamageUnit( UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID, BOOL bChance, SocketData * pcSocketData );
BOOL __cdecl								SendAreaDamageUnit( int iX, int iY, int iZ, UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID );
BOOL 										ApplyLightingEnchantWeaponAoEOnTarget( UnitData * pcUnitTarget );


BOOL __cdecl								FreeUnitData( UnitData * pcUnitData );

void										RenderBlessCastleCrown( UnitData * pcUnitData );


// limite quest
#define MAX_QUEST 180
#define CLASS_PIKEMAN 1
#define CLASS_FIGHTER 2
#define CLASS_ARCHER 3
#define CLASS_ASSASSIN 4
#define CLASS_MECHANICIAN 5


struct ListNPC
{
	int			IDQuest;
	char        szNPCName[32];
	char		szNPCProgressName[32];
	char		szNPCCompleteName[32];
	int			szQuestLevel;
	int			szQuestClass;
	//char		szQuestClass[32] = { 0 };
};



int											QuestNPCCheck(char * NameNPC, int PlayerLVL); // CHECK QUEST NPC XXSTR*/

//int											SendQuestNPCCheck(char * NameNPC); // CHECK QUEST NPC XXSTR

//void										GetQuestNPCCheck(PacketQuestInfo * sPacket); // CHECK QUEST NPC XXSTR

#define CAMERACHARACTER						(SetCharacterCamera)


static void GetMoveLocation( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	CALL_WITH_ARG6( 0x0048AF40, iX, iY, iZ, iAX, iAY, iAZ );
}