#pragma once

class CPikemanSkillHandler
{

public:
	CPikemanSkillHandler();
	virtual ~CPikemanSkillHandler();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
};

