#pragma once
#include "Logger.h"


#define UNITDATA				(UNITGAME->pcUnitData)
#define UNIT					(UNITGAME->pcUnit)
#define UNITDATABYID(id)		(UNITGAME->GetUnitDataByID(id))
#define UNITDATATOUNIT(ud)		(UNITGAME->UnitDataToUnit(ud))

#define UNITGAME				UnitGame::GetInstance()

class UnitGame
{
private:
	static UnitGame			  * pcInstance;

	static int					iSpeedHack;
	static DWORD				dwUpdatePacket;

	int							iLastObjectID = 0;
	UnitData				  * pcViewUnitData;

	int							iLastUnitDataViewItemID = 0;

	int							iParticleAura1st;
	int							iParticleAura2nd;
	int							iParticleAura3rd;
	int							iParticleAuraOverall;

	CLogger						* pcLogger = nullptr;
	CLogger						* pcLoggerCSV = nullptr;

#ifdef ITEMFIXES
	void						GetAffixStats (ItemFixData * eItemFixData, ItemData eItemData);
#endif
	static Unit					cViewUnit;


	Unit						* pcaUnit;

	IMPFNC						pfnGetUnitData = 0x0061EF70;
	IMPFNC						pfnUpdateObjectID = 0x00620F80;
	IMPFNC						pfnAddCharacterEXP = 0x00460830;

	std::mutex	*				cLogMutex;

public:
	static void					CreateInstance() { pcInstance = new UnitGame(); }
	static UnitGame			  * GetInstance() { return pcInstance; }
	static void					DeleteInstance() { delete pcInstance; }

	static bool					bNewStats;

	//static int					iMainStatModifier;
	//static int					iLowestEfficiency;
	//static int					iMaxEfficiency;
	//static int					iStatsToMaxEfficiency;
	//static int					iLevelOverride;

	int							iLegendaryDropParticle;
	int							iEpicDropParticle;
	int							iRareDropParticle;

	UnitData				  * pcaUnitData;
	UnitData				  * pcUnitData;

	Unit					  * pcUnit;

	int							iBellatraRoomInitialPlayerCount = 0;
	int							iBellatraSoloCrownRanking = 0;

	
	void						LogCSV(const std::string& sMessage);
	void						Log(const std::string& sMessage);
	void						LogCSV(const char* format, ...);
	void						Log(const char* format, ...);
	void						FlushLog();

	//Constructor
								UnitGame();
	virtual					   ~UnitGame();

	UnitData				  * GetUnitDataByID( ID lID );

	Unit					  * UnitDataToUnit( UnitData * pcUnitData );

	void						DrawLifeBar( int iX, int iY );

	BOOL						SetCharacterHeadModel( const char * pszHeadModel, BOOL bEffect = FALSE );

	void						ClearFarUnits();

	BOOL						IsWeaponEquippedOnSelf( EItemType eItemType );
	BOOL						IsShieldEquippedOnSelf();
	BOOL						IsOrbEquippedOnSelf();

	void						ReceiveServerInformation( PacketServerInfo * psPacket );
	
	static void					AddEXP( INT64 iEXP );
	INT64						GetEXPFromLevel( int iLevel );
	BOOL						OnLoseEXP();

	int							GetCharacterAbsorption();

	void						SetCharacterFormula( ItemData * pItem, InventoryItemData * pInventory, int * iLevelDamage, int * iSkillDamage, int * iAttackDamage );

	int							PlayerAttackChance( int iMonsterDefenseRating );
	int							GetStatEfficiency (CharacterData* playerData, int iStat, int iCharStatAlloc);

	void						RenderDebugText();
	void						RenderCharacterDebugText();

	void						SendUnitDataEx(BOOL bForceUpdate = FALSE);

	static void					UpdateObjectID( ID lID );

	BOOL						RenderItemMotionBlur( UnitData * pcUnitData, UnitTool * psHand );

	BOOL						IsBlockedRangeTarget();
	BOOL						SetPVPArea( UnitData * pcUnitData );

	BOOL						Fun();

	void						MoveUnit( UnitData * pcUnitData, int iSpeed );

	void						HandlePacket( PacketSaveData * psPacket );

	void						HandlePacketCommand ( PacketCommand * psPacket );

	void						SetPlayerRankingAura ( Unit * pcUnit, short sAuraRankingId );

	void						HandlePacket( PacketUserEffect * psPacket );
	void						HandlePacketUnitStatus( UnitData * pcUnitData, Packet * psPacket );
	void						HandlePacketUnitBuff( UnitData * pcUnitData, PacketBuffData * psPacket );

	void						LoadParticles();

	int							GetFramesSendCount();

};