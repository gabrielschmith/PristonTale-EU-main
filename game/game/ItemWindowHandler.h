#pragma once

enum ETradeColorBox
{
	TRADECOLORBOX_None,
	TRADECOLORBOX_SetItem,
	TRADECOLORBOX_CollideItem,
	TRADECOLORBOX_NotSetItem,
	TRADECOLORBOX_CollideColor,
	TRADECOLORBOX_SelectItem,
	TRADECOLORBOX_NotUse,
	TRADECOLORBOX_NotAgingItem,
	TRADECOLORBOX_NotEnoughGold,

};

#define PERSONALSHOP_SENDING					(*(BOOL*)0x03653E18)
#define ITEMONMOUSE								(*(BOOL*)0x036284A4)
#define CRAFTPROCCESS_START						(*(BOOL*)0x037B7F08)
#define BRIGHTEFFECT_ITEM_PROCESSING			(*(BOOL*)0x037B7F04)
#define RESETMIXWINDOW_DELAY					(*(BOOL*)0x036EFD80)

class CItemWindowHandler
{
private:
	enum
	{
		UNSTACKPOTION_None,
		UNSTACKPOTION_WarehouseGet,
		UNSTACKPOTION_WarehousePut,
		UNSTACKPOTION_TradeGet,
		UNSTACKPOTION_TradePut,
		UNSTACKPOTION_FieldGet,
		UNSTACKPOTION_MouseGet,
	};

	IMPFNC										pfnGetAmountPersonalShop = 0x004E57F0;

	int											GetPersonamShopGoldAmount();

	void										OnOkButtonClickUnstackPotion( ItemData * psIn, ItemData * psOut, int iType );

	int											OnUnstackPotion( ItemData * psIn, ItemData * psOut, int iType );
	BOOL										OnWarehouseAutoPlaceItem( ItemData * psItemData );
	BOOL										OnTradeAutoPlaceItem( ItemData * psItemData );
	BOOL										OnBlacksmithAutoPlaceItem( ItemData * psItemData );
	BOOL										OnPersonalShopAutoPlaceItem( ItemData * psItemData );
	BOOL										OnWarehouseAutoPlaceItemBack( const Point2D * psPoint );
	BOOL										OnTradeAutoPlaceItemBack( const Point2D * psPoint );
public:
	BOOL										IsSellItem( ItemData * psItemData );
	BOOL										IsUseMixNPCItem( DWORD dwCode );
	BOOL										IsUseMixNPC( DWORD dwCode );
	BOOL										IsUseAgingNPC( ItemData * psItemData );

	BOOL										OnMouseClick( CMouse * pcMouse );

	int											CollideItemTrade( RECT * pRect, int iPassItemIndex = 0, int iSlotType = 0 );

												//This sets the trade color for TradeIndex
	ETradeColorBox								GetTradeItemColorBox() { return (*(ETradeColorBox*)0x036EFCA0); };
	void										SetTradeItemColorBox( ETradeColorBox eError ) { (*(ETradeColorBox*)0x036EFCA0) = eError; };
	void										SetTradeRect( RECT * psRect );

	BOOL										IsTradeUse( int iItemID );

	BOOL										IsWarehouseUse( ItemData * psItemData );
	BOOL										IsPersonalShopUse( ItemData * psItemData );

	BOOL										SetItemToNPC( ItemData * psItemData, BOOL bFlag );

	int											GetGoldPersonalShopLimit( int iGold );

	CItemWindowHandler();
	virtual ~CItemWindowHandler();
};

