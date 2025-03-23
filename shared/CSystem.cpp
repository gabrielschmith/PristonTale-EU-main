#include "stdafx.h"
#include "CSystem.h"

CSystem::CSystem()
{
	os_version_info.dwOSVersionInfoSize = sizeof( os_version_info );
	GetVersionEx( (LPOSVERSIONINFO)&os_version_info );

	os_version_info_ex.dwOSVersionInfoSize = sizeof( os_version_info_ex );
	GetVersionEx( (LPOSVERSIONINFO)&os_version_info_ex );

	GetSystemInfo( (LPSYSTEM_INFO)&sys_info );

	lang_id = GetUserDefaultLangID();
}

ESystemOS CSystem::GetSystemOS()
{
	OSVERSIONINFO * v = &os_version_info;
	OSVERSIONINFOEX * e = &os_version_info_ex;

	if( v->dwMajorVersion == 5 && v->dwMinorVersion == 0 )
		return SYSTEMOS_Windows2000;

	if( v->dwMajorVersion == 5 && v->dwMinorVersion == 1 )
		return SYSTEMOS_WindowsXP;

	if( v->dwMajorVersion == 5 && v->dwMinorVersion == 2 )
	{
		if( GetSystemMetrics( SM_SERVERR2 ) != 0 )
			return SYSTEMOS_Windows2003R2;

		return SYSTEMOS_Windows2003;
	}

	if( v->dwMajorVersion == 6 && v->dwMinorVersion == 0 )
	{
		if( e->wProductType != VER_NT_WORKSTATION )
			return SYSTEMOS_Windows2008;

		return SYSTEMOS_WindowsVista;
	}

	if ( v->dwMajorVersion == 6 && v->dwMinorVersion == 1 )
	{
		if ( e->wProductType != VER_NT_WORKSTATION )
			return SYSTEMOS_Windows2008R2;

		return SYSTEMOS_Windows7;
	}

	if ( v->dwMajorVersion == 6 && v->dwMinorVersion == 2 )
	{
		if ( e->wProductType != VER_NT_WORKSTATION )
			return SYSTEMOS_Windows2012;

		return SYSTEMOS_Windows8;
	}

	if ( v->dwMajorVersion == 6 && v->dwMinorVersion == 3 )
	{
		if ( e->wProductType != VER_NT_WORKSTATION )
			return SYSTEMOS_Windows2012R2;

		return SYSTEMOS_Windows8_1;
	}

	if ( v->dwMajorVersion == 10 && v->dwMinorVersion == 0 )
	{
		if ( e->wProductType != VER_NT_WORKSTATION )
			return SYSTEMOS_Windows2016;

		return SYSTEMOS_Windows10;
	}

	return SYSTEMOS_Unknown;
}

ESystemLanguage CSystem::GetSystemLanguage()
{
	switch( lang_id )
	{
	case 0x0412:
		return SYSTEMLANGUAGE_Korean;
	case 0x0C04:
	case 0x1404:
	case 0x1004:
	case 0x0004:
	case 0x7C04:
		return SYSTEMLANGUAGE_ChineseSimplified;
	case 0x0411:
		return SYSTEMLANGUAGE_Japanese;
	case 0x041E:
		return SYSTEMLANGUAGE_Thai;
	}

	return SYSTEMLANGUAGE_English;
}

BOOL CSystem::IsMultiCore()
{
	return (sys_info.dwNumberOfProcessors > 1);
}

int CSystem::GetNumberOfCores()
{
	return sys_info.dwNumberOfProcessors;
}

UINT CSystem::GetRAMAmount()
{
	MEMORYSTATUSEX sMemoryStatus;
	sMemoryStatus.dwLength = sizeof( MEMORYSTATUSEX );
	if ( GlobalMemoryStatusEx( &sMemoryStatus ) )
		return (UINT)(sMemoryStatus.ullTotalPhys / 1024000);

	return 0;
}

int CSystem::ComputeFrameDelayTime()
{
	int iNumberOfCores = GetNumberOfCores();

	if( iNumberOfCores > 2 )
		return 50;

	if( iNumberOfCores == 2 )
		return 100;

	return 250;
}

BOOL CSystem::IsVistaOrLater()
{
	if( os_version_info_ex.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		os_version_info_ex.dwMajorVersion >= 6 )
		return TRUE;

	return FALSE;
}

BOOL CSystem::IsElevated()
{
	if( IsVistaOrLater() == FALSE )
		return TRUE;

	HANDLE hToken;

	if( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) == FALSE )
		return TRUE;

	TOKEN_ELEVATION te = { 0 };
	DWORD dwReturnLength = 0;

	if( GetTokenInformation( hToken, TokenElevation, &te, sizeof( te ), &dwReturnLength ) == FALSE )
	{
		CloseHandle( hToken );

		return TRUE;
	}

	BOOL bElevated = te.TokenIsElevated ? TRUE : FALSE;

	CloseHandle( hToken );

	return bElevated;
}

BOOL CSystem::OpenURL( char * pszURL )
{
	if( pszURL != NULL )
	{
		if( pszURL[0] == 'h' && 
			pszURL[1] == 't' && 
			pszURL[2] == 't' && 
			pszURL[3] == 'p' )
		{
			UINT iSE = (UINT)ShellExecuteA( NULL, "open", pszURL, NULL, NULL, SW_SHOWNORMAL );

			if( iSE > 32 )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CSystem::OpenURLEx( char * pszFormatURL, ... )
{
	va_list vl;

	char szURL[MAX_PATH];

	va_start( vl, pszFormatURL );
	_vsnprintf_s( szURL, _countof( szURL ), _TRUNCATE, pszFormatURL, vl );
	va_end( vl );

	return OpenURL( szURL );
}