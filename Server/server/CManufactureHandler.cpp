#include "stdafx.h"
#include "CManufactureHandler.h"


CManufactureHandler::CManufactureHandler()
{
}


CManufactureHandler::~CManufactureHandler()
{
}

void CManufactureHandler::HandlePacket( User * pcUser, PacketManufactureItem * psPacket )
{
	if (EVENT_CHRISTMAS)
	{
		if (!CHRISTMASHANDLER->OnManufactureItem(pcUser, psPacket))
		{
			psPacket->iResult = FALSE;
			SENDPACKET(pcUser, psPacket);
			return;
		}
	}
	else if (EVENT_EASTER)
	{
		if (!GENERALEVENTHANDLER->OnManufactureItem(pcUser, psPacket))
		{
			psPacket->iResult = FALSE;
			SENDPACKET(pcUser, psPacket);
			return;
		}
	}
	else
	{
		psPacket->iResult = FALSE;
		SENDPACKET(pcUser, psPacket);
		return;
	}
}

BOOL CManufactureHandler::DeleteItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	LogItem sLog;
	ZeroMemory( &sLog, sizeof( LogItem ) );

	if (!ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, iItemID, iChk1, iChk2 ))
	{
		STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
		STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		sLog.ItemCount = 1;
		sLog.Flag = ITEMLOGID_ManufactureFail;
		sLog.Item[0].dwCode = iItemID;
		sLog.Item[0].dwHead = iChk1;
		sLog.Item[0].dwCheckSum = iChk2;
		LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
		return FALSE;
	}

	return TRUE;
}

BOOL CManufactureHandler::DeleteRunes( User * pcUser, EItemID * piaItemID, int * piaChk1, int * piaChk2 )
{
	for ( int i = 0; i < 3; i++ )
	{
		if ( piaItemID[i] != ITEMID_None )
		{
			if ( DeleteItem( pcUser, piaItemID[i], piaChk1[i], piaChk2[i] ) == FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CManufactureHandler::AddItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, iItemID, iChk1, iChk2 ) )
	{
		LogItem sLog;
		ZeroMemory( &sLog, sizeof( LogItem ) );

		STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
		STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		sLog.ItemCount = 1;
		sLog.Flag = ITEMLOGID_ManufactureGet;
		sLog.Item[0].dwCode = iItemID;
		sLog.Item[0].dwHead = iChk1;
		sLog.Item[0].dwCheckSum = iChk2;
		LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

		return TRUE;
	}

	return FALSE;
}

BOOL CManufactureHandler::AddItem( User * pcUser, Item * psItem )
{
	if ( AddItem( pcUser, psItem->sItemID.ToItemID(), psItem->iChk1, psItem->iChk2 ) )
	{
		ITEMSERVER->SaveItemDataToDatabase( psItem );

		return TRUE;
	}

	return FALSE;
}

BOOL CManufactureHandler::HaveItem( PacketManufactureItem * psPacket, EItemID iItemRecipeID, EItemID iItemSlot1ID, int iCount1, EItemID iItemSlot2ID, int iCount2, EItemID iItemSlot3ID, int iCount3 )
{
	BOOL bRet = FALSE;

	if ( (psPacket->iRecipeID == iItemRecipeID) || (iItemRecipeID == ITEMID_None) )
	{
		for ( int i = 0; i < 3; i++ )
		{
			if ( (iItemSlot1ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot1ID) )
				iCount1--;
			if ( (iItemSlot2ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot2ID) )
				iCount2--;
			if ( (iItemSlot3ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot3ID) )
				iCount3--;
		}

		if ( (iCount1 + iCount2 + iCount3) == 0 )
			bRet = TRUE;
	}

	return bRet;
}

BOOL CManufactureHandler::SwapItem( User * pcUser, PacketManufactureItem * psPacket, EItemID iNewItemID, BOOL bSameSpec, BOOL bPerfect )
{
	DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( iNewItemID );
	if ( psItemDef )
	{
		if ( DeleteItem( pcUser, psPacket->iRecipeID, psPacket->iRecipeChk1, psPacket->iRecipeChk2 ) )
		{
			int iClass = bSameSpec ? ClassFlagToCharacterClass( (EClassFlag)psPacket->sItemData.sItem.eSpecialization ) : 0;

			if ( bPerfect && psPacket->sItemData.sItem.iItemSpecialType == 1 )
				ITEMSERVER->CreatePerfectItem( &psPacket->sItemData.sItem, psItemDef, EItemSource::ManufactureSwap, iClass );
			else
				ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef, EItemSource::ManufactureSwap, iClass );

			AddItem( pcUser, &psPacket->sItemData.sItem );

			psPacket->iResult = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}
