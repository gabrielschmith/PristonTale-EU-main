#include "stdafx.h"
#include "ToolTip.h"

CToolTip::CToolTip()
{
}

CToolTip::~CToolTip()
{
}

void CToolTip::Render()
{
	//warming up
	if ( iWarmUp-- > 0 )
		return;

	//count down to disappear
	if ( iCountdown-- > 0 )
	{
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX - 3, iY - 1 - iHeightText, iWidthText + 6, iHeightText + 3 ), D3DCOLOR_ARGB( 200, 0, 0, 0 ) );

		auto const c = D3DCOLOR_ARGB( 100, 255, 255, 255 );

		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX - 2,			  iY - 1 - iHeightText, iWidthText + 4, 1), c ); //top line
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX - 2,			  iY + 1,			    iWidthText + 4,	1), c ); //bottom line

		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX - 3,              iY - iHeightText - 1,		1, iHeightText + 3 ), c ); //left line
		GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + iWidthText + 2, iY - iHeightText - 1,		1, iHeightText + 3 ), c ); //right line

		FONTHANDLER->Draw( iFontID, RECT{ iX, iY, iX, iY }, this->pszText.c_str(), DT_LEFT | DT_BOTTOM | DT_EXPANDTABS | DT_NOCLIP, D3DCOLOR_ARGB(200,255,255,255) );
	}
}

void CToolTip::SetText( int iX, int iY, const char * pszText )
{
	if ( iX != this->iX || iY != this->iY || !STRINGCOMPARE(pszText, this->pszText.c_str() ) )
	{
		iWarmUp = 10;

		this->iX = iX;
		this->iY = iY;
		this->pszText = pszText;

		iWidthText = FONTHANDLER->GetWidthText ( iFontID, this->pszText.c_str (), this->pszText.length () );
		iHeightText = FONTHANDLER->GetHeightText( iFontID, this->pszText.c_str (), RECT{0,0,0,0}  );
	}

	iCountdown = 10;
}

void CToolTip::Tick()
{

}

void CToolTip::Init()
{
	pBorderImages[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\bottomleft.png" );
	pBorderImages[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\bottomright.png" );
	pBorderImages[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\left.png" );
	pBorderImages[3] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\right.png" );
	pBorderImages[4] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\topleft.png" );
	pBorderImages[5] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\topright.png" );
	pBorderImages[4] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\top.png" );
	pBorderImages[5] = UI::ImageLoader::LoadImage( "game\\images\\UI\\tooltip\\bottom.png" );


	iFontID = FONTHANDLER->CreateFont( "Arial", 16, 0, FALSE, FALSE, -1 );

}