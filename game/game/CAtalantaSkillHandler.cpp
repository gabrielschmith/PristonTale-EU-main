#include "stdafx.h"
#include "CAtalantaSkillHandler.h"
#include "SkillManager.h"

CAtalantaSkillHandler::CAtalantaSkillHandler()
{
}

CAtalantaSkillHandler::~CAtalantaSkillHandler()
{
}

void CAtalantaSkillHandler::LoadParticle ()
{
	iValhallaScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3AtalantaTriumphOfValhalla.part" );
}



BOOL CAtalantaSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{		
		case SKILLID_HallofValhalla:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				 USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = AtalantaSkill::HallofValhalla;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );  
			}

			break;
		}

		case SKILLID_TriumphofValhalla:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = AtalantaSkill::TriumphofValhalla;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill( );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );  
			}

			break;

			break;
		}


		case SKILLID_SummonTiger:
			return SUMMONTIGERHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
			break;

		case SKILLID_Windy:
		{
			iValue = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Atalanta_Windy_Range_New, ESkillID::SKILLID_Windy );

			if ( iValue )
			{
				WRITEBYTE ( 0x004BDCBA, (BYTE)iValue );
						
#ifdef SKILLS_BEHAVIOUR_DEBUG
				CHATBOX->AddDebugMessage ( FormatString ( "Add windy range: %d", iValue ) );
#endif
			}

			return FALSE; //allow it to be processed in ASM
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}
BOOL CAtalantaSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	switch ( psSkill->iID )
	{
		case SKILLID_SummonTiger:
		{
			return SUMMONTIGERHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}



		case SKILLID_LightningJavelin:
		{
			if ( UNITDATA->CheckShootingTest ( pcMouseTarget ) == FALSE )
			{
				 UNITDATA->iTargetCritical = -1;
				 UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Idle );
				 return TRUE;
			}

			if ( psSkill->iLevel && USESKILL( psSkill) == TRUE )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = AtalantaSkill::LightningJavelin;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, 0, 0, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				int iRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Atalanta_LightningJavelin_Radius_New, psSkill->iLevel - 1 );

				SKILLMANAGERHANDLER->SetUnitsRange ( pcMouseTarget->sPosition, iRange, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage ( pcMouseTarget, pcMouseTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}

			//if ( SKILL_LEFTSIDE == psSkill && SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget ) ) break;

			return TRUE;
		}
	}	
	
	return FALSE;	
}

BOOL CAtalantaSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D )
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case AtalantaSkill::HallofValhalla:
		{
			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_SKILL_HALL_OF_VALHALLA, point3D);
			break;
		}

		case  AtalantaSkill::TriumphofValhalla:
		{
			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_SKILL_TRIUMPH_OF_VALHALLA1, point3D);
			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}



BOOL CAtalantaSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;
	int duration;
	BOOL bHandled = FALSE;

	switch( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case AtalantaSkill::HallofValhalla:
		{
			break;
		}

		case AtalantaSkill::TriumphofValhalla:
		{
			int iDuration = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Atalanta_TriumphOfValhalla_Duration, psActiveSkill->cLevel - 1 );

			//AssaParticle_TriumphOfValhallaMember_5BE670
			CALL_WITH_ARG3( 0x5BE670,  (DWORD)pcUnitData, (DWORD)pcUnitData, (DWORD)iDuration);

			break;
		}
		default:
		{
			//not handled here, handle it in ASM instead.
			return FALSE;
		}
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
BOOL CAtalantaSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	int duration;


	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_HallofValhalla:
		{
			PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iValhallaScriptID, psPacket->sTimeLeftSec, psPacket->iSkillLevel1Based, pcUnitTarget->bHallOfValhallaLevel1Based, TRUE, TRUE );
			
			//SkillValhallaHallOfValhalla1()
			//CALL_WITH_ARG1( 0x5D4660, (DWORD)pcUnitTarget->pcUnitData ); 
			
			//SkillValhallaHallOfValhallaHandEffect()
			//CALL_WITH_ARG2( 0x5BE710, (DWORD)pcUnitTarget->pcUnitData, psPacket->sTimeLeftSec ); 

			break;
		}

		case ESkillID::SKILLID_TriumphofValhalla:
		{			
			//AssaParticle_TriumphOfValhalla_5BE5E0
			//CALL_WITH_ARG2( 0x5BE5E0, (DWORD)pcUnitTarget->pcUnitData, psPacket->sTimeLeftSec );

			PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iValhallaScriptID, psPacket->sTimeLeftSec, psPacket->iSkillLevel1Based, pcUnitTarget->bTriumphOfValhallaLevel1Based, TRUE, TRUE );


			break;
		}
	}

	return TRUE;
}

BOOL CAtalantaSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_HallofValhalla, ETimerID::HallOfValhalla, ESkillArrayPointer::Atalanta_HallOfValhalla_Duration,
			pStatus->uHallOfValhallaTimeLeft, pStatus->bHallOfValhallaLevel1Based );
		
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_TriumphofValhalla, ETimerID::TriumphOfValhalla, ESkillArrayPointer::Atalanta_TriumphOfValhalla_Duration,
			pStatus->uTriumphOfValhallaTimeLeft, pStatus->bTriumphOfValhallaLevel1Based );
	}

	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iValhallaScriptID, pStatus->uHallOfValhallaTimeLeft, pStatus->bHallOfValhallaLevel1Based, pcUnitTarget->bHallOfValhallaLevel1Based, TRUE, TRUE );
	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iValhallaScriptID, pStatus->uTriumphOfValhallaTimeLeft, pStatus->bTriumphOfValhallaLevel1Based, pcUnitTarget->bTriumphOfValhallaLevel1Based, TRUE, TRUE );

	return TRUE;
}