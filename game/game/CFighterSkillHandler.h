#pragma once

class CFighterSkillHandler
{
private:
	int iDemonsBaneScriptID = 0;
	int iDemonsBaneScriptKeepID = 0;

public:
	CFighterSkillHandler();
	virtual ~CFighterSkillHandler();

	void LoadParticle ( );

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnEndSkill( UnitData * pcThis );

	BOOL OnSkillBuff( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
};

