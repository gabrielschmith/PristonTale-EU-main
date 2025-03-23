#pragma once

class UxxxTarget;
class CKnight
{
private:


	PacketSkillInfoContainer sSkillInfoContainer;
	PacketSkillDataContainer sSkillDataContainer;

public:
	CKnight( );




	void SendServerSkillDataToClient ( User * pcUser );
	void SendServerSkillInfoToClient ( User * pcUser );
	void UpdateAndCacheServerSkillInfo ();
	void UpdateAndCacheServerSkillData ();

	void BuildPacketSkillDataContainer ( PacketSkillDataContainer * container, BOOL bIsThisClass );

	int GetGodsBlessingAttackPowerBoost( User * pcUser );
	int AddHolyConvictionUndeadAttackPowerBoost( User * pcUser, UxxxTarget * p_Target, int TypeDamage );
	int RecvSkillSingleTarget( User * pcUser, UxxxTarget * p_Target, int * piAttackPower, PacketSingleTargetSkillDataEx * psPacketEx );
	int RecvSkillAreaTarget( User * pcUser, UxxxTarget * p_Target, PacketSkillDamageRange * psPacketRange, PacketMultiTargetSkillData * psPacketRangeEx );
	int RecvBuffSkill( User * pcUser, PacketSkillCast * packet);
	int AddHolyValor( User * pcUser, PacketSkillPartyData * packet );
	int AddHolyValorBuff_AOEOnly_UndeadOnly ( User * pcUser, int iFinalAoEDamage );
	int AddHolyConviction( User * pcUser, PacketSkillPartyData * packet );
	void OnCancelDrasticSpirit( User * pcUser );

	int GetHolyValorBuff( User * pcUser );
	void ApplyAndSendHolyValorBuff( User * pcCaster, int iLevel1Based, int iDurationSec );

	~CKnight( );
};

