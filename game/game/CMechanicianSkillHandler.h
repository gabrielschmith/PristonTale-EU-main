#pragma once

class CMechanicianSkillHandler
{
private:
	int electricalOverloadCastParticle = 0;
	int electricalOverloadKeepParticle = 0;

public:
	CMechanicianSkillHandler();
	virtual ~CMechanicianSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnSkillBuff(Unit* pcUnit, PacketUseSkillBase* psPacket);
	BOOL OnBeginSkill(class UnitData* pcThis, ActiveSkill& sActiveSkill, int iLevel1Based, class UnitData* pcUnitTarget, Point3D& point3D);
	BOOL OnSkillBuffUpdate(Unit* pcUnitTarget, PacketSkillBuffStatus* pStatus);
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
};

