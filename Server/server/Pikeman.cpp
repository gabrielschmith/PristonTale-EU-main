#include "stdafx.h"
#include "Pikeman.h"
#include "HNSSkill.h"


CPikeman::CPikeman()
{
}

/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CPikeman::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Pikeman )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CPikeman::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Pikeman )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CPikeman::UpdateAndCacheServerSkillInfo (  )
{
	INFO( "Updating and caching skill info for Pikeman.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Pikeman );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CPikeman::UpdateAndCacheServerSkillData ( )
{
	INFO( "Updating and caching skill data for Pikeman.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Pikeman;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE);
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CPikeman::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass)
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass)
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PikeWind,			Pikeman_PikeWind_ATKPowMin,					"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PikeWind,			Pikeman_PikeWind_ATKPowMax,					"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PikeWind,			Pikeman_PikeWind_Pushback,					"PushbackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PikeWind,			Pikeman_PikeWind_UseMana,					"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_IceAttribute,		Pikeman_IceAttribute_Frost,					"IceResistance");

		AppendZeroIntValuesToServerSkillDataBuffer( temp, SKILLID_IceAttribute_Old,	Pikeman_IceAttribute_Frost);				//zero out the resistence values

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PolearmMastery,		Pikeman_PolearmMastery_AddRange_New,		"AttackRange");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_PreciseHit,		Pikeman_PreciseHit_CritDmg_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PreciseHit,			Pikeman_PreciseHit_ATKPow_New,				"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PreciseHit,			Pikeman_PreciseHit_AddAttackRating_New,		"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PreciseHit,			Pikeman_PreciseHit_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_JumpingCrash,		Pikeman_JumpingCrash_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_JumpingCrash,		Pikeman_JumpingCrash_WPAtkRtgBoost,			"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_JumpingCrash,		Pikeman_JumpingCrash_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_FreezeChance,			"FreezeChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_FreezeDamage,			"FreezeAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_FreezeDuration,			"FreezeDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_SpeedMonDown_New,		"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GroundPike,			Pikeman_GroundPike_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Tornado,				Pikeman_Tornado_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Tornado,				Pikeman_Tornado_Range,						"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Tornado,				Pikeman_Tornado_UseMana,					"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_WeaponBlockMastery,	Pikeman_WeaponBlockMastery_WPBlockBoost_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WeaponBlockMastery,		Pikeman_WeaponBlockMastery_WPBlockBoost_New,	"BlockChance");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Expansion,			Pikeman_Expansion_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Expansion,			Pikeman_Expansion_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Expansion,			Pikeman_Expansion_Range_New,				"AttackRange");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_Numberofspears,			"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_PoisonChance_New,		"PoisonChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_PoisonDamagePercent_New,	"PoisonAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_PoisonDurationSec_New,	"PoisonDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomSpear,			Pikeman_VenomSpear_PoisonNumStack_New,		"PoisonNumStack");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vanish,				Pikeman_Vanish_AfterDamage,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vanish,				Pikeman_Vanish_MonsterSight,				"MonsterSight");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vanish,				Pikeman_Vanish_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vanish,				Pikeman_Vanish_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_CriticalMastery,		Pikeman_CriticalMastery_WPCritBoost,		"CriticalChance");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLance,			Pikeman_ChainLance_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLance,			Pikeman_ChainLance_UseMana,					"MPCost");


		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChargingStrike,		Pikeman_ChargingStrike_ChargedDMGBst,		"ChargedAttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChargingStrike,		Pikeman_ChargingStrike_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChargingStrike,		Pikeman_ChargingStrike_RaiseSpeed,			"ChargingSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChargingStrike,		Pikeman_ChargingStrike_UseMana,				"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vague,			Pikeman_Vague_AddEvasion_DoNotUse); //zero out to disable asm
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vague,				Pikeman_Vague_AddEvasion_New,				"EvasionChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vague,				Pikeman_Vague_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vague,				Pikeman_Vague_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShadowMaster,		Pikeman_ShadowMaster_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShadowMaster,		Pikeman_ShadowMaster_Numofhits,				"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShadowMaster,		Pikeman_ShadowMaster_UseMana,				"MPCost");
	}

	//Other classes need this info for expansion rendering for charging strike
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Expansion,			Pikeman_Expansion_AddSize,					"WeaponSizePercent");

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinsEye,		Pikeman_AssassinsEye_AddCriticalDmg,		"CriticalAttackPower");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinsEye,		Pikeman_AssassinsEye_Duration,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinsEye,		Pikeman_AssassinsEye_AddCriticalChance,		"CriticalChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinsEye,		Pikeman_AssassinsEye_UseMana,				"MPCost");

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



int CPikeman::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Pikeman Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int boost, value;
	int chainLevel = 0;
	int attackPower = *piAttackPower;

	switch ( uSkillID )
	{
		case PikemanSkill::DefaultPikemanAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case PikemanSkill::PreciseHit:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Critical Hit]" );

			value = GetSkillIntValueAt ( pcUser, Pikeman_PreciseHit_ATKPow_New, iLevelSkill );
			boost = attackPower + value;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", attackPower, boost, ( boost + attackPower ) );
			attackPower += boost;

		} break;

		case PikemanSkill::JumpingCrash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Jumping Crash]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_JumpingCrash_DMGBoost, iLevelSkill );
			if ( p_Target->IsTargetUnit() )
			{
				if (p_Target->GetMonsterType() == MONSTERTYPE_Demon )
				{
					value += 30;
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Demon Monster" );
				}
			}
			boost = ( attackPower * value ) / 100;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", attackPower, boost, ( boost + attackPower ) );
			attackPower += boost;



		} break;

		case PikemanSkill::ChainLance:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Chain Lance]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_ChainLance_DMGBoost, iLevelSkill );
			boost = ( attackPower * value ) / 100;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", attackPower, boost, ( boost + attackPower ) );
			attackPower += boost;


			chainLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_PreciseHit );
			if ( chainLevel )
			{
				value = GetSkillIntValueAt( pcUser, Pikeman_PreciseHit_ATKPow_New, chainLevel, TRUE );

				attackPower += value;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Precise Hit Chain [%d]: Added Damage [%d]. Final Damage [%d]", chainLevel,  value, attackPower );
			}

		} break;

		case PikemanSkill::ShadowMaster:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Shadow Master]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_ShadowMaster_DMGBoost, iLevelSkill );
			boost = ( attackPower * value ) / 100;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", attackPower, boost, ( boost + attackPower ) );
			attackPower += boost;

			psPacketEx->sCriticalChance = 100; //100% crit chance

			chainLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_PreciseHit );
			if ( chainLevel )
			{
				value = GetSkillIntValueAt( pcUser, Pikeman_PreciseHit_ATKPow_New, chainLevel, TRUE );
				attackPower += value;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Precise Hit Chain [%d]: Added Damage [%d]. Final Damage [%d]", chainLevel,  value, attackPower );
			}

			chainLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_ChainLance );
			if ( chainLevel )
			{
				value = GetSkillIntValueAt( pcUser, Pikeman_ChainLance_DMGBoost, chainLevel, TRUE );
				boost = ( attackPower * value ) / 100;
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Chain Lance Chain [%d]: Damage Boost [%d%%] Added Damage [%d] Final Damage [%d]", chainLevel, value, boost, attackPower );
			}


		} break;

		case PikemanSkill::ChargingStrike:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Charging Strike]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> WeaponAttackPower[0] = [%d]", psPacketEx->AttackParameter1 );

			if ( psPacketEx->AttackParameter1 == 0 )
			{
				value = GetSkillIntValueAt ( pcUser, Pikeman_ChargingStrike_ChargedDMGBst, iLevelSkill );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Charged Skill" );
			}
			else
			{
				value = GetSkillIntValueAt ( pcUser, Pikeman_ChargingStrike_DMGBoost, iLevelSkill );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Normal Skill" );
			}

			boost = ( attackPower * value ) / 100;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [%d] AttackRating [%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d + %d] (%d)", attackPower, boost, ( boost + attackPower ) );
			attackPower += boost;

			chainLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_Expansion );
			if ( chainLevel )
			{
				value = GetSkillIntValueAt( pcUser, Pikeman_Expansion_DMGBoost, chainLevel, TRUE );
				boost = ( attackPower * value ) / 100;
				attackPower += boost;
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Expansion Chain [%d]: Added Damage [%d]. Final Damage [%d]", chainLevel,  boost, attackPower );
			}




		} break;

		case PikemanSkill::AssassinsEye:
		{
			//not used anymore
			pcUserData->AssassinEyeParam = 0;
			pcUserData->dwAssassinEyeEndTime = 0;

			if (p_Target->IsTargetUnit())
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Assassin's Eye]" );
				value = GetSkillIntValueAt ( pcUser, Pikeman_AssassinsEye_AddCriticalDmg, iLevelSkill );


				UINT uTime = GetSkillIntValueAt ( pcUser, Pikeman_AssassinsEye_Duration, iLevelSkill );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [+%d%%] Duration [%d]", value, uTime );

				p_Target->unitData->sAssassinEyeCritDmg = value;
				p_Target->unitData->dwAssassinEyeEndTime = ( uTime * 1000 ) + TICKCOUNT;

				value = GetSkillIntValueAt ( pcUser, Pikeman_AssassinsEye_AddCriticalChance, iLevelSkill );
				p_Target->unitData->sAssassinEyeCritChance = value;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> %s Target", p_Target->GetCharacterData()->szName );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical Chance [%d] Duration [%d]", value, uTime );
			}
			return 0;
		} break;

		case PikemanSkill::RingOfSpears:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [RingOfSpears]" );
			value = iaPikeT51Duration[iLevelSkill - 1];

            int iResistance = p_Target->GetCharacterData()->sElementalDef[Organic];
            if (iResistance < 0)
                iResistance = 0;
            if (iResistance >= 100)
                iResistance = 100;

            iResistance = 100 - iResistance;

			if ( p_Target->IsTargetUnit() )
			{
				UNITSERVER->SetMotionFromCode(p_Target->unitData, EAnimationType::ANIMATIONTYPE_Flinch);
				p_Target->unitData->iStunTimeLeft = value << 4;
                p_Target->unitData->iStunTimeLeft = (p_Target->unitData->iStunTimeLeft * iResistance) / 100;
                attackPower = 0;
				return 0;
			}
			else if (p_Target->IsTargetPlayer())
			{
				int iStun = ((value * 70) * iResistance) / 100;
				DAMAGEHANDLER->SendDamagePvPData( pcUserData, p_Target->userData, 1, psPacketEx->iAttackRating, EAttackState::State_128_Skill, GetSkillIDFromTinyID(ECharacterClass::CHARACTERCLASS_Pikeman, PikemanSkill::RingOfSpears), -0x5A, iStun );
				attackPower = 0;
				return 0;
			}
		} break;


		default:
			break;

	}

	*piAttackPower = attackPower;

	return 1;

}

int CPikeman::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill1Based = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Pikeman Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketRangeEx->iSkillID );
		return 0;
	}

	int value, chain, range, count;
	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{

		case PikemanSkill::PikeWind:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Pike Wind]" );
			int iDamageMin = GetSkillIntValueAt ( pcUser, Pikeman_PikeWind_ATKPowMin, iLevelSkill1Based );
			int iDamageMax = GetSkillIntValueAt ( pcUser, Pikeman_PikeWind_ATKPowMax, iLevelSkill1Based );
			int iRange = GetSkillIntValueAt ( pcUser, Pikeman_PikeWind_Pushback, iLevelSkill1Based );
			psPacketRange->iDamage = Dice::RandomI ( iDamageMin, iDamageMax );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range [%d] Damage [%d-%d][%d]", iRange, iDamageMin, iDamageMax, psPacketRange->iDamage );
			psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_1_Knockback;

			//chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_GroundPike );
			//if ( chain )
			//{
			//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Ground Pike Chain: Level [%d]");
			//
			//	psPacketRangeEx->eSecondaryAttackProperty = EAttackProperty::Ice;
			//	psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeDamage, chain, TRUE );
			//	psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeDuration, chain, TRUE );
			//	psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeChance, chain, TRUE );
			//	psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_SpeedMonDown_New, chain, TRUE );
			//
			//}

		} break;

		case PikemanSkill::GroundPike:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Ground Pike]" );

			range = GetSkillIntValueAt ( pcUser, Pikeman_GroundPike_Range, iLevelSkill1Based );
			psPacketRange->iRange = range;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range [%d]", psPacketRange->iRange );

			psPacketRange->iAttackProperty = EAttackProperty::Ice;


			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeDamage, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeDuration, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_FreezeChance, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_GroundPike_SpeedMonDown_New, iLevelSkill1Based );


			range = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_PolearmMastery ), TRUE );
			psPacketRange->iRange += range;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Polearm Mastery Chain: Added range [%d] Total range [%d]", range, psPacketRange->iRange );
		} break;

		case PikemanSkill::VenomSpear:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Venom Spear]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_VenomSpear_DMGBoost, iLevelSkill1Based );
			count = GetSkillIntValueAt ( pcUser, Pikeman_VenomSpear_Numberofspears, iLevelSkill1Based );
			range = GetSkillIntValueAt ( pcUser, Pikeman_VenomSpear_Range, iLevelSkill1Based );


			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			psPacketRange->iCount = count;
			*( WORD* )0x08B80AF4 = ( WORD )psPacketRange->iDamage;
			psPacketRange->iRange = range;
			psPacketRange->eAttackState = EAttackState::State_106_Poison;
			psPacketRange->iAttackProperty = EAttackProperty::Poison;

			psPacketRange->iCount &= 0x000000FF;

			psPacketRangeEx->sElementalAttackSetting.bDurationSec	= GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_VenomSpear_PoisonDurationSec_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP	= GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_VenomSpear_PoisonDamagePercent_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance		= GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_VenomSpear_PoisonChance_New, iLevelSkill1Based );

			value = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_VenomSpear_PoisonNumStack_New, iLevelSkill1Based );
			value |= POISON_MAXSTACK << 8;
			psPacketRangeEx->sElementalAttackSetting.sStack = value;


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d] Duration [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * value ) / 100 ),
				psPacketRange->iDamage + ( ( psPacketRange->iDamage * value ) / 100 ), psPacketRangeEx->sCriticalChance );

			range = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_PolearmMastery ), TRUE );
			psPacketRange->iRange += range;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Polearm Mastery Chain: Added range [%d] Total range [%d]", range, psPacketRange->iRange );

		} break;

		case PikemanSkill::Tornado:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Tornado]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_Tornado_DMGBoost, iLevelSkill1Based );
			range = GetSkillIntValueAt ( pcUser, Pikeman_Tornado_Range, iLevelSkill1Based );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * value ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * value ) / 100 ) );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );

			psPacketRange->iRange = range;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range [%d] Crit [%d]", range, pcUserData->sCharacterData.iCritical );


			range = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_PolearmMastery ), TRUE );
			psPacketRange->iRange += range;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Polearm Mastery Chain: Added range [%d] Total range [%d]", range, psPacketRange->iRange );

			psPacketRange->eAttackState = EAttackState::State_0_Normal;
		} break;

		case PikemanSkill::Expansion:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Expansion]" );
			value = GetSkillIntValueAt ( pcUser, Pikeman_Expansion_DMGBoost, iLevelSkill1Based );
			range = GetSkillIntValueAt ( pcUser, Pikeman_Expansion_Range_New, iLevelSkill1Based );

			psPacketRange->iRange = range + 30;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * value ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * value ) / 100 ), range );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			psPacketRange->iCount &= 0x000000FF;

		} break;

		default:
			break;
	}

	return 1;
}

int CPikeman::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
{
	UserData * pcUserData = pcUser->pcUserData;

	int iLevelSkill1Based = (packet->iSkillCode >> 8) & 0xFF;
	int iSkillTinyID = packet->iSkillCode & 0xFF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Target: %d, Param1: %d, Param2: %d", iSkillTinyID, iLevelSkill1Based, packet->iTargetID, packet->iParam1, packet->iParam2 );

	if ( iLevelSkill1Based < 1 || iLevelSkill1Based > 10 )
	{
		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> iSkillLevel is not 1-10 ?? (%d)", iLevelSkill1Based );
		return TRUE;
	}

	int iValue = 0;

	switch ( iSkillTinyID )
	{

		case PikemanSkill::Vanish:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Vanish]" );
			int boost = GetSkillIntValueAt ( pcUser, Pikeman_Vanish_AfterDamage, iLevelSkill1Based );
			pcUserData->saVanishParam[ 0 ] = ( WORD )boost;

			int iVisibility = GetSkillIntValueAt ( pcUser, Pikeman_Vanish_MonsterSight, iLevelSkill1Based );
			pcUserData->saVanishParam[ 1 ] = ( WORD )iVisibility;

			UINT uTime = GetSkillIntValueAt ( pcUser, Pikeman_Vanish_Duration, iLevelSkill1Based );
			pcUserData->dwVanishEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage Boost [+%d%%] Visibility [%d]", boost, iVisibility );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Duration [%d]", uTime );
		} break;

		case PikemanSkill::AssassinsEye:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Assasin's Eye]" );
			int iCriticalDamage = GetSkillIntValueAt ( pcUser, Pikeman_AssassinsEye_AddCriticalDmg, iLevelSkill1Based );
			pcUserData->AssassinEyeParam = iCriticalDamage;

			UINT uTime = GetSkillIntValueAt ( pcUser, Pikeman_AssassinsEye_Duration, iLevelSkill1Based );
			pcUserData->dwAssassinEyeEndTime = ( uTime * 1000 ) + TICKCOUNT;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical [+%d%%] Duration [%d]", iCriticalDamage, uTime );
		} break;

		case PikemanSkill::Vague:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Vague]" );
			int iEvasion = GetSkillIntValueAt ( pcUser, Pikeman_Vague_AddEvasion_New, iLevelSkill1Based );

			//done on client side (for now)
			pcUserData->iVagueParam = 0;
			pcUserData->dwVagueEndTime = 0;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Evasion [%d%%]", iEvasion );
		} break;

		default:
			break;
	}
	return 1;
}


CPikeman::~CPikeman()
{
}
