#pragma once

class CArcherSkillHandler
{

public:
	CArcherSkillHandler();
	virtual ~CArcherSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnPlaySkillAttack ( struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill ( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget,  Point3D & point3D );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
};

