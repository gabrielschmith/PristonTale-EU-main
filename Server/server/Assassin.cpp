#include "stdafx.h"
#include "Assassin.h"
#include "HNSSkill.h"


CAssassin::CAssassin()
{
}


CAssassin::~CAssassin()
{
}


/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CAssassin::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Assassin )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CAssassin::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CAssassin::UpdateAndCacheServerSkillInfo (  )
{
	INFO( "Updating and caching skill info for Assassin.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Assassin );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CAssassin::UpdateAndCacheServerSkillData ( )
{
	INFO( "Updating and caching skill data for Assassin.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Assassin;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE);
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CAssassin::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass)
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Stinger,				Assassin_Stinger_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Stinger,				Assassin_Stinger_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RunningHit,			Assassin_RunningHit_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RunningHit,			Assassin_RunningHit_UseMana,			"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DualWieldMastery,	Assassin_DualWieldMastery_WPAtkPowBoostFlat_New,"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DualWieldMastery,	Assassin_DualWieldMastery_WPAtkPowBoost,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DualWieldMastery,	Assassin_DualWieldMastery_WPAtkRtgBoost_New,	"AttackRatingPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Curse_Duration_New,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Curse_SpeedReduction_New,		"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Curse_DamageReduction,		"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Curse_PercentOfAP_New,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Curse_Chance_New,				"Chance");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Wisp,				Assassin_Wisp_UseMana,					"MPCost");


		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_Duration_New,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_UseMana,				"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_PoisonChance_New,		"PoisonChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_PoisonDamagePercent_New,"PoisonAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_PoisonDurationSec_New,	"PoisonDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VenomThorn,			Assassin_VenomThorn_PoisonNumStack_New,		"PoisonNumStack");



		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulShock,			Assassin_SoulShock_StunChance_New,		"StunChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulShock,			Assassin_SoulShock_StunAtkPowPerc_New,	"StunAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulShock,			Assassin_SoulShock_Radius,				"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulShock,			Assassin_SoulShock_StunTime,			"StunDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulShock,			Assassin_SoulShock_UseMana,				"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_DodgeMastery,	Assassin_AttackMastery_WPBlockBoost_DoNotUse); //zero out the values to disable effect in assembly

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DodgeMastery,		Assassin_DodgeMastery_AddEvasive_New,	"EvasionChance");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoreBlade,			Assassin_SoreBlade_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoreBlade,			Assassin_SoreBlade_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GustSlash,			Assassin_GustSlash_Area,				"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GustSlash,			Assassin_GustSlash_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GustSlash,			Assassin_GustSlash_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Blind,				Assassin_Blind_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Blind,				Assassin_Blind_MonsterSight,			"MonsterSight");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Blind,				Assassin_Blind_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_UseMana,				"MPCost");
		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_SpeedMonDown,			"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_PoisonPercentOfAP_New,	"PoisonAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_PoisonDurationSec,		"PoisonDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_PoisonChance_New,		"PoisonChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Cripple,				Assassin_Cripple_PoisonNumStack_New,	"PoisonNumStack");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinMastery,		Assassin_AssassinMastery_WPCritBoost,		"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AssassinMastery,		Assassin_AssassinMastery_AtkSpeedBoost_New,	"AttackSpeedPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_PoisonPercentOfAP,	"PoisonAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_PoisonDurationSec,	"PoisonDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_PoisonChance_New,		"PoisonChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_PoisonNumStack_New,	"PoisonNumStack");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_MaxHits,				"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_Range,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Polluted,			Assassin_Polluted_UseMana,				"MPCost");


		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_NinjaShadow,			Assassin_NinjaShadow_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_NinjaShadow,			Assassin_NinjaShadow_UseMana,			"MPCost");

	}

	AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inpes,	Assassin_Inpes_ATKSPDBoost_DoNotuse);			//zero values to disable ASM

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inpes,		Assassin_Inpes_MovementSpeed_New,					"Speed");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inpes,		Assassin_Inpes_Duration,							"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inpes,		Assassin_Inpes_UseMana,								"MPCost");

	//Party buff
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Alas,		Assassin_Alas_AddEvasion,							"EvasionChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Alas,		Assassin_Alas_Duration,								"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Alas,		Assassin_Alas_UseMana,								"MPCost");

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


int CAssassin::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill1Based = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Assassin Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketEx->iSkillCode );
		return 0;
	}

	int value = 0;
	int attackPower = *piAttackPower;
	int boost, chain;

	switch ( uSkillID )
	{
		case AssassinSkill::DefaultAssassinAttack:
		{

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			if ( pcUser->uVenomThornEndTime > TIME && pcUser->bVenomLevel1Based )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Venom Thorn] (Lvl %d)",  pcUser->bVenomLevel1Based );

				psPacketEx->eSecondaryAttackProperty = EAttackProperty::Poison;

				psPacketEx->sSecondaryElementalAttackSetting.bDurationSec	= GetSkillIntValueAt ( pcUser, Assassin_VenomThorn_PoisonDurationSec_New, pcUser->bVenomLevel1Based );
				psPacketEx->sSecondaryElementalAttackSetting.sPercentOfAP	= GetSkillIntValueAt( pcUser, Assassin_VenomThorn_PoisonDamagePercent_New, pcUser->bVenomLevel1Based );
				psPacketEx->sSecondaryElementalAttackSetting.bChance		= GetSkillIntValueAt( pcUser, Assassin_VenomThorn_PoisonChance_New, pcUser->bVenomLevel1Based );

				value = GetSkillIntValueAt( pcUser, Assassin_VenomThorn_PoisonNumStack_New, pcUser->bVenomLevel1Based );
				value |= POISON_MAXSTACK << 8;
				psPacketEx->sSecondaryElementalAttackSetting.sStack = value;
			}

		} break;

		case AssassinSkill::Wisp:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Wisp]" );

			attackPower = 0;
			psPacketEx->iRange = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Range, iLevelSkill1Based );
			psPacketEx->eAttackState = EAttackState::State_8_Cursed;

			psPacketEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_PercentOfAP_New, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_Chance_New, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_Duration_New, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_SpeedReduction_New, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.sDamageReduction = GetSkillIntValueAt( pcUser,ESkillArrayPointer::Assassin_Wisp_Curse_DamageReduction, iLevelSkill1Based );

			break;
		}


		case AssassinSkill::Stinger:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Stinger]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_Stinger_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			if ( p_Target->IsTargetUnit() )
			{
				if (p_Target->GetMonsterType() == MONSTERTYPE_Mutant )
				{
					attackPower += ( ( attackPower * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Mutant" );
				}
				else if (p_Target->GetMonsterType() == MONSTERTYPE_Normal )
				{
					attackPower += ( ( attackPower * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Normal" );
				}
			}


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::RunningHit:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Running Hit]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_RunningHit_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::VenomThorn: //now a buff
		{
			attackPower = 0;
		} break;

		case AssassinSkill::SoreBlade:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Sore Blade]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_SoreBlade_DMGBoost, iLevelSkill1Based );
			boost = ( ( attackPower * value ) / 100 );
			attackPower += boost;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );


			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_RunningHit );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Assassin_RunningHit_DMGBoost, chain, TRUE );
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Running Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case AssassinSkill::GustSlash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Gust Slash]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_GustSlash_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::Cripple:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Frost Wind]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_Cripple_DMGBoost, iLevelSkill1Based );
			attackPower += ( ( attackPower * value ) / 100 );

			psPacketEx->iAttackProperty = EAttackProperty::Poison;

			psPacketEx->sElementalAttackSetting.bDurationSec	= GetSkillIntValueAt ( pcUser, Assassin_Cripple_PoisonDurationSec, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.sPercentOfAP	= GetSkillIntValueAt( pcUser, Assassin_Cripple_PoisonPercentOfAP_New, iLevelSkill1Based );
			psPacketEx->sElementalAttackSetting.bChance			= GetSkillIntValueAt( pcUser, Assassin_Cripple_PoisonChance_New, iLevelSkill1Based );

			value = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Cripple_PoisonNumStack_New, iLevelSkill1Based );
			value |= POISON_MAXSTACK << 8;
			psPacketEx->sElementalAttackSetting.sStack = value;

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_Wisp );
			if ( chain )
			{
				psPacketEx->eSecondaryAttackState = EAttackState::State_8_Cursed;

				psPacketEx->sSecondaryElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_PercentOfAP_New, chain, TRUE );
				psPacketEx->sSecondaryElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_Chance_New, chain, TRUE );
				psPacketEx->sSecondaryElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_Duration_New, chain, TRUE );
				psPacketEx->sSecondaryElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Assassin_Wisp_Curse_SpeedReduction_New, chain, TRUE );
				psPacketEx->sSecondaryElementalAttackSetting.sDamageReduction = GetSkillIntValueAt( pcUser,ESkillArrayPointer::Assassin_Wisp_Curse_DamageReduction, chain, TRUE);
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::NinjaShadow:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Ninja Shadow]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_NinjaShadow_DMGBoost, iLevelSkill1Based );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

			boost = ( ( attackPower * value ) / 100 );
			attackPower += boost;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_RunningHit );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Assassin_RunningHit_DMGBoost, chain, TRUE );
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Running Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_SoreBlade );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Assassin_SoreBlade_DMGBoost, chain, TRUE );
				boost = ( ( attackPower * value ) / 100 );
				attackPower += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Sore Blade Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}

		} break;

		case AssassinSkill::ShadowBlink:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Shadow Blink]");
			WRITEDWORD ( ESkillsVarsASM::CRITICAL_BOOST, iaASNT51CritBoost[iLevelSkill1Based - 1] );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower);
		} break;

		case AssassinSkill::RisingSlash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rising Slash]" );
			value = iaDMGBoostASST5S2[iLevelSkill1Based - 1];
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::ViolentStub:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Violent Stub]" );
			value = iaDMGBoostASST5S3[iLevelSkill1Based - 1];
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AssassinSkill::ShadowStorm:
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Shadow Storm]" );
			value = iaDMGBoostASST5S4[iLevelSkill1Based - 1];
			attackPower += ( ( attackPower * value ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
			break;

	}

	*piAttackPower = attackPower;

	return 1;
}

int CAssassin::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill1Based = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Assassin Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketRangeEx->iSkillID );
		return 0;
	}

	int value = 0;
	int chain, boost;

	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{
		case AssassinSkill::GustSlash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Gust Slash]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_GustSlash_DMGBoost, iLevelSkill1Based );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_Stinger );
			if ( chain )
			{
				value = GetSkillIntValueAt ( pcUser, Assassin_Stinger_DMGBoost, chain, TRUE );
				boost = ( ( psPacketRange->iDamage * value ) / 100 );
				psPacketRange->iDamage += boost;

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Stinger Chain [Lvl. %d]", chain );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage: Add [%d%%] Value [%d] Final [%d]", value, boost, attackPower );
			}

			psPacketRange->iRange = GetSkillIntValueAt ( pcUser, Assassin_GustSlash_Area, iLevelSkill1Based );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->sAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			break;
		}

		case AssassinSkill::SoulShock:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Soul Shock]" );
			psPacketRange->iRange = GetSkillIntValueAt ( pcUser, Assassin_SoulShock_Radius, iLevelSkill1Based );

			psPacketRange->eAttackState = EAttackState::State_2_Stun;
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_SoulShock_StunAtkPowPerc_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_SoulShock_StunTime, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_SoulShock_StunChance_New, iLevelSkill1Based );

			psPacketRange->iDamage = 0;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range = %d, Stun Time = %d", psPacketRange->iRange, psPacketRangeEx->sElementalAttackSetting.bDurationSec );
		} break;

		case AssassinSkill::Polluted:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Polluted]" );
			value = GetSkillIntValueAt ( pcUser, Assassin_Polluted_DMGBoost, iLevelSkill1Based );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * value ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * value ) / 100 ) );

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * value ) / 100 );
			psPacketRange->iCount = GetSkillIntValueAt ( pcUser, Assassin_Polluted_MaxHits, iLevelSkill1Based ) & 0xFF;
			psPacketRange->iRange = GetSkillIntValueAt ( pcUser, Assassin_Polluted_Range, iLevelSkill1Based );
			psPacketRange->eAttackState = EAttackState::State_106_Poison;
			psPacketRange->iAttackProperty = EAttackProperty::Poison;

			psPacketRangeEx->sElementalAttackSetting.bDurationSec	= GetSkillIntValueAt ( pcUser, Assassin_Polluted_PoisonDurationSec, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP	= GetSkillIntValueAt( pcUser, Assassin_Polluted_PoisonPercentOfAP, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance		= GetSkillIntValueAt( pcUser, Assassin_Polluted_PoisonChance_New, iLevelSkill1Based );

			value = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Assassin_Polluted_PoisonNumStack_New, iLevelSkill1Based );
			value |= POISON_MAXSTACK << 8;
			psPacketRangeEx->sElementalAttackSetting.sStack = value;

			chain = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_VenomThorn );
			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Venom Thorn Chain [Lvl. %d]", chain );

				value = GetSkillIntValueAt ( pcUser, Assassin_VenomThorn_PoisonDamagePercent_New, chain, TRUE );
				psPacketRangeEx->sElementalAttackSetting.sPercentOfAP += value;
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Add % of AP [+%d%%] Final [%d%%]", value, psPacketRangeEx->sElementalAttackSetting.sPercentOfAP );

				value = GetSkillIntValueAt ( pcUser, Assassin_VenomThorn_PoisonChance_New, chain, TRUE );
				psPacketRangeEx->sElementalAttackSetting.bChance += value;
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Add Posion Chance [+%d%%] Final [%d%%]", value, psPacketRangeEx->sElementalAttackSetting.bChance );
			}

		} break;

	}
	return 1;
}

int CAssassin::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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

	int value = 0;
	int duration;

	switch ( iSkillTinyID )
	{
		case AssassinSkill::VenomThorn:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Venom Thorn]" );
			duration = GetSkillIntValueAt ( pcUser, Assassin_VenomThorn_Duration_New, iLevelSkill1Based );
			SendSelfBuffSkill( pcUser, ESkillID::SKILLID_VenomThorn, iLevelSkill1Based, duration );

			pcUser->uVenomThornEndTime = TIME + duration;
			pcUser->bVenomLevel1Based = iLevelSkill1Based;

			break;
		}

		case AssassinSkill::Inpes:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Inpes]" );

			duration = GetSkillIntValueAt ( pcUser, Assassin_Inpes_Duration, iLevelSkill1Based );
			ApplyInpesBuff( pcUser, iLevelSkill1Based, duration );

			break;
		}

		case AssassinSkill::Alas:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Alas]" );

			duration = GetSkillIntValueAt ( pcUser, Assassin_Alas_Duration, iLevelSkill1Based );
			ApplyAlasBuff( pcUser, iLevelSkill1Based, duration );

			break;
		}

		case AssassinSkill::Blind:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Conceal]" );

			int iVisibility = GetSkillIntValueAt ( pcUser, Assassin_Blind_MonsterSight, iLevelSkill1Based );

			pcUserData->saVanishParam[1]		= ( WORD )iVisibility;
			pcUserData->saVanishParam[0]		= 0;

			UINT uTime = GetSkillIntValueAt ( pcUser, Assassin_Blind_Duration, iLevelSkill1Based );
			pcUser->uConcealEndTime = TIME + uTime;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Visibility [%d]", iVisibility );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Duration [%d]", uTime );
		} break;

	}
	return TRUE;
}

void CAssassin::OnAddFinalDamage( User * pcUser, PacketAttackData * psPacket )
{
}

void CAssassin::ApplyAlasBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bAlasLevel1Based )
		{
			pPartyUser->bAlasLevel1Based = iLevel1Based;
			pPartyUser->uAlasEndTime = TIME + iDurationSec;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_Alas, iLevel1Based, iDurationSec, 0, pBuffUsers );
}

void CAssassin::ApplyInpesBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bInpesLevel1Based )
		{
			pPartyUser->bInpesLevel1Based = iLevel1Based;
			pPartyUser->uInpesEndTime = TIME + iDurationSec;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_Inpes, iLevel1Based, iDurationSec, 0, pBuffUsers );
}

//Iron: This function is no longer used. use above instead
void CAssassin::OnUseWisp( User * pcUser, PacketWisp * psPacket )
{
	UserData * pcUserDataTarget = NULL;
	UnitData * pcUnitDataTarget = NULL;

	int iTime		= psPacket->iTime;
	int iAbs		= psPacket->iAbsorbPercent;
	int iResistance = 0;

	if ( psPacket->iTime == 0 || psPacket->iAbsorbPercent == 0 )
		return;

	if ( (pcUserDataTarget = USERDATABYID( psPacket->dwObjectID )) )
	{
		iResistance = pcUserDataTarget->sCharacterData.sElementalDef[EElementID::Organic];

		if ( iResistance < 0 )
			iResistance = 0;
		if ( iResistance > 100 )
			iResistance = 100;

		iTime	= (iTime*(100 - iResistance)) / 100;
		iAbs	= (iAbs*(100 - iResistance)) / 100;

		if ( iTime > 0 )
		{
			User * pcUserTarget = USERDATATOUSER( pcUserDataTarget );
			if ( pcUserTarget )
			{
				pcUserTarget->dwWispTime	= (iTime * 1000) + TICKCOUNT;
				pcUserTarget->iWispAbsorb	= iAbs;
			}
		}

	}
	else if ( (pcUnitDataTarget = UNITSERVER->GetUnit(psPacket->dwObjectID)) )
	{
		iResistance = pcUnitDataTarget->sCharacterData.sElementalDef[Organic];

		if ( iResistance < 0 )
			iResistance = 0;
		if ( iResistance > 100 )
			iResistance = 100;

		iTime	= iTime << 4;

		iTime	= (iTime*(100 - iResistance)) / 100;
		iAbs	= (iAbs*(100 - iResistance)) / 100;

		if ( iTime > 0 )
		{
			pcUnitDataTarget->sHolyMind[0] = (short)iAbs;
			pcUnitDataTarget->sHolyMind[1] = (short)iTime;
		}
	}
}
