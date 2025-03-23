#pragma once


#define TIMERSKILLBASE (0x096C0000)

enum ETimerSkillID
{
	TIMERSKILLID_QuestNew						= 0x096A0003,
	TIMERSKILLID_QuestNewT						= 0x096B0003,
	TIMERSKILLID_BellatraGold					= 0x096A0061,
	TIMERSKILLID_BellatraSilver					= 0x096A0062,
	TIMERSKILLID_BellatraBronze					= 0x096A0063,
	TIMERSKILLID_BellatraSoloGold				= 0x096A0064,
	TIMERSKILLID_BellatraSoloSilver				= 0x096A0065,
	TIMERSKILLID_BellatraSoloBronze				= 0x096A0066,
	TIMERSKILLID_ChasingHunt					= 0x50020003,
	TIMERSKILLID_Compulsion						= 0x01080002,
	TIMERSKILLID_RollDice						= 0x096B0004,

	TIMERSKILLID_VirtualRegen					= 0x096B0005,
	TIMERSKILLID_VirtualLife					= 0x096B0006,
};

enum class ETimerID : int
{
	BellatraGold = 0x096A0061,
	BellatraSilver,
	BellatraBronze,
	BellatraSoloGold,
	BellatraSoloSilver,
	BellatraSoloBronze,

	//skill timers
	HallOfValhalla = TIMERSKILLBASE + 1,
	TriumphOfValhalla,
	VirtualRegen,
	VirtualLife,
	DemonsBane,
	Alas,
	Inpes,
	ForceofNature,
	EnchantWeapon,
	AdventMigal,
	HolyValor,
	HolyBody,
	DrasticSpirit,
	VenomThorn,
	ElectricalOverload
};

class CTimerSkill
{
protected:
	DWORD										dwChasingHuntTimerTexture = 0;

private:
	void										WritePointer( DWORD Addr, void * ptr );
	const char									* pszTimerDescription	= (char*)0x036762A0;
	const char									* pszTimerValue			= (char*)0x03677628;
public:
	CTimerSkill();
	virtual ~CTimerSkill();

	BOOL										KillTimer( DWORD dwCode );
	BOOL										KillTimer( ETimerID dwCode );
	BOOL										ZeroOutTimer ( DWORD dwCode );
	BOOL										KillTimerQuest( DWORD dwCode, int iQuestID );

	void										PrintAllSkillTimersToChat( );
	DWORD										CreateTextureMaterial( const char * pszFileName );

	BOOL										AddOrUpdateSkillTimer(  ESkillID eSkillId, ETimerID eTimerId, ESkillArrayPointer eDurationArrayPointer, int iTimeLeft, int iLevel1Based, int iParameter = 0 );
	BOOL										AddSkillTimerOverride( struct Skill * psSkill );
	BOOL										AddSkillTimer( struct Skill * psSkill );
	BOOL										SearchSkillDoc( struct Skill * psSkill );

	BOOL										IsSkillTimerActive ( ETimerID dwCode );
	BOOL										IsSkillTimerActive ( DWORD dwCode );
	struct Skill								* FindSkillTimer( ETimerID dwCode );
	struct Skill								* FindSkillTimer( DWORD dwCode );
	int											FindSkillTimerLevel( DWORD eTimerId );
	int											FindSkillTimerIntValue( ETimerID eTimerId, ESkillArrayPointer eSkillArrayPointer );
	int											FindSkillTimerIntValue( DWORD dwCode, ESkillArrayPointer eSkillArrayPointer );
	struct Skill								* GetPointerSkillTimer();

	int											OnRenderTimer( Skill * psSkill );

	int											GetArraySize();

	BOOL										OnAddChasingHunt( int iTime, int iLevel );

	void										PacketHandlerCrystal( struct PacketCrystalUse * psPacket );
	void										PacketHandlerCrystal( struct PacketCrystalUseDie * psPacket );

	void										OnCharacterKilled();

	void										DrawTimer();
	void										DrawTimerBox();

	void										SwapSkillTimer( Skill * psSkill );

	void										Tick1s();
	void										Loop();
};

