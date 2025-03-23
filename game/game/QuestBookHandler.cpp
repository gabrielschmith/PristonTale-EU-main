#include "stdafx.h"
#include "QuestBookHandler.h"



QuestBookHandler::QuestBookHandler()
{
	dwRequestTick = 0;
}

QuestBookHandler::~QuestBookHandler()
{
	vQuestEntries.clear();
	mForceShowHideGroup.clear();
	mQuestDataResponse.clear();
}

void QuestBookHandler::Init()
{
}


void QuestBookHandler::BuildWindow()
{
	//entire quest book UI
	questBookWindow = std::make_shared<UI::Window>(Rectangle2D(0, 0, iQuestBookWindowWidth, iQuestBookWindowHeight));
	questBookWindow->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\Quest\\QuestBook.png"));
	UpdateWindowPosition();

	UI::Button_ptr pClickMask = std::make_shared<UI::Button>( Rectangle2D( 0, 0, iQuestBookWindowWidth, iQuestBookWindowHeight ) );
	questBookWindow->AddElement( pClickMask );

	//left box region
	leftBoxRegion = std::make_shared<UI::Window>(Rectangle2D(iLeftBoxLeftX, iLeftBoxTopY, GetLeftBoxWidth(), GetLeftBoxHeight()));
	questBookWindow->AddElement(this->leftBoxRegion);




	questsList = std::make_shared<UI::List>(Rectangle2D(iQuestListPadding, iQuestListTopYOffset, GetLeftBoxWidth() - iQuestListPadding * 2 - iScrollBarWidth, GetLeftBoxHeight() - iQuestListPadding - iQuestListTopYOffset ));
	questsList->SetPaddingSeparator(iRowPadding);
	questsList->MakeScrollBar(Rectangle2D(questsList->GetWidth(), 0, iScrollBarWidth, questsList->GetHeight()), "game\\images\\UI\\Quest\\scroll.png", "game\\images\\UI\\Quest\\scroller_icon.png", 18, 18 );



	leftBoxRegion->AddElement(this->questsList);

	//right box region
	rightBoxRegion = std::make_shared<UI::Window>(Rectangle2D(iRightBoxLeftX, iRightBoxTopY, GetRightBoxWidth(), GetRightBoxHeight()));
	questBookWindow->AddElement(this->rightBoxRegion);

	rightBoxList = std::make_shared<UI::List>(Rectangle2D(iRightBoxListPadding, iRightBoxListPadding, GetRightBoxWidth() - iRightBoxListPadding * 2 - iScrollBarWidth, GetRightBoxHeight() - iRightBoxListPadding - iRightBoxListPadding ));
	rightBoxList->SetPaddingSeparator(0);
	rightBoxList->MakeScrollBar(Rectangle2D(rightBoxList->GetWidth(), 0, iScrollBarWidth, rightBoxList->GetHeight()), "game\\images\\UI\\Quest\\scroll.png", "game\\images\\UI\\Quest\\scroller_icon.png", 18, 18 );

	rightBoxRegion->AddElement(this->rightBoxList);

	rightBoxSubRegion = std::make_shared<UI::Window>(Rectangle2D(0, 0, rightBoxRegion->GetWidth(), 0));
	rightBoxList->AddWindow( rightBoxSubRegion );

	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( iCloseButtonLeftX, iCloseButtonTopY, iCloseButtonWidth, iCloseButtonHeight ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &QuestBookHandler::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
	questBookWindow->AddElement( pButtonClose );


	UI::CheckBox_ptr checkBox = std::make_shared<UI::CheckBox>( Rectangle2D( iLeftBoxLeftX + 1, iLeftBoxTopY - 20, 13, 13 ) );
	checkBox->SetFont("Arial", 16, 6, FALSE, TRUE, D3DCOLOR_ARGB(255, 0, 0, 0) );
	checkBox->SetID( EElementID::CheckBox_ShowAll );
	checkBox->SetText("Show all");
	checkBox->SetCheck(FALSE);
	checkBox->SetEvent(UI::Event::Build(std::bind(&QuestBookHandler::OnShowAvailableCheckboxClicked, this, std::placeholders::_1)));
	checkBox->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	questBookWindow->AddElement(checkBox);


	UI::Text_ptr locationText = std::make_shared<UI::Text>( Rectangle2D( iLeftBoxLeftX + 10, iLeftBoxTopY + 5, 40, 16 ) );
	//questText->SetID( QuestBookHandler::EElementID::Row_QuestID + i );
	locationText->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	locationText->SetText( "Location" );
	locationText->SetNoClip( TRUE );
	locationText->SetHorizontalAlign( EAlign::ALIGN_Left );
	locationText->SetVerticalAlign( EAlign::ALIGN_Middle );
	questBookWindow->AddElement( locationText );

	UI::Text_ptr levelText = std::make_shared<UI::Text>( Rectangle2D( iLeftBoxLeftX + (GetLeftBoxWidth() - 60), iLeftBoxTopY + 5, 30, 16 ) );
	//questText->SetID( QuestBookHandler::EElementID::Row_QuestID + i );
	levelText->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	levelText->SetText( "Level" );
	levelText->SetNoClip( TRUE );
	levelText->SetHorizontalAlign( EAlign::ALIGN_Left );
	levelText->SetVerticalAlign( EAlign::ALIGN_Middle );
	questBookWindow->AddElement( levelText );
}

void QuestBookHandler::OnExpandCollapseClick( int iIconGroupID, UIEventArgs e )
{
	BOOL bShow = FALSE;
	int iGroupID = -1;

	if ( iIconGroupID >= EElementID::QuestGroup_Expand_ID && iIconGroupID < EElementID::QuestGroup_Collapse_ID )
	{
		//user clicked on a 'Expand' icon
		iGroupID = iIconGroupID - ((int)EElementID::QuestGroup_Expand_ID); //to get the true group id
		bShow = TRUE;
	}
	else
	{
		//user clicked on a 'Collapse' icon
		iGroupID = iIconGroupID - ((int)EElementID::QuestGroup_Collapse_ID); //to get the true group id
		bShow = FALSE;
	}


	auto it = mForceShowHideGroup.find( iGroupID );
	if ( it != mForceShowHideGroup.end() )
		it->second = bShow;
	else
		mForceShowHideGroup.insert( { iGroupID, bShow } );

	BuildQuestList();
}

void QuestBookHandler::OnShowAvailableCheckboxClicked(UIEventArgs e)
{
	bShowAll = questBookWindow->GetElement<UI::CheckBox>(EElementID::CheckBox_ShowAll)->IsChecked();
	BuildQuestList();
}

void QuestBookHandler::OnCloseButtonClick(UIEventArgs e)
{
	if ( IsWindowOpen() )
	{
		this->questBookWindow->Hide();
		//MOUSEHANDLER->UnblockMouse();
	}
}

bool QuestBookHandler::OnKeyPress( CKeyboard * pcKeyBoard )
{
	if ( Game::GetGameMode() != GAMEMODE_InGame || PLAYERDATA == NULL )
		return false;

	if ( !CQUESTGAME->GetQuestInstance() )
		return false;

	if ( pcKeyBoard->GetKey() == VK_F2 ) //has same key code as 'Q' (113)
		return false;

	if (!IsWindowOpen() && pcKeyBoard->IsKeyDown( 'Q' ))
	{
		if ( this->questBookWindow == NULL )
			BuildWindow();

		GetQuestList();

		UpdateWindowPosition();

		this->questBookWindow->Show();

		//MOUSEHANDLER->BlockMouse();

		if (bQuestNamesReceived )
			BuildQuestList();

		return true;
	}
	else if (IsWindowOpen() && (pcKeyBoard->IsKeyDown( 'Q' ) || pcKeyBoard->GetKey() == VK_ESCAPE))
	{
		this->questBookWindow->Hide();

		//MOUSEHANDLER->UnblockMouse();

		return true;
	}

	return false;
}

void QuestBookHandler::Render()
{
	if (!IsWindowOpen())
		return;

	this->questBookWindow->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}


BOOL QuestBookHandler::OnMouseClick(CMouse* pcMouse)
{
	if ( !IsWindowOpen() )
		return FALSE;

	//block mouse click while it's getting data
	if ( !CanProcessRequest() )
		return TRUE;

	if ( this->questBookWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
		return TRUE;

	return FALSE;
}

BOOL QuestBookHandler::OnMouseScroll(CMouse* pcMouse)
{
	if ( !IsWindowOpen() )
		return FALSE;

	if (this->questTextWindow &&
		this->questTextWindow->OnMouseScroll(pcMouse, questBookWindow->GetX(), questBookWindow->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
	{
		return TRUE;
	}

	if (this->leftBoxRegion &&
		this->leftBoxRegion->OnMouseScroll(pcMouse, questBookWindow->GetX(), questBookWindow->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
	{
		return TRUE;
	}

	if (this->rightBoxRegion &&
		this->rightBoxRegion->OnMouseScroll(pcMouse, questBookWindow->GetX(), questBookWindow->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
	{
		return TRUE;
	}

	if( Rectangle2D( questBookWindow->GetX(), questBookWindow->GetY(), questBookWindow->GetWidth(), questBookWindow->GetHeight() ).Inside( pcMouse->GetPosition() ) )
	{
		return TRUE;
	}

	return FALSE;
}

void QuestBookHandler::OnMouseMove(CMouse* pcMouse)
{
	if ( !IsWindowOpen() )
		return;

	this->questBookWindow->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

void QuestBookHandler::UpdateWindowPosition()
{
	if ( !IsWindowOpen() )
		return;

	questBookWindow->SetPosition((RESOLUTION_WIDTH >> 1) - (questBookWindow->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - ((questBookWindow->GetHeight()) >> 1) - 50);
}

void QuestBookHandler::OnResolutionChanged()
{
	if ( !IsWindowOpen() )
		return;

	UpdateWindowPosition();
}

void QuestBookHandler::GetQuestList()
{
	if ( !IsWindowOpen() )
		return;

	if ( !CanProcessRequest() )
		return;

	//already got the quest names? no need to ask again.
	if ( bQuestNamesReceived )
		return;

	PacketQuestNamesRequest s;
	s.iLength = sizeof(PacketQuestNamesRequest);
	s.iHeader = PKTHDR_QuestNamesRequest;

	SENDPACKETL(&s, TRUE);

	dwRequestTick = TICKCOUNT;
}

void QuestBookHandler::ReLoadASMQuests()
{
	for ( auto & quest : vQuestEntries )
	{
		if ( quest.iASMQuestBit > 0 )
		{
			if ( ( PLAYER_QUESTLOG & quest.iASMQuestBit ) != 0 )
			{
				quest.eQuestState = EQuestState::Done;
			}
		}
	}
}


void QuestBookHandler::ReLoadActiveAndFinishedQuest()
{
	vQuestEntries.erase(std::remove_if(
		vQuestEntries.begin(), vQuestEntries.end(),
		[](const QuestBookEntry& x)
		{
			return x.eQuestState == EQuestState::InProgress ||
				   x.eQuestState == EQuestState::Completed ||
				   x.eQuestState == EQuestState::Done;

		}), vQuestEntries.end());

	//Get active quests
	for ( auto const & quest : CQUESTGAME->GetQuestInstance()->GetData() )
	{
		QuestBookEntry entry{};
		entry.iQuestID = quest->GetID();
		entry.sQuestLevelMin = quest->GetLevelMin();
		entry.sQuestLevelMax = quest->GetLevelMax();
		entry.sMapID = quest->GetMapID();
		entry.sGiverNpcID = quest->GetGiverNpcID();
		entry.eQuestType = (EQuestType)quest->GetType();
		entry.eQuestPartyType = quest->GetQuestPartyType();
		entry.sMainQuestID = quest->GetMainQuestID();
		entry.eQuestState = quest->IsFinished() ? EQuestState::Completed : EQuestState::InProgress;
		entry.sGroupNum = quest->GetGroupNum();

		StringCbCopy( entry.szQuestName, _countof( entry.szQuestName ), quest->GetName() );
		StringCbCopy( entry.szGroupName, _countof( entry.szGroupName ), quest->GetGroupName() );

		auto iter = std::find_if(vQuestEntries.begin(), vQuestEntries.end(),
			[&]( const QuestBookEntry & ts ) { return ts.iQuestID == entry.iQuestID; } );

		if ( iter != vQuestEntries.end() )
		{
			iter->eQuestState = entry.eQuestState;
		}
		else
		{
			vQuestEntries.push_back ( entry );
		}
	}

	//get finished (handed in) quests
	for ( auto const & quest : CQUESTGAME->GetQuestInstance()->GetFinishData() )
	{
		QuestBookEntry entry{};
		entry.iQuestID = quest->iID;
		entry.sQuestLevelMin = quest->iMinLevel;
		entry.sQuestLevelMax = quest->iMaxLevel;
		entry.sMapID = quest->sMapID;
		entry.sGiverNpcID = quest->sGiverNpcID;
		entry.eQuestType = quest->eType;
		entry.eQuestPartyType = quest->eParty;
		entry.eQuestState = CQUESTGAME->GetQuestInstance()->IsQuestExpired(quest->eType, quest->sEndDate) ? EQuestState::Expired : EQuestState::Done;
		entry.sGroupNum = quest->sGroupNum;

		StringCbCopy( entry.szQuestName, _countof( entry.szQuestName ), quest->szQuestName );
		StringCbCopy( entry.szGroupName, _countof( entry.szGroupName ), quest->szQuestGroupName );

		auto iter = std::find_if(vQuestEntries.begin(), vQuestEntries.end(),
			[&]( const QuestBookEntry & ts ) { return ts.iQuestID == entry.iQuestID; } );

		if ( iter != vQuestEntries.end() )
		{
			iter->eQuestState = entry.eQuestState;
		}
		else
		{
			vQuestEntries.push_back ( entry );
		}
	}
}

void QuestBookHandler::HandlePacket(PacketQuestNamesResponse* psPacket)
{
	dwRequestTick = 0;

	if ( psPacket->bInitial )
	{
		bQuestNamesReceived = TRUE;
		vQuestEntries.clear();
	}

	int iCountUnlocked = 0;

	for ( int i = 0; i < psPacket->questCount; ++i )
	{
		auto data = &psPacket->questsInformation[i];

		QuestBookEntry entry{};
		entry.iQuestID = data->questID;
		entry.sQuestLevelMin = data->minLevel;
		entry.sQuestLevelMax = data->maxLevel;
		entry.sMapID = data->mapID;
		entry.sGiverNpcID = data->giverNpcID;
		entry.eQuestType = data->questType;
		entry.eQuestPartyType = data->partyMode;
		entry.sRequiredQuestID = data->requiredQuestId;
		entry.sMainQuestID = data->mainQuestId;
		entry.sGroupNum = data->groupNum;
		entry.eQuestState = EQuestState::Avaliable; //level req checks will be done in BuildQuestWindow

		if ( data->minLevel == PLAYERDATA->iLevel )
			iCountUnlocked++;

		StringCbCopy( entry.szQuestName, _countof( entry.szQuestName ), data->name );
		StringCbCopy( entry.szGroupName, _countof( entry.szGroupName ), data->group );

		auto iter = std::find_if(vQuestEntries.begin(), vQuestEntries.end(),
			[&]( const QuestBookEntry & ts ) { return ts.iQuestID == entry.iQuestID; } );

		if ( iter != vQuestEntries.end() )
		{
			iter->eQuestState = entry.eQuestState;
		}
		else
		{
			vQuestEntries.push_back ( entry );
		}
	}

	if ( psPacket->iHeader == PKTHDR_NewQuestNamesPostLvlUp && iCountUnlocked > 0 )
	{
		CHATBOX->AddMessage( FormatString( "> %d new quests available, press Q to view them!", iCountUnlocked ), EChatColor::CHATCOLOR_Normal );
	}

	BuildQuestList();
}

void QuestBookHandler::BuildQuestList( )
{
	if ( !IsWindowOpen() )
		return;

	float fVisibleHeight = static_cast<float>( questsList->GetHeight() );
	int oldTotalRowHeight = iTotalRowHeight;
	float oldScrollPercent = questsList->GetScrollPercentage() / 100.0f;
	float oldScrollableHeight = oldTotalRowHeight > fVisibleHeight ? oldTotalRowHeight - fVisibleHeight : 0;
	iTotalRowHeight = 0;

	questsList->Clear();

	ReLoadActiveAndFinishedQuest();
	RebuildCurrentQuestDataView();

	BOOL bIgnore8000Quests = !QUESTGAMEHANDLER->IsFinishedQuest( 8000 ) && !QUESTGAMEHANDLER->IsActiveQuest( 8000 );
	BOOL bIgnore9000Quests = !QUESTGAMEHANDLER->IsFinishedQuest( 9000 ) && !QUESTGAMEHANDLER->IsActiveQuest( 9000 );

	//check level requirements
	for ( auto & quest : vQuestEntries )
	{
		if ( PLAYERDATA->iLevel > quest.sQuestLevelMax )
		{
			quest.eQuestState = EQuestState::Disable;
			continue;
		}

		if ( quest.eQuestState == EQuestState::Avaliable ||
			 quest.eQuestState == EQuestState::Expired ||
			 quest.eQuestState == EQuestState::LevelReqNotMet ||
			 quest.eQuestState == EQuestState::LevelReqNotMetWithin5 ||
			 quest.eQuestState == EQuestState::Unknown )
		{
			if ( PLAYERDATA->iLevel >= quest.sQuestLevelMin )
				quest.eQuestState = EQuestState::Avaliable;
			else if ( ( PLAYERDATA->iLevel + 5 ) >= quest.sQuestLevelMin )
				quest.eQuestState = EQuestState::LevelReqNotMetWithin5;
			else
				quest.eQuestState = EQuestState::LevelReqNotMet;


		}
	}

	//check for any quests that has required quests id to be completed
	//in order to use it
	for ( auto & quest : vQuestEntries )
	{
		if ( quest.sRequiredQuestID > 0 &&
			 (quest.eQuestState == EQuestState::Avaliable ||
			  quest.eQuestState == EQuestState::LevelReqNotMetWithin5))
		{
			for ( auto const & check : vQuestEntries )
			{
				if ( check.iQuestID == quest.sRequiredQuestID &&
					 check.eQuestState != EQuestState::Done )
				{
					quest.eQuestState = EQuestState::RequiredQuestNotDone;
					StringCbCopy( quest.szOtherQuestName, _countof( quest.szOtherQuestName ), check.szQuestName );
					break;
				}
			}
		}
	}

	//check sub-quests for completion of main quest
	for ( auto & quest : vQuestEntries )
	{
		if ( quest.sMainQuestID > 0)
		{
			for ( auto const & check : vQuestEntries )
			{
				if ( check.iQuestID == quest.sMainQuestID &&
					 check.eQuestState == EQuestState::Done )
				{
					 quest.eQuestState = EQuestState::Done;
					 break;
				}
			}
		}
	}

	//check for any active / in progress quest taken by a NPC
	//giving out a list of quests (usually repeatables)
	for ( auto & quest : vQuestEntries )
	{
		if ( quest.sGiverNpcID > 0 &&
			 quest.eQuestState == EQuestState::Avaliable )
		{
			for ( auto const & check : vQuestEntries )
			{
				if ( check.iQuestID != quest.iQuestID &&
					 check.sGiverNpcID == quest.sGiverNpcID)
				{
					if ( check.eQuestState == EQuestState::InProgress ||
						 check.eQuestState == EQuestState::Completed )
					{
						quest.eQuestState = EQuestState::OtherNpcQuestTaken;
						StringCbCopy( quest.szOtherQuestName, _countof( quest.szOtherQuestName ), check.szQuestName );
						break;
					}
				}
			}
		}
	}

	//sort quest enteries
	std::sort(vQuestEntries.begin(), vQuestEntries.end(), []( const QuestBookEntry& lhs, const QuestBookEntry& rhs )
	{
		//group num asc
		if ( lhs.sGroupNum < rhs.sGroupNum ) return true;
		if ( lhs.sGroupNum > rhs.sGroupNum ) return false;

		if ( lhs.eQuestState < rhs.eQuestState ) return true;
		if ( lhs.eQuestState > rhs.eQuestState ) return false;

		//quest level descending
		if ( lhs.sQuestLevelMin < rhs.sQuestLevelMin ) return true;
		if ( lhs.sQuestLevelMin > rhs.sQuestLevelMin ) return false;

		if ( lhs.eQuestPartyType > rhs.eQuestPartyType ) return true;
		if ( lhs.eQuestPartyType < rhs.eQuestPartyType ) return false;


		if ( lhs.iQuestID < rhs.iQuestID ) return true;
		if ( lhs.iQuestID > rhs.iQuestID ) return false;

		return false;
	});

	BOOL bFirst = TRUE;
	int iCurrentGroupNum = -1;
	BOOL bForceCollapse = FALSE;
	BOOL bForceExpand = FALSE;
	BOOL bAddGapOnMapGroupChange = FALSE;

	//i == 0 show map group containing useful quests (so we show them at top first)
	//i == 1 means all remaining group will appear at bottom
	for ( int i = 0; i < 2; i++ )
	{
		BOOL bShowUsefulFirst = i == 0;

		//don't show useless quests if show all checkbox isn't enabled
		if ( !bShowUsefulFirst && !bShowAll )
			break;

		for ( auto const & entry : vQuestEntries )
		{
			if ( iCurrentGroupNum != entry.sGroupNum )
			{
				//map id is different and previous map has useful quests?
				//add a blank line
				if ( bShowUsefulFirst &&
					bAddGapOnMapGroupChange )
				{
					UI::Window_ptr emptyRow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, GetQuestListRowWidth(), iQuestListRowHeight ) );
					questsList->AddWindow( emptyRow );
					iTotalRowHeight += iQuestListRowHeight + iRowPadding;

					bAddGapOnMapGroupChange = FALSE;
				}

				iCurrentGroupNum = entry.sGroupNum;
			}
			else
			{
				continue;
			}

			bool bHasUsefulQuests = false;
			bool bHasProgressOrCompleteQuests = false;
			for ( auto const & quest : vQuestEntries )
			{
				if ( quest.sGroupNum != iCurrentGroupNum )
					continue;

				if ( quest.eQuestState == EQuestState::InProgress ||
					 quest.eQuestState == EQuestState::Completed )
				{
					bHasProgressOrCompleteQuests = true;
					bHasUsefulQuests = true;
					break;
				}

				if ( quest.eQuestState == EQuestState::Avaliable ||
					 quest.eQuestState == EQuestState::Expired ||
					 quest.eQuestState == EQuestState::LevelReqNotMetWithin5)
				{
					bHasUsefulQuests = true;
				}
			}

			//show useful quests first
			if ( (bShowUsefulFirst && !bHasUsefulQuests) || (!bShowUsefulFirst && bHasUsefulQuests) )
				continue;


			auto it = mForceShowHideGroup.find( iCurrentGroupNum ); //+1 to match (see below for note)

			bForceCollapse = FALSE;
			bForceExpand = FALSE;

			//no force show / hide for this group found, use default
			//which is hide if no useful quest
			if ( it != mForceShowHideGroup.end() )
			{
				//force show / hide for this group was found
				//it->second == FALSE means that the user force hid this group
				if ( it->second == FALSE )
					bForceCollapse = TRUE;
				else
					bForceExpand = TRUE;
			}

			BOOL bDrawTitle = TRUE;

			for ( auto const & quest : vQuestEntries )
			{
				if ( quest.sGroupNum != iCurrentGroupNum )
					continue;

				//don't show the 8000 quest chain if 8000 quest isn't taken / finished yet
				if ( bIgnore8000Quests &&
					 quest.iQuestID >= 8000 && quest.iQuestID <= 8999 )
				{
					continue;
				}

				//don't show the 9000 quest chain if 8000 quest isn't taken / finished yet
				if ( bIgnore9000Quests &&
					 quest.iQuestID >= 9000 && quest.iQuestID <= 9999 )
				{
					continue;
				}

				if ( quest.eQuestState == EQuestState::Disable )
					continue;

				if ( bDrawTitle )
				{
					UI::Window_ptr mapRow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, GetQuestListRowWidth(), iQuestListRowHeight ) );
					questsList->AddWindow( mapRow );
					iTotalRowHeight += iQuestListRowHeight + iRowPadding;

					UI::ImageBox_ptr pExpandCollapseIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 16, 16 ) );

					if ( !bForceCollapse && (bHasUsefulQuests || bForceExpand) )
					{
						if (bHasUsefulQuests)
							pExpandCollapseIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_collapse_white.png" ) );
						else
							pExpandCollapseIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_collapse_gray.png" ) );

						pExpandCollapseIcon->SetToolTip( "Collapse" );
						pExpandCollapseIcon->SetID(  QuestGroup_Collapse_ID + iCurrentGroupNum);
					}
					else
					{
						if (bHasUsefulQuests)
							pExpandCollapseIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_expand_white.png" ) );
						else
							pExpandCollapseIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_expand_gray.png" ) );

						pExpandCollapseIcon->SetToolTip( "Expand" );
						pExpandCollapseIcon->SetID( QuestGroup_Expand_ID + iCurrentGroupNum ); //+1 due to some map id being -1
					}

					pExpandCollapseIcon->SetEvent( UI::Event::Build( std::bind( &QuestBookHandler::OnExpandCollapseClick, this, pExpandCollapseIcon->GetID(), std::placeholders::_1 ) ) );

					mapRow->AddElement( pExpandCollapseIcon );

					UI::Text_ptr mapText = std::make_shared<UI::Text>( Rectangle2D( 20, 0, GetQuestListRowWidth() - 40, iQuestListRowHeight ) );
					mapText->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
					mapText->SetText( entry.szGroupName );

					if ( bHasUsefulQuests )
					{
						mapText->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
					else
					{
						mapText->SetColor( D3DCOLOR_ARGB( 255, 200, 200, 200 ) );
					}

					mapText->SetNoClip( TRUE );
					mapText->SetHorizontalAlign( EAlign::ALIGN_Left );
					mapText->SetVerticalAlign( EAlign::ALIGN_Middle );

					mapRow->AddElement( mapText );

					bDrawTitle = FALSE;
				}

				//force collapse of this quest
				//UNLESS it is a quest that is 'in progress' or 'completed'
				//since we want to always show these.
				if ( bForceCollapse )
					//&& quest.eQuestState != EQuestState::InProgress
					//&& quest.eQuestState != EQuestState::Completed )
					break;

				if ( !bForceExpand && !bHasUsefulQuests )
					break;

				//if show all is NOT checked, only show useful things
				if ( !bShowAll
					  && quest.eQuestState != EQuestState::Avaliable
					  && quest.eQuestState != EQuestState::Expired
					  && quest.eQuestState != EQuestState::InProgress
					  && quest.eQuestState != EQuestState::Completed
					  && quest.eQuestState != EQuestState::LevelReqNotMetWithin5  )
				{
					continue;
				}


				bAddGapOnMapGroupChange = TRUE;

				UI::Window_ptr row = std::make_shared<UI::Window>( Rectangle2D( 4, 0, GetQuestListRowWidth() - 4, iQuestListRowHeight ) );
				questsList->AddWindow( row );
				iTotalRowHeight += iQuestListRowHeight + iRowPadding;

				UI::ImageBox_ptr pIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 16, 16 ) );
				UI::ImageBox_ptr pQuestIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 18, 0, 11, 16 ) );

				bool bRepeatables = quest.eQuestType != EQuestType::SoloOneOff && quest.eQuestType != EQuestType::PartyOneOff;

				if ( quest.eQuestPartyType == EQuestPartyType::SoloOnly )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\solo_icon.png" ) );
					pIcon->SetToolTip( "Can only be completed solo" );
				}
				else if ( quest.eQuestPartyType == EQuestPartyType::PartyOnly )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\party_icon.png" ) );
					pIcon->SetToolTip( "Can only be completed in a party" );
				}
				else if ( quest.eQuestPartyType == EQuestPartyType::SoloAny )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\solo_icon.png" ) );
					pIcon->SetToolTip( "Solo" );
				}
				else if ( quest.eQuestPartyType == EQuestPartyType::SoloPartyAny )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\party_solo_icon.png" ) );
					pIcon->SetToolTip( "Solo or Party" );
				}
				else if ( quest.iQuestID >= 6000 && quest.iQuestID <= 6999 )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\boss_icon.png" ) );
					pIcon->SetToolTip( "Boss quest" );
				}
				else if ( quest.iQuestID >= 3000 && quest.iQuestID <= 3999 )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\sword_icon.png" ) );
					pIcon->SetToolTip( "Solo, Party or Raid (Zone)" );
				}
				else if ( quest.eQuestPartyType == EQuestPartyType::SoloAndParty ||
					quest.eQuestPartyType == EQuestPartyType::SoloPartyAndRaid )
				{
					pIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\party_solo_icon.png" ) );
					pIcon->SetToolTip( "Solo, Party or Raid" );
				}

				if ( pIcon->GetImage() )
				{
					row->AddElement( pIcon );
				}

				if ( quest.eQuestState == EQuestState::Completed )
				{
					if ( bRepeatables )
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_complete_blue.png" ) );
					else
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_complete.png" ) );

					pQuestIcon->SetToolTip( "Quest is ready to be handed in!" );
				}
				else if ( quest.eQuestState == EQuestState::InProgress )
				{
					pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_progress.png" ) );
					pQuestIcon->SetToolTip( "Quest is in progress" );
				}
				else if ( quest.eQuestState == EQuestState::Avaliable || quest.eQuestState == EQuestState::Expired )
				{
					if ( bRepeatables )
					{
						pQuestIcon->SetToolTip( "Quest is avaliable! (Repeatable)" );
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable_blue.png" ) );
					}
					else
					{
						pQuestIcon->SetToolTip( "Quest is avaliable!" );
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable.png" ) );
					}


				}
				else if ( quest.eQuestState == EQuestState::Done )
				{
					pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_check.png" ) );
					pQuestIcon->SetToolTip( "Quest has been handed in" );
				}
				else if ( quest.eQuestState == EQuestState::LevelReqNotMet || quest.eQuestState == EQuestState::LevelReqNotMetWithin5 )
				{
					pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_unavaliable.png" ) );
					pQuestIcon->SetToolTip( "Quest level requirement is not met" );
				}
				else if ( quest.eQuestState == EQuestState::RequiredQuestNotDone )
				{
					if ( bRepeatables )
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable_fade_blue.png" ) );
					else
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable_fade.png" ) );

					if (!IsNull(quest.szOtherQuestName))
						pQuestIcon->SetToolTip( FormatString("Requires quest '%s' to be done", quest.szOtherQuestName) );
					else
						pQuestIcon->SetToolTip( "A required quest is not done yet" );
				}
				else if ( quest.eQuestState == EQuestState::OtherNpcQuestTaken )
				{
					if ( bRepeatables )
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable_fade_blue.png" ) );
					else
						pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_avaliable_fade.png" ) );

					if (!IsNull(quest.szOtherQuestName))
						pQuestIcon->SetToolTip( FormatString("Currently doing quest '%s' for this NPC", quest.szOtherQuestName) );
					else
						pQuestIcon->SetToolTip( "Currently doing a quest for this NPC" );
				}
				else if ( quest.eQuestState == EQuestState::NotReadyYet )
				{
					pQuestIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_unavaliable.png" ) );
					pQuestIcon->SetToolTip( "Quest is not ready yet!" );
				}

				if ( pQuestIcon->GetImage() )
				{
					row->AddElement( pQuestIcon );
				}

				UI::Text_ptr questText = std::make_shared<UI::Text>( Rectangle2D( 32, 0, GetQuestListRowWidth() - 40, iQuestListRowHeight ) );
				//questText->SetID( QuestBookHandler::EElementID::Row_QuestID + i );

				if ( quest.iQuestID == iCurrentViewingQuestID )
					questText->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
				else
					questText->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );

				questText->SetText( quest.szQuestName );

				if ( quest.eQuestState == EQuestState::RequiredQuestNotDone ||
				     quest.eQuestState == EQuestState::OtherNpcQuestTaken ||
				     quest.eQuestState == EQuestState::LevelReqNotMet ||
				     quest.eQuestState == EQuestState::LevelReqNotMetWithin5 ||
					 quest.eQuestState == EQuestState::Done ||
					 quest.eQuestState == EQuestState::NotReadyYet )
				{
					if ( quest.iQuestID == iCurrentViewingQuestID )
					{
						questText->SetColor( D3DCOLOR_ARGB( 255, 1176, 100, 0 ) );
						questText->SetColorHover( D3DCOLOR_ARGB( 255, 176, 100, 0 ) );
					}
					else
					{
						questText->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );
						questText->SetColorHover( D3DCOLOR_ARGB( 255, 176, 100, 0 ) );
					}
				}
				else
				{
					if ( quest.iQuestID == iCurrentViewingQuestID )
					{
						questText->SetColor( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
						questText->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
					}
					else
					{
						questText->SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
						questText->SetColorHover( D3DCOLOR_ARGB( 255, 255, 165, 0 ) );
					}
				}

				questText->SetNoClip( TRUE );
				questText->SetHorizontalAlign( EAlign::ALIGN_Left );
				questText->SetVerticalAlign( EAlign::ALIGN_Middle );
				questText->SetEvent(UI::Event::Build(std::bind(&QuestBookHandler::OnQuestClick, this, quest.iQuestID, std::placeholders::_1)));

				row->AddElement( questText );

				UI::Text_ptr levelText = std::make_shared<UI::Text>( Rectangle2D( 32 + GetQuestListRowWidth() - 90, 0, 40, iQuestListRowHeight ) );
				//questText->SetID( QuestBookHandler::EElementID::Row_QuestID + i );
				levelText->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
				levelText->SetText( FormatString( "%d", quest.sQuestLevelMin ) );
				levelText->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );
				levelText->SetNoClip( TRUE );
				levelText->SetHorizontalAlign( EAlign::ALIGN_Right );
				levelText->SetVerticalAlign( EAlign::ALIGN_Middle );


				row->AddElement( levelText );
			}
		}
	}

	iTotalRowHeight -= iRowPadding; //remove last row padding for more accurate calc

	if ( iTotalRowHeight > fVisibleHeight )
	{
		float newScrollableHeight = iTotalRowHeight - fVisibleHeight;
		float newRowPos = newScrollableHeight * oldScrollPercent * ( oldScrollableHeight / newScrollableHeight );
		float newScrollPer = newRowPos / newScrollableHeight;

		if ( newScrollPer > 1.0 )
			newScrollPer = 1.0;

		questsList->SetScrollPercentage( newScrollPer * 100.f );
	}
	else
	{
		questsList->SetScrollPercentage( 0.0f );
	}
}

const std::string QuestBookHandler::GetQuestStateString( EQuestState eQuestStatus )
{
	switch ( eQuestStatus )
	{
		case EQuestState::Completed:				return "Quest is completed!";
		case EQuestState::InProgress:				return "Quest is in progress";
		case EQuestState::Expired:					return "Quest is now avaliable!";
		case EQuestState::Avaliable:				return "Quest is avaliable!";
		case EQuestState::RequiredQuestNotDone:		return "Required quest not done";
		case EQuestState::OtherNpcQuestTaken:		return "Undertaking another quest from this NPC";
		case EQuestState::LevelReqNotMet:			return "Level requirement not met";
		case EQuestState::LevelReqNotMetWithin5:	return "Level requirement not yet met";
		case EQuestState::NotReadyYet:				return "Pending daily reset";
		case EQuestState::Done:						return "Done";
	}

	return "-";
}

void QuestBookHandler::HandlePacket(PacketQuestDataResponse* psPacket)
{
	dwRequestTick = 0;

	PacketQuestDataResponse copy;
	ZeroMemory( &copy, sizeof( PacketQuestDataResponse ) );
	CopyMemory( &copy, psPacket, psPacket->iLength );
	mQuestDataResponse.emplace( std::make_pair( psPacket->iQuestID, copy ) );

	iCurrentViewingQuestID = psPacket->iQuestID;

	BuildQuestList();
	BuildQuestDataView( &mQuestDataResponse[iCurrentViewingQuestID] );
}

void QuestBookHandler::RebuildCurrentQuestDataView( )
{
	if ( iCurrentViewingQuestID == 0 )
		return;

	auto it = mQuestDataResponse.find( iCurrentViewingQuestID );
	if ( it == mQuestDataResponse.end() )
	{
		RequestQuestData( iCurrentViewingQuestID );
		return;
	}

	BuildQuestDataView( &it->second );
}

void QuestBookHandler::BuildQuestDataView( PacketQuestDataResponse * psPacket )
{
	if ( psPacket == NULL )
		return;

	if ( !IsWindowOpen() )
		return;

	auto iter = std::find_if(vQuestEntries.begin(), vQuestEntries.end(),
		[&]( const QuestBookEntry & ts ) { return ts.iQuestID == psPacket->iQuestID; } );

	if ( iter == vQuestEntries.end() )
		return;

	rightBoxSubRegion->Clear();

	const int iColumn1PosX = 0;
	const int iColumn2PosX = 35;

	const int iFontSize_Title = 28;
	const int iFontSize_Level = 16;
	const int iFontSize_SubTitle = 20;
	const int iFontSize_Text = 20;


	auto iColorActive = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	auto iColorInActive = D3DCOLOR_RGBA( 161, 183, 204, 255 );

	int iYPos = 0;

	int iMaxTextWidth = rightBoxList->GetWidth() - iRightBoxListPadding - iColumn2PosX;
	int iMaxTitleWidth = rightBoxList->GetWidth() - iRightBoxListPadding;

	UI::Text_ptr titleText = std::make_shared<UI::Text>( Rectangle2D( iColumn1PosX, iYPos, iMaxTitleWidth, 20 ) );
	titleText->SetFont( "Arvo", 30, 0, FALSE, TRUE, iColorActive );
	titleText->SetText( iter->szQuestName );
	titleText->SetWordWrap( TRUE );
	titleText->SetMultiLine( TRUE );
	titleText->SetHorizontalAlign( EAlign::ALIGN_Left );
	titleText->SetVerticalAlign( EAlign::ALIGN_Top );
	rightBoxSubRegion->AddElement( titleText );

	iYPos += titleText->GetHeightText();
	iYPos += 5;

	UI::Text_ptr levelAndStatusText = std::make_shared<UI::Text>( Rectangle2D( iColumn1PosX, iYPos, iMaxTitleWidth, 20 ) );
	levelAndStatusText->SetFont( "Arvo", iFontSize_Level, 0, FALSE, FALSE, iColorInActive );
	levelAndStatusText->SetText( FormatString( "Level %d / %s", iter->sQuestLevelMin, GetQuestStateString( iter->eQuestState ).c_str() ) );
	levelAndStatusText->SetNoClip( TRUE );
	levelAndStatusText->SetHorizontalAlign( EAlign::ALIGN_Left );
	levelAndStatusText->SetVerticalAlign( EAlign::ALIGN_Top );
	rightBoxSubRegion->AddElement( levelAndStatusText );

	iYPos += levelAndStatusText->GetHeightText();
	iYPos += 30;


	BOOL bIsDone = iter->eQuestState == EQuestState::Done;
	BOOL bIsInProgress = iter->eQuestState == EQuestState::InProgress;
	BOOL bIsCompleted = iter->eQuestState == EQuestState::Completed;
	BOOL bIsStartPending = iter->eQuestState == EQuestState::Avaliable || iter->eQuestState == EQuestState::Expired;


	if ( !bIsDone )
	{

		UI::Text_ptr descriptionText = std::make_shared<UI::Text>( Rectangle2D( iColumn1PosX, iYPos, iMaxTitleWidth, 20 ) );
		descriptionText->SetFont( "Arvo", iFontSize_SubTitle, 0, FALSE, TRUE, iColorActive );
		descriptionText->SetText( "Description" );
		descriptionText->SetNoClip( TRUE );
		descriptionText->SetHorizontalAlign( EAlign::ALIGN_Left );
		descriptionText->SetVerticalAlign( EAlign::ALIGN_Top );
		rightBoxSubRegion->AddElement( descriptionText );

		iYPos += descriptionText->GetHeightText();
		iYPos += 10;
	}

	QuestData * pcActiveQuest = CQUESTGAME->GetQuestInstance()->GetQuest( psPacket->iQuestID );

	if ( !IsNull( psPacket->szStartText ) && !bIsDone && !bIsInProgress && !bIsCompleted  )
	{
		UI::ImageBox_ptr pStartIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
		pStartIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_msg.png" ) );
		rightBoxSubRegion->AddElement( pStartIcon );

		iYPos += 3;
		UI::Text_ptr startText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );

		startText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsStartPending, bIsStartPending ? iColorActive : iColorInActive );
		startText->SetText( psPacket->szStartText );
		startText->SetWordWrap( TRUE );
		startText->SetMultiLine( TRUE );
		startText->SetHorizontalAlign( EAlign::ALIGN_Left );
		startText->SetVerticalAlign( EAlign::ALIGN_Top );
		rightBoxSubRegion->AddElement( startText );

		iYPos += startText->GetHeightText();
		iYPos += 10;
	}


	if ( !bIsDone && !bIsCompleted )
	{
		if ( !IsNull( psPacket->szTipText ) )
		{
			UI::ImageBox_ptr pMsgIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pMsgIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_msg.png" ) );
			rightBoxSubRegion->AddElement( pMsgIcon );

			iYPos += 3;
			UI::Text_ptr tipText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			tipText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsInProgress, bIsInProgress ? iColorActive : iColorInActive );

			tipText->SetText( psPacket->szTipText );

			tipText->SetWordWrap( TRUE );
			tipText->SetMultiLine( TRUE );
			tipText->SetHorizontalAlign( EAlign::ALIGN_Left );
			tipText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( tipText );

			iYPos += tipText->GetHeightText();
			iYPos += 10;
		}

		if ( psPacket->bNumOfReqMonsters > 0 )
		{
			if ( psPacket->sMapID >= 0 && psPacket->sMapID < NUM_MAPS )
			{
				UI::ImageBox_ptr pTargetIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
				pTargetIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_target.png" ) );
				rightBoxSubRegion->AddElement( pTargetIcon );

				iYPos += 3;
				UI::Text_ptr targetText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
				targetText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsInProgress, bIsInProgress ? iColorActive : iColorInActive );

				if ( psPacket->sMapID >= 0 && psPacket->sMapID < NUM_MAPS )
				{
					targetText->SetText( FormatString( "Hunt at %s", pszaMapsName[psPacket->sMapID] ) );
				}

				targetText->SetWordWrap( TRUE );
				targetText->SetMultiLine( TRUE );
				targetText->SetHorizontalAlign( EAlign::ALIGN_Left );
				targetText->SetVerticalAlign( EAlign::ALIGN_Top );
				rightBoxSubRegion->AddElement( targetText );

				iYPos += targetText->GetHeightText();
				iYPos += 10;
			}
		}

		for ( int i = 0; i < psPacket->bNumOfReqMonsters; i++ )
		{
			UI::ImageBox_ptr pTargetIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pTargetIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_target.png" ) );
			rightBoxSubRegion->AddElement( pTargetIcon );

			iYPos += 3;
			UI::Text_ptr targetText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			targetText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsInProgress, bIsInProgress ? iColorActive : iColorInActive );

			if ( psPacket->saReqMonstersCounts[i] > 0 )
			{
				const int totalKillsDone = bIsDone ? psPacket->saReqMonstersCounts[i] : ( pcActiveQuest == NULL ? 0 : pcActiveQuest->GetMonsterKilledAmount( i ) );

				if ( IsNull( psPacket->szaReqMonsterNames[i] ) )
					targetText->SetText( FormatString( "Monsters (%d/%d)", totalKillsDone, psPacket->saReqMonstersCounts[i] ) );
				else
					targetText->SetText( FormatString( "%s (%d/%d)", psPacket->szaReqMonsterNames[i], totalKillsDone, psPacket->saReqMonstersCounts[i] ) );
			}
			else
			{
				//for tip purposes
				if ( IsNull( psPacket->szaReqMonsterNames[i] ) )
					targetText->SetText( "Hunt Monsters" );
				else
					targetText->SetText( FormatString( "Hunt %s", psPacket->szaReqMonsterNames[i] ) );
			}

			targetText->SetWordWrap( TRUE );
			targetText->SetMultiLine( TRUE );
			targetText->SetHorizontalAlign( EAlign::ALIGN_Left );
			targetText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( targetText );

			iYPos += targetText->GetHeightText();
			iYPos += 10;
		}

		for ( int i = 0; i < psPacket->bNumOfReqItems; i++ )
		{
			UI::ImageBox_ptr pItemIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pItemIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_item.png" ) );
			rightBoxSubRegion->AddElement( pItemIcon );

			iYPos += 3;
			UI::Text_ptr targetText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			targetText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsInProgress, bIsInProgress ? iColorActive : iColorInActive );


			const int iTotalItemCount = bIsDone ? psPacket->saReqItemsCounts[i] : ( pcActiveQuest == NULL ? 0 : pcActiveQuest->GetItemsCollected( i ) );

			auto lsItemData = ITEMHANDLER->FindItemTableByCode( psPacket->iaReqItemIds[i] );
			if ( lsItemData )
				targetText->SetText( FormatString( "%s (%d/%d)", lsItemData->szBaseName, iTotalItemCount, psPacket->saReqItemsCounts[i] ) );
			else
				targetText->SetText( FormatString( "Item (%d/%d)", iTotalItemCount, psPacket->saReqItemsCounts[i] ) );


			targetText->SetWordWrap( TRUE );
			targetText->SetMultiLine( TRUE );
			targetText->SetHorizontalAlign( EAlign::ALIGN_Left );
			targetText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( targetText );

			iYPos += targetText->GetHeightText();
			iYPos += 10;
		}
	}

	if ( !bIsDone )
	{
		if ( !IsNull( psPacket->szEndText ) )
		{
			UI::ImageBox_ptr pEndIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pEndIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_msg.png" ) );
			rightBoxSubRegion->AddElement( pEndIcon );

			iYPos += 3;
			UI::Text_ptr endText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			endText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, bIsCompleted, bIsCompleted ? iColorActive : iColorInActive );
			endText->SetText( psPacket->szEndText );
			endText->SetWordWrap( TRUE );
			endText->SetMultiLine( TRUE );
			endText->SetHorizontalAlign( EAlign::ALIGN_Left );
			endText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( endText );

			iYPos += endText->GetHeightText();
			iYPos += 10;
		}
	}

	if (!bIsDone )
		iYPos += 20;

	if ( psPacket->iExpReward > 0 ||
		 psPacket->bItemCount > 0 ||
		 psPacket->sExtraRewardCount > 0 )
	{
		UI::Text_ptr questRewardsText = std::make_shared<UI::Text>( Rectangle2D( iColumn1PosX, iYPos, iMaxTitleWidth, 20 ) );
		questRewardsText->SetFont( "Arvo", iFontSize_SubTitle, 0, FALSE, TRUE, iColorActive );
		questRewardsText->SetText( "Quest Rewards" );
		questRewardsText->SetNoClip( TRUE );
		questRewardsText->SetHorizontalAlign( EAlign::ALIGN_Left );
		questRewardsText->SetVerticalAlign( EAlign::ALIGN_Top );
		rightBoxSubRegion->AddElement( questRewardsText );

		iYPos += questRewardsText->GetHeightText();
		iYPos += 5;


		if ( psPacket->iExpReward > 0 )
		{
			UI::ImageBox_ptr pExpIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pExpIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_fist.png" ) );
			rightBoxSubRegion->AddElement( pExpIcon );

			iYPos += 3;
			UI::Text_ptr expRewardText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			expRewardText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, FALSE, bIsDone ? iColorInActive : iColorActive );
			expRewardText->SetText( FormatString( "%s XP", FormatNumber( psPacket->iExpReward ) ) );
			expRewardText->SetNoClip( TRUE );
			expRewardText->SetHorizontalAlign( EAlign::ALIGN_Left );
			expRewardText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( expRewardText );

			iYPos += expRewardText->GetHeightText();
			iYPos += 10;
		}

		for ( int i_Extra = 0; i_Extra < psPacket->sExtraRewardCount; i_Extra++ )
		{
			if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::Gold )
			{
				UI::ImageBox_ptr pExpIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
				pExpIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_coins.png" ) );
				rightBoxSubRegion->AddElement( pExpIcon );

				iYPos += 3;
				UI::Text_ptr rewardText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
				rewardText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, FALSE, bIsDone ? iColorInActive : iColorActive );
				rewardText->SetText( FormatString( "%s GP", FormatNumber( psPacket->uExtraRewardValue[i_Extra] ) ) );
				rewardText->SetNoClip( TRUE );
				rewardText->SetHorizontalAlign( EAlign::ALIGN_Left );
				rewardText->SetVerticalAlign( EAlign::ALIGN_Top );
				rightBoxSubRegion->AddElement( rewardText );

				iYPos += rewardText->GetHeightText();
				iYPos += 10;
			}
			if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::WarehousePage )
			{
				UI::ImageBox_ptr pItemIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
				pItemIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_item.png" ) );
				rightBoxSubRegion->AddElement( pItemIcon );

				iYPos += 3;
				UI::Text_ptr rewardText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
				rewardText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, FALSE, bIsDone ? iColorInActive : iColorActive );
				rewardText->SetText( FormatString("Warehouse page: %d", (psPacket->uExtraRewardValue[i_Extra] + 1) ) );
				rewardText->SetNoClip( TRUE );
				rewardText->SetHorizontalAlign( EAlign::ALIGN_Left );
				rewardText->SetVerticalAlign( EAlign::ALIGN_Top );
				rightBoxSubRegion->AddElement( rewardText );

				iYPos += rewardText->GetHeightText();
				iYPos += 10;
			}
			else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::StatPoint ||
					  psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::StatPointPerLvl ||
					  psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::SkillPoint ||
					  psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::DecreaseDeathPlenty ||
					  psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::ExtraHealth ||
					  psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::RankUp )
			{
				UI::ImageBox_ptr pExpIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
				pExpIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_fist.png" ) );
				rightBoxSubRegion->AddElement( pExpIcon );

				iYPos += 3;
				UI::Text_ptr expRewardText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
				expRewardText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, FALSE, bIsDone ? iColorInActive : iColorActive );

				if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::StatPoint )
					expRewardText->SetText( FormatString( "%d extra stat points", psPacket->uExtraRewardValue[i_Extra] ) );
				else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::SkillPoint )
				{
					if ( psPacket->uExtraRewardValue[i_Extra] <= 1 )
						expRewardText->SetText( FormatString( "%d skill point", psPacket->uExtraRewardValue[i_Extra] ) );
					else
						expRewardText->SetText( FormatString( "%d skill points", psPacket->uExtraRewardValue[i_Extra] ) );
				}
				else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::StatPointPerLvl )
					expRewardText->SetText( FormatString( "%d extra stat points per lvl", psPacket->uExtraRewardValue[i_Extra] ) );
				else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::DecreaseDeathPlenty )
					expRewardText->SetText( FormatString( "%d%% decreased dealth plenty", psPacket->uExtraRewardValue[i_Extra] ) );
				else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::ExtraHealth )
					expRewardText->SetText( FormatString( "%d extra health point", psPacket->uExtraRewardValue[i_Extra] ) );
				else if ( psPacket->sExtraRewardType[i_Extra] == EQuestExtraRewardType::RankUp )
				{
					typedef JOB_DATA_CODE*( __cdecl *t_GetJobDataCode ) ( DWORD BaseJob	, int JobCnt );
					t_GetJobDataCode GetJobDataCode = ( t_GetJobDataCode )0x0044D0F0;
					auto jobDataCode = GetJobDataCode( PLAYERDATA->iClass, psPacket->uExtraRewardValue[i_Extra] ); //skill object
					char * invJobName = (char *)0x3625694;

					expRewardText->SetText( FormatString( "Class tier #%d (%s)", ( psPacket->uExtraRewardValue[i_Extra] + 1 ), jobDataCode->szName ) );
				}

				expRewardText->SetNoClip( TRUE );
				expRewardText->SetHorizontalAlign( EAlign::ALIGN_Left );
				expRewardText->SetVerticalAlign( EAlign::ALIGN_Top );
				rightBoxSubRegion->AddElement( expRewardText );

				iYPos += expRewardText->GetHeightText();
				iYPos += 10;
			}
		}



		int iColCount = 0;
		int iItemX = iColumn1PosX;

		if ( psPacket->bItemCount >= 1 )
		{
			UI::ImageBox_ptr pItemIcon = std::make_shared<UI::ImageBox>( Rectangle2D( iColumn1PosX, iYPos, 30, 29 ) );
			pItemIcon->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\Quest\\icon_item.png" ) );
			rightBoxSubRegion->AddElement( pItemIcon );

			iYPos += 3;
			UI::Text_ptr remarkText = std::make_shared<UI::Text>( Rectangle2D( iColumn2PosX, iYPos, iMaxTextWidth, 20 ) );
			remarkText->SetFont( "Open Sans", iFontSize_Text, 0, FALSE, FALSE, bIsDone ? iColorInActive : iColorActive );

			if ( psPacket->bItemCount == 1 )
			{
				if ( psPacket->saRewardItem[0].sItemID.ToItemBase() == EItemBase::ITEMBASE_Potion )
					remarkText->SetText( FormatString( "%s (%d)", psPacket->saRewardItem[0].szItemName, psPacket->saRewardItem[0].iPotionCount ) );
				else
					remarkText->SetText( psPacket->saRewardItem[0].szItemName );
			}
			else if ( psPacket->bItemSelect )
				remarkText->SetText( "Select 1 from the following:" );
			else
				remarkText->SetText( "Multiple items" );

			remarkText->SetNoClip( TRUE );
			remarkText->SetHorizontalAlign( EAlign::ALIGN_Left );
			remarkText->SetVerticalAlign( EAlign::ALIGN_Top );
			rightBoxSubRegion->AddElement( remarkText );

			iYPos += remarkText->GetHeightText();
			iYPos += 10;
		}


		int iMaxRowHeight = 0;
		for ( int i = 0; i < psPacket->bItemCount; i++ )
		{
			if ( iMaxRowHeight == 0 )
			{
				for ( int j = 0; j < psPacket->bItemCount; j++ )
				{
					ItemData sItemData;
					if ( ITEMHANDLER->LoadItemImage( &psPacket->saRewardItem[j], &sItemData ) )
					{
						if ( sItemData.iHeight > iMaxRowHeight )
							iMaxRowHeight = sItemData.iHeight;
					}
				}
			}

			ItemData sItemData;
			if ( ITEMHANDLER->LoadItemImage( &psPacket->saRewardItem[i], &sItemData ) )
			{
				int iItemWidth = sItemData.iWidth;

				UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( iItemX, iYPos, iItemWidth / 22, iMaxRowHeight / 22 );
				pItemBox->SetID( 1 );
				pItemBox->SetMaxItems( 1 );
				pItemBox->DisableMouse();
				pItemBox->SetFreePlace ( FALSE );
				pItemBox->AddItem( &sItemData, 0, 0, 0, 0, FALSE );

				iItemX += iItemWidth + 3;

				rightBoxSubRegion->AddElement( pItemBox );

				iColCount++;

				if ( iItemX > rightBoxSubRegion->GetWidth() - iColumn1PosX || (i == psPacket->bItemCount - 1) )
				{
					iColCount = 0;
					iYPos += iMaxRowHeight + 5;
					iMaxRowHeight = 0;
				}
			}
		}
	}

	rightBoxSubRegion->SetHeight( iYPos + 10 );
}


BOOL QuestBookHandler::CanProcessRequest()
{
	if ( dwRequestTick > 0 && ( TICKCOUNT - dwRequestTick ) < 500 )
		return FALSE;

	return TRUE;
}

BOOL QuestBookHandler::IsWindowOpen()
{
	if ( questBookWindow == NULL )
		return FALSE;

	return questBookWindow->IsOpen();
}

void QuestBookHandler::OnQuestClick(int iID, UIEventArgs e)
{
	auto it = mQuestDataResponse.find( iID );
	if ( it != mQuestDataResponse.end() )
	{
		iCurrentViewingQuestID = iID;
		BuildQuestList();
		BuildQuestDataView( &it->second );
		return;
	}

	if ( !IsWindowOpen() )
		return;

	RequestQuestData( iID );
}

void QuestBookHandler::RequestQuestData( int iQuestID )
{
	if ( !IsWindowOpen() )
		return;

	if ( !CanProcessRequest() )
		return;

	PacketQuestDataRequest questRequest;

	questRequest.iLength = sizeof(PacketQuestDataRequest);
	questRequest.iHeader = PKTHDR_QuestDataRequest;
	questRequest.questID = iQuestID;

	SENDPACKETL(&questRequest, TRUE);

	rightBoxSubRegion->Clear();
	dwRequestTick = TICKCOUNT;
}

