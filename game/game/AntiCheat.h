#pragma once

#include "CAntiDebuggerHandler.h"
#include "CWMIReader.h"

struct SDebugSelf
{
	DWORD dwProcessID;
	DWORD dwThreadID;
};

class CAntiCheat
{
protected:
	struct PacketChecksumFunctionList			* psPacketChecksum		= NULL;
	
	DWORD										dwChecksumRet = 0;
	HANDLE										hThread = NULL;
private:
	static int									iaSkillCoolDown[10][20];


	BOOL										ChecksumCheckMismatch( struct ChecksumFunction * pChecksum );
	BOOL										GetModuleName( DWORD dwThreadID, char * pszBuf, int iSizeBuf );
	static DWORD WINAPI							EnumWindowsProc( HWND hWnd, LPARAM lParam );
	void										WindowFunctionHandler();


	//Handle Skill Cooldown
	BOOL										CheckCooldown();


	static DWORD								dwCheckSumDLL;
	static DWORD								dwCheckSumEXE;

	float										fTimeUpdate = 0.0f;
	float										fTimeUpdateSelf = 0.0f;

public:

	struct PacketWindowList						* psPacketWindowCheat	= NULL;

	static DWORD								dwThreadsID[8];

	bool										bCheckStart = false;

	CAntiCheat();
	virtual ~CAntiCheat();

	void										Update( float fTime );

	static std::string							GetEncDecodeString( std::string str );

	static DWORD								GetModuleChecksum( const std::string strFile );

	static void									DebugSelf( SDebugSelf * ps );

	BOOL										AddNewThreadException( DWORD dwThreadID );

	void										ChecksumFunctionHandler();

	static DWORD WINAPI							ThreadAntiCheatHandler();

	static void __cdecl							CheckThread();

	void										CheckTimingSpeedHack();

	void										ReceivePacketHandler( struct PacketChecksumFunctionList * psPacket );

	void										HandlePacket( PacketWindowList * psPacket );

	static void									AntiDebugger();

	static void									UnloadLibrary();

	static void									UpdateSkillCooldown();

	int											GetMultipleWindows( DWORD dwProcessID );

	static void WINAPI							DeepCheck();
	static void WINAPI							DeepCheckSelf();
	static void									DeepAnalyzeProcess( DWORD dwProcessId );

	static void									UpdateHardwareID();

};

