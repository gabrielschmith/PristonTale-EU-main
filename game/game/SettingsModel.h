#pragma once



class CSettingsModel
{
private:
	struct sSettings
	{
		BOOL											bWindowed;
		BOOL											bWidescreen;
		BOOL											bAutoAdjust;
		DWORD											dwWidth;
		DWORD											dwHeight;

		DWORD											dwBitDepth;
		BOOL											bVSync;
		BOOL											bHighTextureQuality;
		BOOL											bNoEffects;

		BOOL											bFarCameraSight;
		BOOL											bInvertedCamera;
		BOOL											bARPGCamera;

		BOOL											bNoSoundEffects;
		BOOL											bNoMusic;
		int												iMusicVolume;
		int												iEffectVolume;
		BYTE											bWeaponEnchantPerference;

		char											szaMacro[10][100];
		char											szLastAccount[32];

		BOOL											bNoTradeChat;
		BOOL											bNoWhisper;
		BOOL											bNoTradeRequest;
		BOOL											bNoPartyRequest;

		BOOL											bDebugDamage;
		BOOL											bHPBarOnTarget;
		BOOL											bHPBarOnSelf;
		BOOL											bFPS;
		BOOL											bPing;
		BOOL											bDebugKill;
		BOOL											bDefaultPetModeAggressive;
		BOOL											bChatBoxRemoveFocusOnEnter;
		BOOL											bAutoSelectNearestTargetForAoE;
		BOOL											bItemViewTogglableA;

		BOOL											bStartMaximized;
		BOOL											bAntiAliasing;
		int												iCameraRange;

		BOOL											bEnableEquipmentMinLevelFilter;
		BOOL											bEnableLootFilter;
		UINT											iLootFilterFlags;
		UINT											iLootFilterSpecFlags;

		BOOL											bLockUI;
		BOOL											bSaveUI;
		BOOL											bShowPartyMembers;
		BOOL											bShowClanMembers;
		BOOL											bShowEnemies;
		BOOL											bShowNPCs;
		//BOOL											bMinimapAlwaysActive;
		BOOL											bShowRaidMembers;
		BOOL											bShowNextBossTimer;
		BOOL											bShowNameAboveHead;
		BOOL											bShowFrameAboveHead;
		BOOL											bShowNotice;
		BOOL											bTargetDistance;
		BOOL											bTargetHPValue;
		BOOL											bTargetHPPercent;
		BOOL											bCustomZoom;
		int												iZoomLevel;
		BOOL											bCustomZoomTown;
		int												iZoomTownLevel;

		int												iMinEquipmentLevelReq = 0;
	};
	sSettings											sSettingsData;
	sSettings											sOldSettingsData;

	BOOL												bMaximizedWindow = FALSE;

	BOOL												bFirstRead = FALSE;

public:
	CSettingsModel();
	virtual ~CSettingsModel();

	void												OnCheckWindowState();

	BOOL												IsMaximizedWindow() { return bMaximizedWindow; }


	sSettings &											GetSettings();
	sSettings &											GetOldSettings();

	BOOL												IsLootFiltersChanged();
	BOOL												ReadSettings();
	void												WriteSettings();

	BOOL												CanViewLoot( ELootFilter e );
	void												ResetViewLoot();
	void												RemoveViewLoot( ELootFilter e );
	void												AddViewLoot( ELootFilter e );
	void												SetViewLoot( ELootFilter e, BOOL b );

	BOOL												CanViewLootSpec( ELootFilterSpec e );
	void												RemoveViewLootSpec( ELootFilterSpec e );
	void												AddViewLootSpec( ELootFilterSpec e );
	void												SetViewLootSpec( ELootFilterSpec e, BOOL b );
	BOOL												HaveViewLootSpec();

	void												ProcessGeneralSettings( BOOL bRead = FALSE, BOOL bInitial = FALSE );

	void												Init();
};

