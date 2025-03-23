#include "stdafx.h"
#include "CMagicianSkillHandler.h"
#include "SkillManager.h"

CMagicianSkillHandler::CMagicianSkillHandler()
{
}

CMagicianSkillHandler::~CMagicianSkillHandler()
{
}

void CMagicianSkillHandler::LoadParticle()
{
	//iVirtualLifeCastScriptID = PARTICLE->Load( "Effect\\Particle\\Script\\Skill3PriestessVirtualLifeMember_Cast.part" );
	//iVirtualLifeKeepScriptID = PARTICLE->Load( "Effect\\Particle\\Script\\Skill3PriestessVirtualLifeMember_Keep.part" );
}

BOOL CMagicianSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int value;

	switch ( psSkill->iID )
	{
		case SKILLID_FlameWave:
		{
			if ( UNITDATA->IsLowAnimationType() &&
				USESKILL( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::FlameWave;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill();

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
			}

			break;
		}

		//here we do 'right click'. For this case
		//we enchant everyone nearby, refreshing their duration. But
		//requires left click enchantment to change.
		case SKILLID_EnchantWeapon:
		{
			//Unit is not performing an action and can use skill?
			if ( UNITDATA->IsLowAnimationType() &&
				 USESKILL( psSkill ) )
			{
				UNITDATA->pcTarget = NULL;

				//enchantment type is applied

				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::EnchantWeapon;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				//UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 ); //don't play as this would cast default elemental on self
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

			}

			break;
		}

		case SKILLID_DancingSword:
		{
			if ( UNITDATA->IsLowAnimationType() &&
				UNITDATA_HOVER == NULL &&
				//UNITDATA->iEnchantWeaponType == 0 &&
				USESKILL( psSkill ) )
			{
				value = Dice::RandomI( 1, 2 );

				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::DancingSword;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = value; //for server (other players doesn't get this value.....!)

				UNITDATA->BeginSkill( UNITDATA->sActiveSkill.iValue, 0, 0, 0, 0, 0 );

				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendProcessSkillToServer( MagicianSkill::DancingSword, psSkill->iLevel, value, 0 );
			}

			break;
		}

		case SKILLID_Diastrophism:
		{
			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL )
			{
				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_Diastrophism_Area, ESkillID::SKILLID_Diastrophism );

				if ( SETTINGSHANDLER->GetModel()->GetSettings().bAutoSelectNearestTargetForAoE )
				{
					UNITDATA_HOVER = SKILLMANAGERHANDLER->FindNearestMonster( UNITDATA, value );
				}
				else
				{
					if ( UNITDATA->pcTarget == NULL )
					{
						UNITDATA->pcTarget = SKILLMANAGERHANDLER->FindNearestMonster( UNITDATA, value );
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
			//FALSE so that Dia will be casted once (via assembly)
			if ( UNITDATA_HOVER == NULL )
			{
				return FALSE;
			}

			return 2; //return FALSE but also bypass ASM
		}
		case SKILLID_StoneSkin:
		{
			return STONESKINHANDLER->OnCast ( psSkill, UNITDATA );
		}
		case SKILLID_WizardTrance:
		{
			return WIZARDTRANCEHANDLER->OnCastAnimationStart ( psSkill, UNITDATA );
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE; //don't handle it in ASM
}


/// <summary>
/// Attempt to attack skill
/// Leaked func: int PlaySkillAttack( sSKILL *lpSkill , smCHAR *lpChar )
/// </summary>
BOOL CMagicianSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	switch ( psSkill->iID )
	{
		case SKILLID_FlameWave:
		{
			if ( USESKILL( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::FlameWave;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->BeginSkill();
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
			}

			break;
		}

		case SKILLID_EnchantWeapon:
		{
			//disabled
			//if ( pcMouseTarget->PartyFlag && //is in party
			//	 pcMouseTarget->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player && //is a player
			//	 USESKILL( psSkill ) )
			//{
			//	UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::EnchantWeapon;
			//	UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
			//	UNITDATA->sActiveSkill.sParam = Dice::RandomI( 0, 2 );
			//
			//	SKILLMANAGERHANDLER->BeginSkill( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, pcMouseTarget, 0, 0, 0 );
			//	UNITDATA->Animate( ANIMATIONTYPE_Skill );
			//	UNITDATA->iMotionLoopSpeed = 90;
			//	UNITDATA->iMotionLoopCnt = 1;
			//
			//	return TRUE;
			//}
			//
			//UNITDATA->pcTarget = NULL;
			break;
		}

		case SKILLID_Watornado:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::Watornado;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, pcMouseTarget, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = 90;
				UNITDATA->iMotionLoopCnt = 1;

				int iRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Magician_Watornado_Area, psSkill->iLevel - 1 );

				SKILLMANAGERHANDLER->SetUnitsRange ( pcMouseTarget->sPosition, iRange, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage ( pcMouseTarget, pcMouseTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}

			//if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
			break;
		}


		case SKILLID_Diastrophism:
		{
			if ( USESKILL ( psSkill ) )
			{
				UNITDATA->sActiveSkill.cSkillTinyID = MagicianSkill::Diastrophism;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate ( ANIMATIONTYPE_Skill );

				UNITDATA->iMotionLoopSpeed = 120;
				UNITDATA->iMotionLoopCnt = 1;

				//int iRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Magician_Watornado_Area, psSkill->iLevel - 1 );

				//SKILLMANAGERHANDLER->SetUnitsRange ( pcMouseTarget->sPosition, iRange, FALSE );
				//SKILLMANAGERHANDLER->SendRangeDamage ( pcMouseTarget, pcMouseTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}
			break;
		}

		case SKILLID_StoneSkin:		break;
		case SKILLID_WizardTrance:	break;
		case SKILLID_RedRay:
		{
			return REDRAYHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}
		default:
		{
			return FALSE; //return false to process the skill in ASM instead.
		}
	}

	return TRUE;
}



BOOL CMagicianSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D )
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case MagicianSkill::EnchantWeapon:
		{
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_CASTING_MAGICIAN, pcThis );		//magic sound

			//0x531E00 = int sinEffect_StartMagic(POINT3D *pPosi , int CharFlag , int Type)
			CALL_WITH_ARG2( 0x531E00, (DWORD)&pcThis->sPosition, 2 );

			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_ENCHANTWEAPON_CAST, pcThis );

			break;
		}

		default:
		{
			return FALSE; //not handled
		}
	}

	return TRUE; //handled
}

BOOL CMagicianSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt, value, level;

	level = ( psActiveSkill->cLevel & 0xF );

	switch( pcUnitData->sActiveSkill.cSkillTinyID )
	{
		case MagicianSkill::EnchantWeapon:
		{
			//Send buff request when the main casting animation sequence is finished
			//only for self
			if ( pcUnitData == UNITDATA )
			{
				SKILLMANAGERHANDLER->SendProcessSkillToServer( MagicianSkill::EnchantWeapon, level, 0, 0 );
			}

			break;
		}

		case MagicianSkill::DancingSword:
		{
			value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_DancingSword_Duration, level - 1 );

			if ( pcUnitData->sActiveSkill.sParam == 1 )
			{
				//0x5BDB10 = AssaParticle_DancingSwordFire
				CALL_WITH_ARG3( 0x005BDB10, (DWORD)pcUnitData, (DWORD)value, (DWORD) level );
			}
			else if ( pcUnitData->sActiveSkill.sParam == 2 )
			{
				//0x5BDBC0 = AssaParticle_DancingSwordIce
				CALL_WITH_ARG3( 0x005BDBC0, (DWORD)pcUnitData, (DWORD)value, (DWORD) level );
			}

			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD, pcUnitData );


			break;
		}

		case MagicianSkill::FlameWave:
		{
			CALL_WITH_ARG1( 0x5BEF90, (DWORD)pcUnitData ); //0x5BEF90 = SkillArchMageFlameWave

			SKILLMANAGERHANDLER->SkillPlaySound( Dice::RandomI(0, 1) ? SKILL_SOUND_SKILL_FLAME_WAVE1 : SKILL_SOUND_SKILL_FLAME_WAVE2, pcUnitData );

			if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent == 1 )
			{
				//@todo make it triangle based
				rect.left = -70;
				rect.right = 70;
				rect.top = 0;
				rect.bottom = 340;

				//Range-type attack range selection (box)
				SKILLMANAGERHANDLER->SelectRangeBox( pcUnitData, rect, FALSE );

				//Transmission after setting range attack
				SKILLMANAGERHANDLER->SendRangeDamage(pcUnitData, pcUnitData->sPosition, 0, 0, 0, 0, psActiveSkill->iValue );
			}
			break;
		}

		case MagicianSkill::Meteorite:
		{
			if ( pcUnitData->pcTarget)
			{
				Pos2.iX = pcUnitData->pcTarget->sPosition.iX;
				Pos2.iY = pcUnitData->pcTarget->sPosition.iY;
				Pos2.iZ = pcUnitData->pcTarget->sPosition.iZ;

				CALL_WITH_ARG2( 0x5D2E40, (DWORD)&Pos2, 0 ); //0x5D2E40 = SkillArchMageMeteo()

				//only process the first two motion events. the last one is via EndSkill
				if ( pcUnitData == UNITDATA && pcUnitData->iMotionEvent < 3 )
				{
					value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_Meteorite_Area, level - 1 );

					//Range-type attack range selection (won)
					SKILLMANAGERHANDLER->SetUnitsRange( Pos2, value , FALSE );

					//Send after setting range attack

					psActiveSkill->sParam = pcUnitData->iMotionEvent;

					SKILLMANAGERHANDLER->SendRangeDamage(pcUnitData->pcTarget, Pos2, 0, 0, 0, 0, psActiveSkill->iValue );
				}

				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_METEO1, pcUnitData->pcTarget );
			}


			break;
		}

		case SKILLPLAYID_T51:
			return WIZARDTRANCEHANDLER->OnEvent( pcUnitData );
		break;
		case SKILLPLAYID_T52:
			return REDRAYHANDLER->OnEvent( pcUnitData );
		break;
		case SKILLPLAYID_T53:
			return TRUE;
		break;
		case SKILLPLAYID_T54:
			break;
		default:
		{
			return FALSE; //Process in ASM instead
		}
	}

	return TRUE; //don't process in ASM
}


BOOL CMagicianSkillHandler::OnEndSkill ( class UnitData * pcUnitData )
{
	switch ( UNITDATA->sActiveSkill.cSkillTinyID )
	{
		/*case MagicianSkill::Diastrophism:
		{
			return TRUE; //bypass ASM
		}*/
		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// Perhaps have a look at
/// int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill ) in the 2012 code
/// how to make this better, i.e. player coming in range when previously not in range
/// </summary>
BOOL CMagicianSkillHandler::OnSkillBuff( Unit * pcUnit, PacketUseSkillBase * psPacket )
{
	switch ( psPacket->iSkillID )
	{
		case SKILLID_EnchantWeapon: return MAGICIANSKILLHANDLER->OnEnchantWeaponCastEffect( pcUnit, psPacket->iSkillLevel1Based, psPacket->iParameter, psPacket->lCasterID, psPacket->sTimeLeftSec ); break;

		//case SKILLID_WizardTrance:
		//	WIZARDTRANCEHANDLER->OnCastEffect( pcUnitTarget, psPacket->iSkillLevel1Based, 300.0f );
		//	break;
	}

	return FALSE;
}


/// <summary>
/// This function is only called on mage's initial enchant weapon cast.
/// Subsequent updates will be handled via assembly's player buffer data
/// which gets sent to other players from time to time
/// </summary>
BOOL CMagicianSkillHandler::OnEnchantWeaponCastEffect( Unit * pcUnit, int iSkillLevel1Based, int iParameter, ID idCaster, int iOverTime )
{
	if ( iParameter == 0 || iSkillLevel1Based == 0 || iOverTime == 0  )
		return FALSE;

	//only apply 'cast' effect if there is a caster id
	//see enchant weapon logic for rotation on server side.

	if ( idCaster != 0 )
	{
		CALL_WITH_ARG1( 0x5B8C50, (DWORD)pcUnit->pcUnitData ); //0x5B8C50 = AssaParticle_EnchantWeaponJinMember

	}

	CHATBOX->AddDebugMessage( "%d %d %d", iSkillLevel1Based, iOverTime, iParameter );

	//After setting these on this player will in turn add these data
	//to the existing player buffer data which will
	//get sent to other clients to keep them in sync / updated
	pcUnit->pcUnitData->iEnchantWeaponLevel = iSkillLevel1Based;
	pcUnit->pcUnitData->dwEnchantWeaponTimeOut = TICKCOUNT + (iOverTime * 1000);
	pcUnit->pcUnitData->iEnchantWeaponType = iParameter + 1; //see 2012 code

	switch ( (EElementID)iParameter )
	{
		case EElementID::Ice:		CALL_WITH_ARG2( 0x5B8CE0, (DWORD)pcUnit->pcUnitData, iOverTime );  break; //0x5B8CE0 = AssaParticle_EnchantWeaponIceMember
		case EElementID::Fire:		CALL_WITH_ARG2( 0x5B8D80, (DWORD)pcUnit->pcUnitData, iOverTime );  break; //0x5B8D80 = AssaParticle_EnchantWeaponFireMember
		case EElementID::Lightning:	CALL_WITH_ARG2( 0x5B8E20, (DWORD)pcUnit->pcUnitData, iOverTime );  break; //0x5B8E20 = AssaParticle_EnchantWeaponLightMember
	}

	int skillSound = 0;
	switch ( (EElementID)iParameter )
	{
		case EElementID::Ice:		skillSound = Dice::RandomI( 0, 1 ) ? SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE1 : SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE2; break;
		case EElementID::Fire:		skillSound = Dice::RandomI( 0, 1 ) ? SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE1 : SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE2; break;
		case EElementID::Lightning:	skillSound = Dice::RandomI( 0, 1 ) ? SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING1 : SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING2; break;
	}

	if ( skillSound != 0 )
	{
		SKILLMANAGERHANDLER->SkillPlaySound( skillSound, pcUnit->pcUnitData );
	}

	if ( pcUnit == UNIT )
	{
		//TIMERSKILLHANDLER->AddSkillTimer(ESkillID::SKILLID_EnchantWeapon, iOverTime, iSkillLevel1Based );

		//0x4ECEE0 = SearchSkillDoc(..)
		//CALL_WITH_ARG1( 0x4ECEE0, (DWORD)&sSkill);

		ITEMHANDLER->UpdateItemCharacterStatus( );
	}


	//add skill timer
	//CALL_WITH_ARG2( 0x5047E0, (DWORD)iOverTime, (DWORD)iSkillLevel1Based ); //0x5047E0 = int cSKILL::SetEnchant_Weapon(int Time , int Point)

	return TRUE;
}

BOOL CMagicianSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_EnchantWeapon, ETimerID::EnchantWeapon, ESkillArrayPointer::Magician_EnchantWeapon_Duration,
			pStatus->uEnchantTimeLeft, pStatus->bEnchantLevel1Based );
	}


	//is me? add timer
	//if ( pcUnitTarget == UNIT )
	//{
	//	TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_Alas, ETimerID::Alas, ESkillArrayPointer::Assassin_Alas_Duration,
	//		pStatus->uAlasTimeLeft, pStatus->bAlasLevel1Based );
	//}
	//
	//PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iAlasScriptKeepID, pStatus->uAlasTimeLeft, pStatus->bAlasLevel1Based, pcUnitTarget->bAlasLevel1Based );

	//virtu

	return TRUE;
}