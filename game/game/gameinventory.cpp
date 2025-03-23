#include "stdafx.h"
#include "gameinventory.h"


CGameInventory::CGameInventory()
{
}


CGameInventory::~CGameInventory()
{
}

NAKED int CGameInventory::CheckMousePotionCount()
{
	JMP( pfnCheckMousePotionCount );
}

BOOL CGameInventory::OnMouseClick(class CMouse* pcMouse)
{
	int arrowPos = INVENTORY_ARROW_POSITION;

	if (WINDOW_ISOPEN_INVENTORY && arrowPos == INVENTORY_ARROW_POS_MONEY)
	{
		if ( WINDOW_ISOPEN_WAREHOUSE &&														  //warehouse is open
			 WAREHOUSEHANDLER->IsWithdrawalOnlyWarehouse () )							  //withdrawl only warehouse
		{
			return TRUE;
		}
	}

	return FALSE;
}

NAKED void CGameInventory::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

/// <summary>
/// int cINVENTORY::ChangeSpecialItem(int Flag)
/// </summary>
NAKED BOOL CGameInventory::CanChangeSpecialItem( BOOL b )
{
	JMP( 0x004B1130 );
}


BOOL CGameInventory::DeleteItem( int itemCode )
{
	ItemData * pItemData = NULL;
	BOOL found = FALSE;

	for ( int i = 0; i < INVENTORY_DATACOUNT * 2; i++ )
	{
		pItemData = &saInventoryItems[i];

		if ( i < 100 )
		{
			if ( !saInventoryItems[i].bValid )
				continue;

			pItemData = &saInventoryItems[i];
		}
		else
		{
			if ( !saInventoryItemTemp[i - 100].bValid )continue;
			pItemData = &saInventoryItemTemp[i - 100];
		}

		if ( pItemData->sItem.sItemID.ToInt() == itemCode )
		{
			found = TRUE;

			pItemData->bValid = 0;
			pItemData->sItem.iChk1 = 0;
			pItemData->sItem.iChk2 = 0;

			if ( i < 100 )
			{
				if ( pItemData->iItemSlot )
				{
					INVENTORYITEMSLOT[pItemData->iItemSlot - 1].iItemIndex = 0;
					if ( pItemData->iVisibleItemType )
						ITEMHANDLER->SetCharacterItem( pItemData->sBaseItemID.ToItemID(), pItemData->iVisibleItemType, FALSE );
				}
			}
		}
	}

	if ( found )
	{
		ITEMHANDLER->ValidateInventoryItems();		//cInvenTory.ReFormInvenItem();
		CHECK_CHARACTER_CHECKSUM;					//ReformCharForm();//Recertification
		CALLT( 0x004AE430, 0x035EBB20 );			//cInvenTory.ReFormPotionNum();
		ITEMHANDLER->UpdateItemCharacterStatus();	//cInvenTory.SetItemToChar();
		CHARACTERGAME->UpdateWeight();				//cInvenTory.CheckWeight();
	}

	return found;
}

//1 = first page
//2 = second page
//3 = weapon set 1
//4 = weapon set 2
BOOL CGameInventory::ChangeInventorySide( int iType )
{
	//Delay Time for change window
	WRITEDWORD( 0x035EB6E0, TRUE );

	if ( AGEPACKET_SENDING )
		return TRUE;
	if ( NOTCHANGE_ITEM )
		return TRUE;
	if ( PERSONALSHOP_SENDING )
		return TRUE;

	if ( CanChangeSpecialItem( TRUE ) )
		return TRUE;

	if ( WINDOW_ISOPEN_WAREHOUSE )
	{
	//	if ( ITEMONMOUSE )
	//		return TRUE;
	}

	//Tier Up Box Open?
	if ( READDWORD( 0x0095E418 ) )
		return TRUE;

	if ( SHOWSPECIAL_ITEM )
		return TRUE;

	//Teleport Core Open?
	if ( READDWORD( 0x0095F8E0 ) )
		return TRUE;

	if ( CRAFTPROCCESS_START || BRIGHTEFFECT_ITEM_PROCESSING )
		return TRUE;
	else
	{
		if ( (WINDOW_ISOPEN_CRAFTING || WINDOW_ISOPEN_SMELTING) && RESETMIXWINDOW_DELAY )
			return TRUE;
	}

	int iSlot = 1;

	if ( iType > 2 )
		iSlot = 2;

	if ( iType == 1 )
	{
		ARROWINVENTORY_LEFT = FALSE;
		PLAYERDATA->sViewBoxZoom.sMin = 1;
	}
	if ( iType == 2 )
	{
		ARROWINVENTORY_LEFT = TRUE;
		PLAYERDATA->sViewBoxZoom.sMin = 2;
	}
	if ( iType == 3 )
	{
		ARROWINVENTORY_RIGHT = FALSE;
		PLAYERDATA->sViewBoxZoom.sMax = 3;
	}
	if ( iType == 4 )
	{
		ARROWINVENTORY_RIGHT = TRUE;
		PLAYERDATA->sViewBoxZoom.sMax = 4;
	}

	if ( iSlot == 1 )
	{
		CopyMemory( &saBackupInventoryItemTemp, &saInventoryItemTemp, sizeof( ItemData ) * 100 );

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			saInventoryItemTemp[i].bValid = FALSE;
		}

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				if ( (pc->iItemSlot == 0) && (pc->sItem.sSpecialItemType[1] != 8) )
				{
					for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
					{
						//Fix
						if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
							continue;

						ItemData * pc2 = &saInventoryItemTemp[j];

						if ( pc2->bValid == FALSE )
						{
							CopyMemory( pc2, pc, sizeof( ItemData ) );
							pc->bValid = FALSE;
							break;
						}
					}
				}
			}
		}

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			//Fix
			if ( i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT )
				continue;

			ItemData * pc = &saBackupInventoryItemTemp[i];

			if ( pc->bValid )
			{
				for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
				{
					//Fix
					if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
						continue;

					ItemData * pc2 = &saInventoryItems[j];

					if ( pc2->bValid == FALSE )
					{
						CopyMemory( pc2, pc, sizeof( ItemData ) );
						pc->bValid = FALSE;
						break;
					}
				}
			}
		}
	}

	if ( iSlot == 2 )
	{
		//Skill
		WRITEDWORD( 0x03707138, TRUE );

		CopyMemory( &saBackupInventoryItemTemp[INVENTORYSLOT_WHEEL], &saInventoryItemTemp[INVENTORYSLOT_WHEEL], sizeof( ItemData ) * 2 );

		for ( int i = INVENTORYSLOT_WHEEL; i < INVENTORYSLOT_WHEELCOUNT; i++ )
		{
			saInventoryItemTemp[i].bValid = FALSE;
		}


		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				if ( (pc->iItemSlot == 1) || (pc->iItemSlot == 2) )
				{
					for ( int j = INVENTORYSLOT_WHEEL; j < INVENTORYSLOT_WHEELCOUNT; j++ )
					{
						ItemData * pc2 = &saInventoryItemTemp[j];

						if ( pc2->bValid == FALSE )
						{
							CopyMemory( pc2, pc, sizeof( ItemData ) );
							pc->bValid = FALSE;

							INVENTORYITEMSLOT[pc->iItemSlot - 1].iItemIndex = 0;

							if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin && pc->sBaseItemID.ToItemType() == ITEMTYPE_Dagger )
							{
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 2, FALSE );
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 4, FALSE );
							}
							else
								ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), pc->iVisibleItemType, FALSE );

							break;
						}
					}
				}
			}
		}

		for ( int i = INVENTORYSLOT_WHEEL; i < INVENTORYSLOT_WHEELCOUNT; i++ )
		{
			ItemData * pc = &saBackupInventoryItemTemp[i];

			if ( pc->bValid )
			{
				for ( int j = 0; j < INVENTORY_DATACOUNT; j++ )
				{
					//Fix
					if ( j >= INVENTORYSLOT_WHEEL && j < INVENTORYSLOT_WHEELCOUNT )
						continue;

					ItemData * pc2 = &saInventoryItems[j];

					if ( pc2->bValid == FALSE )
					{
						CopyMemory( pc2, pc, sizeof( ItemData ) );
						pc->bValid = FALSE;

						INVENTORYITEMSLOT[pc->iItemSlot - 1].iItemIndex = j + 1;

						if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin && pc->sBaseItemID.ToItemType() == ITEMTYPE_Dagger )
						{
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 2, TRUE );
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), 4, TRUE );
						}
						else
							ITEMHANDLER->SetCharacterItem( pc->sBaseItemID.ToItemID(), pc->iVisibleItemType, TRUE );

						if ( pc->iInvItemSound )
							PLAYMINISOUND( pc->iInvItemSound );

						break;
					}
				}
			}
		}

		ITEMHANDLER->UpdateItemCharacterStatus();
	}

	if ( WINDOW_ISOPEN_PERSONALSHOP_ME )
	{
		int * piaPersonalShopItemIndex = ((int*)0x03653C50);
		ZeroMemory( piaPersonalShopItemIndex, sizeof( int ) * 100 );

		ItemData * psaPersonalShopItemData = (ItemData*)0x0363C114;

		for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
		{
			ItemData * pc = &saInventoryItems[i];

			if ( pc->bValid )
			{
				for ( int j = 0; j < 30; j++ )
				{
					ItemData * pcItemShopData = psaPersonalShopItemData + j;
					if ( pcItemShopData->sItem.iChk1 != 0 &&
					   ( pcItemShopData->sItem.iChk1 == pc->sItem.iChk1) &&
					   ( pcItemShopData->sItem.iChk2 == pc->sItem.iChk2) )
					{
						piaPersonalShopItemIndex[i] = j;
						break;
					}
				}
			}
		}
	}

	ITEMHANDLER->CheckRequirements();
	ITEMHANDLER->CheckPuzzleItemEvent();

	return TRUE;
}

int CGameInventory::GetInventoryPersonalShopItemIndex( ItemData * pcItemData )
{
	int iIndex = 0;
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		ItemData * pc = saInventoryItems + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.iChk1 == pcItemData->sItem.iChk1) && (pc->sItem.iChk2 == pcItemData->sItem.iChk2) )
			{
				if ( pcItemData->sItem.iIndex == (i + 1) )
					iIndex = i + 1;
				break;
			}
		}

		pc = saInventoryItemTemp + i;
		if ( pc->bValid )
		{
			if ( (pc->sItem.iChk1 == pcItemData->sItem.iChk1) && (pc->sItem.iChk2 == pcItemData->sItem.iChk2) )
			{
				iIndex = i + 101;
				break;
			}
		}
	}

	return iIndex;
}

ItemData * CGameInventory::GetInventoryPersonalShopItem(ItemData* pcItemData)
{
	if (pcItemData->bValid == FALSE)
		return NULL;

	int iIndex = 0;
	for (int i = 0; i < INVENTORY_DATACOUNT; i++)
	{
		if (i >= INVENTORYSLOT_WHEEL && i < INVENTORYSLOT_WHEELCOUNT)
			continue;

		ItemData * pc = saInventoryItems + i;
		if (pc->bValid &&
		   (pc->sItem.iChk1 == pcItemData->sItem.iChk1) &&
		   (pc->sItem.iChk2 == pcItemData->sItem.iChk2))
		{
			return pc;
		}

		pc = saInventoryItemTemp + i;
		if (pc->bValid &&
			(pc->sItem.iChk1 == pcItemData->sItem.iChk1) &&
			(pc->sItem.iChk2 == pcItemData->sItem.iChk2))
		{
			return pc;
		}
	}

	return NULL;
}


BOOL CGameInventory::GetInventoryItem( int iX, int iY, BOOL bPickup )
{
	//Point2D sCurMouse = Point2D(MOUSEHANDLER->GetPosition()->iX, MOUSEHANDLER->GetPosition()->iY);
	Point2D sCurMouse = Point2D( iX, iY );

	WRITEDWORD( 0x35EB684, NULL );
	WRITEDWORD( 0x35EB65C, NULL );
	WRITEDWORD( 0x35EB660, NULL );

	for( size_t i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		ItemData * pcItem = saInventoryItems + i;

		if( pcItem )
		{
			if( pcItem->bValid )
			{
				if( sCurMouse.Inside( Rectangle2D( pcItem->sPosition.iX, pcItem->sPosition.iY, pcItem->iWidth, pcItem->iHeight ) ) )
				{
					if( pcItem->iItemSlot == 11 || pcItem->iItemSlot == 12 || pcItem->iItemSlot == 13 )
						return FALSE;

					if( bPickup )
					{
						/*if( MyShopItemIndex[i] )
							return FALSE;*/

						if( pcItem->iItemSlot )
						{
							/*if(sinChar->Weight[0] > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
							sinOverWeightCnt++;
							return FALSE;
							}*/
						}

						pcItem->sOldPosition.iX = pcItem->sPosition.iX;
						pcItem->sOldPosition.iY = pcItem->sPosition.iY;

						if( false )
						{

						}
						else
						{
							if( pcItem->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand )
								GetInventoryTwoHandItem( pcItem );

							CopyMemory( ITEMHANDLER->GetMouseItem(), pcItem, sizeof(ItemData) );
						}

						pcItem->bValid = FALSE;

						if( pcItem->iItemSlot )
						{
							if( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Assassin )
							{
								ITEMHANDLER->SetCharacterItem( pcItem->sBaseItemID.ToItemID(), ITEMSLOTFLAG_LeftHand, FALSE );
								ITEMHANDLER->SetCharacterItem( pcItem->sBaseItemID.ToItemID(), ITEMSLOTFLAG_RightHand, FALSE );
							}
							else
							{
								ITEMHANDLER->SetCharacterItem( pcItem->sBaseItemID.ToItemID(), pcItem->iVisibleItemType, FALSE );
							}
						}

						ITEMHANDLER->ValidateInventoryItems();
						CHARACTERGAME->UpdateWeight();
						ITEMHANDLER->UpdateItemCharacterStatus();
						WRITEDWORD( 0x35EB5F4, 0 );

						return TRUE;
					}
					else
					{
						WRITEDWORD( 0x35EB65C, i + 1 );
						WRITEDWORD( 0x35EB5F4, 5 );
						WRITEDWORD( 0x36286EC, 2 );

						/*
						if(cMyShop.OpenFlag){
						cItem.ShowItemInfo(&InvenItem[i] , 2,i);
						}
						else{
						cItem.ShowItemInfo(&InvenItem[i] , 2);
						}
						*/
					}
				}
			}
		}
	}

	return FALSE;
}

NAKED BOOL CGameInventory::GetInventoryTwoHandItem( ItemData * pcItem )
{
	JMP( pfnGetInventoryTwoHandItem );
}

BOOL CGameInventory::SetInventoryItem( ItemData * psItem )
{
	if( psItem->sBaseItemID.ToItemBase() == ITEMBASE_Potion )
	{
		for( size_t i = 0; i < 3; i++ )
		{
			if( INVENTORYITEMSLOT[10 + i].sBox.Inside( MOUSEHANDLER->GetPosition() ) )
				return TRUE;
		}

		ITEMHANDLER->BackItemToInventory( psItem );
	}
	else
		ITEMHANDLER->BackItemToInventory( psItem );

	return TRUE;
}

NAKED BOOL CGameInventory::SetInventoryItem( Item * psItem )
{
	JMP( pfnSetInventoryItemItem );
}

NAKED BOOL CGameInventory::LoadItemImageEXPGold( ItemData * pcItemData )
{
	JMP( pfnLoadItemImageEXPGold );
}

BOOL CGameInventory::ResetPotion()
{
	JMP( pfnResetPotion );
}
