#include "stdafx.h"
#include "CrystalTowerHandler.h"

CCrystalTowerHandler::CCrystalTowerHandler()
{

}

CCrystalTowerHandler::~CCrystalTowerHandler()
{
}

void CCrystalTowerHandler::Update()
{
	if ( dwTimeLeft > 0 )
		dwTimeLeft--;

	if ( (iRound > 0) && (dwTimeLeft == 0) && bStarted )
		bStarted = FALSE;
}

void CCrystalTowerHandler::Render()
{
	if ( bStarted && MAP_ID == MAPID_T5QuestArena )
	{
		int iX = (RESOLUTION_WIDTH >> 1) - 210;
		int iY = 6;

		if ( iRound > 0 )
		{
			FONTHANDLER->DrawBellatraFont( iX, iY, 0.75f, D3DCOLOR_XRGB( 0, 255, 0 ), "Round %d", iRound );
			FONTHANDLER->DrawBellatraFont( iX + 170, iY, 0.75f, D3DCOLOR_XRGB( 255, 255, 0 ), "Time Left: %02d:%02d", (dwTimeLeft / 60) % 60, (dwTimeLeft % 60) );

		}
		else
		{
			FONTHANDLER->DrawBellatraFont( iX, iY, 0.75f, D3DCOLOR_XRGB( 75, 75, 220 ), "Quest Arena" );
			FONTHANDLER->DrawBellatraFont( iX + 170, iY, 0.75f, D3DCOLOR_XRGB( 255, 255, 0 ), "Time Left: %02d:%02d", (dwTimeLeft / 60) % 60, (dwTimeLeft % 60) );
		}
	}
}

void CCrystalTowerHandler::HandlePacket( PacketQuestArenaTier5 * psPacket )
{
	//Init
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_Round )
	{
		if ( psPacket->iRound == 0 )
		{
			dwTimeLeft	= psPacket->dwTimeLeft;
			iRound		= 0;
			bStarted	= TRUE;
		}
		else
		{
			if ( psPacket->iRound == 1 )
				dwTimeLeft	= psPacket->dwTimeLeft;

			iRound			= psPacket->iRound;
		}
	}

	//Lost
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_Lost )
	{
		dwTimeLeft	= 0;
		iRound		= 0;
		bStarted	= FALSE;
	}
	//Finished
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_FinishQuest )
	{
		dwTimeLeft	= 0;
		iRound		= 0;
		bStarted	= FALSE;
	}
}
