#include "stdafx.h"
#include "CSkillEffectHandler.h"

CSkillEffectHandler::CSkillEffectHandler()
{
	//Priestess T5
	pcMiraculousLight		= new CMiraculousLight();
	pcIceElemental			= new CIceElementalHandler();
	pcVirtualRegen			= new CVirtualRegenHandler();
	pcVirtualLife			= new CVirtualLifeHandler();

	pcFighterSkillHandler		= new CFighterSkillHandler ();
	pcMechanicianSkillHandler	= new CMechanicianSkillHandler ();
	pcArcherSkillHandler		= new CArcherSkillHandler ();
	pcPikemanSkillHandler		= new CPikemanSkillHandler ();
	pcAssassinSkillHandler		= new CAssassinSkillHandler ();

	pcAtalantaSkillHandler		= new CAtalantaSkillHandler ();
	pcKnightSkillHandler		= new CKnightSkillHandler ();
	pcMagicianSkillHandler		= new CMagicianSkillHandler ();
	pcPriestessSkillHandler		= new CPriestessSkillHandler ();
	pcShamanSkillHandler		= new CShamanSkillHandler ();

	//Archer T5
	pcLethalSight			= new CLethalSightHandler();
	pcFierceWind			= new CFierceWindHandler();

	//Fighter T5
	pcSesmicImpact			= new CSesmicImpactHandler();

	//Pikeman T5
	pcRingOfSpears			= new CRingsOfSpearsHandler();

	//Mechanician T5
	pcImpulsion				= new CImpulsionHandler();
	pcRupture				= new CRuptureHandler();

	//Magician T5
	pcStoneSkin				= new CStoneSkinHandler();
	pcRedRay				= new CRedRayHandler();
	pcWizardTrance			= new CWizardTrance();

	//Knight T5
	pcHolyConviction		= new CHolyConvictionHandler();

	//Atalanta T5
	pcSummonTiger			= new CSummonTigerHandler();

	//Assassin
	pcShadowBlink			= new CShadowBlinkHandler();
	
	//Shaman
	pcCreed					= new CCreedHandler();
	pcChasingHunt			= new CChasingHuntHandler();
}

CSkillEffectHandler::~CSkillEffectHandler()
{
	//Priestess T5
	SAFE_DELETE( pcMiraculousLight );
	SAFE_DELETE( pcIceElemental );
	SAFE_DELETE( pcVirtualRegen );
	SAFE_DELETE( pcVirtualLife );

	SAFE_DELETE( pcFighterSkillHandler	);
	SAFE_DELETE( pcMechanicianSkillHandler );
	SAFE_DELETE( pcArcherSkillHandler		);
	SAFE_DELETE( pcPikemanSkillHandler	);
	SAFE_DELETE( pcAssassinSkillHandler	);

	SAFE_DELETE( pcAtalantaSkillHandler	);	
	SAFE_DELETE( pcKnightSkillHandler		);
	SAFE_DELETE( pcMagicianSkillHandler	);	
	SAFE_DELETE( pcPriestessSkillHandler	);	
	SAFE_DELETE( pcShamanSkillHandler		);

	//Archer T5
	SAFE_DELETE( pcLethalSight );
	SAFE_DELETE( pcFierceWind );

	//Mechanician T5
	SAFE_DELETE( pcImpulsion );
	SAFE_DELETE( pcRupture );

	//Fighter T5
	SAFE_DELETE( pcSesmicImpact );

	//Pikeman T5
	SAFE_DELETE( pcRingOfSpears );

	//Magician T5
	SAFE_DELETE( pcStoneSkin );
	SAFE_DELETE( pcRedRay );
	SAFE_DELETE( pcWizardTrance );

	//Knight T5
	SAFE_DELETE( pcHolyConviction );

	//Atalanta T5
	SAFE_DELETE( pcSummonTiger );

	//Assassin
	SAFE_DELETE( pcShadowBlink );

	//Shaman
	SAFE_DELETE( pcCreed );
	SAFE_DELETE( pcChasingHunt );
}

void CSkillEffectHandler::Init()
{
}

/// <summary>
/// Leak: SendProcessSKillToServer
/// ASM: 623790
/// </summary>
/// <param name="pcUnitData"></param>
/// <param name="pcTarget"></param>
void CSkillEffectHandler::ProcessSkillBuff( UnitData * pcUnitData, UnitData * pcTarget )
{
	int iID = pcTarget != NULL ? pcTarget->iID : 0;

	CALL_WITH_ARG4( 0x00623790, pcUnitData->sActiveSkill.cSkillTinyID, pcUnitData->sActiveSkill.cLevel, iID, 0 );
}

void CSkillEffectHandler::Update()
{
	pcShadowBlink->Update();
}
