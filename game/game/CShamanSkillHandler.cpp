#include "stdafx.h"
#include "CShamanSkillHandler.h"
#include "SkillManager.h"

typedef int( __thiscall *t_SkillCelestialMusPel ) ( UnitData * pcUnitData, float fDuration, float fAttackDelay, ESkillID eSkillID );
t_SkillCelestialMusPel SkillCelestialMusPel = ( t_SkillCelestialMusPel )0x5D48F0;

class HoEffectTypeInterface
{
public:
	bool		m_bLiveFlag;
	int			m_iCode;
	float		m_fEndTime;
	UnitData  * m_pAttachChar;

};

struct HoEffectType_MusPel : HoEffectTypeInterface
{
public:
	UnitData	*m_pAttachChar;
	bool		m_bMyCharacterFlag;

	DWORD	   *m_pMesh;
	float		m_fElapsedTime;
	float		m_fEndTime;
	float		m_fCurrentFrame;

	int			m_State;
	int			m_NextState;

	float		m_Alpha;

	int			m_ParticleID;
	DWORD		*m_pObj;

	float		m_fAttackTime;
	float		m_fAttackTimeCount;
	UnitData     *m_AttackMonster;

	bool		m_bHandEffectFlag;
	bool		m_bHitEffectFlag;
	int			m_HandParticleNum;

	float		m_CurrentPos[3];
	int         dwskillCode;
};


CShamanSkillHandler::CShamanSkillHandler()
{
}

CShamanSkillHandler::~CShamanSkillHandler()
{
}


void CShamanSkillHandler::LoadParticle ()
{
	iAdventMigalCastScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\MigalStart.part" );
	iAdventMigalKeepScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\MigalMember.part" ); //icon above player
	//iAdventMigalKeepScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\MigalEnd.part" ); //red-orange glow
	//iAdventMigalKeepScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\MigalStart.part" );  //red-like ball
}

BOOL CShamanSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{			
		case SKILLID_AdventMigal:
		{
			if ( USEMANASTAMINA( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;
		
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::AdventMigal;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->BeginSkill();
		
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}
		}
		break;

		case SKILLID_Creed:			return CREEDHANDLER->OnCastAnimationStart( psSkill );

		case SKILLID_MourningPray:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::MourningPray;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, 0);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_ChasingHunt:
		{
			if ( USESKILL( psSkill ) )
			{
				//Update Skill Data
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::ChasingHunt;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				//Animate...
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				//New FPS based on Weapon Speed
				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
				UNITDATA->iMotionLoopCnt = 1;

				TIMERSKILLHANDLER->OnAddChasingHunt( ((int*)0x00998B10)[psSkill->iLevel - 1], psSkill->iLevel );

				//Buff
				PROCESSSKILLBUFF( UNITDATA );

				if ( UNITDATA->pcTarget )
				{
					//Area Hook?
					SKILLMANAGERHANDLER->BeginSkill( UNITDATA, UNITDATA->sActiveSkill.iValue, 0, UNITDATA->pcTarget, 0, 0, 0 );

					//Left Side? Send Area Skill Hook
					if ( SKILL_LEFTSIDE == psSkill )
						CALL_WITH_ARG1( 0x0040F3F0, (DWORD)UNITDATA->pcTarget );
				}
			}
		}
		break;
		
		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE; //return TRUE to indicate that we have handled it here.
}

BOOL CShamanSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	switch ( psSkill->iID )
	{
		case SKILLID_AdventMigal:
		{
			//handled in OnOpenPlaySkill only
		}
		break;

		case SKILLID_DarkBolt:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::DarkBolt;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = 90;
				//UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_Scratch:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::Scratch;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, 1);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_Haunt:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::Haunt;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_DarkWave:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::DarkWave;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = 90;
				//UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_SpiritualFlare:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::SpiritualFlare;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				UNITDATA->iMotionLoopSpeed = 90;
				//UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, -2);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_Inertia:
		{
			if (USESKILL(psSkill))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = ShamanSkill::Inertia;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill(UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0);
				UNITDATA->Animate(ANIMATIONTYPE_Skill);

				//UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed(UNITDATA->sCharacterData.iAttackSpeed, 3);
				UNITDATA->iMotionLoopCnt = 1;
			}
		}
		break;

		case SKILLID_Creed:
		{
			return CREEDHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CShamanSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D )
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case ShamanSkill::AdventMigal:
		{
			CALL_WITH_ARG3( 0x531E00, (DWORD)&point3D, 10, 0 ); //sinEffect_StartMagic(&pos, 10, 0); (int sinEffect_StartMagic(POINT3D *pPosi, int CharFlag, int Type))
			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_A_MICHAL1, point3D);
		}
		break;

		default:
		{
			return FALSE; //not handled (will be handled in ASM instead)
		}
	}

	return TRUE; //handled
}


BOOL CShamanSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;

	switch ( psActiveSkill->cSkillTinyID )
	{
		case ShamanSkill::AdventMigal:
		{
			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_A_MICHAL2, pcUnitData->sPosition);

			if ( pcUnitData->iMotionEvent )
			{
				//int v10 = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Shaman_AdventMigal_Duration, psActiveSkill->cLevel - 1 );
				//*(float *)&v10 = (float)v10;

				//auto instance = CALL( 0x5CCA70 ); //HoEffectManager::GetInstance();

				//PARTICLE->Start( "MigalEnd", UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
				
			

				//CALL_WITH_ARG4( 0x005D48F0, (DWORD)pcUnitData, (DWORD)v10, (DWORD)0, (DWORD)ESkillID::SKILLID_AdventMigal);

				

				//CALL_WITH_ARG4( 0x005D48F0, (DWORD)pcUnitData, (DWORD)iDurationSec, (DWORD)0.0f, (DWORD)ESkillID::SKILLID_AdventMigal );
				//SkillCelestialMusPel( pcUnitData, (float)iDurationSec, (float)0, ESkillID::SKILLID_AdventMigal ); //SkillCelestialMusPel(this, (float)AdventMigal_Time[point - 1], (float)Summon_Muspell_Attack_Delay[point - 1], SKILL_A_MIGAL);
			
				if ( pcUnitData == UNITDATA )
				{
					//CALL_WITH_ARG2( 0x4ED560, iDurationSec, psActiveSkill->cLevel - 1 ); //this asdds the timer
				}
			}
		}
		break;

		default:
		{
			return FALSE; //not handled (will be handled in ASM instead)
		}
	}

	return TRUE; //handled
}


/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// </summary>
BOOL CShamanSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_AdventMigal:
		{
			PARTICLE->CheckAndStartCastScript( pcUnitTarget, iAdventMigalCastScriptID, psPacket->iSkillLevel1Based, pcUnitTarget->bAdventMigalLevel1Based, TRUE, FALSE, 0.0f, -5000 );
			PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iAdventMigalKeepScriptID, psPacket->sTimeLeftSec, psPacket->iSkillLevel1Based, pcUnitTarget->bAdventMigalLevel1Based );
		}
		break;
	}

	return TRUE;
}

/// <summary>
/// Called every 1s for all units in visble range
/// </summary>
BOOL CShamanSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_AdventMigal, ETimerID::AdventMigal, ESkillArrayPointer::Shaman_AdventMigal_Duration,		
			pStatus->uAdventMigalTimeLeft, pStatus->bAdventMigalLevel1Based );

		//auto instance = (DWORD *)0x03916BC4;

		//if ( pStatus->uAdventMigalTimeLeft)
		//{
		//	HoEffectType_MusPel * instance = (HoEffectType_MusPel *) CALL( 0x5CCA70 ); //  (DWORD *)0x03916BC4;
		//	if ( instance )
		//	{
		//		//instance->m = 0.0f;
		//
		//		CHATBOX->AddDebugMessage( "%.1f %.1f %d", instance->m_fEndTime, instance->m_fEndTime, instance->m_bLiveFlag );
		//
		//		//float * pEndTime = (float *)( &instance + 0x24 );
		//		//
		//		//instance[9] = 0; //set end time to 0
		//		//*pEndTime = 0;
		//
		//	}
		//
		//	//CALLT_WITH_ARG1( 0x005C9590, (DWORD)instance, MUSPEL_DEAD ); //kill off the old one if needed
		//	//CALLT( 0x005CAF20, (DWORD)instance );
		//}
		

		//CALLT_WITH_ARG1( 0x005C9590, (DWORD)&instance, 6 ); //kill off the old one if needed
	}

	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iAdventMigalKeepScriptID, pStatus->uAdventMigalTimeLeft, pStatus->bAdventMigalLevel1Based, pcUnitTarget->bAdventMigalLevel1Based );


	return TRUE;
}
