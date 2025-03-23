#include "stdafx.h"
#include "CFighterSkillHandler.h"
#include "SkillManager.h"


#define SKILL_RAGE_OF_ZECRAM_POWER1			44000
#define SKILL_RAGE_OF_ZECRAM_HIT1			44001
#define SKILL_RAGE_OF_ZECRAM_HIT2			44002

CFighterSkillHandler::CFighterSkillHandler()
{
}

CFighterSkillHandler::~CFighterSkillHandler()
{
}

void CFighterSkillHandler::LoadParticle ()
{
	iDemonsBaneScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3FighterDemonsBane_Cast.part" );
	iDemonsBaneScriptKeepID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3FighterDemonsBane_Keep.part" );
}

BOOL CFighterSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	UnitData * lpCurPlayer = UNITDATA;

	int iValue;

	switch ( psSkill->iID )
	{
		case SKILLID_WarCry:
		{
			if ( USESKILL( psSkill ) )
			{
				lpCurPlayer->sActiveSkill.cSkillTinyID = FighterSkill::WarCry;
				lpCurPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				lpCurPlayer->BeginSkill();
				lpCurPlayer->Animate( EAnimationType::ANIMATIONTYPE_Skill );

				lpCurPlayer->iMotionLoopSpeed = 80;
				lpCurPlayer->iMotionLoopCnt = 1;

				iValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_WarCry_Radius, psSkill->iLevel - 1 );

				SKILLMANAGERHANDLER->SetUnitsRange( lpCurPlayer->sPosition, iValue, FALSE );

				//Transmission after setting range attack
				SKILLMANAGERHANDLER->SendRangeDamage( lpCurPlayer, lpCurPlayer->sActiveSkill.iValue );

				//For party buff
				SKILLMANAGERHANDLER->SendProcessSkillToServer( FighterSkill::WarCry, psSkill->iLevel );
			}

			break;
		}

		//case SKILLID_ConcentrationSA:
		//{
		//	if ( USEMANASTAMINA( psSkill ) )
		//	{
		//		lpCurPlayer->sActiveSkill.cSkillTinyID = FighterSkill::SwiftAxe;
		//		lpCurPlayer->sActiveSkill.cLevel = psSkill->iLevel;
		//		lpCurPlayer->BeginSkill();
		//		lpCurPlayer->Animate( EAnimationType::ANIMATIONTYPE_Skill );
		//
		//		lpCurPlayer->iMotionLoopSpeed = 80;
		//		lpCurPlayer->iMotionLoopCnt = 1;
		//
		//		//For party buff
		//		SKILLMANAGERHANDLER->SendProcessSkillToServer( FighterSkill::SwiftAxe, psSkill->iLevel );
		//	}
		//}
		//break;


		case SKILLID_DemonsBane:
		{
			if ( USEMANASTAMINA( psSkill ) )
			{
				lpCurPlayer->sActiveSkill.cSkillTinyID = FighterSkill::DemonsBane;
				lpCurPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				lpCurPlayer->BeginSkill(); //calls AssaParticle_Concentration (0x5BBC90) and SkillPlaySound
				lpCurPlayer->Animate( EAnimationType::ANIMATIONTYPE_Skill );

				lpCurPlayer->iMotionLoopSpeed = 80;
				lpCurPlayer->iMotionLoopCnt = 1;

				//For party buff
				SKILLMANAGERHANDLER->SendProcessSkillToServer( FighterSkill::DemonsBane, psSkill->iLevel );
			}

			break;
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE; //return true to NOT process it in ASM
}

BOOL CFighterSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D )
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case FighterSkill::DemonsBane:
		{
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_CONCENTRATION2, pcThis );
			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}


BOOL CFighterSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt, level, value, tmp;

	level = pcUnitData->sActiveSkill.cLevel & 0xF;

	switch ( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case FighterSkill::Raving:
		{
			break;
		}

		case FighterSkill::DemonsBane:
		{
			break;
		}

		case FighterSkill::BoneCrash:
		{
			if ( pcUnitData->iTargetCritical >= 0)
			{
				CALL_WITH_ARG1( 0x5BC2B0, (DWORD)pcUnitData ); //0x5B2C60 = void AssaParticle_BoneSmash1(smCHAR* pChar)
				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_BONE_CRASH1, pcUnitData );
			}
			else
			{
				//Miss
				SKILLMANAGERHANDLER->PlayWeaponSound( pcUnitData );
			}

			if ( UNITDATA == pcUnitData && pcUnitData->iMotionEvent <= 2 && pcUnitData->pcTarget )
			{
				tmp = pcUnitData->sCharacterData.iAttackRating;

				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_DoubleImpact_AddAtkRtg, ESkillID::SKILLID_DoubleImpact );
				pcUnitData->sCharacterData.iAttackRating += ( pcUnitData->sCharacterData.iAttackRating * value ) / 100; //for % based

				cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );
				pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected

				pcUnitData->sCharacterData.iAttackRating = tmp; //restore
			}

			break;
		}

		case FighterSkill::BrutalSwing:
		{
			if ( pcUnitData->iTargetCritical >= 0 &&
				 pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ ))
			{
				SKILLMANAGERHANDLER->StartSkillEffect( Pos1, SKILLID_BrutalSwing );
				SKILLMANAGERHANDLER->SetDynLight( Pos1, 100, 0, 0, 0, 400 );
				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_SWING1, Pos1 );
			}
			else
			{
				//Miss
				SKILLMANAGERHANDLER->PlayWeaponSound( pcUnitData );
			}

			if ( UNITDATA == pcUnitData && pcUnitData->iMotionEvent == 1 && pcUnitData->pcTarget )
			{
				cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );
				pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected
			}

			break;
		}

		case FighterSkill::TripleImpact:
		{
			if ( pcUnitData->iTargetCritical >= 0 )
			{
				if ( pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ ) )
				{
					SKILLMANAGERHANDLER->StartSkillEffect( Pos1, SKILLID_TripleImpact );
					SKILLMANAGERHANDLER->SetDynLight( Pos1, 100, 0, 0, 0, 400 );
				}

				pcUnitData->iTargetCritical = 1;
			}

			SKILLMANAGERHANDLER->PlayWeaponSound( pcUnitData );

			cnt = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_TripleImpact_Numofhits, level - 1 );

			//for me only
			if ( pcUnitData == UNITDATA &&
				 pcUnitData->pcTarget &&
				 pcUnitData->iMotionEvent < cnt ) //motion event within count?
			{
				if ( pcUnitData->iTargetCritical >= 0 )
				{
					tmp = pcUnitData->sCharacterData.iAttackRating;

					value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_DoubleImpact_AddAtkRtg, ESkillID::SKILLID_DoubleImpact );
					pcUnitData->sCharacterData.iAttackRating += ( pcUnitData->sCharacterData.iAttackRating * value ) / 100; //for % based

					SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );

					pcUnitData->sCharacterData.iAttackRating = tmp; //restore
				}
			}

			break;
		}

		case FighterSkill::RageOfZecram:
		{
			if ( pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ ) &&
				 pcUnitData->iMotionEvent == 1 )
			{
				SKILLMANAGERHANDLER->StartSkillEffect( Pos1.iX, Pos1.iY, Pos1.iZ, 0, 0, 0, SKILL_RAGE_OF_ZECRAM_HIT1 );
				SKILLMANAGERHANDLER->AddTimeEffect( SKILL_RAGE_OF_ZECRAM_HIT2, TICKCOUNT + 600, Pos1.iX, Pos1.iY, Pos1.iZ );

				if ( pcUnitData == UNITDATA &&
					pcUnitData->pcTarget )
				{
					value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_RageofZecram_AttackRadius_New, level - 1 );

					SKILLMANAGERHANDLER->SetUnitsRange( Pos1, value, FALSE );
					SKILLMANAGERHANDLER->SendRangeDamage( pcUnitData, Pos1, 0, 0, 0, 0, pcUnitData->sActiveSkill.iValue );
					SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_RAGE_OF_ZECRAM1, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, FALSE );

					SetDynamicLight( Pos1.iX, Pos1.iY, Pos1.iZ, 100, 0, 0, 0, 400 );
				}
			}

			break;
		}

		case FighterSkill::AvengingCrash:
		{
			if ( pcUnitData->pcTarget )
			{
				if ( pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ ) )
				{
					if ( pcUnitData->iTargetCritical >= 0 )
					{
						CALL_WITH_ARG1( 0x5B2C60, (DWORD)pcUnitData ); //0x5B2C60 = void AssaParticle_AvangingCrash(smCHAR* pChar)
						SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_AVANGING_CRASH, pcUnitData );
					}
					else
					{
						//Miss Sound
						SOUNDHANDLER->SetWeaponSound( EWeaponSoundID::WEAPONSOUND_TwoHandMiss, pcUnitData );
					}
				}

				if ( UNITDATA == pcUnitData && pcUnitData->iMotionEvent <= 2 )
				{
					tmp = pcUnitData->sCharacterData.iAttackRating;
					value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_AvengingCrash_AddAtkRtg, ESkillID::SKILLID_AvengingCrash );

					pcUnitData->sCharacterData.iAttackRating += ( pcUnitData->sCharacterData.iAttackRating * value ) / 100;

					cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );

					pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected
					pcUnitData->sCharacterData.iAttackRating = tmp; //restore
				}
			}

			break;
		}
		case FighterSkill::Destroyer:
		{
			if ( pcUnitData->pcTarget )
			{
				if ( pcUnitData->iTargetCritical >= 0 &&
					 pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ ))
				{
					SKILLMANAGERHANDLER->SetDynLight( Pos1, 100, 0, 0, 0, 400 );

					CALL_WITH_ARG1( 0x5D3D90, (DWORD) &Pos1 ); //0x5D3D90 = SkillWarriorDestroyerHit

					if ( pcUnitData->iMotionEvent <= 3 )
						SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_AVANGING_CRASH, pcUnitData );
					else
						SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_EXPANSION2, pcUnitData );
				}
				else
				{
					//Miss Sound
					SOUNDHANDLER->SetWeaponSound(EWeaponSoundID::WEAPONSOUND_TwoHandMiss, pcUnitData );
				}

				if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent <= 3 )
				{
					tmp = pcUnitData->sCharacterData.iAttackRating;
					value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_AvengingCrash_AddAtkRtg, ESkillID::SKILLID_AvengingCrash );

					pcUnitData->sCharacterData.iAttackRating += ( pcUnitData->sCharacterData.iAttackRating * value ) / 100;

					cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );

					pcUnitData->iTargetCritical = cnt == FALSE ? -1 : 0;				//-1 = deflected
					pcUnitData->sCharacterData.iAttackRating = tmp; //restore
				}
			}
			break;
		}

		default:
		{
			return FALSE; //Process in assembly instead
		}
	}

	return TRUE; //skip processing in assembly
}

BOOL CFighterSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	UnitData * pcPlayer = UNITDATA;
	int value, cnt;

	if ( psSkill->iLevel <= 0 || psSkill->iLevel > 10 )
		return TRUE; //return TRUE to not execute code in assembly

	switch ( psSkill->iID )
	{
		case SKILLID_Raving:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Attack );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::Raving;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );

				pcPlayer->iMotionLoopCnt = SKILLMANAGERHANDLER->GetSkillIntValue( Fighter_Raving_Numofhits, ESkillID::SKILLID_Raving );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed, 1 );
				pcPlayer->iMotionLoopSpeed += ( 20 * pcPlayer->iMotionLoopCnt );

				pcPlayer->iFireOverlay = 150;

				cnt = SKILLMANAGERHANDLER->SendTransDamage( pcPlayer->pcTarget, pcPlayer->sActiveSkill );

				if ( !cnt ) pcPlayer->iTargetCritical = -1;
				else
				{
					//Life decrease
					pcPlayer->iTargetCritical = 0;

					value = SKILLMANAGERHANDLER->GetSkillIntValue( Fighter_Raving_HPCost_New, ESkillID::SKILLID_Raving );
					CHARACTERGAME->RemoveHPValue( value );
				}
			}
			break;
		}

		case SKILLID_BoneCrash:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::BoneCrash;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed );
				pcPlayer->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_RageofZecram:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::RageOfZecram;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed );
				pcPlayer->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_CycloneStrike:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::CycloneStrike;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed );
				pcPlayer->iMotionLoopCnt = 1;

				value = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_CycloneStrike_Range, ESkillID::SKILLID_CycloneStrike );
				SKILLMANAGERHANDLER->SetUnitsRange ( UNITDATA->sPosition, value, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage ( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, pcPlayer->sActiveSkill.iValue );
			}

			break;
		}

		case SKILLID_BrutalSwing:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::BrutalSwing;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_BrutalSwing_HPCost_New, ESkillID::SKILLID_BrutalSwing );
				CHARACTERGAME->RemoveHPValue( value );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed, 2 ); //+2 to make it faster, it's slow already
				pcPlayer->iMotionLoopCnt = 1;
			}

			break;
		}

		case SKILLID_AvengingCrash:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::AvengingCrash;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_AvengingCrash_HPCost_New, ESkillID::SKILLID_AvengingCrash );
				CHARACTERGAME->RemoveHPValue( value );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed, 2 ); //+2 to make it faster
				pcPlayer->iMotionLoopCnt = 1;
			}
			break;
		}

		case SKILLID_Destroyer:
		{
			if ( USESKILL( psSkill ) )
			{
				pcPlayer->sActiveSkill.cSkillTinyID = FighterSkill::Destroyer;
				pcPlayer->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayer->BeginSkill( pcMouseTarget );
				pcPlayer->Animate( ANIMATIONTYPE_Skill );

				//reduce % of HP after execution of skill
				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Fighter_Destroyer_HPCost_New, ESkillID::SKILLID_Destroyer );
				CHARACTERGAME->RemoveHPValue( value );

				pcPlayer->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayer->sCharacterData.iAttackSpeed );
				pcPlayer->iMotionLoopCnt = 1;
			}
			break;
		}

		case SKILLID_SeismicImpact:
		{
			return SESMICIMPACTHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}

		default:
		{
			return FALSE; //not handled here. handle in assembly instead
		}
	}

	return TRUE; //disable processing in assembly
}

BOOL CFighterSkillHandler::OnEndSkill( UnitData * pcThis )
{
	switch ( pcThis->sActiveSkill.cSkillTinyID )
	{
		case  FighterSkill::BrutalSwing :
			return TRUE;		 //prevent assembly from calling
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
BOOL CFighterSkillHandler::OnSkillBuff(Unit * pcUnit, PacketUseSkillBase * psPacket )
{
	int duration;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_DemonsBane:
		{
			//others only, since the AssaParticle_Concentration will be called for the caster
			//via BeginSkill
			//if ( pcUnit->GetID() != psPacket->lCasterID )
			//{
			//	//0x5BBC90 = AssaParticle_Concentration();
			//	//CALL_WITH_ARG2( 0x5BBC90, (DWORD)pcUnit->pcUnitData, psPacket->sTimeLeftSec );
			//}

			PARTICLE->CheckAndStartCastScript( pcUnit, iDemonsBaneScriptID, psPacket->iSkillLevel1Based, pcUnit->bDemonsBaneLevel1Based );

			break;
		}

		case ESkillID::SKILLID_WarCry:
		{
			if ( pcUnit == UNIT )
			{
				int hpValue = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Fighter_WarCry_HPRestore_New, psPacket->iSkillLevel1Based - 1);
				int spValue = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Fighter_WarCry_SPRestore_New, psPacket->iSkillLevel1Based - 1);

				if ( psPacket->lCasterID != PLAYERDATA->iID )
				{
					hpValue >>= 1;
					spValue >>= 1;
				}

				CHARACTERGAME->GiveHPValuePercent(hpValue);
				CHARACTERGAME->GiveSPValuePercent(spValue);
			}

			break;
		}
	}

	return FALSE;
}


BOOL CFighterSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_DemonsBane, ETimerID::DemonsBane, ESkillArrayPointer::Fighter_DemonBuff_Duration,
			pStatus->uDemonBaneTimeLeft, pStatus->bDemonBaneLevel1Based );
	}

	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iDemonsBaneScriptKeepID, pStatus->uDemonBaneTimeLeft, pStatus->bDemonBaneLevel1Based, pcUnitTarget->bDemonsBaneLevel1Based );

	return TRUE;
}