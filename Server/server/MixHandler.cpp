#include "stdafx.h"
#include "MixHandler.h"
#include "Utilities.h"

#define MAX_CRAFTITEM_INFO 300
#define MAX_SHELTOM_TYPES 15
#define MAX_SHELTOMS 12

CMixHandler::CMixHandler()
{
}


CMixHandler::~CMixHandler()
{
}


void CMixHandler::OnSocketSystemNPCClicked ( User * pcUser, UnitData * pcUnitData )
{
	int iID = pcUnitData->sUnitInfo.NPCEventCode == NPCEVENTCODE_SocketSystemNPCDrill ? 0x0E : 0x0F;
	PacketSimple s;
	s.iLength = sizeof ( PacketSimple );
	s.iHeader = PKTHDR_OpenNPC;
	s.iUnk = 0;
	s.iUnk2 = iID;
	s.iUnk3 = 0;
	s.iUnk4 = 0;
	SENDPACKET ( pcUser, &s, TRUE );
}

void CMixHandler::SendPacketMixList ( User * pcUser )
{
	PacketMixGuild * psPacketMixGuild = new PacketMixGuild;
	ZeroMemory ( psPacketMixGuild, sizeof ( PacketMixGuild ) );
	psPacketMixGuild->iLength = sizeof ( PacketMixGuild );
	psPacketMixGuild->iHeader = PKTHDR_NPCMixGuild;

	//MIX GUILD
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_GameDB_Mixing );

	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( "SELECT TOP 70 * FROM MixList" ) )
		{
			if ( pcDB->Execute () && pcDB->Fetch () )
			{
				for ( int i = 0; i < 70; i++ )
				{
					pcDB->GetData ( 2, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iTypeMix );
					pcDB->GetData ( 3, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iLucidy );
					pcDB->GetData ( 4, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iSereneo );
					pcDB->GetData ( 5, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iFadeo );
					pcDB->GetData ( 6, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iSparky );
					pcDB->GetData ( 7, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iRaident );
					pcDB->GetData ( 8, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iTransparo );
					pcDB->GetData ( 9, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iMurky );
					pcDB->GetData ( 10, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iDevine );
					pcDB->GetData ( 11, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iCelesto );
					pcDB->GetData ( 12, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iMirage );
					pcDB->GetData ( 13, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iInferna );
					pcDB->GetData ( 14, PARAMTYPE_Integer, &psPacketMixGuild->sMixList[i].iEnigma );
					pcDB->GetData ( 43, PARAMTYPE_String, psPacketMixGuild->sMixDesc[i].Description, 32 );

					if ( !pcDB->Fetch () )
						break;
				}

			}
		}

		pcDB->Close ();
	}

	SENDPACKET ( pcUser, psPacketMixGuild );

	SAFE_DELETE ( psPacketMixGuild );
}

//only for login server
void CMixHandler::SQLLoadMixingList()
{
	if (GAME_SERVER)
		return;

	MixServerData* mixDataPointerArray = ((MixServerData*)0x006BB6B0);

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_Mixing);

	const int attributteStartColumnIndex = 23;	//First 'TypeAttribute' column (1-based)
	const int sheltomStartColumnIndex = 7;		//Lucidy column (1-based)

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT * FROM MixList ORDER BY ID ASC"))
		{
			if (pcDB->Execute())
			{
				int mixIndex = 0;
				int uniqueMixID = 0;
				int groupMixTypeID = 0;
				int typeMixValue = 0;

				while (mixIndex < MAX_CRAFTITEM_INFO)
				{
					if (!pcDB->Fetch())
						break;

					MixServerData* mixData = mixDataPointerArray + mixIndex;
					ZeroMemory(mixData, sizeof(MixServerData));

					// Mix Description
					char szDescription[64] = { 0 };

					//column pos 1 = ID (not used)
					pcDB->GetData(2, PARAMTYPE_Integer, &uniqueMixID);	 //should be unique
					pcDB->GetData(3, PARAMTYPE_Integer, &groupMixTypeID); //should be unique per mix type
					pcDB->GetData(4, PARAMTYPE_Integer, &typeMixValue);	 //mix type (weapon, armour etc)
					//column pos 5 = mix type string (not used)
					pcDB->GetData(6, PARAMTYPE_String, szDescription, 64);	 //mix description

					EMixType eTypeMix = (EMixType)typeMixValue;

					mixData->iType = eTypeMix;
					mixData->eMixTypeName = (EMixTypeName)groupMixTypeID;
					mixData->iMixUniqueId = uniqueMixID;

					STRINGCOPYLEN(mixData->szDescription, 64, szDescription);

					// Sheltom List
					for (int j = 0; j < MAX_SHELTOM_TYPES; j++)
					{
						UINT uSheltomCount = 0;
						pcDB->GetData(sheltomStartColumnIndex + j, PARAMTYPE_Integer, &uSheltomCount );

						mixData->iaSheltomCount[j] = uSheltomCount;

						if (uSheltomCount > 0)
						{
							mixData->iSheltomCount += uSheltomCount;
						}
					}

				//	std::cout << mixIndex << " : mixData->iSheltomCount = " << mixData->iSheltomCount << std::endl;

					// Atributtes
					//int iAttrPosition = 0;
					for (int j = 0; j < 8; j++)
					{
						int TypeAtr = 0;
						float Atr = 0;
						int PerAtr = 0;

						int typeAtrColumnIndex  = attributteStartColumnIndex + 0 + (j * 3);
						int atrColumnIndex      = attributteStartColumnIndex + 1 + (j * 3);
						int perAtrColumnIndex   = attributteStartColumnIndex + 2 + (j * 3);

						pcDB->GetData(typeAtrColumnIndex, PARAMTYPE_Integer, &TypeAtr);
						pcDB->GetData(atrColumnIndex, PARAMTYPE_Float, &Atr);
						pcDB->GetData(perAtrColumnIndex, PARAMTYPE_Integer, &PerAtr);

						if ((TypeAtr == 0) || ((float)Atr == 0.0f))
							continue;

						mixData->iaTypeAttributte[j] = TypeAtr;
						mixData->faAttributte[j] = (float)Atr;
						mixData->iaAttributtePercent[j] = PerAtr;
					}



					// Weapons
					if (eTypeMix == EMixType::MIXTYPE_Weapons)
					{
						mixData->iaItemID[0] = ITEMTYPE_Axe;
						mixData->iaItemID[1] = ITEMTYPE_Claw;
						mixData->iaItemID[2] = ITEMTYPE_Hammer;
						mixData->iaItemID[3] = ITEMTYPE_Wand;
						mixData->iaItemID[4] = ITEMTYPE_Scythe;
						mixData->iaItemID[5] = ITEMTYPE_Bow;
						mixData->iaItemID[6] = ITEMTYPE_Sword;
						mixData->iaItemID[7] = ITEMTYPE_Javelin;
						mixData->iaItemID[8] = ITEMTYPE_Phantom;
						mixData->iaItemID[9] = ITEMTYPE_Dagger;
					}
					// Armors and Robes
					else if (eTypeMix == EMixType::MIXTYPE_ArmourAndRobe)
					{
						mixData->iaItemID[0] = ITEMTYPE_Armor;
						mixData->iaItemID[1] = ITEMTYPE_Robe;
					}
					// Shields
					else if (eTypeMix == EMixType::MIXTYPE_Sheilds)
					{
						mixData->iaItemID[0] = ITEMTYPE_Shield;
					}
					// Orbs
					else if (eTypeMix == EMixType::MIXTYPE_Orbs)
					{
						mixData->iaItemID[0] = ITEMTYPE_Orb;
					}
					// Bracelets
					else if (eTypeMix == EMixType::MIXTYPE_Bracelet)
					{
						mixData->iaItemID[0] = ITEMTYPE_Bracelets;
					}
					// Gauntlets
					else if (eTypeMix == EMixType::MIXTYPE_Gauntlets)
					{
						mixData->iaItemID[0] = ITEMTYPE_Gauntlets;
					}
					// Boots
					else if (eTypeMix == EMixType::MIXTYPE_Boots)
					{
						mixData->iaItemID[0] = ITEMTYPE_Boots;
					}
					// Amulets
					else if (eTypeMix == EMixType::MIXTYPE_Amulets)
					{
						mixData->iaItemID[0] = ITEMTYPE_Amulet;
					}
					// Rings
					else if (eTypeMix == EMixType::MIXTYPE_Rings)
					{
						mixData->iaItemID[0] = ITEMTYPE_Ring;
					}

					mixIndex++;
				}
			}
		}
		pcDB->Close();
	}
}

void CMixHandler::SendMixDescription(UserData* pcUserData, const char * pMixDescription)
{
	if (pcUserData)
	{
		PacketMixDescription sPacket;
		sPacket.iLength = sizeof(PacketMixDescription);
		sPacket.iHeader = PKTHDR_MixDescription;
		STRINGCOPY(sPacket.szMixDescription, pMixDescription);

		//std::cout << "SendMixDescription = " << pMixDescription << std::endl;

		SENDPACKET(USERDATATOUSER(pcUserData), &sPacket);
	}
}

//Obsolete - from old mixing code in EXE
void CMixHandler::SendMixDescription(UserData* pcUserData, UINT puMix)
{
	if (pcUserData)
	{
		// Save Mix
		ITEMSERVER->SaveItemDataToDatabase((Item*)0x7878950);

		char* pszDescrition = (char*)((puMix * 0x1A4) + 0x006BB6B0 + 0x0BC);
		int iMixName = *(int*)((puMix * 0x1A4) + 0x006BB6B0 + 0x0BC + 64);
		((Item*)0x7878950)->eMixTypeName = (EMixTypeName)iMixName;

		SendMixDescription(pcUserData, pszDescrition);
	}
}

BOOL CMixHandler::OnMixingItem(User* pcUser, struct PacketMixServer* psPacket)
{
	//std::cout << "BOOL ItemServer::OnMixingItem( User * pcUser, struct PacketMixServer * psPacket )" << std::endl;

	BOOL bResult = FALSE;
	UserData* pcUserData = pcUser->pcUserData;


	bool l_ItemFound = false;
	int l_FindCount = 0;



	int l_ItemSlot = -1;
	for (l_ItemSlot = 0; l_ItemSlot < 316; l_ItemSlot++)
	{
		DropItemData& ps = pcUser->pcUserData->sIntentoryItems[l_ItemSlot];

		if (ps.iItemID == psPacket->sItemData.sItem.sItemID.ToItemID() &&
		   (ps.iChk1 == psPacket->sItemData.sItem.iChk1) &&
		   (ps.iChk2 == psPacket->sItemData.sItem.iChk2))
		{
			l_ItemFound = true;
			break;
		}
	}

	/// SCAM ATTEMPT
	if (!l_ItemFound)
	{
		WARN( "OnMixingItem item not found" );

		PacketLogCheat sPacket;
		sPacket.iCheatID = CHEATLOGID_MixingItemNotFound;
		sPacket.LParam = 0;
		sPacket.SParam = 0;
		sPacket.WxParam = 16;
		LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);

		if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
		{
			NETSERVER->DisconnectUser( pcUser->pcUserData );
			SENDPACKETBLANK( USERDATATOUSER( pcUser->pcUserData ), PKTHDR_Disconnect );
			WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );
		}

		return FALSE;
	}

	int l_BuyPrice = 0;
	switch (psPacket->sItemData.sItem.sItemID.ToItemID())
	{

		//quest item
	case 16844800:
	case 16910336:
	case 17041664:
	case 17107200:
	case 17172992:
	case 17238528:
	case 17303808:
	case 17369344:
	case 17434880:
		if (psPacket->sItemData.sItem.iSalePrice != 20000 && psPacket->sItemData.sItem.iSalePrice != 200000)
			l_BuyPrice = ITEMSERVER->GetItemBuyPrice(psPacket->sItemData.sItem.sItemID.ToItemID(), 0, (int)psPacket->sItemData.sItem.eRarity);

		break;

	case 135025408: //gold chest
		l_BuyPrice = 0;

	default:
		l_BuyPrice = ITEMSERVER->GetItemBuyPrice(psPacket->sItemData.sItem.sItemID.ToItemID(), 0, (int)psPacket->sItemData.sItem.eRarity);
		break;
	}

	if (psPacket->iGold < l_BuyPrice)
	{
		//std::cout << "Mixing packet price " << psPacket->iGold << " server price " << l_BuyPrice << std::endl;

		WARN( "Packet gold (%d) and DB buy price (%d) mismatch!", psPacket->iGold, l_BuyPrice );

		PacketLogCheat sPacket;
		sPacket.iCheatID = CHEATLOGID_GenerateGoldHack;
		sPacket.LParam = 0;
		sPacket.SParam = 0;
		sPacket.WxParam = 17;
		LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);

		if ( pcUser->GetGameLevel() != GAMELEVEL_Four )
		{
			NETSERVER->DisconnectUser( pcUser->pcUserData );

			SENDPACKETBLANK( USERDATATOUSER( pcUser->pcUserData ), PKTHDR_Disconnect );
			WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );
		}

		return FALSE;
	}


	//Socket Mix? Handle it elsewhere.
	if (psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_PurpleDiamond ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_SilverDiamond ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_GoldDiamond ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_RedMarquiseGem ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_GreenMarquiseGem ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_BlueMarquiseGem ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_YellowMarquiseGem ||
		psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_PurpleMarquiseGem)
	{
		SOCKETSYSTEMHANDLER->OnMixingItem(pcUser, psPacket);
		return TRUE;
	}

	//Every time the item get mixed, reduce integrity by -5
	if (psPacket->sItemData.sItem.sItemID.ToInt() > 0)
	{
		ITEMSERVER->UpdateIntegrity(&psPacket->sItemData.sItem, -5);
	}

	if (ITEMSERVER->CheckItemForm(&psPacket->sItemData.sItem) == FALSE)
	{
		PacketLogCheat sPacket;
		sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
		sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
		sPacket.SParam = psPacket->sItemData.sItem.iChk1;
		sPacket.EParam = psPacket->sItemData.sItem.iChk2;
		sPacket.LxParam = (int)psPacket->sItemData.sItem.szItemName;
		LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
		return FALSE;
	}


	//Item Respec
	{
		int iRespecFound = 0;
		EItemID eSpecStone = ITEMID_None;

		for (int i = 0; i < MAX_SHELTOMS; i++)
		{
			if ((psPacket->dwCode[i]))
			{
				if ((psPacket->dwCode[i] & 0xFFFF0000) == ITEMTYPE_Respec)
				{
					iRespecFound++;
				}
			}
		}

		//Valento and Kelvezu amulet cannot be mixed, but allow for respec
		if ((psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_ValentoRing) ||
			//(psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_DraxosBoots) ||
			(psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet))
		{
			if (iRespecFound == 0)
			{
				//Mixing Failed
				psPacket->iResult = 0;
				SENDPACKET(USERDATATOUSER(pcUserData), psPacket);
				return FALSE;
			}
		}

		//Respec stone found?
		if (iRespecFound > 0)
		{
			return OnRespecingItem(pcUser, psPacket, iRespecFound);
		}
	}

	Item sItem;


	//Here, we check item type

	EMixType iTypeMix = GetTypeMixByItemCode(psPacket->sItemData.sItem.sItemID.ToInt());

	int iSheltomCount = 0;
	for (int i = 0; i < MAX_SHELTOMS; i++)
	{
		//Is a sheltom? Should be.
		if (psPacket->dwCode && ((psPacket->dwCode[i] & 0xFFFF0000) == ITEMTYPE_Sheltom))
		{
			iSheltomCount++;
		}
	}

	//std::cout << "iTypeMix = " << (int)iTypeMix << std::endl;

	int sheltomCountList[MAX_SHELTOM_TYPES] = { 0 };
	int mixFound = 0;

	for (int i = 0; i < MAX_SHELTOMS; i++)
	{
		if (psPacket->dwCode[i] > 0)
		{
			int sheltomIndex = GetSheltomIndexFromCode(psPacket->dwCode[i]);
			sheltomCountList[sheltomIndex]++;
		}
	}

	for (int j = 0; j < MAX_SHELTOMS; j++)
	{
	//	std::cout << "sheltomCountList[" << j << "] = " << sheltomCountList[j] << std::endl;
	}

	//std::cout << "mixServerData.iSheltomCount = " << iSheltomCount << std::endl;


	MixServerData* mixServerDataPointerArray = ((MixServerData*)0x006BB6B0);

	for (int mixIndex = 0; mixIndex < MAX_CRAFTITEM_INFO; mixIndex++)
	{
		MixServerData* mixServerData = mixServerDataPointerArray + mixIndex;

		if (mixServerData->iType == iTypeMix && mixServerData->iSheltomCount == iSheltomCount)
		{
			//std::cout << mixIndex << ": Checking mix type: " << (int)mixServerData->iType << " sheltom count: " << mixServerData->iSheltomCount << std::endl;

			BOOL bMatchingMixFound = TRUE;

			for (int iSheltomIndex = 0; iSheltomIndex < MAX_SHELTOM_TYPES; iSheltomIndex++)
			{
				//does not match.
				if (mixServerData->iaSheltomCount[iSheltomIndex] != sheltomCountList[iSheltomIndex])
				{
					//std::cout << "NON MATCH - mixServerData->iaSheltomCount[" << iSheltomIndex << "] = " << mixServerData->iaSheltomCount[iSheltomIndex] << std::endl;
					//std::cout << "VS - sheltomCountList[" << iSheltomIndex << "] = " << sheltomCountList[iSheltomIndex] << std::endl;

					bMatchingMixFound = FALSE; //Skip mix
					break;
				}
			}

			//Match found?
			if (bMatchingMixFound)
			{
				//std::cout << mixIndex << ": FOUND MATCHING MIX " << std::endl;

				//bNonMatchingFound is FALSE, so therefore
				//the sheltom requirements did MATCH prefectly for all sheltom types

				DWORD dwHeadOld = psPacket->sItemData.sItem.iChk1;
				DWORD dwCheckSumOld = psPacket->sItemData.sItem.iChk2;

				ITEMSERVER->DeleteItemInventory(pcUserData, &psPacket->sItemData.sItem);

				//std::cout << "psPacket->sItemData.sItem.iChk1 original:       " << psPacket->sItemData.sItem.iChk1 << std::endl;
				//std::cout << "psPacket->sItemData.sItem.iChk2 original:       " << psPacket->sItemData.sItem.iChk2 << std::endl;
				//std::cout << "psPacket->sItemData.sItem.dwTime original:      " << psPacket->sItemData.sItem.dwTime << std::endl;
				//std::cout << "psPacket->sItemData.sItem.iBackupKey original:  " << psPacket->sItemData.sItem.iBackupKey << std::endl;
				//std::cout << "psPacket->sItemData.sItem.iBackupChk original:  " << psPacket->sItemData.sItem.iBackupChk << std::endl;
				//std::cout << "psPacket->sItemData.sItem.eCraftType original:  " << psPacket->sItemData.sItem.eCraftType << std::endl;
				//std::cout << "psPacket->sItemData.sItem.eMixTypeName original: " << (int) psPacket->sItemData.sItem.eMixTypeName << std::endl;
				//std::cout << "psPacket->sItemData.sItem.sMixUniqueID original: " << psPacket->sItemData.sItem.iBackupChk << std::endl;


				SetMixStatus(&psPacket->sItemData.sItem, mixServerData);
				psPacket->sItemData.sItem.eCraftType = EItemCraftType::ITEMCRAFTTYPE_Mixing;
				psPacket->sItemData.sItem.eMixTypeName = mixServerData->eMixTypeName;
				psPacket->sItemData.sItem.sMixUniqueID1 = mixServerData->iMixUniqueId;
				psPacket->sItemData.sItem.sMixColorId = (USHORT)GetMixColorFromMixType(mixServerData->eMixTypeName);

				////std::cout << "psPacket->sItemData.sItem.iBackupKey before: " << psPacket->sItemData.sItem.iBackupKey << std::endl;
				//std::cout << "psPacket->sItemData.sItem.iBackupChk before:" << psPacket->sItemData.sItem.iBackupChk << std::endl;

				ITEMSERVER->ReformItem(&psPacket->sItemData.sItem);
				ITEMSERVER->SaveItemDataToDatabase(&psPacket->sItemData.sItem);
				ITEMSERVER->AddItemInventory(pcUserData, &psPacket->sItemData.sItem);

				//std::cout << "psPacket->sItemData.sItem.iBackupKey after: " << psPacket->sItemData.sItem.iBackupKey << std::endl;
				//std::cout << "psPacket->sItemData.sItem.iBackupChk after: " << psPacket->sItemData.sItem.iBackupChk << std::endl;

				//std::cout << mixIndex << ": SUBTRACTING GOLD: " << psPacket->iGold << std::endl;

				CHARACTERSERVER->GiveGOLD(USERDATATOUSER(pcUserData), -psPacket->iGold, EWhereAction::WHEREID_MixItem);

				psPacket->iResult = TRUE;

				SendMixDescription(pcUserData, FormatString("%s\n-------\n%s", GetMixTypeStringFromMixType(psPacket->sItemData.sItem.eMixTypeName), mixServerData->szDescription));

				USERSERVER->SendUserMiscCommandToOtherNearbyPlayers ( pcUser, EUnitDataMiscCommand::COMMANDID_ShowSuccessAgingOrMixSkillAnimation );


				//Save New Item Data into ItemCreateLog table
				ITEMSERVER->SaveItemDataToDatabase(&psPacket->sItemData.sItem);
				{
					LogItem sLog;
					sLog.ItemCount = 1;
					sLog.Item[1].dwCode     = psPacket->sItemData.sItem.sItemID.ToItemID();
					sLog.Item[1].dwHead     = psPacket->sItemData.sItem.iChk1;
					sLog.Item[1].dwCheckSum = psPacket->sItemData.sItem.iChk2;
					sLog.Item[0].dwCode     = psPacket->sItemData.sItem.sItemID.ToItemID();
					sLog.Item[0].dwHead     = dwHeadOld;
					sLog.Item[0].dwCheckSum = dwCheckSumOld;
					sLog.Flag = ITEMLOGID_Mixing;
					sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
					STRINGCOPY(sLog.UserID, pcUser->pcUserData->szAccountName);
					STRINGCOPY(sLog.CharName, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));

					//Then log this into ItemLog table
					LOGSERVER->OnLogItem(LogServer::LOGTYPEID_LogItem, 1, &sLog);
				}

				SENDPACKET(USERDATATOUSER(pcUserData), psPacket);
				return TRUE;
			}
		}
	}

	//mix not found
	psPacket->iResult = FALSE;
	SENDPACKET(USERDATATOUSER(pcUserData), psPacket);
	return TRUE;



	//OLD MIXING CODE

	/*__asm
	{
		PUSH psPacket;
		PUSH pcUserData;
		MOV EAX, 0x00569550;
		CALL EAX;
		ADD ESP, 0x08;
		MOV bResult, EAX;
	}*/


	return bResult;
}

EMixColor CMixHandler::GetMixColorFromMixType(EMixTypeName pMixTypeName)
{
	switch (pMixTypeName)
	{
	case EMixTypeName::MIXTYPENAME_LegacyDoNotUse: //due to DB error.. all mix had id of 1
	case EMixTypeName::MIXTYPENAME_Lucidy1:
	case EMixTypeName::MIXTYPENAME_Lucidy2:
	case EMixTypeName::MIXTYPENAME_Lucidy3:
	case EMixTypeName::MIXTYPENAME_Lucidy4:
	case EMixTypeName::MIXTYPENAME_Lucidy5:
	case EMixTypeName::MIXTYPENAME_Lucidy6:
	case EMixTypeName::MIXTYPENAME_Lucidy7:
	case EMixTypeName::MIXTYPENAME_Lucidy8:
	case EMixTypeName::MIXTYPENAME_Lucidy9:
	case EMixTypeName::MIXTYPENAME_Sereneo1:
	case EMixTypeName::MIXTYPENAME_Sereneo2:
	case EMixTypeName::MIXTYPENAME_Sereneo3:
	case EMixTypeName::MIXTYPENAME_Sereneo4:
	case EMixTypeName::MIXTYPENAME_Sereneo5:
	case EMixTypeName::MIXTYPENAME_Sereneo6:
	case EMixTypeName::MIXTYPENAME_Sereneo7:
	case EMixTypeName::MIXTYPENAME_Sereneo8:
	case EMixTypeName::MIXTYPENAME_Sereneo9:
	case EMixTypeName::MIXTYPENAME_Fadeo1:
	case EMixTypeName::MIXTYPENAME_Fadeo2:
	case EMixTypeName::MIXTYPENAME_Fadeo3:
	case EMixTypeName::MIXTYPENAME_Fadeo4:
	case EMixTypeName::MIXTYPENAME_Fadeo5:
	case EMixTypeName::MIXTYPENAME_Fadeo6:
	case EMixTypeName::MIXTYPENAME_Fadeo7:
	case EMixTypeName::MIXTYPENAME_Fadeo8:
	case EMixTypeName::MIXTYPENAME_Fadeo9:
	case EMixTypeName::MIXTYPENAME_Sparky1:
	case EMixTypeName::MIXTYPENAME_Sparky2:
	case EMixTypeName::MIXTYPENAME_Sparky3:
	case EMixTypeName::MIXTYPENAME_Sparky4:
	case EMixTypeName::MIXTYPENAME_Sparky5:
	case EMixTypeName::MIXTYPENAME_Sparky6:
	case EMixTypeName::MIXTYPENAME_Sparky7:
	case EMixTypeName::MIXTYPENAME_Sparky8:
	case EMixTypeName::MIXTYPENAME_Sparky9:
	case EMixTypeName::MIXTYPENAME_Raident1:
	case EMixTypeName::MIXTYPENAME_Raident2:
	case EMixTypeName::MIXTYPENAME_Raident3:
	case EMixTypeName::MIXTYPENAME_Raident4:
	case EMixTypeName::MIXTYPENAME_Raident5:
	case EMixTypeName::MIXTYPENAME_Raident6:
	case EMixTypeName::MIXTYPENAME_Raident7:
	case EMixTypeName::MIXTYPENAME_Raident8:
	case EMixTypeName::MIXTYPENAME_Raident9:
		return EMixColor::MIXCOLOR_Classic;

	case EMixTypeName::MIXTYPENAME_Transparo1:
	case EMixTypeName::MIXTYPENAME_Transparo2:
	case EMixTypeName::MIXTYPENAME_Transparo3:
	case EMixTypeName::MIXTYPENAME_Transparo4:
	case EMixTypeName::MIXTYPENAME_Transparo5:
	case EMixTypeName::MIXTYPENAME_Transparo6:
	case EMixTypeName::MIXTYPENAME_Transparo7:
	case EMixTypeName::MIXTYPENAME_Transparo8:
	case EMixTypeName::MIXTYPENAME_Transparo9:
		return EMixColor::MIXCOLOR_Transparo;

	case EMixTypeName::MIXTYPENAME_Murky1:
	case EMixTypeName::MIXTYPENAME_Murky2:
	case EMixTypeName::MIXTYPENAME_Murky3:
	case EMixTypeName::MIXTYPENAME_Murky4:
	case EMixTypeName::MIXTYPENAME_Murky5:
	case EMixTypeName::MIXTYPENAME_Murky6:
	case EMixTypeName::MIXTYPENAME_Murky7:
	case EMixTypeName::MIXTYPENAME_Murky8:
	case EMixTypeName::MIXTYPENAME_Murky9:
		return EMixColor::MIXCOLOR_Murky;

	case EMixTypeName::MIXTYPENAME_Devine1:
	case EMixTypeName::MIXTYPENAME_Devine2:
	case EMixTypeName::MIXTYPENAME_Devine3:
	case EMixTypeName::MIXTYPENAME_Devine4:
	case EMixTypeName::MIXTYPENAME_Devine5:
	case EMixTypeName::MIXTYPENAME_Devine6:
	case EMixTypeName::MIXTYPENAME_Devine7:
	case EMixTypeName::MIXTYPENAME_Devine8:
	case EMixTypeName::MIXTYPENAME_Devine9:
		return EMixColor::MIXCOLOR_Devine;

	case EMixTypeName::MIXTYPENAME_Celesto1:
	case EMixTypeName::MIXTYPENAME_Celesto2:
	case EMixTypeName::MIXTYPENAME_Celesto3:
	case EMixTypeName::MIXTYPENAME_Celesto4:
	case EMixTypeName::MIXTYPENAME_Celesto5:
	case EMixTypeName::MIXTYPENAME_Celesto6:
	case EMixTypeName::MIXTYPENAME_Celesto7:
	case EMixTypeName::MIXTYPENAME_Celesto8:
	case EMixTypeName::MIXTYPENAME_Celesto9:
		return EMixColor::MIXCOLOR_Celesto;

	case EMixTypeName::MIXTYPENAME_Mirage1:
	case EMixTypeName::MIXTYPENAME_Mirage2:
	case EMixTypeName::MIXTYPENAME_Mirage3:
	case EMixTypeName::MIXTYPENAME_Mirage4:
	case EMixTypeName::MIXTYPENAME_Mirage5:
	case EMixTypeName::MIXTYPENAME_Mirage6:
	case EMixTypeName::MIXTYPENAME_Mirage7:
	case EMixTypeName::MIXTYPENAME_Mirage8:
	case EMixTypeName::MIXTYPENAME_Mirage9:
		return EMixColor::MIXCOLOR_Mirage;

	case EMixTypeName::MIXTYPENAME_Inferna1:
	case EMixTypeName::MIXTYPENAME_Inferna2:
	case EMixTypeName::MIXTYPENAME_Inferna3:
	case EMixTypeName::MIXTYPENAME_Inferna4:
	case EMixTypeName::MIXTYPENAME_Inferna5:
	case EMixTypeName::MIXTYPENAME_Inferna6:
	case EMixTypeName::MIXTYPENAME_Inferna7:
	case EMixTypeName::MIXTYPENAME_Inferna8:
	case EMixTypeName::MIXTYPENAME_Inferna9:
		return EMixColor::MIXCOLOR_Inferna;

	case EMixTypeName::MIXTYPENAME_Enigma1:
	case EMixTypeName::MIXTYPENAME_Enigma2:
	case EMixTypeName::MIXTYPENAME_Enigma3:
	case EMixTypeName::MIXTYPENAME_Enigma4:
	case EMixTypeName::MIXTYPENAME_Enigma5:
	case EMixTypeName::MIXTYPENAME_Enigma6:
	case EMixTypeName::MIXTYPENAME_Enigma7:
	case EMixTypeName::MIXTYPENAME_Enigma8:
	case EMixTypeName::MIXTYPENAME_Enigma9:
		return EMixColor::MIXCOLOR_Enigma;

	case EMixTypeName::MIXTYPENAME_Bellum1:
	case EMixTypeName::MIXTYPENAME_Bellum2:
	case EMixTypeName::MIXTYPENAME_Bellum3:
	case EMixTypeName::MIXTYPENAME_Bellum4:
	case EMixTypeName::MIXTYPENAME_Bellum5:
	case EMixTypeName::MIXTYPENAME_Bellum6:
	case EMixTypeName::MIXTYPENAME_Bellum7:
	case EMixTypeName::MIXTYPENAME_Bellum8:
	case EMixTypeName::MIXTYPENAME_Bellum9:
		return EMixColor::MIXCOLOR_Bellum;

	case EMixTypeName::MIXTYPENAME_Oredo1:
	case EMixTypeName::MIXTYPENAME_Oredo2:
	case EMixTypeName::MIXTYPENAME_Oredo3:
	case EMixTypeName::MIXTYPENAME_Oredo4:
	case EMixTypeName::MIXTYPENAME_Oredo5:
	case EMixTypeName::MIXTYPENAME_Oredo6:
	case EMixTypeName::MIXTYPENAME_Oredo7:
	case EMixTypeName::MIXTYPENAME_Oredo8:
	case EMixTypeName::MIXTYPENAME_Oredo9:

		return EMixColor::MIXCOLOR_Oredo;

	default:
		break;
	}


	return EMixColor::MIXCOLOR_Classic;
}

const char* CMixHandler::GetMixTypeStringFromMixType(EMixTypeName pMixTypeName)
{
	//std::cout << "GetMixTypeStringFromMixType " << (int)pMixTypeName << std::endl;

	switch (pMixTypeName)
	{
	case EMixTypeName::MIXTYPENAME_LegacyDoNotUse: //due to DB error.. all mix had id of 1
		return "Mixed";

	case EMixTypeName::MIXTYPENAME_Lucidy1:
	case EMixTypeName::MIXTYPENAME_Lucidy2:
	case EMixTypeName::MIXTYPENAME_Lucidy3:
	case EMixTypeName::MIXTYPENAME_Lucidy4:
	case EMixTypeName::MIXTYPENAME_Lucidy5:
	case EMixTypeName::MIXTYPENAME_Lucidy6:
	case EMixTypeName::MIXTYPENAME_Lucidy7:
	case EMixTypeName::MIXTYPENAME_Lucidy8:
	case EMixTypeName::MIXTYPENAME_Lucidy9:
		return "Lucidy Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Sereneo1:
	case EMixTypeName::MIXTYPENAME_Sereneo2:
	case EMixTypeName::MIXTYPENAME_Sereneo3:
	case EMixTypeName::MIXTYPENAME_Sereneo4:
	case EMixTypeName::MIXTYPENAME_Sereneo5:
	case EMixTypeName::MIXTYPENAME_Sereneo6:
	case EMixTypeName::MIXTYPENAME_Sereneo7:
	case EMixTypeName::MIXTYPENAME_Sereneo8:
	case EMixTypeName::MIXTYPENAME_Sereneo9:
		return"Sereneo Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Fadeo1:
	case EMixTypeName::MIXTYPENAME_Fadeo2:
	case EMixTypeName::MIXTYPENAME_Fadeo3:
	case EMixTypeName::MIXTYPENAME_Fadeo4:
	case EMixTypeName::MIXTYPENAME_Fadeo5:
	case EMixTypeName::MIXTYPENAME_Fadeo6:
	case EMixTypeName::MIXTYPENAME_Fadeo7:
	case EMixTypeName::MIXTYPENAME_Fadeo8:
	case EMixTypeName::MIXTYPENAME_Fadeo9:
		return "Fadeo Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Sparky1:
	case EMixTypeName::MIXTYPENAME_Sparky2:
	case EMixTypeName::MIXTYPENAME_Sparky3:
	case EMixTypeName::MIXTYPENAME_Sparky4:
	case EMixTypeName::MIXTYPENAME_Sparky5:
	case EMixTypeName::MIXTYPENAME_Sparky6:
	case EMixTypeName::MIXTYPENAME_Sparky7:
	case EMixTypeName::MIXTYPENAME_Sparky8:
	case EMixTypeName::MIXTYPENAME_Sparky9:
		return "Sparky Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Raident1:
	case EMixTypeName::MIXTYPENAME_Raident2:
	case EMixTypeName::MIXTYPENAME_Raident3:
	case EMixTypeName::MIXTYPENAME_Raident4:
	case EMixTypeName::MIXTYPENAME_Raident5:
	case EMixTypeName::MIXTYPENAME_Raident6:
	case EMixTypeName::MIXTYPENAME_Raident7:
	case EMixTypeName::MIXTYPENAME_Raident8:
	case EMixTypeName::MIXTYPENAME_Raident9:
		return "Raident Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Transparo1:
	case EMixTypeName::MIXTYPENAME_Transparo2:
	case EMixTypeName::MIXTYPENAME_Transparo3:
	case EMixTypeName::MIXTYPENAME_Transparo4:
	case EMixTypeName::MIXTYPENAME_Transparo5:
	case EMixTypeName::MIXTYPENAME_Transparo6:
	case EMixTypeName::MIXTYPENAME_Transparo7:
	case EMixTypeName::MIXTYPENAME_Transparo8:
	case EMixTypeName::MIXTYPENAME_Transparo9:
		return "Transparo Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Murky1:
	case EMixTypeName::MIXTYPENAME_Murky2:
	case EMixTypeName::MIXTYPENAME_Murky3:
	case EMixTypeName::MIXTYPENAME_Murky4:
	case EMixTypeName::MIXTYPENAME_Murky5:
	case EMixTypeName::MIXTYPENAME_Murky6:
	case EMixTypeName::MIXTYPENAME_Murky7:
	case EMixTypeName::MIXTYPENAME_Murky8:
	case EMixTypeName::MIXTYPENAME_Murky9:
		return "Murky Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Devine1:
	case EMixTypeName::MIXTYPENAME_Devine2:
	case EMixTypeName::MIXTYPENAME_Devine3:
	case EMixTypeName::MIXTYPENAME_Devine4:
	case EMixTypeName::MIXTYPENAME_Devine5:
	case EMixTypeName::MIXTYPENAME_Devine6:
	case EMixTypeName::MIXTYPENAME_Devine7:
	case EMixTypeName::MIXTYPENAME_Devine8:
	case EMixTypeName::MIXTYPENAME_Devine9:
		return "Devine Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Celesto1:
	case EMixTypeName::MIXTYPENAME_Celesto2:
	case EMixTypeName::MIXTYPENAME_Celesto3:
	case EMixTypeName::MIXTYPENAME_Celesto4:
	case EMixTypeName::MIXTYPENAME_Celesto5:
	case EMixTypeName::MIXTYPENAME_Celesto6:
	case EMixTypeName::MIXTYPENAME_Celesto7:
	case EMixTypeName::MIXTYPENAME_Celesto8:
	case EMixTypeName::MIXTYPENAME_Celesto9:
		return "Celesto Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Mirage1:
	case EMixTypeName::MIXTYPENAME_Mirage2:
	case EMixTypeName::MIXTYPENAME_Mirage3:
	case EMixTypeName::MIXTYPENAME_Mirage4:
	case EMixTypeName::MIXTYPENAME_Mirage5:
	case EMixTypeName::MIXTYPENAME_Mirage6:
	case EMixTypeName::MIXTYPENAME_Mirage7:
	case EMixTypeName::MIXTYPENAME_Mirage8:
	case EMixTypeName::MIXTYPENAME_Mirage9:
		return "Mirage Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Inferna1:
	case EMixTypeName::MIXTYPENAME_Inferna2:
	case EMixTypeName::MIXTYPENAME_Inferna3:
	case EMixTypeName::MIXTYPENAME_Inferna4:
	case EMixTypeName::MIXTYPENAME_Inferna5:
	case EMixTypeName::MIXTYPENAME_Inferna6:
	case EMixTypeName::MIXTYPENAME_Inferna7:
	case EMixTypeName::MIXTYPENAME_Inferna8:
	case EMixTypeName::MIXTYPENAME_Inferna9:
		return "Inferna Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Enigma1:
	case EMixTypeName::MIXTYPENAME_Enigma2:
	case EMixTypeName::MIXTYPENAME_Enigma3:
	case EMixTypeName::MIXTYPENAME_Enigma4:
	case EMixTypeName::MIXTYPENAME_Enigma5:
	case EMixTypeName::MIXTYPENAME_Enigma6:
	case EMixTypeName::MIXTYPENAME_Enigma7:
	case EMixTypeName::MIXTYPENAME_Enigma8:
	case EMixTypeName::MIXTYPENAME_Enigma9:
		return "Enigma Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Bellum1:
	case EMixTypeName::MIXTYPENAME_Bellum2:
	case EMixTypeName::MIXTYPENAME_Bellum3:
	case EMixTypeName::MIXTYPENAME_Bellum4:
	case EMixTypeName::MIXTYPENAME_Bellum5:
	case EMixTypeName::MIXTYPENAME_Bellum6:
	case EMixTypeName::MIXTYPENAME_Bellum7:
	case EMixTypeName::MIXTYPENAME_Bellum8:
	case EMixTypeName::MIXTYPENAME_Bellum9:
		return "Bellum Mix";
		break;

	case EMixTypeName::MIXTYPENAME_Oredo1:
	case EMixTypeName::MIXTYPENAME_Oredo2:
	case EMixTypeName::MIXTYPENAME_Oredo3:
	case EMixTypeName::MIXTYPENAME_Oredo4:
	case EMixTypeName::MIXTYPENAME_Oredo5:
	case EMixTypeName::MIXTYPENAME_Oredo6:
	case EMixTypeName::MIXTYPENAME_Oredo7:
	case EMixTypeName::MIXTYPENAME_Oredo8:
	case EMixTypeName::MIXTYPENAME_Oredo9:
		return "Oredo Mix";
		break;

	default:
		break;
	}


	return "Mixed";
}


int CMixHandler::GetSheltomIndexFromCode(int iSheltomItemCode)
{
	switch (iSheltomItemCode)
	{
		case ITEMID_Lucidy:		return 0;
		case ITEMID_Sereno:		return 1;
		case ITEMID_Fadeo:		return 2;
		case ITEMID_Sparky:		return 3;
		case ITEMID_Raident:	return 4;
		case ITEMID_Transparo:	return 5;
		case ITEMID_Murky:		return 6;
		case ITEMID_Devine:		return 7;
		case ITEMID_Celesto:	return 8;
		case ITEMID_Mirage:		return 9;
		case ITEMID_Inferna:	return 10;
		case ITEMID_Enigma:		return 11;
		case ITEMID_Bellum:		return 12;
		case ITEMID_Oredo:		return 13;
		//case ITEMID_:			return 14;
		default:				return 0;
	}
}

void CMixHandler::TestJeraRespec(int iItemCode)
{
	//Only for game-server
	if (LOGIN_SERVER)
		return;

	const int iRepeatCount = 10000;

	if (CreateDirectory("TestJera", NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		//Check and create dir
	}
	else
	{
		return;
	}

	int iSpec = 0;
	DefinitionItem* psItem = ITEMSERVER->FindItemDefByCode(iItemCode);

	std::map<int, int> vTestResult;

	if (psItem)
	{
		ItemData* psItemData = ITEMSERVER->FindItemPointerTable(iItemCode);
		if (psItemData)
		{
			std::string logFileName = FormatString("TestJera\\ItemCode_%d_%s_lvl_%d.txt", iItemCode, psItemData->szBaseName, psItem->sItem.iLevel);
			CLogger* logger = new CLogger(logFileName);

			Item sItem;

			for (int i = 0; i < iRepeatCount; i++)
			{
				iSpec = 0;

				//if iSpec is 100, it basically guarantine a spec.
				//Note - iSpec 0 does give SPEC but at lower chance.
				//So 50% chance for spec 100 actually yields more than 50% spec
				//i.e. 40% chance gives 65% chance for a spec.
				int iSpec = 0;

				if (psItem->sItem.iLevel < 40)
				{
					//test reuslt = 100% chance to get spec
					iSpec = 100;
				}
				else if (psItem->sItem.iLevel < 80)
				{
					//test result = 85% chance to get spec
					if (Dice::RandomI(0, 99) < 75)
					{
						iSpec = 100;
					}
				}
				else if (psItem->sItem.iLevel < 100)
				{
					//test result = 70% chance to get spec
					if (Dice::RandomI(0, 99) < 50)
					{
						iSpec = 100;
					}
				}
				else
				{
					//test result = 52% chance to get spec
					if (Dice::RandomI(0, 99) < 20)
					{
						iSpec = 100;
					}
				}

				ITEMSERVER->CreateItem(&sItem, psItem, EItemSource::Respec, iSpec);

				if (vTestResult.find(sItem.eSpecialization) == vTestResult.end())
					vTestResult.insert({ sItem.eSpecialization, 0 });

				vTestResult[sItem.eSpecialization]++;
			}

			logger->WriteLine("");
			logger->WriteLine("");
			logger->WriteLine(Util::CurrentDateTime());
			logger->WriteLine("==========================================================================================");
			logger->WriteLine("==========================================================================================");
			logger->WriteLine("10000 x Jera respec result:");
			logger->WriteLine("Item: %s", psItemData->szBaseName);
			logger->WriteLine("");


			std::string specName = "";
			for (auto testResult : vTestResult)
			{
				specName = "Non-spec";

				if (testResult.first != 0)
				{
					EClassFlag e = static_cast<enum EClassFlag>(testResult.first);
					specName = GetCharacterClassStringByFlag(e);
				}

				padTo(specName, 20);


				float fChance = static_cast<float>(testResult.second) / static_cast<float>(iRepeatCount);
				logger->WriteLine("%s = %d (%.3f%%)", specName.c_str(), testResult.second, fChance * 100.f);
			}

			logger->WriteLine("-----------------------------------------------------------------");
			logger->Flush();
			logger->Close();

		}
	}
}

BOOL CMixHandler::OnRespecingItem(User* pcUser, struct PacketMixServer* psPacket, int numOfRespecsFound)
{
	UserData* pcUserData = pcUser->pcUserData;


	//Delete Sheltoms
	for (int i = 0; i < MAX_SHELTOMS; i++)
	{
		if ((psPacket->dwCode[i]))
		{
			//Delete Sheltoms
			if (!ITEMSERVER->DeleteItemInventory(pcUserData, psPacket->dwCode[i], psPacket->dwHead[i], psPacket->dwCheckSum[i]))
			{
				PacketLogCheat sPacket;

				char szItemName[32] = { 0 };
				ITEMSERVER->FindItemName(psPacket->dwCode[i], szItemName);

				sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
				sPacket.LParam = psPacket->dwCode[i];
				sPacket.SParam = psPacket->dwHead[i];
				sPacket.EParam = psPacket->dwCheckSum[i];
				sPacket.LxParam = (int)szItemName;
				LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
				return FALSE;
			}
		}
	}

	//Get Item Timer Create
	DWORD dwItemTimerCreateOld = psPacket->sItemData.sItem.tTime;

	//Mixing Log
	LogItem sLog;
	sLog.ItemCount = 1;
	sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
	sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
	sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
	sLog.Flag = ITEMLOGID_LostRecon;
	sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
	STRINGCOPY(sLog.UserID, pcUser->pcUserData->szAccountName);
	STRINGCOPY(sLog.CharName, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));

	CHARACTERSERVER->GiveGOLD(pcUser, -psPacket->iGold, EWhereAction::WHEREID_RespecItem);

	//Delete Item from Inventory
	if (!ITEMSERVER->DeleteItemInventory(pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2))
	{
		PacketLogCheat sPacket;
		sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
		sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
		sPacket.SParam = psPacket->sItemData.sItem.iChk1;
		sPacket.EParam = psPacket->sItemData.sItem.iChk2;
		sPacket.LxParam = (int)psPacket->sItemData.sItem.szItemName;
		LOGSERVER->OnLogCheat(pcUser->pcUserData, &sPacket);
		return FALSE;
	}

	//Get Item Data and Spec
	int iSpec = 0;
	DefinitionItem* psItem = ITEMSERVER->FindItemDefByCode(psPacket->sItemData.sItem.sItemID.ToItemID());

	if (psItem)
	{
		ItemData* psItemData = ITEMSERVER->FindItemPointerTable(psPacket->sItemData.sItem.sItemID.ToItemID());
		if (psItemData)
		{

			ITEMSERVER->SaveItemDataToDatabase(&psPacket->sItemData.sItem);

			// Get Old Code
			DWORD dwHeadOld = 0, dwCheckSumOld = 0, dwBackupKeyOld = 0, dwBackupChkOld = 0;
			int iSpecOld = psPacket->sItemData.sItem.eSpecialization;
			dwHeadOld = psPacket->sItemData.sItem.iChk1;
			dwCheckSumOld = psPacket->sItemData.sItem.iChk2;
			dwBackupKeyOld = psPacket->sItemData.sItem.iBackupKey;
			dwBackupChkOld = psPacket->sItemData.sItem.iBackupChk;

			//if iSpec is 100, it basically guarantine a spec.
			//Note - iSpec 0 does give SPEC but at lower chance.
			//So 50% chance for spec 100 actually yields more than 50% spec
			//i.e. 40% chance gives 65% chance for a spec.
			int iSpec = 0;

			if (psItem->sItem.iLevel < 40)
			{
				//test result = 100% chance to get spec
				iSpec = 100;
			}
			else if (psItem->sItem.iLevel < 80)
			{
				//test result = 85% chance to get spec
				if (Dice::RandomI(0, 99) < 75)
				{
					iSpec = 100;
				}
			}
			else if (psItem->sItem.iLevel < 100)
			{
				//test result = 70% chance to get spec
				if (Dice::RandomI(0, 99) < 50)
				{
					iSpec = 100;
				}
			}
			else
			{
				//test result = 52% chance to get spec
				if (Dice::RandomI(0, 99) < 20)
				{
					iSpec = 100;
				}
			}

			Item sItem;
			ITEMSERVER->CreateItem(&sItem, psItem, EItemSource::Respec, iSpec, 0, 0, psPacket->sItemData.sItem.eRarity);

			//fully reconstruct item (legacy behaviour)
			if (numOfRespecsFound >= 2)
			{
				psPacket->iDocIndex = sItem.eSpecialization == 0 ? 2000 : 2001; //just for message id

				//Replace the existing item
				ZeroMemory(&psPacket->sItemData, sizeof(ItemData));
				CopyMemory(&psPacket->sItemData, psItemData, sizeof(ItemData));
				CopyMemory(&psPacket->sItemData.sItem, &sItem, sizeof(Item));

				//Preserve the original checksum values
				// so we can track down any duplicated items.
				psPacket->sItemData.sItem.iBackupKey = dwBackupKeyOld;
				psPacket->sItemData.sItem.iBackupChk = dwBackupChkOld;

				//Note the backup header and checksums will stay the same
				ITEMSERVER->ReformItem(&psPacket->sItemData.sItem);

				psPacket->sItemData.sItem.tTime = dwItemTimerCreateOld;
				ITEMSERVER->ValidateItemTime(&psPacket->sItemData.sItem);
			}

			//Only reconstruct spec data only
			//AND char stats requirements because stats depends on character class
			else if (numOfRespecsFound == 1)
			{
				//no change to item.
				psPacket->iDocIndex = sItem.eSpecialization == 0 ? 2002 : 2003;  //just for message id

				//Copy across new spec data from new item to the existing item
				// while perserving the primary stats
				psPacket->sItemData.sItem.eSpecialization = sItem.eSpecialization;

				//also copy stats over
				// becauase different spec can have different stats
				psPacket->sItemData.sItem.iStrength = sItem.iStrength;
				psPacket->sItemData.sItem.iTalent   = sItem.iTalent;
				psPacket->sItemData.sItem.iAgility  = sItem.iAgility;
				psPacket->sItemData.sItem.iSpirit   = sItem.iSpirit;
				psPacket->sItemData.sItem.iHealth   = sItem.iHealth;

				//also the price (otherwise false bans can happen)
				// because spec and non-spec have different prices
				psPacket->sItemData.sItem.iSalePrice = sItem.iSalePrice;

				CopyMemory(&psPacket->sItemData.sItem.sSpecData, &sItem.sSpecData, sizeof(SpecItemData));

				//Note the backup header and checksums will stay the same
				ITEMSERVER->ReformItem(&psPacket->sItemData.sItem);
			}

			//Save New Item Data
			ITEMSERVER->SaveItemDataToDatabase(&psPacket->sItemData.sItem);
			{
				LogItem sLog;
				sLog.ItemCount = 1;
				sLog.Item[1].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
				sLog.Item[1].dwHead = psPacket->sItemData.sItem.iChk1;
				sLog.Item[1].dwCheckSum = psPacket->sItemData.sItem.iChk2;
				sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
				sLog.Item[0].dwHead = dwHeadOld;
				sLog.Item[0].dwCheckSum = dwCheckSumOld;
				sLog.Flag = ITEMLOGID_ItemRespec;
				sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
				STRINGCOPY(sLog.UserID, pcUser->pcUserData->szAccountName);
				STRINGCOPY(sLog.CharName, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData));
				LOGSERVER->OnLogItem(LogServer::LOGTYPEID_LogItem, 1, &sLog);
			}

			//Create New Item
			if (ITEMSERVER->AddItemInventory(pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2) < 0)
			{
				psPacket->iResult = 0;
				SENDPACKET(pcUser, psPacket);
				return FALSE;
			}

			psPacket->iResult = TRUE;
			SENDPACKET(USERDATATOUSER(pcUserData), psPacket);

			return TRUE;
		}
	}

	//Mixing Failed
	psPacket->iResult = 0;
	SENDPACKET(USERDATATOUSER(pcUserData), psPacket);
	return FALSE;
}

BOOL CMixHandler::SetMixStatus( Item * psItem, MixServerData* ps)
{
	if (ps == nullptr)
		return FALSE;

	for ( int i = 0; i < 8; i++ )
	{
		int iType = ps->iaTypeAttributte[i];

		switch ( iType )
		{
			case MIXATTRTYPE_Organic:			AddAttributeBonusToItem(&psItem->saElementalDef[0], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Fire:				AddAttributeBonusToItem(&psItem->saElementalDef[2], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Ice:				AddAttributeBonusToItem(&psItem->saElementalDef[3], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Lightning:			AddAttributeBonusToItem(&psItem->saElementalDef[4], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Poison:			AddAttributeBonusToItem(&psItem->saElementalDef[5], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Critical:			AddAttributeBonusToItem(&psItem->iCritical,			static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_AttackRating:		AddAttributeBonusToItem(&psItem->iAttackRating,		static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_DamageMin:			AddAttributeBonusToItem(&psItem->sDamage.sMin,		static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_DamageMax:			AddAttributeBonusToItem(&psItem->sDamage.sMax,		static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_AttackSpeed:		AddAttributeBonusToItem(&psItem->iAttackSpeed,		static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Absorption:		AddAttributeBonusToItem(&psItem->fAbsorbRating,		ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Defense:			AddAttributeBonusToItem(&psItem->iDefenseRating,    static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_BlockRating:		AddAttributeBonusToItem(&psItem->fBlockRating,      ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_WalkSpeed:			AddAttributeBonusToItem(&psItem->fMovementSpeed,    ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_HP:				AddAttributeBonusToItem(&psItem->fAddHP,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_MP:				AddAttributeBonusToItem(&psItem->fAddMP,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_SP:				AddAttributeBonusToItem(&psItem->fAddSP,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_HPRegen:			AddAttributeBonusToItem(&psItem->fHPRegen,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_MPRegen:			AddAttributeBonusToItem(&psItem->fMPRegen,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_SPRegen:			AddAttributeBonusToItem(&psItem->fSPRegen,			ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_PotionStorage:		AddAttributeBonusToItem(&psItem->iPotionStorage,    static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;

			default:
				break;
		}

		psItem->eMixEffect |= (EMixEffect)iType;
	}

	return TRUE;
}

BOOL CMixHandler::SetClearMixStatus( Item * psItem, int iUniqueID )
{
	//DEBUG ( FormatString ( "CMixHandler::SetClearMixStatus: mix id = %d", iUniqueID ) );

	MixServerData* ps = GetMixServerDataByUniqueID(iUniqueID);

	if (ps == nullptr)
		return FALSE;

	for ( int i = 0; i < 8; i++ )
	{
		int iType = ps->iaTypeAttributte[i];

		switch ( iType )
		{
			case MIXATTRTYPE_Organic:			RemoveAttributeBonusFromItem(&psItem->saElementalDef[0], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Fire:				RemoveAttributeBonusFromItem(&psItem->saElementalDef[2], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Ice:				RemoveAttributeBonusFromItem(&psItem->saElementalDef[3], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Lightning:			RemoveAttributeBonusFromItem(&psItem->saElementalDef[4], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Poison:			RemoveAttributeBonusFromItem(&psItem->saElementalDef[5], static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Critical:			RemoveAttributeBonusFromItem(&psItem->iCritical,		 static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_AttackRating:		RemoveAttributeBonusFromItem(&psItem->iAttackRating,	 static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_DamageMin:			RemoveAttributeBonusFromItem(&psItem->sDamage.sMin,		 static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_DamageMax:			RemoveAttributeBonusFromItem(&psItem->sDamage.sMax,		 static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_AttackSpeed:		RemoveAttributeBonusFromItem(&psItem->iAttackSpeed,		 static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Absorption:		RemoveAttributeBonusFromItem(&psItem->fAbsorbRating,	                                        ps->faAttributte[i], ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_Defense:			RemoveAttributeBonusFromItem(&psItem->iDefenseRating,    static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_BlockRating:		RemoveAttributeBonusFromItem(&psItem->fBlockRating,      ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_WalkSpeed:			RemoveAttributeBonusFromItem(&psItem->fMovementSpeed,    ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_HP:				RemoveAttributeBonusFromItem(&psItem->fAddHP,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_MP:				RemoveAttributeBonusFromItem(&psItem->fAddMP,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_SP:				RemoveAttributeBonusFromItem(&psItem->fAddSP,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_HPRegen:			RemoveAttributeBonusFromItem(&psItem->fHPRegen,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_MPRegen:			RemoveAttributeBonusFromItem(&psItem->fMPRegen,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_SPRegen:			RemoveAttributeBonusFromItem(&psItem->fSPRegen,			 ps->faAttributte[i],                   ps->iaAttributtePercent[i]); break;
			case MIXATTRTYPE_PotionStorage:		RemoveAttributeBonusFromItem(&psItem->iPotionStorage,    static_cast<int>(ps->faAttributte[i]), ps->iaAttributtePercent[i]); break;

			default: break;

		}

		psItem->eMixEffect |= (EMixEffect)iType;
	}

	return TRUE;
}


MixServerData* CMixHandler::GetMixServerDataByUniqueID(int iUniqueID)
{
	if ( iUniqueID == 0 )
	{
		LOGERROR ( "GetMixServerDataByUniqueID - Mix unique id is 0" );
		return nullptr;
	}

	for (int mixIndex = 0; mixIndex < MAX_CRAFTITEM_INFO; mixIndex++)
	{
		MixServerData* ps = ((MixServerData*)0x006BB6B0) + mixIndex;

		if (ps->iMixUniqueId == iUniqueID)
		{
			//std::cout << "FOUND MIX! " << ps->iMixUniqueId << " index = " << mixIndex << std::endl;

			return ps;
		}
	}

	//std::cout << "Cannot find mix by unique id:" << iUniqueID << std::endl;

	return nullptr;

}



void CMixHandler::SetItemMixByUniqueID ( Item * psItem, int iMixUniqueId )
{
	if ( iMixUniqueId == 0 )
	{
		LOGERROR ( "SetItemMixByUniqueID - Mix unique id is 0" );
	}

	//std::cout << "CMixHandler::SetItemMixByMixName" << std::endl;

	EMixType iType = GetTypeMixByItemCode ( psItem->sItemID.ToInt () );

	if ( iType != EMixType::MIXTYPE_Unknown )
	{
		for ( int i = 0; i < MAX_CRAFTITEM_INFO; i++ )
		{
			MixServerData * ps = ( (MixServerData *)0x006BB6B0 ) + i;

			if ( ps->iType == iType && ps->iMixUniqueId == iMixUniqueId )
			{
				SetMixStatus ( psItem, ps );
				ITEMSERVER->ReformItem ( psItem );
				break;
			}
		}
	}
}


/**
Removes attribute bonus from item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to remove. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::RemoveAttributeBonusFromItem(short* value, int attributeBonus, int attributeBonusType)
{
	//1 = flat value
	if ( attributeBonusType == 1 )
		*value -= attributeBonus;

	//0 = percentage based value
	else
		*value = static_cast<short>( ( *value * 100.0f ) / ( attributeBonus + 100.0f ) );
}

/**
Removes attribute bonus from item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to remove. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::RemoveAttributeBonusFromItem(int * value, int attributeBonus, int attributeBonusType)
{
	//1 = flat value
	if ( attributeBonusType == 1 )
		*value -= attributeBonus;

	//0 = percentage based value
	else
		*value = static_cast<int>( ( *value * 100.0f ) / ( attributeBonus + 100.0f ) );
}

/**
Removes attribute bonus from item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to remove. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::RemoveAttributeBonusFromItem(float *value, float attributeBonus, int attributeBonusType)
{
	//DEBUG ( FormatString ( "CMixHandler::RemoveAttributeBonusFromItem ( %.2f, %d, %d )", *value, attributeBonus, attributeBonusType ) );

	//1 = flat value
	if (attributeBonusType == 1)
		*value -= attributeBonus;

	//0 = percentage based value
	else
		*value = (*value * 100.0f) / (attributeBonus + 100.0f);

	//DEBUG ( FormatString ( "CMixHandler::RemoveAttributeBonusFromItem result = %.2f", *value ) );
}


/**
Adds attribute bonus to item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to add. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::AddAttributeBonusToItem(short* value, int attributeBonus, int attributeBonusType)
{
	//1 = flat value
	if ( attributeBonusType == 1 )
		*value += attributeBonus;

	//0 = percentage based value
	else
		*value += static_cast<short>( ( *value * attributeBonus ) / 100.0f );
}

/**
Adds attribute bonus to item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to add. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::AddAttributeBonusToItem(int* value, int attributeBonus, int attributeBonusType)
{
	//1 = flat value
	if ( attributeBonusType == 1 )
		*value += attributeBonus;

	//0 = percentage based value
	else
		*value += static_cast<int>( ( ( *value * attributeBonus ) / 100.0f ) );
}

/**
Adds attribute bonus to item
 - pointer to the value to update
 - attributeBonusType is 1 or 0 (1 is flat value, 0 is percent based value)
 - attributeBonus is the bonus value to add. For percentage based, it is 1 - 100 where 100 is 100%
*/
void CMixHandler::AddAttributeBonusToItem(float* value, float attributeBonus, int attributeBonusType)
{
	//1 = flat value
	if (attributeBonusType == 1)
		*value += attributeBonus;

	//0 = percentage based value
	else
		*value += ((*value * attributeBonus) / 100.0f);
}

EMixType CMixHandler::GetTypeMixByItemCode( int iItemID )
{
	EMixType iType = EMixType::MIXTYPE_Unknown;

	if ( (iItemID & 0xFF000000) == ITEMBASE_Weapon )
	{
		iType = EMixType::MIXTYPE_Weapons;

		//if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Phantom || (iItemID & 0xFFFF0000) == ITEMTYPE_Dagger )
			//iType = EMixType::MIXTYPE_DaggerPhantom;
	}

	if ( (iItemID & 0xFF000000) == ITEMBASE_Defense )
	{
		iType = EMixType::MIXTYPE_ArmourAndRobe;

		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Shield )
			iType = EMixType::MIXTYPE_Sheilds;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Gauntlets )
			iType = EMixType::MIXTYPE_Gauntlets;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Boots )
			iType = EMixType::MIXTYPE_Boots;
	}

	if ( (iItemID & 0xFF000000) == ITEMBASE_Sheltom)
	{
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Orb )
			iType = EMixType::MIXTYPE_Orbs;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Bracelets )
			iType = EMixType::MIXTYPE_Bracelet;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Amulet )
			iType = EMixType::MIXTYPE_Amulets;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Ring )
			iType = EMixType::MIXTYPE_Rings;
		else if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Ring2 )
			iType = EMixType::MIXTYPE_Rings;
	}


	return iType;
}
