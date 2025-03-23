#include "stdafx.h"
#include "CompareItemHandler.h"

CCompareItemHandler::CCompareItemHandler()
{
	pcTempItemData = new ItemData;
	ZeroMemory( pcTempItemData, sizeof( ItemData ) );
}

CCompareItemHandler::~CCompareItemHandler()
{
	SAFE_DELETE( pcTempItemData );
}

void CCompareItemHandler::Init()
{
	pItemInfoBox = std::make_shared<UI::ItemInfoBox>();
	pItemInfoBox->Init();
}

BOOL CCompareItemHandler::OnKeyPress( class CKeyboard * pcKeyBoard )
{
	//Is SHIFT? 
	if ( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyDown && pcKeyBoard->GetKey() == VK_SHIFT )
	{
		// Get Mouse Item
		ItemData* pcMouseItemData = ITEMHANDLER->GetMouseItemHover();

		if (WAREHOUSEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if (TRADEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = TRADEHANDLER->GetMouseItemHover();

		if (TRADEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if (CHATBOX && pcMouseItemData == NULL)
			pcMouseItemData = CHATBOX->GetMouseItemHover();

		if(pcMouseItemData && pcMouseItemData->bValid)
		{
			int iItemType = pcMouseItemData->sItem.sItemID.ToItemType();
			int iItemBase = pcMouseItemData->sItem.sItemID.ToItemBase();

			if (iItemBase == ITEMBASE_Defense || //defense also include sheltoms
				iItemBase == ITEMBASE_Weapon ||
				iItemType == ITEMTYPE_Amulet ||
				iItemType == ITEMTYPE_Bracelets ||
				iItemType == ITEMTYPE_Orb ||
				iItemType == ITEMTYPE_Ring)
			{
				for (int i = 0; i < 14; i++)
				{
					if (INVENTORYITEMSLOT[i].iItemIndex)
					{
						ItemData* psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];
						if ((psItemData->sItem.iChk1 == pcMouseItemData->sItem.iChk1) && (psItemData->sItem.iChk2 == pcMouseItemData->sItem.iChk2))
							return TRUE;
					}
				}

				//Already checked? So dont check again
				if (bAlreadyChecked)
					return TRUE;


				//Find Equipped item with same type
				ItemData* pcItemDataEquipped = ITEMHANDLER->GetItemEquippedByType(pcMouseItemData->sBaseItemID.ToItemType());
				if (pcItemDataEquipped)
				{
					CopyMemory(pcTempItemData, pcItemDataEquipped, sizeof(ItemData));

					//int iXExtra = (pcMouseItemData->sPosition.iX + (pcMouseItemData->iWidth / 2)) - 90;
					//if (iXExtra < 0)
					//	iXExtra = abs(iXExtra);
					//else
					//	iXExtra = 0;

					//Set iItemSlot
					this->pcTempItemData->sPosition.iX = pcMouseItemData->sPosition.iX;
					this->pcTempItemData->sPosition.iY = pcMouseItemData->sPosition.iY;
					this->pcTempItemData->sOldPosition.iX = pcMouseItemData->sOldPosition.iX;
					this->pcTempItemData->sOldPosition.iY = pcMouseItemData->sOldPosition.iY;
					this->pcTempItemData->sPlacePosition.iX = pcMouseItemData->sPlacePosition.iX;
					this->pcTempItemData->sPlacePosition.iY = pcMouseItemData->sPlacePosition.iY;


					ITEMINFOBOX->SetCompareItem(pcTempItemData);
					ITEMINFOBOX->PrepareShowItem(pcMouseItemData, FALSE, FALSE, FALSE);

					int iBaseWidth = ITEMINFOBOX->iBaseMaxWidth;

					pItemInfoBox->SetCompareItem(pcMouseItemData, TRUE);
					pItemInfoBox->PrepareShowItem(pcTempItemData, FALSE, FALSE, FALSE, TRUE, iBaseWidth);

					bVisible = TRUE;
					bAlreadyChecked = TRUE;
				}

				return TRUE;
			}
		}

		return FALSE;
		
	}
	//Key Up and shown? Set Hide
	else if( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyUp && bVisible )
	{
		ZeroMemory( pcTempItemData, sizeof( ItemData ) );
		bAlreadyChecked = FALSE;
		bVisible = FALSE;

		//Not compare Item
		ITEMINFOBOX->ClearCompareItem();
		pItemInfoBox->ClearCompareItem();
	}

	return FALSE;
}

void CCompareItemHandler::Render()
{
	if( !pcTempItemData )
		return;

	// Is Show?
	if ( bVisible )
	{
		// Get Mouse Item
		ItemData* pcMouseItemData = ITEMHANDLER->GetMouseItemHover();

		if (WAREHOUSEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if (TRADEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = TRADEHANDLER->GetMouseItemHover();

		if (TRADEHANDLER && pcMouseItemData == NULL)
			pcMouseItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if (CHATBOX && pcMouseItemData == NULL)
			pcMouseItemData = CHATBOX->GetMouseItemHover();


		if (pcMouseItemData && pcMouseItemData->sBaseItemID.ToItemType() == pcTempItemData->sBaseItemID.ToItemType() )
		{
			// Show Info
			pItemInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
	}
}