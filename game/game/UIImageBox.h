#pragma once
#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class ImageBox> ImageBox_ptr;
class ImageBox : public Element
{
public:
	//Constructor
											ImageBox( class Rectangle2D rRectangle );
	virtual								   ~ImageBox();

	void									Clear();

	//Setters and Getters
	void									SetImage( class Image * pImage );
	void									SetToolTip( const std::string szMessage );
	void									SetDebug( BOOL bEnable ) { bDebug = bEnable; };
	void									SetActiveImage( std::pair<class Image *, class Image *> pPair );
	void									SetGroup( std::shared_ptr<class Group> pGroupMain );
	void									SetColor( DWORD _dwColor ){ dwColor = _dwColor; }
	void									Swap();
	void									Swap( BOOL bActiveImage );
	void									SetSourceBox( Rectangle2D sBox ){ sSourceBox = sBox; }
	void									SetTexture( LPDIRECT3DTEXTURE9 ps ){ psTexture = ps; }
	void									SetScale( float f ){ fScale = f; }
	void									SetEvent( int iEvent ) { iEventID = iEvent; };

	Image								  * GetImage() { return pImage; }
	Image								  * GetImageActive1() { return pImage; }
	Image								  * GetImageActive2() { return pImage; }

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

private:
	class Image								* pImage		= NULL;
	class Image								* pImageActive1	= NULL;
	class Image								* pImageActive2	= NULL;

	std::string								szToolTip		= "";

	std::shared_ptr<class Group>			pGroup			= NULL;

	BOOL									bHovered = FALSE;
	BOOL									bDebug = FALSE;

	DWORD									dwColor = -1;
	int										iEventID = -1;

	LPDIRECT3DTEXTURE9						psTexture = NULL;

	float									fScale = 1.0f;

	Rectangle2D								sSourceBox;
};
}