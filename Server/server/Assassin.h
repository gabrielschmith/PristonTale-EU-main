#pragma once

static int iaDMGBoostASST5S1[10]
{
	47, 52, 57, 62, 67, 72, 77, 82, 87, 92
};

static int iaDMGBoostASST5S2[10]
{
	60, 65, 70, 75, 80, 85, 90, 95, 100, 105
};

static int iaDMGBoostASST5S3[10]
{
	35, 40, 45, 50, 55, 60, 65, 70, 75, 80
};

static int iaDMGBoostASST5S4[10]
{
	60, 64, 68, 72, 76, 80, 84, 88, 92, 96
};
static int iaDMGBoostAreaASST5S4[10]
{
	76, 78, 80, 82, 84, 86, 88, 90, 92, 94
};
static int iaDMGBoostRangeASST5S4[10]
{
	53, 56, 59, 62, 65, 68, 71, 74, 77, 80
};
static int iaAreaASST5S4[10]
{
	86, 90, 94, 98, 102, 106, 110, 114, 118, 122
};
static int iaPushRangeASST5S4[10]
{
	85, 90, 95, 100, 105, 110, 115, 120, 125, 130
};

class UxxxTarget;
class CAssassin
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

	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);

	void OnAddFinalDamage( User * pcUser, PacketAttackData * psPacket );

	void ApplyAlasBuff( User * pcUser, int iSkillLevel1Based, int iDurationSec );
	void ApplyInpesBuff( User * pcUser, int iSkillLevel1Based, int iDurationSec );
	void OnUseWisp( User * pcUser, PacketWisp * psPacket );

	CAssassin();
	virtual ~CAssassin();
};
