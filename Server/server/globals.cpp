#include "stdafx.h"
#include "globals.h"

int								iEventExpGlobal;
int								iServerCodeGlobal;
int								iRateExp;
BOOL							bFreeTradeChat;
BOOL							bHalloweenEvent;
int								iRateHalloweenMonster;
BOOL							bChristmasEvent;
BOOL							bMimicEvent;
int								iSantaId;
int								iSantaMightyId;
int								iSantaSuperId;
int								iHuntEventMaxCount;
int								iHuntEventSpawnDelay;
int								iHuntEventSpawnDistance = DISTANCE_XY_41_meters;
int								iRateWolfMonster;
BOOL							bEasterEvent;
int								iExtraDropsEvent = 0;
BOOL							bStarWarsEvent;
BOOL							bBeeEvent;
BOOL							bEventGirlFree;
int								iBlessCastleMode;

BOOL							bAgingFreeEvent;
BOOL							bAgingNoBreakEvent;
BOOL							bAgingHalfPriceEvent;

//FURY ARENA
BOOL							bFuryArenaEnabled;

//See https://docs.google.com/spreadsheets/d/1GNWePBhgcB1YX1n9pny5WlQk2y5lnsa2jJ8JCBAWMMA/edit#gid=1488786183

int								iFuryArenaKingFuryExpInitial	   = 200'000'000;
int								iFuryArenaKingFuryGoldInitial      = 0;
int								iFuryArenaLordBabelExpInitial	   = 60'000'000;
int								iFuryArenaLordBabelGoldInitial     = 0;

int								iFuryArenaKingFuryExpMinimum		= 100'000'000;
int								iFuryArenaLordBabelExpMinimum		= 20'000'000;

int								iFuryArenaKingFuryExpLossPerTick   = 10'000'000;
int								iFuryArenaKingFuryGoldLossPerTick  = 0;
int								iFuryArenaLordBabelExpLossPerTick  = 4'000'000;
int								iFuryArenaLordBabelGoldLossPerTick = 0;

int								iFuryArenaKingFuryGoldPerMinion    = 50'000;
int								iFuryArenaLordBabelGoldPerMinion   = 20'000;

int								iFuryArenaKingFuryMeteorDamageMin  = 250;
int								iFuryArenaKingFuryMeteorDamageMax  = 400;
int								iFuryArenaLordBabelMeteorDamageMin = 150;
int								iFuryArenaLordBabelMeteorDamageMax = 250;

BOOL							bWantedMoriphHuntEvent;
BOOL							bWantedWolfHuntEvent;

int								iBellatraRoomScoreGoldScale;
int								iBellatraPerformanceGoldScale;
int								iBellatraMaintenanceTax = 0;
int								iBellatraSecondClanTax = 0;
int								iBellatraThirdClanTax = 0;


BOOL							bValantinesDayEvent = FALSE;

// Gold Limits Configuration
int								iGoldLimitLowLevel = 1'000'000;				// Level <= 10
int								iGoldLimitMidLevelMultiplier = 200'000;		// Level < 20 (level * multiplier)
int								iGoldLimitHighLevel = 500'000'000;			// Level >= 20

SYSTEMTIME						sLocalTimeServer;
