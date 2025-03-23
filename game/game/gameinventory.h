#pragma once

#define AGEPACKET_SENDING		(*(BOOL*)0x039C8710)
#define NOTCHANGE_ITEM			(*(BOOL*)0x035EB6C4)
#define SHOWSPECIAL_ITEM		(*(BOOL*)0x035EB6C0)

#define ARROWINVENTORY_LEFT		(*(BOOL*)0x035EB664)
#define ARROWINVENTORY_RIGHT	(*(BOOL*)0x035EB668)

#define INVENTORY_ARROW_POSITION (*(int*)0x35EB66C)  // see InvenArrowPosi in leaked source, INVENTORY_ARROW_POS_ definitions below

#define INVENTORY_DATACOUNT			100
#define INVENTORYSLOT_WHEEL			97
#define INVENTORYSLOT_WHEELCOUNT	99

#define INVENTORY_ARROW_POS_COSTUME				10 //Open costume
#define INVENTORY_ARROW_POS_OPENSHOP			9  //Shop button
#define INVENTORY_ARROW_POS_OPENQUEST			8  //Open quest
#define INVENTORY_ARROW_POS_MONEY				7  //Throw away money or transfer to warehouse
#define INVENTORY_ARROW_POS_CLOSE_BTN			5  //Close inventory window
#define INVENTORY_ARROW_POS_EQUIP2				4
#define INVENTORY_ARROW_POS_EQUIP1				3
#define INVENTORY_ARROW_POS_PAGE2				2
#define INVENTORY_ARROW_POS_PAGE1				1

class CGameInventory
{
protected:
	IMPFNC						pfnCheckMousePotionCount	= 0x004B08B0;
	IMPFNC						pfnOnMouseLClickDown		= 0x004C5940;
	IMPFNC						pfnGetInventoryTwoHandItem	= 0x004BA060;
	IMPFNC						pfnLoadItemImageEXPGold		= 0x004B0D60;
	IMPFNC						pfnSetInventoryItemItem		= 0x004BB6D0;
	IMPFNC						pfnResetPotion				= 0x00461250;

public:
	BOOL						bOpen;
	ItemData					saInventoryItems[INVENTORY_DATACOUNT];
	ItemData					saInventoryItemTemp[INVENTORY_DATACOUNT];
	ItemData					saBackupInventoryItemTemp[INVENTORY_DATACOUNT];

								CGameInventory();
							   ~CGameInventory();

	static CGameInventory		* GetInstance() { return (CGameInventory*)0x035EBB20; }


	int							CheckMousePotionCount();

	BOOL						OnMouseClick( class CMouse* pcMouse );
	void						OnMouseLDownClick( int iX, int iY );

	BOOL						DeleteItem( int itemCode );

	BOOL						CanChangeSpecialItem( BOOL b = FALSE );

	BOOL						ChangeInventorySide( int iType );

	int							GetInventoryPersonalShopItemIndex( ItemData * pcItemData );
	ItemData *					GetInventoryPersonalShopItem( ItemData * pcItemData );

	BOOL						GetInventoryItem( int iX, int iY, BOOL bPickup );
	BOOL						GetInventoryTwoHandItem( ItemData * pcItem );

	BOOL						SetInventoryItem( ItemData * psItem );
	BOOL						SetInventoryItem( Item * psItem );

	BOOL						LoadItemImageEXPGold( ItemData * pcItemData );

	static BOOL					ResetPotion();
};

