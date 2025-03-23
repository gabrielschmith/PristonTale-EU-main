#include "stdafx.h"
#include "EasterHandler.h"
#include "Logger.h"


CGeneralEventHandler::CGeneralEventHandler()
{
}


CGeneralEventHandler::~CGeneralEventHandler()
{
}

BOOL CGeneralEventHandler::IsEasterRune( DWORD dwCode )
{
	BOOL bResult = FALSE;
	switch ( dwCode )
	{
		case ITEMID_RedEasterEgg:
		case ITEMID_GreenEasterEgg:
		case ITEMID_PinkEasterEgg:
			bResult = TRUE;
			break;

		default:
			break;
	}
	return bResult;
}

BOOL CGeneralEventHandler::OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( OnSpawnStarWarsMonsterEvent( psCharacterData, pcMap ) )
		return TRUE;

	if ( OnSpawnEasterMonsterEvent( psCharacterData, pcMap ) )
		return TRUE;

	if ( OnSpawnBeeMonsterEvent( psCharacterData, pcMap ) )
		return TRUE;

	return FALSE;
}


BOOL CGeneralEventHandler::OnSpawnStarWarsMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( GAME_SERVER && EVENT_STARWARS)
	{
		if (pcMap->pcBaseMap->iMapID == MAPID_GardenOfFreedom ||
			pcMap->pcBaseMap->iMapID == MAPID_BambooForest ||
			pcMap->pcBaseMap->iMapID == MAPID_RoadToTheWind ||
			pcMap->pcBaseMap->iMapID == MAPID_ValleyOfTranquility)
		{
			//don't spawn in these maps. let player level  < 10 just enjoy their
			// first game experience without getting killed by easter monsters
			return FALSE;
		}

		const int chance = 1; // 1 in 100 monsters
		CharacterData * psChar = NULL;

		if (pcMap->pcBaseMap->iLevel < 90)
		{
			if (Dice::RandomI(0, 99) < chance) //1 in 100 chance for spawn
			{	
				psChar = UNITSERVER->GetCharacterDataByName("Mama Krika");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Mama Krika' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 90 && pcMap->pcBaseMap->iLevel < 100)
		{
			if (Dice::RandomI(0, 59) < chance) //1 in 60 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Lady Mama Krika");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Lady Mama Krika' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 100 && pcMap->pcBaseMap->iLevel < 110)
		{
			if (Dice::RandomI(0, 39) < chance) //1 in 40 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Queen Mama Krika");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Queen Mama Krika' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 110 && pcMap->pcBaseMap->iLevel <= 120)
		{
			if (Dice::RandomI(0, 19) < chance) //1 in 25 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Majesty Mama Krika");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Majesty Mama Krika' does not exist!" );
				}
			}
		}
	}

	return FALSE;
}

BOOL CGeneralEventHandler::OnSpawnBeeMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( GAME_SERVER && EVENT_BEE)
	{
		if (pcMap->pcBaseMap->iMapID == MAPID_GardenOfFreedom ||
			pcMap->pcBaseMap->iMapID == MAPID_BambooForest ||
			pcMap->pcBaseMap->iMapID == MAPID_RoadToTheWind ||
			pcMap->pcBaseMap->iMapID == MAPID_ValleyOfTranquility)
		{
			//don't spawn in these maps. let player level  < 10 just enjoy their
			// first game experience without getting killed by easter monsters
			return FALSE;
		}
		
		const char * beeMonTier1 = "Foraging Bee"; //@Chaos to do
		const char * beeMonTier2 = "Worker Bee"; //@Chaos to do
		const char * beeMonTier3 = "Guard Bee"; //@Chaos to do
		const char * beeMonTier4 = "Killer Bee"; //@Chaos to do

		const int chance = 1; // 1 in 100 monsters
		CharacterData * psChar = NULL;

		if (pcMap->pcBaseMap->iLevel < 40)
		{
			if (Dice::RandomI(0, 19) < chance) //1 in 80 chance for spawn
			{		
				psChar = UNITSERVER->GetCharacterDataByName(beeMonTier1);
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'%s' does not exist!", beeMonTier1 );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 40 && pcMap->pcBaseMap->iLevel < 70)
		{
			if (Dice::RandomI(0, 19) < chance) //1 in 70 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName(beeMonTier2);
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'%s' does not exist!", beeMonTier2 );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 70 && pcMap->pcBaseMap->iLevel < 100)
		{
			if (Dice::RandomI(0, 19) < chance) //1 in 60 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName(beeMonTier3);
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'%s' does not exist!", beeMonTier3 );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 100 && pcMap->pcBaseMap->iLevel <= 120)
		{
			if (Dice::RandomI(0, 19) < chance) //1 in 50 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName(beeMonTier4);
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'%s' does not exist!", beeMonTier4 );
				}
			}
		}
	}

	return FALSE;
}

BOOL CGeneralEventHandler::OnSpawnEasterMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap )
{
	if ( GAME_SERVER && EVENT_EASTER)
	{
		if (pcMap->pcBaseMap->iMapID == MAPID_GardenOfFreedom ||
			pcMap->pcBaseMap->iMapID == MAPID_BambooForest ||
			pcMap->pcBaseMap->iMapID == MAPID_RoadToTheWind ||
			pcMap->pcBaseMap->iMapID == MAPID_ValleyOfTranquility)
		{
			//don't spawn in these maps. let player level  < 10 just enjoy their
			// first game experience without getting killed by easter monsters
			return FALSE;
		}

		const int chance = 1; // 1 in 100 monsters
		CharacterData * psChar = NULL;

		if (pcMap->pcBaseMap->iLevel < 40)
		{
			if (Dice::RandomI(0, 79) < chance) //1 in 80 chance for spawn
			{		
				psChar = UNITSERVER->GetCharacterDataByName("Easter Kakoa");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Easter Kakoa' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 40 && pcMap->pcBaseMap->iLevel < 70)
		{
			if (Dice::RandomI(0, 69) < chance) //1 in 70 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Easter Mighty Kakoa");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Easter Mighty Kakoa' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 70 && pcMap->pcBaseMap->iLevel < 100)
		{
			if (Dice::RandomI(0, 59) < chance) //1 in 60 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Easter Super Kakoa");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "Easter Super Kakoa' does not exist!" );
				}
			}
		}
		else if (pcMap->pcBaseMap->iLevel >= 100 && pcMap->pcBaseMap->iLevel <= 120)
		{
			if (Dice::RandomI(0, 49) < chance) //1 in 50 chance for spawn
			{
				psChar = UNITSERVER->GetCharacterDataByName("Easter MEGA Kakoa");
				if (psChar)
				{
					CopyMemory(psCharacterData, psChar, sizeof(CharacterData));
					return TRUE;
				}
				else
				{
					WARN( "'Easter MEGA Kakoa' does not exist!" );
				}
			}
		}
	}

	return FALSE;
}

BOOL CGeneralEventHandler::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{	
	if (GAME_SERVER || !EVENT_EASTER)
		return FALSE;

	EItemID dwRune[3];
	dwRune[0] = psPacket->iaRuneID[0];
	dwRune[1] = psPacket->iaRuneID[1];
	dwRune[2] = psPacket->iaRuneID[2];

	if ( IsEasterRune( dwRune[0] ) && IsEasterRune( dwRune[1] ) && IsEasterRune( dwRune[2] ) )
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
		if ( psPacket->iRecipeID == ITEMID_RecipeEaster01 )
		{
			//same as what is defined in game.exe
			iPrice = 10'000;

			int iRand = Dice::RandomI(0, 99);
			if ( iRand < 50 )
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if ( iRand < 60 )
					dwCreateItem = ITEMID_EasterHopyCostume;
				else
					dwCreateItem = ITEMID_EasterCuepyCostume;
			}
			else
			{
				dwCreateItem = ITEMID_SheepHat;
			}
		}

/*		// Elegant
		else if ( psPacket->iRecipeID == ITEMID_RecipeEaster02 )
		{
			//same as what is defined in game.exe
			iPrice = 100'000;

			int iRand = Dice::RandomI(0, 99);
			if ( iRand < 60) //50% chance
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if ( iRand < 60)
					dwCreateItem = ITEMID_EasterCuepyCostume;
				else
					dwCreateItem = ITEMID_EasterHopyCostume;
			}
			else if (iRand < 90) //30% chance
			{
				iRand = Dice::RandomI(0, 99);

				//6 males vs 4 females
				if (iRand < 60)
					dwCreateItem = ITEMID_EasterCuepyCostume;
				else
					dwCreateItem = ITEMID_EasterHopyCostume;
			}
			else
			{
				dwCreateItem = ITEMID_EasterSheepHat;
			}
		} */

		if ( dwCreateItem )
		{
			DefinitionItem* psItemDef = ITEMSERVER->FindItemDefByCode(dwCreateItem);

			if (!psItemDef)
				return FALSE;

			CHARACTERSERVER->GiveGOLD( pcUser, -iPrice);

			ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef, EItemSource::EasterEvent );
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

void CGeneralEventHandler::OnSayTime( UnitData * pcUnitData )
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
					"You will be killed this Easter! muahahahaha",
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
						"I'm your father! Merry Easter (;",
						"Turn back on musics of Easter, have something for you! HEHEHE",
						"Do you like Easter? I like to kill you!",
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
