#pragma once

#define PKT_DATAUNIT_SZ 8096

//Packet Header
struct SPlayDataGroup : Packet
{
	int		iDataCount;
	DWORD	dwCodeMask;
};

//Functions in ASM
enum class ENPCFunction
{
	SEND_SHOP_ITEM_LIST			 = 0x00551290,
	SEND_SKILL_MENU				 = 0x00551500,
	SEND_OPEN_WAREHOUSE			 = 0x00551580,
	SEND_OPEN_MIX_ITEM			 = 0x005515C0,
	SEND_OPEN_EVENT				 = 0x00559B90,
	SEND_OPEN_BLESSCASTLE_INFO	 = 0x00557E80,
	SEND_QUEST_PROGRESSION		 = 0x0055AF20,
};

class UnitServer
{
private:
	IMPFNC										pfnGetTop10DamageUnitData	= 0x0055A3D0;
	IMPFNC										pfnUnitSwapper				= 0x0054FEA0;
	IMPFNC										pfnUnitDamageSkill			= 0x0054FAB0;
	IMPFNC										pfnUnitDataByIDMap			= 0x0054CCA0;


	UnitData									* pcaUnitData		= NULL;
	Unit										* pcaUnit			= NULL;

	std::map<int, int>							mapMonsterToHP;

protected:
	struct PacketRankingList* psPacketRankList = NULL; //xxstr rank list
	struct PacketMixGuild* psPacketMixGuild = NULL; //xxstr  mix list

public:

	//Functions related to NPC clicks in Server.exe
	enum NPC_FUNCTIONS
	{

	};


	Unit										** pcaUnitInGame = NULL;
	UINT										uUnitsInGame = 0;

	UnitServer();

	UnitData *									GetUnit( UINT uEnemyID );
	UnitData *									GetNPCUnit( UINT uNPCID, int iMapID = -1 );

	static UnitData								* UnitDataByIDMap( int iID, int iMapID );
	Unit										* UnitDataToUnit( UnitData * pcUnitData );

	struct CharacterData						* GetCharacterDataByEffect( EMonsterEffectID dwEffect, EMonsterClass iClass = EMonsterClass::Normal, int iMonsterLevel = 0 );
	struct CharacterData						* GetCharacterDataByName( const char * pszName );

	struct UnitInfo								* GetUnitInfoByName( const char * pszName );

	int											GetMonsterHealthPointDefinition( int monsterId );
	void										AddOrUpdateMonsterHealthPointDefinition( int iMonsterId, int iHealthValue );


	int											GetArrayPosition( UnitData * pcUnitData );

	INT64										GetExpLevelDiference( int iLevel1, int iLevel2, INT64 iExp );
	INT64										GetExpLevelDiferenceVsPlayer( int iLevel1, int iLevel2, INT64 iExp );

	INT64										GetExp( UnitData * pcUnitData );
	BOOL										SetExp( UnitData * pcUnitData, INT64 iNewExp );

	BOOL										SetMotionFromCode( UnitData * pcUnitData, int iMotionCode );
	void										SendBossMessage( BaseMap * pcBaseMap, UINT pUnit );

	void										HandleNPCClick( User * pcUser, PacketNPCClick * pPacket );

	BOOL										OnUnitNpcClick( User * pcUserData, UnitData * pcUnitData );
	void										OnSendExp( UserData * pcUserData, UnitData * pcUnitData, int iKillerUserDataId );
	void										OnSetDrop( UserData * pcUserData, UnitData * pcUnitData );

	void										OnSetTargetPosition( UserData * pcUserData, UnitData * pcUnitData );

	UnitData *									CreateUnitEnemy( char * pszName, int iX, int iY, int iZ, UserData * pcUserData = NULL );

	void										OnMonsterAddedToMap( Map * pcMap, UnitData * pcUnitData );

	BOOL										IsMultiplyHPMonter( UnitData * pcUnitData );

	static BOOL									GetTop10Damage( UnitData * pcUnitData, AttackDamageData* psaDamageData );

	BOOL										HandleKill( UnitData * pcUnitData, UserData * pcUserData, int iKillerUserDataId );

	void										HandleBossQuestKill( UnitData * pcUnitData, int iKillerUserDataId );

	void										MainUnitData( UnitData * pcUnitData );

	static void									SkillUnitDamage( UnitData * pcUnitData, UserData * pcUserData );

	static void									UnitSwapper( UnitData * pcUnitData, CharacterData * psCharacterData, UnitInfo * psUnitInfo );

	BOOL										OnUnitChanger( UnitData * pcUnitData );

	BOOL										UpdateUnitData( UnitData * pcUnitData );

	BOOL										UpdateUnitData_AntiStuck( Unit* pcUnit, UnitData* pcUnitData );

	void										UpdateUnit( Unit * pcUnit );

	void										Update();

	int											IsFreezedUnitFollow( UnitData * pcUnitData );

	BOOL										MainServerUnitData( UnitData * pcUnitData );

	AttackDamageData *							GetUserInDamageList( UnitData * pcUnitData, User * pcUser );


	void										ProcessPacketKillUnitID( User * pcUser, int iMonsterID, int iKillerID );

	void										OnHandleUnitDataBufferNew( Unit * pcUnit, PacketPlayData * psPacket );

	void										PlayBuffDataAddDebuff( Unit * pcUnit, PacketPlayData * psPacket );

	void										PlayBuffDataAddBuff( Unit * pcUnit, PacketPlayData * psPacket );

	void										UpdateCharacterData( User * pcUser, UnitData * pcUnitData );

private:
	BOOL									IsStuckPreventionBossUnit( UnitData * pcUnitData );

	BOOL									MainUnitMonsterData( UnitData * pcUnitData );

};

