#pragma once

static int iaCriticalJudment[10]
{
	3, 3, 3, 4, 4, 4, 5, 5, 5, 6
};

class UxxxTarget;
class CShaman
{
private:


	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:
	CShaman();
	virtual ~CShaman();





	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);

	void ApplyAdventMigalBuff( User * pcCaster, int iLevel1Based, int iDurationSec );
	int GetAdventMigalAttackPowerBoost( User * pcUser );
	int GetChasingHuntAtkPowBoost( User * pcUser );
};

