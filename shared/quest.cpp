#include "stdafx.h"
#include "Quest.h"
#include "strings.h"
#include "def.h"

void QuestData::HandleFinish()
{
	CALL_WITH_ARG1( pfnFinished, (DWORD)this );
}

QuestData::QuestData( PacketQuestStartData * psPacket )
{
	StringCbCopy( szName, _countof( szName ), psPacket->szName );
	StringCbCopy( szGroupName, _countof( szGroupName ), psPacket->szQuestGroupName );
	StringCbCopy( szShortDescription, _countof( szShortDescription ), psPacket->szShortDescription );

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		StringCbCopy( szaMonsterName[i], _countof( szaMonsterName[i] ), psPacket->szaMonsterName[i] );

	iID = psPacket->iID;
	iLevelMin = psPacket->iLevelMin;
	iLevelMax = psPacket->iLevelMax;
	iType = psPacket->iType;
	iAreaType = psPacket->iAreaType;
	sMapID = psPacket->sMapID;
	sGiverNpcID = psPacket->sGiverNpcID;
	sGroupNum = psPacket->sQuestGroupNum;

	bTime = psPacket->bTime;
	dwTimeLeft = psPacket->dwTimeLeft;
	dwWaitingTime = psPacket->dwWaitingTime;
	dwTimeTotal = psPacket->dwTimeTotal;
	iXMin = psPacket->iXMin;
	iXMax = psPacket->iXMax;
	iZMin = psPacket->iZMin;
	iZMax = psPacket->iZMax;
	iRadius = psPacket->iRadius;

	iParty = (EQuestPartyType) psPacket->iParty;
	bPvP = psPacket->bPvP;
	bMultiple = psPacket->bMultiple;
	sMainQuestID = psPacket->sMainQuestID;

	CopyMemory( iaMonsterID, psPacket->iaMonsterID, _countof( iaMonsterID ) * sizeof( int ) );
	CopyMemory( saMonstersReq, psPacket->saMonstersReq, _countof( saMonstersReq ) * sizeof( USHORT ) );
	CopyMemory( saMonstersKilled, psPacket->saMonstersKilled, _countof( saMonstersKilled ) * sizeof( USHORT ) );

	BOOL bF = TRUE;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetMonstersReq( i ) == 0 )
			break;

		if ( GetMonsterKilledAmount( i ) < GetMonstersReq( i ) )
		{
			bF = FALSE;
			break;
		}
	}

	bKillsFinished = bF;

	CopyMemory( iaItemID, psPacket->iaItemID, _countof( iaItemID ) * sizeof( int ) );
	CopyMemory( saItemsReq, psPacket->saItemsReq, _countof( saItemsReq ) * sizeof( USHORT ) );
	CopyMemory( saItemsCollected, psPacket->saItemsCollected, _countof( saItemsCollected ) * sizeof( USHORT ) );

	bF = TRUE;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetItemsReq( i ) == 0 )
			break;

		if ( GetItemsCollected( i ) < GetItemsReq( i ) )
		{
			bF = FALSE;
			break;
		}
	}

	bItemsFinished = bF;

	bUpdate = FALSE;
	bFinishHandled = FALSE;
	bForceUpdate = FALSE;

	//Force update server for these quests
	//due to special requirements..
	if ( iID == QUESTID_Rankup_3_Morion || iID == QUESTID_Rankup_3_Tempskron )
	{
		bForceUpdate = TRUE;
	}
}

QuestData::~QuestData()
{

}

void QuestData::SetName( char * pszQuestName )
{
	StringCbCopy( szName, _countof( szName ), pszQuestName );
}

void QuestData::SetDescription( char * pszQuestDescription )
{
	StringCbCopy( szShortDescription, _countof( szShortDescription ), pszQuestDescription );
}


BOOL QuestData::Update()
{
	if ( bUpdate )
		return TRUE;

	if ( IsFinished() )
		return TRUE;

	return FALSE;
}

BOOL QuestData::HandleUnitKill( int iTargetID, int iTargetLevel, int iKillerID, int iMapID, int iX, int iZ )
{
	BOOL bResult = FALSE;

#ifdef _GAME
	if ( MAP_ID == MAPID_Bellatra )
		return FALSE;

	//exclude vamp bees
	if ( iTargetID == VAMPBEE_MONSTER_ID )
		return FALSE;

	if ( bKillsFinished == FALSE )
	{
		if ( !IsInArea( iMapID, iX, iZ ) )
			return FALSE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetMonstersReq( i ) && (GetMonsterID( i ) == iTargetID || GetMonsterID( i ) == 0) )
			{
				if ( GetMonsterKilledAmount( i ) < GetMonstersReq( i ) )
				{
					AddMonsterKill( i );

					bUpdate = TRUE;

					bResult = TRUE;
				}
			}
		}

		BOOL bF = TRUE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetMonstersReq( i ) == 0 )
				break;

			if ( GetMonsterKilledAmount( i ) < GetMonstersReq( i ) )
			{
				bF = FALSE;
				break;
			}
		}

		bKillsFinished = bF;

		if ( IsFinished() )
			HandleFinish();
	}







#endif
	return bResult;
}

BOOL QuestData::RemoveRequiredItemsFromInventory()
{
	BOOL bRemovedAll = TRUE;
	BOOL bRemovedAny = FALSE;

#ifdef _GAME
	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( iaItemID[i] > 0 && saItemsReq[i] > 0)
		{
			//ignore
			if ( iaItemID[i] == EItemID::ITEMID_QuestHoneyQuest )
				continue;

			int iCountRemoved = ITEMHANDLER->RemoveItemsByCode( (EItemID)iaItemID[i], EItemCraftType::ITEMCRAFTTYPE_None, saItemsReq[i] );

			if ( iCountRemoved > 0 )
				bRemovedAny = TRUE;

			if ( iCountRemoved < saItemsReq[i] )
				bRemovedAll = FALSE;
		}
	}

	if ( bRemovedAny )
		SAVE;

#endif

	return bRemovedAll;
}

BOOL QuestData::CheckPlayerItems( )
{
#ifdef _GAME

	BOOL bOldFinish = bItemsFinished;
	BOOL bAllFinished = TRUE;
	BOOL bQueueForUpdate = FALSE;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( iaItemID[i] > 0 && saItemsReq[i] > 0)
		{
			int iCount = ITEMHANDLER->GetItemAmountByCode( (EItemID)iaItemID[i] );

			if ( iCount > saItemsReq[i] )
				iCount = saItemsReq[i];

			if ( iCount < saItemsReq[i] )
				bAllFinished = FALSE;

			//no count change
			if ( saItemsCollected[i] == iCount )
				continue;

			//new count change
			saItemsCollected[i] = iCount;
			bQueueForUpdate = TRUE;
		}
	}


	if ( bQueueForUpdate )
		bUpdate = TRUE;


	bItemsFinished = bAllFinished;

	if ( bOldFinish == FALSE && bItemsFinished == TRUE )
	{
		if ( IsFinished() )
			HandleFinish();
	}
	else if (bOldFinish == TRUE && bItemsFinished == FALSE )
	{
		bForceUpdate = TRUE;
	}

	return bItemsFinished;

#else
	return FALSE;
#endif
}

BOOL QuestData::HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ, int iTotalItem )
{
	BOOL bResult = FALSE;

	if ( bItemsFinished == FALSE )
	{
		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetItemID( i ) == iItemID )
			{
				if ( iTotalItem >= 0 && iTotalItem <= saItemsReq[i] )
				{
					saItemsCollected[i] = iTotalItem;
					bUpdate = TRUE;
					bResult = TRUE;
				}

				else if ( GetItemsCollected( i ) < GetItemsReq( i ) )
				{
					AddItemCollected( i );

					bUpdate = TRUE;
					bResult = TRUE;
				}
			}
		}

		BOOL bF = TRUE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetItemsReq( i ) == 0 )
				break;

			if ( GetItemsCollected( i ) < GetItemsReq( i ) )
			{
				bF = FALSE;
				break;
			}
		}

		bItemsFinished = bF;

		if ( IsFinished() )
			HandleFinish();
	}
	return bResult;
}

void QuestData::Update( float fTime )
{
	if ( bTime && dwTimeLeft > 0 )
		dwTimeLeft -= (int)fTime;
}


/**
This is called from CQuestGame.cpp
upon killing an unit (player or monster)
*/
BOOL Quest::HandleUnitKill( int iTargetID, int iTargetLevel, int iKillerID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid, BOOL bTargetIsPlayer, BOOL bTargetIsBoss)
{

	BOOL bResult = FALSE;

#ifdef _GAME
	if ( MAP_ID == MAPID_Bellatra )
		return FALSE;

	if ( iTargetID == VAMPBEE_MONSTER_ID )
		return FALSE;

	//Note - when bRaid is TRUE, bParty is always TRUE


	BOOL bSolo = bParty == FALSE && bRaid == FALSE;

	//Handle Kills Quests
	for (QuestData* ps : GetData())
	{
		auto ePartyType = ps->GetQuestPartyType();

		//user is not in party or raid
		if (bSolo)
		{
			//Player is solo and quest
			// does not allow solo. Skip
			if (!ps->QuestAllowsSolo())
				continue;
		}

		//user is in raid
		else if (bRaid)
		{
			//Player is in raid and quest does not allow for raid
			if (!ps->QuestAllowsRaid())
				continue;
		}

		//user is in party
		else if (bParty)
		{
			//Player is in party and quest does not allow party
			if (!ps->QuestAllowsParty())
				continue;
		}

		if ( ePartyType == EQuestPartyType::SoloAny )
		{
			if ( UNITDATA->iID != iKillerID )
				continue;
		}

		if ( ePartyType == EQuestPartyType::SoloPartyAny )
		{
			BOOL bFound = FALSE;

			for ( auto & p : PARTYHANDLER->GetPartyMembers() )
			{
				if ( p && p->bActive && p->iID == iKillerID )
				{
					bFound = TRUE;
					break;
				}
			}

			if ( !bFound && UNITDATA->iID != iKillerID )
				continue;
		}

		int iTarget = iTargetID;

		//unit killed was another player
		if (bTargetIsPlayer)
		{
			//quest allows pvp. set iTarget to 0
			if (ps->QuestAllowsPVP())
			{
				iTarget = 0;
			}

			//quest does not allow PVP
			else
			{
				continue;
			}
		}

		//Handle Kills, if handled, return TRUE
		if ( ps->HandleUnitKill( iTarget, iTargetLevel, iKillerID, iMapID, iX, iZ ) )
		{
			bResult = TRUE;
			QUESTBOOKHANDLER->RebuildCurrentQuestDataView();
		}
	}

	//for ( QuestData * ps : GetData() )
	//{
	//	ps->CheckFinishHandled();
	//}

#endif
	return bResult;
}

BOOL Quest::CheckAllItemRequirements( )
{
	BOOL bResult = FALSE;

	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		if ( ps && ps->CheckPlayerItems() )
			bResult = TRUE;
	}

	return bResult;
}

BOOL Quest::CheckAllOtherRequirements( )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;
		ps->CheckOtherRequirementsFinished();
	}

	return TRUE;
}

Quest::Quest()
{
}

Quest::~Quest()
{
	vData.clear();
}

QuestData * Quest::GetQuest( int iID )
{
	QuestData * pcQuestData = NULL;

	//Get Quests on same ID
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		//Same ID? Push It to vector
		if ( ps->GetID() == iID )
		{
			pcQuestData = ps;
			break;
		}
	}

	//Return all data of Quest ID
	return pcQuestData;
}

BOOL Quest::IsActiveQuest( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::IsFinishedQuestKills( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID && ps->IsKillsFinished() )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::IsFinishedQuestItems( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID && ps->IsItemsFinished() )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::AddQuest( QuestData * psQuestData )
{
	//Add QuestKill to vector Data
	GetData().push_back( psQuestData );

	return TRUE;
}

BOOL Quest::AddFinishedQuest( PacketHandleFinishedQuest * psPacket )
{

	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); it++ )
	{
		QuestFinishedData * ps = (*it);

		if ( ps && ps->iID == psPacket->iID )
		{
			delete ps;
			vFinishedData.erase( it );
			break;
		}
	}

	QuestFinishedData * ps = new QuestFinishedData;

	ps->iID = psPacket->iID;
	ps->eType = psPacket->eType;
	ps->eParty = psPacket->eQuestPartyType;
	ps->iMinLevel = psPacket->iMinLevel;
	ps->iMaxLevel = psPacket->iMaxLevel;
	ps->sMapID = psPacket->sMapID;
	ps->sGiverNpcID = psPacket->sGiverNpcID;
	ps->sMainQuestID = psPacket->sMainQuestID;
	ps->sGroupNum = psPacket->sGroupNum;

	StringCbCopy( ps->szQuestName, _countof( ps->szQuestName ), psPacket->szQuestName );
	StringCbCopy( ps->szQuestGroupName, _countof( ps->szQuestGroupName ), psPacket->szQuestGroupName );

	CopyMemory( &ps->sStartDate, &psPacket->sStartDate, sizeof( SYSTEMTIME ) );
	CopyMemory( &ps->sEndDate, &psPacket->sEndDate, sizeof( SYSTEMTIME ) );

	CopyMemory( ps->iaExtraReward, psPacket->iaExtraReward, sizeof( int ) * QUEST_MONSTERITEM_MAX );
	CopyMemory( ps->iaExtraRewardValue, psPacket->iaExtraRewardValue, sizeof( int ) * QUEST_MONSTERITEM_MAX );

	vFinishedData.push_back( ps );

	return TRUE;
}

BOOL Quest::IsDoneQuest( int iQuestID )
{
	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); it++ )
	{
		QuestFinishedData * ps = (*it);

		if ( ps && ps->iID == iQuestID )
			return TRUE;
	}

	return FALSE;
}

#if defined (_GAME)
BOOL Quest::IsQuestExpired(EQuestType eQuestType, const SYSTEMTIME& finishTime)
{
	SYSTEMTIME* current = &GameCore::GetInstance()->sServerTime;
	DWORD dwTime = abs((long)SystemTimeToDWORD(current)) - (long)SystemTimeToDWORD(finishTime);

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
#endif

BOOL Quest::DeleteQuest( int iID )
{
	BOOL bResult = FALSE;

	//Delete All Quests in vector with same ID
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); )
	{
		QuestData * ps = *it;

		if ( ps->GetID() == iID )
		{
			bResult = TRUE;
			SAFE_DELETE( ps );

			it = GetData().erase( it );
		}
		else it++;
	}

	return bResult;
}

BOOL Quest::DeleteFinishQuest( int iID )
{
	BOOL bResult = FALSE;

	//Delete All Quests in vector with same ID
	for ( std::vector<QuestFinishedData*>::iterator it = GetFinishData().begin(); it != GetFinishData().end(); )
	{
		QuestFinishedData * ps = *it;

		if ( ps->iID == iID )
		{
			bResult = TRUE;
			SAFE_DELETE( ps );

			it = GetFinishData().erase( it );
		}
		else it++;
	}

	return bResult;
}

void Quest::DeleteAllQuests()
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		delete ps;
	}

	GetData().clear();
}

int Quest::GetRewardValue( EQuestExtraRewardType iRewardType )
{
	int iValue = 0;

	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); it++ )
	{
		QuestFinishedData * ps = (*it);

		for ( int j = 0; j < QUEST_MONSTERITEM_MAX; j++ )
		{
			if ( ps->iaExtraReward[j] != EQuestExtraRewardType::None && ps->iaExtraReward[j] == iRewardType )
				iValue += ps->iaExtraRewardValue[j];
		}
	}

	return iValue;
}

BOOL QuestData::CanFinishQuestWaitTime()
{
	if ( dwWaitingTime > 0 )
	{
		if ( IsTimeQuest() )
		{
			DWORD dwTime = dwTimeTotal - GetTimeQuest();

			if ( dwTime <= dwWaitingTime )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL QuestData::IsInArea( int iMapID, int iX, int iZ )
{
	if ( GetMapID() != (-1) && GetMapID() != iMapID )
		return FALSE;

	if ( GetAreaType() == QUESTAREATYPE_Bound )
	{
		int iXMap = iX >> 8;
		int iZMap = iZ >> 8;

		if ( iXMap < GetXMin() || iXMap >GetXMax() || iZMap < GetZMin() || iZMap > GetZMax() )
			return FALSE;
	}

	if ( GetAreaType() == QUESTAREATYPE_Radius )
	{
		int iXMap = (iX >> 8) - GetXMin();
		int iZMap = (iZ >> 8) - GetZMin();

		int iDistance = abs( (iXMap*iXMap) * (iZMap*iZMap) );

		if ( iDistance >= GetRadius() )
			return FALSE;
	}

	return TRUE;
}

int QuestData::GetMonsterID( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaMonsterID[i];
}

USHORT QuestData::GetMonstersReq( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return saMonstersReq[i];
}

void QuestData::CheckOtherRequirementsFinished()
{
#ifdef _GAME

	BOOL bOldFinish = bOtherRequirementsFinished;
	BOOL bNewFinish = FALSE;

	//check quest weapon
	if ( HasOtherRequirements() )
	{
		auto itemQuestWeapon = ITEMHANDLER->GetCurrentQuestWeapon();
		if ( itemQuestWeapon &&
			itemQuestWeapon->sItem.sAgeLevel >= ( QUESTWEAPON_TARGET_AGE ) ) //once age 3 is fully matured, it will become age 4
		{
			bNewFinish = TRUE;
		}
		else
		{
			bNewFinish = FALSE;
		}

		bOtherRequirementsFinished = bNewFinish;

		if ( !bOldFinish && bNewFinish )
		{
			if (IsFinished())
				HandleFinish();
		}
		else if (bOldFinish && !bNewFinish )
		{
			bForceUpdate = TRUE;
		}

		return;
	}

	bOtherRequirementsFinished = TRUE; //need to be set to TRUE for all other quests

#endif
}

USHORT QuestData::GetMonsterCount()
{
	USHORT iCount = 0;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetMonstersReq( i ) != 0 )
			iCount++;
	}

	return iCount;
}

USHORT QuestData::GetItemCount()
{
	USHORT iCount = 0;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetItemsReq( i ) != 0 )
			iCount++;
	}

	return iCount;
}

USHORT QuestData::GetMonsterKilledAmount( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return saMonstersKilled[i];
}

int QuestData::GetTotalMonsterKilledAmount( )
{
	int iTotal = 0;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		iTotal += saMonstersKilled[i];

	return iTotal;
}

char * QuestData::GetMonsterName( int i )
{
	static char * pszNull = "";
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return pszNull;

	return szaMonsterName[i];
}

int QuestData::GetTotalItemAmount( )
{
	int iTotal = 0;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		iTotal += saItemsCollected[i];

	return iTotal;
}

char * QuestData::GetItemName( int i )
{
	static char * pszNull = "";
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return pszNull;

	return szaItemName[i];
}

int QuestData::GetItemID( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaItemID[i];
}

USHORT QuestData::GetItemsReq( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return saItemsReq[i];
}

USHORT QuestData::GetItemsReqByItemID( int iItemID )
{
	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( iaItemID[i] == iItemID )
			return saItemsReq[i];
	}

	return 0;
}

USHORT QuestData::GetItemsCollected( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return saItemsCollected[i];
}

BOOL QuestData::AddMonsterKill( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return FALSE;

	if ( saMonstersKilled[i] < saMonstersReq[i] )
		saMonstersKilled[i]++;

	return TRUE;
}

BOOL QuestData::AddItemCollected( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return FALSE;

	if ( saItemsCollected[i] < saItemsReq[i] )
		saItemsCollected[i]++;

	return TRUE;
}