#include "stdafx.h"
#include "CWorldScreen.h"

CWorldScreen::CWorldScreen()
{
}

CWorldScreen::~CWorldScreen()
{
}

BOOL CWorldScreen::Init()
{
	CALL( 0x00542B30 );
	GameCore::SetGameScreen( SCREEN_Character );


	pcBoneTarget = EXEModel::ReadBone( "game\\meshes\\test\\test.ASE" );

	INIT( MINIMAPHANDLER );

	return TRUE;
}

void CWorldScreen::Shutdown()
{
}

void CWorldScreen::Render3D()
{
	CALL( 0x00416960 ); //Render View Sight?

	Point3D sPosition	= Point3D( (*(int*)0x00A17520), (*(int*)0x00A17510), (*(int*)0x00A1619C) );

	Point3D sAngle		= Point3D( (*(int*)0x00824FA8), (*(int*)0x00A17630) & 0x00000FFF, (*(int*)0x00A17634) & 0x00000FFF );


//	Render( sPosition.iX, sPosition.iY, sPosition.iZ, sAngle.iX, sAngle.iY, sAngle.iZ );

	CALL( 0x0041B630 );

}

void CWorldScreen::Render2D()
{
	RENDERER->CloseCamera();

	//ISBIGMAP actually makes the client screen small
	// and display the big map
	//SET TO TRUE to try it out.
	//ISBIGMAP = TRUE;

	if( ISBIGMAP )
		CALL( 0x00417FE0 );
	else if( bHUDInitialized )
	{
		HUDHANDLER->Render();
		BLESSCASTLEHANDLER->Render();
	}
}

int CWorldScreen::SinMain ()
{
	int result; // eax

	CALL ( 0x505F30 );	//sinKeyClickMain
	CALL ( 0x505FA0 );	//sinMouseMain

	if ( CALL_WITH_ARG1 ( 0x505F00, 0 ) == 1 )
		CALL_WITH_ARG1 ( 0x4CFDB0, 10 );

	if ( CALL_WITH_ARG1 ( 0x505F00, 1 ) == 1 )
		CALL_WITH_ARG1 ( 0x4CFDB0, 110 );

	if ( READDWORD ( 0x367E828 ) == 11 )
	{
		CALL_WITH_ARG1 ( 0x4CFDB0, READDWORD ( 0x367E828 ) );
	}
	else
	{
		if ( READDWORD ( 0x367E828 ) != 111 )
			goto LABEL_10;

		CALL_WITH_ARG1 ( 0x4CFDB0, 111 );
	}
	WRITEDWORD ( 0x367E828, 0 );

LABEL_10:
	CALL_WITH_ARG1 ( 0x4CFDB0, 2 );                             // sinProc(SINRBUTTONUP);
	CALL ( 0x50B050 );
	CALL ( 0x4C81E0 );

	//dwCheckTime

	CALLT ( 0x4A8CB0, 0x35E11D0 );	//cINTERFACE::Main

	//Mouse is over first button
	if ( READDWORD ( 0x35E1104 ) == 1 )
	{
		if ( MAP_ID == MAPID_GardenOfFreedom || //GOF
			 MAP_ID == MAPID_CastleOfTheLost || //COTL
			 READDWORD ( 0x209EA4C ) != 0 )			//Resurrection used?
		{
			lstrcpyA ( (LPSTR)0x35E09B8, "Revive in field" );
			lstrcpyA ( (LPSTR)0x35E0D28, "at no cost." );

			//WRITEDWORD ( 0x209EA4C, TRUE ); //PlayUsed_Resurrection
			//WRITEDWORD ( 0x35E1114, 1 );    //ReStartOptionIndex = Restart in field
			//WRITEWORD ( 0x35E09B0, 1400 );  //set timer to force respawn
		}
		else if ( MAP_ID == MAPID_Bellatra )
		{
			WRITEDWORD ( 0x35E1104, 0 );	//ReStartIndex

			lstrcpyA ( (LPSTR)0x35E09B8, "Revive in field" );
			lstrcpyA ( (LPSTR)0x35E0D28, "is not allowed" );
		}
	}

	CALLT ( 0x504BD0, 0x367E148 );
	CALLT ( 0x4C54C0, 0x35EBB20 );
	CALLT ( 0x498E70, 0x35D0EA0 );
	CALLT ( 0x4E6D20, 0x3653E40 );
	CALLT ( 0x51D490, 0x36EFD98 );	//cTRADE::Main
	CALLT ( 0x4CF0F0, 0x3628C08 );
	CALLT ( 0x4D2BE0, 0x362B210 );
	CALLT ( 0x49A990, 0x35E05D8 );
	CALLT ( 0x520710, 0x3707108 );	//cSINWARPGATE::Main
	CALLT ( 0x4A56F0, 0x35E0658 );
	CALLT ( 0x4DCC90, 0x362BF88 );
	CALLT ( 0x4DFB30, 0x362BEA8 );
	CALL  ( 0x48D900 );
	CALLT ( 0x494B10, 0x35D0350 );
	CALL  ( 0x65DFC0 );
	CALL  ( 0x505FD0 );
	result = CALLT ( 0x4D7300, 0x362B388 );
	if ( READDWORD ( 0x362A3BC ) )
	{
		result = READDWORD ( 0x362A3C0 )++ + 1;
		if ( (unsigned int)READDWORD ( 0x362A3C0 ) >= 0xD2 )
			WRITEDWORD ( 0x362A3BC, 0 );
	}
	else
	{
		WRITEDWORD ( 0x362A3C0, 0 );
	}
	return result;
}

EScreen CWorldScreen::Frame()
{
	MOUSEHANDLER->FixedToRealPosition();

	SOCKETGAME->Loop();

	CALL( 0x00416A20 ); //Camera Movement...

	UNITDATA->bRunMode = GameCore::IsWalking();

	if ( !ISBIGMAP )
	{
		Point2D * p = ((Point2D*)0x00A17518);

		CMouse::AdjustMouseToResolution( p, TRUE );


		SinMain ();
		//CALL( 0x004D0740 ); // sinMain


		CMouse::AdjustMouseToResolution( p, FALSE );
	}
	else
	{
		CALL( 0x0050B050 );
	}

	if ( (*(BOOL*)0x03A97614) )
	{
		CALL( 0x00404870 );
	}

	return SCREEN_World;
}

void CWorldScreen::Update( float fTime )
{
	if( UNITDATA && !bHUDInitialized )
	{
		if( UNITDATA->sCharacterData.iFlag > 0 )
		{
			bHUDInitialized = true;
			INIT( HUDHANDLER );
			OnResolutionChanged();
		}
	}
}

void CWorldScreen::OnMouseMove( CMouse * pcMouse )
{
	if( bHUDInitialized )
		HUDHANDLER->OnMouseMove( pcMouse );
}

BOOL CWorldScreen::OnMouseClick( CMouse * pcMouse )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnMouseClick( pcMouse );

	return FALSE;
}

BOOL CWorldScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
	BOOL bRet = FALSE;

	if( bHUDInitialized )
		bRet = HUDHANDLER->OnKeyPress( pcKeyboard );

	if( pcKeyboard->GetKey() == 'F' && pcKeyboard->GetEvent() == KeyDown )
		if( CURRENT_TARGET && (CURRENT_TARGET->sCharacterData.iType != CHARACTERTYPE_NPC) && (CURRENT_TARGET->sCharacterData.iType != CHARACTERTYPE_Player) && (CURRENT_TARGET->PkMode_CharState != CHARACTERTYPE_Player) )
			CHARACTERGAME->FocusTargetPet( CURRENT_TARGET->iID );

	return bRet;
}

BOOL CWorldScreen::OnMouseScroll( CMouse * pcMouse )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnMouseScroll( pcMouse );

	return FALSE;
}

BOOL CWorldScreen::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnKeyChar( pcKeyboard );

	return FALSE;
}

void CWorldScreen::OnResolutionChanged()
{
	if( bHUDInitialized )
		HUDHANDLER->OnResolutionChanged();
}
