#pragma once

class CAtalantaSkillHandler
{

private:

	int iValhallaScriptID = 0;

public:
	CAtalantaSkillHandler();
	virtual ~CAtalantaSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill ( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
};

