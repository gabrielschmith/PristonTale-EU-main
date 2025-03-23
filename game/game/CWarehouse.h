#pragma once
#include "CWareHouseWindow.h"

class CWarehouse
{
private:
	DWORD										dwWHmode = 0;

	std::vector<std::pair<DWORD, DWORD>>		vLoadedWarehouseItems;

protected:
	class CWarehouseWindow						* pcWarehouseWindow = NULL;
public:
	CWarehouse();
	virtual ~CWarehouse();

	class CWarehouseWindow						* GetWindow() { return pcWarehouseWindow; };

	void										Init();

	BOOL										OnMouseClick( class CMouse * pcMouse );

	int											IsWithdrawalOnlyWarehouse () { return dwWHmode == 2; }; //2 = seasonal wh

	int											GetPage() { return (*(int*)0x3693304) + 1; };
	void										SetPage(int iPage);


	int											OnRenderTime(struct Skill * psSkill );

	void										OpenNPCQuest();

	ItemData									* GetItemData();

	BOOL										ItemIsFromWarehouse ( ItemData * pcItemData );

	void										HandlePostPacket( struct PacketWarehouse * psPacket );
	void										HandlePacket( struct PacketWarehouse * psPacket );

	BOOL										OnUnitKilled( class UnitData * pcUnit );

	void										RestoreItems();

	BOOL									    IsWithinWarehousePlacementRegion ( Point2D * position );
	ItemData									* GetMouseItemHover();
};

