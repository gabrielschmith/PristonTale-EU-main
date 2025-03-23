#include "stdafx.h"
#include "CSettingsWindow.h"

extern void ClearDrops();
extern PacketServerInfo sServerInfo;

//#define ENCHANTWEAPON_SETTING

CSettingsWindow::CSettingsWindow()
{
}


CSettingsWindow::~CSettingsWindow()
{
}

void CSettingsWindow::SetResolutionSettings()
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );

	if ( pGroupRatio->GetIndexObject() == CHECKBOXID_Ratio4_3 )
	{
		int iNum = pGroupResolution43->GetIndexObject();
		if ( iNum == CHECKBOXID_1024x768 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1024;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
		}
		else if ( iNum == CHECKBOXID_1280x960 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1280;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 960;
		}
		else if ( iNum == CHECKBOXID_1400x1050 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1400;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 1050;
		}
		else
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 800;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 600;
		}
	}
	else
	{
		int iNum = pGroupResolution169->GetIndexObject();
		if ( iNum == CHECKBOXID_1280x720 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1280;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 720;
		}
		else if ( iNum == CHECKBOXID_1366x768 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1366;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
		}
		else if ( iNum == CHECKBOXID_1600x900 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1600;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 900;
		}
		else if ( iNum == CHECKBOXID_1920x1080 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1920;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 1080;
		}
		else
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 800;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 600;
		}
	}
}

int CSettingsWindow::GetResolutionCheckBox()
{
	DWORD dwWidth	= SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
	DWORD dwHeight	= SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;

	// 4:3
	if ( dwWidth == 1024 && dwHeight == 768 )
		return CHECKBOXID_1024x768;
	if ( dwWidth == 1280 && dwHeight == 960 )
		return CHECKBOXID_1280x960;
	if ( dwWidth == 1400 && dwHeight == 1050 )
		return CHECKBOXID_1400x1050;

	// 16:9
	if ( dwWidth == 1280 && dwHeight == 720 )
		return CHECKBOXID_1280x720;
	if ( dwWidth == 1366 && dwHeight == 768 )
		return CHECKBOXID_1366x768;
	if ( dwWidth == 1600 && dwHeight == 900 )
		return CHECKBOXID_1600x900;
	if ( dwWidth == 1920 && dwHeight == 1080 )
		return CHECKBOXID_1920x1080;

	return -1;
}

void CSettingsWindow::SetResolutionCheckBox()
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );

	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );


	if ( SETTINGSHANDLER->GetModel()->GetSettings().bWidescreen )
	{
		pGroupResolution169->SetIndexObject( GetResolutionCheckBox() );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio16_9 );
		pGroupResolution43->Hide();
		pGroupResolution169->Show();
	}
	else
	{
		pGroupResolution43->SetIndexObject( GetResolutionCheckBox() );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
		pGroupResolution169->Hide();
		pGroupResolution43->Show();
	}
}

void CSettingsWindow::Load()
{
	SETTINGSHANDLER->GetModel()->ReadSettings();

	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	auto pButtonVideo = pWindow1->GetElement<UI::Button>( BUTTONID_Video );
	auto pButtonAudio = pWindow1->GetElement<UI::Button>( BUTTONID_Audio );
	auto pButtonGeneral = pWindow1->GetElement<UI::Button>( BUTTONID_General );
	auto pButtonInterface = pWindow1->GetElement<UI::Button>( BUTTONID_Interface );
	auto pGroupMenu = pWindow1->GetElement<UI::Group>( GROUPID_Menu );

	if( pWindowAudio->IsOpen() )
	{
		pButtonAudio->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Audio );
	}
	else if( pWindowGeneral->IsOpen() )
	{
		pButtonGeneral->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_General );
	}
	else if( pWindowVideo->IsOpen() )
	{
		pButtonVideo->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Video );
	}
	else if( pWindowInterface->IsOpen() )
	{
		pButtonInterface->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Interface );
	}

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		pCheckBoxAutoAdjust->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust );

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		pCheckBoxStartMaximized->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized );

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed )
			pGroupDisplay->SetIndexObject( CHECKBOXID_Windowed );
		else
			pGroupDisplay->SetIndexObject( CHECKBOXID_Fullscreen );

		SetResolutionCheckBox();
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHighTextureQuality )
			pGroupTexture->SetIndexObject( CHECKBOXID_TextureHigh );
		else
			pGroupTexture->SetIndexObject( CHECKBOXID_TextureLow );


		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().dwBitDepth == 16 )
			pGroupBitDepth->SetIndexObject( CHECKBOXID_16BPP );
		else
			pGroupBitDepth->SetIndexObject( CHECKBOXID_32BPP );

		auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects )
			pGroupEffects->SetIndexObject( CHECKBOXID_EffectsOFF );
		else
			pGroupEffects->SetIndexObject( CHECKBOXID_EffectsON );


		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bVSync == TRUE )
			pGroupVSync->SetIndexObject( CHECKBOXID_VSyncON );
		else
			pGroupVSync->SetIndexObject( CHECKBOXID_VSyncOFF );

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );

		if( SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing == TRUE )
			pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingON );
		else
			pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingOFF );
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 0 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Short );
		else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 1 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Far );
		else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 2 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Full );

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bInvertedCamera )
			pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraON );
		else
			pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraOFF );

		auto pGroupARPGCamera = pWindowVideo->GetElement<UI::Group>( GROUPID_ARPGCamera );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			pGroupARPGCamera->SetIndexObject( CHECKBOXID_ARPGCameraON );
		else
			pGroupARPGCamera->SetIndexObject( CHECKBOXID_ARPGCameraOFF );

		auto pCheckBoxCustomZoom = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoom );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoom )
			pCheckBoxCustomZoom->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoom );

		auto pScrollCustomZoom = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoom );
		pScrollCustomZoom->SetPercent( static_cast<float>( SETTINGSHANDLER->GetModel()->GetSettings().iZoomLevel ) );

		auto pCheckBoxCustomZoomTown = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoomTown );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoomTown )
			pCheckBoxCustomZoomTown->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoomTown );

		auto pScrollCustomZoomTown = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoomTown );
		pScrollCustomZoomTown->SetPercent( static_cast<float>( SETTINGSHANDLER->GetModel()->GetSettings().iZoomTownLevel ) );

		ToggleCustomCamera(SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera != TRUE);

		pWindowVideo->GetElement<UI::CheckBox>(CHECKBOXID_ARPGCameraON)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChangedVideo, this, CHECKBOXID_ARPGCameraON, std::placeholders::_1)));
		pWindowVideo->GetElement<UI::CheckBox>(CHECKBOXID_ARPGCameraOFF)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChangedVideo, this, CHECKBOXID_ARPGCameraOFF, std::placeholders::_1)));

	}



	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic )
		{
			pCheckBoxNoMusic->SetCheck( TRUE );
		}
		else
		{
			pCheckBoxNoMusic->SetCheck( FALSE );
		}
		pScrollBGMusic->SetPercent( static_cast<float>( SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume ) );
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects )
		{
			pCheckBoxNoEffects->SetCheck( TRUE );
		}
		else
		{
			pCheckBoxNoEffects->SetCheck( FALSE );
		}
		pScrollEffectSound->SetPercent( static_cast<float>( SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume ) );
	}

	// Boss Time and Server Time
	{
		char szBuf[64] = { 0 };
		STRINGFORMAT( szBuf, "xx:%02d", sServerInfo.iBossTime );
		auto pTextBossTime = pWindow1->GetElement<UI::Text>( TextID_BossTime );
		pTextBossTime->SetText( szBuf );

		auto pTextServerTime = pWindow1->GetElement<UI::Text>( TextID_ServerTime );
		STRINGFORMAT( szBuf, "%02d:%02d:%02d\n%02d-%02d-%04d", GameCore::GetInstance()->sServerTime.wHour, GameCore::GetInstance()->sServerTime.wMinute, GameCore::GetInstance()->sServerTime.wSecond, GameCore::GetInstance()->sServerTime.wDay, GameCore::GetInstance()->sServerTime.wMonth, GameCore::GetInstance()->sServerTime.wYear );
		pTextServerTime->SetText( szBuf );

		auto pTextServerName = pWindow1->GetElement<UI::Text>( TextID_ServerName );
		STRINGFORMAT( szBuf, "%s", (char *)(0x038FEF40 + ((*( int* )0x4B06960 - 1 ) * 0x68)) );
		pTextServerName->SetText( szBuf );

		auto pTextSiegeWarTime = pWindow1->GetElement<UI::Text>( TEXTID_SiegeWarTime );
		STRINGFORMAT( szBuf, "%02d:00h, %s", sServerInfo.sSiegeWarHour, GetDayOfWeek(sServerInfo.sSiegeWarDay).c_str() );
		pTextSiegeWarTime->SetText( szBuf );

		auto pTextSiegeWarType = pWindow1->GetElement<UI::Text>( TEXTID_SiegeWarType );
		auto strSiegeWarMode = "";
		if( sServerInfo.bSiegeWarType == SIEGEWARMODE_Normal )
			strSiegeWarMode = "Normal";
		else if( sServerInfo.bSiegeWarType == SIEGEWARMODE_TheConquest )
			strSiegeWarMode = "The Conquest";
		else if( sServerInfo.bSiegeWarType == SIEGEWARMODE_TheMassacre )
			strSiegeWarMode = "The Massacre";

		STRINGFORMAT( szBuf, "%s", strSiegeWarMode );
		pTextSiegeWarType->SetText( szBuf );
	}

	// Macro
	{
		UI::InputField_ptr pInputField;

		for ( int i = 0; i < 10; i++ )
		{
			pInputField = pWindowGeneral->GetElement<UI::InputField>( InputFieldID_Macro1 + i );
			pInputField->SetText( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[i] );
		}

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoWhisper )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoWhisper );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoTrade )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableParty )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoPartyRequest );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableTrade )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeRequest );
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDebugDamage )
			pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugON );
		else
			pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugOFF );

		auto pGroupTargetHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_TargetHPBar );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnTarget )
			pGroupTargetHPBar->SetIndexObject( CHECKBOXID_TargetHPBarON );
		else
			pGroupTargetHPBar->SetIndexObject( CHECKBOXID_TargetHPBarOFF );

		auto pGroupSelfHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_SelfHPBar );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnSelf )
			pGroupSelfHPBar->SetIndexObject( CHECKBOXID_SelfHPBarShow );
		else
			pGroupSelfHPBar->SetIndexObject( CHECKBOXID_SelfHPBarHide );

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bFPS )
			pGroupFPS->SetIndexObject( CHECKBOXID_FPSON );
		else
			pGroupFPS->SetIndexObject( CHECKBOXID_FPSOFF );

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bPing )
			pGroupPing->SetIndexObject( CHECKBOXID_PingON );
		else
			pGroupPing->SetIndexObject( CHECKBOXID_PingOFF );

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDebugKill )
			pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoON );
		else
			pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoOFF );

		auto pGroupPetInfoInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_PetMode );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive )
			pGroupPetInfoInfo->SetIndexObject( CHECKBOXID_PetModeAgressive );
		else
			pGroupPetInfoInfo->SetIndexObject( CHECKBOXID_PetModePassive );

		auto pGroupChatBoxMode = pWindowGeneral->GetElement<UI::Group>( GROUPID_ChatBoxMode );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bChatBoxRemoveFocusOnEnter )
			pGroupChatBoxMode->SetIndexObject( CHECKBOXID_ChatBoxRemoveFocusOnEnterON );
		else
			pGroupChatBoxMode->SetIndexObject( CHECKBOXID_ChatBoxRemoveFocusOnEnterOFF );

		auto pGroupAutoSelectTarget = pWindowGeneral->GetElement<UI::Group>( GROUPID_AllowAutoSelectTargetForAoE );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bAutoSelectNearestTargetForAoE )
			pGroupAutoSelectTarget->SetIndexObject( CHECKBOXID_AoEAutoSelectTargetON );
		else
			pGroupAutoSelectTarget->SetIndexObject( CHECKBOXID_AoEAutoSelectTargetOFF );

		auto pGroupViewItem = pWindowGeneral->GetElement<UI::Group>(GROUPID_ItemView);

		if (SETTINGSHANDLER->GetModel()->GetSettings().bItemViewTogglableA)
			pGroupViewItem->SetIndexObject(CHECKBOXID_ItemViewToggle);
		else
			pGroupViewItem->SetIndexObject(CHECKBOXID_ItemViewHold);

		//Loot
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_HPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_MPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_SPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Gold ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootJewellery )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Jewellery ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGear )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Gear ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Sheltoms ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_ForceOrbs ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootCrystals )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Crystals ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecJewellery)->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_NonSpecJewellery ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecGear )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_NonSpecGear ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecSheltoms )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot(LOOTFILTER_NonSpecSheltoms) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootEquipmentMinLevel )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bEnableEquipmentMinLevelFilter );

		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->ClearInput();
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetText( FormatString("%d", SETTINGSHANDLER->GetModel()->GetSettings().iMinEquipmentLevelReq) );

		ToggleLootFilter(SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter);

		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecGear)->SetVisible(SETTINGSHANDLER->GetModel()->CanViewLoot(LOOTFILTER_Gear));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecJewellery)->SetVisible(SETTINGSHANDLER->GetModel()->CanViewLoot(LOOTFILTER_Jewellery));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecSheltoms)->SetVisible(SETTINGSHANDLER->GetModel()->CanViewLoot(LOOTFILTER_Sheltoms));

		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootFilterEnable)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChanged, this, CHECKBOXID_LootFilterEnable, std::placeholders::_1)));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootJewellery)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChanged, this, CHECKBOXID_LootJewellery, std::placeholders::_1)));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootGear)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChanged, this, CHECKBOXID_LootGear, std::placeholders::_1)));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootSheltoms)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChanged, this, CHECKBOXID_LootSheltoms, std::placeholders::_1)));
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootEquipmentMinLevel)->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnCheckBoxChanged, this, CHECKBOXID_LootEquipmentMinLevel, std::placeholders::_1)));

		SetEnchantWeaponPerference( SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference );

		/*pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecNS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_NS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecFS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_FS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecAS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_AS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecATA )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ATA ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecKS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_KS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMGS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MGS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPRS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PRS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecASS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ASS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecSHA )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_SHA ) );*/
	}

	//Skills
	{

#ifdef ENCHANTWEAPON_SETTING
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_EW_Rotate )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference == 0 );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_EW_Fire )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference == EElementID::Fire );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_EW_Ice )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference == EElementID::Ice );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_EW_Lightning )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference == EElementID::Lightning );
#endif
	}

	//User Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bLockUI )
			pCheckBoxLockUI->SetCheck( TRUE );

	//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
	//	if( SETTINGSHANDLER->GetModel()->GetSettings().bSaveUI )
	//		pCheckBoxSaveUI->SetCheck( TRUE );

		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers )
			pCheckBoxShowParty->SetCheck( TRUE );

		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers )
			pCheckBoxShowRaid->SetCheck( TRUE );

		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers )
			pCheckBoxShowClan->SetCheck( TRUE );

		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies )
			pCheckBoxShowEnemies->SetCheck( TRUE );

		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs )
			pCheckBoxShowNPCs->SetCheck( TRUE );

		//auto pCheckBoxShowAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		//if( SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive )
			//pCheckBoxShowAlwaysActive->SetCheck( TRUE );

		auto pCheckBoxShowNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer )
			pCheckBoxShowNextBossTimer->SetCheck( TRUE );

		auto pCheckBoxNameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowNameAboveHead);
		if (SETTINGSHANDLER->GetModel()->GetSettings().bShowNameAboveHead)
			pCheckBoxNameAboveHead->SetCheck(TRUE);

		auto pCheckBoxFrameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowFrameAboveHead);
		if (SETTINGSHANDLER->GetModel()->GetSettings().bShowFrameAboveHead)
			pCheckBoxFrameAboveHead->SetCheck(TRUE);

		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNotice )
			pCheckBoxShowNotice->SetCheck( TRUE );

		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue )
			pCheckBoxTargetDistance->SetCheck( TRUE );

		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue )
			pCheckBoxTargetHPValue->SetCheck( TRUE );

		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPPercent )
			pCheckBoxTargetHPPercent->SetCheck( TRUE );

	}
}

void CSettingsWindow::OnCheckBoxChangedVideo(int iID, UIEventArgs e)
{
	auto pWindowVideo = GetWindow(WINDOWID_Video);
	bool checked = pWindowVideo->GetElement<UI::CheckBox>(iID)->IsChecked();

	if (iID == CHECKBOXID_ARPGCameraON)
	{
		ToggleCustomCamera(FALSE);
	}
	else if (iID == CHECKBOXID_ARPGCameraOFF)
	{
		ToggleCustomCamera(TRUE);
	}
}

void CSettingsWindow::OnCheckBoxChanged(int iID, UIEventArgs e)
{
	auto pWindowGeneral = GetWindow(WINDOWID_General);
	bool checked = pWindowGeneral->GetElement<UI::CheckBox>(iID)->IsChecked();


	if (iID == CHECKBOXID_LootFilterEnable)
	{
		ToggleLootFilter(checked);
	}
	else if (iID == CHECKBOXID_LootGear)
	{
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecGear)->SetVisible(checked);
	}
	else if (iID == CHECKBOXID_LootJewellery)
	{
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecJewellery)->SetVisible(checked);
	}
	else if (iID == CHECKBOXID_LootSheltoms)
	{
		pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecSheltoms)->SetVisible(checked);
	}
	else if (iID == CHECKBOXID_LootEquipmentMinLevel)
	{
		if (checked)
		{
			pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetText(FormatString("%d", SETTINGSHANDLER->GetModel()->GetSettings().iMinEquipmentLevelReq));
			pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelect(TRUE);
		}
		else
		{
			pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelect(FALSE);
			pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelected(FALSE);
			pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->ClearInput();
		}
	}
}

void CSettingsWindow::SetEnchantWeaponPerference ( int value )
{
#ifdef ENCHANTWEAPON_SETTING
	auto pWindowGeneral = GetWindow(WINDOWID_General);
	auto pGroup = pWindowGeneral->GetElement<UI::Group>(GROUPID_EnchantWeapon);

	switch ( (EElementID)value )
	{
		case EElementID::Fire:		pGroup->SetIndexObject( CHECKBOXID_EW_Fire );		break;
		case EElementID::Ice:		pGroup->SetIndexObject( CHECKBOXID_EW_Ice );		break;
		case EElementID::Lightning:	pGroup->SetIndexObject( CHECKBOXID_EW_Lightning );	break;
		default:					pGroup->SetIndexObject( CHECKBOXID_EW_Rotate );		break;
	}
#endif
}

void CSettingsWindow::ToggleCustomCamera ( BOOL bEnable )
{
	auto pWindowVideo = GetWindow(WINDOWID_Video);

	pWindowVideo->GetElement<UI::CheckBox>(CHECKBOXID_CustomZoom)->SetEnabled(bEnable);
	pWindowVideo->GetElement<UI::CheckBox>(CHECKBOXID_CustomZoomTown)->SetEnabled(bEnable);

}


void CSettingsWindow::ToggleLootFilter(BOOL bChecked)
{
	auto pWindowGeneral = GetWindow(WINDOWID_General);

	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootHPPotion)->SetEnabled(bChecked);

	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootMPPotion)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootSPPotion)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootGold)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootJewellery)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootGear)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootSheltoms)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootForceOrbs)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootCrystals)->SetEnabled(bChecked);

	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecJewellery)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecGear)->SetEnabled(bChecked);
	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootNonSpecSheltoms)->SetEnabled(bChecked);

	pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootEquipmentMinLevel)->SetEnabled(bChecked);
	//pWindowGeneral->GetElement<UI::Text>(TextID_LootEquipmentMinLevel)->SetDisabled(!bChecked || !pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootEquipmentMinLevel)->IsChecked());

	if (bChecked == FALSE || pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootEquipmentMinLevel)->IsChecked() == FALSE)
	{
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelect(FALSE);
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelected(FALSE);
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->ClearInput();
	}
	else
	{
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelect(TRUE);
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->ClearInput();
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetText(FormatString("%d", SETTINGSHANDLER->GetModel()->GetSettings().iMinEquipmentLevelReq));
	}
}

void CSettingsWindow::Save()
{
	if( MESSAGEBOX->GetType() == FALSE )
	{
		pWindow1->Show();
		return;
	}

	SetResolutionSettings();

	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust = pCheckBoxAutoAdjust->IsChecked();

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized = pCheckBoxStartMaximized->IsChecked();

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		SETTINGSHANDLER->GetModel()->GetSettings().bWindowed = pGroupDisplay->GetIndexObject() == CHECKBOXID_Windowed ? TRUE : FALSE;

		auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
		SETTINGSHANDLER->GetModel()->GetSettings().bWidescreen = pGroupRatio->GetIndexObject() == CHECKBOXID_Ratio4_3 ? FALSE : TRUE;

		SetResolutionCheckBox();
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		SETTINGSHANDLER->GetModel()->GetSettings().bHighTextureQuality = pGroupTexture->GetIndexObject() == CHECKBOXID_TextureHigh ? TRUE : FALSE;

		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		SETTINGSHANDLER->GetModel()->GetSettings().dwBitDepth = pGroupBitDepth->GetIndexObject() == CHECKBOXID_16BPP ? 16 : 32;

		auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
		SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects = pGroupEffects->GetIndexObject() == CHECKBOXID_EffectsON ? FALSE : TRUE;

		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );
		SETTINGSHANDLER->GetModel()->GetSettings().bVSync = pGroupVSync->GetIndexObject() == CHECKBOXID_VSyncON ? TRUE : FALSE;

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );
		SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing = pGroupAntiAliasing->GetIndexObject() == CHECKBOXID_AntiAliasingON ? TRUE : FALSE;
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Short )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 0;
		else if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Far )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 1;
		else if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Full )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 2;

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		SETTINGSHANDLER->GetModel()->GetSettings().bInvertedCamera = pGroupCameraInvert->GetIndexObject() == CHECKBOXID_CameraON ? TRUE : FALSE;

		auto pGroupARPGCamera = pWindowVideo->GetElement<UI::Group>( GROUPID_ARPGCamera );
		SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera = pGroupARPGCamera->GetIndexObject() == CHECKBOXID_ARPGCameraON ? TRUE : FALSE;

		auto pCheckBoxCustomZoom = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoom );
		SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoom = pCheckBoxCustomZoom->IsChecked();

		auto pScrollCustomZoom = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoom );
		SETTINGSHANDLER->GetModel()->GetSettings().iZoomLevel = static_cast<int>(pScrollCustomZoom->GetPercent());

		auto pCheckBoxCustomZoomTown = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoomTown );
		SETTINGSHANDLER->GetModel()->GetSettings().bCustomZoomTown = pCheckBoxCustomZoomTown->IsChecked();

		auto pScrollCustomZoomTown = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoomTown );
		SETTINGSHANDLER->GetModel()->GetSettings().iZoomTownLevel = static_cast<int>(pScrollCustomZoomTown->GetPercent());
	}

	//Skills
	{
#ifdef ENCHANTWEAPON_SETTING
		auto group = pWindowGeneral->GetElement<UI::Group>( GROUPID_EnchantWeapon );
		switch ( group->GetIndexObject() )
		{
			case CHECKBOXID_EW_Fire:		SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference = EElementID::Fire;		break;
			case CHECKBOXID_EW_Ice:			SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference = EElementID::Ice;		break;
			case CHECKBOXID_EW_Lightning:	SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference = EElementID::Lightning;	break;
			default:						SETTINGSHANDLER->GetModel()->GetSettings().bWeaponEnchantPerference = 0;					break;
		}
#endif
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );
		SETTINGSHANDLER->GetModel()->GetSettings().bDebugDamage = pGroupDebugDamage->GetIndexObject() == CHECKBOXID_DamageDebugON ? TRUE : FALSE;

		auto pGroupTargetHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_TargetHPBar );
		SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnTarget = pGroupTargetHPBar->GetIndexObject() == CHECKBOXID_TargetHPBarON ? TRUE : FALSE;

		auto pGroupSelfHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_SelfHPBar );
		SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnSelf = pGroupSelfHPBar->GetIndexObject() == CHECKBOXID_SelfHPBarShow ? TRUE : FALSE;

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );
		SETTINGSHANDLER->GetModel()->GetSettings().bFPS = pGroupFPS->GetIndexObject() == CHECKBOXID_FPSON ? TRUE : FALSE;

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );
		SETTINGSHANDLER->GetModel()->GetSettings().bPing = pGroupPing->GetIndexObject() == CHECKBOXID_PingON ? TRUE : FALSE;

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );
		SETTINGSHANDLER->GetModel()->GetSettings().bDebugKill = pGroupKillInfo->GetIndexObject() == CHECKBOXID_KillInfoON ? TRUE : FALSE;

		auto pGroupItemView = pWindowGeneral->GetElement<UI::Group>(GROUPID_ItemView);
		SETTINGSHANDLER->GetModel()->GetSettings().bItemViewTogglableA = pGroupItemView->GetIndexObject() == CHECKBOXID_ItemViewToggle ? TRUE : FALSE;

		auto pGroupPetMode = pWindowGeneral->GetElement<UI::Group>(GROUPID_PetMode);
		SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive = pGroupPetMode->GetIndexObject() == CHECKBOXID_PetModeAgressive ? TRUE : FALSE;

		auto pGroupChatBoxMode = pWindowGeneral->GetElement<UI::Group>(GROUPID_ChatBoxMode);
		SETTINGSHANDLER->GetModel()->GetSettings().bChatBoxRemoveFocusOnEnter = pGroupChatBoxMode->GetIndexObject() == CHECKBOXID_ChatBoxRemoveFocusOnEnterON ? TRUE : FALSE;

		auto pGroupAutoSelectTarget = pWindowGeneral->GetElement<UI::Group>(GROUPID_AllowAutoSelectTargetForAoE);
		SETTINGSHANDLER->GetModel()->GetSettings().bAutoSelectNearestTargetForAoE = pGroupAutoSelectTarget->GetIndexObject() == CHECKBOXID_AoEAutoSelectTargetON ? TRUE : FALSE;



		//Loot
		SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter = pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->IsChecked();
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_HPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_MPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_SPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Gold, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Jewellery, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootJewellery )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Gear, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGear )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Sheltoms, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_ForceOrbs, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Crystals, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootCrystals )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_NonSpecJewellery, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecJewellery )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_NonSpecGear, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecGear )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_NonSpecSheltoms, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecSheltoms )->IsChecked() );
		SETTINGSHANDLER->GetModel()->GetSettings().bEnableEquipmentMinLevelFilter = pWindowGeneral->GetElement<UI::CheckBox>(CHECKBOXID_LootEquipmentMinLevel)->IsChecked();

		if (SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter && SETTINGSHANDLER->GetModel()->GetSettings().bEnableEquipmentMinLevelFilter)
		{
			int iMinEquipmentLevel = atoi(pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->GetText().c_str());

			if (iMinEquipmentLevel < 0)
				iMinEquipmentLevel = 0;

			if (iMinEquipmentLevel > 150)
				iMinEquipmentLevel = 150;

			SETTINGSHANDLER->GetModel()->GetSettings().iMinEquipmentLevelReq = iMinEquipmentLevel;
		}
	}

	//Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
	//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		//auto pCheckBoxAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		auto pCheckBoxNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		auto pCheckBoxNameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowNameAboveHead);
		auto pCheckBoxFrameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowFrameAboveHead);
		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );


		SETTINGSHANDLER->GetModel()->GetSettings().bLockUI = pCheckBoxLockUI->IsChecked() ? TRUE : FALSE;
	//	SETTINGSHANDLER->GetModel()->GetSettings().bSaveUI = pCheckBoxSaveUI->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers = pCheckBoxShowParty->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers = pCheckBoxShowRaid->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers = pCheckBoxShowClan->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs = pCheckBoxShowNPCs->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies = pCheckBoxShowEnemies->IsChecked() ? TRUE : FALSE;
		//SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive = pCheckBoxAlwaysActive->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer = pCheckBoxNextBossTimer->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNameAboveHead = pCheckBoxNameAboveHead->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowFrameAboveHead = pCheckBoxFrameAboveHead->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNotice = pCheckBoxShowNotice->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetDistance = pCheckBoxTargetDistance->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue = pCheckBoxTargetHPValue->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPPercent = pCheckBoxTargetHPPercent->IsChecked() ? TRUE : FALSE;
	}

	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );
		SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic = pCheckBoxNoMusic->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume = static_cast<int>(pScrollBGMusic->GetPercent());
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );
		SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects = pCheckBoxNoEffects->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume = static_cast<int>(pScrollEffectSound->GetPercent());
	}

	// General Settings
	{
		UI::InputField_ptr pInputField;

		for ( int i = 0; i < 10; i++ )
		{
			pInputField = pWindowGeneral->GetElement<UI::InputField>( InputFieldID_Macro1 + i );
			STRINGCOPY( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[i], pInputField->GetText().c_str() );
		}

		SETTINGSHANDLER->GetModel()->GetSettings().bNoWhisper		= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoWhisper )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat		= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoTrade )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeRequest	= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableTrade )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoPartyRequest	= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableParty )->IsChecked();
	}

	if (Game::GetGameMode() == GAMEMODE_InGame)
	{
		if (SETTINGSHANDLER->GetModel()->IsLootFiltersChanged())
		{
			ClearDrops();
			UNITGAME->SendUnitDataEx( TRUE ); //force update loot filter
		}

	}

	SETTINGSHANDLER->GetModel()->WriteSettings();
	SETTINGSHANDLER->GetModel()->ProcessGeneralSettings();

	DXGraphicEngine::ResolutionChange( FALSE, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight );
	GRAPHICENGINE->Reset( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight, TRUE );

	HUDHANDLER->UpdateObjectsSettings();
	BLESSCASTLEHANDLER->UpdateObjectSettings();

	DXGraphicEngine::ResizeWindow();
	SETTINGSHANDLER->GetModel()->ReadSettings();
	pWindow1->Hide();

	MOUSEHANDLER->UnblockMouse();
}

void CSettingsWindow::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	if ( (*(BOOL*)(0x035E10D0)) )
		pWindowMenu->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( Game::GetGameMode() == GAMEMODE_Login )
	{
		pWindowLogin->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	}
}

BOOL CSettingsWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowMenu->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( Game::GetGameMode() == GAMEMODE_Login )
	{
		if ( pWindowLogin->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CSettingsWindow::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow1->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CSettingsWindow::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pcKeyboard->GetKey() == VK_F10 )
	{
		if ( !pWindow1->IsOpen() && GameCore::GetInstance()->sServerTime.wDay && pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown )
		{
			OnMenuButtonClick( UIEventArgs{} );
			return TRUE;
		}
		else if ( pWindow1->IsOpen() && pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown )
		{
			pWindow1->Hide();
			MOUSEHANDLER->UnblockMouse();
			*(BOOL*)(0x035E10D0) = FALSE;
		}
	}
	else if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL CSettingsWindow::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

void CSettingsWindow::Update( float fTime )
{
	pWindowLogin->Update( fTime );
	pWindow1->Update( fTime );
}

void CSettingsWindow::Render()
{
	pWindowMenu->SetPosition( 698 + 15, GRAPHICENGINE->GetHeightBackBuffer() - 96 - 30 - 30 );
	if ( (*(BOOL*)(0x035E10D0)) )
		pWindowMenu->Show();
	else
		pWindowMenu->Hide();

	pWindowMenu->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
	{
		pWindowLogin->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	}
}

void CSettingsWindow::OnResolutionChanged()
{
	pWindow1->SetPosition( ( RESOLUTION_WIDTH >> 1 ) - ( pWindow1->GetWidth() >> 1 ), ( RESOLUTION_HEIGHT >> 1 ) - ( pWindow1->GetHeight() >> 1 ) );
	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
	{
		pWindowLogin->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowLogin->GetWidth() >> 1), 30 );
	}
}

void CSettingsWindow::OnVideoButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Show();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnCheckBoxRatio43Click( UIEventArgs eArgs )
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
	pGroupResolution43->SetIndexObject( GetResolutionCheckBox() );
	pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
	pGroupResolution169->Hide();
	pGroupResolution43->Show();
}

void CSettingsWindow::OnCheckBoxRatio169Click( UIEventArgs eArgs )
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
	pGroupResolution169->SetIndexObject( GetResolutionCheckBox() );
	pGroupRatio->SetIndexObject( CHECKBOXID_Ratio16_9 );
	pGroupResolution43->Hide();
	pGroupResolution169->Show();
}

void CSettingsWindow::OnAudioButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Show();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnGeneralButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Show();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnInterfaceButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Show();
}

void CSettingsWindow::OnCloseButtonClick( UIEventArgs eArgs )
{
	pWindow1->Hide();
	MOUSEHANDLER->UnblockMouse();
}

void CSettingsWindow::OnSaveButtonClick( UIEventArgs eArgs )
{
	// Show Box to Save
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Save Settings" );
	MESSAGEBOX->SetDescription( "Do you want to save the changes?" );
	MESSAGEBOX->SetEvent( std::bind( &CSettingsWindow::Save, this ) );
	MESSAGEBOX->SetAutoClose( 5 );
	MESSAGEBOX->Show();

	pWindow1->Hide();
}

void CSettingsWindow::OnResetButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		pCheckBoxAutoAdjust->SetCheck( TRUE );

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		pCheckBoxStartMaximized->SetCheck( TRUE );

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		pGroupDisplay->SetIndexObject( CHECKBOXID_Windowed );

		auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
		auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
		auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
		pGroupResolution43->SetIndexObject( CHECKBOXID_1024x768 );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
		pGroupResolution169->Hide();
		pGroupResolution43->Show();
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		pGroupTexture->SetIndexObject( CHECKBOXID_TextureHigh );

		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		pGroupBitDepth->SetIndexObject( CHECKBOXID_32BPP );

		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );
		pGroupVSync->SetIndexObject( CHECKBOXID_VSyncON );

		auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
		pGroupEffects->SetIndexObject( CHECKBOXID_EffectsON );

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );
		pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingON );
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		pGroupCameraSight->SetIndexObject( CHECKBOXID_Far );

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraOFF );

		auto pGroupARPGCamera = pWindowVideo->GetElement<UI::Group>( GROUPID_ARPGCamera );
		pGroupARPGCamera->SetIndexObject( CHECKBOXID_ARPGCameraOFF );

		auto pCheckBoxCustomZoom = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoom );
		pCheckBoxCustomZoom->SetCheck( CHECKBOXID_CustomZoom );

		auto pScrollCustomZoom = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoom );
		pScrollCustomZoom->SetPercent( 50 );

		auto pCheckBoxCustomZoomTown = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_CustomZoomTown );
		pCheckBoxCustomZoomTown->SetCheck( CHECKBOXID_CustomZoomTown );

		auto pScrollCustomZoomTown = pWindowVideo->GetElement<UI::Scroll>( SCROLLID_CustomZoomTown );
		pScrollCustomZoomTown->SetPercent( 50 );

		ToggleCustomCamera ( TRUE );
	}

	//Skills
	{
		SetEnchantWeaponPerference( 0 );
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );
		pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugON );

		auto pGroupHPTargetBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_TargetHPBar );
		pGroupHPTargetBar->SetIndexObject( CHECKBOXID_TargetHPBarON );

		auto pGroupHPSelfBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_SelfHPBar );
		pGroupHPSelfBar->SetIndexObject( CHECKBOXID_SelfHPBarHide );

		auto pGrouChatMode = pWindowGeneral->GetElement<UI::Group>( GROUPID_ChatBoxMode );
		pGrouChatMode->SetIndexObject( CHECKBOXID_ChatBoxRemoveFocusOnEnterON );

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );
		pGroupFPS->SetIndexObject( CHECKBOXID_FPSOFF );

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );
		pGroupPing->SetIndexObject( CHECKBOXID_PingOFF );

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );
		pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoON );

		auto pGroupItemView = pWindowGeneral->GetElement<UI::Group>(GROUPID_ItemView);
		pGroupItemView->SetIndexObject(CHECKBOXID_ItemViewHold);

		auto pGroupLootFilterBehaviour = pWindowGeneral->GetElement<UI::Group>(GROUPID_LootFilterBehaviour);
		pGroupLootFilterBehaviour->SetIndexObject(CHECKBOXID_LootFilterHide);

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootJewellery )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGear )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootCrystals )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecJewellery )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecGear )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootNonSpecSheltoms )->SetCheck( TRUE );

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootEquipmentMinLevel )->SetCheck( FALSE );
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetSelected(FALSE);
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->ClearInput();
		pWindowGeneral->GetElement<UI::InputField>(InputFieldID_LootEquipmentMinLevel)->SetText("0");


		ToggleLootFilter(TRUE);

		/*pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecNS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecFS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecAS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecATA )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecKS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMGS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPRS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecASS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecSHA )->SetCheck( TRUE );*/



	}

	//Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
	//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		//auto pCheckBoxAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		auto pCheckBoxNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		auto pCheckBoxNameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowNameAboveHead);
		auto pCheckBoxFrameAboveHead = pWindowInterface->GetElement<UI::CheckBox>(CHECKBOXID_ShowFrameAboveHead);
		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );


		pCheckBoxLockUI->SetCheck( FALSE );
	//	pCheckBoxSaveUI->SetCheck( TRUE );

		pCheckBoxShowParty->SetCheck( TRUE );
		pCheckBoxShowClan->SetCheck( TRUE );
		pCheckBoxShowEnemies->SetCheck( TRUE );
		pCheckBoxShowNPCs->SetCheck( TRUE );
		//pCheckBoxAlwaysActive->SetCheck( FALSE );
		pCheckBoxShowRaid->SetCheck( TRUE );
		pCheckBoxNextBossTimer->SetCheck( TRUE );
		pCheckBoxNameAboveHead->SetCheck(TRUE);
		pCheckBoxShowNotice->SetCheck( FALSE );
		pCheckBoxTargetDistance->SetCheck( TRUE );
		pCheckBoxTargetHPValue->SetCheck( TRUE );
		pCheckBoxTargetHPPercent->SetCheck( TRUE );
	}

	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );

		pCheckBoxNoMusic->SetCheck( FALSE );
		pScrollBGMusic->SetPercent( 100 );
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );

		pCheckBoxNoEffects->SetCheck( FALSE );
		pScrollEffectSound->SetPercent(100 );
	}
}

void CSettingsWindow::OnWorldMapClick(UIEventArgs eArgs)
{
	//TODO

	*(BOOL*)( 0x035E10D0 ) = FALSE;
}

void CSettingsWindow::OnMenuButtonClick( UIEventArgs eArgs )
{
	pWindow1->Show();
	MOUSEHANDLER->BlockMouse();
	Load();
	*(BOOL*)(0x035E10D0) = FALSE;
}

void CSettingsWindow::OnLoginButtonClick( UIEventArgs e )
{
	pWindow1->Show();
	MOUSEHANDLER->BlockMouse();
	Load();
}

void CSettingsWindow::Init()
{
	BuildWindow1();
	{
		BuildWindow1Video();
		BuildWindow1Audio();
		BuildWindow1General();
		BuildWindow1Interface();
	}

	pWindow1->Hide();

	// Menu
	{
		pWindowMenu = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 82, 30 + 30) );

		UI::Button_ptr pButtonWorldMap = std::make_shared<UI::Button>(Rectangle2D(0, 0, 82, 27));
		pButtonWorldMap->SetImage(UI::ImageLoader::LoadImage("game\\images\\misc\\world_map.png"));
		pButtonWorldMap->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\misc\\world_map.png", "game\\images\\misc\\world_map_.png"));
		pButtonWorldMap->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnWorldMapClick, this, std::placeholders::_1)));
		pWindowMenu->AddElement(pButtonWorldMap);
		pWindowMenu->Hide();

		UI::Button_ptr pButtonSettings = std::make_shared<UI::Button>( Rectangle2D( 0, 29, 82, 27) );
		pButtonSettings->SetImage(UI::ImageLoader::LoadImage("game\\images\\misc\\settings.png"));
		pButtonSettings->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\misc\\settings.png", "game\\images\\misc\\settings_.png"));
		pButtonSettings->SetEvent(UI::Event::Build(std::bind(&CSettingsWindow::OnMenuButtonClick, this, std::placeholders::_1)));
		pWindowMenu->AddElement( pButtonSettings );
		pWindowMenu->Hide();
	}
}

void CSettingsWindow::BuildWindow1()
{
	pWindowLogin = std::make_shared<UI::Window>( Rectangle2D( (GRAPHICENGINE->GetWidthBackBuffer() >> 1) - (220 >> 1), 30, 220, 23 ) );

	UI::Button_ptr pButtonSettingsLogin = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 220, 23 ) );
	pButtonSettingsLogin->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\settings.png", "game\\images\\settings\\settings_.png" ) );
	pButtonSettingsLogin->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnLoginButtonClick, this, std::placeholders::_1 ) ) );
	pWindowLogin->AddElement( pButtonSettingsLogin );

	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( ( GRAPHICENGINE->GetWidthBackBuffer() >> 1 ) - ( 560 >> 1 ), ( GRAPHICENGINE->GetHeightBackBuffer() >> 1 ) - ( 450 >> 1 ), 572, 450 ) );
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\window.png" ) );
	pWindow1->SetID( WINDOWID_Main );

	UI::List_ptr pListMain = std::make_shared<UI::List>( Rectangle2D( 135, 40, 410, 395 ) );
	pListMain->SetID( LISTID_Main );
	pListMain->SetNoUpdatePosition( TRUE );
	pListMain->MakeScrollBar( Rectangle2D( 401, (395 >> 1) - (385 >> 1), 6, 385 ), "game\\images\\settings\\scrollbar.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pListMain->SetScrollFirst( TRUE );
	pWindow1->AddElement( pListMain );

	// Menu Settings
	{
		// Group Menu
		UI::Group_ptr pGroupMenu = std::make_shared<UI::Group>();
		pGroupMenu->SetID( GROUPID_Menu );
		pWindow1->AddElement( pGroupMenu );

		UI::Button_ptr pButtonVideo = std::make_shared<UI::Button>( Rectangle2D( 14, 50, 121, 29 ) );
		pButtonVideo->SetID( BUTTONID_Video );
		pButtonVideo->SetGroup( pGroupMenu );
		pButtonVideo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\video.png", "game\\images\\settings\\video_.png" ) );
		pButtonVideo->SetSelect( TRUE );
		pButtonVideo->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnVideoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonVideo );

		UI::Button_ptr pButtonAudio = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + 29, 121, 29 ) );
		pButtonAudio->SetID( BUTTONID_Audio );
		pButtonAudio->SetGroup( pGroupMenu );
		pButtonAudio->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\audio.png", "game\\images\\settings\\audio_.png" ) );
		pButtonAudio->SetSelect( TRUE );
		pButtonAudio->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnAudioButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonAudio );

		UI::Button_ptr pButtonGeneral = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + ( 29 * 2 ), 121, 29 ) );
		pButtonGeneral->SetID( BUTTONID_General );
		pButtonGeneral->SetGroup( pGroupMenu );
		pButtonGeneral->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\general.png", "game\\images\\settings\\general_.png" ) );
		pButtonGeneral->SetSelect( TRUE );
		pButtonGeneral->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnGeneralButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonGeneral );

		UI::Button_ptr pButtonInterface = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + ( 29 * 3 ), 121, 29 ) );
		pButtonInterface->SetID( BUTTONID_Interface );
		pButtonInterface->SetGroup( pGroupMenu );
		pButtonInterface->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\ui.png", "game\\images\\settings\\ui_.png" ) );
		pButtonInterface->SetSelect( TRUE );
		pButtonInterface->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnInterfaceButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonInterface );

		UI::Button_ptr pButtonReset = std::make_shared<UI::Button>( Rectangle2D( 28, 374, 93, 23 ) );
		pButtonReset->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnreset.png", "game\\images\\settings\\btnreset_.png" ) );
		pButtonReset->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnResetButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonReset );

		UI::Button_ptr pButtonSave = std::make_shared<UI::Button>( Rectangle2D( 28, 374 + 26, 93, 23 ) );
		pButtonSave->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnsave.png", "game\\images\\settings\\btnsave_.png" ) );
		pButtonSave->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnSaveButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonSave );

		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 536, 4, 32, 47 ) );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnclose.png", "game\\images\\settings\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonClose );
	}

	// Boss Time and Server time
	{
		UI::Text_ptr pTextBossTimeHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 20, 100, 20 ) );
		pTextBossTimeHeader->SetText( "Boss Time:" );
		pTextBossTimeHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextBossTimeHeader );

		UI::Text_ptr pTextBossTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 36, 100, 20 ) );
		pTextBossTime->SetText( "" );
		pTextBossTime->SetID( TextID_BossTime );
		pTextBossTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextBossTime );

		UI::Text_ptr pTextServerTimeHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 56, 100, 20 ) );
		pTextServerTimeHeader->SetText( "Server Time:" );
		pTextServerTimeHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextServerTimeHeader );

		UI::Text_ptr pTextServerTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 72, 100, 20 ) );
		pTextServerTime->SetText( "" );
		pTextServerTime->SetMultiLine( TRUE );
		pTextServerTime->SetNoClip( TRUE );
		pTextServerTime->SetID( TextID_ServerTime );
		pTextServerTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextServerTime );

		UI::Text_ptr pTextServerNameHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 72 + 36, 100, 20 ) );
		pTextServerNameHeader->SetText( "Server:" );
		pTextServerNameHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextServerNameHeader );

		UI::Text_ptr pTextServerName = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 88 + 36, 100, 20 ) );
		pTextServerName->SetText( "" );
		pTextServerName->SetID( TextID_ServerName );
		pTextServerName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextServerName );

		UI::Text_ptr pTextSiegeWar = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + ( 26 * 2 ) + 108 + 36, 100, 20 ) );
		pTextSiegeWar->SetText( "Siege War:" );
		pTextSiegeWar->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextSiegeWar );

		UI::Text_ptr pTextSiegeWarTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + ( 26 * 2 ) + 124 + 36, 100, 20 ) );
		pTextSiegeWarTime->SetText( "" );
		pTextSiegeWarTime->SetID( TEXTID_SiegeWarTime );
		pTextSiegeWarTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextSiegeWarTime );

		UI::Text_ptr pTextSiegeWarType = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + ( 26 * 2 ) + 140 + 36, 100, 20 ) );
		pTextSiegeWarType->SetText( "" );
		pTextSiegeWarType->SetID( TEXTID_SiegeWarType );
		pTextSiegeWarType->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextSiegeWarType );
	}

	pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth() - 40, 40 ) );
	pWindow1->SetCanMove( TRUE );
}

void CSettingsWindow::BuildWindow1Video()
{
	UI::Window_ptr pWindowVideo = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 700 ) );
	pWindowVideo->SetID( WINDOWID_Video );

	// Title Screen
	UI::ImageBox_ptr pScreenBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pScreenBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pScreenBar );

	UI::ImageBox_ptr pScreenTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pScreenTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\screen.png" ) );
	pWindowVideo->AddElement( pScreenTitle );

	// Text Display
	UI::Text_ptr pTextDisplay = std::make_shared<UI::Text>( Rectangle2D( 24, 47, 0, 0 ) );
	pTextDisplay->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextDisplay->SetText( "Display:" );
	pWindowVideo->AddElement( pTextDisplay );

	// Group Display
	UI::Group_ptr pGroupDisplay = std::make_shared<UI::Group>();
	pGroupDisplay->SetID( GROUPID_Display );
	pWindowVideo->AddElement( pGroupDisplay );

	UI::CheckBox_ptr pCheckBoxFullscreen = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 67, 13, 13 ) );
	pCheckBoxFullscreen->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFullscreen->SetText( "Fullscreen" );
	pCheckBoxFullscreen->SetGroup( pGroupDisplay );
	pCheckBoxFullscreen->SetID( CHECKBOXID_Fullscreen );
	pCheckBoxFullscreen->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxFullscreen );

	UI::CheckBox_ptr pCheckBoxWindowed = std::make_shared<UI::CheckBox>( Rectangle2D( 150, 67, 13, 13 ) );
	pCheckBoxWindowed->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxWindowed->SetText( "Windowed" );
	pCheckBoxWindowed->SetGroup( pGroupDisplay );
	pCheckBoxWindowed->SetID( CHECKBOXID_Windowed );
	pCheckBoxWindowed->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxWindowed );

	UI::CheckBox_ptr pCheckBoxStartMaximized = std::make_shared<UI::CheckBox>( Rectangle2D( 270, 85, 13, 13 ) );
	pCheckBoxStartMaximized->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxStartMaximized->SetText( "Start Maximized" );
	pCheckBoxStartMaximized->SetID( CHECKBOXID_StartMaximized );
	pCheckBoxStartMaximized->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxStartMaximized );

	UI::CheckBox_ptr pCheckBoxAutoAdjust = std::make_shared<UI::CheckBox>( Rectangle2D( 270, 67, 13, 13 ) );
	pCheckBoxAutoAdjust->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAutoAdjust->SetText( "Auto Adjust" );
	pCheckBoxAutoAdjust->SetID( CHECKBOXID_AutoAdjust );
	pCheckBoxAutoAdjust->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxAutoAdjust );

	// Group Ratio
	UI::Group_ptr pGroupRatio = std::make_shared<UI::Group>();
	pGroupRatio->SetID( GROUPID_Ratio );
	pWindowVideo->AddElement( pGroupRatio );

	// Text Ratio
	UI::Text_ptr pTextRatio = std::make_shared<UI::Text>( Rectangle2D( 24, 90, 0, 0 ) );
	pTextRatio->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextRatio->SetText( "Ratio:" );
	pWindowVideo->AddElement( pTextRatio );

	UI::CheckBox_ptr pCheckBox4_3 = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 110, 13, 13 ) );
	pCheckBox4_3->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox4_3->SetText( "4:3" );
	pCheckBox4_3->SetGroup( pGroupRatio );
	pCheckBox4_3->SetID( CHECKBOXID_Ratio4_3 );
	pCheckBox4_3->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCheckBoxRatio43Click, this, std::placeholders::_1 ) ) );
	pCheckBox4_3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox4_3 );

	UI::CheckBox_ptr pCheckBox16_9 = std::make_shared<UI::CheckBox>( Rectangle2D( 150, 110, 13, 13 ) );
	pCheckBox16_9->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox16_9->SetText( "16:9" );
	pCheckBox16_9->SetGroup( pGroupRatio );
	pCheckBox16_9->SetID( CHECKBOXID_Ratio16_9 );
	pCheckBox16_9->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCheckBoxRatio169Click, this, std::placeholders::_1 ) ) );
	pCheckBox16_9->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox16_9 );

	// Text Resolutions
	UI::Text_ptr pTextResolutions = std::make_shared<UI::Text>( Rectangle2D( 24, 134, 0, 0 ) );
	pTextResolutions->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextResolutions->SetText( "Resolutions:" );
	pWindowVideo->AddElement( pTextResolutions );

	// Group Resolutions 4:3
	UI::Group_ptr pGroupResolutions4_3 = std::make_shared<UI::Group>();
	pGroupResolutions4_3->SetID( GROUPID_Resolutions4_3 );
	pWindowVideo->AddElement( pGroupResolutions4_3 );

	UI::CheckBox_ptr pCheckBox1024x768 = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 154, 13, 13 ) );
	pCheckBox1024x768->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1024x768->SetText( "1024x768" );
	pCheckBox1024x768->SetGroup( pGroupResolutions4_3 );
	pCheckBox1024x768->SetID( CHECKBOXID_1024x768 );
	pCheckBox1024x768->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1024x768 );

	UI::CheckBox_ptr pCheckBox1280x960 = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 174, 13, 13 ) );
	pCheckBox1280x960->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1280x960->SetText( "1280x960" );
	pCheckBox1280x960->SetGroup( pGroupResolutions4_3 );
	pCheckBox1280x960->SetID( CHECKBOXID_1280x960 );
	pCheckBox1280x960->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1280x960 );

	UI::CheckBox_ptr pCheckBox1400x1050 = std::make_shared<UI::CheckBox>( Rectangle2D( 150, 154, 13, 13 ) );
	pCheckBox1400x1050->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1400x1050->SetText( "1400x1050" );
	pCheckBox1400x1050->SetGroup( pGroupResolutions4_3 );
	pCheckBox1400x1050->SetID( CHECKBOXID_1400x1050 );
	pCheckBox1400x1050->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1400x1050 );

	// Group Resolutions 16:9
	UI::Group_ptr pGroupResolutions16_9 = std::make_shared<UI::Group>();
	pGroupResolutions16_9->SetID( GROUPID_Resolutions16_9 );
	pWindowVideo->AddElement( pGroupResolutions16_9 );

	UI::CheckBox_ptr pCheckBox1280x720 = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 154, 13, 13 ) );
	pCheckBox1280x720->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1280x720->SetText( "1280x720" );
	pCheckBox1280x720->SetGroup( pGroupResolutions16_9 );
	pCheckBox1280x720->SetID( CHECKBOXID_1280x720 );
	pCheckBox1280x720->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1280x720 );

	UI::CheckBox_ptr pCheckBox1366x768 = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 174, 13, 13 ) );
	pCheckBox1366x768->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1366x768->SetText( "1366x768" );
	pCheckBox1366x768->SetGroup( pGroupResolutions16_9 );
	pCheckBox1366x768->SetID( CHECKBOXID_1366x768 );
	pCheckBox1366x768->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1366x768 );

	UI::CheckBox_ptr pCheckBox1600x900 = std::make_shared<UI::CheckBox>( Rectangle2D( 150, 154, 13, 13 ) );
	pCheckBox1600x900->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1600x900->SetText( "1600x900" );
	pCheckBox1600x900->SetGroup( pGroupResolutions16_9 );
	pCheckBox1600x900->SetID( CHECKBOXID_1600x900 );
	pCheckBox1600x900->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1600x900 );

	UI::CheckBox_ptr pCheckBox1920x1080 = std::make_shared<UI::CheckBox>( Rectangle2D( 150, 174, 13, 13 ) );
	pCheckBox1920x1080->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox1920x1080->SetText( "1920x1080" );
	pCheckBox1920x1080->SetGroup( pGroupResolutions16_9 );
	pCheckBox1920x1080->SetID( CHECKBOXID_1920x1080 );
	pCheckBox1920x1080->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox1920x1080 );

	// Title Graphics
	UI::ImageBox_ptr pGraphicsBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 202, 339, 23 ) );
	pGraphicsBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pGraphicsBar );

	UI::ImageBox_ptr pGraphicsTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 207, 150, 14 ) );
	pGraphicsTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\graphics.png" ) );
	pWindowVideo->AddElement( pGraphicsTitle );

	// Text Texture Quality
	UI::Text_ptr pTextTextureQuality = std::make_shared<UI::Text>( Rectangle2D( 24, 238, 0, 0 ) );
	pTextTextureQuality->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextTextureQuality->SetText( "Texture Quality:" );
	pWindowVideo->AddElement( pTextTextureQuality );

	// Group Texture Quality
	UI::Group_ptr pGroupTextureQuality = std::make_shared<UI::Group>();
	pGroupTextureQuality->SetID( GROUPID_TextureQuality );
	pWindowVideo->AddElement( pGroupTextureQuality );

	UI::CheckBox_ptr pCheckBoxTextureHigh = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 258, 13, 13 ) );
	pCheckBoxTextureHigh->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTextureHigh->SetText( "High" );
	pCheckBoxTextureHigh->SetGroup( pGroupTextureQuality );
	pCheckBoxTextureHigh->SetID( CHECKBOXID_TextureHigh );
	pCheckBoxTextureHigh->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxTextureHigh );

	UI::CheckBox_ptr pCheckBoxTextureLow = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 258, 13, 13 ) );
	pCheckBoxTextureLow->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTextureLow->SetText( "Low" );
	pCheckBoxTextureLow->SetGroup( pGroupTextureQuality );
	pCheckBoxTextureLow->SetID( CHECKBOXID_TextureLow );
	pCheckBoxTextureLow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxTextureLow );

	// Text Anti Aliasing
	UI::Text_ptr pTextAntiAliasing = std::make_shared<UI::Text>( Rectangle2D( 24, 318, 0, 0 ) );
	pTextAntiAliasing->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextAntiAliasing->SetText( "Anti Aliasing:" );
	pWindowVideo->AddElement( pTextAntiAliasing );
	pTextAntiAliasing->Hide();

	// Group Anti Aliasing
	UI::Group_ptr pGroupAntiAliasing = std::make_shared<UI::Group>();
	pGroupAntiAliasing->SetID( GROUPID_AntiAliasing );
	pWindowVideo->AddElement( pGroupAntiAliasing );

	UI::CheckBox_ptr pCheckBoxAntiAliasingON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 338, 13, 13 ) );
	pCheckBoxAntiAliasingON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAntiAliasingON->SetText( "ON" );
	pCheckBoxAntiAliasingON->SetGroup( pGroupAntiAliasing );
	pCheckBoxAntiAliasingON->SetID( CHECKBOXID_AntiAliasingOFF );
	pCheckBoxAntiAliasingON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxAntiAliasingON );
	pCheckBoxAntiAliasingON->Hide();

	UI::CheckBox_ptr pCheckBoxAntiAliasingOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 338, 13, 13 ) );
	pCheckBoxAntiAliasingOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAntiAliasingOFF->SetText( "OFF" );
	pCheckBoxAntiAliasingOFF->SetGroup( pGroupAntiAliasing );
	pCheckBoxAntiAliasingOFF->SetID( CHECKBOXID_AntiAliasingOFF );
	pCheckBoxAntiAliasingOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxAntiAliasingOFF );
	pCheckBoxAntiAliasingOFF->Hide();

	// Text V-Sync
	UI::Text_ptr pTextVSync = std::make_shared<UI::Text>( Rectangle2D( 24, 278, 0, 0 ) );
	pTextVSync->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextVSync->SetText( "V-Sync:" );
	pWindowVideo->AddElement( pTextVSync );

	// Group V-Sync
	UI::Group_ptr pGroupVSync = std::make_shared<UI::Group>();
	pGroupVSync->SetID( GROUPID_VSync );
	pWindowVideo->AddElement( pGroupVSync );

	UI::CheckBox_ptr pCheckBoxVSyncON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 298, 13, 13 ) );
	pCheckBoxVSyncON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxVSyncON->SetText( "ON" );
	pCheckBoxVSyncON->SetGroup( pGroupVSync );
	pCheckBoxVSyncON->SetID( CHECKBOXID_VSyncON );
	pCheckBoxVSyncON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxVSyncON );

	UI::CheckBox_ptr pCheckBoxVSyncOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 298, 13, 13 ) );
	pCheckBoxVSyncOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxVSyncOFF->SetText( "OFF" );
	pCheckBoxVSyncOFF->SetGroup( pGroupVSync );
	pCheckBoxVSyncOFF->SetID( CHECKBOXID_VSyncOFF );
	pCheckBoxVSyncOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxVSyncOFF );

	// Text Bit Depth
	UI::Text_ptr pTextBitDepth = std::make_shared<UI::Text>( Rectangle2D( 204, 238, 0, 0 ) );
	pTextBitDepth->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextBitDepth->SetText( "Bit Depth:" );
	pWindowVideo->AddElement( pTextBitDepth );

	// Group Bit Depth
	UI::Group_ptr pGroupBitDepth = std::make_shared<UI::Group>();
	pGroupBitDepth->SetID( GROUPID_BitDepth );
	pWindowVideo->AddElement( pGroupBitDepth );

	UI::CheckBox_ptr pCheckBox32BPP = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 258, 13, 13 ) );
	pCheckBox32BPP->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox32BPP->SetText( "32 bit" );
	pCheckBox32BPP->SetGroup( pGroupBitDepth );
	pCheckBox32BPP->SetID( CHECKBOXID_32BPP );
	pCheckBox32BPP->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox32BPP );

	UI::CheckBox_ptr pCheckBox16BPP = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 258, 13, 13 ) );
	pCheckBox16BPP->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox16BPP->SetText( "16 bit" );
	pCheckBox16BPP->SetGroup( pGroupBitDepth );
	pCheckBox16BPP->SetID( CHECKBOXID_16BPP );
	pCheckBox16BPP->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox16BPP );

	// Text Effects
	UI::Text_ptr pTextEffects = std::make_shared<UI::Text>( Rectangle2D( 204, 278, 0, 0 ) );
	pTextEffects->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextEffects->SetText( "Effects:" );
	pWindowVideo->AddElement( pTextEffects );

	// Group Bit Depth
	UI::Group_ptr pGroupEffects = std::make_shared<UI::Group>();
	pGroupEffects->SetID( GROUPID_Effects );
	pWindowVideo->AddElement( pGroupEffects );

	UI::CheckBox_ptr pCheckBoxEffectsON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 298, 13, 13 ) );
	pCheckBoxEffectsON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxEffectsON->SetText( "ON" );
	pCheckBoxEffectsON->SetGroup( pGroupEffects );
	pCheckBoxEffectsON->SetID( CHECKBOXID_EffectsON );
	pCheckBoxEffectsON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxEffectsON );

	UI::CheckBox_ptr pCheckBoxEffectsOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 298, 13, 13 ) );
	pCheckBoxEffectsOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxEffectsOFF->SetText( "OFF" );
	pCheckBoxEffectsOFF->SetGroup( pGroupEffects );
	pCheckBoxEffectsOFF->SetID( CHECKBOXID_EffectsOFF );
	pCheckBoxEffectsOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxEffectsOFF );

	// Title Camera
	UI::ImageBox_ptr pCameraBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 365, 339, 23 ) );
	pCameraBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pCameraBar );

	UI::ImageBox_ptr pCameraTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 370, 150, 14 ) );
	pCameraTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\camera.png" ) );
	pWindowVideo->AddElement( pCameraTitle );

	// Text Camera iSight
	UI::Text_ptr pTextCameraSight = std::make_shared<UI::Text>( Rectangle2D( 24, 401, 0, 0 ) );
	pTextCameraSight->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextCameraSight->SetText( "Camera Range:" );
	pWindowVideo->AddElement( pTextCameraSight );

	// Group Camera iSight
	UI::Group_ptr pGroupCameraSight = std::make_shared<UI::Group>();
	pGroupCameraSight->SetID( GROUPID_CameraSight );
	pWindowVideo->AddElement( pGroupCameraSight );

	UI::CheckBox_ptr pCheckBoxFull = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 421, 13, 13 ) );
	pCheckBoxFull->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFull->SetText( "Full" );
	pCheckBoxFull->SetGroup( pGroupCameraSight );
	pCheckBoxFull->SetID( CHECKBOXID_Full );
	pCheckBoxFull->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxFull );

	UI::CheckBox_ptr pCheckBoxFar = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 421, 13, 13 ) );
	pCheckBoxFar->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFar->SetText( "Far" );
	pCheckBoxFar->SetGroup( pGroupCameraSight );
	pCheckBoxFar->SetID( CHECKBOXID_Far );
	pCheckBoxFar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxFar );

	UI::CheckBox_ptr pCheckBoxShort = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 441, 13, 13 ) );
	pCheckBoxShort->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShort->SetText( "Short" );
	pCheckBoxShort->SetGroup( pGroupCameraSight );
	pCheckBoxShort->SetID( CHECKBOXID_Short );
	pCheckBoxShort->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxShort );

	// Text Inverted Camera
	UI::Text_ptr pTextInvertedCamera = std::make_shared<UI::Text>( Rectangle2D( 204, 401, 0, 0 ) );
	pTextInvertedCamera->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextInvertedCamera->SetText( "Inverted Camera:" );
	pWindowVideo->AddElement( pTextInvertedCamera );

	// Group Inverted Camera
	UI::Group_ptr pGroupInvertedCamera = std::make_shared<UI::Group>();
	pGroupInvertedCamera->SetID( GROUPID_InvertedCamera );
	pWindowVideo->AddElement( pGroupInvertedCamera );

	UI::CheckBox_ptr pCheckBoxCameraON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 421, 13, 13 ) );
	pCheckBoxCameraON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxCameraON->SetText( "ON" );
	pCheckBoxCameraON->SetGroup( pGroupInvertedCamera );
	pCheckBoxCameraON->SetID( CHECKBOXID_CameraON );
	pCheckBoxCameraON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxCameraON );

	UI::CheckBox_ptr pCheckBoxOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 421, 13, 13 ) );
	pCheckBoxOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxOFF->SetText( "OFF" );
	pCheckBoxOFF->SetGroup( pGroupInvertedCamera );
	pCheckBoxOFF->SetID( CHECKBOXID_CameraOFF );
	pCheckBoxOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxOFF );


	int iYOffestARPG = 63;

	// Text ARPG Mode
	UI::Text_ptr pTextARPGCamera = std::make_shared<UI::Text>( Rectangle2D( 24, 401 + iYOffestARPG, 0, 0 ) );
	pTextARPGCamera->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextARPGCamera->SetText( "Enhanced Fixed Camera (Beta):" );
	pWindowVideo->AddElement( pTextARPGCamera );

	// Group Inverted ARPGCamera
	UI::Group_ptr pGroupARPGCamera = std::make_shared<UI::Group>();
	pGroupARPGCamera->SetID( GROUPID_ARPGCamera );
	pWindowVideo->AddElement( pGroupARPGCamera );

	UI::CheckBox_ptr pCheckBoxARPGCameraON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 421+ iYOffestARPG, 13, 13 ) );
	pCheckBoxARPGCameraON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxARPGCameraON->SetText( "ON" );
	pCheckBoxARPGCameraON->SetGroup( pGroupARPGCamera );
	pCheckBoxARPGCameraON->SetID( CHECKBOXID_ARPGCameraON );
	pCheckBoxARPGCameraON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxARPGCameraON );

	UI::CheckBox_ptr pCheckBoxARPGCameraOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 421+ iYOffestARPG, 13, 13 ) );
	pCheckBoxARPGCameraOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxARPGCameraOFF->SetText( "OFF" );
	pCheckBoxARPGCameraOFF->SetGroup( pGroupARPGCamera );
	pCheckBoxARPGCameraOFF->SetID( CHECKBOXID_ARPGCameraOFF );
	pCheckBoxARPGCameraOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxARPGCameraOFF );


	int iYOffset = 330;

	// Text iSound Effects
	UI::Text_ptr pTextCustomZoom = std::make_shared<UI::Text>( Rectangle2D( 24, 181 + iYOffset, 0, 0 ) );
	pTextCustomZoom->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextCustomZoom->SetText( "Override Zoom:" );
	pWindowVideo->AddElement( pTextCustomZoom );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerZoom = std::make_shared<UI::ImageBox>( Rectangle2D( 8, 204 + iYOffset, 386, 56 ) );
	pBackgroundControllerZoom->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowVideo->AddElement( pBackgroundControllerZoom );

	// Scroller Background
	UI::Scroll_ptr pScrollCustomZoom = std::make_shared<UI::Scroll>( Rectangle2D( 110, 230 + iYOffset, 265, 8 ) );
	pScrollCustomZoom->SetVertical( FALSE );
	pScrollCustomZoom->SetMinMax( 0, 100 );
	pScrollCustomZoom->SetID( SCROLLID_CustomZoom );
	pScrollCustomZoom->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollCustomZoom->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollCustomZoom->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pScrollCustomZoom->SetPercent( 50 );
	pWindowVideo->AddElement( pScrollCustomZoom );

	// Checkbox No Effects
	UI::CheckBox_ptr pCheckBoxNoCustom = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 228 + iYOffset, 13, 13 ) );
	pCheckBoxNoCustom->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoCustom->SetText( "Enabled" );
	pCheckBoxNoCustom->SetID( CHECKBOXID_CustomZoom );
	pCheckBoxNoCustom->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxNoCustom );


	iYOffset += 90;

	// Text iSound Effects
	UI::Text_ptr pTextCustomZoomTown = std::make_shared<UI::Text>( Rectangle2D( 24, 181 + iYOffset, 0, 0 ) );
	pTextCustomZoomTown->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextCustomZoomTown->SetText( "Override Zoom Town:" );
	pWindowVideo->AddElement( pTextCustomZoomTown );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerZoomTown = std::make_shared<UI::ImageBox>( Rectangle2D( 8, 204 + iYOffset, 386, 56 ) );
	pBackgroundControllerZoomTown->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowVideo->AddElement( pBackgroundControllerZoomTown );

	// Scroller Background
	UI::Scroll_ptr pScrollCustomZoomTown = std::make_shared<UI::Scroll>( Rectangle2D( 110, 230 + iYOffset, 265, 8 ) );
	pScrollCustomZoomTown->SetVertical( FALSE );
	pScrollCustomZoomTown->SetMinMax( 0, 100 );
	pScrollCustomZoomTown->SetID( SCROLLID_CustomZoomTown );
	pScrollCustomZoomTown->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollCustomZoomTown->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollCustomZoomTown->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pScrollCustomZoomTown->SetPercent( 50 );
	pWindowVideo->AddElement( pScrollCustomZoomTown );

	// Checkbox No Effects
	UI::CheckBox_ptr pCheckBoxCustomZoomTown = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 228 + iYOffset, 13, 13 ) );
	pCheckBoxCustomZoomTown->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxCustomZoomTown->SetText( "Enabled" );
	pCheckBoxCustomZoomTown->SetID( CHECKBOXID_CustomZoomTown );
	pCheckBoxCustomZoomTown->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxCustomZoomTown );

	GetListMain()->AddWindow( pWindowVideo );
}

void CSettingsWindow::BuildWindow1Audio()
{
	UI::Window_ptr pWindowAudio = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 395 ) );
	pWindowAudio->SetID( WINDOWID_Audio );

	// Title Background Music
	UI::ImageBox_ptr pBackgroundMusicBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pBackgroundMusicBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowAudio->AddElement( pBackgroundMusicBar );

	UI::ImageBox_ptr pBackgroundMusicTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pBackgroundMusicTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgmusic.png" ) );
	pWindowAudio->AddElement( pBackgroundMusicTitle );

	// Text Background Music
	UI::Text_ptr pTextBackgroundMusic = std::make_shared<UI::Text>( Rectangle2D( 24, 47, 0, 0 ) );
	pTextBackgroundMusic->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextBackgroundMusic->SetText( "Background Music:" );
	pWindowAudio->AddElement( pTextBackgroundMusic );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerMusic = std::make_shared<UI::ImageBox>( Rectangle2D( 13, 70, 386, 56 ) );
	pBackgroundControllerMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowAudio->AddElement( pBackgroundControllerMusic );

	// Scroller Background
	UI::Scroll_ptr pScrollBackgroundMusic = std::make_shared<UI::Scroll>( Rectangle2D( 110, 95, 265, 8 ) );
	pScrollBackgroundMusic->SetVertical( FALSE );
	pScrollBackgroundMusic->SetMinMax( 0, 100 );
	pScrollBackgroundMusic->SetID( SCROLLID_BGMusic );
	pScrollBackgroundMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollBackgroundMusic->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollBackgroundMusic->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pWindowAudio->AddElement( pScrollBackgroundMusic );

	// Checkbox No iSound
	UI::CheckBox_ptr pCheckBoxNoMusic = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 93, 13, 13 ) );
	pCheckBoxNoMusic->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoMusic->SetText( "No Music" );
	pCheckBoxNoMusic->SetID( CHECKBOXID_NoMusic );
	pCheckBoxNoMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowAudio->AddElement( pCheckBoxNoMusic );

	// Title iSound Effects
	UI::ImageBox_ptr pSoundEffectsBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 145, 339, 23 ) );
	pSoundEffectsBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowAudio->AddElement( pSoundEffectsBar );

	UI::ImageBox_ptr pSoundEffectsTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 150, 150, 14 ) );
	pSoundEffectsTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgsound.png" ) );
	pWindowAudio->AddElement( pSoundEffectsTitle );

	// Text iSound Effects
	UI::Text_ptr pTextSoundEffects = std::make_shared<UI::Text>( Rectangle2D( 24, 181, 0, 0 ) );
	pTextSoundEffects->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextSoundEffects->SetText( "Sound Effects:" );
	pWindowAudio->AddElement( pTextSoundEffects );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerSound = std::make_shared<UI::ImageBox>( Rectangle2D( 13, 204, 386, 56 ) );
	pBackgroundControllerSound->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowAudio->AddElement( pBackgroundControllerSound );

	// Scroller Background
	UI::Scroll_ptr pScrollEffectSound = std::make_shared<UI::Scroll>( Rectangle2D( 110, 230, 265, 8 ) );
	pScrollEffectSound->SetVertical( FALSE );
	pScrollEffectSound->SetMinMax( 0, 100 );
	pScrollEffectSound->SetID( SCROLLID_EffectSound );
	pScrollEffectSound->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollEffectSound->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollEffectSound->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pScrollEffectSound->SetPercent( 20 );
	pWindowAudio->AddElement( pScrollEffectSound );

	// Checkbox No Effects
	UI::CheckBox_ptr pCheckBoxNoEffects = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 228, 13, 13 ) );
	pCheckBoxNoEffects->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoEffects->SetText( "No Effects" );
	pCheckBoxNoEffects->SetID( CHECKBOXID_NoEffects );
	pCheckBoxNoEffects->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowAudio->AddElement( pCheckBoxNoEffects );


	pWindowAudio->Hide();

	GetListMain()->AddWindow( pWindowAudio );
}

void CSettingsWindow::BuildWindow1General()
{
	int iY = 0;

	const int column1 = 24;
	const int column2 = 112;
	const int column3 = 204;
	const int column4 = 292;

	UI::Window_ptr pWindowGeneral = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 860 ) );
	pWindowGeneral->SetID( WINDOWID_General );

	// Title Social
	UI::ImageBox_ptr pSocialBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pSocialBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pSocialBar );

	UI::ImageBox_ptr pSocialTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pSocialTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\social.png" ) );
	pWindowGeneral->AddElement( pSocialTitle );

	// Disable Party Requests
	UI::CheckBox_ptr pCheckBoxDisableParty = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 47, 13, 13 ) );
	pCheckBoxDisableParty->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDisableParty->SetText( "Disable Party Requests" );
	pCheckBoxDisableParty->SetID( CHECKBOXID_DisableParty );
	pCheckBoxDisableParty->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDisableParty );

	// Disable Trade Requests
	UI::CheckBox_ptr pCheckBoxDisableTrade = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 67, 13, 13 ) );
	pCheckBoxDisableTrade->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDisableTrade->SetText( "Disable Trade Requests" );
	pCheckBoxDisableTrade->SetID( CHECKBOXID_DisableTrade );
	pCheckBoxDisableTrade->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDisableTrade );

	// No Whisper
	UI::CheckBox_ptr pCheckBoxNoWhisper = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 87, 13, 13 ) );
	pCheckBoxNoWhisper->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoWhisper->SetText( "No Whisper" );
	pCheckBoxNoWhisper->SetID( CHECKBOXID_NoWhisper );
	pCheckBoxNoWhisper->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxNoWhisper );

	// No Trade chat in All
	UI::CheckBox_ptr pCheckBoxNoTrade = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 107, 13, 13 ) );
	pCheckBoxNoTrade->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoTrade->SetText( "No Trade Chat in All" );
	pCheckBoxNoTrade->SetID( CHECKBOXID_NoTrade );
	pCheckBoxNoTrade->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxNoTrade );

	// Title Keyboard
	UI::ImageBox_ptr pPersonalizationBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 130 + iY, 339, 23 ) );
	pPersonalizationBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pPersonalizationBar );

	UI::ImageBox_ptr pPersonalizationTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 135 + iY, 150, 14 ) );
	pPersonalizationTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\personalization.png" ) );
	pWindowGeneral->AddElement( pPersonalizationTitle );


	iY = 167;

	// Text
	UI::Text_ptr pTextDamageDebug = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextDamageDebug->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextDamageDebug->SetText( "Damage Debug:" );
	pWindowGeneral->AddElement( pTextDamageDebug );


	// Text
	UI::Text_ptr pTextKillInfo = std::make_shared<UI::Text>( Rectangle2D( column3, iY, 0, 0 ) );
	pTextKillInfo->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextKillInfo->SetText( "Kill Debug:" );
	pWindowGeneral->AddElement( pTextKillInfo );

	// Group
	UI::Group_ptr pGroupKillInfo = std::make_shared<UI::Group>();
	pGroupKillInfo->SetID( GROUPID_KillInfo );
	pWindowGeneral->AddElement( pGroupKillInfo );


	// Group
	UI::Group_ptr pGroupDamageDebug = std::make_shared<UI::Group>();
	pGroupDamageDebug->SetID( GROUPID_DamageDebug );
	pWindowGeneral->AddElement( pGroupDamageDebug );

	iY += 20;


	UI::CheckBox_ptr pCheckBoxDamageDebugON = std::make_shared<UI::CheckBox>( Rectangle2D( column1, iY, 13, 13 ) );
	pCheckBoxDamageDebugON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDamageDebugON->SetText( "ON" );
	pCheckBoxDamageDebugON->SetGroup( pGroupDamageDebug );
	pCheckBoxDamageDebugON->SetID( CHECKBOXID_DamageDebugON );
	pCheckBoxDamageDebugON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDamageDebugON );

	UI::CheckBox_ptr pCheckBoxDamageDebugOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column2, 187, 13, 13 ) );
	pCheckBoxDamageDebugOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDamageDebugOFF->SetText( "OFF" );
	pCheckBoxDamageDebugOFF->SetGroup( pGroupDamageDebug );
	pCheckBoxDamageDebugOFF->SetID( CHECKBOXID_DamageDebugOFF );
	pCheckBoxDamageDebugOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDamageDebugOFF );

	UI::CheckBox_ptr pCheckBoxKillInfoON = std::make_shared<UI::CheckBox>( Rectangle2D( column3, iY, 13, 13 ) );
	pCheckBoxKillInfoON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxKillInfoON->SetText( "ON" );
	pCheckBoxKillInfoON->SetGroup( pGroupKillInfo );
	pCheckBoxKillInfoON->SetID( CHECKBOXID_KillInfoON );
	pCheckBoxKillInfoON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxKillInfoON );

	UI::CheckBox_ptr pCheckBoxKillInfoOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column4, iY, 13, 13 ) );
	pCheckBoxKillInfoOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxKillInfoOFF->SetText( "OFF" );
	pCheckBoxKillInfoOFF->SetGroup( pGroupKillInfo );
	pCheckBoxKillInfoOFF->SetID( CHECKBOXID_KillInfoOFF );
	pCheckBoxKillInfoOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxKillInfoOFF );

	iY += 20;


	// Text
	UI::Text_ptr pTextHPBarTarget = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextHPBarTarget->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextHPBarTarget->SetText( "HP Bar on Target:" );
	pWindowGeneral->AddElement( pTextHPBarTarget );

	// Text
	UI::Text_ptr pTextHPBarSelf = std::make_shared<UI::Text>( Rectangle2D( column3, iY, 0, 0 ) );
	pTextHPBarSelf->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextHPBarSelf->SetText( "HP Bar on Self:" );
	pWindowGeneral->AddElement( pTextHPBarSelf );

	iY += 20;

	// Group
	UI::Group_ptr pGroupHPBarTarget = std::make_shared<UI::Group>();
	pGroupHPBarTarget->SetID( GROUPID_TargetHPBar );
	pWindowGeneral->AddElement( pGroupHPBarTarget );

	// Group
	UI::Group_ptr pGroupHPBarSelf = std::make_shared<UI::Group>();
	pGroupHPBarSelf->SetID( GROUPID_SelfHPBar );
	pWindowGeneral->AddElement( pGroupHPBarSelf );

	UI::CheckBox_ptr pCheckBoxHPBarTargetON = std::make_shared<UI::CheckBox>( Rectangle2D( column1, iY, 13, 13 ) );
	pCheckBoxHPBarTargetON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarTargetON->SetText( "Show" );
	pCheckBoxHPBarTargetON->SetGroup( pGroupHPBarTarget );
	pCheckBoxHPBarTargetON->SetID( CHECKBOXID_TargetHPBarON );
	pCheckBoxHPBarTargetON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarTargetON );

	UI::CheckBox_ptr pCheckBoxHPBarTargetOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column2, iY, 13, 13 ) );
	pCheckBoxHPBarTargetOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarTargetOFF->SetText( "Hide" );
	pCheckBoxHPBarTargetOFF->SetGroup( pGroupHPBarTarget );
	pCheckBoxHPBarTargetOFF->SetID( CHECKBOXID_TargetHPBarOFF );
	pCheckBoxHPBarTargetOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarTargetOFF );

	UI::CheckBox_ptr pCheckBoxHPBarSelfON = std::make_shared<UI::CheckBox>( Rectangle2D( column3, iY, 13, 13 ) );
	pCheckBoxHPBarSelfON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarSelfON->SetText( "Show" );
	pCheckBoxHPBarSelfON->SetGroup( pGroupHPBarSelf );
	pCheckBoxHPBarSelfON->SetID( CHECKBOXID_SelfHPBarShow );
	pCheckBoxHPBarSelfON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarSelfON );

	UI::CheckBox_ptr pCheckBoxHPBarSelfOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column4, iY, 13, 13 ) );
	pCheckBoxHPBarSelfOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarSelfOFF->SetText( "Hide" );
	pCheckBoxHPBarSelfOFF->SetGroup( pGroupHPBarSelf );
	pCheckBoxHPBarSelfOFF->SetID( CHECKBOXID_SelfHPBarHide );
	pCheckBoxHPBarSelfOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarSelfOFF );

	iY += 20;

	// Text
	UI::Text_ptr pTextFPS = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextFPS->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextFPS->SetText( "FPS:" );
	pWindowGeneral->AddElement( pTextFPS );


	// Text
	UI::Text_ptr pTextPing = std::make_shared<UI::Text>( Rectangle2D( column3, iY, 0, 0 ) );
	pTextPing->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextPing->SetText( "Ping:" );
	pWindowGeneral->AddElement( pTextPing );


	// Group
	UI::Group_ptr pGroupFPS = std::make_shared<UI::Group>();
	pGroupFPS->SetID( GROUPID_FPS );
	pWindowGeneral->AddElement( pGroupFPS );


	// Group
	UI::Group_ptr pGroupPing = std::make_shared<UI::Group>();
	pGroupPing->SetID( GROUPID_Ping );
	pWindowGeneral->AddElement( pGroupPing );


	iY += 20;

	UI::CheckBox_ptr pCheckBoxFPSON = std::make_shared<UI::CheckBox>( Rectangle2D( column1, iY, 13, 13 ) );
	pCheckBoxFPSON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFPSON->SetText( "ON" );
	pCheckBoxFPSON->SetGroup( pGroupFPS );
	pCheckBoxFPSON->SetID( CHECKBOXID_FPSON );
	pCheckBoxFPSON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxFPSON );

	UI::CheckBox_ptr pCheckBoxFPSOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column2, iY, 13, 13 ) );
	pCheckBoxFPSOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFPSOFF->SetText( "OFF" );
	pCheckBoxFPSOFF->SetGroup( pGroupFPS );
	pCheckBoxFPSOFF->SetID( CHECKBOXID_FPSOFF );
	pCheckBoxFPSOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxFPSOFF );


	UI::CheckBox_ptr pCheckBoxPingON = std::make_shared<UI::CheckBox>( Rectangle2D( column3, iY, 13, 13 ) );
	pCheckBoxPingON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPingON->SetText( "ON" );
	pCheckBoxPingON->SetGroup( pGroupPing );
	pCheckBoxPingON->SetID( CHECKBOXID_PingON );
	pCheckBoxPingON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPingON );

	UI::CheckBox_ptr pCheckBoxPingOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column4, iY, 13, 13 ) );
	pCheckBoxPingOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPingOFF->SetText( "OFF" );
	pCheckBoxPingOFF->SetGroup( pGroupPing );
	pCheckBoxPingOFF->SetID( CHECKBOXID_PingOFF );
	pCheckBoxPingOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPingOFF );

	iY += 20;

	// Group
	UI::Group_ptr pGroupAutoSelectTarget = std::make_shared<UI::Group>();
	pGroupAutoSelectTarget->SetID( GROUPID_AllowAutoSelectTargetForAoE );
	pWindowGeneral->AddElement( pGroupAutoSelectTarget );


	UI::Text_ptr pTextAutoTarget = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextAutoTarget->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextAutoTarget->SetText( "AoE Select Nearest Target" );
	//pTextAutoTarget->SetHoverText( "Allow auto selection of nearest target\nif no target is selected for certain\nAoE right-click skills to allow auto-cast" );
	pWindowGeneral->AddElement( pTextAutoTarget );

	iY += 20;
	UI::CheckBox_ptr pCheckBoxAutoTargetOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column1, iY, 13, 13 ) );
	pCheckBoxAutoTargetOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAutoTargetOFF->SetText( "Disable" );
	pCheckBoxAutoTargetOFF->SetGroup( pGroupAutoSelectTarget );
	pCheckBoxAutoTargetOFF->SetID( CHECKBOXID_AoEAutoSelectTargetOFF );
	pCheckBoxAutoTargetOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxAutoTargetOFF );



	UI::CheckBox_ptr pCheckBoxAutoTargetON = std::make_shared<UI::CheckBox>( Rectangle2D( column2, iY, 13, 13 ) );
	pCheckBoxAutoTargetON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAutoTargetON->SetText( "Enable" );
	pCheckBoxAutoTargetON->SetGroup( pGroupAutoSelectTarget );
	pCheckBoxAutoTargetON->SetID( CHECKBOXID_AoEAutoSelectTargetON );
	pCheckBoxAutoTargetON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxAutoTargetON );


	iY += 20;

	UI::Text_ptr pTextPetModeInfo = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextPetModeInfo->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextPetModeInfo->SetText( "Default Pet Mode:" );
	pWindowGeneral->AddElement( pTextPetModeInfo );


	UI::Text_ptr pTextChatBoxMode = std::make_shared<UI::Text>( Rectangle2D( column3, iY, 0, 0 ) );
	pTextChatBoxMode->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextChatBoxMode->SetText( "Chat Focus After Enter:" );
	pWindowGeneral->AddElement( pTextChatBoxMode );



	iY += 20;

	// Group
	UI::Group_ptr pGroupPetModeInfo = std::make_shared<UI::Group>();
	pGroupPetModeInfo->SetID( GROUPID_PetMode );
	pWindowGeneral->AddElement( pGroupPetModeInfo );

	// Group
	UI::Group_ptr pGroupChatBoxMode = std::make_shared<UI::Group>();
	pGroupChatBoxMode->SetID( GROUPID_ChatBoxMode );
	pWindowGeneral->AddElement( pGroupChatBoxMode );


	UI::CheckBox_ptr pCheckBoxPetModeAggressive = std::make_shared<UI::CheckBox>( Rectangle2D( column1, iY, 13, 13 ) );
	pCheckBoxPetModeAggressive->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPetModeAggressive->SetText( "Aggressive" );
	pCheckBoxPetModeAggressive->SetGroup( pGroupPetModeInfo );
	pCheckBoxPetModeAggressive->SetID( CHECKBOXID_PetModeAgressive );
	pCheckBoxPetModeAggressive->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPetModeAggressive );

	UI::CheckBox_ptr pCheckBoxPetModePassive = std::make_shared<UI::CheckBox>( Rectangle2D( column2, iY, 13, 13 ) );
	pCheckBoxPetModePassive->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPetModePassive->SetText( "Passive" );
	pCheckBoxPetModePassive->SetGroup( pGroupPetModeInfo );
	pCheckBoxPetModePassive->SetID( CHECKBOXID_PetModePassive );
	pCheckBoxPetModePassive->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPetModePassive );



	UI::CheckBox_ptr pCheckBoxChatBoxRemoveFocusON = std::make_shared<UI::CheckBox>( Rectangle2D( column3, iY, 13, 13 ) );
	pCheckBoxChatBoxRemoveFocusON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxChatBoxRemoveFocusON->SetText( "Remove" );
	pCheckBoxChatBoxRemoveFocusON->SetGroup( pGroupChatBoxMode );
	pCheckBoxChatBoxRemoveFocusON->SetID( CHECKBOXID_ChatBoxRemoveFocusOnEnterON );
	pCheckBoxChatBoxRemoveFocusON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxChatBoxRemoveFocusON );

	UI::CheckBox_ptr pCheckBoxChatBoxRemoveFocusOFF = std::make_shared<UI::CheckBox>( Rectangle2D( column4, iY, 13, 13 ) );
	pCheckBoxChatBoxRemoveFocusOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxChatBoxRemoveFocusOFF->SetText( "Keep" );
	pCheckBoxChatBoxRemoveFocusOFF->SetGroup( pGroupChatBoxMode );
	pCheckBoxChatBoxRemoveFocusOFF->SetID( CHECKBOXID_ChatBoxRemoveFocusOnEnterOFF );
	pCheckBoxChatBoxRemoveFocusOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxChatBoxRemoveFocusOFF );


	iY += 20;

	// Text
	UI::Text_ptr pTextItemViewBehaviour = std::make_shared<UI::Text>(Rectangle2D(column1, iY, 0, 0));
	pTextItemViewBehaviour->SetFont("Arial", 16, 0, FALSE, TRUE, -1);
	pTextItemViewBehaviour->SetText("Loot view 'A' behavior:");
	pWindowGeneral->AddElement(pTextItemViewBehaviour);

	// Text
	UI::Text_ptr pTextEquipmentFilterBehaviour = std::make_shared<UI::Text>(Rectangle2D(column3, iY, 0, 0));
	pTextEquipmentFilterBehaviour->SetFont("Arial", 16, 0, FALSE, TRUE, -1);
	pTextEquipmentFilterBehaviour->SetText("Loot Filter behavior:");
	pWindowGeneral->AddElement(pTextEquipmentFilterBehaviour);

	iY += 20;

	// Group
	UI::Group_ptr pGroupItemViewBehaviour = std::make_shared<UI::Group>();
	pGroupItemViewBehaviour->SetID(GROUPID_ItemView);
	pWindowGeneral->AddElement(pGroupItemViewBehaviour);

	UI::CheckBox_ptr pCheckBoxItemViewHold = std::make_shared<UI::CheckBox>(Rectangle2D(column1, iY, 13, 13));
	pCheckBoxItemViewHold->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxItemViewHold->SetText("Hold");
	pCheckBoxItemViewHold->SetGroup(pGroupItemViewBehaviour);
	pCheckBoxItemViewHold->SetID(CHECKBOXID_ItemViewHold);
	pCheckBoxItemViewHold->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pWindowGeneral->AddElement(pCheckBoxItemViewHold);


	UI::CheckBox_ptr pCheckBoxItemViewToggle = std::make_shared<UI::CheckBox>(Rectangle2D(column2, iY, 13, 13));
	pCheckBoxItemViewToggle->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxItemViewToggle->SetText("Toggle");
	pCheckBoxItemViewToggle->SetGroup(pGroupItemViewBehaviour);
	pCheckBoxItemViewToggle->SetID(CHECKBOXID_ItemViewToggle);
	pCheckBoxItemViewToggle->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pWindowGeneral->AddElement(pCheckBoxItemViewToggle);



	// Group
	UI::Group_ptr pLootFilterBehaviour = std::make_shared<UI::Group>();
	pLootFilterBehaviour->SetID(GROUPID_LootFilterBehaviour);
	pWindowGeneral->AddElement(pLootFilterBehaviour);

	UI::CheckBox_ptr pCheckBoxLootFilterHide = std::make_shared<UI::CheckBox>(Rectangle2D(column3, iY, 13, 13));
	pCheckBoxLootFilterHide->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxLootFilterHide->SetText("Invisible");
	pCheckBoxLootFilterHide->SetGroup(pLootFilterBehaviour);
	pCheckBoxLootFilterHide->SetID(CHECKBOXID_LootFilterHide);
	pCheckBoxLootFilterHide->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pWindowGeneral->AddElement(pCheckBoxLootFilterHide);


	UI::CheckBox_ptr pCheckBoxLootFilterDisable = std::make_shared<UI::CheckBox>(Rectangle2D(column4, iY, 13, 13));
	pCheckBoxLootFilterDisable->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxLootFilterDisable->SetText("Disable");
	pCheckBoxLootFilterDisable->SetGroup(pLootFilterBehaviour);
	pCheckBoxLootFilterDisable->SetID(CHECKBOXID_LootFilterDisable);
	pCheckBoxLootFilterDisable->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pWindowGeneral->AddElement(pCheckBoxLootFilterDisable);

	iY += 20;
	iY += 20;

	// Text
	UI::Text_ptr pTextLootFilter = std::make_shared<UI::Text>( Rectangle2D( column1, iY, 0, 0 ) );
	pTextLootFilter->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextLootFilter->SetText( "Loot Filter:" );
	pWindowGeneral->AddElement( pTextLootFilter );

	iY += 20;

	// Group
	UI::Group_ptr pGroupMacro = std::make_shared<UI::Group>();

	//Loot
	{
		UI::CheckBox_ptr pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, TRUE, -1 );
		pCheckBoxLoot->SetText( "Enable Loot Filter" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootFilterEnable );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 30;

		int iYLootSpec = iY;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "HP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootHPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "MP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootMPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "SP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Gold" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootGold );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>(Rectangle2D(24, iY, 13, 13));
		pCheckBoxLoot->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxLoot->SetText("Force Orbs");
		pCheckBoxLoot->SetID(CHECKBOXID_LootForceOrbs);
		pCheckBoxLoot->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxLoot);
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>(Rectangle2D(24, iY, 13, 13));
		pCheckBoxLoot->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxLoot->SetText("Crystals");
		pCheckBoxLoot->SetID(CHECKBOXID_LootCrystals);
		pCheckBoxLoot->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxLoot);
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Rings and Amulets" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootJewellery );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );

		pCheckBoxLoot = std::make_shared<UI::CheckBox>(Rectangle2D(184, iY, 13, 13));
		pCheckBoxLoot->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxLoot->SetText("Non spec rings and amulets");
		pCheckBoxLoot->SetID(CHECKBOXID_LootNonSpecJewellery);
		pCheckBoxLoot->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxLoot);

		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Equipment" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootGear );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );

		pCheckBoxLoot = std::make_shared<UI::CheckBox>(Rectangle2D(184, iY, 13, 13));
		pCheckBoxLoot->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxLoot->SetText("Non spec equipment");
		pCheckBoxLoot->SetID(CHECKBOXID_LootNonSpecGear);
		pCheckBoxLoot->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxLoot);
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D(24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Sheltoms" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSheltoms );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );

		pCheckBoxLoot = std::make_shared<UI::CheckBox>(Rectangle2D(184, iY, 13, 13));
		pCheckBoxLoot->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxLoot->SetText("Non spec sheltoms");
		pCheckBoxLoot->SetID(CHECKBOXID_LootNonSpecSheltoms);
		pCheckBoxLoot->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxLoot);

		iY += 20;

		UI::CheckBox_ptr pCheckBoxEquipmentMinLevel = std::make_shared<UI::CheckBox>(Rectangle2D(24, iY, 13, 13));
		pCheckBoxEquipmentMinLevel->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxEquipmentMinLevel->SetText("Equipment min level:");
		pCheckBoxEquipmentMinLevel->SetID(CHECKBOXID_LootEquipmentMinLevel);
		pCheckBoxEquipmentMinLevel->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxEquipmentMinLevel);

		// Text
		/*UI::Text_ptr pTextEquipmentMinLevelFilter = std::make_shared<UI::Text>(Rectangle2D(54, iY, 0, 0));
		pTextEquipmentMinLevelFilter->SetID(TextID_LootEquipmentMinLevel);
		pTextEquipmentMinLevelFilter->SetFont("Arial", 16, 0, FALSE, FALSE, -1);
		pTextEquipmentMinLevelFilter->SetText("Equipment min level:");
		pWindowGeneral->AddElement(pTextEquipmentMinLevelFilter);*/

		UI::Group_ptr pGroupLootFilter = std::make_shared<UI::Group>();
		pGroupLootFilter->SetID(GROUPID_LootFilterEquipmentLevel);

		UI::InputField_ptr pInputEquipmentMinLevelFilter = std::make_shared<UI::InputField>(Rectangle2D(184, iY, 40, 18));
		pInputEquipmentMinLevelFilter->SetID(InputFieldID_LootEquipmentMinLevel);
		pInputEquipmentMinLevelFilter->SetImage(UI::ImageLoader::LoadImage("game\\images\\settings\\textbox.png"));
		pInputEquipmentMinLevelFilter->SetFont("Arial", 16, 0, FALSE, FALSE, -1);
		pInputEquipmentMinLevelFilter->SetText("");
		pInputEquipmentMinLevelFilter->SetMaxLength(3);
		pInputEquipmentMinLevelFilter->SetSelect(TRUE);
		pInputEquipmentMinLevelFilter->SetGroup(pGroupMacro);
		pInputEquipmentMinLevelFilter->SetAllowedCharacters("0123456789");
		pInputEquipmentMinLevelFilter->SetTextAlign(EAlign::ALIGN_Left, EAlign::ALIGN_Middle);
		pWindowGeneral->AddElement(pInputEquipmentMinLevelFilter);

		iY += 40;
	}

	//enchant weapon
	{

#ifdef ENCHANTWEAPON_SETTING
		UI::Text_ptr pTextEnchantWeapon = std::make_shared<UI::Text>( Rectangle2D( 24, iY, 0, 0 ) );
		pTextEnchantWeapon->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextEnchantWeapon->SetText( "Enchant Weapon Perference:" );
		pWindowGeneral->AddElement( pTextEnchantWeapon );

		iY += 20;

		// Group
		UI::Group_ptr pEnchantWeaponGroup = std::make_shared<UI::Group>();
		pEnchantWeaponGroup->SetID(GROUPID_EnchantWeapon);
		pWindowGeneral->AddElement(pEnchantWeaponGroup);

		UI::CheckBox_ptr pCheckBoxEWRandom = std::make_shared<UI::CheckBox>(Rectangle2D(24, iY, 13, 13));
		pCheckBoxEWRandom->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxEWRandom->SetText("Rotate");
		pCheckBoxEWRandom->SetGroup(pEnchantWeaponGroup);
		pCheckBoxEWRandom->SetID(CHECKBOXID_EW_Rotate);
		pCheckBoxEWRandom->SetValue( 0 );
		pCheckBoxEWRandom->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxEWRandom);

		UI::CheckBox_ptr pCheckBoxEWFire = std::make_shared<UI::CheckBox>(Rectangle2D(112, iY, 13, 13));
		pCheckBoxEWFire->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxEWFire->SetText("Fire");
		pCheckBoxEWFire->SetGroup(pEnchantWeaponGroup);
		pCheckBoxEWFire->SetID(CHECKBOXID_EW_Fire);
		pCheckBoxEWFire->SetValue( EElementID::Fire );
		pCheckBoxEWFire->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxEWFire);

		UI::CheckBox_ptr pCheckBoxEWIce = std::make_shared<UI::CheckBox>(Rectangle2D(204, iY, 13, 13));
		pCheckBoxEWIce->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxEWIce->SetText("Ice");
		pCheckBoxEWIce->SetGroup(pEnchantWeaponGroup);
		pCheckBoxEWIce->SetID(CHECKBOXID_EW_Ice);
		pCheckBoxEWIce->SetValue( EElementID::Ice );
		pCheckBoxEWIce->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxEWIce);

		UI::CheckBox_ptr pCheckBoxEWLightning = std::make_shared<UI::CheckBox>(Rectangle2D(292, iY, 13, 13));
		pCheckBoxEWLightning->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
		pCheckBoxEWLightning->SetText("Lightning");
		pCheckBoxEWLightning->SetGroup(pEnchantWeaponGroup);
		pCheckBoxEWLightning->SetID( CHECKBOXID_EW_Lightning );
		pCheckBoxEWLightning->SetValue( EElementID::Lightning );
		pCheckBoxEWLightning->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
		pWindowGeneral->AddElement(pCheckBoxEWLightning);

		iY += 40;
#endif
	}



	// Title Keyboard
	UI::ImageBox_ptr pKeyboardBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, iY, 339, 23 ) );
	pKeyboardBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pKeyboardBar );

	UI::ImageBox_ptr pKeyboardTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, iY + 5, 150, 14 ) );
	pKeyboardTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\keyboard.png" ) );
	pWindowGeneral->AddElement( pKeyboardTitle );

	iY += 36;

	// Macro
	{

		pGroupMacro->SetID( GROUPID_Macro );
		pWindowGeneral->AddElement( pGroupMacro );

		char szBuf[32] = { 0 };
		for ( int i = 0; i < 10; i++ )
		{
			// Text
			UI::Text_ptr pTextMacro = std::make_shared<UI::Text>( Rectangle2D( 24, (i * 20) + iY, 20, 16 ) );
			STRINGFORMAT( szBuf, "CTRL+F%d", i + 1 );
			pTextMacro->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
			pTextMacro->SetText( szBuf );
			pWindowGeneral->AddElement( pTextMacro );

			// InputField
			UI::InputField_ptr pInputField = std::make_shared<UI::InputField>( Rectangle2D( 88, (i * 20) + iY, 290, 18 ) );
			pInputField->SetID( InputFieldID_Macro1 + i );
			pInputField->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\textbox.png" ) );
			pInputField->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
			pInputField->SetText( "" );
			pInputField->SetMaxLength( 32 );
			pInputField->SetSelect( TRUE );
			pInputField->SetGroup( pGroupMacro );
			pWindowGeneral->AddElement( pInputField );
		}
	}

	pWindowGeneral->Hide();

	GetListMain()->AddWindow( pWindowGeneral );
}

void CSettingsWindow::BuildWindow1Interface()
{
	int iY = 0;

	UI::Window_ptr pWindowInterface = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 500 ) );
	pWindowInterface->SetID( WINDOWID_Interface );

	//Title General
	UI::ImageBox_ptr pGeneralBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pGeneralBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pGeneralBar );

	UI::ImageBox_ptr pGeneralTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pGeneralTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\generalt.png" ) );
	pWindowInterface->AddElement( pGeneralTitle );

	//Lock User Interface
	UI::CheckBox_ptr pCheckBoxLockUI = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 48, 13, 13 ) );
	pCheckBoxLockUI->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxLockUI->SetText( "Lock User Interface" );
	pCheckBoxLockUI->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxLockUI->SetID( CHECKBOXID_LockUI );
	pWindowInterface->AddElement( pCheckBoxLockUI );

	//Title Minimap
	UI::ImageBox_ptr pMinimapBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 72, 339, 23 ) );
	pMinimapBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pMinimapBar );

	UI::ImageBox_ptr pMinimapTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 77, 150, 14 ) );
	pMinimapTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\minimap.png" ) );
	pWindowInterface->AddElement( pMinimapTitle );

	//Show Party Members
	UI::CheckBox_ptr pCheckBoxShowParty = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 112, 13, 13 ) );
	pCheckBoxShowParty->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowParty->SetText( "Show Party Members" );
	pCheckBoxShowParty->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowParty->SetID( CHECKBOXID_ShowPartyMembers );
	pWindowInterface->AddElement( pCheckBoxShowParty );

	//Show Party Members
	UI::CheckBox_ptr pCheckBoxShowRaid = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 132, 13, 13 ) );
	pCheckBoxShowRaid->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowRaid->SetText( "Show Raid Members" );
	pCheckBoxShowRaid->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowRaid->SetID( CHECKBOXID_ShowRaidMembers );
	pWindowInterface->AddElement( pCheckBoxShowRaid );

	//Show Clan Members
	UI::CheckBox_ptr pCheckBoxShowClan = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 152, 13, 13 ) );
	pCheckBoxShowClan->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowClan->SetText( "Show Clan Members" );
	pCheckBoxShowClan->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowClan->SetID( CHECKBOXID_ShowClanMembers );
	pWindowInterface->AddElement( pCheckBoxShowClan );

	//Show Enemies
	UI::CheckBox_ptr pCheckBoxShowEnemies = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 172, 13, 13 ) );
	pCheckBoxShowEnemies->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowEnemies->SetText( "Show Enemies at Bless Castle" );
	pCheckBoxShowEnemies->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowEnemies->SetID( CHECKBOXID_ShowEnemies );
	pWindowInterface->AddElement( pCheckBoxShowEnemies );

	//Show NPCs
	UI::CheckBox_ptr pCheckBoxShowNPCs = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 192, 13, 13 ) );
	pCheckBoxShowNPCs->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowNPCs->SetText( "Show NPCs" );
	pCheckBoxShowNPCs->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowNPCs->SetID( CHECKBOXID_ShowNPCs );
	pWindowInterface->AddElement( pCheckBoxShowNPCs );

	//Show Next Boss Timer
	UI::CheckBox_ptr pCheckBoxShowBossTimer = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 212, 13, 13 ) );
	pCheckBoxShowBossTimer->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowBossTimer->SetText( "Show Next Boss Timer" );
	pCheckBoxShowBossTimer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowBossTimer->SetID( CHECKBOXID_ShowNextBossTimer );
	pWindowInterface->AddElement( pCheckBoxShowBossTimer );

	//Show Name above Head
	UI::CheckBox_ptr pCheckBoxShowNameAbove = std::make_shared<UI::CheckBox>(Rectangle2D(24, 232, 13, 13));
	pCheckBoxShowNameAbove->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxShowNameAbove->SetText("Show Name Above Head");
	pCheckBoxShowNameAbove->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pCheckBoxShowNameAbove->SetID(CHECKBOXID_ShowNameAboveHead);
	pWindowInterface->AddElement(pCheckBoxShowNameAbove);

	//Show Frame above Head
	UI::CheckBox_ptr pCheckBoxFrameNameAbove = std::make_shared<UI::CheckBox>(Rectangle2D(250, 232, 13, 13));
	pCheckBoxFrameNameAbove->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	pCheckBoxFrameNameAbove->SetText("Show Frame");
	pCheckBoxFrameNameAbove->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	pCheckBoxFrameNameAbove->SetID(CHECKBOXID_ShowFrameAboveHead);
	pWindowInterface->AddElement(pCheckBoxFrameNameAbove);

	//Minimap Active - JLM - commented out, this causes too many complaints
	/*UI::CheckBox_ptr pCheckBoxMinimapActive = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 232, 13, 13 ) );
	pCheckBoxMinimapActive->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxMinimapActive->SetText( "Always Active" );
	pCheckBoxMinimapActive->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxMinimapActive->SetID( CHECKBOXID_AlwaysActive );
	pWindowInterface->AddElement( pCheckBoxMinimapActive );*/

	//Title Chat Box
	UI::ImageBox_ptr pChatboxBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 260, 339, 23 ) );
	pChatboxBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pChatboxBar );

	UI::ImageBox_ptr pChatboxTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 265, 150, 14 ) );
	pChatboxTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\chatbox.png" ) );
	pWindowInterface->AddElement( pChatboxTitle );

	//Hide Notices
	UI::CheckBox_ptr pCheckBoxHideNotices = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 300, 13, 13 ) );
	pCheckBoxHideNotices->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHideNotices->SetText( "Show 'Notice' info in Chatbox" );
	pCheckBoxHideNotices->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxHideNotices->SetID( CHECKBOXID_ShowNotice );
	pWindowInterface->AddElement( pCheckBoxHideNotices );

	//Title Target
	UI::ImageBox_ptr pTargetBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 324, 339, 23 ) );
	pTargetBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pTargetBar );

	UI::ImageBox_ptr pTargetTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 329, 150, 14 ) );
	pTargetTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\target.png" ) );
	pWindowInterface->AddElement( pTargetTitle );

	//Target Distance
	UI::CheckBox_ptr pCheckBoxTargetDistance = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 358, 13, 13 ) );
	pCheckBoxTargetDistance->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetDistance->SetText( "Show Target Distance" );
	pCheckBoxTargetDistance->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetDistance->SetID( CHECKBOXID_TargetDistance );
	pWindowInterface->AddElement( pCheckBoxTargetDistance );

	//Target HP Value
	UI::CheckBox_ptr pCheckBoxTargetHPValue = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 378, 13, 13 ) );
	pCheckBoxTargetHPValue->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetHPValue->SetText( "Show Target HP Value" );
	pCheckBoxTargetHPValue->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetHPValue->SetID( CHECKBOXID_TargetHPValue );
	pWindowInterface->AddElement( pCheckBoxTargetHPValue );

	//Target HP in percentage
	UI::CheckBox_ptr pCheckBoxTargetHPPercent = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 398, 13, 13 ) );
	pCheckBoxTargetHPPercent->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetHPPercent->SetText( "Show Target HP in %" );
	pCheckBoxTargetHPPercent->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetHPPercent->SetID( CHECKBOXID_TargetHPPercent );
	pWindowInterface->AddElement( pCheckBoxTargetHPPercent );


	pWindowInterface->Hide();

	GetListMain()->AddWindow( pWindowInterface );
}

UI::List_ptr CSettingsWindow::GetListMain()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Main );
	if ( pList )
		return pList;

	return NULL;
}

UI::Window_ptr CSettingsWindow::GetWindow( int iID )
{
	return GetListMain()->GetWindow( iID );
}
