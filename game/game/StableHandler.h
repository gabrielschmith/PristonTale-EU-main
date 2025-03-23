#include "CStableWindow.h"
#include <CStable.h>
#pragma once
class StableHandler
{
protected:
	CStableWindow				* pcCStableWindow = NULL;

public:
	CStableWindow				* GetWindow() { return pcCStableWindow; }
								StableHandler();
	virtual						~StableHandler();
	void						Init();
	void						Render();
	void						OnClickPetButton(int iPetID);
	void						ProcessPacket(PacketStablePetTab* sPacketStablePetTab);
	
};
