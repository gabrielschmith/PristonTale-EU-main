#include "stdafx.h"
#include "MessageBox.h"

std::vector<std::function<void( UIEventArgs const & )>> vMsgBox;

CMessageBox::CMessageBox()
{
	pWindow1Main = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 230, 161 ) );
	pWindow1RollDice = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 161 ) );
}

CMessageBox::~CMessageBox()
{
}

void CMessageBox::SetEvent( std::function<void( UIEventArgs const & )> const & f )
{
	vMsgBox.clear();
	vMsgBox.push_back( f );
}

void CMessageBox::SetTitle( std::string strText )
{
	std::transform( strText.begin(), strText.end(), strText.begin(), ::toupper );

	if( pWindow1 )
		pWindow1->GetElement<UI::BitmapFont>( TextID_Title )->SetText( strText );
}

void CMessageBox::SetDescription( std::string strText )
{
	if( pWindow1 )
		pWindow1->GetElement<UI::Text>( TextID_Text )->SetText( strText );
}

void CMessageBox::SetAutoClose( int iDuration )
{
	if( !pWindow1 )
		return;

	iElapsedTime = iDuration;
	bAutoClose = TRUE;

	dwTickTime = TICKCOUNT + (iDuration * 1000);

	auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
	if( pTextAutoClose )
	{
		pTextAutoClose->SetTextEx( "Automatically closes in %d sec...", iElapsedTime );
		pTextAutoClose->Show();
	}
}

void CMessageBox::AddExtraText( std::string strText, Rectangle2D sBox, BOOL bBold, UI::Color cColor )
{
	UI::Text_ptr pText = std::make_shared<UI::Text>( strText, sBox, bBold, cColor.Get());
	pText->SetFont( "Arial", 14, 0, FALSE, bBold, cColor.Get());
	pText->SetWordWrap( TRUE );
	pText->SetMultiLine( TRUE );
	pText->SetHorizontalAlign( ALIGN_Center );
	pText->SetVerticalAlign( ALIGN_Middle );

	if( pWindow1 )
		pWindow1->AddElement( pText );

	vExtraElements.push_back( pText );
}

void CMessageBox::OnResolutionChanged()
{
	if( !pWindow1 )
		return;

	if ( !pWindow1->IsOpen() )
		return;

	int iBaseX = (RESOLUTION_WIDTH >> 1)  - (pWindow1->GetWidth() >> 1);
	int iBaseY = (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1);

	pWindow1->SetPosition( iBaseX, iBaseY );
}

void CMessageBox::Show(BOOL bBlockMouse, BOOL bCanMove)
{
	if( !pWindow1 )
		return;

	if ( !pWindow1->IsOpen() )
	{
		pWindow1->Show();

		//put at center
		if ( bFirst || !bCanMove )
		{
			//put at center

			int iBaseX = (RESOLUTION_WIDTH >> 1)  - (pWindow1->GetWidth() >> 1);
			int iBaseY = (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1);

			pWindow1->SetPosition( iBaseX, iBaseY );

			bFirst = FALSE;
		}

		if ( bCanMove )
		{
			pWindow1->SetCanMove( TRUE );
			pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth(), pWindow1->GetHeight() ) );
		}
		else
		{
			pWindow1->SetCanMove( FALSE );
		}

		if ( bBlockMouse )
			MOUSEHANDLER->BlockMouse();

		bUnblockMouseOnClose = bBlockMouse;
	}
}

void CMessageBox::OnYesButtonClick( UIEventArgs eArgs )
{
	if ( bUnblockMouseOnClose )
	{
		MOUSEHANDLER->UnblockMouse();
		bUnblockMouseOnClose = FALSE;
	}

	bType = TRUE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	bAutoClose = FALSE;
	iElapsedTime = 0;

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::OnNoButtonClick( UIEventArgs eArgs )
{
	if ( bUnblockMouseOnClose )
	{
		MOUSEHANDLER->UnblockMouse();
		bUnblockMouseOnClose = FALSE;
	}

	bType = FALSE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	bAutoClose = FALSE;
	iElapsedTime = 0;

	if ( pWindow1 )
	{
		for ( std::vector<Element_ptr>::iterator it = vExtraElements.begin(); it != vExtraElements.end(); it++ )
		{
			Element_ptr p = (*it);

			pWindow1->DelElement( p );
		}

		pWindow1->Hide();
	}

	vExtraElements.clear();
}

void CMessageBox::OnRollButtonClick( UIEventArgs eArgs )
{
	bType = TRUE;

	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_Yes )->Disable();
	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_No )->Disable();
}

void CMessageBox::OnPassButtonClick( UIEventArgs eArgs )
{
	if ( vMsgBox.size() > 0 )
		vMsgBox[0]( UIEventArgs{} );

	vMsgBox.clear();

	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_Yes )->Disable();
	pWindow1RollDice->GetElement<UI::Button>( BUTTONID_No )->Disable();
}

void CMessageBox::SetBoxType( int iType )
{
	iBoxType = iType;

	if ( pWindow1 )
	{
		auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
		if ( pTextAutoClose )
			pTextAutoClose->Hide();
	}

	if ( iBoxType == BOXTYPEID_YesNo )
	{
		pWindow1 = pWindow1Main;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnyes.png", "game\\images\\UI\\messagebox\\btnyes_.png" ) );
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnno.png", "game\\images\\UI\\messagebox\\btnno_.png" ) );

		pWindow1->GetElement<UI::Button>(BUTTONID_Yes)->Enable();
		pWindow1->GetElement<UI::Button>(BUTTONID_No)->Enable();
	}
	else if (iBoxType == BOXTYPEID_Ok)
	{
		pWindow1 = pWindow1Main;

		if (!pWindow1)
			return;

		pWindow1->GetElement<UI::Button>(BUTTONID_Yes)->Disable();
		pWindow1->GetElement<UI::Button>(BUTTONID_No)->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png"));
	}
	else if ( iBoxType == BOXTYPEID_OkCancel )
	{
		pWindow1 = pWindow1Main;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnok.png", "game\\images\\UI\\messagebox\\btnok_.png" ) );
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btncancel.png", "game\\images\\UI\\messagebox\\btncancel_.png" ) );
		pWindow1->GetElement<UI::Button>(BUTTONID_Yes)->Enable();
		pWindow1->GetElement<UI::Button>(BUTTONID_No)->Enable();
	}
	else if ( iBoxType == BOXTYPEID_RollDice )
	{
		pWindow1 = pWindow1RollDice;

		if ( !pWindow1 )
			return;

		pWindow1->GetElement<UI::Button>( BUTTONID_Yes )->Enable();
		pWindow1->GetElement<UI::Button>( BUTTONID_No )->Enable();
	}
}

void CMessageBox::Init()
{
	//Window Main
	{
		pWindow1Main->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_normal.png" ) );
		pWindow1Main->SetID( WINDOWID_Main );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 26, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnyes.png", "game\\images\\UI\\messagebox\\btnyes_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnYesButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1Main->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 115, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnno.png", "game\\images\\UI\\messagebox\\btnno_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnNoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1Main->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 35, 50, 160, 44 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Noto Sans", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Center );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1Main->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 10, 95, 210, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1Main->AddElement( pTextAutoClose );
		pWindow1Main->Hide();

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 12, 16, 207, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->SetHorizontalAlign( ALIGN_Center );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pWindow1Main->AddElement( pTextTitle );
	}

	//Window Roll Dice
	{
		pWindow1RollDice->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\background_diceroll.png" ) );
		pWindow1RollDice->SetID( WINDOWID_RollDice );

		UI::ImageBox_ptr pRollDiceIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 128, 12, 39, 30 ) );
		pRollDiceIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\diceroll\\icon.png" ) );
		pWindow1RollDice->AddElement( pRollDiceIcon );

		UI::Button_ptr pButtonYes = std::make_shared<UI::Button>( Rectangle2D( 109, 116, 88, 35 ) );
		pButtonYes->SetID( BUTTONID_Yes );
		pButtonYes->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnroll.png", "game\\images\\UI\\messagebox\\btnroll_.png" ) );
		pButtonYes->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnRollButtonClick, this, std::placeholders::_1 ) ) );
		pButtonYes->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\btnroll.png" ) );
		pWindow1RollDice->AddElement( pButtonYes );

		UI::Button_ptr pButtonNo = std::make_shared<UI::Button>( Rectangle2D( 198, 116, 88, 35 ) );
		pButtonNo->SetID( BUTTONID_No );
		pButtonNo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\btnpass.png", "game\\images\\UI\\messagebox\\btnpass_.png" ) );
		pButtonNo->SetEvent( UI::Event::Build( std::bind( &CMessageBox::OnPassButtonClick, this, std::placeholders::_1 ) ) );
		pButtonNo->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\btnpass.png" ) );
		pWindow1RollDice->AddElement( pButtonNo );

		//Text
		UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 118, 45, 160, 44 ) );
		pTextDescription->SetID( TextID_Text );
		pTextDescription->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 228, 218, 212 ) );
		pTextDescription->SetText( "" );
		pTextDescription->SetDropdownShadow( TRUE );
		pTextDescription->SetHorizontalAlign( ALIGN_Center );
		pTextDescription->SetMultiLine( TRUE );
		pTextDescription->SetDropdownColor( D3DCOLOR_ARGB( 100, 0, 0, 0 ) );
		pTextDescription->SetWordWrap( TRUE );
		pWindow1RollDice->AddElement( pTextDescription );

		UI::Text_ptr pTextAutoClose = std::make_shared<UI::Text>( Rectangle2D( 112, 100, 170, 60 ) );
		pTextAutoClose->SetID( TEXTID_AutoClose );
		pTextAutoClose->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_XRGB( 180, 180, 180 ) );
		pTextAutoClose->SetText( "" );
		pTextAutoClose->SetHorizontalAlign( ALIGN_Center );
		pWindow1RollDice->AddElement( pTextAutoClose );

		UI::Bitmapfont_ptr pTextTitle = std::make_shared<UI::BitmapFont>( Rectangle2D( 176, 22, 207, 0 ) );
		pTextTitle->SetID( TextID_Title );
		pTextTitle->ReadFontData( "game\\fonts\\messagebox.dat" );
		pTextTitle->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );
		pWindow1RollDice->AddElement( pTextTitle );
		pWindow1RollDice->Hide();

		pAutoCloseBar[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosebar.png" );
		pAutoCloseBar[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosebar_.png" );
		pAutoCloseBar[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\messagebox\\autoclosediv.png" );
	}
}

void CMessageBox::OnMouseMove( class CMouse * pcMouse )
{
	if( pWindow1 )
		pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CMessageBox::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow1 )
	{
		if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CMessageBox::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( !pWindow1 )
		return FALSE;

	if( iBoxType == BOXTYPEID_RollDice )
		return FALSE;

	if ( pWindow1->IsOpen() )
	{
		if( pcKeyboard->GetKey() == VK_RETURN )
		{
			bType = TRUE;
			if ( vMsgBox.size() > 0 )
				vMsgBox[0]( UIEventArgs{} );

			vMsgBox.clear();

			pWindow1->Hide();

			if ( bUnblockMouseOnClose )
			{
				MOUSEHANDLER->UnblockMouse();
				bUnblockMouseOnClose = FALSE;
			}

			return TRUE;
		}
		else if( pcKeyboard->GetKey() == VK_ESCAPE )
		{
			bType = FALSE;

			if ( vMsgBox.size() > 0 )
				vMsgBox[0]( UIEventArgs{} );

			vMsgBox.clear();

			pWindow1->Hide();

			if ( bUnblockMouseOnClose )
			{
				MOUSEHANDLER->UnblockMouse();
				bUnblockMouseOnClose = FALSE;
			}

			return TRUE;
		}
	}

	if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void CMessageBox::Render()
{
	if( !pWindow1 )
		return;

	if ( pWindow1->IsOpen() )
	{
		pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		int iBaseX = pWindow1->GetX();
		int iBaseY = pWindow1->GetY();

		if ( iBoxType == BOXTYPEID_RollDice )
		{
			if ( pAutoCloseBar[0] && pAutoCloseBar[1] && pAutoCloseBar[2] )
			{
				UI::ImageRender::Render( pAutoCloseBar[0], iBaseX + 122, iBaseY + 87, pAutoCloseBar[0]->GetWidth(), pAutoCloseBar[0]->GetHeight(), -1 );

				int iWidth = ((dwTickTime - TICKCOUNT) * pAutoCloseBar[1]->GetWidth()) / (MAX_ROLLDICEDROP_TIME * 1000);
				UI::ImageRender::Render( pAutoCloseBar[1], iBaseX + 123, iBaseY + 88, iWidth, pAutoCloseBar[1]->GetHeight(), iBaseX + 123, 0, -1 );

				int iAlpha = ((dwTickTime - TICKCOUNT) % 65) + 190;
				UI::ImageRender::Render( pAutoCloseBar[2], iBaseX + 122 + iWidth - (pAutoCloseBar[2]->GetWidth() >> 2), iBaseY + 86 - (pAutoCloseBar[2]->GetHeight() >> 2), pAutoCloseBar[2]->GetWidth(), pAutoCloseBar[2]->GetHeight(), D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );
			}
		}
	}
}

void CMessageBox::Update()
{
	if( !pWindow1 )
		return;

	if( pWindow1->IsOpen() && bAutoClose )
	{
		iElapsedTime--;

		auto pTextAutoClose = pWindow1->GetElement<UI::Text>( TEXTID_AutoClose );
		if( pTextAutoClose )
			pTextAutoClose->SetTextEx( "Automatically closes in %d sec...", iElapsedTime );

		if( iElapsedTime <= 0 )
			OnNoButtonClick( UIEventArgs{} );
	}
}
