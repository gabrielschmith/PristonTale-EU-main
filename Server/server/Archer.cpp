#include "stdafx.h"
#include "Archer.h"
#include "HNSSkill.h"

typedef UnitData*( __cdecl *t_SummonMobSkill ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkill SummonMobSkill = ( t_SummonMobSkill )0x0055E5A0;

DWORD dwLastSkillArcherID = 0;

CArcher::CArcher()
{
}

/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CArcher::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Archer )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CArcher::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CArcher::UpdateAndCacheServerSkillInfo (  )
{

	INFO( "Updating and caching skill info for Archer.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );
	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Archer );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CArcher::UpdateAndCacheServerSkillData ( )
{
	INFO( "Updating and caching skill data for Archer.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Archer;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE);
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CArcher::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass)
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass)
	{
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_ScoutHawk,		Archer_ScoutHawk_AddAtkRtg_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ScoutHawk,			Archer_ScoutHawk_AddAtkRtg_New,					"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ScoutHawk,			Archer_ScoutHawk_Rotations,						"NumberOfAttacks");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ScoutHawk,			Archer_ScoutHawk_UseMana,						"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShootingMastery,		Archer_ShootingMastery_AddWeaponDamage,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ShootingMastery,		Archer_ShootingMastery_AddWeaponDamagePercent,	"AttackPowerPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindArrow,			Archer_WindArrow_AddATKSpeed,					"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindArrow,			Archer_WindArrow_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_WindArrow,			Archer_WindArrow_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PerfectAim,			Archer_PerfectAim_AddAtkRtg,					"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PerfectAim,			Archer_PerfectAim_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PerfectAim,			Archer_PerfectAim_UseMana,						"MPCost");


		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DionsEye,			Archer_DionsEye_AddAtkRtg,						"AttackRatingPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Falcon,				Archer_Falcon_Damage,							"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Falcon,				Archer_Falcon_DamagePercent,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Falcon,				Archer_Falcon_Duration,							"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Falcon,				Archer_Falcon_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ArrowofRage,			Archer_ArrowofRage_DMGFlat,						"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ArrowofRage,			Archer_ArrowofRage_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ArrowofRage,			Archer_ArrowofRage_Area,						"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ArrowofRage,			Archer_ArrowofRage_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Avalanche,			Archer_Avalanche_AddATKSpeed,					"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Avalanche,			Archer_Avalanche_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Avalanche,			Archer_Avalanche_NumberofArrows,				"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Avalanche,			Archer_Avalanche_UseMana,						"MPCost");



		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_FireDamage,				"FireAttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_FireDamagePercent,			"FireAtackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_LightningDamage,			"LightingAttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_LightningDamagePercent,	"LightingAttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_LightningRange_New,		"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ElementalShot,		Archer_ElementalShot_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GoldenFalcon,		Archer_GoldenFalcon_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GoldenFalcon,		Archer_GoldenFalcon_FalconDamage,				"SummonAttackPowerMin");
		AppendFloatValuesToServerSkillDataBuffer ( temp, SKILLID_GoldenFalcon,		Archer_GoldenFalcon_HPRegen,					"HPRegen");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GoldenFalcon,		Archer_GoldenFalcon_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BombShot,			Archer_BombShot_Area,							"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BombShot,			Archer_BombShot_Damage,							"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BombShot,			Archer_BombShot_DMGDemons,						"VsDemonsExtraPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_BombShot,			Archer_BombShot_UseMana,						"MPCost");
		//SplashDmg not used?

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Perforation,			Archer_Perforation_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Perforation,			Archer_Perforation_AddShotRange,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Perforation,			Archer_Perforation_UseMana,						"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Perforation,			Archer_Perforation_AddedCritical_New,			"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Perforation,			Archer_Perforation_AttackWidth_New,				"AttackWidth");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_ATKPowMin,						"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_ATKPowMax,						"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_AtkRating,						"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_Defense,						"Defense");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_HP,							"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_UseMana,						"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_RecallWolverine,		Archer_Wolverine_Duration,						"Duration");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_EvasionMastery,	Archer_EvasionMastery_IncreaseEvasion_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_EvasionMastery,		Archer_EvasionMastery_IncreaseEvasion_New,		"EvasionChance");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_AttackRange_New,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_AttackWidth_New,				"AttackWidth");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_ChargingSpeed,				"ChargingSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_ChargedDMGBoost_New,			"ChargedAttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_BurningChance_New,			"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_BurningPercentOfAP_New,		"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhoenixShot,			Archer_PhoenixShot_BurningDurationSec_New,		"BurningDurationSec");

	}

	//party buff.
	AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,		Archer_ForceofNature_AddATKPow_DoNotUse);
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,			Archer_ForceofNature_AddATKPow_New,				"AttackPowerPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,			Archer_ForceofNature_AddAtkRtg,					"AttackRatingPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,			Archer_ForceofNature_Duration,					"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,			Archer_ForceofNature_UseMana,					"MPCost");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ForceofNature,			Archer_ForceofNature_AddedRange_New,			"Range");

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


int CArcher::GetForceOfNatureAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	if ( pcUserData->iMapID == MAPID_Bellatra &&
		 pcUserData->sCharacterData.iClass != CHARACTERCLASS_Archer )
	{
		if ( pcUser->iBellatraRoomInitialPlayerCount == 1 )
			return 0;
	}

	if ( pcUser->bForceOfNature1Based )
	{
		int iValue = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Archer_ForceofNature_AddATKPow_New, pcUser->bForceOfNature1Based );

		//half for non-archers
		if ( pcUser->GetCharacterClass() != ECharacterClass::CHARACTERCLASS_Archer )
			iValue >>= 1;

		return iValue;
	}

	return 0;
}

int CArcher::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat ( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Archer Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int iValue = 0;
	int chain;
	int attackPower = *piAttackPower;

	dwLastSkillArcherID = uSkillID;

	//Lethal Sight (T5)
	//if ( pcUser->dwLethalSightTimeOut > TICKCOUNT )
	//{
	//	if ( ( dwLastSkillArcherID != ArcherSkill::Falcon ) && ( dwLastSkillArcherID != ArcherSkill::GoldenFalcon ) )
	//	{
	//		WRITEDWORD ( ESkillsVarsASM::CRITICAL_BOOST, pcUser->uLethalSightCritical );
	//	}
	//}

	switch ( uSkillID )
	{
		case ArcherSkill::DefaultArcherAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;
		case ArcherSkill::WindArrow:
		{

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Wind Arrow]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_WindArrow_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;

			if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Normal)
					iValue += 30;

			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;

		case ArcherSkill::PerfectAim:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Perfect Aim]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_PerfectAim_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;


            if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Demon)
				iValue += 30;

			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		}break;

		case ArcherSkill::Falcon:
		{
			if ( pcUserData->dwFalconEndTime > TICKCOUNT )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Falcon]" );
				iValue = GetSkillIntValueAt ( pcUser, Archer_Falcon_DamagePercent, iLevelSkill );
				int iStaticDamage = GetSkillIntValueAt ( pcUser, Archer_Falcon_Damage, iLevelSkill );
				int iDamageBoost = attackPower;
				attackPower = iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );


				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Falcon Damage[%d]", attackPower );

				if ( pcUserData->saFalconDamage[0] == 2 )
				{
					chain = GetUserSkillLevel1Based( pcUser, SKILLID_GoldenFalcon );

					if ( chain )
					{
						iValue = GetSkillIntValueAt( pcUser, Archer_GoldenFalcon_FalconDamage, chain, TRUE );
						iDamageBoost = attackPower;
						attackPower += ( ( iDamageBoost * iValue ) / 100 );
						CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Golden Falcon Damage: %d", attackPower );
					}
				}

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );

				WRITEDWORD ( ESkillsVarsASM::CRITICAL_BOOST, 0 );
			}
		}break;

		case ArcherSkill::ElementalShot: //If it's 1vs1 then it's Fire
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Elemental Shot]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_ElementalShot_FireDamagePercent, iLevelSkill );
			int iStaticDamage = GetSkillIntValueAt ( pcUser, Archer_ElementalShot_FireDamage, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			attackPower += iStaticDamage;

			psPacketEx->eAttackState = EAttackState::State_100_Fire;
			psPacketEx->iAttackProperty = EAttackProperty::Fire;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		}break;

		case ArcherSkill::Avalanche:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Avalanche]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_Avalanche_DMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		}break;

		default:
			break;
	}


	*piAttackPower = attackPower;

	return 1;
}

int CArcher::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	//std::cout << "RecvSkillAreaTarget psPacketRange->iCount: " << psPacketRange->iCount << " psPacketRangeEx->iTargetCount: " << psPacketRangeEx->iTargetCount << std::endl;

	UserData * pcUserData = pcUser->pcUserData;


	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Archer Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{
		case ArcherSkill::ArrowofRage:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Arrow of Rage]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_ArrowofRage_DMGBoost, iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			iValue = GetSkillIntValueAt ( pcUser, Archer_ArrowofRage_DMGFlat, iLevelSkill );
			psPacketRange->iDamage += iValue;

			iValue = GetSkillIntValueAt ( pcUser, Archer_ArrowofRage_Area, iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}
		break;

		case ArcherSkill::ElementalShot:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Elemental Shot]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_ElementalShot_LightningDamagePercent, iLevelSkill );
			int iStaticDamage = GetSkillIntValueAt ( pcUser, Archer_ElementalShot_LightningDamage, iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 ) + iStaticDamage;
			iValue = GetSkillIntValueAt ( pcUser, Archer_ElementalShot_LightningRange_New, iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_105_Electricity;
			psPacketRange->iAttackProperty = EAttackProperty::Lightning;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}
		break;

		case ArcherSkill::BombShot:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Bomb Shot AoE]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_BombShot_Damage, iLevelSkill );
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->iRange = psPacketRangeEx->sAttackRange;
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			psPacketRange->iAttackProperty = EAttackProperty::Fire;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		}
		break;

		case ArcherSkill::Perforation:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Perforation]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_Perforation_DMGBoost, iLevelSkill );
			int iValueCritical = GetSkillIntValueAt ( pcUser, Archer_Perforation_AddedCritical_New, iLevelSkill );
			psPacketRangeEx->sCriticalChance = pcUserData->sCharacterData.iCritical + iValueCritical;
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ( ( iDamageBoost * iValue ) / 100 );
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			iValue = GetSkillIntValueAt ( pcUser, Archer_Perforation_AddShotRange, iLevelSkill );
			psPacketRange->iRange = psPacketRangeEx->sAttackRange + iValue;
			psPacketRangeEx->iRange = psPacketRangeEx->sAttackRange + iValue;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketRangeEx->sCriticalChance, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d] Total Range[%d]", psPacketRange->iDamage, psPacketRange->iRange);
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Attack Range[%d] Add Range[%d]", psPacketRangeEx->sAttackRange, iValue );
			psPacketRange->iCount &= 0x000000FF;

			psPacketRange->iAttackProperty = EAttackProperty::Wind;
		}
		break;

		case ArcherSkill::PhoenixShot: // When fully charged!
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Phoenix Shot]" );

			int iDamage = psPacketRange->iDamage;

			if ( sActiveSkill.sParam == 1 ) //charged
			{
				iValue = GetSkillIntValueAt ( pcUser, Archer_PhoenixShot_ChargedDMGBoost_New, iLevelSkill );
			}
			else
			{
				iValue = GetSkillIntValueAt ( pcUser, Archer_PhoenixShot_DMGBoost, iLevelSkill );
			}

			psPacketRange->iDamage = iDamage + ((iDamage * iValue) / 100);
			psPacketRange->eAttackState = EAttackState::State_100_Fire;
			psPacketRange->iRange += 120;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Charged[%d] Skill Damage[%d]", sActiveSkill.sParam, psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

			psPacketRange->iAttackProperty = EAttackProperty::Fire;

			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt ( pcUser, Archer_PhoenixShot_BurningChance_New, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt ( pcUser, Archer_PhoenixShot_BurningDurationSec_New, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt ( pcUser, Archer_PhoenixShot_BurningPercentOfAP_New, iLevelSkill );

		}break;
		case ArcherSkill::FierceWind: //Same as PHShot...
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fierce Wind]" );
			iValue = iaFierceWindDamageBoost[iLevelSkill-1];
			int iDamageBoost = psPacketRange->iDamage;
			psPacketRange->iDamage = iDamageBoost + ((iDamageBoost * iValue) / 100);
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			psPacketRange->iRange = 120 * 2;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

		}break;


		default:
			break;
	}


	psPacketRangeEx->sAttackRange = 0;

	return 1;
}

int CArcher::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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

	PacketUseSkillSingle sPacketSkillNew;

	switch ( iSkillTinyID )
	{
		case ArcherSkill::GoldenFalcon:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Golden Falcon]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_GoldenFalcon_Duration, iLevelSkill1Based );
			pcUserData->dwFalconEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->saFalconDamage[ 0 ] = 2;

		} break;

		case ArcherSkill::Falcon:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Falcon]" );
			iValue = GetSkillIntValueAt ( pcUser, Archer_Falcon_Duration, iLevelSkill1Based );
			pcUserData->dwFalconEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->saFalconDamage[ 0 ] = 1;
		} break;


		case ArcherSkill::ForceofNature:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Force of Nature]" );
			int iTime = GetSkillIntValueAt ( pcUser, Archer_ForceofNature_Duration, iLevelSkill1Based );

			//iValue = GetSkillIntValueAt ( pcUser, Archer_ForceofNature_AddATKPow_New, iLevelSkill1Based );

			ApplyForceOfNatureBuff( pcUser, iLevelSkill1Based, iTime );

			//these are no longer used
			pcUserData->dwForceOfNatureEndTime = 0;
			pcUserData->saForceOfNatureParam[0] = 0;
			pcUserData->saForceOfNatureParam[1] = 0;


		} break;

		case ArcherSkill::RecallWolverine:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Recall Wolverine]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkill( 0x4D, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_Duration, iLevelSkill1Based );
				pcUnitDataPet->dwUpdateCharInfoTime = ( iValue * 1000 ) + TICKCOUNT;

				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_ATKPowMin, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_ATKPowMax, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_AtkRating, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_Defense, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue + ( pcUserData->sCharacterData.iAgility * 2 );

				iValue = GetSkillIntValueAt ( pcUser, Archer_Wolverine_HP, iLevelSkill1Based );
				pcUnitDataPet->InitHealthPoint( iValue );

				pcUnitDataPet->sCharacterData.sSize = 192 + iLevelSkill1Based * 12;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;

				pcUnitDataPet->PartyFlag = 0x200;
			}
		} break;

		case SKILLPLAYID_T51:
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Lethal Sight]" );

			pcUser->uLethalSightLevel = iLevelSkill1Based;
			pcUser->uLethalSightTimeOut = 300 + TIME;
			pcUser->uLethalSightCritical = iaArcherT51CritRate[iLevelSkill1Based - 1];
			pcUser->uLethalSightDamageBoost = iaArcherT51DMGBoost[iLevelSkill1Based - 1];

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Time[%d] Level[%d]", 300, pcUser->uLethalSightLevel );

			sPacketSkillNew.iLength = sizeof( PacketUseSkillSingle );
			sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
			sPacketSkillNew.iSkillID = SKILLID_LethalSight;
			sPacketSkillNew.iSkillLevel1Based = pcUser->uLethalSightLevel;
			sPacketSkillNew.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Archer;
			sPacketSkillNew.lCasterID = pcUser->GetID();
			sPacketSkillNew.lTargetID = pcUser->GetID();
			SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL);
			break;

		//case SKILLPLAYID_T52:
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Fierce Wind]" );
		//
		//	if ( pcUserTarget )
		//	{
		//		pcUserTarget->uFierceWindLevel		= iLevelSkill1Based;
		//		pcUserTarget->dwFierceWindTimeOut	= (iaFierceWindTimeOut[iLevelSkill1Based - 1] * 1000) + TICKCOUNT;
		//
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Time[%d] Level[%d]", iaFierceWindTimeOut[iLevelSkill1Based - 1], pcUserTarget->uFierceWindLevel );
		//
		//		sPacketSkillNew.iLength = sizeof( PacketUseSkillSingle );
		//		sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
		//		sPacketSkillNew.iSkillID = SKILLID_FierceWind;
		//		sPacketSkillNew.iSkillLevel = pcUserTarget->uFierceWindLevel;
		//		sPacketSkillNew.lCasterID = pcUserData->iID;
		//		sPacketSkillNew.lTargetID = pcUserTarget->GetID();
		//		SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL);
		//	}
		//	if ( pcUnitTarget )
		//	{
		//		DWORD dwTimeOut = (iaFierceWindTimeOut[iLevelSkill1Based - 1] * 1000) + TICKCOUNT;
		//
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Time[%d] Level[%d]", dwTimeOut, iLevelSkill1Based );
		//
		//		sPacketSkillNew.iLength = sizeof( PacketUseSkillSingle );
		//		sPacketSkillNew.iHeader = PKTHDR_UseSkillNew;
		//		sPacketSkillNew.iSkillID = SKILLID_FierceWind;
		//		sPacketSkillNew.iSkillLevel = iLevelSkill1Based;
		//		sPacketSkillNew.lCasterID = pcUserData->iID;
		//		sPacketSkillNew.lTargetID = pcUnitTarget->GetID();
		//		SENDPACKETRANGE( &sPacketSkillNew, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL);
		//	}
		//	break;

		default:
			break;
	}
	return 1;
}

void CArcher::ApplyForceOfNatureBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bForceOfNature1Based )
		{
			pPartyUser->bForceOfNature1Based = iLevel1Based;
			pPartyUser->uForceOfNatureEndTime = TIME + iDurationSec;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_ForceofNature, iLevel1Based, iDurationSec, 0, pBuffUsers );
}

void CArcher::OnAddFinalDamage( User * pcUser, PacketAttackData * psPacket )
{
	UserData * pcUserData = pcUser->pcUserData;

	if ( (dwLastSkillArcherID == ArcherSkill::Falcon) || (dwLastSkillArcherID == ArcherSkill::GoldenFalcon) )
		return;


	//if ( pcUser->dwLethalSightTimeOut > TICKCOUNT )
	//{
	//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Lethal Sight Damage [%d + %d]", psPacket->iDamage, (psPacket->iDamage * pcUser->uLethalSightDamageBoost) / 100 );
	//	psPacket->iDamage += (psPacket->iDamage * pcUser->uLethalSightDamageBoost) / 100;
	//
	//	pcUser->dwLethalSightTimeOut = 0;
	//	pcUser->uLethalSightLevel = 0;
	//	pcUserData->saVanishParam[1] = 0;
	//	pcUserData->dwAssassinEyeEndTime = 0;
	//	DAMAGEHANDLER->SendCancelSkill( pcUser, SKILLPLAYID_T51 );
	//}

	dwLastSkillArcherID = 0;
}

CArcher::~CArcher()
{
}
