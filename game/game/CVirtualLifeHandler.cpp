#include "stdafx.h"
#include "CVirtualLifeHandler.h"
#include "SkillManager.h"

CVirtualLifeHandler::CVirtualLifeHandler ()
{
}

CVirtualLifeHandler::~CVirtualLifeHandler ()
{
}

void CVirtualLifeHandler::UpdateTimer ( int iTime, int iLevel )
{
	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualLife );
	if ( psSkill )
	{
		psSkill->iUseTime = iTime;
		psSkill->iLevel = iLevel;
	}
}

void CVirtualLifeHandler::AddTimer ( int iTime, int iLevel )
{
	if ( !iTimerID )
	{
		iTimerID = TIMERSKILLHANDLER->CreateTextureMaterial ( "image\\Sinimage\\skill\\keep\\Pr3_V_LIFE.tga" );
	}

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualLife );

	if ( psSkill )
		ZeroMemory ( psSkill, sizeof ( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualLife ) )
	{
		Skill sSkill;
		ZeroMemory ( &sSkill, sizeof ( sSkill ) );

		STRINGCOPY ( sSkill.szName, "Virtual Life" );
		STRINGCOPY ( sSkill.sSkillInfo.szName, "Virtual Life" );
		STRINGCOPY ( sSkill.sSkillInfo.szDescription, "Increases health and mana points for a period of time" );

		sSkill.iID = TIMERSKILLID_VirtualLife;
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;
		TIMERSKILLHANDLER->AddSkillTimer ( &sSkill );
	}
}
