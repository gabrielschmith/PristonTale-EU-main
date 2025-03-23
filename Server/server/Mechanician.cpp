#include "stdafx.h"
#include "Mechanician.h"
#include "HNSSkill.h"


typedef void( __cdecl *t_SendDeBuffToPlayers ) ( UserData * pcUserData, void * packet );
t_SendDeBuffToPlayers SendDeBuffToPlayers = ( t_SendDeBuffToPlayers )0x00552210;

typedef UnitData*( __cdecl *t_SummonMobSkillMS ) ( int iSkillID, int X, int Y, int Z, UserData * pcUserData, int v0 );
t_SummonMobSkillMS SummonMobSkillMS = ( t_SummonMobSkillMS )0x0055E5A0;


CMechanician::CMechanician()
{
}


/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CMechanician::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Mechanician )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CMechanician::SendServerSkillDataToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Mechanician )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build
/// </summary>
void CMechanician::UpdateAndCacheServerSkillInfo ()
{
	INFO( "Updating and caching skill info for Mechanician.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Mechanician );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CMechanician::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Mechanician.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Mechanician;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CMechanician::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ExtremeShield,		Mech_ExtremeShield_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ExtremeShield,			Mech_ExtremeShield_AddBlock_New,					"BlockChance");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ExtremeShield,			Mech_ExtremeShield_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ExtremeShield,			Mech_ExtremeShield_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_Area,								"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_AttackRange_New,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_AttackFrameSpeed_New,				"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_Damage,							"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_DamagePercent,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_UseMana,							"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_BurningChance,					"BurningChance");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_BurningDamagePercent,				"BurningAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_BurningDurationSec,				"BurningDurationSec");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicBomb,			Mech_MechanicBomb_BurningTickRateMs,				"BurningTickRateMs");

		AppendZeroIntValuesToServerSkillDataBuffer(temp, SKILLID_ElectricalOverload, Mech_PoisonAttribute_Poison_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ElectricalOverload,		Mech_ElectricalOverload_AttackSpeed,				"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_ElectricalOverload,		Mech_ElectricalOverload_Duration,					"Duration");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_PhysicalAbsorb,			Mech_PhysicalAbsorption_ABSBoost,					"Absorption");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_PhysicalAbsorb,			Mech_PhysicalAbsorption_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_PhysicalAbsorb,			Mech_PhysicalAbsorption_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GreatSmash,				Mech_GreatSmash_AddDamage,							"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GreatSmash,				Mech_GreatSmash_AddDamagePercent,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GreatSmash,				Mech_GreatSmash_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Maximize,				Mech_Maximize_AddATKPow,							"AttackPowerMaxPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Maximize,				Mech_Maximize_AtkRtgBoost,							"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Maximize,				Mech_Maximize_ATKSPDBoost_New,						"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Maximize,				Mech_Maximize_Duration,								"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Maximize,				Mech_Maximize_UseMana,								"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Automation,				Mech_Automation_AddATKSpeed,						"AttackSpeed");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Automation,				Mech_Automation_DMGBoost,							"AttackPowerPercent");
		//AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Automation,				Mech_Automation_MPDrain_New,						"MPDrain");
		//AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Automation,				Mech_Automation_SPDrain_New,						"SPDrain");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Automation,				Mech_Automation_Duration,							"Duration");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Spark,					Mech_Spark_Area,									"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Spark,					Mech_Spark_DMGBoostFlat,							"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Spark,					Mech_Spark_DMGBoostPercent,							"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Spark,					Mech_Spark_UseMana,									"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Spark,					Mech_Spark_AttackFrameSpeed_New,					"AttackSpeed");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalArmor,				Mech_MetalArmor_AddDefense,							"DefensePercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalArmor,				Mech_MetalArmor_Duration,							"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalArmor,				Mech_MetalArmor_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GrandSmash,				Mech_GrandSmash_AtkRtgBoost,						"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GrandSmash,				Mech_GrandSmash_DMGBoost,							"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_GrandSmash,				Mech_GrandSmash_UseMana,							"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicWeaponMastery,	Mech_MechanicWeaponMastery_Obsolete);
		//AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicWeaponMastery,	Mech_MechanicWeaponMastery_AddWeaponDamage_New,		"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MechanicWeaponMastery,	Mech_MechanicWeaponMastery_AddWeaponDamagePercent,	"AttackPowerPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_SparkShield,			Mech_SparkShield_AddDefense,						"Defense");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_SparkShield,			Mech_SparkShield_Duration,							"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_SparkShield,			Mech_SparkShield_SparkDamage,						"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_SparkShield,			Mech_SparkShield_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Impulsion,				Mech_Impulsion_Area,								"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Impulsion,				Mech_Impulsion_DMGBoost,							"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Impulsion,				Mech_Impulsion_MaxVolts,							"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Impulsion,				Mech_Impulsion_UseMana,								"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,				Mech_Compulsion_ExtraAbsorb_New,					"Absorption");
		AppendZeroIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,			Mech_Compulsion_AddAbsorbPercent_Old);
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,				Mech_Compulsion_AddAbsorbPercent_New,				"AbsorptionPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,				Mech_Compulsion_Duration,							"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,				Mech_Compulsion_Range,								"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_Compulsion,				Mech_Compulsion_UseMana,							"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MagneticSphere,			Mech_MagneticSphere_AtkDelay,						"AttackDelay");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MagneticSphere,			Mech_MagneticSphere_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MagneticSphere,			Mech_MagneticSphere_Range,							"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MagneticSphere,			Mech_MagneticSphere_SpheresDMG,						"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MagneticSphere,			Mech_MagneticSphere_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_ATKPowMin,							"AttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_ATKPowMax,							"AttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_AtkRating,							"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_Defense,							"Defense");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_HP,									"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_UseMana,							"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp,  SKILLID_MetalGolem,				Mech_MetalGolem_Duration_New,						"Duration");
	}

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


int CMechanician::GetMaximizeAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	UINT uTime = pcUserData->dwMaximizeEndTime;
	if ( uTime >= TICKCOUNT )
	{
		return pcUserData->iMaximizeMaxDamageBoost;
	}

	return 0;
}

int CMechanician::GetAutomationAttackPowerBoost( User * pcUser, UxxxTarget * p_Target, int TypeDamage, EItemType eItemType )
{
	UserData * pcUserData = pcUser->pcUserData;

	UINT uTime = pcUserData->dwAutomationEndTime;
	if ( uTime >= TICKCOUNT )
	{
		if ( ( eItemType != ITEMTYPE_Bow ) && ( eItemType != ITEMTYPE_Javelin ) )
			return 0;

		return pcUserData->iAutomationDamageBoost;
	}
	return 0;
}


int CMechanician::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Mechanician Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int attackPower = *piAttackPower;

	int iValue = 0;

	switch ( uSkillID )
	{
		case MechanicanSkill::DefaultMechAttack:
		{
			UINT uTime = pcUserData->dwAutomationEndTime;
			if ( uTime >= TICKCOUNT && p_Target->GetCharacterType() == CHARACTERTYPE_Monster )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Automation +150%% Damage vs. Monster" );
				attackPower += (attackPower * 150) / 100;
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case MechanicanSkill::GreatSmash: //no longer an AoE skill
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Great Smash 1vs1]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_GreatSmash_AddDamage, iLevelSkill );
			int iPercent = GetSkillIntValueAt ( pcUser, Mech_GreatSmash_AddDamagePercent, iLevelSkill );

			attackPower += ( attackPower * iPercent ) / 100;
			attackPower += iValue; //flat

			if ( p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Normal)
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +50%% Normal Monster" );
				attackPower += (attackPower * 50) / 100;
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );

		} break;


		case MechanicanSkill::GrandSmash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Grand Smash 1vs1]" );

			iValue = GetSkillIntValueAt ( pcUser, Mech_GreatSmash_AddDamagePercent, GetUserSkillLevel1Based ( pcUser, SKILLID_GreatSmash ), TRUE );
			attackPower += ( attackPower * iValue ) / 100;

			iValue = GetSkillIntValueAt ( pcUser, Mech_GreatSmash_AddDamage, GetUserSkillLevel1Based ( pcUser, SKILLID_GreatSmash ), TRUE );
			attackPower += iValue; //flat

			iValue = GetSkillIntValueAt ( pcUser, Mech_GrandSmash_DMGBoost, iLevelSkill );
			attackPower += ( attackPower * iValue ) / 100;

			if ( p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Normal)
			{
				attackPower += ( ( attackPower * ( iValue + 30 ) ) / 100 );
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Normal Monster" );
			}

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;


		case MechanicanSkill::Impulsion:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Impulsion]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_Impulsion_DMGBoost, iLevelSkill );
			attackPower += ( ( attackPower * iValue ) / 100 );

			psPacketEx->iAttackProperty = EAttackProperty::Lightning;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case MechanicanSkill::MagneticSphere:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Magnetic Sphere]" );
			UINT uTime = pcUserData->dwMagneticSphereEndTime;
			if ( uTime >= TICKCOUNT )
			{
				// Use the skill level set in IMagneticSphereParam via RecvBuffSkill. As the game sends on the packet only the skill ID and not the level.
				iLevelSkill = pcUserData->iMagneticSphereParam;
				iValue = GetSkillIntValueAt ( pcUser, Mech_MagneticSphere_SpheresDMG, iLevelSkill );
				attackPower += ( ( attackPower * iValue ) / 100 );
				if (p_Target->IsTargetUnit() && p_Target->GetMonsterType() == MONSTERTYPE_Normal ) //Machine
				{
					attackPower += ( (attackPower * 30 ) / 100 );
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +30%% Normal Monster" );
				}

				psPacketEx->iAttackProperty = EAttackProperty::Lightning;
			}
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;


		case MechanicanSkill::SparkShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spark Shield]" );
			UINT uTime = pcUserData->dwSparkShieldEndTime;
			if ( uTime >= TICKCOUNT )
			{
				int iDamagePercent = pcUserData->iSparkShieldDamage;
				attackPower +=  ((attackPower * iDamagePercent) / 100);
				if ( p_Target->IsTargetUnit() )
				{
					if (p_Target->GetMonsterType() == MONSTERTYPE_Normal ) //Machine
					{
						attackPower += ( attackPower * 50 ) / 100;
					}

					psPacketEx->iAttackProperty = EAttackProperty::Lightning;

					PacketServer::SendSkillCastCommandRange( pcUser, pcUserData->iID, MechanicanSkill::SparkShield, p_Target->GetID() );

					//struct PacketBuff
					//{
					//	int size;
					//	int op;
					//	int IDCodePlayer;
					//	int vSkillBuff;
					//	int IDCodeMonster;
					//	int null3;
					//};
					//struct PacketBuff pBuff;
					//pBuff.size = sizeof( pBuff );
					//pBuff.op = 0x50320A20;
					//pBuff.IDCodePlayer =  pcUserData->iID;
					//pBuff.IDCodeMonster = p_Target->GetID();
					//pBuff.null3 = 0;
					//pBuff.vSkillBuff = 0x3C;
					//SendDeBuffToPlayers( pcUserData, &pBuff );
					//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
				}
			}
		} break;

		case MechanicanSkill::Rupture:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rupture]" );

			iValue = iaMechT51DMGBoost[iLevelSkill-1];

			attackPower += ((attackPower * iValue) / 100);

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;




		default:
			break;
	}

	*piAttackPower = attackPower;

	return 1;
}

int CMechanician::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Mechanician Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{

		case MechanicanSkill::MechanicBomb:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Mechanic Bomb]" );

			const int mechanicBombBaseDamage = psPacketRange->iDamage;
			int mechanicBombDamageBoost = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Mech_MechanicBomb_DamagePercent, iLevelSkill );
			int mechanicBombBurningChance = GetSkillIntValueAt(pcUser, ESkillArrayPointer::Mech_MechanicBomb_BurningChance, iLevelSkill);
			int mechanicBombBurningDuration = GetSkillIntValueAt(pcUser, ESkillArrayPointer::Mech_MechanicBomb_BurningDurationSec, iLevelSkill);
			int mechanicBombBurningDamagePercent = GetSkillIntValueAt(pcUser, ESkillArrayPointer::Mech_MechanicBomb_BurningDamagePercent, iLevelSkill);
			int mechanicBombPlainDamageBoost = GetSkillIntValueAt(pcUser, ESkillArrayPointer::Mech_MechanicBomb_Damage, iLevelSkill);
			int mechanicBombArea = GetSkillIntValueAt(pcUser, ESkillArrayPointer::Mech_MechanicBomb_Area, iLevelSkill);

			psPacketRange->iDamage += ( ( mechanicBombBaseDamage * mechanicBombDamageBoost) / 100 );

			psPacketRange->iAttackProperty = EAttackProperty::Fire;

			psPacketRangeEx->sElementalAttackSetting.bChance = mechanicBombBurningChance;
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = mechanicBombBurningDuration;
			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = mechanicBombBurningDamagePercent;

			psPacketRange->iDamage += mechanicBombPlainDamageBoost;

			psPacketRange->iRange = mechanicBombArea;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		case MechanicanSkill::GreatSmash: //no longer an AoE skill
		{
			psPacketRange->iDamage = 0;
			psPacketRange->iRange = 0;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;

		} break;

		case MechanicanSkill::Spark:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spark]" );

			iValue = GetSkillIntValueAt ( pcUser, Mech_Spark_DMGBoostPercent, iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );

			iValue = GetSkillIntValueAt ( pcUser, Mech_Spark_DMGBoostFlat, iLevelSkill );
			psPacketRange->iDamage += iValue;

			iValue = GetSkillIntValueAt ( pcUser, Mech_Spark_Area, iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_101_Brain;
			psPacketRange->iAttackProperty = EAttackProperty::Lightning;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d + %d]", psPacketRange->iDamage );
		} break;

		case MechanicanSkill::Impulsion:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Impulsion]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_Impulsion_DMGBoost, iLevelSkill );
			psPacketRange->iDamage += ( ( psPacketRange->iDamage * iValue ) / 100 );
			iValue = GetSkillIntValueAt ( pcUser, Mech_Impulsion_Area, iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_101_Brain;
			psPacketRange->iAttackProperty = EAttackProperty::Lightning;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;
		} break;

		case MechanicanSkill::Compulsion:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Compulsion]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_Compulsion_Range, iLevelSkill );
			psPacketRange->iRange = iValue;
			psPacketRange->eAttackState = EAttackState::State_201_PullIn;
			iValue = GetSkillIntValueAt ( pcUser, Mech_Compulsion_Duration, iLevelSkill );
			psPacketRangeEx->iaParametersEx[0] = iValue;
			pcUserData->dwHookedTargetEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->iCompulsionParam = 0;

		} break;

		case MechanicanSkill::Rupture:
		{
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Rupture]" );
			//
			//iValue = iaMechT51DMGBoost[iLevelSkill-1];
			//psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100);
			//iValue = 90;
			//psPacketRange->iRange = iValue;
			//psPacketRange->eAttackState = EAttackState::State_0_Normal;
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", psPacketRange->iRange, pcUserData->sCharacterData.iAttackRating );
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
		} break;

		default:
			break;
	}
	return 1;
}

int CMechanician::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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
		case MechanicanSkill::ExtremeShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Extreme Shield]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_ExtremeShield_AddBlock_New, iLevelSkill1Based );


			//pcUser->pcUserData->dwExtremeShieldEndTime	= 0;
			//pcUser->pcUserData->iExtremeShield			= 0;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Extreme shield = %d", pcUser->pcUserData->iExtremeShield );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Character Block Rating = %d", pcUserData->sCharacterData.iBlockRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Add Block Rtg [%d%%] of shield", iValue );
		} break;

		case MechanicanSkill::PhysicalAbsorption:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Physical Absorption]" );
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
			pcUserData->dwAutomationEndTime = 0;
			pcUserData->iAutomationDamageBoost = 0;
			pcUserData->dwMetalArmorEndTime	= 0;
			pcUserData->iMetalArmorParam = 0;

		} break;

		case MechanicanSkill::ElectricalOverload:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Electrical Overload]");

			const int electricalOverloadDuration = GetSkillIntValueAt(pcUser, Mech_ElectricalOverload_Duration, iLevelSkill1Based );

			SendSelfBuffSkill(pcUser, ESkillID::SKILLID_ElectricalOverload, iLevelSkill1Based, electricalOverloadDuration);

			pcUser->uElectricalOverloadEndTime = TIME + electricalOverloadDuration;
			pcUser->bElectricalOverloadLevel1Based = iLevelSkill1Based;
		} break;

		case MechanicanSkill::Maximize:
		{
			if ( *(UINT*)(packet + 0x10) == 0 )
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Maximize]" );
				iValue = GetSkillIntValueAt ( pcUser, Mech_Maximize_Duration, iLevelSkill1Based );
				pcUserData->dwMaximizeEndTime = (iValue * 1000) + TICKCOUNT;
				iValue = GetSkillIntValueAt ( pcUser, Mech_Maximize_AddATKPow, iLevelSkill1Based );
				pcUserData->iMaximizeMaxDamageBoost = iValue;

				// Automation OFF
				pcUserData->dwAutomationEndTime = 0;
				pcUserData->iAutomationDamageBoost = 0;
			}
		} break;

		case MechanicanSkill::Automation:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Automation]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_Automation_Duration, iLevelSkill1Based );
			pcUserData->dwAutomationEndTime = ( iValue * 1000 ) + TICKCOUNT;
			iValue = GetSkillIntValueAt ( pcUser, Mech_Automation_DMGBoost, iLevelSkill1Based );
			pcUserData->iAutomationDamageBoost = iValue;

			// Maximize OFF
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
		} break;

		case MechanicanSkill::MetalArmor:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Metal Armor]" );
			pcUserData->dwMaximizeEndTime = 0;
			pcUserData->iMaximizeMaxDamageBoost = 0;
			pcUserData->dwAutomationEndTime = 0;
			pcUserData->iAutomationDamageBoost = 0;
			pcUserData->dwPhysicalAbsorbEndTime = 0;
			pcUserData->iPhysicalAbsorbBoost = 0;

		} break;

		case MechanicanSkill::SparkShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Spark Shield]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_SparkShield_Duration, iLevelSkill1Based );
			pcUserData->dwSparkShieldEndTime = ( iValue * 1000 ) + TICKCOUNT;
			iValue = GetSkillIntValueAt ( pcUser, Mech_SparkShield_SparkDamage, iLevelSkill1Based );
			int iSparkLevel = packet->SParam;
			if ( iSparkLevel <= 0 )
			{
				pcUserData->dwSparkShieldEndTime = 0;
				break;
			}
			int iSparkDmgBonus = GetSkillIntValueAt ( pcUser, Mech_Spark_DMGBoostPercent, GetUserSkillLevel1Based ( pcUser, SKILLID_Spark ), TRUE );
			int iSparkDmgBonusFlat = GetSkillIntValueAt ( pcUser, Mech_Spark_DMGBoostFlat, GetUserSkillLevel1Based ( pcUser, SKILLID_Spark ), TRUE );
			pcUserData->iSparkShieldDamage = iSparkDmgBonusFlat + ( iSparkDmgBonus * iValue ) / 100;
		} break;

		case MechanicanSkill::MagneticSphere:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Magnetic Sphere]" );
			iValue = GetSkillIntValueAt ( pcUser, Mech_MagneticSphere_Duration, iLevelSkill1Based );
			pcUserData->dwMagneticSphereEndTime = ( iValue * 1000 ) + TICKCOUNT;
			pcUserData->iMagneticSphereParam = iLevelSkill1Based;
		} break;

		case MechanicanSkill::MetalGolem:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Metal Golem]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			UnitData * pcUnitDataPet = SummonMobSkillMS( 0x0F, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData, 0 );

			if ( pcUnitDataPet )
			{
				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_Duration_New, iLevelSkill1Based );

				pcUnitDataPet->dwUpdateCharInfoTime = ( 300 * 1000 ) + TICKCOUNT;

				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_ATKPowMin, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMinDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_ATKPowMax, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iMaxDamage = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_AtkRating, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iAttackRating = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_Defense, iLevelSkill1Based );
				pcUnitDataPet->sCharacterData.iDefenseRating = iValue;

				iValue = GetSkillIntValueAt ( pcUser, Mech_MetalGolem_HP, iLevelSkill1Based );
				pcUnitDataPet->InitHealthPoint( iValue );

				pcUnitDataPet->sCharacterData.sSize = 224 + iLevelSkill1Based * 15;

				pcUnitDataPet->saMoveStepCount[ 0 ] = 1;
				pcUnitDataPet->saMoveStepCount[ 1 ] = 2;

				pcUnitDataPet->PartyFlag = 0x200;
			}

		} break;

		default:
			break;
	}
	return 1;
}

CMechanician::~CMechanician()
{
}
