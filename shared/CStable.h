#pragma once
enum StablePetRarity : BYTE 
{
	PETCOMMON = 1,
	PETUNCOMMON = 2,
	PETRARE = 3,
	PETEPIC = 4,
	PETLEGENDARY = 5
};

struct CStablePetStucture
{
	BYTE				sPetId;
	StablePetRarity		sStablePetRarity;
	char				cPetName[32];
	BOOL				bActivePet;
};
#define MAX_PET_STABLE 5
struct CStablePetTab 
{
	char	cPetNameOwner[32];
	CStablePetStucture sCStablePetTab[MAX_PET_STABLE];
};

struct PacketStablePetTab : Packet
{
	CStablePetTab						sCStablePetTab;
	short								sPacketNumber;
	short								sPacketMax;
};
struct PacketStablePetActivePet : Packet
{
	BYTE								bPetID;
};

class CStablePet
{
protected:
	CStablePetTab sCStablePetTab;

public:

};
