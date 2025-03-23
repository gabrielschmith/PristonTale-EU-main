#pragma once

class CAssassinSkillHandler
{
private:
	int iAlasScriptKeepID = 0;
	int iAlasScriptCastD = 0;

	int iInpesScriptKeepID = 0;
	int iInpesScriptCastD = 0;

public:
	CAssassinSkillHandler();
	virtual ~CAssassinSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill ( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget,  Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
};

