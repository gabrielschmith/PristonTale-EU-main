#pragma once

#include "CMiraculousLight.h"
#include "CLethalSightHandler.h"
#include "CFierceWindHandler.h"
#include "RessurectionHandler.h"
#include "CSesmicImpactHandler.h"
#include "CStoneSkinHandler.h"
#include "CRuptureHandler.h"
#include "CIceElementalHandler.h"
#include "CHolyConvictionHandler.h"
#include "CSummonTigerHandler.h"
#include "CRedRayHandler.h"
#include "CRingsOfSpearsHandler.h"
#include "CWizardTrance.h"
#include "CShadowBlinkHandler.h"
#include "CCreedHandler.h"
#include "CChasingHuntHandler.h"
#include "CImpulsionHandler.h"
#include "CVIRTUALREGENHANDLER.h"
#include "CVirtualLifeHandler.h"

#include "CArcherSkillHandler.h"
#include "CFighterSkillHandler.h"
#include "CMechanicianSkillHandler.h"
#include "CPikemanSkillHandler.h"
#include "CAssassinSkillHandler.h"
#include "CKnightSkillHandler.h"
#include "CAtalantaSkillHandler.h"
#include "CMagicianSkillHandler.h"
#include "CPriestessSkillHandler.h"
#include "CShamanSkillHandler.h"

//Priestess T5
#define MIRACULOUSLIGHTHANDLER	(SKILLEFFECTHANDLER->GetMiraculousLight())
#define RESSURECTIONHANDLER		(SKILLEFFECTHANDLER->GetRessurection())
#define ICEELEMENTALHANDLER		(SKILLEFFECTHANDLER->GetIceElemental())
#define VIRTUALREGENHANDLER		(SKILLEFFECTHANDLER->GetVirtualRegen())
#define VIRTUALLIFEHANDLER		(SKILLEFFECTHANDLER->GetVirtualLife())


#define FIGHTERSKILLHANDLER		(SKILLEFFECTHANDLER->GetFighterSkillHandler())
#define MECHANICIANSKILLHANDLER	(SKILLEFFECTHANDLER->GetMechanicianSkillHandler())	
#define ARCHERSKILLHANDLER		(SKILLEFFECTHANDLER->GetArcherSkillHandler())		
#define PIKEMANSKILLHANDLER		(SKILLEFFECTHANDLER->GetPikemanSkillHandler())		
#define	ASSASSINSKILLHANDLER	(SKILLEFFECTHANDLER->GetAssassinSkillHandler())		
#define ATALANTASKILLHANDLER	(SKILLEFFECTHANDLER->GetAtalantaSkillHandler())		
#define KNIGHTSKILLHANDLER		(SKILLEFFECTHANDLER->GetKnightSkillHandler())		
#define MAGICIANSKILLHANDLER	(SKILLEFFECTHANDLER->GetMagicianSkillHandler())		
#define PRIESTESSSKILLHANDLER	(SKILLEFFECTHANDLER->GetPriestessSkillHandler())	
#define SHAMANSKILLHANDLER		(SKILLEFFECTHANDLER->GetShamanSkillHandler())		


//Archer T5
#define LETHALSIGHTHANDLER		(SKILLEFFECTHANDLER->GetLethalSight())
#define FIERCEWINDHANDLER		(SKILLEFFECTHANDLER->GetFierceWind())

//Pikeman T5
#define RINGOFSPEARSHANDLER		(SKILLEFFECTHANDLER->GetRingOfSpears())

//Fighter T5
#define	SESMICIMPACTHANDLER		(SKILLEFFECTHANDLER->GetSesmicImpact())

//Mechanician T5
#define IMPULSIONHANDLER		(SKILLEFFECTHANDLER->GetImpulsion())
#define RUPTUREHANDLER			(SKILLEFFECTHANDLER->GetRupture())

//Magician T5
#define STONESKINHANDLER		(SKILLEFFECTHANDLER->GetStoneSkin())
#define REDRAYHANDLER			(SKILLEFFECTHANDLER->GetRedRay())
#define WIZARDTRANCEHANDLER		(SKILLEFFECTHANDLER->GetWizardTrance())

//Knight T5
#define HOLYCONVICTIONHANDLER	(SKILLEFFECTHANDLER->GetHolyConviction())

//Atalanta T5
#define SUMMONTIGERHANDLER		(SKILLEFFECTHANDLER->GetSummonTiger())

//Assassin
#define SHADOWBLINKHANDLER		(SKILLEFFECTHANDLER->GetShadowBlink())

//Shaman
#define CREEDHANDLER			(SKILLEFFECTHANDLER->GetCreed())
#define CHASINGHUNTHANDLER		(SKILLEFFECTHANDLER->GetChasingHunt())

#define USEMANASTAMINA(s)	    (SKILLMANAGERHANDLER->UseManaStamina(s))
#define USESKILL(s)				(CALL((DWORD)((Skill*)s)->sSkillInfo.dwFunctionPointer))
#define PROCESSSKILLBUFFT(ud, target) (SKILLEFFECTHANDLER->ProcessSkillBuff(ud, target))
#define PROCESSSKILLBUFF(ud)	(SKILLEFFECTHANDLER->ProcessSkillBuff(ud, NULL))

class CSkillEffectHandler
{
public:
								CSkillEffectHandler();
	virtual					   ~CSkillEffectHandler();

	//Priestess T5
	CMiraculousLight			* GetMiraculousLight() { return pcMiraculousLight; };
	CRessurectionHandler		* GetRessurection() { return pcRessurection; }
	CIceElementalHandler		* GetIceElemental() { return pcIceElemental; }
	CVirtualRegenHandler			* GetVirtualRegen() { return pcVirtualRegen; }
	CVirtualLifeHandler			* GetVirtualLife() { return pcVirtualLife; }

	//Archer T4

	CFighterSkillHandler		* GetFighterSkillHandler()		{ return pcFighterSkillHandler; };
	CMechanicianSkillHandler	* GetMechanicianSkillHandler()	{ return pcMechanicianSkillHandler; };
	CArcherSkillHandler			* GetArcherSkillHandler()		{ return pcArcherSkillHandler; };
	CPikemanSkillHandler		* GetPikemanSkillHandler()		{ return pcPikemanSkillHandler; };
	CAssassinSkillHandler		* GetAssassinSkillHandler()		{ return pcAssassinSkillHandler; };
	CAtalantaSkillHandler		* GetAtalantaSkillHandler()		{ return pcAtalantaSkillHandler; };
	CKnightSkillHandler			* GetKnightSkillHandler()		{ return pcKnightSkillHandler; };
	CMagicianSkillHandler		* GetMagicianSkillHandler()		{ return pcMagicianSkillHandler; };
	CPriestessSkillHandler		* GetPriestessSkillHandler()	{ return pcPriestessSkillHandler; };
	CShamanSkillHandler			* GetShamanSkillHandler()		{ return pcShamanSkillHandler; };


	//Archer T5
	CLethalSightHandler			* GetLethalSight() { return pcLethalSight; };
	CFierceWindHandler			* GetFierceWind() { return pcFierceWind; };

	//Fighter T5
	CSesmicImpactHandler		* GetSesmicImpact() { return pcSesmicImpact; }

	//Pikeman T5
	CRingsOfSpearsHandler		* GetRingOfSpears(){ return pcRingOfSpears; }

	//Mechanician T5
	CImpulsionHandler			* GetImpulsion() { return pcImpulsion; }
	CRuptureHandler				* GetRupture() { return pcRupture; }

	//Magician T5
	CStoneSkinHandler			* GetStoneSkin() { return pcStoneSkin; }
	CRedRayHandler				* GetRedRay(){ return pcRedRay; }
	CWizardTrance				* GetWizardTrance(){ return pcWizardTrance; }

	//Knight T5
	CHolyConvictionHandler		* GetHolyConviction() { return pcHolyConviction; }

	//Atalanta
	CSummonTigerHandler			* GetSummonTiger(){ return pcSummonTiger; }

	//Assassin
	CShadowBlinkHandler			* GetShadowBlink() { return pcShadowBlink; }

	//Creed
	CCreedHandler				* GetCreed() { return pcCreed; }
	CChasingHuntHandler			* GetChasingHunt() { return pcChasingHunt; }

	void						Init();

	void						ProcessSkillBuff( UnitData * pcUnitData, UnitData * pcTarget );

	void						Update();

protected:
	//Priestess T5
	CMiraculousLight			* pcMiraculousLight;
	CRessurectionHandler		* pcRessurection;
	CIceElementalHandler		* pcIceElemental;
	CVirtualRegenHandler		    * pcVirtualRegen;
	CVirtualLifeHandler		    * pcVirtualLife;

	CFighterSkillHandler		* pcFighterSkillHandler;
	CMechanicianSkillHandler		* pcMechanicianSkillHandler;
	CArcherSkillHandler			* pcArcherSkillHandler;
	CPikemanSkillHandler		* pcPikemanSkillHandler;
	CAssassinSkillHandler		* pcAssassinSkillHandler;

	CAtalantaSkillHandler		* pcAtalantaSkillHandler;
	CKnightSkillHandler			* pcKnightSkillHandler;
	CMagicianSkillHandler		* pcMagicianSkillHandler;
	CPriestessSkillHandler		* pcPriestessSkillHandler;
	CShamanSkillHandler			* pcShamanSkillHandler;

	//Archer T5
	CLethalSightHandler			* pcLethalSight;
	CFierceWindHandler			* pcFierceWind;

	//Fighter T5
	CSesmicImpactHandler		* pcSesmicImpact;

	//Pikeman T5
	CRingsOfSpearsHandler		* pcRingOfSpears;

	//Mechanician T5
	CImpulsionHandler			* pcImpulsion;
	CRuptureHandler				* pcRupture;

	//Magician T5
	CStoneSkinHandler			* pcStoneSkin;
	CRedRayHandler				* pcRedRay;
	CWizardTrance				* pcWizardTrance;

	//Knight T5
	CHolyConvictionHandler		* pcHolyConviction;

	//Atalanta T5
	CSummonTigerHandler			* pcSummonTiger;

	//Assassin
	CShadowBlinkHandler			* pcShadowBlink;

	//Shaman
	CCreedHandler				* pcCreed;
	CChasingHuntHandler			* pcChasingHunt;
};

