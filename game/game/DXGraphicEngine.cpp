#include "stdafx.h"
#include "DXGraphicEngine.h"
#include "Controller.h"
#include "TestUI.h"
#include "LightEffect.h"

static char * szaLoadingImagesLoading[]
{
	//Original
	//"summer",
	//"tempskrons",
	//"euravillage",
	//"women",
	//"priestess",

	//New Titles
	"ch-ft",
	"chars",
	"mrin",
	"Archer_Alone"
	"Priestess_Floating",
	"RIP_EPT",
	"Map",

	//Classic Titles
	"map-all-all",
	"map-all-mrin",
	"map-all-tscr",
	"map-ch-ac",
	"map-ch-at",
	"map-ch-ft",
	"map-ch-knight",
	"map-ch-meca",
	"map-ch-mg",
	"map-ch-pk",
	"map-ch-pt",
	"Map-game",
	"map-logo-RNS",
	"map-SOD",
	"map-tw-nvsc",
	"map-tw-pillai",
	"map-tw-rica",
	"map-user_001",

};

static char* szaLoadingImagesGameTipsLoading[]
{
	// Loading Image Tips
	"Remember to always bring along a boss and solo quest from your hometown",
	"New players should complete the beginner questline from Annie or Millie",
	"Check out the new controls guide by opening the in-game help file",
	"Don't forget to bring Rebirth Scroll(s) on your adventures",
	"Press and hold 'P' on a highlighted piece of equipment to view its perfect stats",
	"Press F10 to quickly bring up the settings window",
	"The settings window shows when bosses spawn and the current server time",
	"You need to learn every skill available before you can rank up",
	"Hold Shift and press 1-3 while hovering over potions to quickly place them in your action bar",
	"Every 30 minutes a random player wins the 'OnlineReward'",
	"Press and hold A to prioritize items over monsters",
	"Press M to enlarge your map other players can be seen on the map",
	"Press CTRL + Home or End to make screenshots without or with HUD",
	"Hold CTRL + right click potions in your warehouse to take out a specified number",
	"Hold CTRL + left click potions in your inventory to move a specified number",
	"Press Backspace to hide/show player names and clan tags",
	"Press CTRL + F1 through F10 to use customizable chart shortcuts",
	"For chat emotes type e1 through e20 in chat",
	"Type yahoo in chat then press space to do a special victory dance",
	"Double left click to automatically run with your character",
	"Press TAB in chat to quickly bring up the command for clan chat",
	"Hold F1 through F8 then left or right click skills to create binds",
	"Press F to command a skill summoned pet to attack your target",
	"Press T to switch between costume and armor visibility",
	"Defeating Boss Monsters will earn you greater rewards!",
	"Make sure to check out your daily solo quest guide!",
	"CTRL + LMB while hovering over an item to send a link to the chat",
};

class UI::Image * pLoadingImageHandler			= NULL;
class UI::Image * pBackgroundImageHandler		= NULL;
class UI::Image * pBorderLoadingImageHandler	= NULL;
class UI::Image * pBoxLoadingImageHandler		= NULL;
class UI::Image * pInfoBoxHandler				= NULL;

BOOL bTakenScreenShot = FALSE;
BOOL bNoHud = FALSE;
DWORD dwTimeScreenShot = 0;

DXGraphicEngine::DXGraphicEngine()
{
	ZeroMemory( &d3dDM, sizeof( D3DDISPLAYMODE ) );
	ZeroMemory( &d3dCaps, sizeof( D3DCAPS9 ) );
	ZeroMemory( &d3dPP, sizeof( D3DPRESENT_PARAMETERS ) );

	lpD3D						= NULL;
	lpD3DDev					= NULL;
	lpPostProcess				= NULL;
	lpLightEffect				= NULL;
	pcVideoRenderer				= NULL;
	bQuitLoading				= FALSE;
	bShowLoading				= FALSE;
	iCurrentLoading				= -1;
	iMaxLoading					= 0;
}

DXGraphicEngine::~DXGraphicEngine()
{
	SAFE_DELETE( lpPostProcess );
	SAFE_DELETE( pcVideoRenderer );
}

void DXGraphicEngine::Log( char * pszFormat, ... )
{
	va_list vl;

	DX::Logger Log;

	char szError[16384];

	va_start( vl, pszFormat );
	_vsnprintf_s( szError, _countof( szError ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	char szMessage[4096];
	STRINGFORMAT( szMessage, "[%08X][%s] %s\r\n", hr, Log.ResultToString( hr ), szError );

	FILE * file;
	CreateDirectoryA( "save\\logs", NULL );
	if( fopen_s( &file, "save\\logs\\init.log", "a" ) == 0 )
	{
		fputs( szMessage, file );
		fflush( file );
		fclose( file );
	}
}

void DXGraphicEngine::Error( char * pszFormat, ... )
{
	va_list vl;

	DX::Logger DXLog;

	char szError[16384];

	va_start( vl, pszFormat );
	_vsnprintf_s( szError, _countof( szError ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	char szMessage[4096];
	STRINGFORMAT( szMessage, "[%08X][%s] %s", hr, DXLog.ResultToString( hr ), szError );

	MessageBoxA( 0, szMessage, "Graphic Engine Error!", MB_OK | MB_ICONEXCLAMATION );

	Log( szError );
}

void DXGraphicEngine::Init ( )
{
	int iRandTip = Dice::RandomI ( 0, _countof ( szaLoadingImagesGameTipsLoading ) - 1 );

	cInfoTitle[0] = { 0 };
	cInfoText[0] = { 0 };

	STRINGCOPY ( cInfoTitle, "Gameplay Tips & Tricks" );
	STRINGCOPY ( cInfoText, szaLoadingImagesGameTipsLoading[iRandTip] );
}

BOOL DXGraphicEngine::InitD3D( HWND hWindow )
{
	// Create the Direct3D interface
	CreateInterface();

	// Create a device class
	CreateDevice( (HWND)hWindow, (*(int*)DXSettings::WidthWindow), (*(int*)DXSettings::HeightWindow) );

	//Create Post Process
	if( !lpPostProcess )
	{
		lpPostProcess = new DX::PostProcess;
		if( !lpPostProcess->Init() )
			return FALSE;
	}

	// Rectangle UI
	if( !bCreatedRectangle )
	{
		D3DXCreateLine( DX::Graphic::GetDevice(), &pRectangleUI );
		bCreatedRectangle = TRUE;
	}

	pGraphicsDevice = std::make_shared<DX::GraphicsDevice>( lpD3DDev );
	pRenderer = std::make_shared<DX::Renderer>( pGraphicsDevice );

	//Video Renderer
	pcVideoRenderer = new DXVideoRenderer();

	pcVertexCache = new EXEVertexCache();

	DX::cSelectGlow1.Init( pRenderer );
	DX::cSelectGlow2.Init( pRenderer );

	// Set display mode
	CALL_WITH_ARG3( 0x0063B220, READDWORD(DXSettings::WidthWindow), READDWORD(DXSettings::HeightWindow), READDWORD(DXSettings::BitDepth) );

	CALL( 0x0041BD10 );

	return TRUE;
}

BOOL DXGraphicEngine::CreateInterface()
{
	
	DX::Logger DXLog;

	lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	*(LPDIRECT3D9*)0x3A9A314 = lpD3D;

	if ( FAILED( hr = lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3dDM ) ) )
	{
		// Error
		return FALSE;
	}

	// Window mode
	BOOL bWindowed = SETTINGSHANDLER->GetModel()->GetSettings().bWindowed;

	// Bit Depth is 32 or 16
	DWORD dwBitDepth = SETTINGSHANDLER->GetModel()->GetSettings().dwBitDepth == 16 ? 16 : 32;

	// Support 32 or 16 bits BitDepth?
	if( FAILED( hr = lpD3D->CheckDeviceType( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dDM.Format,
		dwBitDepth == 32 ? D3DFMT_X8R8G8B8 : D3DFMT_X4R4G4B4,
		bWindowed ) ) )
	{
		dwBitDepth = dwBitDepth == 32 ? 16 : 32;

		if( FAILED( hr = lpD3D->CheckDeviceType( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dDM.Format,
			dwBitDepth == 32 ? D3DFMT_X8R8G8B8 : D3DFMT_X4R4G4B4,
			bWindowed ) ) )
		{
			Error( "Your device must support a 16 or 32 bit Back Buffer" );
			DXLog.Print( "DEVICE BITDEPTH CANNOT SUPPORT 32 OR 16 BIT" );
			return FALSE;
		}
	}

	if( dwBitDepth == 32 )
	{
		Log( "32 bit Back Buffer" );
		d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else if( dwBitDepth == 16 )
	{
		Log( "16 bit Back Buffer" );
		d3dPP.BackBufferFormat = D3DFMT_X4R4G4B4;
	}
	else
	{
		Log( "Unknown Back Buffer Format" );
		return FALSE;
	}

	d3dPP.AutoDepthStencilFormat = D3DFMT_D24S8;

	if( FAILED( hr = lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dDM.Format,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		d3dPP.AutoDepthStencilFormat ) ) )
	{
		d3dPP.AutoDepthStencilFormat = D3DFMT_D15S1;

		if( FAILED( hr = lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dDM.Format,
			D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,
			d3dPP.AutoDepthStencilFormat ) ) )
		{
			Error( "Your device must support a 16(1s) or 32(8s) bit DepthStencil Buffer" );
			return FALSE;
		}
		else
		{
			Log( "D15S1 DepthStencil Buffer" );
		}
	}
	else
	{
		Log( "D24S8 DepthStencil Buffer" );
	}

	// MultiSample
	d3dPP.MultiSampleType		= D3DMULTISAMPLE_NONE;
	d3dPP.MultiSampleQuality	= 0;

	if( SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing )
	{
		D3DMULTISAMPLE_TYPE d3daMultipleSampleType[] = {
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_15_SAMPLES,
			D3DMULTISAMPLE_14_SAMPLES,
			D3DMULTISAMPLE_13_SAMPLES,
			D3DMULTISAMPLE_12_SAMPLES,
			D3DMULTISAMPLE_11_SAMPLES,
			D3DMULTISAMPLE_10_SAMPLES,
			D3DMULTISAMPLE_9_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_7_SAMPLES,
			D3DMULTISAMPLE_6_SAMPLES,
			D3DMULTISAMPLE_5_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_3_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
		};

		for( int i = 0; i < _countof( d3daMultipleSampleType ); i++ )
		{
			D3DMULTISAMPLE_TYPE t = d3daMultipleSampleType[i];
			DWORD q;

			if( SUCCEEDED( hr = lpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dPP.BackBufferFormat,
				bWindowed,
				t,
				&q ) ) &&
				SUCCEEDED( hr = lpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dPP.AutoDepthStencilFormat,
				bWindowed,
				t,
				&q ) ) )
			{
				d3dPP.MultiSampleType = t;
				d3dPP.MultiSampleQuality = q - 1;
				break;
			}
		}
	}

	//Get Device Caps
	if( FAILED( hr = lpD3D->GetDeviceCaps( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&d3dCaps ) ) )
	{
		Error( "Could not get Device Capabilities" );
		return FALSE;
	}
	return TRUE;
}

BOOL DXGraphicEngine::CreateDevice( HWND hWnd, int iWidth, int iHeight )
{
	d3dPP.BackBufferWidth = SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
	d3dPP.BackBufferHeight = SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;
	d3dPP.Windowed = SETTINGSHANDLER->GetModel()->GetSettings().bWindowed;

	if ( !SETTINGSHANDLER->GetModel()->GetSettings().bHighTextureQuality )
	{
		*(UINT*)(0x03A9A334) = 3;
	}

	bWindowed				= d3dPP.Windowed;

	sResolution.uWidth		= d3dPP.BackBufferWidth;
	sResolution.uHeight		= d3dPP.BackBufferHeight;

	DWORD dwBehaviourFlags = D3DCREATE_FPU_PRESERVE;

	d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if( d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE )
	{
		Log( "Pure Device" );
		dwBehaviourFlags |= D3DCREATE_PUREDEVICE;
	}

	if( (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) && (d3dCaps.VertexProcessingCaps != 0) )
	{
		Log( "Hardware Vertex Processing" );
		dwBehaviourFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		Log( "Software Vertex Processing" );
		dwBehaviourFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if ( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE )
	{
		Log( "Presentation Interval Immediate" );
		d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bVSync )
	{
		if ( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE )
		{
			Log( "Presentation Interval One" );
			d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
	}

	d3dPP.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	d3dPP.hDeviceWindow					= hWnd;
	d3dPP.BackBufferCount				= 1;
	d3dPP.EnableAutoDepthStencil		= TRUE;
	d3dPP.Flags						= 0;

	if( d3dPP.Windowed )
	{
		d3dPP.FullScreen_RefreshRateInHz	= 0;

		if( FAILED( hr = lpD3D->CreateDevice( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			dwBehaviourFlags,
			&d3dPP,
			&lpD3DDev ) ) )
		{
			Error( "Could not create Direct3D Device" );
			return FALSE;
		}
		*(LPDIRECT3DDEVICE9*)0x3A9A318 = lpD3DDev;
		memcpy( (D3DPRESENT_PARAMETERS*)0x3A9A2D0, &d3dPP, sizeof( D3DPRESENT_PARAMETERS ) );
		return TRUE;
	}
	else
	{
		d3dPP.Windowed						= FALSE;
		d3dPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;

		if( FAILED( hr = lpD3D->CreateDevice( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			dwBehaviourFlags,
			&d3dPP,
			&lpD3DDev ) ) )
		{
			Error( "Could not create Direct3D Device" );
			return FALSE;
		}
		*(LPDIRECT3DDEVICE9*)0x3A9A318 = lpD3DDev;
		memcpy( (D3DPRESENT_PARAMETERS*)0x3A9A2D0, &d3dPP, sizeof( D3DPRESENT_PARAMETERS ) );
		return TRUE;
	}

}

BOOL DXGraphicEngine::CreateVertexBuffer()
{
	DWORD dwVertexTypeDesc = 324;
	UINT uVertexSize = 28;

	return TRUE;
}

void DXGraphicEngine::BeginScene( D3DXMATRIX mView )
{

	// Clear old render and start render
	if ( FAILED( GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 ) ) )
	{
		return;
	}

	GetDevice()->SetTransform( D3DTS_VIEW, &mView );

	GetDevice()->BeginScene();

//	GRAPHICENGINE->StateSetup();

	Controller::OnBeginScene();



	GameCore::GetInstance()->Render3D();
}

void DXGraphicEngine::EndScene()
{

	Controller::OnEndScene();

//	CALL( 0x005059D0 );

	if ( GetForegroundWindow() == (HWND)(*(UINT*)0x0A17540) )
		SETTINGSHANDLER->GetModel()->OnCheckWindowState();

	// Stop render
	GetDevice()->EndScene();
	GetDevice()->Present( NULL, NULL, NULL, NULL );
}

void DXGraphicEngine::StateSetup()
{

	//Render States
	lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpD3DDev->SetRenderState( D3DRS_CLIPPING, TRUE );
	lpD3DDev->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_ARGB( 0, 0, 0, 0 ) );

	lpD3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

	lpD3DDev->SetRenderState( D3DRS_LASTPIXEL, FALSE );

	lpD3DDev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	lpD3DDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpD3DDev->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );

	lpD3DDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	lpD3DDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );

	lpD3DDev->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	lpD3DDev->SetRenderState( D3DRS_ALPHAREF, 60 );
	lpD3DDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpD3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );

	lpD3DDev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

	for ( int i = 0; i < 8; i++ )
	{
		lpD3DDev->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		lpD3DDev->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		lpD3DDev->SetSamplerState( i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

		lpD3DDev->SetSamplerState( i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		lpD3DDev->SetSamplerState( i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

		lpD3DDev->SetTexture( i, NULL );
	}
}


void DXGraphicEngine::DrawRectangle2D( Rectangle2D rRect, DWORD dwColor )
{
	D3DXVECTOR2 vLine[ 2 ];

	pRectangleUI->SetWidth( ( float )rRect.iWidth );
	pRectangleUI->SetAntialias( FALSE );
	pRectangleUI->SetGLLines( true );

	vLine[ 0 ].x = ( float )rRect.iX + ( float )rRect.iWidth / 2;
	vLine[ 0 ].y = ( float )rRect.iY;
	vLine[ 1 ].x = ( float )rRect.iX + ( float )rRect.iWidth / 2;
	vLine[ 1 ].y = ( float )rRect.iY + ( float )rRect.iHeight;

	pRectangleUI->Begin();
	pRectangleUI->Draw( vLine, 2, dwColor );
	pRectangleUI->End();
}

void DXGraphicEngine::DrawRectangle2D( Rectangle2D sBox, DWORD dwColor1, DWORD dwColor2 )
{
	float u, v, u2, v2;

	//Compute texture coordinates, it's easy
	u = 0.0f;
	v = 0.0f;
	u2 = 1.0f;
	v2 = 1.0f;

	EXEDrawVertex rVertex[4];
	rVertex[0] = { float( sBox.iX ), float( sBox.iY ), 0, 1, dwColor1, 0, u, v };
	rVertex[1] = { float( sBox.iX + sBox.iWidth ), float( sBox.iY ), 0, 1, dwColor1, 0, u2, v };
	rVertex[2] = { float( sBox.iX ), float( sBox.iY + sBox.iHeight ), 0, 1, dwColor2, 0, u, v2 };
	rVertex[3] = { float( sBox.iX + sBox.iWidth ), float( sBox.iY + sBox.iHeight ), 0, 1, dwColor2, 0, u2, v2 };

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );


	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );



	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	//Texture Stage States
	GRAPHICENGINE->GetDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	GRAPHICENGINE->GetDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	GRAPHICENGINE->GetDevice()->SetTexture( 0, 0 );

	GRAPHICENGINE->GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR );
	GRAPHICENGINE->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (LPVOID)rVertex, sizeof(EXEDrawVertex) );
}

BOOL DXGraphicEngine::DrawPrimitive( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices )
{
	if ( iPrimitiveCount == 0 )
		return TRUE;

	UINT uVertexSize = GetVertexSize( dwVertexTypeDesc );

	iLastPrimitiveCount = iPrimitiveCount;
	iLastPrimitiveType = d3dPrimitiveType;
	dwLastVertexType = dwVertexTypeDesc;
	pLastRenderPtr = pVertices;


	//Set FVF
	if ( FAILED( lpD3DDev->SetFVF( dwVertexTypeDesc ) ) )
	{
		DEBUG( "ERROR - DXGraphicEngine::DrawPrimitive (SetFVF)" );
		return FALSE;
	}

	if ( FAILED( lpD3DDev->DrawPrimitiveUP( d3dPrimitiveType, iPrimitiveCount, pVertices, uVertexSize ) ) )
	{
		DEBUG( "ERROR - DXGraphicEngine::DrawPrimitive (DrawPrimitiveUP)" );
		return FALSE;
	}

	return TRUE;
}

BOOL DXGraphicEngine::DrawIndexedPrimitive( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices )
{
	if ( iPrimitiveCount == 0 )
		return TRUE;

	UINT uVertexSize = 0;

	LPDIRECT3DINDEXBUFFER9 lpIB = NULL;
	LPDIRECT3DVERTEXBUFFER9 lpVB = NULL;

	//Get Vertex Size
	if ( dwVertexTypeDesc == 324 )
	{
		uVertexSize = 28;
		lpIB = GetIndexBuffer( 0 );
		lpVB = GetVertexBuffer( 0 );
	}
	else if ( dwVertexTypeDesc == 452 )
	{
		uVertexSize = sizeof( EXEDrawVertex );
		lpIB = GetIndexBuffer( 1 );
		lpVB = GetVertexBuffer( 1 );
	}
	else if ( dwVertexTypeDesc == 708 )
	{
		uVertexSize = sizeof( EXEDrawVertex ) + 8;
		lpIB = GetIndexBuffer( 2 );
		lpVB = GetVertexBuffer( 2 );
	}
	else if ( dwVertexTypeDesc == 964 )
	{
		uVertexSize = sizeof( EXEDrawVertex ) + 16;
		lpIB = GetIndexBuffer( 3 );
		lpVB = GetVertexBuffer( 3 );
	}
	else if ( dwVertexTypeDesc == 1220 )
	{
		uVertexSize = sizeof( EXEDrawVertex ) + 24;
		lpIB = GetIndexBuffer( 4 );
		lpVB = GetVertexBuffer( 4 );
	}
	void * pVoid;

	CONST DWORD dwLockFlags = D3DLOCK_DISCARD;

	lpVB->Lock( 0, 0, &pVoid, dwLockFlags );
	CopyMemory( pVoid, pVertices, uVertexSize * (iPrimitiveCount * 3) );
	lpVB->Unlock();

	lpIB->Lock( 0, 0, &pVoid, dwLockFlags );
	CopyMemory( pVoid, EXERender::saIndices, sizeof( WORD ) * EXERender::iIndexIndices );
	lpIB->Unlock();

	lpD3DDev->SetIndices( lpIB );

	//Set FVF
	if ( FAILED( lpD3DDev->SetFVF( dwVertexTypeDesc ) ) )
	{
		DEBUG( "ERROR - DXGraphicEngine::DrawPrimitive (SetFVF)" );
		return FALSE;
	}

	lpD3DDev->SetStreamSource( 0, lpVB, 0, uVertexSize );

	lpD3DDev->DrawIndexedPrimitive( d3dPrimitiveType, 0, 0, (iPrimitiveCount * 3), 0, iPrimitiveCount );

	return TRUE;
}

BOOL DXGraphicEngine::DrawIndexedPrimitiveGeo( D3DPRIMITIVETYPE d3dPrimitiveType, DWORD dwVertexTypeDesc, int iPrimitiveCount, LPVOID pVertices )
{
	if ( dwVertexTypeDesc == 452 )
	{
		pcGeometryEngine->HandleVertices( pVertices, iPrimitiveCount, sizeof( EXEDrawVertex ) );
		pcGeometryEngine->HandleIndices( EXERender::saIndices, EXERender::iIndexIndices );
		pcGeometryEngine->Render( d3dPrimitiveType );
	}
	else
	{
		DrawPrimitive( d3dPrimitiveType, dwVertexTypeDesc, iPrimitiveCount, pVertices );
	}
	return TRUE;
}

BOOL DXGraphicEngine::DrawLast()
{
	return DrawPrimitive( (D3DPRIMITIVETYPE)iLastPrimitiveType, dwLastVertexType, iLastPrimitiveCount, pLastRenderPtr );
}

DWORD DXGraphicEngine::GetTextureAverageColor( LPDIRECT3DTEXTURE9 lpTexture, UINT uWidth, UINT uHeight, BOOL bHighColor )
{
	static LPDIRECT3DTEXTURE9 lpLastTexture = NULL;
	static DWORD dwLastColor				= 0;

	if ( uWidth == 0 || uHeight == 0)
		return 0;

	if ( lpTexture == NULL )
		return 0;

	if ( lpLastTexture == lpTexture )
		return dwLastColor;

	D3DLOCKED_RECT d3dRect;
	RECT rc;
	rc.left		= 0;
	rc.top		= 0;
	rc.right	= uWidth;
	rc.bottom	= uHeight;

	if ( SUCCEEDED( lpTexture->LockRect( 0, &d3dRect, &rc, D3DLOCK_READONLY ) ) )
	{
		BYTE * pDest = (BYTE*)d3dRect.pBits;

		UINT uColorsCount	= 0;
		UINT uRed			= 0;
		UINT uGreen			= 0;
		UINT uBlue			= 0;

		DX::Color cOldColor( 0 );

		DX::Color cColor( 0 );

		struct ColorDominant
		{
			D3DCOLOR	d3dColor;
			UINT		uRefCount;
		};

		std::vector<ColorDominant*> vColors;

		auto IsNearColor = []( D3DCOLOR color1, D3DCOLOR color2, int iAverage, IMinMax & sMinMax ) -> BOOL
		{
			DX::Color cColor1( color1 );
			DX::Color cColor2( color2 );

			if ( (abs( cColor1.GetRed() - cColor2.GetRed() ) <= iAverage) )
			{
				if ( abs( cColor1.GetGreen() - cColor2.GetGreen() ) <= iAverage )
				{
					if ( abs( cColor1.GetBlue() - cColor2.GetBlue() ) <= iAverage )
					{
						if ( (cColor1.GetRed() >= sMinMax.iMin) && (cColor1.GetRed() <= sMinMax.iMax) )
							if ( (cColor1.GetGreen() >= sMinMax.iMin) && (cColor1.GetGreen() <= sMinMax.iMax) )
								if ( (cColor1.GetBlue() >= sMinMax.iMin) && (cColor1.GetBlue() <= sMinMax.iMax) )
									return TRUE;
					}
				}
			}
			return FALSE;
		};

		for ( UINT y = 0; y < uHeight; y ++ )
		{
			for ( UINT x = 0; x < uWidth; x ++ )
			{
				int iIndex = (d3dRect.Pitch * y) + (4 * x);
				cColor.SetRed( (int)pDest[iIndex+2] );
				cColor.SetGreen( (int)pDest[iIndex+1] );
				cColor.SetBlue( (int)pDest[iIndex] );

				if ( cColor.Get() > 0 )
				{
					if ( bHighColor )
					{
						BOOL bFound = FALSE;
						for ( std::vector<ColorDominant*>::iterator it = vColors.begin(); it != vColors.end(); it++ )
						{
							ColorDominant * ps = (*it);

							if ( (cColor.Get() == ps->d3dColor) || IsNearColor( cColor.Get(), ps->d3dColor, 15, IMinMax( 20, 245 ) ) )
							{
								ps->uRefCount++;
								bFound = TRUE;
								break;
							}
						}

						if ( bFound == FALSE )
						{
							ColorDominant * ps	= new ColorDominant;
							ps->d3dColor		= cColor.Get();
							ps->uRefCount		= 1;
							vColors.push_back( ps );
						}
					}
					else
					{
						uRed	+= cColor.GetRed();
						uGreen	+= cColor.GetGreen();
						uBlue	+= cColor.GetBlue();
						uColorsCount++;
					}
				}
			}
		}

		lpTexture->UnlockRect( 0 );

		if ( bHighColor )
		{
			UINT uLastRefCount = 0;
			for ( std::vector<ColorDominant*>::iterator it = vColors.begin(); it != vColors.end(); it++ )
			{
				ColorDominant * ps = (*it);
				if ( ps->uRefCount > uLastRefCount )
				{
					cColor.Set( ps->d3dColor );
					uRed = cColor.GetRed();
					uGreen = cColor.GetGreen();
					uBlue = cColor.GetBlue();
					uLastRefCount = ps->uRefCount;
				}

				SAFE_DELETE( ps );
			}

			if ( uLastRefCount > 0 )
				uColorsCount = 1;

			vColors.clear();
		}


		cColor.Set( 0 );
		if ( uColorsCount > 0 )
		{
			cColor.SetRed( uRed / uColorsCount );
			cColor.SetGreen( uGreen / uColorsCount );
			cColor.SetBlue( uBlue / uColorsCount );
			cColor.SetAlpha( 255 );
		}

		lpLastTexture	= lpTexture;
		dwLastColor		= cColor.Get();

		return cColor.Get();
	}

	return 0;
}


void DXGraphicEngine::OnResetDevice()
{
	CALL( 0x0063B310 );

	FONTHANDLER->OnResetDevice();

	// Post Process
	lpPostProcess->OnResetDevice();

	// Sprite 2D
	UI::ImageRender::OnResetDevice();

	// Rectangle 2D
	pRectangleUI->OnResetDevice();

	pGraphicsDevice->OnResetDevice();

	DXVIDEORENDERER->OnResetDevice();

	//Hide Cursor
//	ShowCursor( FALSE );
}

void DXGraphicEngine::OnLostDevice()
{
	CALL( 0x0063B4C0 );
	FONTHANDLER->OnLostDevice();

	// Post Process
	lpPostProcess->OnLostDevice();

	// Sprite 2D
	UI::ImageRender::OnLostDevice();

	// Rectangle 2D
	pRectangleUI->OnLostDevice();

	pGraphicsDevice->OnLostDevice();

	DXVIDEORENDERER->OnLostDevice();

	//Hide Cursor
	//ShowCursor( FALSE );
}

void DXGraphicEngine::ReloadPostProcess()
{

}

BOOL DXGraphicEngine::CheckDevice()
{
	switch ( hr = lpD3DDev->TestCooperativeLevel() )
	{
		case D3DERR_DEVICELOST:
			OnLostDevice();

			//Yield CPU
			Sleep( 500 );

			//Log
			Log( "CheckDevice(): Device Lost[0x%08X]", hr );

			return FALSE;

		case D3DERR_DEVICENOTRESET:

			OnLostDevice();

			if ( FAILED( hr = lpD3DDev->Reset( &d3dPP ) ) )
			{
				//Yield CPU
				Sleep( 250 );

				//Log
				Log( "CheckDevice(): Device Reset Failed [0x%08X]", hr );

				return FALSE;
			}

			OnResetDevice();

			break;
	}

	return (hr == S_OK);
}

BOOL DXGraphicEngine::Reset( BOOL bResetWindowed, UINT uResetScreenWidth, UINT uResetScreenHeight, BOOL bResetWindow )
{
	//Apply Camera Range
	if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 0 )
	{
		WRITEDWORD( 0x03245894, 0 );
	}
	else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 1 )
	{
		WRITEDWORD( 0x03245894, 22 );
	}
	else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 2 )
	{
		WRITEDWORD( 0x03245894, 64 );
	}

	//Mode same?
	if ( (bResetWindowed == bWindowed) &&
		(uResetScreenWidth == sResolution.uWidth) &&
		(uResetScreenHeight == sResolution.uHeight) )
		return FALSE;

	//Save current PP
	D3DPRESENT_PARAMETERS d3dOldPP;
	CopyMemory( &d3dOldPP, &d3dPP, sizeof( D3DPRESENT_PARAMETERS ) );

	//Back Buffer
	if ( FAILED( hr = lpD3D->CheckDeviceType( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dDM.Format,
		d3dPP.BackBufferFormat,
		bResetWindowed ) ) )
	{
		if ( FAILED( hr = lpD3D->CheckDeviceType( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dDM.Format,
			d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8 ? D3DFMT_X4R4G4B4 : D3DFMT_X8R8G8B8,
			bResetWindowed ) ) )
			return FALSE;

		d3dPP.BackBufferFormat = (d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8 ? D3DFMT_X4R4G4B4 : D3DFMT_X8R8G8B8);
	}

	if ( d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8 )
	{
		//32 bit Back Buffer
		Log( "Reset(): 32 bit Back Buffer" );
	}
	else if ( d3dPP.BackBufferFormat == D3DFMT_X4R4G4B4 )
	{
		//16 bit Back Buffer
		Log( "Reset(): 16 bit Back Buffer" );
	}
	else
	{
		Log( "Reset(): Unknown Back Buffer Format" );
		return FALSE;
	}

	//New Back Buffer Size
	d3dPP.BackBufferWidth = uResetScreenWidth;
	d3dPP.BackBufferHeight = uResetScreenHeight;

	//Multi Sample
	d3dPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dPP.MultiSampleQuality = 0;

	if( SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing )
	{
		D3DMULTISAMPLE_TYPE d3daMultipleSampleType[] = {
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_15_SAMPLES,
			D3DMULTISAMPLE_14_SAMPLES,
			D3DMULTISAMPLE_13_SAMPLES,
			D3DMULTISAMPLE_12_SAMPLES,
			D3DMULTISAMPLE_11_SAMPLES,
			D3DMULTISAMPLE_10_SAMPLES,
			D3DMULTISAMPLE_9_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_7_SAMPLES,
			D3DMULTISAMPLE_6_SAMPLES,
			D3DMULTISAMPLE_5_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_3_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
		};

		for( int i = 0; i < _countof( d3daMultipleSampleType ); i++ )
		{
			D3DMULTISAMPLE_TYPE t = d3daMultipleSampleType[i];
			DWORD q;

			if( SUCCEEDED( hr = lpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dPP.BackBufferFormat,
				bWindowed,
				t,
				&q ) ) &&
				SUCCEEDED( hr = lpD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dPP.AutoDepthStencilFormat,
				bWindowed,
				t,
				&q ) ) )
			{
				d3dPP.MultiSampleType = t;
				d3dPP.MultiSampleQuality = q - 1;
				break;
			}
		}
	}

	Log( "Reset(): Multi Sample: %d %d", d3dPP.MultiSampleType, d3dPP.MultiSampleQuality );

	//Window specific PP settings
	if ( bResetWindowed )
	{
		d3dPP.Windowed = TRUE;
		d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}
	else
	{
		d3dPP.Windowed = FALSE;
		d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if ( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE )
	{
		Log( "Presentation Interval Immediate" );
		d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bVSync )
	{
		if ( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE )
		{
			Log( "Presentation Interval One" );
			d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
	}

	//Lose Device and attempt Reset on new PP
	OnLostDevice();
	if ( FAILED( hr = lpD3DDev->Reset( &d3dPP ) ) )
	{
		CopyMemory( &d3dPP, &d3dOldPP, sizeof( D3DPRESENT_PARAMETERS ) );

		//Yield CPU
		Sleep( 100 );

		//Log
		Log( "Reset(): Device Reset Failed" );

		//Reset back to previous (working) PP
		OnLostDevice();
		if ( FAILED( hr = lpD3DDev->Reset( &d3dPP ) ) )
		{
			//Yield CPU
			Sleep( 100 );

			//Log
			Log( "Reset(): Device Rollback Failed" );
		}
		else
		{
			OnResetDevice();

			//Log
			Log( "Reset(): Device Rollbacked" );
		}

		return FALSE;
	}

	//Save new mode
	bWindowed = bResetWindowed;

	sResolution.uWidth = uResetScreenWidth;
	sResolution.uHeight = uResetScreenHeight;

	//Reset Device
	OnResetDevice();

	//Log
	Log( "Reset(): Device Reset" );

	return TRUE;
}

BOOL DXGraphicEngine::SaveRenderTargetToFile( const char * pszFilePath )
{
	//Get Render Target
	LPDIRECT3DSURFACE9 lpRenderTarget;
	if( FAILED( hr = lpD3DDev->GetRenderTarget( 0, &lpRenderTarget ) ) )
	{
		Log( "Screenshot(): GetRenderTarget Failed" );

		return FALSE;
	}

	//Save to File
	BOOL bRet = SUCCEEDED( hr = D3DXSaveSurfaceToFileA( pszFilePath, D3DXIFF_PNG, lpRenderTarget, NULL, NULL ) );

	//Log if Fail
	if( !bRet )
		Log( "Screenshot(): D3DXSaveSurfaceToFileA Failed" );

	//Clean up
	RELEASE( lpRenderTarget );

	return bRet;
}

UINT DXGraphicEngine::GetVideoMemory()
{
	if ( lpD3DDev )
		return lpD3DDev->GetAvailableTextureMem();

	return 0;
}

std::string DXGraphicEngine::GetVideoName()
{
	D3DADAPTER_IDENTIFIER9 s;

	if ( FAILED( lpD3D->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &s ) ) )
		return "";

	return std::string( s.Description );
}

UINT DXGraphicEngine::GetVertexSize( DWORD dwVertexTypeDesc )
{
	UINT uVertexSize = 0;

	//Get Vertex Size
	if ( dwVertexTypeDesc == 324 )
		uVertexSize = 28;
	else if ( dwVertexTypeDesc == 452 )
		uVertexSize = sizeof( EXEDrawVertex );
	else if ( dwVertexTypeDesc == 708 )
		uVertexSize = sizeof( EXEDrawVertex ) + 8;
	else if ( dwVertexTypeDesc == 964 )
		uVertexSize = sizeof( EXEDrawVertex ) + 16;
	else if ( dwVertexTypeDesc == 1220 )
		uVertexSize = sizeof( EXEDrawVertex ) + 24;

	return uVertexSize;
}

void DXGraphicEngine::Begin3D()
{
}

void DXGraphicEngine::End3D()
{
}

DWORD WINAPI DXGraphicEngine::ThreadLoadingBar()
{
	HINSTANCE hInst = GetModuleHandleA( NULL );
	HWND hWnd		= GRAPHICENGINE->GetWindowHandler();

	HBITMAP hbaLoadingBar[2];
	hbaLoadingBar[0] = (HBITMAP)LoadImageA( hInst, "game\\images\\misc\\loadingbar_.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	hbaLoadingBar[1] = (HBITMAP)LoadImageA( hInst, "game\\images\\misc\\loadingbar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );

	HDC hDC;

	HDC hDCMem[2];
	hDCMem[0] = CreateCompatibleDC( GetDC( hWnd ) );
	hDCMem[1] = CreateCompatibleDC( GetDC( hWnd ) );

	// Set objects
	SelectObject( hDCMem[0], hbaLoadingBar[0] );
	SelectObject( hDCMem[1], hbaLoadingBar[1] );

	int iWidth	= 0;
	int iHeight = 0;

	// Quit? Loading bar exit.
	while ( !GRAPHICENGINE->bQuitLoading )
	{
		// Hide? Suspend Thread
		if ( !GRAPHICENGINE->bShowLoading || GRAPHICENGINE->iCurrentLoading == -1 )
			SuspendThread( GetCurrentThread() );

		if ( GRAPHICENGINE->bQuitLoading )
			break;

		hDC	= GetDC( GRAPHICENGINE->GetWindowHandler() );

		int iCur = GRAPHICENGINE->iMaxLoading ? ((262 * GRAPHICENGINE->iCurrentLoading) / GRAPHICENGINE->iMaxLoading) : 0;
		if ( iCur > 262 )
			iCur = 262;

		// Render
		BitBlt( hDC, ( RESOLUTION_WIDTH >> 1 ) - ( 262 >> 1 ) - 2, ( RESOLUTION_HEIGHT >> 1 ) + ( 600 >> 1 ) - 113, 262, 33, hDCMem[ 1 ], 0, 0, SRCCOPY );
		BitBlt( hDC, ( RESOLUTION_WIDTH >> 1 ) - ( 262 >> 1 ) - 2, ( RESOLUTION_HEIGHT >> 1 ) + ( 600 >> 1 ) - 113, iCur, 33, hDCMem[ 0 ], 0, 0, SRCCOPY );

		Sleep( 0 );
	}

	// Free mem, no need more
	ReleaseDC( hWnd, hDCMem[ 0 ] );
	ReleaseDC( hWnd, hDCMem[ 1 ] );

	if ( hbaLoadingBar[0] )
		DeleteObject( hbaLoadingBar[0] );
	if ( hbaLoadingBar[1] )
		DeleteObject( hbaLoadingBar[1] );

	return FALSE;
}

void WINAPI DXGraphicEngine::UpdateLoading( int iCurrent, int iMax )
{
	GRAPHICENGINE->iCurrentLoading	= iCurrent;
	GRAPHICENGINE->iMaxLoading		= iMax;
}

void WINAPI DXGraphicEngine::ShowLoading()
{
	GRAPHICENGINE->iCurrentLoading	= 0;
	GRAPHICENGINE->iMaxLoading		= 1;

	/*if ( !GRAPHICENGINE->bShowLoading )
	{
		GRAPHICENGINE->bShowLoading = TRUE;
		ResumeThread( GRAPHICENGINE->hLoadingThread );
	}*/
}

void WINAPI DXGraphicEngine::HideLoading()
{
	GRAPHICENGINE->bShowLoading = FALSE;
}

DWORD WINAPI DXGraphicEngine::DrawLoadingImage()
{
	// Define Infobox text-length, title and text/tip
	int iWidthText = 0;


	// Random Loading Screen
	if( !pLoadingImageHandler && !pBackgroundImageHandler && !pInfoBoxHandler)
	{
		int iRand					= Dice::RandomU( 0, _countof(szaLoadingImagesLoading) - 1 );
		pBackgroundImageHandler		= UI::ImageLoader::LoadImage( "game\\images\\loadingscreens\\%s-blur.png", szaLoadingImagesLoading[iRand] );
		pLoadingImageHandler		= UI::ImageLoader::LoadImage( "game\\images\\loadingscreens\\%s.png", szaLoadingImagesLoading[iRand] );
		pInfoBoxHandler				= UI::ImageLoader::LoadImage( "game\\images\\loadingscreens\\box.png" );
	}

	// Render Shadow Loading if greater than 800x600
	if( GRAPHICENGINE->GetWidthBackBuffer() > 800 && GRAPHICENGINE->GetHeightBackBuffer() > 600 )
	{
		if ( !pBorderLoadingImageHandler )
			pBorderLoadingImageHandler = UI::ImageLoader::LoadImage( "game\\images\\loadingscreens\\border.png" );

		UI::ImageRender::Render( pBackgroundImageHandler, 0, 0, GRAPHICENGINE->GetWidthBackBuffer(), GRAPHICENGINE->GetHeightBackBuffer(), D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		UI::ImageRender::Render( pBorderLoadingImageHandler, ( GRAPHICENGINE->GetWidthBackBuffer() / 2 ) - ( 898 / 2 ), ( GRAPHICENGINE->GetHeightBackBuffer() / 2 ) - ( 698 / 2 ), 898, 698, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	}

	// Render Loading Screen 800x600
	UI::ImageRender::Render( pLoadingImageHandler, (GRAPHICENGINE->GetWidthBackBuffer()/2) - (800/2), (GRAPHICENGINE->GetHeightBackBuffer()/2) - (600/2), 800, 600, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

	// Render Info Box
	UI::ImageRender::Render( pInfoBoxHandler, (GRAPHICENGINE->GetWidthBackBuffer() / 2) - (455 / 2), (GRAPHICENGINE->GetHeightBackBuffer() ) - 147, 455, 90, D3DCOLOR_RGBA(255, 255, 255, 255 ) );

	// Draw Info Box Title

	const char * cInfoTitle = GRAPHICENGINE->GetLoadingInfoTitle ();
	iWidthText = FONTHANDLER->GetWidthText(0, cInfoTitle, lstrlenA(cInfoTitle));
	FONTHANDLER->Draw(3, (GRAPHICENGINE->GetWidthBackBuffer() / 2) - (iWidthText / 2), (GRAPHICENGINE->GetHeightBackBuffer() ) - 140, cInfoTitle, DT_WORDBREAK, -1);

	// Draw Info Box Text
	
	const char * cInfoText = GRAPHICENGINE->GetLoadingInfoText ();

	int iWidth = 430;
	int iHeight = 50;
	int iY = ( GRAPHICENGINE->GetHeightBackBuffer () ) - 115;
	int iX = ( GRAPHICENGINE->GetWidthBackBuffer () / 2 ) - ( iWidth / 2 );

	FONTHANDLER->Draw ( 0, RECT{ iX, iY, iX + iWidth, iY + iHeight }, cInfoText, DT_WORDBREAK | DT_CENTER | DT_VCENTER, -1 );

	return TRUE;
}

DWORD __cdecl DXGraphicEngine::TakenScreenShot(DWORD dw1, DWORD dwType)
{
	bNoHud = FALSE;
	if ( dwType == 100 )
		*(BOOL*)(0x00824F80) = FALSE;

	dwTimeScreenShot = TICKCOUNT + 100;

	bTakenScreenShot = TRUE;

	return TRUE;
}

DWORD WINAPI DXGraphicEngine::GetScreenShot()
{
	if ( !bTakenScreenShot )
		return TRUE;

	if ( dwTimeScreenShot > TICKCOUNT )
		return TRUE;

	bTakenScreenShot = FALSE;

	SYSTEMTIME sLocalTime;
	GetLocalTime( &sLocalTime );
	BOOL bHud = *(BOOL*)(0x00824F80);


	GRAPHICENGINE->GetDevice()->BeginScene();
	GRAPHICENGINE->GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	if ( bHud )
	{
		char szLocalTime[256]	= { 0 };
		char szServerTime[256]	= { 0 };
		char szTitle[128]		= { 0 };
		int iWidthText			= 0;

		SYSTEMTIME sServerTime;
		CopyMemory( &sServerTime, &GameCore::GetInstance()->sServerTime, sizeof( SYSTEMTIME ) );

		STRINGFORMAT( szTitle, "[%d] - %s (%s)", *(int*)(0x04AF7FFC), UNITDATA->sCharacterData.szName, (char *)(0x038FEF40 + ((*(int*)0x4B06960 - 1) * 0x68)) );
		iWidthText = FONTHANDLER->GetWidthText( 0, szTitle, lstrlenA( szTitle ) );
		FONTHANDLER->Draw( 0, (GRAPHICENGINE->GetWidthBackBuffer() >> 1) - (iWidthText >> 1), 4, szTitle, DT_WORDBREAK, -1 );

		STRINGFORMAT( szLocalTime, "Local Time: %04d-%02d-%02d %02d:%02d:%02d", sLocalTime.wYear, sLocalTime.wMonth, sLocalTime.wDay, sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond );
		iWidthText = FONTHANDLER->GetWidthText( 0, szLocalTime, lstrlenA( szLocalTime ) );
		FONTHANDLER->Draw( 0, (GRAPHICENGINE->GetWidthBackBuffer() >> 1) - (iWidthText >> 1), 4 + 16, szLocalTime, DT_WORDBREAK, -1 );

		STRINGFORMAT( szServerTime, "Server Time: %04d-%02d-%02d %02d:%02d:%02d", sServerTime.wYear, sServerTime.wMonth, sServerTime.wDay, sServerTime.wHour, sServerTime.wMinute, sServerTime.wSecond );
		iWidthText = FONTHANDLER->GetWidthText( 0, szServerTime, lstrlenA( szServerTime ) );
		FONTHANDLER->Draw( 0, (GRAPHICENGINE->GetWidthBackBuffer() >> 1) - (iWidthText >> 1), 4 + 16 + 16, szServerTime, DT_WORDBREAK, -1 );

		CALL( 0x005059D0 );
	}
	char szFile[128] = { 0 };
	CreateDirectoryA( "save\\screenshots", NULL );
	CreateDirectoryA( "save\\screenshots\\no_hud", NULL );

	if ( *(BOOL*)(0x00824F80) )
		STRINGFORMAT( szFile, "save\\screenshots\\%04d-%02d-%02d-%02d-%02d-%02d.png", sLocalTime.wYear, sLocalTime.wMonth, sLocalTime.wDay, sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond );
	else
		STRINGFORMAT( szFile, "save\\screenshots\\no_hud\\%04d-%02d-%02d-%02d-%02d-%02d.png", sLocalTime.wYear, sLocalTime.wMonth, sLocalTime.wDay, sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond );

	CALL( 0x004529B0 );

	GRAPHICENGINE->GetDevice()->EndScene();

	//Get Render Target
	LPDIRECT3DSURFACE9 lpRenderTarget;
	if ( SUCCEEDED( GRAPHICENGINE->hr = GRAPHICENGINE->GetDevice()->GetRenderTarget( 0, &lpRenderTarget ) ) )
	{
		D3DXSaveSurfaceToFileA( szFile, D3DXIFF_PNG, lpRenderTarget, NULL, NULL );
	}

	RELEASE( lpRenderTarget );
	GRAPHICENGINE->GetDevice()->Present( NULL, NULL, NULL, NULL );

	ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> Screenshot saved to '%s'", szFile );

	*(BOOL*)(0x00824F80) = TRUE;

	return TRUE;
}

BOOL DXGraphicEngine::ResolutionChange( BOOL bFirstTime, int iWidth, int iHeight )
{
	bool bResolutionChanged = false;

	if( RESOLUTION_WIDTH != iWidth || RESOLUTION_HEIGHT != iHeight )
		bResolutionChanged = true;

	int iCalcWidth = ( iWidth / 4 ) * 3;

	// Write power memory
	DWORD dwOld;
	VirtualProtect( ( void* )0x007AFD88, 8, PAGE_EXECUTE_READWRITE, &dwOld );

	if ( iCalcWidth == iHeight )
	{
		*( float* )0x7AFD88 = 0.001666667f;
		*( float* )0x7AFD8C = 0.00125f;
		iCalcWidth = iWidth / 2;
	}
	else
	{
		*( float* )0x7AFD88 = 0.001466667f;
		*( float* )0x7AFD8C = 0.00094f;
		iCalcWidth = ( ( ( iHeight / 3 ) * 4 ) / 2 );
	}

	*( float* )0x0A1608C = ( float )iHeight * *( float* )0x7AFD88;
	*( float* )0x0A1741C = ( float )iWidth * *( float* )0x7AFD8C;

	int iZCamera = ((iHeight / 3) * 4);

	SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = iWidth;
	SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = iHeight;

	*( int* )0x04C38FA8 = ( SETTINGSHANDLER->GetModel()->GetSettings().dwHeight / 2 ) + 112;

	*( UINT* )DXSettings::WidthWindowConfig		= iWidth;
	*( UINT* )DXSettings::HeightWindowConfig	= iHeight;
	*( UINT* )DXSettings::WidthWindow			= *( UINT* )DXSettings::WidthWindowConfig;
	*( UINT* )DXSettings::HeightWindow			= *( UINT* )DXSettings::HeightWindowConfig;
	*( UINT* )DXSettings::WidthBBDevice			= *( UINT* )DXSettings::WidthWindowConfig;
	*( UINT* )DXSettings::HeightBBDevice		= *( UINT* )DXSettings::HeightWindowConfig;
	*( UINT* )DXSettings::WidthDevice			= *( UINT* )DXSettings::WidthWindowConfig;
	*( UINT* )DXSettings::HeightDevice			= *( UINT* )DXSettings::HeightWindowConfig;
	*( UINT* )DXSettings::WidthCameraView		= iWidth /2;
	*( UINT* )DXSettings::DistanceCamera		= iZCamera;
	*( UINT* )DXSettings::HeightRenderDimension = iHeight / 2;
	*( UINT* )DXSettings::HeightCameraView		= iHeight / 2;
	*( UINT* )DXSettings::WidthRenderDimension	= iWidth / 2;
	*( UINT* )DXSettings::WindowedConfig		= *( UINT* )DXSettings::WindowedRender;


	/*
	UINT uLoginMemory = *( UINT* )0x39016D4;
	if ( uLoginMemory != 0 )
	{
		*(UINT*)(uLoginMemory + 0x834) = (*(UINT*)DXSettings::WidthBBDevice - 800) / 2;
		*(UINT*)(uLoginMemory + 0x838) = (*(UINT*)DXSettings::HeightBBDevice - 600) / 2;
		*(UINT*)(uLoginMemory + 0x820) = *(UINT*)(uLoginMemory + 0x834) + 100;
		*( UINT* )( uLoginMemory + 0x824 ) = *( UINT* )( uLoginMemory + 0x838 ) + 63;

		uLoginMemory = *( UINT* )0x39016D0;
		if ( uLoginMemory != 0 )
			*( UINT* )( uLoginMemory + 0x1C4 ) = *( UINT* )DXSettings::WidthBBDevice / 2;
	}
	*/

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
	{
		CLoginScreen * p = (CLoginScreen*)GAMESCREEN->GetActiveScreen();

		p->SetLoginBoxSize( (*(UINT*)DXSettings::WidthBBDevice - 800) >> 1, (*(UINT*)DXSettings::HeightBBDevice - 600) >> 1 );
		p->SetLoginBoxPosition( ((*(UINT*)DXSettings::WidthBBDevice - 800) >> 1) + 100, ((*(UINT*)DXSettings::HeightBBDevice - 600) >> 1) + 63 );
	}

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Character )
	{
		CCharacterScreen * p = (CCharacterScreen*)GAMESCREEN->GetActiveScreen();

		p->SetScreenHalf( (*(UINT*)DXSettings::WidthBBDevice) >> 1 );
	}

	// New Projection
	typedef void( __cdecl *tfnSetDxProjection )(float radians, int SizeX, int SizeY, float near_z, float far_z);
	tfnSetDxProjection fnSetDxProjection = (tfnSetDxProjection)0x0047E270;
	fnSetDxProjection( ( ( float )3.141593f / 4.4f ), iWidth, iHeight, 20.f, 4000.f );

	//Adjust iItemSlot of Restart box
	{
		WRITEDWORD( 0x035E10FC, ( RESOLUTION_WIDTH >> 1 ) - ( 188 >> 1 ) );
		WRITEDWORD( 0x035E1100, ( RESOLUTION_HEIGHT >> 1 ) - ( 128 >> 1 ) );

		WRITEDWORD( 0x035E1108, ( RESOLUTION_WIDTH >> 1 ) - ( 188 >> 1 ) + 25 );
		WRITEDWORD( 0x035E110C, ( RESOLUTION_HEIGHT >> 1 ) - ( 128 >> 1 ) + 75 );

		WRITEDWORD( 0x04B0DEF0, (RESOLUTION_WIDTH >> 1) - (168 >> 1) );
		WRITEDWORD( 0x04B0DEF4, (RESOLUTION_HEIGHT >> 1) - (124 >> 1) );

		//Message Box Position Update
		struct MessageBoxTemp
		{
			char				szMessage1[32];
			char				szMessage2[32];
			BOOL				bActive;
			int					iType; //1 = OK CANCEL, 2 = YES NO
			int					iX;
			int					iY;
			int					iTextX;
			int					iLine;
			char				szReceiveMessage[64];
		};

		int iIndex = READDWORD( 0x04B0DEE0 );

		MessageBoxTemp * ps = &((MessageBoxTemp*)(0x0095DC20))[iIndex];

		ps->iX = (RESOLUTION_WIDTH >> 1) - (168 >> 1);
		ps->iY = (RESOLUTION_HEIGHT >> 1) - (124 >> 1);
	}

	if( !bFirstTime && bResolutionChanged )
	{
		GAME->OnResolutionChanged();
		GAMECORE->OnResolutionChanged();
	}

	DEBUG( "%d", READDWORD( 0x82BF0C ) );

	return TRUE;
}

BOOL DXGraphicEngine::ResizeWindow()
{
	if ( !SETTINGSHANDLER->GetModel()->GetSettings().bWindowed )
	{

			//RECT R = { 0, 0, GetSystemMetrics( SM_CXSCREEN ),
			//	GetSystemMetrics( SM_CYSCREEN ) };

		RECT srcRect;

		srcRect.right = SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
		srcRect.bottom = SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;
		srcRect.left = 0;
		srcRect.top = 0;

		AdjustWindowRect( &srcRect, WS_FULLSCREEN | WS_VISIBLE, false );

		SetWindowPos( GRAPHICENGINE->GetWindowHandler(), NULL, (GetSystemMetrics( SM_CXSCREEN ) / 2) - (srcRect.right / 2),
			(GetSystemMetrics( SM_CYSCREEN ) / 2) - (srcRect.bottom / 2), srcRect.right, srcRect.bottom, SWP_DRAWFRAME | SWP_NOZORDER );

		SetWindowLongA( GRAPHICENGINE->GetWindowHandler(), GWL_STYLE, WS_FULLSCREEN | WS_VISIBLE );

		//SetWindowPos( GRAPHICENGINE->GetWindowHandler(), NULL, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), SWP_DRAWFRAME | SWP_NOZORDER );
	}
	else
	{

			RECT srcRect;

			srcRect.right = SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
			srcRect.bottom = SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;
			srcRect.left = 0;
			srcRect.top = 0;

			UINT uWindowStyle = WS_VISIBLE | WS_WINDOWED;

			// Adjust Window
			AdjustWindowRect( &srcRect, uWindowStyle, FALSE );

			srcRect.right -= srcRect.left;
			srcRect.bottom -= srcRect.top;

			SetWindowLongA( GRAPHICENGINE->GetWindowHandler(), GWL_STYLE, uWindowStyle );
			SetWindowPos( GRAPHICENGINE->GetWindowHandler(), HWND_NOTOPMOST, ( GetSystemMetrics( SM_CXSCREEN ) / 2 ) - ( srcRect.right / 2 ),
						  ( GetSystemMetrics( SM_CYSCREEN ) / 2 ) - ( srcRect.bottom / 2 ), srcRect.right, srcRect.bottom, SWP_SHOWWINDOW );
	}
	return TRUE;
}

DX::GraphicsDevice_ptr GetGraphicsDevice()
{
	return GRAPHICENGINE->GetRenderer()->GetGraphicsDevice();
}
