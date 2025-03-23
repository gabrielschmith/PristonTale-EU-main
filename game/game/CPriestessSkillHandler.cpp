#include "stdafx.h"
#include "CPriestessSkillHandler.h"
#include "SkillManager.h"

CPriestessSkillHandler::CPriestessSkillHandler()
{
}

CPriestessSkillHandler::~CPriestessSkillHandler()
{
}

void CPriestessSkillHandler::LoadParticle ()
{
	iVirtualLifeCastScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3PriestessVirtualLifeMember_Cast.part" );
	iVirtualLifeKeepScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3PriestessVirtualLifeMember_Keep.part" );

	iVirtualRegenCastScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3PriestessVirtualRegenMember_Cast.part" );
	iVirtualRegenKeepScriptID = PARTICLE->Load ( "Effect\\Particle\\Script\\Skill3PriestessVirtualRegenMember_Keep.part" );
}

BOOL CPriestessSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
    {

		case SKILLID_Healing:
		{
			/// HardCPP : This code below transform a client sided skill to server handled skill in case of Self Healing
			/// Healing other players is still handled by default ASM code and forwarded to server
			/// Source : SkillSub.cpp => case SKILL_HEALING: => LINE 2857 && ASM GAME.EXE:0x4141CC (other) GAME.EXE:0x41572B (self)

			if ( UNITDATA->psModelAnimation->iType < ANIMATIONTYPE_Attack )
			{
				UnitData* pcTarget = UNITDATA;

				if ( UNITDATA_HOVER && UNITDATA_HOVER->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player )
				{
					const int iSkillRange = SKILLMANAGERHANDLER->GetSkillRange(psSkill);

					//is outside of skill range? if so, return 2 so that
					//asm will move the player closer to the target and the healing
					//will be done via OnPlaySkillAttack
					if ( UNITDATA->sPosition.GetPTDistance(UNITDATA_HOVER->sPosition) > iSkillRange )
					{
						return 2;
					}

					pcTarget = UNITDATA_HOVER;
				}

				if (USESKILL( psSkill ))
				{
					UNITDATA->sActiveSkill.iValue		= 0;	///< reset whole field
					UNITDATA->sActiveSkill.cSkillTinyID = Healing;
					UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

					UNITDATA->BeginSkill(UNITDATA->sActiveSkill.iValue, 0, pcTarget, 0, 0, 0);

					UNITDATA->Animate(EAnimationType::ANIMATIONTYPE_Skill);
					UNITDATA->iMotionLoopSpeed = 70;
					UNITDATA->iMotionLoopCnt = 1;

                    auto l_WeaponDamage1 = reinterpret_cast<short*>(0x35EB974)[788 * *(DWORD*)0x82D504];
                    auto l_WeaponDamage2 = reinterpret_cast<short*>(0x35EB976)[788 * *(DWORD*)0x82D504];

					SKILLMANAGERHANDLER->SendTransDamageSock(pcTarget, l_WeaponDamage1, l_WeaponDamage2, 0, 0, UNITDATA->sActiveSkill.iValue, false, SOCKETG);

                   // dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, lpCurPlayer->AttackSkil, FALSE);
				}
			}
            return 1; //return true to NOT handle it in ASM
		}

		case SKILLID_DivineLightning:
		{
			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL)
			{
				int iValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Priestess_DivineLightning_ATKRadius_New, ESkillID::SKILLID_DivineLightning );

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

			/*if ( UNITDATA_HOVER )
			{
				MOUSE_TARGET_TRACE_MODE = TRUE;
				AUTO_ATTACK_MODE = TRUE;
				CAN_ATTACK = TRUE;

				SELECTED_MOUSE_BUTTON = 2; //2 = Right
				AUTO_CAMERA_FLAG = FALSE;
				UNITDATA->bWalking = 0; //Not moving
				DISP_EACH_MODE = 0;
				AUTO_MOUSE_WM_COUNT += 1;
			}*/

			return 2; //return TRUE but also bypass ASM to allow these skills to be held down. See BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
		}

		case SKILLID_IceElemental:
		{
			return ICEELEMENTALHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
		}

		case SKILLID_VirtualRegen:
		{
			if ( USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				/// Recycle VirtualLife animation
				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::VirtualLife; //use virtual life animation
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::VirtualRegen;
				UNITDATA->BeginSkill();

				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}

			break;
		}

		case SKILLID_VirtualLife:
		{
			if ( USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				/// Recycle VirtualLife animation
				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::VirtualLife; //use virtual life animation
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill();
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}

			break;
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}

	}

	return TRUE; //return true to NOT handle it in ASM
}


BOOL CPriestessSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	switch ( psSkill->iID )
	{
		case SKILLID_VirtualLife:
		{
			//not handled here
			break;
		}

		case SKILLID_VirtualRegen:
		{
			//not handled here
			break;
		}

		case SKILLID_Healing:
		{
			if ( USESKILL(psSkill) )
			{
				UNITDATA->sActiveSkill.iValue = 0;
				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::Healing;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill(UNITDATA->sActiveSkill.iValue, 0, UNITDATA->pcTarget, 0, 0, 0);

				UNITDATA->Animate(EAnimationType::ANIMATIONTYPE_Skill);
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				auto l_WeaponDamage1 = reinterpret_cast<short*>( 0x35EB974 )[788 * *(DWORD*)0x82D504];
				auto l_WeaponDamage2 = reinterpret_cast<short*>( 0x35EB976 )[788 * *(DWORD*)0x82D504];

				SKILLMANAGERHANDLER->SendTransDamageSock(UNITDATA->pcTarget, l_WeaponDamage1, l_WeaponDamage2, 0, 0, UNITDATA->sActiveSkill.iValue, false, SOCKETG);
				// dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, lpCurPlayer->AttackSkil, FALSE);
			}
			break;
		}

		case SKILLID_IceElemental:
		{
			return ICEELEMENTALHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget ); break;
		}

		case SKILLID_MultiSpark:
		{
			if ( USESKILL(psSkill) )
			{
				int iNumOfSparkMin = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Priestess_MultiSpark_NumSparksMin_New, psSkill->iLevel - 1 );
				int iNumOfSparkMax = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Priestess_MultiSpark_NumSparksMax, psSkill->iLevel - 1 );

				int iNumOfSpark = Dice::RandomI ( iNumOfSparkMin, iNumOfSparkMax );

				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::MultiSpark;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = iNumOfSpark;

				SKILLMANAGERHANDLER->BeginSkill( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed ( UNITDATA->sCharacterData.iAttackSpeed );
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendTransDamage( UNITDATA->pcTarget, UNITDATA->sActiveSkill );
			}
			break;
		}

		case SKILLID_GrandHealing:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::GrandHealing;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				if ( READDWORD ( 0x9E23BC ) ) //AreaServerMode - no idea what this is. Only used once for all skills
				{
					SKILLMANAGERHANDLER->SendTransDamageSock ( UNITDATA, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue, TRUE, SOCKETG );
				}
				else
				{
					SKILLMANAGERHANDLER->SendTransDamage ( UNITDATA, UNITDATA->sActiveSkill );
				}
			}
			break;
		}

		case SKILLID_DivineLightning:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = PriestessSkill::DivineLightning;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = 85;
				UNITDATA->iMotionLoopCnt = 1;

				//int iRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Magician_Watornado_Area, psSkill->iLevel - 1 );

				//SKILLMANAGERHANDLER->SetUnitsRange ( pcMouseTarget->sPosition, iRange, FALSE );
				//SKILLMANAGERHANDLER->SendRangeDamage ( pcMouseTarget, pcMouseTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}
			break;
		}
		default:
		{
			return FALSE; //return false to get it processed in ASM
		}
	}


	return TRUE; //dont' process in ASM
}

BOOL CPriestessSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D )
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case PriestessSkill::VirtualLife:
		{
			//0x531E00 = int sinEffect_StartMagic(POINT3D *pPosi , int CharFlag , int Type)
			//CALL_WITH_ARG2( 0x531E00, (DWORD)&point3D, 1 );
			break;
		}

		case PriestessSkill::VirtualRegen:
		{
			//0x531E00 = int sinEffect_StartMagic(POINT3D *pPosi , int CharFlag , int Type)
			//CALL_WITH_ARG2( 0x531E00, (DWORD)&point3D, 1 );

			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}



BOOL CPriestessSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;

	switch ( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case PriestessSkill::VirtualLife:
		{
			//AssaParticle_VirtualLife()
			//CALL_WITH_ARG2( 0x5D4660, (DWORD)pcUnitData, psPacket->sTimeLeftSec );

			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_VIRTUAL_LIFE, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
			break;
		}

		case PriestessSkill::VirtualRegen:
		{
			//AssaParticle_VirtualLife()
			//CALL_WITH_ARG2( 0x5D4660, (DWORD)pcUnitData, psPacket->sTimeLeftSec );

			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_REGEN_FIELD2, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
			break;
		}

		case PriestessSkill::MultiSpark:
		{
			if ( pcUnitData )
			{
				//0x52F480 = int sinEffect_MultiSpark(smCHAR *pChar , smCHAR *desChar , int Num)
				CALL_WITH_ARG3( (DWORD)0x52F480, (DWORD)pcUnitData, (DWORD)pcUnitData->pcTarget, (DWORD)pcUnitData->sActiveSkill.sParam );
				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_MULTISPARK, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
			}

			break;
		}

		//case PriestessSkill::DivineLightning:
		//{
		//	if ( pcMouseTarget == UNITDATA )
		//	{
		//
		//	}
		//}

		//Grand Healing
		case PriestessSkill::GrandHealing:
		{
			if ( pcUnitData == UNITDATA )
			{
				for ( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
				{
					UnitData * p = UNITGAME->GetUnitDataByID ( PARTYHANDLER->GetPartyData ().saMembers[i].sData.iID );
					if ( p )
					{
						if ( p->bActive && p->psModelAnimation->iType != ANIMATIONTYPE_Die )
						{
							int iX = ( UNITDATA->sPosition.iX - p->sPosition.iX ) >> 8;
							int iY = ( UNITDATA->sPosition.iY - p->sPosition.iY ) >> 8;
							int iZ = ( UNITDATA->sPosition.iZ - p->sPosition.iZ ) >> 8;
							int iDistance = iX * iX + iY * iY + iZ * iZ;

							if ( iDistance < ( 300 * 300 ) )
							{
								CALL_WITH_ARG1 ( 0x0053BE20, (DWORD)p );
							}
						}
					}
				}
			}
			break;
		}

		case PriestessSkill::Resurrection:
		{
			if ( pcUnitData == UNITDATA )
				return RESSURECTIONHANDLER->OnUseSkill ( pcUnitData );
			break;


			//return TRUE;
			//if (pcMouseTarget == UNITDATA)
			//	return VIRTUALMANAHANDLER->OnCastAnimationStart(pcMouseTarget->sActiveSkill, pcMouseTarget);
			//break;
		}

		default:
		{
			return FALSE; //return false to get it processed in ASM
		}

		break;
	}

	return TRUE; //return true to avoid it getting processed in ASM
}

/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// Perhaps have a look at
/// int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill ) in the 2012 code
/// how to make this better, i.e. player coming in range when previously not in range
/// </summary>
BOOL CPriestessSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	switch ( psPacket->iSkillID )
	{
		case ESkillID::SKILLID_VirtualLife:
		{
			PARTICLE->CheckAndStartCastScript( pcUnitTarget, iVirtualLifeCastScriptID, psPacket->iSkillLevel1Based, pcUnitTarget->bVirtualLifeLevel1Based );
			break;
		}
		case ESkillID::SKILLID_VirtualRegen:
		{
			PARTICLE->CheckAndStartCastScript( pcUnitTarget, iVirtualRegenCastScriptID, psPacket->iSkillLevel1Based, pcUnitTarget->bVirtualRegenLevel1Based );
			break;
		}
	}

	return TRUE;
}

BOOL CPriestessSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_VirtualLife, ETimerID::VirtualLife, ESkillArrayPointer::Priestess_VirtualLife_Duration,
			pStatus->uVirtualLifeTimeLeft, pStatus->bVirtualLifeLevel1Based );

		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_VirtualRegen, ETimerID::VirtualRegen, ESkillArrayPointer::Priestess_VirtualRegen_Duration_New,
			pStatus->uVirtualRegenTimeLeft, pStatus->bVirtualRegenLevel1Based);
	}

	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iVirtualLifeKeepScriptID, pStatus->uVirtualLifeTimeLeft, pStatus->bVirtualLifeLevel1Based, pcUnitTarget->bVirtualLifeLevel1Based );
	PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iVirtualRegenKeepScriptID, pStatus->uVirtualRegenTimeLeft, pStatus->bVirtualRegenLevel1Based, pcUnitTarget->bVirtualRegenLevel1Based );


	return TRUE;
}
