#pragma once

#include "roll.h"
#include "UIEvent.h"

#define MESSAGEBOX_ITEMBOX				((NPCItemBoxMessageBox*)0x0362AAC0)

enum ETypeItemUse
{
	HAIRSTYLETYPEID_None,
	HAIRSTYLETYPEID_MaleOnly,
	HAIRSTYLETYPEID_FemaleOnly,
};

enum EItemCheckInventory
{
	ITEMCHECKINVENTORY_None,
	ITEMCHECKINVENTORY_Potion,
	ITEMCHECKINVENTORY_Special,
	ITEMCHECKINVENTORY_Requirements,
};

struct sHairPotion
{
	DWORD										dwCode;
	ETypeItemUse								eUseType;
	const char									* pszFile;

};

class ItemHandler
{
private:
	ModelAnimation								* psaModelDataDefault[10];

	ItemData									sEventItemData;

	int											iUniqueShopID = 0;
	int											iNPCID = 0;


public:
												ItemHandler();
	virtual										~ItemHandler();

	class ItemData *							FindItemTable( UINT uPosition );
	class ItemData *							FindItemTableByCode( DWORD dwCode );
	class ItemData *							GetMouseItemHover();
	class ItemData *							GetItemEquippedByType( enum EItemType eType );
	int 										GetItemLevelRequirementById( DWORD dwItemID );
	class ItemData *							GetCurrentQuestWeapon( );
	class ItemData *							GetItemInventoryByType( EItemType eType );
	class ItemData *							GetItemInventoryByCode( EItemID eID );
	int											RemoveItemsByCode( EItemID eID, EItemCraftType eCraft, int iTotalToRemove );
	int											GetItemAmountByCode( EItemID eID, EItemCraftType eCraft = ITEMCRAFTTYPE_None );
	char *										GetArmorPatternName( class UnitData * pcUnit, DWORD dwCode );
	int											GetItemAgingPrice(struct Item* pcItem, int p_CurrentAgeLevel);
	BOOL										GetItemPrice( class ItemData * pcItemData, PriceItem * psSellData );
	BOOL										OnQuestWeaponMatured( ItemData * pcItemData );

	static void									DropItemToMap( ItemData* pcItemData );

	void										OnConfirmPremiumUsage ( ItemData pcItemData );
	void										HandlePremiumConfirmation ( bool bAccepted, ItemData pcItemData );

	static BOOL									UseItem( Item * psItem );

	BOOL										DeleteCurrentItemWeapon();
	BOOL										ReplaceCurrentQuestWeapon( Item * pcItem );
	int											DeleteItemQuest( EItemID eID, int iCount = 0 );
	void										DeleteItem( ItemData * psItemData );

	void										SetCharacterItem( EItemID iItemID, int iPosition, BOOL bActive );

	void										CheckPuzzleItemEvent();

	void										CheckRequirements();

	BOOL										LoadItemImage( Item * psIn, ItemData * psOut );

	void										OnLoadArmorCharacter();

	void										CheckDamageItems();
	void										UpdateItemCharacterStatus( BOOL bHackCheck = FALSE );

	int											OnItemPivot( int iPosition, UINT uItemID );

	BOOL										OnSetArmorUnit( class UnitData * psUnit, UINT uItemCode );
	BOOL										OnSetArmorTargetUnit( class UnitData * psUnit, UINT uPosition );

	ECharacterGender							GetItemGender( int iItemID );
	const char *								GetItemLevelWithLetter ( ItemData* psItemData );

	BOOL										CanEquipItem( void * pcClassItemData, ItemData * pcItemData );

	BOOL										EquipItem( UnitData * pcUnitData, EItemID iItemID, int iPositionID );
	BOOL										EquipBody( UnitData * pcUnitData, int iPositionID );

	EItemID 									GetHairPotionIDFromPath( char* path );
	BOOL										SetHairModel( DWORD dwCode, int iRank );
	char *										GetHeadModel( DWORD dwCode, int iRank );

	BOOL										OnHoverItem( DWORD dwCode );
	char *										OnDescriptionItem( DWORD dwCode );
	void										ValidateInventoryItems();
	void										ResetInventoryItemChecksum();
	void										ResetItemMouse();
	void										HandleQuestFinish( EQuestID eQuestID );
	void										HandleQuestItem( Item * psItem );
	void										AutoItemToInventory( Item * psItem );
	BOOL									    AutoSetPotion( ItemData * psItem );
	BOOL										CheckRequireItemToSet( ItemData * psItem );
	BOOL										BackItemToInventory( ItemData * psItem, EItemCheckInventory eCheck = ITEMCHECKINVENTORY_None );


	void										ShowItemInfo( ItemData * pItem, BOOL bBottom = TRUE );
	ItemData									* GetMouseItem() { return (ItemData*)0x036283D8; };
	void										DrawColorBox( DWORD Color, int iX, int iY, int iW, int iH );
	void										DrawSprite( int winX, int winY, int lpdds, int x, int y, int width, int height );
	bool										DelayCristal();
	BOOL										OnSendUpgradeAgingItem( ItemData * psItem );
	BOOL										CheckQuestItemAging( Item * psItemInfo );
	BOOL										OnMatureStone( DWORD dwCode );
	BOOL										OnSkillStone( DWORD dwCode );

	void										OnAcceptRollDiceItem();

	BOOL										OnCheckCanItemDrill( ItemData * pcItemData, int iUnk );

	BOOL										OnRenderRightWeapon( UnitData * pcUnitData, EXEModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix );
	BOOL										OnRenderSecondWeapon( UnitData * pcUnitData, EXEModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix, BOOL bRight );

	int											OnUseItem( ItemData * pcItemData );

	void										ProcessItemSlotAction( ESlotTypeHandle iAction, ESlotTypeHandleWhere iWhere, int iItemID, int iChk1, int iChk2 );

	void										SendGoldUse( int iGold );
	void										ProcessGold();

	void										FormatDropItemGold( DropItemView * ps );
	BOOL										SameSpecFilterDropItem( DropItemView * ps );

	void										HandlePacket( PacketNPCItemBox * psPacket );

	void										HandlePacket( PacketItemShopList * psPacket );

	void										LoadDefaultModels();

	ModelAnimation								* GetModelDefault( int iClass );

	BOOL										BuyItemShop( ItemData * pcItemData, int iCount );
};

