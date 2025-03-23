#include "stdafx.h"
#include "WinDump.h"
#include "Game.h"
#include "LaunchGame.h"

DWORD dwCurrentThreadProcessID = 0;

LONG WINAPI RedirectedSetUnhandledExceptionFilter( EXCEPTION_POINTERS * )
{
	return 0;
}

BOOL HookExceptionHandler()
{

	SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)&unhandled_handler );

	void * apiH = NULL;
	DWORD dwOld;

	// Get Proc Address
	apiH = GetProcAddress( GetModuleHandleA( "kernel32.dll" ), "SetUnhandledExceptionFilter" );

	if ( apiH == NULL )
		return FALSE;

	VirtualProtect( apiH, 5, PAGE_EXECUTE_READWRITE, &dwOld );
	*( BYTE* )( ( DWORD )apiH ) = 0xE9;

	*( DWORD* )( ( DWORD )apiH + 1 ) = ( DWORD )RedirectedSetUnhandledExceptionFilter - ( DWORD )apiH - 5;
	VirtualProtect( apiH, 5, dwOld, &dwOld );

	return TRUE;
}

typedef NTSTATUS( WINAPI * fLdrLoadDll ) //LdrLoadDll function prototype 
(
	IN PWCHAR PathToFile OPTIONAL,
	IN ULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName,
	OUT PHANDLE ModuleHandle
	);

DWORD dwDllLoaderHookedAddr = 0;

BOOL IsElevated()
{
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if ( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof( TOKEN_ELEVATION );
		if ( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) )
		{
			fRet = Elevation.TokenIsElevated;
		}
	}
	if ( hToken )
	{
		CloseHandle( hToken );
	}
	return fRet;
}

typedef NTSTATUS( WINAPI * fLdrLoadDll ) //LdrLoadDll function prototype 
(
	IN PWCHAR PathToFile OPTIONAL,
	IN ULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName,
	OUT PHANDLE ModuleHandle
	);


DWORD ProtectProcess()
{
#if defined(_DEBUG) || defined(DEV_MODE) || defined(_ANTICHEAT_OFF_)
	return 0;
#endif

	// Returned to caller
	DWORD dwResult = (DWORD)-1;

	// Released on exit
	HANDLE hToken = NULL;
	PVOID pTokenInfo = NULL;

	PSID psidEveryone = NULL;
	PSID psidSystem = NULL;
	PSID psidAdmins = NULL;

	PACL pDacl = NULL;
	PSECURITY_DESCRIPTOR pSecDesc = NULL;

	__try
	{
		// Scratch
		DWORD dwSize = 0;
		BOOL bResult = FALSE;

		// If this fails, you can try to fallback to OpenThreadToken
		if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_READ, &hToken ) )
		{
			dwResult = GetLastError();

			__leave; /*failed*/
		}

		bResult = GetTokenInformation( hToken, TokenUser, NULL, 0, &dwSize );
		dwResult = GetLastError();

		if ( !(bResult == FALSE && ERROR_INSUFFICIENT_BUFFER == dwResult) ) { __leave; /*failed*/ }

		if ( dwSize )
		{
			pTokenInfo = HeapAlloc( GetProcessHeap(), 0, dwSize );
			dwResult = GetLastError();

			if ( NULL == pTokenInfo ) { __leave; /*failed*/ }
		}

		bResult = GetTokenInformation( hToken, TokenUser, pTokenInfo, dwSize, &dwSize );
		dwResult = GetLastError();

		if ( !(bResult && pTokenInfo) ) { __leave; /*failed*/ }

		PSID psidCurUser = ((TOKEN_USER *)pTokenInfo)->User.Sid;

		SID_IDENTIFIER_AUTHORITY sidEveryone = SECURITY_WORLD_SID_AUTHORITY;
		bResult = AllocateAndInitializeSid( &sidEveryone, 1,
			SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &psidEveryone );
		dwResult = GetLastError();

		if ( !(bResult && psidEveryone) ) { __leave; /*failed*/ }

		SID_IDENTIFIER_AUTHORITY sidSystem = SECURITY_NT_AUTHORITY;
		bResult = AllocateAndInitializeSid( &sidSystem, 1,
			SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &psidSystem );
		dwResult = GetLastError();

		if ( !(bResult && psidSystem) ) { __leave; /*failed*/ }

		SID_IDENTIFIER_AUTHORITY sidAdministrators = SECURITY_NT_AUTHORITY;
		bResult = AllocateAndInitializeSid( &sidAdministrators, 2,
			SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0, &psidAdmins );
		dwResult = GetLastError();

		if ( !(bResult && psidAdmins) ) { __leave; /*failed*/ }

		const PSID psidArray[] = {
		psidEveryone, /* Deny most rights to everyone */
		psidCurUser, /* Allow what was not denied */
		psidSystem, /* Full control */
		psidAdmins, /* Full control */
		};

		// Determine required size of the ACL
		dwSize = sizeof( ACL );

		// First the DENY, then the ALLOW
		dwSize += GetLengthSid( psidArray[0] );
		dwSize += sizeof( ACCESS_DENIED_ACE ) - sizeof( DWORD );

		for ( UINT i = 1; i < _countof( psidArray ); i++ )
		{
			// DWORD is the SidStart field, which is not used for absolute format
			dwSize += GetLengthSid( psidArray[i] );
			dwSize += sizeof( ACCESS_ALLOWED_ACE ) - sizeof( DWORD );
		}

		pDacl = (PACL)HeapAlloc( GetProcessHeap(), 0, dwSize );
		dwResult = GetLastError();

		if ( NULL == pDacl ) { __leave; /*failed*/ }

		bResult = InitializeAcl( pDacl, dwSize, ACL_REVISION );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		// Mimic Protected Process
		// http://www.microsoft.com/whdc/system/vista/process_vista.mspx
		// Protected processes allow PROCESS_TERMINATE, which is
		// probably not appropriate for high integrity software.
		static const DWORD dwPoison =
			/*READ_CONTROL |*/ WRITE_DAC | WRITE_OWNER |
			PROCESS_CREATE_PROCESS | PROCESS_CREATE_THREAD |
			PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION |
			PROCESS_SET_QUOTA | PROCESS_SET_INFORMATION |
			PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE |
			// In addition to protected process
			PROCESS_SUSPEND_RESUME | PROCESS_TERMINATE;
		bResult = AddAccessDeniedAce( pDacl, ACL_REVISION, dwPoison, psidArray[0] );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		// Standard and specific rights not explicitly denied
		static const DWORD dwAllowed = ~dwPoison & 0x1FFF;
		bResult = AddAccessAllowedAce( pDacl, ACL_REVISION, dwAllowed, psidArray[1] );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		// Because of ACE ordering, System will effectively have dwAllowed even
		// though the ACE specifies PROCESS_ALL_ACCESS (unless software uses
		// SeDebugPrivilege or SeTcbName and increases access).
		// As an exercise, check behavior of tools such as Process Explorer under XP,
		// Vista, and above. Vista and above should exhibit slightly different behavior
		// due to Restricted tokens.
		bResult = AddAccessAllowedAce( pDacl, ACL_REVISION, PROCESS_ALL_ACCESS, psidArray[2] );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		// Because of ACE ordering, Administrators will effectively have dwAllowed
		// even though the ACE specifies PROCESS_ALL_ACCESS (unless the Administrator
		// invokes 'discretionary security' by taking ownership and increasing access).
		// As an exercise, check behavior of tools such as Process Explorer under XP,
		// Vista, and above. Vista and above should exhibit slightly different behavior
		// due to Restricted tokens.
		bResult = AddAccessAllowedAce( pDacl, ACL_REVISION, PROCESS_ALL_ACCESS, psidArray[3] );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		pSecDesc = (PSECURITY_DESCRIPTOR)HeapAlloc( GetProcessHeap(), 0, SECURITY_DESCRIPTOR_MIN_LENGTH );
		dwResult = GetLastError();

		if ( NULL == pSecDesc ) { __leave; /*failed*/ }

		// InitializeSecurityDescriptor initializes a security descriptor in
		// absolute format, rather than self-relative format. See
		// http://msdn.microsoft.com/en-us/library/aa378863(VS.85).aspx
		bResult = InitializeSecurityDescriptor( pSecDesc, SECURITY_DESCRIPTOR_REVISION );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		bResult = SetSecurityDescriptorDacl( pSecDesc, TRUE, pDacl, FALSE );
		dwResult = GetLastError();

		if ( FALSE == bResult ) { __leave; /*failed*/ }

		dwResult = SetSecurityInfo(
			GetCurrentProcess(),
			SE_KERNEL_OBJECT, // process object
			OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
			psidCurUser, // NULL, // Owner SID
			NULL, // Group SID
			pDacl,
			NULL // SACL
		);
		dwResult = GetLastError();

		if ( ERROR_SUCCESS != dwResult ) { __leave; /*failed*/ }

		dwResult = ERROR_SUCCESS;
	}
	__finally
	{
		if ( NULL != pSecDesc )
		{
			HeapFree( GetProcessHeap(), 0, pSecDesc );
			pSecDesc = NULL;
		}
		if ( NULL != pDacl )
		{
			HeapFree( GetProcessHeap(), 0, pDacl );
			pDacl = NULL;
		}
		if ( psidAdmins )
		{
			FreeSid( psidAdmins );
			psidAdmins = NULL;
		}
		if ( psidSystem )
		{
			FreeSid( psidSystem );
			psidSystem = NULL;
		}
		if ( psidEveryone )
		{
			FreeSid( psidEveryone );
			psidEveryone = NULL;
		}
		if ( NULL != pTokenInfo )
		{
			HeapFree( GetProcessHeap(), 0, pTokenInfo );
			pTokenInfo = NULL;
		}
		if ( NULL != hToken )
		{
			CloseHandle( hToken );
			hToken = NULL;
		}
	}

	return dwResult;
}

NAKED NTSTATUS __stdcall _DllLoaderHooked( IN PWCHAR PathToFile OPTIONAL,
	IN ULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName,
	OUT PHANDLE ModuleHandle )
{
	__asm
	{
		MOV EDI, EDI;
		PUSH EBP;
		MOV EBP, ESP;
		MOV EAX, dwDllLoaderHookedAddr;
		JMP EAX;
	};
}


//Prevent Infinite Loop
BOOL bSafeDLL = FALSE;
NTSTATUS WINAPI DllLoaderHooker( IN PWCHAR PathToFile OPTIONAL,
	IN ULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName,
	OUT PHANDLE ModuleHandle )
{
	//Isn't the current Thread?
	if ( (GetCurrentThreadId() != dwCurrentThreadProcessID) && (bSafeDLL == FALSE) )
	{
		BOOL bNotInject = FALSE;

		bSafeDLL = TRUE;

		//Set to Lower Case String
		std::wstring wstr = ModuleFileName->Buffer;
		std::string str( wstr.begin(), wstr.end() );
		std::transform( str.begin(), str.end(), str.begin(), []( unsigned char c ) { return std::tolower( c ); } );

		//Have a Dir?
		if ( str.find( AY_OBFUSCATE( ":" ) ) != std::string::npos )
		{
			//Isn't a System Dir? Don't Inject!
			if ( str.find( AY_OBFUSCATE( "\\windows" ) ) == std::string::npos )
				bNotInject = TRUE;
		}
		else
		{
			//Isn't a DLL MS-API (Win8 or Higher)? Don't Inject!
			if ( str.find( AY_OBFUSCATE( "api-ms" ) ) == std::string::npos )
			{
				//In the same client folder? Don't Inject!
				if ( FileExist( str.c_str() ) )
					bNotInject = TRUE;
			}
		}

		bSafeDLL = FALSE;

		if ( bNotInject )
			return 0;
	}

	return _DllLoaderHooked( PathToFile, Flags, ModuleFileName, ModuleHandle );
}

BOOL HookAntiInjection()
{
#if defined(_DEBUG) || defined(DEV_MODE) || defined(_ANTICHEAT_OFF_)
	return TRUE;
#endif

	void * apiH = NULL;
	DWORD dwOld;

	// Get Proc Address
	apiH = GetProcAddress( GetModuleHandleA( AY_OBFUSCATE( "ntdll.dll" ) ), AY_OBFUSCATE( "LdrLoadDll" ) );

	if ( apiH == NULL )
		return FALSE;

	dwDllLoaderHookedAddr = (DWORD)(apiH)+5;

	VirtualProtect( apiH, 5, PAGE_EXECUTE_READWRITE, &dwOld );
	*(BYTE *)((DWORD)apiH) = 0xE9;

	*(DWORD *)((DWORD)apiH + 1) = (DWORD)DllLoaderHooker - (DWORD)apiH - 5;
	VirtualProtect( apiH, 5, dwOld, &dwOld );

	return TRUE;
}


extern "C" int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HookExceptionHandler();

	GameCore::Hooks();

	// Verify code launcher
	Launch_ptr pLaunchGame = std::make_shared<Launch>();
	pLaunchGame->OpenGameHandler( lpCmdLine );

	ProtectProcess();

	dwCurrentThreadProcessID = GetCurrentThreadId();

	HookAntiInjection();

	//New Code
	CApplication * pcApplication = new CApplication( new CGameWindow() );
	if ( pcApplication->Init( hInstance ) )
	{
		pcApplication->Run();
	}
	else
	{
		if ( pcApplication->GetInitCode() == -1 )
		{

		}
	}

	SAFE_DELETE( pcApplication );

	return 0;
}
