#pragma once

#include <CStable.h>
struct PetSystemSkin
{
	int				iID;
	char			szPetNameSummon[32];
	float			fPetSize;
	StablePetRarity eStablePetRarity;
};
struct PetSystem 
{
	char	szPetName[32];
	int		iPetskin;
};

struct PetSystemStable
{
	int				iPetID;
	StablePetRarity eStablePetRarity;
	char			szPetName[32];
};

class CPetSystemHandler
{
				CPetSystemHandler();
	virtual		~CPetSystemHandler();
private:
	bool		InvalidMapSpawn(UserData* pcUserData);
public:
	int			CheckPremiumActive(UserData* pcUserData);
	void		HandlePacket(UserData* pcUserData, PacketStablePetActivePet* psPacket);
	void		HandlePacket(UserData* pcUserData);
	void		CreatePet(UserData* pcUserData, int iPetSkin);
	void		LoadSkinData();
	void		SpawnPet(UserData* pcUserData);
	void		CheckPetRespawn(UserData* pcUserData);
	void		RemovePet(UserData* pcUserData);
	void		ChangePetName(UserData* pcUserData, char* szNewPetName);
	void		ChangePetSkin(UserData* pcUserData, int iPetID);
	PetSystem * GetPetData(UserData* pcUserData);
};