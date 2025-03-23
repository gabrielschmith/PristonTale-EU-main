#include "stdafx.h"
#include "Shaman.h"
#include "HNSSkill.h"


CShaman::CShaman()
{
}


CShaman::~CShaman()
{
}

typedef UnitData*( __cdecl *t_SummonMobSkillSHA ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillSHA SummonMobSkillSHA = ( t_SummonMobSkillSHA )0x0055E5A0;



/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CShaman::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Shaman )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CShaman::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Shaman )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CShaman::UpdateAndCacheServerSkillInfo ()
{
	INFO( "Updating and caching skill info for Shaman.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Shaman );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CShaman::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Shaman.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Shaman;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}


/// <summary>
/// Build skill data container using data from database
/// </summary>
void CShaman::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkBolt,			Shaman_DarkBolt_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkBolt,			Shaman_DarkBolt_Range,					"Range");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkBolt,			Shaman_DarkBolt_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkWave,			Shaman_DarkWave_ATKRange,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkWave,			Shaman_DarkWave_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkWave,			Shaman_DarkWave_Range,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DarkWave,			Shaman_DarkWave_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Curse_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Curse_SpeedReduction,	"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Curse_DamageReduction_New,	"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Curse_DamagePercent_New,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Curse_Chance_New,			"Chance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_Area,					"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Inertia,				Shaman_Inertia_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_InnerPeace,			Shaman_InnerPeace_ManaBoost,			"MPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_InnerPeace,			Shaman_InnerPeace_ManaPotionBoost,		"MPPotionPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualFlare,		Shaman_SpiritualFlare_ATKRange,			"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualFlare,		Shaman_SpiritualFlare_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualFlare,		Shaman_SpiritualFlare_Range,			"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualFlare,		Shaman_SpiritualFlare_UseMana,			"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualManacle,	Shaman_SpiritualManacle_StunTime,		"StunDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SpiritualManacle,	Shaman_SpiritualManacle_UseMana,		"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,		Shaman_ChasingHunt_AddAtkPowMin_DoNotUse );
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,		Shaman_ChasingHunt_AddAtkPowMax_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,			Shaman_ChasingHunt_AtkPowBoost_New,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,			Shaman_ChasingHunt_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,			Shaman_ChasingHunt_Range,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChasingHunt,			Shaman_ChasingHunt_UseMana,				"MPCost");


		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,		Shaman_Rainmaker_AddEvasion_DoNotUse );
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,		Shaman_Rainmaker_ReduceDamage_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,			Shaman_Rainmaker_AddEvasion_New,		"EvasionChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,			Shaman_Rainmaker_ReduceDamage_New,		"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,			Shaman_Rainmaker_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,			Shaman_Rainmaker_ReduceMP,				"MPReductionPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Rainmaker,			Shaman_Rainmaker_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhantomCall,			Shaman_PhantomCall_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhantomCall,			Shaman_PhantomCall_Range,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhantomCall,			Shaman_PhantomCall_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Haunt,				Shaman_Haunt_LifeAbsorb,				"AbsorbLife");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Haunt,				Shaman_Haunt_LifeAbsorbCap_New,			"HPReductionCap", FALSE, TRUE);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Haunt,				Shaman_Haunt_Radius_New,				"Range");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Haunt,				Shaman_Haunt_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Scratch,				Shaman_Scratch_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Scratch,				Shaman_Scratch_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_ATKPowMin,			"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_ATKPowMax,			"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_AtkRating,			"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_Defense,			"Defense");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_HP,				"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallHestian,		Shaman_SummonHestian_UseMana,			"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Judgement,			Shaman_Judgement_AddCritical,			"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Judgement,			Shaman_Judgement_DMGBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Judgement,			Shaman_Judgement_Range,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Judgement,			Shaman_Judgement_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMidranda,		Shaman_AdventMidranda_ATKSPDBoost,		"AttackSpeedPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMidranda,		Shaman_AdventMidranda_Duration,			"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMidranda,		Shaman_AdventMidranda_UseMana,			"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MourningPray,		Shaman_MourningPray_ATKRange,			"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MourningPray,		Shaman_MourningPray_DMGBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MourningPray,		Shaman_MourningPray_UseMana,			"MPCost");
	}

	//to everyone

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMigal,			Shaman_AdventMigal_AddATKPow,				"AttackPowerPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMigal,			Shaman_AdventMigal_Duration,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_AdventMigal,			Shaman_AdventMigal_UseMana,					"MPCost");

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



int CShaman::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill1Based = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Shaman Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketEx->iSkillCode );
		return 0;
	}

	int iValue = 0;
	int attackPower = *piAttackPower;

	switch ( uSkillID )
	{
		case ShamanSkill::ShamanDefaultAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case ShamanSkill::DarkBolt:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Dark Bolt]" );
			iValue = GetSkillIntValueAt( pcUser, Shaman_DarkBolt_DMGBoost, iLevelSkill1Based );
			attackPower += ((attackPower * iValue) / 100);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;

		case ShamanSkill::Scratch:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Scratch]" );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_Scratch_DMGBoost, iLevelSkill1Based );
			attackPower += ((attackPower * iValue) / 100);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;

		case ShamanSkill::SpiritualManacle:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spiritual Manacle]" );
			iValue = GetSkillIntValueAt( pcUser, Shaman_SpiritualManacle_StunTime, iLevelSkill1Based );


			psPacketEx->iAttackProperty = EAttackProperty::Organic;
			psPacketEx->eAttackState = EAttackState::State_2_Stun;
			attackPower = 0;
			psPacketEx->iRange = 150; //@todo - use db
			psPacketEx->iaParametersEx[0] = iValue;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range = %d, Stun Time = %d", psPacketEx->iRange, psPacketEx->iaParametersEx[0] );

			//normal players only
			if ( p_Target->IsTargetPlayer() )
			{
				int iResistance = p_Target->GetCharacterData()->sElementalDef[Organic];
				if ( iResistance < 0 )
					iResistance = 0;
				if ( iResistance >= 100 )
					iResistance = 100;

				iResistance = 100 - iResistance;

				int iStun = ((iValue * 70) * iResistance) / 100;

				DAMAGEHANDLER->SendDamagePvPData( pcUserData, p_Target->userData, 1, psPacketEx->iAttackRating, EAttackState::State_128_Skill, ESkillID::SKILLID_SpiritualManacle, -0x2B, iStun );

				attackPower = 0;
				psPacketEx->iRange = 0;
				return 0;
			}
		} break;

		case ShamanSkill::Haunt:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Haunt]" );

			attackPower = 0;
			psPacketEx->iRange = GetSkillIntValueAt( pcUser, Shaman_Haunt_Radius_New, iLevelSkill1Based );
			psPacketEx->eAttackState = EAttackState::State_4_LifeSteal;

			psPacketEx->sLifeStealParam.iLifeStealPercent = GetSkillIntValueAt( pcUser, Shaman_Haunt_LifeAbsorb, iLevelSkill1Based );
			psPacketEx->sLifeStealParam.iLifeStealHealthPointCap = GetSkillIntValueAt( pcUser, Shaman_Haunt_LifeAbsorbCap_New, iLevelSkill1Based );

		} break;
		case ShamanSkill::Judgement:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Judgement]" );
			iValue = GetSkillIntValueAt( pcUser, Shaman_Judgement_DMGBoost, iLevelSkill1Based );
			attackPower += ((attackPower * iValue) / 100);
			iValue = GetSkillIntValueAt( pcUser, Shaman_Judgement_Range, iLevelSkill1Based );
			psPacketEx->iRange = iValue;

			iValue = GetSkillIntValueAt( pcUser, Shaman_Judgement_AddCritical, iLevelSkill1Based );
			int iCritBoost = pcUserData->sCharacterData.iCritical + iValue;

			psPacketEx->sCriticalChance = iCritBoost;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Range[%d] Attack Rating[%d]", iCritBoost, psPacketEx->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;
	}

	*piAttackPower = attackPower;

	return TRUE;
}

int CShaman::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill1Based = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill1Based, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill1Based <= 0 || iLevelSkill1Based > 10 )
	{
		WARN ( "Shaman Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill1Based, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int attackPower = psPacketRange->iDamage;



	switch ( uSkillID )
	{
		case ShamanSkill::DarkWave:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Dark Wave]" );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_DarkWave_DMGBoost, iLevelSkill1Based );

			//int iRange = ( pcUser, Shaman_DarkWave_Range, iLevelSkill1Based );
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
			//						   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iRange );

			int iRange = ( pcUser, Shaman_DarkWave_Range, iLevelSkill1Based );
			CHATSERVER->SendDebugChat(pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ((psPacketRange->iDamage * iValue) / 100),
				psPacketRange->iDamage + ((psPacketRange->iDamage * iValue) / 100), iRange);

			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;

			int iRadius = GetSkillIntValueAt(pcUser, Shaman_DarkWave_ATKRange, iLevelSkill1Based);
			psPacketRange->iRange = iRadius;
		} break;

		case ShamanSkill::SpiritualFlare:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spiritual Flare]" );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_SpiritualFlare_DMGBoost, iLevelSkill1Based );

			int iRange = ( pcUser, Shaman_SpiritualFlare_Range, iLevelSkill1Based );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Damage [%d + %d][%d] Range [%d]", psPacketRange->iDamage, ( ( psPacketRange->iDamage * iValue ) / 100 ),
									   psPacketRange->iDamage + ( ( psPacketRange->iDamage * iValue ) / 100 ), iRange );

			psPacketRange->iDamage += ((psPacketRange->iDamage * iValue) / 100);
			psPacketRange->iRange = iValue + 60;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;

			int iRadius = GetSkillIntValueAt(pcUser, Shaman_SpiritualFlare_ATKRange, iLevelSkill1Based);
			psPacketRange->iRange = iRadius;
		} break;

		case ShamanSkill::Inertia:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Inertia]" );
			psPacketRange->iDamage = 0;
			psPacketRange->iRange = 120;
			psPacketRange->eAttackState = EAttackState::State_8_Cursed;
			psPacketRange->iCount &= 0x000000FF;

			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Shaman_Inertia_Curse_DamagePercent_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Shaman_Inertia_Curse_Chance_New, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Shaman_Inertia_Curse_Duration, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Shaman_Inertia_Curse_SpeedReduction, iLevelSkill1Based );
			psPacketRangeEx->sElementalAttackSetting.sDamageReduction = GetSkillIntValueAt( pcUser,ESkillArrayPointer::Shaman_Inertia_Curse_DamageReduction_New, iLevelSkill1Based );

		} break;
		case ShamanSkill::ChasingHunt:
		{
			if ( pcUser )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Chasing Hunt]" );
				iValue = GetSkillIntValueAt ( pcUser, Shaman_ChasingHunt_Range, iLevelSkill1Based );
				psPacketRange->iRange = iValue;
				psPacketRange->eAttackState = EAttackState::State_201_PullIn;
				iValue = GetSkillIntValueAt ( pcUser, Shaman_ChasingHunt_Duration, iLevelSkill1Based );
				pcUserData->dwHookedTargetEndTime = (iValue * 1000) + TICKCOUNT;
				pcUserData->iCompulsionParam = 0;
			}
		} break;

		case ShamanSkill::PhantomCall:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Phantom Call]" );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_PhantomCall_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_PhantomCall_Range, iLevelSkill1Based );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case ShamanSkill::MourningPray:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Mourning Pray]" );
			iValue = GetSkillIntValueAt ( pcUser, Shaman_MourningPray_DMGBoost, iLevelSkill1Based );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );

			iValue = GetSkillIntValueAt ( pcUser, Shaman_MourningPray_ATKRange, iLevelSkill1Based );
			psPacketRange->iRange = iValue;

			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

	}
	return TRUE;
}

int CShaman::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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
	int duration;

	switch ( iSkillTinyID )
	{
		case ShamanSkill::AdventMigal:
		{
			duration = GetSkillIntValueAt ( pcUser, Assassin_Inpes_Duration, iLevelSkill1Based );
			ApplyAdventMigalBuff( pcUser, iLevelSkill1Based, duration );
		}
		break;

		case ShamanSkill::RecallHestian:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Recall Bloody Knight]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillSHA( 0x10, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_Duration, iLevelSkill1Based );
				pcUnitDataPet->dwUpdateCharInfoTime = ( iValue * 1000 ) + TICKCOUNT;

				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_ATKPowMin, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_ATKPowMax, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_AtkRating, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_Defense, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Shaman_SummonHestian_HP, iLevelSkill1Based );
				pcUnitDataPet->InitHealthPoint( iValue );

				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill1Based * 12;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;

				pcUnitDataPet->PartyFlag = 0x200;
			}

		} break;

		case ShamanSkill::Rainmaker:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rainmaker]" );
			int iDuration = GetSkillIntValueAt ( pcUser, Shaman_Rainmaker_Duration, iLevelSkill1Based );
			pcUserData->dwEnergyShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			iValue = GetSkillIntValueAt ( pcUser, Shaman_Rainmaker_ReduceDamage_New, iLevelSkill1Based );
			pcUserData->iEnergyShieldDamageReduce = iValue;

		} break;

		case ShamanSkill::ChasingHunt:
			iValue = GetSkillIntValueAt ( pcUser, Shaman_ChasingHunt_Duration, iLevelSkill1Based );
			pcUser->uChasingHuntEndTime = TIME + iValue;
			pcUser->iChasingHuntAttackPowerBoost = GetSkillIntValueAt( pcUser, Shaman_ChasingHunt_AtkPowBoost_New, iLevelSkill1Based );
			break;

		case SKILLPLAYID_T51:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Creed]" );
			UserData * pcUserDataTarget = USERSERVER->GetUserdata( *(UINT*)(packet + 0x10) );
			if ( pcUserDataTarget )
			{
				*(UINT*)(packet + 8) = JOBCODEID_Shaman;
				SENDPACKET( USERDATATOUSER( pcUserDataTarget ), (void*)packet );
			}
		}; break;

	}
	return TRUE;
}

void CShaman::ApplyAdventMigalBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bAdventMigalLevel1Based )
		{
			pPartyUser->bAdventMigalLevel1Based = iLevel1Based;
			pPartyUser->uAdventMigalEndTime = TIME + iDurationSec;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_AdventMigal, iLevel1Based, iDurationSec, 0, pBuffUsers );
}


int CShaman::GetAdventMigalAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	if ( pcUserData->iMapID == MAPID_Bellatra &&
		 pcUserData->sCharacterData.iClass != CHARACTERCLASS_Shaman )
	{
		if ( pcUser->iBellatraRoomInitialPlayerCount == 1 )
			return 0;
	}

	if ( pcUser->bAdventMigalLevel1Based )
	{
		int iValue = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Shaman_AdventMigal_AddATKPow, pcUser->bAdventMigalLevel1Based );

		if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Shaman )
			iValue >>= 1;

		return iValue;
	}

	return 0;
}

int CShaman::GetChasingHuntAtkPowBoost( User * pcUser )
{
	if ( pcUser->uChasingHuntEndTime > TIME )
	{
		return pcUser->iChasingHuntAttackPowerBoost;
	}

	return 0;
}
