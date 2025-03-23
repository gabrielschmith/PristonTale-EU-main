#pragma once


class CWarehouseHandler
{
public:
	CWarehouseHandler();
	virtual ~CWarehouseHandler();

	CWarehouseBase			* Open( const char * pszFile, BOOL bReadFile = TRUE );

	BOOL					HaveFile( CWarehouseBase * pcWarehouse ) { return pcWarehouse->bHaveFile; }

	void					Receive( CWarehouseBase * pcWarehouse, PacketWarehouse * psPacket );
	BOOL					Send( User * pcUser, CWarehouseBase * pcWarehouse, BOOL bSeasonalWH );
	void					CheckForInvalidItems (User* pcUser, CWarehouseBase* pcWarehouse );
	void					CheckForDupedWarehouseItems (User* pcUser, CWarehouseBase* pcWarehouse );

	BOOL					Encrypt( CWarehouseBase * pcWarehouse );
	BOOL					Decrypt( CWarehouseBase * pcWarehouse );

	BOOL					Save( CWarehouseBase * pcWarehouse );

	void					Close( CWarehouseBase * pcWarehouse );

	void					OnWarehouseOpened ( User * pcUser, BOOL bSeasonalWarehouse );
	void					HandlePacket( User * pcUser, PacketWarehouse * psPacket );

	DWORD					GetChecksum( WarehouseData * psData, UINT uSize );

private:
	std::vector<PacketWarehouse*> GetEncryptedBuffer( CWarehouseBase * pcWarehouse );

	void					SQLGoldLog( User * pcUser, int iGold, BOOL bOpenWarehouse );
	void					SQLItemLog( User * pcUser, int iItemID, int iChk1, int iChk2, BOOL bGotItem );

	void					AddWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse, DropItemData * pcDropItemData);

	BOOL					CheckWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse, BOOL bSeasonalWH);

	void					MakeBackup( User * pcUser, BOOL bSeasonalWarehouse  );
};

