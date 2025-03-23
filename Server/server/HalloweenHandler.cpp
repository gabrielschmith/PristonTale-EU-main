#include "stdafx.h"
#include "HalloweenHandler.h"


CHalloweenHandler::CHalloweenHandler()
{
	dwRuneEvil[0]		= ITEMID_SpiderPlastic;
	dwRuneEvil[1]		= ITEMID_GriffenEgg;
	dwRuneEvil[2]		= ITEMID_ToiletPaper;

	dwRuneInfernal[0]	= ITEMID_GriffenEgg;
	dwRuneInfernal[1]	= ITEMID_HopyToy;
	dwRuneInfernal[2]	= ITEMID_ToiletPaper;
	iCombinationID		= 0;
}


CHalloweenHandler::~CHalloweenHandler()
{
}

void CHalloweenHandler::Init()
{
	if ( EVENT_HALLOWEEN )
	{

	}
}

void CHalloweenHandler::SendEventStatusToAllUsers ( )
{
	DEBUG ( "CHalloweenHandler: SendEventStatusToAllUsers" );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * pcUser = USERDATATOUSER ( &USERSDATA[i] );
		if ( pcUser && pcUser->IsConnected () && pcUser->IsInGame () && pcUser->pcUserData )
		{
			SendEventStatusToUser ( pcUser );
		}
	}
}

void CHalloweenHandler::SendEventStatusToUser ( User * pcUser )
{
	//std::cout << "SendEventStatusToUser" << std::endl;

	PacketHalloweenEventStatus packet;

	ZeroMemory ( &packet, sizeof ( PacketHalloweenEventStatus ) );
	packet.iHeader = PKTHDR_HalloweenEventStatus;
	packet.iLength = sizeof ( PacketHalloweenEventStatus );
	packet.bStatus = EVENT_HALLOWEEN;

	SENDPACKET ( pcUser, &packet );
}

BOOL CHalloweenHandler::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{
	/*
	if ( !EVENT_HALLOWEEN && pcUser->pcUserData->iGameLevel == GAMELEVEL_None )
		return FALSE;

	// Checksum
	for ( int i = 0; i < 3; i++ )
	{
		if ( ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->dwRuneCode[i], psPacket->dwRuneHead[i], psPacket->dwRuneCheckSum[i] ) < 0 )
		{
			// TODO
		}
	}

		if ( ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->dwRecipeCode, psPacket->dwRecipeHead, psPacket->dwRecipeChecksum ) < 0 )
		{
			// TODO
		}


	if ( psPacket->dwRecipeCode != ITEMID_CoreHW )
		return FALSE;

	DWORD dwRune[3];
	dwRune[0] = psPacket->dwRuneCode[0];
	dwRune[1] = psPacket->dwRuneCode[1];
	dwRune[2] = psPacket->dwRuneCode[2];

	DWORD dwCreateItem = ITEMID_RottenLantern;

	if ( dwRune[0] == dwRuneInfernal[0] && dwRune[1] == dwRuneInfernal[1] && dwRune[2] == dwRuneInfernal[2] )
	{
		dwCreateItem = ITEMID_InfernalLantern;
	}
	else if ( dwRune[0] == dwRuneEvil[0] && dwRune[1] == dwRuneEvil[1] && dwRune[2] == dwRuneEvil[2] )
	{
		dwCreateItem = ITEMID_EvilLantern;
	}

	DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( dwCreateItem );

	if ( !psItemDef )
		return FALSE;

	ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef );
	psPacket->iResult = TRUE;
	CHARACTERSERVER->GiveGOLD( pcUser, -psPacket->sItemData.sItem.iSalePrice );
	ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 );
	psPacket->iLength = sizeof( PacketManufactureItem );
	psPacket->iHeader = PKTHDR_ManufactureItem;
	SENDPACKET( pcUser, psPacket );
	*/
	return TRUE;

}

BOOL CHalloweenHandler::OnSpawnEventMonster( struct CharacterData * psCharInfo )
{
	if ( GAME_SERVER && EVENT_HALLOWEEN )
	{
		int iRand = Dice::RandomI(0, 999);
		if ( iRand < RATE_HALLOWEEN )
		{
			iRand = Dice::RandomI(0, 99);
			if ( iRand < 65 )
			{
				CharacterData * psChar = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_BEAR );
				if ( psChar )
				{
					CopyMemory( psCharInfo, psChar, sizeof( CharacterData ) );
					return TRUE;
				}
			}
			else
			{
				CharacterData * psChar = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_BRAZILBEAR );
				if ( psChar )
				{
					CopyMemory( psCharInfo, psChar, sizeof( CharacterData ) );
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CHalloweenHandler::OnUnitKilled( UnitData * pcUnitData )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_BRAZILBEAR )
		{
			int iRand = Dice::RandomI(0, 99);
			if ( iRand < 15 )
			{
				CharacterData * psChar = UNITSERVER->GetCharacterDataByEffect( MONSTEREFFECTID_MRGHOST, EMonsterClass::Ghost );
				if ( psChar )
				{
					UNITSERVER->CreateUnitEnemy( psChar->szName, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CHalloweenHandler::SetCombinationID( int iID )
{
	if ( iCombinationID != iID )
	{
		iCombinationID = iID;

		if ( iCombinationID > 3 )
			iCombinationID = 0;

		if ( iCombinationID == 1 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_SpiderPlastic;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		else if ( iCombinationID == 2 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_HopyToy;

			dwRuneInfernal[0] = ITEMID_HopyToy;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		else if ( iCombinationID == 3 )
		{
			dwRuneEvil[0] = ITEMID_SpiderPlastic;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_HopyToy;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_ToiletPaper;
			dwRuneInfernal[2] = ITEMID_HopyToy;
		}
		else
		{
			dwRuneEvil[0] = ITEMID_HopyToy;
			dwRuneEvil[1] = ITEMID_GriffenEgg;
			dwRuneEvil[2] = ITEMID_ToiletPaper;

			dwRuneInfernal[0] = ITEMID_GriffenEgg;
			dwRuneInfernal[1] = ITEMID_SpiderPlastic;
			dwRuneInfernal[2] = ITEMID_ToiletPaper;
		}
		if ( EVENT_HALLOWEEN )
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "Halloween> Rune Combination Changed!" );
	}
}

void CHalloweenHandler::Tick()
{
	if ( GAME_SERVER )
		return;

	if ( !dwUpdateCombinationTime )
		dwUpdateCombinationTime = TICKCOUNT + (60 * 60 * 8 * 1000);

	if ( TICKCOUNT > dwUpdateCombinationTime )
	{
		SetCombinationID( GetCombinationID() + 1 );
		dwUpdateCombinationTime = TICKCOUNT + (60 * 60 * 8 * 1000);
	}
}
