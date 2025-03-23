#include "stdafx.h"
#include "CWindow.h"

CWindow::CWindow()
{
	bRegisterSuccess		= FALSE;

	hWnd					= NULL;

	dwUpdateTimeInterval	= 0;

	bExit					= FALSE;
	bRunning				= FALSE;
	bMinimized				= FALSE;
}

CWindow::~CWindow()
{
}

void CWindow::LoadWindowMessages()
{
	if( !bRunning )
		return;

	MSG msg;
	while( PeekMessageA( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( ProcessMessage( msg ) )
		{
			if( (msg.message != WM_SYSKEYUP) && (msg.message != WM_SYSKEYDOWN) )
			{
				TranslateMessage( &msg );
				DispatchMessageA( &msg );
			}
		}
	}
}

void CWindow::AddDialog( CDialog * pcDialog )
{
	if( pcDialog == NULL )
		return;

	vDialogs.push_back( pcDialog );
}

void CWindow::RemoveDialog( CDialog * pcDialog )
{
	if( pcDialog == NULL )
		return;
	
	for( vector<CDialog*>::iterator it = vDialogs.begin(); it != vDialogs.end(); it++ )
	{
		CDialog * pcCur = *it;

		if( pcCur == pcDialog )
		{
			SHUTDOWN( pcDialog );
			SAFE_DELETE( pcDialog );

			vDialogs.erase( it );
			break;
		}
	}
}

BOOL CWindow::Register( const char * pszClassName, UINT uStyle, UINT uIcon, HBRUSH hbrBackground )
{
	WNDCLASSEXA wcex = { 0 };

	wcex.cbSize			= sizeof( WNDCLASSEX );

	wcex.style			= uStyle;
	wcex.lpfnWndProc	= (WNDPROC)StaticWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= pApplication->GetProcess()->GetInstanceHandle();
	wcex.hIcon			= LoadIcon( wcex.hInstance, MAKEINTRESOURCE( uIcon ) );
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= hbrBackground;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= pszClassName;
	wcex.hIconSm		= LoadIcon( wcex.hInstance, MAKEINTRESOURCE( uIcon ) );

	bRegisterSuccess	= RegisterClassExA( &wcex ) != 0;

	if( bRegisterSuccess )
		STRINGCOPY( szClassName, pszClassName );

	return bRegisterSuccess;
}

void CWindow::Unregister()
{
	if( bRegisterSuccess )
	{
		UnregisterClass( szClassName, pApplication->GetProcess()->GetInstanceHandle() );

		bRegisterSuccess = FALSE;
	}
}

BOOL CWindow::MakeWindow( LPCSTR lpszTitle, BOOL bServer, BOOL bWindowed, BOOL bMaximized, int iWidth, int iHeight )
{
	HINSTANCE hInstance = pApplication->GetProcess()->GetInstanceHandle();

	if( hWnd != NULL )
		RemoveWindow();

	if( bServer )
	{ //Server
		hWnd = CreateWindowEx( WS_EX_SERVER, "Server", lpszTitle, WS_SERVER, CW_USEDEFAULT, 0, iWidth, iHeight, NULL, NULL, hInstance, this );
	}
	else
	{
		if( bWindowed )
		{ //Windowed
			SIZE sz;
			RECT rc;
			rc.top = rc.left = 0;
			rc.right = iWidth;
			rc.bottom = iHeight;
			AdjustWindowRect( &rc, WS_WINDOWED, FALSE );
			sz.cx = rc.right - rc.left;
			sz.cy = rc.bottom - rc.top;

			SIZE ps;
			if( !bMaximized )
			{
				ps.cx = (GetSystemMetrics( SM_CXSCREEN ) >> 1) - (sz.cx >> 1);
				ps.cy = (GetSystemMetrics( SM_CYSCREEN ) >> 1) - (sz.cy >> 1);
			}
			else
			{
				ps.cx = CW_USEDEFAULT;
				ps.cy = 0;
			}

			hWnd = CreateWindowEx( WS_EX_WINDOWED, lpszTitle, lpszTitle, WS_WINDOWED, ps.cx, ps.cy, sz.cx, sz.cy, NULL, NULL, hInstance, this );
		}
		else
		{ //Fullscreen
			hWnd = CreateWindowEx( WS_EX_FULLSCREEN, lpszTitle, lpszTitle, WS_FULLSCREEN, CW_USEDEFAULT, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), NULL, NULL, hInstance, this );
		}
	}
	
	return hWnd != NULL;
}

BOOL CWindow::PumpWindowMessages()
{
	if( bExit )
		return !bExit;

	MSG msg;
	while( PeekMessageA( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( ProcessMessage( msg ) )
		{
			if( msg.message == WM_QUIT )
			{
				bExit = TRUE;
				break;
			}

			TranslateMessage( &msg );
			DispatchMessageA( &msg );
		}
	}

	return !bExit;
}

void CWindow::LoopWindowMessages()
{
	HANDLE hThread = CreateThread( NULL, KB2, (LPTHREAD_START_ROUTINE)UpdaterThread, this, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );

	MSG msg;
	while( GetMessageA( &msg, NULL, 0, 0 ) )
	{
		if( ProcessMessage( msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessageA( &msg );
		}
	}

	bExit = TRUE;

	WaitForSingleObject( hThread, INFINITE );
	CloseHandle( hThread );
}

BOOL CWindow::IsActive( BOOL bIncludeDialogs )
{
	HWND hWndForeground = GetForegroundWindow();

	if( hWnd == hWndForeground )
		return TRUE;

	if( bIncludeDialogs )
	{
		for( vector<CDialog*>::iterator it = vDialogs.begin(); it != vDialogs.end(); it++ )
		{
			CDialog * pcDialog = *it;

			if( pcDialog->GetWindowHandle() == hWndForeground )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CWindow::IsDialog( HWND hWnd )
{
	for( vector<CDialog*>::iterator it = vDialogs.begin(); it != vDialogs.end(); it++ )
	{
		CDialog * pcDialog = *it;

		if( pcDialog->GetWindowHandle() == hWnd )
			return TRUE;
	}

	return FALSE;
}

BOOL CWindow::ProcessMessage( MSG & msg )
{
	for( vector<CDialog*>::iterator it = vDialogs.begin(); it != vDialogs.end(); it++ )
	{
		CDialog * pcDialog = *it;

		if( IsDialogMessage( pcDialog->GetWindowHandle(), &msg ) )
			return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK CWindow::StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CWindow * pcParent = NULL;

	if( uMsg == WM_CREATE )
	{
		pcParent = (CWindow *)((LPCREATESTRUCT)lParam)->lpCreateParams;
		pcParent->hWnd = hWnd;

		SetWindowLong( hWnd, GWL_USERDATA, (LONG)pcParent );
	}

	if( pcParent == NULL )
		pcParent = (CWindow *)GetWindowLongPtr( hWnd, GWL_USERDATA );

	if( pcParent == NULL )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );

	return pcParent->WndProc( uMsg, wParam, lParam );
}

DWORD WINAPI CWindow::UpdaterThread( CWindow * pThis )
{
	HWND hWnd					= pThis->hWnd;
	DWORD dwUpdateTimeInterval	= pThis->dwUpdateTimeInterval;
	BOOL * ex					= &pThis->bExit;
	BOOL bMultiCore				= pThis->GetApplication()->GetSystem()->IsMultiCore();

	LARGE_INTEGER liFrequency, liLastTick, liNewTick;

	QueryPerformanceFrequency( &liFrequency );
	double dFrequency = (double)liFrequency.QuadPart;

	if( bMultiCore )
		SetThreadAffinityMask( GetCurrentThread(), 1 );

	QueryPerformanceCounter( &liLastTick );
	while( (*ex) == FALSE )
	{
		Sleep( dwUpdateTimeInterval );

		QueryPerformanceCounter( &liNewTick );

		union
		{
			double fTime;
			uint32_t fTimeU32[2];
		} fTimeStruct;

		fTimeStruct.fTime = (((double)(liNewTick.QuadPart - liLastTick.QuadPart)) * 1000.0f) / dFrequency;
		liLastTick = liNewTick;

		SendMessageA( hWnd, WM_UPDATE, (WPARAM)fTimeStruct.fTimeU32[0], (LPARAM)fTimeStruct.fTimeU32[1]);
	}

	return TRUE;
}

void CWindow::RemoveDialogs()
{
	for( vector<CDialog*>::iterator it = vDialogs.begin(); it != vDialogs.end(); it++ )
	{
		CDialog * pcDialog = *it;

		SHUTDOWN( pcDialog );
		SAFE_DELETE( pcDialog );
	}
	vDialogs.clear();
}

void CWindow::RemoveWindow()
{
	RemoveDialogs();

	if( IsWindow( hWnd ) )
		DestroyWindow( hWnd );

	hWnd = NULL;
}