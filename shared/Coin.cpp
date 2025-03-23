#include "stdafx.h"
#include "Coin.h"

int Base::Coin::GetLevelItemEachAge( Item * psItem )
{
	int iLevel = 0;

	if (psItem->iLevel >= 88)
		return 0;

	if ( psItem->sAgeLevel > 0 )
		return 2;

	return 0;
}

int Base::Coin::GetPerfectizePrice( Item * psItem )
{
	int iCoins = 0;

	int iLevelItem = psItem->iLevel;
	if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging && psItem->sAgeLevel > 0 )
	{
		int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );
		
		if ( iLevelAge > 0 )
			iLevelItem -= (psItem->sAgeLevel / iLevelAge);
	}

	//Weapon?
	if ( psItem->sItemID.ToItemBase() == ITEMBASE_Weapon )
	{
		if ( iLevelItem <= 100 )
			iCoins = 250;
		else if ( iLevelItem == 105 )
			iCoins = 500;
		else if ( iLevelItem == 110 )
			iCoins = 750;
		else if ( iLevelItem == 114 )
			iCoins = 1500;
		else if ( iLevelItem == 118 )
			iCoins = 2250;
		else if ( iLevelItem == 122 )
			iCoins = 2600;
		else if ( iLevelItem == 126 )
			iCoins = 3000;
	}
	else if ( psItem->sItemID.ToItemBase() == ITEMBASE_Defense || psItem->sItemID.ToItemBase() == ITEMBASE_Sheltom )
	{
		if ( psItem->sItemID.ToItemType() == ITEMTYPE_Armor ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Robe ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Shield ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Orb )
		{
			if ( iLevelItem <= 100 )
				iCoins = 200;
			else if ( iLevelItem == 105 )
				iCoins = 450;
			else if ( iLevelItem == 110 )
				iCoins = 600;
			else if ( iLevelItem == 114 )
				iCoins = 1000;
			else if ( iLevelItem == 118 )
				iCoins = 1500;
			else if ( iLevelItem == 122 )
				iCoins = 1800;
			else if ( iLevelItem == 126 )
				iCoins = 2100;
		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Boots ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Gauntlets ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Bracelets )
		{
			if ( psItem->sItemID.ToItemID() == ITEMID_DraxosBoots )
				iCoins = 3200;
			else if ( psItem->sItemID.ToItemID() == ITEMID_MokovaBoots )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_GreedyGauntlets )
				iCoins = 35000;
			else if ( iLevelItem <= 100 )
				iCoins = 75;
			else if ( iLevelItem == 105 )
				iCoins = 250;
			else if ( iLevelItem == 110 )
				iCoins = 350;
			else if ( iLevelItem == 114 )
				iCoins = 700;
			else if ( iLevelItem == 118 )
				iCoins = 1000;
			else if ( iLevelItem == 122 )
				iCoins = 1500;
			else if ( iLevelItem == 126 )
				iCoins = 1800;
		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Ring ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Ring2 ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Sheltom ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Amulet )
		{
			if ( psItem->sItemID.ToItemID() == ITEMID_FurySheltom )
				iCoins = 2000;
			else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Sheltom )
			{
				if ( iLevelItem <= 60 )
					iCoins = 15;
				else if ( iLevelItem == 70 )
					iCoins = 25;
				else if ( iLevelItem == 80 )
					iCoins = 38;
				else if ( iLevelItem == 90 )
					iCoins = 50;
				else if ( iLevelItem == 100 )
					iCoins = 75;
				else if ( iLevelItem == 110 )
					iCoins = 125;
			}
			else if ( psItem->sItemID.ToItemID() == ITEMID_ValentoRing )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_AbyssRing )
				iCoins = 2750;
			else if ( psItem->sItemID.ToItemID() == ITEMID_KelvezuAmulet )
				iCoins = 1500;
			else if ( psItem->sItemID.ToItemID() == ITEMID_TullaAmulet )
				iCoins = 2500;
			else if ( iLevelItem <= 100 )
				iCoins = 125;
			else if ( iLevelItem == 110 )
				iCoins = 400;
			else if ( iLevelItem == 118 )
				iCoins = 500;
			else if ( iLevelItem == 126 )
				iCoins = 600;
		}
		else if ( psItem->sItemID.ToItemID() == ITEMID_SpecStoneFS || psItem->sItemID.ToItemID() == ITEMID_SpecStoneMS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneAS || psItem->sItemID.ToItemID() == ITEMID_SpecStonePS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneATA || psItem->sItemID.ToItemID() == ITEMID_SpecStoneKS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneMG || psItem->sItemID.ToItemID() == ITEMID_SpecStonePRS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneASS || psItem->sItemID.ToItemID() == ITEMID_SpecStoneSHA )
		{
			iCoins = 400;
		}
	}

	return iCoins;
}

int Base::Coin::GetSwapperPrice( Item * psItem, int iItemType )
{
	int iCoins = 0;

	//Lambda
	auto GetSwapCategoryPrice = []( int iLevel )->int
	{
		int iAdd = 0;
		
		if ( iLevel <= 100 )
			iAdd = 700;
		else if ( iLevel == 105 )
			iAdd = 900;
		else if ( iLevel == 110 )
			iAdd = 1100;
		else if ( iLevel == 114 )
			iAdd = 1300;
		else if ( iLevel == 118 )
			iAdd = 1500;
		else if ( iLevel == 122 )
			iAdd = 1700;
		else if ( iLevel == 126 )
			iAdd = 2000;

		return iAdd;
	};

	int iLevelItem = psItem->iLevel;
	if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging && psItem->sAgeLevel > 0 )
	{
		int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );

		if ( iLevelAge > 0 )
			iLevelItem -= (psItem->sAgeLevel / iLevelAge);
	}

	//Weapon?
	if ( psItem->sItemID.ToItemBase() == ITEMBASE_Weapon )
	{
		if ( iLevelItem <= 100 )
			iCoins = 200;
		else if ( iLevelItem == 105 )
			iCoins = 400;
		else if ( iLevelItem == 110 )
			iCoins = 600;
		else if ( iLevelItem == 114 )
			iCoins = 800;
		else if ( iLevelItem == 118 )
			iCoins = 1000;
		else if ( iLevelItem == 122 )
			iCoins = 1200;
		else if ( iLevelItem == 126 )
			iCoins = 1400;

		if ( iCoins > 0 && psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) )
		{
			iCoins = GetSwapCategoryPrice( iLevelItem );
		}
	}
	else if ( psItem->sItemID.ToItemBase() == ITEMBASE_Defense || psItem->sItemID.ToItemBase() == ITEMBASE_Sheltom )
	{
		if ( psItem->sItemID.ToItemType() == ITEMTYPE_Armor ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Robe ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Shield ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Orb )
		{
			if ( iLevelItem <= 100 )
				iCoins = 200;
			else if ( iLevelItem == 105 )
				iCoins = 400;
			else if ( iLevelItem == 110 )
				iCoins = 600;
			else if ( iLevelItem == 114 )
				iCoins = 800;
			else if ( iLevelItem == 118 )
				iCoins = 1000;
			else if ( iLevelItem == 122 )
				iCoins = 1200;
			else if ( iLevelItem == 126 )
				iCoins = 1400;

			if ( iCoins > 0 && (psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) || iItemType == ITEMTYPE_Boots || iItemType == ITEMTYPE_Gauntlets) )
			{
				iCoins = GetSwapCategoryPrice( iLevelItem );
			}

		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Boots ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Gauntlets ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Bracelets )
		{
			if ( iLevelItem <= 100 )
				iCoins = 100;
			else if ( iLevelItem == 105 )
				iCoins = 200;
			else if ( iLevelItem == 110 )
				iCoins = 400;
			else if ( iLevelItem == 114 )
				iCoins = 600;
			else if ( iLevelItem == 118 )
				iCoins = 800;
			else if ( iLevelItem == 122 )
				iCoins = 1000;
			else if ( iLevelItem == 126 )
				iCoins = 1200;

			if ( iCoins > 0 && (psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) || 
				iItemType == ITEMTYPE_Armor ||
				iItemType == ITEMTYPE_Shield ||
				iItemType == ITEMTYPE_Orb ||
				iItemType == ITEMTYPE_Robe) )
			{
				iCoins = GetSwapCategoryPrice( iLevelItem );
			}
		}
	}

	return iCoins;
}
