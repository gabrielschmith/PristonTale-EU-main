#include "stdafx.h"
#include "CPikemanSkillHandler.h"
#include "SkillManager.h"

//Shadow Master Multi Strike Point
int	SkillShadowMasterEvent[9][7] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },

	{ 1, 3, 5, 0, 0, 0, 0 },		//3 hits
	{ 1, 3, 5, 6, 0, 0, 0 },
	{ 1, 2, 3, 4, 6, 0, 0 },
	{ 1, 2, 3, 4, 6, 7, 0 },
	{ 1, 2, 3, 4, 5, 6, 7 },		//7 strikes

	{ 1, 2, 3, 4, 5, 6, 7 }
};

CPikemanSkillHandler::CPikemanSkillHandler()
{}

CPikemanSkillHandler::~CPikemanSkillHandler()
{}

BOOL CPikemanSkillHandler::OnOpenPlaySkill( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{
		case ESkillID::SKILLID_Tornado:
		{
			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL )
			{
				int iValue = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Pikeman_Tornado_Range, ESkillID::SKILLID_Tornado);

				if ( SETTINGSHANDLER->GetModel()->GetSettings().bAutoSelectNearestTargetForAoE )
				{
					UNITDATA_HOVER = SKILLMANAGERHANDLER->FindNearestMonster(UNITDATA, iValue);
				}
				else
				{
					if ( UNITDATA->pcTarget == NULL )
					{
						UNITDATA->pcTarget = SKILLMANAGERHANDLER->FindNearestMonster(UNITDATA, iValue);
					}

					if ( UNITDATA->pcTarget )
					{
						CANATTACK = TRUE;
						SELECTED_MOUSE_BUTTON = 2;
						AUTOMOUSE_WM_COUNT++;

						OnPlaySkillAttack(psSkill, UNITDATA->pcTarget);

						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}

			//No nearby target still. Return
			//FALSE so that the skill will be casted once (via assembly)
			if ( UNITDATA_HOVER == NULL )
			{
				return FALSE;
			}

			return 2; //return TRUE but also bypass ASM to allow these skills to be held down. See BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
		}

		case ESkillID::SKILLID_GroundPike:
		{
			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL)
			{
				int iValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_GroundPike_Range, ESkillID::SKILLID_GroundPike );
				iValue += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );

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
			//FALSE so that Venom Spear will be casted once (via assembly)
			if ( UNITDATA_HOVER == NULL )
			{
				return FALSE;
			}

			return 2; //return TRUE but also bypass ASM to allow these skills to be held down. See BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
		}

		case ESkillID::SKILLID_VenomSpear:
		{
			//Check if lpMouseHoverChar is set. If not set
			// find nearest monster and assign it to 'lpMouseHoverChar'
			// we will have a target. Otherwise the skill won't be
			// casted if the mouse isn't on a target.
			if ( UNITDATA_HOVER == NULL )
			{
				int iValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_VenomSpear_Range, ESkillID::SKILLID_VenomSpear );
				iValue += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );

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

					return 2;
				}
			}

			//No nearby target still. Return
			//FALSE so that Venom Spear will be casted once (via assembly)
			if ( UNITDATA_HOVER == NULL )
			{
				return FALSE;
			}

			return 2; //return TRUE but also bypass ASM to allow these skills to be held down. See BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
		}

		case ESkillID::SKILLID_PikeWind:
		{
			//Unit is not performing an action and can use skill?
			if ( USESKILL( psSkill ) )
			{
				UNITDATA->BeginSkill( PikemanSkill::PikeWind, 0, 0, 0, 0, 0 );

				UNITDATA->sActiveSkill.cSkillTinyID = PikemanSkill::PikeWind;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->Animate( ANIMATIONTYPE_Skill );


				int iRange = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PikeWind_Pushback, psSkill->iLevel - 1 );

				UNITDATA->iMotionLoopSpeed = 80;
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, iRange, FALSE );

				SKILLMANAGERHANDLER->SendRangeDamage( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}

			break;
		}

		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}

BOOL CPikemanSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D)
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		default:
		{
			return FALSE; //not handled here. therefore handle it in ASM instead
		}
	}

	return TRUE; //handled here
}

BOOL CPikemanSkillHandler::OnEventSkill( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1, Pos2;
	RECT rect;
	int cnt, tmp, value, chain;
	int range;
	int x,y,z;

	int level = psActiveSkill->cLevel & 0xF;
	USHORT param = psActiveSkill->sParam;

	switch ( psActiveSkill->cSkillTinyID )
	{
		case SKILLPLAYID_T51:
		{
			return RINGOFSPEARSHANDLER->OnEvent( pcUnitData );
		}

		case PikemanSkill::Tornado:
		{
			range = param; //range is received via param

			if ( pcUnitData->pcTarget )
			{
				y = SKILLMANAGERHANDLER->GetRadian2D(pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, pcUnitData->pcTarget->sPosition.iX, pcUnitData->pcTarget->sPosition.iZ);
			}
			else
			{
				y = pcUnitData->sAngle.iY;
			}

			SKILLMANAGERHANDLER->GetMoveLocation(0, 0, range * fONE, 0, y, 0);
			x = pcUnitData->sPosition.iX + RADIAN3D_X;
			y = pcUnitData->sPosition.iY + RADIAN3D_Y;
			z = pcUnitData->sPosition.iZ + RADIAN3D_Z;

			//StartSkillDest
			CALL_WITH_ARG8(0x005742E0, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ,
				x, y, z, ESkillID::SKILLID_Tornado, level);

			SKILLMANAGERHANDLER->SkillPlaySound(SKILL_SOUND_SKILL_TORNADO1, pcUnitData);

			return TRUE;
		}
		break;

		case PikemanSkill::ShadowMaster:
		{
			pcUnitData->dwEventFrameExt[0] = ( 793 - 710 + pcUnitData->psModelAnimation->iBeginFrame ) * 160;
			pcUnitData->dwEventFrameExt[1] = ( 794 - 710 + pcUnitData->psModelAnimation->iBeginFrame ) * 160;
			pcUnitData->dwEventFrameExt[2] = ( 795 - 710 + pcUnitData->psModelAnimation->iBeginFrame ) * 160;

			if ( pcUnitData->pcTarget )
			{

				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_ShadowMaster_Numofhits, level - 1 );

				for(cnt=0; cnt<7; cnt++)
				{
					if ( SkillShadowMasterEvent[value][cnt] == pcUnitData->iMotionEvent )
					{
						//@todo - review this because crit is done on server side..
						if ( pcUnitData->iTargetCritical >= 0 )
						{
							if ( pcUnitData->GetAnimationAttackPosition( &Pos1.iX, &Pos1.iY, &Pos1.iZ )==TRUE )
							{
								SKILLMANAGERHANDLER->StartSkillEffect( Pos1, SKILLID_TripleImpact );
								SKILLMANAGERHANDLER->SetDynLight( Pos1.iX, Pos1.iY, Pos1.iZ, 100, 0, 0, 0, 400 );

								tmp = Dice::RandomI( 0, 2 );
								switch( tmp )
								{
									case 0:
										SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_SHADOW_STRIKE1, pcUnitData );
										break;
									case 1:
										SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_SHADOW_STRIKE2, pcUnitData );
										break;
									case 2:
										SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_SHADOW_STRIKE3, pcUnitData );
										break;
								}
							}

							pcUnitData->iTargetCritical = 1;
						}
						else
						{
							//CHATBOX->AddDebugMessage( "MISS" );

							//Miss Sound
							SOUNDHANDLER->SetWeaponSound( EWeaponSoundID::WEAPONSOUND_TwoHandMiss, pcUnitData );
						}

						if ( UNITDATA == pcUnitData )
						{
							if ( static_cast<int>( CHARACTER_SKILLEVENTDAMAGECOUNT ) < value )
							{
								chain = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PreciseHit_AddAttackRating_New, ESkillID::SKILLID_PreciseHit );

								tmp = pcUnitData->sCharacterData.iAttackRating;
								pcUnitData->sCharacterData.iAttackRating += ( pcUnitData->sCharacterData.iAttackRating * chain ) / 100;
								cnt = SKILLMANAGERHANDLER->SendTransDamage( pcUnitData->pcTarget, pcUnitData->sActiveSkill );
								pcUnitData->sCharacterData.iAttackRating = tmp; //restore

								//CHATBOX->AddDebugMessage( "SendTransDamage result: %d", x );

								if ( ! cnt)
									pcUnitData->iTargetCritical = -1;
								else
									pcUnitData->iTargetCritical = 0;
							}

							CHARACTER_SKILLEVENTDAMAGECOUNT++;
						}
					}
				}
			}

			return TRUE;
		}

		case PikemanSkill::VenomSpear:
		{
			if ( pcUnitData == UNITDATA )
			{
				cnt = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Pikeman_VenomSpear_Numberofspears, level - 1 );
				range = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_VenomSpear_Range, level - 1 );

				range += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );

				SKILLMANAGERHANDLER->SelectDamageCount ( pcUnitData, cnt, range );
				SKILLMANAGERHANDLER->SendRangeDamage ( pcUnitData, pcUnitData->sPosition, 0, 0, 0, 0, pcUnitData->sActiveSkill.iValue );
			}

			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_VENOM_SPEAR1, pcUnitData );

			//0x623E80 = SkillPlay_VenomSpear_Effect
			if ( CALL_WITH_ARG2( (DWORD)0x00623E80, (DWORD)pcUnitData, (DWORD)psActiveSkill->cLevel) > 0)
			{
				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_VENOM_SPEAR2, pcUnitData );
			}

			return TRUE;
		}

		default:
		{
			return FALSE; //process in ASM instead
		}
	}

	return TRUE; //don't process in ASM
}

BOOL CPikemanSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	UnitData * pcPlayerUnitData = UNITDATA;

	int range, temp, value;
	RECT rect;

	if ( psSkill->iLevel <= 0 || psSkill->iLevel > 10 )
		return TRUE; //return TRUE to notexecute code in assembly

	switch ( psSkill->iID )
	{
		case SKILLID_PreciseHit:
		{
			if ( USESKILL( psSkill ))
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );
					pcPlayerUnitData->Animate( ANIMATIONTYPE_Attack );

					pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::PreciseHit;
					pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

					if ( pcPlayerUnitData->pcTarget )
					{
						temp = pcPlayerUnitData->sCharacterData.iAttackRating;

							value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PreciseHit_AddAttackRating_New, pcPlayerUnitData->sActiveSkill.cLevel - 1 );
						pcPlayerUnitData->sCharacterData.iAttackRating += ( pcPlayerUnitData->sCharacterData.iAttackRating * value ) / 100;

						SKILLMANAGERHANDLER->SendTransDamage( pcPlayerUnitData->pcTarget, pcPlayerUnitData->sActiveSkill );

						pcPlayerUnitData->sCharacterData.iAttackRating = temp; //restore
					}

				temp = pcPlayerUnitData->sCharacterData.iAttackSpeed - 6;
				if ( temp < 0 ) temp = 0;
				if ( temp > 6 ) temp = 6;
				temp += 2;

				pcPlayerUnitData->iMotionLoopSpeed = ( 100 * ( fONE + 32 * temp + 32 ) ) >> FLOATNS;
				pcPlayerUnitData->iMotionLoopCnt = 2;
			}

			break;
		}

		case SKILLID_ChainLance:
		{
			if ( USESKILL( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::ChainLance;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill( pcPlayerUnitData, pcPlayerUnitData->sActiveSkill.iValue, 0, pcMouseTarget, 0, 0, 0 );

				pcPlayerUnitData->Animate( ANIMATIONTYPE_Skill );

				pcPlayerUnitData->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( pcPlayerUnitData->sCharacterData.iAttackSpeed );
				pcPlayerUnitData->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendTransDamage( pcMouseTarget, pcPlayerUnitData->sActiveSkill );
			}

			break;
		}

		case SKILLID_Expansion:
		{
			if ( USESKILL( psSkill ) )
			{
				temp = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_Expansion_AddSize, psSkill->iLevel - 1 );
				range = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_Expansion_Range_New, psSkill->iLevel - 1 );

				SKILLMANAGERHANDLER->RetryPlayAttack( pcMouseTarget );
				pcPlayerUnitData->Animate( ANIMATIONTYPE_Attack );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::Expansion;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayerUnitData->sActiveSkill.sParam = static_cast<short>( temp ); //store add size

				//note - other player does not

				SKILLMANAGERHANDLER->BeginSkill( pcPlayerUnitData, pcPlayerUnitData->sActiveSkill.iValue, 0, pcMouseTarget, 0, 0, 0 );

				rect.left = -30;
				rect.right = 30;
				rect.top = 0;
				rect.bottom = range;
				rect.bottom += 30; //get enemies beyond

				SKILLMANAGERHANDLER->SelectRangeBox( pcPlayerUnitData, rect, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage( pcMouseTarget, pcPlayerUnitData->sPosition, 0, 0, 0, 0, pcPlayerUnitData->sActiveSkill.iValue );
			}
			break;
		}

		case SKILLID_GroundPike:
		{
			if ( USESKILL( psSkill ) )
			{
				range = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_GroundPike_Range, psSkill->iLevel - 1 );
				range += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::GroundPike;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				pcPlayerUnitData->Animate( ANIMATIONTYPE_Skill );

				pcPlayerUnitData->iMotionLoopSpeed = 80;
				pcPlayerUnitData->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, range, FALSE );
				SKILLMANAGERHANDLER->SendRangeDamage( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}
			break;
		}

		case SKILLID_VenomSpear:
		{
			if ( USESKILL( psSkill ) )
			{
				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::VenomSpear;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, 0, 0, 0, 0 );
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				pcPlayerUnitData->iMotionLoopSpeed = 80;
				pcPlayerUnitData->iMotionLoopCnt = 1;
			}
			break;
		}


		case SKILLID_Tornado:
		{
			if ( USESKILL( psSkill ) )
			{
				int iDamageBoost = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_Tornado_DMGBoost, psSkill->iLevel - 1 );
				range = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_Tornado_Range, psSkill->iLevel - 1 );

				range += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::Tornado;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayerUnitData->sActiveSkill.sParam = range;

				SKILLMANAGERHANDLER->BeginSkill( pcPlayerUnitData, pcPlayerUnitData->sActiveSkill.iValue, psSkill->iLevel, pcMouseTarget, 0, 0, 0 );
				pcPlayerUnitData->Animate( EAnimationType::ANIMATIONTYPE_Skill );

				//classic range selection
				//int iX = abs( pcPlayerUnitData->sPosition.iX - pcPlayerUnitData->pcTarget->sPosition.iX ) >> 8;
				//int iZ = abs( pcPlayerUnitData->sPosition.iZ - pcPlayerUnitData->pcTarget->sPosition.iZ ) >> 8;
				//
				//int iDistXZ = (int)round( sqrt( ( iX * iX ) + ( iZ * iZ ) ) );
				//
				////cap
				//if ( iDistXZ > range )
				//	iDistXZ = range;
				//
				////add a little extra leeway
				//iDistXZ += 10;

				SKILLMANAGERHANDLER->SetUnitsRange( pcPlayerUnitData->sPosition, range, FALSE );

				SKILLMANAGERHANDLER->SendRangeDamage( pcPlayerUnitData, pcPlayerUnitData->sPosition, 0, 0, 0, 0, pcPlayerUnitData->sActiveSkill.iValue );
			}
			break;
		}

		case SKILLID_ChargingStrike:
		{
			if ( USESKILL( psSkill ) )
			{
				temp = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Pikeman_Expansion_AddSize, psSkill->iLevel - 1);
				int iChargingSpeed = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_ChargingStrike_RaiseSpeed, psSkill->iLevel - 1 );

				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::ChargingStrike;
				pcPlayerUnitData->sActiveSkill.cLevel = 0xF;
				pcPlayerUnitData->sActiveSkill.sParam = temp; //store expansion add size

				SKILLMANAGERHANDLER->BeginSkill( pcPlayerUnitData, pcPlayerUnitData->sActiveSkill.iValue, 0, pcMouseTarget, 0, 0, 0 );
				pcPlayerUnitData->Animate( EAnimationType::ANIMATIONTYPE_Skill );

				CHARACTER_SKILLCHARGINGFLAG = 1; //SkillChargingFlag;

				pcPlayerUnitData->iMotionLoopSpeed = 60 + iChargingSpeed * 2;
				pcPlayerUnitData->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_CHARGING, pcPlayerUnitData );
			}
			break;
		}

		case ESkillID::SKILLID_ShadowMaster:
		{
			if ( USESKILL( psSkill ) )
			{
				pcPlayerUnitData->sActiveSkill.cSkillTinyID = PikemanSkill::ShadowMaster;
				pcPlayerUnitData->sActiveSkill.cLevel = psSkill->iLevel;
				pcPlayerUnitData->BeginSkill( PikemanSkill::ShadowMaster, 0, pcPlayerUnitData->pcTarget, 0, 0, 0 );
				pcPlayerUnitData->Animate( ANIMATIONTYPE_Skill );

				pcPlayerUnitData->iMotionLoopSpeed = SKILLMANAGERHANDLER->GetAttackFrameSpeed( 7 );
				pcPlayerUnitData->iMotionLoopCnt = 1;
			}

			break;
		}
		case SKILLID_RingofSpears:
		{
			return RINGOFSPEARSHANDLER->OnCastAnimationStart ( psSkill, pcMouseTarget );
		}

		default:
		{
			return FALSE; //process in ASM instead
		}
	}

	return TRUE; //return TRUE to NOT process in ASM
}