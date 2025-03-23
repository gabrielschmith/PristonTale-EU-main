#include "stdafx.h"
#include "Fighter.h"
#include "HNSSkill.h"


CFighter::CFighter()
{
}

/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CFighter::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Fighter )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CFighter::SendServerSkillDataToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Fighter )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build
/// </summary>
void CFighter::UpdateAndCacheServerSkillInfo ()
{
	INFO( "Updating and caching skill info for Fighter.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Fighter );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CFighter::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Fighter.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Fighter;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}


/// <summary>
/// Build skill data container using data from database
/// </summary>
void CFighter::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_MeleeMastery,	Fighter_MeleeMastery_WPAtkPowAdd_Obsolete );
		//AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MeleeMastery,		Fighter_MeleeMastery_WPAtkPowAdd_New,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MeleeMastery,		Fighter_MeleeMastery_WPAtkPowAddPercent_New,	"AttackPowerMaxPercent");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Recuperation,	Fighter_FireAttributeOld_Flame_Obsolete);

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Raving,				Fighter_Raving_DMGBoost,						"AttackPowerPercent");
		AppendZeroIntValuesToServerSkillDataBuffer( temp, SKILLID_Raving,			Fighter_Raving_HPCost_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Raving,				Fighter_Raving_HPCost_New,						"HPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Raving,				Fighter_Raving_Numofhits,						"NumberofAttacks");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Raving,				Fighter_Raving_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DoubleImpact,		Fighter_DoubleImpact_AddAtkRtg,					"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DoubleImpact,		Fighter_DoubleImpact_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DoubleImpact,		Fighter_DoubleImpact_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_TripleImpact,		Fighter_TripleImpact_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_TripleImpact,		Fighter_TripleImpact_Numofhits,					"NumberofAttacks");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_TripleImpact,		Fighter_TripleImpact_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BrutalSwing,		Fighter_BrutalSwing_AddCritical,				"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BrutalSwing,		Fighter_BrutalSwing_CriticalDmgBoost_New,		"CriticalAttackPowerBoost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BrutalSwing,		Fighter_BrutalSwing_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BrutalSwing,		Fighter_BrutalSwing_HPCost_New,					"HPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BrutalSwing,		Fighter_BrutalSwing_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_Damage,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_AttackRadius_New,			"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_BurningDamagePercent_New,	"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_BurningChance_New,			"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_RageofZecram,		Fighter_RageofZecram_BurningDurationSec_New,	"BurningDurationSec");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_SwiftAxe_ATKSpdBoost_DoNotUse);
		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_Concentration_AtkRtgBoost_DoNotUse);

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_ConcentrationSA_ATKSpdBoost_New,		"AttackSpeedPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_ConcentrationSA_ATKRtgBoost_New,		"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_ConcentrationSA_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ConcentrationSA,	Fighter_ConcentrationSA_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_AvengingCrash,		Fighter_AvengingCrash_AddAtkRtg,				"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_AvengingCrash,		Fighter_AvengingCrash_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_AvengingCrash,		Fighter_AvengingCrash_HPCost_New,				"HPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_AvengingCrash,		Fighter_AvengingCrash_UseMana,					"MPCost");


		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BoneCrash,			Fighter_BoneCrash_BonusDMGDemons,				"VsDemonsExtraPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BoneCrash,			Fighter_BoneCrash_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_BoneCrash,			Fighter_BoneCrash_UseMana,						"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Destroyer,			Fighter_Destroyer_AddCritical,					"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Destroyer,			Fighter_Destroyer_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Destroyer,			Fighter_Destroyer_HPCost_New,					"HPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Destroyer,			Fighter_Destroyer_UseMana,						"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_AddATKPow_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_AddATKPow_New,				"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_DecreaseAbs,					"Absorption");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_LifeLeechPercent_New,			"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Berserker,			Fighter_Berserker_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_CycloneStrike,		Fighter_CycloneStrike_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_CycloneStrike,		Fighter_CycloneStrike_Maxhits,					"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_CycloneStrike,		Fighter_CycloneStrike_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_CycloneStrike,		Fighter_CycloneStrike_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Resilience,			Fighter_BoostHealth_HPBoost,					"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Resilience,			Fighter_BoostHealth_HPBoostPercent,				"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Resilience,			Fighter_BoostHealth_HPPotionBoost,				"HPPotionPercent");


		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Recuperation,		Fighter_Recuperation_HPRegen_New,				"HPRegen");
	}

	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_Radius,							"AttackRange");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_StunTime,						"StunDurationSec");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_StunChance_New,					"StunChance");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_StunAtkPowPerc_New,				"StunAtkPowPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_HPRestore_New,					"HPPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_SPRestore_New,					"SPPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_HPRegen_New,						"HPRegen");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_SPRegen_New,						"SPRegen");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_RegenDuration_Sec,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_WarCry,				Fighter_WarCry_UseMana,							"MPCost");

	AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DemonsBane, Fighter_ConcentrationOld_AtkRtgBoost_Obsolete);

	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DemonsBane,		Fighter_DemonBuff_Duration,					"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DemonsBane,		Fighter_DemonBuff_UseMana,					"MPCost");
	AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_DemonsBane,		Fighter_DemonBuff_DamageBoostVsDemon_New,	"VsDemonsExtraPercent");

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



int CFighter::GetBerserkerAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	if (pcUser->sBersekerDamageBoost )
	{
		return pcUser->sBersekerDamageBoost;
	}

	return 0;
}

int CFighter::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill1Based = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Fighter Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketEx->iSkillCode );
		return 0;
	}

	int value, boost, chain, crit;

	int attackPower = *piAttackPower;

	switch ( uSkillID )
	{
		case FighterSkill::DefaultFighterAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;
		case FighterSkill::Raving:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Raving]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_Raving_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;



		case FighterSkill::DoubleImpact:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [DoubleImpact]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_DoubleImpact_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case FighterSkill::TripleImpact:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Triple DoubleImpact]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_TripleImpact_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

			if ( psPacketEx->MotionEventNum == 2 )
				psPacketEx->sCriticalChance = 0; //remove crit on 2nd motion..?

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_DoubleImpact );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Fighter_DoubleImpact_DMGBoost, chain, TRUE );
				attackPower += ( ( attackPower * value ) / 100 );

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> DoubleImpact Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d][%d]", value, attackPower );
			}

		} break;

		case FighterSkill::BrutalSwing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Brutal Swing]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_BrutalSwing_AddCritical, iLevelSkill1Based );
			boost = pcUserData->sCharacterData.iCritical + ( ( pcUserData->sCharacterData.iCritical * value ) / 100 );
			psPacketEx->sCriticalChance = boost;

			value = GetSkillIntValueAt ( pcUser, Fighter_BrutalSwing_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			value = GetSkillIntValueAt ( pcUser, Fighter_BrutalSwing_CriticalDmgBoost_New, iLevelSkill1Based );
			psPacketEx->sCriticalDmgBoost = value;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", boost, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical Dmg Boost[%d]", value );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;

		case FighterSkill::RageOfZecram:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rage Of Zecram]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			value = GetSkillIntValueAt ( pcUser, Fighter_RageofZecram_Damage, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

			psPacketEx->iRange = GetSkillIntValueAt ( pcUser, Fighter_RageofZecram_AttackRadius_New, iLevelSkill1Based );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Range [%d]", psPacketEx->iRange );



		}break;

		case FighterSkill::AvengingCrash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Avenging Crash]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			chain = GetUserSkillLevel1Based( pcUser, SKILLID_BrutalSwing );
			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Brutal Swing Chain [Lvl. %d]", chain );

				crit = pcUserData->sCharacterData.iCritical;
				value = GetSkillIntValueAt ( pcUser, Fighter_BrutalSwing_AddCritical, chain, TRUE );
				boost = ( ( ( crit * value ) / 100 ) );

				crit += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Crit Add [%d%%] Value [%d] Final [%d]", value, boost, crit );

				psPacketEx->sCriticalChance = crit; //crit chance
			}


			value = GetSkillIntValueAt ( pcUser, Fighter_AvengingCrash_DMGBoost, iLevelSkill1Based );
			boost = ( ( attackPower * value ) / 100 );
			attackPower += boost;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Prec [%d%%] Value [%d] Final [%d]", value, boost, attackPower );

		} break;

		case FighterSkill::SesmicImpact:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Sesmic DoubleImpact]" );
			psPacketEx->sCriticalChance = pcUserData->sCharacterData.iCritical;
			value = iaSesmicImpactDamage[iLevelSkill1Based-1];
			attackPower += ((attackPower * value) / 100);

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->sCriticalChance, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;

		case FighterSkill::BoneCrash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Bone Crash]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			value = GetSkillIntValueAt ( pcUser, Fighter_BoneCrash_DMGBoost, iLevelSkill1Based );
			boost = ( ( attackPower * value ) / 100 );
			attackPower += boost;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Prec [%d%%] Value [%d] Final [%d]", value, boost, attackPower );

			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Demon  )
			{
				value = GetSkillIntValueAt ( pcUser, Fighter_BoneCrash_BonusDMGDemons, iLevelSkill1Based );
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Demon Boost" );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Prec [%d%%] Value [%d] Final [%d]", value, boost, attackPower );

			}

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_DoubleImpact );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Fighter_DoubleImpact_DMGBoost, chain, TRUE);
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> DoubleImpact Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Prec [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_TripleImpact );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Fighter_TripleImpact_DMGBoost, chain, TRUE );
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Triple DoubleImpact Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Prec [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}

		} break;

		case FighterSkill::Destroyer:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Destroyer]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Attack Rating [%d]", pcUserData->sCharacterData.iAttackRating );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Crit [%d] ", psPacketEx->sCriticalChance );

			chain = GetUserSkillLevel1Based( pcUser, SKILLID_BrutalSwing );
			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Brutal Swing Chain [Lvl. %d]", chain );

				crit = pcUserData->sCharacterData.iCritical;
				value = GetSkillIntValueAt ( pcUser, Fighter_BrutalSwing_AddCritical, chain, TRUE );
				boost = ( ( ( crit * value ) / 100 ) );
				psPacketEx->sCriticalChance += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Crit: Boost [%d%%] Result [%d] Final [%d]", value, boost, psPacketEx->sCriticalChance );
			}


			value = GetSkillIntValueAt ( pcUser, Fighter_Destroyer_DMGBoost, iLevelSkill1Based );
			boost = ( attackPower * value ) / 100;
			attackPower += boost;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Boost [%d%%] Result [%d] Final [%d]", value, boost, attackPower );

			chain = GetUserSkillLevel1Based( pcUser, SKILLID_AvengingCrash );
			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Avenging Crash Chain [Lvl. %d]", chain, TRUE );

				value = GetSkillIntValueAt ( pcUser, Fighter_AvengingCrash_DMGBoost, chain, TRUE );
				boost = ( attackPower * value ) / 100;
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Boost [%d%%] Result [%d] Final [%d]", value, boost, attackPower );
			}


		}break;


		default:
			break;

	}

	*piAttackPower = attackPower;

	return 1;

}

int CFighter::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill1Based = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Fighter Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketRangeEx->iSkillID );
		return 0;
	}

	int value, chain;
	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{
		case FighterSkill::WarCry:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [WarCry]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_WarCry_Radius, iLevelSkill1Based );
			psPacketRange->iDamage = 0;
			psPacketRange->iRange = value;
			psPacketRange->iAttackProperty = EAttackProperty::Organic;
			value = GetSkillIntValueAt ( pcUser, Fighter_WarCry_StunTime, iLevelSkill1Based );

			psPacketRange->eAttackState = EAttackState::State_2_Stun;
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_WarCry_StunAtkPowPerc_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_WarCry_StunTime, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_WarCry_StunChance_New, iLevelSkill1Based );

			psPacketRange->iDamage = 0;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range = %d , Time = %d", psPacketRange->iRange, psPacketRangeEx->iaParametersEx[0] );
		} break;

		case FighterSkill::RageOfZecram:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rage Of Zecram]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_RageofZecram_AttackRadius_New, iLevelSkill1Based );

			psPacketRange->iRange = value;
			value = GetSkillIntValueAt ( pcUser, Fighter_AvengingCrash_DMGBoost, iLevelSkill1Based );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * value) / 100);
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->sAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iAttackProperty = EAttackProperty::Fire;

			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningDamagePercent_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningChance_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec =  GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningDurationSec_New, iLevelSkill1Based );
		}break;

		case FighterSkill::SesmicImpact: //Area Last Hit
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Sesmic DoubleImpact]" );
			psPacketRange->iRange = iaSesmicImpactArea[iLevelSkill1Based-1];
			value = iaSesmicImpactDamage[iLevelSkill1Based-1];
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * value) / 100);
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->sAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iAttackProperty = EAttackProperty::Fire;
		}break;

		case FighterSkill::CycloneStrike:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Cyclone Strike]" );
			value = GetSkillIntValueAt ( pcUser, Fighter_CycloneStrike_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			value = GetSkillIntValueAt ( pcUser, Fighter_CycloneStrike_Range, iLevelSkill1Based );
			psPacketRange->iRange = value;
			value = GetSkillIntValueAt ( pcUser, Fighter_CycloneStrike_Maxhits, iLevelSkill1Based );
			psPacketRange->iCount = value;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage = %d, Range = %d", psPacketRange->iDamage, psPacketRange->iRange );

			psPacketRange->iCount &= 0x000000FF;

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_RageofZecram );
			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Rage of Zecram Chain [Lvl. %d]", chain );

				psPacketRangeEx->eSecondaryAttackProperty = EAttackProperty::Fire;
				psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningDamagePercent_New, chain, TRUE );
				psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningChance_New, chain, TRUE );
				psPacketRangeEx->sElementalAttackSetting.bDurationSec =  GetSkillIntValueAt( pcUser, ESkillArrayPointer::Fighter_RageofZecram_BurningDurationSec_New, chain, TRUE );
			}

		}break;

		default:
			break;
	}

	return 1;
}

int CFighter::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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

	int duration;

	switch ( iSkillTinyID )
	{
		case FighterSkill::DemonsBane:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Demon Buff]" );
			duration = GetSkillIntValueAt( pcUser, Fighter_DemonBuff_Duration, iLevelSkill1Based );
			ApplyAndSendDemonBuff( pcUser, iLevelSkill1Based, duration );

			break;
		}

		case FighterSkill::WarCry:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [WarCry]" );

			int duration = GetSkillIntValueAt( pcUser, Fighter_WarCry_RegenDuration_Sec, iLevelSkill1Based );

			//add everyone else in the party within 15 meters
			if ( pcUser->bParty && pcUser->psParty )
			{
				for ( User * pPartyUser : PARTYHANDLER->GetPartyMembers( pcUser->psParty ) ) //this will include self too
				{
					if ( pPartyUser->pcUserData->sPosition.WithinXZYDistance( &pcUser->pcUserData->sPosition, DISTANCE_XY_15_meters, 100 ) )
					{
						ApplyAndSendWryCryBuff( pPartyUser, iLevelSkill1Based, duration, pcUser->GetID() );
					}
				}
			}
			else
			{
				ApplyAndSendWryCryBuff( pcUser, iLevelSkill1Based, duration, pcUser->GetID() );
			}

			break;
		}

		case FighterSkill::Berserker:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Berserker]" );

			const int berserkerAttackPowBoost = GetSkillIntValueAt ( pcUser, Fighter_Berserker_AddATKPow_New, iLevelSkill1Based );
			const int berserkerDuration = GetSkillIntValueAt(pcUser, Fighter_Berserker_Duration, iLevelSkill1Based);
			const int berserkerLifeLeech = GetSkillIntValueAt(pcUser, Fighter_Berserker_LifeLeechPercent_New, iLevelSkill1Based);
			const int ravingLevel = GetUserSkillLevel1Based(pcUser, ESkillID::SKILLID_Raving);
			int berserkerBoost = berserkerAttackPowBoost;

			if (ravingLevel)
			{
				const int ravingAttackPowBoost = GetSkillIntValueAt ( pcUser, Fighter_Raving_DMGBoost, ravingLevel, TRUE );

				berserkerBoost += ravingAttackPowBoost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Raving Chain [Lvl. %d]", ravingLevel);
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage Increase: [%d%%] Final: [%d%%]", ravingAttackPowBoost, berserkerBoost);
			}

			pcUser->sBersekerDamageBoost = berserkerBoost;
			pcUser->sBersekerDamageToHPPercent = berserkerLifeLeech;
			pcUser->uBerserkerEndTime = TIME + berserkerDuration;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "Damage Increase: %d%%", berserkerAttackPowBoost);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Life Leech [%d%%]", berserkerLifeLeech);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Duration [%ds]", berserkerDuration );
		} break;

		default:
			break;
	}
	return 1;
}

void CFighter::ApplyAndSendWryCryBuff( User * pcTargetUser, int iLevelSkill1Based, int iDurationSec, ID idCaster )
{
	CHATSERVER->SendDebugChat( pcTargetUser, CHATCOLOR_Error, "> [WarCry] Lvl. %d", iLevelSkill1Based  );

	PacketUseSkillSingle s;
	ZeroMemory( &s, sizeof( PacketUseSkillSingle ) );
	s.iLength = sizeof ( PacketUseSkillSingle );
	s.iHeader = PKTHDR_UseSkillNew;
	s.iSkillID = SKILLID_WarCry;
	s.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Fighter;
	s.iSkillLevel1Based = iLevelSkill1Based;
	s.lCasterID = idCaster;
	s.lTargetID = pcTargetUser->GetID();
	s.iParameter = iDurationSec;

	SENDPACKETRANGE ( &s, &pcTargetUser->pcUserData->sPosition, pcTargetUser->GetMapID(), DISTANCE_MAX_SKILL_VISUAL );
}

void CFighter::ApplyAndSendDemonBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	time_t endTime = TIME + iDurationSec;
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bDemonBaneLevel1Based )
		{
			pPartyUser->bDemonBaneLevel1Based = iLevel1Based;
			pPartyUser->uDemonBaneEndTime = endTime;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_DemonsBane, iLevel1Based, iDurationSec, 0, pBuffUsers );
}


CFighter::~CFighter()
{
}
