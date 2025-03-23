#include "stdafx.h"
#include "HNSSkill.h"
#include "Priestess.h"



typedef void( __cdecl *t_RestoreHPPacket ) ( void * packet );
t_RestoreHPPacket RestoreHPPacket = ( t_RestoreHPPacket )0x00551700;

typedef UINT( __cdecl *t_FindPlayerByIDCode ) ( UINT IDPlayer );
t_FindPlayerByIDCode FindPlayerByIDCode = ( t_FindPlayerByIDCode )0x0054CC10;



CPriestess::CPriestess()
{
}


/// <summary>
/// Send the cached server skill info to client
/// </summary>
void CPriestess::SendServerSkillInfoToClient ( User * pcUser )
{
	if ( pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Priestess )
		return;

	SENDPACKET ( pcUser, &sSkillInfoContainer );
}

/// <summary>
/// Send the cached server skill data to client
/// </summary>
void CPriestess::SendServerSkillDataToClient ( User * pcUser )
{
	//for pikeman only
	if ( pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Priestess )
	{
		SENDPACKET ( pcUser, &sSkillDataContainer );
	}
}

/// <summary>
/// Build and cache server skill info locally
/// </summary>
void CPriestess::UpdateAndCacheServerSkillInfo ()
{
	INFO( "Updating and caching skill info for Priestess.." );

	ZeroMemory ( &sSkillInfoContainer, sizeof ( PacketSkillInfoContainer ) );

	BuildPacketSkillInfoContainer ( &sSkillInfoContainer, ECharacterClass::CHARACTERCLASS_Priestess );
	CacheSkillInfoFromContainer( &sSkillInfoContainer );
}

/// <summary>
/// Update and cache server skill info and data locally
/// to improve performance.
/// </summary>
void CPriestess::UpdateAndCacheServerSkillData ()
{
	INFO( "Updating and caching skill data for Priestess.." );

	ZeroMemory ( &sSkillDataContainer, sizeof ( PacketSkillDataContainer ) );

	sSkillDataContainer.iHeader = PKTHDR_ServerSkillData;
	sSkillDataContainer.iCharacterClass = CHARACTERCLASS_Priestess;
	sSkillDataContainer.iCount = 0;

	BuildPacketSkillDataContainer ( &sSkillDataContainer, TRUE );
	CacheSkillArrayDataFromContainer ( &sSkillDataContainer );

}

/// <summary>
/// Build skill data container using data from database
/// </summary>
void CPriestess::BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass )
{
	auto temp = new PacketSkillDataContainer ();
	ZeroMemory ( temp, sizeof ( PacketSkillDataContainer ) );
	temp->iCount = 0;

	if ( bIsThisClass )
	{
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Healing,				Priestess_Healing_LifeRecovery,					"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Healing,				Priestess_Healing_LifeRecoveryPercent,			"HPRegen");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Healing,				Priestess_Healing_UseMana,						"MPCost");

		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBolt,		Priestess_HolyBolt_DamageBoost_DoNotUse);
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBolt,			Priestess_HolyBolt_Damage,						"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBolt,			Priestess_HolyBolt_DamageBoost_New,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyBolt,			Priestess_HolyBolt_UseMana,						"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MultiSpark,			Priestess_MultiSpark_Damage,					"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MultiSpark,			Priestess_MultiSpark_DamageBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MultiSpark,			Priestess_MultiSpark_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyMind,			Priestess_HolyMind_Duration_New,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyMind,			Priestess_HolyMind_ReduceDamage,				"AttackPowerReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyMind,			Priestess_HolyMind_UseMana,						"MPCost");

		AppendFloatValuesToServerSkillDataBuffer ( temp, SKILLID_Meditation,		Priestess_Meditation_AddMPRegen,				"MPRegen");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Meditation,			Priestess_Meditation_MPPotionBoost,				"MPPotionPercent");

		AppendIntValuesToServerSkillDataBuffer(temp, SKILLID_DivineLightning,		 Priestess_DivineLightning_Damage_New,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer(temp, SKILLID_DivineLightning,		 Priestess_DivineLightning_DamageBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer(temp, SKILLID_DivineLightning,		 Priestess_DivineLightning_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLightning,		Priestess_ChainLightning_Damage_New,			"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLightning,		Priestess_ChainLightning_DamageBoost,			"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLightning,		Priestess_ChainLightning_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyReflection,		Priestess_HolyReflection_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyReflection,		Priestess_HolyReflection_ReflectDMG,			"ReflectDamage");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_HolyReflection,		Priestess_HolyReflection_UseMana,				"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandHealing,		Priestess_GrandHealing_LifeRecovery,			"HP");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandHealing,		Priestess_GrandHealing_LifeRecoveryPercent,		"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandHealing,		Priestess_GrandHealing_Range,					"BuffRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GrandHealing,		Priestess_GrandHealing_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VigorBall,			Priestess_VigorBall_AtkRtgBoost,				"AttackRatingPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VigorBall,			Priestess_VigorBall_Damage_New,					"AttackPower");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VigorBall,			Priestess_VigorBall_DamageBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VigorBall,			Priestess_VigorBall_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Ressurection,		Priestess_Resurrection_SuccessRate,				"SuccessRate");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Ressurection,		Priestess_Resurrection_MaxPlayers,				"NumberOfTargets");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Ressurection,		Priestess_Resurrection_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Extinction,			Priestess_Extinction_HPReduction,				"HPPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Extinction,			Priestess_Extinction_HPReductionCap_New,		"HPReductionCap", FALSE, TRUE); //ignore 0 warning, it is used to disable cap
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Extinction,			Priestess_Extinction_SuccessRate,				"SuccessRate");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Extinction,			Priestess_Extinction_Radius_New,				"AttackRadius");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_Extinction,			Priestess_Extinction_UseMana,					"MPCost");

		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_DamageBoost,				"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_ATKRange,				"AttackRange");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_UseMana,					"MPCost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_SpeedMonDown_New,		"SpeedReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_FreezeChance,			"FreezeChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_FreezePercentOfAP_New,	"FreezeAtkPowPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_GlacialSpike,		Priestess_GlacialSpike_FreezeDurationSec,		"FreezeDurationSec");


		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,	Priestess_SummonMuspell_DamageBoostMin_DoNotUse );
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,	Priestess_SummonMuspell_DamageBoostMax_DoNotUse );
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,	Priestess_SummonMuspell_AddEvasion_DoNotUse );
		AppendZeroIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,	Priestess_SummonMuspell_UndeadAbsorb_DoNotUse );
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_DamageBoost_New,		"AttackPowerPercent");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_AddEvasion_New,			"EvasionChance");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_MuspellAttackDelaySec,	"AttackDelay");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_Duration,				"Duration");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_MuspellDMGBoost,		"SummonDamageBoost");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_UndeadAbsorb_New,		"VsUndeadAbsorb");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_VirtualLifeReduction,	"VirtualLifeReduction");
		AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_SummonMuspell,		Priestess_SummonMuspell_UseMana,				"MPCost");

	}

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MultiSpark,			Priestess_MultiSpark_NumSparksMin_New,			"NumberOfTargetsMax");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_MultiSpark,			Priestess_MultiSpark_NumSparksMax,				"NumberOfTargetsMin");


	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLightning,		Priestess_ChainLightning_Numberofbolts,			"NumberOfTargets");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_ChainLightning,		Priestess_ChainLightning_Range,					"AttackRange");

	AppendIntValuesToServerSkillDataBuffer(temp, SKILLID_DivineLightning,		 Priestess_DivineLightning_ATKRadius_New,	"AttackRadius");
	AppendIntValuesToServerSkillDataBuffer(temp, SKILLID_DivineLightning,		 Priestess_DivineLightning_NumOfBolts,		"NumberOfTargets");


	//set existing regen values to zero on client side
	AppendZeroIntValuesToServerSkillDataBuffer( temp, SKILLID_VirtualRegen,		Priestess_RegenerationField_AddLifeRegen );
	AppendZeroIntValuesToServerSkillDataBuffer( temp, SKILLID_VirtualRegen,		Priestess_RegenerationField_AddManaRegen );

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualRegen,		Priestess_VirtualRegen_HPRegen_New,					"HPRegen");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualRegen,		Priestess_VirtualRegen_MPRegen_New,					"MPRegen");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualRegen,		Priestess_VirtualRegen_Duration_New,				"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualRegen,		Priestess_VirtualRegen_UseMana,						"MPCost");

	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualLife,			Priestess_VirtualLife_HPBoost,						"HPPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualLife,			Priestess_VirtualLife_MPBoost_New,					"MPPercent");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualLife,			Priestess_VirtualLife_Duration,						"Duration");
	AppendIntValuesToServerSkillDataBuffer ( temp, SKILLID_VirtualLife,			Priestess_VirtualLife_UseMana,						"MPCost");

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


int CPriestess::GetSummonMuspellAttackPowerBoost( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	if ( pcUserData->SummonMuspellTime > TICKCOUNT )
	{
		return pcUser->iSummonMuspellDamageBoost;
	}

	return 0;
}

void CPriestess::OnUseHolyMind( User * pcUser, PacketHolyMind * psPacket )
{
	UserData * pcUserDataTarget = NULL;
	UnitData * pcUnitDataTarget = NULL;

	int iTime		= psPacket->iTime;
	int iAbs		= psPacket->iAbsorbPercent;
	int iResistance = 0;

	if ( (pcUserDataTarget = USERDATABYID( psPacket->dwObjectID )) )
	{
		iResistance = pcUserDataTarget->sCharacterData.sElementalDef[Organic];

		if ( iResistance < 0 )
			iResistance = 0;
		if ( iResistance > 100 )
			iResistance = 100;

		iTime	= (iTime*(100 - iResistance)) / 100;
		iAbs	= (iAbs*(100 - iResistance)) / 100;

		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Holy Mind [%s] Time[%d] Abs[%d] Res[%d]", CHARACTERSERVER->GetCharacterName( pcUserDataTarget ), iTime, iAbs, iResistance );

		if ( iTime > 0 )
		{
			User * u = USERDATATOUSER( pcUserDataTarget );
			if ( u )
			{
				u->dwHolyMindTime	= (iTime * 1000) + TICKCOUNT;
				u->iHolyMindAbsorb	= iAbs;
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

void CPriestess::OnHolyMindHandler( User * pcUserTarget, int * piDamage )
{
	if ( ( pcUserTarget && pcUserTarget->IsValid () ) == FALSE )
		return;

	UserData * pcUserDataTarget = pcUserTarget->pcUserData;

	if ( pcUserDataTarget )
	{
		User * u = USERDATATOUSER( pcUserDataTarget );
		if ( u )
		{
			if ( u->dwHolyMindTime > TICKCOUNT )
			{
				*piDamage -= ((*piDamage) * u->iHolyMindAbsorb) / 100;
			}
			else
			{
				u->dwHolyMindTime	= 0;
				u->iHolyMindAbsorb	= 0;
			}
			if ( u->dwWispTime > TICKCOUNT )
			{
				*piDamage -= ((*piDamage) * u->iWispAbsorb) / 100;
			}
			else
			{
				u->dwWispTime	= 0;
				u->iWispAbsorb	= 0;
			}
		}
	}
}

int CPriestess::RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketEx->iSkillCode;
	int uSkillID = psPacketEx->iSkillCode & 0xFF;
	int iLevelSkill = ( psPacketEx->iSkillCode >> 8 ) & 0xF;
	int iParam = ( psPacketEx->iSkillCode >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Priestess Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketEx->iSkillCode );
		return 0;
	}

	int iValue = 0;
	int attackPower = *piAttackPower;

	switch ( sActiveSkill.cSkillTinyID )
	{
		case PriestessSkill::PriestessDefaultAttack:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Default Attack]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
		} break;

		case PriestessSkill::HolyReflection:
		{
			CHATSERVER->SendDebugChat(pcUser, CHATCOLOR_Error, "> [Holy Reflection]");
			UINT uTime = pcUserData->dwHolyReflectionEndTime;
			if (uTime >= TICKCOUNT)
			{
				//int iDamagePercent = pcUserData->iHolyReflectionDamage;
				//attackPower += ((attackPower * iDamagePercent) / 100);
				if (p_Target->GetMonsterType() == MONSTERTYPE_Undead)
				{
					//psPacketEx->iAttackProperty = EAttackProperty::Lightning;
					int iHolyReflectDamage = GetSkillIntValueAt(pcUser, Priestess_HolyReflection_ReflectDMG, GetUserSkillLevel1Based(pcUser, SKILLID_HolyReflection), TRUE);
					//int iSparkDmgBonusFlat = GetSkillIntValueAt(pcUser, Priestess_HolyReflection_ReflectDMG, GetUserSkillLevel1Based(pcUser, SKILLID_Spark), TRUE);
					pcUserData->iHolyReflectionDamage = iHolyReflectDamage * 100;
				}
			}
		} break;

		case PriestessSkill::Healing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Healing]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target ID = %d", psPacketEx->dwTarObjectSerial);
			iValue = GetSkillIntValueAt ( pcUser, Priestess_Healing_LifeRecoveryPercent, iLevelSkill );
			int iLifeRecovery = GetSkillIntValueAt ( pcUser, Priestess_Healing_LifeRecovery, iLevelSkill );
			int iDamageBoost = attackPower;

			if (p_Target->IsTargetPlayer() &&
				p_Target->GetMonsterEffectID() != MONSTEREFFECTID_CrystalTower )
			{
				PacketHealing l_HealingPacket;
				l_HealingPacket.iLength		= sizeof ( l_HealingPacket );
				l_HealingPacket.iHeader		= PKTHDR_Skill_Healing;
				l_HealingPacket.iHealAmount	= ( ( iDamageBoost * iValue ) / 100 ) + iLifeRecovery;
				l_HealingPacket.iTargetID	= p_Target->GetID();
				l_HealingPacket.iCasterID	= pcUserData->iID;

                SENDPACKET(p_Target->userData, &l_HealingPacket);

				/// Feedback packet
				if (p_Target->GetID() != pcUserData->iID)
				{
					PacketDebugDamageInfoContainer l_InfoPacket;
					l_InfoPacket.iHeader = PKTHDR_DamageInfoContainer;
					l_InfoPacket.sAmount = 1;

					auto l_Block = reinterpret_cast<PacketDebugDamageInfo*>(l_InfoPacket.baBuffer);
					l_Block->dwObjectID		= p_Target->GetID();
					l_Block->sTypeAction	= EDamageTextType::Healing;
					l_Block->sParameter		= -1;
					l_Block->iValue			= l_HealingPacket.iHealAmount;
					l_Block->dwAttackerID	= pcUserData->iID;

					l_InfoPacket.iLength = sizeof(Packet) + sizeof(short) + (l_InfoPacket.sAmount * sizeof(PacketDebugDamageInfo));

					SENDPACKET(pcUserData, &l_InfoPacket);
				}

				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> HP Healed: %d", l_HealingPacket.iHealAmount );
			}
			else if (!p_Target->IsValid())
			{
				CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target user not valid: %d", psPacketEx->dwTarObjectSerial );
			}

			return 0;
		} break;



		case PriestessSkill::GrandHealing:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Grand Healing]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_GrandHealing_LifeRecoveryPercent, iLevelSkill );
            int iValueFlat		= GetSkillIntValueAt(pcUser, Priestess_GrandHealing_LifeRecovery, iLevelSkill);
            int iRange			= GetSkillIntValueAt(pcUser, Priestess_GrandHealing_Range, iLevelSkill);
			int iDamageBoost	= attackPower;

			int iValueAddPercent = GetSkillIntValueAt ( pcUser, Priestess_Healing_LifeRecoveryPercent, GetUserSkillLevel1Based(pcUser, ESkillID::SKILLID_Healing), TRUE );
			int iValueAddFlat = GetSkillIntValueAt ( pcUser, Priestess_Healing_LifeRecoveryPercent, GetUserSkillLevel1Based ( pcUser, ESkillID::SKILLID_Healing ), TRUE );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Healing Chain: [%d%% + %d]", iValueAddPercent, iValueAddFlat );

			iValue += iValueAddPercent;
			iValueFlat += iValueFlat;

			PacketGrandHealing phpr;
			phpr.iLength		= sizeof( PacketGrandHealing );
			phpr.iHeader		= PKTHDR_Skill_GrandHealing;
			phpr.iHealAmount	= ( ( iDamageBoost * iValue ) / 100 ) + iValueFlat;
			phpr.iSkillLevel	= iLevelSkill;
			phpr.iRange			= GetSkillIntValueAt ( pcUser, Priestess_GrandHealing_Range, iLevelSkill );
			phpr.iCasterID		= pcUserData->iID;

			OnGrandHealingCasted ( pcUser, &phpr, iRange );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> HP Restored: %d", phpr.iHealAmount );
			return 0;
		} break;

		case PriestessSkill::HolyBolt:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Bolt]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_HolyBolt_DamageBoost_New, iLevelSkill );
			int const iStaticDamage = GetSkillIntValueAt ( pcUser, Priestess_HolyBolt_Damage, iLevelSkill );

			attackPower += ( ( attackPower * iValue ) / 100 );
			attackPower += iStaticDamage;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		case PriestessSkill::VigorBall:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Vigor Ball]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_VigorBall_DamageBoost, iLevelSkill );
			int iStaticDamage = GetSkillIntValueAt ( pcUser, Priestess_VigorBall_Damage_New, iLevelSkill );
			int iDamageBoost = attackPower;

			attackPower = iStaticDamage + ( ( iDamageBoost * iValue ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
			psPacketEx->sCriticalChance = pcUserData->sCharacterData.iCritical;
		} break;

		case PriestessSkill::MultiSpark:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Multi Spark]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_MultiSpark_DamageBoost, iLevelSkill );
			int iStaticDamage = GetSkillIntValueAt ( pcUser, Priestess_MultiSpark_Damage, iLevelSkill );
			int iDamageBoost = attackPower;
			int iNumSparksMin = GetSkillIntValueAt ( pcUser, Priestess_MultiSpark_NumSparksMin_New, iLevelSkill );
			int iNumSparksMax = GetSkillIntValueAt ( pcUser, Priestess_MultiSpark_NumSparksMax, iLevelSkill );

			int iNumSparksClient = sActiveSkill.sParam;

			if ( ( iNumSparksClient < iNumSparksMin ) || ( iNumSparksClient > iNumSparksMax ) )
			{
				iNumSparksClient = 1;
			}

			attackPower = ( iStaticDamage + ( ( iDamageBoost * iValue ) / 100 ) ) * iNumSparksClient;

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;
		case PriestessSkill::SummonMuspell:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Summon Muspell]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_SummonMuspell_MuspellDMGBoost, iLevelSkill );
			int iDamageBoost = attackPower;
			attackPower += ( ( iDamageBoost * iValue ) / 100 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Critical[%d] Attack Rating[%d]", pcUserData->sCharacterData.iCritical, psPacketEx->iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", attackPower );
		} break;

		default:
			break;
	}


	*piAttackPower = attackPower;

	return 1;
}



int CPriestess::RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx )
{
	UserData * pcUserData = pcUser->pcUserData;

	ActiveSkill sActiveSkill = (ActiveSkill)psPacketRangeEx->iSkillID;
	int uSkillID = psPacketRangeEx->iSkillID & 0xFF;
	int iLevelSkill = ( psPacketRangeEx->iSkillID >> 8 ) & 0xF;
	int iParam = ( psPacketRangeEx->iSkillID >> 12 ) & 0xF;

	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> ID %d, Lvl: %d, Param: %d, %d", uSkillID, iLevelSkill, iParam, sActiveSkill.sParam );

	if ( uSkillID != 0 && iLevelSkill <= 0 || iLevelSkill > 10 )
	{
		WARN ( "Priestess Skill (%d) level (%d) is wrong. Raw = %d", uSkillID, iLevelSkill, psPacketRangeEx->iSkillID );
		return 0;
	}

	int iValue = 0;
	int attackPower = psPacketRange->iDamage;
	int boost, percent, flat;

	switch ( uSkillID )
	{
		case PriestessSkill::DivineLightning:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Divine Lightning]" );
			percent = GetSkillIntValueAt ( pcUser, Priestess_DivineLightning_DamageBoost, iLevelSkill );
			flat = GetSkillIntValueAt ( pcUser, Priestess_DivineLightning_Damage_New, iLevelSkill );

			boost = ( ( psPacketRange->iDamage * percent ) / 100 );

			psPacketRange->eAttackState = EAttackState::State_103_DivineLightning;
			psPacketRange->iDamage += boost;
			psPacketRange->iDamage += flat;
			psPacketRange->iRange = GetSkillIntValueAt ( pcUser, Priestess_DivineLightning_ATKRadius_New, iLevelSkill );
			psPacketRange->iCount = GetSkillIntValueAt ( pcUser, Priestess_DivineLightning_NumOfBolts, iLevelSkill );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> # Bolts = %d , Atk Rtg = %d", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			psPacketRange->iCount &= 0x000000FF;
		}break;

		case PriestessSkill::ChainLightning:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Chain Lightning]" );
			percent = GetSkillIntValueAt ( pcUser, Priestess_ChainLightning_DamageBoost, iLevelSkill );
			flat =  GetSkillIntValueAt ( pcUser, Priestess_ChainLightning_Damage_New, iLevelSkill );

			boost = ( ( psPacketRange->iDamage * percent ) / 100 );

			psPacketRange->eAttackState = EAttackState::State_103_DivineLightning;
			psPacketRange->iDamage += boost;
			psPacketRange->iDamage += flat;
			psPacketRange->iRange =  GetSkillIntValueAt ( pcUser, Priestess_ChainLightning_Range, iLevelSkill );
			psPacketRange->iCount = GetSkillIntValueAt ( pcUser, Priestess_ChainLightning_Numberofbolts, iLevelSkill );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> # Bolts[%d] Attack Rating[%d]", iValue, pcUserData->sCharacterData.iAttackRating );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );

			psPacketRange->iCount &= 0x000000FF;
		}break;

		case PriestessSkill::Extinction:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Extinction]" );
			psPacketRange->iDamage = 0;
			psPacketRange->eAttackState = EAttackState::State_6_Extinction;
			psPacketRange->iRange = GetSkillIntValueAt(pcUser, Priestess_Extinction_Radius_New, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt(pcUser, Priestess_Extinction_SuccessRate, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bParameter = GetSkillIntValueAt ( pcUser, Priestess_Extinction_HPReduction, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.sParameter = GetSkillIntValueAt ( pcUser, Priestess_Extinction_HPReductionCap_New, iLevelSkill );
		} break;



		case PriestessSkill::GlacialSpike:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Glacial Spike]" );
			percent = GetSkillIntValueAt ( pcUser, Priestess_GlacialSpike_DamageBoost, iLevelSkill );

			boost = ( ( psPacketRange->iDamage * percent ) / 100 );

			psPacketRange->iAttackProperty = EAttackProperty::Ice;

			psPacketRangeEx->sElementalAttackSetting.sPercentOfAP = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Priestess_GlacialSpike_FreezePercentOfAP_New, iLevelSkill  );
			psPacketRangeEx->sElementalAttackSetting.bDurationSec = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Priestess_GlacialSpike_FreezeDurationSec, iLevelSkill  );
			psPacketRangeEx->sElementalAttackSetting.bChance = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Priestess_GlacialSpike_FreezeChance, iLevelSkill );
			psPacketRangeEx->sElementalAttackSetting.bSpeedReduction = GetSkillIntValueAt( pcUser, ESkillArrayPointer::Priestess_GlacialSpike_SpeedMonDown_New, iLevelSkill );

			psPacketRange->iDamage += boost;
			psPacketRange->iRange = GetSkillIntValueAt( pcUser, Priestess_GlacialSpike_ATKRange, iLevelSkill );
			psPacketRange->iCount &= 0x000000FF;


			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Skill Damage[%d]", psPacketRange->iDamage );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Freeze Time [%d] Range [%d]", psPacketRangeEx->sElementalAttackSetting.bDurationSec, psPacketRange->iRange );

		}

		default:
			break;
	}
	return 1;
}

int CPriestess::RecvBuffSkill( User * pcUser, PacketSkillCast * packet)
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
		case PriestessSkill::HolyReflection:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Holy Reflection]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_HolyReflection_ReflectDMG, iLevelSkill1Based );
			pcUserData->iHolyReflectionDamage = iValue;
			iValue = GetSkillIntValueAt ( pcUser, Priestess_HolyReflection_Duration, iLevelSkill1Based );
			pcUserData->dwHolyReflectionEndTime = ( iValue * 1000 ) + TICKCOUNT;

			int iHolyReflectionLevel = packet->SParam;
			if (iHolyReflectionLevel <= 0)
			{
				pcUserData->dwHolyReflectionEndTime = 0;
				break;
			}
		}; break;

		//Old
		/*case PriestessSkill::VirtualLife:
		{
			int iTargetID = *(UINT *)( packet + 0x10 );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Virtual Life]" );

			if ( iTargetID != 0 )
			{
				UserData * pcUserDataTarget = USERSERVER->GetUserdata ( iTargetID );
				User * pcUserTarget = USERDATATOUSER ( pcUserDataTarget );

				if ( pcUserTarget && pcUserTarget->IsValid () )
				{
					*(UINT *)( packet + 8 ) = JOBCODEID_Priestess;
					SENDPACKET ( pcUserTarget, (void *)packet );
				}
			}
		}*/

		case PriestessSkill::VirtualLife:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Virtual Life]" );

			const int duration = GetSkillIntValueAt(pcUser, Priestess_VirtualLife_Duration, iLevelSkill1Based);
			ApplyAndSendVirtualLifeBuff( pcUser, iLevelSkill1Based, duration );

		} break;

		case PriestessSkill::VirtualRegen:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Virtual Regen]" );

			const int duration = GetSkillIntValueAt(pcUser, Priestess_VirtualRegen_Duration_New, iLevelSkill1Based);
			ApplyAndSendVirtualRegenBuff( pcUser, iLevelSkill1Based, duration );

		} break;

		//case PriestessSkill::VirtualLife:
		//{
		//	int iTargetID = packet->SParam;
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Virtual Life]" );
		//
		//	int iVirtualLifeHPBoost = 0;
		//	int iVirtualLifeMPBoost = 0;
		//	int iDuration = 300;
		//
		//	User * pcUserTarget = NULL;
		//
		//	if ( iTargetID != 0 )
		//	{
		//		pcUserTarget = USERDATATOUSER (  USERSERVER->GetUserdata ( iTargetID ) );
		//	}
		//
		//
		//
		//	if ( iLevelSkill1Based > 0 && iLevelSkill1Based <= 10 )
		//	{
		//		PacketUseSkillSingle s;
		//		s.iLength = sizeof ( PacketUseSkillSingle );
		//		s.iHeader = PKTHDR_UseSkillNew;
		//		s.iSkillID = SKILLID_VirtualLife;
		//		s.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Priestess;
		//		s.iSkillLevel1Based = iLevelSkill1Based;
		//		s.lCasterID = pcUser->GetID ();
		//		s.lTargetID = pcUserTarget == NULL ? pcUser->GetID () : pcUserTarget->GetID();
		//		SENDPACKETRANGE ( &s, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL );
		//
		//		iVirtualLifeHPBoost = GetSkillIntValueAt(pcUser, Priestess_VirtualLife_HPBoost, iLevelSkill1Based );
		//		iVirtualLifeMPBoost = GetSkillIntValueAt(pcUser, Priestess_VirtualLife_MPBoost_New, iLevelSkill1Based );
		//		iDuration = GetSkillIntValueAt(pcUser, Priestess_VirtualLife_Duration, iLevelSkill1Based );
		//	}
		//	else
		//	{
		//		WARN( "iLevelSkill1Based value (%d) is outside of range", iLevelSkill1Based );
		//	}
		//
		//
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> HP Boost [%d] MP Boost [%d] Duration [%d]", iVirtualLifeHPBoost, iVirtualLifeMPBoost, iDuration );
		//
		//
		//	//Target is other
		//	if ( pcUserTarget )
		//	{
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target is '%s' (%d)", pcUserTarget->GetName(), pcUserTarget->GetID() );
		//
		//		if ( pcUserTarget && pcUserTarget->IsValid() )
		//		{
		//			pcUserTarget->uVirtualLifeLevel = iLevelSkill1Based;
		//			pcUserTarget->uVirtualLifeTimeOut = iLevelSkill1Based > 0 ? TIME + iDuration : 0;
		//			pcUserTarget->uVirtualLifeHPBoost = iVirtualLifeHPBoost;
		//			pcUserTarget->uVirtualLifeMPBoost = iVirtualLifeMPBoost;
		//
		//			pcUserTarget->uVirtualRegenLevel = 0;
		//			pcUserTarget->uVirtualRegenTimeOut = 0;
		//
		//			//cancel existing virtual life if there is any
		//			pcUserTarget->pcUserData->dwVirtualLifeEndTime = 0;
		//			pcUserTarget->pcUserData->iVirtualLifeData = 0;
		//		}
		//	}
		//
		//	//Target is self
		//	else
		//	{
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target is self (%d)", pcUser->GetID() );
		//
		//		pcUser->uVirtualLifeLevel = iLevelSkill1Based;
		//		pcUser->uVirtualLifeTimeOut = iLevelSkill1Based > 0 ? TIME + iDuration : 0;
		//		pcUser->uVirtualLifeHPBoost = iVirtualLifeHPBoost;
		//		pcUser->uVirtualLifeMPBoost = iVirtualLifeMPBoost;
		//
		//		pcUser->uVirtualRegenLevel = 0;
		//		pcUser->uVirtualRegenTimeOut = 0;
		//
		//		//cancel existing virtual life if there is any
		//		pcUser->pcUserData->dwVirtualLifeEndTime = 0;
		//		pcUser->pcUserData->iVirtualLifeData = 0;
		//
		//		//std::cout << "Target self" << std::endl;
		//	}
		//
		//
		//}; break;


		//case PriestessSkill::VirtualRegen:
		//{
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Virtual Regeneration]" );
		//
		//	int iTargetID = packet->SParam;
		//
		//	User * pcUserTarget = NULL;
		//	if ( iTargetID != 0 )
		//	{
		//		pcUserTarget = USERDATATOUSER (  USERSERVER->GetUserdata ( iTargetID ) );
		//		if ( pcUserTarget->IsValid() == FALSE )
		//			pcUserTarget = NULL;
		//	}
		//
		//	int iVirtualRegenHPRegen = 0;
		//	int iVirtualRegenMPRegen = 0;
		//	int iVirtualRegenDuration = 300;
		//	int iVirtualLifeHPBoost = 0;
		//	int iVirtualLifeMPBoost = 0;
		//
		//	//Get VL level
		//	int iVLSkillLevel = pcUser->pData.iLevelSkill[11];
		//
		//	if (iVLSkillLevel > 0 && iVLSkillLevel <= 10)
		//	{
		//		iVirtualLifeHPBoost = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Priestess_VirtualLife_HPBoost, iVLSkillLevel );
		//		iVirtualLifeMPBoost = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Priestess_VirtualLife_MPBoost_New, iVLSkillLevel );
		//	}
		//	else
		//	{
		//		WARN( "iVLSkillLevel value (%d) is outside of range", iVLSkillLevel );
		//	}
		//
		//	if ( iLevelSkill1Based > 0 && iLevelSkill1Based <= 10 )
		//	{
		//		PacketUseSkillSingle s;
		//		s.iLength = sizeof ( PacketUseSkillSingle );
		//		s.iHeader = PKTHDR_UseSkillNew;
		//		s.iSkillID = SKILLID_VirtualRegen;
		//		s.eCasterCharacterClass = ECharacterClass::CHARACTERCLASS_Priestess;
		//		s.iSkillLevel1Based = iLevelSkill1Based;
		//		s.iParameter = iVLSkillLevel;
		//		s.lCasterID = pcUser->GetID ();
		//		s.lTargetID = pcUserTarget == NULL ? pcUser->GetID () : pcUserTarget->GetID();
		//		SENDPACKETRANGE ( &s, &pcUserData->sPosition, pcUserData->iMapID, DISTANCE_MAX_SKILL_VISUAL );
		//
		//		iVirtualRegenHPRegen = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Priestess_VirtualRegen_HPRegen_New, iVLSkillLevel );
		//		iVirtualRegenMPRegen = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Priestess_VirtualRegen_MPRegen_New, iVLSkillLevel );
		//		iVirtualRegenDuration = GetSkillIntValueAt ( pcUser, ESkillArrayPointer::Priestess_VirtualRegen_Duration_New, iVLSkillLevel );
		//	}
		//	else
		//	{
		//		WARN( "iLevelSkill1Based value (%d) is outside of range", iLevelSkill1Based );
		//	}
		//
		//
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> HP Boost [%d] MP Boost [%d]", iVirtualLifeHPBoost, iVirtualLifeMPBoost );
		//	CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> HP Regen [%d] MP Regen [%d] Duration [%d]", iVirtualRegenHPRegen, iVirtualRegenMPRegen, iVirtualRegenDuration );
		//
		//
		//	//Target is other
		//	if ( pcUserTarget )
		//	{
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target is '%s' (%d)", pcUserTarget->GetName(), pcUserTarget->GetID() );
		//
		//		pcUserTarget->uVirtualRegenLevel = iLevelSkill1Based;
		//		pcUserTarget->uVirtualRegenTimeOut = iLevelSkill1Based > 0 ? TIME + iVirtualRegenDuration : 0;
		//		pcUserTarget->uVirtualRegenVLLevel = iVLSkillLevel;
		//
		//		pcUserTarget->uVirtualLifeHPBoost = iVirtualLifeHPBoost;
		//		pcUserTarget->uVirtualLifeMPBoost = iVirtualLifeMPBoost;
		//		pcUserTarget->uVirtualRegenHPRegen = iVirtualRegenHPRegen;
		//		pcUserTarget->uVirtualRegenMPRegen = iVirtualRegenMPRegen;
		//	}
		//
		//	//Target is self
		//	else
		//	{
		//		CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target is self (%d)", pcUser->GetID() );
		//
		//		pcUser->uVirtualRegenLevel = iLevelSkill1Based;
		//		pcUser->uVirtualRegenVLLevel = iVLSkillLevel;
		//		pcUser->uVirtualRegenTimeOut = iLevelSkill1Based > 0 ? TIME + iVirtualRegenDuration : 0;
		//
		//		pcUser->uVirtualLifeHPBoost = iVirtualLifeHPBoost;
		//		pcUser->uVirtualLifeMPBoost = iVirtualLifeMPBoost;
		//		pcUser->uVirtualRegenHPRegen = iVirtualRegenHPRegen;
		//		pcUser->uVirtualRegenMPRegen = iVirtualRegenMPRegen;
		//	}
		//
		//
		//
		//}; break;

		case PriestessSkill::SummonMuspell:
		{

			int iDuration = GetSkillIntValueAt ( pcUser, Priestess_SummonMuspell_Duration, iLevelSkill1Based );
			pcUserData->SummonMuspellTime = ( iDuration * 1000 ) + TICKCOUNT;
			pcUserData->SummonMuspellParam = 0; //done on client side for now.
			pcUser->iSummonMuspellDamageBoost = GetSkillIntValueAt ( pcUser, Priestess_SummonMuspell_DamageBoost_New, iLevelSkill1Based );

			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Summon Muspell]" );
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Duration: %d  Level: %d", iDuration, iLevelSkill1Based );
		}; break;

		case PriestessSkill::Resurrection:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Resurrection]" );
			iValue = GetSkillIntValueAt ( pcUser, Priestess_Resurrection_SuccessRate, iLevelSkill1Based );
			srand( ( UINT )time( NULL ) );
			int PerSuc = Dice::RandomI(0, 99);
			if ( PerSuc <= iValue )
			{
				UserData * pcUserDataDead = USERSERVER->GetUserdata( packet->iParam1 );
				if ( pcUserDataDead )
				{
					if ( pcUserDataDead->iMapID == MAPID_Bellatra )
						break;

					packet->LParam = ECharacterClass::CHARACTERCLASS_Priestess;

					CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> Target '%s' Ressurrected!", pcUserDataDead->sCharacterData.szName );
					SENDPACKET( USERDATATOUSER( pcUserDataDead ), ( void* )packet );
				}

            }
            /// Feedback packet
			else
			{
                PacketDebugDamageInfoContainer l_InfoPacket;
                l_InfoPacket.iHeader = PKTHDR_DamageInfoContainer;
                l_InfoPacket.sAmount = 1;

                auto l_Block = reinterpret_cast<PacketDebugDamageInfo*>(l_InfoPacket.baBuffer);
                l_Block->dwObjectID		= packet->iParam1;
                l_Block->sTypeAction	= EDamageTextType::Miss;
                l_Block->dwAttackerID	= pcUserData->iID;

                l_InfoPacket.iLength = sizeof(Packet) + sizeof(short) + (l_InfoPacket.sAmount * sizeof(PacketDebugDamageInfo));

                SENDPACKET(pcUserData, &l_InfoPacket);
			}
		}; break;

		case PriestessSkill::IceElemental:
		{
			CHATSERVER->SendDebugChat( pcUser, CHATCOLOR_Error, "> [Ice Elemental]" );

			if ( MAPSERVER->IsSummonMap( pcUser->GetMapID() ) == FALSE )
			{
				CHATSERVER->SendTitleBox( pcUserData, "You can't summon a pet on this map" );
				break;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = UNITSERVER->CreateUnitEnemy( "Ice Elemental", pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ, pcUserData );

			if ( pcUnitData )
			{
				pcUnitData->sUnitInfo.dwHashCharacterName	= 0;
				pcUnitData->sCharacterData.iMonsterType		= MONSTERTYPE_Summon;
				pcUnitData->sCharacterData.iOwnerID			= pcUserData->iID;

				pcUnitData->InitHealthPoint( iaIceElementalHP[iLevelSkill1Based - 1] );
				pcUnitData->sCharacterData.iMinDamage = iaIceElementalAttackPower[iLevelSkill1Based - 1];
				pcUnitData->sCharacterData.iMaxDamage = iaIceElementalAttackPower[iLevelSkill1Based - 1];
				pcUnitData->sCharacterData.iAttackRating = iaIceElementalAttackRating[iLevelSkill1Based - 1];

				pcUnitData->pcOwner				= pcUserData;
				pcUnitData->dwUpdateCharInfoTime	= TICKCOUNT + (300 * 1000);

				pcUnitData->sCharacterData.sSize = 224 + iLevelSkill1Based * 15;

				pcUnitData->sCharacterData.iAbsorbRating = 50;

				pcUnitData->saMoveStepCount[0] = 1;
				pcUnitData->saMoveStepCount[1] = 2;

				pcUserData->pcPetData		= pcUnitData;
				pcUserData->dwObjectPetID	= pcUnitData->iID;
				pcUnitData->iPetID			= SKILLID_IceElemental;

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData, SKILLID_IceElemental, 300 );

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

BOOL CPriestess::OnGrandHealingCasted ( User * pcCasterUser, PacketGrandHealing * psPacket, int iRange)
{
	if ( pcCasterUser && pcCasterUser->IsValid () )
	{
		if ( pcCasterUser->bParty && pcCasterUser->psParty )
		{
			std::vector<User *> vUsers = PARTYHANDLER->GetPartyMembers ( pcCasterUser->psParty );
			for ( auto & p : vUsers )
			{
				if ( p && p->IsValid () && !p->pcUserData->IsDead() && p->pcUserData->sPosition.GetPTDistanceXZ( &pcCasterUser->pcUserData->sPosition ) < (iRange * iRange))
				{
					SENDPACKET ( p, psPacket );
				}
			}
		}
		else
		{
			SENDPACKET ( pcCasterUser, psPacket );
		}
	}

	return TRUE;
}



BOOL CPriestess::ApplyAndSendVirtualLifeBuff(User * pcCaster, int iLevel1Based, int iDurationSec )
{
	time_t endTime = TIME + iDurationSec;
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters )) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bVirtualLifeLevel1Based )
		{
			pPartyUser->bVirtualLifeLevel1Based = iLevel1Based;
			pPartyUser->uVirtualLifeEndTime = endTime;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_VirtualLife, iLevel1Based, iDurationSec, 0, pBuffUsers );

	return TRUE;
}

BOOL CPriestess::ApplyAndSendVirtualRegenBuff(User * pcCaster, int iLevel1Based, int iDurationSec)
{
	time_t endTime = TIME + iDurationSec;
	vector<User *> pBuffUsers;

	for ( User * pPartyUser : PARTYHANDLER->GetPartyMembersIncludingSelf( pcCaster, TRUE, DISTANCE_XY_15_meters )) //this will include self too
	{
		//only apply / update if the existing buff level is equal or higher
		if ( iLevel1Based >= pPartyUser->bVirtualRegenLevel1Based )
		{
			pPartyUser->bVirtualRegenLevel1Based = iLevel1Based;
			pPartyUser->uVirtualRegenEndTime = endTime;

			pBuffUsers.push_back( pPartyUser );
		}
	}

	SendPartyBuffSkill( pcCaster, ESkillID::SKILLID_VirtualRegen, iLevel1Based, iDurationSec, 0, pBuffUsers );

	return TRUE;
}


CPriestess::~CPriestess()
{
}
