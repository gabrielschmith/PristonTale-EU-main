#include "stdafx.h"
#include "SocketServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>


using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

SocketServer * SocketServer::pcInstance = NULL;

UINT SocketServer::uBytePacketFlow = 32 * 1024;	///< 32K per second byte limit

CRITICAL_SECTION                                        sSectionAccept;
CMutex                            * pcSocketCloseMutex = NULL;


SocketServer::SocketServer()
{
	sListenSocket = INVALID_SOCKET;
	iMaxConnections = 0;
	pcMutex = new CMutex();

#ifdef DEBUG
	uNextPacketLogTime = TIME + 10; //every 10 sec
#else
	uNextPacketLogTime = TIME + 600; //every 10 min
#endif

#define ADD_OPCODE_NAME(x) m_OpcodesNames[x] = std::string(#x);

	ADD_OPCODE_NAME(PKTHDR_NetIdentifier			   );
	ADD_OPCODE_NAME(PKTHDR_NetExp					   );
	ADD_OPCODE_NAME(PKTHDR_NetGetExp				   );
	ADD_OPCODE_NAME(PKTHDR_NetGameMaster			   );
	ADD_OPCODE_NAME(PKTHDR_NetGetGameMaster			   );
	ADD_OPCODE_NAME(PKTHDR_NetClan					   );
	ADD_OPCODE_NAME(PKTHDR_NetDisconnectUser		   );
	ADD_OPCODE_NAME(PKTHDR_NetForceOrb				   );
	ADD_OPCODE_NAME(PKTHDR_NetVersion				   );
	ADD_OPCODE_NAME(PKTHDR_NetCharacterSync			   );
	ADD_OPCODE_NAME(PKTHDR_NetGiveExp				   );
	ADD_OPCODE_NAME(PKTHDR_NetPingPong				   );
	ADD_OPCODE_NAME(PKTHDR_NetBCStatusReq			   );
	ADD_OPCODE_NAME(PKTHDR_NetBCStatusSnd			   );
	ADD_OPCODE_NAME(PKTHDR_NetBCNewOwnerClanID		   );
	ADD_OPCODE_NAME(PKTHDR_NetBotAdd				   );
	ADD_OPCODE_NAME(PKTHDR_NetPremiumEXP			   );
	ADD_OPCODE_NAME(PKTHDR_NetUnloadUser			   );
	ADD_OPCODE_NAME(PKTHDR_NetEXPEvent				   );
	ADD_OPCODE_NAME(PKTHDR_NetUsersOnline			   );
	ADD_OPCODE_NAME(PKTHDR_NetTradeCoin				   );
	ADD_OPCODE_NAME(PKTHDR_NetQuestT5Data			   );
	ADD_OPCODE_NAME(PKTHDR_NetQuestUpdateData          );
	ADD_OPCODE_NAME(PKTHDR_NetQuestUpdateDataPart      );
	ADD_OPCODE_NAME(PKTHDR_NetQuestCancelOrFinish	   );
	ADD_OPCODE_NAME(PKTHDR_NetLoadPVPData			   );
	ADD_OPCODE_NAME(PKTHDR_NetLoadQuestData			   );
	ADD_OPCODE_NAME(PKTHDR_NetSyncFinishedQuestData	   );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerWorldToken		   );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerInventory          );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerItemPut            );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerGoldDiff           );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerGold			   );
	ADD_OPCODE_NAME(PKTHDR_NetPlayerThrow			   );
	ADD_OPCODE_NAME(PKTHDR_PerfectItemData			   );
	ADD_OPCODE_NAME(PKTHDR_MixDescription			   );
	ADD_OPCODE_NAME(PKTHDR_OpenMixItem			       );
	ADD_OPCODE_NAME(PKTHDR_RecvWarehouse			   );
	ADD_OPCODE_NAME(PKTHDR_SendWarehouse			   );
	ADD_OPCODE_NAME(PKTHDR_TradeData				   );
	ADD_OPCODE_NAME(PKTHDR_TradeRequest				   );
	ADD_OPCODE_NAME(PKTHDR_TradeReady				   );
	ADD_OPCODE_NAME(PKTHDR_TradeItems				   );
	ADD_OPCODE_NAME(PKTHDR_ThrowItem				   );
	ADD_OPCODE_NAME(PKTHDR_CheckItemUseSlot			   );
	ADD_OPCODE_NAME(PKTHDR_CheckItemUseSlotEx		   );
	ADD_OPCODE_NAME(PKTHDR_KeySet					   );
	ADD_OPCODE_NAME(PKTHDR_Crash					   );
	ADD_OPCODE_NAME(PKTHDR_CrashData				   );
	ADD_OPCODE_NAME(PKTHDR_PacketFun				   );
	ADD_OPCODE_NAME(PKTHDR_SetExp					   );
	ADD_OPCODE_NAME(PKTHDR_AttackRatingSend			   );
	ADD_OPCODE_NAME(PKTHDR_DamageDebugInfo			   );
	ADD_OPCODE_NAME(PKTHDR_ServerInfo				   );
	ADD_OPCODE_NAME(PKTHDR_PlayDataEx				   );
	ADD_OPCODE_NAME(PKTHDR_PremiumData				   );
	ADD_OPCODE_NAME(PKTHDR_SetItemTimer				   );
	ADD_OPCODE_NAME(PKTHDR_DisableGM				   );
	ADD_OPCODE_NAME(PKTHDR_QuestData				   );
	ADD_OPCODE_NAME(PKTHDR_UpdateQuestData			   );
	ADD_OPCODE_NAME(PKTHDR_FinishQuest				   );
	ADD_OPCODE_NAME(PKTHDR_RestartQuest				   );
	ADD_OPCODE_NAME(PKTHDR_BellatraCrown			   );
	ADD_OPCODE_NAME(PKTHDR_PVPDataUpdate			   );
	ADD_OPCODE_NAME(PKTHDR_UpdateSWPhase			   );
	ADD_OPCODE_NAME(PKTHDR_CancelQuest				   );
	ADD_OPCODE_NAME(PKTHDR_CrystalUse				   );
	ADD_OPCODE_NAME(PKTHDR_CrystalUseDie			   );
	ADD_OPCODE_NAME(PKTHDR_UserOnline				   );
	ADD_OPCODE_NAME(PKTHDR_CancelForceOrb			   );
	ADD_OPCODE_NAME(PKTHDR_FireMeteorite			   );
	ADD_OPCODE_NAME(PKTHDR_NPCEasterEgg				   );
	ADD_OPCODE_NAME(PKTHDR_PvPKill					   );
	ADD_OPCODE_NAME(PKTHDR_NPCIhinRewardRequest		   );
	ADD_OPCODE_NAME(PKTHDR_CharacterHeadOldUse		   );
	ADD_OPCODE_NAME(PKTHDR_BlessCastleIndividualScore  );
	ADD_OPCODE_NAME(PKTHDR_BlessCastleUserScoreUpdate  );
	ADD_OPCODE_NAME(PKTHDR_NPCRudolphChristmas			   );
	ADD_OPCODE_NAME(PKTHDR_CancelItemTimer			   );
	ADD_OPCODE_NAME(PKTHDR_LogCheat					   );
	ADD_OPCODE_NAME(PKTHDR_DCAccount				   );
	ADD_OPCODE_NAME(PKTHDR_ReconnectedServer		   );
	ADD_OPCODE_NAME(PKTHDR_SendToLoginServer		   );
	ADD_OPCODE_NAME(PKTHDR_Microphone				   );
	ADD_OPCODE_NAME(PKTHDR_LogDataSimple			   );
	ADD_OPCODE_NAME(PKTHDR_CheckSum					   );
	ADD_OPCODE_NAME(PKTHDR_LogDebugger				   );
	ADD_OPCODE_NAME(PKTHDR_UndentifiedPacket		   );
	ADD_OPCODE_NAME(PKTHDR_AddExp					   );
	ADD_OPCODE_NAME(PKTHDR_LoseExp					   );
	ADD_OPCODE_NAME(PKTHDR_GetItemData				   );
	ADD_OPCODE_NAME(PKTHDR_SetItemData				   );
	ADD_OPCODE_NAME(PKTHDR_SetGold					   );
	ADD_OPCODE_NAME(PKTHDR_WindowList				   );
	ADD_OPCODE_NAME(PKTHDR_SettingsData				   );
	ADD_OPCODE_NAME(PKTHDR_RetryWarehouse			   );
	ADD_OPCODE_NAME(PKTHDR_WarehouseSaveSuccess		   );
	ADD_OPCODE_NAME(PKTHDR_DamageQuick				   );
	ADD_OPCODE_NAME(PKTHDR_QuestNPCOpen				   );
	ADD_OPCODE_NAME(PKTHDR_QuestNPCAccept			   );
	ADD_OPCODE_NAME(PKTHDR_QuestStartData			   );
	ADD_OPCODE_NAME(PKTHDR_QuestUpdateData			   );
	ADD_OPCODE_NAME(PKTHDR_QuestCancel				   );
	ADD_OPCODE_NAME(PKTHDR_QuestNPCFinish			   );
	ADD_OPCODE_NAME(PKTHDR_QuestNPCItemList			   );
	ADD_OPCODE_NAME(PKTHDR_QuestHandleFinished		   );
	ADD_OPCODE_NAME(PKTHDR_CharacterDataEx			   );
	ADD_OPCODE_NAME(PKTHDR_ItemSlotHandle			   );
	ADD_OPCODE_NAME(PKTHDR_T5QuestArena				   );
	ADD_OPCODE_NAME(PKTHDR_DynamicQuestOpen			   );
	ADD_OPCODE_NAME(PKTHDR_DynamicQuestOpenNPC		   );
	ADD_OPCODE_NAME(PKTHDR_QuestItemSwap			   );
	ADD_OPCODE_NAME(PKTHDR_BossTimeUpdate			   );
	ADD_OPCODE_NAME(PKTHDR_RequestParty				   );
	ADD_OPCODE_NAME(PKTHDR_JoinParty				   );
	ADD_OPCODE_NAME(PKTHDR_UpdateParty				   );
	ADD_OPCODE_NAME(PKTHDR_UpdatePartyData			   );
	ADD_OPCODE_NAME(PKTHDR_ActionParty				   );
	ADD_OPCODE_NAME(PKTHDR_RequestRaid				   );
	ADD_OPCODE_NAME(PKTHDR_JoinRaid					   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopNPC				   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopAmount			   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopOpenTab			   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopTab				   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopOpenItemData		   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopItemData			   );
	ADD_OPCODE_NAME(PKTHDR_CoinShopBuy				   );
	ADD_OPCODE_NAME(PKTHDR_OpenCoinShop				   );
	ADD_OPCODE_NAME(PKTHDR_CoinServiceOpen			   );
	ADD_OPCODE_NAME(PKTHDR_CoinServiceBuy			   );
	ADD_OPCODE_NAME(PKTHDR_CoinServiceAgingRecoveryOpen);
	ADD_OPCODE_NAME(PKTHDR_CoinTraded				   );
	ADD_OPCODE_NAME(PKTHDR_CoinTradeSet				   );
	ADD_OPCODE_NAME(PKTHDR_PlayerInfo2				   );
	ADD_OPCODE_NAME(PKTHDR_KillsInSod				   );
	ADD_OPCODE_NAME(PKTHDR_ChatGame					   );
	ADD_OPCODE_NAME(PKTHDR_ChatMessage				   );
	ADD_OPCODE_NAME(PKTHDR_ChatItemLink				   );
	ADD_OPCODE_NAME(PKTHDR_PersonalShopMessage		   );
	ADD_OPCODE_NAME(PKTHDR_SyncData1				   );
	ADD_OPCODE_NAME(PKTHDR_SyncData2				   );
	ADD_OPCODE_NAME(PKTHDR_CharacterData			   );
	ADD_OPCODE_NAME(PKTHDR_Ping						   );
	ADD_OPCODE_NAME(PKTHDR_PingLoginServer			   );
	ADD_OPCODE_NAME(PKTHDR_FuryArena				   );
	ADD_OPCODE_NAME(PKTHDR_FuryArenaItemData		   );
	ADD_OPCODE_NAME(PKTHDR_FuryArenaBossData		   );
	ADD_OPCODE_NAME(PKTHDR_NPCLarryOpen				   );
	ADD_OPCODE_NAME(PKTHDR_SkillStatus				   );
	ADD_OPCODE_NAME(PKTHDR_UseSkillNew				   );
	ADD_OPCODE_NAME(PKTHDR_NPCMarinaOpen			   );
	ADD_OPCODE_NAME(PKTHDR_SaveAndClose				   );
	ADD_OPCODE_NAME(PKTHDR_Skill_Healing			   );
	ADD_OPCODE_NAME(PKTHDR_PhoenixPet				   );
	ADD_OPCODE_NAME(PKTHDR_Warehouse				   );
	ADD_OPCODE_NAME(PKTHDR_Save						   );
	ADD_OPCODE_NAME(PKTHDR_Disconnect				   );
	ADD_OPCODE_NAME(PKTHDR_Reconnect				   );
	ADD_OPCODE_NAME(PKTHDR_ReconnectLogin			   );
	ADD_OPCODE_NAME(PKTHDR_UpdateLevelClan			   );
	ADD_OPCODE_NAME(PKTHDR_TitleBoxMessage			   );
	ADD_OPCODE_NAME(PKTHDR_PlayDataChar				   );
	ADD_OPCODE_NAME(PKTHDR_UnitStatusContainer		   );
	ADD_OPCODE_NAME(PKTHDR_GameStatus				   );
	ADD_OPCODE_NAME(PKTHDR_KillCharacter			   );
	ADD_OPCODE_NAME(PKTHDR_RegenChar				   );
	ADD_OPCODE_NAME(PKTHDR_UpdatePetData			   );
	ADD_OPCODE_NAME(PKTHDR_UpdatePetDataEx			   );
	ADD_OPCODE_NAME(PKTHDR_AttackData				   );
	ADD_OPCODE_NAME(PKTHDR_GoldBellatraClan			   );
	ADD_OPCODE_NAME(PKTHDR_OpenMyPlayerShop            );
	ADD_OPCODE_NAME(PKTHDR_OpenPlayerShop              );
	ADD_OPCODE_NAME(PKTHDR_PlayerShopConfig            );
	ADD_OPCODE_NAME(PKTHDR_PlayerShopTrade			   );
	ADD_OPCODE_NAME(PKTHDR_GetPlayerInfoData			   );
	ADD_OPCODE_NAME(PKTHDR_LastAttackDamage			   );
	ADD_OPCODE_NAME(PKTHDR_SingleTargetSkillData				   );
	ADD_OPCODE_NAME(PKTHDR_MultiTargetSkillData				   );
	ADD_OPCODE_NAME(PKTHDR_BuyItemNPCShop			   );
	ADD_OPCODE_NAME(PKTHDR_SellItemNPCShop             );
	ADD_OPCODE_NAME(PKTHDR_RepairItemNPCShop           );
	ADD_OPCODE_NAME(PKTHDR_ServerList				   );
	ADD_OPCODE_NAME(PKTHDR_SaveData					   );
	ADD_OPCODE_NAME(PKTHDR_SelectCharacter			   );
	ADD_OPCODE_NAME(PKTHDR_FailRecordData			   );
	ADD_OPCODE_NAME(PKTHDR_UserInfo					   );
	ADD_OPCODE_NAME(PKTHDR_CreateCharacter			   );
	ADD_OPCODE_NAME(PKTHDR_DeleteCharacter			   );
	ADD_OPCODE_NAME(PKTHDR_Version					   );
	ADD_OPCODE_NAME(PKTHDR_AccountLoginCode			   );
	ADD_OPCODE_NAME(PKTHDR_LoginUser				   );
	ADD_OPCODE_NAME(PKTHDR_BuffSkillsParty			   );
	ADD_OPCODE_NAME(PKTHDR_CancelDrasticSpirit		   );
	ADD_OPCODE_NAME(PKTHDR_CancelSkill				   );
	ADD_OPCODE_NAME(PKTHDR_SkillCast				   );
	ADD_OPCODE_NAME(PKTHDR_SkillCastData			   );
	ADD_OPCODE_NAME(PKTHDR_EvasionSuccess			   );
	ADD_OPCODE_NAME(PKTHDR_PartyUsePot				   );
	ADD_OPCODE_NAME(PKTHDR_HolyMind					   );
	ADD_OPCODE_NAME(PKTHDR_Wisp						   );
	ADD_OPCODE_NAME(PKTHDR_OpenNPC					   );
	ADD_OPCODE_NAME(PKTHDR_NpcData					   );
	ADD_OPCODE_NAME(PKTHDR_SetExpOld_DontUse		  );
	ADD_OPCODE_NAME(PKTHDR_TeleportEvent			   );
	ADD_OPCODE_NAME(PKTHDR_SetExpOld_DontUse	       );
	ADD_OPCODE_NAME(PKTHDR_UnitStatusMove			   );
	ADD_OPCODE_NAME(PKTHDR_PlayData					   );
	ADD_OPCODE_NAME(PKTHDR_UnitInfoQuick			   );
	ADD_OPCODE_NAME(PKTHDR_OpenEventGirl			   );
	ADD_OPCODE_NAME(PKTHDR_WarpGateField			   );
	ADD_OPCODE_NAME(PKTHDR_DeleteFlagPoint			   );
	ADD_OPCODE_NAME(PKTHDR_TheDeath					   );
	ADD_OPCODE_NAME(PKTHDR_PlayItem					   );
	ADD_OPCODE_NAME(PKTHDR_SaveThrowMoney              );
	ADD_OPCODE_NAME(PKTHDR_SaveThrowItem2              );
	ADD_OPCODE_NAME(PKTHDR_UseUnionCore				   );
	ADD_OPCODE_NAME(PKTHDR_WingItem                    );
	ADD_OPCODE_NAME(PKTHDR_BellatraInformation		   );
	ADD_OPCODE_NAME(PKTHDR_SocketItemDataToServer	   );
	ADD_OPCODE_NAME(PKTHDR_SocketItemDataToClient	   );
	ADD_OPCODE_NAME(PKTHDR_DeleteItemFromMap		   );
	ADD_OPCODE_NAME(PKTHDR_NewItem					   );
	ADD_OPCODE_NAME(PKTHDR_NewItemAuto				   );
	ADD_OPCODE_NAME(PKTHDR_MapIndicators			   );
	ADD_OPCODE_NAME(PKTHDR_QuestMonsterName			   );
	ADD_OPCODE_NAME(PKTHDR_QuestMonsterNameReq		   );
	ADD_OPCODE_NAME(PKTHDR_UpgradeQuestWeapon		   );
	ADD_OPCODE_NAME(PKTHDR_MonsterKill				   );
	ADD_OPCODE_NAME(PKTHDR_GetItem                     );
	ADD_OPCODE_NAME(PKTHDR_PaymentMoney                );
	ADD_OPCODE_NAME(PKTHDR_UpgradeAgingItem			   );
	ADD_OPCODE_NAME(PKTHDR_AgingItem				   );
	ADD_OPCODE_NAME(PKTHDR_OpenSmelting				   );
	ADD_OPCODE_NAME(PKTHDR_OpenResetItem			   );
	ADD_OPCODE_NAME(PKTHDR_OpenManufacture			   );
	ADD_OPCODE_NAME(PKTHDR_ManufactureItem			   );
	ADD_OPCODE_NAME(PKTHDR_MixItem					   );
	ADD_OPCODE_NAME(PKTHDR_ResetItem				   );
	ADD_OPCODE_NAME(PKTHDR_NPCItemBox				   );
	ADD_OPCODE_NAME(PKTHDR_RollDiceDropItem			   );
	ADD_OPCODE_NAME(PKTHDR_RollDiceRanking			   );
	ADD_OPCODE_NAME(PKTHDR_IntegrityUpdateItem		   );
	ADD_OPCODE_NAME(PTKHDR_Effect_Ice				   );
	ADD_OPCODE_NAME(PKTHDR_NetStartMaintenance		   );
	ADD_OPCODE_NAME(PKTHDR_NetStopMaintenance		   );
	ADD_OPCODE_NAME(PKTHDR_EnterBellatraCastle         );
	ADD_OPCODE_NAME(PKTHDR_GameTimeSync                );
	ADD_OPCODE_NAME(PTKHDR_EventGirlReset				);
	ADD_OPCODE_NAME(PKTHDR_RankingListRequest			);
	ADD_OPCODE_NAME(PKTHDR_NumfOfInitialBellatraPlayers	);
	ADD_OPCODE_NAME(PKTHDR_ExtraUserData				);
	ADD_OPCODE_NAME(PKTHDR_SoloRankingListRequest		);
	ADD_OPCODE_NAME(PKTHDR_PremiumDataSync 				);
	ADD_OPCODE_NAME(PKTHDR_MiscUnitCommand 				);
	ADD_OPCODE_NAME(PKTHDR_ForceOrbCancel				);
	ADD_OPCODE_NAME(PKTHDR_MapSpawnMarkers 				);
	ADD_OPCODE_NAME(PKTHDR_ServerSkillData 				);
	ADD_OPCODE_NAME(PKTHDR_SkillBuffStatus				);
	ADD_OPCODE_NAME(PKTHDR_HalloweenEventStatus			);
	ADD_OPCODE_NAME(PKTHDR_TitleList					);
	ADD_OPCODE_NAME(PKTHDR_SetTitle						);
	ADD_OPCODE_NAME(PKTHDR_RecoverItem					);
	ADD_OPCODE_NAME(PKTHDR_CHECKITEM					);
	ADD_OPCODE_NAME(PKTHDR_LIMIT_DAMAGE					);
	ADD_OPCODE_NAME(PKTHDR_BLESSCASTLE_INFO				);
	ADD_OPCODE_NAME(PKTHDR_BLESSCASTLE_TAX				);
	ADD_OPCODE_NAME(PKTHDR_BellatraCrownFull			);
	ADD_OPCODE_NAME(PKTHDR_BellatraCrownSolo			);
	ADD_OPCODE_NAME(PKTHDR_RecordResult					);
	ADD_OPCODE_NAME(PKTHDR_SendGameServer				);
	ADD_OPCODE_NAME(PKTHDR_DamageEncodeMem				);
	ADD_OPCODE_NAME(PKTHDR_DamageEncodeMem2				);
	ADD_OPCODE_NAME(PKTHDR_PROCESSINFO					);
	ADD_OPCODE_NAME(PKTHDR_NetGetMuteSync				);
	ADD_OPCODE_NAME(PKTHDR_NetMuteSync					);
	ADD_OPCODE_NAME(PKTHDR_Effect_User					);
	ADD_OPCODE_NAME(PKTHDR_Connected					);
	ADD_OPCODE_NAME(PKTHDR_QuestHandleFinishedContainer	);
	ADD_OPCODE_NAME(PKTHDR_AttackResult					);
	ADD_OPCODE_NAME(PKTHDR_SendDataServer				);
	ADD_OPCODE_NAME(PKTHDR_GoldPickup				    );
	ADD_OPCODE_NAME(PKTHDR_CharacterCombatData			);
	ADD_OPCODE_NAME(PKTHDR_Shop_ItemList				);
	ADD_OPCODE_NAME(PKTHDR_NewItemNPC				    );
	ADD_OPCODE_NAME(PKTHDR_CommandUser				    );
	ADD_OPCODE_NAME(PKTHDR_NetBellatraSoloRanking		);
	ADD_OPCODE_NAME(PKTHDR_NetHallowenEvent				);
	ADD_OPCODE_NAME(PKTHDR_NetMimicEvent				);
	ADD_OPCODE_NAME(PKTHDR_PROCESS_TIMEMAX				);
	ADD_OPCODE_NAME(PKTHDR_HACKTRAP						);
	ADD_OPCODE_NAME(PKTHDR_USEITEM_CODE					);
	ADD_OPCODE_NAME(PKTHDR_MAKE_LINKCORE				);
	ADD_OPCODE_NAME(PKTHDR_DamageInfoContainer			);
	ADD_OPCODE_NAME(PKTHDR_UseSkillPartyBuffNew			);
	ADD_OPCODE_NAME(PKTHDR_QuestNamesRequest			);
	ADD_OPCODE_NAME(PKTHDR_QuestNamesResponse			);
	ADD_OPCODE_NAME(PKTHDR_NewQuestNamesPostLvlUp		);
	ADD_OPCODE_NAME(PKTHDR_QuestDataRequest             );
	ADD_OPCODE_NAME(PKTHDR_QuestDataResponse            );
	ADD_OPCODE_NAME(PKTHDR_NeverSinkTeleportRequest     );
	ADD_OPCODE_NAME(PKTHDR_NeverSinkOpenUI              );
	ADD_OPCODE_NAME(PKTHDR_MissingRequiredQuestItems    );
	ADD_OPCODE_NAME(PKTHDR_RemoveFinishedQuest          );
	ADD_OPCODE_NAME(PKTHDR_RollDiceDropUserResult       );

	ADD_OPCODE_NAME(PKTHDR_ServerSkillInfo				);
	ADD_OPCODE_NAME(PKTHDR_NETWORK_QUALITY				);
	ADD_OPCODE_NAME(PKTHDR_UPDATE_SERVER_PARAM			);
	ADD_OPCODE_NAME(PKTHDR_Warning_Skill				);
	ADD_OPCODE_NAME(PKTHDR_Client_Error					);
	ADD_OPCODE_NAME(PKTHDR_Resistance					);
    ADD_OPCODE_NAME(PKTHDR_QuickHealthUpdate			);
    ADD_OPCODE_NAME(PKTHDR_ClearUnitDataFromMap			);
    ADD_OPCODE_NAME(PKTHDR_NpcQuestStatus			);

	ADD_OPCODE_NAME(PKTHDR_NETWORK_QUALITY				);
	ADD_OPCODE_NAME(PKTHDR_ForceOrbData					);
	ADD_OPCODE_NAME(PKTHDR_NpcUnitClick					);
	ADD_OPCODE_NAME(PKTHDR_QuestCommand					);
	ADD_OPCODE_NAME(PKTHDR_SET_BLACKLIST2				);
	ADD_OPCODE_NAME(PKTHDR_BlessCastleTopClanSync		);
	ADD_OPCODE_NAME(PKTHDR_NetStarWarsEvent				);
	ADD_OPCODE_NAME(PKTHDR_NetBeeEvent					);
	ADD_OPCODE_NAME(PKTHDR_NetOpenItemDistributor		);
	ADD_OPCODE_NAME(PKTHDR_NetGameMasterCommand			);
	ADD_OPCODE_NAME(PKTHDR_NetPlayDataEx				);
	ADD_OPCODE_NAME(PKTHDR_NetRemoveFinishedQuestData	);
	ADD_OPCODE_NAME(PKTHDR_NetSendItemData				);
	ADD_OPCODE_NAME(PKTHDR_YahooMotion					);
	ADD_OPCODE_NAME(PKTHDR_ServerSkillChainData			);
	ADD_OPCODE_NAME(PKTHDR_ServerCommonSkillInfo		);
	ADD_OPCODE_NAME(PKTHDR_OpenAgingMaster				);

	ADD_OPCODE_NAME(PKTHDR_NPCRankListAllPlayers		);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListPVP				);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListByClass			);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListClanSod			);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListIndividualSod		);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListBlessedCastle		);
	ADD_OPCODE_NAME(PKTHDR_NPCRankListSoloSod			);

	ADD_OPCODE_NAME(PKTHDR_Unknown1						);
	ADD_OPCODE_NAME(PKTHDR_Unknown2						);
	ADD_OPCODE_NAME(PKTHDR_Unknown3						);
	ADD_OPCODE_NAME(PKTHDR_Unknown4						);
	ADD_OPCODE_NAME(PKTHDR_Unknown5						);
	ADD_OPCODE_NAME(PKTHDR_Unknown6						);
	ADD_OPCODE_NAME(PKTHDR_Unknown7						);
	ADD_OPCODE_NAME(PKTHDR_Unknown8						);
	ADD_OPCODE_NAME(PKTHDR_Unknown9						);
	ADD_OPCODE_NAME(PKTHDR_Unknown10					);
	ADD_OPCODE_NAME(PKTHDR_Unknown11					);
	ADD_OPCODE_NAME(PKTHDR_Unknown12					);
	ADD_OPCODE_NAME(PKTHDR_SendStablePet				);
	ADD_OPCODE_NAME(PKTHDR_SendStablePet_ActivePet		);

	ZeroMemory(reconnectedUsers, sizeof(UserData*)* _countof(reconnectedUsers));
}


SocketServer::~SocketServer()
{
	CloseHandle( hEventSocket );
}

int SocketServer::GetNumFreeSlots()
{
	int iFreeSlots = 0;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( !sd->bInUse )
			iFreeSlots++;
	}

	return iFreeSlots;
}

SocketData * SocketServer::GetFreeSocketData()
{
	SocketData * r = NULL;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;

		if ( !sd->bInUse )
		{
			sd->Init();

			r = sd;
			break;
		}
	}

	return r;
}

SocketData * SocketServer::GetSocketData( const char * pszIP, int iPort )
{
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bConnected && STRINGCOMPARE( sd->szIP, pszIP ) && sd->iPort == iPort )
			return sd;
	}
	return NULL;
}

int SocketServer::GetConnectionCount( UINT lIP )
{
	int iRet = 0;

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		if ( sd->bInUse && sd->bKeepAlive == FALSE && sd->lIP == lIP )
		{
			iRet++;
			if ( sd->u && sd->u->GetGameLevel() >= GAMELEVEL_Two )
				iRet--;
		}
	}

	return iRet;
}

void SocketServer::DisconnectIP( UINT lIP )
{
	if ( pcaSocketData )
	{
		for ( int i = 0; i < iMaxConnections; i++ )
		{
			SocketData * sd = pcaSocketData + i;
			if ( sd->bInUse && sd->bConnected && sd->bKeepAlive == FALSE && sd->lIP == lIP )
				WNDPROC_SOCKETCLOSE( hWnd, sd );
		}
	}
}

SocketData * SocketServer::Connect( const char * pszIP, int iPort )
{
	INFO( "Connect %s:%d", pszIP, iPort );

	SocketData * sd = NULL;

	if ( (sd = GetInstance()->GetSocketData( pszIP, iPort )) == NULL )
	{
		sd = GetInstance()->GetFreeSocketData();

		if ( sd )
		{
			if ( !sd->Connect( pszIP, iPort ) )
			{
				sd->UnInit();

				return NULL;
			}

			if ( (sd->u = USERSERVER->GetFreeUser()) != NULL )
			{
				sd->ud = sd->u->pcUserData;

				sd->u->Start( sd );
			}
		}
	}

	return sd;
}


void SocketServer::SocketAccept( SOCKET s, sockaddr_in * addr )
{
	//std::cout << "SocketAccept: " << std::endl;

	// Mutex Area
	EnterCriticalSection( &sSectionAccept );

	SocketData * sd = NULL;
	BOOL bAccepted = FALSE;

	if ( IsFull() == FALSE )
	{
		BOOL bPass = (USERS_ONLINE < MAX_USERS) ? TRUE : FALSE;

		if ( bPass )
		{
			if ( (sd = GetFreeSocketData()) != NULL )
			{
				sd->u = NULL;
				sd->ud = NULL;

				if ( (sd->u = USERSERVER->GetFreeUser()) != NULL )
				{
					sd->ud = sd->u->pcUserData;

					sd->u->Start( sd );
					sd->Open( s, addr );

					bAccepted = TRUE;
				}
			}
		}
	}

	if ( bAccepted )
	{
		//Send the Byte Encryption Set
		sd->SendKeySet( GetPacketObfuscatorByte() );

		// Send Version
		PacketVersion sPacket;
		sPacket.iLength		= sizeof( PacketVersion );
		sPacket.iHeader		= PKTHDR_Version;
		sPacket.bServerFull = FALSE;
		sPacket.iUnk2		= 0;
		sPacket.iVersion	= (*(int*)0x006E4F5C);
		sd->SendPacket( &sPacket, FALSE );
	}
	else
	{
		if ( sd )
		{
			sd->UnInit();

			sd = NULL;
		}

		shutdown(s, SD_BOTH);
		closesocket( s );
		s = INVALID_SOCKET;
	}

	LeaveCriticalSection( &sSectionAccept );
}




void SocketServer::LogPacketTimes()
{
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_LogDB_PacketLog );
	if ( pcDB->Open () )
	{
		const char * szQuery = "INSERT INTO PacketLog ([ServerType],[PacketID],[TotalCount],[TotalDurationMS],[MinDurationMS],[MaxDurationMS],[DateTime]) VALUES (?,?,?,?,?,?,GETDATE())";
		DWORD pHeader = 0;
		PacketStat * pStat = NULL;

		for ( auto & kv : m_PacketReceiveStat )
		{
			pHeader = kv.first;
			pStat = &kv.second;

			pcDB->Prepare(szQuery );

			pcDB->BindParameterInput( 1, EDatabaseDataType::PARAMTYPE_Integer, &GAME_SERVER );
			pcDB->BindParameterInput( 2, EDatabaseDataType::PARAMTYPE_Integer, &pHeader );
			pcDB->BindParameterInput( 3, EDatabaseDataType::PARAMTYPE_Integer, &pStat->iCounter );
			pcDB->BindParameterInput( 4, EDatabaseDataType::PARAMTYPE_Float, &pStat->fTotalDuration );
			pcDB->BindParameterInput( 5, EDatabaseDataType::PARAMTYPE_Float, &pStat->fMinDuration );
			pcDB->BindParameterInput( 6, EDatabaseDataType::PARAMTYPE_Float, &pStat->fMaxDuration );

			pcDB->Execute();
			pcDB->Clear();
		}

		pcDB->Close ();

		pStat = NULL;
	}
}

void SocketServer::SocketPacket( SocketData * sd, PacketReceiving * p )
{
    SOCKETMUTEX->Lock( 5000 );
    if ( sd )
    {
        auto l_User     = sd->u;
        auto l_UserData = l_User->pcUserData;
        auto l_IP       = sd->lIP;
        auto l_Packet   = (Packet *)p->baPacket;
        auto l_Opcode   = l_Packet->iHeader;


#ifdef MEASURE_PACKET_EXECUTION_TIME

		if ( TIME > uNextPacketLogTime )
		{
#ifdef DEBUG
			uNextPacketLogTime = TIME + 10; //every 10 sec
#else
			uNextPacketLogTime = TIME + 600; //every 10 min
#endif
			LogPacketTimes();

			m_PacketReceiveStat.clear();
		}

		if ( m_PacketReceiveStat.find( l_Opcode ) == m_PacketReceiveStat.end() )
		{
			m_PacketReceiveStat.insert( { l_Opcode, PacketStat{} } );
			m_PacketReceiveStat[l_Opcode].fMinDuration = 100000.0f;
		}

		auto t1 = high_resolution_clock::now();

#endif


        switch (SocketServer::AllowPacket(sd))
        {
            case EAllowPacketResult::BytesRateLimitExeeded:
            {
                /// We now disconnect instead of ban
                //long l_IP = sd->l_IP;
                //WNDPROC_SOCKETCLOSE(hWnd, sd);
                //AddBanIP(l_IP, "WRONG_PACKET");

                char l_IPStr[32] = { 0 };
                STRINGFORMAT(l_IPStr, "%d.%d.%d.%d", l_IP & 0xFF, (l_IP >> 8) & 0xFF, (l_IP >> 16) & 0xFF, (l_IP >> 24) & 0xFF);

                auto l_Message = "SocketServer::SocketPacket() disconnect " + std::string(l_IPStr) + " for EAllowPacketResult::BytesRateLimitExeeded " + std::to_string(sd->iCountIN) + "/" + std::to_string(MAX_PACKETBYTESFLOW) + " bytes";

                LOGSERVER->LogSocketClose(sd, l_Message.c_str(), _ReturnAddress());
                DEBUG(l_Message.c_str());

                WNDPROC_SOCKETCLOSE(hWnd, sd);
                break;
            }

            case EAllowPacketResult::Allow:
            default:
            {
                if ((l_User) && (l_UserData) && (l_UserData->pcSocketData))
                {
                    DWORD dwTickCount;

                    if ((dwTickCount = UserServer::AllowPacket(l_User)) != 0)
                    {
#ifdef DEV_MODE
						if ( l_User->GetGameLevel() == EGameLevel::GAMELEVEL_Four )
						{
							LOGGER->AttachGMUser( l_User );
						}
#endif
                        PACKETSERVER->AnalyzePacket(l_User, p);


#ifdef MEASURE_PACKET_EXECUTION_TIME

						duration<double, std::milli> ms_double = high_resolution_clock::now() - t1;

						double fDuration = ms_double.count();

						m_PacketReceiveStat[l_Opcode].iCounter += 1;
						m_PacketReceiveStat[l_Opcode].fTotalDuration += static_cast<float>( fDuration );

						if ( fDuration > m_PacketReceiveStat[l_Opcode].fMaxDuration )
							m_PacketReceiveStat[l_Opcode].fMaxDuration = static_cast<float>( fDuration );

						if ( fDuration < m_PacketReceiveStat[l_Opcode].fMinDuration )
							m_PacketReceiveStat[l_Opcode].fMinDuration = static_cast<float>( fDuration );
#endif

#ifdef DEV_MODE
						if ( l_User->GetGameLevel() == EGameLevel::GAMELEVEL_Four )
						{
							LOGGER->DeattachGMUser( );
						}
#endif

                    }
                    else
                    {
						const std::string & l_OpcodeName = SOCKETSERVER->GetOpcodeNameOrDefault(l_Opcode);
                        WNDPROC_SOCKETCLOSE(hWnd, sd);
                        AddBanIP(l_IP, ("WRONG_PACKET_AUTHED UserServer::AllowPacket(" + l_OpcodeName + ")").c_str());
                    }
                }
                else
                {
					LOGSERVER->LogSocketClose( sd, FormatString( "SocketServer::SocketPacket()::NotUserData (pcUser = %s) (pcUserData = %s) (socket = %s)",
						( l_User == NULL ? "NULL" : "OK" ),
						( l_UserData == NULL ? "NULL" : "OK" ),
						( l_UserData && l_UserData->pcSocketData == NULL ? "NULL" : "OK" )), _ReturnAddress() );

					WNDPROC_SOCKETCLOSE(hWnd, sd);
                }

                break;
            }
        }

        sd->FreePacketReceiving( p );
        sd->dwTimeLastPacketReceived = TICKCOUNT;


		auto t2 = steady_clock::now();
    }
    if ( SOCKETMUTEX->Unlock() )
    {
    //    DEBUG( "Socket Packet [%d][0x%08X]", p->sPacket.iLength, p->sPacket.iHeader );
    }
}

void SocketServer::SocketClose( SocketData * sd )
{
	if ( !pcSocketCloseMutex )
		pcSocketCloseMutex = new CMutex();

	pcSocketCloseMutex->Lock( 5005 );
	if ( sd && sd->bInUse )
	{
		User * u = sd->u;
		UserData * ud = u->pcUserData;

		if ( u && ud )
		{
			if ( u->bClosingSocket == TRUE )
			{
				pcSocketCloseMutex->Unlock();
				return;
			}
			u->bClosingSocket = TRUE;

			if ( !u->bNetServer )
			{
				if ( GAME_SERVER )
					PARTYHANDLER->LeaveParty( u );

			//	if ( u->IsInGame() )
			//		RECONNECTSERVERHANDLER->AddUser( u );

				CharacterFile * cf = sd->u->pcUserData->psCharacterFile;
				if ( cf && ud->szAccountName[0] && ud->szCharacterName[0] && cf->iLength > 0 )
				{
					CharacterData * ch = (CharacterData*)&cf->sCharInfo;
					ch->szName[31] = 0;
					if ( STRINGCOMPAREI( ch->szName, ud->szCharacterName ) )
					{
						CALL_WITH_ARG1( 0x0044D070, (DWORD)ud );
						USERSERVER->InventoryServerCheck( ud, 1 );
						if ( LOGIN_SERVER )
						{
							CHARACTERSERVER->SetCharacterEXPOnDisconnect( ud->szCharacterName, ud->liEXP.QuadPart );
							ch->iCurrentExpLow	= ud->liEXP.LowPart;
							ch->iCurrentExpHigh = ud->liEXP.HighPart;
						}

						char * pszCharacterName = ud->szCharacterName;
						DWORD safecheck = 0;
						__asm
						{
							PUSH cf;
							PUSH pszCharacterName;
							PUSH ud;
							MOV EAX, 0x0044EED0;
							CALL EAX; //Save?
							ADD ESP, 0x0C;
							MOV safecheck, EAX;
						}
						if ( safecheck != TRUE )
							LOGERROR( "ERR: Save Failed %s", ud->szCharacterName );

						cf->iLength = 0;
					}
				}


				CALLT( 0x00576040, (DWORD)ud );
				CALL_WITH_ARG1( 0x00559000, (DWORD)ud );

				if ( ud->pcWarehouse )
				{
					WAREHOUSEHANDLER->Close( ud->pcWarehouse );
					ud->pcWarehouse = NULL;
				}
				if ( ud->psUnknown1 )
					SAFE_DELETE( ud->psUnknown1 );
			}

			LOGSERVER->LogSocketClose( sd, "SocketServer::SocketClose()", _ReturnAddress() );

			if ( sd )
				SOCKETSERVER->AddDisconnectedUser( sd->lIP );

			if ( NETSERVER->Disconnected( sd ) )
				DEBUG( "NET SERVER DC" );

			if ( GAME_SERVER )
			{
				QUESTARENAHANDLER->OnDisconnected( u );
			}
			u->Stop();
			u->UnInit();
		}
		sd->Close();
		sd->UnInit();
	}
	pcSocketCloseMutex->Unlock();
}

void SocketServer::AddReconnectUser(UserData* pcUser)
{
	for (int i = 0; i < MAX_RECONNECTCONNECTIONS; i++)
	{
		if (reconnectedUsers[i] == NULL)
		{
			reconnectedUsers[i] = pcUser;
		}
	}
}

void SocketServer::Load()
{
	iMaxConnections = 500;

	pcaSocketData = new SocketData[iMaxConnections];

	hEventSocket = CreateEvent( NULL, FALSE, FALSE, NULL );

	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * sd = pcaSocketData + i;
		sd->pcThis = sd;
		sd->bInUse = FALSE;
		sd->Prepare( 128, 128 );
		sd->hReceiveThread = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)&SocketServer::Receiver, sd, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
		sd->hSendThread = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)&SocketServer::Sender, sd, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	}
	bActive = TRUE;
}

DWORD WINAPI SocketServer::Sender( SocketData * sd )
{
	SocketServer * m = GetInstance();

	HWND hWnd = m->GetHWND();

	do
	{
		WaitForSingleObject( sd->hSendThreadSignal, INFINITE );

		if ( !m->IsActive() )
			break;

		PacketSending * p;
		while ( (p = sd->NextPacketSending()) != NULL )
		{
			//Post Packet Message
			if ( !sd->Send( p ) )
			{
				sd->FreePacketSending( p );
				break;
			}
			sd->FreePacketSending( p );
		}

	}
	while ( m->IsActive() );

	return TRUE;
}

DWORD WINAPI SocketServer::Receiver( SocketData * sd )
{
	SocketServer * m = GetInstance();

	HWND hWnd = m->GetHWND();

	do
	{
		WaitForSingleObject( sd->hReceiveThreadSignal, INFINITE );

		if ( !m->IsActive() )
			break;

		//u_long iMode = 0;
		//ioctlsocket(sd->s, FIONBIO, &iMode);

		PacketReceiving * p;
		while ( (p = sd->ReceivePacket()) != NULL )
		{
			PostMessageA( GetInstance()->GetHWND(), WM_SOCKETPACKET, (WPARAM)sd, (LPARAM)p );
		}

		sd->bBlockSend = TRUE;

		if ( m->IsActive() )
		{
			if ( sd->bConnected )
			{
				long lIP = 0;
				bool shouldLog = true;
				std::string reason = "SocketError";

				if (sd->iRecvEC == 0x0F0F0F0F)
				{
					reason = "NoMoreFreePacketReceiving";
				}
				//Anti-DDoS
				else if ( sd->iRecvEC == 0x6A6A ) //27242
				{
					lIP = sd->lIP;
					reason = "AntiDDOS";
                }
                //10060 = Connection timed out error with proxy server or ISA Server on slow link (Connection timed out)
                //10054 = Connection was closed by the remote site.
				else if(sd->iRecvEC == 10054)
                {
                    reason = "RemotlyClosed";
					shouldLog = false;
                    shutdown(sd->s, SD_BOTH);
                    closesocket(sd->s);
					sd->s = INVALID_SOCKET;
				}

				if (shouldLog)
				{
					std::stringstream l_Error;
					l_Error << "SocketServer::Receiver() [" << sd->iRecvEC << ", " << sd->iRecvRet << "] " << reason;
					if (reason == "SocketError")
						l_Error << std::strerror(sd->iRecvRet);

					LOGERROR(l_Error.str());
                    LOGSERVER->LogSocketClose(sd, l_Error.str().c_str(), _ReturnAddress());
				}

				WNDPROC_SOCKETCLOSE( m->GetHWND(), sd );

				// HARDCPP Commented for now
				//if ( lIP != 0 )
				//	m->AddBanIP( lIP, "PACKET_SIZE_OVERFLOW");
			}
		}

	}
	while ( m->IsActive() );

	return TRUE;
}

DWORD WINAPI SocketServer::Listener( SOCKET * ls )
{
	SOCKET s = *ls;
	SocketServer * pc = GetInstance();

	sockaddr_in * sout = new sockaddr_in;
	int len = sizeof( sockaddr_in );

	while ( pc->IsActive() )
	{
		SOCKET n = INVALID_SOCKET;
		if ( (n = accept( s, (SOCKADDR *)sout, &len )) != INVALID_SOCKET )
		{
			BOOL bContinue = TRUE;
			if ( pc->GetConnectionCount( sout->sin_addr.s_addr ) >= MAX_SOCKET_ON_SAME_IP)
			{
				shutdown(n, SD_BOTH);
				closesocket( n );
				bContinue = FALSE;
			}

			if ( bContinue )
			{
				//Send Accept Message
				SendMessageA( GetInstance()->GetHWND(), WM_SOCKETACCEPT, n, (LPARAM)sout );
			}
		}
	}
	delete sout;

	return TRUE;
}

DWORD SocketServer::DeniedSocketsHandler()
{
	SocketServer * m = GetInstance();

	do
	{
#ifdef RECONNECT_SYSTEM
		m->HandleReconnectedUsers();
#endif
		m->HandleBanFirewall();

		WaitForSingleObject( m->GetHandleEventSocket(), 2500 );
	}
	while ( m->IsActive() );

	return 0;
}

void SocketServer::HandleReconnectedUsers()
{
#ifdef RECONNECT_SYSTEM
	for (int i = 0; i < MAX_RECONNECTCONNECTIONS; i++)
	{
		if (reconnectedUsers[i])
		{
			SENDPACKETBLANK(USERDATATOUSER(reconnectedUsers[i]), LOGIN_SERVER ? PKTHDR_ReconnectLogin : PKTHDR_Reconnect);
			reconnectedUsers[i] = NULL;
		}
	}
#endif
}

void SocketServer::HandleBanFirewall()
{
	char l_IPStr[32] = { 0 };

	pcMutex->Lock( 6101 );
	while (!m_IPsToBan.empty())
	{
		BanIPElement & l_Current = m_IPsToBan.front();

		STRINGFORMAT(l_IPStr, "%d.%d.%d.%d", l_Current.IP & 0xFF, (l_Current.IP >> 8) & 0xFF, (l_Current.IP >> 16) & 0xFF, (l_Current.IP >> 24) & 0xFF);

		LOGSERVER->AddFirewallBan(l_IPStr, l_Current.Reason);

		m_IPsToBan.pop();
	}
	pcMutex->Unlock();
}

void SocketServer::HandleConnectionCount()
{
	if ( pcaSocketData )
	{
		for ( int i = 0; i < iMaxConnections; i++ )
		{
			SocketData * sd = pcaSocketData + i;
			if ( sd->bInUse && sd->bConnected && sd->bKeepAlive == FALSE )
			{
				int iConn = 0;
				if ( (iConn = GetConnectionCount( sd->lIP )) >= MAX_SOCKET_ON_SAME_IP)
				{
					DisconnectIP( sd->lIP );
					AddBanIP(sd->lIP, "MAX_SOCKET_ON_SAME_IP");
					LOGERROR( "Ban IP: [%d][%d]", sd->lIP, iConn );
				}
			}
		}
	}
}

EAllowPacketResult SocketServer::AllowPacket( SocketData * sd )
{
	EAllowPacketResult bRet = EAllowPacketResult::Allow;

	//Flood protection
	if (sd->iCountIN > MAX_PACKETBYTESFLOW)
		bRet = EAllowPacketResult::BytesRateLimitExeeded;
	else if ( (TICKCOUNT - sd->dwTimeIN) > 1000 )
	{
		sd->iCountIN = 0;
		sd->dwTimeIN = TICKCOUNT;
	}

	if( sd->bKeepAlive )
		bRet = EAllowPacketResult::Allow;

	return bRet;
}

void SocketServer::AddBanIP( DWORD dwIP, const char * p_Reason)
{
	if (IsBannedIP(dwIP))
		return;

	BanIPElement l_Element;
	l_Element.IP		= dwIP;
	l_Element.Reason	= p_Reason;

	pcMutex->Lock( 6101 );
	m_BanIPs.emplace(dwIP);
	m_IPsToBan.push(l_Element);
	pcMutex->Unlock();
}

BOOL SocketServer::IsBannedIP( UINT lIP )
{
	pcMutex->Lock( 6101 );
	bool l_Result = m_BanIPs.find(lIP) != m_BanIPs.end();
	pcMutex->Unlock();

	return l_Result;
}

void SocketServer::AddDisconnectedUser( UINT lIP )
{
	std::lock_guard<std::mutex> l_Lock(m_DisconnectionHistoryMutex);

	auto l_It = m_DisconnectionHistory.find(lIP);

	if (l_It == m_DisconnectionHistory.end())
	{
		DisconnectedUser l_Entry;
		l_Entry.iDisconnectTimes	= 1;
		l_Entry.dwTime				= TICKCOUNT;
	}
	else
	{
		DisconnectedUser & l_Entry = l_It->second;

		l_Entry.iDisconnectTimes++;
		if ((l_Entry.dwTime + MAX_DISCONNECT_TIME) >= TICKCOUNT)
		{
			if (l_Entry.iDisconnectTimes >= MAX_DISCONNECT_TICK)
			{
				AddBanIP(lIP, "MAX_DISCONNECT_TICK");
			}
		}
		else
			l_Entry.iDisconnectTimes = 1;
	}
}

BOOL SocketServer::Init( HWND hwnd )
{
	hWnd = hwnd;

	SetPacketObfuscatorByte( SERVER_PORT );

	Listen( SERVER_PORT );
	return TRUE;
}
void SocketServer::Listen( int iPort )
{
	int iStatus = 0;
	// Start
	if ( (iStatus = WSAStartup( MAKEWORD( 2, 2 ), &sWsaData )) != 0 )
	{
		DEBUG( "SocketServer::Init()WSAStartup():: %d", iStatus );
		return;
	}

	if ( (sListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) != INVALID_SOCKET )
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons( iPort );

		if ( ::bind( sListenSocket, (SOCKADDR *)&sin, sizeof( sin ) ) != SOCKET_ERROR )
		{
			if ( listen( sListenSocket, SOMAXCONN ) == SOCKET_ERROR )
			{
				DEBUG( "SocketServer::Init()listen():: %d", WSAGetLastError() );
				closesocket( sListenSocket );
				sListenSocket = INVALID_SOCKET;
				return;
			}
		}
		else
		{
			DEBUG( "SocketServer::Init()bind():: %d", WSAGetLastError() );
			closesocket( sListenSocket );
			sListenSocket = INVALID_SOCKET;
			return;
		}
	}
	else
	{
		DEBUG( "SocketServer::Init()socket():: %d", WSAGetLastError() );
		return;
	}

	bActive = TRUE;

	InitializeCriticalSection( &sSectionAccept );


	// Success
	hListener = CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)Listener, &sListenSocket, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE)DeniedSocketsHandler, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL );
	Load();

}

void SocketServer::OnReadHandler( SOCKET s, DWORD dwParam )
{
	// Find Socket
	SocketData * sd = NULL;
	for ( int i = 0; i < iMaxConnections; i++ )
	{
		SocketData * p = pcaSocketData + i;
		if ( p->bInUse && p->s == s )
		{
			sd = p;
			break;
		}
	}

	if ( sd )
	{
		switch ( dwParam )
		{
			case FD_READ:
				//    SetEvent( sd->hReceiveThreadSignal );
				break;

			case FD_CLOSE:
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), sd );
				break;

			default:
				break;
		}
	}
}