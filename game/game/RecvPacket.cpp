#include "stdafx.h"
#include "RecvPacket.h"
#include "CDamageInfo.h"
#include "Logger.h"

int				iWarehouseError;

extern void OnLoadServerSkillChainDataContainer ( PacketGenericContainer * psContainer );
extern void OnLoadServerSkillDataContainer ( PacketSkillDataContainer * psSkillDataContainer );
extern void OnLoadServerSkillInfoContainer ( PacketSkillInfoContainer * psSkillInfoContainer );
extern void OnLoadServerCommonSkillInfoContainer ( PacketCommonSkillInfoContainer * psContainer );

RecvPacket::RecvPacket()
{
}

RecvPacket::~RecvPacket()
{
}

PacketCreateCharacter sPacketStaticCreateCharacter;


int64_t s_ServTimeOffset = 0;
int counter = 0;

//This occurs AFTER the packet is processed in EXE
//where AnalyzePacket is BEFORE
BOOL RecvPacket::OnPostProcessedPacket ( SocketData * pcSocketData, Packet * psPacket )
{
	switch ( psPacket->iHeader )
	{
	case PKTHDR_AgingItem:
		AGINGWINDOWHANDLER->HandlePostPacket ( (PacketAgingItem *)psPacket );
		break;

	case PKTHDR_SendWarehouse:
		WAREHOUSEHANDLER->HandlePostPacket( (PacketWarehouse*)psPacket );
		break;

	}

	return TRUE;
}

BOOL RecvPacket::IsPacketIgnored( Packet * psPacket )
{
	switch ( psPacket->iHeader )
	{
#ifndef DEV_MODE
		case PKTHDR_HACKTRAP:
		{
			return TRUE;
		}
#endif

		case PKTHDR_AgingItem:
		{
			if ( ((PacketAgingItem*)psPacket)->sItem.sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_QuestWeapon)
			{
				return TRUE;
			}

			break;
		}

		case PKTHDR_UpgradeAgingItem:
		{
			if ( ((PacketItem*)psPacket)->sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_QuestWeapon)
			{
				return TRUE;
			}

			break;
		}


		case PKTHDR_SET_BLACKLIST:
		case PKTHDR_SET_BLACKLIST2:
		case PKTHDR_SET_BLACKLIST3:
		{
			switch ( ( (PacketTransCommand *)psPacket )->WParam )
			{
				case 8580: return TRUE; //this spams the server whenever monsters attack players
			}
			break;
		}
		case PKTHDR_Client_Error:
		{
#ifdef DEV_MODE
			if (GM_MODE && MAP_ID == MAPID_QuestArena )
				return TRUE;
#endif
			break;
		}
	}

	return FALSE;
}


BOOL RecvPacket::AnalyzePacket( SocketData * pcSocketData, Packet * psPacket )
{
#if defined(LOG_INCOMING_OUTGOING_PACKETS)
	//JLM - Debug what packets we receive
	char hexString[20];
	itoa(psPacket->iHeader, hexString, 16);

	if (SOCKETL && SOCKETG)
	{
		CLogger::GetLogger ()->Log ( FormatString ( "%d,IN,%s,%d,%d,%d", TICKCOUNT, hexString, psPacket->iLength, SOCKETL->iReceiveWheel, SOCKETG->iReceiveWheel ) );
	}
	else
	{
		CLogger::GetLogger ()->Log ( FormatString ( "%d,IN,%s,%d,0,0", TICKCOUNT, hexString, psPacket->iLength ) );
	}

	if (counter > 10)
	{
		CLogger::GetLogger ()->Flush();
		counter = 0;
	}
	counter++;
#endif

	switch ( psPacket->iHeader )
	{
		case PKTHDR_TitleBoxMessage:
		{
			PacketChatBoxMessage * l_Packet = reinterpret_cast<PacketChatBoxMessage*>(psPacket);
			TITLEBOX( l_Packet->szChatBoxMessage );
			CHATBOX->AddMessage( FormatString( "> %s", l_Packet->szChatBoxMessage), EChatColor::CHATCOLOR_Normal);

			psPacket->iHeader = 0; //disable processing in assembly
		}
		break;
		case PKTHDR_NpcQuestStatus:
		{
			QUESTGAMEHANDLER->HandlePacket( reinterpret_cast<PacketTransCommand *>( psPacket ) );
		}
		break;

		case PKTHDR_ClearUnitDataFromMap:
		{
			PacketCommand * l_Packet = reinterpret_cast<PacketCommand*>(psPacket);

			auto pcUnitData = UNITDATABYID ( l_Packet->p1 );

			if ( pcUnitData )
			{
				pcUnitData->dwLastActiveTime = TICKCOUNT + 16000;
				pcUnitData->bVisible = FALSE;
				pcUnitData->Free();
			}
		}
		break;

		case PKTHDR_QuestMonsterName:
		{
			QUESTGAMEHANDLER->HandlePacket( (PacketQuestMonsterName *)psPacket );
			break;
		}

		case PKTHDR_Skill_Healing:
		{
            DAMAGEINFOHANDLER->HandlePacket((PacketHealing *)psPacket);
			if (UNITDATA->psModelAnimation->iType != EAnimationType::ANIMATIONTYPE_Die)
            {
                reinterpret_cast<int(*)()>(0x45BC90)();			/// CheckCharForm();
                if (((PacketHealing*)psPacket)->iHealAmount)
				{
                    CHARACTERGAME->SetCurrentHP(CHARACTERGAME->GetCurrentHP() + ((PacketHealing*)psPacket)->iHealAmount);

                    PacketQuickHealthUpdate l_HealthUpdate;
                    l_HealthUpdate.iHeader	= PKTHDR_QuickHealthUpdate;
                    l_HealthUpdate.iLength	= sizeof(PacketQuickHealthUpdate);
                    l_HealthUpdate.dwID		= UNITDATA->iID;
                    l_HealthUpdate.sHP		= CHARACTERGAME->GetCurrentHP();

                    SOCKETG->SendPacket(&l_HealthUpdate, true);
                }
                reinterpret_cast<void(*)()>(0x45BC70)();		/// ReformCharForm();
            }

            psPacket->iHeader = 0; //disable processing in assembly
			break;
		}

		case PKTHDR_Skill_GrandHealing:
        {
            DAMAGEINFOHANDLER->HandlePacket((PacketGrandHealing *)psPacket);
			//CHATBOX->AddDebugMessage ( "PKTHDR_Skill_GrandHealing received" );
			break;
		}

		case PKTHDR_NewItemAuto:
		{
			ITEMHANDLER->AutoItemToInventory(&reinterpret_cast<PacketItem*>(psPacket)->sItem);
			break;
		}

		case PKTHDR_NewItemNPC:
		{
			PacketItem * l_Packet = reinterpret_cast<PacketItem*>(psPacket);

			if ( l_Packet->sItem.iChk1 != 0 &&
				 l_Packet->sItem.iChk2 != 0 )
			{
				WRITEWORD ( 0x3653E08, 0 ); //clears BuyItemServerFlag to allow buying from npc again

				ItemData sItemData;
				if ( ITEMHANDLER->LoadItemImage ( &l_Packet->sItem, &sItemData ) )
				{
					ITEMHANDLER->BackItemToInventory ( &sItemData, ITEMCHECKINVENTORY_None );
					ITEMHANDLER->ResetInventoryItemChecksum ();

					// Validate Inventory
					CALLT ( 0x0050E510, 0x036932FC );

					CHARACTERGAME->UpdateWeight ();
					SAVE;
				}
			}

			break;
		}

		case PKTHDR_RecoverItem:
		{
			PacketRecoverItem * l_Packet = reinterpret_cast<PacketRecoverItem*>(psPacket);

			if ( l_Packet && l_Packet->iStatus == 1 &&
				l_Packet->sItem.iChk1 != 0 &&
				l_Packet->sItem.iChk2 != 0 )
			{
				ItemData sItemData;
				if ( ITEMHANDLER->LoadItemImage ( &l_Packet->sItem, &sItemData ) )
				{
					ITEMHANDLER->BackItemToInventory ( &sItemData, ITEMCHECKINVENTORY_None );
					ITEMHANDLER->ResetInventoryItemChecksum ();

					// Validate Inventory
					CALLT ( 0x0050E510, 0x036932FC );

					CHARACTERGAME->UpdateWeight ();
					SAVE;
				}
			}

			break;
		}

        case PKTHDR_Connected:
        case PKTHDR_GameTimeSync:
        {
            PacketTransCommand * l_Packet = reinterpret_cast<PacketTransCommand *>(psPacket);

            int64_t l_MyTicks       = GetTickCount();
            int64_t l_ServerTicks   = l_Packet->LParam;

            int64_t l_Diff = l_ServerTicks - l_MyTicks;

            s_ServTimeOffset = l_Diff;

            *reinterpret_cast<DWORD *>(0xCF4794) = static_cast<DWORD>(l_ServerTicks / 800);

            break;
        }

		case PKTHDR_MiscUnitCommand:
			UNITGAME->HandlePacketCommand ( reinterpret_cast<PacketCommand *>( psPacket ) );
			break;

		//Party
		case PKTHDR_RequestParty:
			PARTYHANDLER->HandlePacket( (PacketRequestParty*)psPacket );
			break;
		case PKTHDR_UpdateParty:
			PARTYHANDLER->HandlePacket( (PacketUpdateParty*)psPacket );
			break;
		case PKTHDR_UpdatePartyData:
			PARTYHANDLER->HandlePacket( (PacketUpdatePartyData*)psPacket );
			break;
		case PKTHDR_RequestRaid:
			PARTYHANDLER->HandlePacket( (PacketRequestRaid*)psPacket );
			break;

		case PKTHDR_RollDiceDropItem:
			{
				HUDHANDLER->OpenDiceRollRanking();
				HUDHANDLER->UpdateRollDice();
				HUDHANDLER->ClearRollDiceRanking( true );

				ItemData sItemData;
				if ( ITEMHANDLER->LoadItemImage( &((PacketRollDiceDropItem*)psPacket)->sItem , &sItemData ) )
				{
					UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 64 - (sItemData.iWidth >> 1), 74 - (sItemData.iHeight >> 1), sItemData.iWidth / 22, sItemData.iHeight / 22 );
					pItemBox->SetID( 1 );
					pItemBox->SetMaxItems( 1 );
					pItemBox->DisableMouse();
					pItemBox->SetFreePlace ( FALSE );
					pItemBox->AddItem( &sItemData, 0, 0, 0, 0, FALSE );

					// Show Box to Save
					MESSAGEBOX->ClearExtraElements();
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_RollDice );
					MESSAGEBOX->SetTitle( "DICE ROLL" );
					MESSAGEBOX->SetDescription( "Do you want to try your luck?" );
					MESSAGEBOX->AddExtraText( ((PacketRollDiceDropItem*)psPacket)->sItem.szItemName, Rectangle2D( 25, 118, 76, 24 ), TRUE, UI::Color( 255, 255, 255, 255 ) );
					MESSAGEBOX->AddExtraElement( pItemBox );
					MESSAGEBOX->SetEvent( std::bind( &ItemHandler::OnAcceptRollDiceItem, ITEMHANDLER ) );
					MESSAGEBOX->SetAutoClose( MAX_ROLLDICEDROP_TIME );
					MESSAGEBOX->Show(FALSE, TRUE);
				}
			}
			break;

		case PKTHDR_UpdateSWPhase:
			BLESSCASTLEHANDLER->PHUpdate( (PacketSiegeWarPhase*)psPacket );
			break;

		case PKTHDR_RollDiceRanking:
			HUDHANDLER->HandlePacket( (PacketRollDiceRanking*)psPacket );
			break;

		case PKTHDR_Ping:
			GameCore::GetInstance()->HandlePacket( pcSocketData, (PacketPing*)psPacket );
			break;

		case PKTHDR_ServerInfo:
			UNITGAME->ReceiveServerInformation( (PacketServerInfo*)psPacket );
			break;

		case PKTHDR_IntegrityUpdateItem:
			CHARACTERGAME->HandlePacket( (PacketUpdateIntegrity*)psPacket );
			break;

		case PKTHDR_UpdatePetDataEx:
			CHARACTERGAME->HandlePacket( (PacketUpdatePetDataEx*)psPacket );
			break;

		case PKTHDR_DisableGM:
			GM_MODE = FALSE;
			SENDPACKETG( psPacket );
			break;

		case PKTHDR_AddExp:
			UNITGAME->AddEXP( ((PacketExperience*)psPacket)->iAddExp );
			break;


		case PKTHDR_DamageDebugInfo:
		{
			auto l_Packet = (PacketDebugDamageInfo *)psPacket;

			if ( l_Packet->dwObjectID == UNITDATA->iID &&
				l_Packet->sTypeAction <= EDamageTextType::Evade )
			{
				CALLT_WITH_ARG1( 0x004A7400, 0, (int)l_Packet->sTypeAction );
			}

			DAMAGEINFOHANDLER->AddTarget( l_Packet->dwAttackerID, l_Packet->dwObjectID, l_Packet->sTypeAction, l_Packet->iValue, l_Packet->sParameter );

#ifdef DEV_MODE
			if ( GM_MODE && MAP_ID == MAPID_QuestArena )
			{
				TESTMAPHANDLER->AddTargetDamageDealt( l_Packet->dwAttackerID, l_Packet->dwObjectID, l_Packet->sTypeAction, l_Packet->iValue );
			}
#endif

			break;
		}

		case PKTHDR_PremiumDataSync:
			ITEMTIMERHANDLER->HandlePacket ( (PacketItemPremiumData *)psPacket, TRUE );
			break;

		case PKTHDR_HalloweenEventStatus:
			HALLOWEENHANDLER->PHSetEventStatus ( (PacketHalloweenEventStatus *)psPacket );
		break;

		case PKTHDR_ForceOrbCancel:
			ITEMTIMERHANDLER->CancelForceOrb( (PacketCommand *)psPacket );
			break;

		case PKTHDR_ForceOrbData:
			return TRUE;
			//ChatGame::AddChatBoxText ( "> PKTHDR_ForceOrbData" );
			break;

		case PKTHDR_PremiumData:
			ITEMTIMERHANDLER->HandlePacket( (PacketItemPremiumData*)psPacket, FALSE );
			break;

		case PKTHDR_SetItemTimer:
			ITEMTIMERHANDLER->HandlePacket( (PacketNewItemTimer*)psPacket );
			break;

		case PKTHDR_TeleportEvent:
			if ( CHARACTERGAME->PHTeleport( (PacketTeleportEvent*)psPacket ) )
			{
				((PacketTeleportEvent*)psPacket)->iHeader = 0;
				((PacketTeleportEvent*)psPacket)->iLength = 0;
			}
			break;

		case PKTHDR_OpenEventGirl:
			EVENTGIRLHANDLER->Open( (PacketEventGirl*)psPacket );
			break;

		case PKTHDR_OpenAgingMaster:
			AGINGWINDOWHANDLER->Open((PacketAgingEvent*)psPacket);
			break;

		case PTKHDR_EventGirlReset:
			EVENTGIRLHANDLER->HandlePacket((PacketEventGirlResetResult*)psPacket);
			break;

		case PKTHDR_Disconnect:
			DISCONNECT_TYPE = pcSocketData == SOCKETL ? 3 : 2;
			DISCONNECT_TIME = TICKCOUNT;
			break;

		case PKTHDR_NPCItemBox:
			ITEMHANDLER->HandlePacket( (PacketNPCItemBox*)psPacket );
			break;

		case PKTHDR_GetItemData:
			if ( GM_MODE )
			{
				// Is Selected?
				if ( ITEMINDEX_INVENTORY >= 0 )
				{
					// Send Item
					PacketItemData sPacket;
					sPacket.iLength = sizeof( PacketItemData );
					sPacket.iHeader = PKTHDR_GetItemData;
					sPacket.iItemIndex = ITEMINDEX_INVENTORY;
					CopyMemory( &sPacket.sItem, &INVENTORYITEMS[ITEMINDEX_INVENTORY], sizeof( ItemData ) );
					SENDPACKETG( &sPacket );
				}
			}
			break;

		case PKTHDR_Reconnect:
			*(UINT*)(0x04B0B23C) = 0;

			ChatGame::AddChatBoxText( "> Reconnected to Game Server!" );

			UNITGAME->UpdateObjectID(UNITDATA->iID);
			break;

		case PKTHDR_SetItemData:
			if ( GM_MODE )
			{
				// Set Item
				PacketItemData * psPacketITM;
				psPacketITM = ((PacketItemData*)psPacket);
				CopyMemory( &INVENTORYITEMS[psPacketITM->iItemIndex], &psPacketITM->sItem, sizeof( ItemData ) );
				ITEMHANDLER->ValidateInventoryItems();
			}
			break;

		case PKTHDR_AttackRatingSend:
			UnitData * pcUnit;
			pcUnit = UNITDATABYID( ((PacketAttackRating*)psPacket)->iEnemyID );
			if ( pcUnit )
				pcUnit->sCharacterData.iAttackRating = ((PacketAttackRating*)psPacket)->iAttackRating;
			break;

		case PKTHDR_KillsInSod:
			*(UINT*)0x4B0BC54 = ((PacketSimple*)psPacket)->iUnk;
			break;

		case PKTHDR_MonsterKill:

			pcUnit = UNITDATABYID( ((MonsterKill*)psPacket)->iMonsterID );
			if ( pcUnit )
			{
				QUESTGAMEHANDLER->OnKillUnit( pcUnit );
				CQUESTGAME->OnUnitKilled( pcUnit, ((MonsterKill*)psPacket)->iKillerID );
			}

			break;

		case PKTHDR_SetExpOld_DontUse:
			psPacket->iHeader = 0; // really set to 0 to disable processing of this packet in ASM!

			break;

		case PKTHDR_SendStablePet:
			STABLEHANDLER->ProcessPacket((PacketStablePetTab*)psPacket);
			break;

			// Skills
		case PKTHDR_SkillCast:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillCancel*)psPacket );
			break;

		case PKTHDR_SkillCastData:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillCast*)psPacket );
			break;

		case PKTHDR_BuffSkillsParty:
			SKILLMANAGERHANDLER->HandlePacket( (PacketSkillPartyData*)psPacket );
			break;

		case PKTHDR_SkillBuffStatus:
			SKILLMANAGERHANDLER->HandlePacket ( (PacketSkillBuffStatus *)psPacket );
			break;

		case PKTHDR_DamageInfoContainer:
			DAMAGEINFOHANDLER->HandlePacket ( (PacketDebugDamageInfoContainer *)psPacket );
			break;

		case PKTHDR_UseSkillPartyBuffNew:
			SKILLMANAGERHANDLER->HandlePacket( (PacketUseSkillParty*)psPacket );
			break;

		case PKTHDR_UseSkillNew:
			SKILLMANAGERHANDLER->HandlePacket( (PacketUseSkillSingle*)psPacket );
			break;

		case PKTHDR_AttackData:
			CHARACTERGAME->PHDamage( (PacketAttackData*)psPacket );
			break;

			// Crystals
		case PKTHDR_CrystalUse:
			TIMERSKILLHANDLER->PacketHandlerCrystal( (PacketCrystalUse*)psPacket );
			break;

		case PKTHDR_CrystalUseDie:
			TIMERSKILLHANDLER->PacketHandlerCrystal( (PacketCrystalUseDie*)psPacket );
			break;

		case PKTHDR_OpenCoinShop:
			COINHANDLER->OpenCoinShopNPC();
			break;

		//xxstr mix guild
		case PKTHDR_NPCMixGuild:
			MIXGUILDHANDLER->GetWindow()->HandlePacket( (PacketMixGuild*)psPacket );
			break;


		case PKTHDR_NPCRankListAllPlayers:
			RANKLISTHANDLER->GetWindow()->HandlePacket( (PacketRankingListAllPlayer*)psPacket );
			break;

		case PKTHDR_NPCRankListSoloSod:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketSoloRankingList*)psPacket);
			break;

		case PKTHDR_NPCRankListByClass:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketRankingListByClass*)psPacket);
			break;

		case PKTHDR_NPCRankListClanSod:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketRankingListClanSodList*)psPacket);
			break;

		case PKTHDR_NPCRankListIndividualSod:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketRankingListIndividualSod*)psPacket);
			break;

		case PKTHDR_NPCRankListBlessedCastle:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketRankingListBlessedCastle*)psPacket);
			break;

		case PKTHDR_NPCRankListPVP:
			RANKLISTHANDLER->GetWindow()->HandlePacket((PacketRankingListPVP*)psPacket);
			break;

		case PKTHDR_CoinTraded:
			TRADEHANDLER->HandlePacket( (PacketCoinsTraded*)psPacket );
			break;

		case PKTHDR_CoinTradeSet:
			TRADEHANDLER->HandlePacket( (PacketTradeCoinSet*)psPacket );
			break;

		case PKTHDR_NPCLarryOpen:
			CQUESTGAME->GetWindow()->OpenLarry();
			break;

		case PKTHDR_NPCMarinaOpen:
			CQUESTGAME->GetWindow()->GetAbyssWindow()->Open();
			break;

		case PKTHDR_WrapCamera:
			CHATGAMEHANDLER->HandlePacket( (PacketWrapCamera*)psPacket );
			break;

		case PKTHDR_FuryArena:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaRequest*)psPacket );
			break;

		case PKTHDR_FuryArenaBossData:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaBossData*)psPacket );
			break;

		case PKTHDR_NPCEasterEgg:
			EASTEREVENTHANDLER->HandlePacket( (PacketNPCEasterEgg*)psPacket );
			break;

		case PKTHDR_NPCRudolphChristmas:
			CHRISTMASHANDLER->OpenRudolphWindow();
			break;

		case PKTHDR_QuestCancel:
			CQUESTGAME->HandlePacket( (PacketQuestHandleCancel*)psPacket );
			break;

		case PKTHDR_T5QuestArena:
			CQUESTGAME->HandlePacket( (PacketQuestArenaTier5*)psPacket );
			CRYSTALTOWERHANDLER->HandlePacket( (PacketQuestArenaTier5*)psPacket );
			break;

		case PKTHDR_DynamicQuestOpen:
			CQUESTGAME->HandlePacket( (PacketDynamicQuestOpen*)psPacket );
			break;

		case PKTHDR_FireMeteorite:
			PacketFireMeteorite * psPFM;
			psPFM = (PacketFireMeteorite*)psPacket;
			for ( int i = 0; i < psPFM->iCount; i++ )
				EffectGame::FireMeteorite( (Point3D*)&psPFM->sBegin, (Point3D*)&psPFM->sEnd, psPFM->iDelay, psPFM->bSmallMeteor );
			break;

		case PKTHDR_DamageQuick:
			DAMAGEHANDLER->HandlePacket( (PacketDamageQuick*)psPacket );
			break;

		case PKTHDR_QuestNPCOpen:
			CQUESTGAME->HandlePacket( (PacketQuestNPCOpen*)psPacket );
			break;

		case PKTHDR_NPCIhinRewardRequest:
			BELLATRAREWARD->Open();
			break;

		case PKTHDR_CharacterData:
			CHARACTERGAME->HandlePacket( (PacketUnitInfo*)psPacket );
			break;

		case PKTHDR_CharacterDataEx:
			CHARACTERGAME->HandlePacket( (PacketCharacterDataEx*)psPacket );
			break;

		case PKTHDR_PvPKill:
			BLESSCASTLEHANDLER->HandlePacket( (PacketPvPKillType*)psPacket );
			break;

		case PKTHDR_BlessCastleIndividualScore:
			BLESSCASTLEHANDLER->HandlePacket( (PacketBlessCastleTopIndividual*)psPacket );
			break;

		case PKTHDR_BlessCastleUserScoreUpdate:
			BLESSCASTLEHANDLER->HandlePacket( (PacketBlessCastleUserScoreUpdate *)psPacket );
			break;

		case PKTHDR_BlessCastleTopClanSync:
			BLESSCASTLEHANDLER->HandlePacket( (PacketBlessCastleTopClanSync *)psPacket );
			break;

		case PKTHDR_CoinShopAmount:
			COINHANDLER->HandlePacket( (PacketCoinAmount*)psPacket );
			break;
		case PKTHDR_QuestNamesResponse:
			QUESTBOOKHANDLER->HandlePacket((PacketQuestNamesResponse*)psPacket);
			break;
		case PKTHDR_NewQuestNamesPostLvlUp:
			QUESTBOOKHANDLER->HandlePacket((PacketQuestNamesResponse*)psPacket);
			break;
			//Chat
		case PKTHDR_QuestDataResponse:
			QUESTBOOKHANDLER->HandlePacket((PacketQuestDataResponse*)psPacket);
			break;
		case PKTHDR_RemoveFinishedQuest:
			CQUESTGAME->RemoveFinishQuest( ( (PacketCommand *)psPacket )->p1 );
			break;
		case PKTHDR_NeverSinkOpenUI:
			NEVERSINKHANDLER->HandlePacket((PacketNeverSinkOpenUI*)psPacket);
			break;
		case PKTHDR_ChatMessage:
			{
				PacketChatBoxMessage pChatboxMessage;
				ZeroMemory( &pChatboxMessage, sizeof( PacketChatBoxMessage ) );
				CopyMemory( &pChatboxMessage, psPacket, psPacket->iLength );

				if (pChatboxMessage.iChatColor == CHATCOLOR_Command)
				{
					if (std::string(pChatboxMessage.szChatBoxMessage) == "Online_Reward_Won")
					{
						ChatGame::AddChatBoxText(">OnlineReward: You won this time!", CHATCOLOR_Notice);

						int stageID = CHARACTERGAME->GetStageID();
						if (stageID == MAPID_RicartenTown ||
							stageID == MAPID_PillaiTown)
						{
							HUDHANDLER->ShowMessageToUser("Online Rewards", "You won this time!  Visit Item Distributor to collect your prize.");
						}
						else
						{
							CTITLEBOX->SetText("Online Rewards: You won this time!", 10);
						}
					}
					else if (std::string(pChatboxMessage.szChatBoxMessage) == "Santa_Presents_Delivered")
					{
						ChatGame::AddChatBoxText(">Santa Claus: Your presents have arrived!", CHATCOLOR_Notice);

						int stageID = CHARACTERGAME->GetStageID();
						if (stageID == MAPID_RicartenTown ||
							stageID == MAPID_PillaiTown)
						{
							HUDHANDLER->ShowMessageToUser("Santa Claus Landed!", "Your presents have arrived!");
						}
						else
						{
							CTITLEBOX->SetText("Santa Claus Landed! Your presents have arrived!", 10);
						}
					}
					else if (std::string(pChatboxMessage.szChatBoxMessage) == "SOD_Entry_Is_Open")
					{
						CTITLEBOX->SetText("Bellatra SoD is now open and will start in 2 minutes!", 10);
					}
					else if (std::string(pChatboxMessage.szChatBoxMessage) == "SOD_Started")
					{
						CTITLEBOX->SetText("Bellatra SoD started! May the best team win!", 10);
					}
				}
				else
				{
					HUDHANDLER->GetChatBox()->HandlePacket(&pChatboxMessage);
				}
			}
			break;

		case PKTHDR_ChatItemLink:
			HUDHANDLER->GetChatBox()->HandlePacket( (PacketItemLinkChat*)psPacket );
			break;

		case PKTHDR_BossTimeUpdate:
			HUDHANDLER->GetMiniMapHandler()->HandlePacket( (PacketBossTimeUpdate*)psPacket );
			break;

        case PKTHDR_QuickHealthUpdate:
			if (reinterpret_cast<PacketQuickHealthUpdate*>(psPacket)->dwID != UNITDATA->iID)
			{
				auto l_HealthPacket = reinterpret_cast<PacketQuickHealthUpdate*>(psPacket);
				auto l_Target		= UNITDATABYID(l_HealthPacket->dwID);

				if (l_HealthPacket->sHP > 0 && l_Target)
					l_Target->sCharacterData.sHP.sCur = l_HealthPacket->sHP;
			}
            return true;

			// Warehouse
		case PKTHDR_RetryWarehouse:
			CALL( 0x0062A2CB );
			break;

		case PKTHDR_PacketFun:
			UNITGAME->Fun();
			break;

		case PKTHDR_SaveData:

			//UNITGAME->Log ( "PKTHDR_SaveData" );
			//UNITGAME->Log ( "dwForceOrbTimeOut 1 %d = %d = %d", TICKCOUNT, UNITDATA->dwForceOrbTimeOut, ( ( UNITDATA->dwForceOrbTimeOut - TICKCOUNT ) / 1000 ) );

			UNITGAME->HandlePacket( (PacketSaveData*)psPacket );
			SETTINGSHANDLER->GetModel()->ProcessGeneralSettings( TRUE, TRUE );

			//UNITGAME->Log ( "dwForceOrbTimeOut 2 %d = %d = %d", TICKCOUNT, UNITDATA->dwForceOrbTimeOut, ( ( UNITDATA->dwForceOrbTimeOut - TICKCOUNT ) / 1000 ) );
			//UNITGAME->Log ( "PKTHDR_SaveData end" );
			break;

		case PKTHDR_WarehouseSaveSuccess:
			SAVE;
			CALLT( 0x00513D90, 0x036932FC );
			(*(DWORD*)0x0209EA3C) = FALSE;
			(*(DWORD*)0x0209EA40) = FALSE;
			(*(DWORD*)0x036EFC2C) = FALSE;
			ITEMHANDLER->ResetInventoryItemChecksum();
			CALL( 0x0045DE30 );
			CHARACTERGAME->UpdateWeight ();
			break;

		case PKTHDR_MixDescription:
			MIXHANDLER->HandlePacket( (PacketMixDescription*)psPacket );
			//lstrcpyA( (char*)0x99A1CC, (char*)((UINT)psPacket + 8) );
			break;

		case PKTHDR_MixItem:
			MIXHANDLER->HandlePacket( (PacketMixServer*)psPacket );
			break;



		case PKTHDR_Version:
			/*if (!SOCKETL)
			{
					DISCONNECT_TYPE = 3;
					DISCONNECT_TIME = TICKCOUNT;
					break;
			}*/
			SENDPACKETL( psPacket );
			break; // EDIT CHAOS

		case PKTHDR_SetGold:
			UNITDATA->sCharacterData.iGold = ((PacketSetCharacterGold*)psPacket)->dwGold;
			CHECK_CHARACTER_CHECKSUM;
			PLAYMINISOUND( 18 );
			break;

		case PKTHDR_CheckSum:
			ANTICHEATHANDLER->ReceivePacketHandler( (PacketChecksumFunctionList*)psPacket );
			break;

		case PKTHDR_CreateCharacter:
			CopyMemory( &sPacketStaticCreateCharacter, (PacketCreateCharacter*)psPacket, sizeof( PacketCreateCharacter ) );
			(*(DWORD*)0x039016E4) = (DWORD)&sPacketStaticCreateCharacter;
			break;

		case PKTHDR_PerfectItemData:
			PERFECTVIEWHANDLER->PHItemData( (PacketItemPerfectView*)psPacket );
			break;

		case PKTHDR_AccountLoginCode:
			static char szaLoginException[0x18];
			CopyMemory( szaLoginException, psPacket, 0x18 );
			(*(DWORD*)0x039016E8) = (DWORD)&szaLoginException;
			STRINGCOPYLEN( (char*)0x04B0DFA0, 256, ((PacketAccountLoginCode*)psPacket)->szMessage );
			WRITEDWORD( 0x04B0E0A0, (RESOLUTION_WIDTH >> 1) - (FONTHANDLER->GetWidthText( 1, (char*)0x04B0DFA0, STRLEN( (char*)0x04B0DFA0 ) ) >> 1) - 22 );

			break;

		case PKTHDR_ReconnectLogin:
			ChatGame::AddChatBoxText( "> Reconnected to Login Server!" );
			SAVE;
			break;


		case PKTHDR_EvasionSuccess:
		{
			PacketEvasion* packet = (PacketEvasion*)psPacket;

			//shouldn't happen
			CHATBOX->AddDebugMessage( "PKTHDR_EvasionSuccess" );

			//disable processing in ASM
			packet->iHeader = 0;

			break;
		}

		case PKTHDR_SendWarehouse:
			WAREHOUSEHANDLER->HandlePacket( (PacketWarehouse*)psPacket );
			break;

		case PKTHDR_WindowList:
			ANTICHEATHANDLER->HandlePacket( (PacketWindowList*)psPacket );
			break;

		case PKTHDR_UserOnline:
			MANAGEWINDOW->PHUserStatus( (PacketUserOnline*)psPacket );
			break;

		case PKTHDR_BellatraCrownSolo:
			UNITGAME->iBellatraSoloCrownRanking = ((PacketCommand*)psPacket)->p1;

			//CHATBOX->AddMessage( "Crown solo: " + UNIT->iBellatraSoloCrownRanking );
			break;

		// Bellatra
		case PKTHDR_BellatraCrown:
			(*(DWORD*)(0x03B18BC8)) = ((PacketBellatraCrown*)psPacket)->iBellatraCrown;

			//UNITGAME->Log("((PacketBellatraCrown*)psPacket)->iBellatraCrown = %d", ((PacketBellatraCrown*)psPacket)->iBellatraCrown);

			break;

			// Bellatra
		case PKTHDR_BellatraCrownFull:
		{
			auto packet = ((PacketBellatraCrownFull*)psPacket);

			auto bellatra = GAMECORE->GetBellatraGameHandler();
			bellatra->iBellatraCrownGoldClanId = packet->iBellatraCrownGoldClanId;
			bellatra->iBellatraCrownSilverClanId = packet->iBellatraCrownSilverClanId;
			bellatra->iBellatraCrownBronzeClanId = packet->iBellatraCrownBronzeClanId;

			/*UNITGAME->Log("PKTHDR_BellatraCrownFull");

			UNITGAME->Log("packet->iBellatraCrownGoldClanId = %d", packet->iBellatraCrownGoldClanId);
			UNITGAME->Log("packet->iBellatraCrownSilverClanId = %d", packet->iBellatraCrownSilverClanId);
			UNITGAME->Log("packet->iBellatraCrownBronzeClanId = %d", packet->iBellatraCrownBronzeClanId);*/

			break;
		}

		case PKTHDR_MapSpawnMarkers:
			MINIMAPHANDLER->HandlePacket ( ( PacketMapSpawnMarkers * )psPacket );
			break;

		case PKTHDR_MapIndicators:
			MINIMAPHANDLER->HandlePacket ( ( PacketMapIndicators * )psPacket );
			break;

		case PKTHDR_ServerSkillData:
			OnLoadServerSkillDataContainer ( (PacketSkillDataContainer *)psPacket );
			break;

		case PKTHDR_ServerSkillChainData:
			OnLoadServerSkillChainDataContainer ( (PacketGenericContainer *)psPacket );
			break;

		case PKTHDR_ServerCommonSkillInfo:
			OnLoadServerCommonSkillInfoContainer ( (PacketCommonSkillInfoContainer *)psPacket );
			break;

		case PKTHDR_ServerSkillInfo:
			OnLoadServerSkillInfoContainer ( (PacketSkillInfoContainer *)psPacket );
			break;

		case PKTHDR_QuestData:
			QUESTGAMEHANDLER->PHQuestData( /*(PacketQuestData*)psPacket*/ );
			break;

		case PKTHDR_QuestNPCFinish:
			CQUESTGAME->HandlePacket( (PacketQuestNPCFinish*)psPacket );
			break;

		case PKTHDR_QuestNPCItemList:
			CQUESTGAME->HandlePacket( (PacketQuestNPCItemList*)psPacket );
			break;

		case PKTHDR_RestartQuest:
			QUESTGAMEHANDLER->PHQuestRestart( (PacketQuestRestart*)psPacket );
			break;

		case PKTHDR_QuestStartData:
			CQUESTGAME->HandlePacket( (PacketQuestStartData*)psPacket );
			break;

		case PKTHDR_QuestHandleFinishedContainer:
			CQUESTGAME->HandlePacket(psPacket->iHeader, (PacketGenericContainer*)psPacket);
			break;

		case PKTHDR_QuestHandleFinished:
			CQUESTGAME->HandlePacket( (PacketHandleFinishedQuest*)psPacket );
			break;

		case PKTHDR_PVPDataUpdate:
			BLESSCASTLEHANDLER->PHUpdate( (PacketPvPData*)psPacket );
			break;

		case PKTHDR_CoinShopNPC:
			COINHANDLER->HandlePacket( (PacketCoinShopNPC*)psPacket );
			break;

		case PKTHDR_CoinShopTab:
			COINHANDLER->HandlePacket( (PacketCoinTab*)psPacket );
			break;

		case PKTHDR_CoinShopItemData:
			COINHANDLER->HandlePacket( (PacketCoinItemData*)psPacket );
			break;

		case PKTHDR_CoinShopBuy:
			COINHANDLER->HandlePacket( (PacketCoinBuy*)psPacket );
			break;

		case PKTHDR_CoinServiceOpen:
			COINHANDLER->HandlePacket( (PacketCoinOpenService*)psPacket );
			break;

		case PKTHDR_CoinServiceBuy:
			COINHANDLER->HandlePacket( (PacketCoinServiceBuy*)psPacket );
			break;

		case PKTHDR_CoinServiceAgingRecoveryOpen:
			COINHANDLER->HandlePacket( (PacketOpenAgingRecoveryService*)psPacket );
			break;

		case PKTHDR_TitleList:
			CHATBOX->HandlePacket ( (PacketPlayerTitleList *)psPacket );
			break;

		case PKTHDR_Effect_User:
			UNITGAME->HandlePacket ( (PacketUserEffect *)psPacket );
			break;

		case PKTHDR_GoldPickup:
			DAMAGEINFOHANDLER->HandlePacket( (PacketGoldPickup*)psPacket );
			break;

		case PKTHDR_NumfOfInitialBellatraPlayers:
			UNITGAME->iBellatraRoomInitialPlayerCount = ((PacketNumberOfInitialBellatraPlayers*)psPacket)->iCount;

			if (UNITGAME->iBellatraRoomInitialPlayerCount == 1)
			{
				BELLATRAGAMEHANDLER->HandleSoloSodStart();
			}

			//DEBUGINFO("Count = %d", UNIT->iBellatraRoomInitialPlayerCount);
			break;

		case PKTHDR_FuryArenaItemData:
			GRANDFURYHANDLER->HandlePacket( (PacketFuryArenaItemData*)psPacket );
			break;

		case PKTHDR_CharacterHeadOldUse:
			ITEMTIMERHANDLER->HandlePacket( (PacketOldHeadUse*)psPacket );
			break;

		case PKTHDR_Shop_ItemList:
			ITEMHANDLER->HandlePacket( (PacketItemShopList *)psPacket );
			break;

		case PKTHDR_SaveAndClose:
			SAVE;

			GameCore::QuitAndSave( TRUE );
			break;

	}

	return TRUE;
}