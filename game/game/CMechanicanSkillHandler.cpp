#include "stdafx.h"
#include "CMechanicianSkillHandler.h"
#include "SkillManager.h"

CMechanicianSkillHandler::CMechanicianSkillHandler()
{
}

CMechanicianSkillHandler::~CMechanicianSkillHandler()
{
}

void CMechanicianSkillHandler::LoadParticle()
{
	this->electricalOverloadCastParticle = PARTICLE->Load("Effect\\Particle\\Script\\Alas_Cast.part");
	this->electricalOverloadKeepParticle = PARTICLE->Load("Effect\\Particle\\Script\\Alas_Keep.part");
}

BOOL CMechanicianSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{
		case SKILLID_ElectricalOverload:
			if (UNITDATA->IsLowAnimationType() &&
				USEMANASTAMINA(psSkill))
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = 0x5B; // Use Tier 5, skill 2 animation
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill(UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->sActiveSkill.cSkillTinyID = MechanicanSkill::ElectricalOverload;
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer(UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel);

				TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_ElectricalOverload, ETimerID::ElectricalOverload, ESkillArrayPointer::Mech_ElectricalOverload_Duration,
					SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Mech_ElectricalOverload_Duration, psSkill->iLevel - 1 ), psSkill->iLevel );

			}
			break;
		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}

BOOL CMechanicianSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;
	int	damage;
	int value;

	switch ( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case MechanicanSkill::MechanicBomb:
		{
			if ( pcUnitData->pcTarget )
			{
				if ( psActiveSkill->cLevel )
				{
					//player
					Pos1.iX = pcUnitData->sPosition.iX;
					Pos1.iY = pcUnitData->sPosition.iY + 7680; //start above player
					Pos1.iZ = pcUnitData->sPosition.iZ;

					//target
					Pos2.iX = pcUnitData->pcTarget->sPosition.iX;
					Pos2.iY = pcUnitData->pcTarget->sPosition.iY;
					Pos2.iZ = pcUnitData->pcTarget->sPosition.iZ;

					SKILLMANAGERHANDLER->StartSkillEffect ( Pos1, Pos2, SKILLID_MechanicBomb, psActiveSkill->cLevel );
					SKILLMANAGERHANDLER->SkillPlaySound( 0x1020, Pos1.iX, Pos1.iY, Pos1.iZ ); //0x1020 = SKILL_SOUND_MECHANIC_BOMB1

					//is me, otherwise someone else played the skill
					if ( UNITDATA == pcUnitData )
					{
						value = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Mech_MechanicBomb_Area, ESkillID::SKILLID_MechanicBomb );
						SKILLMANAGERHANDLER->SetUnitsRange ( UNITDATA->pcTarget->sPosition, value, FALSE );
						SKILLMANAGERHANDLER->SendRangeDamage ( UNITDATA->pcTarget, UNITDATA->pcTarget->sPosition, 0, 0, 0, 0, psActiveSkill->iValue );
					}
				}
			}

			return TRUE;
		}



		break;

		case MechanicanSkill::Impulsion:	return IMPULSIONHANDLER->OnEvent( pcUnitData );
		case SKILLPLAYID_T51:
		return RUPTUREHANDLER->OnEvent( pcUnitData );
		break;
		case SKILLPLAYID_T52:
		break;
		case SKILLPLAYID_T53:
		break;
		case SKILLPLAYID_T54:
		break;
	}

	return FALSE;
}

BOOL CMechanicianSkillHandler::OnSkillBuff(Unit* pcUnit, PacketUseSkillBase* psPacket)
{
	int duration;

	BOOL bIsTheCaster = pcUnit->GetID() == psPacket->lCasterID;

	switch (psPacket->iSkillID)
	{
		case ESkillID::SKILLID_ElectricalOverload:
		{
			PARTICLE->CheckAndStartCastScript(pcUnit, this->electricalOverloadCastParticle, psPacket->iSkillLevel1Based, psPacket->iSkillLevel1Based);
			break;
		}
	}

	return TRUE;
}

BOOL CMechanicianSkillHandler::OnBeginSkill(UnitData* pcThis, ActiveSkill& sActiveSkill, int iLevel1Based, UnitData* pcUnitTarget, Point3D& point3D)
{
	switch (sActiveSkill.cSkillTinyID)
	{
		case MechanicanSkill::ElectricalOverload:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_SKILL_EXTINTION, point3D);

			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CMechanicianSkillHandler::OnSkillBuffUpdate(Unit* pcUnitTarget, PacketSkillBuffStatus* pStatus)
{
	if (pcUnitTarget == UNIT)
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer(ESkillID::SKILLID_ElectricalOverload, ETimerID::ElectricalOverload, ESkillArrayPointer::Mech_ElectricalOverload_Duration,
			pStatus->uElectricalOverloadTimeLeft, pStatus->bElectricalOverloadLevel1Based);
	}

	PARTICLE->CheckAndStartKeepScript(pcUnitTarget, this->electricalOverloadKeepParticle, pStatus->uElectricalOverloadTimeLeft, pStatus->bElectricalOverloadLevel1Based, pcUnitTarget->bElectricalOverloadLevel1Based);
	return TRUE;
}

BOOL CMechanicianSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	int iTemp;

	switch ( psSkill->iID )
	{
		case SKILLID_Spark:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = MechanicanSkill::Spark;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = 0;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, MechanicanSkill::MechanicBomb, 0, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				UNITDATA->bHideRightWeapon = TRUE;

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
				UNITDATA->iMotionLoopCnt = 1;
			}

			return TRUE;
		}

		case SKILLID_MechanicBomb:
		{
			if ( USESKILL ( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = MechanicanSkill::MechanicBomb;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = 0;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, MechanicanSkill::MechanicBomb, 0, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				UNITDATA->bHideRightWeapon = TRUE;

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed, 1 );
				UNITDATA->iMotionLoopCnt = 1;
			}

			return TRUE;
		}

		case SKILLID_GreatSmash:
		{
			if ( USESKILL ( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = MechanicanSkill::GreatSmash;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = 0;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, 0, 0, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed ( UNITDATA->sCharacterData.iAttackSpeed, 5);
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendTransDamage ( UNITDATA->pcTarget, UNITDATA->sActiveSkill );
			}

			return TRUE;
		}

		case SKILLID_GrandSmash:
		{
			if ( USESKILL ( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = MechanicanSkill::GrandSmash;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = 0;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, 0, 0, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed ( UNITDATA->sCharacterData.iAttackSpeed, 4 );
				UNITDATA->iMotionLoopCnt = 1;

				iTemp = UNITDATA->sCharacterData.iAttackRating;

				if ( CHARACTERGAME->IsLeftSlotEquippped ( EItemType::ITEMTYPE_Hammer ) || CHARACTERGAME->IsLeftSlotEquippped ( EItemType::ITEMTYPE_Claw ) )
				{
					int iAttackRatingBoost = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Mech_GrandSmash_AtkRtgBoost, ESkillID::SKILLID_GrandSmash );
					UNITDATA->sCharacterData.iAttackRating += iAttackRatingBoost;
				}

				SKILLMANAGERHANDLER->SendTransDamage ( UNITDATA->pcTarget, UNITDATA->sActiveSkill );

				UNITDATA->sCharacterData.iAttackRating = iTemp; //restore
			}

			return TRUE;
		}

		case SKILLID_Rupture:
		{
			return RUPTUREHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}
	}

	return FALSE;
}