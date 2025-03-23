#include "stdafx.h"
#include "UISkillInfoBox.h"
#include "SkillManager.h"

int iaUpgradeSkillCost[20];
int iaUpgradeSkillCostPlus[20];

#define SKILLINFOBOXWIDTH 264

namespace UI
{
SkillInfoBox::SkillInfoBox()
{
	iSkillInfoOffset = 0;
	iSkillInfoBoxHeight = 0;
}

SkillInfoBox::~SkillInfoBox()
{
}


BOOL SkillInfoBox::IsDifferentSkill ( Skill * psSkill )
{
	if ( cSkillWrapper.sSkillInfo.eSkillID == psSkill->sSkillInfo.eSkillID )
		return FALSE;

	return TRUE;
}


BOOL SkillInfoBox::CanPrepareShowSkill( Skill * psSkill, Rectangle2D sBox )
{
	BOOL bRet = TRUE;

	if ( cSkillWrapper.sSkillInfo.eSkillID == psSkill->sSkillInfo.eSkillID )
		bRet = FALSE;

	//special case, to force redraw of skill stats
	if ( bInvalidate )
	{
		bRet = TRUE;
		bInvalidate = FALSE;
	}

	if( !bRet )
	{
		if ( cSkillWrapper.iLevel != psSkill->iLevel )
			bRet = TRUE;

		if( !sSkillBox.Equals( &sBox ) )
			bRet = TRUE;
	}

	return bRet;
}

void SkillInfoBox::AddSkillElement( EElementID eElementID )
{
	switch ( eElementID )
	{
		case EElementID::Organic:
		case EElementID::Fire:
		case EElementID::Lightning:
		case EElementID::Ice:
		case EElementID::Poison:
			break;
		default:
			return; //invalid
	}

	vSkillElements.push_back( eElementID );
}

UI::Text_ptr SkillInfoBox::AddString( int iSide, const std::string & strString, DWORD dwColor, BOOL bBold, EAlign eAlign )
{
	switch( iSide )
	{
		case 0: //Left Side
			//Is Multiline?
			if(strString == "\n")
			{
				size_t n = std::count( strString.begin(), strString.end(), '\n' );
				iSkillInfoBoxHeight += 14 * n;
			}
			else if (strString.find('\n') != std::string::npos)
			{
				UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(14, iSkillInfoBoxHeight, SKILLINFOBOXWIDTH + iExtraWidth - 20, 0), bBold, dwColor);
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine(TRUE);
				pText->SetWordWrap(TRUE);
				pText->SetHorizontalAlign ( eAlign );
				pText->SetValue( iValue );
				iSkillInfoBoxHeight += pText->GetHeightText();
				vLeftText.push_back(pText);

				return pText;
			}
			else
			{
				UI::Text_ptr pText = std::make_shared<Text>( strString, Rectangle2D( 14, iSkillInfoBoxHeight, SKILLINFOBOXWIDTH + iExtraWidth - 20, 0 ), bBold, dwColor );
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine( TRUE );
				pText->SetWordWrap( TRUE );
				pText->SetHorizontalAlign ( eAlign );
				pText->SetValue( iValue );
				iSkillInfoBoxHeight += pText->GetHeightText();
				vLeftText.push_back( pText );

				return pText;
			}
			break;
		case 1: //Right Side
		{
			UI::Text_ptr pText = std::make_shared<Text>( strString, Rectangle2D( 14 + vLeftText[vLeftText.size() - 1]->GetTextWidth() + 6, iSkillInfoBoxHeight - vLeftText[vLeftText.size() - 1]->GetHeightText(), SKILLINFOBOXWIDTH + iExtraWidth - 20, 20 ), bBold, dwColor );
			pText->SetText(trim((std::string)strString));
			pText->SetMultiLine( TRUE );
			pText->SetWordWrap( TRUE );
			pText->SetValue( iValue );
			pText->SetHorizontalAlign ( eAlign );
			vRightText.push_back( pText );

			return pText;

			break;
		}
		case 2:
		{
			UINT uIndex = uSkillStatusPos + vCompareText.size();

			if ( (uIndex < vLeftText.size()) && (uIndex < vRightText.size()) )
			{
				if ( eAlign == EAlign::ALIGN_Right )
				{
					UI::Text_ptr pText = std::make_shared<Text> ( strString, Rectangle2D ( 14, vLeftText[uIndex]->GetY (), SKILLINFOBOXWIDTH + iExtraWidth - 20, 0 ), bBold, dwColor );
					pText->SetText ( trim ( ( std::string )strString ) );
					pText->SetMultiLine ( TRUE );
					pText->SetWordWrap ( TRUE );
					pText->SetValue( iValue );
					pText->SetHorizontalAlign ( EAlign::ALIGN_Right );
					vCompareText.push_back ( pText );

					return pText;
				}
				else
				{
					UI::Text_ptr pText = std::make_shared<Text> ( strString, Rectangle2D ( vLeftText[uIndex]->GetTextWidth () + vRightText[uIndex]->GetTextWidth () + 24, vLeftText[uIndex]->GetY (), SKILLINFOBOXWIDTH + iExtraWidth - 20, 0 ), bBold, dwColor );
					pText->SetText ( trim ( ( std::string )strString ) );
					pText->SetMultiLine ( TRUE );
					pText->SetWordWrap ( TRUE );
					pText->SetValue( iValue );
					pText->SetHorizontalAlign ( EAlign::ALIGN_Left );
					vCompareText.push_back ( pText );

					return pText;
				}


			}
			break;
		}
	}

	return nullptr;
}

void SkillInfoBox::SetLineColor( DWORD dwColor, int iSide, int iLineCount )
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

void SkillInfoBox::Init()
{
	//Load Images
	pImageMaskLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topleft.png" );
	pImageMaskRightTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topright.png" );
	pImageMaskLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomleft.png" );
	pImageMaskRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomright.png" );

	//Load Border Images
	pImageBorderLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topleft.png" );
	pImageBorderRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topright.png" );
	pImageBorderTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\top.png" );
	pImageBorderRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomright.png" );
	pImageBorderLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomleft.png" );
	pImageBorderBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottom.png" );
	pImageBorderLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\left.png" );
	pImageBorderRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\right.png" );

	pRenderTargetMask = GetGraphicsDevice()->MakeRenderTarget( 5001, 600 + iExtraWidth, 600 );
	pEffectMask = GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\InfoBox.fx" );

	for( int i = 0; i < 12; i++ )
	{
		pImageIconItems[i] = ImageLoader::LoadImage( "image\\Sinimage\\skill\\WeaponIcon\\%d.bmp", i + 1 );
	}

	pImageSkillElements[EElementID::Organic]  = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\element\\Organic.png" );
	pImageSkillElements[EElementID::Fire]	   = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\element\\Fire.png" );
	pImageSkillElements[EElementID::Ice]	   = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\element\\Ice.png" );
	pImageSkillElements[EElementID::Lightning] = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\element\\Lighting.png" );
	pImageSkillElements[EElementID::Poison]   = UI::ImageLoader::LoadImageA( "game\\images\\UI\\hud\\target\\element\\Poison.png" );
}

BOOL SkillInfoBox::OnKeyPress ( class CKeyboard * pcKeyboard )
{
	if ( WINDOW_ISOPEN_SKILLS )
	{
		int SkillInfoShowFlag = *(int *)0x367DF48;
		int SkillButtonIndex = *(int *)0x367DF88;

		if ( SkillInfoShowFlag && SkillButtonIndex )
		{
			if ( pcKeyboard->GetKey () == VK_RIGHT || pcKeyboard->GetKey () == VK_LEFT )
			{
				Skill * sCurrentSkill = &SKILLSCHARACTER[SkillButtonIndex - 1];

				if ( pcKeyboard->GetEvent () == EKeyboardEvent::KeyUp )
				{
					if ( pcKeyboard->GetKey () == VK_LEFT )
					{
						if ( ( sCurrentSkill->iLevel + iSkillInfoOffset ) >= 2 )
						{
							iSkillInfoOffset--;
						}
					}
					else
					{
						if ( ( sCurrentSkill->iLevel + iSkillInfoOffset ) <= 9 )
						{
							iSkillInfoOffset++;
						}
					}
				}

				//return True to prevent other key presses (which causes zooming)
				return TRUE;
			}
		}
	}

	return FALSE;
}

void SkillInfoBox::FormatSkillInfo()
{
	BOOL bUpdateSkill = FALSE;

	DWORD iSkillID = 0;


	if ( ( cSkillWrapper.iID & 0xFFFF0000 ) == TIMERSKILLBASE )
	{
		iSkillID = cSkillWrapper.sSkillInfo.eSkillID;
	}
	else
	{
		iSkillID = cSkillWrapper.iID;
	}

	//Tier 5 Skills? So format skill info here
	if ( (iSkillID & 0x00FF0000) == 0x00100000 )
		bUpdateSkill = TRUE;

	uSkillStatusPos = vLeftText.size();
	UINT uSkillLevelZeroBased = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevelZeroBased = 0;


	if ( SKILLMANAGERHANDLER->AddSkillInfoToBox ( iSkillID, uSkillLevelZeroBased ) )
	{
		bUpdateSkill = TRUE;
	}
	else
	{

		switch ( iSkillID )
		{

		case SKILLID_Rupture:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMechT51DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaMechT51DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_ParasitShot:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMechT52DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Poison Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaMechT52PoisonTimeDuration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaMechT52DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaMechT52PoisonTimeDuration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HardeningShield:
			AddString ( 0, "Damage Reduction:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMechT53DMGReduction[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "WP ATK Rate Reduction:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMechT53WPAtkRateRed[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaMechT53DMGReduction[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaMechT53WPAtkRateRed[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MagneticDischarge:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMechT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaMechT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_SurvivalInstinct:
			AddString ( 0, "Add HP Regen:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%.1f", faFighterT51AddHPRegen[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Add HP Potion Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaFighterT51AddHPPotionBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Add HP Boost Health:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaFighterT51BoostHealthBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%.1f)", faFighterT51AddHPRegen[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaFighterT51AddHPPotionBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaFighterT51BoostHealthBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SeismicImpact:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaFighterT52DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Area:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaFighterT52Area[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaFighterT52DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaFighterT52Area[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RuthlessofZecram:
			AddString ( 0, "HP Decrease:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%.1f%%", faFighterT53HPDec[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Hits for boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaFighterT53HitsBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Final DMG Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaFighterT53DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%.1f%%)", faFighterT53HPDec[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaFighterT53HitsBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaFighterT53DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Whirlwind:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaFighterT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaFighterT54Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "MP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaFighterT54MPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "SP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaFighterT54SPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaFighterT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaFighterT54Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaFighterT54MPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaFighterT54SPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RingofSpears:
			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", iaPikeT51Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%dsec)", iaPikeT51Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_Focus:
			AddString ( 0, "Min ATK Reduced:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPikeT52MinATKRed[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Max ATK Increased:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPikeT52MaxATKInc[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d)", iaPikeT52MinATKRed[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPikeT52MaxATKInc[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DeathMaster:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPikeT53DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "WP ATK Rate Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPikeT53ATKRate[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaPikeT53DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaPikeT53ATKRate[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Twister:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPikeT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaPikeT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_LethalSight:
			AddString ( 0, "Range Add:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaArcherT51RangeAdd[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Critical Rate:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaArcherT51CritRate[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "After Damage:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaArcherT51DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d)", iaArcherT51RangeAdd[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaArcherT51CritRate[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaArcherT51DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FierceWind:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaArcherT52DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Critical DMG Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaArcherT52CDMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaArcherT52DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaArcherT52CDMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_EntalingRoots:
			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaArcherT53Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d)", iaArcherT53Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_Bombardment:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaArcherT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaArcherT54Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "MP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaArcherT54MPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "SP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaArcherT54SPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaArcherT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaArcherT54Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaArcherT54MPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaArcherT54SPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyConviction:
			AddString ( 0, "Undead DMG Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaKnightT51DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaKnightT51DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_DivineInquisiton:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaKnightT52DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Area:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaKnightT52Area[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Undead Damage:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaKnightT52UndeadDMG[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaKnightT52DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaKnightT52Area[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaKnightT52UndeadDMG[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GloriousShield:
			AddString ( 0, "Shield Def Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaKnightT53DefBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaKnightT53DefBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_DivineCross:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaKnightT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
				AddString ( 2, FormatString ( "(%d%%)", iaKnightT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			break;
		case SKILLID_SummonTiger:
			AddString ( 0, "Pet Settings:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%s", SKILLMANAGERHANDLER->IsAgressive ( SKILLID_SummonTiger ) ? "Agressive" : "Passive" ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Attack Power:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d-%d", iaSummonTigerAttackPower[uSkillLevelZeroBased], iaSummonTigerAttackPower[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Attack Rating:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaSummonTigerAttackRating[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "HP:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaSummonTigerHP[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", 300 ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "" ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d-%d)", iaSummonTigerAttackPower[uSkillLevelZeroBased + 1], iaSummonTigerAttackPower[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaSummonTigerAttackRating[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaSummonTigerHP[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%dsec)", 300 ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PlagueJavelin:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaAtaT51DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Targets:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaAtaT51Targets[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Poison Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaAtaT51PoisonDuration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaAtaT51DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaAtaT51Targets[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaAtaT51PoisonDuration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GoldenApple:
			AddString ( 0, "Speed:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaAtaT52Speed[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Evade:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaAtaT52Evade[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaAtaT52Speed[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaAtaT52Evade[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceElemental:
			AddString ( 0, "Pet Settings:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%s", SKILLMANAGERHANDLER->IsAgressive ( SKILLID_IceElemental ) ? "Aggressive" : "Passive" ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Attack Power:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d-%d", iaIceElementalAttackPower[uSkillLevelZeroBased], iaIceElementalAttackPower[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Attack Rating:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaIceElementalAttackRating[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "HP:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaIceElementalHP[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", 300 ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "" ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d-%d)", iaIceElementalAttackPower[uSkillLevelZeroBased + 1], iaIceElementalAttackPower[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaIceElementalAttackRating[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaIceElementalHP[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%dsec)", 300 ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_LightningSurge:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPrsT52DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Area:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPrsT52Area[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Charged Area:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPrsT52ChargedArea[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaPrsT52DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPrsT52Area[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPrsT52ChargedArea[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HeavenlyLight:
			AddString ( 0, "ATK Rate Reduction:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPrsT53ATKRate[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPrsT53Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Charged Area:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPrsT53Area[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaPrsT53ATKRate[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPrsT53Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPrsT53Area[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Consecration:
			AddString ( 0, "Defense:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaPrsT54Defense[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Absorption:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaPrsT54Absorption[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaPrsT54Defense[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaPrsT54Absorption[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WizardTrance:
			AddString ( 0, "Orb Block Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMageT51BlockBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "ATK Rate Reduced:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMageT51ATKRateRed[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaMageT51BlockBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaMageT51ATKRateRed[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_StoneSkin:
			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", iaMageT52Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Run Speed Reduction:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMageT52RunSpeedRed[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%dsec)", iaMageT52Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaMageT52RunSpeedRed[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RedRay:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMageT53DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Add Range:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaMageT53AddRange[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaMageT53DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaMageT53AddRange[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Cataclysm:
			AddString ( 0, "Attack Power Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaMageT54DMGBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", iaMageT54Duration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "MP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaMageT54MPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "SP Drain:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaMageT54SPDrain[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaMageT54DMGBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%dsec)", iaMageT54Duration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaMageT54MPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaMageT54SPDrain[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShadowBlink:
			AddString ( 0, "Critical DMG Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaASNT51CritBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Range:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d", iaASNT51Range[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaASNT51CritBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d)", iaASNT51Range[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Creed:
			AddString ( 0, "MP Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaShaT51MPBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "STM Boost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%d%%", iaShaT51STMBoost[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", 300 ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString ( 2, FormatString ( "(%d%%)", iaShaT51MPBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%d%%)", iaShaT51STMBoost[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%dsec)", 300 ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}
			break;

			///// This is only to update in-game tooltip data
			// Newly added/modified skill info preperations for Season 1
			//

		//Priestess Tier 2
		/*case SKILLID_DivineLightning:
			AddString(0, "ATK Range:", D3DCOLOR_XRGB(243, 237, 216));
			AddString(1, FormatString("%d", iaPriestT2Range[uSkillLevel]), D3DCOLOR_XRGB(238, 210, 151));

			AddString(0, "DMG Boost:", D3DCOLOR_XRGB(243, 237, 216));
			AddString(1, FormatString("%d%%", iaPriestT2DMGBoost[uSkillLevel]), D3DCOLOR_XRGB(238, 210, 151));

			AddString(0, "Number of bolts:", D3DCOLOR_XRGB(243, 237, 216));

			AddString(1, FormatString("%d", SERVER_IS_SEASONAL ? iaPriestT2BoltsSeasonal[uSkillLevel] : iaPriestT2Bolts[uSkillLevel]), D3DCOLOR_XRGB(238, 210, 151));

			if (cSkillWrapper.iLevel < 10)
			{
				AddString(2, FormatString("(%d)", iaPriestT2Range[uSkillLevel + 1]), D3DCOLOR_XRGB(92, 163, 99));
				AddString(2, FormatString("(%d%%)", iaPriestT2DMGBoost[uSkillLevel + 1]), D3DCOLOR_XRGB(92, 163, 99));
				AddString ( 2, FormatString ( "(%d)", (SERVER_IS_SEASONAL ? iaPriestT2BoltsSeasonal[uSkillLevel + 1] : iaPriestT2Bolts[uSkillLevel + 1]) ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}

			bUpdateSkill = TRUE;
			break;*/

			//Magician Tier 1
		/*case SKILLID_Zenith:
		{
			if ( SERVER_IS_SEASONAL )
			{
				AddString ( 0, "Add Absorb: ", D3DCOLOR_XRGB ( 243, 237, 216 ) );
				AddString ( 1, FormatString ( "1 + %d%%", iaMagicianAddAbsorbPercentSeasonal[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );
			}
			else
			{
				AddString ( 0, "Add Absorb: ", D3DCOLOR_XRGB ( 243, 237, 216 ) );
				AddString ( 1, FormatString ( "%d", iaMagicianAddAbsorb[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );
			}

			//AddString ( 0, "Add Elemental Resistance:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			//AddString ( 1, FormatString ( "%d", iaMagicianAddResistance[uSkillLevel] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			AddString ( 0, "Duration:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%dsec", iaMagicianDuration[uSkillLevelZeroBased] ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

				if ( cSkillWrapper.iLevel < 10 )
				{
					AddString ( 2, FormatString ( "(1 + %d)", iaMagicianAddAbsorbPercentSeasonal[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				}
				else
				{
					AddString ( 2, FormatString ( "(%d)", iaMagicianAddAbsorb[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				}

				//AddString ( 2, FormatString ( "(%d)", iaMagicianAddResistance[uSkillLevel + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
				AddString ( 2, FormatString ( "(%dsec)", iaMagicianDuration[uSkillLevelZeroBased + 1] ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
			}

			bUpdateSkill = TRUE;

			break;
		}*/
		}
	}

	//Upgrade Cost
	if( cSkillWrapper.iLevel < 10 && WINDOW_ISOPEN_SKILLMASTER > 0 )
	{
		CopyMemory( iaUpgradeSkillCost, (char*)0x9BBE58, sizeof( int ) * 20 );
		CopyMemory( iaUpgradeSkillCostPlus, (char*)0x9BBEA8, sizeof( int ) * 20 );

		strSkillNextLevelCost = FormatString("%s gp",FormatNumber(iaUpgradeSkillCost[cSkillWrapper.sSkillInfo.iSkillIndex] + (iaUpgradeSkillCostPlus[cSkillWrapper.sSkillInfo.iSkillIndex] * (cSkillWrapper.iLevel))));
	}



	{
		std::string strBufferLeft = (char *)0x036762A0;
		std::string strBufferRight = (char *)0x03677628;

		std::vector<std::string> vStringLeft;
		vStringLeft = split ( strBufferLeft, '\r' );

		std::vector<std::string> vStringRight;
		vStringRight = split ( strBufferRight, '\r' );

#ifdef DEV_MODE

		//See https://docs.google.com/spreadsheets/d/1H4qZvjk9zpcN1MvRZEqCPs8Pt6SPQHrfiJ8mr-ZwoQE/edit#gid=2089939344
		//Todo - dump all raw asm skills into spreadsheet
		//then figure out the address to the array based on the raw values
		if ( GM_MODE )
		{
			static std::string lastCSVHeader;
			static std::string lastCSVValues;

			std::stringstream csvHeader;
			std::stringstream csvValues;

			csvHeader << "Skill level,";
			csvValues << cSkillWrapper.iLevel << ",";

			for ( unsigned int i = 0; i < vStringLeft.size (); i++ )
			{
				std::string key = trim ( vStringLeft[i] );
				std::string value = trim ( vStringRight[i] );


				if ( key.empty () )
					continue;

				if ( key.find ( ':' ) == key.length () - 1 )
				{
					key = key.substr ( 0, key.size () - 1 );
				}

				if ( vStringLeft[i].compare ( "Skill Metadata" ) == 0 )
					break;

				csvHeader << key << ",";
				csvValues << value << ",";
			}

			if ( cSkillWrapper.sSkillInfo.dwUseManaPointer )
			{
				csvHeader << "Use Mana" << ",";
				csvValues << ( (int *)cSkillWrapper.sSkillInfo.dwUseManaPointer )[cSkillWrapper.iLevel - 1] << ","; //Use Mana value
			}


			if ( lastCSVHeader.empty () ||
				 lastCSVHeader != csvHeader.str () )
			{
				std::string skillId = "";

				int iTier = 1;
				int iSkillNum = cSkillWrapper.sSkillInfo.iSkillIndex + 1;
				int iTierSkillNum = iSkillNum;

				if ( iSkillNum <= 4 )
				{
					iTier = 1;
					iTierSkillNum = iSkillNum;
				}
				else if ( iSkillNum >= 5 &&  iSkillNum <= 8 )
				{
					iTier = 2;
					iTierSkillNum -= 4;
				}
				else if ( iSkillNum >= 9 && iSkillNum <= 12 )
				{
					iTier = 3;
					iTierSkillNum -= 8;
				}
				else if ( iSkillNum >= 13 && iSkillNum <= 16 )
				{
					iTier = 4;
					iTierSkillNum -= 12;
				}

				auto headers = csvHeader.str ();
				headers = headers.substr ( 0, headers.size () - 1 ); //remove the last comma

				//UNITGAME->LogCSV ( "\n" );
				//UNITGAME->LogCSV ( FormatString ( "T%d_%d,Lvl_%d,%d,%d\n",  iTier, iTierSkillNum, cSkillWrapper.sSkillInfo.iSkillLevelRequired, cSkillWrapper.sSkillInfo.iSkillMasteryBase, cSkillWrapper.sSkillInfo.iSkillMasteryPerLevel ) );
				//UNITGAME->LogCSV ( FormatString ( "%s\n", cSkillWrapper.sSkillInfo.szName ) );
				//UNITGAME->LogCSV ( "---------------------------------------------\n" );
				//UNITGAME->LogCSV ( "%s\n", headers.c_str () );
				//UNITGAME->FlushLog ();

				lastCSVHeader = csvHeader.str ();
			}

			if ( lastCSVValues.empty () ||
				 lastCSVValues != csvValues.str () )
			{
				auto values = csvValues.str ();

				values = values.substr ( 0, values.size () - 1 ); //remove the last comma

				//UNITGAME->LogCSV ( "%s\n", values.c_str() );
				//UNITGAME->FlushLog ();

				lastCSVValues = csvValues.str ();
			}
		}
#endif

		if ( bUpdateSkill == FALSE )
		{
			for ( size_t i = 0; i < vStringLeft.size (); i++ )
			{
				if ( vStringLeft[i].empty () )
				{
					vStringLeft[i].erase ();
					vStringRight[i].erase ();
				}
			}

			bool bNextLevel = false;
			for ( size_t i = 0; i < vStringLeft.size (); i++ )
			{
				if ( i == 0 )
					uSkillStatusPos = vLeftText.size () + 1;

				if ( vStringLeft[i].compare ( "Skill Metadata" ) == 0 )
					bNextLevel = true;

				if ( !bNextLevel )
				{
					AddString ( 0, vStringLeft[i], D3DCOLOR_XRGB ( 243, 237, 216 ) );
					AddString ( 1, vStringRight[i], D3DCOLOR_XRGB ( 238, 210, 151 ) );
				}
				else
				{
					if ( !vStringRight[i].empty () )
					{
						if ( vStringRight[i].find ( "gp" ) != std::string::npos )
							strSkillNextLevelCost = vStringRight[i];
						else
							AddString ( 2, FormatString ( "(%s)", vStringRight[i].c_str () ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
					}
				}
			}
		}
	}
}




BOOL SkillInfoBox::PrepareShowSkill( Skill * pcSkill, BOOL bSkillTimer, Rectangle2D sBox )
{
	bInfoBoxSkillTimer = bSkillTimer;

	//Save Box Position
	sSkillBox = sBox;

	vSkillElements.clear();

	//Clear Vectors
	vLeftText.clear();
	vRightText.clear();
	vCompareText.clear();
	strSkillNextLevelCost.clear();

	uSkillStatusPos = 0;
	iSkillInfoBoxHeight = 20;

	//Copy pcSkill to cSkillWrapper
	CopyMemory( &cSkillWrapper, pcSkill, sizeof(Skill) );

	DWORD iSkillID = 0;

	if ( (pcSkill->iID & 0xFFFF0000) == TIMERSKILLBASE )
	{
		iSkillID = pcSkill->sSkillInfo.eSkillID;
	}
	else
	{
		iSkillID = pcSkill->iID;
	}


	int SkillButtonIndex = *(int *)0x367DF88;

	if ( iSkillInfoOffset != 0 && SkillButtonIndex != 0)
	{
		AddString ( 0, "PREVIEW", D3DCOLOR_XRGB ( 176, 171, 19 ), TRUE );
		AddString ( 1, "" );
		AddString ( 0, "----------------" );
		AddString ( 1, "" );
	}

	//Name Skill
	if ( iSkillID == SKILLID_DefaultAttack )
	{
		AddString( 0, "Default Attack", D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
		AddString( 1, "" );
	}
	else
	{
		AddString( 0, cSkillWrapper.sSkillInfo.szName, D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
		AddString( 1, "" );
	}

	//Level Skill
	if( cSkillWrapper.iLevel > 0 && iSkillID != SKILLID_ForceOrb  && iSkillID != SKILLID_QUEST1 && iSkillID != SKILLID_QUEST2 &&
		iSkillID != SKILLID_QUEST3 && iSkillID != SKILLID_QUEST4 && iSkillID != SKILLID_QUESTNew && iSkillID != SKILLID_QUESTNewTime &&
		(iSkillID < ITEMID_HopyCrystal || iSkillID > ITEMID_ArcticCrystal) && iSkillID != SKILLID_BellatraGold && iSkillID != SKILLID_BellatraSilver &&
		iSkillID != SKILLID_BellatraBronze && iSkillID != SKILLID_DefaultAttack && iSkillID != SKILLID_AbsorptionOfGlory && iSkillID != SKILLID_PowerSiege &&
		iSkillID != SKILLID_EvasionAdaption && iSkillID != SKILLID_BlessCastleBuff2 && iSkillID != SKILLID_BlessCastleBuff3 )
	{
		AddString( 0, "Rank" );
		AddString( 1, FormatString( "%d", cSkillWrapper.iLevel ) );
		SetLineColor( D3DCOLOR_XRGB( 243, 237, 216 ) );
	}

	//Required Level (only need to show in preview mode)
	if( (cSkillWrapper.iLevel >= 0 && cSkillWrapper.iLevel <= 10) && !bInfoBoxSkillTimer && iSkillID != SKILLID_DefaultAttack )
	{
		int iRequiredLevel = 0;

		//preview mode
		//if ( iSkillInfoOffset >= 1 )
			iRequiredLevel = (cSkillWrapper.sSkillInfo.iSkillLevelRequired + ((cSkillWrapper.iLevel - 1) * 2));
		//else
		//	iRequiredLevel = (cSkillWrapper.sSkillInfo.iSkillLevelRequired + (cSkillWrapper.iLevel -1 ) * 2));

		AddString( 0, "Required Level:" );
		AddString( 1, FormatString( "%d", iRequiredLevel ) );
		SetLineColor( D3DCOLOR_XRGB( 255, 185, 89 ) );
	}

	//Next Level
	if( cSkillWrapper.iLevel < 10 && cSkillWrapper.iLevel > 0 && !bInfoBoxSkillTimer && iSkillID != SKILLID_DefaultAttack )
	{
		int iRequiredLevel = 0;

		iRequiredLevel = (cSkillWrapper.sSkillInfo.iSkillLevelRequired + ((cSkillWrapper.iLevel + 0) * 2));

		AddString( 0, "Next Required Level:" );
		AddString( 1, FormatString( "%d", iRequiredLevel ) );
		SetLineColor( D3DCOLOR_XRGB( 92, 163, 99 ) );
	}


	//Type Skill
	if( iSkillID != SKILLID_ForceOrb &&
		iSkillID != SKILLID_QUEST1 &&
		iSkillID != SKILLID_QUEST2 &&
		iSkillID != SKILLID_QUEST3 &&
		iSkillID != SKILLID_QUEST4 )
	{
		AddString ( 0, "" );
		AddString ( 1, "" );
		SetLineColor( D3DCOLOR_XRGB( 255, 185, 89 ) );
	}

	if ( iSkillID == TIMERSKILLID_QuestNew || iSkillID == TIMERSKILLID_QuestNewT )
	{
		EQuestPartyType iQuestType = CQUESTGAME->GetQuestPartyType( &cSkillWrapper );

		if ( iQuestType == EQuestPartyType::SoloOnly )
			AddString( 0, "Solo only Quest" );
		else if ( iQuestType == EQuestPartyType::PartyOnly )
			AddString( 0, "Party only Quest" );
		else if ( iQuestType == EQuestPartyType::SoloAny )
			AddString( 0, "Solo Quest" );
		else if ( iQuestType == EQuestPartyType::SoloPartyAny )
			AddString( 0, "Solo/Party Quest" );
		else if ( iQuestType == EQuestPartyType::SoloAndParty )
			AddString( 0, "Solo/Party only Quest" );
		else if ( iQuestType == EQuestPartyType::PartyAndRaid )
			AddString( 0, "Party/Raid only Quest" );
		else if ( iQuestType == EQuestPartyType::SoloPartyAndRaid )
			AddString( 0, "Solo/Party/Raid Quest" );
		else if ( iQuestType == EQuestPartyType::RaidOnly )
			AddString( 0, "Raid only Quest" );
		else
			AddString( 0, "Quest" );

		AddString( 1, "" );

		iExtraWidth = 30;
	}

	AddString( 0, "\n" );

	//Description Skill
	AddString(0, cSkillWrapper.sSkillInfo.szDescription);
	AddString(1, "");
	SetLineColor(D3DCOLOR_XRGB(169, 169, 169));

	AddString(0, "\n");

	//Skill Locked
	if( SKILLDATAHANDLER->IsSkillLocked( iSkillID ) )
	{
		AddString( 0, "Skill not avaliable!" );
		AddString( 1, "" );
		SetLineColor( D3DCOLOR_XRGB( 200, 90, 90 ) );

		AddString( 0, "\n" );
	}

	//Correct Item Group
	if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[0] != ITEMTYPE_None )
	{
		uImageIconsItemsY = iSkillInfoBoxHeight - 4;
		AddString( 0, "\n" );
		AddString( 0, "\n" );
	}

	BOOL bAdded = FALSE;

	if ( SKILLMANAGERHANDLER->AddDamageVsMonsterTypeInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddSpecificMonsterTypeInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddSummonInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddBuffInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddDebuffInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddPassiveInfo ( &cSkillWrapper ) ) bAdded = TRUE;
	if ( SKILLMANAGERHANDLER->AddHintInfo ( &cSkillWrapper ) ) bAdded = TRUE;

	if ( bAdded )
	{
		AddString ( 0, "\n" ); //add a line break
	}



	//Format Skill Info
	FormatSkillInfo();

	if( !bInfoBoxSkillTimer && cSkillWrapper.sSkillInfo.eSkillType != SKILLTYPE_Passive )
		AddString( 0, "\n" );

	if( cSkillWrapper.iLevel > 0 && !bInfoBoxSkillTimer )
	{
		//MP
		if( cSkillWrapper.sSkillInfo.dwUseManaPointer )
		{
			int * piaUseMana = (int*)cSkillWrapper.sSkillInfo.dwUseManaPointer;
			int iManaCost = piaUseMana[cSkillWrapper.iLevel - 1];

			if( iManaCost != 0 )
			{
				AddString ( 0, "MP Cost:", D3DCOLOR_XRGB ( 243, 237, 216 ) );


				//Decrease MP Item? for example MP down
				if( ( READDWORD( 0x035D0C48 ) > 0 ) && ( READDWORD( 0x035D0C4C ) > 0 ) )
				{
					int iDecreaseMP = ( iManaCost * READDWORD( 0x035D0C4C ) ) / 100;
					AddString ( 1, FormatString ( "%d", iManaCost - iDecreaseMP ), D3DCOLOR_XRGB ( 67, 149, 204 ) );
					AddString ( 2, FormatString ( " (-%d)", iDecreaseMP ), D3DCOLOR_XRGB ( 100, 100, 100 ) );
				}
				else
				{
					AddString ( 1, FormatString ( "%d", iManaCost ), D3DCOLOR_XRGB ( 234, 186, 81 ) );
					AddString ( 2, "" );
				}
			}
		}

		//STM
		if( cSkillWrapper.sSkillInfo.iaSkillUseStamina[0] != 0 )
		{
			AddString( 0, "SP Cost:", D3DCOLOR_XRGB( 243, 237, 216 ) );

			int iSP = cSkillWrapper.sSkillInfo.iaSkillUseStamina[0] + ( cSkillWrapper.sSkillInfo.iaSkillUseStamina[1] * ( cSkillWrapper.iLevel - 1 ) );


			//Decrease STM Item?
			if( ( READDWORD( 0x035D0C60 ) > 0 ) && ( READDWORD( 0x035D0C64 ) > 0 ) ) // read game.exe offset value
			{
				int iDecreaseSP = ( iSP * READDWORD( 0x035D0C64 ) ) / 100;

				AddString ( 1, FormatString ( "%d", iSP - iDecreaseSP ), D3DCOLOR_XRGB ( 133, 204, 67 ) );
				AddString ( 2, FormatString ( " (-%d)", iDecreaseSP ), D3DCOLOR_XRGB ( 100, 100, 100 ) );
			}
			else
			{
				AddString ( 1, FormatString ( "%d", iSP ), D3DCOLOR_XRGB ( 234, 186, 81 ) );
				AddString ( 2, "" );
			}

			//SetLineColor( D3DCOLOR_XRGB( 234, 186, 81 ), 1 );
		}
	}

	//Cooldown Skill
	if( !bInfoBoxSkillTimer && (cSkillWrapper.iLevel > 0) && (cSkillWrapper.sSkillInfo.eSkillType != SKILLTYPE_Passive) && iSkillID != SKILLID_DefaultAttack )
	{
		int iMastery = cSkillWrapper.Mastery;

		int iMasteryBase = cSkillWrapper.sSkillInfo.iSkillMasteryBase;
		int iMasteryPerLevel = cSkillWrapper.sSkillInfo.iSkillMasteryPerLevel;

		if ( iMastery > MAX_MASTERY )
			iMastery = MAX_MASTERY;
		else if ( iMastery < 1 )
			iMastery = 1;

		float fSkillCooldown = ((MAX_MASTERY / (MAX_MASTERY / ((float)iMastery / 2.0f))) * 35) * (1000.0f / 60.0f);
		fSkillCooldown /= 1000.0f;

		int iFullyTrainedMastery = iMasteryBase + ( iMasteryPerLevel * cSkillWrapper.iLevel ) - ( 10000 / 100 );

		if ( iFullyTrainedMastery > MAX_MASTERY )
			iFullyTrainedMastery = MAX_MASTERY;
		else if ( iFullyTrainedMastery < 1 )
			iFullyTrainedMastery = 1;

		float fSkillCooldownFullyTrained = ( ( MAX_MASTERY / ( MAX_MASTERY / ( (float)iFullyTrainedMastery / 2.0f ) ) ) * 35 ) * ( 1000.0f / 60.0f );
		fSkillCooldownFullyTrained /= 1000.0f;

		BOOL bIsFullyTrained = fSkillCooldownFullyTrained == fSkillCooldown;


		//preview mode
		if ( iSkillInfoOffset != 0 )
		{
			AddString ( 0, "Cooldown:", D3DCOLOR_XRGB ( 243, 237, 216 ) );
			AddString ( 1, FormatString ( "%.1fs ( 100%% trained )", fSkillCooldownFullyTrained ), D3DCOLOR_XRGB ( 234, 186, 81 ) );
		}
		else
		{
			AddString ( 0, "Cooldown:", D3DCOLOR_XRGB ( 243, 237, 216 ) );

			if ( bIsFullyTrained )
				AddString ( 1, FormatString ( "%.1fs", fSkillCooldown ), D3DCOLOR_XRGB ( 234, 186, 81 ) );
			else
				AddString ( 1, FormatString ( "%.1fs ( %.0f%% trained )", fSkillCooldown, static_cast<float>( iMastery ) / static_cast<float>( iFullyTrainedMastery ) ), D3DCOLOR_XRGB ( 234, 186, 81 ) );

			if ( !bIsFullyTrained )
			{
				AddString ( 0, "Cooldown:", D3DCOLOR_XRGB ( 112, 111, 107 ) );
				AddString ( 1, FormatString ( "%.1fs ( at 100%% )", fSkillCooldownFullyTrained ), D3DCOLOR_XRGB ( 153, 123, 58 ) );
			}
		}
	}

	//Next Level Cost
	if( cSkillWrapper.iLevel < 10 && !strSkillNextLevelCost.empty() )
	{
		AddString( 0, "Upgrade Cost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
		AddString( 1, strSkillNextLevelCost, D3DCOLOR_XRGB( 234, 186, 81 ) );
	}
	else if ( cSkillWrapper.iLevel > 0 && !bInfoBoxSkillTimer && iSkillID != SKILLID_DefaultAttack && SkillButtonIndex >= 1 )
	{
		AddString ( 0, "-------------------", D3DCOLOR_XRGB ( 100, 100, 100 ) );
		AddString ( 1, "" );
		AddString ( 0, "Press left / right arrrow keys", D3DCOLOR_XRGB ( 100, 100, 100 ) );
		AddString ( 1, "" );
		AddString ( 0, "to preview skills info at other levels", D3DCOLOR_XRGB ( 100, 100, 100 ) );
		AddString ( 1, "" );
	}

	//Time Left
	if( bInfoBoxSkillTimer && (iSkillID & 0xFFFF0000 ) != 0x096A0000 && iSkillID != SKILLID_AbsorptionOfGlory && iSkillID != SKILLID_PowerSiege && iSkillID != SKILLID_EvasionAdaption &&
		(iSkillID != SKILLID_BlessCastleBuff2) && (iSkillID != SKILLID_BlessCastleBuff3) )
	{
		int iTimeLeft = 0;

		if( (cSkillWrapper.iCheckTime / 70) > 0 )
			iTimeLeft = cSkillWrapper.iUseTime - (cSkillWrapper.iCheckTime / 70);
		else
			iTimeLeft = (cSkillWrapper.iCheckTime / 70) - cSkillWrapper.iUseTime;

		int iHour = abs( iTimeLeft / 3600 );
		int iMinutes = abs( (iTimeLeft) / 60 ) % 60;
		int iSeconds = abs( (iTimeLeft) % 60 );

		AddString( 0, "\n" );
		AddString( 0, "Time Left:", D3DCOLOR_XRGB( 243, 237, 216 ) );
		AddString( 1, FormatString("%02d:%02d:%02d", iHour, iMinutes, iSeconds), D3DCOLOR_XRGB( 234, 186, 81 ) );

		if ( iSkillID == SKILLID_ForceOrb &&
			 (MAP_ID == MAPID_RicartenTown ||
			  MAP_ID == MAPID_PillaiTown ||
			  MAP_ID == MAPID_NaviskoTown ||
			  MAP_ID == MAPID_Eura ||
			  MAP_ID == MAPID_Atlantis))
		{
			AddString ( 0, "\n" );
			AddString ( 0, "Timer paused", D3DCOLOR_XRGB ( 255, 238, 0 ) );
			AddString ( 1, "" );
		}
	}

	return TRUE;
}

void SkillInfoBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( !READDWORD ( 0x367DF48 ) && !bInfoBoxSkillTimer )
	{
		iSkillInfoOffset = 0;
		return;
	}

	if ( bInfoBoxSkillTimer && !READDWORD ( 0x0367DE8C ) )
	{
		iSkillInfoOffset = 0;
		return;
	}
	//Set iWidth and iHeight of Box
	int iSkillInfoBoxWidth = SKILLINFOBOXWIDTH + iExtraWidth;

	//Adjust Box Position
	if( !bInfoBoxSkillTimer )
	{
		iX = sSkillBox.iX - ( iSkillInfoBoxWidth >> 1 ) + 20;
		iY = sSkillBox.iY - iSkillInfoBoxHeight;
	}
	else
	{
		iX = sSkillBox.iX;
		iY = sSkillBox.iY;
	}

	//iItemSlot of box is out the window?
	if( iX < 0 )
		iX = 0;

	if( iY < 0 )
		iY = 16;

	if( iX + iSkillInfoBoxWidth > (int)RESOLUTION_WIDTH )
		iX -= ( ( iX + iSkillInfoBoxWidth ) - RESOLUTION_WIDTH ) + 16;

	//Fix position
	if( !bInfoBoxSkillTimer )
		iY += RESOLUTION_HEIGHT - 600;

	//Draw Skill Info Box Background
	DrawSkillInfoBox( iX, iY + 20, iSkillInfoBoxWidth, iSkillInfoBoxHeight );

	int elementIndex = 0;

	for ( EElementID element : vSkillElements )
	{
		UI::ImageRender::Render( pImageSkillElements[element], iX + (iSkillInfoBoxWidth - ( elementIndex * 18 ) - 15 - 10), iY + 22, 15, 15, -1 );
		elementIndex += 1;
	}

	//Drawing Border
	UI::ImageRender::Render( pImageBorderLeftTop, iX, iY + 10, pImageBorderLeftTop->GetWidth(), pImageBorderLeftTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderTop, iX + pImageBorderLeftTop->GetWidth(), iY + 10, iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderRightTop, iX + pImageBorderLeftTop->GetWidth() + iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), iY + 10, pImageBorderRightTop->GetWidth(), pImageBorderRightTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderLeft, iX, iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderLeft->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
	UI::ImageRender::Render( pImageBorderRight, iX + iSkillInfoBoxWidth - pImageBorderRight->GetWidth(), iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderRight->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
	UI::ImageRender::Render( pImageBorderLeftBottom, iX, iY + 10 + iSkillInfoBoxHeight - pImageBorderLeftBottom->GetHeight(), pImageBorderLeftBottom->GetWidth(), pImageBorderLeftBottom->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderRightBottom, iX + iSkillInfoBoxWidth - pImageBorderRightBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderRightBottom->GetHeight(), pImageBorderRightBottom->GetWidth(), pImageBorderRightBottom->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderBottom, iX + pImageBorderLeftBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderBottom->GetHeight(), iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderBottom->GetHeight(), -1 );

	//Render Text
	int iAddTextHeight = 0;
	if( vLeftText.size() == vRightText.size() )
	{
		for( size_t i = 0; i < vLeftText.size(); i++ )
		{
			Text_ptr pLeftText = vLeftText[i];
			Text_ptr pRightText = vRightText[i];

			if ( pLeftText->GetPropertyID() == ETextPropertyID::Elemental )
			{
				UI::ImageRender::Render( pImageSkillElements[pLeftText->GetPropertyValue()], iX + (iSkillInfoBoxWidth - 15 - 10), iY + pLeftText->GetRect().iY, 15, 15, -1 );
			}

			//Load Texts
			pLeftText->SetNoClip( TRUE );
			pRightText->SetNoClip( TRUE );

			//pLeftText->SetHorizontalAlign( ALIGN_Left );
			//pRightText->SetHorizontalAlign( ALIGN_Left );

			//Render Texts
			pLeftText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			pRightText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}

		if( !bInfoBoxSkillTimer )
		{
			for( size_t i = 0; i < vCompareText.size(); i++ )
			{
				Text_ptr pCompareText = vCompareText[i];
				if( pCompareText )
				{
					pCompareText->SetNoClip( TRUE );
					//pCompareText->SetHorizontalAlign( ALIGN_Left );
					pCompareText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
			}
		}
	}

	//Render Correct Item Group
	if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[0] != ITEMTYPE_None && uImageIconsItemsY > 0 )
	{
		for( size_t i = 0; i < 8; i++ )
		{
			if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Axe )
				UI::ImageRender::Render( pImageIconItems[1], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Wand )
				UI::ImageRender::Render( pImageIconItems[2], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Hammer )
				UI::ImageRender::Render( pImageIconItems[3], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Shield )
				UI::ImageRender::Render( pImageIconItems[4], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Scythe )
				UI::ImageRender::Render( pImageIconItems[5], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Sword )
				UI::ImageRender::Render( pImageIconItems[6], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Claw )
				UI::ImageRender::Render( pImageIconItems[7], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Bow )
				UI::ImageRender::Render( pImageIconItems[8], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Javelin )
				UI::ImageRender::Render( pImageIconItems[9], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Dagger )
				UI::ImageRender::Render( pImageIconItems[10], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Phantom )
				UI::ImageRender::Render( pImageIconItems[11], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
		}
	}
}

void SkillInfoBox::DrawSkillInfoBox( int iX, int iY, int iWidth, int iHeight )
{
	//Draw Item Info Box Background
	if ( pRenderTargetMask )
	{
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );


			int SkillButtonIndex = *(int *)0x367DF88;

			//Draw Background
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( 0, 0, iWidth, iHeight ),
				(iSkillInfoOffset == 0 || SkillButtonIndex == 0) ? D3DCOLOR_ARGB( 235, 45, 39, 27 ) : D3DCOLOR_ARGB ( 235, 9, 66, 71 ), //background
				(iSkillInfoOffset == 0 || SkillButtonIndex == 0) ? D3DCOLOR_ARGB( 245, 9, 23, 22 ) : D3DCOLOR_ARGB ( 235, 9, 23, 22 ) ); //border

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