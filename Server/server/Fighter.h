#pragma once

class UxxxTarget;
class CFighter
{
private:


	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:
	CFighter();




	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	int GetBerserkerAttackPowerBoost( User * pcUser );
	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);

	void ApplyAndSendDemonBuff( User * pcTargetUser, int iLevelSkill1Based, int iDurationSec );
	void ApplyAndSendWryCryBuff( User * pcTargetUser, int iLevelSkill1Based, int iDuration, ID idCaster );

	~CFighter();
};

