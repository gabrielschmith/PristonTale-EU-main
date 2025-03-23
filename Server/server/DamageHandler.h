#pragma once

/// <summary>
/// Pointers to variable related to skills in ASM (Server.exe)
/// </summary>
enum class ESkillsVarsASM : DWORD
{
	//SKILLS_CODE_TEMP = 0x7ACA048,  //dm_SkillCode_Temp - not used any more I think
	CRITICAL_BOOST	 = 0x8B80ACC,  //Critical damage
	CRITICAL_HIT	 = 0x8B81260,  //Was a critical hit
	ATTACK_DAMAGE	 = 0x8B8176C,  //Attack damage
};

class UxxxTarget;
class CDamageHandler
{
private:
	IMPFNC						pfnSetSkillRange			= 0x00578130;
	IMPFNC						pfnSingleDamageRangeHandler = 0x0057CB90;
	IMPFNC						pfnDamageRangeHandler		= 0x0057AFF0;
	IMPFNC						pfnDamageRangeUserHandler	= 0x0057A630;
	IMPFNC						pfnSkillRangeMonsterSet		= 0x005572E0;
	IMPFNC						pfnSendDamageBySocket		= 0x0041C7D0;
	IMPFNC						pfnDecodePacket				= 0x005798F0;
	IMPFNC						pfnSetSkillPlus				= 0x00577980;

	float						fLevelBasedPvpDamageScale = 1.0;
	float						fLevelBasedPvpAbsScale = 1.0;
	float						fGlobalDamageReduction = 1.0;
	float						fGlobalAbsReduction = 1.0;


	BOOL						bAreaDamage = FALSE;

	static BOOL					SkillRangeMonsterSet( UnitData * pcUnitData, UserData * pcTarget, int iRange, int iDamagePercentDecrease );
	static BOOL					DecodePacket ( UserData * lpPlayInfo, PacketSingleTargetSkillData * psPacketAttackDamageEx );
	static BOOL					SetSkillPlus ( UserData * lpPlayInfo, UnitData * lpChar, PacketAttackData * lpTransAttackData, PacketSingleTargetSkillData * lpTransAttackData2 );


	static DWORD				GetDamageChecksum ( PacketSingleTargetSkillData * psPacketAttackDamageEx );

	static BOOL					SendDamageBySocket( UnitData * pcThis, UnitData * pcUnitData, SocketData * pcSocketData, int iAttackID = 0, int iDamageAdd = 0, int iResistance = 0 );

public:
	CDamageHandler();
	virtual ~CDamageHandler();


	void						SetIsAreaDamage( BOOL b ) { bAreaDamage = b; }
	BOOL						IsAreaDamage() { return bAreaDamage; }

	int							GetAdjustedCriticalChance ( UserData * lpPlayInfo, UxxxTarget * p_Target, int Critical_Hit );

	static BOOL					SingleDamageRangeHandler( UserData * pcUserData, PacketAttackData * psSkillNormal, UnitData * psUnitData, int iType );
	static BOOL					DamageRangeHandler( UserData * pcUserData, PacketSkillDamageRange * psRangeData, int iMapID, int iSkillID );

	void						UserTick1s( User * pcUser );
	void						ClearExpiredSkillBuffs( User * pcUser, BOOL bForceExpiry );
	void						ClearExpiredSkillBuffs( UserData * pcUserData, BOOL bForceExpiry );
	void						ClearExpiredDebuffs( Unit * pcUnit );
	void						ClearExpiredDebuffs( UnitData * pcUnitData );

	int							GetResistanceAdjustedValue(UxxxTarget * p_Target, int value, enum EElementID eElementID );
	BOOL						SetStun( User * pcAttacker, int iDurationSec, UxxxTarget * p_Target);
	BOOL						ApplyPikeWind( User * pcAttacker, PacketSkillDamageRange * psRangeData, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyLightningStun( User * pcAttacker, EAttackProperty eAttackProperty, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyDistortion( User * pcAttacker, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyCurse( User * pcAttacker, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyFreezing( User * pcAttacker, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	int							ApplyBerserkerLifeSteal( UserData * pcUserData,  int damage, UxxxTarget * p_Target, BOOL bMultiTarget, ESkillID p_SkillID, int p_SkillCode);
	int							ApplyExtinction( EAttackState eAttackState, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode );
	int							ApplyLifeSteal( EAttackState eAttackState, LifeStealParam sLifeStealParam, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyPoison( User * pcAttacker, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyEnchantWeapon( User * pcAttacker, EAttackProperty eAttackProperty, UxxxTarget * p_Target, BOOL bMultiTarget, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyEnchantWeaponLightningAoE( User * pcAttacker, ElementalAttackSetting * sDoT, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyBurning( User * pcAttacker, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	BOOL						ApplyFrostJavelin( User * pcAttacker, EAttackProperty eAttackProperty, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	int							GetAdjustedCriticalDamageForUnit( UxxxTarget * p_Target, int dmg );
	BOOL						ApplyCriticalDamage (ESkillID eSkillID, int * piDmg, int iCritChance, int iCritDamageBoost, UserData * pcUserAttacker, UxxxTarget * p_Target, EAttackState sAttackState, EAttackProperty eAttackProperty, BOOL bMultiTargetSkill );
	BOOL						ApplyStun( User * pcAttacker, EAttackState eAttackState, ElementalAttackSetting * sSetting, UxxxTarget * p_Target, ESkillID p_SkillID, int p_SkillCode);
	void						ApplyMonsterTypeDamageBuffFromSkill( int * piDmg, UxxxTarget * p_Target, User * pcUser, int iSkillLevel1Based, ESkillID p_SkillID );
	void						ApplyPriorityAttacker(UxxxTarget * p_Target, UserData * pcAttacker, int iDamage, ESkillID p_SkillID, int p_SkillCode);
	void						AdjustDamageFromMonsterResistence( EAttackProperty property, int * piDmg, UxxxTarget * p_Target);
	void						AdjustDamageFromAbsorb( int * piDmg, UxxxTarget * p_Target);
	void						ConvertDamageToHPFromBeserker( User * pcUser, const int iDamage, BOOL bHalf );
	void						ApplyPartyHealFromHolyBody( User * pcUser, const int iDamage, BOOL bHalf );
	void						GrantStolenHealthToUser( User * pcUser, const int iHpStolen );
	void						ApplyPullBackAndTargetHook( ESkillID eSkillID, int iHookDurationSec, UxxxTarget * p_Target, UserData * pcUserData, Point3D * pcSkillPos );
	BOOL						ApplySkillFlinchAndPushback( UserData * pcUserData, BOOL bCritSuccess, int iFlinchChance, int iPushbackRange, UxxxTarget * p_Target, Point3D * pcSkillPos, EAttackState eAttackState, ESkillID p_SkillID, int skillCode);
	BOOL						ApplyFlinch( UserData * pcAttacker, UxxxTarget * p_Target, int iFlinchChance, ESkillID p_SkillID, int p_SkillCode, BOOL bForceFlinch = FALSE);
	BOOL						ApplyPushback(UxxxTarget * p_Target, Point3D * pcRefernecePos, int iPushbackRange, ESkillID p_SkillID, int p_SkillCode);

	BOOL						SingleTargetSkillDamageHandler (  User * pcCaster, PacketAttackDataEx * lpTransAttackData, PacketSingleTargetSkillDataEx * lpTransAttackDataEx, UxxxTarget * p_Target, int iType, ESkillID eSkillID );

	BOOL						MultiTargetSkillHandler ( User * pcCaster, PacketSkillDamageRange * psRangeData, PacketMultiTargetSkillData * psRangeDataEx, int iMapID, int iSkillID, ESkillID eSkillID );


	BOOL						HandlePacket( User * pcUser, PacketSingleTargetSkillData* psSkillsDataPacket );
	BOOL						HandlePacket( User * pcUser, PacketMultiTargetSkillData* psSkillsDataPacket );

	BOOL						UpdateBCPVPScoring ( UserData * pcKiller, UserData * pcVictim, int iDamage );

	BOOL						IsMonsterAreaDamage( UnitData * pcUnitData );

	int							GetPlayerAttackChancePVP( int iAttackerAttackRating, int iTargetDefenseRating );

	BOOL						SendDamagePvPRaw(User* p_Attacker, User* p_Target, int p_Damage, int p_AttackRating, EAttackState p_State, ESkillID p_SkillID, BOOL bCritical, int16_t p_Param1 = 0, int16_t p_Param2 = 0);
	BOOL						SendDamagePvPData(UserData* p_Attacker, UserData* p_Target, int p_Damage, int p_AttackRating, EAttackState p_State, ESkillID p_SkillID, BOOL bCritical, int16_t p_Param1 = 0, int16_t p_Param2 = 0);

	BOOL						RefreshPvPDamageBuff(UserData* p_Attacker, UserData* p_Target, int p_Damage, EAttackState p_State, ESkillID p_SkillID, PvPDamageBuffSlot p_Slot, int p_RepeatCount, int p_Interval, int16_t p_Param1 = 0, int16_t p_Param2 = 0);
	BOOL						UpdateDamagePvPDamageBuffs( User * pcUser );


	int							RecordCharDamage ( UserData * lpPlayInfo, UnitData * lpChar, int Damage );

	void						SendCancelSkill( User * pcUser, int iPlayID );

	BOOL						CanHitTargetAreaDamage( UserData * pcUserData, UnitData * pcUnitData );

	void						SetNewPVPLevelBasedDamageScale(float f) { fLevelBasedPvpDamageScale = f; }
	void						SetNewPVPLevelBasedAbsScale(float f) { fLevelBasedPvpAbsScale = f; }
	float						GetNewPVPLevelBasedDamageScale() { return fLevelBasedPvpDamageScale; }
	float						GetNewPVPLevelBasedAbsScale() { return fLevelBasedPvpAbsScale; }

	void						SetNewPVPGlobalDamageReduction(float f) { fGlobalDamageReduction = f; }
	void						SetNewPVPGlobalAbsReduction(float f) { fGlobalAbsReduction = f; }
	float						GetNewPVPGlobalDamageReduction() { return fGlobalDamageReduction; }
	float						GetNewPVPGlobalAbsReduction() { return fGlobalAbsReduction; }

	void						UpdatePVPScalingIni(char* iniSection, char* iniKey, double iniValue);
	void						UpdatePVPScalingIni(char* iniSection, char* iniKey, char* iniValue);

	void 						LoadNewPVPScalingSettings();
};

