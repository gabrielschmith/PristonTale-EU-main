#include "stdafx.h"
#include "CAssassinSkillHandler.h"
#include "SkillManager.h"

CAssassinSkillHandler::CAssassinSkillHandler()
{
}

CAssassinSkillHandler::~CAssassinSkillHandler()
{
}


void CAssassinSkillHandler::LoadParticle ()
{
	iAlasScriptCastD = PARTICLE->Load ( "Effect\\Particle\\Script\\Alas_Cast.part" );
	iAlasScriptKeepID = PARTICLE->Load ( "Effect\\Particle\\Script\\Alas_Keep.part" );

	iInpesScriptCastD = PARTICLE->Load ( "Effect\\Particle\\Script\\Inpes_Cast.part" );
	iInpesScriptKeepID = PARTICLE->Load ( "Effect\\Particle\\Script\\Inpes_Keep.part" );
}

BOOL CAssassinSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{
		case SKILLID_VenomThorn:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::VenomThorn;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}


			break;
		}

		case SKILLID_Inpes:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				 USEMANASTAMINA( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Inpes;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}

			break;
		}

		case SKILLID_Alas:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				USEMANASTAMINA( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Alas;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}

			break;
		}

		case SKILLID_Polluted:
		{


			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL)
			{
				int iValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Assassin_Polluted_Range, ESkillID::SKILLID_Polluted );

				if ( SETTINGSHANDLER->GetModel()->GetSettings().bAutoSelectNearestTargetForAoE )
				{
					UNITDATA_HOVER = SKILLMANAGERHANDLER->FindNearestMonster( UNITDATA, iValue );
				}
				else
				{
					if ( UNITDATA->pcTarget == NULL )
					{
						UNITDATA->pcTarget = SKILLMANAGERHANDLER->FindNearestMonster( UNITDATA, iValue );
					}

					if ( UNITDATA->pcTarget )
					{
						CANATTACK = TRUE;
						SELECTED_MOUSE_BUTTON = 2;
						AUTOMOUSE_WM_COUNT++;

						OnPlaySkillAttack( psSkill, UNITDATA->pcTarget );

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}

			//No nearby target still. Return
			//FALSE so that DL will be casted once (via assembly)
			if ( UNITDATA_HOVER == NULL )
			{
				return FALSE;
			}

			return 2; //return TRUE but also bypass ASM to allow these skills to be held down. See BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}



BOOL CAssassinSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	//CHATBOX->AddDebugMessage( "----------------" );
	//CHATBOX->AddDebugMessage( "FOCUS: %s", UNITDATA_FOCUSED == NULL ? "None" : UNITDATA_FOCUSED->GetName() );
	//CHATBOX->AddDebugMessage( "HOVER: %s", UNITDATA_HOVER == NULL ? "None" : UNITDATA_HOVER->GetName() );
	//CHATBOX->AddDebugMessage( "FOLLOWMODE_ACTION: %d", FOLLOWMODE_ACTION );
	//CHATBOX->AddDebugMessage( "CANATTACK: %d", CANATTACK );
	//CHATBOX->AddDebugMessage( "SELECTED_MOUSE_BUTTON: %d", SELECTED_MOUSE_BUTTON );
	//CHATBOX->AddDebugMessage( "DISP_EACH_MODE: %d", DISP_EACH_MODE );
	//CHATBOX->AddDebugMessage( "AUTOMOUSE_WM_COUNT: %d", AUTOMOUSE_WM_COUNT );

	switch ( psSkill->iID )
	{
		case SKILLID_GustSlash:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::GustSlash;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, 0);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_SoreBlade:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::SoreBlade;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, 0);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_Stinger:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Stinger;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_RunningHit:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::RunningHit;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);


				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_NinjaShadow:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::NinjaShadow;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = 80;
				//UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendTransDamage(UNITDATA->pcTarget, UNITDATA->sActiveSkill);
			}
		}
		break;

		case SKILLID_Wisp:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Wisp;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
				UNITDATA->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_Polluted:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Polluted;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed);

				//UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_Cripple:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = AssassinSkill::Cripple;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
				UNITDATA->iMotionLoopCnt = 1;
			}
		}

		case SKILLID_ShadowBlink:
		{
			return SHADOWBLINKHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}

		default:
		{
			return FALSE; //return FALSE to have it handled in ASM instead
		}
	}

	return TRUE; //return TRUE as it is handled here
}

BOOL CAssassinSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D)
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case AssassinSkill::VenomThorn:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_V_THRONE, point3D );
			break;
		}

		case AssassinSkill::Wisp:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_WISP, point3D );
			SKILLMANAGERHANDLER->AssaParticle_AttackSkillEffect( "Wisp1", pcThis, 0 );
			break;
		}

		case AssassinSkill::Cripple:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_F_WIND, point3D );
			SKILLMANAGERHANDLER->AssaParticle_AttackSkillEffect( "FrostWindBegin", pcThis, 0 );
			break;
		}

		case AssassinSkill::Inpes:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_INPES, point3D );
			break;
		}

		case AssassinSkill::Alas:
		{
			pcThis->iAttackEffect = TRUE;
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_ALAS, point3D );
			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CAssassinSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;
	int duration;
	BOOL bHandled = FALSE;

	switch( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case AssassinSkill::VenomThorn:
		{
			break;
		}

		case AssassinSkill::GustSlash:
		{
			if ( pcUnitData->pcTarget )
			{
				SKILLMANAGERHANDLER->AssaParticle_AttackSkillEffect("BeatUp", pcUnitData->pcTarget);

				if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent )
				{
					const int iRange = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Assassin_GustSlash_Area, pcUnitData->sActiveSkill.cLevel - 1);

					SKILLMANAGERHANDLER->SetUnitsRange(pcUnitData->pcTarget->sPosition, iRange, FALSE);
					SKILLMANAGERHANDLER->SendRangeDamage(pcUnitData, pcUnitData->sActiveSkill.iValue);
				}
			}
			break;
		}

		case AssassinSkill::Wisp:
		{
			if ( pcUnitData->pcTarget )
			{
				if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent )
				{
					SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill, FALSE );
				}

				//duration = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Assassin_Wisp_Curse_Duration_New, )
				//
				//SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect("Wisp2", pcUnitData->pcTarget, 15, ESkillID::SKILLID_Wisp);
				//
				//if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent  )
				//{
				//	cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );
				//	pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected
				//}

			}
			break;
		}
		case AssassinSkill::Cripple:
		{
			if ( pcUnitData->pcTarget )
			{
				SKILLMANAGERHANDLER->AssaParticle_AttackSkillEffect( "FrostWindAttack", pcUnitData->pcTarget );

				if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent  )
				{
					cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );
					pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected
				}
			}

			break;
		}

		case AssassinSkill::Alas:
		{
			if ( pcUnitData->iMotionEvent )
			{
				//SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect( "Alas", pcUnitData, 300, ESkillID::SKILLID_Alas );
			}

			break;
		}

		case AssassinSkill::Inpes:
		{
			if ( pcUnitData->iMotionEvent )
			{
				//SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect( "Inpes", pcUnitData, 300, ESkillID::SKILLID_Inpes );
			}

			break;
		}

		default:
		{
			//not handled here, handle it in ASM instead.
			return FALSE;
		}

		//case SKILLPLAYID_T51: return SHADOWBLINKHANDLER->OnEvent( pcUnitData );
		//break;
	}

	//it was handled here, don't handle it in ASM
	return TRUE;
}


/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// Perhaps have a look at
/// int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill ) in the 2012 code
/// how to make this better, i.e. player coming in range when previously not in range
/// </summary>
BOOL CAssassinSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	int duration;

	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_VenomThorn:
		{
			SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect( "VenomThrone", pcUnitTarget->pcUnitData, psPacket->sTimeLeftSec, psPacket->iSkillID );
			break;
		}

		case ESkillID::SKILLID_Inpes:
		{
			PARTICLE->CheckAndStartCastScript( pcUnitTarget, iInpesScriptCastD, psPacket->iSkillLevel1Based, pcUnitTarget->bInpesLevel1Based );

			break;
		}


		case ESkillID::SKILLID_Alas:
		{
			PARTICLE->CheckAndStartCastScript( pcUnitTarget, iAlasScriptCastD, psPacket->iSkillLevel1Based, pcUnitTarget->bAlasLevel1Based );

			break;
		}
	}

	return TRUE;
}



BOOL CAssassinSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_VenomThorn, ETimerID::VenomThorn, ESkillArrayPointer::Assassin_VenomThorn_Duration_New,
			pStatus->uVenomThornTimeLeft, pStatus->bVenomLevel1Based );

		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_Alas, ETimerID::Alas, ESkillArrayPointer::Assassin_Alas_Duration,
			pStatus->uAlasTimeLeft, pStatus->bAlasLevel1Based );

		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_Inpes, ETimerID::Inpes, ESkillArrayPointer::Assassin_Inpes_Duration,
			pStatus->uInpesTimeLeft, pStatus->bInpesLevel1Based );
	}

	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iAlasScriptKeepID, pStatus->uAlasTimeLeft, pStatus->bAlasLevel1Based, pcUnitTarget->bAlasLevel1Based );
	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iInpesScriptKeepID, pStatus->uInpesTimeLeft, pStatus->bInpesLevel1Based, pcUnitTarget->bInpesLevel1Based );


	return TRUE;
}