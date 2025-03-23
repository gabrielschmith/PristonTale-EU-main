#include "stdafx.h"
#include "CVirtualRegenHandler.h"
#include "SkillManager.h"

CVirtualRegenHandler::CVirtualRegenHandler ()
{
}

CVirtualRegenHandler::~CVirtualRegenHandler ()
{
}

void CVirtualRegenHandler::UpdateTimer ( int iTime, int iLevel )
{
	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualRegen );
	if ( psSkill )
	{
		psSkill->iUseTime = iTime;
		psSkill->iLevel = iLevel;
	}
}

void CVirtualRegenHandler::AddTimer ( int iTime, int iLevel )
{
	if ( !iTimerID )
		iTimerID = TIMERSKILLHANDLER->CreateTextureMaterial ( "image\\Sinimage\\skill\\keep\\Pr4_R_FIELD.tga" );


	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualRegen );

	if ( psSkill )
		ZeroMemory ( psSkill, sizeof ( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer ( TIMERSKILLID_VirtualRegen ) )
	{
		Skill sSkill;
		ZeroMemory ( &sSkill, sizeof ( sSkill ) );
		STRINGCOPY ( sSkill.szName, "Virtual Regeneration" );
		STRINGCOPY ( sSkill.sSkillInfo.szName, "Virtual Regeneration" );
		STRINGCOPY ( sSkill.sSkillInfo.szDescription, "Increases health and mana regeneration for a period of time" );
		sSkill.iUseTime = iTime;
		sSkill.iLevel = iLevel;
		sSkill.MatIcon = iTimerID;

		sSkill.iID = TIMERSKILLID_VirtualRegen;

		TIMERSKILLHANDLER->AddSkillTimer ( &sSkill );
	}
}