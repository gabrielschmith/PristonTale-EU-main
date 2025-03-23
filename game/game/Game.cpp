#include "stdafx.h"
#include "Game.h"
#include "CSound.h"
#include "Controller.h"

extern BOOL								bMouseCreatedHandler;

static BOOL								bKillFocus = FALSE;

char Game::szHardwareID[40];

// Window Procedure
static LRESULT							CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

extern GameCore										* pGameCoreHandler;

// Return Window Procedure of PT
typedef									LRESULT( CALLBACK *t_WndProcPT ) ( HWND, UINT, WPARAM, LPARAM );
t_WndProcPT								WndProcPT = ( t_WndProcPT )0x0041C330;

Game::Game()
{
}

Game::~Game()
{
}


EXEModelData * Game::ReadInx( char * pszFile )
{

	typedef EXEModelData *( __thiscall *tfnReadINX )(DWORD dwClass, char * pszFile);
	tfnReadINX ReadINX = (tfnReadINX)0x0041EBE0;

	EXEModelData * ps = ReadINX( 0x00A79248, pszFile );

	if ( ps )
		ANIMATIONHANDLER->HandleBoneCache( ps );

	return ps;
}

void Game::RenderInit()
{
	CALLT( 0x0047F2E0, 0x03245878 );
}

void Game::RenderClipStateSet( DWORD dw )
{
	typedef void( __thiscall *tfnP )(DWORD dwClass, DWORD p);
	tfnP fnp = (tfnP)0x0047F2F0;
	fnp( 0x03245878, dw );
}

void Game::RenderClip()
{
	typedef void( __thiscall *tfnP )(DWORD dwClass);
	tfnP fnp = (tfnP)0x00483D10;
	fnp( 0x03245878 );
}

void Game::RenderGeomVertex2D(DWORD dwColor)
{
	typedef void( __thiscall *tfnP )(DWORD dwClass, DWORD dw);
	tfnP fnp = (tfnP)0x00483EA0;
	fnp( 0x03245878, dwColor );
}

void Game::RenderD3D()
{
	typedef void( __thiscall *tfnP )(DWORD dwClass);
	tfnP fnp = (tfnP)0x004849B0;
	fnp( 0x03245878 );
}

void Game::AnimObjectTree( void * pObject, int iFrame, int iAX, int iAY, int iAZ )
{
	typedef void( __cdecl *tfnP )( void * pObject, int iFrame, int iAX, int iAY, int iAZ );
	tfnP fnP = (tfnP)0x0041E710;
	fnP( pObject, iFrame, iAX, iAY, iAZ );
}

void Game::CreateInx( char * pszFile )
{
	char szFileName[MAX_PATH] = { 0 };
	STRINGCOPY( szFileName, pszFile );

	typedef BOOL( __cdecl *tfnReadINX )(char * pszFile);
	tfnReadINX ReadINX = (tfnReadINX)0x00457BA0;

	char * pBuffer = NULL;
	DWORD dwAddr = 0;
	if ( ReadINX( pszFile ) )
	{
		DWORD dwCount = (*(DWORD*)0x0209EA48) - 1;
		dwAddr = (*(DWORD*)(0x02081628 + (dwCount * 4)));
		if ( dwAddr )
		{
			pBuffer = (char*)(*(DWORD*)(dwAddr + 0x54));
			if ( pBuffer )
			{
				ChangeFileExtension( pszFile, "inx", szFileName );
				FILE * fp;
				fopen_s( &fp, szFileName, "wb" );
				if ( fp )
				{
					fwrite( pBuffer, 95268, 1, fp );
					fclose( fp );
				}
			}
		}
	}
}

BOOL Game::Initialize()
{
	int iScreenWidth, iScreenHeight;

	// hInstance
	hInst = *( HINSTANCE* )0x0A1752C;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	SETTINGSHANDLER->GetModel()->ReadSettings();
	iScreenWidth = SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
	iScreenHeight = SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;

	//STRINGCOPYLEN( (char*)0x039033E8, 32, SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount );
	STRINGCOPYLEN((char*)0x039033E8, 32, "testing so much");

	// Create iSound
	DX::Sound_ptr pSound = std::make_shared<DX::Sound>();
	pSound->CreateDevices();

	DeleteClanTag("Babel");
	DeleteClanTag("Seasonal");

	DX::Graphic::ResolutionChange( TRUE, iScreenWidth, iScreenHeight ); 

	// Initialize the windows api.
	InitializeWindow( iScreenWidth, iScreenHeight );

	// Create Edit Window
	HWND hWndEdt = CreateWindowEx( NULL, "EDIT", "", 0x40801004, 10, 400, 500, 40, hWindow, ( HMENU )101, hInst, NULL );

	SetWindowTextA( hWndEdt, (char*)0x039033E8 );

	// Copy hWndEdit for Executable
	*( UINT* )0x0A17544 = ( UINT )hWndEdt;
	*( UINT* )0x0A15DF4 = SetWindowLong( hWndEdt, GWL_WNDPROC, 0x416810 );
	*( UINT* )0x0A176DC = ( UINT )hWndEdt;
	*( UINT* )0x0A17418 = ( UINT )ImmGetContext( hWindow );
	*( UINT* )0x0A15DE0 = ( UINT )ImmGetContext( hWndEdt );

	// Initial Context
	CALL_WITH_ARG1( 0x00416780, ( DWORD )0 );

	// Initialize Font
	CALL( 0x0041A210 );

	// Initialize Font System
	FONTHANDLER->Init();
	FONTHANDLER->CreateFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	FONTHANDLER->CreateFont( "Arial", 15, 6, FALSE, FALSE, -1 );

	// Initialization Game
	CALL_WITH_ARG1( 0x00417D70, ( DWORD )1 );

	// Set Window Timer
	SetTimer( hWindow, 0, 100, 0 );
	SetTimer( hWindow, 0x6A6A6A6A, 850, 0 );
	SetTimer( hWindow, 0x6A6A6A6B, 10, 0 );

	// Set sockets to 0
	WRITEBYTE( 0x38FED40, 0 );
	WRITEBYTE( 0x38FEE40, 0 );
	WRITEDWORD( 0x04C38C28, 27242 );

	// Time Sleep when game is minimized
	WRITEDWORD( 0x824F6C, 200 );


	pGameCoreHandler->Init();

	//Multi Core?
	if ( SYSTEM->IsMultiCore() )
	{
		DWORD_PTR dw = 0x1;
		
		//Quad Core?
		if ( SYSTEM->GetNumberOfCores() == 4 )
		{
			//Update for 3 Cores, since 4 cores not working( ??? )
			dw = 0xE;
		}
		else
		{
			//Update Cores
			for ( int i = 1; i < SYSTEM->GetNumberOfCores(); i++ )
				dw |= 1 << i;
		}

		//Set Affinity
	//	SetProcessAffinityMask( GetCurrentProcess(), dw );
	}

	// RenderD3D
	CALL( 0x0041C2E0 );

	// Shutdown Game
	CALL( 0x0046D1E0 );
	CALL( 0x00417810 );
	CALL( 0x0045A410 );

	DEVMODE devMode;
	ZeroMemory( &devMode, sizeof( DEVMODE ) );
	MoveMemory( &devMode, ( void* )0x0A16268, sizeof( DEVMODE ) );

	// If is fullscreen
	if ( *( int* )0x009CB028 == 0 )
	{
		if ( devMode.dmDriverVersion != 0 )
			ChangeDisplaySettingsA( &devMode, CDS_UPDATEREGISTRY );
	}

	return TRUE;
}

HINTERNET WINAPI Game::InternetOpenUrlClan( HINTERNET hInternet, LPCTSTR lpszUrl, LPCTSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext )
{
	return InternetOpenUrlA( hInternet, lpszUrl, "Accept: */*\r\n\r\n", lstrlenA( "Accept: */*\r\n\r\n" ), INTERNET_FLAG_DONT_CACHE, NULL );
}

void Game::OnSetRGBATimer()
{
	static int isR, isG, isB;
	static DWORD dwTimerColorStage;
	int iHour	= (*(int*)0x00826D10);
	int iMinute = (*(int*)0x00CF4798);

	int & R = (*(int*)0x034DF460);
	int & G = (*(int*)0x034DF464);
	int & B = (*(int*)0x034DF468);
	int & D = (*(int*)0x00CF4774);

	R -= D;
	G -= D;
	B -= D;
	isR = R;
	isG = G;
	isB = B;
}

void Game::Shutdown()
{
	// Shutdown the window.
	ShutdownWindow();
}

void Game::Run()
{
	MSG msg;
	BOOL bDone;

	// Initialize the message structure.
	ZeroMemory( &msg, sizeof( MSG ) );

	// Loop until there is a quit message from the window or the user.
	bDone = FALSE;
	while ( !bDone )
	{
		// Handle the windows messages.
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// If windows signals to end the application then exit out.
		if ( msg.message == WM_QUIT )
			bDone = TRUE;
		else
			bDone = TRUE;
	}
}

void Game::SetMousePos( int iX, int iY )
{
	*( int* )0x0A17518 = iX;
	*( int* )0x0A1751C = iY;
}

void Game::GetMousePos( D3DXVECTOR2 * pVec )
{
	// Is vector?
	if ( pVec )
	{
		// Set mouse pos in vector
		pVec->x = ( float )*( int* )0x0A17518;
		pVec->y = ( float )*( int* )0x0A1751C;
	}
}

BOOL Game::IsPoint2D( int iX, int iY, int iW, int iH )
{
	int iXMouse = *( int* )0x0A17518;
	int iYMouse = *( int* )0x0A1751C;

	if ( ( iXMouse >= iX ) && ( iXMouse <= ( iX + iW ) ) )
	{
		if ( ( iYMouse >= iY ) && ( iYMouse <= ( iY + iH ) ) )
			return TRUE;
	}
	return FALSE;
}

BOOL bNewLoader = FALSE;

void Game::LoaderThread( int i )
{
	while ( TRUE )
	{
		if ( bNewLoader )
		{
			CALLT_WITH_ARG1( 0x00438AB0, 0, 0 );
			bNewLoader = FALSE;
		}

		Sleep( 1 );
	}
}

void Game::CreateLoaderThread()
{
	CALLT_WITH_ARG1( 0x00438AB0, 0, 0 );
	return;

	static BOOL bLoaderThread = FALSE;

	if ( !bLoaderThread )
	{
		HANDLE h = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&Game::LoaderThread, NULL, 0, NULL );

		if ( h )
		{
			if ( SYSTEM->IsMultiCore() && SYSTEM->GetNumberOfCores() >= 4 )
			{
				DWORD_PTR dw = 0x1;

				SetThreadAffinityMask( h, dw );
			}
		}

		bLoaderThread = TRUE;
	}

	bNewLoader = TRUE;

	while ( bNewLoader )
	{
		Sleep( 0 );
	}
}

void Game::InitializeWindow( int iWidth, int iHeight )
{
	WNDCLASSEXA wc;

	// Create Pointer DXGraphicEngine
	DX::Graphic_ptr GraphicEngine_ptr = std::make_shared<DX::Graphic>();

	// Give the application a name.
	lpAppName = GAME_NAME;

	// Level Cap
	*(int*)(0x04B06F14) = SERVER_LEVEL_MAX;

	// Clear out the window class for use
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIconA( hInst, (LPCSTR)111 );
	wc.hCursor = LoadCursorA( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszClassName = lpAppName;
	wc.cbSize = sizeof( WNDCLASSEX );

	// Register the window class.
	RegisterClassExA( &wc );

	// Determine the resolution of the clients desktop screen.
	iWidth = GetSystemMetrics( SM_CXSCREEN );
	iHeight = GetSystemMetrics( SM_CYSCREEN );

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if ( *( BOOL* )0x9CB028 == FALSE )
	{
		// Create the window and use the result as the handle
		hWindow = CreateWindowExA( WS_POPUP, lpAppName, lpAppName, WS_POPUP, CW_USEDEFAULT, 0, iWidth, iHeight, NULL, NULL, hInst, NULL );
	}
	else
	{
		RECT srcRect = { 0, 0, (LONG)*( UINT* )0x824F70, (LONG)*( UINT* )0x824F74 };
		UINT uWindowStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		// Adjust Window
		AdjustWindowRect( &srcRect, uWindowStyle, FALSE );

		srcRect.right -= srcRect.left;
		srcRect.bottom -= srcRect.top;

		// Create the window and use the result as the handle 0x80CF0000
		hWindow = CreateWindowExA( NULL, lpAppName, lpAppName, uWindowStyle, ( GetSystemMetrics( SM_CXSCREEN ) / 2 ) - ( srcRect.right / 2 ),
								   ( GetSystemMetrics( SM_CYSCREEN ) / 2 ) - ( srcRect.bottom / 2 ), srcRect.right, srcRect.bottom, NULL, NULL, hInst, NULL );
	}

	// HWND of Game
	*( UINT* )0x0A17540 = ( UINT )hWindow;

	// Create Unknown Thread
	CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE )0x416750, NULL, NULL, NULL );

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( hWindow, SW_SHOW );
	UpdateWindow( hWindow );

	// Hide the mouse cursor.
	//ShowCursor( FALSE );

	// Init D3D 
	GRAPHICENGINE->InitD3D( hWindow );
}

void Game::ShutdownWindow()
{
	// Show the mouse cursor.
	//ShowCursor( TRUE );

	// Fix the display settings if leaving full screen mode.
	if ( *( BOOL* )0x9CB028 == FALSE )
		ChangeDisplaySettings( NULL, 0 );

	// Destroy Window
	DestroyWindow( hWindow );
	hWindow = NULL; 

	// Remove the application instance.
	UnregisterClassA( lpAppName, hInst );
	hInst = NULL;
}

void Game::DeleteClanTag(std::string serverName)
{
	WIN32_FIND_DATA sFindData;
	char szFileName[256] = { 0 };
	HANDLE hFile = NULL;
	STRINGCOPY( szFileName, FormatString("save\\clanDATA\\%s\\*.bmp", serverName.c_str()));

	char szPath[512] = { 0 };

	if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
	{
		do
		{
			STRINGFORMAT( szPath, "save\\clanDATA\\%s\\%s", serverName.c_str (), sFindData.cFileName );
			DeleteFileA( szPath );
		}
		while ( FindNextFileA( hFile, &sFindData ) );

		FindClose( hFile );
	}

	STRINGCOPY( szFileName, FormatString("save\\clanDATA\\%s\\*.rep", serverName.c_str ()) );

	if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
	{
		do
		{
			STRINGFORMAT( szPath, "save\\clanDATA\\%s\\%s", serverName.c_str (), sFindData.cFileName );
			DeleteFileA( szPath );
		}
		while ( FindNextFileA( hFile, &sFindData ) );

		FindClose( hFile );
	}
}

void Game::SetHardwareID( const char * pszHardwareID )
{
	STRINGCOPY( Game::szHardwareID, pszHardwareID );
}

//Ugly, Remove...
LRESULT CALLBACK WndProc( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	std::shared_ptr<Controller> pController = std::shared_ptr<Controller>();
	std::shared_ptr<Game> pGame = std::shared_ptr<Game>();

	switch ( uMsg )
	{
		case WM_MOUSEMOVE:
	//		if ( GetForegroundWindow() != hWindow )
	//			return 0;

			*( WORD* )0x04B0B228 = ( WORD )round( LOWORD( lParam ) );
			*( WORD* )0x04B0B22A = ( WORD )round( HIWORD( lParam ) );
			*( int* )0x0A17518 = ( int )round( LOWORD( lParam ) );
			*( int* )0x0A1751C = ( int )round( HIWORD( lParam ) );
			*( int* )0x0A16194 = *( int* )0x0A17518;
			*( int* )0x0A16198 = *( int* )0x0A1751C;
			if ( bMouseCreatedHandler )
				MOUSEHANDLER->OnMouseMoveHandler( *(int*)0x0A17518, *(int*)0x0A1751C );
			break;

		case WM_MOUSEWHEEL:
			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseScrollHandler( GET_WHEEL_DELTA_WPARAM( wParam ) ) )
				return 0;
			break;
			/*
		case WM_TIMER:
			if ( wParam == 0x6A6A6A6B )
			{
				if ( *(UINT*)0x3A97620 && *(UINT*)0x3A97624 && *(UINT*)0x3A97628 )
					CALL( 0x0046E3A0 );
				*(UINT*)0x0A17684 += 1;
				return 0;
			}
			break;
			*/

		case WM_KILLFOCUS:

			break;

		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			CHARACTERGAME->CancelAttack();
			break;

		case WM_LBUTTONDOWN:

			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;
		case WM_LBUTTONUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_RBUTTONDOWN:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_RBUTTONUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;

			if ( bMouseCreatedHandler && MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;
			break;

		case WM_CHAR:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyChar( wParam ) )
				return 0;
			break;

		case WM_KEYDOWN:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyPress( wParam, TRUE ) )
				return 0;
			break;

		case WM_KEYUP:
			if ( GetForegroundWindow() != hWindow )
				return 0;
			if ( (*(UINT*)0x04B0D364) && bMouseCreatedHandler && KEYBOARDHANDLER->OnKeyPress( wParam, FALSE ) )
				return 0;
			break;

		default:
			break;
	}
	WndProcPT( hWindow, uMsg, wParam, lParam );

	// Keys?
	if ( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST )
		return 0;

	// Close?
	if ( uMsg == WM_CLOSE || uMsg == WM_QUIT )
		return 0;

	return DefWindowProcA( hWindow, uMsg, wParam, lParam );
}