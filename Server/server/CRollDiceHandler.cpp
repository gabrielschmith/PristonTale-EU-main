#include "stdafx.h"
#include "CRollDiceHandler.h"

CRollDiceHandler::CRollDiceHandler()
{
	for ( int i = 0; i < 200; i++ )
	{
		vListMap.push_back( new RollDiceMap( i ) );
	}
}

CRollDiceHandler::~CRollDiceHandler()
{
}


void CRollDiceHandler::ProcessPacket( User * pcUser, PacketRollDiceRanking * psPacket )
{
	psPacket->iHeader = PKTHDR_RollDiceRanking;
	psPacket->iLength = sizeof(PacketRollDiceRanking);

	SENDPACKET( pcUser, psPacket, TRUE );
}

/// <summary>
/// Received from user on accept or pass to a roll window
/// </summary>
void CRollDiceHandler::HandlePacket( User * pcUser, PacketRollDiceDrop * psPacket )
{
	if ( !pcUser || !psPacket )
		return;

	if ( psPacket->iID == pcUser->GetID() )
	{
		if ( (pcUser->GetMapID() >= 0) && ((UINT)pcUser->GetMapID() < vListMap.size()) )
		{
			std::vector<RollDiceDrop *> & vItemDrops = vListMap[pcUser->GetMapID()]->vListDrop;

			//Check if have item to roll
			if ( vItemDrops.size() > 0 )
			{
				RollDiceDrop * pcItemDrop = vItemDrops[0];

				//Item drop valid for usage?
				if ( (pcItemDrop) &&
					pcItemDrop->dwEndTime > TICKCOUNT &&
					pcItemDrop->vListUsers.size() > 0 &&
					pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() != ITEMID_None )
				{
					for ( auto & pcRollUser : pcItemDrop->vListUsers )
					{
						//Set Number by Index
						if ( pcRollUser->iID == psPacket->iID && pcRollUser->iRandomNumber == 0 && pcRollUser->pcUser == pcUser )
						{
							pcRollUser->iRandomNumber = psPacket->bAcceptItem ? Dice::RandomI( 1, 900 ) : 0;

							if ( psPacket->bAcceptItem )
								CHATSERVER->SendChatEx( pcRollUser->pcUser, CHATCOLOR_Global, "Roll> You rolled #%d, Good Luck!", pcRollUser->iRandomNumber );

							//Prepare Packet of Ranking to others players
							PacketRollDiceRanking sPacket;
							ZeroMemory( &sPacket, sizeof( PacketRollDiceRanking ) );

							STRINGCOPY( sPacket.szCharacterName, CHARACTERSERVER->GetCharacterName( pcRollUser->pcUser->pcUserData ) );
							sPacket.uNumber = pcRollUser->iRandomNumber;

							//Send for all players of dice roll
							for ( auto & pcOtherRollUser : pcItemDrop->vListUsers )
							{
								ProcessPacket( pcOtherRollUser->pcUser, &sPacket );
							}

							break;
						}
					}
				}

				if ( psPacket->bAcceptItem == FALSE )
				{
					for ( auto & pcRollUser : pcItemDrop->vListUsers )
					{
						CHATSERVER->SendChatEx( pcRollUser->pcUser, CHATCOLOR_Global, "Roll> %s PASS!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
					}
				}
			}
		}
	}
}

void CRollDiceHandler::Tick()
{
	for ( auto & vMap : vListMap )
	{
		std::vector<RollDiceDrop*> & vItemDrops = vMap->vListDrop;

		//Have item to roll?
		if ( vItemDrops.size() > 0 )
		{
			RollDiceDrop * pcItemDrop = vItemDrops[0]; //it is important to select first item, not last item, because the first drop appears in 60s, second drop appears in 120s etc.

			//Item drop valid for usage?
			if ( (pcItemDrop) && pcItemDrop->vListUsers.size() > 0 &&
				 pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() != ITEMID_None )
			{
				//if dwEndTime is -1, so will send the roll dice to all users in the array
				if ( pcItemDrop->dwEndTime == -1 )
				{
					pcItemDrop->dwEndTime = TICKCOUNT + (MAX_ROLLDICEDROP_TIME * 1000);

					PacketRollDiceDropItem sPacket;
					ZeroMemory( &sPacket, sizeof( PacketRollDiceDropItem ) );
					sPacket.iHeader = PKTHDR_RollDiceDropItem;
					sPacket.iLength = sizeof(PacketRollDiceDropItem);
					sPacket.eItemID = pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID();
					sPacket.bAcceptItem = FALSE;

					CopyMemory( &sPacket.sItem, &pcItemDrop->psLoot->sLootItem.sItem, sizeof( Item ) );

					//Packet for user confirmation box
					for ( auto &pcRollUser : pcItemDrop->vListUsers )
					{
						if ( pcRollUser->iID && pcRollUser->iRandomNumber == -1 && pcRollUser->pcUser )
						{
							sPacket.iID = pcRollUser->pcUser->GetID();

							pcRollUser->iRandomNumber = 0;

							SENDPACKET( pcRollUser->pcUser, &sPacket, TRUE );
						}
					}
				}
				else if ( TICKCOUNT > pcItemDrop->dwEndTime )
				{
					int iLastNumber = -1;
					User * pcWinnerUser = NULL;

					//Check for the Random number
					for ( const auto &pcRollUser : pcItemDrop->vListUsers )
					{
						if ( pcRollUser->iID && pcRollUser->iRandomNumber > 0 && pcRollUser->pcUser )
						{
							if ( pcRollUser->iRandomNumber > iLastNumber )
							{
								iLastNumber = pcRollUser->iRandomNumber;
								pcWinnerUser = pcRollUser->pcUser;
							}
						}
					}

					//Have a Winner? so send the item and delete loot from the map...
					if ( iLastNumber > 0 && pcWinnerUser )
					{
						ItemData * psItemData = ITEMSERVER->FindItemPointerTable( pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() );
						if ( psItemData )
						{
							if ( pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemType() == ITEMTYPE_MoneyChest )
							{
								DefinitionItem * psDefItem = ITEMSERVER->FindItemDefByCode( pcItemDrop->psLoot->sLootItem.sItem.sItemID.ToItemID() );
								if ( psDefItem )
								{
									//Send item to Distributor
									if ( ITEMSERVER->AddItemOpenBox( pcWinnerUser->pcUserData, psItemData->szInventoryName, psDefItem->sItem.iSalePrice ) )
									{
										CHATSERVER->SendChat( pcWinnerUser, CHATCOLOR_Error, "> Check Item Distributor to get your item!" );
									}
									else
									{
										WARN( "Item '%s' (%d) was not sent to distributor!", psItemData->szInventoryName, psDefItem->sItem.iSalePrice );
									}
								}
							}
							else
							{
								//Send item to Distributor
								if ( ITEMSERVER->AddItemOpenBox( pcWinnerUser->pcUserData, psItemData->szInventoryName, 0, 1, FALSE, &pcItemDrop->psLoot->sLootItem.sItem ) )
								{
									CHATSERVER->SendChat( pcWinnerUser, CHATCOLOR_Error, "> Check Item Distributor to get your item!" );
								}
								else
								{
									WARN( "Item '%s' was not sent to distributor!", psItemData->szInventoryName );
								}
							}

							//Chat Alert
							if ( pcItemDrop->vListUsers.size() > 1 )
								CHATSERVER->SendChatAllUsersInStage( pcWinnerUser->GetMapID(), CHATCOLOR_Global, "Roll> %s #%d won %s from %d players!", CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ), iLastNumber, pcItemDrop->psLoot->sLootItem.sItem.szItemName, pcItemDrop->vListUsers.size() );
							else
								CHATSERVER->SendChatAllUsersInStage( pcWinnerUser->GetMapID(), CHATCOLOR_Global, "Roll> %s #%d won %s!", CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ), iLastNumber, pcItemDrop->psLoot->sLootItem.sItem.szItemName );

							//Log of send item
							LOGSERVER->OnLogItemEx( pcWinnerUser, ITEMLOGID_RollDiceWon, "Character[%s] Won Roll Dice MapID[%d] ItemName[%s] TotalUsers[%d] RollNumber[%d]",
								CHARACTERSERVER->GetCharacterName( pcWinnerUser->pcUserData ),
								pcWinnerUser->GetMapID(),
								pcItemDrop->psLoot->sLootItem.sItem.szItemName,
								pcItemDrop->vListUsers.size(),
								iLastNumber );

							//Delete Item from Map
							Map * pcMap = MAPSDATA + pcItemDrop->iMapID;
							if ( pcMap )
							{
								pcMap->DeleteItem( pcItemDrop->psLoot );
							}

							//Delete Item Drop from Handler
							DeleteItemDrop( pcItemDrop, pcItemDrop->iMapID );

							SENDPACKETBLANK( pcWinnerUser, PKTHDR_PacketFun ); //dance
						}
					}
					else
					{
						pcItemDrop->psLoot->dwDropTime			= TICKCOUNT + 1000;				//drop in 1s
						pcItemDrop->psLoot->dwEndTime			= TICKCOUNT + (4 * 60 * 1000);	//expire in 4s
						pcItemDrop->psLoot->eLootType			= ELootType::Initial;
						pcItemDrop->psLoot->Level				= 0;

						DeleteItemDrop( pcItemDrop, pcItemDrop->iMapID );
					}
				}
			}
		}
	}
}

BOOL CRollDiceHandler::OnExchangeItemDrop( UnitData * pcUnitData, Map * pcMap, Loot * psLoot, int iDropIndex )
{
	BOOL bRet = FALSE;

	//Unit Boss?
	if ( pcUnitData->sCharacterData.sMonsterClass == EMonsterClass::Boss &&
	   ( psLoot->sLootItem.sItem.sItemID.ToItemType() != ITEMTYPE_GoldAndExp &&
		 psLoot->sLootItem.sItem.sItemID.ToItemBase() != ITEMBASE_Potion) )
	{
		int iDamageNeeded = (pcUnitData->GetMaxHealth() * ROLLDICE_DAMAGE_PERCENT) / 100;

		std::vector<RollDiceUser*> v;

		if ( pcUnitData->psaDamageUsersData )
		{
			for ( int i = 0; i < PLAYERS_MAX; i++ )
			{
				UserData * ud = USERSDATA + i;
				User * pcUser = USERDATATOUSER( ud );

				AttackDamageData * psDamageData = NULL;

				//User is In-game and on Unit Users List?
				if ( pcUser->IsConnected() && pcUser->IsInGame() &&
				   ( psDamageData = UNITSERVER->GetUserInDamageList( pcUnitData, pcUser )) )
				{
					//Compute distance beetween User and Unit
					int iX = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
					int iZ = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
					iX *= iX;
					iZ *= iZ;
					int iDistance = iX + iZ;
					if ( iDistance < DISTANCE_MAX_BOSS )
					{
						//User damage are >= % needed?
						if ( psDamageData->iDamage >= iDamageNeeded )
						{
							//Check if have same IP...
							BOOL bAllowedUser = TRUE;
							for ( const auto vUser : v )
							{
								if ( vUser->pcUser->pcUserData->pcSocketData )
								{
									if ( vUser->pcUser->pcUserData->pcSocketData->lIP == pcUser->pcUserData->pcSocketData->lIP )
									{
										bAllowedUser = FALSE;
										break;
									}
								}
							}

							//Add User to Roll Dice...
							if ( bAllowedUser )
							{
								v.push_back( new RollDiceUser( pcUser->GetID(), pcUser ) );

								bRet = TRUE;

								//Log it...
								LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_RollDiceJoin, "Character[%s] Join on Roll Dice MapID[%d]",
									CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->GetMapID() );
							}
						}
					}
				}
			}
		}

		//Add Drop...
		if ( bRet )
		{
			AddItemDrop( new RollDiceDrop( v, psLoot, pcMap->pcBaseMap->iMapID, pcUnitData->iID ), pcMap->pcBaseMap->iMapID );
			psLoot->dwDropTime = TICKCOUNT + (iDropIndex * 60000);
			psLoot->dwEndTime += (iDropIndex * 60000);
			psLoot->eLootType = ELootType::Initial;
			psLoot->Level = 10;
		}
	}


	return bRet;
}

BOOL CRollDiceHandler::TestItemDiceRoll( User * pcUser, const std::string szItemCode, EItemSource eItemSource, EItemRarity eItemRarity )
{
	std::vector<RollDiceUser*> v;
	v.push_back( new RollDiceUser( pcUser->GetID(), pcUser ) );

	Map * lcMap = MAPSDATA + (int)pcUser->GetMapID();

	int iDropIndex = 1;

	auto pDefItem = ITEMSERVER->FindItemDefByCode((char*)szItemCode.c_str());
	if ( pDefItem )
	{
		LootItem sItemLoot{};
		ZeroMemory( &sItemLoot, sizeof(LootItem) );
		ITEMSERVER->CreateItem( &sItemLoot.sItem, pDefItem, eItemSource, 0, 0, 0, eItemRarity );
		sItemLoot.bUse = TRUE;

		Loot* psItemD = MAPSERVER->AddItemV2(lcMap, &sItemLoot, pcUser->pcUserData->sPosition.iX >> 8, pcUser->pcUserData->sPosition.iY >> 8, pcUser->pcUserData->sPosition.iZ >> 8, ELootType::Initial);
		if ( psItemD )
		{
			psItemD->dwDropTime += 5000;

			AddItemDrop( new RollDiceDrop( v, psItemD, lcMap->pcBaseMap->iMapID, pcUser->pcUserData->iID ), lcMap->pcBaseMap->iMapID );
			psItemD->dwDropTime = TICKCOUNT + (iDropIndex * 60000);
			psItemD->dwEndTime += (iDropIndex * 60000);
			psItemD->eLootType = ELootType::Initial;
			psItemD->Level = 10;

			return TRUE;
		}
	}

	return FALSE;
}

void CRollDiceHandler::AddItemDrop( RollDiceDrop * p, int iMapID )
{
	if ( p )
	{
		if( (iMapID >= 0) && ((UINT)iMapID < vListMap.size()) )
			vListMap[iMapID]->vListDrop.push_back( p );
	}
}

void CRollDiceHandler::DeleteItemDrop( RollDiceDrop * p, int iMapID )
{
	if ( (iMapID >= 0) && ((UINT)iMapID < vListMap.size()) )
	{
		std::vector<RollDiceDrop *> & vItemDrops = vListMap[iMapID]->vListDrop;

		for ( std::vector<RollDiceDrop *>::iterator it = vItemDrops.begin(); it != vItemDrops.end(); )
		{
			RollDiceDrop * pcItemDrop = (*it);
			if ( pcItemDrop == p )
			{
				//Delete Users
				for ( std::vector<RollDiceUser *>::iterator it = pcItemDrop->vListUsers.begin(); it != pcItemDrop->vListUsers.end(); it++ )
				{
					RollDiceUser * pcRollUser = (*it);
					SAFE_DELETE ( pcRollUser );
				}

				//Clear User List
				pcItemDrop->vListUsers.clear();

				//Delete drop
				SAFE_DELETE( pcItemDrop );

				it = vItemDrops.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
}
