#pragma once

class CKnightSkillHandler
{

public:
	CKnightSkillHandler();
	virtual ~CKnightSkillHandler();

	int iHolyValorScriptID = 0;
	int iHolyBodyScriptID = 0;
	int iDrasticSpiritScriptID = 0;

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill ( class UnitData * pcThis, ActiveSkill & sActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget,  Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnSkillBuff ( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
	BOOL AssaParticle_HolyValor_Member_5B7970( UnitData * pcUnitData, int iDuration );
};

