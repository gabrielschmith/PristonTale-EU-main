#include "stdafx.h"
#include "CChatBox.h"
#include <regex>

CChatBox::CChatBox()
{
}

CChatBox::~CChatBox()
{
}

void CChatBox::Init()
{
	BuildWindow();

	//Initialize Friendly Commands
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/sweat", "e1" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/love", "e2" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/angry", "e3" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/bigeye", "e4" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/sad", "e5" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/cry", "e6" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/ops", "e7" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/nerd", "e8" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/craving", "e9" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/hurt", "e10" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/silence", "e11" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/crazy", "e12" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/hello", "e13" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/tired", "e14" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/shame", "e15" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/brokeheart", "e16" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/abused", "e17" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/kappa", "e18" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/kappapride", "e19" ) );
	mFriendlyCommands.insert( FriendlyCommands::value_type( "/pogchamp", "e20" ) );

	for ( int i = 0; i < 7; i++ )
		saItemLink[i].iID = LISTID_ChatAll + i;

	ZeroMemory( &cItemLinkDataWrapper, sizeof( ItemData ) );
}

void CChatBox::Render()
{
	pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	/*auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	auto pButtonSendMessage = pWindow1->GetElement<UI::Button>( BUTTONID_SendMessage );
	BOOL bActive = FALSE;

	if( pWindow1->IsHover() )
		bActive = TRUE;
	if( pInputField->IsSelected() )
		bActive = TRUE;

	for( size_t i = 0; i < 6; i++ )
	{
		auto pButtonTabs = pWindow1->GetElement<UI::Button>( BUTTONID_TabAll + i );
		if( pButtonTabs )
			bActive ? pButtonTabs->Show() : pButtonTabs->Hide();
	}

	if( bActive )
		pWindow1->SetColorBackground( UI::Color( 70, 0, 0, 0 ) );
	else
		pWindow1->SetColorBackground( UI::Color( 0, 0, 0, 0 ) );

	if( pInputField )
		bActive ? pInputField->Show() : pInputField->Hide();

	if( pButtonSendMessage )
		bActive ? pButtonSendMessage->Show() : pButtonSendMessage->Hide();*/

	if ( cItemLinkDataWrapper.bValid )
		ITEMHANDLER->ShowItemInfo( &cItemLinkDataWrapper, FALSE );


	CheckCollideWindow( FALSE );
}

ItemData* CChatBox::GetMouseItemHover()
{
	if (pWindow1)
	{
		if (pWindow1->IsOpen() == FALSE)
			return NULL;

		if (cItemLinkDataWrapper.bValid)
			return &cItemLinkDataWrapper;
	}

	return NULL;
}

void CChatBox::AddDebugMessage ( std::string strMessage )
{
#ifdef DEV_MODE
	AddMessage ( FormatString ( "DEBUG> %s", strMessage.c_str () ), EChatColor::CHATCOLOR_Normal );
#endif
}

void CChatBox::AddDebugMessage( const char * format, ... )
{
#ifdef DEV_MODE
	char szBuf[ 256 ];
	ZeroMemory( szBuf, 256 );
	va_list ap;
	va_start( ap, format );
	_vsnprintf_s( szBuf, 256, 255, format, ap );
	va_end( ap );

	AddMessage( FormatString( "DEBUG > %s", szBuf ), EChatColor::CHATCOLOR_Normal );

#endif
}

void CChatBox::AddMessage( std::string strMessage, EChatColor eChatColor, ItemData * pcItemData, BOOL bSkipDuplicate )
{
	//avoid having to write duplicate message twice in a row.
	if ( bSkipDuplicate && lastMessage == strMessage )
		return;

	lastMessage = strMessage;

	if( !pWindow1 )
		return;

	std::vector<UI::List_ptr> vListToAdd;

	if( eChatColor != CHATCOLOR_Notice )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>(LISTID_ChatAll) );
	else if( eChatColor == CHATCOLOR_Notice )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatNotice ) );

	if( eChatColor == CHATCOLOR_Party )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>(LISTID_ChatParty) );
	else if( eChatColor == CHATCOLOR_Trade )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatTrade ) );
	else if( eChatColor == CHATCOLOR_Clan || eChatColor == CHATCOLOR_ClanLeader )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatClan ) );
	else if( eChatColor == CHATCOLOR_Whisper || eChatColor == CHATCOLOR_Blue || eChatColor == CHATCOLOR_Error )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatPM ) );
	else if( eChatColor == CHATCOLOR_Raid )
		vListToAdd.push_back( pWindow1->GetElement<UI::List>( LISTID_ChatRaid ) );

	if( vListToAdd.size() > 0 )
	{
		for( auto &v : vListToAdd )
		{
			if( v )
			{
				UI::Window_ptr pBaseWindow = std::make_shared <UI::Window>( Rectangle2D( 5, 0, 275, 0 ) );
				UI::Text_ptr pTextMessage = std::make_shared<UI::Text>( Rectangle2D( 0, 0, 0, 0 ) );

				//Add Item Link
				ItemData * pcItemLinkWrapper = new ItemData();
				if ( pcItemData )
					CopyMemory( pcItemLinkWrapper, pcItemData, sizeof( ItemData ) );
				else
					ZeroMemory( pcItemLinkWrapper, sizeof( ItemData ) );

				for ( int i = 0; i < MAX_LISTCHAT; i++ )
				{
					if ( saItemLink[i].iID == v->GetID() )
					{
						saItemLink[i].vItemData.push_back( pcItemLinkWrapper );
						break;
					}
				}

				if( eChatColor == CHATCOLOR_Normal && strMessage.find( ":" ) != std::string::npos )
				{
					pTextMessage->SetHighlightText( TRUE );
					pTextMessage->SetHighlightTextColor( D3DCOLOR_XRGB( 100, 255, 50 ) );
				}

				pTextMessage->SetFont( "Noto Sans", 16, 0, FALSE, FALSE, -1 );
				pTextMessage->SetColor( GetMessageColorFromType( eChatColor ) );

				if ( pcItemData )
				{
					pTextMessage->SetColor( D3DCOLOR_ARGB( 255, 240, 240, 0 ) );
					pBaseWindow->SetHoverEvent( UI::Event::Build( std::bind( &CChatBox::OnItemLinkHover, this, pcItemLinkWrapper, pBaseWindow, std::placeholders::_1 ) ) );

					if ( eChatColor != CHATCOLOR_Normal )
					{
						pTextMessage->SetHighlightText( TRUE );
						pTextMessage->SetHighlightTextColor( GetMessageColorFromType( eChatColor ) );
					}
				}

				pTextMessage->SetDropdownShadow( TRUE );
				pTextMessage->SetDropdownColor( UI::Color( 150, 0, 0, 0 ).Get() );
				pTextMessage->SetMultiLine( TRUE );
				pTextMessage->SetMaxTextWidth( 275 );
				pTextMessage->SetText( strMessage );
				pTextMessage->SetNoClip( TRUE );
				pBaseWindow->SetBox( Rectangle2D( 5, 0, 275, pTextMessage->GetHeightText() ) );

				//Emote
				{
					std::string strReplaceEmote = "";
					int iEmoteIndex = -1;
					if ( IsEmoteMessage( strMessage, strReplaceEmote, iEmoteIndex, eChatColor ) )
					{
						std::string strNewMessage = strMessage;
						strNewMessage.erase( strNewMessage.find( strReplaceEmote ), strReplaceEmote.length() );

						pTextMessage->SetText( strNewMessage );
						pBaseWindow->SetBox( Rectangle2D( 5, 0, 275, pTextMessage->GetHeightText() ) );

						UI::ImageBox_ptr pEmoteIcon = std::make_shared<UI::ImageBox>( Rectangle2D( pTextMessage->GetTextWidth(), 0, 103, 103 ) );
						pEmoteIcon->SetImage( UI::ImageLoader::LoadImage( "game\\textures\\particles\\emote%d.png", iEmoteIndex ) );
						pEmoteIcon->SetScale( 0.16f );
						pBaseWindow->AddElement( pEmoteIcon );
					}
				}

				pBaseWindow->AddElement( pTextMessage );

				v->AddWindow( pBaseWindow );

				//Analyze
				if ( eChatColor == CHATCOLOR_Blue )
				{
					AddRecentPlayer( strMessage );
				}

				//Max list count?
				if ( v->GetCountItemsList() > MAX_CHATTEXTLIST )
				{
					//Get Window
					auto pTextWindow = v->GetWindowList()[0];

					//Old Position
					int iHeight = pTextWindow->GetHeight();

					//Clear and delete it
					CLEARPTR( pTextWindow );
					RESETPTR( pTextWindow );

					//Clear window in the vector
					v->GetWindowList().pop_front();

					for ( int i = 0; i < MAX_LISTCHAT; i++ )
					{
						if ( saItemLink[i].iID == v->GetID() )
						{
							if ( saItemLink[i].vItemData.size() > 0 )
							{
								ItemData * pc = saItemLink[i].vItemData[0];
								if ( (cItemLinkDataWrapper.sItem.iChk1 == pc->sItem.iChk1) && (cItemLinkDataWrapper.sItem.iChk2 == pc->sItem.iChk2) )
									cItemLinkDataWrapper.bValid = FALSE;

								SAFE_DELETE( saItemLink[i].vItemData[0] );
								saItemLink[i].vItemData.pop_front();
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CChatBox::SendMessageChat( std::string strMessage )
{
	if( strMessage.size() > 0 )
	{
		BOOL bSendToLoginServer = FALSE;

		std::string strEmote = strMessage;
		BOOL bEmote = OnEmoteCommand( strEmote );

		if ( iTabID == CHATBOXTABID_All )
		{
			if ( bEmote )
			{
				AddMessage( FormatString( "%s: %s", UNITDATA->sCharacterData.szName, strMessage.c_str() ), CHATCOLOR_Normal );
				CHATGAMEHANDLER->SendChat( FALSE, strEmote.c_str() );
				return;
			}
		}

		int iRetChat = CHATGAMEHANDLER->AnalyzeChat( (char*)strMessage.c_str() );

		BOOL bContinue = TRUE;

		if( strMessage[0] == '/' || strMessage[0] == '@' || strMessage[0] == '#' || strMessage[0] == '$' )
		{
			bContinue = FALSE;

			if ( bEmote )
				bContinue = TRUE;

			if ( strMessage[0] == '/' && (strMessage[1] == ';' || strMessage[1] == ':') )
			{
				strMessage = trim( strMessage );
				bSendToLoginServer = TRUE;
			}
			else if ( strMessage.size() > 1 && strMessage[1] == '!' )
				bSendToLoginServer = TRUE;
			else if ( (strMessage.compare( 0, 7, "/TRADE>" ) == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2) )
			{
				if ( strMessage[0] == '$' )
				{
					strMessage.erase( 0, 1 );
					strMessage.insert( 0, "/TRADE>" );
				}

				bSendToLoginServer = TRUE;
			}
		}

		if( bContinue )
		{
			switch( iTabID )
			{
				case CHATBOXTABID_All:
				case CHATBOXTABID_PM:
					if( iRetChat == 0 )
						break;

					if( true )
					{
						int iLastMessageTick = (TICKCOUNT - dwLastMessageTick);
						if( dwLastMessageTick && iLastMessageTick < 2000 )
						{
							iTotalMessages++;

							if( iTotalMessages > 10 )
							{
								if( iLastMessageTick > 0 )
								{
									lstrcpy( UNITDATA->szMessage, FormatString( "%s: %s", UNITDATA->sCharacterData.szName, "My tongue froze because I talk too much -_-" ) );
									UNITDATA->dwMessageEndTime = TICKCOUNT + 1000 * 7;
									dwLastMessageTick = TICKCOUNT + 1000 * 7;

									OnEmoteCommand( std::string("e6"), UNITDATA );
								}

								return;
							}
						}
						else if( iLastMessageTick > 2000 )
							iTotalMessages = 0;

						dwLastMessageTick = TICKCOUNT;
					}

					strMessage = FormatString( "%s: %s", UNITDATA->sCharacterData.szName, strMessage.c_str() );
					STRINGCOPY( UNITDATA->szMessage, strMessage.c_str() );
					UNITDATA->dwMessageEndTime = TICKCOUNT + 1000 * 7;
					AddMessage( strMessage, CHATCOLOR_Normal );
					break;
				case CHATBOXTABID_Clan:
					strMessage = FormatString( "/CLAN> %s", strMessage.c_str() );
					break;
				case CHATBOXTABID_Party:
					strMessage = FormatString( "@%s", strMessage.c_str() );
					break;
				case CHATBOXTABID_Raid:
					strMessage = FormatString( "#%s", strMessage.c_str() );
					break;
				case CHATBOXTABID_Trade:
					strMessage = FormatString( "/TRADE>%s", strMessage.c_str() );
					bSendToLoginServer = TRUE;
					break;
			}
		}

		if( iRetChat == 1 )
		{
			CHATGAMEHANDLER->SendChat( bSendToLoginServer, strMessage.c_str() );
		}
	}
}

void CChatBox::OnEnableChatBox()
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );

	if( pInputField )
	{
		pInputField->SetSelected( TRUE );
		pInputField->SetSelect( TRUE );
		pInputField->ClearInput();

		bFirstDown = TRUE;
		uSaveCommandWheel = vSaveCommand.size() - 1;

		pWindow1->SetColorBackground(UI::Color(160, 0, 0, 0));
	}
}

void CChatBox::OnEnterKeyPressed( UIEventArgs eArgs )
{
	bFirstDown = TRUE;

	auto pInputField		= pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );

	if( pInputField )
	{
		if( pInputField->GetText().length() > 0 )
		{
			AddSaveCommand( pInputField->GetText() );
			SendMessageChat( pInputField->GetText() );

			//Private message handle
			if ( (pInputField->GetText().length() >= 3) &&
				 (pInputField->GetText()[0] == '/') &&
				 ((pInputField->GetText()[1] == ':') || (pInputField->GetText()[1] == ';')) )
			{
				std::string raw = trim(pInputField->GetText().substr(2, std::string::npos));
				size_t index = raw.find(' ');
				if ( index != std::string::npos )
				{
					const std::string name = raw.substr(0, index);

					std::string builder = "/";
					builder += pInputField->GetText()[1];
					builder += name;
					builder += " ";

					//Add it back
					pInputField->ClearInput();
					pInputField->AddInput(builder);

					return;
				}
			}

			if( (pInputField->GetText().length() > 7) && (pInputField->GetText()[0] == '/' && pInputField->GetText()[5] == '>') && pInputField->GetText().find( "/CLAN>" ) != std::string::npos )
			{
				pInputField->ClearInput();
				pInputField->AddInput( "/CLAN> " );
				return;
			}

			pInputField->ClearInput();

			if ( SETTINGSHANDLER->GetModel()->GetSettings().bChatBoxRemoveFocusOnEnter )
			{
				pInputField->SetSelected( FALSE );
			}
		}
		else
		{
			pInputField->ClearInput();
			pInputField->SetSelected( FALSE );
		}
	}
}

void CChatBox::OnDiscordButton( UIEventArgs eArgs )
{
	if( MESSAGEBOX->GetType() )
		ShellExecute( 0, 0, "https://discord.gg/tvAT567", 0, 0, SW_SHOW );
}

void CChatBox::OnDiscordButtonClick( UIEventArgs eArgs )
{
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_OkCancel );
	MESSAGEBOX->SetTitle( "Discord" );
	MESSAGEBOX->SetDescription( "Do you want to visit the Pristontale EU Discord server?" );
	MESSAGEBOX->SetEvent( std::bind( &CChatBox::OnDiscordButton, this, std::placeholders::_1 ) );
	MESSAGEBOX->Show();
}

BOOL CChatBox::IsInputFieldActive()
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
		return pInputField->IsSelected();

	return FALSE;
}

void CChatBox::SetInputFieldText( std::string strText )
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
		pInputField->AddInput( strText );
}

void CChatBox::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

	if (pWindow1->IsHover() || IsInputFieldActive())
		pWindow1->SetColorBackground(UI::Color(170, 0, 0, 0));
	else
		pWindow1->SetColorBackground(UI::Color(100, 0, 0, 0));
}

BOOL CChatBox::OnMouseClick( class CMouse * pcMouse )
{
	auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
	if( pInputField )
	{
		if( !pWindow1->IsHover() && pInputField->IsSelected() && !pInputField->GetText().length() )
		{
			pInputField->SetSelected( FALSE );
			pInputField->SetSelect( FALSE );
		}
	}

	//Ctrl + Click
	if ( (pcMouse->GetEvent() == EMouseEvent::ClickDownL) && KEYBOARDHANDLER->IsKeyDown( VK_CONTROL ) && pWindow1->IsOpen() )
	{
		if ( TICKCOUNT > dwDelayItemLinkTime )
		{
			SendItemLink( ITEMHANDLER->GetMouseItemHover() );
			dwDelayItemLinkTime = TICKCOUNT + 2000;
			pInputField->ClearInput();
			return TRUE;
		}
		else
		{
			return TRUE; //to prevent player taking item out of their slot
		}
	}

	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if( READDWORD( 0x362B1E0 ) )
		return FALSE;

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_ESCAPE && !pcKeyboard->IsKeyDown(VK_SHIFT) && pWindow1->IsOpen() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if( pInputField )
		{
			pInputField->SetSelected( FALSE );
			pInputField->ClearInput();
		}

		pWindow1->Hide();
		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !pWindow1->IsOpen() && !MESSAGEBOX->IsOpen() )
	{
		OnEnableChatBox();
		pWindow1->Show();
		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !IsInputFieldActive() && !MESSAGEBOX->IsOpen() )
	{
		OnEnableChatBox();
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_TAB && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( "/CLAN> " );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_UP && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( GetSaveCommand( TRUE ) );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_DOWN && IsInputFieldActive() )
	{
		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			pInputField->ClearInput();
			pInputField->AddInput( GetSaveCommand( FALSE ) );
		}
		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_PRIOR && IsInputFieldActive() )
	{
		for ( size_t i = 0; i < 6; i++ )
		{
			auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
			if ( pListChat )
			{
				if ( pListChat->IsOpen() )
				{
					auto pScroll = pListChat->GetScrollBar();
					if ( pScroll )
					{
						float fPercent = pScroll->GetPercent() - 5.0f;
						if ( fPercent < 0.0f )
							fPercent = 0.0f;

						pScroll->SetPercent( fPercent );
					}
					break;
				}
			}
		}

		return TRUE;
	}

	if ( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_NEXT && IsInputFieldActive() )
	{
		for ( size_t i = 0; i < 6; i++ )
		{
			auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
			if ( pListChat )
			{
				if ( pListChat->IsOpen() )
				{
					auto pScroll = pListChat->GetScrollBar();
					if ( pScroll )
					{
						float fPercent = pScroll->GetPercent() + 5.0f;
						if ( fPercent > 100.0f )
							fPercent = 100.0f;

						pScroll->SetPercent( fPercent );
					}
					break;
				}
			}
		}

		return TRUE;
	}

	if( pcKeyboard->GetEvent() == KeyDown && pcKeyboard->IsKeyDown(VK_CONTROL) && (pcKeyboard->GetKey() >= VK_F1 && pcKeyboard->GetKey() <= VK_F10) )
	{
		char * pszMacroText = SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[(pcKeyboard->GetKey() & 0x0F)];

		if( pszMacroText[0] != NULL )
		{
			OnEnableChatBox();
			auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
			if( pInputField )
				pInputField->AddInput( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[(pcKeyboard->GetKey() & 0x0F)] );

			pWindow1->Show();
			return TRUE;
		}
	}

	if( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnMouseScroll( CMouse * pcMouse )
{
	if( pWindow1->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL CChatBox::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void CChatBox::ToggleNotice( BOOL bOpen )
{
	int iY = 0;
	int iHeight = 0;

	auto pImageBoxChatBar = pWindow1->GetElement<UI::ImageBox>( IMAGEID_ChatBoxBar );
	auto pListChatNotice = pWindow1->GetElement<UI::List>( LISTID_ChatNotice );
	if( !pImageBoxChatBar || !pListChatNotice )
		return;

	if( bOpen )
	{
		iY = 74;
		iHeight = 100;
		pImageBoxChatBar->Show();
		pListChatNotice->Show();
	}
	else
	{
		iY = 8;
		iHeight = 166;
		pImageBoxChatBar->Hide();
		pListChatNotice->Hide();
	}

	for( size_t i = 0; i < 6; i++ )
	{
		auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
		if( pListChat )
		{
			pListChat->SetBox( Rectangle2D( pListChat->GetX(), iY, pListChat->GetWidth(), iHeight ) );
			pListChat->GetScrollBar()->SetHeight( iHeight + 5 );
			pListChat->GetScrollBar()->SetPercent( 100 );
		}
	}
}

void CChatBox::HandlePacket( PacketChatBoxMessage * psPacket )
{
	UnitData * pcUnitData = NULL;
	BOOL bEmote = FALSE;

	int msgLen = sizeof( PacketChatBoxMessage ) - 0x8 + 0x4 + 0x4;

	char szTemp[256];
	memset(&szTemp[0], 0, sizeof(szTemp));
	STRINGCOPYLEN( szTemp, msgLen, psPacket->szChatBoxMessage );

	if( psPacket->lID )
		pcUnitData = UNITDATABYID( psPacket->lID );

	if ( pcUnitData )
		bEmote = OnEmoteCommand( std::string( szTemp ), pcUnitData );

	if ( ((pcUnitData && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->PkMode_CharState == CHARACTERTYPE_Player)) || psPacket->lID == 0) )
		AddMessage( szTemp, psPacket->lID ? CHATCOLOR_Normal : psPacket->iChatColor );

	if ( pcUnitData && (psPacket->iChatColor != CHATCOLOR_Party) )
	{
		if ( bEmote == FALSE )
		{
			STRINGCOPY( pcUnitData->szMessage, szTemp );
			pcUnitData->dwMessageEndTime = TICKCOUNT + 5000;
		}
	}
}

void CChatBox::HandlePacket( PacketItemLinkChat * psPacket )
{
	AddMessage( psPacket->szMessage, (EChatColor)psPacket->eChatColor, &psPacket->cItemData );
}

void CChatBox::HandlePacket( PacketPlayerTitleList * psPacket )
{
	//@todo - have NPC to display title list instead

	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		PlayerTitle * title = (PlayerTitle* )( psPacket->baData + ( i * sizeof ( PlayerTitle ) ) );
		AddMessage ( FormatString ( "Title> '%s' (id = %d)", title->szTitle, title->sTitleID), EChatColor::CHATCOLOR_Normal );

	}

	AddMessage ( "Use /title_set <id> to set a title", EChatColor::CHATCOLOR_White );
	AddMessage ( "Use /title_clear to clear title", EChatColor::CHATCOLOR_White );
}

void CChatBox::ProcessPacket( PacketItemLinkChat * psPacket, BOOL bLoginServer )
{
	psPacket->iLength = sizeof( PacketItemLinkChat );
	psPacket->iHeader = PKTHDR_ChatItemLink;

	if ( bLoginServer )
		SENDPACKETL( psPacket );
	else
		SENDPACKETG( psPacket );
}

DWORD CChatBox::GetMessageColorFromType( EChatColor eType )
{
	switch( eType )
	{
		case CHATCOLOR_Error:
			return D3DCOLOR_ARGB( 255, 255, 128, 96 );	//Red
		case CHATCOLOR_Whisper:
			return D3DCOLOR_ARGB( 255, 150, 190, 255 );	//Blue
		case CHATCOLOR_Blue:
			return D3DCOLOR_ARGB( 255, 150, 190, 255 );	//Blue
		case CHATCOLOR_Global:
			return D3DCOLOR_ARGB( 255, 230, 160, 255 );	//Purple
		case CHATCOLOR_Party:
			return D3DCOLOR_ARGB( 255, 200, 255, 0 );	//Green
		case CHATCOLOR_Clan:
			return D3DCOLOR_ARGB( 255, 255, 255, 80 );	//Yellow (clan)
		case CHATCOLOR_Trade:
			return D3DCOLOR_ARGB( 255, 200, 255, 255 );	//Whitish Cyan (trade)
		case CHATCOLOR_Notice:
			return D3DCOLOR_ARGB( 255, 200, 128, 128 );	//Purple
		case CHATCOLOR_ClanLeader:
			return D3DCOLOR_ARGB( 255, 255, 180, 70 );	//Orange (clan leader)
		case CHATCOLOR_Normal:
			return D3DCOLOR_ARGB( 255, 255, 255, 128 );	//Yellow (normal chat)
		case CHATCOLOR_Raid:
			return D3DCOLOR_ARGB( 255, 40, 240, 230 );	//Cyan
		case CHATCOLOR_GameMaster:
			return D3DCOLOR_XRGB( 255, 255, 255);		//White
		case CHATCOLOR_White:
			return D3DCOLOR_XRGB( 255, 255, 255);		//White
	}

	return D3DCOLOR_XRGB( 255, 255, 255 );
}

void CChatBox::OnSelectChatBoxTab( int iID )
{
	for ( size_t i = 0; i < 6; i++ )
	{
		auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + i );
		if ( pListChat )
		{
			pListChat->Hide();

			auto pScroll = pListChat->GetScrollBar();
			if ( pScroll )
				pScroll->SetPercent( 100 );
		}
	}

	auto pListChat = pWindow1->GetElement<UI::List>( LISTID_ChatAll + (iID - BUTTONID_TabAll) );
	if( pListChat )
	{
		iTabID = iID - BUTTONID_TabAll;
		pListChat->Show();
	}
}

void CChatBox::BuildWindow()
{
	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 295, 218 ) );
	pWindow1->SetPosition( 4, RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

	pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth() - 15, pWindow1->GetHeight() - 40 ) );
	pWindow1->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );


	pWindow1->SetColorBackground(UI::Color(100, 0, 0, 0));


	UI::ImageBox_ptr pImageChatBoxBar = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 66, 295, 3 ) );
	pImageChatBoxBar->SetID( IMAGEID_ChatBoxBar );
	pImageChatBoxBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\chatbox\\chatbar.png" ) );
	pWindow1->AddElement( pImageChatBoxBar );

	for( size_t i = 0; i < 6; i++ )
	{
		UI::List_ptr pListChat = std::make_shared<UI::List>( Rectangle2D( 0, 74, 280, 100 ) );
		pListChat->SetID( LISTID_ChatAll + i );
		pListChat->MakeScrollBar( Rectangle2D( 285, 0, 6, 105 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
		pListChat->SetForceDown( TRUE );
		pListChat->SetScrollFirst( TRUE );

		if ( pListChat->GetID() != LISTID_ChatAll )
			pListChat->Hide();

		iTabID = CHATBOXTABID_All;

		pWindow1->AddElement( pListChat );
	}

	UI::List_ptr pListChatNotice = std::make_shared<UI::List>( Rectangle2D( 0, 2, 280, 62 ) );
	pListChatNotice->SetID( LISTID_ChatNotice );
	pListChatNotice->MakeScrollBar( Rectangle2D( 285, 2, 6, 54 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pListChatNotice->SetForceDown( TRUE );
	pWindow1->AddElement( pListChatNotice );

	UI::Group_ptr p = std::make_shared<UI::Group>();
	pWindow1->AddElement( p );

	//Chat Tabs
	{
		UI::Group_ptr pGroupTabs = std::make_shared<UI::Group>();
		pWindow1->AddElement( pGroupTabs );

		int iBaseX = 3;
		int iBaseY = 177;

		UI::Button_ptr pButtonTabAll = std::make_shared<UI::Button>( Rectangle2D( iBaseX, iBaseY, 43, 15 ) );
		pButtonTabAll->SetGroup( pGroupTabs );
		pButtonTabAll->SetID( BUTTONID_TabAll );
		pButtonTabAll->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\all.png", "game\\images\\UI\\hud\\chatbox\\all_.png" ) );
		pButtonTabAll->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabAll->GetID() ) ));
		pButtonTabAll->SetSelect( TRUE );
		pButtonTabAll->SetSelected( TRUE );
		pWindow1->AddElement( pButtonTabAll );

		UI::Button_ptr pButtonTabClan = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 45, iBaseY, 43, 15 ) );
		pButtonTabClan->SetGroup( pGroupTabs );
		pButtonTabClan->SetID( BUTTONID_TabClan );
		pButtonTabClan->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\clan.png", "game\\images\\UI\\hud\\chatbox\\clan_.png" ) );
		pButtonTabClan->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabClan->GetID() ) ) );
		pButtonTabClan->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabClan );

		UI::Button_ptr pButtonTabParty = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 90, iBaseY, 43, 15 ) );
		pButtonTabParty->SetGroup( pGroupTabs );
		pButtonTabParty->SetID( BUTTONID_TabParty );
		pButtonTabParty->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\party.png", "game\\images\\UI\\hud\\chatbox\\party_.png" ) );
		pButtonTabParty->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabParty->GetID() ) ) );
		pButtonTabParty->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabParty );

		UI::Button_ptr pButtonTabRaid = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 135, iBaseY, 43, 15 ) );
		pButtonTabRaid->SetGroup( pGroupTabs );
		pButtonTabRaid->SetID( BUTTONID_TabRaid );
		pButtonTabRaid->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\raid.png", "game\\images\\UI\\hud\\chatbox\\raid_.png" ) );
		pButtonTabRaid->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabRaid->GetID() ) ) );
		pButtonTabRaid->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabRaid );

		UI::Button_ptr pButtonTabTrade = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 180, iBaseY, 43, 15 ) );
		pButtonTabTrade->SetGroup( pGroupTabs );
		pButtonTabTrade->SetID( BUTTONID_TabTrade );
		pButtonTabTrade->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\trade.png", "game\\images\\UI\\hud\\chatbox\\trade_.png" ) );
		pButtonTabTrade->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabTrade->GetID() ) ) );
		pButtonTabTrade->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabTrade );

		UI::Button_ptr pButtonTabPM = std::make_shared<UI::Button>( Rectangle2D( iBaseX + 225, iBaseY, 43, 15 ) );
		pButtonTabPM->SetGroup( pGroupTabs );
		pButtonTabPM->SetID( BUTTONID_TabPM );
		pButtonTabPM->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\pm.png", "game\\images\\UI\\hud\\chatbox\\pm_.png" ) );
		pButtonTabPM->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnSelectChatBoxTab, this, pButtonTabPM->GetID() ) ) );
		pButtonTabPM->SetSelect( TRUE );
		pWindow1->AddElement( pButtonTabPM );
	}

	int iMaxLen = MAX_CHATLENGTH - strlen(UNITDATA->sCharacterData.szName) - 3;

	UI::InputField_ptr pInputMessage = std::make_shared<UI::InputField>( Rectangle2D( 25, 195, 245, 23 ) );
	pInputMessage->SetID( INPUTFIELDID_Message );
	pInputMessage->SetGroup( p );
	pInputMessage->SetFont( "Noto Sans", 16, 0, FALSE, FALSE, -1 );
	pInputMessage->SetText( "" );
	pInputMessage->SetSelect( TRUE );
	pInputMessage->SetMaxLength( iMaxLen );
	pInputMessage->SetTextAlign( EAlign::ALIGN_Left, EAlign::ALIGN_Middle );
	pInputMessage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\chatbox\\input.png" ) );
	pInputMessage->BindActionKeyboard( UI::ActionKeyboard( VK_RETURN, UI::Event::Build( std::bind( &CChatBox::OnEnterKeyPressed, this, std::placeholders::_1 ) ) ) );
	pWindow1->AddElement( pInputMessage );

	UI::Button_ptr pButtonSendMessage = std::make_shared<UI::Button>( Rectangle2D( 0, 195, 25, 23 ) );
	pButtonSendMessage->SetID( BUTTONID_SendMessage );
	pButtonSendMessage->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\send.png", "game\\images\\UI\\hud\\chatbox\\send_.png" ) );
	pButtonSendMessage->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnEnterKeyPressed, this, std::placeholders::_1 ) ) );
	pButtonSendMessage->SetSelect( TRUE );
	pWindow1->AddElement( pButtonSendMessage );

	UI::Button_ptr pDiscordButton = std::make_shared<UI::Button>( Rectangle2D( 270, 195, 25, 23 ) );
	pDiscordButton->SetID( BUTTONID_Discord );
	pDiscordButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\hud\\chatbox\\discord.png", "game\\images\\UI\\hud\\chatbox\\discord_.png" ) );
	pDiscordButton->SetEvent( UI::Event::Build( std::bind( &CChatBox::OnDiscordButtonClick, this, std::placeholders::_1 ) ) );
	pDiscordButton->SetSelect( TRUE );
	pWindow1->AddElement( pDiscordButton );
}

BOOL CChatBox::IsEmoteMessage( std::string strMessage, std::string & strReplace, int & iEmoteIndex, EChatColor eChatColor )
{
	BOOL bRet = FALSE;

	iEmoteIndex = -1;

	if ( strMessage.length() >= 2 )
	{
		iEmoteIndex = -1;

		std::string strCommand = strMessage;

		if ( (eChatColor == CHATCOLOR_Trade) || (eChatColor == CHATCOLOR_Global) )
		{
			if ( strMessage.find( ">" ) != std::string::npos )
				strCommand = trim(strMessage.substr( strMessage.find( ">" ) + 1 ));
		}
		else
		{
			if ( strMessage.find( ":" ) != std::string::npos )
				strCommand = trim(strMessage.substr( strMessage.find( ":" ) + 1 ));
		}

		if ( strCommand[0] == 'e' )
		{
			strReplace = strCommand;
			iEmoteIndex = atoi( strCommand.substr( 1 ).c_str() );
		}
		else if ( strCommand[0] == '/' )
		{
			for ( std::map<std::string, std::string>::iterator it = mFriendlyCommands.begin(); it != mFriendlyCommands.end(); it++ )
			{
				if ( STRINGCOMPAREI( it->first.c_str(), strCommand.c_str() ) )
				{
					strReplace = strCommand;
					iEmoteIndex = atoi( it->second.substr( 1 ).c_str() );
					break;
				}
			}
		}

		if ( (iEmoteIndex > 0) && (iEmoteIndex <= MAX_EMOTELIST) )
			bRet = TRUE;
	}
	return bRet;
}

void CChatBox::OnResolutionChanged()
{
	pWindow1->SetPosition( 4, RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

	CheckCollideWindow( TRUE );
}

BOOL CChatBox::OnEmoteCommand( std::string & strMessage, UnitData * pcUnitData )
{
	BOOL bRet = FALSE;
	std::string strEffectPath;
	int iEmoteIndex = 0;

	for( std::map<std::string, std::string>::iterator it = mFriendlyCommands.begin(); it != mFriendlyCommands.end(); it++ )
	{
		if( STRINGCOMPAREI( it->first.c_str(), strMessage.c_str() ) )
		{
			strMessage = it->second;
			break;
		}
	}

	size_t uPos = strMessage.find_first_of( ":" );
	if( uPos != std::string::npos )
		if( uPos + 1 <= strMessage.length() )
			strMessage = trim( strMessage.substr( strMessage.find_first_of( ":" ) + 1 ) );

	if( (strMessage.length() <= 3) && (strMessage.length() > 0) )
	{
		iEmoteIndex = atoi(strMessage.substr(1).c_str());

		if( strMessage[0] == 'e' && isdigit(strMessage[1]) && iEmoteIndex > 0 && iEmoteIndex <= MAX_EMOTELIST )
		{
			bRet			= TRUE;
			strEffectPath	= FormatString("game\\scripts\\particles\\Emote%d.luac", iEmoteIndex );
		}
	}

	if( pcUnitData && bRet )
	{
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( strEffectPath, 0, TRUE );

		if( pc )
		{
			pc->SetID( iEmoteIndex + 1000 );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( UNITDATATOUNIT( pcUnitData ) );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}
	}

	return bRet;
}

void CChatBox::CheckCollideWindow( BOOL bResolutionChanged )
{
	if ( (((pWindow1->GetY() + pWindow1->GetHeight()) > (RESOLUTION_HEIGHT - 175)) && (pWindow1->GetX() < 800)) || bResolutionChanged )
	{
		//Open Inventory/Skills/Character Info Window
		if ( WINDOW_ISOPEN_INVENTORY && WINDOW_YINVENTORYMOVE > 0 && (WINDOW_YINVENTORYMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_YINVENTORYMOVE)) + (RESOLUTION_HEIGHT - 600) );

		if ( WINDOW_ISOPEN_SKILLS && WINDOW_SKILLMOVE > 0 && (WINDOW_SKILLMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_SKILLMOVE)) + (RESOLUTION_HEIGHT - 600) );

		if ( WINDOW_ISOPEN_CHARACTERINFO && WINDOW_CHARACTERINFOMOVE > 0 && (WINDOW_CHARACTERINFOMOVE != 256 || bResolutionChanged) )
			pWindow1->SetPosition( pWindow1->GetX(), (205 + (256 - WINDOW_CHARACTERINFOMOVE)) + (RESOLUTION_HEIGHT - 600) );
	}

	//Closing
	if ( !WINDOW_ISOPEN_INVENTORY && !WINDOW_ISOPEN_SKILLS && !WINDOW_ISOPEN_CHARACTERINFO )
	{
		if ( pWindow1->GetY() + pWindow1->GetHeight() > RESOLUTION_HEIGHT - 180 && pWindow1->GetX() + pWindow1->GetWidth() < 300 )
		{
			if ( WINDOW_YINVENTORYMOVE > 0 && WINDOW_YINVENTORYMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

			if ( WINDOW_SKILLMOVE > 0 && WINDOW_SKILLMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );

			if ( WINDOW_CHARACTERINFOMOVE > 0 && WINDOW_CHARACTERINFOMOVE < 256 )
				pWindow1->SetPosition( pWindow1->GetX(), RESOLUTION_HEIGHT - pWindow1->GetHeight() - 4 );
		}
	}
}

void CChatBox::AddRecentPlayer( std::string strMessage )
{
	std::string strName = "";

	if ( strMessage.find( "From> " ) == 0 )
		strName = strMessage.substr( STRLEN( "From> " ), strMessage.find( ':' ) - STRLEN( "From> " ) );
	else if ( strMessage.find( "To> " ) == 0 )
		strName = strMessage.substr( STRLEN( "To> " ), strMessage.find( ':' ) - STRLEN( "To> " ) );
	else
	{
		if ( strMessage.length() > 2 )
		{
			size_t iFind = strMessage.find( ' ' );

			if ( (strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')) )
				strName = strMessage.substr( 2, iFind != std::string::npos ? (iFind - 2) : 0 );
		}
	}

	if ( strName.length() > 0 )
	{
		CALLT_WITH_ARG1( 0x005A57D0, 0x039114D0, (DWORD)strName.c_str() );
	}
}

void CChatBox::AddSaveCommand( std::string strMessage )
{
	if ( strMessage[0] == '/' && strMessage.size() >= 3 )
	{
		std::string cache = "";
		std::string command = "";
		int startIndex = 1;

		cache += strMessage[0];

		//private message
		if ( strMessage[1] == ':' || strMessage[1] == ';' )
		{
			startIndex = 2;
			cache += strMessage[1];
		}

		std::string raw = trim(strMessage.substr(startIndex, std::string::npos));
		size_t index = raw.find(' ');
		if ( index == std::string::npos )
			index = raw.size();

		command = raw.substr(0, index);
		if ( command.empty() )
			return;

		cache += command;

		BOOL bFound = FALSE;
		for ( std::vector<std::string>::iterator it = vSaveCommand.begin(); it != vSaveCommand.end(); it++ )
		{
			std::string s = (*it);
			if ( STRINGCOMPARE( s.c_str(), cache.c_str() ) )
			{
				vSaveCommand.erase(it);
				vSaveCommand.push_back(cache);
				uSaveCommandWheel = vSaveCommand.size() - 1;

				bFound = TRUE;
				break;
			}
		}

		if ( bFound == FALSE )
		{
			vSaveCommand.push_back(cache);
			if ( vSaveCommand.size() > MAX_SAVECOMMANDSCHAT )
				vSaveCommand.erase( vSaveCommand.begin() );

			//set wheel position to be at latest
			uSaveCommandWheel = vSaveCommand.size() - 1;
		}
	}
}

std::string CChatBox::GetSaveCommand( BOOL bUP )
{
	std::string str = "";

	if ( vSaveCommand.size() > 0 )
	{
		if (bUP == FALSE)
		{
			if (bFirstDown &&
				uSaveCommandWheel == vSaveCommand.size() - 1)
			{
				str = vSaveCommand[uSaveCommandWheel];
				bFirstDown = FALSE;
			}
			else if (uSaveCommandWheel > 0)
				str = vSaveCommand[--uSaveCommandWheel];
			else
				str = vSaveCommand[uSaveCommandWheel];
		}
		else
		{
			if (uSaveCommandWheel < vSaveCommand.size() - 1)
				str = vSaveCommand[++uSaveCommandWheel];
			else
				str = vSaveCommand[uSaveCommandWheel];
		}

		str.insert( str.length(), " " );
	}
	return str;
}

void CChatBox::SendItemLink( ItemData * pcItemData )
{
	if ( pcItemData && pcItemData->bValid )
	{
		PacketItemLinkChat sPacket;
		ZeroMemory( &sPacket, sizeof( PacketItemLinkChat ) );

		sPacket.eChatColor = CHATCOLOR_Normal;

		BOOL bLoginServer = FALSE;

		std::string rarityPrefix = "";
		switch ( pcItemData->sItem.eRarity )
		{
			case  EItemRarity::RARE:		rarityPrefix = "Rare ";			break;
			case  EItemRarity::EPIC:		rarityPrefix = "Epic ";			break;
			case  EItemRarity::LEGENDARY:	rarityPrefix = "Legendary ";	break;
			default:						rarityPrefix = "";				break;
		}


		auto pInputField = pWindow1->GetElement<UI::InputField>( INPUTFIELDID_Message );
		if ( pInputField )
		{
			std::string strMessage = rtrim(pInputField->GetText());

			if ( strMessage.length() > 2 )
			{
				//Whisper
				if ( (strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')) )
				{
					size_t iFind = strMessage.find( " " );
					std::string strName = "";
					strName = strMessage.substr( 2, iFind != std::string::npos ? (iFind - 2) : std::string::npos );


					if ( strName.length() > 0 )
					{
						STRINGCOPY( sPacket.szCharacterName, strName.c_str() );
						sPacket.eChatColor = CHATCOLOR_Whisper;
						AddRecentPlayer( strMessage );
						pInputField->ClearInput();
					}

					std::string body = "";
					if (iFind != std::string::npos)
					{
						body = strMessage.substr(iFind);
					}


					if (body == "")
					{
						if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing && pcItemData->sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
						{
							strMessage = FormatString("%s%s (%s)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, MIXHANDLER->GetMixStringName(pcItemData->sItem.eMixTypeName).c_str());
						}
						else if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0)
						{
							strMessage = FormatString("%s%s (+%d)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, pcItemData->sItem.sAgeLevel);
						}
						else
						{
							strMessage = FormatString("%s%s", rarityPrefix.c_str(), pcItemData->sItem.szItemName);
						}
					}
					else
					{
						strMessage = body;
					}
				}
			}

			if (strMessage.length() >= 2)
			{
				//S>
				if ((strMessage[0] == 'S' || strMessage[0] == 's') && strMessage[1] == '>')
				{
					if (strMessage.length() > 2)
					{
						std::string body = trim(strMessage.substr(2));

						//matches: 12k, 1k, 123k, 1.2kk, 2kk, 3kkk, 1.2k, 123KK
						std::regex e("^(\\d*\\.)?\\d+k+$", std::regex_constants::icase);

						//exact match? i.e. S> 1.2kk
						if (std::regex_match(body, e))
						{
							if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing && pcItemData->sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
							{
								strMessage = FormatString("S> %s%s (%s) -- %s", rarityPrefix.c_str(), pcItemData->sItem.szItemName, MIXHANDLER->GetMixStringName(pcItemData->sItem.eMixTypeName).c_str(), body.c_str());
							}
							else if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0)
							{
								strMessage = FormatString("S> %s%s (+%d) -- %s", rarityPrefix.c_str(), pcItemData->sItem.szItemName, pcItemData->sItem.sAgeLevel, body.c_str());
							}
							else
							{
								strMessage = FormatString("S> %s%s -- %s", rarityPrefix.c_str(), pcItemData->sItem.szItemName, body.c_str());
							}
						}
					}

					//S> or s>
					else
					{
						if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing && pcItemData->sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
						{
							strMessage = FormatString("S> %s%s (%s)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, MIXHANDLER->GetMixStringName(pcItemData->sItem.eMixTypeName).c_str());
						}
						else if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0)
						{
							strMessage = FormatString("S> %s%s (+%d)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, pcItemData->sItem.sAgeLevel);
						}
						else
						{
							strMessage = FormatString("S> %s%s", rarityPrefix.c_str(), pcItemData->sItem.szItemName);
						}
					}
				};
			}

			//empty string
			else if (strMessage == "")
			{
				if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing && pcItemData->sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
				{
					strMessage = FormatString("%s%s (%s)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, MIXHANDLER->GetMixStringName(pcItemData->sItem.eMixTypeName).c_str());
				}
				else if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0)
				{
					strMessage = FormatString("%s%s (+%d)", rarityPrefix.c_str(), pcItemData->sItem.szItemName, pcItemData->sItem.sAgeLevel);
				}
				else
				{
					strMessage = FormatString("%s%s", rarityPrefix.c_str(), pcItemData->sItem.szItemName);
				}
			}


			if ( strMessage[0] == '/' || strMessage[0] == '$' )
			{
				if ( (strMessage.compare( 0, 7, "/TRADE>" ) == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2) )
				{
					sPacket.eChatColor = CHATCOLOR_Trade;

					bLoginServer = TRUE;

					if ( strMessage[0] == '$' )
					{
						strMessage.replace( 0, 1, "/TRADE>" );
					}
				}

				else if ( (strMessage.compare( 0, 6, "/CLAN>" ) == 0 && strMessage.length() > 7) )
				{
					sPacket.eChatColor = CHATCOLOR_Clan;
					bLoginServer = TRUE;
				}
			}
			else
			{
				if ( iTabID == CHATBOXTABID_Clan )
				{
					strMessage.insert( 0, "/CLAN>" );
					sPacket.eChatColor = CHATCOLOR_Clan;
					bLoginServer = TRUE;
				}
				else if (iTabID == CHATBOXTABID_Party)
				{
					sPacket.eChatColor = CHATCOLOR_Party;
				}
				else if (iTabID == CHATBOXTABID_Raid)
				{
					sPacket.eChatColor = CHATCOLOR_Raid;
				}
				else if ( iTabID == CHATBOXTABID_Trade )
				{
					strMessage.insert( 0, "/TRADE>" );
					sPacket.eChatColor = CHATCOLOR_Trade;
					bLoginServer = TRUE;
				}
			}

			STRINGCOPY( sPacket.szMessage, strMessage.c_str() );
		}



		CopyMemory( &sPacket.cItemData, pcItemData, sizeof( ItemData ) );
		ProcessPacket( &sPacket, bLoginServer );
	}
}

void CChatBox::OnItemLinkHover( ItemData * pcItemData, UI::Window_ptr pWindowHover, UIEventArgs eArgs )
{
	if ( pWindowHover->IsHover() )
	{
		if ( (cItemLinkDataWrapper.sItem.iChk1 != pcItemData->sItem.iChk1) ||
			 (cItemLinkDataWrapper.sItem.iChk2 != pcItemData->sItem.iChk2) )
		{
			CopyMemory( &cItemLinkDataWrapper, pcItemData, sizeof( ItemData ) );

			cItemLinkDataWrapper.sPosition.iX = pWindow1->GetX() + (pWindow1->GetWidth() >> 1);
			cItemLinkDataWrapper.sPosition.iY = MOUSEHANDLER->GetPosition()->iY;
		}

		cItemLinkDataWrapper.bValid = TRUE;
	}
	else
	{
		cItemLinkDataWrapper.bValid = FALSE;
	}
}
