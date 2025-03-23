#include "stdafx.h"
#include "CQuestGame.h"


void CQuestGame::HandleFinish( QuestData * pcQuestData )
{
	if ( pcQuestData )
	{
		TITLEBOX( "%s completed!", pcQuestData->GetName() );
		PacketQuestUpdateData s;
		ZeroMemory( &s, sizeof( PacketQuestUpdateData ) );
		s.iLength = sizeof( PacketQuestUpdateData );
		s.iHeader = PKTHDR_QuestUpdateData;

		CopyMemory( s.saMonstersKilled, pcQuestData->GetMonsterKilledData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.saItemsCollected, pcQuestData->GetItemsCollectData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.saMonstersReq, pcQuestData->GetMonstersReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.saItemsReq, pcQuestData->GetItemsReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

		s.iItemsCount = pcQuestData->GetItemCount();
		s.iMonsterCount = pcQuestData->GetMonsterCount();
		s.dwTimeLeft = pcQuestData->GetTimeQuest();
		s.iID = pcQuestData->GetID();
		s.bFinishedOtherRequirements = pcQuestData->IsOtherRequirementsFinished();
		SENDPACKETL( &s, TRUE );

		QUESTBOOKHANDLER->BuildQuestList(); //force rebuild
	}
}

#ifdef DEV_MODE
void CQuestGame::ForceFinishDebugOnly( int iQuestID )
{
	auto pcQuestData = GetQuestInstance()->GetQuest( iQuestID );
	if ( pcQuestData && !pcQuestData->IsFinished() )
	{
		TITLEBOX( "%s force completed!", pcQuestData->GetName() );

		PacketQuestUpdateData s;
		ZeroMemory( &s, sizeof( PacketQuestUpdateData ) );
		s.iLength = sizeof( PacketQuestUpdateData );
		s.iHeader = PKTHDR_QuestUpdateData;

		CopyMemory( pcQuestData->GetMonsterKilledData(), pcQuestData->GetMonstersReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( pcQuestData->GetItemsCollectData(),  pcQuestData->GetItemsReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

		pcQuestData->SetForceFinished();
		pcQuestData->SetKillsFinished();
		pcQuestData->SetItemsFinished();
		pcQuestData->SetOtherRequirementsFinished();

		CopyMemory( s.saMonstersKilled, pcQuestData->GetMonsterKilledData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.saMonstersReq,	pcQuestData->GetMonstersReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

		CopyMemory( s.saItemsCollected, pcQuestData->GetItemsCollectData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
		CopyMemory( s.saItemsReq,		pcQuestData->GetItemsReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

		s.iItemsCount = pcQuestData->GetItemCount();
		s.iMonsterCount = pcQuestData->GetMonsterCount();
		s.dwTimeLeft = pcQuestData->GetTimeQuest();
		s.iID = pcQuestData->GetID();
		s.bFinishedOtherRequirements = pcQuestData->IsOtherRequirementsFinished();
		SENDPACKETL( &s, TRUE );

		QUESTBOOKHANDLER->BuildQuestList(); //force rebuild
	}
}
#endif

void CQuestGame::HandleTimeOver( QuestData * pcQuestData )
{
	TITLEBOX( "%s time is over!", pcQuestData->GetName() );

	TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );

	QUESTBOOKHANDLER->BuildQuestList(); //force rebuild
}

void CQuestGame::HandleDeleteItem( int iQuestID, BOOL bFinished )
{
	switch ( iQuestID )
	{
		case QUESTID_AMatterOfStrenght:
			ITEMHANDLER->DeleteItemQuest( ITEMID_ValentoEye );
			ITEMHANDLER->DeleteItemQuest( ITEMID_PrinceEye );
			break;

		case QUESTID_CryMeARiver:
			ITEMHANDLER->DeleteItemQuest( ITEMID_DiamondTears );
			break;

		case QUESTID_GreenLiquidQuestStep1:
		case QUESTID_GreenLiquidQuestStep2:
			ITEMHANDLER->DeleteItemQuest( ITEMID_PotionAgingQuestBee );
			ITEMHANDLER->DeleteItemQuest( ITEMID_GreenLiquidTree );
			ITEMHANDLER->DeleteItemQuest( ITEMID_MagicRune, 5 );
			break;

		default:
			break;
	}
}

void CQuestGame::HandleT5QuestCancel( int iQuestID )
{
	if ( iQuestID == QUESTID_T5QuestArena )
	{
		T5QUEST_ACTIVE = QUESTT5ACTIVE_None;
	}
}

void CQuestGame::HandleAlertOnQuestDone( int iQuestID )
{
	char szTextTitle[256]	= { 0 };
	char szTextChat[256]	= { 0 };

	//T5 Quest
	if ( (iQuestID == QUESTID_AMatterOfStrenght) || (iQuestID == QUESTID_CryMeARiver) || (iQuestID == QUESTID_TheLastStage) ||
		(iQuestID == QUESTID_AnEndlessBattle) || (iQuestID == QUESTID_AnEndlessSuffering) || (iQuestID == QUESTID_ATestOfBravery) )
	{
		STRINGCOPY( szTextTitle, "You won 1 Skill Point!" );
		STRINGCOPY( szTextChat, "> You won 1 Skill Point!" );
	}

	if ( szTextTitle[0] != 0 )
		TITLEBOX( szTextTitle );
}

CQuestGame::CQuestGame()
{
	pcWindow = new CQuestWindow();
	pcQuest = new Quest();
}


CQuestGame::~CQuestGame()
{
	SAFE_DELETE( pcWindow );
	SAFE_DELETE( pcQuest );
}

BOOL CQuestGame::IsFinishing()
{
	return bFinishingQuest;
}

void CQuestGame::Init()
{
	pcWindow->Init();
}

void CQuestGame::SetQuestTimer( char * pszName, char * pszDescription, int iQuestID, int iTime, int iLevel )
{
	typedef DWORD( __cdecl *tfnCreateTexture )(const char * pszFile, int iUnk1, int iUnk2, int iUnk3, int iUnk4, int iBlend);
	tfnCreateTexture fnCreateTexture = (tfnCreateTexture)0x0048A140;
	typedef DWORD( __cdecl *tfnSetSkillBuffer )(Skill * psSkill);
	tfnSetSkillBuffer fnSetSkillBuffer = (tfnSetSkillBuffer)0x00520F60;

	if ( (*(DWORD*)(0x367DF40)) == 0 )
	{
		(*(DWORD*)(0x367DF40)) = fnCreateTexture( "image\\Sinimage\\skill\\keep\\QuestTimer.tga", 0, 0, 0, 0, 1 );
		CALL( 0x0048A260 );
	}


	Skill sTempSkill;

	ZeroMemory( &sTempSkill, sizeof( Skill ) );
	STRINGCOPY( sTempSkill.sSkillInfo.szName, pszName );
	STRINGCOPY( sTempSkill.sSkillInfo.szDescription, pszDescription );

	sTempSkill.iID			= iTime != 0 ? 0x096B0003 : 0x096A0003;
	sTempSkill.iUseTime		= iTime != 0 ? iTime : 70 * 70;
	sTempSkill.iLevel		= 1;
	sTempSkill.MatIcon		= (int)(*(DWORD*)(0x367DF40));
	sTempSkill.PlusState[4] = iQuestID;
	sTempSkill.sSkillInfo.iSkillLevelRequired = iLevel;
	fnSetSkillBuffer( &sTempSkill );

	PLAYMINISOUND( 25 );
}

int CQuestGame::OnRenderTimer( Skill * psSkillData )
{
	int iLineCount = 0;

	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
	{
		QuestData * pc = (*it);

		if ( pc && pc->GetID() == psSkillData->PlusState[4] )
		{
			if ( pc->GetMonstersReq( 0 ) > 0 )
			{
				for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
				{
					if ( pc->GetMonstersReq( i ) == 0 )
						break;

					if ( pc->GetMonstersReq( i ) > 0 )
					{
						SKILLMANAGERHANDLER->AddSkillInfo( 1, "%s:\r", pc->GetMonsterName( i ) );
						SKILLMANAGERHANDLER->AddSkillInfo( 2, "%d/%d\r", pc->GetMonsterKilledAmount( i ), pc->GetMonstersReq( i ) );
						iLineCount += 1;
					}
				}
			}

			if ( pc->GetItemsReq( 0 ) > 0 )
			{
				if ( iLineCount > 0 )
				{
					//Next Line
					SKILLMANAGERHANDLER->AddSkillInfo( 1, "\r" );
					SKILLMANAGERHANDLER->AddSkillInfo( 2, "\r" );
					iLineCount += 1;
				}

				for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
				{
					if ( pc->GetItemsReq( i ) == 0 )
						break;

					if ( pc->GetItemsReq( i ) > 0 )
					{
						SKILLMANAGERHANDLER->AddSkillInfo( 1, "%s:\r", pc->GetItemName( i ) );
						SKILLMANAGERHANDLER->AddSkillInfo( 2, "%d/%d\r", pc->GetItemsCollected( i ), pc->GetItemsReq( i ) );
						iLineCount += 1;
					}
				}
			}

			if ( pc->IsTimeQuest() && (pc->GetWaitTime() > 0) )
			{
				int iHour = abs( (int)pc->GetWaitTime() / 3600 );
				int iMinutes = abs( (int)pc->GetWaitTime() / 60 ) % 60;
				int iSeconds = abs( (int)pc->GetWaitTime() % 60 );

				SKILLMANAGERHANDLER->AddSkillInfo( 1, "Wait Time:\r" );
				SKILLMANAGERHANDLER->AddSkillInfo( 2, "%02d:%02d:%02d\r", iHour, iMinutes, iSeconds );
				iLineCount += 1;
			}
		}
	}

	return iLineCount;
}

EQuestPartyType CQuestGame::GetQuestPartyType( Skill * psSkill )
{
	EQuestPartyType iRet = EQuestPartyType::SoloPartyAndRaid;

	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
	{
		QuestData * pc = (*it);

		if ( pc && pc->GetID() == psSkill->PlusState[4] )
		{
			iRet = pc->GetQuestPartyType();
			break;
		}
	}

	return iRet;
}

/**
OnUnitKilled event is triggered when
a non-player unit is killed by this player.
This includes bosses
This is based on 'last hit'
*/
BOOL CQuestGame::OnUnitKilled( UnitData * pcUnitData, int iKillerID )
{
	BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
	BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;

	//exclude vamp bees
	if ( pcUnitData->sCharacterData.iUniqueMonsterID == VAMPBEE_MONSTER_ID )
		return FALSE;

	if ( MAP_ID == MAPID_Bellatra )
		return FALSE;

	if ( iKillerID == UNITDATA->iID )
	{
		//@todo - move this code below
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
		if ( psItemData
			&& psItemData->sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon
			&& psItemData->sItem.uQuestMonId > 0
			&& (  ( psItemData->sItem.uQuestMonId == pcUnitData->sCharacterData.iUniqueMonsterID )
			   || ( psItemData->sItem.uQuestMonId == QUEST_MONSTERID_ANY && pcUnitData->sCharacterData.iLevel >= psItemData->sItem.iLevel)) )
		{
			if ( psItemData->sItem.sMatureBar.sCur > 0 )
			{
				psItemData->sItem.sMatureBar.sCur--;

				if ( psItemData->sItem.sMatureBar.sCur <= 0 )
				{
					ITEMHANDLER->OnQuestWeaponMatured( psItemData );
				}
			}
		}
	}

	return pcQuest->HandleUnitKill( pcUnitData->sCharacterData.iUniqueMonsterID, pcUnitData->sCharacterData.iLevel, iKillerID,
		CHARACTERGAME->GetStageID(), pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, bParty, bRaid, FALSE, FALSE );
}

/**
OnUnitPvPKilled event is triggered when
a player unit is killed by this player
This is based on 'last hit'
*/
BOOL CQuestGame::OnUnitPvPKilled( UnitData * pcUnitData )
{
	if ( pcUnitData )
	{
		BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
		BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;

		return pcQuest->HandleUnitKill( pcUnitData->sCharacterData.iUniqueMonsterID, pcUnitData->sCharacterData.iLevel, UNITDATA->iID,
			CHARACTERGAME->GetStageID(), pcUnitData->sPosition.iX, pcUnitData->sPosition.iZ, bParty, bRaid, TRUE, FALSE );
	}
	return FALSE;
}

BOOL CQuestGame::OnItemCollect( int iItemID )
{
	return FALSE;
}

BOOL CQuestGame::Tick1s()
{
	pcQuest->CheckAllItemRequirements( );
	pcQuest->CheckAllOtherRequirements( );

	return TRUE;
}

void CQuestGame::HandlePacket( PacketQuestNPCOpen * psPacket )
{
	GetWindow()->Open( psPacket->iID, psPacket->szName, psPacket->szMessage, psPacket->iType, psPacket->iAutoItemRewardId );
}

void CQuestGame::HandlePacket( PacketQuestNPCFinish * psPacket )
{
	QuestData * pcQuestData = pcQuest->GetQuest( psPacket->iID );
	if ( pcQuestData )
	{
		TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );
		HandleDeleteItem( pcQuestData->GetID(), TRUE );
		pcQuest->DeleteQuest( pcQuestData->GetID() );

		if ( !pcQuestData->RemoveRequiredItemsFromInventory() )
		{
			SENDCOMMANDL( PacketsHeader::PKTHDR_MissingRequiredQuestItems, psPacket->iID );
		}
	}

	bFinishingQuest = FALSE;

#ifdef DEV_MODE
	//CHATBOX->AddDebugMessage( "[PacketQuestNPCFinish] bFinishingQuest: %d", bFinishingQuest );
#endif
}

void CQuestGame::HandlePacket( PacketQuestHandleCancel * psPacket )
{
	QuestData * pcQuestData = pcQuest->GetQuest( psPacket->iID );
	if ( pcQuestData )
	{
		TIMERSKILLHANDLER->KillTimerQuest( pcQuestData->IsTimeQuest() ? 0x096B0003 : 0x096A0003, pcQuestData->GetID() );
		HandleDeleteItem( pcQuestData->GetID(), FALSE );

		HandleT5QuestCancel( pcQuestData->GetID() );

		CQUESTGAME->GetQuestInstance()->DeleteQuest( pcQuestData->GetID() );
	}
}

void CQuestGame::HandlePacket( PacketQuestStartData * psPacket )
{
	QuestData * pcQuestData = new QuestData( psPacket );

	if ( pcQuestData->GetItemsReq( 0 ) > 0 )
	{
		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( pcQuestData->GetItemsReq( 0 ) == 0 )
				break;

			if ( pcQuestData->GetItemsReq( i ) > 0 )
			{
				ItemData * pc = ITEMHANDLER->FindItemTableByCode( pcQuestData->GetItemID( i ) );

				if ( pc )
					STRINGCOPYLEN( pcQuestData->GetItemName( i ), 32, pc->szBaseName );
			}
		}
	}

	DWORD dwTime = pcQuestData->IsTimeQuest() ? pcQuestData->GetTimeQuest() : 0;

	if ( dwTime == 0 && pcQuestData->IsTimeQuest() )
		dwTime = 1;

	//Timer
	SetQuestTimer( pcQuestData->GetName(), pcQuestData->GetShortDescription(), pcQuestData->GetID(), dwTime, pcQuestData->GetLevelMin() );

	pcQuestData->SetFunctionFinishHandle( &CQuestGame::HandleFinish );

	pcQuest->AddQuest( pcQuestData );

	pcQuestData->Update( TRUE );

	//T5 Quest?
	if ( pcQuestData->GetID() == QUESTID_T5QuestArena )
		T5QUEST_ACTIVE = QUESTT5ACTIVE_Active;

	//for newly accepted quest, set it as the current viewing quest
	if ( !psPacket->bInitialLoad )
		QUESTBOOKHANDLER->SetCurrentViewingQuestID( psPacket->iID );

	QUESTBOOKHANDLER->BuildQuestList(); //force rebuild
	QUESTBOOKHANDLER->RebuildCurrentQuestDataView();
}

void CQuestGame::HandlePacket( PacketQuestNPCItemList * psPacket )
{
	if ( psPacket->cItemData.bValid )
	{
		ItemData * pcItemData = ITEMHANDLER->FindItemTableByCode( psPacket->cItemData.sItem.sItemID.ToInt() );
		if ( pcItemData )
		{
			ItemData sNew;
			ITEMHANDLER->LoadItemImage( &psPacket->cItemData.sItem, &sNew );

			GetWindow()->AddItem( &sNew );
		}
	}

	if ( psPacket->bLast )
		GetWindow()->OpenItems( psPacket->iID );
}

void CQuestGame::HandlePacket( int iPacketHeader, PacketGenericContainer * psPacket)
{
	if (iPacketHeader == PacketsHeader::PKTHDR_QuestHandleFinishedContainer)
	{
		UINT uPosition = 0;

		for (int i = 0; i < psPacket->iCount; i++)
		{
			PacketHandleFinishedQuest * data = (PacketHandleFinishedQuest*)(psPacket->baBuffer + uPosition);
			HandlePacket(data);
			uPosition += sizeof(PacketHandleFinishedQuest);
		}
	}
}

void CQuestGame::HandlePacket( PacketHandleFinishedQuest * psPacket )
{
	GetQuestInstance()->AddFinishedQuest( psPacket );

	if ( psPacket->bLoginTime == FALSE )
	{
		HandleAlertOnQuestDone( psPacket->iID );
		QUESTBOOKHANDLER->BuildQuestList(); //force rebuild (for new finished quests)
	}

	ITEMHANDLER->HandleQuestFinish((EQuestID)psPacket->iID);

	if ( psPacket->eAsmQuestBit > 0 )
	{
		BOOL bUpdate = FALSE;

		switch ( psPacket->eAsmQuestBit )
		{
			case EAsmQuestBit::Level_30:	bUpdate = CQUESTGAME->CompleteAsmQuest_30_ForHer( );					break;
			case EAsmQuestBit::Level_55:	bUpdate = CQUESTGAME->CompleteAsmQuest_55_TheCave( );					break;
			case EAsmQuestBit::Level_70:	bUpdate = CQUESTGAME->CompleteAsmQuest_70_FriendshipWithMitchell( );	break;
			case EAsmQuestBit::Level_80:	bUpdate = CQUESTGAME->CompleteAsmQuest_80_SealedFury( );				break;
			case EAsmQuestBit::Level_80_2:	bUpdate = CQUESTGAME->CompleteAsmQuest_80_TestOfKingdom( );				break;
			case EAsmQuestBit::Level_85:	bUpdate = CQUESTGAME->CompleteAsmQuest_85_TearsOfKalia();				break;
			case EAsmQuestBit::Level_90:	bUpdate = CQUESTGAME->CompleteAsmQuest_90_EuraVillage();				break;
			case EAsmQuestBit::Level_90_2:	bUpdate = CQUESTGAME->CompleteAsmQuest_90_BitterOrdeal();				break;
			default:
				return;
		}

		if ( bUpdate )
		{
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;
			SAVEEX;
		}
	}

	for ( int j = 0; j < QUEST_MONSTERITEM_MAX; j++ )
	{
		if ( psPacket->iaExtraReward[j] == EQuestExtraRewardType::RankUp )
		{
			const int rankUpTier0Based = psPacket->iaExtraRewardValue[j];

			if ( rankUpTier0Based >= 0 && rankUpTier0Based <= 3 &&
				 rankUpTier0Based > PLAYER_TIER )
			{
				PLAYER_TIER = rankUpTier0Based;

				typedef void( __thiscall *t_SearchUseSkill ) ( UINT uSkillsPtr );
				t_SearchUseSkill SearchUseSkill = ( t_SearchUseSkill )0x004ED7F0;
				SearchUseSkill( 0x367E148 ); //skill object

				if ( psPacket->bLoginTime == FALSE )
				{
					//SKILL_INITIALIZATION_FLAG = 1; (enable if you want to show the 'reset skill' dialog, but requires re-training of all skills.., prob not a good idea?)
					CALL( 0x45CEE0 ); //ChangeJobFace() //render new crown

					typedef JOB_DATA_CODE*( __cdecl *t_GetJobDataCode ) ( DWORD BaseJob	, int JobCnt );
					t_GetJobDataCode GetJobDataCode = ( t_GetJobDataCode )0x0044D0F0;
					auto jobDataCode = GetJobDataCode( PLAYERDATA->iClass, PLAYER_TIER ); //skill object
					char * invJobName = (char *)0x3625694;

					STRINGCOPYLEN( invJobName, 32, jobDataCode->szName );

					SAVE;
				}
			}
		}
	}
}

void CQuestGame::HandlePacket( PacketQuestArenaTier5 * psPacket )
{
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_FinishQuest )
	{
		BOOL bParty = PARTYHANDLER->GetPartyData().iMembersCount > 0 ? TRUE : FALSE;
		BOOL bRaid = PARTYHANDLER->GetPartyData().iRaidCount > 0 ? TRUE : FALSE;

		pcQuest->HandleUnitKill( QUESTMONSTERID_CrystalTower, 0, 0, MAPID_T5QuestArena, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, bParty, bRaid, FALSE, FALSE );
	}
}

void CQuestGame::HandlePacket( PacketDynamicQuestOpen * psPacket )
{
	GetWindow()->ClearDynamicQuest();
	GetWindow()->SetDynamicQuestMain( psPacket->szImagePath, psPacket->szMainText );

	for ( int i = 0; i < psPacket->iQuestCount; i++ )
	{
		if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( psPacket->saQuestData[i].iQuestID ) )
		{
			GetWindow()->ClearDynamicQuest();
			TITLEBOX( "You have an active quest!" );
			return;
		}

		GetWindow()->AddDynamicQuestMainMenu( &psPacket->saQuestData[i] );
	}

	GetWindow()->OpenDynamicQuestWindow();
}

void CQuestGame::ProcessPacket( PacketQuestNPCAccept * psPacket )
{
	if ( dwAcceptQuestTime > TICKCOUNT )
		return;

	psPacket->iLength = sizeof( PacketQuestNPCAccept );
	psPacket->iHeader = PKTHDR_QuestNPCAccept;
	SENDPACKETL( psPacket, TRUE );

	dwAcceptQuestTime = TICKCOUNT + 2000;
}

void CQuestGame::CancelFinishQuest( )
{
	bFinishingQuest = FALSE;

#ifdef DEV_MODE
	//CHATBOX->AddDebugMessage( "[CancelFinishQuest] bFinishingQuest: %d", bFinishingQuest );
#endif
}

void CQuestGame::ProcessPacket( PacketQuestNPCFinish * psPacket )
{
	bFinishingQuest = TRUE;
#ifdef DEV_MODE
	//CHATBOX->AddDebugMessage( "[PacketQuestNPCFinish] bFinishingQuest: %d", bFinishingQuest );
#endif

	psPacket->iLength = sizeof( PacketQuestNPCFinish );
	psPacket->iHeader = PKTHDR_QuestNPCFinish;

	psPacket->sWeight[0] = UNITDATA->sCharacterData.sWeight.sCur;
	psPacket->sWeight[1] = UNITDATA->sCharacterData.sWeight.sMax;

	SENDPACKETL( psPacket, TRUE );
}

void CQuestGame::ProcessPacket( PacketDynamicQuestNPCOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketDynamicQuestNPCOpen );
	psPacket->iHeader = PKTHDR_DynamicQuestOpenNPC;
	SENDPACKETG( psPacket );
}

void CQuestGame::ProcessPacket( PacketQuestItemSwap * psPacket )
{
	psPacket->iLength = sizeof( PacketQuestItemSwap );
	psPacket->iHeader = PKTHDR_QuestItemSwap;
	SENDPACKETL( psPacket );
}

void CQuestGame::ProcessQuestCancel( int iQuestID )
{
	PacketQuestHandleCancel s;
	s.iLength = sizeof( PacketQuestHandleCancel );
	s.iHeader = PKTHDR_QuestCancel;
	s.iID = iQuestID;
	SENDPACKETL( &s, TRUE );
}

void CQuestGame::RemoveFinishQuest( int iQuestID )
{
	for ( auto questData : CQUESTGAME->GetQuestInstance()->GetFinishData() )
	{
		if ( questData && questData->iID == iQuestID )
		{
			CQUESTGAME->GetQuestInstance()->DeleteFinishQuest( questData->iID );
		}
	}

	QUESTBOOKHANDLER->BuildQuestList();

}

void CQuestGame::HandleCancelQuest( int iTimerID, int iQuestID )
{
	if ( MESSAGEBOX->GetType() == FALSE )
		return;
	else
	{
		QuestData * pc = CQUESTGAME->GetQuestInstance()->GetQuest( iQuestID );



		if ( pc )
		{
			if ( pc->GetMainQuestID() > 0 )
			{
				for ( auto questData : CQUESTGAME->GetQuestInstance()->GetFinishData() )
				{
					if ( questData && questData->sMainQuestID != 0 &&
						 questData->sMainQuestID == pc->GetMainQuestID() )
					{
						HandleDeleteItem( questData->iID, FALSE );
						CQUESTGAME->GetQuestInstance()->DeleteFinishQuest( questData->iID );
					}
				}
			}

			if ( iQuestID == QUESTID_T5QuestArena )
			{
				if ( MAP_ID == MAPID_T5QuestArena )
				{
					TITLEBOX( "You can't cancel this quest now!" );
					return;
				}
			}

			ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> %s cancelled!", pc->GetName() );
			HandleDeleteItem( iQuestID, FALSE );
			CQUESTGAME->ProcessQuestCancel( iQuestID );
			CQUESTGAME->GetQuestInstance()->DeleteQuest( iQuestID );
			TIMERSKILLHANDLER->KillTimerQuest( iTimerID, iQuestID );

			HandleT5QuestCancel( iQuestID );
		}



		QUESTBOOKHANDLER->BuildQuestList(); //force rebuild
	}
}

void CQuestGame::Update()
{
	if ( dwTimeUpdate == 0 )
		dwTimeUpdate = TICKCOUNT + 5000;

	for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); )
	{
		QuestData * pc = (*it);

		if ( pc )
		{
			pc->Update( 1.0f );

			if ( pc->IsTimeQuest() && pc->GetTimeQuest() == 0 )
			{
				CQuestGame::HandleTimeOver( pc );

				HandleDeleteItem( pc->GetID(), FALSE );

				ProcessQuestCancel( pc->GetID() );

				HandleT5QuestCancel( pc->GetID() );

				SAFE_DELETE( pc );

				it = pcQuest->GetData().erase( it );
			}
			else if ( pc->IsForceUpdate() )
			{
				PacketQuestUpdateData s;
				ZeroMemory( &s, sizeof( PacketQuestUpdateData ) );
				s.iLength = sizeof( PacketQuestUpdateData );
				s.iHeader = PKTHDR_QuestUpdateData;

				CopyMemory( s.saMonstersKilled, pc->GetMonsterKilledData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saItemsCollected, pc->GetItemsCollectData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saMonstersReq, pc->GetMonstersReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saItemsReq, pc->GetItemsReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

				s.iItemsCount = pc->GetItemCount();
				s.iMonsterCount = pc->GetMonsterCount();
				s.dwTimeLeft = pc->GetTimeQuest();
				s.iID = pc->GetID();
				s.bFinishedOtherRequirements = pc->IsOtherRequirementsFinished();

				SENDPACKETL( &s, TRUE );

				pc->ForceUpdate( FALSE );
				pc->Update( FALSE );
			}
			it++;
		}
		else
			it++;
	}

	if ( dwTimeUpdate <= TICKCOUNT )
	{
		if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Active )
		{
			if ( pcQuest->IsActiveQuest( QUESTID_T5QuestArena ) == FALSE )
				T5QUEST_ACTIVE = QUESTT5ACTIVE_None;
		}

		PacketQuestUpdateData s;
		ZeroMemory( &s, sizeof( PacketQuestUpdateData ) );
		s.iLength = sizeof( PacketQuestUpdateData );
		s.iHeader = PKTHDR_QuestUpdateData;


		for ( std::vector<QuestData*>::iterator it = pcQuest->GetData().begin(); it != pcQuest->GetData().end(); it++ )
		{
			QuestData * pc = (*it);

			if ( pc && pc->Update() )
			{
				CopyMemory( s.saMonstersKilled, pc->GetMonsterKilledData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saItemsCollected, pc->GetItemsCollectData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saMonstersReq, pc->GetMonstersReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );
				CopyMemory( s.saItemsReq, pc->GetItemsReqData(), sizeof( USHORT ) * QUEST_MONSTERITEM_MAX );

				s.iItemsCount = pc->GetItemCount();
				s.iMonsterCount = pc->GetMonsterCount();
				s.dwTimeLeft = pc->GetTimeQuest();
				s.iID = pc->GetID();
				s.bFinishedOtherRequirements = pc->IsOtherRequirementsFinished();

				SENDPACKETL( &s, TRUE );

				pc->Update( FALSE );
			}
		}

		dwTimeUpdate = TICKCOUNT + 10000;
	}
}

BOOL CQuestGame::OnQuestT5OpenProgress()
{
	if ( pcQuest->IsActiveQuest( QUESTID_T5QuestArena ) )
	{
		if ( pcQuest->IsFinishedQuestKills( QUESTID_T5QuestArena ) )
		{
			//Text
			if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Active )
			{
				T5QUEST_ACTIVE = QUESTT5ACTIVE_Finishing;

				GetWindow()->OpenT5QuestWindow( FALSE );
			}
			else
			{
				if ( T5QUEST_ACTIVE == QUESTT5ACTIVE_Finishing )
				{
					//Message Box Rankup
					MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
					MESSAGEBOX->SetTitle( "Rank Up" );
					MESSAGEBOX->SetDescription( "Do you wanna rank up?" );
					MESSAGEBOX->SetEvent( std::bind( &CQuestGame::OnMessageBoxT5Click, this, std::placeholders::_1 ) );
					MESSAGEBOX->Show();
				}
			}

			return TRUE;
		}
		else
		{
			//Message Box Rankup
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Teleport" );
			MESSAGEBOX->SetDescription( "Do you want to travel?" );
			MESSAGEBOX->SetEvent( std::bind( &CQuestGame::OnMessageBoxT5TeleportClick, this, std::placeholders::_1 ) );
			MESSAGEBOX->Show();
		}
	}

	return FALSE;
}

void CQuestGame::OnQuestT5Start()
{
	if( UNITDATA->sCharacterData.iRank == CHARACTERRANK_Rank4 && CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AMatterOfStrenght ) && CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_CryMeARiver ) &&
		CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_TheLastStage ) && CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_T5QuestArena ) == FALSE )
	{
		WRITEDWORD( 0x0362BA7C, 0 );
		WINDOW_ISOPEN_SKILLMASTER = FALSE;

		//Window
		GetWindow()->OpenT5QuestWindow( TRUE );

		WRITEDWORD( 0x0367E8AC, 1 );
	}
	else
	{
		TITLEBOX( "You can't get this quest now!" );
	}
}

BOOL CQuestGame::OnMessageBoxT5Click( UIEventArgs e )
{
	T5QUEST_ACTIVE = QUESTT5ACTIVE_Active;

	if ( MESSAGEBOX->GetType() )
	{
		UNITDATA->sCharacterData.iRank = CHARACTERRANK_Rank5;

		UPDATE_CHARACTER_CHECKSUM;
		SAVE;
		SAVEEX;

		GetWindow()->CloseT5QuestWindow();

		PacketQuestNPCFinish s;
		s.iID = QUESTID_T5QuestArena;
		CQUESTGAME->ProcessPacket( &s );

		T5QUEST_ACTIVE = QUESTT5ACTIVE_None;

		CHARACTERGAME->ResetHead();
		SKILLMANAGERHANDLER->SkillEffectLearn();
	}
	return TRUE;
}

BOOL CQuestGame::OnMessageBoxT5TeleportClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		PacketQuestArenaTier5 s;
		s.iLength = sizeof( PacketQuestArenaTier5 );
		s.iHeader = PKTHDR_T5QuestArena;
		s.iType = PacketQuestArenaTier5::EType::TYPEID_EnterArena;
		SENDPACKETG( &s );
	}
	return TRUE;
}

//int SendQuestCommandToServer( DWORD dwQuestCode , int Param1, int Param2, int Param3 )
void CQuestGame::SendQuestCommand( EAsmQuestCode eQuestCode, int Param1, int Param2, int Param3 )
{
	typedef void( __cdecl *t_SendQuestCommandToServer ) (SocketData* Socket, UINT Opcode, UINT Code, UINT Unk1, UINT Unk2, UINT Unk3);
	t_SendQuestCommandToServer SendQuestCommandToServer = (t_SendQuestCommandToServer)0x00627720;

	if ( SOCKETL != 0 )
		SendQuestCommandToServer( SOCKETL, PKTHDR_QuestCommand, eQuestCode, Param1, Param2, Param3 );
}


//int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog )
//The character state value is newly corrected
void CQuestGame::ReformCharacterStatPoint( )
{
	typedef void( __cdecl * t_ReformCharStatePoint ) ( UINT PlayerMemory, UINT Points );
	t_ReformCharStatePoint ReformCharStatePoint = (t_ReformCharStatePoint)0x00462900;
	ReformCharStatePoint( CHAR_DATA_PTR, PLAYER_QUESTLOG );
}

//int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog )
//The character state value is newly corrected
void CQuestGame::EndQuestCode( EAsmQuestCode dwQuestCode )
{
	typedef void( __cdecl *t_RecordLastQuest ) (EAsmQuestCode uCode);
	t_RecordLastQuest EndQuest_Code = (t_RecordLastQuest)0x004631C0;
	EndQuest_Code( dwQuestCode );
}


//int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog )
//The character state value is newly corrected
void CQuestGame::UpdateQuestBoard( )
{
	typedef void( __cdecl *t_SetQuestBoard ) ();
	t_SetQuestBoard Set_Quest_Board = (t_SetQuestBoard)0x0041A280;
	Set_Quest_Board( );
}

BOOL CQuestGame::CompleteAsmQuest_30_ForHer( )
{
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0444 ) = 3;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x043C ) = EAsmQuestCode::Level30;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0438 ) = 0;

	EndQuestCode( EAsmQuestCode::Level30 );
	SendQuestCommand( EAsmQuestCode::Level30, 0, 0, 0 );

	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_30 ) == 0 )
	{
		// For Her
		PLAYER_STATS_POINT += 5;
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_30;
		ReformCharacterStatPoint();

		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_55_TheCave()
{
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0110 ) = 3;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0108 ) = EAsmQuestCode::Level55;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0104 ) = 0;

	// The Cave
	EndQuestCode( EAsmQuestCode::Level55 );
	SendQuestCommand( EAsmQuestCode::Level55, 0, 0, 0 );

	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_55 ) == 0 )
	{
		PLAYER_SKILL_POINT += 1;
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_55;

		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_70_FriendshipWithMitchell()
{
	EndQuestCode( EAsmQuestCode::Level70 );
	SendQuestCommand( EAsmQuestCode::Level70, 0, 0, 0 );

	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0084 ) = 3;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x007C ) = EAsmQuestCode::Level70;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0078 ) = 0;

	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_70 ) == 0 )
	{
		PLAYER_SKILL_POINT += 1;
		PLAYER_STATS_POINT += 5;
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_70;

		ReformCharacterStatPoint();

		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_80_SealedFury()
{
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0470 ) = 3;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0468 ) = EAsmQuestCode::Level80;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0464 ) = 0;

	EndQuestCode( EAsmQuestCode::Level80 );
	SendQuestCommand( EAsmQuestCode::Level80, 0, 0, 0 );

	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_80 ) == 0 )
	{
		PLAYER_SKILL_POINT += 2;
		PLAYER_STATS_POINT += 5;
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_80;

		ReformCharacterStatPoint();

		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_80_TestOfKingdom()
{
	if ( PLAYERDATA->iLevel >= 80 )
	{
		*(UINT *)( QUESTCODE_BASE_ADDR + 0x00F4 ) = 5; //quest state
		*(UINT *)( QUESTCODE_BASE_ADDR + 0x00EC ) = EAsmQuestCode::Level80_2;
		*(UINT *)( QUESTCODE_BASE_ADDR + 0x00E8 ) = 0;

		EndQuestCode( EAsmQuestCode::Level80_2 );
		SendQuestCommand( EAsmQuestCode::Level80_2, 0, 0, 0 );

		ITEMHANDLER->DeleteItemQuest( ITEMID_QuestRecommendationLetter );

		if (( PLAYER_QUESTLOG & EAsmQuestBit::Level_80_2) == 0 )
		{
			PLAYER_STATS_POINT += ( PLAYERDATA->iLevel - 79 ) * 2;
			PLAYER_QUESTLOG |= EAsmQuestBit::Level_80_2;

			ReformCharacterStatPoint();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_85_TearsOfKalia()
{
	EndQuestCode( EAsmQuestCode::Level85 );

	*(UINT*)(QUESTCODE_BASE_ADDR + 0x0068) = 4;
	*(UINT*)(QUESTCODE_BASE_ADDR + 0x0060) = EAsmQuestCode::Level85;
	*(UINT*)(QUESTCODE_BASE_ADDR + 0x005C) = 0;


	const int iItemCount = ITEMHANDLER->GetItemAmountByCode( ITEMID_TearsOfKalia );
	if ( iItemCount == 0 || iItemCount > 1 ) // > 1 = if we have duplicates, send the command anyway then we'll tidy it up when receiving item again
	{
		SendQuestCommand( EAsmQuestCode::Level85, 0, 0, 0 ); //send this command to get the item
	}


	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_85 ) == 0 )
	{
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_85;
		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_90_EuraVillage()
{
	EndQuestCode( EAsmQuestCode::Level90 );

	*(UINT *)( QUESTCODE_BASE_ADDR + 0x000C ) = 3;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0004 ) = EAsmQuestCode::Level90;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x0000 ) = 0;

	const int iItemCount = ITEMHANDLER->GetItemAmountByCode( ITEMID_Roitra );
	if ( iItemCount == 0 || iItemCount > 1 ) // > 1 = if we have duplicates, send the command anyway then we'll tidy it up when receiving item again
	{
		SendQuestCommand( EAsmQuestCode::Level90, 0, 0, 0 ); //send this command to get the item
	}

	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_90 ) == 0 )
	{
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_90;
		return TRUE;
	}

	return FALSE;
}

BOOL CQuestGame::CompleteAsmQuest_90_BitterOrdeal()
{
	//requires test of kingdom quest first
	if ( ( PLAYER_QUESTLOG & EAsmQuestBit::Level_80_2 ) == 0 )
		return FALSE;

	if ( PLAYERDATA->iLevel < 90 )
		return FALSE;


	EndQuestCode( EAsmQuestCode::Level90_2 );
	SendQuestCommand( EAsmQuestCode::Level90_2, 0, 0, 0 );

	*(UINT *)( QUESTCODE_BASE_ADDR + 0x00D8 ) = 4;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x00D0 ) = EAsmQuestCode::Level90_2;
	*(UINT *)( QUESTCODE_BASE_ADDR + 0x00CC ) = 0;

	if ( (PLAYER_QUESTLOG & EAsmQuestBit::Level_90_2) == 0 )
	{
		PLAYER_QUESTLOG |= EAsmQuestBit::Level_90_2;
		PLAYER_STATS_POINT += ( PLAYERDATA->iLevel - 89 ) * 3;

		ReformCharacterStatPoint();

		return TRUE;
	}

	return FALSE;
}

void CQuestGame::SkipOriginalQuests( BOOL bChatMessage )
{
	CompleteAsmQuest_30_ForHer();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> For Her Skipped!" );

	CompleteAsmQuest_55_TheCave();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> The Cave Skipped!" );

	// Friendship Mitchell
	CompleteAsmQuest_70_FriendshipWithMitchell();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Friendship Mitchell Skipped!" );

	// Sealed Fury
	CompleteAsmQuest_80_SealedFury();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Sealed Fury Skipped!" );

	// Test of Kingdom
	CompleteAsmQuest_80_TestOfKingdom();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Test of Kingdom Skipped!" );

	// Tears Of Kalia
	CompleteAsmQuest_85_TearsOfKalia();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Tears Of Kalia Skipped!" );

	// Eura Village
	CompleteAsmQuest_90_EuraVillage();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Eura Village Skipped!" );

	// Bitter Ordeal
	CompleteAsmQuest_90_BitterOrdeal();
	if ( bChatMessage )
		CHATGAMEHANDLER->AddChatBoxText( "> Bitter Ordeal Skipped!" );

	UpdateQuestBoard(); //for old quest ui

	WRITEDWORD( 0x0362BA2C, 4 );
	CHECK_CHARACTER_CHECKSUM;
	SAVE;
}

BOOL CQuestGame::CanGetItem( PacketItem * psPacket, int iPacketHeader )
{
	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_MagicRune) && pcQuest->IsActiveQuest( QUESTID_GreenLiquidQuestStep1 ) )
		return TRUE;

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_PotionAgingQuestBee) && (pcQuest->IsActiveQuest( QUESTID_GreenLiquidQuestStep1 ) || pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep1 )) )
	{
		if ( pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) == FALSE )
			return TRUE;
	}

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_BlessOfKalia) && pcQuest->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) )
	{
		if ( pcQuest->IsFinishedQuestItems( QUESTID_GreenLiquidQuestStep2 ) )
			return TRUE;
	}

	if ( (psPacket->sItem.sItemID.ToItemID() == ITEMID_GoldBag) && (pcQuest->IsActiveQuest( QUESTID_AnAncientThreat ) || pcQuest->IsDoneQuest( QUESTID_AnAncientThreat )) )
	{
		return TRUE;
	}
	return (BOOL)CALL_WITH_ARG2( 0x00624DA0, (DWORD)psPacket, (DWORD)iPacketHeader );
}
