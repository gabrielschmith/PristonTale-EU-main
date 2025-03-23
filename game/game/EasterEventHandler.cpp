#include "stdafx.h"
#include "EasterEventHandler.h"

CEasterEventHandler::CEasterEventHandler()
{
}

CEasterEventHandler::~CEasterEventHandler()
{
}

void CEasterEventHandler::HandlePacket( PacketNPCEasterEgg * psPacket )
{
	//Open NPC
	TEXTWINDOW_OPEN( QUESTID_EasterNPC, "game\\npcs\\easter\\main.txt" );
}

/**
For Manufacturing - for Easter Recipe
*/
BOOL CEasterEventHandler::IsValidRune(class ItemData* pcItem)
{
	switch (pcItem->sItem.sItemID.ToItemID())
	{
		case ITEMID_GreenEasterEgg:
		case ITEMID_PinkEasterEgg:
		case ITEMID_RedEasterEgg:
			return TRUE;
	}

	return FALSE;
}

void CEasterEventHandler::OnClickOKNPC()
{
	int iCount = ITEMHANDLER->GetItemAmountByCode( ITEMID_RedEasterEgg ) + ITEMHANDLER->GetItemAmountByCode( ITEMID_GreenEasterEgg ) + ITEMHANDLER->GetItemAmountByCode( ITEMID_PinkEasterEgg );

	if ( iCount >= EASTER_ITEMS_MAX )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Are you sure?" );
		MESSAGEBOX->SetDescription( "Do you like to exchange the eggs?" );
		MESSAGEBOX->SetEvent( std::bind( &CEasterEventHandler::OnOKBoxClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CEasterEventHandler::OnOKBoxClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( TRADEHANDLER->IsWindowTradeOpen() )
		{
			TRADEHANDLER->CloseTradeWindow();
		}

		PacketNPCEasterEgg s;
		s.iLength = sizeof( PacketNPCEasterEgg );
		s.iHeader = PKTHDR_NPCEasterEgg;
		s.iAction = PacketNPCEasterEgg::ACTION_GetReward;

		int iCount = 0;
		
		for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
		{
			ItemData * ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_RedEasterEgg );
			
			if ( ps == NULL )
				ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GreenEasterEgg );
			
			if ( ps == NULL )
				ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_PinkEasterEgg );

			if ( ps && ps->bValid )
			{
				s.iaItemID[i]		= ps->sItem.sItemID.ToItemID();
				s.dwaHead[i]		= ps->sItem.iChk1;
				s.dwaCheckSum[i]	= ps->sItem.iChk2;
				ITEMHANDLER->DeleteItem( ps );
				iCount++;
			}

		}
		
		if ( iCount != EASTER_ITEMS_MAX )
			return;

		SENDPACKETL( &s, TRUE );
	}
}
