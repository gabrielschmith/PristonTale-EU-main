#include "stdafx.h"
#include "ChristmasHandler.h"
#include "Logger.h"


CChristmasHandler::CChristmasHandler()
{
}


CChristmasHandler::~CChristmasHandler()
{
}

BOOL CChristmasHandler::IsChristmasRune( DWORD dwCode )
{
	BOOL bResult = FALSE;
	switch ( dwCode )
	{
		case ITEMID_RedButton:
		case ITEMID_GreenCloth:
		case ITEMID_Cotton:
			bResult = TRUE;
			break;

		default:
			break;
	}
	return bResult;
}

BOOL CChristmasHandler::OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( GAME_SERVER && EVENT_CHRISTMAS )
	{
		if (pcMap->pcBaseMap->iMapID == MAPID_GardenOfFreedom ||
			pcMap->pcBaseMap->iMapID == MAPID_BambooForest ||
			pcMap->pcBaseMap->iMapID == MAPID_RoadToTheWind ||
			pcMap->pcBaseMap->iMapID == MAPID_ValleyOfTranquility)
		{
			//don't spawn in these maps. let player level  < 10 just enjoy their
			// first game experience without getting killed by santas
			return FALSE;
		}

		const int chance = 1; // 1 in 100 monsters

		if (pcMap->pcBaseMap->iLevel < 40)
		{
			if (Dice::RandomI(0, 79) < chance)
			{
				CharacterData* psChar = UNITSERVER->GetCharacterDataByName("Santa Goblin");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));

					return TRUE;
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 40 && pcMap->pcBaseMap->iLevel < 70)
		{
			if (Dice::RandomI(0, 69) < chance)
			{
				CharacterData* psChar = UNITSERVER->GetCharacterDataByName("Santa Mighty Goblin");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));

					return TRUE;
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 70 && pcMap->pcBaseMap->iLevel <= 120)
		{
			if (Dice::RandomI(0, 59) < chance)
			{
				CharacterData* psChar = UNITSERVER->GetCharacterDataByName("Santa Super Goblin");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CChristmasHandler::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{	
	if ( GAME_SERVER || !EVENT_CHRISTMAS )
		return FALSE;

	EItemID dwRune[3];
	dwRune[0] = psPacket->iaRuneID[0];
	dwRune[1] = psPacket->iaRuneID[1];
	dwRune[2] = psPacket->iaRuneID[2];

	if ( IsChristmasRune( dwRune[0] ) && IsChristmasRune( dwRune[1] ) && IsChristmasRune( dwRune[2] ) )
	{
		if ( (dwRune[0] == dwRune[1]) || (dwRune[0] == dwRune[2]) ||
			 (dwRune[1] == dwRune[0]) || (dwRune[1] == dwRune[2]) ||
			 (dwRune[2] == dwRune[0]) || (dwRune[2] == dwRune[1]) )
		{
			return FALSE;
		}

		// Checksum
		for (int i = 0; i < 3; i++)
		{
			if (ITEMSERVER->DeleteItemInventory(pcUser->pcUserData, psPacket->iaRuneID[i], psPacket->iaChk1[i], psPacket->iaChk2[i]) < 0)
			{
				LogItem sLog;
				sLog.ItemCount = 1;
				sLog.Flag = ITEMLOGID_ManufactureFail;
				sLog.Item[0].dwCode = psPacket->iaRuneID[i];
				sLog.Item[0].dwHead = psPacket->iaChk1[i];
				sLog.Item[0].dwCheckSum = psPacket->iaChk2[i];
				LOGSERVER->OnLogItem(LogServer::LOGTYPEID_LogItem, 1, &sLog);

				PacketLogCheat sCheat;
				sCheat.iCheatID = CHEATLOGID_ManufactureItemFake;
				sCheat.LParam = psPacket->iaRuneID[i];
				sCheat.SParam = psPacket->iaChk1[i];
				sCheat.EParam = psPacket->iaChk2[i];
				LOGSERVER->OnLogCheat(pcUser->pcUserData, &sCheat);
				return FALSE;
			}
		}

 						
		// Get Old Code
		DWORD dwHeadOld = 0, dwCheckSumOld = 0;
		dwHeadOld = psPacket->sItemData.sItem.iChk1;
		dwCheckSumOld = psPacket->sItemData.sItem.iChk2;

 		if ( !ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->iRecipeID, dwHeadOld, dwCheckSumOld ) )
 		{
 			// TODO
			LogItem sLog;
			sLog.ItemCount = 1;
 			sLog.Flag = ITEMLOGID_ManufactureFail;
 			sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToInt();
 			sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
 			sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
 			LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

 			PacketLogCheat sCheat;
 			sCheat.iCheatID = CHEATLOGID_ManufactureItemFake;
 			sCheat.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
 			sCheat.SParam = psPacket->sItemData.sItem.iChk1;
 			sCheat.EParam = psPacket->sItemData.sItem.iChk2;
 			LOGSERVER->OnLogCheat( pcUser->pcUserData, &sCheat );
			return FALSE;
 		}

		DWORD dwCreateItem = 0;

		int iPrice = 0;

		// Cloat
		if ( psPacket->iRecipeID == ITEMID_RecipeChristmas01 )
		{
			//same as what is defined in game.exe
			iPrice = 10'000;

			int iRand = Dice::RandomI(0, 99);
			if ( iRand < 50 )
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if ( iRand < 60 )
					dwCreateItem = ITEMID_ChristmasCloatCostumeTM;
				else
					dwCreateItem = ITEMID_ChristmasCloatCostumeTF;
			}
			else
			{
				dwCreateItem = ITEMID_ChristmasRedHat;
			}
		}

		// Elegant
		else if ( psPacket->iRecipeID == ITEMID_RecipeChristmas02 )
		{
			//same as what is defined in game.exe
			iPrice = 100'000;

			int iRand = Dice::RandomI(0, 99);
			if ( iRand < 60) //50% chance
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if ( iRand < 60)
					dwCreateItem = ITEMID_ChristmasElegantCostumeM;
				else
					dwCreateItem = ITEMID_ChristmasElegantCostumeF;
			}
			else if (iRand < 90) //30% chance
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if (iRand < 60)
					dwCreateItem = ITEMID_ChristmasCloatCostumeTM;
				else
					dwCreateItem = ITEMID_ChristmasCloatCostumeTF;
			}
			else
			{
				dwCreateItem = ITEMID_ChristmasRedHat;
			}
		}

		if ( dwCreateItem )
		{
			DefinitionItem* psItemDef = ITEMSERVER->FindItemDefByCode(dwCreateItem);

			if (!psItemDef)
				return FALSE;

			CHARACTERSERVER->GiveGOLD( pcUser, -iPrice);

			ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef, EItemSource::ChristmasEvent );

			psPacket->sItemData.sItem.eItemLastSource = EItemSource::PlayerTrade;
			psPacket->sItemData.sItem.eItemOriginalSource = EItemSource::PlayerTrade;

			psPacket->iResult = TRUE;
			ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 );
			psPacket->iLength = sizeof( PacketManufactureItem );
			psPacket->iHeader = PKTHDR_ManufactureItem;
			SENDPACKET( pcUser, psPacket );

			return TRUE;
		}
	}
	return FALSE;
}

void CChristmasHandler::OnSayTime( UnitData * pcUnitData )
{
	/*if ( pcUnitData->dwMessageEndTime < TICKCOUNT )
	{
		if ( STRINGCOMPAREI( pcUnitData->sCharacterData.szName, "Santa Skeleton" ) )
		{
			if ( (*(UINT*)pcUnitData->psModelAnimation) == ANIMATIONTYPE_Die )
			{
				char * pszaSkeletonXMASPhrase[6]
				{
					"I Kidnaped Santa Claus and smashed him with a stick, hahaha",
					"You will be killed this Christmas! muahahahaha",
						"See you in the next life (:",
						"I know that I'm a computed data turned into a monster!",
						"GM-Prog's PC can't handle me! buggy pc",
						"Merry XMas in hell ((:",
				};
				int iRand = Dice::RandomI(0, 5);
				pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;
				CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaSkeletonXMASPhrase[iRand] );
			}
		}
		else if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_HOBGOBLIN && pcUnitData->sCharacterData.iLevel == 60 )
		{
			if ( (*(UINT*)pcUnitData->psModelAnimation) == ANIMATIONTYPE_Die )
			{
				char * pszaSantaGoblinPhrase[6]
				{
						"",
						"I'm your father! Merry Christmas (;",
						"Turn back on musics of Christmas, have something for you! HEHEHE",
						"Do you like Christmas? I like to kill you!",
						"Are you a good boy? hehehehe 666",
						"Your parents are liers, Santa Claus doesn't exist!",
				};
				int iRand = Dice::RandomI(0, 5);
				pcUnitData->dwMessageEndTime = TICKCOUNT + 60000;

				if ( iRand == 0 )
				{
					if ( pcUnitData->pcKillerData )
						CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s, I will catch you this night! MUAHAHA", pcUnitData->sCharacterData.szName, CHARACTERSERVER->GetCharacterName( pcUnitData->pcKillerData ) );
				}
				else
					CHATSERVER->SendUserBoxChatRange( pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, 0x4B000, pcUnitData->iID, "%s: %s", pcUnitData->sCharacterData.szName, pszaSantaGoblinPhrase[iRand] );
			}
		}
	}*/
}
