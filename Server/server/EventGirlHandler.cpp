#include "stdafx.h"
#include "EventGirlHandler.h"


CEventGirlHandler::CEventGirlHandler()
{
}


CEventGirlHandler::~CEventGirlHandler()
{
}

void CEventGirlHandler::OnEventGirlResetRequest(User* pcUser, PacketEventGirlReset* psPacket)
{
	if (GAME_SERVER)
		return;

	auto pcUserData = pcUser->pcUserData;
	int iTotalCost = psPacket->iTotalCost;

	if (EVENT_FREEEVENTGIRL)
	{
		iTotalCost = 0;
	}

	//safe-guard
	if (iTotalCost < 0)
		iTotalCost = 0;

	PacketEventGirlResetResult s;
	s.iLength = sizeof(PacketEventGirlResetResult);
	s.iHeader = PTKHDR_EventGirlReset;
	s.iTotalCost = iTotalCost;
	s.bResetType = psPacket->bResetType;
	s.bResult = FALSE;

	CopyMemory(s.baReset, psPacket->baReset, _countof(s.baReset) * sizeof(BOOL));

	//reset failed. recover gold
	if (psPacket->bResetType == 99)
	{
		if (CHARACTERSERVER->GiveGOLD(pcUser, iTotalCost, WHEREID_EventGirl))
		{
			s.bResult = TRUE;
		}
	}
	else if (psPacket->bResetType >= 0 && psPacket->bResetType <= 2)
	{
		if (iTotalCost < pcUserData->GetGold())
		{
			if (CHARACTERSERVER->GiveGOLD(pcUser, -iTotalCost, WHEREID_EventGirl))
			{
				s.bResult = TRUE;
			}
		}
	}

	SENDPACKET(pcUser, &s, TRUE);
}