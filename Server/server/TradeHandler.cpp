#include "stdafx.h"
#include "TradeHandler.h"


CTradeHandler::CTradeHandler()
{
}


CTradeHandler::~CTradeHandler()
{
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeData * psPacket )
{
	/// Disable COIN TRADING


	pcUser->TradeWindowOpen = true;
	//std::cout << "PacketTradeData TradeWindowOpen true" << std::endl;

	TradeDataItem sData;
	ZeroMemory( &sData, sizeof( TradeDataItem ) );

	UserData * pcUserData	= pcUser->pcUserData;

	char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUserData );

	//Decode Trade Data
	ITEMSERVER->DecodeItemsData( psPacket->baData, &sData, 0x7FFFF );

	//Log All Items on Trade
	for ( int i = 0; i < 20; i++ )
	{
		ItemData * pcItemData		= &sData.cItemData[i];

        if (!ITEMSERVER->IsPotion(pcItemData->sItem.sItemID.ToItemID()))
        {
//             bool l_ItemFound = std::count_if(pcUser->OnTradeItems.begin(), pcUser->OnTradeItems.end(), [&](const DropItemData & p_Data) {
//                 return p_Data.iItemID == pcItemData->sItem.sItemID.ToItemID() && p_Data.iChk1 == pcItemData->sItem.iChk1 && p_Data.iChk2 == pcItemData->sItem.iChk2;
//             }) >= 1;
//
//             if (!l_ItemFound)
//             {
//                 SENDPACKETBLANK(USERDATATOUSER(pcUserData), PKTHDR_Disconnect);
//                 WNDPROC_SOCKETCLOSE(SOCKETSERVER->GetHWND(), pcUserData->pcSocketData);
//
//                 NETSERVER->DisconnectUser(pcUserData);
//
//                 return;
//             }
        }

		DropItemData * psItemTrade	= pcUser->saTradeItems + i;

		//Valid Item?
		if ( pcItemData->bValid )
		{
			//Not same old item?
			if ( (psItemTrade->iItemID != pcItemData->sItem.sItemID.ToItemID() ||
				psItemTrade->iChk1    != pcItemData->sItem.iChk1 ||
				psItemTrade->iChk2    != pcItemData->sItem.iChk2) )
			{
				//Log It
				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put On Trade ( [%s][0x%08X] %d x %d )",
					pcUserData->szAccountName, pszCharacterName, pcItemData->sItem.szItemName, pcItemData->sItem.sItemID.ToItemID(),
					pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Add To User
				psItemTrade->iItemID = pcItemData->sItem.sItemID.ToItemID();
				psItemTrade->iChk1	= pcItemData->sItem.iChk1;
				psItemTrade->iChk2	= pcItemData->sItem.iChk2;
			}
		}
		else if ( pcItemData->sBaseItemID.ToItemID() == 0 && psItemTrade->iItemID != 0 ) // Removed Item?
		{

			//Get Item Name
			char szItemName[32] = { 0 };
			ITEMSERVER->FindItemName( psItemTrade->iItemID, szItemName );

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemsRemove, "[%s](%s) Removed from Trade ( [%s][0x%08X] %d x %d )",
				pcUserData->szAccountName, pszCharacterName, szItemName, psItemTrade->iItemID,
				pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

			//Add To User
			psItemTrade->iItemID = 0;
			psItemTrade->iChk1	= 0;
			psItemTrade->iChk2	= 0;
		}
	}

	int iGold = sData.iGold - 193;

	if (iGold < 0)
		iGold = 0;

	//std::cout << "Trade data igold " << iGold << " GetGold() " << pcUserData->GetGold() << " tradegold " << pcUser->iTradeGold << std::endl;

	if (iGold > pcUserData->GetGold())
	{
		LOGSERVER->OnLogItemEx(pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put on Trade ( [FAKE_Gold][%d] )",
			pcUserData->szAccountName, pszCharacterName, iGold);

		PacketSetCharacterGold sGoldPacket;
		sGoldPacket.iHeader = PKTHDR_SetGold;
		sGoldPacket.iLength = sizeof(PacketSetCharacterGold);
		sGoldPacket.dwGold	= pcUserData->GetGold();

		PACKETSERVER->Send(pcUserData, &sGoldPacket);

		PacketLogCheat sPacket;
		sPacket.iCheatID	= CHEATLOGID_TradeGoldHack;
		sPacket.SParam      = pcUserData->GetGold();
		sPacket.LParam      = iGold;

		LOGSERVER->OnLogCheat(pcUserData, &sPacket);

		LOGERROR("[PLAYER] Packet spoofing %s", pcUserData->pcSocketData->szIP);

		if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
		{
			SENDPACKETBLANK( USERDATATOUSER( pcUserData ), PKTHDR_Disconnect );
			WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUserData->pcSocketData );

			NETSERVER->DisconnectUser( pcUserData );
		}

		sData.iGold = 0 + 193;

		return;
	}

	//Gold
	if ( pcUser->iTradeGold > 0 )
	{
		//Added More Gold?
		if ( iGold > pcUser->iTradeGold )
		{
			int iDifference = iGold - pcUser->iTradeGold;

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put on Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iDifference, iGold );

			pcUser->iTradeGold = iGold;
		}
		else if ( iGold < pcUser->iTradeGold )
		{
			int iDifference = pcUser->iTradeGold - iGold;

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemsRemove, "[%s](%s) Removed from Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iDifference, iGold );

			pcUser->iTradeGold = iGold;
		}
	}
	else
	{
		//Added Gold?
		if ( iGold > 0 )
		{
			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put on Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iGold, iGold );

			pcUser->iTradeGold = iGold;
		}
	}
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeRequest * psPacket )
{
	User * pcPartner = USERDATATOUSER( USERDATABYID( psPacket->iReceiverID ) );

	if ( pcPartner && pcPartner->pcUserData->pcSocketData )
	{
		if ( psPacket->iType == 2 )
		{
			PacketCoinAmount s;

			pcUser->iTradeCoin = 0;
			pcPartner->iTradeCoin = 0;

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount( pcUser->pcUserData->szAccountName );
			COINSHOPHANDLER->ProcessPacket( pcUser, &s );

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount(pcPartner->pcUserData->szAccountName);
			COINSHOPHANDLER->ProcessPacket( pcPartner, &s );

			PacketCoinsTraded s1;

			s1.iLength = sizeof( PacketCoinsTraded );
			s1.iHeader = PKTHDR_CoinTraded;

			s1.iCoinsTraded = COINSHOPHANDLER->GetCoinTradeAmount( pcUser->pcUserData->szAccountName );
			SENDPACKET( pcUser, &s1 );

			s1.iCoinsTraded = COINSHOPHANDLER->GetCoinTradeAmount( pcPartner->pcUserData->szAccountName );
			SENDPACKET( pcPartner, &s1 );

			//Gold
			pcUser->iTradeCoin              = 0;
			pcUser->TradeWindowOpen         = true;
            pcUser->WasInTwoSideTrade       = true;

			pcPartner->iTradeCoin           = 0;
			pcPartner->TradeWindowOpen      = true;
            pcPartner->WasInTwoSideTrade    = true;
		}
	}

	if (psPacket->iType == 3)
	{
		pcUser->iTradeCoin      = 0;
		pcUser->TradeWindowOpen = false;

		if (pcPartner && pcPartner->pcUserData->pcSocketData)
		{
			pcPartner->iTradeCoin           = 0;
			pcPartner->TradeWindowOpen      = false;
            pcPartner->WasInTwoSideTrade    = false;
		}

        /// Strange ?
        if (!pcUser->WasInTwoSideTrade)
        {
			if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
			{
				/// Kick to avoid dup item to be saved
				SENDPACKETBLANK( pcUser, PKTHDR_Disconnect );
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );

				NETSERVER->DisconnectUser( pcUser->pcUserData );
			}
        }

        pcUser->WasInTwoSideTrade = false;

		//std::cout << "TradeRequest 3 TradeWindowOpen flase" << std::endl;
	}
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeSuccess * psPacket )
{
	User * pcPartner = USERDATATOUSER( USERDATABYID( psPacket->iReceiverID ) );

	pcUser->iTradeCoin          = 0;
	pcUser->TradeWindowOpen     = false;
    pcUser->WasInTwoSideTrade   = false;

	if (pcPartner)
	{
		pcPartner->iTradeCoin           = 0;
		pcPartner->TradeWindowOpen      = false;
        pcPartner->WasInTwoSideTrade    = false;
	}

	//std::cout << "PacketTradeSuccess  TradeWindowOpen flase" << std::endl;

	if ( pcPartner && pcPartner->pcUserData->pcSocketData )
	{
		NETSERVER->SendTradeCoin(pcUser->pcUserData->iID, pcPartner->pcUserData->iID);

		//l_NetPacket.ObjectIDB			= pcPartner->pcUserData->iID;
		//l_NetPacket.TradeWindowOpenB	= false;

		TRADEHANDLER->HandleTradeCoin(pcUser, pcPartner);

/*
		UserData * pcUserData = pcUser->pcUserData;

		TradeDataItem sData;
		ZeroMemory( &sData, sizeof( TradeDataItem ) );

		char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUserData );

		char * pszReceiverName = CHARACTERSERVER->GetCharacterName( pcPartner->pcUserData );

		//Decode Trade Data
		ITEMSERVER->DecodeItemsData( psPacket->baData, &sData, 0x7FFFF );

		//Gold
		if ( sData.iGold > 0 )
		{
			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemSuccess, "[%s](%s) Traded item to [%s](%s)( [Gold][%d] )",
				pcUserData->szAccountName, pszCharacterName, pcPartner->pcUserData->szAccountName, pszReceiverName, sData.iGold );

			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemSuccessReceive, "[%s](%s) Received Trade item from [%s](%s)( [Gold][%d] )",
				pcPartner->pcUserData->szAccountName, pszReceiverName, pcUserData->szAccountName, pszCharacterName, sData.iGold );
		}
*/
	}
}

void CTradeHandler::HandleTradeCoin( User * pcUser, User * pcPartner )
{
	if ( pcPartner && pcPartner->pcUserData->pcSocketData && pcUser && pcUser->pcUserData->pcSocketData )
	{
		//Trade Coins...
		if ( pcUser->iTradeCoin && COINSHOPHANDLER->HasCoinAmount( pcUser->pcUserData->szAccountName, pcUser->iTradeCoin * 0.90) )
		{
			//Add Coin to Partner...
			COINSHOPHANDLER->AddCoin( pcPartner->pcUserData->szAccountName, pcUser->iTradeCoin * 0.90 );

			//Discount from user...
			COINSHOPHANDLER->UseCoin( pcUser->pcUserData->szAccountName, pcUser->iTradeCoin );
			COINSHOPHANDLER->AddTradeCoin( pcUser->pcUserData->szAccountName, pcUser->iTradeCoin );

			CHATSERVER->SendChatEx(pcPartner, EChatColor::CHATCOLOR_Trade, "> 10 percent of the coins was taxed.");

			CHATSERVER->SendChatEx( pcPartner, EChatColor::CHATCOLOR_Trade, "> You received %s credits from %s!", FormatNumber( pcUser->iTradeCoin * 0.90), pcUser->pcUserData->sCharacterData.szName );
			CHATSERVER->SendChatEx( pcPartner, EChatColor::CHATCOLOR_Trade, "> You now have %s credits!", FormatNumber( COINSHOPHANDLER->GetCoinAmount( pcPartner->pcUserData->szAccountName ) ) );

			CHATSERVER->SendChatEx(pcUser, EChatColor::CHATCOLOR_Trade, "> 10% of the coins was taxed.");

			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Trade, "> You sent %s credits to %s!", FormatNumber( pcUser->iTradeCoin * 0.90), pcPartner->pcUserData->sCharacterData.szName );
			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Trade, "> You now have %s credits!", FormatNumber( COINSHOPHANDLER->GetCoinAmount( pcUser->pcUserData->szAccountName ) ) );

			//Coin Log
			COINSHOPHANDLER->SQLCoinLog( pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] CoinAmount[%d] Sent Coin[%d] To AccountName[%s] Character[%s] CoinAmount[%d]",
				pcUser->pcUserData->szAccountName, pcUser->pcUserData->sCharacterData.szName, COINSHOPHANDLER->GetCoinAmount( pcUser->pcUserData->szAccountName ), pcUser->iTradeCoin,
				pcPartner->pcUserData->szAccountName, pcPartner->pcUserData->sCharacterData.szName, COINSHOPHANDLER->GetCoinAmount( pcPartner->pcUserData->szAccountName ) ) );

			//Notify new value
			PacketCoinAmount s;

			pcUser->iTradeCoin = 0;

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount( pcUser->pcUserData->szAccountName );
			COINSHOPHANDLER->ProcessPacket( pcUser, &s );

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount( pcPartner->pcUserData->szAccountName );
			COINSHOPHANDLER->ProcessPacket( pcPartner, &s );
		}
	}
}

void CTradeHandler::Tick1s()
{
	SYSTEMTIME * ps = GetServerTime();

	if ( wDay != ps->wDay )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_PrimaryServer_2 );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE UserInfo SET CoinsTraded=0" ) )
			{
				pcDB->ExecuteUpdate();
			}
			pcDB->Close();
		}

		wDay = ps->wDay;
	}
}

BOOL CTradeHandler::IsInTrade( User * pcUser, int iItemID, DWORD dwHead, DWORD dwCheckSum )
{

	return FALSE;
}
