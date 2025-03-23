#pragma once

#include "FuryArenaHandler.h"
#include "BlessCastleHandler.h"
#include "HalloweenHandler.h"
#include "BotServerHandler.h"
#include "CoinShopHandler.h"
#include "eventserver.h"
#include "ChristmasHandler.h"
#include "TradeHandler.h"
#include "DamageHandler.h"
#include "AgeHandler.h"
#include "MixHandler.h"
#include "QuestArenaHandler.h"
#include "CPartyHandler.h"
#include "CRollDiceHandler.h"
#include "CWarehouseHandler.h"
#include "CManufactureHandler.h"
#include "EventGirlHandler.h"
#include "RankingListHandler.h"
#include "EasterHandler.h"
#include "TestMapHandler.h"
#include "CPetSystemHandler.h"

#define FURYARENAHANDLER				(SERVERHANDLER->GetFuryArenaHandler())
#define BLESSCASTLEHANDLER				(SERVERHANDLER->GetBlessCastleHandler())
#define HALLOWEENHANDLER				(SERVERHANDLER->GetHalloweenHandler())
#define CHRISTMASHANDLER				(SERVERHANDLER->GetChristmasHandler())
#define GENERALEVENTHANDLER				(SERVERHANDLER->GetGeneralEventHandler())
#define BOTSERVER						(SERVERHANDLER->GetBotServerHandler())
#define COINSHOPHANDLER					(SERVERHANDLER->GetCoinShopHandler())
#define EVENTSERVER						(SERVERHANDLER->GetEventServerHandler())
#define TRADEHANDLER					(SERVERHANDLER->GetTradeHandler())
#define DAMAGEHANDLER					(SERVERHANDLER->GetDamageHandler())
#define AGEHANDLER						(SERVERHANDLER->GetAgeHandler())
#define MIXHANDLER						(SERVERHANDLER->GetMixHandler())
#define QUESTARENAHANDLER				(SERVERHANDLER->GetQuestArenaHandler())
#define PARTYHANDLER					(SERVERHANDLER->GetPartyHandler())
#define ROLLDICEHANDLER					(SERVERHANDLER->GetRollDiceHandler())
#define WAREHOUSEHANDLER				(SERVERHANDLER->GetWarehouseHandler())
#define MANUFACTUREHANDLER				(SERVERHANDLER->GetManufactureHandler())
#define EVENTGIRLAHANDLER				(SERVERHANDLER->GetEventGirlHandler())
#define RANKINGLISTHANDLER				(SERVERHANDLER->GetRankingListHandler())
#define TESTMAPHANDLER					(SERVERHANDLER->GetTestMapHandler())
#define PETSYSTEMHANDLER				(SERVERHANDLER->GetPetSystemHandler())

class CServerHandler
{
private:
	CFuryArenaHandler			* pcFuryArenaHandler;
	CBlessCastleHandler			* pcBlessCastleHandler;
	CHalloweenHandler			* pcHalloweenHandler;
	CChristmasHandler			* pcChristmasHandler;
	CGeneralEventHandler		* pcGeneralEventHandler;
	CBotServerHandler			* pcBotServerHandler;
	CCoinShopHandler			* pcCoinShopHandler;
	CEventServer				* pcEventServerHandler;
	CTradeHandler				* pcTradeHandler;
	CDamageHandler				* pcDamageHandler;
	CAgeHandler					* pcAgeHandler;
	CMixHandler					* pcMixHandler;
	CQuestArenaHandler			* pcQuestArenaHandler;
	CPartyHandler				* pcPartyHandler;
	CRollDiceHandler			* pcRollDiceHandler;
	CWarehouseHandler			* pcWarehouseHandler;
	CManufactureHandler			* pcManufactureHandler;
	CEventGirlHandler			* pcEventGirlHandler;
	CRankingListHandler			* pcRankingListHandler;
	CTestMapHandler			    * pcTestMapHandler;
	CPetSystemHandler			* pcPetSystemHandler;

public:
						  CServerHandler();
	virtual				 ~CServerHandler();

	BOOL				  Init();
	void				  Shutdown();

	void				  Update();

	CFuryArenaHandler			* GetFuryArenaHandler() { return pcFuryArenaHandler; }
	CBlessCastleHandler			* GetBlessCastleHandler() { return pcBlessCastleHandler; }
	CHalloweenHandler			* GetHalloweenHandler() { return pcHalloweenHandler; }
	CChristmasHandler			* GetChristmasHandler() { return pcChristmasHandler; }
	CGeneralEventHandler		* GetGeneralEventHandler() { return pcGeneralEventHandler; }
	CBotServerHandler			* GetBotServerHandler() { return pcBotServerHandler; }
	CCoinShopHandler			* GetCoinShopHandler() { return pcCoinShopHandler; }
	CEventServer				* GetEventServerHandler() { return pcEventServerHandler; }
	CTradeHandler				* GetTradeHandler() { return pcTradeHandler; }
	CDamageHandler				* GetDamageHandler() { return pcDamageHandler; }
	CAgeHandler					* GetAgeHandler() { return pcAgeHandler; }
	CMixHandler					* GetMixHandler() { return pcMixHandler; }
	CQuestArenaHandler			* GetQuestArenaHandler() { return pcQuestArenaHandler; }
	CPartyHandler				* GetPartyHandler() { return pcPartyHandler; }
	CRollDiceHandler			* GetRollDiceHandler() { return pcRollDiceHandler; }
	CWarehouseHandler			* GetWarehouseHandler() { return pcWarehouseHandler; }
	CManufactureHandler			* GetManufactureHandler() { return pcManufactureHandler; }
	CEventGirlHandler			* GetEventGirlHandler() { return pcEventGirlHandler; }
	CRankingListHandler			* GetRankingListHandler() { return pcRankingListHandler; }
	CTestMapHandler				* GetTestMapHandler() { return pcTestMapHandler; }
	CPetSystemHandler			* GetPetSystemHandler() { return pcPetSystemHandler; }

private:


};