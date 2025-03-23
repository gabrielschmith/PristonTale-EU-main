#include "stdafx.h"
#include "TestMapHandler.h"

CTestMapHandler::CTestMapHandler ()
{
	for ( int i = 0; i < _TESTMAP_DPS_SPAWN_COUNT; i++ )
	{
		pcUnitDataDps[i] = NULL;
	}

	iMonsterHP = 1'000'000;

}


CTestMapHandler::~CTestMapHandler ()
{
}

void CTestMapHandler::SetMonsterHP(int iHP)
{
	iMonsterHP = iHP;
}

void CTestMapHandler::SpawnTestMonstersDps (User * pcUser)
{
	if ( pcUser == NULL || pcUser->IsValidAndInGame() == FALSE )
		return;

	bActive = TRUE;
	eTestMode = ETestMode::DPS;

	ClearUnitsFromMap ();
	SendSpawnFlagMarkersAoE ( pcUser );

	// Spawn Minions
	for ( int i = 0; i < _TESTMAP_DPS_SPAWN_COUNT; i++ )
	{
		if ( saDpsSpawns[i].iX != 0 )
		{
			pcUnitDataDps[i] = UNITSERVER->CreateUnitEnemy ( (char*)GetTestMonsterName().c_str(), saDpsSpawns[i].iX << 8, saDpsSpawns[i].iY << 8, saDpsSpawns[i].iZ << 8 );

			if ( pcUnitDataDps[i] )
			{
				pcUnitDataDps[i]->bTestMapUnit = TRUE;
				pcUnitDataDps[i]->InitHealthPoint( iMonsterHP );
				STRINGCOPY( pcUnitDataDps[i]->sCharacterData.szName, FormatString( "%s (%d)", pcUnitDataDps[i]->sCharacterData.szName, i ) );
			}
		}
	}

	bSpawned = TRUE;
}

void CTestMapHandler::Reset ()
{
	ClearUnitsFromMap ();
	bSpawned = FALSE;
	bActive = FALSE;
}


BOOL CTestMapHandler::UnitCanTargetPlayer ( UnitData * pcUnitData )
{
	if ( pcUnitData->bTestMapUnit &&
		 pcUnitData->pcMap->pcBaseMap->iMapID == MAPID_QuestArena &&
		 eTestMode == ETestMode::DPS)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTestMapHandler::UnitCanMove( UnitData * pcUnitData )
{
	if ( pcUnitData->bTestMapUnit &&
		 pcUnitData->pcMap->pcBaseMap->iMapID == MAPID_QuestArena &&
		 eTestMode == ETestMode::DPS)
	{
		return FALSE;
	}

	return TRUE;
}


/// <summary>
/// Send spawn flag markers so that they will be shown in mini map
/// </summary>
void CTestMapHandler::SendSpawnFlagMarkersAoE ( User * pcUser )
{
	PacketMapSpawnMarkers sPacket;

	ZeroMemory ( &sPacket, sizeof ( PacketMapSpawnMarkers ) );
	sPacket.iLength = sizeof ( PacketMapSpawnMarkers );
	sPacket.iHeader = PKTHDR_MapSpawnMarkers;
	sPacket.iCount = _TESTMAP_DPS_SPAWN_COUNT;

	CopyMemory ( &sPacket.saMarkers, saDpsSpawns, _TESTMAP_DPS_SPAWN_COUNT * sizeof ( Point3D ) );

	SENDPACKET ( pcUser, &sPacket );
}

void CTestMapHandler::ClearUnitsFromMap ()
{
	// Minions
	for ( int i = 0; i < _TESTMAP_DPS_SPAWN_COUNT; i++ )
	{
		if ( pcUnitDataDps[i] )
		{
			if ( !pcUnitDataDps[i]->IsDead () )
			{
				UNITSERVER->SetMotionFromCode ( pcUnitDataDps[i], EAnimationType::ANIMATIONTYPE_Die );
			}
		}
		pcUnitDataDps[i] = NULL;
	}
}

void CTestMapHandler::Tick1s ()
{
	//Only for game server
	if ( LOGIN_SERVER )
		return;

	// Not active
	if ( bActive == FALSE )
		return;

	//std::cout << "CTestMapHandler::Tick1s" << std::endl;

	for ( int i = 0; i < _TESTMAP_DPS_SPAWN_COUNT; i++ )
	{
		//Keep monsters healed
		if ( pcUnitDataDps[i] )
		{
			//pcUnitData[i]->SetCurrentHealthToMax();
			pcUnitDataDps[i]->iAliveCounter = 256;
			pcUnitDataDps[i]->dwLastActiveTime = TICKCOUNT;
			pcUnitDataDps[i]->dwUpdateCharInfoTime = TICKCOUNT + ( 30 * 60 * 1000 ); //30 minute alive time
		}
	}
}

double CTestMapHandler::GetPlayerAccuracyPercent(User * pcUser, int iMonsterDefenseRating )
{
	const int baseline = 90; //90% accuracy is the base for players (if attack rating and defense is same)
	const int scalingAbove = 50;
	const int scalingBelow = 20;
	const int max = 100;
	const int min = 30;

	//if difference is +ve then monster's attack rating is higher
	//if difference is -ve then monster's attack rating is lower
	const int diff = pcUser->pcUserData->sCharacterData.iAttackRating - iMonsterDefenseRating;
	const int accuracy = baseline + ( diff / (diff > 0 ? scalingAbove : scalingBelow) );

	double ReturnValue = 0;

	if ( accuracy > max )
		ReturnValue = (double)max / 100;
	else if ( accuracy < min )
		ReturnValue = (double)min / 100;
	else
		ReturnValue = (double)accuracy / 100;

	return ReturnValue;
}


int CTestMapHandler::GetStat( std::string MonsterStatName, std::string MonsterName)
{
	int ReturnInt = 0;

	SQLConnection * pcDB2 = SQLCONNECTION( DATABASEID_GameDB_Monsters, 7 );

	if ( pcDB2->Open () )
	{
		const char * CurrentMonName = MonsterName.c_str ();
		const char * CurrentStatName = MonsterStatName.c_str ();

		if ( pcDB2->Prepare (FormatString ("SELECT [%s] FROM MonsterList WHERE Name=?",CurrentStatName) ))
		{
			pcDB2->BindParameterInput( 1, PARAMTYPE_String, (char*)CurrentMonName, 32 );
			//pcDB2->BindParameterInput ( 1, PARAMTYPE_String, &CurrentStatName, STRLEN( CurrentStatName ) );
			//pcDB2->BindParameterInput ( 2, PARAMTYPE_String, &CurrentMonName, STRLEN( CurrentMonName )  );
			if ( pcDB2->Execute() )
			{
				if ( pcDB2->Fetch () )
				{
					if ( MonsterStatName == "EXP" )
					{
						//Get Experience
						char  CurrentExpChar[50] = { 0 };
						pcDB2->GetData ( 1, PARAMTYPE_String, CurrentExpChar, 50 );
						ReturnInt = atoi ( CurrentExpChar ) ;
					}
					else
					{
						pcDB2->GetData ( 1, PARAMTYPE_Integer, &ReturnInt );
					}
				}
			}
		}

		pcDB2->Close();
	}

	return ReturnInt;


}

BOOL CTestMapHandler::GetExperienceRatePerFlag ( User * pcUser, char * DPS, char * HSFlagCount )
{
	// /exprate
	int iDPS = atoi(DPS);
	if ( iDPS == 0 )
		return 0;

	EMapID iMapId = pcUser->eMapID;

	int iLevel = pcUser->pcUserData->sCharacterData.iLevel;
	int iAtkRtg = pcUser->pcUserData->sCharacterData.iAttackRating;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB_Monsters, 8 );

	std::vector<int> MonsterCountList;
	std::vector<string> MonsterNameList;

	if ( pcDB->Open () )
	{

		if ( pcDB->Prepare ( FormatString ( "SELECT [Monster1],[Count1],[Monster2],[Count2],[Monster3],[Count3],[Monster4],[Count4],[Monster5],[Count5],[Monster6],[Count6],[Monster7],[Count7],[Monster8],[Count8],[Monster9],[Count9],[Monster10],[Count10],[Monster11],[Count11],[Monster12],[Count12] FROM MapMonster WHERE Stage=%d", iMapId ) ) )
		{

			if ( pcDB->Execute () && pcDB->Fetch () )
			{
				int j = 0;
				for ( int i = 0; i < 12; i++ )
				{
					string CurrentMon = "";
					int CurrentCount = 0;
					char  szMonsterName[150] = { 0 };

					pcDB->GetData ( j + 1, PARAMTYPE_String, szMonsterName, 150 );
					pcDB->GetData ( j + 2, PARAMTYPE_Integer, &CurrentCount );

					CurrentMon = std::string ( szMonsterName );
					if ( CurrentMon != "" )
					{
						MonsterNameList.push_back ( CurrentMon );
						MonsterCountList.push_back ( CurrentCount );
					}
					j += 2;
				}
			}
		}

		pcDB->Close();
	}


	std::vector<int> MonsterExpList;
	std::vector<int> MonsterHealthListNoMiss;
	std::vector<int> MonsterHealthListMiss;
	std::vector<int> MonsterAbsList;
	std::vector<int> MonsterSpawnMinList;
	std::vector<int> MonsterSpawnMaxList;
	std::vector<int> MonsterLevelList;
	std::vector<int> MonsterDefenseList;
	std::vector<int> MonsterBlockList;

	for ( string sMonsterName : MonsterNameList )
	{
		MonsterExpList.push_back ( GetStat ( "EXP", sMonsterName ));
		MonsterHealthListNoMiss.push_back ( GetStat ( "HealthPoint", sMonsterName ));
		MonsterHealthListMiss.push_back ( GetStat ( "HealthPoint", sMonsterName ));
		MonsterAbsList.push_back ( GetStat ( "Absorb", sMonsterName ));
		MonsterSpawnMinList.push_back ( GetStat ( "SpawnMin", sMonsterName ));
		MonsterSpawnMaxList.push_back ( GetStat ( "SpawnMax", sMonsterName ));
		MonsterLevelList.push_back ( GetStat ( "Level", sMonsterName ));
		MonsterDefenseList.push_back ( GetStat ( "Defense", sMonsterName ));
		MonsterBlockList.push_back ( GetStat ( "Block", sMonsterName ));
	}

	//Adjust Count based on spawn count
	std::vector<int> MonsterCountListRevised;
	int i = 0;
	for ( int MonsterCount : MonsterCountList )
	{
		MonsterCountListRevised.push_back ( MonsterCount * ((MonsterSpawnMinList[i] + MonsterSpawnMaxList[i])/2) );
		i++;
	}
	MonsterCountList = MonsterCountListRevised;

	//Modify Health Based on Abs, block, and accuracy
	std::vector<int> MonsterHealthListNoMissRevised;
	std::vector<int> MonsterHealthListMissRevised;
	i = 0;
	for ( int MonsterHealth : MonsterHealthListNoMiss )
	{
		double Accuracy = GetPlayerAccuracyPercent ( pcUser, MonsterDefenseList[i] ) ;
		MonsterHealthListNoMissRevised.push_back ( (int)((double)MonsterHealth / (1-((double)MonsterAbsList[i]/100)) / (1 - ((double)MonsterBlockList[i]/100))) );
		MonsterHealthListMissRevised.push_back ( (int)((double)MonsterHealth / ( 1 - ( (double)MonsterAbsList[i] / 100 ) ) / ( 1 - ( (double)MonsterBlockList[i] / 100 ) ) / Accuracy));
		i++;
	}
	MonsterHealthListNoMiss = MonsterHealthListNoMissRevised;
	MonsterHealthListMiss = MonsterHealthListMissRevised;

	//Get Sums
	int TotalExp = 0;
	int TotalHPNoMiss = 0;
	int TotalHPMiss = 0;
	i = 0;
	for ( int MonsterHealth : MonsterHealthListNoMiss )
	{
		TotalExp += MonsterExpList[i] * MonsterCountList[i];
		TotalHPNoMiss += MonsterHealthListNoMiss[i] * MonsterCountList[i];
		TotalHPMiss += MonsterHealthListMiss[i] * MonsterCountList[i];
		i++;
	}

	int TotalMonCount = 0;
	for ( int MonsterCount : MonsterCountList )
		TotalMonCount += MonsterCount;

	int WeightedExp = TotalExp / TotalMonCount;
	int WeightedHealthNoMiss = TotalHPNoMiss / TotalMonCount;
	int WeightedHealthMiss = TotalHPMiss / TotalMonCount;

	//Calculate Kill Speed
	int TestSeconds = 300;

	int SecondsToKillNoMiss = static_cast<int>( ceil( WeightedHealthNoMiss / iDPS ) );
	if ( SecondsToKillNoMiss < 1 )
		SecondsToKillNoMiss = 1;
	int AmountKilledNoMiss = static_cast<int>( floor( TestSeconds / ( SecondsToKillNoMiss + 8 ) ) );
	int TextExpGainedNoMiss = AmountKilledNoMiss * WeightedExp;
	int XpGainedFinalNoMiss = TextExpGainedNoMiss/TestSeconds;


	int SecondsToKillMiss = static_cast<int>(ceil(WeightedHealthMiss / iDPS));
	if ( SecondsToKillMiss < 1 )
		SecondsToKillMiss = 1;
	int AmountKilledMiss = static_cast<int>( floor( TestSeconds / ( SecondsToKillMiss + 8 ) ) );
	int TextExpGainedMiss = AmountKilledMiss * WeightedExp;
	int XpGainedFinalMiss = TextExpGainedMiss/TestSeconds;

	int iHSFlagCount = atoi ( HSFlagCount );

	CHATSERVER->SendChatEx ( pcUser, CHATCOLOR_Error, "> Exp Per Second Per %d Flag HS", iHSFlagCount);
	CHATSERVER->SendChatEx ( pcUser, CHATCOLOR_Error, "> No Miss: %d", XpGainedFinalNoMiss * iHSFlagCount);
	CHATSERVER->SendChatEx ( pcUser, CHATCOLOR_Error, "> Miss: %d", XpGainedFinalMiss * iHSFlagCount);


	return TRUE;
}

