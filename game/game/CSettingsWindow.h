#pragma once
#include "UI.h"



class CSettingsWindow
{
public:
													CSettingsWindow();
	virtual											~CSettingsWindow();

	void											OnMouseMove( class CMouse * pcMouse );
	BOOL											OnMouseClick( class CMouse * pcMouse );
	BOOL											OnMouseScroll( class CMouse * pcMouse );

	BOOL											OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL											OnKeyChar( class CKeyboard * pcKeyboard );

	void											OnResolutionChanged();

	void											Update( float fTime );
	void											Render();
	void											Init();

	enum
	{
		// Windows
		WINDOWID_Main,

		// Child Windows
		WINDOWID_Video,
		WINDOWID_Audio,
		WINDOWID_General,
		WINDOWID_Interface,

		WINDOWID_Screen,
		WINDOWID_Graphics,
		WINDOWID_Camera,

		LISTID_Main,

		// Buttons
		BUTTONID_Video,
		BUTTONID_Audio,
		BUTTONID_General,
		BUTTONID_Interface,

		// Groups
		GROUPID_Menu,

		GROUPID_Display,
		GROUPID_Ratio,

		GROUPID_Resolutions4_3,
		GROUPID_Resolutions16_9,

		GROUPID_TextureQuality,
		GROUPID_BitDepth,
		GROUPID_VSync,
		GROUPID_Effects,
		GROUPID_DamageDebug,
		GROUPID_TargetHPBar,
		GROUPID_SelfHPBar,
		GROUPID_FPS,
		GROUPID_LootFilterEquipmentLevel,
		GROUPID_Ping,
		GROUPID_KillInfo,
		GROUPID_PetMode,
		GROUPID_ChatBoxMode,
		GROUPID_AllowAutoSelectTargetForAoE,
		GROUPID_ItemView,
		GROUPID_LootFilterBehaviour,
		GROUPID_EnchantWeapon,

		GROUPID_CameraSight,
		GROUPID_InvertedCamera,
		GROUPID_ARPGCamera,

		GROUPID_Macro,
		GROUPID_AntiAliasing,

		// Checkbox
		CHECKBOXID_Fullscreen,
		CHECKBOXID_Windowed,
		CHECKBOXID_AutoAdjust,
		CHECKBOXID_StartMaximized,

		CHECKBOXID_Ratio4_3,
		CHECKBOXID_Ratio16_9,

		CHECKBOXID_1024x768,
		CHECKBOXID_1280x960,
		CHECKBOXID_1400x1050,

		CHECKBOXID_1280x720,
		CHECKBOXID_1366x768,
		CHECKBOXID_1600x900,
		CHECKBOXID_1920x1080,

		CHECKBOXID_TextureHigh,
		CHECKBOXID_TextureLow,

		CHECKBOXID_32BPP,
		CHECKBOXID_16BPP,

		CHECKBOXID_VSyncON,
		CHECKBOXID_VSyncOFF,

		CHECKBOXID_Far,
		CHECKBOXID_Short,
		CHECKBOXID_Full,

		CHECKBOXID_CameraON,
		CHECKBOXID_CameraOFF,

		CHECKBOXID_ARPGCameraON,
		CHECKBOXID_ARPGCameraOFF,

		CHECKBOXID_NoMusic,
		CHECKBOXID_NoEffects,

		CHECKBOXID_EffectsON,
		CHECKBOXID_EffectsOFF,

		CHECKBOXID_DamageDebugON,
		CHECKBOXID_DamageDebugOFF,

		CHECKBOXID_TargetHPBarON,
		CHECKBOXID_TargetHPBarOFF,

		CHECKBOXID_SelfHPBarShow,
		CHECKBOXID_SelfHPBarHide,

		CHECKBOXID_FPSON,
		CHECKBOXID_FPSOFF,

		CHECKBOXID_PingON,
		CHECKBOXID_PingOFF,

		CHECKBOXID_KillInfoON,
		CHECKBOXID_KillInfoOFF,

		CHECKBOXID_PetModeAgressive,
		CHECKBOXID_PetModePassive,

		CHECKBOXID_ChatBoxRemoveFocusOnEnterON,
		CHECKBOXID_ChatBoxRemoveFocusOnEnterOFF,

		CHECKBOXID_AoEAutoSelectTargetON,
		CHECKBOXID_AoEAutoSelectTargetOFF,

		CHECKBOXID_ItemViewHold,
		CHECKBOXID_ItemViewToggle,

		CHECKBOXID_LootFilterHide,
		CHECKBOXID_LootFilterDisable,

		CHECKBOXID_LootFilterEnable,
		CHECKBOXID_LootHPPotion,
		CHECKBOXID_LootMPPotion,
		CHECKBOXID_LootSPPotion,
		CHECKBOXID_LootGold,
		CHECKBOXID_LootJewellery,
		CHECKBOXID_LootGear,
		CHECKBOXID_LootSheltoms,
		CHECKBOXID_LootForceOrbs,
		CHECKBOXID_LootCrystals,
		CHECKBOXID_LootNonSpecJewellery,
		CHECKBOXID_LootNonSpecGear,
		CHECKBOXID_LootNonSpecSheltoms,
		CHECKBOXID_LootEquipmentMinLevel,

		//Loot Spec
		CHECKBOXID_LootSpecNS,
		CHECKBOXID_LootSpecFS,
		CHECKBOXID_LootSpecMS,
		CHECKBOXID_LootSpecAS,
		CHECKBOXID_LootSpecPS,
		CHECKBOXID_LootSpecATA,
		CHECKBOXID_LootSpecKS,
		CHECKBOXID_LootSpecMGS,
		CHECKBOXID_LootSpecPRS,
		CHECKBOXID_LootSpecASS,
		CHECKBOXID_LootSpecSHA,

		CHECKBOXID_EW_Ice,
		CHECKBOXID_EW_Fire,
		CHECKBOXID_EW_Lightning,
		CHECKBOXID_EW_Rotate,

		CHECKBOXID_DisableParty,
		CHECKBOXID_DisableTrade,
		CHECKBOXID_NoWhisper,
		CHECKBOXID_NoTrade,

		CHECKBOXID_AntiAliasingON,
		CHECKBOXID_AntiAliasingOFF,

		CHECKBOXID_LockUI,
		CHECKBOXID_SaveUI,
		CHECKBOXID_ShowPartyMembers,
		CHECKBOXID_ShowRaidMembers,
		CHECKBOXID_ShowClanMembers,
		CHECKBOXID_ShowEnemies,
		CHECKBOXID_ShowNPCs,
		CHECKBOXID_ShowNextBossTimer,
		CHECKBOXID_ShowNotice,
		CHECKBOXID_TargetDistance,
		CHECKBOXID_TargetHPValue,
		CHECKBOXID_TargetHPPercent,
		CHECKBOXID_ShowNameAboveHead,
		CHECKBOXID_ShowFrameAboveHead,
		CHECKBOXID_CustomZoom,
		CHECKBOXID_CustomZoomTown,

		// Scrolls
		SCROLLID_BGMusic,
		SCROLLID_EffectSound,
		SCROLLID_CustomZoom,
		SCROLLID_CustomZoomTown,

		// Texts
		TextID_BossTime,
		TextID_ServerTime,
		TextID_ServerName,
		TEXTID_SiegeWarTime,
		TEXTID_SiegeWarType,

		// InputField
		InputFieldID_Macro1,
		InputFieldID_Macro2,
		InputFieldID_Macro3,
		InputFieldID_Macro4,
		InputFieldID_Macro5,
		InputFieldID_Macro6,
		InputFieldID_Macro7,
		InputFieldID_Macro8,
		InputFieldID_Macro9,
		InputFieldID_Macro10,

		TextID_LootEquipmentMinLevel,
		InputFieldID_LootEquipmentMinLevel,

		//List
		LISTID_Video,
	};

private:

	void											SetResolutionSettings();
	int												GetResolutionCheckBox();
	void											SetResolutionCheckBox();

	void											Load();
	void											Save();

	void											SetEnchantWeaponPerference( int value );
	void											ToggleLootFilter(BOOL bChecked);
	void											ToggleCustomCamera(BOOL bChecked);
	void											OnCheckBoxChanged(int iID, UIEventArgs e);
	void											OnCheckBoxChangedVideo(int iID, UIEventArgs e);

	void											OnCheckBoxRatio43Click( UIEventArgs eArgs );
	void											OnCheckBoxRatio169Click( UIEventArgs eArgs );
	void											OnVideoButtonClick( UIEventArgs eArgs );
	void											OnAudioButtonClick( UIEventArgs eArgs );
	void											OnGeneralButtonClick( UIEventArgs eArgs );
	void											OnInterfaceButtonClick( UIEventArgs eArgs );

	void											OnCloseButtonClick( UIEventArgs eArgs );
	void											OnSaveButtonClick( UIEventArgs eArgs );
	void											OnResetButtonClick( UIEventArgs eArgs );

	void											OnMenuButtonClick( UIEventArgs eArgs );
	void											OnWorldMapClick( UIEventArgs eArgs );

	void											OnLoginButtonClick( UIEventArgs e );

	void											BuildWindow1();
	void											BuildWindow1Video();
	void											BuildWindow1Audio();
	void											BuildWindow1General();
	void											BuildWindow1Interface();

	UI::List_ptr									GetListMain();

	UI::Window_ptr									GetWindow( int iID );

protected:
	UI::Window_ptr									pWindow1 = NULL;
	UI::Window_ptr									pWindowLogin = NULL;

	UI::Window_ptr									pWindowMenu = NULL;

	const int										iWindowSettingsWidth  = 560;
	const int										iWindowSettingsHeight = 450;


};

