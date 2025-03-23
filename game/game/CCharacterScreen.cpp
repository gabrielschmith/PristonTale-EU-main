#include "stdafx.h"
#include "CCharacterScreen.h"


CCharacterScreen::CCharacterScreen()
{
	CALLT_WITH_ARG1( 0x005881A0, GetThis(), TRUE );
}


CCharacterScreen::~CCharacterScreen()
{
	CALLT( 0x00589530, GetThis() );
}

BOOL CCharacterScreen::Init()
{
	CALLT( 0x005882F0, GetThis() );

	Sky::GetInstance()->SetSky( 128 );

	//Store account name
	INI::CReader cReader ( "game.ini" );
	if ( (char *)0x39033E8 )
	{
		cReader.WriteString ( "Game", "LastAccount", (char *)0x39033E8 );
	}

	//Get server name
	char szServerName[64] = { 0 };
	STRINGFORMAT ( szServerName, "%s", (char *)( 0x038FEF40 + ( ( *(int *)0x4B06960 - 1 ) * 0x68 ) ) );

	DEBUG ( "CWorldScreen::Init(). SERVER NAME = %s", szServerName );

	Game::DeleteClanTag ( szServerName );

	return TRUE;
}

void CCharacterScreen::Shutdown()
{
}

void CCharacterScreen::Render3D()
{
	Sky::Draw( 0, 0, 0, 0, 0, 0 );

	CALLT( 0x00589620, GetThis() );
}

void CCharacterScreen::Render2D()
{
}

EScreen CCharacterScreen::Frame()
{
	DWORD dwRet = CALLT( 0x00588710, GetThis() );
	CALLT( 0x00589500, GetThis() );

	SOCKETGAME->PingConnections();

	if ( dwRet == 1 )
	{
		if ( bLoadingTime )
		{
			return SCREEN_World;
		}
	}

	if ( dwRet == 2 )
		return SCREEN_Login;

	if ( dwRet == 3 )
	{
		bLoadingTime = TRUE;
	}

	return SCREEN_Character;
}

void CCharacterScreen::OnMouseMove( CMouse * pcMouse )
{
}

BOOL CCharacterScreen::OnMouseClick( CMouse * pcMouse )
{
	return 0;
}
