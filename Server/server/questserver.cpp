#include "stdafx.h"
#include "questserver.h"
#include "Logger.h"


QuestServer::QuestServer()
{
}


QuestServer::~QuestServer()
{
}

EQuestClassFlag QuestServer::ECharacterClassToQuestClassFlag( ECharacterClass eCharacterClass )
{
	switch ( eCharacterClass )
	{
		case ECharacterClass::CHARACTERCLASS_Fighter:			return EQuestClassFlag::Fighter;
		case ECharacterClass::CHARACTERCLASS_Mechanician:		return EQuestClassFlag::Mechanician;
		case ECharacterClass::CHARACTERCLASS_Archer:			return EQuestClassFlag::Archer;
		case ECharacterClass::CHARACTERCLASS_Pikeman:			return EQuestClassFlag::Pikeman;
		case ECharacterClass::CHARACTERCLASS_Atalanta:			return EQuestClassFlag::Atalanta;
		case ECharacterClass::CHARACTERCLASS_Knight:			return EQuestClassFlag::Knight;
		case ECharacterClass::CHARACTERCLASS_Magician:			return EQuestClassFlag::Magician;
		case ECharacterClass::CHARACTERCLASS_Priestess:			return EQuestClassFlag::Priestess;
		case ECharacterClass::CHARACTERCLASS_Assassin:			return EQuestClassFlag::Assassin;
		case ECharacterClass::CHARACTERCLASS_Shaman:			return EQuestClassFlag::Shaman;
	}

	return EQuestClassFlag::None;
}

EQuestClassFlag QuestServer::GetQuestClassFlagFromRaw( const char * szRaw )
{
	int iClassFlag = 0;

	//Class Restriction - Need this for Quest Mark restricting to Class. | Chaos
	if ( !IsNull( szRaw ) )
	{
		//Check if Class can do this Quest
		for ( auto const v : split( szRaw, ',' ) )
		{
			iClassFlag |= (int)ECharacterClassToQuestClassFlag( static_cast<ECharacterClass>( atoi( v.c_str() ) ) );
		}
	}

	return static_cast<EQuestClassFlag>( iClassFlag );
}

BOOL QuestServer::GetQuestWeapon(User * pcUser, Item * pcItem, int iRank )
{
	const std::string itemCode = GetCharacterTier3QuestItem( pcUser->pcUserData->sCharacterData.iClass );

	return GetQuestWeapon( pcUser, pcItem, itemCode, iRank );
}


BOOL QuestServer::GetQuestWeapon( User * pcUser, Item * pcItem, const std::string itemCode, int iRank )
{
	auto psDef = ITEMSERVER->FindQuestWeaponDefByCode( itemCode.c_str() );
	if ( psDef )
	{
		Item sItem;
		ZeroMemory( &sItem, sizeof( Item ) );

		ITEMSERVER->CreateItem( &sItem, psDef, EItemSource::QuestReward, pcUser->pcUserData->sCharacterData.iClass, 0, 0, EItemRarity::NONE );

		sItem.eCraftType = EItemCraftType::ITEMCRAFTTYPE_QuestWeapon;
		sItem.iItemUniqueID = UNIQUEID_QUEST_WEAPON;
		sItem.bDisallowDrop = TRUE;
		sItem.sAgeLevel = iRank;
		sItem.eRarity = (EItemRarity)iRank;
		sItem.sIntegrity.sCur = sItem.sIntegrity.sMax;
		SetQuestWeaponMonsterIDAndCount( &sItem, iRank + 1 );
		sItem.iLevel = psDef->sItem.iLevel;

		//iRank = 0 is for viewing only
		ITEMSERVER->ReformItemNew( &sItem );

		CopyMemory( pcItem, &sItem, sizeof( Item ) );

		return TRUE;
	}

	return FALSE;
}

/// <summary>
///
/// </summary>
/// <param name="pcUser">User object</param>
/// <param name="szItemCode">Item code</param>
/// <param name="pcItem">Pointer to item</param>
/// <param name="bViewMode">Item is intended for viewing only (i.e. Set to FALSE to make Item valid</param>
/// <returns></returns>
BOOL QuestServer::GetItemRewardFromCode( User * pcUser, DWORD dwItemID, Item * pcItem, BOOL bViewMode, int iQuestID )
{
	if ( GAME_SERVER )
	{
		WARN( "GetItemRewardFromCode should ideally be called on Login server to ensure rewards are consistent!" );
	}

	//Quest item Rewards are default to Common
	EItemRarity eItemRarity = EItemRarity::COMMON;

	//Unless the quest ID equals this it's Uncommon
	if ( iQuestID == 8023 || iQuestID == 9022 )
		eItemRarity = EItemRarity::UNCOMMON;

	pcItem->iChk1 = 0;
	pcItem->iChk2 = 0;

	DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( dwItemID );
	if ( psDef == NULL )
		return FALSE;

	if ( mQuestRewards.find( psDef->sItem.sItemID.ToInt() ) == mQuestRewards.end() )
	{
		Item sItem{};

		if ( ITEMSERVER->IsItemAbleToHaveRarity( &psDef->sItem ) )
		{
			//hack to ensure we get a spec by trying up to 5 times
			for ( int i = 0; i < 5; i++ )
			{
				ZeroMemory( &sItem, sizeof( Item ) );
				ITEMSERVER->CreateItem( &sItem, psDef, EItemSource::QuestReward, 100, 1, 0, eItemRarity ); //100 to ensure a spec (where valid) to get spec data

				if ( sItem.eSpecialization != EClassFlag::CLASSFLAG_None )
					break;
			}

			if ( sItem.eSpecialization == EClassFlag::CLASSFLAG_None )
			{
				WARN( "Unable to get item spec for a quest reward for item: %s", sItem.szItemName );
			}
		}
		else
		{
			ZeroMemory( &sItem, sizeof( Item ) );
			ITEMSERVER->CreateItem( &sItem, psDef, EItemSource::QuestReward, 0, 0, 0, EItemRarity::NONE );
		}

		mQuestRewards.insert( { psDef->sItem.sItemID.ToInt(), sItem } );
	}

	CopyMemory( pcItem, &mQuestRewards[psDef->sItem.sItemID.ToInt()], sizeof( Item ) );

	//if this item has a spec, then we can assume it's a equippable item
	if ( pcItem->eSpecialization != EClassFlag::CLASSFLAG_None )
	{
		auto eTargetClassFlag = EClassFlag::CLASSFLAG_None;
		auto eClassFlag = CharacterClassToClassFlag( pcUser->pcUserData->sCharacterData.iClass );

		//check main spec
		if ( psDef->sItem.eSpecialization == eClassFlag )
			eTargetClassFlag = eClassFlag;

		//check other specs
		for ( int j = 0; j < 12; j++ )
		{
			if ( psDef->JobBitCodeRandom[j] &&
				 psDef->JobBitCodeRandom[j] == eClassFlag)
			{
				eTargetClassFlag = eClassFlag;
				break;
			}
		}

		pcItem->eSpecialization = eTargetClassFlag;

		if ( pcItem->eSpecialization != EClassFlag::CLASSFLAG_None )
		{
			//only in non-viewing mode.
			if ( !bViewMode )
			{
				//Create item with matching spec
				//then copy over stats requirements.
				*(UINT *)0x8B70264 = 1;
				*(UINT *)0x8B70268 = pcUser->pcUserData->sCharacterData.iClass;
				Item sItemTemp;
				ITEMSERVER->CreateItem( &sItemTemp, psDef, EItemSource::QuestReward, pcUser->pcUserData->sCharacterData.iClass );
				*(UINT *)0x8B70264 = 0;
				*(UINT *)0x8B70268 = 0;

				pcItem->iStrength = sItemTemp.iStrength;
				pcItem->iTalent = sItemTemp.iTalent;
				pcItem->iSpirit = sItemTemp.iSpirit;
				pcItem->iHealth = sItemTemp.iHealth;
				pcItem->iAgility = sItemTemp.iAgility;

				pcItem->iItemSpecialType = 2; //quest reward
			}
		}
		else
		{
			//clear spec data
			ZeroMemory( &pcItem->sSpecData, sizeof( SpecItemData ) );
		}
	}

	pcItem->iChk1 = 0;
	pcItem->iChk2 = 0;
	pcItem->iBackupKey = 0;
	pcItem->iBackupChk = 0;
	pcItem->tTime = 0;

	if ( !bViewMode )
	{
		ITEMSERVER->ReformItem( pcItem );
	}

	return TRUE;
}

/// <summary>
/// Set quest monster id and count based on rank
/// </summary>
/// <param name="pcUser"></param>
/// <param name="sItem"></param>
/// <param name="iRank"></param>
/// <returns></returns>
BOOL QuestServer::SetQuestWeaponMonsterIDAndCount(Item * sItem, int iNextRank )
{
	if (iNextRank == 1 )
	{
		sItem->uQuestMonId = QUEST_MONSTERID_ANY;	 //Skeleton Ranger
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 30;
	}
	else if (iNextRank == 2 )
	{
		sItem->uQuestMonId = QUEST_MONSTERID_ANY;	 //Skeleton Warrior
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 40;
	}
	else if (iNextRank == 3 )
	{
		sItem->uQuestMonId = QUEST_MONSTERID_ANY;	 //Headcutter
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 50;
	}
	else if (iNextRank == 4 )
	{
		sItem->uQuestMonId = QUEST_MONSTERID_ANY;	//Any Monsters
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 100;
	}
	else if (iNextRank == QUESTWEAPON_MAX_AGE )
	{
		sItem->uQuestMonId = QUEST_MONSTERID_ANY;	//Any Monsters
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 300;
	}
	else //Final age doesn't have any monster id
	{
		sItem->uQuestMonId = 0;
	}

	if ( iNextRank >= 1 && iNextRank <= QUESTWEAPON_MAX_AGE )
	{
#ifdef DEV_MODE
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 2;
#endif
	}
	else
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 0;



	return TRUE;
}

// PacketAgingItem = sCRAFTITEM_SERVER *pCraftItem_Server
BOOL QuestServer::SetQuestWeaponRank ( Item * sItem, int iRank )
{
	if(iRank <= 0 || iRank >= 4)
		return FALSE;

	sItem->sAgeLevel = iRank;

	switch ( sItem->sItemID.ToItemType() )
	{
		case EItemType::ITEMTYPE_Axe:
		case EItemType::ITEMTYPE_Claw:
		case EItemType::ITEMTYPE_Scythe:
		case EItemType::ITEMTYPE_Dagger:
		case EItemType::ITEMTYPE_Sword:
		case EItemType::ITEMTYPE_Bow:
		case EItemType::ITEMTYPE_Javelin:
		{
			while (iRank-- > 0 )
			{
				sItem->fAddHP += 4.0f;
				sItem->sDamage.sMin += 2;
				sItem->sDamage.sMax += 2;
				sItem->iAttackRating += 15;
				sItem->eMixEffect = SIN_ADD_DAMAGE_MIN | SIN_ADD_DAMAGE_MAX | SIN_ADD_ATTACK_RATE | SIN_ADD_LIFE;
			}
			break;
		}
		case EItemType::ITEMTYPE_Wand:
		case EItemType::ITEMTYPE_Phantom:
		{
			while (iRank-- > 0 )
			{
				sItem->fAddHP += 4.0f;
				sItem->sDamage.sMin += 2;
				sItem->sDamage.sMax += 2;
				sItem->fMPRegen += 0.5f;
				sItem->eMixEffect = SIN_ADD_DAMAGE_MIN | SIN_ADD_DAMAGE_MAX | SIN_ADD_MANAREGEN | SIN_ADD_LIFE;
			}
			break;
		}
		default:
		{
			WARN( "[SetQuestWeaponAge] Unexpected item type: %d", sItem->sItemID.ToItemType() );
			return FALSE;
		}
	}

	return TRUE;
}

// PacketAgingItem = sCRAFTITEM_SERVER *pCraftItem_Server

BOOL QuestServer::HandleQuestWeaponUpgrade(User * pcUser, PacketQuestWeaponMature * sPacket )
{
	//int rsRecvWingItem(rsPLAYINFO *lpPlayInfo, sCRAFTITEM_SERVER *pCraftItem_Server)
	//tfnRecvAgingItem fnRecvAgingItem = (tfnRecvAgingItem)0x005690B0;
	//fnRecvAgingItem( pcUserData, psPacket );

	Item * sItem = &sPacket->sItem;

	int Temp = 0;
	float fTemp = 0;
	if(sItem->sAgeLevel > QUESTWEAPON_MAX_AGE)
		return FALSE; //can only be done up to the 5rd round

	if ( sItem->sMatureBar.sCur > 0 )
		return FALSE;

	int iOldAgeLevel = sItem->sAgeLevel;

	if ( sItem->sAgeLevel < QUESTWEAPON_MAX_AGE )
	{
		sItem->sAgeLevel++; //next age to achieve

		switch ( sItem->sItemID.ToItemType() )
		{
			case EItemType::ITEMTYPE_Axe:
			case EItemType::ITEMTYPE_Claw:
			case EItemType::ITEMTYPE_Scythe:
			case EItemType::ITEMTYPE_Dagger:
			case EItemType::ITEMTYPE_Sword:
			case EItemType::ITEMTYPE_Bow:
			case EItemType::ITEMTYPE_Javelin:
			{
				if ( sItem->sAgeLevel > 0 )
				{
					sItem->fAddHP += 4.0f;
					sItem->sDamage.sMin += 2;
					sItem->sDamage.sMax += 2;
					sItem->iAttackRating += 15;
					sItem->eMixEffect = SIN_ADD_DAMAGE_MIN | SIN_ADD_DAMAGE_MAX | SIN_ADD_ATTACK_RATE | SIN_ADD_LIFE;
				}
				break;
			}
			case EItemType::ITEMTYPE_Wand:
			case EItemType::ITEMTYPE_Phantom:
			{
				if ( sItem->sAgeLevel > 0 )
				{
					sItem->fAddHP += 4.0f;
					sItem->sDamage.sMin += 2;
					sItem->sDamage.sMax += 2;
					sItem->fMPRegen += 0.5f;
					sItem->eMixEffect = SIN_ADD_DAMAGE_MIN | SIN_ADD_DAMAGE_MAX | SIN_ADD_MANAREGEN | SIN_ADD_LIFE;
				}
				break;
			}
			default:
			{
				WARN( "Unexpected item type: %d", sItem->sItemID.ToItemType() );
				return FALSE;
			}
		}

		SetQuestWeaponMonsterIDAndCount( sItem, sItem->sAgeLevel + 1 );
		USERSERVER->SendUserMiscCommandToOtherNearbyPlayers ( pcUser, EUnitDataMiscCommand::COMMANDID_ShowSuccessAgingOrMixSkillAnimation );

		QUESTSERVER->HandleQuestMonsterNameRequest( pcUser, sItem->uQuestMonId ); //send monster name to client
	}
	else
	{
		sItem->sMatureBar.sCur = sItem->sMatureBar.sMax = 0;
		sItem->uQuestMonId = 0;
	}

	sItem->eRarity = (EItemRarity)sItem->sAgeLevel;

	ITEMSERVER->ReformItemNew(sItem, iOldAgeLevel >= 4 ? sItem->tTime : 0); //keep the original creation time
	ITEMSERVER->AddItemInventory( pcUser->pcUserData, sItem );
	ITEMSERVER->SendItemData( pcUser, sItem, TRUE );		//has special handling on client side for quest weapons

	if ( sItem->sAgeLevel < QUESTWEAPON_MAX_AGE )
		CHATSERVER->SendTitleBox( pcUser, "Your quest weapon has been upgraded to tier %d", sItem->sAgeLevel );
	else
		CHATSERVER->SendTitleBox( pcUser, "Your quest weapon has been fully upgraded!" );

	return TRUE;
}

struct TempData
{
	SYSTEMTIME sSysTime;
	int iDBID;
	int iQuestID;
	int iQuestType;

};

void QuestServer::LoadFinishedQuestsForUser( User * pcUser )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 12 );

	std::vector<TempData> vData;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT CQ.ID, CQ.QuestID, QL.QuestType, CQ.EndDate FROM CharacterQuest AS CQ INNER JOIN [GameDB].[dbo].[QuestList] AS QL ON QL.ID = CQ.QuestID  WHERE (CQ.CharacterID=?) AND CQ.EndDate IS NOT NULL ORDER BY CQ.QuestID, CQ.EndDate DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					TempData s;

					pcDB->GetData( 1, PARAMTYPE_Integer, &s.iDBID );
					pcDB->GetData( 2, PARAMTYPE_Integer, &s.iQuestID );
					pcDB->GetData( 3, PARAMTYPE_Integer, &s.iQuestType );
					pcDB->GetData( 4, PARAMTYPE_Time, &s.sSysTime );

					vData.push_back( s );
				}
			}
		}

		pcDB->Close();
	}

	int iLastQuestID = -1;

	for ( auto const & temp : vData )
	{
		//delete duplicate
		if ( temp.iQuestID == iLastQuestID )
		{
			SQLConnection * pcDB1 = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 16 );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "DELETE FROM CharacterQuest WHERE ID = ?" ) )
				{
					INFO( "Deleting dup quest: %d", temp.iQuestID );

					int iDBID = temp.iDBID;
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iDBID );
					pcDB->Execute();
				}

				pcDB->Close();
			}

			continue;
		}

		iLastQuestID = temp.iQuestID;

		UpdateAndSyncFinishedQuestUser( pcUser, temp.iQuestID, static_cast<EQuestType>( temp.iQuestType ), temp.sSysTime, TRUE );
	}
}


BOOL QuestServer::IsQuestExpired(  User * pcUser, int iQuestID )
{
	if ( LOGIN_SERVER )
	{
		LOGERROR( "IsQuestExpired 1 called on login server" );
		return FALSE;
	}

	auto it = pcUser->mFinishedQuestData->find( iQuestID );

	if ( it != pcUser->mFinishedQuestData->end() )
		return IsQuestExpired( it->second.eQuestType, it->second.sDateFinished );

	return TRUE;
}

BOOL QuestServer::IsQuestExpired(EQuestType eQuestType, const SYSTEMTIME & finishTime)
{
	SYSTEMTIME * current = GetServerTime();
	DWORD dwTime = abs( (long)SystemTimeToDWORD( current ) ) - (long)SystemTimeToDWORD( finishTime );

	switch ( eQuestType )
	{
		case EQuestType::SoloOneOff:			return FALSE; //never expires
		case EQuestType::PartyOneOff:			return FALSE; //never expires
		case EQuestType::Daily:					return dwTime >= ( 60 * 60 * 23 );
		case EQuestType::DailyMidNight:			return current->wDay != finishTime.wDay || current->wMonth != finishTime.wMonth;
		case EQuestType::DailyWeek:				return dwTime >= ( 60 * 60 * 23 * 6 ) + ( 60 * 60 * 23 );
		case EQuestType::DailyMonth:			return dwTime >= ( 60 * 60 * 24 * 29 ) + ( 60 * 60 * 23 );
		case EQuestType::Repeatable:			return dwTime >= 1; //can be repeated any time (1s added to prevent any race conditions)
		case EQuestType::Weekend:				return ( ( current->wDayOfWeek == 0 ) || ( current->wDayOfWeek == 6 ) ) && dwTime >= ( 60 * 60 * 24 * 4 );
		case EQuestType::WeekendDaily:			return ( ( current->wDayOfWeek == 0 ) || ( current->wDayOfWeek == 6 ) ) && dwTime >= ( 60 * 60 * 23 );
		case EQuestType::WeekendRepeatable:		return ( ( current->wDayOfWeek == 0 ) || ( current->wDayOfWeek == 6 ) );
		case EQuestType::WeekendDailyMidNight:	return ( ( current->wDayOfWeek == 0 ) || ( current->wDayOfWeek == 6 ) ) && finishTime.wDay != current->wDay;
	}

	return FALSE;
}

const std::string QuestServer::GetQuestWaitMessage( EQuestType eQuestType )
{
	switch ( eQuestType )
	{
		case EQuestType::Daily:					return "You need to wait 24 hours before doing this quest again!";
		case EQuestType::DailyMidNight:			return "You need to wait for the next day before doing this quest again!";
		case EQuestType::DailyWeek:				return "You need to wait one week before doing this quest again!";
		case EQuestType::DailyMonth:			return "You need to wait one month before doing this quest again!";
		case EQuestType::Weekend:				return "You need to wait next weekend before doing this quest again!";
		case EQuestType::WeekendDaily:			return "You need to wait 24 hours on this weekend before doing this quest again!";
		case EQuestType::WeekendRepeatable:		return "You can only do this quest during weekends";
		case EQuestType::WeekendDailyMidNight:	return "You need to wait next day on this weekend before doing this quest again!";
		case EQuestType::WeekFriday:			return "You need to wait next friday before doing this quest again!";
	}

	return "";
}

BOOL QuestServer::TryTakeAvaliableQuest( User * pcUser, int iQuestID, int iNPCID, BOOL bSendMessage )
{
	auto it = mQuests.find( iQuestID );
	if ( it == mQuests.end() )
	{
		WARN( "[TryTakeAvaliableQuest] Quest data not found for NPC %d for quest %d", iNPCID, iQuestID );
		return FALSE;
	}

	auto const sQuestData = &it->second;

	if ( pcUser->pcUserData->sCharacterData.iLevel < sQuestData->iMinLevel ||
		 pcUser->pcUserData->sCharacterData.iLevel > sQuestData->iMaxLevel )
	{
		if ( bSendMessage )
		{
			if ( sQuestData->iMaxLevel >= 150 || sQuestData->iMaxLevel == 0 )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You must be level %d to get this quest!", sQuestData->iMinLevel );
			else
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You must be level %d-%d to get this quest!", sQuestData->iMinLevel, sQuestData->iMaxLevel );
		}

		return FALSE;
	}


	if ( sQuestData->eClassFlag != EQuestClassFlag::None )
	{
		EQuestClassFlag eClassFlag = ECharacterClassToQuestClassFlag( pcUser->pcUserData->sCharacterData.iClass );
		if ( ( sQuestData->eClassFlag & eClassFlag ) == EQuestClassFlag::None )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "A %s cannot take this quest!", GetCharacterClassStringByClassEnum( pcUser->pcUserData->sCharacterData.iClass ) );


			return FALSE;
		}
	}

	if ( sQuestData->bMultiple == FALSE )
	{
		if ( IsUndergoingAnyQuest( pcUser ) )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You cannot take this quest while undergoing other quests!");

			return FALSE;
		}
	}
	else
	{
		if ( IsUndergoingLoneNonMultipleQuest( pcUser ) )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "You cannot take this quest while undergoing the lone quest!");

			return FALSE;
		}
	}

	if ( sQuestData->iMainQuestID > 0 )
	{
		if ( pcUser->GetQuestStatus(sQuestData->iMainQuestID) == EQuestUserStatus::Done )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "Main quest is already done!");

			return FALSE;
		}
	}


	for ( short sQuestID : sQuestData->sRequiredQuestIDs )
	{
		if ( pcUser->GetQuestStatus(sQuestID) != EQuestUserStatus::Done )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "A required quest is not finished yet!");

			return FALSE;
		}
	}

	for ( short sQuestID : sQuestData->sInclusiveQuestIDs )
	{
		if ( pcUser->GetQuestStatus(sQuestID) != EQuestUserStatus::InProgress )
		{
			if ( bSendMessage )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, "All required quests are not taken yet!");

			return FALSE;
		}
	}


	if ( iQuestID >= 8000 && iQuestID <= 9000 &&
		pcUser->pcUserData->sCharacterData.iLevel >= 50 )
	{
		return FALSE;
	}


	return TRUE; // All good!
}

BOOL QuestServer::IsDoneMainQuestID( User * pcUser, SimpleQuestData * psQuestData )
{
	return pcUser->GetQuestStatus( psQuestData->iMainQuestID ) == EQuestUserStatus::Done;
}

int QuestServer::IsCompletedRequiredQuestIDs( User * pcUser, SimpleQuestData * psQuestData )
{
	for ( short sQuestID : psQuestData->sRequiredQuestIDs )
	{
		if ( pcUser->GetQuestStatus(sQuestID) != EQuestUserStatus::Done )
		{
			return FALSE;
		}
	}

	return TRUE;
}

int QuestServer::GetNpcQuestGiverIDForQuest( int iQuestID )
{
	for ( auto const & it : mNPCQuests )
	{
		for ( auto const & questData : it.second )
		{
			if ( questData.iQuestID == iQuestID )
			{
				if ( questData.IsGiver() )
				{
					return it.first;
				}
			}
		}
	}

	return -1;
}

BOOL QuestServer::SQLDeleteQuest( User * pcUser, int iQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 15 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->Execute();
		}
		pcDB->Close();
	}

	return TRUE;
}


BOOL QuestServer::SQLCancelQuest( User * pcUser, int iQuestID, BOOL bAutoStartQuest )
{
	auto eUserQuestStatus = pcUser->GetQuestStatus( iQuestID );

	if ( eUserQuestStatus == EQuestUserStatus::InProgress || eUserQuestStatus == EQuestUserStatus::Completed )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 2 );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "DELETE FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}
			pcDB->Close();
		}

		if ( IsT5QuestProgress( iQuestID ) )
		{
			pcUser->uQuestT5ID = 0;
			pcUser->sQuestT5ProgressValue[0] = 0;
			pcUser->sQuestT5ProgressValue[1] = 0;
		}

		//Log
		char szQuestName[64] = { 0 };

		pcDB = SQLCONNECTION( DATABASEID_GameDB, 4);
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [Name] FROM QuestList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szQuestName, _countof( szQuestName ) );
				}
			}

			pcDB->Close();

			//Log It
			LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestCancel, "Character[%s] Cancelled Quest [%s]",
				CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szQuestName );
		}
	}

	NETSERVER->SendQuestCancelOrFinish( pcUser, iQuestID, FALSE );

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( (ps->iID != 0) && (ps->iID == iQuestID) )
		{
			ZeroMemory( ps, sizeof( PacketQuestUpdateData ) );
			break;
		}
	}

	return TRUE;
}

BOOL QuestServer::SQLFinishQuest( User * pcUser, int iQuestID )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 3 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterQuest SET EndDate=GETDATE(), Counter=Counter+1 WHERE (CharacterID=?) AND (QuestID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );
			pcDB->ExecuteUpdate();

			bRet = TRUE;
		}

		pcDB->Close();
	}

	return bRet;
}

int QuestServer::GetAutoStartQuestID( User * pcUser, int iQuestID )
{
	int iRet = 0;

	auto it = mQuests.find( iQuestID );

	if ( it == mQuests.end() )
		return 0;

	return it->second.iAutoStartQuestID;
}

std::vector<int> QuestServer::SQLGetActiveQuestsID( User * pcUser )
{
	std::vector<int> vQuests;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 4 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT QuestID FROM CharacterQuest WHERE (CharacterID=?) AND (EndDate IS NULL)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iQuestID = 0;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iQuestID );
					vQuests.push_back( iQuestID );
				}
			}
		}

		pcDB->Close();
	}

	return vQuests;
}


/// <summary>
/// Check if an account have finished a specific quest.
/// Some quests are ideally 'account-wide'. i.e. unlock warehouse page
/// NOte - QuestID should match ID in QuestList
/// </summary>
BOOL QuestServer::AccountHasFinishedQuest ( char * szAccountName, enum EQuestID eQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 5 );

	if ( pcDB->Open () )
	{
		int iCount = 0;
		int iQuestID = (int)eQuestID;

		if ( pcDB->Prepare ( "SELECT COUNT(*) FROM CharacterQuest WHERE QuestID=? AND EndDate IS NOT NULL AND AccountName=?" ) )
		{
			pcDB->BindParameterInput ( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput ( 2, PARAMTYPE_String, szAccountName, 32 );

			if ( pcDB->Execute () && pcDB->Fetch() )
			{
				pcDB->GetData ( 1, PARAMTYPE_Integer, &iCount );
			}
		}

		pcDB->Close ();

		if ( iCount >= 1 )
		{
			DEBUG ( "AccountHasFinishedQuest - %s = TRUE", szAccountName );

			return TRUE;
		}
	}

	DEBUG ( "AccountHasFinishedQuest - %s = FALSE", szAccountName );

	return FALSE;
}

BOOL QuestServer::HandleFinishedWarehouseQuestUser ( User * pcUser )
{
	if ( QUESTSERVER->AccountHasFinishedQuest ( pcUser->pcUserData->szAccountName, EQuestID::QUESTID_Warehouse2Page ) )
	{
		PacketHandleFinishedQuest sPacket;
		ZeroMemory( &sPacket, sizeof( PacketHandleFinishedQuest ) );

		sPacket.iLength = sizeof( PacketHandleFinishedQuest );
		sPacket.iHeader = PKTHDR_QuestHandleFinished;
		sPacket.bLoginTime = TRUE;
		sPacket.iID = EQuestID::QUESTID_Warehouse2Page;
		sPacket.eType = EQuestType::SoloOneOff;
		sPacket.sGiverNpcID = 90; //WH NPC
		sPacket.sMapID = EMapID::MAPID_PillaiTown;
		sPacket.eQuestPartyType = EQuestPartyType::SoloOnly;
		sPacket.iaExtraReward[0] = EQuestExtraRewardType::WarehousePage;
		sPacket.iaExtraRewardValue[0] = 1; //1 = page 2
		sPacket.iMinLevel = 40;
		sPacket.sGroupNum = 2;
		STRINGCOPY( sPacket.szQuestName, "Warehouse Keeper #1" );
		STRINGCOPY( sPacket.szQuestGroupName, "Side Quests" );

		CopyMemory ( &sPacket.sStartDate, GetServerTime (), sizeof ( SYSTEMTIME ) );
		CopyMemory ( &sPacket.sEndDate, GetServerTime (), sizeof ( SYSTEMTIME ) );

		SENDPACKET( pcUser, &sPacket, TRUE );
	}

	return TRUE;
}

BOOL QuestServer::SQLInsertFinishedASMQuest(User * pcUser, int iQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 13 );
	if ( pcDB->Open( ) )
	{
		if ( pcDB->Prepare( "INSERT INTO CharacterQuest([QuestID],[QuestRewardID],[CharacterID],[AccountName],[CharacterName],[StartDate],[EndDate],[MonsterCount],[ItemCount],[TimeLeft],[Counter]) VALUES (?,?,?,?,?,GETDATE(),GETDATE(),'','',0,1)", true ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUser->pcUserData->szAccountName, _countof( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}

	return TRUE;
}

BOOL QuestServer::SQLInsertFinisheRankUpQuest(User * pcUser, int iQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest_Sub, 2 );
	if ( pcDB->Open( ) )
	{
		if ( pcDB->Prepare( "INSERT INTO CharacterQuest([QuestID],[QuestRewardID],[CharacterID],[AccountName],[CharacterName],[StartDate],[EndDate],[MonsterCount],[ItemCount],[TimeLeft],[Counter]) VALUES (?,?,?,?,?,GETDATE(),GETDATE(),'','',0,1)", true ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUser->pcUserData->szAccountName, _countof( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 5, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );

			pcDB->Execute();
		}

		pcDB->Close();
	}

	return TRUE;
}

BOOL QuestServer::CheckAndInsertFinishedRankupQuestUser( User * pcUser, EQuestID eQuestID, ECharacterRace eCharacterRace, int iRankUpLevel0Based )
{
	auto eThisRace = GetCharacterRace( pcUser->pcUserData->sCharacterData.iClass );

	if ( eCharacterRace != eThisRace )
		return FALSE;

	int dwCharacterTier = (int)pcUser->pcUserData->sCharacterData.iRank;

	//DEBUGSTATUS( "dwCharacterTier: %d", dwCharacterTier );

	if ( dwCharacterTier >= iRankUpLevel0Based )
	{
		if ( pcUser->mFinishedQuestData->find( eQuestID ) == pcUser->mFinishedQuestData->end() )
		{
			SQLInsertFinisheRankUpQuest( pcUser, eQuestID );
			HandleFinishedQuestUser( pcUser, eQuestID, TRUE );

			return TRUE;
		}
	}

	return FALSE;
}


BOOL QuestServer::CheckAndInsertFinishedWarpGateQuestUser( User * pcUser, EQuestID eQuestID, EItemID eCurrWarpWingID, EItemID eWarpWingCheckID )
{
	int iCurrWarpWingTier = eCurrWarpWingID & 0x0000FFFF;
	int iCheckWarpWingTier = eWarpWingCheckID & 0x0000FFFF;

	if ( iCurrWarpWingTier >= iCheckWarpWingTier )
	{
		if ( pcUser->mFinishedQuestData->find( eQuestID ) == pcUser->mFinishedQuestData->end() )
		{
			DEBUG( "Force finish warpgate quest: %d", eQuestID );

			SQLInsertFinisheRankUpQuest( pcUser, eQuestID );
			HandleFinishedQuestUser( pcUser, eQuestID, TRUE, TRUE );

			return TRUE;
		}
	}

	return FALSE;
}

BOOL QuestServer::CheckAndInsertFinishedASMQuestUser( User * pcUser, EQuestID eQuestID, EAsmQuestBit eAsmQuestID )
{
	DWORD dwQuestLog = USERSERVER->GetQuestLogBit( pcUser->pcUserData );

	if ( (dwQuestLog & eAsmQuestID) != 0 )
	{
		if ( pcUser->mFinishedQuestData->find( eQuestID ) == pcUser->mFinishedQuestData->end() )
		{
			SQLInsertFinishedASMQuest( pcUser, eQuestID );
			HandleFinishedQuestUser( pcUser, eQuestID, TRUE );

			return TRUE;
		}
	}

	return FALSE;
}

BOOL QuestServer::HandleFinishedASMQuestUser( User * pcUser )
{
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_30_ForHer,				EAsmQuestBit::Level_30 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_55_TheCave,				EAsmQuestBit::Level_55 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_70_FriendShipWithMichell, EAsmQuestBit::Level_70 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_80_SealedFury,			EAsmQuestBit::Level_80 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_80_TestOfKingdom_5,		EAsmQuestBit::Level_80_2 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_85_TearsOfKalia,		    EAsmQuestBit::Level_85 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_90_EuraVillage,			EAsmQuestBit::Level_90 );
	CheckAndInsertFinishedASMQuestUser( pcUser, EQuestID::QUESTID_ASM_90_BitterOrdeal_3,		EAsmQuestBit::Level_90_2 );

	return TRUE;
}

BOOL QuestServer::HandleFinishedRankupQuestUser( User * pcUser )
{
	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_2_Tempskron,	ECharacterRace::CHARACTERRACE_Tempskron, 1 );
	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_3_Tempskron,	ECharacterRace::CHARACTERRACE_Tempskron, 2 );
	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_4_Tempskron,	ECharacterRace::CHARACTERRACE_Tempskron, 3 );

	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_2_Morion_4,		ECharacterRace::CHARACTERRACE_Morion, 1 );
	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_3_Morion,		ECharacterRace::CHARACTERRACE_Morion, 2 );
	CheckAndInsertFinishedRankupQuestUser( pcUser, EQuestID::QUESTID_Rankup_4_Morion,		ECharacterRace::CHARACTERRACE_Morion, 3 );

	return TRUE;
}

BOOL QuestServer::HandleFinishedWarpGateQuestUser( User * pcUser )
{
	EItemID eItemID = ITEMSERVER->GetWarpGateWing(pcUser);

	if ( ( (ItemID)eItemID ).ToItemType() != EItemType::ITEMTYPE_Wing )
		return FALSE;

	//warp wing special case

	for ( const auto & quest : mQuests )
	{
		//warp wing quests have id range of 2100-2199
		//skip other quests so that we don't mess up the flow
		//for instruction quest chain
		if ( quest.first < 2100 || quest.first > 2199 )
			continue;

		if ( quest.second.sItemRewardsIDs.size() == 0 )
			continue;

		for ( auto itemID : quest.second.sItemRewardsIDs )
		{
			if ( itemID.ToItemType() == EItemType::ITEMTYPE_Wing )
			{
				CheckAndInsertFinishedWarpGateQuestUser( pcUser, (EQuestID)quest.first, eItemID, itemID.ToItemID() );
			}
		}
	}

	return TRUE;
}

BOOL QuestServer::CheckAndAutoFinishOtherQuests( User * pcUser, int iFinishedQuestID)
{
	if ( GAME_SERVER )
		return FALSE;

	auto it = mQuests.find( iFinishedQuestID );

	if ( it == mQuests.end() )
		return FALSE;

	const auto & questData = it->second;

	//warp wing special case

	for ( auto itemID : questData.sItemRewardsIDs )
	{
		if ( itemID.ToItemType() == EItemType::ITEMTYPE_Wing )
		{
			for ( const auto &  checkQuest : mQuests )
			{
				//warp wing quests have id range of 2100-2199
				//skip other quests so that we don't mess up the flow
				//for instruction quest chain
				if ( checkQuest.first < 2100 || checkQuest.first > 2199 )
					continue;

				if ( checkQuest.first == iFinishedQuestID )
					continue;

				for ( auto itemCheckID : checkQuest.second.sItemRewardsIDs )
				{
					if ( itemCheckID.ToItemType() != EItemType::ITEMTYPE_Wing )
						continue;

					CheckAndInsertFinishedWarpGateQuestUser( pcUser, (EQuestID)checkQuest.first, itemID.ToItemID(), itemCheckID.ToItemID() );
				}
			}
		}
	}

	return TRUE;
}

BOOL QuestServer::UpdateAndSyncFinishedQuestUser( User * pcUser, int iQuestID, EQuestType eQuestType, const SYSTEMTIME & sSystemTime, BOOL bLoadStage )
{
	if ( iQuestID == 0 )
		return FALSE;

	auto it = pcUser->mFinishedQuestData->find( iQuestID );

	if ( it == pcUser->mFinishedQuestData->end() )
	{
		FinishedQuestData finishedData{};
		finishedData.eQuestType = (EQuestType)eQuestType;
		CopyMemory( &finishedData.sDateFinished, &sSystemTime, sizeof( SYSTEMTIME ) );

		//DEBUGSTATUS( "UpdateAndSyncFinishedQuestUser: quest id %d", iQuestID );

		pcUser->mFinishedQuestData->insert( {iQuestID, finishedData} );

		if (!bLoadStage )
			CheckAndAutoFinishOtherQuests(pcUser, iQuestID);

		if ( LOGIN_SERVER )
		{
			//DEBUGSTATUS( "Add finished quest and sync'ing: %d", iQuestID );

			//sync with game server
			PacketSyncFinishedQuestData syncData{};
			syncData.iHeader = PKTHDR_NetSyncFinishedQuestData;
			syncData.iLength = sizeof( PacketSyncFinishedQuestData );
			syncData.dwUserID = pcUser->GetID();
			syncData.iQuestID = iQuestID;
			syncData.eQuestType = (EQuestType)eQuestType;
			CopyMemory( &syncData.sDateFinished, &sSystemTime, sizeof( SYSTEMTIME ) );
			NETSERVER->SendPacketGameServers( &syncData );
		}
	}
	else if (!bLoadStage )
	{
		//use most recent finished date
		if ( abs( (long)SystemTimeToDWORD( sSystemTime ) ) > abs( (long)SystemTimeToDWORD( it->second.sDateFinished ) ))
		{
			it->second.eQuestType = (EQuestType)eQuestType;
			CopyMemory( &it->second.sDateFinished, &sSystemTime, sizeof( SYSTEMTIME ) );

			if ( LOGIN_SERVER )
			{
				//sync with game server
				PacketSyncFinishedQuestData syncData{};
				syncData.iHeader = PKTHDR_NetSyncFinishedQuestData;
				syncData.iLength = sizeof( PacketSyncFinishedQuestData );
				syncData.dwUserID = pcUser->GetID();
				syncData.iQuestID = iQuestID;
				syncData.eQuestType = (EQuestType)eQuestType;
				CopyMemory( &syncData.sDateFinished, &sSystemTime, sizeof( SYSTEMTIME ) );

				NETSERVER->SendPacketGameServers( &syncData );
			}
		}
	}

	return TRUE;
}

BOOL QuestServer::HandleQuestCommand( User * pcUser, PacketTransCommand * lpTransCommand )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return FALSE;


	switch (lpTransCommand->WParam)
	{
		case SIN_QUEST_CODE_LEVEL85:
		{
			//Send item specific user's inventory
			if ( pcUser->GetCharacterLevel() >= 85 )
			{
				ITEMSERVER->SendItemData( pcUser, EItemID::ITEMID_TearsOfKalia, EItemSource::QuestReward );
			}
		}
		break;

		case SIN_QUEST_CODE_LEVEL90:
		{
			if ( pcUser->GetCharacterLevel() >= 90 )
			{
				ITEMSERVER->SendItemData( pcUser, EItemID::ITEMID_Roitra, EItemSource::QuestReward );
			}
		}
		break;

		//tier 4
		case SIN_QUEST_CODE_CHANGEJOB4:
		{
			if ( pcUser->GetCharacterLevel() >= 60 && lpTransCommand->LParam == QUEST_ARENA_FIELD )
			{
				//Quest Master Battle Room Call
				OpenMonsterQuestArea( pcUser, SIN_QUEST_CODE_CHANGEJOB4 );
			}
		}
		break;

		case HAQUEST_CODE_FURYOFPHANTOM:
		{
			if ( pcUser->GetCharacterLevel() >= 99 && lpTransCommand->LParam == QUEST_ARENA_FIELD )
			{
				//Quest Master Battle Room Call
				OpenMonsterQuestArea(  pcUser, HAQUEST_CODE_FURYOFPHANTOM, lpTransCommand->SParam );
			}
			break;
		}
	}

	return TRUE;
}

BOOL QuestServer::OpenMonsterQuestArea( User * pcUser, DWORD dwQuestCode, int Param )
{
	UserData * lpPlayInfo = pcUser->pcUserData;
	Map	* lpStgArea;
	MonsterMapSpawnHandle StgCharInfo;
	int	len, pow;
	UnitData	* lpChar;
	EMonsterEffectID dwSndCode;
	int		OpenLevel = 0;

	switch (dwQuestCode)
	{
		case SIN_QUEST_CODE_CHANGEJOB4:
		{
			if ( lpPlayInfo->sCharacterData.iClass <= 4 || lpPlayInfo->sCharacterData.iClass == 9 )
				dwSndCode = EMonsterEffectID::MONSTEREFFECTID_NPC_SKILLMASTER;
			else
				dwSndCode = EMonsterEffectID::MONSTEREFFECTID_NPC_SKILLMASTER;
			break;
		}

		case HAQUEST_CODE_FURYOFPHANTOM:
		{
			dwSndCode = EMonsterEffectID::MONSTEREFFECTID_FURY;
			switch ( Param )
			{
				case 1:
					OpenLevel = 118; break;
				case 2:
					OpenLevel = 119; break;
				case 3:
					OpenLevel = 120; break;
			}
		}
	}

	lpStgArea = MAPSDATA + QUEST_ARENA_FIELD;

	if (lpStgArea)
	{
		//Remove any mobs that have already been summoned.
		for ( int i = 0; i < MAX_UNITS; i++ )
		{
			UnitData * pc = UNITSDATA + i;

			if ( pc->bActive &&
				pc->pcExclusiveTarget == lpPlayInfo &&
				pc->lExclusiveTargetID == lpPlayInfo->iID )
			{
				pc->Free();
				lpStgArea->iNumAliveUnitData--;

				MAPSERVER->DelMonsterSpawn( pc->pcMap, pc );
			}
		}

		len = 0;

		StgCharInfo.iPositionState = smCHAR_STATE_MONSTER;

		auto charData = UNITSERVER->GetCharacterDataByEffect( dwSndCode, EMonsterClass::Normal, OpenLevel );

		if ( charData == NULL )
		{
			LOGERROR( "Quest monster not found for monster effect id %d", dwSndCode );
			return FALSE;
		}

		CopyMemory( &StgCharInfo.sCharacterData, charData, sizeof( CharacterData ) );

		StgCharInfo.sCharacterData.sHP.sCur = 100;
		StgCharInfo.sCharacterData.sHP.sMax = 100;
		StgCharInfo.sCharacterData.sMP.sCur = 100;
		StgCharInfo.sCharacterData.iType = ECharacterType::CHARACTERTYPE_Monster;


		lpStgArea->bForceCreateMonster = TRUE;

		lpChar = MAPSERVER->CreateUnit( lpStgArea, &StgCharInfo, 0 );
		if (lpChar)
		{
			lpChar->SetPosition(QUEST_ARENA_NPC_POS_X, 0, QUEST_ARENA_NPC_POS_Z, 0, 0, 0);
			lpChar->iLureDistance = 0;
			lpChar->Animate(EAnimationType::ANIMATIONTYPE_Idle);
			lpChar->iTargetMoveCount = 0;
			lpChar->pcExclusiveTarget = lpPlayInfo;
			lpChar->lExclusiveTargetID = lpPlayInfo->iID;
			lpChar->dwUpdateCharInfoTime = TICKCOUNT + 30 * 60 * 1000;						//Removed after 30 minutes

			pow = lpPlayInfo->sCharacterData.iLevel - 60;
			if (pow < 0) pow = 0;
			pow += pow / 2;

			int hp = UNITSERVER->GetMonsterHealthPointDefinition(lpChar->sUnitInfo.iUniqueMonsterID);
			hp += (1000 * pow) / 100;

			lpChar->InitHealthPoint( hp );

			lpChar->sCharacterData.iMinDamage += ( lpChar->sCharacterData.iMinDamage * pow ) / 100;
			lpChar->sCharacterData.iMaxDamage += ( lpChar->sCharacterData.iMinDamage * pow ) / 100;

			lpChar->sUnitInfo.sSkillPower.sMin += ( lpChar->sUnitInfo.sSkillPower.sMin * pow ) / 100;
			lpChar->sUnitInfo.sSkillPower.sMax += ( lpChar->sUnitInfo.sSkillPower.sMin * pow ) / 100;

			lpChar->sUnitInfo.iMaxPotions = 3;		//Add 3 more potions
			lpChar->sUnitInfo.iPerPotions = 50;     //50% chance to use a potion

			return TRUE;
		}
	}

	return FALSE;
}

BOOL QuestServer::HandleFinishedQuestUser( User * pcUser, int iQuestID, BOOL bLoginTime, BOOL bForceFinish )
{
	//DEBUGSTATUS( "HandleFinishedQuestUser - QuestID: %d, bLoginTime: %d, bForceFinish: %d", iQuestID, bLoginTime, bForceFinish );

	SQLConnection * pcDB = NULL;

	if (bForceFinish)
		pcDB =  SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest_Sub, 1 ); //to prevent lock
	else
		pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 6 );

	PacketGenericContainer sPacketContainer;
	ZeroMemory(&sPacketContainer, sizeof(PacketGenericContainer));
	sPacketContainer.iHeader = PKTHDR_QuestHandleFinishedContainer;

	const int minSizeRequired = sizeof(PacketHandleFinishedQuest) + sizeof(Packet) + sizeof(int) + sizeof(int);


	if ( iQuestID != 0 )
	{
		if ( IsT5QuestProgress( iQuestID ) )
		{
			pcUser->uQuestT5ID					= 0;
			pcUser->sQuestT5ProgressValue[0]	= 0;
			pcUser->sQuestT5ProgressValue[1]	= 0;
		}
	}


	if ( pcDB->Open() )
	{
		std::string pszQuery = "";

		if ( iQuestID != 0 )
		{
			pszQuery = "SELECT QuestID, StartDate, EndDate"
				" FROM CharacterQuest"
				" WHERE (CharacterID=?) AND (QuestID=?) AND (EndDate IS NOT NULL)";
		}
		else
		{
			pszQuery = "SELECT QuestID, StartDate, EndDate"
				" FROM CharacterQuest"
				" WHERE ( CharacterID=? )"
				" AND ( EndDate IS NOT NULL )"
				" AND ( QuestID != 1501)" //skip warehouse quest. handled in HandleFinishedWarehouseQuestUser
				" AND ( ( QuestID < 3000 OR QuestID > 7000 ) OR ( QuestID >= 4000 AND QuestID <= 5999 ) OR"
				      " ( ( ( QuestID >= 3000 AND QuestID <= 3999 ) OR ( QuestID >= 6000 AND QuestID <= 6999 ) OR ( QuestID >= 10000 AND QuestID <= 10999 ) ) AND EndDate >= DATEADD(day,-7,GETDATE() ) ) )"
				" ORDER By QuestID DESC";
		}

		if ( pcDB->Prepare( pszQuery.c_str() ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( iQuestID != 0 )
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					PacketHandleFinishedQuest sPacket;
					ZeroMemory(&sPacket, sizeof(PacketHandleFinishedQuest));

					sPacket.bLoginTime = bLoginTime;

					pcDB->GetData( 1, PARAMTYPE_Integer, &sPacket.iID );
					pcDB->GetData( 2, PARAMTYPE_Time,	 &sPacket.sStartDate );
					pcDB->GetData( 3, PARAMTYPE_Time,	 &sPacket.sEndDate );

					auto it = mQuests.find( sPacket.iID );
					if ( it == mQuests.end() )
						continue;

					const auto & questData = it->second;

					if ( questData.iMainQuestID > 0 &&
						questData.iMainQuestID != sPacket.iID &&
						 pcUser->GetQuestStatus( questData.iMainQuestID ) == EQuestUserStatus::Done )
					{
						continue;
					}

					sPacket.eType = questData.eType;
					sPacket.eQuestPartyType = questData.ePartyType;
					sPacket.iMinLevel = questData.iMinLevel;
					sPacket.sGiverNpcID = questData.iGiverNpcID;
					sPacket.sMapID = questData.iMapID;
					sPacket.sMainQuestID = questData.iMainQuestID;
					sPacket.sGroupNum = questData.iQuestBookGroupRank;
					sPacket.eAsmQuestBit = questData.iASMQuestBit;

					STRINGCOPY( sPacket.szQuestName, questData.szQuestName );
					STRINGCOPY( sPacket.szQuestGroupName, questData.szQuestBookGroupName );

					for ( UINT i = 0; i < questData.eExtraRewardsTypes.size() && i < QUEST_MONSTERITEM_MAX; i++ )
					{
						sPacket.iaExtraReward[i]	  = questData.eExtraRewardsTypes[i];
						sPacket.iaExtraRewardValue[i] = questData.uExtraRewardsValues[i];
					};

					UpdateAndSyncFinishedQuestUser( pcUser, sPacket.iID, questData.eType, sPacket.sEndDate, bLoginTime );

					if ( iQuestID != 0 )
					{
						if ( !bLoginTime )
						{
							for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
							{
								PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

								if ( ( ps->iID != 0 ) && ( ps->iID == iQuestID ) )
								{
									ZeroMemory( ps, sizeof( PacketQuestUpdateData ) );
									break;
								}
							}
						}

						NETSERVER->SendQuestCancelOrFinish( pcUser, iQuestID, bForceFinish );
					}


					CopyMemory(sPacketContainer.baBuffer + (sPacketContainer.iCount * sizeof(PacketHandleFinishedQuest)), &sPacket, sizeof(PacketHandleFinishedQuest));
					sPacketContainer.iCount++;
					sPacketContainer.iLength = (sPacketContainer.iCount * sizeof(PacketHandleFinishedQuest)) + sizeof(Packet) + sizeof(int) + sizeof(int);

					//reaching max packet size, send this container and continue
					if (sPacketContainer.iLength + minSizeRequired > MAX_PKTSIZ)
					{
						SENDPACKET(pcUser, &sPacketContainer, TRUE);
						ZeroMemory(&sPacketContainer, sizeof(PacketGenericContainer));
						sPacketContainer.iHeader = PKTHDR_QuestHandleFinishedContainer;
					}

					if ( iQuestID != 0 )
						break;
				}
			}
		}

		pcDB->Close();
	}


	if (sPacketContainer.iCount > 0)
	{
		sPacketContainer.iParam = 1; //last
		SENDPACKET(pcUser, &sPacketContainer, TRUE);
	}

	return TRUE;
}

//game server
void QuestServer::RemoveExpiredQuestFinishData( User * pcUser )
{
	if ( LOGIN_SERVER )
	{
		LOGERROR( "RemoveExpiredQuestFinishData called on login server" );
		return;
	}

	for ( auto it = pcUser->mFinishedQuestData->begin(); it != pcUser->mFinishedQuestData->end();)
	{
		if ( IsQuestExpired( it->second.eQuestType, it->second.sDateFinished ) )
		{
			const int iQuestID = it->first;

			DEBUG( "Quest has expired: %d", iQuestID );

			UpdateNPCQuestIconForUser( pcUser, iQuestID );
			NETSERVER->SendCommandToLoginServer( PacketsHeader::PKTHDR_NetRemoveFinishedQuestData, pcUser->GetID(), iQuestID );
			SENDPACKETCOMMAND( pcUser, PKTHDR_RemoveFinishedQuest, iQuestID );

			it = pcUser->mFinishedQuestData->erase(it);
		}
		else
		{
			it++;
		}
	}
}

void QuestServer::RemoveQuestFinishData( User * pcUser, int iQuestID, BOOL bSyncWithOtherServer )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return;

	for ( auto it = pcUser->mFinishedQuestData->begin(); it != pcUser->mFinishedQuestData->end(); it++)
	{
		if ( it->first == iQuestID )
		{
			if ( GAME_SERVER )
			{
				UpdateNPCQuestIconForUser( pcUser, it->first );
			}

			it = pcUser->mFinishedQuestData->erase( it );
			break;
		}
	}

	if ( bSyncWithOtherServer )
	{
		if ( LOGIN_SERVER )
		{
			NETSERVER->SendCommandGameServers( PacketsHeader::PKTHDR_NetRemoveFinishedQuestData, pcUser->GetID(), iQuestID );
		}
		else
		{
			NETSERVER->SendCommandToLoginServer( PacketsHeader::PKTHDR_NetRemoveFinishedQuestData, pcUser->GetID(), iQuestID );
		}
	}
}

void QuestServer::UpdateNPCQuestIconForUser( User * pcUser, int iQuestID )
{
	if ( LOGIN_SERVER )
	{
		LOGERROR( "UpdateNPCQuestIconForUser called on login server" );
		return;
	}

	for ( auto const & it : mNPCQuests )
	{
		for ( auto const & questData : it.second )
		{
			if ( questData.iQuestID == iQuestID )
			{
				SendNpcQuestStatusIcon( pcUser, it.first, TRUE, iQuestID );
				break; //continue to next npc
			}
		}
	}
}

void QuestServer::UpdateNPCQuestIconForUserPostLevelUp( User * pcUser )
{
	if ( LOGIN_SERVER )
	{
		LOGERROR( "UpdateNPCQuestIconForUserPostLevelUp called on login server" );
		return;
	}

	for ( auto const & it : mNPCQuests )
	{
		for ( auto const & questData : it.second )
		{
			auto quest = mQuests.find( questData.iQuestID );
			if ( quest != mQuests.end() )
			{
				//only send updated quest icon if the new level did unlock new status
				if ( ( pcUser->pcUserData->sCharacterData.iLevel )     >= (quest->second.iMinLevel    ) &&
					 ( pcUser->pcUserData->sCharacterData.iLevel )     <= (quest->second.iMinLevel + 5) )
				{
					//continue to next npc
					SendNpcQuestStatusIcon( pcUser, it.first, TRUE, -1);
					break;
				}
			}
		}
	}
}


int QuestServer::SQLGetQuestTimeLeft( User * pcUser, int iQuestID, DWORD & dwTimeLeft )
{
	int iRet = 0;

	DWORD dwTime = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 7 );

	SYSTEMTIME sStartDate;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT TimeLeft, StartDate FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &dwTime );
				pcDB->GetData( 2, PARAMTYPE_Time, &sStartDate );
			}
		}

		pcDB->Close();
	}



	DWORD dwMax = 0;

	int iType = 0;

	pcDB = SQLCONNECTION( DATABASEID_GameDB, 8 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DurationType, MaxDuration FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 2, PARAMTYPE_Integer, &dwMax );

				if ( iType == QUESTDURATIONTYPE_Running )
				{
					DWORD dwStart = abs( (long)SystemTimeToDWORD( &sStartDate ) - (long)SystemTimeToDWORD( GetServerTime() ) );

					if ( dwStart > dwTime )
						dwTime = 0;
					else
						dwTime -= dwStart;

					iRet = QUESTDURATIONTYPE_Running;
				}
				else if ( iType == QUESTDURATIONTYPE_Amount )
				{
					iRet = QUESTDURATIONTYPE_Amount;
				}
			}
		}

		pcDB->Close();
	}

	dwTimeLeft = dwTime;

	return iRet;
}

int QuestServer::SQLGetQuestTimeLeft( int iQuestID, DWORD & dwTimeLeft )
{
	BOOL iRet = 0;
	DWORD dwMax = 0;
	int iType = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB, 9 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DurationType, MaxDuration FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 2, PARAMTYPE_Integer, &dwMax );

				iRet = iType;
			}
		}

		pcDB->Close();
	}

	dwTimeLeft = dwMax;

	return iRet;
}

BOOL QuestServer::SQLGetAreaQuest( int iQuestID, short & sMapID, short & sGiverNpcID, int & iMinX, int & iMaxX, int & iMinZ, int & iMaxZ, int & iType, int & iRadius )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB, 10 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MapID, NPCID, AreaType, MinX, MaxX, MinZ, MaxZ, Radius FROM QuestList WHERE (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				int iMapID;
				int iGiverNpcID;

				pcDB->GetData( 1, PARAMTYPE_Integer, &iMapID );
				pcDB->GetData( 2, PARAMTYPE_Integer, &iGiverNpcID );
				pcDB->GetData( 3, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 4, PARAMTYPE_Integer, &iMinX );
				pcDB->GetData( 5, PARAMTYPE_Integer, &iMaxX );
				pcDB->GetData( 6, PARAMTYPE_Integer, &iMinZ );
				pcDB->GetData( 7, PARAMTYPE_Integer, &iMaxZ );
				pcDB->GetData( 8, PARAMTYPE_Integer, &iRadius );
				bRet = TRUE;

				sMapID = (short)iMapID;
				sGiverNpcID = (short)iGiverNpcID;
			}
		}

		pcDB->Close();
	}

	return bRet;
}


BOOL QuestServer::SQLGetMonsterItemCharacter( User * pcUser, int iQuestID, char * pszMonsterCounter, char * pszItemCounter )
{
	BOOL bResult = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 8 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT MonsterCount, ItemCount FROM CharacterQuest WHERE (QuestID=?) AND (CharacterID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, pszMonsterCounter, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszItemCounter, 64 );
				bResult = TRUE;
			}
		}

		pcDB->Close();
	}

	return bResult;
}

BOOL QuestServer::SQLGetQuestNameShortDesc( int iQuestID, char * pszName, char * pszGroupName, char * pszShortDescription, BOOL & bPvP, int & iParty, int & iType, int & iLevelMin, int& iLevelMax, BOOL & bMultiple, DWORD & dwWaitingTime, DWORD & dwTimeTotal, short & sMainQuestID, short & sGroupNum )
{
	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB, 12 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT Name, QuestBookGroupID, ShortDescription, Party, PVP, QuestType, MinLevel, MaxLevel, Multiple, WaitTime, MaxDuration, MainQuestID, QuestBookGroupRank FROM QuestList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				int iMainQuestID;
				int iGroupNum;

				pcDB->GetData( 1, PARAMTYPE_String, pszName, 64 );
				pcDB->GetData( 2, PARAMTYPE_String, pszGroupName, 32 );
				pcDB->GetData( 3, PARAMTYPE_String, pszShortDescription, 256 );
				pcDB->GetData( 4, PARAMTYPE_Integer, &iParty );
				pcDB->GetData( 5, PARAMTYPE_Integer, &bPvP );
				pcDB->GetData( 6, PARAMTYPE_Integer, &iType );
				pcDB->GetData( 7, PARAMTYPE_Integer, &iLevelMin );
				pcDB->GetData( 8, PARAMTYPE_Integer, &iLevelMax);
				pcDB->GetData( 9, PARAMTYPE_Integer, &bMultiple );
				pcDB->GetData( 10, PARAMTYPE_Integer, &dwWaitingTime );
				pcDB->GetData( 11, PARAMTYPE_Integer, &dwTimeTotal );
				pcDB->GetData( 12, PARAMTYPE_Integer, &iMainQuestID );
				pcDB->GetData( 13, PARAMTYPE_Integer, &iGroupNum );

				sMainQuestID = static_cast<short>( iMainQuestID );
				sGroupNum = static_cast<short>( iGroupNum );

				bRet = TRUE;
			}
		}

		pcDB->Close();
	}

	return bRet;
}


BOOL QuestServer::SQLStartQuest( User * pcUser, int iQuestID )
{
	EQuestUserStatus iRet = pcUser->GetQuestStatus( iQuestID );
	BOOL bInsert	= TRUE;

	if ( iRet == EQuestUserStatus::Completed )
	{
		WARN( "[SQLStartQuest] Quest %d is already pending hand in!", iQuestID );
		return FALSE;
	}

	if ( iRet == EQuestUserStatus::InProgress )
	{
		WARN( "[SQLStartQuest] Quest %d is already in progress!", iQuestID );
		return FALSE;
	}

	if ( iRet == EQuestUserStatus::Done )
	{
		auto const it = pcUser->mFinishedQuestData->find( iQuestID );
		if ( it != pcUser->mFinishedQuestData->end() )
		{
			const std::string msg = GetQuestWaitMessage( (EQuestType)it->second.eQuestType );
			if ( !msg.empty() )
				CHATSERVER->SendTitleBox( pcUser->pcUserData, msg.c_str() );
		}

		return FALSE;
	}

	if ( iRet == EQuestUserStatus::NotStarted )
		bInsert = TRUE;

	if ( iRet == EQuestUserStatus::Expired )
	{
		DEBUG( "[SQLStartQuest] Quest expired: %d", iQuestID );
		bInsert = FALSE;
	}

	auto it = mQuests.find( iQuestID );
	if ( it == mQuests.end() )
		return FALSE;

	char szMonsterC[64] = { 0 };
	for ( UINT u = 0; u < it->second.sReqMonsterCounts.size(); u++ )
	{
		if ( u == (it->second.sReqMonsterCounts.size() - 1) )
			StringCchCatA( szMonsterC, 64, "0" );
		else
			StringCchCatA( szMonsterC, 64, "0," );
	}

	char szItemC[64] = { 0 };
	for ( UINT u = 0; u < it->second.sReqItemCounts.size(); u++ )
	{
		if ( u == (it->second.sReqItemCounts.size() - 1) )
			StringCchCatA( szItemC, 64, "0" );
		else
			StringCchCatA( szItemC, 64, "0," );
	}

	DWORD dwTime = 0;
	SQLGetQuestTimeLeft( iQuestID, dwTime );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 9 );
	if ( pcDB->Open() )
	{
		if ( bInsert )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterQuest([QuestID],[QuestRewardID],[CharacterID],[AccountName],[CharacterName],[StartDate],[EndDate],[MonsterCount],[ItemCount],[TimeLeft],[Counter]) VALUES (?,?,?,?,?,GETDATE(),NULL,?,?,?,1)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iQuestID );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUser->pcUserData->szAccountName, _countof( pcUser->pcUserData->szAccountName ) );
				pcDB->BindParameterInput( 5, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
				pcDB->BindParameterInput( 6, PARAMTYPE_String, szMonsterC, STRLEN( szMonsterC ) );
				pcDB->BindParameterInput( 7, PARAMTYPE_String, szItemC, STRLEN( szItemC ) );
				pcDB->BindParameterInput( 8, PARAMTYPE_Integer, &dwTime );

				pcDB->Execute();
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE CharacterQuest SET [StartDate]=GETDATE(), [EndDate]=NULL, [TimeLeft]=?, [MonsterCount]=?, [ItemCount]=? WHERE (CharacterID=?) AND (QuestID=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &dwTime );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, szMonsterC, STRLEN( szMonsterC ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, szItemC, STRLEN( szItemC ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iQuestID );

				pcDB->ExecuteUpdate();
			}
		}

		pcDB->Close();
	}

	return TRUE;
}

BOOL QuestServer::SQLQuestSwapClass( char * pszCharacterName, int iOldQuestID, int iNewQuestID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 10 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE CharacterQuest SET QuestID=? WHERE (CharacterName=?) AND (QuestID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iNewQuestID );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, STRLEN( pszCharacterName ) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iOldQuestID );

			pcDB->ExecuteUpdate();
		}
		pcDB->Close();
	}

	return TRUE;
}

// Load Quests (this occur after char load on game server)
BOOL QuestServer::OnLoadUser( User * pcUser )
{
	if ( GAME_SERVER )
		return FALSE;

	if ( pcUser->bQuestLoaded )
		return TRUE;

	LoadFinishedQuestsForUser( pcUser );
	HandleFinishedQuestUser( pcUser, 0, TRUE );
	HandleFinishedWarehouseQuestUser( pcUser );
	HandleFinishedASMQuestUser( pcUser );
	HandleFinishedWarpGateQuestUser( pcUser );
	HandleFinishedRankupQuestUser( pcUser );
	HandleLoadQuests( pcUser );

	pcUser->bQuestLoaded = TRUE;



	return TRUE;
}

BOOL QuestServer::OnUnLoadUser( User * pcUser )
{
	// Not Login Server? not acceptable
	if ( GAME_SERVER )
		return FALSE;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( (ps->iID != 0) )
		{
			HandlePacketUnload( pcUser, ps );
		}
	}
	return TRUE;
}

void QuestServer::OnSayTime( UnitData * pcUnitData )
{
	if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_SKILLMASTER || pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MAGICMASTER )
	{
		if ( pcUnitData->GetCurrentHealth() <= 0 &&
			pcUnitData->pcExclusiveTarget &&
			pcUnitData->lExclusiveTargetID == pcUnitData->pcExclusiveTarget->iID &&
			pcUnitData->pcExclusiveTarget->pcSocketData )
		{
			UserData * ud = pcUnitData->pcExclusiveTarget;
			DWORD dw = pcUnitData->iID;
			ud->dwObjectIDT4 = dw;
			ud->pcUnitDataT4 = pcUnitData;
			// Process T4 Quest
			__asm
			{
				PUSH dw;
				PUSH 0x10; // T4 Quest Code
				PUSH pcUnitData;
				PUSH ud;
				MOV EAX, 0x0055AF20;
				CALL EAX;
				ADD ESP, 0x10;
			}
			ud->dwT4RoomTime = TICKCOUNT + 6000;
			pcUnitData->pcExclusiveTarget = NULL;

			pcUnitData->iFrameCounter = 401;
		}
	}
}



void QuestServer::LoadNPCQuests()
{
	mNPCQuests.clear();
	mQuests.clear();

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB);
	if (pcDB->Open())
	{
		//Other NPC quest givers
		if (pcDB->Prepare("SELECT QL.ID,QL.NPCID,QL.ProgressNPCID,QL.ConclusionNPCID,QL.QuestType,QL.MinLevel,QL.MaxLevel,QL.MapID,QL.Multiple,QL.PVP,QL.AutoStartQuestID,QL.RequiredQuestIDs,QL.InclusionQuestIDs,QL.ClassRestriction,QL.MonsterID,QRL.MonsterQuantities,QL.RequiredItems,QRL.RequiredDropQuantities,QRL.ItemRewardSelect,QRL.ItemsReward,QRL.ItemsRewardQuantities,QRL.ExtraRewardType,QRL.ExtraRewardValues,QRL.EXPReward,QRL.EXPPotBonus,QRL.EXPLevelDifference,QL.Name FROM QuestList AS QL INNER JOIN QuestRewardList AS QRL ON QRL.QuestID = QL.ID") &&
			pcDB->Execute())
		{
			char szRequiredQuestIDs[64] = { 0 };
			char szInclusionQuestIDs[64] = { 0 };
			char szJobRestriction[32] = { 0 };
			char szReqMonsterIds[64] = { 0 };
			char szMonsterCounts[64] = { 0 };
			char szReqItemIds[64] = { 0 };
			char szItemCounts[64] = { 0 };
			char szItemRewards[64] = { 0 };
			char szItemRewardCounts[64] = { 0 };
			char szExtraRewardTypes[64] = { 0 };
			char szExtraRewardValues[64] = { 0 };

			int iQuestID = 0;
			int iGiverID;
			int iProgressID;
			int iCompleteID;
			int iQuestType;
			int iASMQuestBit;

			while (pcDB->Fetch())
			{
				SimpleQuestData lsQuestData{};

				pcDB->GetData(1, PARAMTYPE_Integer, &iQuestID);
				pcDB->GetData(2, PARAMTYPE_Integer, &iGiverID);
				pcDB->GetData(3, PARAMTYPE_Integer, &iProgressID);
				pcDB->GetData(4, PARAMTYPE_Integer, &iCompleteID);
				pcDB->GetData(5, PARAMTYPE_Integer, &iQuestType);
				pcDB->GetData(6, PARAMTYPE_Integer, &lsQuestData.iMinLevel);
				pcDB->GetData(7, PARAMTYPE_Integer, &lsQuestData.iMaxLevel);
				pcDB->GetData(8, PARAMTYPE_Integer, &lsQuestData.iMapID);
				pcDB->GetData(9, PARAMTYPE_Integer, &lsQuestData.bMultiple);
				pcDB->GetData(10, PARAMTYPE_Integer, &lsQuestData.bPVP);
				pcDB->GetData(11, PARAMTYPE_Integer, &lsQuestData.iAutoStartQuestID);
				pcDB->GetData(12, PARAMTYPE_String, szRequiredQuestIDs, _countof(szRequiredQuestIDs));
				pcDB->GetData(13, PARAMTYPE_String, szInclusionQuestIDs, _countof(szInclusionQuestIDs));
				pcDB->GetData(14, PARAMTYPE_String, szJobRestriction, _countof(szJobRestriction));
				pcDB->GetData(15, PARAMTYPE_String, szReqMonsterIds, _countof(szReqMonsterIds));
				pcDB->GetData(16, PARAMTYPE_String, szMonsterCounts, _countof(szMonsterCounts));
				pcDB->GetData(17, PARAMTYPE_String, szReqItemIds, _countof(szReqItemIds));
				pcDB->GetData(18, PARAMTYPE_String, szItemCounts, _countof(szItemCounts));
				pcDB->GetData(19, PARAMTYPE_Integer, &lsQuestData.bItemRewardSelection);
				pcDB->GetData(20, PARAMTYPE_String, szItemRewards, _countof(szItemRewards));
				pcDB->GetData(21, PARAMTYPE_String, szItemRewardCounts, _countof(szItemRewardCounts));
				pcDB->GetData(22, PARAMTYPE_String, szExtraRewardTypes, _countof(szExtraRewardTypes));
				pcDB->GetData(23, PARAMTYPE_String, szExtraRewardValues, _countof(szExtraRewardValues));
				pcDB->GetData(24, PARAMTYPE_Int64, &lsQuestData.lExpReward);
				pcDB->GetData(25, PARAMTYPE_Integer, &lsQuestData.bEXPPotBonus);
				pcDB->GetData(26, PARAMTYPE_Integer, &lsQuestData.iExpLEvelDiff);
				pcDB->GetData(27, PARAMTYPE_String, lsQuestData.szQuestName, _countof(lsQuestData.szQuestName));

				lsQuestData.iQuestID = iQuestID;
				lsQuestData.iASMQuestBit = static_cast<EAsmQuestBit>(iASMQuestBit);
				lsQuestData.eType = static_cast<EQuestType>(iQuestType);
				lsQuestData.iGiverNpcID = iGiverID;
				lsQuestData.eClassFlag = GetQuestClassFlagFromRaw(szJobRestriction);

				//Check if have Required Quests
				for (auto const v : split(szRequiredQuestIDs, ','))
					lsQuestData.sRequiredQuestIDs.push_back(static_cast<short>(atoi(v.c_str())));

				//Check if have Inclusive Quests
				for (auto const v : split(szInclusionQuestIDs, ','))
					lsQuestData.sInclusiveQuestIDs.push_back(static_cast<short>(atoi(v.c_str())));


				if (!IsNull(szReqMonsterIds) && !IsNull(szMonsterCounts))
				{
					std::vector<std::string> vMonsterID = split(szReqMonsterIds, ',');
					std::vector<std::string> vMonsterCounter = split(szMonsterCounts, ',');

					if (vMonsterID.size() == vMonsterCounter.size())
					{
						for (UINT i = 0; i < vMonsterID.size(); i++)
						{
							auto numMonstersReq = atoi(vMonsterCounter[i].c_str());
							auto iMonsterID = atoi(vMonsterID[i].c_str());

							//any monster in specific map
							if (iMonsterID == 0)
							{
								lsQuestData.sReqMonsterCounts.push_back(static_cast<short>(numMonstersReq));
								lsQuestData.sReqMonsterIDs.push_back(0);
								continue;
							}

							auto ps = UNITINFODATA->GetCharacterDataByMonsterID(iMonsterID);
							if (ps)
							{
								lsQuestData.sReqMonsterCounts.push_back(static_cast<short>(numMonstersReq));
								lsQuestData.sReqMonsterIDs.push_back(ps->iUniqueMonsterID);
							}
							else
							{
								LOGERROR("[LoadNPCQuests] Unknown quest monster id: %d", iMonsterID);
							}
						}
					}
					else
					{
						LOGERROR("[LoadNPCQuests] Item ids and counts mismatch for quest %d", iQuestID);
					}
				}

				if (!IsNull(szItemCounts) || !IsNull(szReqItemIds))
				{
					std::vector<std::string> vItemID = split(szReqItemIds, ',');
					std::vector<std::string> vItemCounts = split(szItemCounts, ',');

					if (vItemID.size() == vItemCounts.size())
					{
						for (UINT i = 0; i < vItemID.size(); i++)
						{
							ItemData* pcItemData = ITEMSERVER->FindItemPointerTable(vItemID[i].c_str());
							if (pcItemData)
							{
								const int numItemsRequired = atoi(vItemCounts[i].c_str());

								lsQuestData.sReqItemCounts.push_back(static_cast<short>(numItemsRequired));
								lsQuestData.sReqItemIDs.push_back(pcItemData->sBaseItemID);
							}
							else
							{
								LOGERROR("[LoadNPCQuests] Item code (%s) does not exist!", vItemID[i].c_str());
							}
						}
					}
					else
					{
						LOGERROR("[LoadNPCQuests] Item ids and counts mismatch for quest %d", iQuestID);
					}
				}

				if (!IsNull(szItemRewards) || !IsNull(szItemRewardCounts))
				{
					std::vector<std::string> vItemID = split(szItemRewards, ',');
					std::vector<std::string> vItemCounts = split(szItemRewardCounts, ',');

					if (vItemID.size() == vItemCounts.size())
					{
						for (UINT i = 0; i < vItemID.size(); i++)
						{
							ItemData* pcItemData = ITEMSERVER->FindItemPointerTable(vItemID[i].c_str());
							if (pcItemData)
							{
								const int numItems = atoi(vItemCounts[i].c_str());

								lsQuestData.sItemRewardsCount.push_back(static_cast<short>(numItems));
								lsQuestData.sItemRewardsIDs.push_back(pcItemData->sBaseItemID);
							}
							else
							{
								LOGERROR("[LoadNPCQuests] Reward item code (%s) does not exist!", vItemID[i].c_str());
							}
						}
					}
					else
					{
						LOGERROR("[LoadNPCQuests] Reward item ids and counts mismatch for quest %d", iQuestID);
					}
				}

				if (!IsNull(szExtraRewardTypes) || !IsNull(szExtraRewardValues))
				{
					std::vector<std::string> vExtraTypes = split(szExtraRewardTypes, ',');
					std::vector<std::string> vExtraValuess = split(szExtraRewardValues, ',');

					if (vExtraTypes.size() == vExtraValuess.size())
					{
						for (UINT i = 0; i < vExtraTypes.size(); i++)
						{
							EQuestExtraRewardType eExtraRewardType = static_cast<EQuestExtraRewardType>(atoi(vExtraTypes[i].c_str()));

							if (eExtraRewardType != EQuestExtraRewardType::None)
							{
								int extraValue = atoi(vExtraValuess[i].c_str());

								if (extraValue < 0)
								{
									LOGERROR("Extra reward value cannot be less than 0! Quest id: %d", iQuestID);
									extraValue = 0;
								}

								lsQuestData.eExtraRewardsTypes.push_back(eExtraRewardType);
								lsQuestData.uExtraRewardsValues.push_back(static_cast<UINT>(extraValue));
							}
							else
							{
								LOGERROR("[LoadNPCQuests] Extra reward type (%s) is invalid!", vExtraTypes[i].c_str());
							}
						}
					}
					else
					{
						if (!(vExtraTypes.size() == 1 && static_cast<EQuestExtraRewardType>(atoi(vExtraTypes[0].c_str())) == EQuestExtraRewardType::None))
							LOGERROR("[LoadNPCQuests] Extra reward item ids and counts mismatch for quest %d", iQuestID);
					}
				}

				mQuests.insert({ iQuestID, lsQuestData });

				if (iGiverID <= 0)
					continue;

				int iNPCList[] = { iGiverID, iProgressID, iCompleteID };

				for (int iNPCID : iNPCList)
				{
					auto it = mNPCQuests.find(iNPCID);

					int iFlag = 0;
					if (iGiverID == iNPCID)        iFlag |= NPCQuestData::EFlag::Giver;
					if (iProgressID == iNPCID)    iFlag |= NPCQuestData::EFlag::Progresser;
					if (iCompleteID == iNPCID)    iFlag |= NPCQuestData::EFlag::Completer;

					if (lsQuestData.eType != EQuestType::SoloOneOff && lsQuestData.eType != EQuestType::PartyOneOff)
						iFlag |= NPCQuestData::EFlag::Repeatable;

					//npc doesn't exist
					if (it == mNPCQuests.end())
					{
						NPCQuestData data{};
						data.iQuestID = iQuestID;
						data.eFlag = (NPCQuestData::EFlag)iFlag;

						mNPCQuests.insert({ iNPCID, std::vector<NPCQuestData>() });
						mNPCQuests[iNPCID].push_back(data);
					}

					//npc exists, update quest data or add
					else
					{
						BOOL bFound = FALSE;
						for (auto& questData : mNPCQuests[iNPCID])
						{
							if (questData.iQuestID == iQuestID)
							{
								iFlag = questData.eFlag;

								if (iGiverID == iNPCID)        iFlag |= NPCQuestData::EFlag::Giver;
								if (iProgressID == iNPCID)    iFlag |= NPCQuestData::EFlag::Progresser;
								if (iCompleteID == iNPCID)    iFlag |= NPCQuestData::EFlag::Completer;

								questData.eFlag = (NPCQuestData::EFlag)iFlag;

								bFound = true;
								break;
							}
						}

						if (!bFound)
						{
							NPCQuestData data{};
							data.iQuestID = iQuestID;
							data.eFlag = (NPCQuestData::EFlag)iFlag;
							mNPCQuests[iNPCID].push_back(data);
						}
					}
				}
			}
		}

		pcDB->Clear();

		//Load dailies
		if (pcDB->Prepare("SELECT [NPCID],[QuestListID] FROM QuestWindowList") &&
			pcDB->Execute())
		{
			char szQuestList[255] = { 0 };
			DWORD iNPCID = 0;

			while (pcDB->Fetch())
			{
				szQuestList[0] = { 0 };

				pcDB->GetData(1, PARAMTYPE_Integer, &iNPCID);
				pcDB->GetData(2, PARAMTYPE_String, szQuestList, _countof(szQuestList));

				DEBUG("Loaded NPC (%d) Quest Window List: %s", iNPCID, szQuestList);

				auto it = mNPCQuests.find(iNPCID);

				if (it == mNPCQuests.end())
					mNPCQuests.insert({ iNPCID, std::vector<NPCQuestData>() });


				auto vector = split(szQuestList, ',');

				if (vector.size() > MAX_QUESTDYNAMICMENU)
				{
					LOGERROR("Too many quests for QuestWindowList NPC %d", iNPCID);
					continue;
				}

				for (const auto v : split(szQuestList, ','))
				{
					BOOL bFound = FALSE;
					int iQuestID = atoi(v.c_str());

					auto questData = mQuests.find(iQuestID);
					if (questData == mQuests.end())
					{
						WARN("Quest not found for %d for npc in QuestWindowList %d", iQuestID, iNPCID);
						continue;
					}

					for (auto& npcQuestData : mNPCQuests[iNPCID])
					{
						if (npcQuestData.iQuestID == iQuestID)
						{
							int iFlag = npcQuestData.eFlag;

							iFlag |= NPCQuestData::EFlag::All;
							iFlag |= NPCQuestData::EFlag::QuestWindow;

							if (questData->second.eType != EQuestType::SoloOneOff && questData->second.eType != EQuestType::PartyOneOff)
							{
								iFlag |= NPCQuestData::EFlag::Repeatable;
							}

							npcQuestData.eFlag = (NPCQuestData::EFlag)iFlag;
							bFound = TRUE;
							break;
						}
					}

					if (!bFound)
					{
						int iFlag = 0;
						iFlag |= NPCQuestData::EFlag::All;
						iFlag |= NPCQuestData::EFlag::QuestWindow;

						if (questData->second.eType != EQuestType::SoloOneOff && questData->second.eType != EQuestType::PartyOneOff)
						{
							iFlag |= NPCQuestData::EFlag::Repeatable;
						}

						NPCQuestData data{};
						data.iQuestID = atoi(v.c_str());
						data.eFlag = (NPCQuestData::EFlag)iFlag;
						mNPCQuests[iNPCID].push_back(data);
					}
				}
			}
		}


		pcDB->Close();
	}

	for (auto& npcQuest : mNPCQuests)
	{
		BOOL bContainsWindow = FALSE;
		BOOL bNoWindow = FALSE;

		for (auto& npcQuestData : npcQuest.second)
		{
			if ((npcQuestData.eFlag & NPCQuestData::EFlag::QuestWindow) != 0)
				bContainsWindow = TRUE;
			else
				bNoWindow = TRUE;
		}

		for (auto& npcQuestData : npcQuest.second)
		{
			if (npcQuestData.iQuestID >= 3000 && npcQuestData.iQuestID <= 3999
				|| npcQuestData.iQuestID >= 6000 && npcQuestData.iQuestID <= 6999
				|| npcQuestData.iQuestID >= 10000 && npcQuestData.iQuestID <= 10999)
			{
				if ((npcQuestData.eFlag & NPCQuestData::EFlag::QuestWindow) == 0 && bContainsWindow)
					WARN("Quest ID %d is not part of quest window for npc %d", npcQuestData.iQuestID, npcQuest.first);
			}
		}
	}
}
BOOL QuestServer::IsNpcQuestGiver( int iNPCID )
{
	return mNPCQuests.find( iNPCID ) != mNPCQuests.end();
}

void QuestServer::HandleGetUnitInfoData( User * pcUser, int iUnitID )
{
	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITSDATA + i;

		if ( pc && pc->iID == iUnitID &&
			 pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
		{
			SendNpcQuestStatusIcon( pcUser, pc->sCharacterData.iNPCId );
		}
	}
}

void QuestServer::SendNpcQuestStatusIcon( User * pcUser, int iNPCID, BOOL bForceSendStatus, int iQuestID, BOOL bClicked )
{
	if ( LOGIN_SERVER )
		return;

	auto pNpcUnit = UNITSERVER->GetNPCUnit( iNPCID );
	const char * szNpcName = pNpcUnit == NULL ? "Unknown" : pNpcUnit->GetName();

	if ( !IsNpcQuestGiver( iNPCID ) )
		return;

	EQuestIcon eQuestIcon = EQuestIcon::None;

	auto it = mNPCQuests.find( iNPCID );
	if ( it == mNPCQuests.end() )
		return;

	for ( int i = 0; i <= 5; i++ )
	{
		//set priority here.
		bool bCheckCompleteOnly			= i == 0 || i == 1;
		bool bCheckProgressOnly			= i == 2 || i == 4;
		bool bCheckAvaliableOnly		= i == 3 || i == 5;
		bool bCheckRepeatableOnly		= i == 1 || i == 4 || i == 5;

		for ( auto & npcQuestData : it->second )
		{
			bool isRepeatable = ( npcQuestData.eFlag & NPCQuestData::EFlag::Repeatable ) != 0;

			if ( ( bCheckRepeatableOnly && !isRepeatable ) || ( !bCheckRepeatableOnly && isRepeatable ) )
				continue;

			if (bClicked )
				DEBUG( "[%s]][%d] Checking quest id: %d, repeatable: %d", szNpcName, i, npcQuestData.iQuestID, isRepeatable );

			//todo
			//show progress for daily quests

			auto lsActiveQuest = GetActiveQuestData( pcUser, npcQuestData.iQuestID );
			if ( lsActiveQuest )
			{
				//quest is finished?
				if ( IsCompletedQuestRequirements( pcUser, lsActiveQuest ) )
				{
					if (bClicked )
						DEBUG( "Quest is finished" );

					if ( npcQuestData.IsCompleter() && bCheckCompleteOnly )
					{
						if ( isRepeatable )
						{
							eQuestIcon = EQuestIcon::CompleteRepeatable;
							break; //break and show this icon
						}
						else
						{
							eQuestIcon = EQuestIcon::Complete;
							break; //break and show this icon
						}
					}
					else if ( npcQuestData.IsProgresser() && bCheckProgressOnly )
					{
						eQuestIcon = EQuestIcon::Progress;
						break; //break and show this icon
					}
				}

				//quest is not yet finished
				else
				{
					if ( npcQuestData.IsProgresser() && bCheckProgressOnly )
					{

						if (bClicked )
							DEBUG( "User has not finished active quest yet" );

						eQuestIcon = EQuestIcon::Progress;
						break; //break and show this icon
					}
				}
			}
			else if ( bCheckAvaliableOnly )
			{
				if (bClicked )
					DEBUG( "Quest is not active" );

				//not a giver? skip
				if ( !npcQuestData.IsGiver() )
					continue;

				auto it = mQuests.find( npcQuestData.iQuestID );
				if ( it != mQuests.end() )
				{
					auto data = &it->second;

					const auto eUserQuestStatus = pcUser->GetQuestStatus( npcQuestData.iQuestID );

					if ( IsDoneMainQuestID( pcUser, data ) )
						continue;

					//user has done the quest? (i.e. handed in)
					if ( eUserQuestStatus == EQuestUserStatus::Done )
					{
						//these quest types, once finished, cannot be done again.
						if ( data->eType == EQuestType::SoloOneOff ||
							data->eType == EQuestType::PartyOneOff )
						{
							continue;
						}

						eQuestIcon = EQuestIcon::Unavaliable;
					}

					if ( eUserQuestStatus == EQuestUserStatus::InProgress )
					{
						WARN( "Quest is in progress?? But not an active quest" );
						continue;
					}

					//check if we can take the avaliable quest
					if ( TryTakeAvaliableQuest( pcUser, npcQuestData.iQuestID, iNPCID, FALSE ) )
					{
						if (bClicked )
							DEBUG( "Can take avaliable quest!" );

						eQuestIcon = isRepeatable ? EQuestIcon::AvaliableRepeatable : EQuestIcon::Avaliable;
						break;
					}
					else
					{
						if ( (pcUser->pcUserData->sCharacterData.iLevel + 5) >= data->iMinLevel &&
							 IsCompletedRequiredQuestIDs(pcUser, data))
						{
							eQuestIcon = EQuestIcon::Unavaliable;
						}

						if (bClicked )
							DEBUG( "Can't take avaliable quest!" );
					}
				}
			}
		}

		if ( eQuestIcon != EQuestIcon::None && eQuestIcon != EQuestIcon::Unavaliable )
			break;
	}

	if ( eQuestIcon == EQuestIcon::None && !bForceSendStatus )
		return;

	if (bClicked )
		DEBUG( "Update icon for %s with status of: %d", szNpcName, eQuestIcon );

	SENDPACKETCOMMAND( pcUser, PKTHDR_NpcQuestStatus, iNPCID, (int)eQuestIcon );
}

BOOL QuestServer::OnClickDynamicNPC( User * pcUser, UnitData * pcUnitData )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "OnClickDynamicNPC pcUser is not valid" );
		return FALSE;
	}

	if ( pcUnitData == NULL || pcUnitData->sCharacterData.iType != CHARACTERTYPE_NPC )
		return FALSE;

	BOOL bRet = FALSE;

	int iID = CHARACTERSERVER->SQLGetNPCID( pcUnitData->GetName(), pcUser->pcUserData->iMapID );

	if ( iID >= 0 )
	{
		PacketDynamicQuestOpen s;
		ZeroMemory( &s, sizeof( PacketDynamicQuestOpen ) );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_QuestList_Top, 2 );

		char szQuestList[255] = { 0 };

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [QuestListID],[MainImage],[MainText] FROM QuestWindowList WHERE NPCID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szQuestList, _countof( szQuestList ) );
					pcDB->GetData( 2, PARAMTYPE_String, s.szImagePath, _countof( s.szImagePath ) );
					pcDB->GetData( 3, PARAMTYPE_String, s.szMainText, _countof( s.szMainText ) );

					std::vector<std::string> vQuestID = split( szQuestList, ',' );

					s.iQuestCount = 0;

					for ( const auto v : vQuestID )
					{
						int iQuestID = atoi( v.c_str() );

						const auto & it = mQuests.find( iQuestID );
						if ( it == mQuests.end() )
							continue;

						if ( pcUser->pcUserData->sCharacterData.iLevel > it->second.iMaxLevel )
							continue;

						const EQuestType eQuestType = it->second.eType;
						const EQuestUserStatus eQuestUserStatus = pcUser->GetQuestStatus(iQuestID);

						//don't show one-off quests again if it's already been done.
						if ( ( eQuestType == EQuestType::SoloOneOff || eQuestType == EQuestType::PartyOneOff )
							&& eQuestUserStatus == EQuestUserStatus::Done )
						{
							continue;
						}

						s.saQuestData[s.iQuestCount].iQuestID = iQuestID;
						STRINGCOPY( s.saQuestData[s.iQuestCount].szQuestName, it->second.szQuestName );
						s.saQuestData[s.iQuestCount].sRequiredLevel.iMin = it->second.iMinLevel;
						s.saQuestData[s.iQuestCount].sRequiredLevel.iMax = it->second.iMaxLevel;
						s.saQuestData[s.iQuestCount].eQuestType = eQuestType;
						s.saQuestData[s.iQuestCount].eQuestUserStatus = eQuestUserStatus;

						s.iQuestCount++;


						if ( s.iQuestCount >= MAX_QUESTDYNAMICMENU )
						{
							WARN( "More than %d quests", MAX_QUESTDYNAMICMENU);
							break;
						}
					}

					bRet = TRUE;
				}
			}

			pcDB->Close();
		}

		if ( bRet )
			ProcessPacket( pcUser, &s );
	}

	return bRet;
}

BOOL QuestServer::OnClickNPC( User * pcUser, UnitData * pcUnitData )
{
	if ( pcUnitData == NULL || pcUnitData->sCharacterData.iType != CHARACTERTYPE_NPC )
		return FALSE;

	int iNpcID = pcUnitData->sCharacterData.iNPCId;

	if ( iNpcID <= 0 )
	{
		WARN( "[OnClickNPC] iID is %d", iNpcID );
		return FALSE;
	}

	SendNpcQuestStatusIcon( pcUser, iNpcID, TRUE, -1, TRUE );

	return OnClickNPC( pcUser, iNpcID );
}

BOOL QuestServer::OnClickNPC( User * pcUser, int iNpcID, int iAutoStartQuestID)
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "[OnClickNPC] pcUser is not valid" );
		return FALSE;
	}


	BOOL bNPCHandled = FALSE;

	//int iID = CHARACTERSERVER->SQLGetNPCID( pcUnitData->GetName(), pcUser->pcUserData->iMapID );

	auto it = mNPCQuests.find( iNpcID );
	if ( it == mNPCQuests.end() )
		return FALSE;

	int iSelQuestID = -1;
	int iMinLevel = 999;

	if ( iAutoStartQuestID == -1 )
	{
		for ( int i = 0; i < 5; i++ )
		{
			//check progress / complete-pending quests first
			for ( auto const & npcQuestData : it->second )
			{
				//set priority here.
				bool bCheckCompleteOnly			= i == 0 || i == 1;
				bool bCheckProgressOnly			= i == 2 || i == 4;
				bool bCheckAvaliableOnly		= i == 3 || i == 5;

				bool bCheckRepeatableOnly		= i == 1 || i == 4 || i == 5;

				bool isRepeatable = ( npcQuestData.eFlag & NPCQuestData::EFlag::Repeatable ) != 0;

				if ( ( bCheckRepeatableOnly && !isRepeatable ) || ( !bCheckRepeatableOnly && isRepeatable ) )
					continue;

				auto eFinishQuestResult = pcUser->GetQuestStatus( npcQuestData.iQuestID );

				if ( eFinishQuestResult == EQuestUserStatus::InProgress )
				{
					if ( bCheckProgressOnly && npcQuestData.IsProgresser() )
					{
						SendNpcQuestMessage( pcUser, npcQuestData.iQuestID, 2 );
						return TRUE;
					}
				}
				else if ( eFinishQuestResult == EQuestUserStatus::Completed )
				{
					if ( bCheckCompleteOnly && npcQuestData.IsCompleter() )
					{
						SendNpcQuestMessage( pcUser, npcQuestData.iQuestID, 3 );
						return TRUE;
					}
					else if ( bCheckProgressOnly && npcQuestData.IsProgresser() )
					{
						SendNpcQuestMessage( pcUser, npcQuestData.iQuestID, 2 );
						return TRUE;
					}
				}
				else if ( bCheckAvaliableOnly && npcQuestData.IsGiver() )
				{
					if ( eFinishQuestResult != EQuestUserStatus::Expired && eFinishQuestResult != EQuestUserStatus::NotStarted )
						continue;

					//skip quests that are part of quest window, as they are handled
					//via OnClickDynamicNPC
					if ( (npcQuestData.eFlag & NPCQuestData::EFlag::QuestWindow) != 0 )
						continue;

					//we got a non-repeatable quest, let's break out!
					if ( bCheckRepeatableOnly && iSelQuestID != -1 )
						break;

					if ( TryTakeAvaliableQuest( pcUser, npcQuestData.iQuestID, iNpcID, FALSE ) )
					{
						if ( mQuests[npcQuestData.iQuestID].iMinLevel < iMinLevel )
						{
							iSelQuestID = npcQuestData.iQuestID;
						}
					}
				}
			}
		}
	}
	else
	{
		for ( auto const & npcQuestData : it->second )
		{
			if ( npcQuestData.iQuestID == iAutoStartQuestID )
			{
				if ( TryTakeAvaliableQuest( pcUser, npcQuestData.iQuestID, iNpcID, FALSE ) )
				{
					iSelQuestID = npcQuestData.iQuestID;
				}

				break;
			}
		}
	}

	if ( iSelQuestID != -1 )
	{
		SendNpcQuestMessage( pcUser, iSelQuestID, 1 );
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// Received on game server side from login server
/// </summary>
void QuestServer::HandlePacket( User * pcUser, PacketSyncFinishedQuestData * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketSyncFinishedQuestData) pcUser is not valid" );
		return;
	}

	//DEBUGSTATUS( "PacketSyncFinishedQuestData: quest id %d", psPacket->iQuestID );

	auto it = pcUser->mFinishedQuestData->find( psPacket->iQuestID );

	if ( it == pcUser->mFinishedQuestData->end() )
	{
		FinishedQuestData finishedData{};
		finishedData.eQuestType = psPacket->eQuestType;
		finishedData.sDateFinished = psPacket->sDateFinished;

		pcUser->mFinishedQuestData->insert( {psPacket->iQuestID, finishedData} );
	}
	else
	{
		//use most recent finished date
		if ( abs( (long)SystemTimeToDWORD( psPacket->sDateFinished ) ) > abs( (long)SystemTimeToDWORD( it->second.sDateFinished ) ))
		{
			if (psPacket->eQuestType != EQuestType::None )
				it->second.eQuestType =  psPacket->eQuestType;

			it->second.sDateFinished = psPacket->sDateFinished;
		}
	}
}

/// <summary>
/// Received on login server side
/// </summary>
void QuestServer::HandlePacket( User * pcUser, PacketQuestNPCAccept * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketQuestHandleCancel) pcUser is not valid" );
		return;
	}

	if ( TryTakeAvaliableQuest ( pcUser, psPacket->iID, -1, TRUE ) )
	{
		if ( SQLStartQuest( pcUser, psPacket->iID ) )
		{
			PacketQuestStartData s;
			ZeroMemory( &s, sizeof( PacketQuestStartData ) );

			SQLGetAreaQuest( psPacket->iID, s.sMapID, s.sGiverNpcID, s.iXMin, s.iXMax, s.iZMin, s.iZMax, s.iAreaType, s.iRadius );

			s.bTime = SQLGetQuestTimeLeft( psPacket->iID, s.dwTimeLeft ) > 0 ? TRUE : FALSE;

			ZeroMemory( s.iaMonsterID, _countof( s.iaMonsterID ) * sizeof( int ) );
			ZeroMemory( s.saMonstersReq, _countof( s.saMonstersReq ) * sizeof( USHORT ) );
			ZeroMemory( s.saMonstersKilled, _countof( s.saMonstersKilled ) * sizeof( USHORT ) );
			ZeroMemory( s.szaMonsterName, _countof( s.szaMonsterName ) * ( sizeof( char ) * 32 ) );
			ZeroMemory( s.iaItemID, _countof( s.iaItemID ) * sizeof( int ) );
			ZeroMemory( s.saItemsReq, _countof( s.saItemsReq ) * sizeof( USHORT ) );
			ZeroMemory( s.saItemsCollected, _countof( s.saItemsCollected ) * sizeof( USHORT ) );


			auto it = mQuests.find( psPacket->iID );
			if ( it == mQuests.end() )
			{
				WARN( "[PacketQuestNPCAccept] unable to find quest data for %d", psPacket->iID );
				return;
			}

			int iMonsterCounter = 0;

			CharacterData * ps = NULL;

			auto questData = &it->second;

			SQLGetQuestNameShortDesc( psPacket->iID, s.szName, s.szQuestGroupName, s.szShortDescription, s.bPvP, s.iParty, s.iType, s.iLevelMin, s.iLevelMax, s.bMultiple, s.dwWaitingTime, s.dwTimeTotal, s.sMainQuestID, s.sQuestGroupNum );


			PacketQuestUpdateData updateData{};

			for ( UINT i = 0; i < questData->sReqMonsterCounts.size(); i++ )
			{
				auto iNumMonsterReq = questData->sReqMonsterCounts[i];

				s.iaMonsterID[iMonsterCounter] = questData->sReqMonsterIDs[i];
				s.saMonstersReq[iMonsterCounter] = iNumMonsterReq;
				updateData.saMonstersReq[iMonsterCounter] = iNumMonsterReq;

				if ( s.iaMonsterID[iMonsterCounter] != 0 )
				{
					ps = UNITINFODATA->GetCharacterDataByMonsterID( s.iaMonsterID[iMonsterCounter] );
					if ( ps )
						STRINGCOPY( s.szaMonsterName[iMonsterCounter], ps->szName );
				}
				else
				{
					if ( s.bPvP )
						STRINGCOPY( s.szaMonsterName[iMonsterCounter], "Players" );
					else
						STRINGCOPY( s.szaMonsterName[iMonsterCounter], "Monsters" );
				}

				iMonsterCounter++;
			}

			int iItemCounter = 0;

			for ( UINT i = 0; i <  questData->sReqItemCounts.size(); i++ )
			{
				ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( questData->sReqItemIDs[i].ToInt() );
				if ( pcItemData )
				{
					const int numItemsRequired = questData->sReqItemCounts[i];

					s.iaItemID[iItemCounter] = pcItemData->sBaseItemID.ToInt();
					s.saItemsReq[iItemCounter] = numItemsRequired;
					updateData.saItemsReq[iItemCounter] = numItemsRequired;

					iItemCounter++;
				}
				else
				{
					WARN( "[PacketQuestNPCAccept] Item code (%d) does not exist!", questData->sReqItemIDs[i] );
				}
			}

			s.iID = psPacket->iID;

			if ( IsT5QuestProgress( psPacket->iID ) )
			{
				pcUser->uQuestT5ID = 0;
				pcUser->sQuestT5ProgressValue[0] = 0;
				pcUser->sQuestT5ProgressValue[1] = 0;
			}


			//Tier5
			ProcessT5Quests( pcUser, s.iID, 0, 0 );

			ProcessPacket( pcUser, &s );

			updateData.iID = psPacket->iID;
			updateData.iMonsterCount = iMonsterCounter;
			updateData.iItemsCount = iItemCounter;
			updateData.dwTimeLeft = s.dwTimeLeft;


			if ( psPacket->iID == QUESTID_Rankup_3_Morion || psPacket->iID  == QUESTID_Rankup_3_Tempskron )
			{
				updateData.bFinishedOtherRequirements = FALSE; //initial
			}
			else
			{
				updateData.bFinishedOtherRequirements = TRUE;
			}

			HandlePacket( pcUser, &updateData, TRUE ); //add to user's active quest list so we can get updated NPC quest icon



#ifdef DEV_MODE
			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_White, "Debug > Quest started: %d", updateData.iID );
#endif

			//Log
			char szQuestName[64] = { 0 };

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_QuestList_Top, 4 );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT [Name] FROM QuestList WHERE ID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psPacket->iID );

					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_String, szQuestName, _countof( szQuestName ) );
					}
				}

				pcDB->Close();

				//Log It
				LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestGot, "Character[%s] Got Quest [%s]",
					CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), szQuestName );
			}
		}
		else
		{
			WARN( "PacketQuestNPCAccept: User failed to start quest %d", psPacket->iID );
		}
	}
	else
	{
		WARN( "PacketQuestNPCAccept: User cannot get quest %d", psPacket->iID );
	}
}

//received on login server side
void QuestServer::HandlePacket( User * pcUser, PacketQuestHandleCancel * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketQuestHandleCancel) pcUser is not valid" );
		return;
	}

	auto it = mQuests.find( psPacket->iID );
	if ( it != mQuests.end() && it->second.iMainQuestID > 0)
	{
		for ( auto const & questCheck : mQuests )
		{
			if ( questCheck.first == psPacket->iID )
				continue;

			if ( questCheck.second.iMainQuestID == it->second.iMainQuestID )
			{
				SQLDeleteQuest( pcUser, questCheck.second.iQuestID );
				QUESTSERVER->RemoveQuestFinishData( pcUser, questCheck.second.iQuestID, TRUE );
				NETSERVER->SendQuestCancelOrFinish( pcUser, questCheck.second.iQuestID, FALSE );
			}
		}
	}

	SQLCancelQuest( pcUser, psPacket->iID, FALSE );
}

void QuestServer::HandlePacketUnload( User * pcUser, PacketQuestUpdateData * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacketUnload(PacketQuestUpdateData) pcUser is not valid" );
		return;
	}

	if ( pcUser->GetQuestStatus( psPacket->iID ) == EQuestUserStatus::InProgress )
	{
		if( psPacket->iID == QUESTID_T5QuestArena )
			SQLCancelQuest( pcUser, psPacket->iID, FALSE );

		SQLSaveQuestProgress( pcUser, psPacket );
	}
}

void QuestServer::SQLSaveQuestProgress(User * pcUser, PacketQuestUpdateData * psPacket)
{
	if ( GAME_SERVER )
		return;

	auto eQuestStatus = pcUser->GetQuestStatus( psPacket->iID );

	if ( eQuestStatus == EQuestUserStatus::Expired )
	{
		WARN( "SQLSaveQuestProgress failed. Quest expired?" );
		return;
	}
	else if ( eQuestStatus != EQuestUserStatus::InProgress && eQuestStatus != EQuestUserStatus::Completed )
	{
		WARN( "SQLSaveQuestProgress failed. Quest is not in progress? (%d)", eQuestStatus );
		return;
	}
	else
	{
#ifdef DEV_MODE
		DEBUGSTATUS( "SQLSaveQuestProgress for quest id: %d", psPacket->iID );
#endif
	}

	DWORD dwTimeLeft = 0;

	int iTime = SQLGetQuestTimeLeft( psPacket->iID, dwTimeLeft );

	if ( iTime == QUESTDURATIONTYPE_Amount )
		dwTimeLeft = psPacket->dwTimeLeft;

	char szBuffer[32] = { 0 };


	std::string strM;
	std::string strI;

	BOOL bMonstersFound = FALSE;
	BOOL bItemsFound = FALSE;

	if ( psPacket->iMonsterCount < 0 )
		psPacket->iMonsterCount = 0;
	else if ( psPacket->iMonsterCount > QUEST_MONSTERITEM_MAX )
		psPacket->iMonsterCount = QUEST_MONSTERITEM_MAX;

	if ( psPacket->iItemsCount < 0 )
		psPacket->iItemsCount = 0;
	else if ( psPacket->iItemsCount > QUEST_MONSTERITEM_MAX )
		psPacket->iItemsCount = QUEST_MONSTERITEM_MAX;

	for ( int i = 0; i < psPacket->iMonsterCount; i++ )
	{
		STRINGFORMAT( szBuffer, "%d,", psPacket->saMonstersKilled[i] );
		strM += szBuffer;
	}

	for ( int i = 0; i < psPacket->iItemsCount; i++ )
	{
		STRINGFORMAT( szBuffer, "%d,", psPacket->saItemsCollected[i] );
		strI += szBuffer;
	}

	//Tier5
	ProcessT5Quests( pcUser, psPacket->iID, psPacket->saItemsCollected[0], psPacket->saItemsCollected[1] );

	if ( strI.length() > 0 )
		strI.erase( strI.end() - 1 );

	if ( strM.length() > 0 )
		strM.erase( strM.end() - 1 );

	char szQuery[512] = { 0 };
	STRINGFORMAT( szQuery, "UPDATE CharacterQuest SET [MonsterCount]=%s, [ItemCount]=%s, [TimeLeft]=? WHERE (QuestID=?) AND (CharacterID=?)", strM.length() > 0 ? "?" : "[MonsterCount]", strI.length() > 0 ? "?" : "[ItemCount]" );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB_LocalServer_Quest, 11 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( szQuery ) )
		{
			char szBuffer[128] = { 0 };

			int i = 1;

			if ( strM.length() > 0 )
			{
				STRINGCOPY( szBuffer, strM.c_str() );
				pcDB->BindParameterInput( i, PARAMTYPE_String, szBuffer, STRLEN( szBuffer ) );
				i++;
			}

			if ( strI.length() > 0 )
			{
				STRINGCOPY( szBuffer, strI.c_str() );
				pcDB->BindParameterInput( i, PARAMTYPE_String, szBuffer, STRLEN( szBuffer ) );
				i++;
			}

			pcDB->BindParameterInput( i, PARAMTYPE_Integer, &dwTimeLeft );
			i++;
			pcDB->BindParameterInput( i, PARAMTYPE_Integer, &psPacket->iID );
			i++;
			pcDB->BindParameterInput( i, PARAMTYPE_Integer, &pcUser->iCharacterID );


			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestUpdateData * psPacket, BOOL bUpdateNPCQuestIcon )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketQuestUpdateData) pcUser is not valid" );
		return;
	}

	BOOL bFound = FALSE;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( ps->iID != 0 && (psPacket->iID == ps->iID ) )
		{
			//INFO( "Update active quest data: Quest id: %d", psPacket->iID  );

			BOOL bOldFinish = IsCompletedQuestRequirements( pcUser, ps );

			BOOL bAnyChanged = FALSE;

			//check if we need to save progress
			for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
			{
				if ( (ps->saMonstersKilled[i] != psPacket->saMonstersKilled[i])
				  || (ps->saItemsCollected[i] != psPacket->saItemsCollected[i])
				  || (ps->bFinishedOtherRequirements != psPacket->bFinishedOtherRequirements) )
				{
					bAnyChanged = TRUE;
					break;
				}
			}

			CopyMemory( ps, psPacket, sizeof( PacketQuestUpdateData ) );

			if (bAnyChanged )
				SQLSaveQuestProgress( pcUser, ps );

			BOOL bNewFinish = IsCompletedQuestRequirements( pcUser, ps );

			//user has finished quest?
			//force update npc quest icon
			if ( (bOldFinish == FALSE && bNewFinish == TRUE) ||
				 (bOldFinish == TRUE && bNewFinish == FALSE))
			{
				//DEBUGSTATUS( "FORCE NPC ICON UPDATE FOR QUEST: %d", ps->iID );
				bUpdateNPCQuestIcon = TRUE;
			}

			NETSERVER->SendQuestUpdateDataPart( pcUser, ps, bUpdateNPCQuestIcon );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
		{
			PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

			if ( ps->iID == 0 )
			{
				//INFO( "Add active quest data: Quest id: %d", psPacket->iID  );

				CopyMemory( ps, psPacket, sizeof( PacketQuestUpdateData ) );
				NETSERVER->SendQuestUpdateDataPart( pcUser, ps, TRUE ); //force update NPC icon because it's new
				break;
			}
		}
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketNetQuestUpdateData * psPacket )
{
	if ( LOGIN_SERVER )
		return;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketNetQuestUpdateData) pcUser is not valid" );
		return;
	}

	if ( pcUser )
	{
		CopyMemory( pcUser->saPacketQuestData, psPacket->saPacket, sizeof( PacketQuestUpdateData ) * MAX_QUESTPACKETDATA );
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketNetQuestUpdateDataPart * psPacket )
{
	if ( LOGIN_SERVER )
		return;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketNetQuestUpdateDataPart) pcUser is not valid" );
		return;
	}

	HandlePacket( pcUser, &psPacket->sData );

	if ( psPacket->bUpdateNPCQuestIcon )
	{
		UpdateNPCQuestIconForUser( pcUser, psPacket->sData.iID );
	}
}

/// <summary>
/// Handle quest cancel command sent from login server
/// </summary>
void QuestServer::HandleQuestCancelOrFinish( User * pcUser, int iQuestID, BOOL bForceFinish )
{
	if ( LOGIN_SERVER )
		return;

	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandleQuestCancel pcUser is not valid" );
		return;
	}

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( (ps->iID != 0) && (iQuestID == ps->iID) )
		{
			ZeroMemory( ps, sizeof( PacketQuestUpdateData ) );

			UpdateNPCQuestIconForUser( pcUser, iQuestID );
			return;
		}
	}

	if (!bForceFinish)
		WARN( "User quest not found for id: %d", iQuestID );
}

int QuestServer::GetAutoQuestRewardItemCodeIfAny( int iQuestID )
{
	auto it = mQuests.find( iQuestID );
	if ( it == mQuests.end() )
		return 0;

	auto const questData = &it->second;

	if ( questData->bItemRewardSelection == FALSE )
		return 0;

	if ( questData->sItemRewardsIDs.size() == 0 )
		return 0;

	return questData->sItemRewardsIDs[0].ToInt();
}

//received on login server
void QuestServer::HandlePacket( User * pcUser, PacketQuestNPCFinish * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "PacketQuestNPCFinish pcUser is not valid" );
		return;
	}

	if ( pcUser->GetQuestStatus( psPacket->iID ) == EQuestUserStatus::Completed )
	{
		auto it = mQuests.find( psPacket->iID );
		if ( it == mQuests.end() )
			return;

		auto const questData = &it->second;

		if ( (questData->bItemRewardSelection == TRUE && psPacket->iItemIndex > 0) || (questData->bItemRewardSelection == FALSE) )
		{
			BOOL bCanFinish = TRUE;

			//Items Reward?
			if ( questData->sItemRewardsIDs.size() > 0 )
			{
				if ( questData->bItemRewardSelection )
				{
					int iRewardCounter = 0;

					for ( UINT i = 0; i < questData->sItemRewardsIDs.size(); i++ )
					{
						auto vItemRewardID = questData->sItemRewardsIDs[i];

						DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( vItemRewardID.ToInt() );
						if ( psDef == NULL || IsRewardFilteredOutForUser( pcUser, psDef ))
						{
							continue;
						}

						iRewardCounter++;

						//got the matching item the user wanted
						//as some rewards get filtered out when sent out.
						//see below for another usage of 'IsRewardFilteredOutForUser' func
						if ( iRewardCounter == psPacket->iItemIndex )
						{
							int iQuantity = questData->sItemRewardsCount[i];

							ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( vItemRewardID.ToInt() );
							if ( pcItemData )
							{
								if ( pcItemData->sBaseItemID.ToItemBase() == ITEMBASE_Potion )
								{
									ITEMSERVER->SendPotions( pcUser, pcItemData->sItem.sItemID.ToItemID(), iQuantity, EItemSource::QuestReward, TRUE );
									CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", pcItemData->szBaseName, iQuantity );
								}
								else
								{
									int iWeight = psDef->sItem.iWeight * iQuantity;

									if ( (psPacket->sWeight[0] + iWeight) <= psPacket->sWeight[1]  )
									{
										for ( int j = 0; j < iQuantity; j++ )
										{
											Item sItem{};
											if ( GetItemRewardFromCode( pcUser, vItemRewardID.ToInt(), &sItem, FALSE,  questData->iQuestID ) )
											{
												ITEMSERVER->SendItemData( pcUser->pcUserData, &sItem, TRUE );
												ITEMSERVER->AddItemInventory( pcUser->pcUserData, &sItem );
												CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", pcItemData->szBaseName, iQuantity );
											}
										}
									}
									else
									{
										CHATSERVER->SendTitleBox( pcUser->pcUserData, "Weight limit exceeded" );

										bCanFinish = FALSE;

										psPacket->iID = -1;
										SENDPACKET( pcUser, psPacket );
									}
								}
							}

							break;
						}


					}
				}
				else
				{
					for ( UINT i = 0; i < questData->sItemRewardsIDs.size(); i++ )
					{
						auto iItemReward = questData->sItemRewardsIDs[i];
						int iQuantity = questData->sItemRewardsCount[i];

						DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( iItemReward.ToInt());
						if ( psDef )
						{
							int iWeight = psDef->sItem.iWeight * iQuantity;

							if ( (psPacket->sWeight[0] + iWeight) <= psPacket->sWeight[1] )
							{
								if ( psDef->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
								{
									ITEMSERVER->SendPotions( pcUser, psDef->sItem.sItemID.ToItemID(), iQuantity, EItemSource::QuestReward, TRUE );
									CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", psDef->sItem.szItemName, iQuantity );
								}
								else
								{
									for ( int j = 0; j < iQuantity; j++ )
									{
										Item sItem;
										ZeroMemory( &sItem, sizeof( Item ) );
										if ( GetItemRewardFromCode( pcUser, iItemReward.ToInt(), &sItem, FALSE,  questData->iQuestID  ) )
										{
											ITEMSERVER->SendItemData( pcUser->pcUserData, &sItem, TRUE );
											ITEMSERVER->AddItemInventory( pcUser->pcUserData, &sItem );
											CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s, Quantity: %d", psDef->sItem.szItemName, iQuantity );
										}
									}
								}
							}
							else
							{
								CHATSERVER->SendTitleBox( pcUser->pcUserData, "Weight limit exceeded" );

								psPacket->iID = -1;
								SENDPACKET( pcUser, psPacket );

								bCanFinish = FALSE;
							}
						}
					}
				}

			}

			//EXP?
			if ( bCanFinish && questData->lExpReward > 0 )
			{
				auto iExp = questData->lExpReward;

				if ( questData->bEXPPotBonus )
				{
					if ( pcUser->iEXPPotion > 0 )
						iExp += (iExp * (INT64)pcUser->iEXPPotion) / 100;

					if ( (*(int*)0x0084601C) > 0 )
						iExp += (iExp * (*(int*)0x0084601C)) / 100;
				}

				if ( questData->iExpLEvelDiff )
					iExp = UNITSERVER->GetExpLevelDiference( pcUser->pcUserData->sCharacterData.iLevel, questData->iMinLevel, iExp );

				std::string strEXP = FormatNumber( iExp );

				CHARACTERSERVER->GiveEXP( pcUser, iExp );
				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s EXP!", strEXP.c_str() );

				//Log It
				LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterQuestEXP, "Character[%s] Finished Quest [%s] Received [%s] EXP",
					CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), questData->szQuestName, strEXP.c_str() );
			}

			if ( bCanFinish && questData->eExtraRewardsTypes.size() > 0 )
			{
				for ( UINT i = 0; i < questData->eExtraRewardsTypes.size(); i++ )
				{
					const auto iType = questData->eExtraRewardsTypes[i];
					const auto iValue = questData->uExtraRewardsValues[i];

					if ( iType == EQuestExtraRewardType::Gold )
					{
						CHARACTERSERVER->GiveGOLD( pcUser, iValue );
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Reward: %s gp!", FormatNumber( iValue ) );
					}
					else if ( iType == EQuestExtraRewardType::EXPPotBuff )
					{
						ITEMSERVER->CreatePremium( pcUser, ITEMID_ExpUp24H, ITEMTIMERTYPE_ExpUp, (iValue * 60), true );
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: EXP Potion!" );
					}
					else if ( iType == EQuestExtraRewardType::ThirdEyeBuff )
					{
						ITEMSERVER->CreatePremium( pcUser, ITEMID_ThirdEye24H, ITEMTIMERTYPE_ThirdEye, (iValue * 60), true);
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Third Eye!" );
					}
					else if ( iType == EQuestExtraRewardType::HPUpBuff )
					{
						ITEMSERVER->CreatePremium( pcUser, ITEMID_HPUp12H, ITEMTIMERTYPE_HPUp, (iValue * 60), true);
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Vampire Teeth!" );
					}
					else if ( iType == EQuestExtraRewardType::MPUpBuff)
					{
						ITEMSERVER->CreatePremium( pcUser, ITEMID_MPUp12H, ITEMTIMERTYPE_MPUp, (iValue * 60), true);
						CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Reward: Mana Aura!" );
					}
					else if ( iType == EQuestExtraRewardType::StatPoint )
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Extra stat points: %d", iValue );
					}
					else if ( iType == EQuestExtraRewardType::SkillPoint )
					{
						if (iValue <= 1)
							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Extra skill point: %d", iValue );
						else
							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Extra skill points: %d", iValue );
					}
					else if ( iType == EQuestExtraRewardType::StatPointPerLvl )
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Extra stat points per level: %d", iValue );
					}
					else if ( iType == EQuestExtraRewardType::DecreaseDeathPlenty )
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Death plenty decreased by: %d%%", iValue );
					}
					else if ( iType == EQuestExtraRewardType::ExtraHealth )
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Health point increased by: %d", iValue );
					}
					else if ( iType == EQuestExtraRewardType::RankUp )
					{
						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Your character tier is now at rank %d!", iValue + 1 );
					}
					else if ( iType == EQuestExtraRewardType::QuestWeapon )
					{
						Item sItem{};
						if ( GetQuestWeapon( pcUser, &sItem, 0 ) )
						{
							ITEMSERVER->SendItemData( pcUser->pcUserData, &sItem, TRUE );
							ITEMSERVER->AddItemInventory( pcUser->pcUserData, &sItem );
							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Received quest weapon '%s'", sItem.szItemName );
						}
					}
					else if (iType == EQuestExtraRewardType::SixthSenseBuff)
					{
						ITEMSERVER->CreatePremium(pcUser, ITEMID_SixthSense1H, ITEMTIMERTYPE_SixthSense, (iValue * 60), true);
						CHATSERVER->SendChat(pcUser, CHATCOLOR_Error, "> Reward: Sixth Sense!");
					}
				}
			}

			if ( bCanFinish && SQLFinishQuest( pcUser, psPacket->iID ) )
			{
				SENDPACKET( pcUser, psPacket, TRUE );
				HandleFinishedQuestUser( pcUser, psPacket->iID );

				SENDPACKETBLANK( pcUser, PKTHDR_Save, TRUE );

				//Auto Start
				int iAutoStartQuestID = GetAutoStartQuestID( pcUser, psPacket->iID );
				if ( iAutoStartQuestID != 0 )
				{
					int iAutoStartNpcID = QUESTSERVER->GetNpcQuestGiverIDForQuest( iAutoStartQuestID );
					if ( iAutoStartNpcID > 0 )
					{
						if ( TryTakeAvaliableQuest( pcUser, iAutoStartQuestID, iAutoStartNpcID ) )
						{
							QUESTSERVER->OnClickNPC( pcUser, iAutoStartNpcID, iAutoStartQuestID );
						}
					}
				}
			}
		}

		if ( questData->bItemRewardSelection && psPacket->iItemIndex == 0 &&
			 questData->sItemRewardsIDs.size() > 0)
		{
			PacketQuestNPCItemList s;

			s.iLength = sizeof( PacketQuestNPCItemList );
			s.iHeader = PKTHDR_QuestNPCItemList;

			s.iID = psPacket->iID;
			BOOL bSentLast = FALSE;

			for ( UINT i = 0; i < questData->sItemRewardsIDs.size(); i++ )
			{
				auto iRewardItemID = questData->sItemRewardsIDs[i];

				s.bLast = FALSE;

				DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( iRewardItemID.ToInt() );
				if ( psDef )
				{
					if ( IsRewardFilteredOutForUser( pcUser, psDef ) )
						continue;

					if ( GetItemRewardFromCode( pcUser, iRewardItemID.ToInt(), &s.cItemData.sItem, TRUE,  questData->iQuestID  ) )
					{
						if ( i == (questData->sItemRewardsIDs.size() - 1) )
							bSentLast = s.bLast = TRUE;

						s.cItemData.bValid = TRUE;
						s.iQuantity = questData->sItemRewardsCount[i];
						SENDPACKET( pcUser, &s, TRUE );
					}
				}
			}

			if ( !bSentLast )
			{
				s.cItemData.bValid = FALSE;
				s.bLast = TRUE;
				SENDPACKET( pcUser, &s, TRUE );
			}
		}
	}
}

BOOL QuestServer::IsRewardFilteredOutForUser( User * pcUser, DefinitionItem * psDef )
{
	if ( psDef == NULL )
		return TRUE;

	//filter out low level weapons where needed to avoid confusion
	//i.e. daggers looks like a sword but cannot be wield by a non-assassin class
	if ( psDef->sItem.iLevel < 20 &&
		psDef->sItem.sItemID.ToItemBase() == EItemBase::ITEMBASE_Weapon )
	{
		if ( psDef->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Dagger &&
			pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Assassin )
			return TRUE;

		if ( psDef->sItem.sItemID.ToItemType() != EItemType::ITEMTYPE_Dagger &&
			pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin )
			return TRUE;

		if ( psDef->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Phantom &&
			pcUser->pcUserData->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Shaman )
			return TRUE;

		if ( psDef->sItem.sItemID.ToItemType() != EItemType::ITEMTYPE_Phantom &&
			pcUser->pcUserData->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Shaman )
			return TRUE;
	}

	return FALSE;
}


void QuestServer::SendNpcQuestMessage( User * pcUser, int iQuestID, int iType )
{
	PacketQuestNPCOpen s{};

	std::string query = "";
	switch ( iType )
	{
		case 1:	query = "SELECT [Name],[Description] FROM QuestList WHERE (ID=?)"; break;
		case 2:	query = "SELECT [Name],[ProgressText] FROM QuestList WHERE (ID=?)"; break;
		case 3:	query = "SELECT [Name],[ConclusionText] FROM QuestList WHERE (ID=?)"; break;
		default:
			return;
	}

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_QuestList_Top, 5 );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( query.c_str() ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iQuestID );

			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_String, s.szName, _countof( s.szName ) );
				pcDB->GetData( 2, PARAMTYPE_String, s.szMessage, _countof( s.szMessage ) );
			}
		}

		pcDB->Close();
	}

	s.iLength = sizeof( PacketQuestNPCOpen );
	s.iHeader = PKTHDR_QuestNPCOpen;
	s.iID = iQuestID;
	s.iType = iType;

	if ( iType == 3 )
	{
		s.iAutoItemRewardId = GetAutoQuestRewardItemCodeIfAny( iQuestID );
	}

	SENDPACKET( pcUser, &s );

}

void QuestServer::HandlePacket( User * pcUser, PacketDynamicQuestNPCOpen * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketDynamicQuestNPCOpen) pcUser is not valid" );
		return;
	}

	if ( TryTakeAvaliableQuest( pcUser, psPacket->iQuestID, -1, TRUE ) )
	{
		SendNpcQuestMessage( pcUser, psPacket->iQuestID, 1 );
	}
}

void QuestServer::HandlePacket( User * pcUser, PacketQuestItemSwap * psPacket )
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
	{
		WARN( "HandlePacket(PacketQuestItemSwap) pcUser is not valid" );
		return;
	}

	auto GetQuestDataUpdate = [&pcUser]( int iQuestID )->PacketQuestUpdateData*
	{
		for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
		{
			PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

			if ( ps->iID != 0 && ps->iID == iQuestID )
				return ps;
		}

		return nullptr;
	};

	PacketQuestUpdateData * psQuestData = NULL;

	switch ( psPacket->iQuestID )
	{
		case QUESTID_GreenLiquidQuestStep2:
			if ( (psQuestData = GetQuestDataUpdate( psPacket->iQuestID )) )
			{
				if ( psQuestData->saItemsCollected[0] < 1 )
				{
					if (ITEMSERVER->DeleteItemInventory( pcUser->pcUserData, psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ))
					{
						ITEMSERVER->CreateItem1( pcUser, ITEMID_GreenLiquidTree, EItemSource::T5Quest, 1 );
					}
				}
			}
			break;

		default:
			break;
	}
}

//login server
void QuestServer::HandlePacket(User* pcUser, PacketQuestNamesRequest* psPacket)
{
	PacketQuestNamesResponse questNamesPacket;
	ZeroMemory( &questNamesPacket, sizeof( PacketQuestNamesResponse ) );
	questNamesPacket.bInitial = TRUE;



	std::vector<std::string> queries;

	//boss quests (within level ranges)
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[QuestBookGroupID],[QuestBookGroupRank],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction] FROM QuestList WHERE QuestType = %d AND %d >= MinLevel AND %d <= MaxLevel AND ID >= 6000 AND ID <= 6999", EQuestType::DailyMidNight, pcUser->GetCharacterLevel(), pcUser->GetCharacterLevel() ) );

	//lone wolves (within level ranges)
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[QuestBookGroupID],[QuestBookGroupRank],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction] FROM QuestList WHERE %d >= MinLevel AND %d <= MaxLevel AND ID >= 10000 AND ID <= 10999", pcUser->GetCharacterLevel(), pcUser->GetCharacterLevel() ) );

	//map dailies (within 20 levels, but we will only show top 2 for zones)
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[QuestBookGroupID],[QuestBookGroupRank],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction] FROM QuestList WHERE QuestType = %d AND MinLevel >= (%d - 20) AND MinLevel <= %d AND ID >= 3000 AND ID <= 3999 ORDER BY MinLevel Desc", EQuestType::DailyMidNight, pcUser->GetCharacterLevel(), pcUser->GetCharacterLevel() ) );

	//ASM quests Rank up quests and warp wings
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[QuestBookGroupID],[QuestBookGroupRank],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction] FROM QuestList WHERE (ID >= 2000 AND ID <= 2999) OR (ID >= 4000 AND ID <= 4999) OR (ID >= 5000 AND ID <= 5999) AND MinLevel <= (%d + 20)", pcUser->GetCharacterLevel() ) );


	//low level quest chain. only show if char is low level
	if ( pcUser->GetCharacterLevel() < 50 )
	{
		queries.push_back( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[QuestBookGroupID],[QuestBookGroupRank],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction] FROM QuestList WHERE ID >= 8000 AND ID <= 9999" );
	}

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_QuestList_Top, 6);
	if (pcDB->Open())
	{
		for ( auto & query : queries )
		{
			pcDB->Clear();

			if ( pcDB->Prepare( query.c_str() ) && pcDB->Execute() )
			{
				int iLastMapID = -1;
				int iMapZoneQuestCounter = 0;
				int iLoneWolfQuestCounter = 0;

				while (pcDB->Fetch())
				{
					QuestInformation * data = &questNamesPacket.questsInformation[questNamesPacket.questCount];

					int iQuestID;
					int iPartyMode;
					int iQuestType;
					int iMinLevel;
					int iMaxLevel;
					int iMapID;
					int iNpcID;
					int iMainQuestID;
					int iGroupRank;
					char szRequiredQuestIDs[64] = { 0 };
					char szJobRestriction[32] = { 0 };

					pcDB->GetData(1, PARAMTYPE_Integer, &iQuestID);
					pcDB->GetData(2, PARAMTYPE_String, data->name, _countof(data->name));
					pcDB->GetData(3, PARAMTYPE_Integer, &iPartyMode );
					pcDB->GetData(4, PARAMTYPE_Integer, &iMinLevel);
					pcDB->GetData(5, PARAMTYPE_Integer, &iMaxLevel);
					pcDB->GetData(6, PARAMTYPE_String, data->group, _countof( data->group ));
					pcDB->GetData(7, PARAMTYPE_Integer, &iGroupRank);
					pcDB->GetData(8, PARAMTYPE_Integer, &iMapID);
					pcDB->GetData(9, PARAMTYPE_Integer, &iNpcID);
					pcDB->GetData(10, PARAMTYPE_Integer, &iQuestType);
					pcDB->GetData(11, PARAMTYPE_String, szRequiredQuestIDs, _countof( szRequiredQuestIDs ) );
					pcDB->GetData(12, PARAMTYPE_Integer, &iMainQuestID );
					pcDB->GetData(13, PARAMTYPE_String, szJobRestriction, _countof( szJobRestriction ) );


					auto eQuestClassFlag = GetQuestClassFlagFromRaw( szJobRestriction );
					if (eQuestClassFlag != EQuestClassFlag::None)
					{
						EQuestClassFlag eClassFlag = ECharacterClassToQuestClassFlag( pcUser->pcUserData->sCharacterData.iClass );
						if ( (eQuestClassFlag & eClassFlag ) == EQuestClassFlag::None )
						{
							continue;
						}
					}

					if ( iMainQuestID > 0 && iQuestID != iMainQuestID &&
						 pcUser->GetQuestStatus( iMainQuestID ) == EQuestUserStatus::Done )
					{
						continue;
					}

					//zone quests
					if ( iQuestID >= 3000 && iQuestID <= 3999 )
					{
						if ( iLastMapID != iMapID )
						{
							iMapZoneQuestCounter++;
							iLastMapID = iMapID;
						}

						//skip after 3 (so only top 2 zones will be shown)
						if ( iMapZoneQuestCounter >= 3 )
							continue;
					}

					//lone wolf quests
					if ( iQuestID >= 10000 && iQuestID <= 10999 )
					{
						iLoneWolfQuestCounter++;

						//skip after 3 (so only top 3 lone wolf quests will be shown)
						if ( iLoneWolfQuestCounter >= 4 )
							continue;
					}

					if ( !IsNull( szRequiredQuestIDs ) )
					{
						std::vector<std::string> vQuestID = split( szRequiredQuestIDs, ',' );
						for ( auto v : vQuestID )
						{
							int iReqQuestID = atoi( v.c_str() );
							if ( iReqQuestID > 0 )
								data->requiredQuestId = iReqQuestID;
						}
					}

					data->groupNum = static_cast<short>( iGroupRank );
					data->questID = static_cast<short>( iQuestID );
					data->partyMode = static_cast<EQuestPartyType>( iPartyMode );
					data->questType = static_cast<EQuestType>( iQuestType );
					data->minLevel = static_cast<BYTE>( iMinLevel );
					data->maxLevel = static_cast<BYTE>( iMaxLevel );
					data->mapID = static_cast<short>( iMapID );
					data->giverNpcID = static_cast<short>( iNpcID );
					data->mainQuestId = static_cast<short>( iMainQuestID );

					questNamesPacket.questCount++;

					if ( questNamesPacket.questCount >= MAX_QUEST_INFO )
					{
						DEBUG("Quest count >= %d", MAX_QUEST_INFO);

						questNamesPacket.iHeader = PKTHDR_QuestNamesResponse;
						questNamesPacket.iLength = sizeof(Packet) + sizeof(BOOL) + sizeof(int) + ( sizeof(QuestInformation) * questNamesPacket.questCount );

						//send and reset
						SENDPACKET(pcUser, &questNamesPacket);

						ZeroMemory(&questNamesPacket, sizeof(PacketQuestNamesResponse));
						questNamesPacket.bInitial = FALSE;

						continue;
					}
				}
			}
		}

		pcDB->Close(); //whenever a connection is opened, we need to make sure we close it at the end.
	}

	if ( questNamesPacket.questCount > 0 )
	{
		questNamesPacket.iHeader = PKTHDR_QuestNamesResponse;
		questNamesPacket.iLength = sizeof( Packet ) + sizeof(BOOL) + sizeof( int ) + ( sizeof( QuestInformation ) * questNamesPacket.questCount );

		SENDPACKET( pcUser, &questNamesPacket );
	}
}


void QuestServer::SendNewQuestsPostLevelUp(User* pcUser)
{
	if ( LOGIN_SERVER )
	{
		LOGERROR( "SendNewQuestsPostLevelUp called on login server" );
		return;
	}

	PacketQuestNamesResponse questNamesPacket;
	ZeroMemory( &questNamesPacket, sizeof( PacketQuestNamesResponse ) );
	questNamesPacket.bInitial = FALSE;

	std::vector<std::string> queries;

	//boss quests
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction],[QuestBookGroupID],[QuestBookGroupRank] FROM QuestList WHERE QuestType = %d AND MinLevel = %d AND ID >= 6000 AND ID <= 6999", EQuestType::DailyMidNight, pcUser->GetCharacterLevel() ) );

	//lone wolves
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction],[QuestBookGroupID],[QuestBookGroupRank] FROM QuestList WHERE MinLevel = %d AND ID >= 10000 AND ID <= 10999", EQuestType::DailyMidNight, pcUser->GetCharacterLevel() ) );

	//map dailies (within 20 levels, but we will only show top 2 for zones)
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction],[QuestBookGroupID],[QuestBookGroupRank] FROM QuestList WHERE QuestType = %d AND (MinLevel = %d OR MinLevel = (%d + 20)) AND ID >= 3000 AND ID <= 3999 ORDER BY MinLevel Desc", EQuestType::DailyMidNight, pcUser->GetCharacterLevel(), pcUser->GetCharacterLevel() ) );

	//ASM quests and wings
	queries.push_back( FormatString( "SELECT [ID],[QuestBookName],[Party],[MinLevel],[MaxLevel],[MapID],[NPCID],[QuestType],[RequiredQuestIDs],[MainQuestID],[ClassRestriction],[QuestBookGroupID],[QuestBookGroupRank] FROM QuestList WHERE ((ID >= 5000 AND ID <= 5999) OR (ID >= 4000 AND ID <= 4999) OR (ID >= 2000 AND ID <= 2999 )) AND (MinLevel = %d OR MinLevel = (%d + 20))", pcUser->GetCharacterLevel(), pcUser->GetCharacterLevel() ) );

	SQLConnection* pcDB = SQLCONNECTION(DATABASEID_GameDB_QuestList_Top, 9);
	if (pcDB->Open())
	{
		for ( auto & query : queries )
		{
			pcDB->Clear();

			if ( pcDB->Prepare( query.c_str() ) && pcDB->Execute() )
			{
				while (pcDB->Fetch())
				{
					QuestInformation * data = &questNamesPacket.questsInformation[questNamesPacket.questCount];

					int iQuestID;
					int iPartyMode;
					int iQuestType;
					int iMinLevel;
					int iMaxLevel;
					int iMapID;
					int iNpcID;
					int iMainQuestID;
					int iGroupNum;
					char szRequiredQuestIDs[64] = { 0 };
					char szJobRestriction[32] = { 0 };

					pcDB->GetData(1, PARAMTYPE_Integer, &iQuestID);
					pcDB->GetData(2, PARAMTYPE_String, data->name, 40);
					pcDB->GetData(3, PARAMTYPE_Integer, &iPartyMode );
					pcDB->GetData(4, PARAMTYPE_Integer, &iMinLevel);
					pcDB->GetData(5, PARAMTYPE_Integer, &iMaxLevel);
					pcDB->GetData(6, PARAMTYPE_Integer, &iMapID);
					pcDB->GetData(7, PARAMTYPE_Integer, &iNpcID);
					pcDB->GetData(8, PARAMTYPE_Integer, &iQuestType);
					pcDB->GetData(9, PARAMTYPE_String, szRequiredQuestIDs, _countof( szRequiredQuestIDs ) );
					pcDB->GetData(10, PARAMTYPE_Integer, &iMainQuestID );
					pcDB->GetData(11, PARAMTYPE_String, szJobRestriction, _countof( szJobRestriction ) );
					pcDB->GetData(12, PARAMTYPE_String, data->group, _countof( data->group ));
					pcDB->GetData(13, PARAMTYPE_Integer, &iGroupNum);

					if ( iMainQuestID > 0 && iQuestID != iMainQuestID &&
						pcUser->GetQuestStatus( iMainQuestID ) == EQuestUserStatus::Done )
					{
						continue;
					}

					auto eQuestClassFlag = GetQuestClassFlagFromRaw( szJobRestriction );
					if (eQuestClassFlag != EQuestClassFlag::None)
					{
						EQuestClassFlag eClassFlag = ECharacterClassToQuestClassFlag( pcUser->pcUserData->sCharacterData.iClass );
						if ( (eQuestClassFlag & eClassFlag ) == EQuestClassFlag::None )
						{
							continue;
						}
					}

					if ( !IsNull( szRequiredQuestIDs ) )
					{
						std::vector<std::string> vQuestID = split( szRequiredQuestIDs, ',' );
						for ( auto v : vQuestID )
						{
							int iReqQuestID = atoi( v.c_str() );
							if ( iReqQuestID > 0 )
								data->requiredQuestId = iReqQuestID;
						}
					}

					data->questID = static_cast<short>( iQuestID );
					data->partyMode = static_cast<EQuestPartyType>( iPartyMode );
					data->questType = static_cast<EQuestType>( iQuestType );
					data->minLevel = static_cast<BYTE>( iMinLevel );
					data->maxLevel = static_cast<BYTE>( iMaxLevel );
					data->mapID = static_cast<short>( iMapID );
					data->giverNpcID = static_cast<short>( iNpcID );
					data->mainQuestId = static_cast<short>( iMainQuestID );
					data->groupNum = static_cast<short>( iGroupNum );

					questNamesPacket.questCount++;

					if ( questNamesPacket.questCount >= MAX_QUEST_INFO )
					{
						DEBUG("Quest count >= %d", MAX_QUEST_INFO);

						questNamesPacket.iHeader = PKTHDR_NewQuestNamesPostLvlUp;
						questNamesPacket.iLength = sizeof(Packet) + sizeof(BOOL) + sizeof(int) + ( sizeof(QuestInformation) * questNamesPacket.questCount );

						//send and reset
						SENDPACKET(pcUser, &questNamesPacket);

						ZeroMemory(&questNamesPacket, sizeof(PacketQuestNamesResponse));
						questNamesPacket.bInitial = FALSE;

						continue;
					}
				}
			}
		}

		pcDB->Close(); //whenever a connection is opened, we need to make sure we close it at the end.
	}

	if ( questNamesPacket.questCount > 0 )
	{
		questNamesPacket.iHeader = PKTHDR_NewQuestNamesPostLvlUp;
		questNamesPacket.iLength = sizeof( Packet ) + sizeof(BOOL) + sizeof( int ) + ( sizeof( QuestInformation ) * questNamesPacket.questCount );

		SENDPACKET( pcUser, &questNamesPacket );
	}
}

void QuestServer::HandlePacket(User* pcUser, PacketQuestDataRequest* psPacket)
{
	BOOL bFound = FALSE;

	auto it = mQuests.find( psPacket->questID );
	if ( it == mQuests.end() )
		return;

	const auto & questData = it->second;

	PacketQuestDataResponse questDataResponse;
	ZeroMemory( &questDataResponse, sizeof( PacketQuestDataResponse ) );
	questDataResponse.iHeader = PKTHDR_QuestDataResponse;
	questDataResponse.iQuestID = psPacket->questID;
	questDataResponse.iExpReward = questData.lExpReward;
	questDataResponse.sMapID = questData.iMapID;

	STRINGCOPY( questDataResponse.szStartText, questData.szQuestBookStartText );
	STRINGCOPY( questDataResponse.szTipText,   questData.szQuestBookTipText );
	STRINGCOPY( questDataResponse.szEndText,   questData.szQuestBookEndText );


	for ( UINT i = 0; i < questData.sItemRewardsIDs.size(); i++ )
	{
		auto iItemID = questData.sItemRewardsIDs[i];
		int iQuantity = questData.sItemRewardsCount[i];

		DefinitionItem * psDef = ITEMSERVER->FindItemDefByCode( iItemID.ToInt() );
		if ( psDef == NULL )
			continue;

		if ( IsRewardFilteredOutForUser( pcUser, psDef ) )
			continue;

		if ( psDef->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
		{
			ITEMSERVER->CreateItem( &questDataResponse.saRewardItem[questDataResponse.bItemCount], psDef, EItemSource::QuestReward );
			questDataResponse.saRewardItem[questDataResponse.bItemCount].iPotionCount = iQuantity;

			questDataResponse.bItemCount++;
			if ( questDataResponse.bItemCount >= MAX_QUESTREWARD_ITEM )
				break;
		}
		else
		{
			for ( int j = 0; j < iQuantity; j++ )
			{
				Item sItem;
				ZeroMemory( &sItem, sizeof( Item ) );
				if ( GetItemRewardFromCode( pcUser, iItemID.ToInt(), &sItem, TRUE,  questData.iQuestID  ) )
				{
					CopyMemory( &questDataResponse.saRewardItem[questDataResponse.bItemCount], &sItem, sizeof( Item ) );
					questDataResponse.bItemCount++;

					if ( questDataResponse.bItemCount >= MAX_QUESTREWARD_ITEM )
						break;
				}
			}

			if ( questDataResponse.bItemCount >= MAX_QUESTREWARD_ITEM )
				break;
		}
	}

	for ( UINT i = 0; i < questData.eExtraRewardsTypes.size(); i++ )
	{
		auto eType = questData.eExtraRewardsTypes[i];
		const UINT iValue = questData.uExtraRewardsValues[i];

		if ( eType == EQuestExtraRewardType::None )
			continue;

		questDataResponse.sExtraRewardType[questDataResponse.sExtraRewardCount] = eType;
		questDataResponse.uExtraRewardValue[questDataResponse.sExtraRewardCount] = iValue;

		if ( questDataResponse.sExtraRewardType[questDataResponse.sExtraRewardCount] == ( EQuestExtraRewardType::QuestWeapon ) )
		{
			Item sItem{};
			if ( GetQuestWeapon( pcUser, &sItem, 0 ) )
			{
				sItem.iChk1 = 0;
				sItem.iChk2 = 0;
				sItem.iBackupKey = 0;
				sItem.iBackupChk = 0;
				sItem.tTime = 0;

				CopyMemory( &questDataResponse.saRewardItem[questDataResponse.bItemCount], &sItem, sizeof( Item ) );
				questDataResponse.bItemCount++;
			}
		}

		questDataResponse.sExtraRewardCount++;
		if ( questDataResponse.sExtraRewardCount >= MAX_QUESTEXTRAREWARD_COUNT )
			break;

	}

	for ( UINT i = 0; i < questData.sReqMonsterIDs.size(); i++ )
	{
		auto numMonstersReq = questData.sReqMonsterCounts[i];
		auto iMonsterID = questData.sReqMonsterIDs[i];

		//any monster in specific map
		if ( iMonsterID == 0 )
		{
			questDataResponse.saReqMonstersCounts[questDataResponse.bNumOfReqMonsters] = numMonstersReq;
			questDataResponse.bNumOfReqMonsters++;
			break;
		}

		auto ps = UNITINFODATA->GetCharacterDataByMonsterID( iMonsterID );
		if ( ps == NULL )
			continue;

		STRINGCOPY( questDataResponse.szaReqMonsterNames[questDataResponse.bNumOfReqMonsters], ps->szName );
		questDataResponse.saReqMonstersCounts[questDataResponse.bNumOfReqMonsters] = numMonstersReq;
		questDataResponse.bNumOfReqMonsters++;
	}


	for ( UINT i = 0; i < questData.sReqItemIDs.size(); i++ )
	{
		auto itemID = questData.sReqItemIDs[i];

		ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( itemID.ToInt() );
		if ( pcItemData == NULL )
			continue;

		const int numItemsRequired = questData.sReqItemCounts[i];

		questDataResponse.iaReqItemIds[questDataResponse.bNumOfReqItems] = pcItemData->sBaseItemID.ToInt();
		questDataResponse.saReqItemsCounts[questDataResponse.bNumOfReqItems] = numItemsRequired;
		questDataResponse.bNumOfReqItems++;
	}

	//adjust packet len based on item count
	questDataResponse.iLength = sizeof(PacketQuestDataResponse) - (sizeof(Item) * (MAX_QUESTREWARD_ITEM - questDataResponse.bItemCount));
	SENDPACKET( pcUser, &questDataResponse );
}

void QuestServer::RunTests ()
{
	if ( LOGIN_SERVER )
		return;

	/// <summary>
	/// Check MonsterID values in QuestList
	/// </summary>
	SQLConnection * pcDB = SQLCONNECTION ( DATABASEID_GameDB_QuestList_Top, 8 );
	if ( pcDB->Open () )
	{
		if ( pcDB->Prepare ( "SELECT ID, MonsterID FROM QuestList WHERE MonsterID > '0'", true ) )
		{
			if ( pcDB->Execute () )
			{
				int iID;
				char szMonsterID[64];

				while ( pcDB->Fetch () )
				{
					pcDB->GetData ( 1, EDatabaseDataType::PARAMTYPE_Integer, &iID );
					pcDB->GetData ( 2, EDatabaseDataType::PARAMTYPE_String, szMonsterID, 64);

					std::vector<std::string> vMonsterID = split ( szMonsterID, ',' );

					for ( auto v : vMonsterID )
					{
						int iMonsterID = atoi ( v.c_str () );

						if ( UNITINFODATA->GetCharacterDataByMonsterID ( iMonsterID ) == NULL )
						{
							LOGERROR ( "MonsterID %s does not exist for quest ID: %d", v.c_str (), iID );
						}
					}
				}
			}
		}

		pcDB->Close ();
	}
}

void QuestServer::HandleLoadQuests( User * pcUser )
{
	if ( pcUser && pcUser->pcUserData )
	{
		pcUser->uQuestT5ID					= 0;
		pcUser->sQuestT5ProgressValue[0]	= 0;
		pcUser->sQuestT5ProgressValue[1]	= 0;

		std::vector<int> vQuestID = SQLGetActiveQuestsID( pcUser );

		for ( std::vector<int>::iterator it = vQuestID.begin(); it != vQuestID.end(); it++ )
		{
			int iQuestID = (*it);

			if ( iQuestID != 0 )
			{
				PacketQuestStartData s;
				ZeroMemory(&s, sizeof(PacketQuestStartData));

				s.bInitialLoad = TRUE;
				s.iID = iQuestID;
				s.bTime = SQLGetQuestTimeLeft( pcUser, iQuestID, s.dwTimeLeft ) > 0 ? TRUE : FALSE;


				char szUserMonsterCount[64] = { 0 };
				char szUserItemCount[64]	= { 0 };

				SQLGetAreaQuest( iQuestID, s.sMapID, s.sGiverNpcID, s.iXMin, s.iXMax, s.iZMin, s.iZMax, s.iAreaType, s.iRadius );
				SQLGetQuestNameShortDesc( iQuestID, s.szName, s.szQuestGroupName, s.szShortDescription, s.bPvP, s.iParty, s.iType, s.iLevelMin, s.iLevelMax, s.bMultiple, s.dwWaitingTime, s.dwTimeTotal, s.sMainQuestID, s.sQuestGroupNum );

				auto it = mQuests.find( iQuestID );
				if ( it == mQuests.end() )
				{
					WARN( "[HandleLoadQuests] unable to find quest data for %d", iQuestID );
					continue;
				}

				auto questData = &it->second;

				SQLGetMonsterItemCharacter( pcUser, iQuestID, szUserMonsterCount, szUserItemCount );

				//user item and monster count so far.
				std::vector<std::string> vUserItemCount = split( szUserItemCount, ',' );
				std::vector<std::string> vUserMonsterCount = split( szUserMonsterCount, ',' );

				PacketQuestUpdateData updateData{};
				int iMonsterCounter = 0;
				int iItemCounter = 0;

				if ( questData->sReqItemCounts.size() > 0 || questData->sReqMonsterCounts.size() > 0 ||
					( iQuestID == QUESTID_Rankup_3_Morion || iQuestID == QUESTID_Rankup_3_Tempskron ) ) //special case
				{
					if ( ( vUserItemCount.size() == questData->sReqItemCounts.size() )
						&& ( vUserMonsterCount.size() == questData->sReqMonsterCounts.size() ) )
					{
						CharacterData * ps = NULL;

						for ( UINT i = 0; i < questData->sReqMonsterCounts.size(); i++ )
						{
							auto numMonstersReq = questData->sReqMonsterCounts[i];
							auto numMonstersKilled = atoi( vUserMonsterCount[i].c_str() );

							s.iaMonsterID[iMonsterCounter] = questData->sReqMonsterIDs[i];
							s.saMonstersReq[iMonsterCounter] = numMonstersReq;
							s.saMonstersKilled[iMonsterCounter] = numMonstersKilled;
							updateData.saMonstersReq[iMonsterCounter] = numMonstersReq;

							if ( s.iaMonsterID[iMonsterCounter] != 0 )
							{
								ps = UNITINFODATA->GetCharacterDataByMonsterID( s.iaMonsterID[iMonsterCounter] );
								if ( ps )
								{
									STRINGCOPY( s.szaMonsterName[iMonsterCounter], ps->szName );
								}
							}
							else
							{
								if ( s.bPvP )
									STRINGCOPY( s.szaMonsterName[iMonsterCounter], "Players" );
								else
									STRINGCOPY( s.szaMonsterName[iMonsterCounter], "Monsters" );
							}

							iMonsterCounter++;
						}


						for ( UINT i = 0; i < questData->sReqItemCounts.size(); i++ )
						{
							ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( questData->sReqItemIDs[i].ToInt() );
							if ( pcItemData )
							{
								const int numItemsRequired = questData->sReqItemCounts[i];
								const int numItemsCollectedByUser = atoi( vUserItemCount[i].c_str() );

								s.iaItemID[iItemCounter] = pcItemData->sBaseItemID.ToInt();
								s.saItemsReq[iItemCounter] = numItemsRequired;
								s.saItemsCollected[iItemCounter] = numItemsCollectedByUser;
								updateData.saItemsReq[iItemCounter] = numItemsRequired;

								iItemCounter++;
							}
							else
							{
								WARN( "[HandleLoadQuests] Item code (%d) does not exist!", questData->sReqItemIDs[i].ToInt() );
							}
						}

					}
					else
					{
						WARN( "User quest format is incorrect for quest %d, deleting quest from Quest", iQuestID );
						SQLDeleteQuest( pcUser, iQuestID );
						continue;
					}
				}

				//Tier5
				ProcessT5Quests( pcUser, s.iID, s.saItemsCollected[0], s.saItemsCollected[1] );
				ProcessPacket( pcUser, &s );

				updateData.iID = iQuestID;
				updateData.iMonsterCount = iMonsterCounter;
				updateData.iItemsCount = iItemCounter;
				updateData.dwTimeLeft = s.dwTimeLeft;

				if ( iQuestID == QUESTID_Rankup_3_Morion || iQuestID == QUESTID_Rankup_3_Tempskron )
				{
					updateData.bFinishedOtherRequirements = FALSE; //initial
				}
				else
				{
					updateData.bFinishedOtherRequirements = TRUE;
				}

				HandlePacket( pcUser, &updateData, TRUE ); //add to user's active quest list so we can get updated NPC quest icon
			}
		}
	}
}

void QuestServer::ProcessPacket( User * pcUser, PacketQuestStartData * psPacket )
{
	psPacket->iLength = sizeof( PacketQuestStartData );
	psPacket->iHeader = PKTHDR_QuestStartData;
	SENDPACKET( pcUser, psPacket );
}

void QuestServer::ProcessPacket( User * pcUser, PacketDynamicQuestOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketDynamicQuestOpen );
	psPacket->iHeader = PKTHDR_DynamicQuestOpen;
	SENDPACKET( pcUser, psPacket );
}

void QuestServer::ProcessT5Quests( User * pcUser, int iQuestID, int iValue1, int iValue2 )
{
	if ( IsT5QuestProgress( iQuestID ) )
	{
		pcUser->uQuestT5ID					= iQuestID;
		pcUser->sQuestT5ProgressValue[0]	= (USHORT)iValue1;
		pcUser->sQuestT5ProgressValue[1]	= (USHORT)iValue2;

		NETSERVER->SendQuestData( pcUser );
	};
}

void QuestServer::HandlePacket( PacketNetT5QuestData * psPacket )
{
	User * pcUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) );
	if ( pcUser )
	{
		pcUser->uQuestT5ID					= psPacket->uQuestID;
		pcUser->sQuestT5ProgressValue[0]	= psPacket->sQuestValue[0];
		pcUser->sQuestT5ProgressValue[1]	= psPacket->sQuestValue[1];

//		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Quest Handle Packet" );
	}
}

int QuestServer::GetReduceQuestDrop( User * pcUser, UnitData * pcUnitData )
{
	int iDropReduce = 0;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( ps->iID != 0 )
		{
			for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
			{
				QuestActiveMonsterKillData * psQuestData = (*it);

				if ( psQuestData && (psQuestData->iQuestID == ps->iID) )
				{
					for ( int j = 0; j < psQuestData->sMonsterCount; j++ )
					{
						if ( (psQuestData->sMapID == -1) || (psQuestData->sMapID == pcUser->GetMapID()) )
						{
							if ( ((pcUnitData->sCharacterData.iUniqueMonsterID == psQuestData->iaMonsterID[j]) ||
								(psQuestData->iaMonsterID[j] == 0)) && (ps->saMonstersKilled[j] < psQuestData->saMonsterKillCount[j]) )
							{
								iDropReduce += psQuestData->iDropReduce;
								break;
							}
						}
					}
				}
			}
		}
	}

	return iDropReduce;
}

void QuestServer::HandleQuestMonsterNameRequest ( User * pcUser, int iUniqueMonsterID )
{
	if ( iUniqueMonsterID == QUEST_MONSTERID_ANY ) // 'Any monster'
		return;

	auto charDef = UNITINFODATA->GetCharacterDataByMonsterID( iUniqueMonsterID );
	if ( charDef == NULL )
	{
		return;
	}

	SendQuestMonsterName( pcUser, charDef );
}

void QuestServer::SendQuestMonsterName( User * pcUser, CharacterData * psCharData )
{
	PacketQuestMonsterName packet;
	ZeroMemory( &packet, sizeof( PacketQuestMonsterName ) );

	packet.iLength = sizeof( PacketQuestMonsterName );
	packet.iHeader = PKTHDR_QuestMonsterName;
	packet.uMonsterUniqueID = psCharData->iUniqueMonsterID;
	StringCbCopy( packet.szMonsterName, 32, psCharData->szName );

	SENDPACKET( pcUser, &packet );
}

INT64 QuestServer::GetQuestEXPOnKill( User * pcUser, UnitData * pcUnitData, INT64 iEXP )
{
	INT64 iIncreaseEXP = 0;

	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;

		if ( ps->iID != 0 )
		{
			for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
			{
				QuestActiveMonsterKillData * psQuestData = (*it);

				if ( psQuestData && (psQuestData->iQuestID == ps->iID) )
				{
					for ( int j = 0; j < psQuestData->sMonsterCount; j++ )
					{
						if ( (psQuestData->sMapID == -1) || (psQuestData->sMapID == pcUser->GetMapID()) )
						{
							if ( ((pcUnitData->sCharacterData.iUniqueMonsterID == psQuestData->iaMonsterID[j]) ||
								(psQuestData->iaMonsterID[j] == 0)) && (ps->saMonstersKilled[j] < psQuestData->saMonsterKillCount[j]) )
							{
								iIncreaseEXP += (iEXP * (INT64)psQuestData->iPercentIncreaseEXP) / 100;
								break;
							}
						}
					}
				}
			}
		}
	}

	return iIncreaseEXP;
}

//void QuestServer::UpdateQuestActiveMonsterKill()
//{
//	//Clear List
//	{
//		for ( std::vector<QuestActiveMonsterKillData*>::iterator it = vQuestActiveMonsterKill.begin(); it != vQuestActiveMonsterKill.end(); it++ )
//		{
//			QuestActiveMonsterKillData * ps = (*it);
//			SAFE_DELETE( ps );
//		}
//
//		vQuestActiveMonsterKill.clear();
//	}
//
//
//	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_QuestEventList, 1 );
//	if ( pcDB->Open() )
//	{
//		if ( pcDB->Prepare( "SELECT [QuestID],[PercentIncreaseEXP],[DropReduction] FROM QuestEventList" ) )
//		{
//			if ( pcDB->Execute() )
//			{
//				while ( pcDB->Fetch() )
//				{
//					QuestActiveMonsterKillData * ps = new QuestActiveMonsterKillData;
//					pcDB->GetData( 1, PARAMTYPE_Integer, &ps->iQuestID );
//					pcDB->GetData( 2, PARAMTYPE_Integer, &ps->iPercentIncreaseEXP );
//					pcDB->GetData( 3, PARAMTYPE_Integer, &ps->iDropReduce );
//
//					//Monster Kill Count
//					char szMonsterID[64] = { 0 };
//					char szMonsterCount[64] = { 0 };
//					char szItem[64] = { 0 };
//					char szItemCount[64] = { 0 };
//					if ( SQLGetMonsterItem( ps->iQuestID, szMonsterID, szMonsterCount, szItem, szItemCount ) )
//					{
//						std::vector<std::string> vMonsterCount = split( szMonsterCount, ',' );
//						std::vector<std::string> vMonsterID = split( szMonsterID, ',' );
//
//						ps->sMonsterCount = 0;
//						for ( const auto v : vMonsterCount )
//						{
//							const auto c = vMonsterID[ps->sMonsterCount];
//
//							ps->iaMonsterID[ps->sMonsterCount]			= atoi( c.c_str() );
//							ps->saMonsterKillCount[ps->sMonsterCount]	= atoi( v.c_str() );
//							ps->sMonsterCount++;
//						}
//					}
//
//					int iAreaType = 0;
//
//					int iXMin = 0;
//					int iXMax = 0;
//					int iZMin = 0;
//					int iZMax = 0;
//
//					int iRadius = 0;
//
//					SQLGetAreaQuest( ps->iQuestID, ps->sMapID, ps->sGiverNpcID, iXMin, iXMax, iZMin, iZMax, iAreaType, iRadius );
//
//					vQuestActiveMonsterKill.push_back( ps );
//				}
//			}
//		}
//
//		pcDB->Close();
//	}
//}

BOOL QuestServer::OnUnitKilled( User * pcUser, Unit * pcUnit )
{
	PacketQuestUpdateData sData;

	BOOL bRet = FALSE;
	if ( pcUnit->GetMonsterEffectID() == MONSTEREFFECTID_Gurkob )
	{
		if ( IsActiveQuest( pcUser, QUESTID_GreenLiquidQuestStep1, &sData ) )
		{
			//Chance to Drop?
			if ( Dice::RandomI( 0, 99 ) < 10 )
			{
				//Not completed yet?
				if ( sData.saItemsCollected[0] < 5 )
				{
					//Send Item
					ITEMSERVER->CreateItem1( pcUser, ITEMID_MagicRune, EItemSource::QuestReward, 1 );
				}
			}
		}
	}
	return bRet;
}

BOOL QuestServer::IsActiveQuest( User * pcUser, int iQuestID, PacketQuestUpdateData * psData )
{
	BOOL bRet = FALSE;
	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( ps->iID != 0 )
		{
			if ( ps->iID == iQuestID )
			{
				if ( psData )
					CopyMemory( psData, ps, sizeof( PacketQuestUpdateData ) );

				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

PacketQuestUpdateData * QuestServer::GetActiveQuestData( User * pcUser, int iQuestID )
{
	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( ps->iID != 0 &&
			 ps->iID == iQuestID )
		{
			return ps;
		}
	}

	return NULL;
}

BOOL QuestServer::IsUndergoingAnyQuest( User * pcUser )
{
	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( ps->iID != 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

/// <summary>
/// Checks if the user is currently undergoing a quest that has bMultiple set to FALSE
/// </summary>
BOOL QuestServer::IsUndergoingLoneNonMultipleQuest( User * pcUser )
{
	for ( int i = 0; i < MAX_QUESTPACKETDATA; i++ )
	{
		PacketQuestUpdateData * ps = pcUser->saPacketQuestData + i;
		if ( ps->iID != 0 )
		{
			auto it = mQuests.find( ps->iID );
			if ( it != mQuests.end() )
			{
				if ( it->second.bMultiple == FALSE )
					return TRUE;
			}
		}
	}

	return FALSE;
}


/// <summary>
/// Check if the user has finished the quest requirements
/// </summary>
BOOL QuestServer::IsCompletedQuestRequirements( User * pcUser, PacketQuestUpdateData * pQuestData )
{
	//not finished other requirements.
	if ( pQuestData->bFinishedOtherRequirements == FALSE )
		return FALSE;

	//this quest has no monster or item requirements. Typically these quests
	//only requires you to talk to a specific person to complete it
	if ( pQuestData->IsRequirementsEmpty() )
		return TRUE;

	for ( int i = 0; i < pQuestData->iMonsterCount; i++ )
	{
		//if a condition isn't met, then the quest isn't completed. Return FALSE
		if ( pQuestData->saMonstersKilled[i] < pQuestData->saMonstersReq[i] )
			return FALSE;
	}

	for ( int i = 0; i < pQuestData->iItemsCount; i++ )
	{
		//if a condition isn't met, then the quest isn't completed. Return FALSE
		if ( pQuestData->saItemsCollected[i] < pQuestData->saItemsReq[i] )
			return FALSE;
	}

	//all conditions have met!
	return TRUE;
}


BOOL QuestServer::IsT5QuestProgress( int iQuestID )
{
	if ( (iQuestID == QUESTID_AMatterOfStrenght) || (iQuestID == QUESTID_CryMeARiver) )
		return TRUE;

	return FALSE;
}

