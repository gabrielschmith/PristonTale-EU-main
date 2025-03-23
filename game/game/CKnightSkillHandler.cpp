#include "stdafx.h"
#include "CKnightSkillHandler.h"
#include "SkillManager.h"


int							iParticleID = -1;

CKnightSkillHandler::CKnightSkillHandler()
{
}

CKnightSkillHandler::~CKnightSkillHandler()
{
}

void CKnightSkillHandler::LoadParticle()
{
	iHolyValorScriptID = PARTICLE->Load("Effect\\Particle\\Script\\HolyValor.part");
	iHolyBodyScriptID = PARTICLE->Load("Effect\\Particle\\Script\\HolyBody.part");
	iDrasticSpiritScriptID = PARTICLE->Load("Effect\\Particle\\Script\\DrasticSpirit.part");
}

BOOL CKnightSkillHandler::OnOpenPlaySkill ( struct Skill * psSkill )
{
	int iValue;

	switch ( psSkill->iID )
	{
		case SKILLID_HolyValor:
		{
			if ( UNITDATA->IsLowAnimationType() &&
				 USEMANASTAMINA( psSkill ))
			{
				UNITDATA->pcTarget = NULL;

				UNITDATA->sActiveSkill.cSkillTinyID = KnightSkill::HolyValor;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill();
				UNITDATA->Animate( ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				//send buff to server
				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}
		}
		break;

		case SKILLID_HolyBody:
		{
			if ( UNITDATA->IsLowAnimationType() &&
				USEMANASTAMINA ( psSkill ))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = KnightSkill::HolyBody;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill();
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}
		}

		break;

		case SKILLID_DrasticSpirit:
		{
			if ( UNITDATA->IsLowAnimationType() &&
				USEMANASTAMINA( psSkill ))
			{
				UNITDATA->sActiveSkill.cSkillTinyID = KnightSkill::DrasticSpirit;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				UNITDATA->BeginSkill();
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );
				UNITDATA->iMotionLoopSpeed = 70;
				UNITDATA->iMotionLoopCnt = 1;

				SKILLMANAGERHANDLER->SendProcessSkillToServer( UNITDATA->sActiveSkill.cSkillTinyID, psSkill->iLevel );
			}
		}
		break;

		case SKILLID_HolyConviction:
		{
			return HOLYCONVICTIONHANDLER->OnCastAnimationStart ( psSkill, UNITDATA );
			break;
		}
		default:
		{
			return FALSE; //return false to handle it in ASM instead
		}
	}

	return TRUE;
}
BOOL CKnightSkillHandler::OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcMouseTarget )
{
	RECT rect;

	switch ( psSkill->iID )
	{
		case SKILLID_SwordBlast:
		{
			if ( USESKILL ( psSkill ) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = KnightSkill::SwordBlast;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				SKILLMANAGERHANDLER->BeginSkill ( UNITDATA, 0, 0, 0, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				int iAttackRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Knight_SwordBlast_AttackRange, psSkill->iLevel - 1 );
				int iAttackWidth = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Knight_SwordBlast_AttackWidth_New, psSkill->iLevel - 1 );

				rect.left = -( iAttackWidth >> 1 );	//half
				rect.right = ( iAttackWidth >> 1 );	//half
				rect.top = 0;
				rect.bottom = iAttackRange;

				SKILLMANAGERHANDLER->SelectRangeBox ( UNITDATA, rect, FALSE );

				SKILLMANAGERHANDLER->SendRangeDamage ( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}

			return TRUE;
		}

		case SKILLID_Piercing:
		{
			if ( USESKILL(psSkill) )
			{
				SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget );

				UNITDATA->sActiveSkill.cSkillTinyID = KnightSkill::Piercing;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;
				UNITDATA->sActiveSkill.sParam = 0;

				SKILLMANAGERHANDLER->BeginSkill( UNITDATA, 0, 0, 0, 0, 0, 0 );
				UNITDATA->Animate ( EAnimationType::ANIMATIONTYPE_Skill );

				SOUNDHANDLER->SetSkillSound ( SKILL_SOUND_SKILL_PIERCING, pcMouseTarget->sPosition.iX, pcMouseTarget->sPosition.iY, pcMouseTarget->sPosition.iZ );

				int iAttackRange = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Knight_Piercing_PierceRange_New, psSkill->iLevel - 1 );
				int iAttackWidth = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Knight_Piercing_PierceWidth_New, psSkill->iLevel - 1 );

				rect.left = -( iAttackWidth >> 1 );	//half
				rect.right = ( iAttackWidth >> 1 );	//half
				rect.top = 0;
				rect.bottom = iAttackRange;

				SKILLMANAGERHANDLER->SelectRangeBox ( UNITDATA, rect, FALSE );

				SKILLMANAGERHANDLER->SendRangeDamage ( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
			}

			//if ( SKILL_LEFTSIDE == psSkill && SKILLMANAGERHANDLER->RetryPlayAttack ( pcMouseTarget ) ) break;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CKnightSkillHandler::OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D)
{
	switch ( sActiveSkill.cSkillTinyID )
	{
		case KnightSkill::HolyValor:
		{
			//CASSAChar::StartMacro( pcThis, HOLY_VALOR_MEMBER, HOLY_ACTION );
			SKILLMANAGERHANDLER->SkillPlaySound ( SKILL_SOUND_SKILL_HOLY_VALOR1, UNITDATA );
		}
		break;

		case KnightSkill::HolyBody:
		{
			SKILLMANAGERHANDLER->SkillPlaySound( SKILL_SOUND_SKILL_HOLYBODY, UNITDATA );
		}
		break;

		case KnightSkill::DrasticSpirit:
		{
			//CASSAChar::StartMacro( pcThis, DRASTIC_SPIRIT_MEMBER, DRASTIC_ACTION );
			SKILLMANAGERHANDLER->SkillPlaySound (SKILL_SOUND_SKILL_DRASTICSPIRIT1, UNITDATA );
		}
		break;


		default:
		{
			return FALSE; //not handled here. therefore handle it in ASM instead
		}
	}

	return TRUE; //handled here
}

BOOL CKnightSkillHandler::OnEventSkill ( UnitData * pcUnitData, ActiveSkill * psActiveSkill )
{
	Point3D	Pos1,Pos2;
	RECT rect;
	int cnt;

	switch ( psActiveSkill->cSkillTinyID )
	{
		case KnightSkill::HolyValor:
		{
			//handled
		}
		break;

		case KnightSkill::HolyBody:
		{
			//Nothing here
		}
		break;

		case KnightSkill::DrasticSpirit:
		{
			//handled
		}
		break;

		default:
		{
			return FALSE; //not handled here. therefore handle it in ASM instead
		}
	}

	return TRUE; //handled here
}
/// <summary>
/// Called when a unit (other or self) received a timed skill buff
/// using the new packet structure
/// Perhaps have a look at
/// int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill ) in the 2012 code
/// how to make this better, i.e. player coming in range when previously not in range
/// </summary>
BOOL CKnightSkillHandler::OnSkillBuff( Unit * pcUnitTarget, PacketUseSkillBase * psPacket )
{
	int duration;

	BOOL bIsTheCaster = pcUnitTarget->GetID() == psPacket->lCasterID;

	//AssaParticle_HolyValor_Member


	return TRUE;
}



BOOL CKnightSkillHandler::OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus )
{
	//if ( pStatus->uHolyBodyTimeLeft > 0 && pcUnitTarget->bHolyBodyLevel1Based == 0 )
	//{
	//	CALL_WITH_ARG1(0x52E9B0, (DWORD)pcUnitTarget);
	//}
	//else
	//{
	//
	//}

	//is me? add timer
	if ( pcUnitTarget == UNIT )
	{
		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_HolyValor, ETimerID::HolyValor, ESkillArrayPointer::Knight_HolyValor_Duration,
			pStatus->uHolyValorTimeLeft, pStatus->bHolyValorLevel1Based, pStatus->bDrasticSpiritLevel1Based > 0 ? TRUE : FALSE ); //halved buff values if Drastic Spirit is active

		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_HolyBody, ETimerID::HolyBody, ESkillArrayPointer::Knight_HolyBody_Duration,
			pStatus->uHolyBodyTimeLeft, pStatus->bHolyBodyLevel1Based );

		TIMERSKILLHANDLER->AddOrUpdateSkillTimer( ESkillID::SKILLID_DrasticSpirit, ETimerID::DrasticSpirit, ESkillArrayPointer::Knight_DrasticSpirit_Duration,
			pStatus->uDrasticSpiritTimeLeft, pStatus->bDrasticSpiritLevel1Based );
	}

	//005B78EB	0x18	56 68 BC 02 00 00 57 E8 A9 CF FF FF 56 68 EE 02 00 00 57 E8 9D CF FF FF 	90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90

	//if ( pStatus->uDrasticSpiritTimeLeft > 0 )
	//{
	//	pcUnitTarget->bDrasticSpiritLevel1Based = pStatus->bDrasticSpiritLevel1Based;
	//	CASSAChar::StartMacro( pcUnitTarget->pcUnitData, DRASTIC_SPIRIT_MEMBER, DRASTIC_ACTION );
	//}
	//else
	//{
	//	if ( pcUnitTarget->bDrasticSpiritLevel1Based )
	//	{
	//		EffectBaseFollow::StopEffect( pcUnitTarget->pcUnitData, DRASTIC_ACTION );
	//		pcUnitTarget->bDrasticSpiritLevel1Based = 0;
	//	}
	//}
	//
	//if ( pStatus->uHolyBodyTimeLeft > 0 && pcUnitTarget->bHolyBodyLevel1Based == 0)
	//{
	//	CALL_WITH_ARG1(0x52E9B0, (DWORD)pcUnitTarget);
	//}
	//else
	//{
	//
	//}

	PARTICLE->CheckAndStartKeepScript(pcUnitTarget, iDrasticSpiritScriptID, pStatus->uDrasticSpiritTimeLeft,	pStatus->bDrasticSpiritLevel1Based,		pcUnitTarget->bDrasticSpiritLevel1Based);
	PARTICLE->CheckAndStartKeepScript(pcUnitTarget, iHolyBodyScriptID,		pStatus->uHolyBodyTimeLeft,			pStatus->bHolyBodyLevel1Based,			pcUnitTarget->bHolyBodyLevel1Based);
	PARTICLE->CheckAndStartKeepScript(pcUnitTarget, iHolyValorScriptID,		pStatus->uHolyValorTimeLeft,		pStatus->bHolyValorLevel1Based,			pcUnitTarget->bHolyValorLevel1Based);

	//if ( pStatus->uHolyValorTimeLeft > 0 )
	//{
	//	pcUnitTarget->bHolyValorLevel1Based = pStatus->bHolyValorLevel1Based;
	//
	//	//SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect("HolyValor", pcUnitTarget->pcUnitData, 300, SKILLID_HolyValor);
	//
	//	//CASSAChar::StartMacro( pcUnitTarget->pcUnitData, HOLY_VALOR_MEMBER, HOLY_ACTION );  //todo - try and understand why it doesn't stop the effect when going into town
	//}
	//else
	//{
	//	if ( pcUnitTarget->bHolyValorLevel1Based )
	//	{
	//		EffectBaseFollow::StopEffect( pcUnitTarget->pcUnitData, HOLY_ACTION );
	//		pcUnitTarget->bHolyValorLevel1Based = 0;
	//	}
	//}

	//AssaParticle_HolyValor_Jang_5B7330 - int AssaParticle_HolyValor_Jang(smCHAR *pChar, int maxTime)
	//AssaParticle_HolyValor_Member_5B7970 - int AssaParticle_HolyValor_Member(smCHAR *pChar, int maxTime)
	//AssaParticle_DrasticSpirit_Jang_5B7A30 - int AssaParticle_DrasticSpirit_Jang(smCHAR *pChar, int maxTime)


	//StopAssaCodeEffect_5B48A0((int)a1, HOLY_ACTION, 0); //for Holy Valor
	//StopAssaCodeEffect_5B48A0((int)a1, DRASTIC_ACTION, 0); //For Drastic Spirit

	//todo - research why Holy Valor cannot be used with other buffs..

	//AssaParticle_HolyValor_Jang_5B7330
	//int AssaParticle_HolyValor_Jang(smCHAR *pChar, int maxTime)

	//SKILLMANAGERHANDLER->AssaParticle_KeepSkillEffect( "VenomThrone", pcUnitTarget->pcUnitData, psPacket->sTimeLeftSec, psPacket->iSkillID );


	//PARTICLE->CheckAndStartKeepScript( pcUnitTarget, iInpesScriptKeepID, pStatus->uInpesTimeLeft, pStatus->bInpesLevel1Based, pcUnitTarget->bInpesLevel1Based );

	return TRUE;
}

