#include "stdafx.h"
#include "UIMessageBallon.h"

extern BOOL bDisableMessageBox;



namespace UI
{
MessageBalloon::MessageBalloon( Rectangle2D rRectangle ) : Element(rRectangle)
{
}

MessageBalloon::~MessageBalloon()
{
	vCache.clear();
}

void MessageBalloon::Init()
{
	//Normal Skin
	saMessagesBalloon[SKINID_Normal][0].pImageLeftTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageCenter		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageLeftBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRightBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageTailBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTail.png" );

	//Blue Skin
	saMessagesBalloon[SKINID_Blue][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopMiddle.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopCenter.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomMiddle.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTail.png" );

	//Common Skin
	saMessagesBalloon[SKINID_Common][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Common][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Common][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Common][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Common][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Common][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageRight.png" );
	saMessagesBalloon[SKINID_Common][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Common][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Common][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Common][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Common\\MessageTail.png" );

	//Uncommon Skin
	saMessagesBalloon[SKINID_Uncommon][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageRight.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Uncommon][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Uncommon\\MessageTail.png" );

	//Rare Skin
	saMessagesBalloon[SKINID_Rare][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageRight.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Rare][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Rare\\MessageTail.png" );

	//Epic Skin
	saMessagesBalloon[SKINID_Epic][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageRight.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Epic][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Epic\\MessageTail.png" );

	//Legendary Skin
	saMessagesBalloon[SKINID_Legendary][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageRight.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Legendary][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\Legendary\\MessageTail.png" );


	pcaBellatraCrown[0]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_01.tga" );
	pcaBellatraCrown[1]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_02.tga" );
	pcaBellatraCrown[2]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_03.tga" );
	pcaBellatraCrown[3]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_01_humor.tga" );
	pcaBellatraCrown[4]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_02_humor.tga" );
	pcaBellatraCrown[5]								= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_03_humor.tga" );


	pcaBellatraCrownSoloSword[0]					= ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\BellaSolo_01s.png");
	pcaBellatraCrownSoloSword[1]					= ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\BellaSolo_02s.png");
	pcaBellatraCrownSoloSword[2]					= ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\BellaSolo_03s.png");

	pcaClassIcon14Pixel[CHARACTERCLASS_Archer		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_as_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Assassin		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_ass_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Atalanta		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_ata_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Fighter		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_fs_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Knight		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_ks_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Magician		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_mg_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Mechanician	- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_ms_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Priestess	- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_prs_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Pikeman		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_ps_14.png");
	pcaClassIcon14Pixel[CHARACTERCLASS_Shaman		- 1 ] = ImageLoader::LoadImage("StartImage\\MessageBox\\BellaBox\\class_sh_14.png");

	// IMG NOTIFICACAO QUEST XXSTR
	pcNotifyQuest[0] = ImageLoader::LoadImage("game\\images\\hud\\notify_quest.png");
	pcNotifyQuest[1] = ImageLoader::LoadImage("game\\images\\hud\\progress_quest.png");
	pcNotifyQuest[2] = ImageLoader::LoadImage("game\\images\\hud\\complete_quest.png");
	pcNotifyQuest[3] = ImageLoader::LoadImage("game\\images\\hud\\high_quest.png");
	pcNotifyQuest[4] = ImageLoader::LoadImage("game\\images\\hud\\notify_repeatable_quest.png");
	pcNotifyQuest[5] = ImageLoader::LoadImage("game\\images\\hud\\complete_repeatable_quest.png");

	pTextMessage = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, -1 );
	pTextMessage->SetMaxTextWidth( 200 );
	pTextMessage->SetNoClip( TRUE );
	pTextMessage->SetMultiLine( TRUE );
	pTextMessage->SetFont ( "Noto Sans", 16, 0, FALSE, FALSE, -1 );

	pTextClanName = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, D3DCOLOR_XRGB( 150, 255, 250 ) );
	pTextClanName->SetMaxTextWidth( 200 );
	pTextClanName->SetNoClip( TRUE );
	pTextClanName->SetMultiLine( TRUE );
	pTextClanName->SetFont ( "Noto Sans", 16, 0, FALSE, FALSE, -1 );

	pTextPetName = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, -1 );
	pTextPetName->SetMaxTextWidth( 200 );
	pTextPetName->SetNoClip( TRUE );
	pTextPetName->SetMultiLine( TRUE );
	pTextPetName->SetFont ( "Noto Sans", 16, 0, FALSE, FALSE, -1 );

	pTextTitle = std::make_shared<Text>(Rectangle2D(rRect), FALSE, -1);
	pTextTitle->SetMaxTextWidth(200);
	pTextTitle->SetNoClip(TRUE);
	pTextTitle->SetMultiLine(TRUE);
	pTextTitle->SetFont ( "Noto Sans", 16, 0, FALSE, FALSE, -1 );

	iBellatraCrown = -1;
	sBellatraSoloCrown = -1;
	eCharClass = ECharacterClass::CHARACTERCLASS_None;

	eNotifyQuest = EQuestIcon::None;

	bHover = FALSE;
	bIsMessage = FALSE;
	bIsCharacterMessage = FALSE;
	bShowClanIcon = FALSE;
	bClanName = FALSE;
	bPetName = FALSE;
	bHasTitle = FALSE;
	psTextureClanIcon = NULL;

}

void MessageBalloon::SetMessage( std::string strMessage )
{
	if( strMessage.find( ":" ) != std::string::npos )
	{
		if( bShowClanIcon )
			strMessage.insert( 0, "      " );

		bIsMessage = TRUE;
		pTextMessage->SetHighlightText( TRUE );
		pTextMessage->SetHighlightTextColor( D3DCOLOR_XRGB( 240, 180, 100 ) );
	}

	//Get Message
	if ( auto & it = vCache.find( std::hash<std::string>{}(strMessage) ); it != vCache.end() )
	{
		pTextMessage->SetText( it->second.strText, false );

		if ( pTextMessage->GetIsHighlightText() )
		{
			pTextMessage->SetHighlightTextWidthBuild( it->second.iHighlightWidth );

			pTextMessage->SetHighlightTextBuild( strMessage.substr( 0, strMessage.find( ":" ) + 1 ) );
		}

		it->second.dwTimeOut = TICKCOUNT + TIME_STRINGHASHBALLON_UPDATE;
	}
	else
	{
		pTextMessage->SetText( strMessage );
		//pTextMessage->SetFont ( "Arial", pTextMessage->GetHeightFont(), 0, FALSE, TRUE,pTextMessage->GetColor() ); //Make Bold
		vCache.insert( std::pair<std::size_t, StringBalloonHash>( std::hash<std::string>{}(strMessage), StringBalloonHash( pTextMessage->GetText(), pTextMessage->GetHighlightTextWidthBuild(), TICKCOUNT + TIME_STRINGHASHBALLON_UPDATE ) ) );
	}
}

void MessageBalloon::SetClanName(std::string strText)
{
	//strText.insert(0, "          ");
	pTextClanName->SetText(strText);
	pTextMessage->GetText() = trim(pTextMessage->GetText());
	bClanName = TRUE;
}

void MessageBalloon::SetPetName( std::string strText, DWORD dwColor )
{
	pTextPetName->SetText( strText );
	pTextPetName->SetColor( dwColor );
	bPetName = TRUE;
}


void MessageBalloon::AddCharTitleText(char* strText, int iRarity)
{
	if ( iRarity == 0 )
		return;

	DWORD dwColor = RGBA(255, 255, 255, 200);	//Default color if no rarity

	switch (iRarity)
	{
		case 1:
			dwColor = RGBA(255, 255, 255, 200); //White (Default)
			break;
		case 2:
			dwColor = RGBA(255, 66, 255, 82);	//Green
			break;
		case 3:
			dwColor = RGBA(255, 71, 101, 255);	//Blue
			break;
		case 4:
			dwColor = RGBA(255, 228, 112, 255);	//Purple
			break;
		case 5:
			dwColor = RGBA(255, 255, 105, 30);	//Orange
			break;
		case 6:
			dwColor = RGBA(255, 91, 200, 255);	//Teal
			break;
	}

	pTextTitle->SetText(FormatString("< %s >", strText));
	pTextTitle->SetColor(dwColor);

	bHasTitle = TRUE;
}

void MessageBalloon::ResetMessage()
{
	pTextMessage->SetHighlightText( FALSE );
	bHover = FALSE;
	bIsMessage = FALSE;
	bIsCharacterMessage = FALSE;
	bShowClanIcon = FALSE;
	bClanName = FALSE;
	bPetName = FALSE;
	bHasTitle = FALSE;

	psTextureClanIcon = NULL;
	iBellatraCrown = -1;
	sBellatraSoloCrown = -1;
	eCharClass = ECharacterClass::CHARACTERCLASS_None;
	pTextClanName->SetText( "" );
	pTextMessage->SetText( "" );
	pTextPetName->SetText( "" );
	pTextTitle->SetText( "" );

	eNotifyQuest = EQuestIcon::None;
}

void MessageBalloon::DisplayBox ( BOOL bDisplay )
{
	bDisableMessageBox = TRUE;
}

void MessageBalloon::Render ( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( bDisableMessageBox )
		return;

	int iRenderX = GetX () + iX;
	int iRenderY = GetY () + iY;
	int iRenderWidth = low ( GetWidth (), iWidth - GetX () );
	int iRenderHeight = low ( GetHeight (), iHeight - GetY () );

	int iYBase = 0;
	int iXBase = 0;

	if ( iRenderX < 0 || iRenderX > RESOLUTION_WIDTH )
		return;

	if ( iRenderY < 0 || iRenderY > RESOLUTION_HEIGHT )
		return;

	int iMessageBalloonBoxWidth = pTextMessage->GetTextWidth ();
	int iMessageBalloonBoxHeight = pTextMessage->GetHeightText ();

	int iClanNameOffset = 0;

	int iLREdgeSpacing = 2;
	int iClanNameSpacing = 6;

	//if ( bShowClanIcon && !bIsMessage )
	//	iMessageBalloonBoxWidth += 40;

	//if (bIsMessage && !bShowClanIcon)
	//	iMessageBalloonBoxWidth += iLREdgeSpacing *2;

	if ( bClanName && bHasTitle )
	{
		int iLowerWidth = 0;
		int iClanWidth = 32;

		if ( pTextClanName->GetTextWidth () > pTextMessage->GetTextWidth () )
		{
			iLowerWidth = iLREdgeSpacing + iClanWidth + iClanNameSpacing + pTextClanName->GetTextWidth () + iLREdgeSpacing;
		}
		else
		{
			iLowerWidth = iLREdgeSpacing + iClanWidth + iClanNameSpacing + pTextMessage->GetTextWidth () + iLREdgeSpacing;
		}

		if ( iLowerWidth > pTextTitle->GetTextWidth () )
		{
			iMessageBalloonBoxWidth = iLowerWidth;
		}
		else
		{
			iMessageBalloonBoxWidth = iLREdgeSpacing + pTextTitle->GetTextWidth () + iLREdgeSpacing;
		}


		iClanNameOffset = ( iMessageBalloonBoxWidth - iLowerWidth ) / 2;


		iMessageBalloonBoxHeight += pTextClanName->GetHeightFont () + 2;

	}

	//New Width and Height
	if ( bClanName && !bHasTitle )
	{
		int iClanWidth = 32;
		if ( pTextClanName->GetTextWidth () > pTextMessage->GetTextWidth () )
		{
			iMessageBalloonBoxWidth = iLREdgeSpacing + iClanWidth + iClanNameSpacing + pTextClanName->GetTextWidth () + iLREdgeSpacing;
		}
		else
		{
			iMessageBalloonBoxWidth = iLREdgeSpacing + iClanWidth + iClanNameSpacing + pTextMessage->GetTextWidth () + iLREdgeSpacing;
		}

		iMessageBalloonBoxHeight += pTextClanName->GetHeightFont () + 2;

	}

	if ( !bClanName && !bHasTitle )
	{
		iMessageBalloonBoxWidth = iLREdgeSpacing + pTextMessage->GetTextWidth () + iLREdgeSpacing;
	}


	if ( bPetName && !bHasTitle )
	{
		if ( iMessageBalloonBoxWidth < pTextPetName->GetTextWidth () )
			iMessageBalloonBoxWidth = pTextPetName->GetTextWidth () + iLREdgeSpacing * 2;

		iMessageBalloonBoxHeight += pTextPetName->GetHeightFont () + 2;
	}



	if ( bHasTitle )
	{
		int iTitleWidth = iLREdgeSpacing + pTextTitle->GetTextWidth () + iLREdgeSpacing;

		if ( iMessageBalloonBoxWidth < iTitleWidth )
		{
			iMessageBalloonBoxWidth = iTitleWidth;
		}

		iMessageBalloonBoxHeight += pTextPetName->GetHeightFont () + 2;
	}

	if ( iMessageBalloonBoxWidth < 20 )
	{
		iMessageBalloonBoxWidth = 20;
		iXBase += 10 - ( pTextMessage->GetTextWidth () >> 1 );
	}




	//Render Box
	if ( eSkinID != SKINID_Transparent )
		DrawMessageBalloonBox ( iRenderX - iMessageBalloonBoxWidth / 2 - iLREdgeSpacing * 2, iRenderY - 4 - iMessageBalloonBoxHeight - 18, iMessageBalloonBoxWidth - iLREdgeSpacing * 2, iMessageBalloonBoxHeight );

	//Title Render
	if ( bHasTitle )
	{

		//ps->pText->Render(iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18, iWidth, iHeight, iSourceX, iSourceY);

		pTextTitle->Render ( iX - iMessageBalloonBoxWidth / 2 + ( ( iMessageBalloonBoxWidth - pTextTitle->GetTextWidth () + iLREdgeSpacing + iLREdgeSpacing ) / 2 ), iY - iMessageBalloonBoxHeight - 18, iWidth, iHeight, iSourceX, iSourceY );


		//ps->pText->Render(iX - (iMessageBalloonBoxWidth >> 1), iY - iMessageBalloonBoxHeight - 18, iWidth, iHeight, iSourceX, iSourceY);

		//ps->pText->Render(iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY);

		//if (iMessageBalloonBoxWidth > ps->pText->GetTextWidth())
		//	iXBase = (iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1);

		iYBase += pTextTitle->GetHeightFont ();
	}


	//Clan Icon
	if ( bShowClanIcon )
	{
		int iClanWidth = psTextureClanIcon->sTexture.iWidth;
		int iClanHeight = psTextureClanIcon->sTexture.iHeight;

		UI::ImageRender::Render ( psTextureClanIcon->sTexture.pcD3DTexture, iX - iMessageBalloonBoxWidth / 2 + (bIsMessage ? iLREdgeSpacing : (4 + iClanNameOffset)) , iY - iMessageBalloonBoxHeight - 17 + iYBase, iClanWidth, iClanHeight, 0, 0, -1, bIsMessage ? 0.5f : 1.0f );

		pTextClanName->Render(iX - iMessageBalloonBoxWidth / 2 + iLREdgeSpacing + psTextureClanIcon->sTexture.iWidth + iClanNameOffset + iClanNameSpacing, iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY);

		if ( !bIsMessage )
			iYBase += pTextClanName->GetHeightFont ();
	}


	if ( bPetName )
	{
		if ( iMessageBalloonBoxWidth > pTextPetName->GetTextWidth () )
			pTextPetName->Render ( iX - ( iMessageBalloonBoxWidth >> 1 ) + ( ( iMessageBalloonBoxWidth >> 1 ) - ( pTextPetName->GetTextWidth () >> 1 ) ), iY - iMessageBalloonBoxHeight - 2, iWidth, iHeight, iSourceX, iSourceY );
		else
			pTextPetName->Render ( iX - ( iMessageBalloonBoxWidth >> 1 ), iY - iMessageBalloonBoxHeight - 2, iWidth, iHeight, iSourceX, iSourceY );

		if ( iMessageBalloonBoxWidth > pTextMessage->GetTextWidth () )
			iXBase = ( iMessageBalloonBoxWidth >> 1 ) - ( pTextMessage->GetTextWidth () >> 1 );
	}


	if ( bShowClanIcon & !bIsMessage )
		pTextMessage->Render(iX - iMessageBalloonBoxWidth / 2 + iLREdgeSpacing + psTextureClanIcon->sTexture.iWidth + iClanNameOffset + iClanNameSpacing, iY - iMessageBalloonBoxHeight - 19 + iYBase, iWidth, iHeight, iSourceX, iSourceY);
	else if ( bShowClanIcon & bIsMessage )
		pTextMessage->Render(iX - iMessageBalloonBoxWidth / 2 + iLREdgeSpacing , iY - iMessageBalloonBoxHeight - 19 + iYBase, iWidth, iHeight, iSourceX, iSourceY);
	else
		pTextMessage->Render(iX - iMessageBalloonBoxWidth / 2 + ((iMessageBalloonBoxWidth - pTextMessage->GetTextWidth() + iLREdgeSpacing + iLREdgeSpacing) / 2), iY - iMessageBalloonBoxHeight - 19 + iYBase, iWidth, iHeight, iSourceX, iSourceY);


	iYBase += pTextMessage->GetHeightFont();

	UI::Image * pcImage = nullptr;

	switch ( eNotifyQuest )
	{
		case EQuestIcon::Avaliable:				pcImage = pcNotifyQuest[0]; break; // notify quest
		case EQuestIcon::Progress:				pcImage = pcNotifyQuest[1]; break; // progress
		case EQuestIcon::Complete:				pcImage = pcNotifyQuest[2]; break; // complete
		case EQuestIcon::Unavaliable:			pcImage = pcNotifyQuest[3]; break; // Gray !
		case EQuestIcon::AvaliableRepeatable:	pcImage = pcNotifyQuest[4]; break; // notify quest (blue)
		case EQuestIcon::CompleteRepeatable:	pcImage = pcNotifyQuest[5]; break; // progress (blue)
	}

	if ( pcImage )
	{
		int iIconX = iRenderX - (iMessageBalloonBoxWidth >> 1);
		int iIconY = iRenderY - iMessageBalloonBoxHeight - 18;

		UI::ImageRender::Render(pcImage, iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImage->GetWidth() >> 1), iIconY - pcImage->GetHeight() - 2, pcImage->GetWidth(), pcImage->GetHeight(), -1);
	}


	//Bellatra Icon
	if ( iBellatraCrown > 0 || sBellatraSoloCrown > 0)
	{
		int iOffsetX = 0;
		int iIconX = iRenderX - (iMessageBalloonBoxWidth >> 1); //center of message balloon
		int iIconY = iRenderY - iMessageBalloonBoxHeight - 18;

		if (iBellatraCrown > 0)
		{
			UI::Image* pcImage = pcaBellatraCrown[iBellatraCrown - 1];

			//only apply offset if the is two crowns
			if (sBellatraSoloCrown > 0)
			{
				iOffsetX = (pcImage->GetWidth() >> 1) - 3;
			}

			UI::ImageRender::Render(pcImage, iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImage->GetWidth() >> 1) - iOffsetX, iIconY - pcImage->GetHeight() - 2, pcImage->GetWidth(), pcImage->GetHeight(), -1);
		}

		if (sBellatraSoloCrown > 0 && eCharClass > 0)
		{
			UI::Image* pcImageSolo = pcaBellatraCrown[sBellatraSoloCrown - 1];

			//humor mode
			if (sBellatraSoloCrown > 3)
			{
				UI::ImageRender::Render(pcImageSolo, iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImageSolo->GetWidth() >> 1), iIconY - pcImageSolo->GetHeight() - 2, pcImageSolo->GetWidth(), pcImageSolo->GetHeight(), -1);
			}
			else
			{
				UI::Image* pcImageSoloSword = pcaBellatraCrownSoloSword[sBellatraSoloCrown - 1];
				UI::Image* pcClassImage = pcaClassIcon14Pixel[((int)eCharClass - 1)];

				if (pcImageSolo && pcImageSoloSword && pcClassImage)
				{
					UI::ImageRender::Render(pcImageSolo, iOffsetX + iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImageSolo->GetWidth() >> 1), iIconY - pcImageSolo->GetHeight() - 2, pcImageSolo->GetWidth(), pcImageSolo->GetHeight(), -1);
					UI::ImageRender::Render(pcClassImage, iOffsetX + iIconX + (iMessageBalloonBoxWidth >> 1) - (pcClassImage->GetWidth() >> 1), iIconY - pcClassImage->GetHeight() - 2 - 4, pcClassImage->GetWidth(), pcClassImage->GetHeight(), -1);
					UI::ImageRender::Render(pcImageSoloSword, iOffsetX + iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImageSoloSword->GetWidth() >> 1), iIconY - pcImageSoloSword->GetHeight() - 2, pcImageSoloSword->GetWidth(), pcImageSoloSword->GetHeight(), -1);
				}
			}
		}
	}

	Element::Render( iX, iY, iWidth, iHeight, iSourceX, iSourceY );
}

void MessageBalloon::SetColor( DWORD dwColor )
{
	pTextMessage->SetColor( dwColor );
}

void MessageBalloon::IsCharacterMessage( BOOL b )
{
	bIsCharacterMessage = b;
}

void MessageBalloon::Update( float fTime )
{
	for ( auto & it = vCache.begin(); it != vCache.end(); )
	{
		if ( it->second.dwTimeOut < TICKCOUNT )
			it = vCache.erase( it );
		else
			it++;
	}
}

void MessageBalloon::SetClanIconTexture( EXESurfaceOld * ps )
{
	psTextureClanIcon = ps;

	bShowClanIcon = TRUE;
}

void MessageBalloon::DrawMessageBalloonBox( int iX, int iY, int iWidth, int iHeight )
{
	if (bDisableMessageBox)
		return;

	//Check if have Hover Images
	if( !saMessagesBalloon[eSkinID][1].pImageLeftTop )
		bHover = 0;

	//Drawing Top
	if( saMessagesBalloon[eSkinID][bHover].pImageLeftTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeftTop, iX, iY, 16, 8, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageMiddleTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageMiddleTop, iX + 16, iY, iWidth - 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRightTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRightTop, iX + iWidth, iY, 16, 8, 0, 0, -1 );

	//Drawing Middle
	if( saMessagesBalloon[eSkinID][bHover].pImageLeft )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeft, iX, iY + 8, 16, iHeight - 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageCenter )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageCenter, iX + 16, iY + 8, iWidth - 16, iHeight - 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRight )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRight, iX + iWidth, iY + 8, 16, iHeight - 8, 0, 0, -1 );

	//Drawing Bottom
	if( saMessagesBalloon[eSkinID][bHover].pImageLeftBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeftBottom, iX, iY + iHeight, 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageMiddleBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageMiddleBottom, iX + 16, iY + iHeight, iWidth - 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRightBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRightBottom, iX + iWidth, iY + iHeight, 16, 8, 0, 0, -1 );

	//Drawing Tail Bottom
	if( saMessagesBalloon[eSkinID][bHover].pImageTailBottom && bIsMessage )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageTailBottom, iX + (iWidth >> 1), iY + iHeight + 5, 16, 16, 0, 5, -1 );
}
};
