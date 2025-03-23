#pragma once

static char * pszaNotAllowedNamesChangeNick[]
{
	"[gm]",
	"[adm]",
	"sandurr",
	"realmpt",
	"gm-",
	"adm-",
	"gamemaster"
};


class CCoinShopHandler
{
protected:
	class CCoinShop										* pcCoinShop = NULL;

	int													iChangeNickPrice = 0;
	int													iAgingRecoveryPrice = 0;

public:
	CCoinShopHandler();
	virtual ~CCoinShopHandler();

	void												SQLCoinLog( char * pszAccountName, char * pszDescription );

	void												LoadCoinShop( int iCoinShopID = -1 );

	void												Init();

	int													GetCoinAmount( char * pszAccountName );

	int													GetCoinTradeAmount( char * pszAccountName );

	int													GetBulkValue( CCoinShopItem * pcItem, int iQuantity );

	int													UseCoin( char * pszAccountName, int iCoinAmount );
	
	void												AddTradeCoin( char * pszAccountName, int iCoinAmount );
	int													AddCoin( char * pszAccountName, int iCoinAmount );
	int													HasCoinAmount( char * pszAccountName, int iCoinAmount );

	BOOL												PerfectizeItem( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ItemSwapper( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ChangeNick( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												AgingRecovery( User * pcUser, struct PacketCoinServiceBuy * psPacket );
	BOOL												ChangeClass( User * pcUser, struct PacketCoinServiceBuy * psPacket );

	void												ProcessPacket( User * pcUser, PacketCoinServiceBuy * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinShopNPC * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinAmount * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinOpenTab * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinTab * psPacket );
	void												ProcessPacket( User * pcUser, struct PacketCoinItemData * psPacket );

	void												HandlePacket( User * pcUser, struct PacketCoinShopNPC * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinOpenTab * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinAmount * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinOpenItemData * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinBuy * psPacket );
	void												HandlePacket( User * pcUser, struct PacketCoinServiceBuy * psPacket );

	void												HandlePacket( User * pcUser, struct PacketTradeCoinSet * psPacket );

};

