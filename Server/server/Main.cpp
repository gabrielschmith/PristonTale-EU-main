#include "stdafx.h"
#include "WinDump.h"
#include "CServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>  // For _CrtSetReportMode

BOOL HookAPI( char* moduleName, char* apiName, void* callBack, void* backup = NULL )
{
	DWORD old;

	void * pAPIHandle = GetProcAddress( GetModuleHandleA( moduleName ), apiName );

	if ( pAPIHandle == NULL )
		return FALSE;

	if ( backup != NULL )
		MoveMemory( backup, pAPIHandle, 5 );

	VirtualProtect( pAPIHandle, 5, PAGE_EXECUTE_READWRITE, &old );
	*(BYTE*)((DWORD)pAPIHandle) = 0xE9;
	*(DWORD*)((DWORD)pAPIHandle + 1) = (DWORD)callBack - (DWORD)pAPIHandle - 5;
	VirtualProtect( pAPIHandle, 5, old, &old );

	return TRUE;
}

LONG WINAPI RedirectedSetUnhandledExceptionFilter( EXCEPTION_POINTERS * p )
{
	return NULL;
}

void Launch()
{
	STARTUPINFOA s = { 0 };
	PROCESS_INFORMATION p = { 0 };

	/// Build the server hopefully console will pop up
	// Debug Console
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");

	CreateProcessA( NULL, """server.exe""", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &s, &p );
}

void myInvalidParameterHandler(const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	wprintf(L"Invalid parameter detected in function %s."
		L" File: %s Line: %d\n", function, file, line);
	wprintf(L"Expression: %s\n", expression);

	create_minidump(NULL);

	abort();
}



extern "C" int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SetUnhandledExceptionFilter( unhandled_handler );
	HookAPI( "kernel32.dll","SetUnhandledExceptionFilter", RedirectedSetUnhandledExceptionFilter );
	
    //See: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/set-invalid-parameter-handler-set-thread-local-invalid-parameter-handler?view=vs-2019
	_invalid_parameter_handler oldHandler, newHandler;
	newHandler = myInvalidParameterHandler;
	oldHandler = _set_invalid_parameter_handler(newHandler);

	// Disable the message box for assertions.
	_CrtSetReportMode(_CRT_ASSERT, 0);

	//To test above, place the following code anywhere.
	//char* formatString;
	//formatString = NULL;
	//printf(formatString);

	//Pointer Window Procedure
	*( UINT* )0x08B813B0 = ( UINT )0x00574000;
	*(UINT*)0x08B867C0 = (UINT)&GetMessageA;

	pcApplication = new CApplication( new CServerWindow() );
	

	if ( pcApplication->Init( hInstance ) )
	{
		pcApplication->Run();

		if ( pcApplication->Shutdown() )
			Launch();
	}
	else
	{
		if ( pcApplication->GetInitCode() == -1 )
			Launch();
	}

	SAFE_DELETE( pcApplication );

	return 0;
}