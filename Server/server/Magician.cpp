#include "stdafx.h"
#include "Magician.h"
#include "HNSSkill.h"

typedef UnitData*( __cdecl *t_SummonMobSkillMG ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillMG SummonMobSkillMG = ( t_SummonMobSkillMG )0x0055E5A0;


CMagician::CMagician()
{
}


/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CMagician::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Magician )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CMagician::SendServerSkillDataToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Magician )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CMagician::UpdateAndCacheServerSkillInfo ()
{

	INFO( "Updating and caching skill info for Magician.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Magician );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CMagician::UpdateAndCacheServerSkillData ()
{

	INFO( "Updating and caching skill data for Magician.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Magician;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );
}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CMagician::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Agony,			Magician_Agony_HPTaken,						"AbsorbLife");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Agony,			Magician_Agony_MPAdded,						"MP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Agony,			Magician_Agony_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBolt,		Magician_FireBolt_DMGBoost,					"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBolt,		Magician_FireBolt_BurningChance,			"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBolt,		Magician_FireBolt_BurningDamagePercent,		"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBolt,		Magician_FireBolt_BurningDurationSec,		"BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBolt,		Magician_FireBolt_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Zenith,			Magician_Zenith_AddAbsorb,					"Absorption");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Zenith,			Magician_Zenith_AddAbsorbPercent,			"AbsorptionPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Zenith,			Magician_Zenith_AddElement,					"ElementalResistance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Zenith,			Magician_Zenith_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Zenith,			Magician_Zenith_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_Area,						"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_Range_NotUsed,			"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_BurningChance,			"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_BurningDamagePercent,		"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_BurningDurationSec,		"BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_DMGBoostPercent,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireBall,		Magician_FireBall_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MentalMastery,	Magician_MentalMastery_MPPotionBoost,		"MPPotionPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MentalMastery,	Magician_MentalMastery_MPBoost,				"MPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MentalMastery,	Magician_MentalMastery_EW_Chain_MPDrain_New, "MPDrain"); //for Energy shield chain only

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Watornado,		Magician_Watornado_Area,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Watornado,		Magician_Watornado_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Watornado,		Magician_Watornado_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Watornado,		Magician_Watornado_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DeathRay,		Magician_DeathRay_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DeathRay,		Magician_DeathRay_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DeathRay,		Magician_DeathRay_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnergyShield,	Magician_EnergyShield_ReduceDamage,			"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnergyShield,	Magician_EnergyShield_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnergyShield,	Magician_EnergyShield_MPDrain_New,			"MPDrain");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnergyShield,	Magician_EnergyShield_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Diastrophism,	Magician_Diastrophism_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Diastrophism,	Magician_Diastrophism_Area,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Diastrophism,	Magician_Diastrophism_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritElemental, Magician_SpiritElemental_DamageBoostMin,	"AttackPowerMinPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritElemental, Magician_SpiritElemental_DamageBoostMax,	"AttackPowerMaxPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritElemental, Magician_SpiritElemental_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritElemental, Magician_SpiritElemental_UseMana,			"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_DamagePercentOfAP,	"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_AtkDelay,				"AttackDelay");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_UseMana,				"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_FireBurningChance_New,		"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_FireBurningDamagePercent_New,	"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_FireBurningDurationSec_New,	"BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_IceFreezeChance_New,			"FreezeChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_IceFreezeAPPercent,			"FreezeAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_IceFreezeDuration_New,		"FreezeDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DancingSword,	Magician_DancingSword_IceFreezeSpeedReduction_New,	"SpeedReduction");


		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_ATKPowMin,			"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_ATKPowMax,			"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_AtkRating,			"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_HP,					"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireElemental,	Magician_FireElemental_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_Area,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_BurningChance,			"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_BurningDamagePercent,	"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_BurningDurationSec,		"BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FlameWave,		Magician_FlameWave_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_DamagePercent,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_Chance,					"Chance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_SpeedReduction,			"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_DamageReduction,		"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_Area,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Distortion,		Magician_Distortion_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Meteorite,		Magician_Meteorite_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Meteorite,		Magician_Meteorite_Area,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Meteorite,		Magician_Meteorite_UseMana,					"MPCost");
	}

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_FireBurningChance,		"BurningChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_FireBurningDamagePercent,"BurningAtkPowPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_FireBurningDuration,		"BurningDurationSec");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_LightningStunChance,		"StunChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_LightningStunAPPercent,	"StunAtkPowPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_LightningStunDuration,	"StunDurationSec");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_IceFreezeChance,			"FreezeChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_IceFreezeAPPercent,		"FreezeAtkPowPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_IceFreezeDuration,		"FreezeDurationSec");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_IceFreezeSpeedReduction,	"SpeedReduction");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_LightningRange_New,		"AttackRange");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_Duration,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EnchantWeapon,		Magician_EnchantWeapon_UseMana,					"MPCost");

	if ( temp->iCount > 0 )
	{
		if ( container->iCount + temp->iCount > MAX_SKILL_ARRAY_DATA )
		{
			//Likely because there is many skills data with sync flag of 1. Use
			//skills_update_everyone
			LOGERROR( "Skill data count count extended the max allowed count of %d!", MAX_SKILL_ARRAY_DATA );
		}
		else
		{
			CopyMemory ( &container->skillArrayData[container->iCount], temp->skillArrayData, temp->iCount * sizeof ( SkillArrayData ) );
			container->iCount += temp->iCount;
			container->iLength = sizeof ( Packet ) + 4 + 4 + (container->iCount * sizeof ( SkillArrayData ) );
		}
	}

	SAFE_DELETE ( temp );
}


int CMagician::UseDistortionMonster( User * pcUser, PacketSkillDamageRange * psPacket, int iLevelSkill )
{
	UserData * pcUserData = pcUser->pcUserData;

	int iSpeedDB = GetSkillIntValueAt ( pcUser, Magician_Distortion_SpeedReduction, iLevelSkill );

	int iSpeed = SLOW_SPEED_BASE - ( ( SLOW_SPEED_BASE * iSpeedDB ) / 100 );
	int iDamage = GetSkillIntValueAt ( pcUser, Magician_Distortion_DamageReduction, iLevelSkill ) * 17;


	return TRUE;
}

int CMagician::UseDistortionUser( User * pcUser, int iTimeByLevel )
{
	if ( pcUser == NULL || pcUser->IsValid () == FALSE )
		return FALSE;

	UserData * pcUserData = pcUser->pcUserData;

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ps = USERSDATA + i;
		if ( ps && ps->pcSocketData && ps->sCharacterData.iType && ps->iID && ps != pcUserData )
		{
			if ( pcUserData->iClanID && ps->iClanID && pcUserData->iClanID == ps->iClanID )
				continue;

			int iX			= (ps->sPosition.iX - pcUserData->sPosition.iX) >> 8;
			int iZ			= (ps->sPosition.iZ - pcUserData->sPosition.iZ) >> 8;
			int iDistance	= abs((iX*iX) + (iZ*iZ));

			int iResistance = ps->sCharacterData.sElementalDef[EElementID::Organic];
			int iTime = (((100 - iResistance) * iTimeByLevel) / 100) * 70;

			int iID = pcUserData->sCharacterData.iClass == CHARACTERCLASS_Magician ? 0x4C : 0x1C;

			if ( abs( iX ) <= (27 * 15) && abs( iZ ) <= (27 * 15) )
			{
				CHATSERVER->SendDebugChat( pcUser, EChatColor::CHATCOLOR_Error, "> Distorted[%s] Time[%d] Res[%d]",
					ps->sCharacterData.szName, iTime, iResistance );
				__asm
				{
					PUSH iTime;
					MOV EAX, iID;
					NEG EAX;
					PUSH EAX;
					PUSH 0x80;
					PUSH 1;
					PUSH ps;
					PUSH pcUserData;
					MOV EAX, 0x00401FF4;
					CALL EAX;
					ADD ESP, 0x18;
				}
			}
		}
	}

	return 0;
}

int CMagician::GetSpiritElementalAttackPowerBoost( User * pcUser, int TypeDamage )
{
	UserData * pcUserData = pcUser->pcUserData;

	UINT uTime = pcUserData->dwSpiritElementalEndTime;
	if ( uTime >= TICKCOUNT )
	{
		return pcUserData->sSpiritElementalBoost.sMin;
	}
	return 0;
}

int CMagician::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill1Based = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Magician Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketEx->iSkillCode );
		return 0;
	}

	int attackPower = *piAttackPower;

	int value = 0;
	int chain, boost, level;

	switch ( uSkillID )
	{
		case MagicianSkill::DefaultMagicianAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

		} break;

		case MagicianSkill::FireBolt:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fire Bolt]" );
			value = GetSkillIntValueAt ( pcUser, Magician_FireBolt_DMGBoost, iLevelSkill1Based );
			attackPower += value;

			psPacketEx->iAttackProperty = EAttackProperty::Fire;

			psPacketEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBolt_BurningDamagePercent, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBolt_BurningDurationSec, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBolt_BurningChance, iLevelSkill1Based );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical: [%d] Attack Rating: [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", attackPower, value, attackPower );

		} break;

		case MagicianSkill::DeathRay:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Death Ray]" );
			value = GetSkillIntValueAt ( pcUser, Magician_DeathRay_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case MagicianSkill::DancingSword:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Dancing Sword]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			if ( pcUser->uDancingSwordEndTime > TIME )
			{
				level = pcUser->bDancingSwordLevel;

				//value = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_DamagePercentOfAP, level  );
				//boost = ( attackPower * value ) / 100;
				//attackPower += boost;

				//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", attackPower, boost, attackPower );

				chain = pcUser->bDancingSwordChainLevelDR;
				if ( chain )
				{
					value = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DeathRay_DMGBoost, chain );
					boost = ( value * attackPower ) / 100;
					attackPower += boost;

					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Death Ray Chain: [Lv. %d] [AP: +%d%%]", chain, value);
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Boost [%d] Final [%d]", boost, attackPower );
				}



				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> EW Chain [Lv. %d] [AP: +%d%%]", chain);

				if ( pcUser->eDancingSwordElement == EElementID::Fire )
				{
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Fire Damage Type" );

					psPacketEx->iAttackProperty = EAttackProperty::Fire;

					psPacketEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_FireBurningDamagePercent_New, level );
					psPacketEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_FireBurningDurationSec_New, level );
					psPacketEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_FireBurningChance_New, level );

					chain = pcUser->bDancingSwordChainLevelEW;
					if ( chain )
					{
						psPacketEx->sElementalAttackSetting.sPercentOfAP += GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_EnchantWeapon_FireBurningDamagePercent, chain, TRUE  );
					}
				}
				else
				{

					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Ice Damage Type" );

					psPacketEx->iAttackProperty = EAttackProperty::Ice;

					psPacketEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_IceFreezeAPPercent, level  );
					psPacketEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_IceFreezeDuration_New, level  );
					psPacketEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_IceFreezeChance_New, level );
					psPacketEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_DancingSword_IceFreezeSpeedReduction_New, level );

					chain = pcUser->bDancingSwordChainLevelEW;
					if ( chain )
					{
						psPacketEx->sElementalAttackSetting.sPercentOfAP += GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_EnchantWeapon_IceFreezeAPPercent, chain, TRUE  );
					}
				}
			}
			else
			{
				attackPower = 0; //dancing sword had expired.
			}
		} break;

		default:
			break;
	}

	*piAttackPower = attackPower;

	return 1;
}

int CMagician::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill1Based = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;


	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Magician Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketRangeEx->iSkillID );
		return 0;
	}

	int attackPower = psPacketRange->iDamage;

	int level, flat, boost, value, percent;

	switch ( uSkillID )
	{
		case MagicianSkill::FireBall:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fire Ball]" ); // yes its a bug, needs to be fixed xD

			value = GetSkillIntValueAt ( pcUser, Magician_FireBall_Area, iLevelSkill1Based );
			psPacketRange->iRange = value;
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range: [%d] Attack Rating: [%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );

			percent = GetSkillIntValueAt ( pcUser, Magician_FireBall_DMGBoostPercent, iLevelSkill1Based );
			boost = ( ( psPacketRange->iDamage * percent ) / 100 );
			psPacketRange->iDamage += boost;


			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBall_BurningDamagePercent, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBall_BurningDurationSec, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FireBall_BurningChance, iLevelSkill1Based );

			psPacketRange->iAttackProperty = EAttackProperty::Fire;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Fire Ball [+%d%%]", percent );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", attackPower, boost, psPacketRange->iDamage );

			level = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_FireBolt );
			if ( level )
			{
				flat = GetSkillIntValueAt ( pcUser, Magician_FireBolt_DMGBoost, level, TRUE  );

				value = psPacketRange->iDamage;
				boost = flat;
				psPacketRange->iDamage += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Fire bolt chain (Lv. %d) [+%d]", level, flat );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", value, boost, psPacketRange->iDamage );
			}

		} break;

		case MagicianSkill::Watornado:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Watornado]" );
			value = GetSkillIntValueAt ( pcUser, Magician_Watornado_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			value = GetSkillIntValueAt ( pcUser, Magician_Watornado_Area, iLevelSkill1Based );
			psPacketRange->iRange = value;
			psPacketRange->eAttackState = EAttackState::State_104_Water;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case MagicianSkill::Diastrophism:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Diastrophism]" );
			value = GetSkillIntValueAt ( pcUser, Magician_Diastrophism_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			value = GetSkillIntValueAt ( pcUser, Magician_Diastrophism_Area, iLevelSkill1Based );
			psPacketRange->iRange = value;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			psPacketRange->iAttackProperty = EAttackProperty::Earth;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case MagicianSkill::FlameWave:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Flame Wave]" );

			value = GetSkillIntValueAt ( pcUser, Magician_FlameWave_DMGBoost, iLevelSkill1Based );
			boost = ( ( psPacketRange->iDamage * value ) / 100 );
			psPacketRange->iDamage += boost;
			psPacketRange->iRange = 340;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", attackPower, boost, psPacketRange->iDamage );

			level = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_FireBolt );
			if ( level )
			{
				flat = GetSkillIntValueAt ( pcUser, Magician_FireBolt_DMGBoost, level, TRUE  );

				value = psPacketRange->iDamage;
				boost = flat;
				psPacketRange->iDamage += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Fire bolt chain (Lv. %d) [+%d]", level, flat );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", value, boost, psPacketRange->iDamage );
			}


			level = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_FireBall );
			if ( level )
			{
				percent = GetSkillIntValueAt ( pcUser, Magician_FireBall_DMGBoostPercent, level, TRUE  );

				value = psPacketRange->iDamage;
				boost = ( ( psPacketRange->iDamage * percent ) / 100 );
				psPacketRange->iDamage += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Fire ball chain (Lv. %d) [+%d%%]", level, percent );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: [%d] + [%d] = [%d]", value, boost, psPacketRange->iDamage );
			}

			value = GetSkillIntValueAt ( pcUser, Magician_FlameWave_Area, iLevelSkill1Based );
			psPacketRange->iRange = value;
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			psPacketRange->iAttackProperty = EAttackProperty::Fire;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );


			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FlameWave_BurningDamagePercent, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FlameWave_BurningChance, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec =  GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_FlameWave_BurningDurationSec, iLevelSkill1Based );

		} break;

		case MagicianSkill::EnchantWeapon: //lightning only
		{
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Enchant Weapon]" );
			//
			//int iValueMin = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_Ligh, iLevelSkill1Based );
			//int iValueMax = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightDamagePercent, iLevelSkill1Based );
			//
			//value = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightningRange, iLevelSkill1Based );
			//
			//psPacketRange->iDamage = Dice::RandomI ( iValueMin, iValueMax );
			//psPacketRange->iRange = value;
			//psPacketRange->eAttackState = EAttackState::State_105_Electricity;
			//
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case MagicianSkill::Meteorite:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Meteorite]" );

			value = GetSkillIntValueAt ( pcUser, Magician_Meteorite_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );

			value = GetSkillIntValueAt ( pcUser, Magician_Meteorite_Area, iLevelSkill1Based );
			psPacketRange->iRange = value;

			psPacketRange->iAttackProperty = EAttackProperty::Earth;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			//only apply on first Meteorite
			if ( sActiveSkill.sParam == 1 )
			{
				level = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_Distortion );
				if ( level )
				{
					psPacketRangeEx->eSecondaryAttackState = EAttackState::State_7_Distortion;
					psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_DamagePercent, level, TRUE  );
					psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_Chance, level, TRUE  );
					psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_Duration, level, TRUE  );
					psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_SpeedReduction, level, TRUE  );
					psPacketRangeEx->sElementalAttackSetting.sDamageReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_DamageReduction, level, TRUE  );

					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Distortion chain (Lv. %d)", level );
				}
			}

		} break;

		case MagicianSkill::Distortion:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Distortion]" );


			psPacketRange->iDamage = 0;
			psPacketRange->iRange = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_Area, iLevelSkill1Based );
			psPacketRange->eAttackState = EAttackState::State_7_Distortion;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage[%d] Range [%d]", psPacketRange->iDamage, psPacketRange->iRange );

			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_DamagePercent, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_Chance, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_Duration, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_SpeedReduction, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.sDamageReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Magician_Distortion_DamageReduction, iLevelSkill1Based );

		} break;

		default:
			break;
	}

	psPacketRange->iCount &= 0x000000FF;

	return 1;
}

int CMagician::RecvBuffSkill( User * pcUser, PacketSkillCast * packet )
{
	UserData * pcUserData = pcUser->pcUserData;

	int iLevelSkill1Based = ( packet->iSkillCode >> 8 ) & 0xFF;
	int iSkillTinyID = packet->iSkillCode & 0xFF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Target: %d, Param1: %d, Param2: %d", iSkillTinyID, iLevelSkill1Based, packet->iTargetID, packet->iParam1, packet->iParam2 );

	if ( iLevelSkill1Based < 1 || iLevelSkill1Based > 10 )
	{
		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> iSkillLevel is not 1-10 ?? (%d)", iLevelSkill1Based );
		return TRUE;
	}

	int chainLevel;
	int value, boost;

	switch ( iSkillTinyID )
	{
		case MagicianSkill::EnchantWeapon:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Enchant Weapon]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Type = %d", packet->iParam2 );

			OnEnchantWeaponCasted( pcUser, packet, iLevelSkill1Based );
		} break;

		case MagicianSkill::EnergyShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Energy Shield]" );
			int iDuration = GetSkillIntValueAt( pcUser, Magician_EnergyShield_Duration, iLevelSkill1Based );
			pcUserData->dwEnergyShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			value = GetSkillIntValueAt( pcUser, Magician_EnergyShield_ReduceDamage, iLevelSkill1Based );
			pcUserData->iEnergyShieldDamageReduce = value;
		} break;

		case MagicianSkill::SpiritElemental:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spirit Elemental]" );
			int iDuration = GetSkillIntValueAt( pcUser, Magician_SpiritElemental_Duration, iLevelSkill1Based );
			pcUserData->dwSpiritElementalEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			pcUserData->sSpiritElementalBoost.sMin = GetSkillIntValueAt( pcUser, Magician_SpiritElemental_DamageBoostMin, iLevelSkill1Based );
			pcUserData->sSpiritElementalBoost.sMax = GetSkillIntValueAt( pcUser, Magician_SpiritElemental_DamageBoostMax, iLevelSkill1Based );
		} break;

		case MagicianSkill::DancingSword:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Dancing Sword][Buff]" );
			int iDuration = GetSkillIntValueAt( pcUser, Magician_DancingSword_Duration, iLevelSkill1Based );

			//fire
			if ( ( packet->iParam1 & 0xF ) == 1 )
			{
				pcUser->eDancingSwordElement = EElementID::Fire;
			}
			else //ice
			{
				pcUser->eDancingSwordElement = EElementID::Ice;
			}

			pcUser->bDancingSwordLevel = iLevelSkill1Based;
			pcUser->uDancingSwordEndTime = TIME + iDuration;
			pcUser->bDancingSwordChainLevelEW = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_EnchantWeapon );
			pcUser->bDancingSwordChainLevelDR = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_DeathRay );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Level: %d, Type: %d", iLevelSkill1Based, packet->iParam1 );
		} break;

		case MagicianSkill::FireElemental:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fire Elemental]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillMG( 0x8D, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				value = GetSkillIntValueAt ( pcUser, Magician_FireElemental_Duration, iLevelSkill1Based );
				pcUnitDataPet->dwUpdateCharInfoTime = ( value * 1000 ) + TICKCOUNT;

				value = GetSkillIntValueAt ( pcUser, Magician_FireElemental_ATKPowMin, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMinDamage = value;

				value = GetSkillIntValueAt ( pcUser, Magician_FireElemental_ATKPowMax, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMaxDamage = value;

				chainLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_SpiritElemental );

				if ( chainLevel > 0 )
				{
					value = GetSkillIntValueAt ( pcUser, Magician_SpiritElemental_DamageBoostMin, chainLevel, TRUE );
					boost = ( value * pcUnitDataPet->sCharacterData.iMinDamage ) / 100;
					pcUnitDataPet->sCharacterData.iMinDamage += boost;

					value = GetSkillIntValueAt ( pcUser, Magician_SpiritElemental_DamageBoostMax, chainLevel, TRUE );
					boost = ( value * pcUnitDataPet->sCharacterData.iMaxDamage ) / 100;
					pcUnitDataPet->sCharacterData.iMaxDamage += boost;
				}

				value = GetSkillIntValueAt ( pcUser,Magician_FireElemental_AtkRating, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iAttackRating = value;

				value = GetSkillIntValueAt ( pcUser, Magician_FireElemental_HP, iLevelSkill1Based );
				pcUnitDataPet->InitHealthPoint( value );

				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill1Based * 12;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;

				pcUnitDataPet->PartyFlag = 0x200;
			}
		} break;

		case MagicianSkill::WizardTrance:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Wizard Trance]" );

			pcUser->uWizardTranceLevel = iLevelSkill1Based;
			pcUser->dwWizardTranceTimeOut = TICKCOUNT + (300 * 1000);

			PacketUseSkillSingle s;
			s.iLength = sizeof( PacketUseSkillSingle );
			s.iHeader = PKTHDR_UseSkillNew;
			s.iSkillID = SKILLID_WizardTrance;
			s.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Magician;
			s.iSkillLevel1Based = pcUser->uWizardTranceLevel;
			s.lCasterID = pcUser->GetID();
			s.lTargetID = pcUser->GetID();
			SENDPACKETRANGE( &s, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL );
		} break;

		default:
			break;
	}
	return 1;
}


BOOL CMagician::OnEnchantWeaponCasted ( User * pcCasterUser, PacketSkillCast * packet, int iLevelSkill1Based )
{
	if ( pcCasterUser == NULL || pcCasterUser->IsValidAndInGame() == FALSE )
		return FALSE;

	int duration = GetSkillIntValueAt( pcCasterUser, Magician_EnchantWeapon_Duration, iLevelSkill1Based );

	EElementID element = EElementID::None;

	std::vector<User *> vPartyUsers = PARTYHANDLER->GetPartyMembersIncludingSelf( pcCasterUser, TRUE, DISTANCE_XY_15_meters ); //this includes self

	for ( User * pcUser : vPartyUsers )
	{
		//existing enchant weapon is higher. skip user
		if ( pcUser->bEnchantWeaponLevel1Based > iLevelSkill1Based )
			continue;

		pcUser->bEnchantWeaponLevel1Based = iLevelSkill1Based;


		//only set full duration at initial enchant weapon cast
		pcUser->uEnchantWeaponEndTime = TIME + duration;
		pcUser->bForceSkillBuffUpdate = TRUE;


#ifdef ENCHANTWEAPON_SETTING
		element = (EElementID)pcUser->sSettingsData.bWeaponEnchantPerference;

		//rotation
		if ( element == 0 || element == EElementID::None )
		{
			//only set switch time if it's not yet set
			if ( pcUser->uEnchantWeaponNextSwitchTime == 0 )
			{
				pcUser->bEnchantWeaponRotationCounter = Dice::RandomI( 0, 4 );
				pcUser->uEnchantWeaponNextSwitchTime = TIME;

				ApplyEnchantWeaponRotation( pcUser, pcCasterUser->GetID() );
			}
		}
		else
		{
			switch ( element )
			{
				case EElementID::Fire:
				case EElementID::Ice:
				case EElementID::Lightning:
				{
					break;
				}

				//no perference. use RNG
				default:
				{
					switch ( Dice::RandomI( 1, 3 ) ) //pick a random enchantment
					{
						case 1: element = EElementID::Fire; break;
						case 2: element = EElementID::Ice; break;
						case 3: element = EElementID::Lightning; break;
					}

					break;
				}
			}

			//clear rotation if there is one.
			pcUser->uEnchantWeaponNextSwitchTime = 0;
			pcUser->bEnchantWeaponRotationCounter = 0;

			int duration = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_Duration, iLevelSkill1Based );

			ApplyEnchantWeapon( pcUser, element, iLevelSkill1Based, duration, pcCasterUser->GetID() );
		}
#else
		//only set switch time if it's not yet set
		if ( pcUser->uEnchantWeaponNextSwitchTime == 0 )
		{
			pcUser->bEnchantWeaponRotationCounter = Dice::RandomI( 0, 4 );
			pcUser->uEnchantWeaponNextSwitchTime = TIME;

			ApplyEnchantWeaponRotation( pcUser, pcCasterUser->GetID() );
		}
#endif
	}

	return TRUE;
}

BOOL CMagician::ApplyEnchantWeapon (User * pcUser,  EElementID element, int iLevelSkill1Based, int iDurationSec, ID idCaster )
{
	PacketUseSkillSingle s;
	ZeroMemory( &s, sizeof( PacketUseSkillSingle ) );
	s.iLength = sizeof ( PacketUseSkillSingle );
	s.iHeader = PKTHDR_UseSkillNew;
	s.iSkillID = SKILLID_EnchantWeapon;
	s.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Magician;
	s.iSkillLevel1Based = iLevelSkill1Based;
	s.lCasterID = idCaster;
	s.sTimeLeftSec = iDurationSec;

	pcUser->eEnchantWeaponElement = element;

	//Ice
	if ( element == EElementID::Ice )
	{
		pcUser->sEnchantWeaponAtkPowerPercent = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_IceFreezeAPPercent, iLevelSkill1Based );
		pcUser->bEnchantWeaponChance = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_IceFreezeChance, iLevelSkill1Based );
		pcUser->bEnchantWeaponDurationSec = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_IceFreezeDuration, iLevelSkill1Based );
		pcUser->sEnchantWeaponParameter = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_IceFreezeSpeedReduction, iLevelSkill1Based );
	}

	//Lightning
	else if ( element == EElementID::Lightning )
	{
		pcUser->eEnchantWeaponElement = EElementID::Lightning;
		pcUser->sEnchantWeaponAtkPowerPercent = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightningStunAPPercent, iLevelSkill1Based );
		pcUser->bEnchantWeaponChance = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightningStunChance, iLevelSkill1Based );
		pcUser->bEnchantWeaponDurationSec = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightningStunDuration, iLevelSkill1Based );
		pcUser->sEnchantWeaponParameter = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_LightningRange_New, iLevelSkill1Based );
	}

	//Fire
	else if ( element == EElementID::Fire )
	{
		pcUser->eEnchantWeaponElement = EElementID::Fire;
		pcUser->sEnchantWeaponAtkPowerPercent = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_FireBurningDamagePercent, iLevelSkill1Based );
		pcUser->bEnchantWeaponChance = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_FireBurningChance, iLevelSkill1Based );
		pcUser->bEnchantWeaponDurationSec = GetSkillIntValueAt ( pcUser, Magician_EnchantWeapon_FireBurningDuration, iLevelSkill1Based );
		pcUser->sEnchantWeaponParameter = BURNING_TICKRATE_MS; //tick rate ms
	}
	else
	{
		WARN( "ApplyEnchantWeapon failed. Invalid element: %d", element );
		return FALSE;
	}


	s.lTargetID = pcUser->GetID();
	s.iParameter = pcUser->eEnchantWeaponElement;


	SENDPACKETRANGE ( &s, &pcUser->pcUserData->sPosition, pcUser->GetMapID(), DISTANCE_MAX_SKILL_VISUAL );

	return TRUE;
}


void CMagician::ApplyEnchantWeaponRotation( User * pcUser, ID idCaster )
{
	if ( pcUser->uEnchantWeaponNextSwitchTime &&
		TIME >= pcUser->uEnchantWeaponNextSwitchTime )
	{
		int iDurationRemaining = static_cast<int>(pcUser->uEnchantWeaponEndTime - TIME);

		switch ( pcUser->bEnchantWeaponRotationCounter % 4 )
		{
			case 0: //fire
			{
				ApplyEnchantWeapon( pcUser, EElementID::Fire, pcUser->bEnchantWeaponLevel1Based, iDurationRemaining, idCaster );
				break;
			}
			case 1: //lightning
			{
				ApplyEnchantWeapon( pcUser, EElementID::Lightning, pcUser->bEnchantWeaponLevel1Based, iDurationRemaining, idCaster );
				break;
			}
			case 2: //ice
			{
				ApplyEnchantWeapon( pcUser, EElementID::Ice, pcUser->bEnchantWeaponLevel1Based, iDurationRemaining, idCaster );
				break;
			}
			case 3: //lighting
			{
				ApplyEnchantWeapon( pcUser, EElementID::Lightning, pcUser->bEnchantWeaponLevel1Based, iDurationRemaining, idCaster );
				break;
			}
		}

		pcUser->bEnchantWeaponRotationCounter++;

		if ( pcUser->bEnchantWeaponRotationCounter == 255 )
			pcUser->bEnchantWeaponRotationCounter = 0;

		pcUser->uEnchantWeaponNextSwitchTime = TIME + 10;
	}
}

CMagician::~CMagician()
{
}
