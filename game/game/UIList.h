#pragma once

#include "UIElement.h"
#include "UIScroll.h"

namespace UI
{
typedef std::shared_ptr<class List> List_ptr;
class List : public Element
{
public:
	//Constructor
												List();
												List( Rectangle2D rRect );
	virtual									   ~List();

	void										Clear();
	Scroll_ptr									MakeScrollBar( Rectangle2D sBox, std::string strScrollBarPath, std::string strScrollerPath, int iScrollerWidth = 8, int iScrollerHeight = 11 );

	//Getters and Setters
	int											GetCountItemsList() { return vWindows.size(); };

	int											GetListWidth();
	int											GetListHeight();
	int											GetPaddingSeparator(){return iPaddingSeparator;}
	void										SetPaddingSeparator( int i ) { iPaddingSeparator = i; }
	float										GetScrollPercentage() { return (pScrollBar ? pScrollBar->GetPercent() : 0.0f); };
	void										SetScrollPercentage(float f) { if (pScrollBar) { pScrollBar->SetPercent(f); } }

	void										SetCountAxisHidden( BOOL b ) { bCountAxisHidden = b; }
	void										SetForceDown( BOOL b );
	void										SetNoUpdatePosition( BOOL b ) { bNoUpdatePosition = b; }

	void										SetHorizontal( BOOL b ) { bHorizontalPadding = b; };

	void										SetNoClip( BOOL b ){ bNoClip = b; }

	void										AddWindow( Window_ptr pWindow );
	void										SetScrollBar( Scroll_ptr p ) { pScrollBar = p; }
	void										SetScrollFirst( BOOL b ) { bScrollFirst = b; }
	void										SetColorBackground( Color c ){ cColorBackground = c; }

	void										ResetAxis( );
	void										AddAxis( int iX, int iY );
	void										SetAxis( int iX, int iY );

	//Render
	void										Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void										Update( float fTime );

	//Events Handlers
	BOOL										OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL										OnMouseScroll( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL										OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL										OnKeyChar( class CKeyboard * pcKeyboard );
	BOOL										OnKeyPress( class CKeyboard * pcKeyboard );

	UI::Scroll_ptr								GetScrollBar() { return pScrollBar; }

	UI::Window_ptr								GetWindow( int iIDObject )
	{
		// Find object and return pointer to this
		for ( UINT u = 0; u < vWindows.size(); u++ )
		{
			if ( vWindows[u]->GetID() == iIDObject )
			{
				return vWindows[u];
			}
		}
		return NULL;
	};

	std::deque<UI::Window_ptr>					& GetWindowList() { return vWindows; }

protected:
	std::deque<UI::Window_ptr>					vWindows;

	Scroll_ptr									pScrollBar;

	BOOL										bHorizontalPadding = FALSE;

	int											iAddAxisX = 0;
	int											iAddAxisY = 0;
	int											iPaddingSeparator = 0;

	BOOL										bCountAxisHidden = FALSE;

private:
	int											iScroll;
	BOOL										bForceDown = FALSE;

	BOOL										bScrollFirst = FALSE;
	BOOL										bNoClip = FALSE;

	BOOL										bNoUpdatePosition = FALSE;

	Color										cColorBackground = 0;

	BOOL										IsForceDown() { return ((bForceDown == TRUE) && (bHorizontalPadding == FALSE)); }
};
}
