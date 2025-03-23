#include "stdafx.h"
#include "CWarehouse.h"

int iSizeLastPacketWarehouse = 0;


CWarehouse::CWarehouse()
{
}


CWarehouse::~CWarehouse()
{
	SAFE_DELETE( pcWarehouseWindow );
	vLoadedWarehouseItems.clear ();
}

void CWarehouse::Init()
{
	dwWHmode = 0;
	pcWarehouseWindow = new CWarehouseWindow();
	pcWarehouseWindow->Init();
}

void CWarehouse::SetPage( int iPage )
{
	// Check for quests
	int iOldPage = GetPage();

	if ( iOldPage < iPage )
	{
		if ( CQUESTGAME->GetQuestInstance()->GetRewardValue( EQuestExtraRewardType::WarehousePage ) < ( iPage - 1) )
		{
			if ( iPage == 3 )
			{
				TITLEBOX( "Quest Warehouse #%d is not avaliable yet!", iPage - 1 );
			}
			else
			{
				TITLEBOX( "You need to complete the Quest Warehouse #%d", iPage - 1 );
			}
			return;
		}
	}

	*(int*)0x3693304 = iPage-1;
	if ( *(int*)0x3693304 > 4 )
		*(int*)0x3693304 = 4;
	else if ( *(int*)0x3693304 < 0 )
		*(int*)0x3693304 = 0;
}

ItemData * CWarehouse::GetItemData()
{
	return (ItemData*)0x036A1380;
}

BOOL CWarehouse::ItemIsFromWarehouse ( ItemData * pcItemData )
{
	if ( pcItemData->bValid == FALSE )
		return FALSE;

	//only 2 pages for now.
	for ( std::pair<DWORD, DWORD> p : vLoadedWarehouseItems )
	{
		if ( p.first == pcItemData->sItem.iChk1 &&
			p.second == pcItemData->sItem.iChk2 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//After packet processing is done by ASM
void CWarehouse::HandlePostPacket ( PacketWarehouse * psPacket )
{
	dwWHmode = psPacket->dwWHMode;

	if ( psPacket->iCurrentPage == (psPacket->iMaxPage - 1) || psPacket->iMaxPage == 0 )
	{
		vLoadedWarehouseItems.clear ();

		//only 2 pages for now.
		for ( int i_Page = 0; i_Page < 2; i_Page++ )
		{
			ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData() + (i_Page * 81);

			for ( int i_ItemIndex = 0; i_ItemIndex < 81; i_ItemIndex++ )
			{
				ItemData * psWarehouseItem = psaDataWarehouse + i_ItemIndex;
				if ( psWarehouseItem->bValid )
				{
					vLoadedWarehouseItems.push_back ( std::make_pair ( (DWORD)psWarehouseItem->sItem.iChk1, (DWORD)psWarehouseItem->sItem.iChk2 ) );
				}
			}
		}
	}
}

void CWarehouse::HandlePacket( PacketWarehouse * psPacket )
{
	//WH mode. 1 = normal, 2 = seasonal

	if ( psPacket->iCurrentPage == 0 )
	{
		WRITEDWORD ( 0x3A976E4, 0 ); // dwLastWareHouseChkSum

		//ZeroMemory( (void*)0x397A738, 319444 );

		/*for ( int i_Page = 0; i_Page < 2; i_Page++ )
		{
			ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData () + ( i_Page * 81 );
			ZeroMemory ( psaDataWarehouse, sizeof ( ItemData ) * 81 );
		}*/
	}

	//int iPage		= psPacket->dwCheckSum;
	//int iPageCount	= (int)psPacket->wPage;
	//PacketWarehouse * pPacketWarehouse = (PacketWarehouse *)0x397A738;
//	if ( iPage == 0 )
//		ZeroMemory( (void*)0x397A738, 319444 );


//	CopyMemory( pPacketWarehouse + (iPage * 7936), &psPacket->Data[8], psPacket->WareHouseMoney );
}

int CWarehouse::OnRenderTime( struct Skill * psSkill )
{
	int iLine = 0;

	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();

	char szBuffer[128] = { 0 };

	return iLine;
}

void CWarehouse::RestoreItems()
{
	CALLT( 0x0050CC90, 0x036932FC );
}

/// <summary>
/// Return TRUE if the position is within the warehouse placement region
/// </summary>
BOOL CWarehouse::IsWithinWarehousePlacementRegion ( Point2D * position )
{
	// Warehouse rect box
	Rectangle2D rWarehouseBox{ 21, 86, 21 + 200, 86 + 200 };

	if ( rWarehouseBox.Inside( position ) )
	{
		return TRUE;
	}

	return FALSE;
}

ItemData * CWarehouse::GetMouseItemHover()
{
	if ( WINDOW_ISOPEN_WAREHOUSE == FALSE )
		return NULL;

	ItemData * pcItemData = NULL;

	// Items in warehouse from current page
	ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData() + ((WAREHOUSEHANDLER->GetPage() - 1) * 81);

	// Warehouse rect box
	RECT rWarehouseBox{ 21, 86, 21 + 200, 86 + 200 };

	// Rows
	for ( int i = 0; i < 9; i++ )
	{
		// Columns
		for ( int j = 0; j < 9; j++ )
		{
			// Get Item from mouse inside
			for ( int k = 0; k < 81; k++ )
			{
				ItemData * psWarehouseItem = psaDataWarehouse + k;

				if ( !psWarehouseItem->bValid )
					continue;

				Rectangle2D rRectangle;
				rRectangle.iX = psWarehouseItem->sPosition.iX;
				rRectangle.iY = psWarehouseItem->sPosition.iY;
				rRectangle.iWidth = psWarehouseItem->iWidth - 1;
				rRectangle.iHeight = psWarehouseItem->iHeight - 1;

				if ( rRectangle.Inside( MOUSEHANDLER->GetPosition() ) )
				{
					pcItemData = psWarehouseItem;
					break;
				}
			}

			if ( pcItemData )
				break;
		}

		if ( pcItemData )
			break;
	}


	return pcItemData;
}

BOOL CWarehouse::OnMouseClick( class CMouse * pcMouse )
{
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR )
	{

	}
	return TRUE;
}

void CWarehouse::OpenNPCQuest()
{
	GetWindow()->OpenWindowQuest2Page();
}
