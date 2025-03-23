#include "stdafx.h"
#include "Knight.h"
#include "HNSSkill.h"
#include "Logger.h"


CKnight::CKnight()
{
}

/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CKnight::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Knight )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CKnight::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Knight )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CKnight::UpdateAndCacheServerSkillInfo ()
{

	INFO( "Updating and caching skill info for Knight.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Knight );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CKnight::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Knight.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Knight;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}


/// <summary>
/// Build skill data container using data from database
/// </summary>
void CKnight::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordBlast,			Knight_SwordBlast_DMGBoostFlat,					"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordBlast,			Knight_SwordBlast_DMGBoostPercent,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordBlast,			Knight_SwordBlast_AttackRange,					"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordBlast,			Knight_SwordBlast_AttackWidth_New,				"AttackWidth");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordBlast,			Knight_SwordBlast_UseMana,						"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBody,			Knight_HolyBody_DMGReduction_Obsolete,			"DamageReduction");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBody,			Knight_HolyBody_DmgToHealConversion_New,		"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBody,			Knight_HolyBody_HealRadius_New,					"BuffRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBody,			Knight_HolyBody_Duration,						"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBody,			Knight_HolyBody_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_PhysicalTraining,	Knight_PhysicalTraining_AddStamina,				"SPPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DoubleCrash,			Knight_DoubleCrash_AddCritical,					"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DoubleCrash,			Knight_DoubleCrash_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DoubleCrash,			Knight_DoubleCrash_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Brandish,			Knight_Brandish_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Brandish,			Knight_Brandish_Area,							"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Brandish,			Knight_Brandish_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Piercing,			Knight_Piercing_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Piercing,			Knight_Piercing_Area,							"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Piercing,			Knight_Piercing_PierceRange_New,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Piercing,			Knight_Piercing_PierceWidth_New,				"AttackWidth");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Piercing,			Knight_Piercing_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DrasticSpirit,		Knight_DrasticSpirit_UndeadDmgReduction_New,	"DamageReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DrasticSpirit,		Knight_DrasticSpirit_AddDefense,				"DefensePercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DrasticSpirit,		Knight_DrasticSpirit_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DrasticSpirit,		Knight_DrasticSpirit_UseMana,					"MPCost");

		//AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordMastery,		Knight_SwordMastery_AddWeaponDamage,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordMastery,		Knight_SwordMastery_AddWeaponDamagePercent,		"AttackPowerPercent");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivineShield,	Knight_DivineShield_AddBlock_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivineShield,		Knight_DivineShield_AddBlock_New,				"BlockChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivineShield,		Knight_DivineShield_ConvertLife,				"AbsorbLife");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivineShield,		Knight_DivineShield_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivineShield,		Knight_DivineShield_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_LifeIncrease,			"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_SuccessRate,				"SuccessRate");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_MaxHP_New,				"SummonMaxHP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_MaxAbs_New,				"SummonMaxAbsorb");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_MaxDMGMin_New,			"SummonMaxAttackPowerMin");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_MaxDMGMax_New,			"SummonMaxAttackPowerMax");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyIncantation,		Knight_HolyIncantation_DMGIncrease_New,			"VsUndeadExtraPercent");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandCross,			Knight_GrandCross_DMGBoost,						"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandCross,			Knight_GrandCross_AtkRtgBoost,					"AttackRating");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandCross,			Knight_GrandCross_UndeadDamage,					"VsUndeadExtraPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandCross,			Knight_GrandCross_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordofJustice,		Knight_SwordofJustice_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordofJustice,		Knight_SwordofJustice_Area,						"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SwordofJustice,		Knight_SwordofJustice_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodlyShield,			Knight_GodlyShield_ShieldABSBoost,				"AbsorptionPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodlyShield,			Knight_GodlyShield_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodlyShield,			Knight_GodlyShield_UseMana,						"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer( temp, SKILLID_GodsBlessing,		Knight_GodsBlessing_AddATKPow_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodsBlessing,		Knight_GodsBlessing_AddATKPowPercent_New,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodsBlessing,		Knight_GodsBlessing_Duration,					"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GodsBlessing,		Knight_GodsBlessing_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivinePiercing,		Knight_DivinePiercing_AddCritical,				"CriticalChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivinePiercing,		Knight_DivinePiercing_DMGBoost,					"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivinePiercing,		Knight_DivinePiercing_Numberofattacks,			"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_DivinePiercing,		Knight_DivinePiercing_UseMana,					"MPCost");
	}

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyValor,		Knight_HolyValor_DamageBoostVsUndead,							"FinalAttackPowerPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyValor,		Knight_HolyValor_Duration,								"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyValor,		Knight_HolyValor_UseMana,								"MPCost");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyValor,		Knight_HolyValor_BuffRadius_New,						"BuffRadius");


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

int CKnight::GetGodsBlessingAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	UINT uTime = pcUserData->GodsBlessingTime;
	if ( uTime >= TICKCOUNT )
	{
		return pcUserData->GodsBlessingParam;
	}
	return 0;
}



int CKnight::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Knight Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int iValue = 0;
	int attackPower = *piAttackPower;

	switch ( uSkillID )
	{
		case KnightSkill::DefaultKnightAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case KnightSkill::DivineShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Divine Shield]" );
			attackPower = sActiveSkill.sParam;
			psPacketEx->eAttackState = EAttackState::State_9_DivineShield;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage [%d]", attackPower );
		} break;


		case KnightSkill::DoubleCrash:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Double Crash]" );
			int iCritical = GetSkillIntValueAt ( pcUser, Knight_DoubleCrash_AddCritical, iLevelSkill );
			iValue = GetSkillIntValueAt ( pcUser, Knight_DoubleCrash_DMGBoost, iLevelSkill );
			attackPower += ( attackPower * iValue ) / 100;
			psPacketEx->sCriticalChance += iCritical;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->sCriticalChance, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case KnightSkill::DivinePiercing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Divine Piercing]" );
			iValue = GetSkillIntValueAt ( pcUser, Knight_DivinePiercing_DMGBoost, iLevelSkill );
			attackPower += ( attackPower * iValue ) / 100;
			int iCritical = GetSkillIntValueAt ( pcUser, Knight_DivinePiercing_AddCritical, iLevelSkill );
			psPacketEx->sCriticalChance += iCritical;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->sCriticalChance, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;



		case KnightSkill::HolyIncantation:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Incantation]" );
			if ( p_Target->IsTargetUnit() && p_Target->GetMonsterClass() != EMonsterClass::Boss )
			{
				if (p_Target->GetMonsterType() == MONSTERTYPE_Undead && !p_Target->unitData->pcOwner)
				{

					// Is Incantation Alive?
					if ( pcUserData->dwHolyIncantationID )
					{
						UnitData * pcUnitDataIncantaten = UNITSERVER->GetUnit( pcUserData->dwHolyIncantationID );
						if ( pcUnitDataIncantaten && pcUnitDataIncantaten->sCharacterData.iOwnerID == pcUserData->iID && pcUnitDataIncantaten->GetCurrentHealth() > 0 &&
							pcUnitDataIncantaten->sHolyIncantation[1] > 0 )
						{
							CHATSERVER->SendTitleBox( pcUserData, "You can't convert more than one monster!" );
							return 0;
						}
					}

					if ( p_Target->unitData->PartyFlag == 0x200 )
					{
						p_Target->unitData->SetCurrentHealth( 0 );
						p_Target->unitData->Animate( EAnimationType::ANIMATIONTYPE_Die );
					}

					pcUserData->dwHolyIncantationID = 0;

					int iElement = p_Target->GetCharacterData()->sElementalDef[ Organic ];
					if ( iElement > 100 )
						iElement = 100;

					//repurpose this
					//% of monster dmg, or based on knight's str value?
					//flat based monster hp value, or have it based on knight's hp value?


					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_SuccessRate, iLevelSkill );
					int iPercent = ( ( 100 - iElement ) * iValue ) / 100;

					int iChance = Dice::RandomI(1, 100);
					if ( iChance > iPercent )
						return 0;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_LifeIncrease, iLevelSkill );

					p_Target->GetCharacterData()->cNewLoad++;

					int iMax = ((int)p_Target->GetCharacterData()->sHP.sMax * iValue) / 100;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_MaxHP_New, iLevelSkill );

					if ( iMax <= 0 )
						iMax = 100;	//shouldn't happen, but just there in case
					else if ( iMax > iValue )
						iMax = iValue;

					if ( iMax > 30000 )
						iMax = 30000; //max due to 'short' type being maxed at ~ 31,000

					p_Target->unitData->sCharacterData.sHP.sCur = iValue;
					p_Target->unitData->sCharacterData.sHP.sMax = iValue;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_MaxAbs_New, iLevelSkill );

					if (p_Target->GetCharacterData()->iAbsorbRating > iValue )
						p_Target->GetCharacterData()->iAbsorbRating = iValue;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_DMGIncrease_New, iLevelSkill );

					p_Target->GetCharacterData()->iMinDamage += (p_Target->GetCharacterData()->iMinDamage * iValue) / 100;
					p_Target->GetCharacterData()->iMaxDamage += (p_Target->GetCharacterData()->iMaxDamage * iValue) / 100;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_MaxDMGMin_New, iLevelSkill );

					if (p_Target->GetCharacterData()->iMinDamage > iValue )
						p_Target->GetCharacterData()->iMinDamage = iValue;

					iValue = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_MaxDMGMax_New, iLevelSkill );

					if (p_Target->GetCharacterData()->iMaxDamage > iValue )
						p_Target->GetCharacterData()->iMaxDamage = iValue;

					p_Target->unitData->sUnitInfo.dwHashCharacterName = 0;
					p_Target->unitData->pcOwner = pcUserData;
					p_Target->unitData->sHolyIncantation[0] = ( short )p_Target->GetMonsterType();

					int iDuration = GetSkillIntValueAt ( pcUser, Knight_HolyIncantation_Duration, iLevelSkill );
					p_Target->unitData->sHolyIncantation[1] = iDuration * 19;

					p_Target->GetCharacterData()->iOwnerID = pcUserData->iID;
					p_Target->GetCharacterData()->iMonsterType = MONSTERTYPE_Summon;
					STRINGCOPYLEN( ( char* )p_Target->GetCharacterData()->Pet.szOwnerName, 59, pcUserData->sCharacterData.szName );

					ITEMSERVER->OnUseCrystal( pcUserData, p_Target->unitData, SKILLID_HolyIncantation, iDuration );

					p_Target->unitData->pcAttackTarget = NULL;
					p_Target->unitData->pcFocusTarget = NULL;
					p_Target->unitData->pcFocusingTarget = NULL;

					p_Target->unitData->PartyFlag = 0x200;

					//Decrease Monster Counter on Map and delete from Spawn Flag
					MAPSERVER->DisableMonsterSpawn( p_Target->unitData->pcMap, p_Target->unitData );
					UNITSERVER->UpdateCharacterData(USERDATATOUSER(pcUserData), p_Target->unitData);

					typedef void( __cdecl *t_SendMobEncanten )( UserData * pcUserData, void * packet );
					t_SendMobEncanten SendMobEncanten = ( t_SendMobEncanten )0x00552130;
					BYTE PacketPT[ 44 ];
					ZeroMemory( PacketPT, 44 );
					*( UINT* )( &PacketPT[ 8 ] )	= p_Target->GetID();
					pcUserData->dwHolyIncantationID = p_Target->GetID();
					*( UINT* )( &PacketPT[ 12 ] ) = EAnimationType::ANIMATIONTYPE_Die;
					*( UINT* )( &PacketPT[ 16 ] ) = iDuration;
					*( UINT* )( &PacketPT[ 20 ] ) = 0;
					SendMobEncanten( pcUserData, PacketPT );
				}
			}
			return 0;
		} break;

		case KnightSkill::GrandCross:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Grand Cross]" );
			iValue = GetSkillIntValueAt ( pcUser, Knight_GrandCross_DMGBoost, iLevelSkill );

			int iValueAdd = GetSkillIntValueAt ( pcUser, Knight_GrandCross_UndeadDamage, iLevelSkill );
			if (p_Target->IsTargetUnit())
			{
				if (p_Target->GetMonsterType() == MONSTERTYPE_Undead )
				{
					iValue += iValueAdd;
					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +%d%% Undead Monster", iValueAdd );
				}
			}

			iValueAdd = GetSkillIntValueAt ( pcUser, Knight_DoubleCrash_DMGBoost, GetUserSkillLevel1Based ( pcUser, SKILLID_DoubleCrash ), TRUE );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +%d%% Double Crash Chain", iValueAdd );
			iValue += iValueAdd;

			attackPower += ( attackPower * iValue ) / 100;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", psPacketEx->sCriticalChance, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		default:
			break;

	}

	*piAttackPower = attackPower;

	return 1;

}


int CKnight::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;


	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Knight Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int attackPower = psPacketRange->iDamage;

	switch ( uSkillID )
	{
		case KnightSkill::SwordBlast:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Sword Blast]" );
			int iDamagePercent = GetSkillIntValueAt ( pcUser, Knight_SwordBlast_DMGBoostPercent, iLevelSkill );
			int iStaticDamage = GetSkillIntValueAt ( pcUser, Knight_SwordBlast_DMGBoostFlat, iLevelSkill );
			int iRange = GetSkillIntValueAt ( pcUser, Knight_SwordBlast_AttackRange, iLevelSkill );
			psPacketRange->iDamage += ((psPacketRange->iDamage * iDamagePercent ) / 100) + iStaticDamage;
			psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
		} break;

		case KnightSkill::Brandish:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Brandish]" );
			iValue = GetSkillIntValueAt ( pcUser, Knight_Brandish_DMGBoost, iLevelSkill );
			int iRange = GetSkillIntValueAt ( pcUser, Knight_Brandish_Area, iLevelSkill );
			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			//std::cout << "Brandish Monster count: " << psPacketRange->iCount << "  ex: " << psPacketRangeEx->iTargetCount << std::endl;

		} break;

		case KnightSkill::Piercing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Piercing]" );
			iValue = GetSkillIntValueAt ( pcUser, Knight_Piercing_DMGBoost, iLevelSkill );

			int iValueAddPercent = GetSkillIntValueAt ( pcUser, Knight_SwordBlast_DMGBoostPercent,  GetUserSkillLevel1Based ( pcUser, ESkillID::SKILLID_SwordBlast ), TRUE );
			int iValueAddFlat = GetSkillIntValueAt ( pcUser, Knight_SwordBlast_DMGBoostFlat,		GetUserSkillLevel1Based ( pcUser, ESkillID::SKILLID_SwordBlast ), TRUE );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Sword Blast Chain [+%d + +%d%%]", iValueAddFlat, iValueAddPercent );
			iValue += iValueAddPercent;

			int iRange = GetSkillIntValueAt ( pcUser, Knight_Piercing_Area, iLevelSkill );
			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			psPacketRange->iDamage += iValueAddFlat;

			psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			//std::cout << "Piercing Monster count: " << psPacketRange->iCount << "  ex: " << psPacketRangeEx->iTargetCount << std::endl;

		} break;

		case KnightSkill::SwordofJustice:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Sword of Justice]" );
			iValue = GetSkillIntValueAt ( pcUser, Knight_SwordofJustice_DMGBoost, iLevelSkill );
			//int iRange = GetSkillIntValueAt ( pcUser, Knight_SwordofJustice_Area, iLevelSkill );
			int iRadius = GetSkillIntValueAt( pcUser, Knight_SwordofJustice_Area, iLevelSkill );
			psPacketRange->iRange = iRadius;


			int iValueAdd = GetSkillIntValueAt ( pcUser, Knight_Brandish_DMGBoost, GetUserSkillLevel1Based ( pcUser, SKILLID_Brandish ), TRUE );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> +%d%% Brandish Chain", iValueAdd );
			iValue += iValueAdd;

			psPacketRange->iDamage += ( psPacketRange->iDamage * iValue ) / 100;
			//psPacketRange->iRange = iRange;
			psPacketRange->eAttackState = EAttackState::State_0_Normal;
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Range[%d] Attack Rating[%d]", iRange, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			psPacketRange->iCount &= 0x000000FF;

		} break;

		default:
			break;
	}

	return 1;
}

int CKnight::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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
		case KnightSkill::HolyValor:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Valor]" );
			int iDuration = GetSkillIntValueAt ( pcUser, Knight_HolyValor_Duration, iLevelSkill1Based );

			ApplyAndSendHolyValorBuff( pcUser, iLevelSkill1Based, iDuration );
		}
		break;

		case KnightSkill::HolyBody:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Body]" );
			duration = GetSkillIntValueAt ( pcUser, Knight_HolyBody_Duration, iLevelSkill1Based );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "Duration = %d", duration );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "DMG Reduction = %d", pcUserData->iHolyBodyAbsorb );

			pcUser->uHolyBodyEndTime = TIME + duration;
			pcUser->bHolyBodyLevel1Based = iLevelSkill1Based;

		} break;

		case KnightSkill::DrasticSpirit:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Drastic Spirit]" );
			duration = GetSkillIntValueAt ( pcUser, Knight_DrasticSpirit_Duration, iLevelSkill1Based  );
			iValue = GetSkillIntValueAt ( pcUser, Knight_DrasticSpirit_UndeadDmgReduction_New, iLevelSkill1Based );

			pcUserData->iHolyBodyAbsorb = iValue;
			pcUserData->dwHolyBodyEndTime = ( duration * 1000 ) + TICKCOUNT;

			pcUser->uDrasticSpiritEndTime = TIME + GetSkillIntValueAt ( pcUser, Knight_HolyBody_Duration, iLevelSkill1Based );
			pcUser->bDrasticSpiritLevel1Based = iLevelSkill1Based;


			break;
		}

		case KnightSkill::GodlyShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Godly Shield]" );
		}break;

		case KnightSkill::DivineShield:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Divine Shield]" );
			int iDuration = GetSkillIntValueAt ( pcUser, Knight_DivineShield_Duration, iLevelSkill1Based );
			pcUserData->dwDivineShieldEndTime = ( iDuration * 1000 ) + TICKCOUNT;
			iValue = GetSkillIntValueAt ( pcUser, Knight_DivineShield_ConvertLife, iLevelSkill1Based );
			pcUserData->sDivineShieldAbsorb = (WORD)iValue;

			pcUserData->sDivineShieldAttackRating = 0;
		} break;

		case KnightSkill::GodsBlessing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [God's Blessing]" );
			int iDuration = GetSkillIntValueAt ( pcUser, Knight_GodsBlessing_Duration, iLevelSkill1Based );
			pcUserData->GodsBlessingTime = ( iDuration * 1000 ) + TICKCOUNT;
			pcUserData->GodsBlessingParam = GetSkillIntValueAt ( pcUser, Knight_GodsBlessing_AddATKPowPercent_New, iLevelSkill1Based );
		} break;

		case KnightSkill::HolyConviction:
		{
			//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Conviction]" );
			//pcUserData->dwHolyValorEndTime_Obsolete = TICKCOUNT + (300 * 1000);
			//iValue = GetSkillIntValueAt ( pcUser, Knight_HolyValor_DamageBoostVsUndead, GetUserSkillLevel1Based ( pcUser, SKILLID_HolyValor ), TRUE  );
			//pcUserData->iHolyValorParam_Obsolete = iValue;
			//
			//pcUserData->dwHolyBodyEndTime = TICKCOUNT + (300 * 1000);
			//iValue = GetSkillIntValueAt ( pcUser, Knight_HolyBody_DMGReduction, GetUserSkillLevel1Based ( pcUser, SKILLID_HolyBody ), TRUE  );
			//pcUserData->iHolyBodyAbsorb = iValue;
			//
			//pcUser->uHolyConvictionLevel	= iLevelSkill1Based;
			//pcUser->dwHolyConvictionTimeOut = TICKCOUNT + (300 * 1000);

		} break;

		default:
			break;
	}
	return 1;
}

int CKnight::AddHolyConviction( User * pcUser, PacketSkillPartyData * packet )
{
	//UserData * pcUserData = pcUser->pcUserData;
	//int iLevelSkill = packet->iLevel;
	//
	//CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "%d Effect", packet->iSkillID );
	//
	//int iValue = 0;
	//
	//if ( packet->iSkillID == KnightSkill::HolyConviction )
	//{
	//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Valor]" );
	//	pcUserData->dwHolyValorEndTime = TICKCOUNT + (300 * 1000);
	//	iValue = GetSkillIntValueAt ( pcUser, Knight_HolyValor_DamageBoostVsUndead, GetUserSkillLevel1Based ( pcUser, SKILLID_HolyValor ), TRUE  );
	//	pcUserData->iHolyValorParam = iValue;
	//	packet->iClass = 6;
	//
	//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Body]" );
	//	pcUserData->dwHolyBodyEndTime = TICKCOUNT + (300 * 1000);
	//	iValue = GetSkillIntValueAt ( pcUser, Knight_HolyBody_DMGReduction, GetUserSkillLevel1Based ( pcUser , SKILLID_HolyBody ), TRUE  );
	//	pcUserData->iHolyBodyAbsorb = iValue >> 1;
	//
	//	User * pcUser = USERDATATOUSER( pcUserData );
	//	pcUser->uHolyConvictionLevel = iLevelSkill;
	//	pcUser->dwHolyConvictionTimeOut = TICKCOUNT + (300 * 1000);
	//
	//	return 1;
	//}
	return 0;
}

void CKnight::OnCancelDrasticSpirit( User * pcUser )
{
	INFO( "CKnight::OnCancelDrasticSpirit" );

	pcUser->uDrasticSpiritEndTime = TIME; //expiry now
}

int CKnight::GetHolyValorBuff( User * pcUser )
{
	if ( pcUser->bHolyValorLevel1Based == 0 )
		return 0;

	//solo mode
	if ( pcUser->GetMapID() == MAPID_Bellatra &&
		 pcUser->pcUserData->sCharacterData.iClass != CHARACTERCLASS_Knight &&
		 pcUser->iBellatraRoomInitialPlayerCount == 1)
	{
		return 0;
	}

	int buff = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Knight_HolyValor_DamageBoostVsUndead, pcUser->bHolyValorLevel1Based );

	//half for non-ks
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Knight )
		buff >>= 1;

	//if drastic spirit is active, half the buff
	if ( pcUser->bDrasticSpiritLevel1Based > 0 )
	{
		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Valor] Halved due to Drastic Spirit" );
		buff >>= 1; //half it
	}

	return buff;
}

void CKnight::ApplyAndSendHolyValorBuff( User * pcCaster, int iLevel1Based, int iDurationSec )
{
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters ) ) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bHolyValorLevel1Based )
		{
			pPartyUser->bHolyValorLevel1Based = iLevel1Based;
			pPartyUser->uHolyValorEndTime = TIME + iDurationSec;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_HolyValor, iLevel1Based, iDurationSec, 0, pBuffUsers );
}


CKnight::~CKnight()
{
}
