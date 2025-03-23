#include "stdafx.h"
#include "CServerHandler.h"

#include "CServer.h"

extern void		HNSSkill_Tick1s ();

CServerHandler::CServerHandler()
{
	pcFuryArenaHandler			= new CFuryArenaHandler();
	pcBlessCastleHandler		= new CBlessCastleHandler();
	pcHalloweenHandler			= new CHalloweenHandler();
	pcChristmasHandler			= new CChristmasHandler();
	pcGeneralEventHandler		= new CGeneralEventHandler();
	pcBotServerHandler			= new CBotServerHandler();
	pcCoinShopHandler			= new CCoinShopHandler();
	pcEventServerHandler		= new CEventServer();
	pcTradeHandler				= new CTradeHandler();
	pcDamageHandler				= new CDamageHandler();
	pcAgeHandler				= new CAgeHandler();
	pcMixHandler				= new CMixHandler();
	pcQuestArenaHandler			= new CQuestArenaHandler();
	pcPartyHandler				= new CPartyHandler();
	pcRollDiceHandler			= new CRollDiceHandler();
	pcWarehouseHandler			= new CWarehouseHandler();
	pcManufactureHandler		= new CManufactureHandler();
	pcEventGirlHandler          = new CEventGirlHandler();
	pcRankingListHandler        = new CRankingListHandler();
	pcTestMapHandler			= new CTestMapHandler();
}

CServerHandler::~CServerHandler()
{
	SAFE_DELETE( pcFuryArenaHandler );
	SAFE_DELETE( pcBlessCastleHandler );
	SAFE_DELETE( pcHalloweenHandler );
	SAFE_DELETE( pcChristmasHandler );
	SAFE_DELETE( pcGeneralEventHandler );
	SAFE_DELETE( pcBotServerHandler );
	SAFE_DELETE( pcCoinShopHandler );
	SAFE_DELETE( pcEventServerHandler );
	SAFE_DELETE( pcTradeHandler );
	SAFE_DELETE( pcDamageHandler );
	SAFE_DELETE( pcAgeHandler );
	SAFE_DELETE( pcMixHandler );
	SAFE_DELETE( pcQuestArenaHandler );
	SAFE_DELETE( pcPartyHandler );
	SAFE_DELETE( pcRollDiceHandler );
	SAFE_DELETE( pcWarehouseHandler );
	SAFE_DELETE( pcManufactureHandler );
	SAFE_DELETE( pcEventGirlHandler );
	SAFE_DELETE( pcRankingListHandler );
	SAFE_DELETE( pcTestMapHandler );
}

BOOL CServerHandler::Init()
{
	pcCoinShopHandler->Init();
	pcFuryArenaHandler->Init();
	pcEventServerHandler->Init ();

	return TRUE;
}

void CServerHandler::Shutdown()
{
}

void CServerHandler::Update()
{
	pcQuestArenaHandler->Update();
	pcEventServerHandler->Tick();

	HNSSkill_Tick1s ();
}
