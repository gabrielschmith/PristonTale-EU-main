#include "stdafx.h"
#include "CoinShopHandler.h"


CCoinShopHandler::CCoinShopHandler()
{
	iChangeNickPrice = 500;
	iAgingRecoveryPrice = 1000;
}


CCoinShopHandler::~CCoinShopHandler()
{
}

void CCoinShopHandler::SQLCoinLog( char * pszAccountName, char * pszDescription )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 10 );


	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO CoinLog([AccountName],[Description],[Date]) VALUES (?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszDescription, lstrlenA( pszDescription ) );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CCoinShopHandler::LoadCoinShop( int iCoinShopID )
{
	SAFE_DELETE( pcCoinShop );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB, 1);


	if( pcDB->Open() )
	{
		BOOL bCoinShopLoaded = FALSE;
		int iSelectedCoinShopID;
		char szName[64];
		char szMessage[256];
		int iDiscountP;

		if( iCoinShopID != -1 )
		{
			if( pcDB->Prepare( "SELECT ID, Name, Message, Discount FROM CoinShop WHERE ID=?" ) )
			{
				//Bind Parameter
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCoinShopID );
			}
		}
		else
		{
			pcDB->Prepare( "SELECT ID, Name, Message, Discount From CoinShop WHERE Active=1" );
		}

		if( pcDB->Execute() && pcDB->Fetch() )
		{
			pcDB->GetData( 1, PARAMTYPE_Integer, &iSelectedCoinShopID );
			pcDB->GetData( 2, PARAMTYPE_String, szName, _countof( szName ) );
			pcDB->GetData( 3, PARAMTYPE_String, szMessage, _countof( szMessage ) );
			pcDB->GetData( 4, PARAMTYPE_Integer, &iDiscountP );

			bCoinShopLoaded = TRUE;
		}

		if( bCoinShopLoaded )
		{
			pcDB->Clear();

			if( (iCoinShopID != -1) && (iCoinShopID == iSelectedCoinShopID) )
			{
				pcDB->Prepare( "UPDATE CoinShop SET Active=0" );
				pcDB->ExecuteUpdate();
				pcDB->Clear();

				if( pcDB->Prepare( "UPDATE CoinShop SET Active=1 WHERE ID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCoinShopID );

					pcDB->ExecuteUpdate();
				}
				pcDB->Clear();
			}

			pcCoinShop = new CCoinShop( iSelectedCoinShopID );
			pcCoinShop->SetName( szName );
			pcCoinShop->SetMessage( szMessage );
			pcCoinShop->SetDiscount( iDiscountP );

			//Tabs
			int lCoinShopTabID = -1;
			int lCoinShopTabParentID = 0;
			char szTabName[32];
			int iTabDiscountP = 0;
			int iTabBulk = 0;
			int iMaxTabBulk = 0;

			if( pcDB->Prepare( "SELECT ID, Name, ParentID, Discount, [Bulk], MaxBulk FROM CoinShopTab WHERE CoinShopID=? ORDER BY ParentID ASC, ListOrder ASC" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iSelectedCoinShopID );

				if( pcDB->Execute() )
				{
					while( pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &lCoinShopTabID );
						pcDB->GetData( 2, PARAMTYPE_String, szTabName, _countof( szTabName ) );
						pcDB->GetData( 3, PARAMTYPE_Integer, &lCoinShopTabParentID );
						pcDB->GetData( 4, PARAMTYPE_Integer, &iTabDiscountP );
						pcDB->GetData( 5, PARAMTYPE_Integer, &iTabBulk );
						pcDB->GetData( 6, PARAMTYPE_Integer, &iMaxTabBulk );

						CCoinShopTab * pcTab = new CCoinShopTab( lCoinShopTabID, szTabName, lCoinShopTabParentID, iTabBulk, iMaxTabBulk );
						pcTab->SetDiscount( iDiscountP != 0 ? iDiscountP : iTabDiscountP );

						if( pcTab->GetParentTabID() != 0 )
						{
							//Has a Parent, so add it as a Child Tab of another Tab
							CCoinShopTab * pcParentTab = pcCoinShop->GetTabByID( pcTab->GetParentTabID() );

							if( pcParentTab )
								pcParentTab->AddChildTab( pcTab );
						}
						else
						{
							//Must be a root Tab
							pcCoinShop->AddTab( pcTab );
						}
					}
				}
			}

			//Items
			std::vector<CCoinShopTab*> vTabs = pcCoinShop->GetAllTabs();
			for( std::vector<CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
			{
				pcDB->Clear();

				CCoinShopTab * pcTab = *it;

				int lTabID = pcTab->GetID();

				if( pcDB->Prepare( "SELECT ID, Name, Description, Code, Image, Value, Discount,	[Bulk], MaxBulk, IsSpec, IsQuantity FROM CoinShopItem WHERE TabID=? AND Disabled=0 ORDER BY ListOrder ASC" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &lTabID );

					if( pcDB->Execute() )
					{
						while( pcDB->Fetch() )
						{
							int lID;
							char szName[32];
							char szDescription[128];
							char szCode[8];
							char szImage[64];
							int iValue;
							int iDiscount;
							int iBulk;
							int iMaxBulk;
							BOOL bSpec;
							BOOL bQuantity;

							pcDB->GetData( 1, PARAMTYPE_Integer, &lID );
							pcDB->GetData( 2, PARAMTYPE_String, szName, _countof( szName ) );
							pcDB->GetData( 3, PARAMTYPE_String, szDescription, _countof( szDescription ) );
							pcDB->GetData( 4, PARAMTYPE_String, szCode, _countof( szCode ) );
							pcDB->GetData( 5, PARAMTYPE_String, szImage, _countof( szImage ) );
							pcDB->GetData( 6, PARAMTYPE_Integer, &iValue );
							pcDB->GetData( 7, PARAMTYPE_Integer, &iDiscount );
							pcDB->GetData( 8, PARAMTYPE_Integer, &iBulk );
							pcDB->GetData( 9, PARAMTYPE_Integer, &iMaxBulk );
							pcDB->GetData( 10, PARAMTYPE_Integer, &bSpec );
							pcDB->GetData( 11, PARAMTYPE_Integer, &bQuantity );

							//Calculate real Discount
							int iRealDiscount = iDiscount;
							if( iRealDiscount == 0 )
							{
								if( iDiscountP != 0 )
								{
									//Use the Global Discount Percentile to Discount this Item
									iRealDiscount = (iValue * iDiscountP) / 100;
								}
							}

							//Create Item
							CCoinShopItem * pcItem = new CCoinShopItem( lID, lTabID, szName, szDescription, szCode, szImage, iValue, iRealDiscount, iBulk, iMaxBulk, bSpec, bQuantity );

							//Add Item to Tab
							pcTab->AddItem( pcItem );
						}
					}
				}
			}
		}
		pcDB->Close();
	}
}

void CCoinShopHandler::Init()
{
	if ( !GAME_SERVER )
		LoadCoinShop();
}

int CCoinShopHandler::GetCoinAmount( char * pszAccountName )
{
	if( (pszAccountName == NULL) || (pszAccountName[0] == 0) )
		return 0;

	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "SELECT Coins FROM UserInfo WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );

			if( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	return iRet;
}

int CCoinShopHandler::GetCoinTradeAmount( char * pszAccountName )
{
	if ( (pszAccountName == NULL) || (pszAccountName[0] == 0) )
		return 0;

	int iRet = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT CoinsTraded FROM UserInfo WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );

			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );
		}

		pcDB->Close();
	}

	return iRet;
}

int CCoinShopHandler::GetBulkValue( CCoinShopItem * pcItem, int iQuantity )
{
	if ( pcItem )
	{
		if ( pcItem->GetBulk() && iQuantity >= 5 )
		{
			int iValue = iQuantity / 5;
			if ( (iValue*pcItem->GetBulk()) > pcItem->GetMaxBulk() )
				iValue = pcItem->GetMaxBulk() / 5;

			int iPrice = (pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue())  * iQuantity;

			return (iPrice*(pcItem->GetBulk() * iValue)) / 100;
		}
	}
	return 0;
}

int CCoinShopHandler::UseCoin( char * pszAccountName, int iCoinAmount )
{
	if( pszAccountName[0] == 0 )
		return 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "UPDATE UserInfo SET Coins=Coins-? WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCoinAmount );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );

			if( !pcDB->ExecuteUpdate() )
			{
				//Log Error

			}
		}

		pcDB->Close();
	}

	return GetCoinAmount( pszAccountName );
}

void CCoinShopHandler::AddTradeCoin( char * pszAccountName, int iCoinAmount )
{
	if ( pszAccountName[0] == 0 )
		return;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE UserInfo SET CoinsTraded=CoinsTraded+? WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCoinAmount );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );

			if ( !pcDB->ExecuteUpdate() )
			{
				//Log Error

			}
		}

		pcDB->Close();
	}
}

int CCoinShopHandler::AddCoin( char * pszAccountName, int iCoinAmount )
{
	if( pszAccountName[0] == 0 )
		return 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "UPDATE UserInfo SET Coins=Coins+? WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCoinAmount );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszAccountName, lstrlenA( pszAccountName ) );

			if( !pcDB->ExecuteUpdate() )
			{
				//Log Error

			}
		}

		pcDB->Close();
	}

	return GetCoinAmount( pszAccountName );
}

BOOL CCoinShopHandler::HasCoinAmount( char * pszAccountName, int iCoinAmount )
{
	int iCoins = GetCoinAmount( pszAccountName );

	if( iCoins >= iCoinAmount )
		return TRUE;

	return FALSE;
}

BOOL CCoinShopHandler::PerfectizeItem( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	ItemData * pcItemData = &psPacket->ItemPerfect.cItemData;

	Item sOldItem;
	CopyMemory( &sOldItem, &pcItemData->sItem, sizeof( Item ) );

	//Item cannot be Perfect
	if ( pcItemData->sItem.iItemSpecialType != 1 )
	{
		int iCoins = 0;

		//Set Price
		iCoins = Base::Coin::GetPerfectizePrice( &pcItemData->sItem );

		//Global Discount
		if ( pcCoinShop->GetDiscount() > 0 )
			iCoins = (pcCoinShop->GetDiscount() * iCoins) / 100;

		//Price setted and have Coins?
		if ( iCoins > 0 && HasCoinAmount( pcUser->pcUserData->szAccountName, iCoins ) )
		{
			Item sItem;

			int iItemID = pcItemData->sItem.sItemID.ToInt();

			if ( (iItemID & 0xFFFFFF00) == ITEMID_SpecStoneCode )
				iItemID += 10; // Deluxe

			DefinitionItem * psDefItem = ITEMSERVER->FindItemDefByCode( iItemID );

			//Item Exists?
			if ( psDefItem )
			{

				int iSpec = pcItemData->sItem.eSpecialization;

				//Check if Have Spec
				if ( iSpec != 0 )
				{
					int iNewSpec = 0;

					for ( int i = 0; i < psDefItem->JobBitCodeRandomCount; i++ )
					{
						if ( psDefItem->JobBitCodeRandom[i] == iSpec )
						{
							iNewSpec = iSpec;
							break;
						}
					}

					if ( iNewSpec != iSpec )
					{
						if ( psDefItem->sItem.eSpecialization != iSpec )
							iSpec = 0;
					}
				}

				//Create Perfect Item
				ITEMSERVER->CreatePerfectItem( &sItem, psDefItem, EItemSource::CoinShopPerfectItem, ClassFlagToCharacterClass( (EClassFlag)iSpec ) );

				//Discount Coins
				UseCoin( pcUser->pcUserData->szAccountName, iCoins );

				//Delete Old Item
				ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Update Item
				CopyMemory( &pcItemData->sItem, &sItem, sizeof( Item ) );

				//Item NS?
				if ( iSpec == 0 )
				{
					pcItemData->sItem.eSpecialization = 0;
					ZeroMemory( &pcItemData->sItem.sSpecData, sizeof( SpecItemData ) );
				}

				//Upgrade Status
				ITEMSERVER->UpgradeNewItem( &sOldItem, &pcItemData->sItem );

				//Name
				if ( (iItemID & 0xFFFFFF00) != ITEMID_SpecStoneCode )
					STRINGCOPY( pcItemData->sItem.szItemName, sOldItem.szItemName );

				//Validate
				ITEMSERVER->ReformItem( &pcItemData->sItem );

				ITEMSERVER->SaveItemDataToDatabase( &pcItemData->sItem );

				ITEMSERVER->AddItemInventory( pcUser->pcUserData, pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );


				//Success
				psPacket->iRetCode = 1;

				//Send to User
				ProcessPacket( pcUser, psPacket );

				char szItemName[32] = { 0 };
				ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName );

				//Log It
				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_PerfectizedItem, "[%s](%s) Perfectized Item ( [%s][0x%08X] %d x %d -> %d x %d )",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, pcItemData->sItem.sItemID.ToInt(),
					sOldItem.iChk1, sOldItem.iChk2, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Log It...
				SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[Perfectize] Spec[%d] Price[%d] CoinAmount[%d]",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ItemPerfect.iSpec,
					iCoins, GetCoinAmount( pcUser->pcUserData->szAccountName ) ) );

				//Success!
				return TRUE;
			}
			else
			{
				psPacket->iRetCode = -3; //Item Dont exists
				ProcessPacket( pcUser, psPacket );
			}
		}
		else
		{
			CHATSERVER->SendTitleBox( pcUser->pcUserData, "Not enough credits!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Not enough credits!" );
			psPacket->iRetCode = -1; //Not coins
			ProcessPacket( pcUser, psPacket );
		}
	}
	else
	{
		psPacket->iRetCode = -4; //Already Perfect
		ProcessPacket( pcUser, psPacket );
		CHATSERVER->SendTitleBox( pcUser->pcUserData, "This item is already perfect!" );
	}
	//FAIL!
	return FALSE;
}

BOOL CCoinShopHandler::ItemSwapper( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	ItemData * pcItemData = &psPacket->ItemSwapper.cItemData;

	Item sOldItem;
	CopyMemory( &sOldItem, &pcItemData->sItem, sizeof( Item ) );

	//Item cannot be Perfect
	if ( pcItemData->sItem.iItemSpecialType == 1 )
	{
		int iCoins = 0;

		int iLevelItem = pcItemData->sItem.iLevel;
		if ( pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Aging && pcItemData->sItem.sAgeLevel > 0 )
		{
			int iLevelAge = Base::Coin::GetLevelItemEachAge( &pcItemData->sItem );

			if ( iLevelAge > 0 )
				iLevelItem -= (pcItemData->sItem.sAgeLevel / iLevelAge);
		}


		//Set Price
		iCoins = Base::Coin::GetSwapperPrice( &pcItemData->sItem, psPacket->ItemSwapper.iItemBase );

		//Global Discount
		if ( pcCoinShop->GetDiscount() > 0 )
			iCoins = (pcCoinShop->GetDiscount() * iCoins) / 100;

		//Price setted and have Coins?
		if ( iCoins > 0 && HasCoinAmount( pcUser->pcUserData->szAccountName, iCoins ) )
		{
			Item sItem;
			DefinitionItem * psDefItem = ITEMSERVER->FindItemDefByLevelBase( psPacket->ItemSwapper.iItemBase, iLevelItem );

			//Item Exists?
			if ( psDefItem )
			{

				int iSpec = pcItemData->sItem.eSpecialization;

				//Check if Have Spec
				if ( iSpec != 0 )
				{
					int iNewSpec = 0;

					for ( int i = 0; i < psDefItem->JobBitCodeRandomCount; i++ )
					{
						if ( psDefItem->JobBitCodeRandom[i] == iSpec )
						{
							iNewSpec = iSpec;
							break;
						}
					}

					if ( iNewSpec != iSpec )
					{
						if ( psDefItem->sItem.eSpecialization != iSpec )
							iSpec = 0;
					}
				}

				//Create Perfect Item
				ITEMSERVER->CreatePerfectItem( &sItem, psDefItem, EItemSource::CoinShopPerfectItem, ClassFlagToCharacterClass( (EClassFlag)iSpec ) );

				//Discount Coins
				UseCoin( pcUser->pcUserData->szAccountName, iCoins );

				ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Update Item
				CopyMemory( &pcItemData->sItem, &sItem, sizeof( Item ) );

				//Item NS?
				if ( iSpec == 0 )
				{
					pcItemData->sItem.eSpecialization = 0;
					ZeroMemory( &pcItemData->sItem.sSpecData, sizeof( SpecItemData ) );
				}

				//Upgrade Status
				ITEMSERVER->UpgradeNewItem( &sOldItem, &pcItemData->sItem );

				//Validate
				ITEMSERVER->ReformItem( &pcItemData->sItem );

				ITEMSERVER->SaveItemDataToDatabase( &pcItemData->sItem );

				ITEMSERVER->AddItemInventory( pcUser->pcUserData, pcItemData->sItem.sItemID.ToInt(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Success
				psPacket->iRetCode = 1;

				//Send to User
				ProcessPacket( pcUser, psPacket );

				char szItemName[32] = { 0 };
				ITEMSERVER->FindItemName( pcItemData->sItem.sItemID.ToInt(), szItemName );

				//Log It
				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_SwappedItem, "[%s](%s) Swapped Item ( [%s][0x%08X] %d x %d -> %d x %d )",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szItemName, pcItemData->sItem.sItemID.ToInt(),
					sOldItem.iChk1, sOldItem.iChk2, pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Log It...
				SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[ItemSwapper] Spec[%d] Price[%d] CoinAmount[%d]",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ItemPerfect.iSpec,
					iCoins, GetCoinAmount( pcUser->pcUserData->szAccountName ) ) );

				//Success!
				return TRUE;
			}
			else
			{
				psPacket->iRetCode = -3; //Item Dont exists
				ProcessPacket( pcUser, psPacket );
			}
		}
		else
		{
			CHATSERVER->SendTitleBox( pcUser->pcUserData, "Not enough credits!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Not enough credits!" );
			psPacket->iRetCode = -1;
			ProcessPacket( pcUser, psPacket );
		}
	}
	else
	{
		psPacket->iRetCode = -2; //Must be perfect
		ProcessPacket( pcUser, psPacket );
	}

	//FAIL!
	return FALSE;
}

BOOL CCoinShopHandler::ChangeNick( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	if ( HasCoinAmount( pcUser->pcUserData->szAccountName, iChangeNickPrice ) )
	{
		//check allowable chars
		for ( int i = 0; i < lstrlenA( psPacket->ChangeNick.szNewCharacterName ); i++ )
		{
			if ( psPacket->ChangeNick.szNewCharacterName[i] >= 'a' && psPacket->ChangeNick.szNewCharacterName[i] <= 'z' )
				continue;
			if ( psPacket->ChangeNick.szNewCharacterName[i] >= 'A' && psPacket->ChangeNick.szNewCharacterName[i] <= 'Z' )
				continue;
			if (psPacket->ChangeNick.szNewCharacterName[i] >= '0' && psPacket->ChangeNick.szNewCharacterName[i] <= '9' )
				continue;

			if ( psPacket->ChangeNick.szNewCharacterName[i] == '-' ||
				 psPacket->ChangeNick.szNewCharacterName[i] == '_' ||
				 psPacket->ChangeNick.szNewCharacterName[i] == '=' )
			{
				continue;
			}

			CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name not allowed! choose other!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name not allowed! choose other!" );

			psPacket->iRetCode = -3;
			ProcessPacket( pcUser, psPacket );
			return FALSE;
		}

		for ( int i = 0; i < _countof( pszaNotAllowedNamesChangeNick ); i++ )
		{
			if ( strstr( tolower(psPacket->ChangeNick.szNewCharacterName).c_str(), pszaNotAllowedNamesChangeNick[i] ) )
			{
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name not allowed! choose other!" );
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name not allowed! choose other!" );

				psPacket->iRetCode = -3;
				ProcessPacket( pcUser, psPacket );
				return FALSE;
			}
		}

		SQLCharacter s;

		if ( SERVER_IS_SEASONAL )
		{
			if ( CHARACTERSERVER->SQLGetCharacter ( SQLCONNECTION ( EDatabaseID::DATABASEID_UserDB_PrimaryServer ), psPacket->ChangeNick.szNewCharacterName, &s ) == TRUE )
			{
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name already exists!" );
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name already exists!" );
				psPacket->iRetCode = -2;
				ProcessPacket( pcUser, psPacket );
				return FALSE;
			}
		}

		if ( CHARACTERSERVER->SQLGetCharacter ( SQLCONNECTION ( DATABASEID_UserDB_LocalServer_CharInfo ), psPacket->ChangeNick.szNewCharacterName, &s ) == TRUE )
		{
			CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name already exists!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name already exists!" );
			psPacket->iRetCode = -2;
			ProcessPacket( pcUser, psPacket );
			return FALSE;
		}

		//Okay we should be good to go


		//add to local only. on rename action, it will also rename on babel too
		SERVERCOMMAND->AddCommand( SQLSERVERCOMMAND_ChangeCharacterName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ChangeNick.szNewCharacterName, "" );

		UseCoin( pcUser->pcUserData->szAccountName, iChangeNickPrice );

		//Log It...
		SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[ChangeCharacterName] NewCharacterName[%s] Coins[%d]",
			pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ChangeNick.szNewCharacterName, iChangeNickPrice ) );

		psPacket->iRetCode = 1;
		ProcessPacket( pcUser, psPacket );
	}
	else
	{
		CHATSERVER->SendTitleBox( pcUser->pcUserData, "Not enough credits!" );
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Not enough credits!" );
		psPacket->iRetCode = -1;
		ProcessPacket( pcUser, psPacket );
		return FALSE;
	}

	return TRUE;
}

BOOL CCoinShopHandler::AgingRecovery( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	if ( HasCoinAmount( pcUser->pcUserData->szAccountName, iAgingRecoveryPrice ) )
	{
		Item sItem;

		if ( AGEHANDLER->GetItemDataBroke( pcUser, &sItem, psPacket->AgingRecovery.iID ) )
		{
			UseCoin( pcUser->pcUserData->szAccountName, iAgingRecoveryPrice );

			ITEMSERVER->AddItemInventory( pcUser->pcUserData, sItem.sItemID.ToInt(), sItem.iChk1, sItem.iChk2 );

			CopyMemory( &psPacket->AgingRecovery.cItemData.sItem, &sItem, sizeof( Item ) );

			//Log It...
			SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[AgingRecovery] ItemName[%s] ItemID[0x%08X] Code1[%d] Code2[%d] Coins[%d]",
				pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), sItem.szItemName, sItem.sItemID.ToInt(), sItem.iChk1, sItem.iChk2, iAgingRecoveryPrice ) );

			AGEHANDLER->SQLItemBrokeRecovered( pcUser, psPacket->AgingRecovery.iID );

			psPacket->iRetCode = 1;
			ProcessPacket( pcUser, psPacket );
		}
	}
	else
	{
		CHATSERVER->SendTitleBox( pcUser->pcUserData, "Not enough credits!" );
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Not enough credits!" );
		psPacket->iRetCode = -1;
		ProcessPacket( pcUser, psPacket );
	}

	return TRUE;
}

BOOL CCoinShopHandler::ChangeClass( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	int iCoins = 0;

	//Set Price
	iCoins = psPacket->ChangeClass.bPremiumChangeClass == TRUE ? 6500 : 3500;

	//Global Discount
	if ( pcCoinShop->GetDiscount() > 0 )
		iCoins = (pcCoinShop->GetDiscount() * iCoins) / 100;

	//Price setted and have Coins?
	if ( iCoins > 0 && HasCoinAmount( pcUser->pcUserData->szAccountName, iCoins ) )
	{
		if ( SERVERCOMMAND->SQLReadFromCharacter( CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), SQLSERVERCOMMAND_ChangeCharacterClass, SQLServerCommand() ) )
		{
			CHATSERVER->SendTitleBox( pcUser->pcUserData, "You have a change class in the queue!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> You have a change class in the queue!" );
			psPacket->iRetCode = -4;
			ProcessPacket( pcUser, psPacket );
			return FALSE;
		}

		if ( psPacket->ChangeClass.szNewCharacterName[0] != 0 )
		{
			if ( CHARACTERSERVER->SQLGetCharacterClanID( psPacket->ChangeClass.szNewCharacterName ) == (-1) )
			{
				for ( int i = 0; i < _countof( pszaNotAllowedNamesChangeNick ); i++ )
				{
					if ( strstr( tolower( psPacket->ChangeClass.szNewCharacterName ).c_str(), pszaNotAllowedNamesChangeNick[i] ) )
					{
						CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name not allowed! choose other!" );
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name not allowed! choose other!" );

						psPacket->iRetCode = -3;
						ProcessPacket( pcUser, psPacket );
						return FALSE;
					}
				}

				SERVERCOMMAND->AddCommand( SQLSERVERCOMMAND_ChangeCharacterName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ChangeClass.szNewCharacterName, "" );

				//Log It...
				SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[ChangeCharacterName] NewCharacterName[%s] Change Class",
					pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psPacket->ChangeClass.szNewCharacterName ) );
			}
			else
			{
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Character Name already exists!" );
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Character Name already exists!" );
				psPacket->iRetCode = -2;
				ProcessPacket( pcUser, psPacket );
				return FALSE;
			}
		}
		else
			STRINGCOPY( psPacket->ChangeClass.szNewCharacterName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		char szClass[32] = { 0 };
		STRINGFORMAT( szClass, "%d", psPacket->ChangeClass.iClass );

		char szItemID[32] = { 0 };
		STRINGFORMAT( szItemID, "JV1%d", psPacket->ChangeClass.iClass + 10 );

		UseCoin( pcUser->pcUserData->szAccountName, iCoins );

		SERVERCOMMAND->AddCommand( SQLSERVERCOMMAND_ChangeCharacterClass, psPacket->ChangeClass.szNewCharacterName, szClass, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		if ( psPacket->ChangeClass.bPremiumChangeClass )
			ITEMSERVER->AddItemOpenBox( pcUser->pcUserData->szAccountName, szItemID, 0, 8 );

		//Log It...
		SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] Bought[ChangeCharacterClass] NewClass[%d] Premium[%s]",
			pcUser->pcUserData->szAccountName, psPacket->ChangeClass.szNewCharacterName, psPacket->ChangeClass.iClass, psPacket->ChangeClass.bPremiumChangeClass ? "Yes" : "No" ) );

		psPacket->iRetCode = 1;
		ProcessPacket( pcUser, psPacket );
	}

	return TRUE;
}


void CCoinShopHandler::ProcessPacket( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinServiceBuy );
	psPacket->iHeader = PKTHDR_CoinServiceBuy;
	SENDPACKET( pcUser, psPacket, TRUE );
}

void CCoinShopHandler::ProcessPacket( User * pcUser, struct PacketCoinShopNPC * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinShopNPC );
	psPacket->iHeader = PKTHDR_CoinShopNPC;
	SENDPACKET( pcUser, psPacket );
}

void CCoinShopHandler::ProcessPacket( User * pcUser, struct PacketCoinAmount * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinAmount );
	psPacket->iHeader = PKTHDR_CoinShopAmount;
	SENDPACKET( pcUser, psPacket );
}

void CCoinShopHandler::ProcessPacket( User * pcUser, struct PacketCoinOpenTab * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinOpenTab );
	psPacket->iHeader = PKTHDR_CoinShopOpenTab;
	SENDPACKET( pcUser, psPacket );
}

void CCoinShopHandler::ProcessPacket( User * pcUser, struct PacketCoinTab * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinTab );
	psPacket->iHeader = PKTHDR_CoinShopTab;
	SENDPACKET( pcUser, psPacket );
}

void CCoinShopHandler::ProcessPacket( User * pcUser, struct PacketCoinItemData * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinItemData );
	psPacket->iHeader = PKTHDR_CoinShopItemData;
	SENDPACKET( pcUser, psPacket );
}

void CCoinShopHandler::HandlePacket( User * pcUser, PacketCoinShopNPC * psPacket )
{
	if( psPacket->iLength != sizeof( PacketCoinShopNPC ) )
		return;

	if( GAME_SERVER )
		return;

	if( !pcUser->IsInGame() )
		return;

	if( pcCoinShop == NULL )
		return;

	//Fill Packet

	psPacket->sPacketNumber		= 0;
	psPacket->sPacketMax		= (short)(pcCoinShop->GetTabs().size() / MAX_COINSHOPTABS) + 1;

	//ID
	psPacket->iID = pcCoinShop->GetID();

	//Message
	STRINGCOPY( psPacket->szName, pcCoinShop->GetName() );
	STRINGCOPY( psPacket->szMessage, pcCoinShop->GetMessage() );

	//Discount Percentile
	psPacket->iDiscount = pcCoinShop->GetDiscount();

	//Tabs
	psPacket->iTabs = 0;

	std::vector<CCoinShopTab*> & vTabs = pcCoinShop->GetTabs();
	for( std::vector<CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;

		//Add Tab to the Packet
		int iTab = psPacket->iTabs;
		if( iTab >= MAX_COINSHOPTABS )
		{
			psPacket->sPacketNumber++;

			//Send Packet
			ProcessPacket( pcUser, psPacket );

			//Reset Packet
			iTab = psPacket->iTabs = 0;
		}

		if( iTab < MAX_COINSHOPTABS )
		{
			psPacket->aTabs[iTab].iID = pcTab->GetID();
			psPacket->aTabs[iTab].iParentTabID = pcTab->GetParentTabID();

			STRINGCOPY( psPacket->aTabs[iTab].szName, pcTab->GetName() );

			psPacket->aTabs[iTab].iDiscount = pcTab->GetDiscount();
			psPacket->aTabs[iTab].iBulk = pcTab->GetBulk();
			psPacket->aTabs[iTab].iMaxBulk = pcTab->GetMaxBulk();

			psPacket->iTabs++;
		}

		//Add Child Tabs to the Packet
		std::vector<CCoinShopTab*> & vChildTabs = pcTab->GetChildTabs();
		for( std::vector<CCoinShopTab*>::iterator it = vChildTabs.begin(); it != vChildTabs.end(); it++ )
		{
			CCoinShopTab * pcChildTab = *it;

			int iParentTab = psPacket->iTabs;
			if( iParentTab >= MAX_COINSHOPTABS )
			{
				psPacket->sPacketNumber++;

				//Send Packet
				ProcessPacket( pcUser, psPacket );

				//Reset Packet
				iParentTab = psPacket->iTabs = 0;
			}

			if( iParentTab < MAX_COINSHOPTABS )
			{
				psPacket->aTabs[iParentTab].iID = pcChildTab->GetID();
				psPacket->aTabs[iParentTab].iParentTabID = pcChildTab->GetParentTabID();

				STRINGCOPY( psPacket->aTabs[iParentTab].szName, pcChildTab->GetName() );

				psPacket->aTabs[iParentTab].iDiscount = pcChildTab->GetDiscount();
				psPacket->aTabs[iParentTab].iBulk = pcChildTab->GetBulk();
				psPacket->aTabs[iParentTab].iMaxBulk = pcChildTab->GetMaxBulk();

				psPacket->iTabs++;
			}
		}
	}

	psPacket->sPacketNumber++;

	//Send Packet
	ProcessPacket( pcUser, psPacket );
}

void CCoinShopHandler::HandlePacket( User * pcUser, PacketCoinOpenTab * psPacket )
{
	if( psPacket->iLength != sizeof( PacketCoinOpenTab ) )
		return;

	if( GAME_SERVER )
		return;

	if( !pcUser->IsInGame() )
		return;

	if( pcCoinShop == NULL )
		return;

	CCoinShopTab * pcTab = pcCoinShop->GetTabByID( psPacket->iID );

	if( pcTab == NULL )
	{
		psPacket->iReturnCode = -1;
		ProcessPacket( pcUser, psPacket );

		return;
	}

	PacketCoinTab sPacket;

	//Fill Packet

	sPacket.sPacketNumber	= 0;
	sPacket.sPacketMax		= (short)(pcTab->GetItems().size() / MAX_COINSHOPITEMS) + 1;

	//ID
	sPacket.iID = pcTab->GetID();

	//Items
	sPacket.iItems = 0;

	std::vector<CCoinShopItem*> & vItems = pcTab->GetItems();
	for( std::vector<CCoinShopItem*>::iterator it = vItems.begin(); it != vItems.end(); it++ )
	{
		CCoinShopItem * pcItem = *it;

		//Add Item to the Packet
		int iItem = sPacket.iItems;
		if( iItem >= MAX_COINSHOPITEMS )
		{
			sPacket.sPacketNumber++;

			//Send Packet
			ProcessPacket( pcUser, &sPacket );

			//Reset
			iItem = sPacket.iItems = 0;
		}

		if( iItem < MAX_COINSHOPITEMS )
		{
			sPacket.aItems[iItem].iID = pcItem->GetID();
			STRINGCOPY( sPacket.aItems[iItem].szName, pcItem->GetName() );
			STRINGCOPY( sPacket.aItems[iItem].szDescription, pcItem->GetDescription() );
			STRINGCOPY( sPacket.aItems[iItem].szCode, pcItem->GetCode() );
			STRINGCOPY( sPacket.aItems[iItem].szImage, pcItem->GetImage() );
			sPacket.aItems[iItem].iValue = pcItem->GetValue();
			sPacket.aItems[iItem].iDiscount = pcItem->GetDiscount();
			sPacket.aItems[iItem].iBulk = pcItem->GetBulk();
			sPacket.aItems[iItem].iMaxBulk = pcItem->GetMaxBulk();

			sPacket.iItems++;
		}
	}

	sPacket.sPacketNumber++;

	//Send Packet
	ProcessPacket( pcUser, &sPacket );
}


void CCoinShopHandler::HandlePacket( User * pcUser, PacketCoinAmount * psPacket )
{
	psPacket->iCoinAmount = GetCoinAmount( pcUser->pcUserData->szAccountName );
	ProcessPacket( pcUser, psPacket );
}

void CCoinShopHandler::HandlePacket( User * pcUser, PacketCoinOpenItemData * psPacket )
{
	CCoinShopTab * pcTab = pcCoinShop->GetTabByID( psPacket->iTabID );
	if ( pcTab )
	{
		CCoinShopItem * pcItem = pcTab->GetItemByID( psPacket->iID );

		if ( pcItem )
		{
			int iPrice = pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue();

			//Perfectize?
			if ( STRINGCOMPAREI( pcItem->GetCode(), "Perf" ) )
			{
				PacketCoinOpenService s;
				s.iLength = sizeof( PacketCoinOpenService );
				s.iHeader = PKTHDR_CoinServiceOpen;
				s.iID = ITEMCOIN_ItemPerfect;
				s.iPrice = iPrice;
				SENDPACKET( pcUser, &s, TRUE );
				return;
			}

			//Swap?
			if ( STRINGCOMPAREI( pcItem->GetCode(), "Swap" ) )
			{
				PacketCoinOpenService s;
				s.iLength = sizeof( PacketCoinOpenService );
				s.iHeader = PKTHDR_CoinServiceOpen;
				s.iID = ITEMCOIN_ItemSwapper;
				s.iPrice = iPrice;
				SENDPACKET( pcUser, &s, TRUE );
				return;
			}

			//ChangeNick?
			if ( STRINGCOMPAREI( pcItem->GetCode(), "CNick" ) )
			{
				PacketCoinOpenService s;
				s.iLength = sizeof( PacketCoinOpenService );
				s.iHeader = PKTHDR_CoinServiceOpen;
				s.iID = ITEMCOIN_ChangeNick;
				s.iPrice = iPrice;
				iChangeNickPrice = iPrice;
				SENDPACKET( pcUser, &s, TRUE );
				return;
			}

			//Aging Recovery?
			if ( STRINGCOMPAREI( pcItem->GetCode(), "AgeRec" ) )
			{
				PacketOpenAgingRecoveryService s;
				s.iLength = sizeof( PacketOpenAgingRecoveryService );
				s.iHeader = PKTHDR_CoinServiceAgingRecoveryOpen;
				s.iPrice  = iPrice;
				iAgingRecoveryPrice = iPrice;

				std::vector<AgingRecoveryDataHandle> vData = AGEHANDLER->SQLGetBrokeItems( pcUser->pcUserData->szAccountName );

				s.iCount		= 0;
				s.sPacketNumber = 0;
				s.sPacketMax	= (short)(vData.size() / MAX_AGINGRECOVERY_PER_PACKET) + 1;

				for ( UINT i = 0; i < vData.size(); i++ )
				{
					CopyMemory( &s.saData[s.iCount], &vData[i], sizeof( AgingRecoveryDataHandle ) );
					s.iCount++;

					if ( s.iCount >= MAX_AGINGRECOVERY_PER_PACKET )
					{
						s.sPacketNumber++;

						SENDPACKET( pcUser, &s, TRUE );

						s.iCount = 0;
					}
				}

				s.sPacketNumber++;

				SENDPACKET( pcUser, &s, TRUE );

				return;
			}

			//Change Class?
			if ( STRINGCOMPAREI( pcItem->GetCode(), "CClass" ) )
			{
				PacketCoinOpenService s;
				s.iLength = sizeof( PacketCoinOpenService );
				s.iHeader = PKTHDR_CoinServiceOpen;
				s.iID = ITEMCOIN_ChangeClass;
				s.iPrice = iPrice;
				SENDPACKET( pcUser, &s, TRUE );
				return;
			}

			PacketCoinItemData sPacket;
			sPacket.iID			= psPacket->iID;
			sPacket.iTabID		= psPacket->iTabID;
			sPacket.iType		= ITEMCOIN_Normal;

			// Find Item Data by Code
			DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( pcItem->GetCode() );
			if ( psItemDef )
			{
				// Create Item Data
				CopyMemory( &sPacket.sItemDefinition, psItemDef, sizeof( DefinitionItem ) );

				sPacket.iType = ITEMCOIN_Data;
			}

			sPacket.bSpec		= pcItem->GetIsSpec();
			sPacket.bQuantity	= pcItem->GetIsQuantity();

			ProcessPacket( pcUser, &sPacket );
		}
	}
}

void CCoinShopHandler::HandlePacket( User * pcUser, struct PacketCoinBuy * psPacket )
{
	PacketCoinAmount sPacketCoinAmount;
	CCoinShopTab * pcTab = pcCoinShop->GetTabByID( psPacket->iTabID );
	psPacket->iReturnCode = 0;
	if ( pcTab )
	{
		CCoinShopItem * pcItem = pcTab->GetItemByID( psPacket->iID );
		if ( pcItem )
		{
			int iValue = pcItem->GetDiscount() > 0 ? pcItem->GetDiscount() : pcItem->GetValue();

			if ( pcItem->GetIsQuantity() )
			{
				if ( psPacket->iParam2 <= 0 )
					psPacket->iParam2 = 1;

				int iBulk = GetBulkValue( pcItem, psPacket->iParam2 );

				if ( HasCoinAmount( pcUser->pcUserData->szAccountName, (iValue * psPacket->iParam2) - iBulk ) )
				{
					ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, pcItem->GetCode(), pcItem->GetIsSpec() == TRUE ? psPacket->iParam1 : 0, psPacket->iParam2, TRUE );
					sPacketCoinAmount.iCoinAmount = UseCoin( pcUser->pcUserData->szAccountName, (iValue * psPacket->iParam2) - iBulk );
					// Log
					SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Bought[%s] Spec[%d] Quantity[%d] Price[%d] CoinAmount[%d]",
						pcUser->pcUserData->szAccountName, pcItem->GetCode(), pcItem->GetIsSpec() == TRUE ? psPacket->iParam1 : 0,
						psPacket->iParam2, (iValue * psPacket->iParam2) - iBulk, sPacketCoinAmount.iCoinAmount ) );


					psPacket->iReturnCode = 1;
				}
			}
			else
			{
				if ( HasCoinAmount( pcUser->pcUserData->szAccountName, iValue ) )
				{
					ITEMSERVER->AddItemOpenBox( pcUser->pcUserData, pcItem->GetCode(), pcItem->GetIsSpec() == TRUE ? psPacket->iParam1 : 0, 1, TRUE );
					sPacketCoinAmount.iCoinAmount = UseCoin( pcUser->pcUserData->szAccountName, iValue );

					// Log
					SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Bought[%s] Spec[%d] Price[%d] CoinAmount[%d]",
						pcUser->pcUserData->szAccountName, pcItem->GetCode(), pcItem->GetIsSpec() == TRUE ? psPacket->iParam1 : 0,
						iValue, sPacketCoinAmount.iCoinAmount ) );


					psPacket->iReturnCode = 1;
				}
			}
		}
		else
			psPacket->iReturnCode = -2;
	}
	else
		psPacket->iReturnCode = -1;

	SENDPACKET( pcUser, psPacket );

	// Coin Amount
	if ( psPacket->iReturnCode == 1 )
		ProcessPacket( pcUser, &sPacketCoinAmount );

}

void CCoinShopHandler::HandlePacket( User * pcUser, PacketCoinServiceBuy * psPacket )
{
	if ( pcUser && pcUser->pcUserData )
	{
		switch ( psPacket->iType )
		{
			case ITEMCOIN_ItemSwapper:
				ItemSwapper( pcUser, psPacket );
				break;
			case ITEMCOIN_ItemPerfect:
				PerfectizeItem( pcUser, psPacket );
				break;
			case ITEMCOIN_ChangeNick:
				ChangeNick( pcUser, psPacket );
				break;
			case ITEMCOIN_AgingRecovery:
				AgingRecovery( pcUser, psPacket );
				break;
			case ITEMCOIN_ChangeClass:
				ChangeClass( pcUser, psPacket );
				break;
			default:
				break;
		}

		//Update Coins
		ProcessPacket( pcUser, &PacketCoinAmount( GetCoinAmount( pcUser->pcUserData->szAccountName ) ) );
	}
}

void CCoinShopHandler::HandlePacket( User * pcUser, PacketTradeCoinSet * psPacket )
{
	/// ENABLED COIN TRADE
	//return;

	if ( psPacket->iCoins >= 0 )
	{
		if ( HasCoinAmount( pcUser->pcUserData->szAccountName, psPacket->iCoins ) )
		{
			pcUser->iTradeCoin = psPacket->iCoins;

			User * pcPartner = USERDATATOUSER( USERDATABYID( psPacket->iID ) );

			if ( pcPartner )
				SENDPACKET( pcPartner, psPacket );
		}
	}
}

