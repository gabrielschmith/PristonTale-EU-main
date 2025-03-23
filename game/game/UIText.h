#pragma once
#include "UIElement.h"

enum EAlign
{
	ALIGN_Left		= 0,	//Horizontal
	ALIGN_Center	= 1,	//Horizontal
	ALIGN_Right		= 2,	//Horizontal

	ALIGN_Top		= 0,	//Vertical
	ALIGN_Middle	= 1,	//Vertical
	ALIGN_Bottom	= 2,	//Vertical
};

enum class ETextPropertyID : short
{
	Unknown = 0,
	Elemental = 1,
};


namespace UI
{
typedef std::shared_ptr<class Text> Text_ptr;
class Text : public Element
{
public:
	//Constructor
											Text( class Rectangle2D rRectangle );
											Text( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor );
											Text( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor, BOOL bItalic );
											Text( const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor);
											Text( const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor, BOOL bItalic);
	virtual								   ~Text();

	void									Clear();

	//Render
	void									Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void									Update( float fTime );

	//Setters and Getters
	int										GetTextWidth();

	int										GetHeightText();

	void									SetFont( const char * pszFont, int iHeight, int iWidth, BOOL bItalic, BOOL bBold, DWORD dwColor );
	int										GetHeightFont();
	int										GetDrawnHeight() { return iDrawnHeight; };

	void									SetMaxTextWidth(int i) { iMaxTextWidth = i; };
	void									UpdateText();

	int										GetCountLines();
	std::string								& GetText() { return strText; };
	void									SetText( std::string str, bool _bUpdateText = true ) { strText = str; if ( _bUpdateText ) UpdateText(); };
	void									SetTextEx( char * pszFormat, ... );
	void									PrependText( std::string str ){ strText.insert(0,str); }
	void									AppendText( std::string str ) { strText.append(str); }

	void									SetEvent( int iEvent ) { iEventID = iEvent; };
	void									SetHoverEvent( int iEvent ) { iHoverEventID = iEvent; }
	void									SetSelectType( BOOL b ){ bSelectType = b; }

	void									SetDisabled( BOOL b ) { bDisable = b; };
	BOOL									GetDisabled() { return bDisable; };

	DWORD									GetColor() { return dwColorText; };
	void									SetColor( DWORD dwColor ) { dwColorText = dwColor; };

	void									SetDropdownColor( DWORD dwColor ){ dwColorTextShadow = dwColor; }
	void									SetOutlineColor( DWORD dwColor ){ dwColorTextOutline = dwColor; }
	void									SetDropdownShadowDistance( int iDistance ){ iShadowDistance = iDistance; }

	void									SetColorHover( DWORD dwColor ) { dwColorTextLink = dwColor; };
	void									SetDropdownShadow( BOOL b ) { bShadow = b; }

	void									SetMultiLine( BOOL b ) { bMultiLine = b; }
	BOOL									GetMultiLine() const { return bMultiLine; }

	void									SetWordWrap( BOOL b ) { bWordWrap = b; }
	BOOL				  					GetWordWrap() const { return bWordWrap; }

	void				  					SetNoClip( BOOL b ) { bNoClip = b; }
	BOOL				  					GetNoClip() const { return bNoClip; }

	void				  					SetHorizontalAlign( EAlign e ) { eHorizontalAlign = e; }
	EAlign				  					GetHorizontalAlign() const { return eHorizontalAlign; }

	void				  					SetVerticalAlign( EAlign e ) { eVerticalAlign = e; }
	EAlign				  					GetVerticalAlign() const { return eVerticalAlign; }

	void									SetPropertyID( ETextPropertyID id ) { eTextPropertyID = id; };
	ETextPropertyID							GetPropertyID( ) const { return eTextPropertyID; }

	void									SetPropertyValue( int value ) { iPropertyValue = value; };
	int										GetPropertyValue( ) const { return iPropertyValue; }

	BOOL									GetIsHighlightText() { return bHighlightText; }
	void									SetHighlightText( BOOL b ){ bHighlightText = b; }
	void									SetHighlightTextColor( DWORD dwColor ){ dwColorTextHighlight = dwColor; }

	int										GetHighlightTextWidthBuild() { return iHighlightTextWidth; }
	void									SetHighlightTextWidthBuild( int i ) { iHighlightTextWidth = i; }
	void									SetHighlightTextBuild( const std::string _strHighlightText ) { strHighlightText = _strHighlightText; }


	void									SetHoverText( const std::string _strHoverText );

	UINT									BuildFormat() const;

	//Event Handlers
	BOOL									OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	BOOL									IsMousePosInsideText( Point2D * psMousePosition, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );



	std::shared_ptr<class Tooltip>			pTooltipUnit;

private:
	std::string								strText;
	std::string								strHighlightText;
	std::string								strHoverText;
	int										iDrawnHeight    = 0; //incl word wrap
	int										iFontHandler	= -1;
	int										iMaxTextWidth	= -1;
	int										iHighlightTextWidth = -1;
	BOOL									bHover			= FALSE;
	BOOL									bMousePosInText	= FALSE;
	int										iHoverEventID	= -1;
	ETextPropertyID							eTextPropertyID	= ETextPropertyID::Unknown;
	int										iPropertyValue	= -1;
	BOOL									bDisable		= FALSE;
	BOOL									bSelect			= FALSE;
	BOOL									bSelectType		= FALSE;
	BOOL									bParentChild	= TRUE;
	std::shared_ptr<class Group>			pGroupMain		= NULL;
	enum EMouseEvent						eMouseEvent		= EMouseEvent::ClickDownL;
	DWORD									dwColorText		= -1;
	DWORD									dwColorTextLink	= 0;
	DWORD									dwColorTextHighlight = -1;
	DWORD									dwColorTextShadow = -1;
	DWORD									dwColorTextOutline = 0;
	int										iShadowDistance = 1;
	int										iEventID		= -1;
	BOOL									bShadow			= FALSE;
	BOOL									bHighlightText	= FALSE;

	BOOL									bMultiLine		= FALSE;
	BOOL									bWordWrap		= FALSE;
	BOOL									bNoClip			= TRUE;

	BOOL									bColorFade		= FALSE;
	float									fColorFadeTime = 1000.0f;

	EAlign									eHorizontalAlign = ALIGN_Left;
	EAlign									eVerticalAlign   = ALIGN_Top;

};
}