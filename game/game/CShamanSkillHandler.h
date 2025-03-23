#pragma once

class CShamanSkillHandler
{

private:
	int iAdventMigalCastScriptID = 0;
	int iAdventMigalKeepScriptID = 0;

public:
	CShamanSkillHandler();
	virtual ~CShamanSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
};

