#pragma once

class UxxxTarget;
class CPriestess
{
private:



	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:

	CPriestess();




	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	int GetSummonMuspellAttackPowerBoost ( User * pcUserData );
	void OnUseHolyMind( User * pcUser, PacketHolyMind * psPacket );
	void OnHolyMindHandler( User * pcUserTarget, int * piDamage );
	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);
	BOOL OnGrandHealingCasted ( User * pcUser, PacketGrandHealing * psPacket, int iRange );
	BOOL ApplyAndSendVirtualLifeBuff( User * pcCaster, int iLevel1Based, int iDurationSec );
	BOOL ApplyAndSendVirtualRegenBuff( User * pcCaster, int iLevel1Based, int iDurationSec );
	~CPriestess();
};

