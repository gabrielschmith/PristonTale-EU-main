#pragma once

class UxxxTarget;
class CAtalanta
{
private:

	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:



	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	CAtalanta( );

	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);


	int GetTriumphofValhallaAttackPowerBoost( User * pcUser );
	int SendHallOfValhallaBuff(User * pcCaster, int iLevel1Based, int iDurationSec);
	int SendTriumphOfValhallaBuff(User * pcCaster, int iLevel1Based, int iDurationSec);

	~CAtalanta( );
};

