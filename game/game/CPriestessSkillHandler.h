#pragma once

class CPriestessSkillHandler
{
private:
	int iVirtualLifeCastScriptID = 0;
	int iVirtualLifeKeepScriptID = 0;

	int iVirtualRegenCastScriptID = 0;
	int iVirtualRegenKeepScriptID = 0;

public:
	CPriestessSkillHandler();
	virtual ~CPriestessSkillHandler();

	void LoadParticle ( );
	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
};

