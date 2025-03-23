#pragma once

class UxxxTarget;

class CArcher
{
private:





	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:
	CArcher( );




	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	int GetForceOfNatureAttackPowerBoost( User * pcUser );
	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);

	void ApplyForceOfNatureBuff( User * pcCaster, int iLevel1Based, int iDurationSec );
	void OnAddFinalDamage( User * pcUser, PacketAttackData * psPacket );

	~CArcher();
};

