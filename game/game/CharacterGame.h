#pragma once

#define SAVE									CharacterGame::SaveTimer()
#define SAVE									CharacterGame::SaveCustom()
#define SAVEEX									CharacterGame::SaveEx()

#define UPDATE_CHARACTER_CHECKSUM				CharacterGame::ChecksumUpdate( &UNITDATA->sCharacterData )
#define CHECK_CHARACTER_CHECKSUM				CharacterGame::ChecksumCheck()

#define MAP_ID									CharacterGame::GetStageID()

#define CHARACTER_PLAYCOUNTER					(*(DWORD*)0x00CF4824)
#define CHARACTER_LASTPLAYCOUNTER				(*(DWORD*)0x03A3E3BC)
#define CHARACTER_LASTSENDPLAYCOUNTER			(*(DWORD*)0x03A9777C)
#define CHARACTER_LASTTIMESENDPLAYDATA			(*(DWORD*)0x03A97780)
#define CHARACTER_VISIBLECOUNT					(*(DWORD*)0x03A97664)
#define CHARACTER_SKILLCHARGINGFLAG				(*(DWORD*)0x00A1CCD8)
#define CHARACTER_SKILLEVENTDAMAGECOUNT			(*(DWORD*)0x00A1CCC8)



class CharacterGame
{
protected:
	DWORD										dwLastItemCode = 0;

	int											iLastMapID = -1;

	int											iLastFocusID = -1;

	int											iCurrentWeaponID = 0;

	DWORD										dwLastCombatDataSaveTime = 0;
	PacketCharacterCombatData					sCharacterCombatData;

private:
	static const int							iMaxRunSpeed = (((25 * 10) + 250) * 460) >> 8; // Run fMovementSpeed = 25

	static const DWORD							pfnSave				= 0x00627D20; //SaveGameData
	static const DWORD							pfnChecksumUpdate	= 0x0045BC20;

	IMPFNC										pfnResetAllStats	= 0x00497B20;
	IMPFNC										pfnResetStats		= 0x00497E60;

	//Phoenix Pet
	const DWORD									pbShowPhoenixPet	= 0x0362B590;
	const DWORD									pfnSetPhoenix		= 0x004D65B0;
	const DWORD									pfnDeletePhoenix	= 0x004D6640;
	const DWORD									pcPhoenix			= 0x0362B388;
	const DWORD									piActivePhoenix		= pcPhoenix + 0x204;

	const DWORD									pdwMPDownTime		= 0x035D0C48;
	const DWORD									pdwMPDownPercent	= 0x035D0C4C;
	const DWORD									pdwSPDownTime		= 0x035D0C60;
	const DWORD									pdwSPDownPercent	= 0x035D0C64;

	INT64										ExpXor();

public:
	BOOL										bMPDown;
	BOOL										bSPDown;
	BOOL										bHPUp;
	BOOL										bMPUp;
	BOOL										bTopPVP;
	BOOL										bHoneyDamage;
	BOOL										bHoneyFast;
	BOOL										bHoneySmart;
	BOOL										bHoneyDefense;
	BOOL										bWeightStone;
	BOOL										bPhoenixFireSmall;
	BOOL										bPhoenixIceSmall;
	BOOL										bPhoenixHealingSmall;
	BOOL										bPhoenixLightningSmall;

	BOOL										bSHPRegen;
	BOOL										bSSPRegen;
	BOOL										bSMPRegen;
	BOOL										bSMPDown;
	BOOL										bSSPDown;
	BOOL										bSBonusDamage;
	BOOL										bSBonusAbsorb;
	BOOL										bSMoveSpeed;

	BOOL										bIceResistanceBuff;


	DWORD										dwForceOrbTotalDuration;
	DWORD										dwForceOrbTimeRemainingAtCity;
	DWORD										dwForceOrbTimeRemaining;

	CharacterDataEx								sCharacterDataEx;

												CharacterGame();
	virtual									   ~CharacterGame();

	void										Tick1s();
	void										HandlePacket( Packet * psPacket );
	void										OnCharacterUpdateData();
	void										OnCharacterSetCharInfoEx();
	BOOL										IsStageVillage();
	static int									GetStageID();
	int											GetLastStageID() { return iLastMapID; };
	void										SetLastStageID( int i ) { iLastMapID = i; };


	int											GetBlockRating( int desLV );
	int											GetEvadeRating( int desLV );

	int											GetCurrentMP();
	int											GetCurrentHP();
	int											GetCurrentSP();

	void										GiveHPValuePercent( int iPercent );
	void										GiveHPValue( int iValue );
	void										GiveSPValuePercent( int iPercent );
	void										GiveSPValue( int iValue );
	void										RemoveHPValue( int iValue );
	void										ApplyHPReduction( int iPercent );

	void										SetCurrentMP( int iMP );
	void										SetCurrentHP( int iHP );
	void										SetCurrentSP( int iSP );

	int											GetFreeSkillPointForTier5() { return sCharacterDataEx.iFSP; }
	void										SetFreeSkillPointForTier5( int i ) { sCharacterDataEx.iFSP = i; }

	void										SetPhoenix( BOOL bActive, int iPhoenixID = 0 );

	void										Die();

	static void									SaveTimer ();
	static void									SaveCustom ();
	static void									Save();

	static void									SaveEx();
	void										SyncCombatData();

	int											MakePlayBuffFromRecvData(UnitData* pcUnitData, PlayBuff* StartBuff, PlayBuff* EndBuff, int len);

	static BOOL									ChecksumUpdate( CharacterData * psCharacterData );
	static BOOL __cdecl							ChecksumCheck();

	void										UpdateWeight();
	void										ReFormMousePotionNum();
	BOOL										CheckEquipmentSpace( ItemData * psItemData );
	BOOL										CheckInventorySpace( ItemData * psItemData, BOOL bUseTemporaryItem = TRUE );
	BOOL										CheckInventoryWeight( ItemData * psItemData );

	int											OnMotionChange( UnitData * pcUnitGame, int iMotion, int iAnimationID );

	void										RemoveHairCrown();
	void										ResetHairCrown();
	void										ResetHead( BOOL bDefault = FALSE );

	void										PHDamage( struct PacketAttackData * psPacket );
	BOOL										XorAttackTrans( UnitData* pcUnitData, PacketAttackData* lpTransAttackData );
	DWORD										GetAttackTrans_XorCode( UnitData* unitData );
	DWORD										EncodeDamagePacket( PacketAttackData * lpTransAttackData );
	DWORD										GetSpeedSum( char * szName );
	DWORD										GetDamgeChkSum_S2V(PacketAttackData* psPacket);
	DWORD										GetDamgeChkSum_S2V( PacketSingleTargetSkillData * psPacket );
	BOOL										PHTeleport( struct PacketTeleportEvent * psPacket );

	void										CancelAttack();

	void										OnUseManaPotion( int iMP );

	void										OnUseHPPotion( int iHP );

	BOOL										CanUseGold( int iGold );

	void										ResetStats( int iNum );

	void										FocusTargetPet( int iID );

	void										HandlePacket( PacketUpdatePetDataEx * psPacket );
	void										HandlePacket( PacketUnitInfo * psPacket );
	void										HandlePacket( PacketCharacterDataEx * psPacket );
	void										HandlePacket( PacketUpdateIntegrity * psPacket );

	int 										GetAttackRatingFromEquippedItems ( );
	BOOL										IsLeftSlotEquippped ( enum EItemType eItemType );
	BOOL										IsRightSlotEquippped ( enum EItemType eItemType );

	void										SetEXP( INT64 iEXP );

	INT64										GetEXP();

	int											GetCanCarryGold();
	BOOL										CanCarryGold( int iGold );

	DWORD										GetCharacterColorByClass( int iClass );

	void										UpdateCharacterPlayData();
};

