#pragma once

#include "HalloweenModel.h"



class CHalloweenHandler
{
protected:
	BOOL												bEventIsAlive = FALSE;

	class CHalloweenModel								* pcModel = NULL;

	DWORD												dwRenderUpdate = 0;
	DWORD												dwRenderUpdateStop = 0;

	int													iCountParticle = 0;
	int													iParticleID[1025];
	int													iParticleUseID = 0;

	int													iParticlePointer = 0;

public:
	CHalloweenHandler();
	virtual ~CHalloweenHandler();

	void												Init();

	BOOL												EventIsAlive() { return bEventIsAlive; };

	BOOL												IsUsingSuit( class UnitData * pcUnitData );

	void												OnSetTimeDay( DWORD * pdwHour, DWORD * pdwMinute );

	BOOL												IsValidRune( class ItemData * pcItem );

	BOOL												IsLantern( DWORD dwCode );

	void												PHSetEventStatus( struct PacketHalloweenEventStatus * psPacket );

	void												Render();

	void												LoadParticle();

};

