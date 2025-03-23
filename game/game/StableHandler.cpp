#include "stdafx.h"
#include "StableHandler.h"

StableHandler::StableHandler()
{
	pcCStableWindow = new CStableWindow();
}

StableHandler::~StableHandler()
{
	SAFE_DELETE(pcCStableWindow);
}

void StableHandler::Init()
{
	pcCStableWindow->Init();
}

void StableHandler::Render()
{
	pcCStableWindow->Render();
}

void StableHandler::OnClickPetButton(int iPetID)
{
	PacketStablePetActivePet PetActive;
	ZeroMemory(&PetActive, sizeof(PetActive));

	PetActive.iHeader = PKTHDR_SendStablePet_ActivePet;
	PetActive.iLength = sizeof(PetActive);
	PetActive.bPetID = iPetID + 1;
	SENDPACKETG(&PetActive, 0);
}

void StableHandler::ProcessPacket(PacketStablePetTab* sPacketStablePetTab)
{
	pcCStableWindow->LoadPets(sPacketStablePetTab);
}

