#include "stdafx.h"
#include "UIItemInfoBox.h"


namespace UI
{
ItemInfoBox::ItemInfoBox()
{
	iSocketGem[0] = iSocketGem[1] = -1;
}

ItemInfoBox::~ItemInfoBox()
{
}

void ItemInfoBox::AddString( int iSide, const std::string & strString, DWORD dwColor, BOOL bBold, BOOL bItalic )
{
	std::vector<std::string> vString;

	//Find breakline in String
	BOOL bMultiLine = FALSE;
	if( strString.find( '\n' ) != std::string::npos )
	{
		vString = split( strString, '\n' );
		bMultiLine = TRUE;
	}

	switch( iSide )
	{
	case 0: //Left Side
		//Is Multiline?
		if( bMultiLine )
		{
			//String from vector
			int i = 0;
			for( const auto & strSplitted : vString )
			{
				vLeftText.push_back( std::make_shared<Text>( strSplitted, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor, bItalic ) );
				i++;

				if( i != vString.size() )
				{
					vRightText.push_back( std::make_shared<Text>( "", Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor, bItalic ) );
				}
			}
		}
		else
		{
			vLeftText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor, bItalic  ) );
		}

		break;

	case 1: //Right Side
		vRightText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor, bItalic ) );
		break;
	}
}

void ItemInfoBox::AddCompareString( const std::string & strString, DWORD dwColor )
{
	vRowCompare.push_back( vLeftText.size() );
	vCompareText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), FALSE, dwColor ) );
}

void ItemInfoBox::SetLineColor( DWORD dwColor, int iSide, int iLineCount )
{
	int iSetColor = 0;
	if( iLineCount < 0 )
		iSetColor = vLeftText.size() - 1;
	else
		iSetColor = iLineCount;

	switch( iSide )
	{
	case -1:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	case 0:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		break;
	case 1:
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	}
}

void ItemInfoBox::AppendLineText( std::string str, int iSide, int iLineCount )
{
	int iSetText = 0;
	if( iLineCount < 0 )
		iSetText = vLeftText.size() - 1;
	else
		iSetText = iLineCount;

	switch( iSide )
	{
		case -1:
			vLeftText[ iSetText ]->AppendText(str);
			vRightText[ iSetText ]->AppendText( str );
			break;
		case 0:
			vLeftText[ iSetText ]->AppendText( str );
			break;
		case 1:
			vRightText[ iSetText ]->AppendText( str );
			break;
	}
}

void ItemInfoBox::Init()
{
	//Load Gradient
	pImageGradient				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\gradient.png" );
	pImagePKey				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\p_key.png" );

	//Load Images
	pImageMaskLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\item\\topleft.png" );
	pImageMaskRightTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\item\\topright.png" );
	pImageMaskLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\item\\bottomleft.png" );
	pImageMaskRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\item\\bottomright.png" );

	//Load Border Images
	pImageBorderLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topleft.png" );
	pImageBorderRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topright.png" );
	pImageBorderTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\top.png" );
	pImageBorderRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomright.png" );
	pImageBorderLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomleft.png" );
	pImageBorderBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottom.png" );
	pImageBorderLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\left.png" );
	pImageBorderRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\right.png" );

	//Load Border None Images
	pImageBorderNoneLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\topleft.png" );
	pImageBorderNoneRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\topright.png" );
	pImageBorderNoneTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\top.png" );
	pImageBorderNoneRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\bottomright.png" );
	pImageBorderNoneLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\bottomleft.png" );
	pImageBorderNoneBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\bottom.png" );
	pImageBorderNoneLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\left.png" );
	pImageBorderNoneRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\right.png" );

	pImageBorderNoneDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\divider.png" );
	pImageBorderNoneInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\info.png" );
	pImageBorderNoneInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\infoleft.png" );
	pImageBorderNoneInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\infoRight.png" );

	pImageBorderNoneNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\nameframeleft.png" );
	pImageBorderNoneNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\nameframecenter.png" );
	pImageBorderNoneNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\nameframeright.png" );
	pImageBorderNoneNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\None\\nameframebackgroud.png" );

	//Load Border Common Images
	pImageBorderCommonLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\topleft.png" );
	pImageBorderCommonRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\topright.png" );
	pImageBorderCommonTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\top.png" );
	pImageBorderCommonRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\bottomright.png" );
	pImageBorderCommonLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\bottomleft.png" );
	pImageBorderCommonBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\bottom.png" );
	pImageBorderCommonLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\left.png" );
	pImageBorderCommonRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\right.png" );

	pImageBorderCommonDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\divider.png" );
	pImageBorderCommonInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\info.png" );
	pImageBorderCommonInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\infoleft.png" );
	pImageBorderCommonInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\infoRight.png" );

	pImageBorderCommonNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\nameframeleft.png" );
	pImageBorderCommonNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\nameframecenter.png" );
	pImageBorderCommonNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\nameframeright.png" );
	pImageBorderCommonNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Common\\nameframebackgroud.png" );

	//Load Border Uncommon Images
	pImageBorderUncommonLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\topleft.png" );
	pImageBorderUncommonRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\topright.png" );
	pImageBorderUncommonTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\top.png" );
	pImageBorderUncommonRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\bottomright.png" );
	pImageBorderUncommonLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\bottomleft.png" );
	pImageBorderUncommonBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\bottom.png" );
	pImageBorderUncommonLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\left.png" );
	pImageBorderUncommonRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\right.png" );

	pImageBorderUncommonDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\divider.png" );
	pImageBorderUncommonCorner				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\corner.png" );
	pImageBorderUncommonBottomCorner		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\bottomcorner.png" );
	pImageBorderUncommonInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\info.png" );
	pImageBorderUncommonInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\infoleft.png" );
	pImageBorderUncommonInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\infoRight.png" );

	pImageBorderUncommonNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\nameframeleft.png" );
	pImageBorderUncommonNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\nameframecenter.png" );
	pImageBorderUncommonNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\nameframeright.png" );
	pImageBorderUncommonNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Uncommon\\nameframebackgroud.png" );

	//Load Border Rare Images
	pImageBorderRareLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\topleft.png" );
	pImageBorderRareRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\topright.png" );
	pImageBorderRareTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\top.png" );
	pImageBorderRareRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\bottomright.png" );
	pImageBorderRareLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\bottomleft.png" );
	pImageBorderRareBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\bottom.png" );
	pImageBorderRareLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\left.png" );
	pImageBorderRareRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\right.png" );

	pImageBorderRareDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\divider.png" );
	pImageBorderRareCorner				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\corner.png" );
	pImageBorderRareBottomCorner		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\bottomcorner.png" );
	pImageBorderRareInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\info.png" );
	pImageBorderRareInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\infoleft.png" );
	pImageBorderRareInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\rare\\infoRight.png" );

	pImageBorderRareNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Rare\\nameframeleft.png" );
	pImageBorderRareNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Rare\\nameframecenter.png" );
	pImageBorderRareNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Rare\\nameframeright.png" );
	pImageBorderRareNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Rare\\nameframebackgroud.png" );

	//Load Border Epic Images
	pImageBorderEpicLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\topleft.png" );
	pImageBorderEpicRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\topright.png" );
	pImageBorderEpicTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\top.png" );
	pImageBorderEpicRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\bottomright.png" );
	pImageBorderEpicLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\bottomleft.png" );
	pImageBorderEpicBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\bottom.png" );
	pImageBorderEpicLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\left.png" );
	pImageBorderEpicRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\right.png" );

	pImageBorderEpicDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\divider.png" );
	pImageBorderEpicCorner				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\corner.png" );
	pImageBorderEpicBottomCorner		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\bottomcorner.png" );
	pImageBorderEpicInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\info.png" );
	pImageBorderEpicInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\infoleft.png" );
	pImageBorderEpicInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\infoRight.png" );

	pImageBorderEpicNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\nameframeleft.png" );
	pImageBorderEpicNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\nameframecenter.png" );
	pImageBorderEpicNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\nameframeright.png" );
	pImageBorderEpicNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Epic\\nameframebackgroud.png" );

	//Load Border Legendary Images
	pImageBorderLegendaryLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\topleft.png" );
	pImageBorderLegendaryRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\topright.png" );
	pImageBorderLegendaryTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\top.png" );
	pImageBorderLegendaryRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\bottomright.png" );
	pImageBorderLegendaryLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\bottomleft.png" );
	pImageBorderLegendaryBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\bottom.png" );
	pImageBorderLegendaryLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\left.png" );
	pImageBorderLegendaryRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\right.png" );

	pImageBorderLegendaryDivider				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\divider.png" );
	pImageBorderLegendaryCorner				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\corner.png" );
	pImageBorderLegendaryBottomCorner		= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\bottomcorner.png" );
	pImageBorderLegendaryInfo					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\info.png" );
	pImageBorderLegendaryInfoLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\infoleft.png" );
	pImageBorderLegendaryInfoRight					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\infoRight.png" );

	pImageBorderLegendaryNameFrameLeft					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\nameframeleft.png" );
	pImageBorderLegendaryNameFrameCenter				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\nameframecenter.png" );
	pImageBorderLegendaryNameFrameRight			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\nameframeright.png" );
	pImageBorderLegendaryNameFrameBackground			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\Legendary\\nameframebackgroud.png" );

	//Load Icons
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo101.bmp" )); //Lucidy
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo102.bmp" )); //Sereneo
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo103.bmp" )); //Fadeo
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo104.bmp" )); //Sparky
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo105.bmp" )); //Raident
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo106.bmp" )); //Transparo
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo107.bmp" )); //Murky
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo108.bmp" )); //Devine
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo109.bmp" )); //Celesto
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo110.bmp" )); //Mirage
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo111.bmp" )); //Inferna
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo112.bmp" )); //Enigma
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo113.bmp" )); //Bellum
	vSheltomImages.push_back(ImageLoader::LoadImage( "Image\\SinImage\\Items\\Accessory\\itfo114.bmp" )); //Ordo


	paSheltomCountText[0] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[1] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[2] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[3] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[4] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[5] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
	paSheltomCountText[6] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));


	paSheltomCountText[0]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[1]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[2]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[3]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[4]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[5]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
	paSheltomCountText[6]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));

	//Load Aging Bar
	pImageAgingBar					= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Bar.bmp");
	pImageAgingBarGage				= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Bar_.bmp");
	pImageAgingBarNum				= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Aging.bmp");

	pTextAging = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	pTextAging->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_ARGB(255,215,215,215) );

	//cBackgroundColor2 = UI::Color( 235, 45, 39, 27 );
	//cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );

	paSocketText[0][0] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[0][0]->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );

	paSocketText[0][1] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[0][1]->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );

	paSocketText[1][0] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[1][0]->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );

	paSocketText[1][1] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[1][1]->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );


	pRenderTargetMask	= GetGraphicsDevice()->MakeRenderTarget( 5001, 600, 600 );
	pEffectMask			= GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\InfoBox.fx" );

	//Socket Images
	pImageSocketBackground			= ImageLoader::LoadImage("image\\Sinimage\\Inven\\InfoBox\\socketbg.png");

	for( int i = 0; i < 6; i++ )
		paSocket[i] = ImageLoader::LoadImage( "image\\Sinimage\\Inven\\InfoBox\\socket%d.png", i + 1 );

	for( int i = 0; i < 15; i++ )
		paSocketGems[i] = ImageLoader::LoadImage( "image\\Sinimage\\Items\\Event\\itPR5%02d.bmp", i + 1 );


	//auto-generate from (copy n paste here)
	//https://docs.google.com/spreadsheets/d/1GNWePBhgcB1YX1n9pny5WlQk2y5lnsa2jJ8JCBAWMMA/edit#gid=1702125964
	vAgeSheltoms[1] = { 0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[2] = { 0, 0, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[3] = { 0, 0, 6, 6, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[4] = { 0, 0, 8, 8, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[5] = { 0, 0, 10, 10, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[6] = { 0, 0, 12, 12, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[7] = { 0, 0, 14, 14, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[8] = { 0, 0, 16, 16, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[9] = { 0, 0, 16, 18, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[10] = { 0, 0, 16, 20, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[11] = { 0, 0, 0, 20, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[12] = { 0, 0, 0, 20, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0 };
	vAgeSheltoms[13] = { 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0 };
	vAgeSheltoms[14] = { 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0 };
	vAgeSheltoms[15] = { 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0 };
	vAgeSheltoms[16] = { 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0 };
	vAgeSheltoms[17] = { 0, 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0 };
	vAgeSheltoms[18] = { 0, 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0 };
	vAgeSheltoms[19] = { 0, 0, 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0 };
	vAgeSheltoms[20] = { 0, 0, 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0 };


	//auto-generated from (copy n paste here)
	//https://docs.google.com/spreadsheets/d/1GNWePBhgcB1YX1n9pny5WlQk2y5lnsa2jJ8JCBAWMMA/edit#gid=1558279364
	vMixSheltoms[301] =  { 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[302] =  { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[303] =  { 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[304] =  { 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[305] =  { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[306] =  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[307] =  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[308] =  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[309] =  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[310] =  { 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[311] =  { 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[312] =  { 0, 3, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[313] =  { 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[314] =  { 0, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[315] =  { 0, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[316] =  { 0, 0, 1, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[317] =  { 0, 0, 5, 0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[318] =  { 0, 4, 4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[319] =  { 0, 2, 1, 1, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[320] =  { 4, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[321] =  { 0, 0, 0, 0, 0, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[322] =  { 0, 0, 0, 0, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[323] =  { 0, 0, 0, 0, 3, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[324] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[325] =  { 0, 0, 0, 1, 3, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[326] =  { 0, 0, 0, 0, 2, 3, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[327] =  { 0, 0, 0, 0, 0, 3, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[328] =  { 0, 0, 0, 0, 0, 0, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[329] =  { 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[330] =  { 0, 0, 0, 0, 0, 2, 2, 2, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[201] =  { 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[202] =  { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[203] =  { 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[204] =  { 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[205] =  { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[206] =  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[207] =  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[208] =  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[209] =  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[210] =  { 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[211] =  { 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[212] =  { 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[213] =  { 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[214] =  { 0, 1, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[215] =  { 0, 1, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[216] =  { 0, 1, 0, 4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[217] =  { 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[218] =  { 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[219] =  { 0, 1, 0, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[220] =  { 0, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[221] =  { 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[222] =  { 0, 0, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[223] =  { 0, 0, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[224] =  { 0, 0, 0, 1, 1, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[225] =  { 0, 0, 0, 0, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[226] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[227] =  { 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[228] =  { 0, 0, 0, 2, 0, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[229] =  { 0, 0, 0, 0, 3, 0, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[230] =  { 0, 0, 0, 0, 0, 0, 1, 2, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[231] =  { 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[232] =  { 0, 0, 0, 0, 0, 1, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[401] =  { 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[402] =  { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[403] =  { 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[404] =  { 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[405] =  { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[406] =  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[407] =  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[408] =  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[409] =  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[410] =  { 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[411] =  { 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[412] =  { 0, 3, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[413] =  { 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[414] =  { 0, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[415] =  { 0, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[416] =  { 0, 0, 1, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[417] =  { 0, 0, 3, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[418] =  { 0, 5, 5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[419] =  { 0, 2, 1, 1, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[420] =  { 5, 1, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[421] =  { 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[422] =  { 0, 0, 0, 2, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[423] =  { 0, 0, 0, 0, 5, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[424] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[425] =  { 0, 0, 0, 2, 0, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[426] =  { 0, 0, 0, 0, 1, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[427] =  { 0, 0, 0, 0, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[428] =  { 0, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[429] =  { 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[430] =  { 0, 0, 0, 0, 0, 1, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[101] =  { 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[102] =  { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[103] =  { 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[104] =  { 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[105] =  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[106] =  { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[107] =  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[108] =  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[109] =  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[110] =  { 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[111] =  { 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[112] =  { 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[113] =  { 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[114] =  { 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[115] =  { 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[116] =  { 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[117] =  { 0, 1, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[118] =  { 0, 1, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[119] =  { 1, 1, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[120] =  { 0, 1, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[121] =  { 0, 1, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[122] =  { 0, 0, 2, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[123] =  { 1, 1, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[124] =  { 0, 1, 3, 5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[125] =  { 0, 0, 0, 0, 5, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[126] =  { 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[127] =  { 0, 0, 3, 3, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[128] =  { 0, 0, 2, 2, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[129] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[130] =  { 0, 0, 0, 2, 0, 0, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[131] =  { 0, 0, 0, 1, 0, 4, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[132] =  { 0, 0, 0, 0, 2, 1, 2, 3, 2, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[133] =  { 0, 0, 0, 0, 0, 0, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[134] =  { 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[135] =  { 0, 0, 0, 0, 0, 1, 1, 5, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[701] =  { 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[702] =  { 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[703] =  { 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[704] =  { 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[705] =  { 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[706] =  { 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[707] =  { 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[708] =  { 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[709] =  { 0, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[710] =  { 0, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[711] =  { 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[712] =  { 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[713] =  { 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[714] =  { 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[715] =  { 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[716] =  { 0, 0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[717] =  { 0, 0, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[718] =  { 0, 0, 0, 1, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[719] =  { 0, 0, 0, 0, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[720] =  { 0, 0, 0, 0, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[721] =  { 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[722] =  { 0, 0, 0, 0, 1, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[723] =  { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[724] =  { 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[725] =  { 0, 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[726] =  { 0, 0, 0, 0, 0, 1, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[727] =  { 0, 0, 0, 0, 0, 0, 1, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[728] =  { 0, 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[729] =  { 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[730] =  { 0, 0, 0, 0, 0, 0, 2, 2, 2, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[731] =  { 0, 0, 0, 0, 0, 0, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[732] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[733] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[734] =  { 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[735] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 6, 0, 0, 0, 0, 0 };
	vMixSheltoms[736] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0 };
	vMixSheltoms[737] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 6, 0, 0, 0, 0 };
	vMixSheltoms[738] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0, 0 };
	vMixSheltoms[739] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0, 0 };
	vMixSheltoms[740] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0, 0 };
	vMixSheltoms[741] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 6, 0, 0, 0 };
	vMixSheltoms[742] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0 };
	vMixSheltoms[743] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0 };
	vMixSheltoms[744] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0 };
	vMixSheltoms[745] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0 };
	vMixSheltoms[746] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 8, 0, 0 };
	vMixSheltoms[747] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 0, 0 };
	vMixSheltoms[748] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 };
	vMixSheltoms[601] =  { 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[602] =  { 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[603] =  { 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[604] =  { 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[605] =  { 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[606] =  { 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[607] =  { 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[608] =  { 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[609] =  { 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[610] =  { 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[611] =  { 0, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[612] =  { 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[613] =  { 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[614] =  { 0, 0, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[615] =  { 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[616] =  { 0, 0, 1, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[617] =  { 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[618] =  { 0, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[619] =  { 0, 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[620] =  { 0, 0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[621] =  { 0, 0, 1, 1, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[622] =  { 0, 0, 0, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[623] =  { 0, 0, 0, 0, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[624] =  { 0, 0, 0, 0, 1, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[625] =  { 0, 0, 0, 0, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[626] =  { 0, 0, 0, 1, 1, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[627] =  { 0, 0, 0, 0, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[628] =  { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[629] =  { 0, 0, 0, 0, 0, 1, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[630] =  { 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[631] =  { 0, 0, 0, 0, 1, 1, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[632] =  { 0, 0, 0, 0, 0, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[633] =  { 0, 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[634] =  { 0, 0, 0, 0, 0, 0, 1, 2, 6, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[635] =  { 0, 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[636] =  { 0, 0, 0, 0, 0, 1, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[637] =  { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[638] =  { 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[639] =  { 0, 0, 0, 0, 0, 0, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[640] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[641] =  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 4, 0, 0, 0, 0, 0 };
	vMixSheltoms[642] =  { 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[643] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[644] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 7, 0, 0, 0, 0, 0 };
	vMixSheltoms[645] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0 };
	vMixSheltoms[646] =  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 4, 0, 0, 0, 0 };
	vMixSheltoms[647] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0, 0 };
	vMixSheltoms[648] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0, 0 };
	vMixSheltoms[649] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0, 0, 0 };
	vMixSheltoms[650] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0, 0 };
	vMixSheltoms[651] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 4, 0, 0, 0 };
	vMixSheltoms[652] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0 };
	vMixSheltoms[653] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0 };
	vMixSheltoms[654] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0, 0 };
	vMixSheltoms[655] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0 };
	vMixSheltoms[656] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 6, 0, 0 };
	vMixSheltoms[657] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 8, 0, 0 };
	vMixSheltoms[658] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0 };
	vMixSheltoms[659] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0 };
	vMixSheltoms[660] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 };
	vMixSheltoms[501] =  { 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[502] =  { 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[503] =  { 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[504] =  { 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[505] =  { 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[506] =  { 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[507] =  { 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[508] =  { 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[509] =  { 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[510] =  { 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[511] =  { 0, 0, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[512] =  { 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[513] =  { 0, 0, 0, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[514] =  { 0, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[515] =  { 0, 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[516] =  { 0, 0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[517] =  { 0, 0, 0, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[518] =  { 0, 0, 0, 0, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[519] =  { 0, 0, 0, 0, 1, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[520] =  { 0, 0, 0, 0, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[521] =  { 0, 0, 0, 0, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[522] =  { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[523] =  { 0, 0, 0, 0, 0, 1, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[524] =  { 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[525] =  { 0, 0, 0, 0, 0, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[526] =  { 0, 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[527] =  { 0, 0, 0, 0, 0, 0, 1, 2, 6, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[528] =  { 0, 0, 0, 0, 0, 0, 0, 2, 7, 0, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[529] =  { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[530] =  { 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[531] =  { 0, 0, 0, 0, 0, 0, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[532] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0, 0 };
	vMixSheltoms[533] =  { 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[534] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 };
	vMixSheltoms[535] =  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 7, 0, 0, 0, 0, 0 };
	vMixSheltoms[536] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 0, 0, 0, 0, 0 };
	vMixSheltoms[537] =  { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0, 0 };
	vMixSheltoms[538] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0, 0 };
	vMixSheltoms[539] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0, 0, 0 };
	vMixSheltoms[540] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0, 0 };
	vMixSheltoms[541] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 6, 0, 0, 0 };
	vMixSheltoms[542] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 0, 0, 0 };
	vMixSheltoms[543] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8, 0, 0, 0 };
	vMixSheltoms[544] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0, 0 };
	vMixSheltoms[545] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 8, 0, 0 };
	vMixSheltoms[546] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 8, 0, 0 };
	vMixSheltoms[547] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 10, 0, 0 };
	vMixSheltoms[548] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0 };

}

void ItemInfoBox::ClearCompareItem()
{
	//DEBUG("ItemInfoBox::ClearCompareItem()");
	ZeroMemory( &cItemCompare, sizeof( ItemData ) );
	bCompareItem = FALSE;
}

void ItemInfoBox::Invalidate()
{
	ZeroMemory( &cItemWrapper, sizeof( ItemData ) );
	bCompareItem = FALSE;
}

void ItemInfoBox::SetCompareItem( const ItemData * pcCompareItemData, BOOL bEquippedItem)
{
	//DEBUG("ItemInfoBox::SetCompareItem(%d)", bEquippedItem);

	CopyMemory( &cItemCompare, pcCompareItemData, sizeof( ItemData ) );

	bCompareItem = TRUE;
	bIsEquippedItem = bEquippedItem;
}

BOOL ItemInfoBox::IsBeginnerSet( ItemData * pcItemData )
{
	BOOL bRet = FALSE;
	switch ( pcItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerSword:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

BOOL ItemInfoBox::IsSameItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice )
{
	if( cItemWrapper.sItem.iChk1 == pcItemData->sItem.iChk1 &&
		cItemWrapper.sItem.iChk2 == pcItemData->sItem.iChk2 &&
		cItemWrapper.sItem.eRarity == pcItemData->sItem.eRarity &&
		cItemWrapper.sItem.sItemID.ToInt() == pcItemData->sItem.sItemID.ToInt() &&
		cItemWrapper.sPosition.Equals(&pcItemData->sPosition) &&
		bShowingBuyPrice == bShowBuyPrice &&
		bShowingSellPrice == bShowSellPrice )
	{
		return TRUE;
	}

	return FALSE;
}

char * ItemInfoBox::GetItemTypeName (EItemType eItemTYpe)
{
	if ( eItemTYpe == EItemType::ITEMTYPE_Amulet )
		return "Amulet";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Armor )
		return "Armor";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Axe )
		return "Axe";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Boots )
		return "Boots";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Bow )
		return "Bow";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Bracelets )
		return "Bracelets";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Claw )
		return "Claw";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Dagger )
		return "Dagger";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Gauntlets )
		return "Gauntlets";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Hammer )
		return "Hammer";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Javelin )
		return "Javelin";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Phantom )
		return "Phantom";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Ring )
		return "Ring";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Robe )
		return "Robe";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Scythe )
		return "Scythe";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Shield )
		return "Shield";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Sword )
		return "Sword";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Wand )
		return "Wand";
	else if ( eItemTYpe == EItemType::ITEMTYPE_Sheltom )
		return "Sheltom";
	else
		return "";
}

char * ItemInfoBox::GetItemRarityName ( EItemRarity eItemRarity )
{
	if ( eItemRarity == EItemRarity::NONE )
		return "";
	else if ( eItemRarity == EItemRarity::COMMON )
		return "Common";
	else if ( eItemRarity == EItemRarity::UNCOMMON )
		return "Uncommon";
	else if ( eItemRarity == EItemRarity::RARE )
		return "Rare";
	else if ( eItemRarity == EItemRarity::EPIC )
		return "Epic";
	else if ( eItemRarity == EItemRarity::LEGENDARY )
		return "Legendary";
	else
		return "";
}

char* ItemInfoBox::GetHanded(EItemSlotFlag eItemSlot)
{
	if ((eItemSlot & ITEMSLOTFLAG_RightHand) && (eItemSlot & ITEMSLOTFLAG_LeftHand))
		return "Two-Handed";
	else
		return "One-Handed";
}

BOOL ItemInfoBox::FormatItemInfo( ItemData * pcItemData )
{
	iSheltomYPosExtra = 5;

	//Item Name Rarity
	//if ( pcItemData->sItem.eRarity != EItemRarity::NONE )
	//{
	//	switch (cItemWrapper.sItem.eRarity)
	//	{
	//		case EItemRarity::COMMON:		AddString( 0, "Common",		D3DCOLOR_ARGB( 255, 255, 255, 200)); break; //White (Default)
	//		case EItemRarity::UNCOMMON:		AddString( 0, "Uncommon",	D3DCOLOR_ARGB( 255, 184, 255, 190)); break; //Green
	//		case EItemRarity::RARE:			AddString( 0, "Rare",		D3DCOLOR_ARGB( 255, 180, 192, 255)); break; //Blue
	//		case EItemRarity::EPIC:			AddString( 0, "Epic",		D3DCOLOR_ARGB(255, 238, 165, 255));  break; //Purple
	//		case EItemRarity::LEGENDARY:	AddString( 0, "Legendary",  D3DCOLOR_ARGB(255, 255, 166, 121));  break; //Orange
	//	}

	//	AddString( 1, "" );

	//	iSheltomYPosExtra += 14;
	//}

	if ( pcItemData->sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_QuestWeapon )
	{
		AddString( 0, "Quest Weapon", D3DCOLOR_RGBA( 0, 191, 255, 255 ) );
		AddString( 1, "" );
	}

	//if ( pcItemData->sItem.iItemSpecialType == 2 )
	//{
	//	AddString( 0, "Quest Reward", D3DCOLOR_RGBA( 0, 191, 255, 255 ) );
	//	AddString( 1, "" );
	//}

	if( (pcItemData->sItem.iItemSpecialType == 1) || PERFECTVIEWHANDLER->IsSet())
	{
		AddString( 0, "Perfect Item", D3DCOLOR_RGBA( 0, 191, 255, 255 ) );
		AddString( 1, "" );

		iSheltomYPosExtra += 14;
	}
	//else if (PERFECTVIEWHANDLER->IsItemPreviewable(pcItemData))
	//{
	//	//note - don't make texts that are 'once you know it, you know it 'in your face'. It's part of UX design good practices ;-)
	//	AddString(0, "Hold P to see perfect stats", D3DCOLOR_RGBA(125, 120, 100, 255), FALSE);
	//	AddString(1, "");

	//	iSheltomYPosExtra += 14;
	//}

	if( bIsEquippedItem && bCompareItem )
	{
		AddString( 0, "Equipped Item", D3DCOLOR_RGBA( 70, 125, 80, 255 ) );
		AddString( 1, "" );

		iSheltomYPosExtra += 14;
	}

	int iGender = ITEMHANDLER->GetItemGender( pcItemData->sItem.sItemID.ToInt() );
	if ( iGender != CHARACTERGENDER_None )
	{
		if ( iGender == CHARACTERGENDER_Male )
			AddString( 0, "Male Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		else
			AddString( 0, "Female Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );

		AddString( 1, "" );

		iSheltomYPosExtra += 14;
	}

	char * sItemType = GetItemTypeName ( pcItemData->sBaseItemID.ToItemType ());
	char * sItemRarity = GetItemRarityName (pcItemData->sItem.eRarity);
	char* sHanded = GetHanded(pcItemData->iItemSlotFlag);

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_ValentoRing)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_AbyssRing)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_DevilShyRing)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_DraxosBoots)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_IgnisBracelets)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToInt() == ITEMID_MokovaBoots)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToInt() == ITEMID_GreedyGauntlets)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_FurySheltom)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_TullaAmulet)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	BOOL bHandedText = FALSE;

	EItemType eItemType = pcItemData->sBaseItemID.ToItemType();
	switch (eItemType)
	{
		case ITEMTYPE_Axe:
		case ITEMTYPE_Hammer:
		case ITEMTYPE_Wand:
		case ITEMTYPE_Scythe:
		case ITEMTYPE_Bow:
		case ITEMTYPE_Sword:
			bHandedText = TRUE;
			break;
	}

	if (pcItemData->sItem.eRarity != EItemRarity::NONE)
	{
		AddString(0, FormatString("%s    ", sItemRarity), D3DCOLOR_ARGB(255, 170, 160, 170));
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing)
	{
		std::string mixIdName = MIXHANDLER->GetMixStringName(pcItemData->sItem.eMixTypeName);

		const char* sDesc;

		if (bHandedText)
			sDesc = FormatString("%sed %s %s    ", mixIdName.c_str(), sHanded, sItemType);
		else
			sDesc = FormatString("%sed %s    ", mixIdName.c_str(), sItemType);

		if (strlen(sDesc) > 20)
		{
			AddString(0, FormatString("%sed    ", mixIdName.c_str()), D3DCOLOR_ARGB(255, 170, 160, 170));
			AddString(1, "");

			if (bHandedText)
				sDesc = FormatString("%s %s    ", sHanded, sItemType);
			else
				sDesc = FormatString("%s    ",sItemType);

			iSheltomYPosExtra += 14;
		}

		AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
		AddString(1, "");
		




		if (pcItemData->sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
		{
			AddString(0, "");
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
		}
	}
	else if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging)
	{
		const char* sDesc;


		if (bHandedText)
			sDesc = FormatString("Aged %s %s    ", sHanded, sItemType);
		else
			sDesc = FormatString("Aged %s    ", sItemType);

		if (strlen(sDesc) > 20)
		{
			if (bHandedText)
			{
				AddString(0, FormatString("Aged    "), D3DCOLOR_ARGB(255, 170, 160, 170));
				AddString(1, "");

				sDesc = FormatString("%s %s    ", sHanded, sItemType);
			}
			else
			{
				AddString(0, FormatString("Aged    "), D3DCOLOR_ARGB(255, 170, 160, 170));
				AddString(1, "");

				sDesc = FormatString("%s    ", sItemType);
			}

			iSheltomYPosExtra += 14;
		}
		

		AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
		AddString(1, "");


		AddString(0, "");
		AddString(1, "");
		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;
		iSheltomYPosExtra += 14;
	}
	else
	{
		const char* sDesc;


		if (bHandedText)
			sDesc = FormatString("%s %s    ", sHanded, sItemType);
		else
			sDesc = FormatString("%s    ", sItemType);
		

		AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
		AddString(1, "");

	}

	bDisplayHelp = FALSE;



	if ( IsBeginnerSet( pcItemData ) )
	{
		AddString( 0, "Quest Item", D3DCOLOR_ARGB( 255, 132, 50, 254 ) );
		AddString( 1, "" );

		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_Season1CostumeArmor ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_Season2CostumeArmor ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_Season3CostumeArmor ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_Season4CostumeArmor ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_Season1CostumeRobe ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_DarkSeasonArmorFemale ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_DarkSeasonArmorMale ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_DarkSeasonRobeMale ||
		pcItemData->sItem.sItemID.ToItemID() == ITEMID_DarkSeasonRobeFemale)
	{
		AddString(0, "Account Bound", D3DCOLOR_RGBA(255, 132, 50, 254));
		AddString(1, "");

		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;

	}

	
	if ( (pcItemData->sItem.sItemID.ToInt() & 0xFFFF0000) == ITEMID_RuneCodeManufacture1 )
	{
		AddString( 0, "Craft Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );

		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}
	if (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Recipe)
	{
		AddString(0, "Manufacture Recipe", D3DCOLOR_RGBA(255, 155, 50, 255));
		AddString(1, "");

		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}

	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_BrilliantStone )
	{
		AddString( 0, "Bellatra Event Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );

		AddString(0, "");
		AddString(1, "");

		iSheltomYPosExtra += 14;
	}


	switch ( eItemType )
	{
		case ITEMTYPE_Axe:
		case ITEMTYPE_Claw:
		case ITEMTYPE_Hammer:
		case ITEMTYPE_Wand:
		case ITEMTYPE_Scythe:
		case ITEMTYPE_Bow:
		case ITEMTYPE_Sword:
		case ITEMTYPE_Javelin:
		case ITEMTYPE_Phantom:
		case ITEMTYPE_Dagger:
		case ITEMTYPE_Armor:
		case ITEMTYPE_Boots:
		case ITEMTYPE_Gauntlets:
		case ITEMTYPE_Shield:
		case ITEMTYPE_Robe:
		case ITEMTYPE_Sheltom:
		case ITEMTYPE_Amulet:
		case ITEMTYPE_Bracelets:
		case ITEMTYPE_Orb:
		case ITEMTYPE_Ring:
		bDisplayHelp = TRUE;

		AddString( 0, " " );
		AddString( 1, " " );

		break;
	}


	EItemID eItemID = pcItemData->sBaseItemID.ToItemID();
	switch( eItemID )
	{
		case ITEMID_LightBlueRune:
			AddString( 0, "Used to Craft Tulla Amulet", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DarkRedRune:
			AddString( 0, "Used to Craft Draxos Boots", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightGreenRune:
			AddString( 0, "Used to Craft Abyss Ring", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BrilliantStone:
			AddString( 0, "Exchange on Ihin\nBellatra Event NPC", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BlueStone:
			AddString( 0, "Resets all of your stat points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RedStone:
			AddString( 0, "Resets all of your skill points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GreenStone:
			AddString( 0, "Resets all of your skill and stat points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SpecStoneCode:
			AddString(0, "Resets all of your skill and stat points.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_FatalEdge:
			AddString( 0, "+10% Final Damage\nOnly works on SoD", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AvertScroll:
			AddString( 0, "+10% of Absorption\nOnly works on SoD", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneA:
			AddString( 0, "Completely fills the aging bar\nof your equipped weapon.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneB:
			AddString( 0, "Completely fills the aging bar\nof your equipped orb/shield.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneC:
			AddString( 0, "Completely fills the aging bar\nof your equipped robe/armor.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;


		case ITEMID_SapphireStone:
			AddString( 0, "Train all of your tier 1-3\nto 100% instantly.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_EmeraldStone:
			AddString( 0, "Train all of your tier 1-3\nby 30% instantly.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RubyStone:
			AddString( 0, "Train all of your tier 1-3\nby 10% instantly.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ExpUp1H:
			AddString(0, "Boosts experience gain by 30% for 1 hour. \nThis bonus does not apply to quests.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ExpUp24H:
			AddString(0, "Boosts experience gain by 30% for 24 hours. \nThis bonus does not apply to quests.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ExpUp48H:
			AddString(0, "Boosts experience gain by 30% for 48 hours. \nThis bonus does not apply to quests.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ExpUp72H:
			AddString( 0, "Boosts experience gain by 30% for 72 hours. \nThis bonus does not apply to quests.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_HPUp12H:
			AddString( 0, "Boosts HP regen by 5 for 12 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_HONEYDAMAGE:
			AddString(0, "+10% ATK Power", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_HONEYFAST:
			AddString(0, "10% Experience gained", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_HONEYSMART:
			AddString(0, "+5 MP Regen and +50 MP", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_HONEYDEFENSE:
			AddString(0, "+10% Absorb", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_MPUp12H:
			AddString( 0, "Boosts MP regen by 5 for 12 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MPDown12H:
			AddString( 0, "Reduces MP usage by 25% for 12 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SPDown12H:
			AddString( 0, "Reduces STM usage by 25% for 12 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_WitchHat:
		case ITEMID_CartolaHat:
			AddString( 0, "Halloween Item", D3DCOLOR_ARGB( 255, 180, 100, 175 ) );
			AddString( 1, "" );
			AddString( 0, "+5 HP Regen\n+10 Add HP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_Gyfu:
			AddString(0, "1 to respec an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "2 to reroll an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Talk to a Mixing Craftsman to use the Gyfu.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
			break;
		case ITEMID_Nied:
			AddString(0, "1 to respec an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "2 to reroll an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Talk to a Mixing Craftsman to use the Nied.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
			break;
		case ITEMID_Jera:
			AddString(0, "1 to respec an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "2 to reroll an item,", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Talk to a Mixing Craftsman to use the Jera.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
			break;

		case ITEMID_Wing_Metal:
			AddString(0, "Tier 1 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel to Bamboo Forest and Land of Dusk", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;
		case ITEMID_Wing_Silver:
			AddString(0, "Tier 2 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel up to Ruinen Village", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;
		case ITEMID_Wing_Gold:
			AddString(0, "Tier 3 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel up to Navisko Town", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;
		case ITEMID_Wing_Diamond:
			AddString(0, "Tier 4 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel up to Forbidden Land (Level 55+)", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;
		case ITEMID_Wing_Chaos:
			AddString(0, "Tier 5 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel up to Eura Village (Level 90+)", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;
		case ITEMID_Wing_Imperial:
			AddString(0, "Tier 6 Wing", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Enables Warpgate travel up to Land of Chaos (Level 95+)", D3DCOLOR_RGBA(255, 180, 100, 175));
			AddString(1, "");
			break;

		case ITEMID_ChristmasRedHat:
			AddString(0, "Boosts your stats with Christmas joy!\nThe hat expires after 12 hours.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Adds +50 MP/STM", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			break;

		case ITEMID_RecipeChristmas01:
		case ITEMID_RecipeChristmas02:
			AddString(0, "Combine the recipe with three different Christmas ingredients.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Speak to Master Shalon to proceed with the crafting process.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_GiftBox1:
		case ITEMID_GiftBox2:
			AddString(0, "Deliver the lost presents to Rudolph, the Reindeer!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_ColorfulChristmasTree:
		case ITEMID_SnowyChristmasTree:
			AddString(0, "Right click to place this Christmas Tree", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;


		case ITEMID_HopyCrystal:
		case ITEMID_HobgoblinCrystal:
		case ITEMID_DecoyCrystal:
		case ITEMID_BargonCrystal:
		case ITEMID_HeadCutterCrystal:
		case ITEMID_FigonCrystal:
		case ITEMID_KingHopyCrystal:
		case ITEMID_HulkCrystal:
			AddString(0, "Right click to summon a monster.\nCannot be used against bosses.", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Used to create wings to unlock warpgates.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
			break;

		case ITEMID_GuardianSaintCrystal:
		case ITEMID_WebCrystal:
		case ITEMID_DarkSpecterCrystal:
		case ITEMID_IronGuardCrystal:
		case ITEMID_RicartenMilitiaGuardCrystal:
		case ITEMID_RicartenGuardCrystal:
		case ITEMID_RoyalBlessGuardCrystal:
		case ITEMID_OccultCrystal:
		case ITEMID_DoomedCrystal:
		case ITEMID_DuskyCrystal:
		case ITEMID_MechanicCrystal:
		case ITEMID_AbysmalCrystal:
		case ITEMID_ArcticCrystal:
		case ITEMID_MechanicKingCrystal:
		case ITEMID_MysticCrystal:
		case ITEMID_FieryCrystal:
			AddString(0, "Right click to summon a monster.\nCannot be used against bosses.", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
			break;

		case ITEMID_Lucidy:
		case ITEMID_Sereno:
		case ITEMID_Fadeo:
		case ITEMID_Sparky:
		case ITEMID_Raident:
		case ITEMID_Transparo:
		case ITEMID_Murky:
		case ITEMID_Devine:
		case ITEMID_Celesto:
		case ITEMID_Mirage:
		case ITEMID_Inferna:
		case ITEMID_Enigma:
		case ITEMID_Bellum:
		case ITEMID_Oredo:
			AddString(0, "Used to create a force orb,\nUsed to mix or age equipment.", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Talk to a Force Master,\nMorald or Moriph.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			AddString(0, "\n");
			AddString(1, "");
			break;

		case ITEMID_KingStone:
			if ( pcItemData->sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_TGFKingFury )
			{
				AddString( 0, "King Fury Item", D3DCOLOR_ARGB( 255, 255, 248, 0 ) );
				AddString( 1, "" );

			}
			else if ( pcItemData->sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_TGFLordBabel )
			{
				AddString( 0, "Lord Babel Item", D3DCOLOR_ARGB( 255, 255, 248, 0 ) );
				AddString( 1, "" );

			}
			AddString( 0, FormatString( "Killed in %d Minute%s", pcItemData->sItem.tTime, pcItemData->sItem.tTime > 1 ? "s" : "" ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ThirdEye1H:
			AddString(0, "Adds one additional drop for 1 hour.\nThis applies to all monster kills.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_SixthSense1H:
			AddString(0, "Adds a 25% chance for one additional drop for 1 hour.\nThis applies to all monster kills.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ThirdEye24H:
			AddString(0, "Adds one additional drop for 24 hours.\nThis applies to all monster kills.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ThirdEye48H:
			AddString(0, "Adds one additional drop for 48 hours.\nThis applies to all monster kills.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_ThirdEye72H:
			AddString( 0, "Adds one additional drop for 72 hours.\nThis applies to all monster kills.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_PhoenixEgg:
			AddString( 0, "Summons a random Phoenix Pet\nthat will last 72 hours", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_PhoenixFire:
			AddString(0, "Summons a Fire Phoenix Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixIce:
			AddString(0, "Summons a Ice Phoenix Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixHealing:
			AddString(0, "Summons a Healing Phoenix Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixLightning:
			AddString(0, "Summons a Lightning Phoenix Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_PhoenixFireSmall :
			AddString(0, "Summons a Little Fire Dragon Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixIceSmall:
			AddString(0, "Summons a Little Ice Dragon Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixHealingSmall :
			AddString(0, "Summons a Little Healing Dragon Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_PhoenixLightningSmall:
			AddString(0, "Summons a Little Lightning Dragon Pet\nthat will last 24 hours", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_GravityStone72H:
			AddString( 0, "Boosts your carry capacity,\nby 250 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TeleportCore:
			AddString( 0, "Allows you to teleport to any map you choose.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AgingStone:
			AddString( 0, "Decrease 30% chance\nof fail aging", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_CopperOre:
			AddString( 0, "Prevents equipment from\nbreaking while aging.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_CleanItem:
			AddString(0, "Used to remove mixing or aging\nfrom equipment.", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Talk to Lucy in Pillai.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;

		case ITEMID_Fireworks:
			AddString( 0, "Lets Celebrate!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		// SERVER WIDE BUFFS
		case ITEMID_SHPRegen:
		case ITEMID_SSPRegen:
		case ITEMID_SMPRegen:
		case ITEMID_SMPDown:
		case ITEMID_SSPDown:
		case ITEMID_SBonusDamage:
		case ITEMID_SBonusAbsorb:
		case ITEMID_SMoveSpeed:
		case ITEMID_SExpBuff:
		case ITEMID_SDropBuff:
			AddString(0, "Starts a 1 hour server wide buff!\nOther players will appreciate you!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		// END SERVER WIDE BUFFS

		// Resistance Flasks
		case ITEMID_IceResistance:
			AddString(0, "Starts a 1 hour Ice resistance buff", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Only works in ICE type maps", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;

		case ITEMID_HairPotionA:
		case ITEMID_HairPotionB:
		case ITEMID_HairPotionC:
		case ITEMID_HairPotionD:
		case ITEMID_HairPotionE:
		case ITEMID_HairPotionF:
		case ITEMID_HairPotionG:
		case ITEMID_HairPotionH:
		case ITEMID_HairPotionI:
		case ITEMID_HairPotionJ:
		case ITEMID_HairPotionK:
		case ITEMID_HairPotionL:
		case ITEMID_HairPotionM:
		case ITEMID_HairPotionN:
		case ITEMID_HairPotionO:
		case ITEMID_HairPotionP:
		case ITEMID_HairPotionQ:
		case ITEMID_HairPotionR:
		case ITEMID_HairPotionS:
			AddString( 0, "Changes your hairstyle.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RebirthScroll:
			AddString(0, "Ressurects you 5 seconds after dying\nand prevents any experience loss.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_BigHeadHappyness:
			AddString( 0, "You'll look super happy!\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSadness:
			AddString( 0, "You'll look very sad.\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadShyness:
			AddString( 0, "You'll look very shy.\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadLove:
			AddString( 0, "You'll look in love!\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadValentine:
			AddString(0, "You have a secret admirer! +150 Weight,\n 100HP, 20HP Regen", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_BigHeadAngry:
			AddString( 0, "Rawr this'll make you look angry!\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSurprised:
			AddString( 0, "You'll look super surprised!\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSensual:
			AddString( 0, "Awimbawe, Awimbawe\nBoosts carry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TopazStone:
			AddString( 0, "A strong mature stone.\nMatures all equipped items", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TurquoiseStone:
			AddString( 0, "A weak mature stone.\nMatures all equipped items", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SheepHat:
			AddString( 0, "Gives you a wooly hat, boosting\ncarry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
/*		case ITEMID_EasterSheepHat:
			AddString(0, "Behhh Behhh, +150 Weight", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;*/
		case ITEMID_GiraffeHat:
			AddString( 0, "Gives you a long neck hat, boosting\ncarry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SoccerHat:
			AddString( 0, "Gives you a soccer hat, boosting\ncarry capacity by 150 for 72 hours.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SoccerHatSpeed:
			AddString( 0, "Gives you a soccer cap! +2 Move Speed", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RecipeEaster01:
			AddString(0, "Combine with three different Easter eggs!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;

		case ITEMID_RicartenCore:
			AddString( 0, "Teleport to Ricarten Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_NaviskoCore:
			AddString( 0, "Teleport to Navisko Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AtlantisCore:
			AddString( 0, "Teleport to Atlantis Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PillaiCore:
			AddString( 0, "Teleport to Pillai Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_UnionCore:
			AddString( 0, "Teleport to a Player", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );


			if( cItemWrapper.sItem.szItemName[ lstrlenA( cItemWrapper.sItem.szItemName ) + 1 ] != 0 )
			{
				AddString( 0, FormatString( "%s", cItemWrapper.sItem.szItemName + lstrlenA( cItemWrapper.sItem.szItemName ) + 1 ), D3DCOLOR_ARGB( 255, 110, 165, 250 ) );
				AddString( 1, "" );
			}
			break;
		case ITEMID_MightPotionFighter:
			AddString( 0, "5% Damage Boost\nagainst Fighter\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionMechanician:
			AddString( 0, "5% Damage Boost\nagainst Mechanician\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionPikeman:
			AddString( 0, "5% Damage Boost\nagainst Pikeman\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionArcher:
			AddString( 0, "5% Damage Boost\nagainst Archer\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionKnight:
			AddString( 0, "5% Damage Boost\nagainst Knight\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionAtalanta:
			AddString( 0, "5% Damage Boost\nagainst Atalanta\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionMagician:
			AddString( 0, "5% Damage Boost\nagainst Magician\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionPriestess:
			AddString( 0, "5% Damage Boost\nagainst Priestess\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DeadlyEdge:
			AddString( 0, "Add critical rating by +5% \nin PVP areas", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AvailOfEvade:
			AddString( 0, "Add evasion rating by +5% \nin PVP areas", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BolsterRecovery:
			AddString( 0, "HP MP SP Recovery\nfor party", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_Restoration:
			AddString( 0, "Resurrection Scroll", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneFire:
			AddString( 0, "Decrease Damage by 50%\nof Fire Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Fire Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneIce:
			AddString( 0, "Decrease Damage by 50%\nof Ice Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Ice Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneLight:
			AddString( 0, "Decrease Damage by 50%\nof Lightning Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60 seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Light Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LucidyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 2 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SerenoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 4 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_FadeoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 7 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SparkyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_RaidentForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 15 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 700 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_TransparoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 25 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 900 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_MurkyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 40 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 1200 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_DevineForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 60 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 1400 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_CelestoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 80 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 1600 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_MirageForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 100 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 1800 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_InfernaForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 120 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 2000 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_EnigmaForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 140 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 2400 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_BellumForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 160 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 3000 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_OredoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 180 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 3600 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SoccerShirt:
			AddString( 0, "Won't work with costumes", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );

			AddString( 0, " " );
			AddString( 1, " " );
			break;
	}


	if ( ( pcItemData->sItem.eItemFlags & EItemFlag::ForceAllowDrop ) == EItemFlag::ForceAllowDrop )
	{
		//Break Line
		AddString( 0, " " );
		AddString( 1, " " );

		AddString( 0, "Item can be dropped!", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		AddString( 1, "" );
	}

	return TRUE;
}

//this will also delete the item when expired.
DWORD ItemInfoBox::GetItemPlayTime( Item * pcItem )
{
	if ( pcItem->eCraftType == EItemCraftType::ITEMCRAFTTYPE_QuestWeapon &&
		 pcItem->sAgeLevel >= 4)
	{
		return ( 60 * 60 * 24 * 5); //5 days
	}

	DWORD dwTime = 0;
	switch ( pcItem->sItemID.ToItemID() )
	{
		// 7 Days
		// Beginner Set
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:

		// Christmas
		case ITEMID_ChristmasElegantCostumeM:
		case ITEMID_ChristmasElegantCostumeF:
		case ITEMID_ChristmasCloatCostumeTM:
		case ITEMID_ChristmasCloatCostumeTF:
		case ITEMID_ChristmasCloatCostumeMM:
		case ITEMID_ChristmasCloatCostumeMF:
		// Halloween
		case ITEMID_VampireSuit:
		case ITEMID_Succubus:

		// Easter
		case ITEMID_EasterHopyCostume:
		case ITEMID_EasterCuepyCostume:

			/*
		//Costumes
		case ITEMID_PartyCostumeMale:
		case ITEMID_PartyCostumeFemale:
		case ITEMID_IdasCostumeMale:
		case ITEMID_IdasCostumeFemale:
		case ITEMID_ThalesArmorMale:
		case ITEMID_ThalesArmorFemale:
		case ITEMID_Costume4ArmorMale:
		case ITEMID_Costume4ArmorFemale:
		case ITEMID_Costume5ArmorMale:
		case ITEMID_Costume5ArmorFemale:
		case ITEMID_WingArmorMale:
		case ITEMID_WingArmorFemale:
		case ITEMID_DarkWingArmorMale:
		case ITEMID_DarkWingArmorFemale:
		case ITEMID_SummerClotheMale:
		case ITEMID_SummerClotheFemale:
		case ITEMID_SpringCostumeMale:
		case ITEMID_SpringCostumeFemale:
		case ITEMID_YellowSpringCostumeMale:
		case ITEMID_YellowSpringCostumeFemale:
		case ITEMID_Costume11ArmorMale:
		case ITEMID_Costume11ArmorFemale:
		case ITEMID_BlackSuit:
		case ITEMID_WhiteDress:
		case ITEMID_Costume13ArmorMale:
		case ITEMID_Costume13ArmorFemale:
		case ITEMID_CasualSchoolUniformMale:
		case ITEMID_CasualSchoolUniformFemale:
		case ITEMID_ElegantSchoolUniformMale:
		case ITEMID_ElegantSchoolUniformFemale:
		case ITEMID_Costume16ArmorMale:
		case ITEMID_Costume16ArmorFemale:
		case ITEMID_MarineCostumeMale:
		case ITEMID_MarineCostumeFemale:
		case ITEMID_HopyCostume:
		case ITEMID_CuepyCostume:
		case ITEMID_ChineseSuitF:
		case ITEMID_GaiaArmorMale:
		case ITEMID_GaiaArmorFemale:
		case ITEMID_IriaRobeMale:
		case ITEMID_IriaRobeFemale:
		case ITEMID_DarkGaiaArmorMale:
		case ITEMID_DarkGaiaArmorFemale:
		case ITEMID_DarkIriaRobeMale:
		case ITEMID_DarkIriaRobeFemale:
		case ITEMID_WeddingSuit:
		case ITEMID_WeddingDress:
		case ITEMID_WarriorsHanbokM:
		case ITEMID_WarriorsHanbokF:
		case ITEMID_KimonoCostumeM:
		case ITEMID_KimonoCostumeF:
		case ITEMID_ChineseSuitM:
		case ITEMID_ChineseSuitM:
		case ITEMID_MikalRobeMale:
		case ITEMID_MikalRobeFemale:
		case ITEMID_MartanosRobeMale:
		case ITEMID_MartanosRobeFemale:
		case ITEMID_SilverCostumeMale:
		case ITEMID_SilverCostumeFemale:
		*/

			dwTime = (60 * 60 * 24 * 7);
			break;

		default:
			break;
	}

	switch ( pcItem->sItemID.ToItemID() )
	{
		case ITEMID_StarWarsShirt:

		case ITEMID_StarWarsLightSaber1H:
		case ITEMID_StarWarsLightSaber1H90:
		case ITEMID_StarWarsLightSaber1H100:
		case ITEMID_StarWarsLightSaber1H110:

		case ITEMID_StarWarsLightSaber2H:
		case ITEMID_StarWarsLightSaber2H90:
		case ITEMID_StarWarsLightSaber2H100:
		case ITEMID_StarWarsLightSaber2H110:

		case ITEMID_StarWarsBow:
		case ITEMID_StarWarsBow90:
		case ITEMID_StarWarsBow100:
		case ITEMID_StarWarsBow110:

		case ITEMID_StarWarsJavelin:
		case ITEMID_StarWarsJavelin90:
		case ITEMID_StarWarsJavelin100:
		case ITEMID_StarWarsJavelin110:

		case ITEMID_StarWarsDagger:
		case ITEMID_StarWarsDagger90:
		case ITEMID_StarWarsDagger100:
		case ITEMID_StarWarsDagger110:

		case ITEMID_StarWarsScythe:
		case ITEMID_StarWarsScythe90:
		case ITEMID_StarWarsScythe100:
		case ITEMID_StarWarsScythe110:

		case ITEMID_StarWarsPhantom:
		case ITEMID_StarWarsPhantom90:
		case ITEMID_StarWarsPhantom100:
		case ITEMID_StarWarsPhantom110:

		case ITEMID_StarWarsStaff:
		case ITEMID_StarWarsStaff90:
		case ITEMID_StarWarsStaff100:
		case ITEMID_StarWarsStaff110:

		case ITEMID_RedForce:
		case ITEMID_GreenForce:
		case ITEMID_WhiteForce:
		case ITEMID_PurpleForce:
		case ITEMID_YellowForce:

		case ITEMID_HONEYDAMAGE:
		//case ITEMID_HONEYFAST:
		case ITEMID_HONEYSMART:
		case ITEMID_HONEYDEFENSE:

		case ITEMID_MechanicCrystal:
		case ITEMID_MechanicKingCrystal:

		// 3 Days
		//Soccer Timed Champions League
		case ITEMID_ChelseaSoccerTimed:
		case ITEMID_ManchesterSoccerTimed:

		case ITEMID_SummerClotheTimedMale:
		case ITEMID_SummerClotheTimedFemale:

		dwTime = (60 * 60 * 24 * 3);
		break;

	default:
		break;
	}




	switch ( pcItem->sItemID.ToItemID() )
	{
	case ITEMID_SummerClotheTimedMale:
	case ITEMID_SummerClotheTimedFemale:

		dwTime = (60 * 60 * 1 * 3);
		break;

	default:
		break;
	}

	return dwTime;
}
/// <summary>
/// 2012 code: int DeleteEventItem_TimeOut( sITEMINFO	*lpsItem )
/// </summary>
/// <param name="psItemInfo"></param>
/// <returns></returns>
BOOL ItemInfoBox::IsExpiredItemTime( Item * psItemInfo )
{
	EItemID eItemID = psItemInfo->sItemID.ToItemID();

	DWORD dwTime = ItemInfoBox::GetItemPlayTime( psItemInfo );

	if ( dwTime && psItemInfo->szItemName[0] != 0 )
	{
		if ( GETPLAYTIME > (psItemInfo->tTime + dwTime) )
			return TRUE;
	}

	return FALSE;
}

BOOL ItemInfoBox::FormatItemTimeInfo( ItemData * pcItemData )
{
	DWORD dwTime		= 0;
	DWORD dwItemTime	= pcItemData->sItem.tTime;
	BOOL bResult		= FALSE;

	if ( dwItemTime == 0 )
		return FALSE;

	EItemID eItemID = pcItemData->sBaseItemID.ToItemID();

	DWORD dwTotalTime = ItemInfoBox::GetItemPlayTime( &pcItemData->sItem );

	if ( dwTotalTime )
	{
		if ( ItemInfoBox::IsExpiredItemTime( &pcItemData->sItem ) )
		{
			AddString( 0, "" );
			AddString( 1, "" );
			AddString( 0, "Expired Item", D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "\n " );
		}
		else
		{
			AddString( 0, "" );
			AddString( 1, "" );
			dwTime = dwTotalTime - (GETPLAYTIME - dwItemTime);
			AddString( 0, FormatString( "Time Left: %02d:%02d:%02d", dwTime / 3600, (dwTime / 60) % 60, dwTime % 60 ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "\n " );
		}
		bResult = TRUE;
	}

	return bResult;
}

BOOL ItemInfoBox::PrepareShowItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice, BOOL bShowChecksums, BOOL isItemCompare,int iBaseWidth )
{
	//Clear Vectors
	vLeftText.clear();
	vRightText.clear();
	vCompareText.clear();
	vRowCompare.clear();

	bShowingSellPrice = bShowSellPrice;
	bShowingBuyPrice = bShowBuyPrice;

	iSocketCount = 0;
	bIsPerfectItem = FALSE;
	bCantUseItem = FALSE;
	bHasLargeName = FALSE;
	//cBackgroundColor1 = UI::Color( 235, 45, 39, 27 );
	//cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );

	isCompare = isItemCompare;

	if ( iBaseWidth != -1 )
		iBaseMaxWidth = iBaseWidth;

	sItemName = "";
#ifdef ITEMFIXES
	sPrefixName = "";
	bHasPrefix = FALSE;
	bHasSuffix = FALSE;
	sSuffixName = "";
#endif

	// Item Data Copy
	{
		//This piece of cold is to fix empty item box if you mouse over a 2H item
		// on the right slot. But it also breaks viewing other player's item in linked chat
		// so we gonna check if it's MY 2-hand item
		if (pcItemData->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand &&
			pcItemData->iItemSlot &&
			pcItemData->bValid &&
			pcItemData->sItem.iChk1 == 0 && pcItemData->sItem.iChk2 == 0)
		{
			CopyMemory((void*)pdwItemInfoData, &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1].sItem, sizeof(Item));
			CopyMemory(&cItemWrapper, &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1], sizeof(ItemData));

			cItemWrapper.sPosition = pcItemData->sPosition;
		}
		else
		{
			CopyMemory( (void*)pdwItemInfoData, &pcItemData->sItem, sizeof( Item ) );
			CopyMemory( &cItemWrapper, pcItemData, sizeof( ItemData ) );

			iItemInfoBoxWidth = -1;
		}
	}

	if( IsViewingPerfectItem() )
		iItemInfoBoxWidth = -1;

	//Copy psItem to cItemWrapper
	CopyMemory( &cItemWrapper.sItem, (void*)pdwItemInfoData, sizeof( Item ) );

	if ( bIsViewingPerfectItem )
	{
		cItemWrapper.sItem.eRarity = cItemCompare.sItem.eRarity;
	}

	BOOL bBeginner = IsBeginnerSet( &cItemWrapper );

	bCantUseItem = cItemWrapper.sItem.bCanNotUse;

	//Perfect Item
	if( cItemWrapper.sItem.iItemSpecialType == 1 )
		bIsPerfectItem = TRUE;

	if( PERFECTVIEWHANDLER->IsSet() )
		bIsPerfectItem = TRUE;

	//RGB rarityColor = rarityColors[cItemWrapper.sItem.eRarity];	//background color based on rarity
	RGB rarityColor = rarityColors[0];								//All background colors are the same
	if ( bIsPerfectItem )
	{
		cBackgroundColor1 = UI::Color(245, rarityColor.red/6, rarityColor.green/6, rarityColor.blue/6);
		cBackgroundColor2 = UI::Color(230, rarityColor.red/2, rarityColor.green/2, rarityColor.blue/2);
	}
	else
	{
		cBackgroundColor1 = UI::Color(245, rarityColor.red/3, rarityColor.green/3, rarityColor.blue/3);
		cBackgroundColor2 = UI::Color(230, static_cast<int>(round(rarityColor.red/1.2f)), static_cast<int>(round(rarityColor.green/1.2f)), static_cast<int>(rarityColor.blue/1.2f));
	}


	D3DCOLOR FixColor = D3DCOLOR_ARGB ( 255, 130, 255, 188 );
	D3DCOLOR FixColorLighter = D3DCOLOR_ARGB ( 200, 130, 255, 188 );

	D3DCOLOR TitleColor = D3DCOLOR_ARGB( 255, 222, 231, 255 );
	//Color Name Item
	if( cItemWrapper.sItem.iItemUniqueID )
	{
		//Unique Color
		TitleColor = D3DCOLOR_ARGB ( 255, 128, 255, 128 );
	}
	else
	{

		if ( bBeginner )
			TitleColor = D3DCOLOR_ARGB( 255, 0, 222, 52 );


		//Item Name Rarity
		//switch (cItemWrapper.sItem.eRarity)
		//{
		//	case EItemRarity::COMMON:
		//	SetLineColor( D3DCOLOR_ARGB( 255, 255, 255, 200)); //White (Default)
		//	break;
		//	case EItemRarity::UNCOMMON:
		//	SetLineColor( D3DCOLOR_ARGB( 255, 66, 255, 82)); //Green
		//	break;
		//	case EItemRarity::RARE:
		//	SetLineColor( D3DCOLOR_ARGB(255, 71, 101, 255)); //Blue
		//	break;
		//	case EItemRarity::EPIC:
		//	SetLineColor( D3DCOLOR_ARGB(255, 228, 112, 255)); //Purple
		//	break;
		//	case EItemRarity::LEGENDARY:
		//	SetLineColor( D3DCOLOR_ARGB(255, 255, 105, 30)); //Orange
		//	break;

		//}

		//Set Special Colors
		if( cItemWrapper.sItem.eCraftType )
		{
			switch( cItemWrapper.sItem.eCraftType )
			{
				case ITEMCRAFTTYPE_Mixing:
				TitleColor =D3DCOLOR_ARGB( 255, 255, 110, 255);
				break;
				case ITEMCRAFTTYPE_Aging:
				TitleColor = D3DCOLOR_ARGB( 255, 100, 230, 255 );
				break;
				case ITEMCRAFTTYPE_QuestWeapon:
				TitleColor = D3DCOLOR_ARGB( 255, 132, 50, 254 ) ;
				break;
			}
		}
	}

	EItemType eItemType = cItemWrapper.sBaseItemID.ToItemType();
	switch ( eItemType )
	{
		case ITEMTYPE_Axe:
		case ITEMTYPE_Claw:
		case ITEMTYPE_Hammer:
		case ITEMTYPE_Wand:
		case ITEMTYPE_Scythe:
		case ITEMTYPE_Bow:
		case ITEMTYPE_Sword:
		case ITEMTYPE_Javelin:
		case ITEMTYPE_Phantom:
		case ITEMTYPE_Dagger:
		case ITEMTYPE_Armor:
		case ITEMTYPE_Boots:
		case ITEMTYPE_Gauntlets:
		case ITEMTYPE_Shield:
		case ITEMTYPE_Robe:
		case ITEMTYPE_Sheltom:
		case ITEMTYPE_Amulet:
		case ITEMTYPE_Bracelets:
		case ITEMTYPE_Orb:
		case ITEMTYPE_Ring:
		case ITEMTYPE_Ring2:
			bHasLargeName = TRUE;
		break;
	}

#ifdef ITEMFIXES
	if ( cItemWrapper.sItem.sPrefixData.iID > 0 )
	{
		bHasPrefix = TRUE;
		sPrefixName = cItemWrapper.sItem.sPrefixData.sName;
	}

	if ( cItemWrapper.sItem.sSuffixData.iID > 0 )
	{
		bHasSuffix = TRUE;
		sSuffixName = cItemWrapper.sItem.sSuffixData.sName;
	}
#endif

	BOOL bIsPotion = cItemWrapper.sItem.sItemID.ToItemBase() == EItemBase::ITEMBASE_Potion;


	if ( bHasLargeName )
	{
		if ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon )
			sItemName = FormatString( "%s\nTier %d", cItemWrapper.sItem.szItemName, cItemWrapper.sItem.sAgeLevel );
		else
			sItemName = FormatString("%s %s %s", sPrefixName, cItemWrapper.sItem.szItemName, sSuffixName);//cItemWrapper.sItem.szItemName;

		AddString( 0, "" );
		AddString( 1, "" );

		AddString( 0, "" );
		AddString( 1, "" );

		AddString( 0, "" );
		AddString( 1, "" );
	}
	else
	{
		if ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon )
			AddString( 0, FormatString( "%s Tier %d", cItemWrapper.sItem.szItemName, cItemWrapper.sItem.sAgeLevel ), -1, TRUE );
		else if ( bIsPotion )
			AddString( 0, FormatString( "%s (%d)", cItemWrapper.sItem.szItemName, cItemWrapper.sItem.iPotionCount ), -1, TRUE );
		else
			AddString( 0, FormatString("%s %s", sPrefixName,cItemWrapper.sItem.szItemName), -1, TRUE );
			//AddString( 0, cItemWrapper.sItem.szItemName, -1, TRUE );
		//sPrefixName
		AddString( 1, "" );
		SetLineColor ( TitleColor );
	}

	//Make Description Item
	FormatItemInfo( &cItemWrapper );

	//Aging Bar
	if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
	{
		AddString( 0, "\n " );
		AddString( 1, "\n " );
	}

	//Quest Item
	if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon && cItemWrapper.sItem.sAgeLevel <= QUESTWEAPON_MAX_AGE && cItemWrapper.sItem.uQuestMonId > 0 && cItemWrapper.sItem.sMatureBar.sCur > 0 )
	{
		//CQUESTGAME->GetQuestInstance()->GetQuest(QUEST)

		auto szMonName = QUESTGAMEHANDLER->GetQuestMonsterNameFromID( cItemWrapper.sItem.uQuestMonId );

		if ( !szMonName.empty() )
		{
			AddString( 0, FormatString( "%s: %d/%d", szMonName.c_str(), cItemWrapper.sItem.sMatureBar.sMax - cItemWrapper.sItem.sMatureBar.sCur, cItemWrapper.sItem.sMatureBar.sMax ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "\n " );

			AddString( 0, "" );
			AddString( 1, "" );
		}
	}

	//Respec Stone Level Range
	RespecStone * psList = ( RespecStone* )0x0099A0A8;

	for( int i = 0; i < 4; i++ )
	{
		EItemID eItemID = cItemWrapper.sBaseItemID.ToItemID();
		if( psList[ i ].eItemID == eItemID )
		{
			AddString( 0, "Level Req.:" );
			AddString( 1, FormatString( "%d-%d", psList[i].iMinLevel, psList[i].iMaxLevel ) );
			break;
		}
	}

	//Attack Power
	if( cItemWrapper.sItem.sDamage.sMin || cItemWrapper.sItem.sDamage.sMax )
	{
		AddString( 0, "Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sDamage.sMin, cItemWrapper.sItem.sDamage.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MinAttackPower ) || ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MaxAttackPower ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemBase() == ITEMBASE_Weapon ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && ( bIsEquippedItem || bIsPerfectItem ) )
		{
			int iCalcMin = cItemWrapper.sItem.sDamage.sMin - cItemCompare.sItem.sDamage.sMin;
			int iCalcMax = cItemWrapper.sItem.sDamage.sMax - cItemCompare.sItem.sDamage.sMax;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if(iCalcMin != 0 || iCalcMax != 0 )
				AddCompareString( FormatString( "%s%d / %s%d",
					iCalcMin > 0 ? "+" : "",
					iCalcMin,
					iCalcMax > 0 ? "+" : "",
					iCalcMax ),
					(iCalcMin + iCalcMax) > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Attack fMovementSpeed
	if( cItemWrapper.sItem.iAttackSpeed )
	{
		AddString( 0, "Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackSpeed ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_WeaponSpeed )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iAttackSpeed - cItemCompare.sItem.iAttackSpeed;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}

	}

	//Range
	if( cItemWrapper.sItem.iAttackRange )
	{
		AddString( 0, "Range:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackRange ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iAttackRange - cItemCompare.sItem.iAttackRange;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Critical
	if( cItemWrapper.sItem.iCritical )
	{
		AddString( 0, "Critical:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.iCritical ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Critical )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Claw || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Scythe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Dagger || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Hammer ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bow || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Javelin || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Sword || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iCritical - cItemCompare.sItem.iCritical;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Defense
	if( cItemWrapper.sItem.iDefenseRating )
	{
		AddString( 0, "Defense:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iDefenseRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_DefenseRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Armor || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Robe ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Boots || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Gauntlets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bracelets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = 0;

			if ( bIsPerfectItem && iDefenseOverride > 0 )
			{
				iCalc = iDefenseOverride - cItemCompare.sItem.iDefenseRating;
			}
			else
			{
				iCalc = cItemWrapper.sItem.iDefenseRating - cItemCompare.sItem.iDefenseRating;
			}

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Attack Rating
	if( cItemWrapper.sItem.iAttackRating )
	{

		AddString( 0, "Attack Rating:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_AttackRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Axe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Claw || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Hammer || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Scythe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Sword || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bracelets ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Dagger ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iAttackRating - cItemCompare.sItem.iAttackRating;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Absorption
	if( cItemWrapper.sItem.fAbsorbRating )
	{
		AddString( 0, "Absorb:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fAbsorbRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_AbsorbRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Armor || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Robe ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Gauntlets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Boots ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fAbsorbRating, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fAbsorbRating, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Block
	if( cItemWrapper.sItem.fBlockRating )
	{
		AddString( 0, "Block:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );

		if( fabs( cItemWrapper.sItem.fBlockRating - round( cItemWrapper.sItem.fBlockRating ) ) < FLT_MIN )
			AddString( 1, FormatString( "%.0f%%", round( cItemWrapper.sItem.fBlockRating ) ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
		else
			AddString( 1, FormatString( "%.1f%%", cItemWrapper.sItem.fBlockRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Block )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield ) ||
			( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fBlockRating, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fBlockRating, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f%%", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//fMovementSpeed
	if( cItemWrapper.sItem.fMovementSpeed )
	{
		AddString( 0, "Run Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fMovementSpeed ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_WalkSpeed )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fMovementSpeed, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fMovementSpeed, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Integrity
	if( cItemWrapper.sItem.sIntegrity.sCur || cItemWrapper.sItem.sIntegrity.sMax )
	{
		AddString( 0, "Integrity:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d/%d", cItemWrapper.sItem.sIntegrity.sCur, cItemWrapper.sItem.sIntegrity.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Aging Color
		if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		//Commented out. Who cares about Integrity!
		/*if( bCompareItem && bIsEquippedItem)
		{
			int iCalc = cItemWrapper.sItem.sIntegrity.sCur - cItemCompare.sItem.sIntegrity.sCur;
			int iCalcMax = cItemWrapper.sItem.sIntegrity.sMax - cItemCompare.sItem.sIntegrity.sMax;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d/%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}*/
	}

	//Mana
	if( cItemWrapper.sItem.sMPRecovery.sMin || cItemWrapper.sItem.sMPRecovery.sMax )
	{
		AddString( 0, "MP Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sMPRecovery.sMin, cItemWrapper.sItem.sMPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sMPRecovery.sMin - cItemCompare.sItem.sMPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sMPRecovery.sMax - cItemCompare.sItem.sMPRecovery.sMax;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//HP
	if( cItemWrapper.sItem.sHPRecovery.sMin || cItemWrapper.sItem.sHPRecovery.sMax )
	{
		AddString( 0, "HP Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sHPRecovery.sMin, cItemWrapper.sItem.sHPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sHPRecovery.sMin - cItemCompare.sItem.sHPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sHPRecovery.sMax - cItemCompare.sItem.sHPRecovery.sMax;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//STM
	if( cItemWrapper.sItem.sSPRecovery.sMin || cItemWrapper.sItem.sSPRecovery.sMax )
	{
		AddString( 0, "STM Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sSPRecovery.sMin, cItemWrapper.sItem.sSPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSPRecovery.sMin - cItemCompare.sItem.sSPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sSPRecovery.sMax - cItemCompare.sItem.sSPRecovery.sMax;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Resistances
	{
		if( cItemWrapper.sItem.saElementalDef[0] )
		{
			AddString( 0, "Organic:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[0] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Organic )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[0] - cItemCompare.sItem.saElementalDef[0];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[2] )
		{
			AddString( 0, "Fire:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[2] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Flame )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 2 ] - cItemCompare.sItem.saElementalDef[ 2 ];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[3] )
		{
			AddString( 0, "Ice:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[3] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Frost )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 3 ] - cItemCompare.sItem.saElementalDef[ 3 ];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[4] )
		{
			AddString( 0, "Lightning:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[4] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Light )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 4 ] - cItemCompare.sItem.saElementalDef[ 4 ];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[5] )
		{
			AddString( 0, "Poison:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[5] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Poison )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 5 ] - cItemCompare.sItem.saElementalDef[ 5 ];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}
	}

	//HP Regen
	if( cItemWrapper.sItem.fHPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "HP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fHPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_HPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fHPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fHPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//MP Regen
	if( cItemWrapper.sItem.fMPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "MP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fMPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) )
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fMPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fMPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//STM Regen
	if( cItemWrapper.sItem.fSPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "STM Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fSPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_SPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fSPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fSPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add HP
	if( cItemWrapper.sItem.fAddHP )
	{
		AddString( 0, "Add HP:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddHP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_HP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fAddHP, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fAddHP, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.0f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add MP
	if( cItemWrapper.sItem.fAddMP )
	{
		AddString( 0, "Add MP:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddMP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bow || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Javelin ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fAddMP, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fAddMP, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.0f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add STM
	if( cItemWrapper.sItem.fAddSP )
	{
		AddString( 0, "Add STM:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddSP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_SP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.fAddSP, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.fAddSP, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.0f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Potion Storage
	if( cItemWrapper.sItem.iPotionStorage )
	{
		AddString( 0, "Pot Count:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iPotionStorage ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if (cItemWrapper.sItem.eMixEffect & MIXEFFECT_PotionStorage)
			SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iPotionStorage - cItemCompare.sItem.iPotionStorage;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	// Time on item
	FormatItemTimeInfo( &cItemWrapper );


#ifdef ITEMFIXES
	//Define Line before Fixes
	iFixLine = 0;
	if ( bHasPrefix || bHasSuffix )
	{
		iFixLine = vLeftText.size();

		AddString( 0, " " );
		AddString(1, " ");
	}

	//Item Fixes

	//Attack Power Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iATKPow + cItemWrapper.sItem.sSuffixData.iATKPow) > 0 )
	{
		AddString( 0, "Add Attack Power:", FixColor);
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iATKPow + cItemWrapper.sItem.sSuffixData.iATKPow ), FixColor );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iATKPow > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iATKPow ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iATKPow - cItemCompare.sItem.sPrefixData.iATKPow;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iATKPow > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iATKPow ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iATKPow - cItemCompare.sItem.sSuffixData.iATKPow;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Critical Affix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fCritical + cItemWrapper.sItem.sSuffixData.fCritical) > 0.1f )
	{
		AddString( 0, "Add Critical:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fCritical + cItemWrapper.sItem.sSuffixData.fCritical ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fCritical > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fCritical ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fCritical, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fCritical, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fCritical > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fCritical ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fCritical, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fCritical, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Defense Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iDefense + cItemWrapper.sItem.sSuffixData.iDefense) > 0)
	{
		//
		//if (bHasPrefix)
		//{
		//	AddString(0, FormatString("%s", sPrefixName), FixColor);
		//	AddString(1, "");
		//}
		//
		//if (bHasSuffix)
		//{
		//	AddString(0, FormatString("%s", sSuffixName), FixColor);
		//	AddString(1, "");
		//}
		//
		//if (bHasPrefix || bHasSuffix)
		//{		
		//	AddString(0, "");
		//	AddString(1, "");
		//}
		AddString( 0, "Add Defense:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iDefense + cItemWrapper.sItem.sSuffixData.iDefense ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iDefense > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iDefense ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iDefense - cItemCompare.sItem.sPrefixData.iDefense;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iDefense > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iDefense ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iDefense - cItemCompare.sItem.sSuffixData.iDefense;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Attack Rating Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iATKRating + cItemWrapper.sItem.sSuffixData.iATKRating) > 0)
	{
		AddString( 0, "Add Attack Rating:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iATKRating + cItemWrapper.sItem.sSuffixData.iATKRating ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iATKRating > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iATKRating ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iATKRating - cItemCompare.sItem.sPrefixData.iATKRating;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iATKRating > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iATKRating ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iATKRating - cItemCompare.sItem.sSuffixData.iATKRating;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}



	//Absorb Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fAbsorb + cItemWrapper.sItem.sSuffixData.fAbsorb) > 0.1f )
	{
		AddString( 0, "Add Absorb:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fAbsorb + cItemWrapper.sItem.sSuffixData.fAbsorb ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fAbsorb > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fAbsorb ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fAbsorb, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fAbsorb, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fAbsorb > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fAbsorb ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fAbsorb, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fAbsorb, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Block Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fBlock + cItemWrapper.sItem.sSuffixData.fBlock) > 0.1f )
	{
		AddString( 0, "Add Block:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fBlock + cItemWrapper.sItem.sSuffixData.fBlock ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fBlock > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fBlock ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fBlock, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fBlock, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fBlock > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fBlock ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fBlock, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fBlock, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Run Speed Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fRunSpeed + cItemWrapper.sItem.sSuffixData.fRunSpeed) > 0.1f )
	{
		AddString( 0, "Add Run Speed:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fRunSpeed + cItemWrapper.sItem.sSuffixData.fRunSpeed ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fRunSpeed > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fRunSpeed ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fRunSpeed, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fRunSpeed, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fRunSpeed > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fRunSpeed ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fRunSpeed, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fRunSpeed, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Organic Res Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iOrganic + cItemWrapper.sItem.sSuffixData.iOrganic) > 0)
	{
		AddString( 0, "Add Organic:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iOrganic + cItemWrapper.sItem.sSuffixData.iOrganic ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iOrganic > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iOrganic ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iOrganic - cItemCompare.sItem.sPrefixData.iOrganic;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iOrganic > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iOrganic ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iOrganic - cItemCompare.sItem.sSuffixData.iOrganic;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Fire Res Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iFire + cItemWrapper.sItem.sSuffixData.iFire) > 0)
	{
		AddString( 0, "Add Fire:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iFire + cItemWrapper.sItem.sSuffixData.iFire ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iFire > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iFire ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iFire - cItemCompare.sItem.sPrefixData.iFire;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iFire > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iFire ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iFire - cItemCompare.sItem.sSuffixData.iFire;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Ice Res Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iFrost + cItemWrapper.sItem.sSuffixData.iFrost) > 0)
	{
		AddString( 0, "Add Ice:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iFrost + cItemWrapper.sItem.sSuffixData.iFrost ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iFrost > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iFrost ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iFrost - cItemCompare.sItem.sPrefixData.iFrost;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iFrost > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iFrost ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iFrost - cItemCompare.sItem.sSuffixData.iFrost;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Lightning Res Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iLightning + cItemWrapper.sItem.sSuffixData.iLightning) > 0)
	{
		AddString( 0, "Add Lightning:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iLightning + cItemWrapper.sItem.sSuffixData.iLightning ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iLightning > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iLightning ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iLightning - cItemCompare.sItem.sPrefixData.iLightning;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iLightning > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iLightning ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iLightning - cItemCompare.sItem.sSuffixData.iLightning;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Poison Res Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iPoison + cItemWrapper.sItem.sSuffixData.iPoison) > 0)
	{
		AddString( 0, "Add Lightning:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iPoison + cItemWrapper.sItem.sSuffixData.iPoison ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iPoison > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iPoison ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iPoison - cItemCompare.sItem.sPrefixData.iPoison;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iPoison > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iPoison ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iPoison - cItemCompare.sItem.sSuffixData.iPoison;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//HP Regen Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fRegenerationHP + cItemWrapper.sItem.sSuffixData.fRegenerationHP) > 0.1f )
	{
		AddString( 0, "Add HP Regen:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fRegenerationHP + cItemWrapper.sItem.sSuffixData.fRegenerationHP ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fRegenerationHP > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fRegenerationHP ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fRegenerationHP, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fRegenerationHP, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fRegenerationHP > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fRegenerationHP ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fRegenerationHP, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fRegenerationHP, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//MP Regen Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.fRegenerationMP + cItemWrapper.sItem.sSuffixData.fRegenerationMP) > 0.1f )
	{
		AddString( 0, "Add MP Regen:", FixColor );
		AddString( 1, FormatString( "%.1f",
			cItemWrapper.sItem.sPrefixData.fRegenerationMP + cItemWrapper.sItem.sSuffixData.fRegenerationMP ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.fRegenerationMP > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.fRegenerationMP ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sPrefixData.fRegenerationMP, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sPrefixData.fRegenerationMP, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.fRegenerationMP > 0 )
			{
				AddString( 0, FormatString( "   %s (%.1f)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.fRegenerationMP ), FixColorLighter, FALSE, TRUE);

				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSuffixData.fRegenerationMP, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSuffixData.fRegenerationMP, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if( fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Add HP Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iAddHP + cItemWrapper.sItem.sSuffixData.iAddHP) > 0)
	{
		AddString( 0, "Add HP:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iAddHP + cItemWrapper.sItem.sSuffixData.iAddHP ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iAddHP > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iAddHP ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iAddHP - cItemCompare.sItem.sPrefixData.iAddHP;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iAddHP > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iAddHP ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iAddHP - cItemCompare.sItem.sSuffixData.iAddHP;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}

	//Add MP Fix
	if ( (bHasPrefix || bHasSuffix) && (cItemWrapper.sItem.sPrefixData.iAddMP + cItemWrapper.sItem.sSuffixData.iAddMP) > 0)
	{
		AddString( 0, "Add MP:", FixColor );
		AddString( 1, FormatString( "%d",
			cItemWrapper.sItem.sPrefixData.iAddMP + cItemWrapper.sItem.sSuffixData.iAddMP ), FixColor);

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			if ( cItemWrapper.sItem.sPrefixData.iAddMP > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sPrefixData.sName,cItemWrapper.sItem.sPrefixData.iAddMP ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sPrefixData.iAddMP - cItemCompare.sItem.sPrefixData.iAddMP;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
			if ( cItemWrapper.sItem.sSuffixData.iAddMP > 0 )
			{
				AddString( 0, FormatString( "   %s (%d)", cItemWrapper.sItem.sSuffixData.sName,cItemWrapper.sItem.sSuffixData.iAddMP ), FixColorLighter, FALSE, TRUE);

				int iCalc = cItemWrapper.sItem.sSuffixData.iAddMP - cItemCompare.sItem.sSuffixData.iAddMP;

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );

				AddString(1, "");
			}
		}
	}
#endif

	//Define Line before Stats
	iStatLine = 0;
	if ( cItemWrapper.sItem.iLevel || cItemWrapper.sItem.iStrength || cItemWrapper.sItem.iSpirit || cItemWrapper.sItem.iTalent || cItemWrapper.sItem.iAgility || cItemWrapper.sItem.iHealth )
	{
		iStatLine = vLeftText.size();

		AddString( 0, " " );
		AddString(1, " ");
	}

	//Req Level
	if( cItemWrapper.sItem.iLevel )
	{
		AddString( 0, "Required Level:" );

		auto itemLevelStr = ITEMHANDLER->GetItemLevelWithLetter( &cItemWrapper);

		AddString(1, itemLevelStr);

		if( PLAYERDATA->iLevel < cItemWrapper.sItem.iLevel )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.iLevel - cItemCompare.sItem.iLevel;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Strenght
	if( cItemWrapper.sItem.iStrength )
	{
		AddString( 0, "Req. Strength:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iStrength ) );

		if( PLAYERDATA->iStrength < cItemWrapper.sItem.iStrength )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItem.eSpecialization != EClassFlag::CLASSFLAG_None)
		{
			int iCalc = cItemWrapper.sItem.iStrength - cItemCompare.sItem.iStrength;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//DEBUG("%s = mix color = %d", cItemWrapper.sItem.szItemName, cItemWrapper.sItem.sMixColorId);

	//Req Spirit
	if( cItemWrapper.sItem.iSpirit )
	{
		AddString( 0, "Req. Spirit:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iSpirit ) );

		if( PLAYERDATA->iSpirit < cItemWrapper.sItem.iSpirit )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItem.eSpecialization != EClassFlag::CLASSFLAG_None)
		{
			int iCalc = cItemWrapper.sItem.iSpirit - cItemCompare.sItem.iSpirit;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Talent
	if( cItemWrapper.sItem.iTalent )
	{
		AddString( 0, "Req. Talent:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iTalent ) );

		if( PLAYERDATA->iTalent < cItemWrapper.sItem.iTalent )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItem.eSpecialization != EClassFlag::CLASSFLAG_None)
		{
			int iCalc = cItemWrapper.sItem.iTalent - cItemCompare.sItem.iTalent;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Agility
	if( cItemWrapper.sItem.iAgility )
	{
		AddString( 0, "Req. Agility:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAgility ) );

		if( PLAYERDATA->iAgility < cItemWrapper.sItem.iAgility )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItem.eSpecialization != EClassFlag::CLASSFLAG_None)
		{
			int iCalc = cItemWrapper.sItem.iAgility - cItemCompare.sItem.iAgility;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Health
	if( cItemWrapper.sItem.iHealth )
	{
		AddString( 0, "Req. Health:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iHealth ) );

		if( PLAYERDATA->iHealth < cItemWrapper.sItem.iHealth )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItem.eSpecialization != EClassFlag::CLASSFLAG_None)
		{
			int iCalc = cItemWrapper.sItem.iHealth - cItemCompare.sItem.iHealth;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}



	//Spec
	iSpecLine = 0;
	if( cItemWrapper.sItem.eSpecialization )
	{

		iSpecLine = vLeftText.size();

		EClassFlag e = static_cast<EClassFlag>( cItemWrapper.sItem.eSpecialization );
		AddString( 0, FormatString( "\n%s Spec", GetCharacterClassStringByFlag( e ) ) );
		AddString( 1, "\n" );

		if( PLAYERDATA->iFlag == cItemWrapper.sItem.eSpecialization )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 220, 0 ) );
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}

	//Spec ATK fMovementSpeed
	if( cItemWrapper.sItem.sSpecData.iSpecAttackSpeed )
	{
		AddString( 0, "Attack Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecAttackSpeed ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackSpeed - cItemCompare.sItem.sSpecData.iSpecAttackSpeed;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Critical
	if( cItemWrapper.sItem.sSpecData.iSpecCritical )
	{
		AddString( 0, "Critical Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.sSpecData.iSpecCritical ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecCritical - cItemCompare.sItem.sSpecData.iSpecCritical;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d%%", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Defense
	if( cItemWrapper.sItem.sSpecData.iSpecDefenseRating )
	{
		AddString( 0, "Defense:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecDefenseRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecDefenseRating - cItemCompare.sItem.sSpecData.iSpecDefenseRating;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec fAbsorbRating
	if( cItemWrapper.sItem.sSpecData.fSpecAbsorbRating )
	{
		AddString( 0, "Absorption:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecAbsorbRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecAbsorbRating, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecAbsorbRating, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f)
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Block
	if( cItemWrapper.sItem.sSpecData.fSpecBlockRating )
	{
		AddString( 0, "Block:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );

		if( fabs( cItemWrapper.sItem.sSpecData.fSpecBlockRating - round( cItemWrapper.sItem.sSpecData.fSpecBlockRating ) ) < FLT_MIN )
			AddString( 1, FormatString( "%.0f%%", round( cItemWrapper.sItem.sSpecData.fSpecBlockRating ) ) );
		else
			AddString( 1, FormatString( "%.1f%%", cItemWrapper.sItem.sSpecData.fSpecBlockRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecBlockRating, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecBlockRating, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f%%", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MagicAPT
	if( cItemWrapper.sItem.sSpecData.fSpecMagicMastery )
	{
		AddString( 0, "Magic APT:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.sSpecData.fSpecMagicMastery ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecMagicMastery, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecMagicMastery, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if (fCalc >= 0.1f || fCalc <= -0.1f)
				AddCompareString( FormatString( "%s%.0f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec fMovementSpeed
	if( cItemWrapper.sItem.sSpecData.fSpecMovementSpeed )
	{
		AddString( 0, "Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecMovementSpeed ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecMovementSpeed, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecMovementSpeed, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if (fCalc >= 0.1f || fCalc <= -0.1f)
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Resistances
	{
		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[0] )
		{
			AddString( 0, "Organic:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[0] - cItemCompare.sItem.sSpecData.saSpecElementalDef[0];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] )
		{
			AddString( 0, "Fire:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] - cItemCompare.sItem.sSpecData.saSpecElementalDef[2];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] )
		{
			AddString( 0, "Frost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] - cItemCompare.sItem.sSpecData.saSpecElementalDef[3];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] )
		{
			AddString( 0, "Lightning:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] - cItemCompare.sItem.sSpecData.saSpecElementalDef[4];

				if (bIsEquippedItem)
					SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[5] )
		{
			AddString( 0, "Poison:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[5] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );
		}
	}

	//Spec ATK Power
	if( cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv )
	{
		AddString( 0, "Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = 0;
			BOOL bForceBlue = FALSE;

			if (cItemCompare.sItem.sSpecData.iSpecAttackPowerDiv == 0)
			{
				iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv;
				bForceBlue = TRUE; //force text to be blue
			}
			else
			{
				iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv - cItemCompare.sItem.sSpecData.iSpecAttackPowerDiv;
			}

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString(FormatString( "%sLV/%d",
						iCalc > 0 ? "+" : "",
						iCalc ),
					iCalc > 0 && !bForceBlue ? D3DCOLOR_ARGB( 255, 225, 30, 0 ) : D3DCOLOR_ARGB( 255, 0, 130, 255 ) );
		}
	}

	//Spec ATK Rtg
	if( cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv )
	{
		AddString( 0, "Attack Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = 0;
			BOOL bForceBlue = FALSE;

			if (cItemCompare.sItem.sSpecData.iSpecAttackRatingDiv == 0)
			{
				iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv;
				bForceBlue = TRUE; //force text to be blue
			}
			else
			{
				iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv - cItemCompare.sItem.sSpecData.iSpecAttackRatingDiv;
			}

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%sLV/%d",
					iCalc > 0 ? "+" : "",
					iCalc ),
					iCalc > 0 && !bForceBlue ? D3DCOLOR_ARGB( 255, 225, 30, 0 ) : D3DCOLOR_ARGB( 255, 0, 130, 255 ) );
		}
	}

	//Spec Range
	if( cItemWrapper.sItem.sSpecData.iSpecAttackRange )
	{
		AddString( 0, "Range:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecAttackRange ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackRange - cItemCompare.sItem.sSpecData.iSpecAttackRange;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%s%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec HP Boost
	if( cItemWrapper.sItem.sSpecData.iSpecAddHPDiv )
	{
		AddString( 0, "Max HP Boost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAddHPDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAddHPDiv - cItemCompare.sItem.sSpecData.iSpecAddHPDiv;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%sLV/%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MP Boost
	if( cItemWrapper.sItem.sSpecData.iSpecAddMPDiv )
	{
		AddString( 0, "Max MP Boost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAddMPDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAddMPDiv - cItemCompare.sItem.sSpecData.iSpecAddMPDiv;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "%sLV/%d", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec HP Regen
	if( cItemWrapper.sItem.sSpecData.fSpecHPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "HP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecHPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecHPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecHPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MP Regen
	if( cItemWrapper.sItem.sSpecData.fSpecMPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "MP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecMPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecMPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecMPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec STM Regen
	if( cItemWrapper.sItem.sSpecData.fSpecSPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "STM Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecSPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && (bIsEquippedItem || bIsPerfectItem))
		{
			float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItem.sSpecData.fSpecSPRegen, 1);
			float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItem.sSpecData.fSpecSPRegen, 1);
			float fCalc = fThisValueRounded - fCompareValueRounded;

			if (bIsEquippedItem)
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc >= 0.1f || fCalc <= -0.1f )
				AddCompareString( FormatString( "%s%.1f", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	// Next Line
	//AddString(0, "");
	//AddString(1, "");

	//AddString(0, "Price:");
	//AddString(1, cItemWrapper.sItem.iSalePrice > 0 ? FormatString("%s Gold", FormatNumber(cItemWrapper.sItem.iSalePrice)) : "Free");

	// Price
	// Shop is open?
	if ( WINDOW_ISOPEN_NPC_SHOP )
	{
		// Next Line
		AddString( 0, "" );
		AddString( 1, "" );

		if ( bShowBuyPrice )
		{
			// Normal, price
			if ( bShowBuyPrice == 2 )
			{
				// Get Inventory Price Item
				PriceItem sSellItemData = { 0, 0, 0 };
				ITEMHANDLER->GetItemPrice( (ItemData*)pcItemData, &sSellItemData );
				pcItemData->iSalePrice = sSellItemData.iPrice;

				if ( MOUSE_ICON == EMOUSEICON_Repair )
				{
					if ( pcItemData->sItem.sIntegrity.sMax > 0 )
					{
						AddString( 0, "Repair Price:" );
						AddString( 1, sSellItemData.iRepairPrice > 0 ? FormatString( "%s Gold", FormatNumber( sSellItemData.iRepairPrice ) ) : " - " );
					}
				}
				else
				{
					// Set Price Item
					if ( pcItemData->sItem.eCraftType != EItemCraftType::ITEMCRAFTTYPE_QuestWeapon && pcItemData->sItem.eCraftType != EItemCraftType::ITEMCRAFTTYPE_Quest )
					{
						AddString( 0, "Sell Price:" );
						AddString( 1, pcItemData->iSalePrice > 0 ? FormatString( "%s Gold", FormatNumber( pcItemData->iSalePrice ) ) : "Free" );
					}

					if ( pcItemData->sItem.sIntegrity.sMax > 0 )
					{
						AddString( 0, "Repair Price:" );
						AddString( 1, sSellItemData.iRepairPrice > 0 ? FormatString( "%s Gold", FormatNumber( sSellItemData.iRepairPrice ) ) : " - " );
					}
				}
			}

			if ( bShowBuyPrice == 1 )
			{
				// Set Text
				char szTempText[32] = { 0 };
				int iPrice = cItemWrapper.sItem.iSalePrice;

				// Get Price with BC Tax
				__asm
				{
					PUSH iPrice;
					MOV EAX, 0x004E5A10;
					CALL EAX;
					MOV iPrice, EAX;
				}


				AddString( 0, "Buy Price:" );
				AddString( 1, iPrice > 0 ? FormatString("%s Gold",FormatNumber( iPrice )) : "Free" );
			}
		}
	}

	// Personal Shop is open?
	if ( WINDOW_ISOPEN_PERSONALSHOP_ME || WINDOW_ISOPEN_PERSONALSHOP_OTHERS )
	{
		if ( bShowSellPrice )
		{
			// Next Line
			AddString( 0, "" );
			AddString( 1, "" );

			// Set Text
			int iPrice = (*(int*)0x03653E38); // Value from personal shop

			AddString( 0, "Price:" );
			AddString( 1, iPrice > 0 ? FormatString("%s Gold",FormatNumber( iPrice )) : "Free" );
		}
	}

	//Debug Item Info
	if( bShowChecksums )
	{
		AddString( 0, "\n" );
		AddString( 1, "" );

		AddString( 0, FormatString( "%d", cItemWrapper.sItem.iChk1 ) );
		AddString( 1, "" );

		AddString( 0, FormatString( "%d", cItemWrapper.sItem.iChk2 ) );
		AddString( 1, "" );
	}

	//Socket Item
	if( cItemWrapper.sItem.sSocketData[0].eSocketType != SOCKETTYPE_None )
	{
		AddString( 0, "\n\n " );
		AddString( 1, "\n\n " );
	}

	////Can't use Item?
	//if( bCantUseItem )
	//{
	//	cBackgroundColor1 = UI::Color( 235, 10, 10, 10 );
	//	cBackgroundColor2 = UI::Color( 235, 75, 25, 25 );
	//}

	//Clear Text
	paSocketText[0][0]->SetText( "" );
	paSocketText[0][1]->SetText( "" );
	paSocketText[1][0]->SetText( "" );
	paSocketText[1][1]->SetText( "" );

	for( int i = 0; i < 2; i++ )
	{
		if( cItemWrapper.sItem.sSocketData[i].eSocketType != SOCKETTYPE_None )
		{
			iSocketCount += 1;

			//Get Gem Image
			switch( cItemWrapper.sItem.sSocketData[i].eGemItem )
			{
				case GEMITEM_Ruby:
					iSocketGem[i] = 0;
				break;
				case GEMITEM_Topaz:
					iSocketGem[i] = 3;
				break;
				case GEMITEM_Emerald:
					iSocketGem[i] = 6;
				break;
				case GEMITEM_Aquamarine:
					iSocketGem[i] = 9;
				break;
				case GEMITEM_Amethyst:
					iSocketGem[i] = 12;
				break;
			}

			//Format Gem Type
			if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Triangular )
				paSocketText[i][1]->SetText( "+2" );
			else if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Square )
			{
				paSocketText[i][1]->SetText( "+4" );
				iSocketGem[i] += 1;
			}
			else if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Marquise )
			{
				paSocketText[i][1]->SetText( "+6" );
				iSocketGem[i] += 2;
			}

			//Format Name and Stats of Gem
			switch( cItemWrapper.sItem.sSocketData[i].eGemItem )
			{
				case GEMITEM_Topaz:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 255, 220, 0 ) );
					paSocketText[i][0]->SetText( "Topaz Gem" );

					paSocketText[i][1]->AppendText( " Talent" );
				break;
				case GEMITEM_Aquamarine:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 0, 150, 255 ) );
					paSocketText[i][0]->SetText( "Aquamarine Gem" );

					paSocketText[i][1]->AppendText( " Spirit" );
				break;
				case GEMITEM_Ruby:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 180, 25, 25 ) );
					paSocketText[i][0]->SetText( "Ruby Gem" );

					paSocketText[i][1]->AppendText( " Strenght" );
				break;
				case GEMITEM_Amethyst:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 150, 5, 235 ) );
					paSocketText[i][0]->SetText( "Amethyst Gem" );

					paSocketText[i][1]->AppendText( " Health" );
				break;
				case GEMITEM_Emerald:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 0, 170, 10 ) );
					paSocketText[i][0]->SetText( "Emerald Gem" );

					paSocketText[i][1]->AppendText( " Agility" );
				break;
			}
		}
	}

	//Not have description or stats? So only show the Title of Item
	if( vLeftText.size() < 3 )
	{
		vLeftText.erase( vLeftText.begin() + 1 );
		vRightText.erase( vRightText.begin() + 1 );
	}

	return TRUE;
}

void ItemInfoBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	//If player is viewing Item
	if( *( BOOL* )0x036286EC )
	{
		int iFoundWidth = 0;
		int iMaxWidth = 184; //Minimum Width

		int iItemWidth = cItemWrapper.iWidth;
		int iItemHeight = cItemWrapper.iHeight;

		int iItemImageXOffset = 0;
		if ( iItemWidth == 22 )
			iItemImageXOffset = 22;
		else if ( iItemWidth == 44 )
			iItemImageXOffset = 11;
		else if ( iItemWidth == 66 )
			iItemImageXOffset = 0;

		int iItemImageYOffset = 0;
		if ( iItemHeight == 22 )
			iItemImageYOffset = 26;
		else if ( iItemHeight == 44 )
			iItemImageYOffset = 11;
		else if ( iItemHeight == 66 )
			iItemImageYOffset = 0;

		int padding = 20;
		int spacing = 10;

		//Determine Width
		if ( ( bIsEquippedItem || bIsPerfectItem ) && bCompareItem )
		{

			for ( UINT i = 0; i < vRowCompare.size (); i++ )
			{
				Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
				Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
				Text_ptr pCompareText = vCompareText[i];

				//Get Widest text line
				if ( ( pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + spacing + pCompareText->GetTextWidth () + padding * 2 ) > iMaxWidth )
					iMaxWidth = (pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + spacing + pCompareText->GetTextWidth () + padding * 2) > iMaxWidth ?
					pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + spacing + pCompareText->GetTextWidth () + padding * 2 : iMaxWidth ;
			}
		}

		for (UINT i = 0; i < vLeftText.size(); i++)
		{
			Text_ptr pLeftText = vLeftText[i];
			Text_ptr pRightText = vRightText[i];

			//Get Widest text line
			if ( ( pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + padding * 2 ) > iMaxWidth )
				iMaxWidth = ( pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + padding * 2 ) > iMaxWidth ?
				pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + padding * 2 : iMaxWidth;
		}

		//Apply Title if has Affixes
		Text_ptr pTitleText;
		Text_ptr pTitleText2;
		BOOL hasSecondLine = FALSE;
		if ( bHasLargeName )
		{
			int iEdgeSpacing = 20;
			UI::Text_ptr pFirstLine;

			int iFormat = 0;

			//Split Name if too long
			std::string strItemName;
			strItemName.assign ( sItemName.c_str() );

			std::string sFirstLine;
			sFirstLine.assign ( sItemName.c_str() );

			std::string sSecondLine;

			int iNameLength = strlen ( sItemName.c_str() );

			BOOL bHasMultiLine = strItemName.find( '\n' ) != std::string::npos;

			if ( iNameLength > 15 && !bHasMultiLine )
			{
				std::vector<int> SpaceList;

				int iSpaceCount = 0;
				for(std::string::iterator it = strItemName.begin(); it != strItemName.end(); ++it)
				{
					if(*it == ' ')
					{
						SpaceList.push_back ( iNameLength - strlen(it._Ptr) + 1 );
					}
				}

				//Find the space that is closest to the item name center
				int iMidSpace = 0;
				int iMidSpaceDistance = 1000;  //Set high to start
				for ( int i : SpaceList )
				{
					int iCurrentMidSpaceDist = abs ( i - ( iNameLength / 2 ) );

					if ( iMidSpaceDistance > iCurrentMidSpaceDist )
					{
						iMidSpaceDistance = iCurrentMidSpaceDist;
						iMidSpace = i;
					}
				}

				sFirstLine = strItemName.substr ( 0, iMidSpace - 1 );
				sSecondLine = strItemName.substr ( iMidSpace );

				SpaceList.clear ();
				hasSecondLine = TRUE;
			}

			pTitleText = std::make_shared<Text> ( FormatString("%s", sFirstLine.c_str())
				, Rectangle2D ( 0, 0, 180, 50 ),FALSE,D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			pTitleText->SetFont ( "Arial", 16, 6, FALSE, FALSE, -1 );
			pTitleText->SetMultiLine( TRUE );
			pTitleText->SetNoClip( TRUE );
			pTitleText->SetColor ( D3DCOLOR_ARGB( 255, 245, 240, 218 )  );
			pTitleText->SetHorizontalAlign( ALIGN_Center );


			if ( hasSecondLine )
			{
				pTitleText2 = std::make_shared<Text> ( FormatString("%s", sSecondLine.c_str())
					, Rectangle2D ( 0, 0, 180, 50 ),FALSE,D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
				pTitleText2->SetFont ( "Arial", 16, 6, FALSE, FALSE, -1 );
				pTitleText2->SetMultiLine( TRUE );
				pTitleText2->SetNoClip( TRUE );
				pTitleText2->SetColor ( D3DCOLOR_ARGB( 255, 245, 240, 218 )  );
				pTitleText2->SetHorizontalAlign( ALIGN_Center );
			}

			int iGreatestWidth = 0;

			if (cItemWrapper.sItem.sItemID.ToItemType () == ITEMTYPE_Amulet )  //I Have absolutely no idea why, but amulats are always coming back with too small a number
				iGreatestWidth = pTitleText->GetTextWidth () + iEdgeSpacing * 2 + 15;
			else
				iGreatestWidth = pTitleText->GetTextWidth () + iEdgeSpacing * 2 ;

			if ( hasSecondLine )
				if ( iGreatestWidth < ( pTitleText2->GetTextWidth () + iEdgeSpacing * 2 ) )
					iGreatestWidth = pTitleText2->GetTextWidth () + iEdgeSpacing * 2;

			iMaxWidth = iGreatestWidth > iMaxWidth ? iGreatestWidth : iMaxWidth;

			int iItemPictureText = iGreatestWidth + 15 + cItemWrapper.iWidth + 22 - iItemImageXOffset;

			if ( bHasLargeName )
				iMaxWidth = iItemPictureText > iMaxWidth ? iItemPictureText : iMaxWidth;


			if ( hasSecondLine  )
			{
				pTitleText->SetBox( Rectangle2D((iMaxWidth - pTitleText->GetWidth())/2, 3, pTitleText->GetWidth(), pTitleText->GetHeightText() ) );
				pTitleText2->SetBox( Rectangle2D((iMaxWidth - pTitleText2->GetWidth())/2, 19, pTitleText2->GetWidth(), pTitleText2->GetHeightText() ) );
			}
			else
			{
				pTitleText->SetBox ( Rectangle2D ( ( iMaxWidth - pTitleText->GetWidth () ) / 2, bHasMultiLine ? 3 : 11, pTitleText->GetWidth (), pTitleText->GetHeightText () ) );
			}

		}

		if ( isCompare != TRUE )
			iBaseMaxWidth = iItemInfoBoxWidth;

		iItemInfoBoxWidth = iMaxWidth;

		//Adjusting the iItemSlot

		if ( isCompare != TRUE )
			iX = ( cItemWrapper.sPosition.iX + ( cItemWrapper.iWidth / 2 ) ) - iItemInfoBoxWidth/2 ;
		else
			iX = ( cItemWrapper.sPosition.iX + ( cItemWrapper.iWidth / 2 ) ) - iBaseMaxWidth/2 ;

		iY = cItemWrapper.sPosition.iY - ( vLeftText.size () * 14 ) ;

		if ( bDisplayHelp )
			iY -= 37;

		if ( bIsPerfectItem )
			iY += 19;

		if ( !bHasLargeName )
			iY -= 17;

		int iSocketHeightExtra = 0;

		for( int i = 0; i < 2; i++ )
		{
			if( cItemWrapper.sItem.sSocketData[i].eSocketType )
				iSocketHeightExtra += i > 0 ? 36 : 8;
		}

		//Set iWidth and iHeight of Box
		int iItemInfoBoxHeight = 0;
		if ( vLeftText.size() <= 2 )
			iItemInfoBoxHeight = ( 14 * 3 );  //Minimum Box height
		else
			iItemInfoBoxHeight = ( 14 * vLeftText.size() );

		//iItemSlot of box is out the window?
		if( iX < 0 )
			iX = 12;

		if( iY < 0 )
			iY = 24;

		if ( isCompare != TRUE )
		{
			if( iX + iItemInfoBoxWidth > (int)RESOLUTION_WIDTH )
				iX -= ( ( iX + iItemInfoBoxWidth ) - RESOLUTION_WIDTH ) + 16;
		}
		else
		{
			if( iX + iBaseMaxWidth > (int)RESOLUTION_WIDTH )
				iX -= ( ( iX + iBaseMaxWidth ) - RESOLUTION_WIDTH ) + 16;
		}

		if (isCompare)
			iX = iX + iBaseMaxWidth + 5;


		//Fix position if is showing from Inventory
		if( *( UINT* )0x036286EC == 2 )
			iY += RESOLUTION_HEIGHT - 600 - iSocketHeightExtra;

		//Update Item Info Box Width and Height
		iFoundWidth += 10;
		iItemInfoBoxHeight += 30 + iSocketHeightExtra;

		EItemRarity rarity = EItemRarity::NONE;

		if ( bIsEquippedItem )
		{
			rarity = cItemWrapper.sItem.eRarity;
		}

		//if ( bIsEquippedItem  ) ? cItemCompare.sItem.eRarity : cItemWrapper.sItem.eRarity;
		rarity = cItemWrapper.sItem.eRarity;

		//Draw New "Hold P" box
		if (iItemInfoBoxWidth != -1 && bIsPerfectItem != TRUE && bDisplayHelp )
		{
			Text_ptr pLeftText = std::make_shared<Text> ( "Hold       to see perfect stats", Rectangle2D ( 0, 0, 180, 20 ),FALSE,D3DCOLOR_ARGB( 255, 240, 215, 150 ) );


			pLeftText->SetFont ( "Arial", 14, 6, FALSE, FALSE, -1 );

			pLeftText->SetMultiLine( TRUE );
			pLeftText->SetNoClip( TRUE );

			pLeftText->SetColor ( D3DCOLOR_ARGB ( 220, 255, 255, 255 ) );

			//UI::ImageRender::Render( pImageBorderNoneDivider, (iX + 90) - (pImageBorderNoneDivider->GetWidth() >> 1), iY + (iSpecLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1 );

			pLeftText->SetHorizontalAlign( ALIGN_Center );
			pLeftText->SetBox( Rectangle2D((iItemInfoBoxWidth - pLeftText->GetWidth())/2, iItemInfoBoxHeight - 12, 180, iHeight ) );

			int Width = 0;

			if ( iItemInfoBoxWidth > 200 )
				Width = iItemInfoBoxWidth - 30;
			else
				Width = iItemInfoBoxWidth - 15;

			int Height = 25;
			int LeftEdge = iX + ( iItemInfoBoxWidth - Width)/ 2;
			int TopEdge = iY + iItemInfoBoxHeight - 15;

			int WidthBG = Width - 14;
			int HeightBG = 17;
			int LeftEdgeBG = iX + ( iItemInfoBoxWidth - WidthBG)/ 2;
			int TopEdgeBG = iY + iItemInfoBoxHeight - 15;

			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( LeftEdgeBG, TopEdgeBG, WidthBG, HeightBG), cBackgroundColor2.Get(), cBackgroundColor2.Get() );


			switch( rarity )
			{
				case EItemRarity::NONE:
				{
					UI::ImageRender::Render( pImageBorderNoneInfoLeft, LeftEdge, TopEdge, pImageBorderNoneInfoLeft->GetWidth(), pImageBorderNoneInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderNoneInfo, LeftEdge + pImageBorderNoneInfoLeft->GetWidth(), TopEdge, Width - pImageBorderNoneInfoLeft->GetWidth() - pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderNoneInfoRight, LeftEdge + Width - pImageBorderNoneInfoRight->GetWidth(), TopEdge, pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfoRight->GetHeight(), 0, 0, -1 );				}
				break;

				case EItemRarity::COMMON:
				{
					UI::ImageRender::Render( pImageBorderCommonInfoLeft, LeftEdge, TopEdge, pImageBorderCommonInfoLeft->GetWidth(), pImageBorderCommonInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderCommonInfo, LeftEdge + pImageBorderCommonInfoLeft->GetWidth(), TopEdge, Width - pImageBorderCommonInfoLeft->GetWidth() - pImageBorderCommonInfoRight->GetWidth(), pImageBorderCommonInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderCommonInfoRight, LeftEdge + Width - pImageBorderCommonInfoRight->GetWidth(), TopEdge, pImageBorderCommonInfoRight->GetWidth(), pImageBorderCommonInfoRight->GetHeight(), 0, 0, -1 );
				}
				break;

				case EItemRarity::UNCOMMON:
				{
					UI::ImageRender::Render( pImageBorderUncommonInfoLeft, LeftEdge, TopEdge, pImageBorderUncommonInfoLeft->GetWidth(), pImageBorderUncommonInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderUncommonInfo, LeftEdge + pImageBorderUncommonInfoLeft->GetWidth(), TopEdge, Width - pImageBorderUncommonInfoLeft->GetWidth() - pImageBorderUncommonInfoRight->GetWidth(), pImageBorderUncommonInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderUncommonInfoRight, LeftEdge + Width - pImageBorderUncommonInfoRight->GetWidth(), TopEdge, pImageBorderUncommonInfoRight->GetWidth(), pImageBorderUncommonInfoRight->GetHeight(), 0, 0, -1 );
				}
				break;

				case EItemRarity::RARE:
				{
					UI::ImageRender::Render( pImageBorderRareInfoLeft, LeftEdge, TopEdge, pImageBorderRareInfoLeft->GetWidth(), pImageBorderRareInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderRareInfo, LeftEdge + pImageBorderRareInfoLeft->GetWidth(), TopEdge, Width - pImageBorderRareInfoLeft->GetWidth() - pImageBorderRareInfoRight->GetWidth(), pImageBorderRareInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderRareInfoRight, LeftEdge + Width - pImageBorderRareInfoRight->GetWidth(), TopEdge, pImageBorderRareInfoRight->GetWidth(), pImageBorderRareInfoRight->GetHeight(), 0, 0, -1 );
				}
				break;

				case EItemRarity::EPIC:
				{
					UI::ImageRender::Render( pImageBorderEpicInfoLeft, LeftEdge, TopEdge, pImageBorderEpicInfoLeft->GetWidth(), pImageBorderEpicInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderEpicInfo, LeftEdge + pImageBorderEpicInfoLeft->GetWidth(), TopEdge, Width - pImageBorderEpicInfoLeft->GetWidth() - pImageBorderEpicInfoRight->GetWidth(), pImageBorderEpicInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderEpicInfoRight, LeftEdge + Width - pImageBorderEpicInfoRight->GetWidth(), TopEdge, pImageBorderEpicInfoRight->GetWidth(), pImageBorderEpicInfoRight->GetHeight(), 0, 0, -1 );
				}
				break;

				case EItemRarity::LEGENDARY:
				{
					UI::ImageRender::Render( pImageBorderLegendaryInfoLeft, LeftEdge, TopEdge, pImageBorderLegendaryInfoLeft->GetWidth(), pImageBorderLegendaryInfoLeft->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderLegendaryInfo, LeftEdge + pImageBorderLegendaryInfoLeft->GetWidth(), TopEdge, Width - pImageBorderLegendaryInfoLeft->GetWidth() - pImageBorderLegendaryInfoRight->GetWidth(), pImageBorderLegendaryInfo->GetHeight(), 0, 0, -1 );
					UI::ImageRender::Render( pImageBorderLegendaryInfoRight, LeftEdge + Width - pImageBorderLegendaryInfoRight->GetWidth(), TopEdge, pImageBorderLegendaryInfoRight->GetWidth(), pImageBorderLegendaryInfoRight->GetHeight(), 0, 0, -1 );
				}
				break;
			}

			UI::ImageRender::Render( pImageGradient, LeftEdge + 2, TopEdge , Width - 4, pImageGradient->GetHeight(), 0, 0, -1 );
			UI::ImageRender::Render( pImagePKey, iX + (iItemInfoBoxWidth/2) - 43, TopEdge + 4 , pImagePKey->GetWidth(), pImagePKey->GetHeight(), 0, 0, -1 );
			pLeftText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		}

		//Draw Item Info Box Background
		if( iItemInfoBoxWidth != -1 )
		{
			DrawItemInfoBox( iX, iY, iItemInfoBoxWidth, iItemInfoBoxHeight );

			if ( bHasLargeName )
			{
				int Width = iItemInfoBoxWidth - 10;
				int LeftEdge = iX + ( iItemInfoBoxWidth - (Width) )/ 2;
				int TopEdge = iY - 5;

				switch( rarity )
				{
					case EItemRarity::NONE:
					{
						UI::ImageRender::Render( pImageBorderNoneNameFrameLeft, LeftEdge, TopEdge, pImageBorderNoneNameFrameLeft->GetWidth(), pImageBorderNoneNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderNoneNameFrameCenter, LeftEdge + pImageBorderNoneNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderNoneNameFrameLeft->GetWidth() - pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderNoneNameFrameRight, LeftEdge + Width - pImageBorderNoneNameFrameRight->GetWidth(), TopEdge, pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderNoneNameFrameBackground,iX + (Width-pImageBorderNoneNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderNoneNameFrameBackground->GetWidth(), pImageBorderNoneNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
					case EItemRarity::COMMON:
					{
						UI::ImageRender::Render( pImageBorderCommonNameFrameLeft, LeftEdge, TopEdge, pImageBorderCommonNameFrameLeft->GetWidth(), pImageBorderCommonNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderCommonNameFrameCenter, LeftEdge + pImageBorderCommonNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderCommonNameFrameLeft->GetWidth() - pImageBorderCommonNameFrameRight->GetWidth(), pImageBorderCommonNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderCommonNameFrameRight, LeftEdge + Width - pImageBorderCommonNameFrameRight->GetWidth(), TopEdge, pImageBorderCommonNameFrameRight->GetWidth(), pImageBorderCommonNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderCommonNameFrameBackground,iX + (Width-pImageBorderCommonNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderCommonNameFrameBackground->GetWidth(), pImageBorderCommonNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
					case EItemRarity::UNCOMMON:
					{
						UI::ImageRender::Render( pImageBorderUncommonNameFrameLeft, LeftEdge, TopEdge, pImageBorderUncommonNameFrameLeft->GetWidth(), pImageBorderUncommonNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderUncommonNameFrameCenter, LeftEdge + pImageBorderUncommonNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderUncommonNameFrameLeft->GetWidth() - pImageBorderUncommonNameFrameRight->GetWidth(), pImageBorderUncommonNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderUncommonNameFrameRight, LeftEdge + Width - pImageBorderUncommonNameFrameRight->GetWidth(), TopEdge, pImageBorderUncommonNameFrameRight->GetWidth(), pImageBorderUncommonNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderUncommonNameFrameBackground,iX + (Width-pImageBorderUncommonNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderUncommonNameFrameBackground->GetWidth(), pImageBorderUncommonNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
					case EItemRarity::RARE:
					{
						UI::ImageRender::Render( pImageBorderRareNameFrameLeft, LeftEdge, TopEdge, pImageBorderRareNameFrameLeft->GetWidth(), pImageBorderRareNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderRareNameFrameCenter, LeftEdge + pImageBorderRareNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderRareNameFrameLeft->GetWidth() - pImageBorderRareNameFrameRight->GetWidth(), pImageBorderRareNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderRareNameFrameRight, LeftEdge + Width - pImageBorderRareNameFrameRight->GetWidth(), TopEdge, pImageBorderRareNameFrameRight->GetWidth(), pImageBorderRareNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderRareNameFrameBackground,iX + (Width-pImageBorderRareNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderRareNameFrameBackground->GetWidth(), pImageBorderRareNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
					case EItemRarity::EPIC:
					{
						UI::ImageRender::Render( pImageBorderEpicNameFrameLeft, LeftEdge, TopEdge, pImageBorderEpicNameFrameLeft->GetWidth(), pImageBorderEpicNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderEpicNameFrameCenter, LeftEdge + pImageBorderEpicNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderEpicNameFrameLeft->GetWidth() - pImageBorderEpicNameFrameRight->GetWidth(), pImageBorderEpicNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderEpicNameFrameRight, LeftEdge + Width - pImageBorderEpicNameFrameRight->GetWidth(), TopEdge, pImageBorderEpicNameFrameRight->GetWidth(), pImageBorderEpicNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderEpicNameFrameBackground,iX + (Width-pImageBorderEpicNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderEpicNameFrameBackground->GetWidth(), pImageBorderEpicNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
					case EItemRarity::LEGENDARY:
					{
						UI::ImageRender::Render( pImageBorderLegendaryNameFrameLeft, LeftEdge, TopEdge, pImageBorderLegendaryNameFrameLeft->GetWidth(), pImageBorderLegendaryNameFrameLeft->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderLegendaryNameFrameCenter, LeftEdge + pImageBorderLegendaryNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderLegendaryNameFrameLeft->GetWidth() - pImageBorderLegendaryNameFrameRight->GetWidth(), pImageBorderLegendaryNameFrameCenter->GetHeight(), 0, 0, -1 );
						UI::ImageRender::Render( pImageBorderLegendaryNameFrameRight, LeftEdge + Width - pImageBorderLegendaryNameFrameRight->GetWidth(), TopEdge, pImageBorderLegendaryNameFrameRight->GetWidth(), pImageBorderLegendaryNameFrameRight->GetHeight(), 0, 0, -1 );

						UI::ImageRender::Render( pImageBorderLegendaryNameFrameBackground,iX + (Width-pImageBorderLegendaryNameFrameBackground->GetWidth())/2 , iY - 5, pImageBorderLegendaryNameFrameBackground->GetWidth(), pImageBorderLegendaryNameFrameBackground->GetHeight(), 0, 0, -1 );
					}
					break;
				}

				pTitleText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				if ( hasSecondLine )
					pTitleText2->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

			}



			if ( rarity == EItemRarity::COMMON )
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderCommonLeftTop, iX, iY - 10, pImageBorderCommonLeftTop->GetWidth(), pImageBorderCommonLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderCommonTop, iX + pImageBorderCommonLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderCommonRightTop, iX + pImageBorderCommonLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), iY - 10, pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderCommonLeft, iX, iY - 10 + pImageBorderCommonLeftTop->GetHeight(), pImageBorderCommonLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderCommonRight, iX + iItemInfoBoxWidth - pImageBorderCommonRight->GetWidth(), iY - 10 + pImageBorderCommonLeftTop->GetHeight(), pImageBorderCommonRight->GetWidth(), iItemInfoBoxHeight - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderCommonLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderCommonLeftBottom->GetHeight(), pImageBorderCommonLeftBottom->GetWidth(), pImageBorderCommonLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderCommonRightBottom, iX + iItemInfoBoxWidth - pImageBorderCommonRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderCommonRightBottom->GetHeight(), pImageBorderCommonRightBottom->GetWidth(), pImageBorderCommonRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderCommonBottom, iX + pImageBorderCommonLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderCommonBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonBottom->GetHeight(), -1 );
			}
			else if ( rarity == EItemRarity::UNCOMMON )
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderUncommonLeftTop, iX, iY - 10, pImageBorderUncommonLeftTop->GetWidth(), pImageBorderUncommonLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonTop, iX + pImageBorderUncommonLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonRightTop, iX + pImageBorderUncommonLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), iY - 10, pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonLeft, iX, iY - 10 + pImageBorderUncommonLeftTop->GetHeight(), pImageBorderUncommonLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonRight, iX + iItemInfoBoxWidth - pImageBorderUncommonRight->GetWidth(), iY - 10 + pImageBorderUncommonLeftTop->GetHeight(), pImageBorderUncommonRight->GetWidth(), iItemInfoBoxHeight - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonLeftBottom->GetHeight(), pImageBorderUncommonLeftBottom->GetWidth(), pImageBorderUncommonLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonRightBottom, iX + iItemInfoBoxWidth - pImageBorderUncommonRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonRightBottom->GetHeight(), pImageBorderUncommonRightBottom->GetWidth(), pImageBorderUncommonRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderUncommonBottom, iX + pImageBorderUncommonLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonBottom->GetHeight(), -1 );

				UI::ImageRender::Render( pImageBorderUncommonCorner, iX - 11, iY - 22, pImageBorderUncommonCorner->GetWidth(), pImageBorderUncommonCorner->GetHeight(), 0, 0, -1,1,0 );
				//UI::ImageRender::Render( pImageBorderUncommonBottomCorner, iX + iItemInfoBoxWidth - pImageBorderUncommonBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonBottomCorner->GetHeight() + 10,pImageBorderUncommonBottomCorner->GetWidth(), pImageBorderUncommonBottomCorner->GetHeight(), -1 );
			}
			else if ( rarity == EItemRarity::RARE )
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderRareLeftTop, iX, iY - 10, pImageBorderRareLeftTop->GetWidth(), pImageBorderRareLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderRareTop, iX + pImageBorderRareLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), pImageBorderRareTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderRareRightTop, iX + pImageBorderRareLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), iY - 10, pImageBorderRareRightTop->GetWidth(), pImageBorderRareRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderRareLeft, iX, iY - 10 + pImageBorderRareLeftTop->GetHeight(), pImageBorderRareLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderRareRight, iX + iItemInfoBoxWidth - pImageBorderRareRight->GetWidth(), iY - 10 + pImageBorderRareLeftTop->GetHeight(), pImageBorderRareRight->GetWidth(), iItemInfoBoxHeight - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderRareLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderRareLeftBottom->GetHeight(), pImageBorderRareLeftBottom->GetWidth(), pImageBorderRareLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderRareRightBottom, iX + iItemInfoBoxWidth - pImageBorderRareRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderRareRightBottom->GetHeight(), pImageBorderRareRightBottom->GetWidth(), pImageBorderRareRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderRareBottom, iX + pImageBorderRareLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderRareBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), pImageBorderRareBottom->GetHeight(), -1 );

				UI::ImageRender::Render( pImageBorderRareCorner, iX - 11, iY - 22, pImageBorderRareCorner->GetWidth(), pImageBorderRareCorner->GetHeight(), 0, 0, -1,1,0 );
				//UI::ImageRender::Render( pImageBorderRareBottomCorner, iX + iItemInfoBoxWidth - pImageBorderRareBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderRareBottomCorner->GetHeight() + 10,pImageBorderRareBottomCorner->GetWidth(), pImageBorderRareBottomCorner->GetHeight(), -1 );
			}
			else if ( rarity == EItemRarity::EPIC )
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderEpicLeftTop, iX, iY - 10, pImageBorderEpicLeftTop->GetWidth(), pImageBorderEpicLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderEpicTop, iX + pImageBorderEpicLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderEpicRightTop, iX + pImageBorderEpicLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), iY - 10, pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderEpicLeft, iX, iY - 10 + pImageBorderEpicLeftTop->GetHeight(), pImageBorderEpicLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderEpicRight, iX + iItemInfoBoxWidth - pImageBorderEpicRight->GetWidth(), iY - 10 + pImageBorderEpicLeftTop->GetHeight(), pImageBorderEpicRight->GetWidth(), iItemInfoBoxHeight - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderEpicLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderEpicLeftBottom->GetHeight(), pImageBorderEpicLeftBottom->GetWidth(), pImageBorderEpicLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderEpicRightBottom, iX + iItemInfoBoxWidth - pImageBorderEpicRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderEpicRightBottom->GetHeight(), pImageBorderEpicRightBottom->GetWidth(), pImageBorderEpicRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderEpicBottom, iX + pImageBorderEpicLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderEpicBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicBottom->GetHeight(), -1 );

				UI::ImageRender::Render( pImageBorderEpicCorner, iX - 11, iY - 22, pImageBorderEpicCorner->GetWidth(), pImageBorderEpicCorner->GetHeight(), 0, 0, -1,1,0 );
				//UI::ImageRender::Render( pImageBorderEpicBottomCorner, iX + iItemInfoBoxWidth - pImageBorderEpicBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderEpicBottomCorner->GetHeight() + 10,pImageBorderEpicBottomCorner->GetWidth(), pImageBorderEpicBottomCorner->GetHeight(), -1 );
			}
			else if ( rarity == EItemRarity::LEGENDARY )
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderLegendaryLeftTop, iX, iY - 10, pImageBorderLegendaryLeftTop->GetWidth(), pImageBorderLegendaryLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryTop, iX + pImageBorderLegendaryLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryRightTop, iX + pImageBorderLegendaryLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), iY - 10, pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryLeft, iX, iY - 10 + pImageBorderLegendaryLeftTop->GetHeight(), pImageBorderLegendaryLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryRight, iX + iItemInfoBoxWidth - pImageBorderLegendaryRight->GetWidth(), iY - 10 + pImageBorderLegendaryLeftTop->GetHeight(), pImageBorderLegendaryRight->GetWidth(), iItemInfoBoxHeight - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryLeftBottom->GetHeight(), pImageBorderLegendaryLeftBottom->GetWidth(), pImageBorderLegendaryLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryRightBottom, iX + iItemInfoBoxWidth - pImageBorderLegendaryRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryRightBottom->GetHeight(), pImageBorderLegendaryRightBottom->GetWidth(), pImageBorderLegendaryRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderLegendaryBottom, iX + pImageBorderLegendaryLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryBottom->GetHeight(), -1 );

				UI::ImageRender::Render( pImageBorderLegendaryCorner, iX - 11, iY - 22, pImageBorderLegendaryCorner->GetWidth(), pImageBorderLegendaryCorner->GetHeight(), 0, 0, -1,1,0 );
				//UI::ImageRender::Render( pImageBorderLegendaryBottomCorner, iX + iItemInfoBoxWidth - pImageBorderLegendaryBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryBottomCorner->GetHeight() + 10,pImageBorderLegendaryBottomCorner->GetWidth(), pImageBorderLegendaryBottomCorner->GetHeight(), -1 );
			}
			else
			{
				//Drawing Border
				UI::ImageRender::Render( pImageBorderNoneLeftTop, iX, iY - 10, pImageBorderNoneLeftTop->GetWidth(), pImageBorderNoneLeftTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderNoneTop, iX + pImageBorderNoneLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderNoneRightTop, iX + pImageBorderNoneLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), iY - 10, pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneRightTop->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderNoneLeft, iX, iY - 10 + pImageBorderNoneLeftTop->GetHeight(), pImageBorderNoneLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderNoneRight, iX + iItemInfoBoxWidth - pImageBorderNoneRight->GetWidth(), iY - 10 + pImageBorderNoneLeftTop->GetHeight(), pImageBorderNoneRight->GetWidth(), iItemInfoBoxHeight - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneLeftBottom->GetWidth(), -1 );
				UI::ImageRender::Render( pImageBorderNoneLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderNoneLeftBottom->GetHeight(), pImageBorderNoneLeftBottom->GetWidth(), pImageBorderNoneLeftBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderNoneRightBottom, iX + iItemInfoBoxWidth - pImageBorderNoneRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderNoneRightBottom->GetHeight(), pImageBorderNoneRightBottom->GetWidth(), pImageBorderNoneRightBottom->GetHeight(), -1 );
				UI::ImageRender::Render( pImageBorderNoneBottom, iX + pImageBorderNoneLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderNoneBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneBottom->GetHeight(), -1 );
			}


			//Disabled til we can fix the trade chat link bug
			if ( bHasLargeName )
			{

				const EItemBase itemBase = cItemWrapper.sItem.sItemID.ToItemBase ();
				const EItemType itemType = cItemWrapper.sItem.sItemID.ToItemType ();

				if ( itemBase == ITEMBASE_Weapon ||
					itemType == ITEMTYPE_Armor ||
					itemType == ITEMTYPE_Boots ||
					itemType == ITEMTYPE_Shield ||
					itemType == ITEMTYPE_Gauntlets ||
					itemType == ITEMTYPE_Robe ||
					itemType == ITEMTYPE_Bracelets ||
					itemType == ITEMTYPE_Ring ||
					itemType == ITEMTYPE_Ring2 ||
					itemType == ITEMTYPE_Amulet ||
					itemType == ITEMTYPE_Orb ||
					itemType == ITEMTYPE_Sheltom )
				{

					const int iItemDrawPosX = iX + iItemInfoBoxWidth - 18 - cItemWrapper.iWidth + 22 - iItemImageXOffset;
					const int iItemDrawPosY = iY - 18 + iItemImageYOffset;


					
					//if (cItemWrapper.pcInventoryTexture)
					if (ITEMHANDLER->LoadItemImage(&cItemWrapper.sItem, &cItemWrapperImage))
					{
						ITEMHANDLER->DrawSprite ( iItemDrawPosX, iItemDrawPosY, (int)cItemWrapperImage.pcInventoryTexture, 0, 0, cItemWrapper.iWidth, cItemWrapper.iHeight );

						iPerivousItemID = cItemWrapper.sBaseItemID.ToInt ();
						iPerivousItemImage = (int)cItemWrapperImage.pcInventoryTexture;
						iPerivousItemWidth = cItemWrapper.iWidth ;
						iPerivousItemHeight = cItemWrapper.iHeight ;
					}
					else
					{
						if ( cItemWrapper.sBaseItemID.ToInt () == iPerivousItemID )
							ITEMHANDLER->DrawSprite ( iItemDrawPosX, iItemDrawPosY, iPerivousItemImage, 0, 0, iPerivousItemWidth, iPerivousItemHeight );
						else
						{
							iPerivousItemID = 0;
							iPerivousItemImage = 0;
							iPerivousItemWidth = 0;
							iPerivousItemHeight = 0;
						}
					}
				}
			}
		}

		//Render Text
		if( vLeftText.size() == vRightText.size() )
		{


			for( size_t i = 0; i < vLeftText.size(); i++ )
			{
				Text_ptr pLeftText = vLeftText[i];
				Text_ptr pRightText = vRightText[i];

				//Load Texts
				pLeftText->SetMultiLine( TRUE );
				pLeftText->SetNoClip( TRUE );

				pRightText->SetNoClip( TRUE );

				pLeftText->SetHorizontalAlign( ALIGN_Left );
				pLeftText->SetBox( Rectangle2D( 20, 5 + (14 * i), 180, iHeight ) );

				pRightText->SetHorizontalAlign( ALIGN_Left );
				pRightText->SetBox( Rectangle2D( pLeftText->GetTextWidth() + 25, 5 + (14 * i), 180, iHeight ) );

				if( pLeftText->GetTextWidth() + pRightText->GetTextWidth() + 20 > iFoundWidth )
					iFoundWidth = pLeftText->GetTextWidth() + pRightText->GetTextWidth() + 25;

				//Render Texts
				if( iItemInfoBoxWidth != -1 )
				{
					pLeftText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
					pRightText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
			}
		}



		if (iItemInfoBoxWidth != -1 && iSpecLine > 0 )
		{
			switch( rarity )
			{
				case EItemRarity::NONE:
				UI::ImageRender::Render( pImageBorderNoneDivider, iX +  (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::COMMON:
				UI::ImageRender::Render( pImageBorderCommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::UNCOMMON:
				UI::ImageRender::Render( pImageBorderUncommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::RARE:
				UI::ImageRender::Render( pImageBorderRareDivider, iX +  (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::EPIC:
				UI::ImageRender::Render( pImageBorderEpicDivider, iX +  (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::LEGENDARY:
				UI::ImageRender::Render( pImageBorderLegendaryDivider, iX +  (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth())/2 , iY + (iSpecLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1 );
				break;
			}
		}

#ifdef ITEMFIXES
		if (iItemInfoBoxWidth != -1 && iFixLine > 0 )
		{
			switch( rarity )
			{
				case EItemRarity::NONE:
				UI::ImageRender::Render( pImageBorderNoneDivider, iX +  (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::COMMON:
				UI::ImageRender::Render( pImageBorderCommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::UNCOMMON:
				UI::ImageRender::Render( pImageBorderUncommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::RARE:
				UI::ImageRender::Render( pImageBorderRareDivider, iX +  (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::EPIC:
				UI::ImageRender::Render( pImageBorderEpicDivider, iX +  (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::LEGENDARY:
				UI::ImageRender::Render( pImageBorderLegendaryDivider, iX +  (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth())/2 , iY + (iFixLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1 );
				break;
			}
		}
#endif

		if (iItemInfoBoxWidth != -1 &&  iStatLine > 0 )
		{
			switch( rarity )
			{
				case EItemRarity::NONE:
				UI::ImageRender::Render( pImageBorderNoneDivider, iX +  (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::COMMON:
				UI::ImageRender::Render( pImageBorderCommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::UNCOMMON:
				UI::ImageRender::Render( pImageBorderUncommonDivider, iX +  (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::RARE:
				UI::ImageRender::Render( pImageBorderRareDivider, iX +  (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::EPIC:
				UI::ImageRender::Render( pImageBorderEpicDivider, iX +  (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1 );
				break;
				case EItemRarity::LEGENDARY:
				UI::ImageRender::Render( pImageBorderLegendaryDivider, iX +  (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth())/2 , iY + (iStatLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1 );
				break;
			}
		}

		////Render Socket System
		//if( iSocketHeightExtra && iItemInfoBoxWidth != -1 )
		//{
		//	iItemInfoBoxHeight -= iSocketHeightExtra + 20;

		//	GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + 15, iY + iItemInfoBoxHeight - 30, 150, 1 ), D3DCOLOR_ARGB( 255, 160, 130, 105 ) );

		//	for( int i = 0; i < iSocketCount; i++ )
		//	{
		//		//Render Socket Info
		//		paSocketText[i][0]->SetNoClip( TRUE );
		//		paSocketText[i][0]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 22 + ( i * 38 ), 120, 18 ) );
		//		paSocketText[i][0]->SetHorizontalAlign( ALIGN_Left );
		//		paSocketText[i][0]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		//		paSocketText[i][1]->SetNoClip( TRUE );
		//		paSocketText[i][1]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 8 + ( i * 38 ), 120, 18 ) );
		//		paSocketText[i][1]->SetHorizontalAlign( ALIGN_Left );
		//		paSocketText[i][1]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

		//		//Render Socket Background
		//		if( pImageSocketBackground )
		//			UI::ImageRender::Render( pImageSocketBackground, iX + 17, iY + iItemInfoBoxHeight - 18 + ( i * 38 ), 27, 27, -1 );

		//		//Render Socket Type
		//		for( int k = 1; k < SOCKETTYPE_Count; k++ )
		//		{
		//			if( cItemWrapper.sItem.sSocketData[i].eSocketType == (ESocketType)k )
		//			{
		//				UI::ImageRender::Render( paSocket[k-1], iX + 16, iY + iItemInfoBoxHeight - 19 + ( i * 38 ), 29, 29, -1 );
		//				break;
		//			}
		//		}

		//		//Render Socket Gem
		//		if( cItemWrapper.sItem.sSocketData[i].eGemItem != GEMITEM_None && paSocketGems[iSocketGem[i]] )
		//			UI::ImageRender::Render( paSocketGems[iSocketGem[i]], iX + 19, iY + iItemInfoBoxHeight - 15 + ( i * 38 ), 22, 22, -1 );
		//	}
		//}

		//iItemInfoBoxWidth = iFoundWidth < 184 ? 184 : iFoundWidth + 22;

		//Render Compare Text
		if ((bIsEquippedItem || bIsPerfectItem) && bCompareItem)
		{
			int padding = 20;
			int spacing = 10;

			//if (cItemCompare.sItem.sItemID.ToItemBase() == ITEMBASE_Weapon)
			//	padding = 40;

			//

			int iMaxWidth = 0;

			for (UINT i = 0; i < vRowCompare.size(); i++)
			{
				Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
				Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
				Text_ptr pCompareText = vCompareText[i];

				//Get Widest text line
				if ( ( pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + spacing + pCompareText->GetTextWidth () + padding * 2 ) > iMaxWidth )
					iMaxWidth =  pLeftText->GetTextWidth () + 5 + pRightText->GetTextWidth () + spacing + pCompareText->GetTextWidth () + padding * 2  ;
			}

			if ( iMaxWidth > iItemInfoBoxWidth )
			{
				iItemInfoBoxWidth = iMaxWidth;
			}

			for (UINT i = 0; i < vRowCompare.size(); i++)
			{
				Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
				Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
				Text_ptr pCompareText = vCompareText[i];

				//Load Texts
				pCompareText->SetNoClip(TRUE);
				pCompareText->SetHorizontalAlign(ALIGN_Right);

				pCompareText->SetBox(Rectangle2D(padding, 5 + (14 * (vRowCompare[i] - 1)), iItemInfoBoxWidth - padding * 2, iHeight));

				//Render Texts
				pCompareText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
			}

			//iItemInfoBoxWidth += padding;
		}

		//Mixing View
		if (iItemInfoBoxWidth != -1 &&
			cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Mixing &&
			cItemWrapper.sItem.eMixTypeName != EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
		{
			auto it = vMixSheltoms.find(cItemWrapper.sItem.sMixUniqueID1);
			if (it != vMixSheltoms.end())
			{
				int iYExtra = 59;

				int iSheltomWidth = 22;
				int iSheltomHeight = 22;

				std::vector<int> sheltoms = it->second;

				int iSheltomTypes = 0;
				for ( unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size (); iSheltomIndex++ )
					if ( sheltoms[iSheltomIndex] > 0 )
						iSheltomTypes++;

				int iTotalWidth = iSheltomTypes * iSheltomWidth;
				int iInitialOffset = (iItemInfoBoxWidth-iTotalWidth)/2;

				int iOffsetX = 0;
				int iUniqueSheltomCount = 0;

				for (unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++)
				{
					if (sheltoms[iSheltomIndex] > 0)
					{
						UI::ImageRender::Render(vSheltomImages[iSheltomIndex], iX + iInitialOffset + iOffsetX, iY + 5 + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight, 0, 0, -1);

						//6 - there is only 6 sheltom count text
						paSheltomCountText[iUniqueSheltomCount]->SetText(FormatString("%d", sheltoms[iSheltomIndex]));
						paSheltomCountText[iUniqueSheltomCount]->SetNoClip(TRUE);
						paSheltomCountText[iUniqueSheltomCount]->SetBox(Rectangle2D(iInitialOffset + iOffsetX, iSheltomWidth + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight));
						paSheltomCountText[iUniqueSheltomCount]->SetHorizontalAlign(ALIGN_Center);
						paSheltomCountText[iUniqueSheltomCount]->SetVerticalAlign(ALIGN_Center);
						paSheltomCountText[iUniqueSheltomCount]->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

						iOffsetX += iSheltomWidth;
						iUniqueSheltomCount++;
					}
				}
			}
		}

		//Render Aging Bar
		if( iItemInfoBoxWidth != -1 )
		{
			if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
			{
				int iYExtra = 30;

				if ( bHasLargeName )
				{
					iYExtra += 28;
				}

				if( cItemWrapper.sItem.sMatureBar.sMax )
				{
					int iBarLenght = (int)((float)((float)125 * (((float)cItemWrapper.sItem.sMatureBar.sCur / (float)cItemWrapper.sItem.sMatureBar.sMax))));

					if( pImageAgingBar )
						UI::ImageRender::Render( pImageAgingBar, iX + (iItemInfoBoxWidth - pImageAgingBar->GetWidth())/2, iY + iSheltomYPosExtra - 20 + iYExtra, 130, 19, 0, 0, -1 );

					if( pImageAgingBarGage )
						UI::ImageRender::Render( pImageAgingBarGage, iX + (iItemInfoBoxWidth - pImageAgingBarGage->GetWidth())/2, iY + iSheltomYPosExtra - 13 + iYExtra, iBarLenght, 4, 0, 0, -1 );
				}

				if ( pImageAgingBarNum )
					UI::ImageRender::Render( pImageAgingBarNum, iX + ( iItemInfoBoxWidth - pImageAgingBarNum->GetWidth() ) / 2, iY + iSheltomYPosExtra - 20 + iYExtra, 26, 19, 0, 0, -1 );

				//Render Aging Text
				if ( pTextAging )
				{
					pTextAging->SetNoClip( TRUE );
					pTextAging->SetText( FormatString( "+%d", cItemWrapper.sItem.sAgeLevel ) );
					pTextAging->SetBox( Rectangle2D( ( iItemInfoBoxWidth - 24 ) / 2, iSheltomYPosExtra - 20 + iYExtra, 24, 18 ) );
					pTextAging->SetHorizontalAlign( ALIGN_Center );
					pTextAging->SetVerticalAlign( ALIGN_Center );
					pTextAging->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}

				int iSheltomWidth = 22;
				int iSheltomHeight = 22;

				const int ageLevel = cItemWrapper.sItem.bMaturing ? cItemWrapper.sItem.sAgeLevel + 1 : cItemWrapper.sItem.sAgeLevel;

				//Render Sheltoms used for aging
				auto it = vAgeSheltoms.find( ageLevel );
				if ( it != vAgeSheltoms.end() )
				{
					std::vector<int> sheltoms = it->second;

					int iSheltomTypes = 0;
					for ( unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++ )
						if ( sheltoms[iSheltomIndex] > 0 )
							iSheltomTypes++;

					int iTotalWidth = iSheltomTypes * iSheltomWidth;
					int iInitialOffset = ( iItemInfoBoxWidth - iTotalWidth ) / 2;

					int iOffsetX = 0;
					int iUniqueSheltomCount = 0;

					for ( unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++ )
					{
						if ( sheltoms[iSheltomIndex] > 0 )
						{
							UI::ImageRender::Render( vSheltomImages[iSheltomIndex], iX + iInitialOffset + iOffsetX, iY + 5 + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight, 0, 0, -1 );

							//7 - there is only 7 sheltom count text
							paSheltomCountText[iUniqueSheltomCount]->SetText( FormatString( "%d", sheltoms[iSheltomIndex] ) );
							paSheltomCountText[iUniqueSheltomCount]->SetNoClip( TRUE );
							paSheltomCountText[iUniqueSheltomCount]->SetBox( Rectangle2D( iInitialOffset + iOffsetX, iSheltomWidth + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight ) );
							paSheltomCountText[iUniqueSheltomCount]->SetHorizontalAlign( ALIGN_Center );
							paSheltomCountText[iUniqueSheltomCount]->SetVerticalAlign( ALIGN_Center );
							paSheltomCountText[iUniqueSheltomCount]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

							iOffsetX += iSheltomWidth;
							iUniqueSheltomCount++;
						}
					}
				}

			}
		}

	}
}

void ItemInfoBox::Update( float fTime )
{
	//if( bIsPerfectItem )
	//{
	//	if( bFadeToColor )
	//	{
	//		fProgress += 0.01f;

	//		if( fProgress > 1.0f )
	//		{
	//			bFadeToColor = FALSE;
	//			fProgress = 1.0f;
	//		}
	//	}
	//	else if( !bFadeToColor )
	//	{
	//		fProgress -= 0.005f;

	//		if( fProgress < 0.0f )
	//		{
	//			bFadeToColor = TRUE;
	//			fProgress = 0.0f;
	//		}
	//	}

	//	cBackgroundColor2 = UI::Color( 245, 9 + (int)(15.0f * fProgress), 23 + (int)(19.0f * fProgress), 22 + (int)(34.0f * fProgress) );
	//}
}

void ItemInfoBox::DrawItemInfoBox( int iX, int iY, int iWidth, int iHeight )
{
	//Draw Item Info Box Background
	if ( pRenderTargetMask )
	{
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

			//Draw Background
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( 8, 8, iWidth - 16, iHeight - 16), cBackgroundColor1.Get(), cBackgroundColor2.Get() );

			//Drawing Mask Border
			UI::ImageRender::Render( pImageMaskLeftTop, 0, 0, pImageMaskLeftTop->GetWidth(), pImageMaskLeftTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightTop, iWidth - pImageMaskRightTop->GetWidth(), 0, pImageMaskRightTop->GetWidth(), pImageMaskRightTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskLeftBottom, 0, iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskLeftBottom->GetWidth(), pImageMaskLeftBottom->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightBottom, iWidth - pImageMaskRightTop->GetWidth(), iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskRightBottom->GetWidth(), pImageMaskRightBottom->GetHeight(), -1 );

			//Back Old Render Target
			GetGraphicsDevice()->UnsetRenderTarget();
		}

		//Set Sprite Effect...
		UI::ImageRender::SetEffect( pEffectMask );
		pEffectMask->SetTechnique( "Mask" );

		if ( pEffectMask->Begin() >= 0 )
		{
			//Render the Render Target and mask it on effect
			UI::ImageRender::Render( pRenderTargetMask->GetTexture(), iX, iY - 10, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), FALSE );

			pEffectMask->End();
		}
		UI::ImageRender::SetEffect( NULL );
	}
}
}