#include "stdafx.h"
#include "ManufactureHandler.h"


CManufactureHandler::CManufactureHandler()
{
}


CManufactureHandler::~CManufactureHandler()
{
}

void CManufactureHandler::SetManufactureAreaCheck( class ItemData * pcItem )
{
	POINT sRunePoint[3] = { {240, 159},  {215, 190}, {265, 190} };

	ManufactureRune * psRuneData = (ManufactureRune*)0x03692E70;

	DWORD dwCode = (*(DWORD*)0x0368D9A0);

	RECT * pRect = ((RECT*)0x036EFCAC);
	CALL( 0x0050BDC0 );

	ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_None );

	if ( IsRecipe( pcItem->sItem.sItemID.ToItemID() ) || CanRespec( pcItem->sItem.sItemID.ToItemID() ) )
	{
		pRect->left		= 58;
		pRect->top		= 193 - 50;
		pRect->right	= (pRect->left + (3 * 22)) - pRect->left;
		pRect->bottom	= (pRect->top + (4 * 22)) - pRect->top;

		Rectangle2D cRectItem{ pRect->left, pRect->top, (3 * 22), (4 * 22) };

		if ( cRectItem.Inside( MOUSEHANDLER->GetPosition() ) )
		{
			pcItem->sPlacePosition.iX = pRect->left + (((3 * 22) / 2) - (pcItem->iWidth / 2));
			pcItem->sPlacePosition.iY = pRect->top + (((4 * 22) / 2) - (pcItem->iHeight / 2));
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );

			if (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeChristmas01 || 
				pcItem->sItem.sItemID.ToItemID() ==  ITEMID_RecipeEaster01)
			{
				WRITEDWORD(0x036EFD24, 10'000);
			}
			else if (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeChristmas02)
			{
				WRITEDWORD(0x036EFD24, 100'000);
			}
			else
			{
				WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice );
			}

			if ( (int)READDWORD( 0x036EFD24 ) > UNITDATA->sCharacterData.iGold )
			{
				TITLEBOX( "Not enough gold" );
				ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );
				return;
			}

			if ( ((*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( pRect, 0, 5 )) )
			{
				ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );
				return;
			}

		}
		return;
	}

	BOOL bCollides = FALSE;
	for ( int i = 0; i < 3; i++ )
	{
		Rectangle2D cRect( sRunePoint[i].x-11, sRunePoint[i].y-11, 22, 22 );
		Point2D sPoint = { pcItem->sPosition.iX, pcItem->sPosition.iY };

		if ( cRect.Inside( &sPoint ) )
		{
			pRect->left		= sRunePoint[i].x;
			pRect->top		= sRunePoint[i].y;
			pRect->right	= pcItem->iWidth;
			pRect->bottom	= pcItem->iHeight;
			bCollides		= TRUE;
			break;
		}
	}
	if ( !bCollides )
		return;

	if ( dwCode && (IsRespecStone( pcItem->sItem.sItemID.ToItemID() ) || pcItem->sItem.sItemID.ToItemID() == ITEMID_Jera) )
	{
		if ( CanRespec( (EItemID)dwCode ) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	// Check is in slot
	if ( !dwCode || !IsRune( pcItem->sItem.sItemID.ToItemID() ) )
		return;

	ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_NotSetItem );

	if ( ((*(DWORD*)0x036EFCA4) = ITEMWINDOWHANDLER->CollideItemTrade( pRect, 0, 5 )) )
	{
		return;
	}
	if ( dwCode == ITEMID_CoreHW )
	{
		if ( HALLOWEENHANDLER->IsValidRune( pcItem ) )
		{
			pcItem->sPlacePosition.iX			= pRect->left;
			pcItem->sPlacePosition.iY			= pRect->top;
			pcItem->iItemSlot	= 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		
		return;
	}
	if ( dwCode == ITEMID_RecipeChristmas01 || dwCode == ITEMID_RecipeChristmas02 )
	{
		if ( CHRISTMASHANDLER->IsValidRune( pcItem ) )
		{
			pcItem->sPlacePosition.iX			= pRect->left;
			pcItem->sPlacePosition.iY			= pRect->top;
			pcItem->iItemSlot	= 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		
		return;
	}

	if (dwCode == ITEMID_RecipeEaster01)
	{
		if (EASTEREVENTHANDLER->IsValidRune(pcItem))
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox(TRADECOLORBOX_SetItem);
			return;
		}

		return;
	}

	//Tulla Amulet
	if ( dwCode == ITEMID_KelvezuAmulet )
	{
		if ( IsValidRuneTullaAmulet( pcItem->sItem.sItemID.ToItemID() ) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}
	//Valento Ring
	if ( dwCode == ITEMID_ValentoRing )
	{
		if ( IsValidRuneAbyssRing( pcItem->sItem.sItemID.ToItemID() ) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}

	//Draxos Boots
	if ( dwCode == ITEMID_MokovaBoots )
	{
		if ( (pcItem->sItem.sItemID.ToItemID() == ITEMID_DarkRedRune) || (pcItem->sItem.sItemID.ToItemID() == ITEMID_Oredo) )
		{
			pcItem->sPlacePosition.iX = pRect->left;
			pcItem->sPlacePosition.iY = pRect->top;
			pcItem->iItemSlot = 0;
			ITEMWINDOWHANDLER->SetTradeItemColorBox( TRADECOLORBOX_SetItem );
			return;
		}
		return;
	}
}

BOOL CManufactureHandler::OnClickItemSlotPut( class ItemData * pcItem )
{
	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	ManufactureItem * psaManufactureData = (ManufactureItem*)0x0368D9A0;

	if ( IsRune( pcItem->sBaseItemID.ToItemID() ) )
	{
		// Check Slots
		ItemData * psaItemData = (ItemData*)0x0368DD80;
		for ( int i = 1; i < 4; i++ )
		{
			if ( psaManufactureData->sItemData[i].bValid == FALSE )
			{
				ItemData * ps	= &psaManufactureData->sItemData[i];
				
				CopyMemory( ps, pcItem, sizeof( ItemData ) );
				
				ps->sPosition.iX	= pcItem->sPlacePosition.iX;
				ps->sPosition.iY	= pcItem->sPlacePosition.iY;
				pcItem->bValid		= FALSE;
				
				PLAYMINISOUND( pcItem->iInvItemSound );
				
				CALLT( 0x005125B0, 0x0369DD10 );

				break;
			}
		}
	}
	else if ( psaManufactureData->sItemData[0].bValid == FALSE )
	{

		if ( pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeChristmas01 || 
			 pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeEaster01)
		{
			WRITEDWORD( 0x036EFD24, 10'000 );
		}
		else if (pcItem->sItem.sItemID.ToItemID() == ITEMID_RecipeChristmas02)
		{
			WRITEDWORD(0x036EFD24, 100'000);
		}
		else
		{
			WRITEDWORD( 0x036EFD24, pcItem->sItem.iSalePrice );
		}

		if ( (int)READDWORD( 0x036EFD24 ) > UNITDATA->sCharacterData.iGold )
		{
			TITLEBOX( "Not enough gold" );
			return TRUE;
		}

		ItemData * ps	= &psaManufactureData->sItemData[0];

		CopyMemory( ps, pcItem, sizeof( ItemData ) );

		ps->sPosition.iX	= pcItem->sPlacePosition.iX;
		ps->sPosition.iY	= pcItem->sPlacePosition.iY;
		pcItem->bValid		= FALSE;

		PLAYMINISOUND( pcItem->iInvItemSound );

		ItemData * psCraft = (ItemData*)0x03692FA0;
		CopyMemory( psCraft, ps, sizeof( ItemData ) );

		CALLT( 0x005125B0, 0x0369DD10 );

	}

	return TRUE;
}

void CManufactureHandler::SetManufacture()
{
	(*(BOOL*)0x037B7F08) = TRUE;
	CALL( 0x0052B020 );
	(*(BOOL*)0x036EFD80) = TRUE;
}

void CManufactureHandler::Update()
{
	struct ManufactureItem
	{
		ItemData					sItemData[4];
		int							iRuneIndex[12];
	};

	if ( WINDOW_ISOPEN_CRAFTING && CRAFTPROCCESS_START )
	{
		ManufactureItem * psaManufactureData = (ManufactureItem*)0x0368D9A0;

		if ( !CHARACTERGAME->CheckInventorySpace( &psaManufactureData->sItemData[0] ) )
		{
			TITLEBOX( "Not enough space in inventory" );
			CRAFTPROCCESS_START = FALSE;
		}

		// Check iWeight
		int iItemID = psaManufactureData->sItemData[0].sItem.sItemID.ToInt();
		if ( (iItemID == ITEMID_RecipeChristmas01) || 
			 (iItemID == ITEMID_RecipeChristmas02) || 
			 (iItemID == ITEMID_RecipeEaster01) )
		{
			if ( !CHARACTERGAME->CheckInventorySpace( ITEMHANDLER->FindItemTableByCode( ITEMID_BrazilianSoccer ) ) )
			{
				TITLEBOX( "Not enough space in inventory" );
				CRAFTPROCCESS_START = FALSE;
			}

			if ( (UNITDATA->sCharacterData.sWeight.sCur + 40) > UNITDATA->sCharacterData.sWeight.sMax )
			{
				TITLEBOX( "Weight limit exceeded" );
				CRAFTPROCCESS_START = FALSE;
			}
		}
	}
}


BOOL CManufactureHandler::IsRecipe( EItemID eID )
{
	BOOL b = FALSE;

	switch ( eID )
	{
		case ITEMID_KelvezuAmulet:
			b = TRUE;
			break;

		default:
			break;
	}

	if ( (eID & 0xFFFF0000) == ITEMID_RecipeCode )
		b = TRUE;

	return b;
}

BOOL CManufactureHandler::CanRespec( EItemID eID )
{
	switch ( (eID & 0xFF000000) )
	{
		case ITEMBASE_Weapon:
		case ITEMBASE_Defense:
		case ITEMBASE_Sheltom:
		{
			if ( IsRune( eID ) )
				return FALSE;

			if ( (eID & 0xFFFF0000) == ITEMTYPE_Sheltom || (eID & 0xFFFF0000) == ITEMTYPE_Amulet )
			{
				if ( eID != ITEMID_FurySheltom &&
					eID != ITEMID_KelvezuAmulet &&
					eID != ITEMID_TullaAmulet &&
					eID != ITEMID_GuidanceAmulet &&
					eID != ITEMID_GuidanceAmuletPlus1 &&
					eID != ITEMID_DevilShyRing &&
					eID != ITEMID_AbyssRing &&
					eID != ITEMID_ValentoRing )
					return FALSE;
			}

			if ( IsRespecStone( eID ) )
				return FALSE;

			return TRUE;
		}
		default: 
			break;
	}

	return FALSE;
}

BOOL CManufactureHandler::IsRespecStone( EItemID eID )
{
	if ( (eID & 0xFFFFFF00) == ITEMID_SpecStoneCode )
		return TRUE;

	return FALSE;
}

BOOL CManufactureHandler::IsRune( EItemID eID )
{
	BOOL b = FALSE;

	switch ( eID )
	{
		case ITEMID_Oredo:
			b = TRUE;
			break;

		default:
			break;
	}

	if ( (eID & 0xFFFF0000) == ITEMID_RuneCodeManufacture1 )
		b = TRUE;
	if ( (eID & 0xFFFF0000) == ITEMTYPE_Respec )
		b = TRUE;

	return b;
}

BOOL CManufactureHandler::IsValidRuneTullaAmulet( EItemID eID )
{
	BOOL bValid = FALSE;
	switch ( eID )
	{
		case ITEMID_Oredo:
		case ITEMID_LightBlueRune:
			bValid = TRUE;
			break;

		default:
			break;
	}

	// Check if have items
	if ( bValid )
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem * psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iOredos = 0;
		int iLight = 0;

		for ( int i = 0; i < 4; i++ )
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if ( iItemID == ITEMID_Oredo )
				iOredos++;
			else if ( iItemID == ITEMID_LightBlueRune )
				iLight++;
		}

		if ( (iOredos >= 2 && eID == ITEMID_Oredo) || (iLight >= 1 && eID == ITEMID_LightBlueRune) )
			bValid = FALSE;
	}

	return bValid;
}

BOOL CManufactureHandler::IsValidRuneAbyssRing( EItemID eID )
{
	BOOL bValid = FALSE;
	switch ( eID )
	{
		case ITEMID_Oredo:
		case ITEMID_LightGreenRune:
			bValid = TRUE;
			break;

		default:
			break;
	}

	// Check if have items
	if ( bValid )
	{
		struct ManufactureItem
		{
			ItemData					sItemData[4];
			int							iRuneIndex[12];
		};

		ManufactureItem * psManufactureData = (ManufactureItem*)0x0368D9A0;
		int iOredos = 0;
		int iLight = 0;

		for ( int i = 0; i < 4; i++ )
		{
			int iItemID = psManufactureData->sItemData[i].sBaseItemID.ToInt();

			if ( iItemID == ITEMID_Oredo )
				iOredos++;
			else if ( iItemID == ITEMID_LightGreenRune )
				iLight++;
		}

		if ( (iOredos >= 2 && eID == ITEMID_Oredo) || (iLight >= 1 && eID == ITEMID_LightGreenRune) )
			bValid = FALSE;
	}

	return bValid;
}
