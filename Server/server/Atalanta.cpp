#include "stdafx.h"
#include "Atalanta.h"
#include "HNSSkill.h"

typedef void( __cdecl *tfnSendSkillDebuff )(UserData * pcUserData, UserData * pcUserDataTarget, int iUnk, int iu, int iSkillID, int iTime);
tfnSendSkillDebuff fnSendSkillDebuff = (tfnSendSkillDebuff)0x00401FF4;

CAtalanta::CAtalanta()
{
}

/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CAtalanta::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Atalanta )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CAtalanta::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Atalanta )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CAtalanta::UpdateAndCacheServerSkillInfo ()
{

	INFO( "Updating and caching skill info for Atalanta.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Atalanta );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CAtalanta::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Atalanta.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Atalanta;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CAtalanta::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_ATKPowMin,				"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_ATKPowMax,				"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_StunDamage,				"StunAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_StunChance,				"StunChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShieldStrike,		Atalanta_ShieldStrike_StunDurationSec,			"StunDurationSec");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Farina,				Atalanta_Farina_AddAtkRtg,						"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Farina,				Atalanta_Farina_AddATKSpeed,					"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Farina,				Atalanta_Farina_DMGBoost,						"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Farina,				Atalanta_Farina_DMGBoostPercent_New,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Farina,				Atalanta_Farina_UseMana,						"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ThrowingMastery,		Atalanta_ThrowingMastery_DMGBoost_New,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ThrowingMastery,		Atalanta_ThrowingMastery_DMGBoostPercent_New,	"AttackPowerMaxPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BiggerSpear,			Atalanta_BiggerSpear_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BiggerSpear,			Atalanta_BiggerSpear_UseMana,					"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_Windy,           Atalanta_Windy_AtkRtgBoost_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Windy,				Atalanta_Windy_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Windy,				Atalanta_Windy_UseMana,							"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Windy,				Atalanta_Windy_Range_New,						"Range");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Windy,               Atalanta_Windy_AddAttackRating_New,             "AttackRatingPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindJavelin,		    Atalanta_WindJavelin_AtkRtgBoost,				"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindJavelin,		    Atalanta_WindJavelin_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindJavelin,		    Atalanta_WindJavelin_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulSucker,			Atalanta_SoulSucker_LifeAbsorb,					"AbsorbLife");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulSucker,			Atalanta_SoulSucker_LifeAbsorbCap_New,			"HPReductionCap", FALSE, TRUE);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulSucker,			Atalanta_SoulSucker_Range_New,					"Range");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SoulSucker,			Atalanta_SoulSucker_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,			Atalanta_FireJavelin_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,			Atalanta_FireJavelin_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,         Atalanta_FireJavelin_BurningChance,             "BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,         Atalanta_FireJavelin_BurningDamagePercent,      "BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,         Atalanta_FireJavelin_BurningDurationSec,        "BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FireJavelin,         Atalanta_FireJavelin_BurningTickRateMs,         "BurningTickRateMs");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SplitJavelin,		Atalanta_SplitJavelin_AtkRtgBoost,				"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SplitJavelin,		Atalanta_SplitJavelin_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SplitJavelin,		Atalanta_SplitJavelin_NumberOfAttacks,			"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SplitJavelin,		Atalanta_SplitJavelin_UseMana,					"MPCost");


		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_LightningJavelin,	Atalanta_LightningJavelin_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_LightningJavelin,	Atalanta_LightningJavelin_Radius_New,			"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_LightningJavelin,	Atalanta_LightningJavelin_UseMana,				"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_LightningJavelin,	Atalanta_LightningJavelin_StunChance,			"StunChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_LightningJavelin,	Atalanta_LightningJavelin_StunDurationSec,		"StunDurationSec");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_StormJavelin,		Atalanta_StormJavelin_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_StormJavelin,		Atalanta_StormJavelin_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ExtremeRage,			Atalanta_ExtremeRage_Area,						"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ExtremeRage,			Atalanta_ExtremeRage_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ExtremeRage,			Atalanta_ExtremeRage_UseMana,					"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer(temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_FreezeChance_DoNotUse);
		AppendZeroIntValuesToServerSkillDataBuffer(temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_FreezeDamage_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,        Atalanta_FrostJavelin_FreezeDamage_New,         "FreezeAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,        Atalanta_FrostJavelin_FreezeChance_New,         "FreezeChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_SpeedMonDown,				"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_FreezeDuration,			"FreezeDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_FrostJavelin,		Atalanta_FrostJavelin_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vengeance,			Atalanta_Vengeance_AddCritical,					"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vengeance,			Atalanta_Vengeance_AddRange,					"Range");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vengeance,			Atalanta_Vengeance_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Vengeance,			Atalanta_Vengeance_UseMana,						"MPCost");

	}

	AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_TriumphofValhalla,	Atalanta_TriumphOfValhalla_AddMaxAtkPow_DoNotUse);
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_TriumphofValhalla,	Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New,		"AttackPowerMax");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_TriumphofValhalla,	Atalanta_TriumphOfValhalla_Duration,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_TriumphofValhalla,	Atalanta_TriumphOfValhalla_UseMana,					"MPCost");

	AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_HallofValhalla,		Atalanta_HallOfValhalla_AddEvasion_DoNotUse );
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HallofValhalla,		Atalanta_HallOfValhalla_AddBlock_New,				"BlockChance");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HallofValhalla,		Atalanta_HallOfValhalla_Duration,					"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HallofValhalla,		Atalanta_HallOfValhalla_UseMana,					"MPCost");

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


int CAtalanta::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Atalanta Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int iValue = 0;
	int attackPower = *piAttackPower;

	switch ( uSkillID )
	{
		case AtalantaSkill::AtalantaDefaultAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case AtalantaSkill::Farina:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Farina]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_Farina_DMGBoostPercent_New, iLevelSkill );

			int iBaseAttackPower = attackPower; //base damage

			attackPower = iBaseAttackPower + ( ( iBaseAttackPower * iValue ) / 100 );

			iValue = GetSkillIntValueAt ( pcUser, Atalanta_Farina_DMGBoost, iLevelSkill );
			attackPower += iValue; //additive

			if ( p_Target->IsTargetUnit() && p_Target->GetMonsterType() == EMonsterType::MONSTERTYPE_Normal  )
			{
				attackPower += ( ( iBaseAttackPower * 20 ) / 100 );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +20%% Normal Monster" );
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AtalantaSkill::BiggerSpear:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Bigger Spear]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_BiggerSpear_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == EMonsterType::MONSTERTYPE_Normal )
            {
                attackPower += ((iDamageBoost * 30) / 100);
                CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Normal Monster");
            }
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;


		case AtalantaSkill::WindJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Twist Javelin]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_WindJavelin_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Demon )
            {
                attackPower += ((iDamageBoost * 30) / 100);
                CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Demon Monster");
            }
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AtalantaSkill::FireJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fire Javelin]" );

			const int fireJavelinDamageBoost = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_DMGBoost, iLevelSkill);
			const int fireJavelinBurningChance = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningChance, iLevelSkill);
			const int fireJavelinBurningDuration = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningDurationSec, iLevelSkill);
			const int fireJavelinBurningDamagePercent = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningDamagePercent, iLevelSkill);
			const int fireJavelingBaseDamage = attackPower;

			attackPower = fireJavelingBaseDamage + ( (fireJavelingBaseDamage * fireJavelinDamageBoost) / 100 );
			psPacketEx->iAttackProperty = EAttackProperty::Fire;

			psPacketEx->sElementalAttackSetting.bChance = fireJavelinBurningChance;
			psPacketEx->sElementalAttackSetting.bDurationSec = fireJavelinBurningDuration;
			psPacketEx->sElementalAttackSetting.sPercentOfAP = fireJavelinBurningDamagePercent;

			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Mutant )
			{
				attackPower += ( (fireJavelingBaseDamage * 30 ) / 100 );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Mutant Monster" );
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AtalantaSkill::SplitJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Split Javelin]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_SplitJavelin_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;


		case AtalantaSkill::LightningJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Lightning Javelin]" );

			const int lightningJavelinBaseDamage = attackPower;
			const int lightningJavelinDamageBoost = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_DMGBoost, iLevelSkill);
			const int lightningJavelinStunChance = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_StunChance, iLevelSkill);
			const int lightningJavelinStunDurationSec = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_StunDurationSec, iLevelSkill);

			attackPower = lightningJavelinBaseDamage + ( (lightningJavelinBaseDamage * lightningJavelinDamageBoost) / 100 );
			psPacketEx->iAttackProperty = EAttackProperty::Lightning;

			psPacketEx->sElementalAttackSetting.bChance = lightningJavelinStunChance;
			psPacketEx->sElementalAttackSetting.bDurationSec = lightningJavelinStunDurationSec;

			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Undead )
			{
				attackPower += ( (lightningJavelinBaseDamage * 30 ) / 100 );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Undead Monster" );
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case AtalantaSkill::Vengeance:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Vengeance]" );
			int vengeanceDamageBoost = GetSkillIntValueAt ( pcUser, Atalanta_Vengeance_DMGBoost, iLevelSkill );

			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * vengeanceDamageBoost) / 100 );

			int vengeanceCriticalBoost = GetSkillIntValueAt ( pcUser, Atalanta_Vengeance_AddCritical, iLevelSkill );
			int iCritBoost = pcUserData->sCharacterData.iCritical + ( ( ( pcUserData->sCharacterData.iCritical * vengeanceCriticalBoost) / 100 ) );
			psPacketEx->sCriticalChance = iCritBoost;

			int extremeRageLevel = GetUserSkillLevel1Based ( pcUser, ESkillID::SKILLID_ExtremeRage );
			int extremeRageDamageBoost = GetSkillIntValueAt( pcUser, Atalanta_ExtremeRage_DMGBoost, extremeRageLevel, TRUE  );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Extreme Rage chain level [%d]", extremeRageLevel);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Extreme Rage damageBoost [%d]", extremeRageDamageBoost);

			attackPower += ((iDamageBoost * extremeRageDamageBoost) / 100);

			int splitJavalinLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_SplitJavelin );
			int splitJavelinDamageBoost = GetSkillIntValueAt( pcUser, Atalanta_SplitJavelin_DMGBoost, splitJavalinLevel, TRUE  );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Split Javelin chain level [%d]", splitJavalinLevel);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Split Javelin damageBoost [%d]", splitJavelinDamageBoost);

			attackPower += ( ( iDamageBoost * splitJavelinDamageBoost ) / 100);

			//AddedRange
			//iValue = SQLSKILL->GetSkillValueInt ( "SELECT AddedRange FROM ATA_Vengeance WHERE SkillLevel=%d", iLevelSkill );
			//@todo - can we get character's attack range on server side?

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;


		default:
			break;

	}

	*piAttackPower = attackPower;

	return 1;

}

int CAtalanta::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Atalanta Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int chain;

	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{
		case AtalantaSkill::ShieldStrike:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Shield Strike AoE]" );

			const int shieldStrikeBaseDamage = psPacketRange->iDamage;
			const int shieldStrikeMinDamage = GetSkillIntValueAt ( pcUser, Atalanta_ShieldStrike_ATKPowMin, iLevelSkill );
			const int shieldStrikeMaxDamage = GetSkillIntValueAt ( pcUser, Atalanta_ShieldStrike_ATKPowMax, iLevelSkill );
			const int shieldStrikeAddedDamage = Dice::RandomI(shieldStrikeMinDamage, shieldStrikeMaxDamage);

			psPacketRange->iDamage = shieldStrikeBaseDamage + shieldStrikeAddedDamage;
			psPacketRange->iAttackProperty = EAttackProperty::Organic;
			psPacketRange->eAttackState = EAttackState::State_2_Stun;

			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Atalanta_ShieldStrike_StunDamage, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt ( pcUser,ESkillArrayPointer::Atalanta_ShieldStrike_StunDurationSec, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Atalanta_ShieldStrike_StunChance, iLevelSkill );

			psPacketRange->iCount &= 0x000000FF;
		} break;

		case AtalantaSkill::SoulSucker:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Soul Sucker AoE]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_SoulSucker_LifeAbsorb, iLevelSkill );
			psPacketRange->iDamage = 0;
			psPacketRange->eAttackState = EAttackState::State_4_LifeSteal;
			psPacketRange->iRange = GetSkillIntValueAt ( pcUser, Atalanta_SoulSucker_Range_New, iLevelSkill );
			psPacketRangeEx->sLifeStealParam.iLifeStealPercent = iValue;
			psPacketRangeEx->sLifeStealParam.iLifeStealHealthPointCap = GetSkillIntValueAt ( pcUser, Atalanta_SoulSucker_LifeAbsorbCap_New, iLevelSkill );
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case AtalantaSkill::StormJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Storm Javelin AoE]" );

			const int stormJavelinDamageBoost = GetSkillIntValueAt ( pcUser, Atalanta_StormJavelin_DMGBoost, iLevelSkill );
			const int stormJavelinBaseDamage = psPacketRange->iDamage;

			psPacketRange->iDamage = stormJavelinBaseDamage + ( (stormJavelinBaseDamage * stormJavelinDamageBoost) / 100 );
			psPacketRange->iRange = 300;

			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			psPacketRange->iCount &= 0x000000FF;

			const int windJavelinLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_WindJavelin);
			if ( windJavelinLevel )
			{
				int windJavelinDamageBoost = GetSkillIntValueAt ( pcUser, Atalanta_WindJavelin_DMGBoost, windJavelinLevel, TRUE);

				psPacketRange->iDamage += ((stormJavelinBaseDamage * windJavelinDamageBoost) / 100);

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Chain Damage Boost[%d]", windJavelinDamageBoost);
			}

			const int lightningJavelinLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_LightningJavelin );
			if ( lightningJavelinLevel )
			{
				const int lightningJavelinRange = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_Radius_New, lightningJavelinLevel);

				psPacketRange->iRange += lightningJavelinRange;
			}

			const int fireJavelinLevel = GetUserSkillLevel1Based( pcUser, ESkillID::SKILLID_FireJavelin );
			if ( fireJavelinLevel )
			{
				const int fireJavelinBurningChance = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningChance, fireJavelinLevel);
				const int fireJavelinBurningDuration = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningDurationSec, fireJavelinLevel);
				const int fireJavelinBurningDamagePercent = GetSkillIntValueAt(pcUser, Atalanta_FireJavelin_BurningDamagePercent, fireJavelinLevel);

				psPacketRange->iAttackProperty = EAttackProperty::Fire;

				psPacketRangeEx->sElementalAttackSetting.bChance = fireJavelinBurningChance;
				psPacketRangeEx->sElementalAttackSetting.bDurationSec = fireJavelinBurningDuration;
				psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = fireJavelinBurningDamagePercent;
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->sAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}break;

		case AtalantaSkill::ExtremeRage:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Extreme Rage AoE]" );
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_ExtremeRage_DMGBoost, iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			chain = GetUserSkillLevel1Based ( pcUser, ESkillID::SKILLID_SplitJavelin );

			if ( chain )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Chain Level [%d]", chain );

				iValue = GetSkillIntValueAt ( pcUser, Atalanta_SplitJavelin_DMGBoost, chain, TRUE);
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Chain Damage Boost[%d]", iValue );
				psPacketRange->iDamage += ( ( iDamageBoost * iValue ) / 100 );
			}

			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			iValue = GetSkillIntValueAt ( pcUser, Atalanta_ExtremeRage_Area, iLevelSkill );
			psPacketRange->iRange = iValue;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Total Damage[%d]", psPacketRange->iDamage );
		}break;

		case AtalantaSkill::LightningJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Lightning Javelin AoE]");

			const int lightningJavelinBaseDamage = psPacketRange->iDamage;
			const int lightningJavelinDamageBoost = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_DMGBoost, iLevelSkill);
			const int lightningJavelinRange = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_Radius_New, iLevelSkill);
			const int lightningJavelinStunChance = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_StunChance, iLevelSkill);
			const int lightningJavelinStunDurationSec = GetSkillIntValueAt(pcUser, Atalanta_LightningJavelin_StunDurationSec, iLevelSkill);

			psPacketRange->iDamage = lightningJavelinBaseDamage + ( (lightningJavelinBaseDamage * lightningJavelinDamageBoost) / 100 );
			psPacketRange->iRange = lightningJavelinRange;
			psPacketRange->eAttackState = EAttackState::State_105_Electricity;

			psPacketRange->iAttackProperty = EAttackProperty::Lightning;
			psPacketRangeEx->sElementalAttackSetting.bChance = lightningJavelinStunChance;
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = lightningJavelinStunDurationSec;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketRangeEx->sAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}

		default:
			break;
	}

	return 1;
}


int CAtalanta::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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
		case AtalantaSkill::TriumphofValhalla:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Triumph of Valhalla]" );

			const int triumphOfValhallaDuration = GetSkillIntValueAt(pcUser, Atalanta_TriumphOfValhalla_Duration, iLevelSkill1Based);
			SendTriumphOfValhallaBuff( pcUser, iLevelSkill1Based, triumphOfValhallaDuration );

		} break;

		case AtalantaSkill::HallofValhalla:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Hall of Valhalla]" );

			const int hallOfValhallaDuration = GetSkillIntValueAt(pcUser, Atalanta_HallOfValhalla_Duration, iLevelSkill1Based);
			SendHallOfValhallaBuff( pcUser, iLevelSkill1Based, hallOfValhallaDuration );

		} break;

		case AtalantaSkill::FrostJavelin:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Frost Javelin]" );
			int frostJavelinSkillDuration = GetSkillIntValueAt ( pcUser, Atalanta_FrostJavelin_Duration, iLevelSkill1Based );
			int frostJavelinFreezeDuration = GetSkillIntValueAt ( pcUser, Atalanta_FrostJavelin_FreezeDuration, iLevelSkill1Based );
			int frostJavelinFreezeChance = GetSkillIntValueAt ( pcUser, Atalanta_FrostJavelin_FreezeChance_New, iLevelSkill1Based );
			int frostJavelinFreezeAtkPowerPercent = GetSkillIntValueAt(pcUser, Atalanta_FrostJavelin_FreezeDamage_New, iLevelSkill1Based);
			int frostJavelinMonsterSpeedDown = GetSkillIntValueAt ( pcUser, Atalanta_FrostJavelin_SpeedMonDown, iLevelSkill1Based );

			pcUser->uFrostJavelinEndTime = TIME + frostJavelinSkillDuration;
			pcUser->bFrostJavelinDurationSec = frostJavelinFreezeDuration;
			pcUser->sFrostJavelinAtkPowerPercent = frostJavelinFreezeAtkPowerPercent;
			pcUser->bFrostJavelinChance = frostJavelinFreezeChance;
			pcUser->bFrostJavelinWeaponMonsterSlowdown = frostJavelinMonsterSpeedDown;

		} break;

		case AtalantaSkill::SummonTiger:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Summon Tiger]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = UNITSERVER->CreateUnitEnemy( "Summon Tiger", pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData );

			if ( pcUnitData )
			{
				pcUnitData->sUnitInfo.dwHashCharacterName = 0;
				pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Summon;
				pcUnitData->sCharacterData.iOwnerID = pcUserData->iID;

				pcUnitData->InitHealthPoint( iaSummonTigerHP[iLevelSkill1Based - 1] );
				pcUnitData->sCharacterData.iMinDamage		= iaSummonTigerAttackPower[iLevelSkill1Based - 1];
				pcUnitData->sCharacterData.iMaxDamage		= iaSummonTigerAttackPower[iLevelSkill1Based - 1];
				pcUnitData->sCharacterData.iAttackRating	= iaSummonTigerAttackRating[iLevelSkill1Based - 1];

				pcUnitData->pcOwner = pcUserData;
				pcUnitData->dwUpdateCharInfoTime = TICKCOUNT + (300 * 1000);

				pcUnitData->sCharacterData.sSize = 224 + iLevelSkill1Based * 15;

				pcUnitData->sCharacterData.iAbsorbRating = 50;

				pcUnitData->saMoveStepCount[0] = 1;
				pcUnitData->saMoveStepCount[1] = 2;

				pcUserData->pcPetData = pcUnitData;
				pcUserData->dwObjectPetID = pcUnitData->iID;
				pcUnitData->iPetID = SKILLID_SummonTiger;

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData, SKILLID_SummonTiger, 300 );

				//User Warp Effect summon crystal
				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID = 0x100;
				sPacketEffect.iX = pcUserData->sPosition.iX;
				sPacketEffect.iY = pcUserData->sPosition.iY;
				sPacketEffect.iZ = pcUserData->sPosition.iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );

				return TRUE;
			}
		}
		break;

		default:
			break;
	}
	return 1;
}

BOOL CAtalanta::SendHallOfValhallaBuff(User * pcCaster, int iLevel1Based, int iDurationSec )
{
	time_t endTime = TIME + iDurationSec;
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters )) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bHallOfValhallaLevel1Based )
		{
			pPartyUser->bHallOfValhallaLevel1Based = iLevel1Based;
			pPartyUser->uHallOfValhallaEndTime = endTime;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_HallofValhalla, iLevel1Based, iDurationSec, 0, pBuffUsers );

	return TRUE;
}

BOOL CAtalanta::SendTriumphOfValhallaBuff(User * pcCaster, int iLevel1Based, int iDurationSec)
{
	time_t endTime = TIME + iDurationSec;
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters )) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bTriumphOfValhallaLevel1Based )
		{
			pPartyUser->bTriumphOfValhallaLevel1Based = iLevel1Based;
			pPartyUser->uTriumphOfValhallaEndTime = endTime;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_TriumphofValhalla, iLevel1Based, iDurationSec, 0, pBuffUsers );

	return TRUE;
}


int CAtalanta::GetTriumphofValhallaAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	if ( pcUserData->iMapID == MAPID_Bellatra &&
		pcUserData->sCharacterData.iClass != CHARACTERCLASS_Atalanta )
	{
		if ( pcUser && pcUser->iBellatraRoomInitialPlayerCount == 1 )
		{
			return 0;
		}
	}

	int iValue = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New, pcUser->bTriumphOfValhallaLevel1Based );

	//half for non-ata
	if ( pcUser->GetCharacterClass() != ECharacterClass::CHARACTERCLASS_Atalanta )
		iValue >>= 1;

	return iValue;
}

CAtalanta::~CAtalanta()
{
}
