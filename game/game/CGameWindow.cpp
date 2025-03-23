#include "stdafx.h"
#include "CGameWindow.h"

CGameWindow * CGameWindow::pcInstance;

CGameWindow::CGameWindow() : CWindow()
{
	CGameWindow::pcInstance = this;

	pcGame = new CGame();

	pcTimer = NULL;
	uTimerID = 0;
}


CGameWindow::~CGameWindow()
{
	SAFE_DELETE( pcGame );
}

UINT CGameWindow::Init()
{
	SETTINGSHANDLER->GetModel()->ReadSettings();

	//Apply Camera Range
	if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 0 )
	{
		WRITEDWORD( 0x03245894, 0 );
	}
	else if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 1 )
	{
		WRITEDWORD( 0x03245894, 22 );
	}
	else if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 2 )
	{
		WRITEDWORD( 0x03245894, 64 );
	}

	CSettingsModel * p = SETTINGSHANDLER->GetModel();

	DX::Graphic::ResolutionChange( TRUE, p->GetSettings().dwWidth, p->GetSettings().dwHeight );

	if ( !Register( GAME_NAME, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 111, (HBRUSH)(GetStockObject( BLACK_BRUSH )) ) )
	{
		ERRORBOX( "Register() failed!\n\nGetLastError returned %d", GetLastError() );
		return 0;
	}

	if ( !MakeWindow( GAME_NAME, FALSE, p->GetSettings().bWindowed, FALSE, p->GetSettings().dwWidth, p->GetSettings().dwHeight ) )
	{
		ERRORBOX( "MakeWindow() failed!\n\nGetLastError returned %d", GetLastError() );
		return 0;
	}

	// Create Edit Window
	HWND hWndEdt = CreateWindowEx( NULL, "EDIT", "", 0x40801004, 10, 400, 500, 40, hWnd, (HMENU)101, GetApplication()->GetProcess()->GetInstanceHandle(), NULL );

	SetWindowTextA( hWndEdt, (char*)0x039033E8 );

	// Copy hWndEdit for Executable
	*(UINT*)0x0A17544 = (UINT)hWndEdt;
	*(UINT*)0x0A15DF4 = SetWindowLong( hWndEdt, GWL_WNDPROC, 0x416810 );
	*(UINT*)0x0A176DC = (UINT)hWndEdt;
	*(UINT*)0x0A17418 = (UINT)ImmGetContext( hWnd );
	*(UINT*)0x0A15DE0 = (UINT)ImmGetContext( hWndEdt );

	SetTimer( hWnd, 0, 100, 0 );
	SetTimer( hWnd, 0x6A6A6A6A, 850, 0 );
	SetTimer( hWnd, 0x6A6A6A6B, 10, 0 );

	// Initial Context
	CALL_WITH_ARG1( 0x00416780, (DWORD)0 );

	//Audio
	DX::Sound_ptr pSound = std::make_shared<DX::Sound>();
	pSound->CreateDevices();

	WRITEDWORD( 0x0A17540, hWnd );

	GRAPHICENGINE->Init ( );
	GRAPHICENGINE->InitD3D( hWnd );

	// Init Socket
	SOCKETGAME->Load(hWnd);

	// Initialize Font System
	FONTHANDLER->Init();
	FONTHANDLER->CreateFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	FONTHANDLER->CreateFont( "Arial", 15, 6, FALSE, FALSE, -1 );


	if ( !pcGame->Init() )
		return 0;

	CLogger::GetLogger()->Log( "" ); //just to delete old Log.txt (old logs did contain sensitive data)

	ANTICHEATHANDLER->AddNewThreadException( GetCurrentThreadId() );

	pcGame->Begin();

#ifdef _DEBUG
	if( FILEEXIST("testscreen.ini") )
		GAMESCREEN->ChangeScreen( SCREEN_Test );
	else
		GAMESCREEN->ChangeScreen( SCREEN_Login );
#else
	GAMESCREEN->ChangeScreen( SCREEN_Login );
#endif

	bExit		= FALSE;
	bRunning	= FALSE;
	bRender		= TRUE;

	pcTimer		= GAMETIMER;
	uTimerID	= Dice::RandomI( 500, 2500 );

	iWindowInactiveFrameSleepTime = GetApplication()->GetSystem()->ComputeFrameDelayTime();

	pcTimer->SetMultiCore( GetApplication()->GetSystem()->IsMultiCore() );
	pcTimer->Update();

	if ( GetApplication()->GetSystem()->IsMultiCore() )
	{
		DWORD_PTR dw = (1 << GetApplication()->GetSystem()->GetNumberOfCores()) - 1;

//		SetProcessAffinityMask( GetCurrentProcess(), dw );
	}

	return 1;
}

BOOL CGameWindow::Shutdown()
{
	return 0;
}

BOOL CGameWindow::Run()
{
	HWND h = FindWindowA( "TFrmFortressPT", NULL );

	if ( h != 0 )
		PostMessageA( h, WM_WINDOWREADY, 0, 0 );

	if( SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized )
		ShowWindow( hWnd, SW_MAXIMIZE );
	else
		ShowWindow( hWnd, SW_SHOWNORMAL );

	DEBUG( "Run" );

	//DEP
	SetProcessDEPPolicy( 0 );
	//discord
	//DiscordAPI::Functions::Initialize(); // discord api start

	bExit = FALSE;
	bRunning = TRUE;

	while ( PumpWindowMessages() )
	{
		//Game Loop
		Loop();

		//Render Lock if Window Inactive
		if ( (!IsActive()) )
			Sleep( iWindowInactiveFrameSleepTime );
	}

	bRunning = FALSE;
	bExit = TRUE;

	return TRUE;
}
extern int64_t s_ServTimeOffset;

LRESULT CGameWindow::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	BOOL bUseOld = FALSE;


	if ( uMsg == WM_NCRBUTTONDOWN || uMsg == WM_NCLBUTTONDOWN )
		CHARACTERGAME->CancelAttack();

	switch ( uMsg )
	{
		/*case WM_DESTROY:
		case WM_QUIT:*/
		case WM_CLOSE:
			if (Exit() == FALSE)
			{
				CALL(0x0045C35A);
				Exit(TRUE);
			}
			break;

		case WM_SETCURSOR:
			if ( (HWND)wParam == hWnd )
			{
				MOUSEHANDLER->SetCursorGame( MOUSE_ICON );
				return TRUE;
			}
			break;

		case WM_SIZE:
			if ( wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED )
				MINIMIZED_GAME = FALSE;
			else
			{
				if ( wParam == SIZE_MINIMIZED )
				{
					MINIMIZED_GAME = TRUE;
				}
			}
			break;


		case WM_KILLFOCUS:
			SetFocus( hWnd );
			return TRUE;

		case WM_MOUSEMOVE:
			int iX;
			iX = (int)round( LOWORD( lParam ) );
			int iY;
			iY = (int)round( HIWORD( lParam ) );

			if ( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed && SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust == FALSE )
			{
				RECT rc;
				rc.left = 0;
				rc.top = 0;
				rc.right = SETTINGSHANDLER->GetModel()->GetOldSettings().dwWidth;
				rc.bottom = SETTINGSHANDLER->GetModel()->GetOldSettings().dwHeight;
				GetWindowRect( DX::Graphic::GetWindowHandler(), &rc );

				UINT uWindowStyle = GetWindowLongA( DX::Graphic::GetWindowHandler(), GWL_STYLE );

				// Adjust Window
				AdjustWindowRect( &rc, uWindowStyle, FALSE );
				DWORD dwHeight = (rc.bottom + rc.top);

				WINDOWPLACEMENT wc;

				GetWindowPlacement( DX::Graphic::GetWindowHandler(), &wc );

				if ( wc.showCmd == SW_MAXIMIZE )
				{
					iX = (iX * SETTINGSHANDLER->GetModel()->GetOldSettings().dwWidth) / GetSystemMetrics( SM_CXSCREEN );
					iY = (iY * SETTINGSHANDLER->GetModel()->GetOldSettings().dwHeight) / dwHeight;
				}
			}

			*(WORD*)0x04B0B228 = (WORD)iX;
			*(WORD*)0x04B0B22A = (WORD)iY;
			*(int*)0x0A17518 = (int)iX;
			*(int*)0x0A1751C = (int)iY;
			*(int*)0x0A16194 = *(int*)0x0A17518;
			*(int*)0x0A16198 = *(int*)0x0A1751C;

			MOUSEHANDLER->OnMouseMoveHandler( iX, iY );

			struct BIG_WORD
			{
				union
				{
					DWORD	dwAddr;
					WORD	wAddr[2];
				};
			};

			BIG_WORD * bwAddr;
			bwAddr = (BIG_WORD*)&lParam;

			bwAddr->wAddr[0] = iX;
			bwAddr->wAddr[1] = iY;


			bUseOld = TRUE;
			break;

		case WM_MOUSEWHEEL:
			if ( MOUSEHANDLER->OnMouseScrollHandler( GET_WHEEL_DELTA_WPARAM( wParam ) ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_LBUTTONDBLCLK:
			bUseOld = TRUE;
			break;

		case WM_LBUTTONDOWN:
			if ( GetForegroundWindow() != hWnd )
				return 0;

			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_LBUTTONUP:
			if ( GetForegroundWindow() != hWnd )
				return 0;
			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_RBUTTONDOWN:
			if ( GetForegroundWindow() != hWnd )
				return 0;
			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_RBUTTONUP:
			if ( GetForegroundWindow() != hWnd )
				return 0;
			if ( MOUSEHANDLER->OnMouseClickHandler( uMsg ) )
				return 0;

			bUseOld = TRUE;
			break;

		case WM_CHAR:
			if ( GetForegroundWindow() == hWnd )
			{
				if ( KEYBOARDHANDLER->OnKeyChar( wParam ) )
					return 0;
			}
			bUseOld = TRUE;
			break;

		case WM_KEYDOWN:
			if ( GetForegroundWindow() == hWnd )
			{
				if ( KEYBOARDHANDLER->OnKeyPress( wParam, TRUE ) )
					return 0;
			}
			bUseOld = TRUE;
			break;

		case WM_KEYUP:
			if ( GetForegroundWindow() == hWnd )
			{
				if ( KEYBOARDHANDLER->OnKeyPress( wParam, FALSE ) )
					return 0;
			}

			bUseOld = TRUE;
			break;

		case WM_TIMER:
			bUseOld = TRUE;
			break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			if ( wParam == VK_F10 )
			{
				KEYBOARDHANDLER->OnKeyPress( wParam, (uMsg == WM_SYSKEYDOWN) );
				return 0;
			}
			break;

		case WM_SOCKETPACKET:
			SOCKETGAME->SocketPacket( (SocketData*)wParam, (PacketReceiving*)lParam );
			return 0;
			break;


		case WM_NCACTIVATE:
			if ( Game::GetGameMode() == GAMEMODE_InGame )
			{
				UnitData * p = UNITDATA;

				if ( (p) && (p->iID != 0) )
				{
					CALL( 0x0045BC90 );
				}
			}
			return DefWindowProcA( hWnd, uMsg, wParam, lParam );

		default:
			return DefWindowProcA( hWnd, uMsg, wParam, lParam );
	}

	if (bUseOld)
	{
		DEBUG("WNDPROC %u %u %u", uMsg, lParam, wParam);
		DWORD dwRet = 0;

		__asm
		{
			PUSH lParam;
			PUSH wParam;
			PUSH uMsg;
			PUSH hWnd;
			MOV EAX, 0x0041C330;
			CALL EAX;
			MOV dwRet, EAX;
		}

		return dwRet;
	}

    if (uMsg == WM_TIMER)
    {
        int64_t l_Time = static_cast<int64_t>(GetTickCount()) + s_ServTimeOffset;

        if (l_Time < 0)
            l_Time = 0;

        l_Time /= 800;

        DWORD dwGameWorldTime = static_cast<DWORD>(l_Time);

        DWORD dwGameHour    = dwGameWorldTime + *reinterpret_cast<DWORD*>(0xCF479C);   ///< time offset
        dwGameHour    = dwGameHour / 60;
        dwGameHour    = dwGameHour - (((int)(dwGameHour / 24)) * 24);
        DWORD dwGameMin     = dwGameWorldTime - (((int)(dwGameWorldTime / 60)) * 60);

        *reinterpret_cast<DWORD *>(0x826D10) = dwGameHour;
        *reinterpret_cast<DWORD *>(0xCF4798) = dwGameMin;
    }

	return FALSE;
}

void CGameWindow::Loop()
{
	Update( pcTimer->Update() );

	Render();

	if ( Quit() )
		bExit = TRUE;

	CALL( 0x004165B0 );

	if (bExit)
	{
		UNITGAME->FlushLog();
		//DiscordAPI::Functions::Shutdown(); // discord api shutdown
	}
}

NAKED bool CGameWindow::ResetCheck()
{
	JMP( 0x0063B4E0 );
}

void CGameWindow::Render()
{
	if ( bRender == FALSE )
		return;

	if ( GRAPHICENGINE->CheckDevice() == FALSE )
		return;

	GRAPHICENGINE->BeginScene( CAMERA->GetViewMatrix() );

	pcGame->Render3D();

	pcGame->Render2D();


	GRAPHICENGINE->EndScene();
}

void CGameWindow::Update( double fTime )
{
	static double fTick = (1000.0f / ((double)60));
	static double fOffs = 0.0f;

	//Split time into frames
	fOffs += fTime;
	if ( fOffs >= fTick )
	{
		do
		{
			pcGame->Frame();

			fOffs -= fTick;
		}
		while ( fOffs >= fTick );
	}

	//Update
	pcGame->Update( (float)fTime );

	if ( fOffs < fTick && GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
		Sleep( (DWORD)(fTick - fOffs) );
}

BOOL CGameWindow::Quit()
{
	if ( GameCore::QuitGame() == TRUE && TRADEHANDLER->IsTradeMaskTime() == FALSE )
	{
		if ( Game::GetGameMode() == GAMEMODE_InGame && SOCKETG )
		{
			if ( GameCore::QuitAndSave() )
			{
				if ( GameCore::IsSaved() )
					return TRUE;

				if ( ((*(DWORD*)0x00CF4750) - (*(DWORD*)0x00D0FC34)) > 15000 )
					return TRUE;

				return FALSE;
			}

			if ( TRADEHANDLER->IsWindowTradeOpen() )
			{
				//Send Cancel Trade
				CALL_WITH_ARG2( 0x000628290, TRADE_UNITID, 3 );
				//Close
				CALLT( 0x0050C310, 0x036EFD98 );
			}

			if ( WINDOW_ISOPEN_WAREHOUSE )
				WAREHOUSEHANDLER->RestoreItems();

			ITEMHANDLER->ResetItemMouse();

			SAVE;

			GameCore::QuitAndSave( TRUE );
		}
		else
			return TRUE;
	}
	return FALSE;
}
