#pragma once

class CMagicianSkillHandler
{

public:
	CMagicianSkillHandler();
	virtual ~CMagicianSkillHandler();

	void LoadParticle();

	BOOL OnOpenPlaySkill ( struct Skill * psSkill );
	BOOL OnPlaySkillAttack (  struct Skill * psSkill, class UnitData * pcUnitData );
	BOOL OnBeginSkill( class UnitData * pcThis, ActiveSkill & psActiveSkill, int iLevel1Based, class UnitData * pcUnitTarget, Point3D & point3D );
	BOOL OnEventSkill (  UnitData * pcUnitData, ActiveSkill * psActiveSkill );
	BOOL OnEndSkill ( class UnitData * pcUnitData );

	BOOL OnSkillBuff( Unit * pcUnit, PacketUseSkillBase * psPacket );
	BOOL OnSkillBuffUpdate( Unit * pcUnitTarget, PacketSkillBuffStatus * pStatus );
	BOOL OnEnchantWeaponCastEffect( Unit * pcUnit, int uSkillLevel, int iParameter, ID idCaster, int iOverTime );
};

