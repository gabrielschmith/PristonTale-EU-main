#pragma once

class CMessageBox
{
public:
	CMessageBox();
	virtual ~CMessageBox();

	enum
	{
		WINDOWID_Main,
		WINDOWID_RollDice,

		//Box Type
		BOXTYPEID_YesNo,
		BOXTYPEID_OkCancel,
		BOXTYPEID_Ok,
		BOXTYPEID_RollDice,

		//Buttons
		BUTTONID_Yes,
		BUTTONID_No,

		//Texts
		TextID_Title,
		TextID_Text,
		TEXTID_AutoClose,
	};

	void												Init();

	void												SetBoxType( int iType = 1 );


	void												SetEvent( std::function<void( UIEventArgs const & )> const & f );

	void												SetTitle( std::string strText );
	void												SetDescription( std::string strText );

	void												SetAutoClose( int iDuration );

	void												AddExtraText( std::string strText, Rectangle2D sBox, BOOL bBold, UI::Color cColor );
	void												AddExtraElement( Element_ptr p ) { if ( pWindow1 )pWindow1->AddElement( p ); vExtraElements.push_back( p ); }

	BOOL												GetType(){ return bType; }

	BOOL												IsOpen(){ if( pWindow1 )return pWindow1->IsOpen(); return FALSE; }

	void												OnResolutionChanged();

	void												Show(BOOL bBlockMouse = TRUE, BOOL bCanMove = FALSE);

	void												OnMouseMove( class CMouse * pcMouse );
	BOOL												OnMouseClick( class CMouse * pcMouse );

	BOOL												OnKeyPress( class CKeyboard * pcKeyboard );

	int													GetElapsedTime(){ return iElapsedTime; }

	void												Render();
	void												Update();

	void												ClearExtraElements(){ vExtraElements.clear(); }
protected:
	UI::Window_ptr										pWindow1	= NULL;

	UI::Window_ptr										pWindow1Main = NULL;
	UI::Window_ptr										pWindow1RollDice = NULL;
	int													iBoxType	= 1;
	BOOL												bType		= FALSE;
	BOOL												bUnblockMouseOnClose = FALSE;

	UI::Image										  * pAutoCloseBar[3];

	BOOL												bAutoClose	= FALSE;
	int													iElapsedTime = 0;
	DWORD												dwTickTime = 0;

	BOOL												bFirst = TRUE;

	std::vector<Element_ptr>							vExtraElements;

	void												OnYesButtonClick( UIEventArgs eArgs );
	void												OnNoButtonClick( UIEventArgs eArgs );
	void												OnRollButtonClick( UIEventArgs eArgs );
	void												OnPassButtonClick( UIEventArgs eArgs );
};

