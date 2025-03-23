#include "stdafx.h"
#include "CArcherSkillHandler.h"
#include "SkillManager.h"

CArcherSkillHandler::CArcherSkillHandler()
{
}

CArcherSkillHandler::~CArcherSkillHandler()
{
}

void CArcherSkillHandler::LoadParticle ()
{
	//iAlasScriptCastD = PARTICLE->Load ( "Effect\\Particle\\Script\\Alas_Cast.part" );
	//iAlasScriptKeepID = PARTICLE->Load ( "Effect\\Particle\\Script\\Alas_Keep.part" );
	//
	//iInpesScriptCastD = PARTICLE->Load ( "Effect\\Particle\\Script\\Inpes_Cast.part" );
	//iInpesScriptKeepID = PARTICLE->Load ( "Effect\\Particle\\Script\\Inpes_Keep.part" );
}


BOOL CArcherSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{
		case SKILLID_ForceofNature:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() && USEMANASTAMINA( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = ArcherSkill::ForceofNature;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}

			break;
		}

		case SKILLID_LethalSight:
		{
			return LETHALSIGHTHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
		}
		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}

BOOL CArcherSkillHandler::OnPlaySkillAttack ( struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	RECT rect;
	UnitData * pcPlayerUnitData = UNITDATA;

	switch ( psSkill->iID )
	{
		case SKILLID_FierceWind:
		{
			return FIERCEWINDHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}

		case SKILLID_Perforation:
		{
			if ( USESKILL ( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Attack );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = ArcherSkill::Perforation;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

				pcPlayerUnitData->BeginSkill( pcMouseTarget );

				int iAttackRange = pcPlayerUnitData->sCharacterData.iAttackRange + SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Archer_Perforation_AddShotRange, psSkill->iLevel - 1 );
				int iAttackWidth = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Archer_Perforation_AttackWidth_New, psSkill->iLevel - 1 );

				rect.left = -( iAttackWidth >> 1 );	//half
				rect.right = ( iAttackWidth >> 1 );	//half
				rect.top = 0;
				rect.bottom = iAttackRange;

				SKILLMANAGERHANDLER->SelectRangeBox ( pcPlayerUnitData, rect, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage ( pcPlayerUnitData, pcPlayerUnitData->sPosition, 0, 0, 0, 0, pcPlayerUnitData->sActiveSkill.iValue );

				pcPlayerUnitData->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayerUnitData->sCharacterData.iAttackSpeed, 1 );
				pcPlayerUnitData->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_Avalanche:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = ArcherSkill::Avalanche;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

				pcPlayerUnitData->BeginSkill( pcMouseTarget );

				pcPlayerUnitData->Animate( ANIMATIONTYPE_Skill );

				pcPlayerUnitData->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayerUnitData->sCharacterData.iAttackSpeed, SKILLMANAGERHANDLER->GetSkillIntValue( Archer_Avalanche_AddATKSpeed, psSkill->iLevel - 1 ) );
				pcPlayerUnitData->iMotionLoopCnt = SKILLMANAGERHANDLER->GetSkillIntValue( Archer_Avalanche_NumberofArrows, psSkill->iLevel - 1 );

				SKILLMANAGERHANDLER->SendTransDamage( pcMouseTarget, pcPlayerUnitData->sActiveSkill );
			}

			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CArcherSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D)
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CArcherSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;

	switch ( psActiveSkill->cSkillTinyID )
	{
		case SKILLPLAYID_T52:
		{
			return FIERCEWINDHANDLER->OnEvent ( pcUnitData );
		}

		case ArcherSkill::ForceofNature:
		{
			return TRUE; //don't handled in ASM
		}

		case ArcherSkill::PhoenixShot:
		{
			int iSkillLevel = SKILLMANAGERHANDLER->GetLevelSkill ( SKILLID_PhoenixShot );

			if ( pcUnitData && pcUnitData->pcTarget && psActiveSkill->cLevel )
			{
				cnt = pcUnitData->iFrame - pcUnitData->psModelAnimation->iBeginFrame * 160;

				if ( pcUnitData->psModelAnimation->iaEventFrame[1] <= (int)cnt )
				{
					SKILLMANAGERHANDLER->GetMoveLocation ( 0, 24 << 8, 24 << 8, 0, pcUnitData->sAngle.iY, 0 );

					Pos1.iX = pcUnitData->sPosition.iX + RADIAN3D_X;
					Pos1.iY = pcUnitData->sPosition.iY + RADIAN3D_Y;
					Pos1.iZ = pcUnitData->sPosition.iZ + RADIAN3D_Z;

					Pos2.iX = pcUnitData->pcTarget->sPosition.iX + RADIAN3D_X;
					Pos2.iY = pcUnitData->pcTarget->sPosition.iY + RADIAN3D_Y;
					Pos2.iZ = pcUnitData->pcTarget->sPosition.iZ + RADIAN3D_Z;

					CALL_WITH_ARG3 ( 0x5D4F30, (DWORD)pcUnitData, (DWORD)&Pos1, (DWORD)&Pos2 ); //SkillSagittarionPhoneixShot

					//Is me
					if ( pcUnitData == UNITDATA )
					{
						SKILLSOUND_STOP ( SKILL_SOUND_SKILL_CHARGING );

						int iAttackWidth = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Archer_PhoenixShot_AttackWidth_New, psActiveSkill->cLevel - 1 );
						int iAttackRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Archer_PhoenixShot_AttackRange_New, psActiveSkill->cLevel - 1 );

						rect.left = -( iAttackWidth >> 1 );	//half
						rect.right = ( iAttackWidth >> 1 );	//half
						rect.top = 0;
						rect.bottom = iAttackRange;

						if ( pcUnitData->bIsCharging == FALSE ) //Note - FALSE means that it is a charged attack.
						{
							psActiveSkill->sParam = 1; //Charged attack
						}
						else
						{
							psActiveSkill->sParam = 0; //Non-Charged attack
						}

						SKILLMANAGERHANDLER->SelectRangeBox ( UNITDATA, rect, FALSE );
						SKILLMANAGERHANDLER->SendRangeDamage ( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, psActiveSkill->iValue );

						UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed ( UNITDATA->sCharacterData.iAttackSpeed );
						UNITDATA->iMotionLoopCnt = 1;

						WRITEDWORD ( 0xA1756C, 0 );	//MouseButton[1] = 0;?
					}

					SKILLMANAGERHANDLER->SkillPlaySound ( SKILL_SOUND_SKILL_PHOENIX_SHOT, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
				}
				else
				{
					if ( pcUnitData == UNITDATA )
					{
						UNITDATA->bIsCharging = FALSE;
					}
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// Perhaps have a look at
/// int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill ) in the 2012 code
/// how to make this better, i.e. player coming in range when previously not in range
/// </summary>
BOOL CArcherSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	int duration;

	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_ForceofNature:
		{
			int v10;
			*(float *)&v10 = 5;
			CALL_WITH_ARG2( 0x5D4380, (DWORD)pcUnitTarget->pcUnitData, (DWORD)v10 ); //void SkillSagittarionForceOfNature1(smCHAR *pChar, float time)
			break;
		}
	}

	return TRUE;
}



BOOL CArcherSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_ForceofNature, ETimerID::ForceofNature, ESkillArrayPointer::Archer_ForceofNature_Duration,
			pStatus->uForceOfNatureTimeLeft, pStatus->bForceOfNatureLevel1Based );
	}

	int v10;
	*(float *)&v10 = pStatus->uForceOfNatureTimeLeft;
	CALL_WITH_ARG2( 0x5D4430, (DWORD)pcUnitTarget->pcUnitData, (DWORD)v10 ); //void SkillSagittarionForceOfNature2(smCHAR *pChar, float time)

	return TRUE;
}