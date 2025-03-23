#include "stdafx.h"
#include "AgeHandler.h"


CAgeHandler::CAgeHandler()
{
}


CAgeHandler::~CAgeHandler()
{
}

void CAgeHandler::SQLItemBroke( User * pcUser, Item * psItem )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 4 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO AgingRecovery([AccountName],[CharacterID],[Date],[ItemName],[AgeNumber],[Code1],[Code2],[DateRecovered]) VALUES (?,?,GETDATE(),?,?,?,?,NULL)" ) )
		{
			int iAge = (int)psItem->sAgeLevel;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, psItem->szItemName, STRLEN( psItem->szItemName ) );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iAge );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &psItem->iChk1 );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psItem->iChk2 );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CAgeHandler::SQLItemBrokeRecovered( User * pcUser, int iID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 5 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE AgingRecovery SET DateRecovered=GETDATE() WHERE (AccountName=?) AND (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iID );

			pcDB->ExecuteUpdate();
		}

		pcDB->Close();
	}
}

BOOL CAgeHandler::GetItemDataBroke( User * pcUser, Item * psItem, int iID )
{
	BOOL bRet = FALSE;

	char szCode[128] = { 0 };

	if ( pcUser && pcUser->pcUserData && psItem )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 6 );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [Code1],[Code2] FROM AgingRecovery WHERE (AccountName=?) AND (ID=?) AND (DateRecovered IS NULL)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					ZeroMemory( psItem, sizeof( Item ) );

					int iCode1 = 0, iCode2 = 0;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iCode1 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iCode2 );

					STRINGFORMAT( szCode, "%d@%d", iCode1, iCode2 );
				}
			}

			pcDB->Close();
		}

		if ( szCode[0] != 0 )
		{
			ItemLoadData sData;

			if ( ITEMSERVER->OnLoadItemData( &sData, szCode ) )
			{
				CopyMemory( psItem, &sData.sItem, sizeof( Item ) );
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

std::vector<AgingRecoveryDataHandle> CAgeHandler::SQLGetBrokeItems( char * pszAccountName )
{
	std::vector<AgingRecoveryDataHandle> vData;

	AgingRecoveryDataHandle sData;

	ZeroMemory( &sData, sizeof( AgingRecoveryDataHandle ) );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB, 7 );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID],[ItemName],[AgeNumber],[Date] FROM AgingRecovery WHERE (AccountName=?) AND (DateRecovered IS NULL) ORDER BY ID DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					ZeroMemory( &sData, sizeof( AgingRecoveryDataHandle ) );

					pcDB->GetData( 1, PARAMTYPE_Integer, &sData.iID );
					pcDB->GetData( 2, PARAMTYPE_String, sData.szItemName, _countof( sData.szItemName ) );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sData.iAgeNumber );
					pcDB->GetData( 4, PARAMTYPE_Time, &sData.sDate );

					vData.push_back( sData );
				}
			}
		}

		pcDB->Close();
	}

	return vData;
}

void CAgeHandler::DownDamage( Item * psItem, int iDown, int iLevel )
{
	if ( iLevel < 10 )
	{
		psItem->sDamage.sMin -= (short)iDown;
		psItem->sDamage.sMax -= (short)iDown;
	}
	else
	{
		switch ( iDown )
		{
			case 1:
				psItem->sDamage.sMin -= 2;
				psItem->sDamage.sMax -= 2;
				break;
			case 2:
				if ( iLevel == 10 )
				{
					psItem->sDamage.sMin -= 2 + 1;
					psItem->sDamage.sMax -= 2 + 1;
				}
				else
				{
					psItem->sDamage.sMin -= 2 * 2;
					psItem->sDamage.sMax -= 2 * 2;
				}
				break;
			case 3:
				if ( iLevel == 10 )
				{
					psItem->sDamage.sMin -= 2 + 1 + 1;
					psItem->sDamage.sMax -= 2 + 1 + 1;
				}
				else if ( iLevel == 11 )
				{
					psItem->sDamage.sMin -= (2 * 2) + 1;
					psItem->sDamage.sMax -= (2 * 2) + 1;
				}
				else
				{
					psItem->sDamage.sMin -= 2 * 3;
					psItem->sDamage.sMax -= 2 * 3;
				}
				break;

			default:
				break;
		}
	}
}

void CAgeHandler::DownDefense( Item * psItem, int iDown, int iPercent )
{
	for ( int i = 0; i < iDown; i++ )
	{
		float f = (100.0f * (float)psItem->iDefenseRating) / (100.0f + (float)iPercent);
		psItem->iDefenseRating = (int)round( f );
	}
}

void CAgeHandler::DownAttackRating( Item * psItem, int iDown, int iNum )
{
	psItem->iAttackRating -= iNum * iDown;
}

void CAgeHandler::DownAbsorbRating( Item * psItem, int iDown, float fPercent, int iLevel )
{
	psItem->fAbsorbRating -= fPercent * (float)iDown;

	int i = iLevel - 9;
	if ( i > 0 )
	{
		if ( i == 1 )
			psItem->fAbsorbRating -= fPercent;
		else
			psItem->fAbsorbRating -= fPercent * (float)iDown;
	}
}

void CAgeHandler::DownCritical( Item * psItem, int iDown )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
	{
		psItem->iCritical -= 1;
		//std::cout << "DownCritical minus 1 at age: " << psItem->sAgeLevel << " : " << psItem->iCritical << std::endl;
	}
	else
	{
		//std::cout << "DownCritical 0 at age (no change): " << psItem->sAgeLevel << " : " << psItem->iCritical << std::endl;
	}

	if ( iDown > 2 )
	{
		if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
			psItem->iCritical -= 1;
	}
}

void CAgeHandler::DownBlockRating( Item * psItem, int iDown )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->fBlockRating -= 1.0f;

	if ( iDown > 2 )
	{
		if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
			psItem->fBlockRating -= 1.0f;
	}
}

void CAgeHandler::UpDamage( Item * psItem )
{
	psItem->sDamage.sMin++;
	psItem->sDamage.sMax++;

	if ( psItem->sAgeLevel >= 9 )
	{
		psItem->sDamage.sMin++;
		psItem->sDamage.sMax++;
	}
}

void CAgeHandler::UpDefense( Item * psItem, int iPercent )
{
	float f = (float)psItem->iDefenseRating / (100.0f / (float)iPercent);
	psItem->iDefenseRating += (int)round( f );
}

void CAgeHandler::UpAttackRating( Item * psItem, int iNum )
{
	psItem->iAttackRating += iNum;
}

void CAgeHandler::UpAbsorbRating( Item * psItem, float fPercent )
{
	psItem->fAbsorbRating += fPercent;

	if ( psItem->sAgeLevel >= 9 )
		psItem->fAbsorbRating += fPercent;
}

void CAgeHandler::UpCritical( Item * psItem )
{
	if (psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1)
	{
		psItem->iCritical++;
		//std::cout << "UpCritical plus 1 at age: " << psItem->sAgeLevel << " : " << psItem->iCritical << std::endl;
	}
	else
	{
		//std::cout << "UpCritical 0 at age (no change): " << psItem->sAgeLevel << " : " << psItem->iCritical << std::endl;
	}

}

void CAgeHandler::UpBlockRating( Item * psItem )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->fBlockRating += 1.0f;
}

BOOL CAgeHandler::OnDownAge( Item * psItem )
{
	//the age level before the down-age
	int iOriginalAgeLevel = (int)psItem->sAgeLevel;

	int iLevelPerAge = Base::Coin::GetLevelItemEachAge(psItem);
	if (iLevelPerAge > 0)
	{
		if ((psItem->sAgeLevel % iLevelPerAge) == 0)
			psItem->iLevel--;
	}

	//note - this should come after iLevel adjustment becase its needs the current age number
	// before taking one off for the code to work
	psItem->sAgeLevel--;

	switch ( psItem->sItemID.ToItemType() )
	{
		case ITEMTYPE_Axe:
			DownDamage( psItem, 1, iOriginalAgeLevel );
			DownAttackRating( psItem, 1, 10 );
			break;

		case ITEMTYPE_Hammer:
			DownDamage( psItem, 1, iOriginalAgeLevel );
			DownAttackRating( psItem, 1, 8 );
			DownCritical( psItem, 1 );
			break;

		case ITEMTYPE_Scythe:
		case ITEMTYPE_Dagger:
		case ITEMTYPE_Sword:
		case ITEMTYPE_Claw:
			DownDamage( psItem, 1, iOriginalAgeLevel );
			DownAttackRating( psItem, 1, 5 );
			DownCritical( psItem, 1 );
			break;

		case ITEMTYPE_Phantom:
		case ITEMTYPE_Wand:
			DownDamage( psItem, 1, iOriginalAgeLevel );
			DownAttackRating( psItem, 1, 8 );
			DownCritical( psItem, 1 );
			psItem->fAddMP -= 10;
			break;

		case ITEMTYPE_Bow:
		case ITEMTYPE_Javelin:
			DownDamage( psItem, 1, iOriginalAgeLevel );
			DownCritical( psItem, 1 );
			break;

		case ITEMTYPE_Shield:
		case ITEMTYPE_Orb:
			DownDefense( psItem, 1, 5 );
			DownBlockRating( psItem, 1 );
			DownAbsorbRating( psItem, 1, 0.4f, iOriginalAgeLevel );
			break;

		case ITEMTYPE_Robe:
		case ITEMTYPE_Armor:
			DownDefense( psItem, 1, 5 );
			DownAbsorbRating( psItem, 1, 0.5f, iOriginalAgeLevel );
			break;

		default:
			break;
	}


	psItem->bMaturing = FALSE;
	psItem->sMatureBar.sCur = 0;
	psItem->sMatureBar.sMax = 0;




	return TRUE;
}

/// <summary>
/// Notes:
/// If -1 de-age occurs (i.e due to failed aging)
/// it actually deages twice, but the 2nd de-age will put
/// mature bar at 50% for the player to mature it back up to -1 from -2
/// </summary>
BOOL CAgeHandler::OnDownAgeHandler( Item * psItem, int iCount )
{
	//std::cout << "-------------------------- OnDownAgeHandler iCount: " << iCount << std::endl;

	if ( iCount > 0 )
	{
		for ( int i = 0; i < iCount; i++ )
			OnDownAge( psItem );
	}

	OnDownAge( psItem );

	ITEMSERVER->ReformItem(psItem);

	return TRUE;
}

BOOL CAgeHandler::OnUpAge( Item * psItem )
{
	switch ( psItem->sItemID.ToItemType() )
	{
		case ITEMTYPE_Axe:
			UpDamage( psItem );
			UpAttackRating( psItem, 10 );
			break;

		case ITEMTYPE_Hammer:
			UpDamage( psItem );
			UpAttackRating( psItem, 8 );
			UpCritical( psItem );
			break;

		case ITEMTYPE_Scythe:
		case ITEMTYPE_Dagger:
		case ITEMTYPE_Sword:
		case ITEMTYPE_Claw:
			UpDamage( psItem );
			UpAttackRating( psItem, 5 );
			UpCritical( psItem );
			break;

		case ITEMTYPE_Phantom:
		case ITEMTYPE_Wand:
			UpDamage( psItem );
			UpAttackRating( psItem, 8 );
			UpCritical( psItem );
			psItem->fAddMP += 10;
			break;

		case ITEMTYPE_Bow:
		case ITEMTYPE_Javelin:
			UpDamage( psItem );
			UpCritical( psItem );
			break;

		case ITEMTYPE_Shield:
		case ITEMTYPE_Orb:
			UpDefense( psItem, 5 );
			UpBlockRating( psItem );
			UpAbsorbRating( psItem, 0.4f );
			break;

		case ITEMTYPE_Robe:
		case ITEMTYPE_Armor:
			UpDefense( psItem, 5 );
			UpAbsorbRating( psItem, 0.5f );
			break;

		default:
			break;
	}

	psItem->bMaturing = FALSE;
	psItem->sMatureBar.sCur = 0;
	psItem->sMatureBar.sMax = 0;

	psItem->sAgeLevel++;


	int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );
	if ( iLevelAge > 0 )
	{
		if ( (psItem->sAgeLevel % iLevelAge) == 0 )
			psItem->iLevel++;
	}

	return TRUE;
}

BOOL CAgeHandler::OnUpAgeHandler( Item * psItem, int iCount )
{
	//std::cout << "-------------------------- OnUpAgeHandler iCount: " << iCount << std::endl;

	for ( int i = 0; i < iCount; i++ )
		OnUpAge( psItem );

	ITEMSERVER->ReformItem( psItem );

	return TRUE;
}

void CAgeHandler::OnBrokeItemHandler( User * pcUser, Item * psItem )
{
	if ( pcUser && psItem )
	{
		ITEMSERVER->SaveItemDataToDatabase( psItem );

		SQLItemBroke( pcUser, psItem );
	}
}
