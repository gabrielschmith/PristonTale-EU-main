#include "stdafx.h"
#include "UIImageBox.h"

namespace UI
{
ImageBox::ImageBox(Rectangle2D rRectangle) : Element(rRectangle)
{
}

ImageBox::~ImageBox()
{
}

void ImageBox::Clear()
{
	CLEARPTR( pImage );
	UI::ImageLoader::DeleteImage( pImage );

	if ( pImageActive1 != pImage )
	{
		CLEARPTR( pImageActive1 );
		UI::ImageLoader::DeleteImage( pImageActive1 );
	}

	if ( pImageActive2 != pImage )
	{
		CLEARPTR( pImageActive2 );
		UI::ImageLoader::DeleteImage( pImageActive2 );
	}
}

void ImageBox::SetImage( class UI::Image * pImage )
{
	this->pImage = pImage;
}

void ImageBox::SetActiveImage( std::pair<class UI::Image *, class UI::Image *> pPair )
{
	pImageActive1 = pPair.first;
	pImageActive2 = pPair.second;

	this->pImage = pImageActive1;
}

void ImageBox::SetToolTip( const std::string szMessage  )
{
	szToolTip = szMessage;
}



void ImageBox::SetGroup( std::shared_ptr<UI::Group> pGroupMain )
{
	pGroup = pGroupMain;
}

void ImageBox::Swap()
{
	/*if ( pImage )
	{
		pImage = UI::ImageLoader::Swap( pImage->GetPointer() );
	}
	if ( pImageActive1 )
	{
		pImageActive1 = UI::ImageLoader::Swap( pImageActive1->GetPointer() );
	}
	if ( pImageActive2 )
	{
		pImageActive2 = UI::ImageLoader::Swap( pImageActive2->GetPointer() );
	}*/

	if( pImage == pImageActive1 )
		pImage = pImageActive2;
	else
		pImage = pImageActive1;
}

void ImageBox::Swap( BOOL bActiveImage )
{
	pImage = bActiveImage ? pImageActive2 : pImageActive1;
}

BOOL ImageBox::OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( EMouseEvent::ClickDownL != pcMouse->GetEvent() )
		return FALSE;

	Rectangle2D rBox = { GetX() + iX, GetY() + iY, GetWidth(), GetHeight() };
	if ( rBox.Inside( pcMouse->GetPosition() ) )
	{
		UI::Event::Invoke( iEventID );
		return TRUE;
	}

	return FALSE;
}

BOOL ImageBox::OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	Rectangle2D rBox = { GetX() + iX, GetY() + iY, GetWidth(), GetHeight() };
	if ( rBox.Inside( pcMouse->GetPosition() ) )
	{
		bHovered = TRUE;
	}
	else
	{
		bHovered = FALSE;
	}

	return bHovered;
}

void ImageBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( IsOpen() )
	{
		const int iRenderX = GetX() + iX;
		const int iRenderY = GetY() + iY;
		int iRenderWidth = low( GetWidth(), iWidth );
		int iRenderHeight = low( GetHeight(), iHeight );

		if ( fScale != 1.0f )
		{
			iRenderWidth = GetWidth();
			iRenderHeight = GetWidth();
		}

		if ( psTexture )
			UI::ImageRender::Render( psTexture, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, dwColor, fScale );
		else
			UI::ImageRender::Render( pImage, iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY, dwColor, fScale );

		Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );

		if ( bHovered && !szToolTip.empty() )
		{
			TOOLTIP( iRenderX, iRenderY, szToolTip.c_str() );





			//if ( pTooltip )
			//{
			//	FONTHANDLER->Draw( 0, 200, 200,  "PAUSED" , DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 209, 158, 17, 255 ) );
			//
			//	//FONTHANDLER->Draw( 0, iRenderX + iRenderWidth, iRenderY,  "PAUSED" , DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 209, 158, 17, 255 ) );
			//
			//
			//	//pTooltip->SetText( "Hello" );
			//	//pTooltip->SetTextColor( UI::Color(255, 175, 230, 175) );
			//	//pTooltip->Render( iRenderX + iRenderWidth + 5, iRenderY, 0, 0, 0, 0 );
			//}
		}
	}
}
}