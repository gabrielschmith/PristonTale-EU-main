#include "stdafx.h"
#include "CWarehouseHandler.h"


CWarehouseHandler::CWarehouseHandler()
{
}


CWarehouseHandler::~CWarehouseHandler()
{
}

CWarehouseBase * CWarehouseHandler::Open( const char * pszFile, BOOL bReadFile )
{
	//DEBUG ( "CWarehouseHandler::Open" );

	CWarehouseBase * pcWarehouse = new CWarehouseBase;
	ZeroMemory( pcWarehouse, sizeof( CWarehouseBase ) );
	pcWarehouse->strFile	= pszFile;
	pcWarehouse->bEncrypted = TRUE;

	if ( bReadFile )
	{
		FILE * pFile = NULL;
		fopen_s( &pFile, pszFile, "rb" );
		if ( pFile )
		{
			pcWarehouse->bHaveFile = TRUE;

			PacketWarehouse sPacket;

			fseek( pFile, 0, SEEK_END );
			UINT uFileSize = ftell( pFile );

			UINT uBufferPosition = 0;
			while ( uBufferPosition < uFileSize )
			{
				//Read Packet Size
				fseek( pFile, uBufferPosition, SEEK_SET );
				fread( &sPacket, 4, 1, pFile );

				//Get Packet Buffer Block
				fseek( pFile, uBufferPosition, SEEK_SET );
				fread( &sPacket, sPacket.iLength, 1, pFile );

				//Add Packet
				Receive( pcWarehouse, &sPacket );

				//Last?
				if ( pcWarehouse->sPacketCount.iMin == pcWarehouse->sPacketCount.iMax )
					break;

				//Next Block
				uBufferPosition += sPacket.iLength;
			}

			fclose( pFile );
		}
	}

	return pcWarehouse;
}

void CWarehouseHandler::Receive( CWarehouseBase * pcWarehouse, PacketWarehouse * psPacket )
{
	//DEBUG ( "CWarehouseHandler::Receive" );

	if ( pcWarehouse )
	{
		//First?
		if ( pcWarehouse->sPacketCount.iMin == 0 )
		{
			//Set Page
			pcWarehouse->iPageCount			= psPacket->iMaxPage;

			//Update Packet Count
			pcWarehouse->sPacketCount.iMax	= psPacket->iMaxPage;
			if ( pcWarehouse->sPacketCount.iMax == 0 )
				pcWarehouse->sPacketCount.iMax = 1;

			//Set Opened User Pages
			PacketWarehouse * ps		= (PacketWarehouse*)&psPacket->baData[8];
			pcWarehouse->iMaxPageUser	= ps->iMaxPage;
		}

		//More Packets to Receive?
		if ( pcWarehouse->sPacketCount.iMin < pcWarehouse->sPacketCount.iMax )
		{
			//Next Packet
			pcWarehouse->sPacketCount.iMin++;

			//Error?
			if ( psPacket->uBufferCount > 0x1F00 )
			{
				if ( pcWarehouse->bHaveFile )
					pcWarehouse->bHaveFile = FALSE;

				pcWarehouse->sPacketCount.iMin = pcWarehouse->sPacketCount.iMax;
				return;
			}

			//Add Data Packet into Buffer
			CopyMemory( pcWarehouse->baData + pcWarehouse->uBufferPosition, &psPacket->baData[8], psPacket->uBufferCount );
			pcWarehouse->uBufferPosition += psPacket->uBufferCount;
		}
	}
}

BOOL CWarehouseHandler::Send( User * pcUser, CWarehouseBase * pcWarehouse, BOOL bSeasonalWH )
{
	//DEBUG ( "CWarehouseHandler::Send" );

	if ( pcWarehouse )
	{
		//Have File to send?
		if ( HaveFile( pcWarehouse ) )
		{
			//Encrypt
			if ( pcWarehouse->bEncrypted == FALSE )
				Encrypt( pcWarehouse );

			//Get Packets on Encrypted Buffer
			std::vector<PacketWarehouse*> vPackets = GetEncryptedBuffer( pcWarehouse );

			for ( std::vector<PacketWarehouse*>::iterator it = vPackets.begin(); it != vPackets.end(); it++ )
			{
				PacketWarehouse * psPacket = (*it);

				if ( psPacket )
				{
					psPacket->dwWHMode = bSeasonalWH ? 2 : 1;

					//Send
					SENDPACKET ( pcUser, psPacket );

					SAFE_DELETE ( psPacket );
				}
			}

			//Clear Packets
			vPackets.clear();
		}
		else
		{
			//Send Empty Warehouse, just for open
			PacketWarehouse sPacket;
			ZeroMemory( &sPacket, sizeof( PacketWarehouse ) );
			sPacket.iLength		= sizeof( PacketWarehouse ) - (_countof( sPacket.baData ) * sizeof( BYTE ));
			sPacket.iHeader		= PKTHDR_SendWarehouse;
			sPacket.iMaxPage	= 1;
			sPacket.dwWHMode	= bSeasonalWH ? 2 : 1;
			SENDPACKET( pcUser, &sPacket );
		}
	}

	return TRUE;
}

void CWarehouseHandler::CheckForDupedWarehouseItems( User* pcUser, CWarehouseBase* pcWarehouse )
{
	DEBUG ( "CWarehouseHandler::CheckForDupedWarehouseItems" );

	if (pcWarehouse)
	{
		//Loop over all WH items
		for (int i = 0; i < 405; i++)
		{
			ItemData& pc1 = pcWarehouse->psWarehouse->saItem[i];

			//If not valid, then it's an empty item / WH slot
			if (pc1.bValid)
			{
				//skip potions for now. There's lots of duped pots out there for some reason. But not really a big deal.
				if (pc1.sItem.iPotionCount > 0)
					continue;

				//Check other warehouse items
				for (int j = 0; j < 405; j++)
				{
					if (pc1.bValid == FALSE)
						break;

					//same item. skip
					if (j == i)
						continue;

					ItemData& pc2 = pcWarehouse->psWarehouse->saItem[j];

					//If not valid, then it's an empty item / WH slot
					if (pc2.bValid)
					{
						//skip potions for now. There's lots of duped pots out there for some reason. But not really a big deal.
						if (pc2.sItem.iPotionCount > 0)
							continue;

						if (pc1.sItem.sItemID.ToItemID() == pc2.sItem.sItemID.ToItemID() &&
							pc1.sItem.iChk1 == pc2.sItem.iChk1 &&
							pc1.sItem.iChk2 == pc2.sItem.iChk2)
						{
							if (pcUser)
							{
								PacketLogCheat sPacket;
								sPacket.iCheatID = CHEATLOGID_DuplicateItemsFoundInWHa;
								sPacket.LParam = pc1.sItem.sItemID.ToItemID();
								sPacket.SParam = pc1.sItem.iChk1;
								sPacket.EParam = pc1.sItem.iChk2;
								LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
							}

							pc1.bValid = FALSE;
							ZeroMemory(&pc1, sizeof(ItemData));

							continue;
						}

						if (pc1.sItem.sItemID.ToItemID() == pc2.sItem.sItemID.ToItemID() &&
							pc1.sItem.iBackupKey == pc2.sItem.iBackupKey &&
							pc1.sItem.iBackupChk == pc2.sItem.iBackupChk)
						{
							if (pcUser)
							{
								PacketLogCheat sPacket;
								sPacket.iCheatID = CHEATLOGID_DuplicateItemsFoundInWHb;
								sPacket.LParam = pc1.sItem.sItemID.ToItemID();
								sPacket.SParam = pc1.sItem.iChk1;
								sPacket.EParam = pc1.sItem.iChk2;
								LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
							}

							pc1.bValid = FALSE;
							ZeroMemory(&pc1, sizeof(ItemData));

							continue;
						}
					}
				}

				if (pcUser)
				{
					for (i = 0; i < 316; i++)
					{
						DropItemData& ps = pcUser->pcUserData->sIntentoryItems[i];

						if (ps.iItemID == pc1.sItem.sItemID.ToItemID() &&
							ps.iChk1   == pc1.sItem.iChk1 &&
							ps.iChk2   == pc1.sItem.iChk2)
						{
							PacketLogCheat sPacket;
							sPacket.iCheatID = CHEATLOGID_DuplicateWHItemFoundInPlayerInventory;
							sPacket.LParam = pc1.sItem.sItemID.ToItemID();
							sPacket.SParam = pc1.sItem.iChk1;
							sPacket.EParam = pc1.sItem.iChk2;
							LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);


							pc1.bValid = FALSE;
							ZeroMemory(&pc1, sizeof(ItemData));

							break;
						}
					}
				}
			}
		}
	}
}


void CWarehouseHandler::CheckForInvalidItems( User * pcUser, CWarehouseBase* pcWarehouse )
{
	DEBUG ( "CWarehouseHandler::CheckForInvalidItems" );

	if (pcWarehouse)
	{
		for (int i = 0; i < 405; i++)
		{
			ItemData & pc = pcWarehouse->psWarehouse->saItem[i];

			if (pc.bValid)
			{
				if (ITEMSERVER->CheckItemForm(&pc.sItem) == FALSE)
				{
					if (pcUser)
					{
						PacketLogCheat sPacket;
						sPacket.iCheatID = CHEATLOGID_InvalidWHItem;
						sPacket.LParam = pc.sItem.sItemID.ToItemID();
						sPacket.SParam = pc.sItem.iChk1;
						sPacket.EParam = pc.sItem.iChk2;
						LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
					}

					pc.bValid = FALSE;
					ZeroMemory(&pc, sizeof(ItemData));
				}

			}
		}
	}
}

BOOL CWarehouseHandler::Encrypt( CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		if ( pcWarehouse->psWarehouse && (pcWarehouse->bEncrypted == FALSE) )
		{
			//Clear
			ZeroMemory( pcWarehouse->baData, 331748 );

			//Encrypt
			PacketWarehouse * psPacket = (PacketWarehouse*)pcWarehouse->baData;
			int iEncryptSize = ITEMSERVER->EncodeItemsData( pcWarehouse->psWarehouse, psPacket->baData, sizeof( WarehouseData ), (sizeof( WarehouseData ) - 0x10) );
			iEncryptSize += 0x120;

			//Get Page Count
			pcWarehouse->iPageCount = (iEncryptSize / 0x1F00);
			if ( ((iEncryptSize % 0x1F00) > 0) && (iEncryptSize > 0x1F00) )
				pcWarehouse->iPageCount++;

			//Update Packet Count
			pcWarehouse->sPacketCount.iMax = pcWarehouse->iPageCount;
			if ( pcWarehouse->sPacketCount.iMax == 0 )
				pcWarehouse->sPacketCount.iMax = 1;
			pcWarehouse->sPacketCount.iMin = pcWarehouse->sPacketCount.iMax;

			//Packet Header
			psPacket->iLength			= iEncryptSize;
			psPacket->iHeader			= PKTHDR_Warehouse;
			psPacket->dwCheckSum		= GetChecksum( pcWarehouse->psWarehouse, sizeof( WarehouseData ) );
			psPacket->iMaxPage			= pcWarehouse->iMaxPageUser;
			psPacket->iGold				= (pcWarehouse->iGold + 2023) ^ psPacket->iHeader ^ psPacket->dwCheckSum;
			psPacket->iDataLength		= iEncryptSize - 0x120;

			//Clear Memory
			SAFE_DELETE( pcWarehouse->psWarehouse );

			pcWarehouse->bEncrypted = TRUE;

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWarehouseHandler::Decrypt( CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		if ( pcWarehouse->bEncrypted )
		{
			PacketWarehouse * psPacket = (PacketWarehouse*)pcWarehouse->baData;

			//Clear Memory
			SAFE_DELETE( pcWarehouse->psWarehouse );

			//New Warehouse
			pcWarehouse->psWarehouse = new WarehouseData;
			ZeroMemory( pcWarehouse->psWarehouse, sizeof( WarehouseData ) );

			//Gold
			pcWarehouse->iGold = (int)(((psPacket->iGold ^ psPacket->dwCheckSum) ^ psPacket->iHeader) - 2023);
			if ( pcWarehouse->iGold == (-2023) )
				pcWarehouse->iGold = 0;

			//Decrypt Data
			ITEMSERVER->DecodeItemsData( psPacket->baData, pcWarehouse->psWarehouse, sizeof( WarehouseData ) );

			pcWarehouse->bEncrypted = FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWarehouseHandler::Save( CWarehouseBase * pcWarehouse )
{
	//DEBUG ( "CWarehouseHandler::Save" );

	if ( pcWarehouse )
	{
		//Open File
		FILE * pFile = NULL;
		fopen_s( &pFile, pcWarehouse->strFile.c_str(), "wb" );
		if ( pFile )
		{
			//Get Packets on Encrypted Buffer
			std::vector<PacketWarehouse*> vPackets = GetEncryptedBuffer( pcWarehouse );

			//Buffer Position for file
			UINT uBufferPosition = 0;

			for ( std::vector<PacketWarehouse*>::iterator it = vPackets.begin(); it != vPackets.end(); it++ )
			{
				PacketWarehouse * psPacket = (*it);

				//Write Packet into file
				fseek( pFile, uBufferPosition, SEEK_SET );
				fwrite( psPacket, psPacket->iLength, 1, pFile );

				//Next Block to write
				uBufferPosition += psPacket->iLength;

				SAFE_DELETE( psPacket );
			}

			//Clear Packets
			vPackets.clear();

			fclose( pFile );

			//Success
			return TRUE;
		}
	}
	return FALSE;
}

void CWarehouseHandler::Close( CWarehouseBase * pcWarehouse )
{
	//DEBUG ( "CWarehouseHandler::Close" );

	//Clear Memory
	SAFE_DELETE( pcWarehouse->psWarehouse );
	SAFE_DELETE( pcWarehouse );
}

/// <summary>
/// Triggered when a user clicks on a warehouse NPC.
/// See UnitServer::OnUnitNpcClick
/// </summary>
/// <param name="bSeasonalWarehouse">True if the WH is from seasonal, otherwise false</param>
void CWarehouseHandler::OnWarehouseOpened( User * pcUser, BOOL bSeasonalWH )
{
	//only for login server
	if ( GAME_SERVER )
		return;

	if ( pcUser == NULL || pcUser->IsValidAndInGame () == FALSE )
		return;

	DEBUG ( "CWarehouseHandler::OnWarehouseOpened. Seasonal = %d", bSeasonalWH );

	char szFile[MAX_PATH] = { 0 };

	//Opened?
	if ( pcUser->pcUserData->iWarehouseStatus )
	{
		DEBUG ( "CWarehouseHandler::OnWarehouseOpened: iWarehouseStatus = %d", pcUser->pcUserData->iWarehouseStatus );
		return;

	}

	if ( ACCOUNTSERVER->CanAccessWarehousePage ( pcUser, 1 ) == FALSE )
	{
		 CHATSERVER->SendChat(pcUser, CHATCOLOR_Error, "> You need to be level 10 to open the warehouse.");
		 return;
	}

	//Open File
	if ( bSeasonalWH )
	{
		//WH from Seasonal - this is temp (until next seasonal)
		STRINGFORMAT( szFile, "Data\\WarehouseSeasonal\\%s.dat", pcUser->pcUserData->szAccountName );
	}
	else
	{
		STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );
	}

	//std::cout << szFile << std::endl;
	CWarehouseBase * pcWarehouse = Open( szFile );

	//Decrypt
	Decrypt( pcWarehouse );

	//Set Gold
	pcUser->iWarehouseGold = pcWarehouse->iGold;

	//Log Gold
	SQLGoldLog( pcUser, pcWarehouse->iGold, TRUE );

	//Add to Server Inventory
	AddWarehouseInventoryItems( pcUser, pcWarehouse, bSeasonalWH ? pcUser->sWarehouseItems_Seasonal : pcUser->sWarehouseItems );

	//Send to User
	Send( pcUser, pcWarehouse, bSeasonalWH );

	//Opened!
	pcUser->pcUserData->iWarehouseStatus = bSeasonalWH ? 2 : 1;

	DEBUG ( "CWarehouseHandler::OnWarehouseOpened OK: iWarehouseStatus = %d", pcUser->pcUserData->iWarehouseStatus );

	//Close and Clear Warehouse Data
	Close( pcWarehouse );
}

void CWarehouseHandler::HandlePacket( User * pcUser, PacketWarehouse * psPacket )
{
	//only for login server
	if ( GAME_SERVER )
		return;

	if ( pcUser->IsValidAndInGame () == FALSE )
		return;

	DEBUG ( "CWarehouseHandler::HandlePacket( User * pcUser, PacketWarehouse * psPacket ). iWarehouseStatus = %d", pcUser->pcUserData->iWarehouseStatus );

	if ( pcUser->pcUserData->iWarehouseStatus )
	{
		BOOL bSeasonalWH = pcUser->pcUserData->iWarehouseStatus == 2;

		CWarehouseBase * pcWarehouse = pcUser->pcUserData->pcWarehouse;

		//Open File
		if ( pcWarehouse == NULL )
		{
			char szFile[MAX_PATH] = { 0 };
			if ( bSeasonalWH )
			{
				STRINGFORMAT( szFile, "Data\\WarehouseSeasonal\\%s.dat", pcUser->pcUserData->szAccountName );
			}
			else
			{
				STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );
			}

			pcWarehouse = Open( szFile, FALSE );

			pcUser->pcUserData->pcWarehouse = pcWarehouse;
		}

		//Receive Data
		Receive( pcWarehouse, psPacket );

		//Last Packet?
		if ( pcWarehouse->sPacketCount.iMin == pcWarehouse->sPacketCount.iMax )
		{
			//Decrypt
			Decrypt( pcWarehouse );

			if (pcWarehouse->iGold < pcUser->iWarehouseGold)
			{
				int l_Diff = pcUser->iWarehouseGold - pcWarehouse->iGold;

				if (l_Diff > pcUser->iWarehouseGold)
				{
					PacketLogCheat sPacket;
					sPacket.iCheatID	= CHEATLOGID_GenerateGoldHack;
					sPacket.LParam		= pcUser->iWarehouseGold;
					sPacket.SParam		= l_Diff;
					sPacket.WxParam		= 4;
					LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);

					if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
					{
						NETSERVER->DisconnectUser( pcUser->pcUserData );

						SENDPACKETBLANK( USERDATATOUSER( pcUser->pcUserData ), PKTHDR_Disconnect );
						WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );
					}

                    return;
				}

				pcUser->iWarehouseGold -= l_Diff;

				USERSERVER->AddServerUserGold( pcUser->pcUserData, l_Diff, WHEREID_OpenWarehouse );

				PacketSetCharacterGold sPacket;
				sPacket.iHeader = PKTHDR_SetGold;
				sPacket.iLength = sizeof(PacketSetCharacterGold);
				sPacket.dwGold	= pcUser->pcUserData->GetGold();

				PACKETSERVER->Send(pcUser->pcUserData, &sPacket);
				SQLGoldLog(pcUser, -pcWarehouse->iGold, FALSE);
			}
			else if (pcWarehouse->iGold > pcUser->iWarehouseGold)
			{
				int l_Diff = pcWarehouse->iGold - pcUser->iWarehouseGold;

				if (l_Diff > pcUser->pcUserData->GetGold())
				{
					PacketLogCheat sPacket;
					sPacket.iCheatID	= CHEATLOGID_GenerateGoldHack;
					sPacket.LParam		= pcUser->pcUserData->GetGold();
					sPacket.SParam		= pcWarehouse->iGold;
					sPacket.WxParam		= 5;
					LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);

					if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
					{
						NETSERVER->DisconnectUser( pcUser->pcUserData );

						SENDPACKETBLANK( USERDATATOUSER( pcUser->pcUserData ), PKTHDR_Disconnect );
						WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );
					}

                    return;
				}

				pcUser->iWarehouseGold += l_Diff;

				USERSERVER->SubServerUserGold(pcUser->pcUserData, l_Diff, WHEREID_OpenWarehouse);

				PacketSetCharacterGold sPacket;
				sPacket.iHeader = PKTHDR_SetGold;
				sPacket.iLength = sizeof(PacketSetCharacterGold);
				sPacket.dwGold	= pcUser->pcUserData->GetGold();

				PACKETSERVER->Send(pcUser->pcUserData, &sPacket);
				SQLGoldLog(pcUser, pcWarehouse->iGold, FALSE);
			}

			//Everything good?
			if ( CheckWarehouseInventoryItems( pcUser, pcWarehouse, bSeasonalWH ) )
			{
				//Make Backup
				MakeBackup( pcUser, bSeasonalWH );

				//Check for invalid WH items
				CheckForInvalidItems ( pcUser, pcWarehouse );

				//Check for any duplicates (ignoring pots)
				CheckForDupedWarehouseItems ( pcUser, pcWarehouse );

				//Encrypt
				Encrypt( pcWarehouse );

				//Save Warehouse
				Save( pcWarehouse );

				//Closed
				pcUser->pcUserData->iWarehouseStatus = 0;

				//Send Success
				SENDPACKETBLANK( pcUser, PKTHDR_WarehouseSaveSuccess, TRUE );
			}

			//Close and Clear Warehouse Data
			Close( pcWarehouse );

			pcUser->pcUserData->pcWarehouse = NULL;
			pcUser->pcUserData->iWarehouseStatus = 0;

			DEBUG ( "CWarehouseHandler::HandlePacket: iWarehouseStatus final = %d", pcUser->pcUserData->iWarehouseStatus );
		}
	}
}

DWORD CWarehouseHandler::GetChecksum( WarehouseData * psData, UINT uSize )
{
	DWORD dwRet = 0;
	for ( UINT i = 0; i < uSize; i++ )
		dwRet += ((BYTE*)psData)[i] ^ 0x6A;

	return dwRet;
}

std::vector<PacketWarehouse*> CWarehouseHandler::GetEncryptedBuffer( CWarehouseBase * pcWarehouse )
{
	std::vector<PacketWarehouse*> v;

	if ( pcWarehouse->bEncrypted )
	{
		PacketWarehouse * psPacketBase = (PacketWarehouse*)pcWarehouse->baData;

		//Total Size of Buffer
		UINT uSize = psPacketBase->iLength;

		//Buffer Position
		UINT uBufferPosition = 0;

		pcWarehouse->sPacketCount.iMax = (uSize / 0x1F00);
		if ( (uSize % 0x1F00) > 0 && (uSize > 0x1F00) )
			pcWarehouse->sPacketCount.iMax++;

		int iCurrentPage = 0;

		//Create Packets
		do
		{
			//Write Encrypted Buffer into Packet Block
			PacketWarehouse * psPacket = new PacketWarehouse;
			ZeroMemory( psPacket, sizeof( PacketWarehouse ) );

			//Size of Block
			UINT uDataSize = uSize - uBufferPosition;
			if ( uDataSize > 0x1F00 )
				uDataSize = 0x1F00;

			//Packet Header
			psPacket->iLength		= uDataSize + 0x100;
			psPacket->iHeader		= PKTHDR_Warehouse;
			psPacket->iCurrentPage	= iCurrentPage++;
			psPacket->iMaxPage		= pcWarehouse->iPageCount;
			psPacket->uBufferCount	= uDataSize;

			//Copy into Packet
			CopyMemory( &psPacket->baData[8], pcWarehouse->baData + uBufferPosition, uDataSize );

			//Add to Packets vector
			v.push_back( psPacket );

			//Next Buffer
			uBufferPosition += uDataSize;
		}
		while ( uBufferPosition < uSize );
	}

	return v;
}

void CWarehouseHandler::SQLGoldLog( User * pcUser, int iGold, BOOL bOpenWarehouse )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 11 );
	if ( pcDB->Open() )
	{
		char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,4,?,?,?,0,GETDATE())";
		if ( bOpenWarehouse )
			pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,3,?,?,?,0,GETDATE())";

		if ( pcDB->Prepare( pszQuery ) )
		{
			int iCode1 = ITEMID_Gold;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)"Gold", 4 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iCode1 );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iGold );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CWarehouseHandler::SQLItemLog( User * pcUser, int iItemID, int iChk1, int iChk2, BOOL bGotItem )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 12 );
	if ( pcDB->Open() )
	{
		char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,1,?,?,?,?,GETDATE())";
		if ( bGotItem )
			char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,2,?,?,?,?,GETDATE())";

		if ( pcDB->Prepare( pszQuery ) )
		{
			char * pszItemName = "Unknown";
			ItemData * psItemWH = ITEMSERVER->FindItemPointerTable( iItemID );
			if ( psItemWH )
				pszItemName = psItemWH->szBaseName;

			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszItemName, lstrlenA( pszItemName ) );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iItemID );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iChk1 );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iChk2 );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CWarehouseHandler::AddWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse, DropItemData * pcDropItemData )
{
	DEBUG ( "CWarehouseHandler::AddWarehouseInventoryItems" );

	//Clear Warehouse Inventory
	ZeroMemory( pcDropItemData, sizeof( DropItemData ) * 420 );

	int iCount = 0;
	//Add items to inventory
	for ( int i = 0; i < 405; i++ )
	{
		ItemData & pc = pcWarehouse->psWarehouse->saItem[i];
		if ( pc.bValid )
		{
			pcDropItemData[iCount].iItemID = pc.sItem.sItemID.ToItemID();
			pcDropItemData[iCount].iChk1	= pc.sItem.iChk1;
			pcDropItemData[iCount].iChk2	= pc.sItem.iChk2;
			iCount++;

			ITEMSERVER->SaveItemDataToDatabase( &pc.sItem );

			//Potion
			if ( pc.sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
				ITEMSERVER->AddPotion( pcUser->pcUserData, pc.sItem.sItemID.ToInt(), pc.sItem.iPotionCount );
		}
	}
}

BOOL CWarehouseHandler::CheckWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse, BOOL bSeasonalWH )
{
	DEBUG ( "CWarehouseHandler::CheckWarehouseInventoryItems" );

	//Check Got Items
	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		DropItemData & psItemWarehouse = bSeasonalWH ? pcUser->sWarehouseItems_Seasonal[i] : pcUser->sWarehouseItems[i];

		if ( psItemWarehouse.iItemID )
		{
			BOOL bFound = FALSE;

			int iWHIndex = -1;

			//Find Item on Warehouse
			for ( int j = 0; j < 405; j++ )
			{
				ItemData & pc = pcWarehouse->psWarehouse->saItem[j];
				//Found item on Warehouse?
				if ( psItemWarehouse.iItemID == pc.sItem.sItemID.ToItemID() &&
					 psItemWarehouse.iChk1 == pc.sItem.iChk1 &&
					 psItemWarehouse.iChk2 == pc.sItem.iChk2 )
				{
					//Set Found Index
					iWHIndex = j;
					bFound = TRUE;
					break;
				}
			}

			//Not Found? Log it
			if ( bFound == FALSE )
			{
				//Log Item
				SQLItemLog( pcUser, psItemWarehouse.iItemID, psItemWarehouse.iChk1, psItemWarehouse.iChk2, TRUE );

				//Add to User Inventory
				ITEMSERVER->AddItemInventory( pcUser->pcUserData, psItemWarehouse.iItemID, psItemWarehouse.iChk1, psItemWarehouse.iChk2 );

				psItemWarehouse.iItemID = 0;
			}
			else
			{
				ItemData * pc = pcWarehouse->psWarehouse->saItem + iWHIndex;

				//Potion?
				if ( pc->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
				{
					ITEMSERVER->AddPotion( pcUser->pcUserData, pc->sItem.sItemID.ToInt(), pc->sItem.iPotionCount );
				}
			}
		}
	}

	//Check Put Items
	for ( int i = 0; i < 405; i++ )
	{
		ItemData & pc = pcWarehouse->psWarehouse->saItem[i];

		if ( pc.bValid )
		{

			BOOL bFound = FALSE;

			//Find Item on Warehouse
			for ( int j = 0; j < WAREHOUSE_ITEM_MAX; j++ )
			{
				DropItemData * psItemWarehouse = bSeasonalWH ? (pcUser->sWarehouseItems_Seasonal + j) : (pcUser->sWarehouseItems + j);

				//Found Item on Warehouse?
				if ( psItemWarehouse->iItemID == pc.sItem.sItemID.ToItemID() &&
					psItemWarehouse->iChk1 == pc.sItem.iChk1 &&
					psItemWarehouse->iChk2 == pc.sItem.iChk2 )
				{
					//Set Found
					bFound = TRUE;
					break;
				}
			}

			//Not Found?
			if ( !bFound )
			{
				//Log Put
				SQLItemLog( pcUser, pc.sItem.sItemID.ToInt(), pc.sItem.iChk1, pc.sItem.iChk2, FALSE );

				//Item Not Found?
				if (!ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, pc.sItem.sItemID.ToInt(), pc.sItem.iChk1, pc.sItem.iChk2 ))
				{
// 					//Hack Clone Warehouse Log
// 					PacketLogCheat sPacketCheat;
// 					sPacketCheat.iCheatID = CHEATLOGID_WarehouseNewClone;
// 					sPacketCheat.LParam = pc.sItem.sItemID.ToItemID();
// 					sPacketCheat.SParam = pc.sItem.iChk1;
// 					sPacketCheat.EParam = pc.sItem.iChk2;
// 					ITEMSERVER->SaveItemData( &pc.sItem );
// 					LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacketCheat );
// 					return FALSE;
				}

				//Add to Warehouse Inventory
				DropItemData sItem;
				sItem.iItemID = pc.sItem.sItemID.ToItemID();
				sItem.iChk1 = pc.sItem.iChk1;
				sItem.iChk2 = pc.sItem.iChk2;
				ACCOUNTSERVER->AddWarehouseItem( pcUser->pcUserData, &sItem, bSeasonalWH );
				ITEMSERVER->SaveItemDataToDatabase( &pc.sItem );

				//Potion
				if ( pc.sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
				{
					ITEMSERVER->AddPotion( pcUser->pcUserData, pc.sItem.sItemID.ToInt(), (-pc.sItem.iPotionCount) );
				}
			}
		}
	}

	return TRUE;
}

void CWarehouseHandler::MakeBackup( User * pcUser, BOOL bSeasonalWH )
{
	char szFile[MAX_PATH] = { 0 };
	char szNewFile[MAX_PATH] = { 0 };

	if ( bSeasonalWH )
	{
		STRINGFORMAT( szFile, "Data\\WarehouseSeasonal\\%s.dat", pcUser->pcUserData->szAccountName );
	}
	else
	{
		STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );
	}


	if ( bSeasonalWH )
	{
		STRINGFORMAT( szNewFile, "Data\\WarehouseSeasonalBackup\\%s.dat", pcUser->pcUserData->szAccountName );
	}
	else
	{
		STRINGFORMAT( szNewFile, "Data\\WarehouseBackup\\%s.dat", pcUser->pcUserData->szAccountName );
	}

	CopyFileA( szFile, szNewFile, FALSE );
}
