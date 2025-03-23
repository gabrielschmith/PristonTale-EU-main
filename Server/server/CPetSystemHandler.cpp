#include "stdafx.h"
#include "CPetSystemHandler.h"



std::vector<PetSystemSkin*> vPetSystemSkin;
int iTick_Count = 0;
CPetSystemHandler::CPetSystemHandler()
{
}
CPetSystemHandler::~CPetSystemHandler()
{
	vPetSystemSkin.clear();
}

bool CPetSystemHandler::InvalidMapSpawn(UserData* pcUserData)
{
	if (pcUserData->iMapID <= MAPID_Invalid || pcUserData->iMapID == MAPID_BlessCastle)
	{
		pcUserData->dwObjectPetSystemID = 0;
		pcUserData->pcPetSystem = nullptr;
		return true;
	}
	return false;
}

int CPetSystemHandler::CheckPremiumActive(UserData* pcUserData)
{
	return ITEMSERVER->GetPremiumUse(USERDATATOUSER(pcUserData), ITEMTIMERTYPE_PetSystem) != NULL ? 1 : 0;
}

//change pet skin
void CPetSystemHandler::HandlePacket(UserData* pcUserData, PacketStablePetActivePet* psPacket)
{
	
	if (psPacket->bPetID == pcUserData->bPetSystemSkinID)
		return;

	if (pcUserData->bPetSystemSummomTimeDelay < 10)
	{
		CHATSERVER->SendTitleBox(pcUserData, "You need to wait 10 seconds to summon another pet skin");
		return;
	}
	pcUserData->bPetSystemSummomTimeDelay = 0;
	ChangePetSkin(pcUserData, psPacket->bPetID);
	RemovePet(pcUserData);
	SpawnPet(pcUserData);
	HandlePacket(pcUserData);
}

//send petdata to client
void CPetSystemHandler::HandlePacket(UserData* pcUserData)
{

	PacketStablePetTab PetTab ;
	ZeroMemory(&PetTab, sizeof(PacketStablePetTab));

	PetTab.iHeader = PKTHDR_SendStablePet;
	PetTab.iLength = sizeof(PacketStablePetTab);
	PetSystem* scPetSytem = GetPetData(pcUserData);

	/*STRINGCOPY(PetTab.sCStablePetTab.cPetNameOwner, scPetSytem->szPetName);*/
	int i = 0;
	for (std::vector<PetSystemSkin*>::iterator it = vPetSystemSkin.begin(); it != vPetSystemSkin.end(); it++)
	{
		//if (i > MAX_PET_STABLE - 1)
		//{
		//	WARN("CStablePetStucture bigger than it should be > %d", MAX_PET_STABLE);
		//	break;
		//}
		//PetSystemSkin* scPetSystemSkin = *it;
		//STRINGCOPY(PetTab.sCStablePetTab.sCStablePetTab[i].cPetName, scPetSystemSkin->szPetNameSummon);
		//PetTab.sCStablePetTab.sCStablePetTab[i].sPetId = scPetSystemSkin->iID;
		//PetTab.sCStablePetTab.sCStablePetTab[i].sStablePetRarity = scPetSystemSkin->eStablePetRarity;

		//PetTab.sCStablePetTab.sCStablePetTab[i].bActivePet = scPetSytem->iPetskin == scPetSystemSkin->iID ? true : false;
		//i++;
	}

	SENDPACKET(pcUserData, &PetTab);
}

void CPetSystemHandler::CreatePet(UserData* pcUserData, int iPetSkin)
{
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_PetSystem);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("BEGIN IF NOT EXISTS (SELECT * FROM [GameDB].[dbo].[PetSystem] WHERE PetOwnerName = ?) BEGIN INSERT INTO [dbo].[PetSystem] ([PetOwnerName], [PetName], [PetSkinId]) VALUES (?,?,?)  END END"))
		{
			char szNewNamePet[32] = { 0 };
			if (STRLEN(pcUserData->sCharacterData.szName) <= 23)
				STRINGFORMAT(szNewNamePet, "%s's Pet", pcUserData->sCharacterData.szName);
			else
				STRINGFORMAT(szNewNamePet, "%s's Pet", std::string(pcUserData->sCharacterData.szName).substr(0, 23).c_str());

			pcDB->BindParameterInput(1, PARAMTYPE_String, pcUserData->sCharacterData.szName, STRLEN(pcUserData->sCharacterData.szName));
			pcDB->BindParameterInput(2, PARAMTYPE_String, pcUserData->sCharacterData.szName, STRLEN(pcUserData->sCharacterData.szName));
			pcDB->BindParameterInput(3, PARAMTYPE_String, szNewNamePet, STRLEN(szNewNamePet));
			pcDB->BindParameterInput(4, PARAMTYPE_Integer, &iPetSkin, sizeof(int));

			pcDB->Execute();

		}
		pcDB->Close();
	}
	SpawnPet(pcUserData);
}

void CPetSystemHandler::SpawnPet(UserData* pcUserData)
{
	if (!CheckPremiumActive(pcUserData))
		return;
	if (InvalidMapSpawn(pcUserData) || pcUserData == nullptr)
		return;
	if (pcUserData->dwObjectPetSystemID)
		return;
	PetSystem* scPetSytem = GetPetData(pcUserData);
	PetSystemSkin* scPetSkin = nullptr;

	if (scPetSytem != nullptr)
	{
		for (std::vector<PetSystemSkin*>::iterator it = vPetSystemSkin.begin(); it != vPetSystemSkin.end(); it++)
		{
			if ((*it)->iID == scPetSytem->iPetskin)
			{
				scPetSkin = *(it);
				break;
			}
		}
	}

	if (scPetSkin != nullptr)
	{
		if (pcUserData->dwObjectPetSystemID != 0)
			return;
		UnitData* pcUnitData = UNITSERVER->CreateUnitEnemy(scPetSkin->szPetNameSummon, pcUserData->sPosition.iX, pcUserData->sPosition.iY, pcUserData->sPosition.iZ);
		char szNewNamePet[32] = { 0 };
		if (pcUnitData)
		{
			if (STRLEN(scPetSytem->szPetName))
			{
				STRINGCOPY(szNewNamePet, scPetSytem->szPetName);
			}
			else
			{
				if (STRLEN(pcUserData->sCharacterData.szName) <= 23)
					STRINGFORMAT(szNewNamePet, "%s's Pet", pcUserData->sCharacterData.szName);
				else
					STRINGFORMAT(szNewNamePet, "%s's Pet", std::string(pcUserData->sCharacterData.szName).substr(0, 23).c_str());
			}

			STRINGCOPY(pcUnitData->sCharacterData.szName, szNewNamePet);
			pcUnitData->sCharacterData.iType = CHARACTERTYPE_NPC;
			pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Summon;
			pcUnitData->sCharacterData.sSize = (short)(scPetSkin->fPetSize * 256.0f);
			pcUnitData->pcAttackTarget = NULL;
			pcUnitData->pcFocusTarget = NULL;
			pcUnitData->pcFocusingTarget = NULL;
			pcUnitData->sCharacterData.iOwnerID = pcUserData->iID;
			pcUnitData->Animate(ANIMATIONTYPE_Idle);
			pcUnitData->pcExclusiveTarget = pcUserData;
			pcUnitData->lExclusiveTargetID = pcUserData->iID;
			pcUnitData->sCharacterData.iAbsorbRating = 1000;
			pcUnitData->pcMapSpawn = NULL;
			pcUnitData->pcOwner = pcUserData;
			pcUnitData->dwUpdateCharInfoTime = TICKCOUNT + (300 * 1000);
			pcUnitData->iLureDistance = DISTANCE_XY_10_meters;
			pcUserData->dwObjectPetSystemID = pcUnitData->iID;
			pcUserData->pcPetSystem = pcUnitData;
			pcUserData->bPetSystemSkinID = scPetSkin->iID;

			PacketUserWarpEffect sPacketEffect;
			ZeroMemory(&sPacketEffect, sizeof(PacketUserWarpEffect));
			sPacketEffect.iID = 0x100;
			sPacketEffect.iX = pcUserData->sPosition.iX;
			sPacketEffect.iY = pcUserData->sPosition.iY;
			sPacketEffect.iZ = pcUserData->sPosition.iZ;
			CALL_WITH_ARG2(0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect);
		}
	}
}

void CPetSystemHandler::CheckPetRespawn(UserData* pcUserData)
{
	if (pcUserData->bPetSystemSummomTimeDelay >= 10)
		pcUserData->bPetSystemSummomTimeDelay = 10;
	else
		pcUserData->bPetSystemSummomTimeDelay++;

	if (pcUserData->dwObjectPetSystemID != 0)
		return;
	if (pcUserData->bTickCountPetSystem >= 5)
	{
		pcUserData->bTickCountPetSystem = 0;
		SpawnPet(pcUserData);
	}
	else
		pcUserData->bTickCountPetSystem++;	
}

void CPetSystemHandler::RemovePet(UserData* pcUserData)
{
	if (!pcUserData || !pcUserData->pcPetSystem && GAME_SERVER)
		return;
	UnitData* pcUnitData = pcUserData->pcPetSystem;

	pcUnitData->SetCurrentHealth(0);
	pcUnitData->Animate(ANIMATIONTYPE_Die);

	pcUserData->bPetSystemSkinID = 0;
	pcUserData->dwObjectPetSystemID = 0;
	pcUserData->pcPetSystem == nullptr;
	
}

void CPetSystemHandler::ChangePetName(UserData* pcUserData, char* szNewPetName)
{
	if (pcUserData && !pcUserData->szAccountName[0] && STRLEN(szNewPetName) <= 32)
	{
		char szUserID[32] = { 0 };
		ZeroMemory(szUserID, 32);

		char* pszCharName = CHARACTERSERVER->GetCharacterName(pcUserData);

		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_PetSystem);
		if (pcDB->Open())
		{
			if (pcDB->Prepare("UPDATE PetSystem SET [PetSystemName] = ? WHERE Name=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, szNewPetName, 32);
				pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharName, 32);
				pcDB->ExecuteUpdate();
			}
			pcDB->Close();
		}
	}
}

void CPetSystemHandler::ChangePetSkin(UserData* pcUserData, int iPetID)
{
	if (pcUserData  && iPetID)
	{
		char szUserID[32] = { 0 };
		ZeroMemory(szUserID, 32);

		char* pszCharName = CHARACTERSERVER->GetCharacterName(pcUserData);

		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_PetSystem);
		if (pcDB->Open())
		{
			if (pcDB->Prepare("UPDATE PetSystem SET [PetSkinId] = ? WHERE PetOwnerName=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_Integer, &iPetID,sizeof(iPetID));
				pcDB->BindParameterInput(2, PARAMTYPE_String, pszCharName, 32);
				pcDB->ExecuteUpdate();
			}
			pcDB->Close();
		}
	}
}

PetSystem* CPetSystemHandler::GetPetData(UserData* pcUserData)
{
	PetSystem* scPetSystem = new PetSystem();
	if (pcUserData && pcUserData->szAccountName[0])
	{
		SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_PetSystem_Skin);
		if (pcDB->Open())
		{
			if (pcDB->Prepare("SELECT [PetName],[PetSkinId] FROM PetSystem WHERE PetOwnerName=?"))
			{
				pcDB->BindParameterInput(1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUserData), 32);
				//true supress error log execute sql if not have char owner
				if (pcDB->Execute(true) && pcDB->Fetch())
				{
					pcDB->GetData(1, PARAMTYPE_String, scPetSystem->szPetName, 32);
					pcDB->GetData(2, PARAMTYPE_Integer, &scPetSystem->iPetskin);

				}
				else
					return nullptr;
			}
			pcDB->Close();
		}
	}
	return scPetSystem;
}

void CPetSystemHandler::LoadSkinData()
{
	vPetSystemSkin.clear();
	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_PetSystem_Skin);
	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT [id],[PetNameSummon],[PetSize], [PetRarity] FROM [GameDB].[dbo].[PetSystemSkin]"))
		{
			if (pcDB->Execute())
			{
				while (pcDB->Fetch())
				{
					PetSystemSkin* scPetSytemSkin = new PetSystemSkin();
					BYTE bPetSkinRarity = 0;
					pcDB->GetData(1, PARAMTYPE_Integer, &scPetSytemSkin->iID);
					pcDB->GetData(2, PARAMTYPE_String, scPetSytemSkin->szPetNameSummon, 32);
					pcDB->GetData(3, PARAMTYPE_Float, &scPetSytemSkin->fPetSize);
					pcDB->GetData(4, PARAMTYPE_Byte, &bPetSkinRarity);
					scPetSytemSkin->eStablePetRarity = (StablePetRarity)bPetSkinRarity;
					vPetSystemSkin.push_back(scPetSytemSkin);
				}
			}
		}
		pcDB->Close();
	}
}
