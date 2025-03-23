#pragma once

class UxxxTarget;
class CMagician
{
private:


	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;


	BOOL OnEnchantWeaponCasted( User * pcCasterUser, PacketSkillCast * packet, int iLevelSkill1Based );
	BOOL ApplyEnchantWeapon( User * pcCasterUser, EElementID element, int iLevelSkill1Based, int iDurationSec, ID iCasterID );

public:
	CMagician();
	~CMagician();

	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	static int UseDistortionMonster( User * pcUser, PacketSkillDamageRange * psPacket, int iLevelSkill );
	static int UseDistortionUser( User * pcUser, int iTimeByLevel );
	int GetSpiritElementalAttackPowerBoost( User * pcUser, int TypeDamage );
	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);

	void ApplyEnchantWeaponRotation( User * pcUser, ID idCaster = 0 );


};

