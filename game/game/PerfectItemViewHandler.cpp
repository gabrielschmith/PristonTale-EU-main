#include "stdafx.h"
#include "PerfectItemViewHandler.h"


CPerfectItemViewHandler::CPerfectItemViewHandler()
{
	pcPrefectItemData = new ItemData;
	iDefenseOverride = 0;
	ZeroMemory(pcPrefectItemData, sizeof( ItemData ) );
}


CPerfectItemViewHandler::~CPerfectItemViewHandler()
{
	SAFE_DELETE(pcPrefectItemData);
}

BOOL CPerfectItemViewHandler::IsItemPreviewable(ItemData* pcItemData)
{
	const EItemType iItemType = pcItemData->sItem.sItemID.ToItemType();;
	const EItemBase iItemBase = pcItemData->sItem.sItemID.ToItemBase();
	const EItemCraftType iItemCraftType = pcItemData->sItem.eCraftType;

	return (iItemType != EItemType::ITEMTYPE_BlessCastle) &&
		(iItemType != EItemType::ITEMTYPE_Bellatra) &&
		(iItemType != EItemType::ITEMTYPE_Respec) &&
		(iItemType != EItemType::ITEMTYPE_Smelting) &&
		(iItemType != EItemType::ITEMTYPE_Crafting) &&
		(iItemType != EItemType::ITEMTYPE_Event) &&
		(iItemType != EItemType::ITEMTYPE_EventPR5) &&
		(iItemType != EItemType::ITEMTYPE_ForceOrb) &&
		(iItemBase != EItemBase::ITEMBASE_Core) &&
		(iItemBase != EItemBase::ITEMBASE_Potion) &&
		(iItemBase != EItemBase::ITEMBASE_Premium) &&
		(iItemBase != EItemBase::ITEMBASE_Quest1) &&
		(iItemBase != EItemBase::ITEMBASE_Quest2) &&
		(iItemBase != EItemBase::ITEMBASE_Crystal) &&
		(iItemBase != EItemBase::ITEMBASE_Recipe) &&
		(iItemType != EItemType::ITEMTYPE_Costume) &&
		(iItemType != EItemType::ITEMTYPE_Costume2) &&
		(iItemType != EItemType::ITEMTYPE_Costume3) &&
		(iItemType != EItemType::ITEMTYPE_Costume4) &&
		(iItemCraftType == EItemCraftType::ITEMCRAFTTYPE_Aging || iItemCraftType == EItemCraftType::ITEMCRAFTTYPE_None || iItemCraftType == EItemCraftType::ITEMCRAFTTYPE_Mixing);
}

void CPerfectItemViewHandler::PHItemData( PacketItemPerfectView * psPacket )
{
	// Copy Item Data
	CopyMemory(pcPrefectItemData, &psPacket->cItemData, sizeof( ItemData ) );

	// Set sItemID
	dwCodeReceived = dwCode = psPacket->cItemData.sBaseItemID.ToInt();
	eClassFlagReceived = eClassFlag = static_cast<EClassFlag>(psPacket->cItemData.sItem.eSpecialization);
	iItemRarityReceived = iItemRarity = psPacket->cItemData.sItem.eRarity;
	bReceivedItem = TRUE;
	iDefenseOverride = psPacket->iDefenseOverride;
}

BOOL CPerfectItemViewHandler::OnKeyPress( class CKeyboard * pcKeyBoard )
{
	// Is "p"? 
	if ( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyDown && tolower(pcKeyBoard->GetKey()) == CHAR('p') )
	{
		// Get Item Mouse Hover
		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if (CHATBOX && pcItemData == NULL)
			pcItemData = CHATBOX->GetMouseItemHover();

		if ( pcItemData && this->IsItemPreviewable(pcItemData))
		{
			// Send Data
			ProcessPacket( pcItemData );

			// Set Visible
			bVisible = TRUE;
		}
	}
	// Key Up and shown? Set Hide
	else if( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyUp && bVisible )
	{
		bVisible = FALSE;
		ITEMINFOBOX->SetViewingPerfectItem( FALSE );
		ITEMINFOBOX->SetPerfectItemDefenseOverride ( 0 );
		ITEMINFOBOX->ClearCompareItem ();
	}

	return FALSE;
}

void CPerfectItemViewHandler::Render()
{
	// Is Show?
	if ( bVisible )
	{
		// Get Mouse Item
		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if (pcItemData == NULL)
			pcItemData = CHATBOX->GetMouseItemHover();

		if ( pcItemData && pcItemData->sBaseItemID.ToItemID() )
		{
			// Set iItemSlot
			this->pcPrefectItemData->sPosition.iX	=		pcItemData->sPosition.iX;
			this->pcPrefectItemData->sPosition.iY	=		pcItemData->sPosition.iY;
			this->pcPrefectItemData->sOldPosition.iX =		pcItemData->sOldPosition.iX;
			this->pcPrefectItemData->sOldPosition.iY =		pcItemData->sOldPosition.iY;
			this->pcPrefectItemData->sPlacePosition.iX =	pcItemData->sPlacePosition.iX;
			this->pcPrefectItemData->sPlacePosition.iY	=	pcItemData->sPlacePosition.iY;
			


			int iThisItemRarity = pcItemData->sItem.eRarity;
			if ( iThisItemRarity == 0 )
				iThisItemRarity = 1;


			//Show Info
			if( bReceivedItem )
			{
				ITEMINFOBOX->SetViewingPerfectItem(TRUE);
				ITEMINFOBOX->SetPerfectItemDefenseOverride (iDefenseOverride);

				ITEMINFOBOX->SetCompareItem(pcItemData, FALSE);
				ITEMINFOBOX->PrepareShowItem( this->pcPrefectItemData, FALSE, FALSE, FALSE );
				bReceivedItem = false;
			}

			//Check that the current dwCode matches the current hovered item
			else if (this->dwCodeReceived == pcItemData->sBaseItemID.ToInt() && 
				     this->iItemRarityReceived == iThisItemRarity)
			{
				if (pcItemData->sItem.eSpecialization == EClassFlag::CLASSFLAG_None ||
				   (pcItemData->sItem.eSpecialization == this->eClassFlagReceived) )
				{
					ITEMINFOBOX->SetViewingPerfectItem(TRUE);
					ITEMINFOBOX->SetPerfectItemDefenseOverride ( iDefenseOverride );

					//BOOL __stdcall PrepareItemInfo( ItemData * psItemData, int iFlag, int iIndex )
					//replaces the prefect item with a standard version,
					//So will need to check here again and update.
					if (ITEMINFOBOX->IsSameItem(this->pcPrefectItemData, FALSE, FALSE) == FALSE)
					{
						bReceivedItem = true;
						ITEMINFOBOX->SetCompareItem(pcItemData, FALSE);
						ITEMINFOBOX->PrepareShowItem(this->pcPrefectItemData, FALSE, FALSE, FALSE);
						bReceivedItem = FALSE;
					}
				}
			}			
		}
	}
}

void CPerfectItemViewHandler::ProcessPacket( ItemData * pcItemData )
{
	if (!pcItemData->bValid)
		return;

	//->sBaseItemID.ToInt(), (EClassFlag)pcItemData->sItem.eSpecialization, (int)pcItemData->sItem.sAgeLevel, pcItemData->sItem.eMixTypeName
	//DWORD dwCode, EClassFlag iItemSpec, int iAgeLevel, EMixTypeName eMixTypeName


	//ode, EClassFlag iItemSpec, int iAgeLevel, EMixTypeName eMixTypeName

	//We need to get new item prefect stats if:
	// - item type is different
	// - item type is same but spec is different
	
	DWORD dwCode = pcItemData->sBaseItemID.ToInt();

	int thisRarity = pcItemData->sItem.eRarity;
	
	if ( thisRarity == 0 ) //rarity is none??
		thisRarity = 1;   //force it to be 1, so that it will match the incoming rarirty from server (common)

	BOOL bViewingSameRarityItem = this->iItemRarity == thisRarity;
	BOOL bViewingSameItemType = dwCode == this->dwCode && pcItemData->sItem.sAgeLevel == this->sAgeLevel && pcItemData->sItem.eMixTypeName == this->eMixTypeName && bViewingSameRarityItem;
	BOOL bViewingDifferentItemType = !bViewingSameItemType;
	BOOL bViewingNonSpecItem = pcItemData->sItem.eSpecialization == EClassFlag::CLASSFLAG_None;
	BOOL bViewingSameSpecItem = this->eClassFlag == pcItemData->sItem.eSpecialization;
	BOOL bViewingDifferentSpecItem = !bViewingSameSpecItem;

	//Note - if viewing an non-spec item, the mouse over item is non-spec
	// but the received item will have a spec.
	// so don't try and get new stats if viewing specs of same item type
	if (bViewingSameItemType && bViewingNonSpecItem)
		return;

	//Same item. Same spec.
	if (bViewingSameItemType && bViewingSameSpecItem)
		return;

	//DEBUG("REQUESTING PREFECT STATS");

	this->eClassFlag = (EClassFlag)pcItemData->sItem.eSpecialization;
	this->dwCode = dwCode;
	this->sAgeLevel = pcItemData->sItem.sAgeLevel;
	this->eMixTypeName = pcItemData->sItem.eMixTypeName;
	this->dwCodeReceived = 0;
	this->eClassFlagReceived = EClassFlag::CLASSFLAG_Unknown;
	this->iItemRarity = thisRarity;
	this->iItemRarityReceived = -1;

	//don't process old mixes (mix id of 1)
	//it was due to incorrect set up of mixes in the first place. There is no
	//way for us to easily tell which mix each item has.
	if (pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing && pcItemData->sItem.eMixTypeName == EMixTypeName::MIXTYPENAME_LegacyDoNotUse)
	{
		TITLEBOX("Perfect 'P' does not work with old mixes");
		return;
	}

	// Send Request
	PacketGetItemPerfectView sPacket;
	sPacket.iLength = sizeof( PacketGetItemPerfectView );
	sPacket.iHeader = PKTHDR_PerfectItemData;
	sPacket.dwCode	= dwCode;
	sPacket.sAgeLevel = pcItemData->sItem.sAgeLevel;
	sPacket.iItemSpec = pcItemData->sItem.eSpecialization;
	sPacket.iMixType = (int)pcItemData->sItem.eMixTypeName;
	sPacket.sMixColor = pcItemData->sItem.sMixColorId;
	sPacket.sUniqueMixId = pcItemData->sItem.sMixUniqueID1;
	sPacket.iMixEffect = pcItemData->sItem.eMixEffect;
	sPacket.iDefenseValue = pcItemData->sItem.iDefenseRating;
	sPacket.eItemRarity = (EItemRarity)thisRarity;
#ifdef ITEMFIXES
	sPacket.iPrefixID = pcItemData->sItem.sPrefixData.iID;
	sPacket.iSuffixID = pcItemData->sItem.sSuffixData.iID;
#endif
	SENDPACKETL(&sPacket);

#ifdef DEV_MODE
	CHATBOX->AddMessage ( FormatString ( "DEBUG> Get perfect stats for '%s' (Rarity: %d)", pcItemData->sItem.szItemName, sPacket.eItemRarity ) );
#endif 
	bReceivedItem = FALSE;

	
}