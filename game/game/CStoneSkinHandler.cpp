#include "stdafx.h"
#include "CStoneSkinHandler.h"


CStoneSkinHandler::CStoneSkinHandler()
{
}


CStoneSkinHandler::~CStoneSkinHandler()
{
}

void CStoneSkinHandler::LoadParticle()
{
	iParticleID = PARTICLE->Load( "game\\scripts\\particles\\R5StoneSkin.part" );
}

bool CStoneSkinHandler::OnCast( Skill * psSkill, UnitData * pcUnitData )
{
	if ( pcUnitData == UNITDATA )
	{
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T51;
		UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

		UNITDATA->iMotionLoopSpeed = 80;
		UNITDATA->iMotionLoopCnt = 1;
		
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		PARTICLE->Start( iParticleID, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}

	return true;
}

BOOL CStoneSkinHandler::OnAnimationEnd( UnitData * pcUnitData )
{
	pcUnitData->bNoDeath = iaStoneSkinImmortal[pcUnitData->sActiveSkill.cLevel - 1] * 70;

	return TRUE;
}
