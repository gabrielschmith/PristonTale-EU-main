#pragma once
#include "CoinShopWindow.h"

class CCoinHandler
{
protected:
	CCoinShopWindow										* pcCoinShopWindow = NULL;

	class CCoinShop										* pcCoinShop = NULL;

public:
	CCoinHandler();
	virtual ~CCoinHandler();

	CCoinShopWindow										* GetWindow() { return pcCoinShopWindow; };
	class CCoinShop										* GetCoinShop() { return pcCoinShop; }

	void												Init();

	void												Render();

	void												OpenCoinShopNPC();

	void												OnBuyCredits();

	void												ProcessPacket( struct PacketCoinShopNPC * psPacket );
	void												ProcessPacket( struct PacketCoinAmount * psPacket );
	void												ProcessPacket( struct PacketCoinOpenTab * psPacket );
	void												ProcessPacket( struct PacketCoinOpenItemData * psPacket );
	void												ProcessPacket( struct PacketCoinBuy * psPacket );
	void												ProcessPacket( struct PacketCoinServiceBuy * psPacket );

	void												HandlePacket( struct PacketCoinShopNPC * psPacket );
	void												HandlePacket( struct PacketCoinAmount * psPacket );
	void												HandlePacket( struct PacketCoinTab * psPacket );
	void												HandlePacket( struct PacketCoinItemData * psPacket );
	void												HandlePacket( struct PacketCoinBuy * psPacket );
	void												HandlePacket( struct PacketCoinOpenService * psPacket );
	void												HandlePacket( struct PacketCoinServiceBuy * psPacket );
	void												HandlePacket( struct PacketOpenAgingRecoveryService * psPacket );


};

