#include "stdafx.h"
#include "CharacterGame.h"

typedef int( __thiscall *tfnChangeMotion )(void * ptr, int iMotion, int iAnimation);
tfnChangeMotion fnChangeMotion = (tfnChangeMotion)0x0041F930;

typedef void( __thiscall *tfnMessageBoxTitle ) (DWORD dwClass, int iID, char * pszTitle);
tfnMessageBoxTitle MessageBoxTitle = (tfnMessageBoxTitle)0x004D1680;

DWORD dwLastSaveTime = 0;
DWORD dwLastNonIdleTime = 0;

// Static flag for Discord character login tracking
static bool bDiscordCharacterLoginSent = false;

// Function to reset Discord character login flag
void ResetDiscordCharacterLoginFlag()
{
	bDiscordCharacterLoginSent = false;
}

NAKED INT64 CharacterGame::ExpXor()
{
	JMP( 0x00459820 );
}

CharacterGame::CharacterGame()
{
	ZeroMemory( &sCharacterDataEx, sizeof( CharacterDataEx ) );
	ZeroMemory( &sCharacterCombatData, sizeof( PacketCharacterCombatData ) );

	dwForceOrbTotalDuration = 0;
	dwForceOrbTimeRemaining = 0;
	dwForceOrbTimeRemainingAtCity = 0;

	sCharacterCombatData.iLength = sizeof( PacketCharacterCombatData );
	sCharacterCombatData.iHeader = PKTHDR_CharacterCombatData;

	dwLastCombatDataSaveTime = 0;

	bMPDown = FALSE;
	bSPDown = FALSE;
	bHPUp = FALSE;
	bMPUp = FALSE;
	bTopPVP = FALSE;
	bHoneyDamage = FALSE;
	bHoneyFast = FALSE;
	bHoneySmart = FALSE;
	bHoneyDefense = FALSE;
	bWeightStone = FALSE;
	bPhoenixFireSmall = FALSE;
	bPhoenixIceSmall = FALSE;
	bPhoenixHealingSmall = FALSE;
	bPhoenixLightningSmall = FALSE;

	bSHPRegen = FALSE;
	bSSPRegen = FALSE;
	bSMPRegen = FALSE;
	bSMPDown = FALSE;
	bSSPDown = FALSE;
	bSBonusDamage = FALSE;
	bSBonusAbsorb = FALSE;
	bSMoveSpeed = FALSE;

	bIceResistanceBuff = FALSE;
}

CharacterGame::~CharacterGame()
{
}

/// <summary>
/// Every 1 second
/// </summary>
void CharacterGame::Tick1s()
{
	SaveTimer();

	//need some logic here to load the required data from server
	//for loaded to be 'true'
	//todo: flags
	//check every 1s
	//check vs last requested

}

void CharacterGame::HandlePacket( Packet * psPacket )
{

}

void CharacterGame::OnCharacterUpdateData()
{
	CGameProtect::SetDecreaseMP( 0.0f );
	CGameProtect::SetDecreaseSP( 0.0f );

	UnitData * pcUnitData = UnitGame::GetInstance ()->pcUnitData;

	// Discord character login integration
	if (!bDiscordCharacterLoginSent && pcUnitData && pcUnitData->sCharacterData.szName[0] != '\0' && pcUnitData->sCharacterData.iLevel > 0)
	{
		// Get character class name
		std::string sClassName = "Unknown";
		switch (pcUnitData->sCharacterData.iClass)
		{
		case 1: sClassName = "Fighter"; break;
		case 2: sClassName = "Archer"; break;
		case 3: sClassName = "Pikeman"; break;
		case 4: sClassName = "Atalanta"; break;
		case 5: sClassName = "Knight"; break;
		case 6: sClassName = "Magician"; break;
		case 7: sClassName = "Priestess"; break;
		case 8: sClassName = "Assassin"; break;
		case 9: sClassName = "Shaman"; break;
		default: break;
		}

		// Send character login to Discord
		GAMEDISCORD->OnCharacterLogin(
			std::string(pcUnitData->sCharacterData.szName),
			pcUnitData->sCharacterData.iLevel,
			sClassName
		);

		bDiscordCharacterLoginSent = true;
	}

	//JLM - Hack to fix the pet crash bug
	//The reason for the crash is PetKind is not
	// within 1-4 range for some reason...
	if (pcUnitData->sCharacterData.iLevel > 0 &&
		pcUnitData->sCharacterData.iLevel < 10)
	{
		auto petKind = (*(INT*)(0x035E085C));
		auto petShow = (*(INT*)(0x035E0860));

		if (petShow)
		{
			if (petKind < 0 || petKind > 4)
			{
				petKind = Dice::RandomI(1, 4);
				(*(INT*)(0x035E085C)) = petKind;
			}
		}
	}


	//T5
	SetFreeSkillPointForTier5( 0 );
	if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_T5QuestArena ) )
	{
		int iPoints = 0;

		if ( pcUnitData->sCharacterData.iLevel >= 100 )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AMatterOfStrenght ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_CryMeARiver ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_TheLastStage ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnAncientThreat ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnEndlessBattle ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnEndlessSuffering ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_ATestOfBravery ) )
			iPoints++;
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_GreenLiquidQuestStep2 ) )
			iPoints += 2;

		iPoints += (pcUnitData->sCharacterData.iLevel - 100) / 5;

		for ( int i = 16; i < 20; i++ )
			iPoints -= SKILLSCHARACTER[i + 1].iLevel;

		SetFreeSkillPointForTier5( iPoints );
	}

	//Weapon Using
	{
		if ( INVENTORYITEMSLOT[0].iItemIndex )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];

			WRITEDWORD( 0x04B0DA14, 0 );
			if ( pcItemData->bValid )
			{
				WRITEDWORD( 0x04B0DA14, pcItemData->sItem.sItemID.ToInt() );

				if ( pcItemData->sItem.sItemID.ToInt () != iCurrentWeaponID )
				{
					if ( iCurrentWeaponID != 0 )
					{
						//force sync of client to server whenever the weapon slot is changed
						UNITGAME->SendUnitDataEx ( TRUE );
					}

					iCurrentWeaponID = pcItemData->sItem.sItemID.ToInt ();
				}
			}
		}
	}


	if ( (*(DWORD*)0x04B0719C) && !TIMERSKILLHANDLER->IsSkillTimerActive( ETimerID::DrasticSpirit ) )
	{
		__asm
		{
			PUSHAD;
			MOV EAX, DWORD PTR DS : [0x04B0719C];
			PUSH EAX;
			MOV ECX, 0x039032CC; //particle system? 0x039032CC
			MOV EAX, 0x00598B80;
			CALL EAX;

			MOV EAX, DWORD PTR DS : [0x04B0719C];
			PUSH EAX;
			MOV ECX, 0x039032CC; //particle system? 0x039032CC
			MOV EAX, 0x00598B40;
			CALL EAX;
			POPAD;
		};

		(*(DWORD*)0x04B0719C) = 0;
	}

	// Premium Data
	if ( Game::GetGameMode() == GAMEMODE_InGame && pcUnitData->bActive )
	{
		if ( pcUnitData->bBlessCastleCrown == FALSE )
		{
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_AbsorptionOfGlory );
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_PowerSiege );
		//	TIMERSKILLHANDLER->KillTimer( SKILLID_EvasionAdaption );
		}

		if ( bHPUp )
			pcUnitData->sCharacterData.fHPRegen += 5.0f;

		if ( bMPUp )
			pcUnitData->sCharacterData.fMPRegen += 5.0f;

		WRITEDWORD( pdwMPDownTime, 0 );
		WRITEDWORD( pdwMPDownPercent, 0 );

		WRITEDWORD( pdwSPDownTime, 0 );
		WRITEDWORD( pdwSPDownPercent, 0 );

		if ( bMPDown )
		{
			WRITEDWORD( pdwMPDownTime, 1 );
			WRITEDWORD( pdwMPDownPercent, 25 );
		}

		if ( bSPDown )
		{
			WRITEDWORD( pdwSPDownTime, 1 );
			WRITEDWORD( pdwSPDownPercent, 25 );
		}

		if (bTopPVP)
		{
			UNITDATA->sCharacterData.sHP.sMax += 100;
		}

		if ( bHoneyDamage )
		{
			UNITDATA->sCharacterData.iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 10) / 100;
			UNITDATA->sCharacterData.iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 10) / 100;
		}

		//Place holder for 10% EXP gain.
		/*if (bHoneyFast)
		{
			UNITDATA->sCharacterData.iAttackSpeed += (UNITDATA->sCharacterData.iAttackSpeed * 50) / 100;
		}*/

		// SERVER WIDE BUFFS
		if (bSHPRegen)
		{
			pcUnitData->sCharacterData.fHPRegen += 3.0f;
		}
		if (bSSPRegen)
		{
			pcUnitData->sCharacterData.fSPRegen += 3.0f;
		}
		if (bSMPRegen)
		{
			pcUnitData->sCharacterData.fMPRegen += 3.0f;
		}
		// SMPDown
		if (bSMPDown)
		{
			WRITEDWORD(pdwMPDownTime, 1);
			WRITEDWORD(pdwMPDownPercent, 10);
		}
		// SSPDown
		if (bSSPDown)
		{
			WRITEDWORD(pdwSPDownTime, 1);
			WRITEDWORD(pdwSPDownPercent, 10);
		}
		// SBonusDamage
		if (bSBonusDamage)
		{
			UNITDATA->sCharacterData.iMinDamage += (UNITDATA->sCharacterData.iMinDamage * 5) / 100;
			UNITDATA->sCharacterData.iMaxDamage += (UNITDATA->sCharacterData.iMaxDamage * 5) / 100;
		}
		// SBonusAbsorb
		if (bSBonusAbsorb)
		{
			UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * 5) / 100;
		}
		// SMoveSpeed
		if (bSMoveSpeed)
		{
			UNITDATA->sCharacterData.iMovementSpeed += 2;
		}

		if (bIceResistanceBuff && MAP_ID == MAPID_GallubiaValley )
		{
			UNITDATA->sCharacterData.sElementalDef[3]/*3 = Ice*/ += 70;
		}

		if (bIceResistanceBuff && MAP_ID == MAPID_FrozenSanctuary )
		{
			UNITDATA->sCharacterData.sElementalDef[3]/*3 = Ice*/ += 70;
		}

		if (bIceResistanceBuff && MAP_ID == MAPID_IceMineF1 )
		{
			UNITDATA->sCharacterData.sElementalDef[3]/*3 = Ice*/ += 70;
		}

		if ( bHoneySmart )
		{
			UNITDATA->sCharacterData.sMP.sMax += 50;
			UNITDATA->sCharacterData.fMPRegen += 5.0f;
		}

		if (bHoneyDefense)
		{
			UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * 10) / 100;
		}

		if (bWeightStone)
		{
			UNITDATA->sCharacterData.sWeight.sMax += 250;
		}

		//Hats
		ItemTimer * pcITM = ITEMTIMERHANDLER->GetHead();
		if ( pcITM )
		{
			// Cartola?
			switch ( pcITM->iType )
			{
				case ITEMTIMERTYPE_CartolaHat:
				case ITEMTIMERTYPE_WitchHat:
					pcUnitData->sCharacterData.fHPRegen += 5.0f;
					pcUnitData->sCharacterData.sHP.sMax += 10;
					break;

				case ITEMTIMERTYPE_ChristmasGreenHat:
					pcUnitData->sCharacterData.fHPRegen += 10.0f;
					pcUnitData->sCharacterData.sHP.sMax += 10;
					break;

				case ITEMTIMERTYPE_ChristmasRedHat:
					pcUnitData->sCharacterData.sMP.sMax += 50;
					pcUnitData->sCharacterData.sSP.sMax += 50;
					break;

				case ITEMTIMERTYPE_SoccerHat:
					UNITDATA->sCharacterData.sWeight.sMax += 150;
					break;

				case ITEMTIMERTYPE_SoccerHatSpeed:
					UNITDATA->sCharacterData.iMovementSpeed += 2;
					break;

				case ITEMTIMERTYPE_SheepHat:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iDefenseRating += 150;
					break;

				case ITEMTIMERTYPE_GiraffeHat:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iAttackRating += 75;
					break;

				case ITEMTIMERTYPE_BigHeadHappiness:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iAttackRating += 50;
					break;

				case ITEMTIMERTYPE_BigHeadLove:
					pcUnitData->sCharacterData.sWeight.sMax += 150;


					//NITDATA->sCharacterData.sHP.sMax += 50;
					//pcUnitData->sCharacterData.fHPRegen += 10.0f;

					break;

				case ITEMTIMERTYPE_BigHeadValentine:
					pcUnitData->sCharacterData.sWeight.sMax += 150;

					//if valentine's event on

					pcUnitData->sCharacterData.sHP.sMax += 100;
					pcUnitData->sCharacterData.fHPRegen += 5.0f;
					pcUnitData->sCharacterData.fMPRegen += 5.0f;

					break;

				case ITEMTIMERTYPE_BigHeadSadness:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.sMP.sMax += 10;
					//pcUnitData->sCharacterData.fMPRegen += 10.0f;
					break;

				case ITEMTIMERTYPE_BigHeadShyness:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iDefenseRating += 100;
					break;

				case ITEMTIMERTYPE_BigHeadAngry:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.sHP.sMax += 50;
					//pcUnitData->sCharacterData.sSP.sMax += 50;
					//pcUnitData->sCharacterData.sMP.sMax += 50;
					break;

				case ITEMTIMERTYPE_BigHeadSurprised:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iBlockRating += 2;
					break;

				case ITEMTIMERTYPE_BigHeadSensual:
					pcUnitData->sCharacterData.sWeight.sMax += 150;
					//pcUnitData->sCharacterData.iCritical += 2;
					break;
			}
		}

		//Bless Castle Buffs
		if ( MAP_ID != MAPID_Bellatra )
		{
			switch ( BLESSCASTLEHANDLER->GetCrownSkillID( UNITDATA->sCharacterData.iClanID ) )
			{
				case SKILLID_BlessCastleBuff2:
					//UNITDATA->sCharacterData.iAttackSpeed++;
					PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 6) / 100;
					PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 6) / 100;
					break;

				case SKILLID_BlessCastleBuff3:
					//UNITDATA->sCharacterData.iMovementSpeed++;
					PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 3) / 100;
					PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 3) / 100;
					break;

				default:
					break;
			}
		}


		//Regen HP/SP/MP in Test map. Only for GM
#ifdef DEV_MODE

		if ( GM_MODE && MAP_ID == MAPID_QuestArena )
		{
			CHARACTERGAME->SetCurrentHP( UNITDATA->GetMaxHealth() );
			CHARACTERGAME->SetCurrentSP( UNITDATA->GetMaxStamina());
			CHARACTERGAME->SetCurrentMP( UNITDATA->GetMaxMana());
		}

#endif // DEV_MODE
	}

	// Skill Timers
	{
		Skill * skill = NULL;

		static int iaVLBackup[10];

		int * piVLReduction = (int *)( 0x04B0DCB4 );
		int * piVL = (int *)( 0x00996E28 );

		if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SummonMuspell ) )
		{
			int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SummonMuspell );

			if ( iLevelSkill > 0 )
			{
				if ( iaVLBackup[9] == 0 )
					CopyMemory( iaVLBackup, piVL, sizeof( int ) * 10 );

				if ( iaVLBackup[9] == piVL[9] )
				{
					for ( int i = 0; i < 10; i++ )
						piVL[i] = piVL[i] - ( ( piVL[i] * abs( piVLReduction[i] ) ) / 100 );
				}
			}
		}
		else
		{
			if ( iaVLBackup[9] != 0 && iaVLBackup[9] != piVL[9] )
				CopyMemory( piVL, iaVLBackup, sizeof( int ) * 10 );
		}


		UNIT->iEvadeRating = 0;


		if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraSoloGold ) )
		{
			if ( MAP_ID != MAPID_Bellatra &&
				MAP_ID != MAPID_BlessCastle )
			{
				//note - absorb bonus is handled in UnitGame::GetCharacterAbsorption()
				pcUnitData->sCharacterData.iMovementSpeed += 5;

				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_Orange );
			}
			else
			{
				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_White );
			}
		}

		if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraSoloSilver ) )
		{
			if ( MAP_ID != MAPID_Bellatra &&
				MAP_ID != MAPID_BlessCastle )
			{
				//note - absorb bonus is handled in UnitGame::GetCharacterAbsorption()
				pcUnitData->sCharacterData.iMovementSpeed += 3;
				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_Orange );
			}
			else
			{
				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_White );
			}
		}

		if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraSoloBronze ) )
		{
			if ( MAP_ID != MAPID_Bellatra &&
				MAP_ID != MAPID_BlessCastle )
			{
				//note - absorb bonus is handled in UnitGame::GetCharacterAbsorption()
				pcUnitData->sCharacterData.iMovementSpeed += 2;
				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_Orange );
			}
			else
			{
				SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Speed, CHARSTATUSCOLOR_White );
			}
		}

		SKILLMANAGERHANDLER->ApplySkillBuffToSelf();
		SKILLMANAGERHANDLER->ApplyPassiveSkillToSelf();


		if ( pcUnitData->sCharacterData.iAbsorbRating != CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating )
		{
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_Blue );
		}
		else
		{
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_White );
		}

		//ABS Sod
		/*if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_AvertScroll ) )
		{
			if (MAP_ID == MAPID_Bellatra)
			{
				pcUnitData->sCharacterData.iAbsorbRating += (pcUnitData->sCharacterData.iAbsorbRating * 10) / 100;
			}
		}*/

	}

	if ( UNITDATA && UNITDATA->psModelAnimation )
	{
		if ( UNITDATA->psModelAnimation->iType != ANIMATIONTYPE_None &&
			 UNITDATA->psModelAnimation->iType != ANIMATIONTYPE_Idle )
		{
			dwLastNonIdleTime = TICKCOUNT;
		}

		if ( UNITDATA->psModelAnimation->iType == ANIMATIONTYPE_Die)
			TIMERSKILLHANDLER->OnCharacterKilled();
	}



	//Change Map?
	if ( GetLastStageID() != MAP_ID )
	{
		WRITEDWORD( 0x00A17650, 0 );
		WRITEDWORD( 0x00A17654, 0 );
		WRITEDWORD( 0x00A17658, 0 );
		WRITEDWORD( 0x00A1765C, 0 );

		//Update Handlers
		HUDHANDLER->GetMiniMapHandler()->OnChangeMap( MAP_ID );

		// Update Discord status for map change
		if ( MAP_ID >= 0 && MAP_ID < (sizeof(pszaMapsName) / sizeof(pszaMapsName[0])) )
		{
			GAMEDISCORD->OnMapChange( std::string(pszaMapsName[MAP_ID]) );
		}

		if ( IsStageVillage() )
		{
			SKILLMANAGERHANDLER->ResetSkillEffectParticle( SKILLEFFECTID_HolyValorAction );
			WIZARDTRANCEHANDLER->OnCastEnd( UNIT );
		}

		SetLastStageID( MAP_ID );

		if ( pcUnitData->sCharacterData.iLevel >= 2 )
		{
			UPDATE_CHARACTER_CHECKSUM;

			//Save char whenever map is changed
			//so that on login, map will be restored
			//important note - do not set to iLevel >= 1
			// because it causes char validation failure for a brand new char!
			SAVE;
			SAVEEX;
		}
	}

	CHECK_CHARACTER_CHECKSUM;
}

void CharacterGame::OnCharacterSetCharInfoEx()
{
	/*
	4B0C5EC CRIT
	4B0C4B0 BLOCK
	4B0C210 EVADE
	*/
	int * piCritical = (int*)0x04B0C5EC;
	int * piBlock = (int*)0x04B0C4B0;
	int * piEvade = (int*)0x04B0C210;

	*piEvade += UNIT->iEvadeRating;

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_EvasionAdaption ) )
	{
		*piEvade += 10;
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_DeadlyEdge ) )
	{
		*piCritical += 5;
	}

	*piCritical += (*(int*)0x0362A2D0);

	if ( *piCritical > 100 )
		*piCritical = 100;

	if ( *piBlock > 50 )
		*piBlock = 50;

	if ( *piEvade > 50 )
		*piEvade = 50;
}

BOOL CharacterGame::IsStageVillage()
{
	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return FALSE;

	if ( UNITDATA->iLoadedMapIndex >= 0 )
	{
		DWORD dwMapType = *(DWORD*)((UNITDATA->iLoadedMapIndex * 4) + 0x00CF4748);

		if ( dwMapType == 0 )
			return FALSE;

		if ( *(DWORD*)(dwMapType + 0x0C4) == 0x100 )
			return TRUE;
	}
	return FALSE;
}

int CharacterGame::GetStageID()
{
	if ( UNITDATA->iLoadedMapIndex <= (-1) )
		return -1;

	DWORD dwPtr = *(int*)((UNITDATA->iLoadedMapIndex * 4) + 0x0CF4748);
	if ( dwPtr == 0 )
		return -1;

	return *(int*)(dwPtr + 0x1FD4);
}

int CharacterGame::GetBlockRating( int desLV )
{
	if ( UNITDATA->sCharacterData.iBlockRating == 0 )
		return 0;

	float fDesLV = (float)desLV;
	float fMyLV  = (float)UNITDATA->sCharacterData.iLevel;

	int value = static_cast<int>( UNITDATA->sCharacterData.iBlockRating + ( ( fMyLV - fDesLV ) / 100.0f ) * 20.0f );

	//cap
	if ( value > 50 )
		value = 50;

	return value;
}

int CharacterGame::GetEvadeRating( int desLV )
{
	if ( UNIT->iEvadeRating == 0 )
		return 0;

	float fDesLV = (float)desLV;
	float fMyLV  = (float)UNITDATA->sCharacterData.iLevel;

	int value = (int)(  UNIT->iEvadeRating + ( ( fMyLV - fDesLV ) / 100 ) * 20 );

	if ( value > 50 )
		value = 50;

	return value;
}


int CharacterGame::GetCurrentMP()
{
	int iMana = 0;
	__asm
	{
		mov eax, 0x00507760;
		call eax;
		movsx eax, ax;
		mov iMana, eax;
	}
	return iMana;
}

/// <summary>
/// 2012 func: sinGetLife
/// </summary>
/// <returns></returns>
int CharacterGame::GetCurrentHP()
{
	int iHP = 0;
	__asm
	{
		MOV EAX, 0x00507740;
		CALL EAX;
		MOVSX EAX, AX;
		MOV iHP, EAX;
	}
	return iHP;
}

/// <summary>
/// Apply reduction of current health points
/// based on percentage of max health.
/// Usually via skills, like Raving or Averging Crash
/// If the HP is less than 1, set to 1
/// </summary>
/// <returns></returns>
void CharacterGame::ApplyHPReduction( int iPercent )
{
	int max = UNITDATA->GetMaxHealth();
	int curr = CHARACTERGAME->GetCurrentHP();

	int value = (max * iPercent) / 100;

	curr = curr - value;

	//lowest hp after reduction can only be 1
	//only others can kill this unit not self..
	if ( curr < 1 )
		curr = 1;

	CHARACTERGAME->SetCurrentHP( curr );
}

/// <summary>
/// Remove HP value (usually via a skill)
/// if HP is below 1, set to 1
/// </summary>
void CharacterGame::RemoveHPValue( int iValue )
{
	int curr = CHARACTERGAME->GetCurrentHP();

	curr = curr - iValue;

	//lowest hp after reduction can only be 1
	//only others can kill this unit not self..
	if ( curr < 1 )
		curr = 1;

	CHARACTERGAME->SetCurrentHP( curr );
}

void CharacterGame::GiveHPValuePercent( int iPercent )
{
	if ( iPercent < 0 || iPercent > 100 )
		return;

	GiveHPValue( (UNITDATA->GetMaxHealth() * iPercent) / 100 );
}

/// <summary>
/// Give HP value with checks in place
/// </summary>
void CharacterGame::GiveHPValue( int iValue )
{
	int max = UNITDATA->GetMaxHealth();
	int value = CHARACTERGAME->GetCurrentHP() + iValue;

	if ( value > max )
		value = max;

	CHARACTERGAME->SetCurrentHP( value );
}

void CharacterGame::GiveSPValuePercent( int iPercent )
{
	if ( iPercent < 0 || iPercent > 100 )
		return;

	GiveSPValue( (UNITDATA->GetMaxStamina() * iPercent) / 100 );
}

/// <summary>
/// Give HP value with checks in place
/// </summary>
void CharacterGame::GiveSPValue( int iValue )
{
	int max = UNITDATA->GetMaxStamina();
	int value = CHARACTERGAME->GetCurrentSP() + iValue;

	if ( value > max )
		value = max;

	CHARACTERGAME->SetCurrentSP( value );
}

int CharacterGame::GetCurrentSP()
{
	int iSP = 0;
	__asm
	{
		MOV EAX, 0x00507770;
		CALL EAX;
		MOVSX EAX, AX;
		MOV iSP, EAX;
	}
	return iSP;
}

void CharacterGame::SetCurrentMP( int iMP )
{
	CALL_WITH_ARG1( 0x00507820, (DWORD)iMP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)1 ); //ResetEnergyGraph(1)
}

/// <summary>
/// Sets current HP points
/// 2012 func: sinSetLife
/// </summary>
void CharacterGame::SetCurrentHP( int iHP )
{
	CALL_WITH_ARG1( 0x00507780, (DWORD)iHP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)0 ); //ResetEnergyGraph(0)
}

void CharacterGame::SetCurrentSP( int iSP )
{
	CALL_WITH_ARG1( 0x005078B0, (DWORD)iSP );
	CALL_WITH_ARG1( 0x0045D910, (DWORD)2 ); //ResetEnergyGraph(2)
}

void CharacterGame::SetPhoenix( BOOL bActive, int iPhoenixID )
{
	(*(DWORD*)piActivePhoenix) = iPhoenixID;

	if ( bActive )
		CALLT( pfnSetPhoenix, pcPhoenix );
	else
		CALLT( pfnDeletePhoenix, pcPhoenix );

	(*(BOOL*)pbShowPhoenixPet) = bActive;
}

void CharacterGame::Die()
{
	if ( UNITDATA && UNITDATA->psModelAnimation && UNITDATA->psModelAnimation->iType != ANIMATIONTYPE_Die )
	{
		UNITDATA->Animate( ANIMATIONTYPE_Die );

		SetCurrentHP( 0 );
		CHECK_CHARACTER_CHECKSUM;

		CALL( 0x0046CC4A );

		PLAYUNITSOUND( UNITDATA );
	}
}

void CharacterGame::RemoveHairCrown()
{
	if (ITEMTIMERHANDLER->GetHead() != NULL)
	{
		TITLEBOX("Removal of hair crown does not work while time-based head is active!");
		return;
	}

	EItemID itemId = ITEMHANDLER->GetHairPotionIDFromPath(UNITDATA->sCharacterData.Player.szHeadModel);

	if (itemId == EItemID::ITEMID_None)
	{
		TITLEBOX("Removal of hair crown failed. Report this to #bug-report");
		return;
	}

	if (ITEMHANDLER->SetHairModel((DWORD)itemId, 0))
	{
		//Save Old Head
		ITEMTIMERHANDLER->ProcessPacket(&PacketOldHeadUse(TRUE));
		STRINGCOPY(CHARACTERGAME->sCharacterDataEx.szOldHead, UNITDATA->sCharacterData.Player.szHeadModel);

		TITLEBOX("Removal of hair crown succesful!");
	}
	else
	{
		TITLEBOX("Removal of hair crown failed. Report this to #bug-report");
	}
}

void CharacterGame::ResetHairCrown()
{
	if (ITEMTIMERHANDLER->GetHead() != NULL)
	{
		TITLEBOX("Reset hair crown does not work while time-based head is active!");
		return;
	}

	EItemID itemId = ITEMHANDLER->GetHairPotionIDFromPath(UNITDATA->sCharacterData.Player.szHeadModel);

	if (itemId == EItemID::ITEMID_None)
	{
		TITLEBOX("Reset of hair crown failed. Report this to #bug-report");
		return;
	}

	if (ITEMHANDLER->SetHairModel((DWORD)itemId, UNITDATA->sCharacterData.iRank))
	{
		TITLEBOX("Reset hair crown successfull!");

		//Save Old Head
		ITEMTIMERHANDLER->ProcessPacket(&PacketOldHeadUse(TRUE));
		STRINGCOPY(CHARACTERGAME->sCharacterDataEx.szOldHead, UNITDATA->sCharacterData.Player.szHeadModel);
	}
	else
	{
		TITLEBOX("Reset of hair crown failed. Report this to #bug-report");
	}
}

void CharacterGame::ResetHead( BOOL bDefault )
{
	char szNewHeadModel[128] = { 0 };
	char szNewHeadModelBase[128] = { 0 };
	char szNewHeadModelBase1[128] = { 0 };
	std::string searchString = "";
	std::string searchString1 = "";

	//0x61 = a, rank
	//0x62 = b,
	//0x63 = c,
	//0x64 = d,
	char cTier[2] = { UNITDATA->sCharacterData.iRank ? 0x60 + (char)UNITDATA->sCharacterData.iRank : 0, 0 };
	char cChar[2] = { UNITDATA->sCharacterData.iRank == 2 ? '_' : '-', 0 };

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Mechanician:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Archer:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Pikeman:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier );

			if (UNITDATA->sCharacterData.iRank == 3)
			{
				STRINGFORMAT(szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf");
			}
			break;

		case CHARACTERCLASS_Atalanta:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Knight:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Magician:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Priestess:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Assassin:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier );
			break;

		case CHARACTERCLASS_Shaman:
			STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier );
			break;

		default:
			break;
	}

	//Old Head Not Null

	if ( sCharacterDataEx.szOldHead[0] != 0 && bDefault == FALSE )
	{
		UNITGAME->SetCharacterHeadModel(sCharacterDataEx.szOldHead);
		return;
	}

	// Set Head
	UNITGAME->SetCharacterHeadModel( szNewHeadModel );
}

void CharacterGame::SaveTimer ()
{
	//only save if client is in the foreground
	if ( GetForegroundWindow () != DX::Graphic::GetWindowHandler () )
	{
		return;
	}

	if ( dwLastSaveTime == 0 )
	{
		dwLastSaveTime = TICKCOUNT;
	}

	const int iDelayCheck = 60 * 1000;
	const int iIdleCheck = 61 * 1000;

	if ( dwLastNonIdleTime != 0 &&
		 TICKCOUNT < dwLastNonIdleTime + iIdleCheck &&
		 TICKCOUNT > dwLastSaveTime + iDelayCheck )
	{
		if ( GM_MODE )
		{
#ifdef DEV_MODE
			CHATBOX->AddMessage ( "Debug: SAVE (Timer)" );
#endif
		}

		dwLastNonIdleTime = 0;
		dwLastSaveTime = TICKCOUNT;
		Save ();
	}
}


void CharacterGame::SaveCustom ()
{
	if ( GM_MODE )
	{
#ifdef DEV_MODE
		CHATBOX->AddMessage ( "Debug: SAVE" );
#endif
	}

	dwLastNonIdleTime = 0;
	dwLastSaveTime = TICKCOUNT;
	Save ();
}


/// <summary>
/// 2012 source: int SaveGameData()
/// </summary>
/// <returns></returns>
NAKED void CharacterGame::Save()
{
	JMP( pfnSave );
}

void CharacterGame::SaveEx()
{
	PacketCharacterDataEx s;
	s.iLength = sizeof( PacketCharacterDataEx );
	s.iHeader = PKTHDR_CharacterDataEx;
	CopyMemory( &s.sData, &CHARACTERGAME->sCharacterDataEx, sizeof( CharacterDataEx ) );

	SENDPACKETL( &s );
}

/// <summary>
/// SendCombatData is called at a high rate (100ms)
/// but the data will only get sent to the server
/// if any parameters did change
/// </summary>
void CharacterGame::SyncCombatData()
{
	if ( PLAYERDATA == NULL || UNIT == NULL )
		return;

	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return;

	switch ( MAP_ID )
	{
		case EMapID::MAPID_RicartenTown:
		case EMapID::MAPID_PillaiTown:
			return;
	}

	BOOL bSend = FALSE;

	if ( sCharacterCombatData.sBlockChance != PLAYERDATA->iBlockRating )
	{
		sCharacterCombatData.sBlockChance = PLAYERDATA->iBlockRating;
		bSend = TRUE;
	}

	if ( sCharacterCombatData.iDefenseRating != PLAYERDATA->iDefenseRating )
	{
		sCharacterCombatData.iDefenseRating = PLAYERDATA->iDefenseRating;
		bSend = TRUE;
	}

	if ( sCharacterCombatData.sEvadeChance != UNIT->iEvadeRating)
	{
		sCharacterCombatData.sEvadeChance = UNIT->iEvadeRating;
		bSend = TRUE;
	}

	DWORD minSyncTimeMs = 10000; //10s

	if ( MAP_ID == MAPID_BlessCastle )
	{
		minSyncTimeMs = 5000; //5s
	}

	//force send data every 5 sec as minimum
	//just in case the instant change didn't get to the server..
	if ( dwLastCombatDataSaveTime > 0 &&
		TICKCOUNT - dwLastCombatDataSaveTime > minSyncTimeMs )
	{
		bSend = TRUE;
	}

	if ( bSend )
	{
		//update player's combat data on server side
		SENDPACKETG( &sCharacterCombatData );
		dwLastCombatDataSaveTime = TICKCOUNT;
	}
}

int CharacterGame::MakePlayBuffFromRecvData(UnitData* pcUnitData, PlayBuff* StartBuff, PlayBuff* EndBuff, int len)
{
	int cnt;
	int x, y, z;
	int px, py, pz;
	int mx, my, mz;
	int PlayBuffPosi;
	int aX, aY, aZ;
	int saX, saY, saZ;
	int taX, taY, taZ;
	int frameState;
	int frameStep;

	if ( !pcUnitData->psBody || len <= 1 ) return FALSE;

	if ( StartBuff->saAngle[2] > 0 )
	{
		CHATBOX->AddDebugMessage(" StartBuff->saAngle[2] = %d", StartBuff->saAngle[2]);
	}

	if ( EndBuff->saAngle[2] > 0 )
	{
		CHATBOX->AddDebugMessage(" EndBuff->saAngle[2] = %d", EndBuff->saAngle[2]);
	}

	//if EndBuff is not defined, then it's typically for standing motion only because there is only one PlayBuff
	//this can be used by potion, skill, attack too. because while attacking, the character cannot move and is facing the same direction.
	if ( !EndBuff )
	{
		PlayBuffPosi = pcUnitData->iPlayBuffPosiEnd & PLAYBUFF_MASK;

		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iX = StartBuff->sPosition.iX;
		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iY = StartBuff->sPosition.iY;
		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iZ = StartBuff->sPosition.iZ;

		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[0] = 0; //unused / spare
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[1] = StartBuff->saAngle[1] & PTANGLE_Mask;
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[2] = StartBuff->saAngle[2]; //skill parameter

		//action (animation code)
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[3] = StartBuff->saAngle[3];

		pcUnitData->saPlayBuff[PlayBuffPosi].iFrame = StartBuff->iFrame;
		pcUnitData->saPlayBuff[PlayBuffPosi].lTargetID = StartBuff->lTargetID;

		memcpy(pcUnitData->saPlayBuff[PlayBuffPosi].saEquippedItems, StartBuff->saEquippedItems, sizeof(short) * 4);
		pcUnitData->iPlayBuffPosiEnd++;

		return TRUE;
	}

	//From this point onwards, it's non-standing motion.

	px = StartBuff->sPosition.iX;
	py = StartBuff->sPosition.iY;
	pz = StartBuff->sPosition.iZ;

	//delta movement to make per frame step
	mx = ( ( EndBuff->sPosition.iX - px ) << 8 ) / len;
	my = ( ( EndBuff->sPosition.iY - py ) << 8 ) / len;
	mz = ( ( EndBuff->sPosition.iZ - pz ) << 8 ) / len;

	aX = 0; //StartBuff->saAngle[0];
	aY = StartBuff->saAngle[1];
	aZ = 0; //StartBuff->saAngle[2];

	//target angle to make
	taX = 0; // PTANGLE_180 - EndBuff->saAngle[0];
	taY = PTANGLE_180 - EndBuff->saAngle[1];
	taZ = 0; //PTANGLE_180 - EndBuff->saAngle[2];

	saX = 0; // ( aX + taX )& PTANGLE_Mask;
	saY = ( aY + taY ) & PTANGLE_Mask;
	saZ = 0; // ( aZ + taZ )& PTANGLE_Mask;

	//angle movement to make per frame step
	saX = 0; // ( ( saX - PTANGLE_180 ) << 8 ) / len;
	saY = ( ( saY - PTANGLE_180 ) << 8 ) / len;
	saZ = 0; // ( ( saZ - PTANGLE_180 ) << 8 ) / len;

	if ( StartBuff->iFrame < CHAR_FRAME_MASK )
	{
		frameState = pcUnitData->psBody->saModelAnimation[StartBuff->saAngle[3]].iType;

		if ( frameState == EAnimationType::ANIMATIONTYPE_Walking || frameState == EAnimationType::ANIMATIONTYPE_Running )
		{
			//adjust angle of character to be based from A to B
			aY = SKILLMANAGERHANDLER->GetRadian2D(StartBuff->sPosition.iX, StartBuff->sPosition.iZ, EndBuff->sPosition.iX, EndBuff->sPosition.iZ);
			saY = 0;
		}
	}

	x = 0;
	y = 0;
	z = 0;

	taX = 0;
	taY = 0;
	taZ = 0;

	cnt = len - 1;

	if ( cnt <= 0 )
	{
		frameStep = 80; //typical frame rate
	}
	else
	{
		//difference between this frame and next frame / len - 1
		//len is BuffCount difference of next vs curr
		//which is typically pBuffStep. In the end frameStep will be close to 80
		frameStep = ( EndBuff->iFrame - StartBuff->iFrame ) / cnt;
	}

	//caps
	if ( frameStep >= 4096 || frameStep < 20 )
	{
		frameStep = 80;
	}

	//note - pcUnitData->iPlayBuffPosiEnd is actually one position AFTER the previous start end position
	//because the previously sent packet is -1 from the actual last frame pos
	//int startBuff = pcUnitData->iPlayBuffPosiEnd;


	//len is FPS difference between curr buff and next buff
	//if we only received start and last buff, then the len would be 70
	//This code fills the gap basically. To stop jittery behavior
	for ( cnt = 0; cnt < len; cnt++ )
	{
		PlayBuffPosi = ( pcUnitData->iPlayBuffPosiEnd + cnt ) & PLAYBUFF_MASK;

		//pcUnitData->iPlayBuffPosiEnd is the last received PlayBuff pos for the target player.
		//if the buff counter goes past the end position, it would only increment frame by 80
		//until we get another packet data for the player

		//adjust positions
		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iX = px + ( x >> 8 );
		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iY = py + ( y >> 8 );
		pcUnitData->saPlayBuff[PlayBuffPosi].sPosition.iZ = pz + ( z >> 8 );

		//adjust angles. Only [1] is used for player heading
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[0] = 0; //was aX & PTANGLE_Mask;
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[1] = aY & PTANGLE_Mask;
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[2] = StartBuff->saAngle[2]; //skill parameter; was aZ & PTANGLE_Mask

		//this is the 'action' code
		pcUnitData->saPlayBuff[PlayBuffPosi].saAngle[3] = StartBuff->saAngle[3];

		//smPLAYBUFF_FRAME_NONE basically tells how much frame steps to do each tick, rather than using actual frame numbers.
		//see code related to `if ( playBuff->frame<smPLAYBUFF_FRAME_NONE ) {
		//it basically does nothing special. just increment frames.
		pcUnitData->saPlayBuff[PlayBuffPosi].iFrame = smPLAYBUFF_FRAME_NONE | frameStep;

		//if the unit should be focusing on a character or not
		pcUnitData->saPlayBuff[PlayBuffPosi].lTargetID = StartBuff->lTargetID;

		memcpy(pcUnitData->saPlayBuff[PlayBuffPosi].saEquippedItems, StartBuff->saEquippedItems, sizeof(short) * 4);

		x += mx;
		y += my;
		z += mz;

		taX += 0; // saX;
		taY += saY;
		taZ += 0; // saZ;
	}



	//sets actual iFrame for first
	pcUnitData->saPlayBuff[pcUnitData->iPlayBuffPosiEnd & PLAYBUFF_MASK].iFrame = StartBuff->iFrame;

	//sets actual iFrame for last
	pcUnitData->saPlayBuff[( pcUnitData->iPlayBuffPosiEnd + len - 1 ) & PLAYBUFF_MASK].iFrame = EndBuff->iFrame;

	//UNITGAME->Log( "start action: %d, end action: %d", StartBuff->saAngle[3], EndBuff->saAngle[3] );
	//UNITGAME->Log( "iPlayBuffPosiEnd: %d + %d = %d", pcUnitData->iPlayBuffPosiEnd, len, ( pcUnitData->iPlayBuffPosiEnd + len ) );

	//updates iPlayBuffPosiEnd to reflect received data
	pcUnitData->iPlayBuffPosiEnd = pcUnitData->iPlayBuffPosiEnd + len;

	return TRUE;
}

NAKED BOOL CharacterGame::ChecksumUpdate( CharacterData * psCharacterData )
{
	JMP( pfnChecksumUpdate );
}

BOOL __cdecl CharacterGame::ChecksumCheck()
{
	ANTICHEATHANDLER->CheckThread();
	ANTICHEATHANDLER->CheckTimingSpeedHack();

	return UPDATE_CHARACTER_CHECKSUM;
}

void CharacterGame::UpdateWeight()
{
	typedef void( __thiscall *tfnCheckWeight )(DWORD dwClass);
	tfnCheckWeight fnCheckWeight = (tfnCheckWeight)0x004AD2E0;
	fnCheckWeight( 0x035EBB20 );
}

void CharacterGame::ReFormMousePotionNum() ////Check when potion is set on the mouse
{
	typedef void( __thiscall *tfnReFormMousePotionNum )(DWORD dwClass);
	tfnReFormMousePotionNum fnReFormMousePotionNum = (tfnReFormMousePotionNum)0x004B0980;
	fnReFormMousePotionNum( 0x035EBB20 );
}


/// <summary>
/// Check the 15 slots (equipment) for space
/// </summary>
BOOL CharacterGame::CheckEquipmentSpace( ItemData * psItemData )
{
	for (int i=0 ; i < 15; i++)
	{
		if(psItemData->iItemSlotFlag & INVENTORYITEMSLOT[i].iItemSlot)
		{
			if(!INVENTORYITEMSLOT[i].iItemIndex)
			{
				//If the item can not be set on / hands region
				if (psItemData->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand)
				{
					if ( INVENTORYITEMSLOT[0].iItemIndex || INVENTORYITEMSLOT[1].iItemIndex )
						return FALSE;
				}

				//If you can not set a shortage stats
				if(psItemData->sItem.bCanNotUse)
					return FALSE;

				return TRUE;
			}
		}
	}

	return FALSE;
}

/// <summary>
/// Leaked func: int cINVENTORY::InvenEmptyAearCheck(sITEM *pItem)
/// Check the void in the inventories
/// </summary>
BOOL CharacterGame::CheckInventorySpace( ItemData * psItemData, BOOL bUseTemporaryItem )
{
	ItemData sItemDataTemp;
	CopyMemory( &sItemDataTemp, psItemData, sizeof( ItemData ) );
	typedef BOOL( __thiscall *tfnCheckInventorySpace )(DWORD dwClass, ItemData * psItemData);
	tfnCheckInventorySpace fnCheckInventorySpace = (tfnCheckInventorySpace)0x004B0C60;

	if ( bUseTemporaryItem )
		return fnCheckInventorySpace( 0x035EBB20, &sItemDataTemp );

	return fnCheckInventorySpace( 0x035EBB20, psItemData );
}

BOOL CharacterGame::CheckInventoryWeight( ItemData * psItemData )
{
	if ( psItemData == NULL )
		return FALSE;

	EItemBase eItemBase = psItemData->sBaseItemID.ToItemBase();

	if ( eItemBase == ITEMBASE_Potion )
	{
		if ( (UNITDATA->sCharacterData.sWeight.sCur + psItemData->sItem.iPotionCount) > UNITDATA->sCharacterData.sWeight.sMax )
			return FALSE;
	}

	if ( (UNITDATA->sCharacterData.sWeight.sCur + psItemData->sItem.iWeight) > UNITDATA->sCharacterData.sWeight.sMax )
		return FALSE;

	if ( psItemData->sBaseItemID.ToItemID() == ITEMID_VampireSuit )
	{
		if ( (UNITDATA->sCharacterData.sWeight.sCur + 10) > UNITDATA->sCharacterData.sWeight.sMax )
			return FALSE;
	}

	return TRUE;
}

int CharacterGame::OnMotionChange( UnitData * pcUnitGame, int iMotion, int iAnimationID )
{
	DWORD dwCodeMotion = (*(DWORD*)(0x04B07000));

	if ( pcUnitGame )
	{
		// Assassin
		if ( pcUnitGame->sCharacterData.iClass == CHARACTERCLASS_Assassin )
		{
			if ( !IsStageVillage() && ((pcUnitGame->sRightHandTool.eItemID & 0xFFFF0000) == 0x010A0000) )
			{
				switch ( dwCodeMotion )
				{
					case ANIMATIONTYPE_Idle:
						iMotion = 47;
						break;
					case ANIMATIONTYPE_Walking:
						iMotion = 16;
						break;
					case ANIMATIONTYPE_Running:
						iMotion = 36;
						break;
					case ANIMATIONTYPE_Attack:
						iMotion = 69;
						break;
				}
			}
		}

		if ( iMotion < 0 || iMotion >= 512 )
			return TRUE;

		return fnChangeMotion( pcUnitGame, iMotion, iAnimationID );
	}
	return TRUE;
}

//Recived damage data from server
//to this player
void CharacterGame::PHDamage( PacketAttackData * psPacket )
{
	//If damage received is from a monster, dwTargetObjectSerial will be zero
	//If damage received is from a player, dwTargetObjectSerial will be non-zero

	//int compare = GetDamgeChkSum_S2V( psPacket );
	//CHATBOX->AddDebugMessage( "%d vs %d", psPacket->dwDamageChkSum, compare );


	UnitData* pcUnitData = UNITDATABYID(psPacket->dwDestObjectSerial);

	if (pcUnitData)
	{
		if (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player ||
			pcUnitData->PkMode_CharState == CHARACTERTYPE_Player)
		{
			//Skip modification of player attack data (and let it get processed in EXE instead) because
			// the animation events doesn't match the combos that players do
			// i.e. might only do 1 attack where player's skill actually does 2 hits.
			// Maybe the system that we implement for monsters can work for PVP
			// but requires further investigation.

			//DEBUG("%d = CharacterGame::PHDamage (FROM PLAYER): Damage: %d, State: %d", TICKCOUNT, psPacket->iDamage, psPacket->iDamageState);
			return;
		}
		else
		{
			UnitData* pcTargetUnitData = UNITDATABYID(psPacket->dwTarObjectSerial);

			Unit* pcUnit = UNITDATATOUNIT(pcUnitData);

			if (pcUnit)
			{
				//DEBUG("%d = PHDamage", TICKCOUNT);

				pcUnit->cMutex->lock();

				//See int rsSendTransSkillAttack(smCHAR *lpChar, rsPLAYINFO *lpPlayInfo)
				if ( ( psPacket->iAttackState & 0xFF) == 0x80 )
				{
					if (pcTargetUnitData)
					{
						//DEBUG("%d = PHDamage   SKILL  : Mon %s, Damage: %d, State: %d, Range: %d, Target = %s", TICKCOUNT, pcUnit->GetName(), psPacket->iDamage, psPacket->iDamageState, psPacket->iRange, pcTargetUnitData->GetName());
					}
					else
					{
						//DEBUG("%d = PHDamage   SKILL  : Mon %s,  Damage: %d, State: %d, Range: %d, TargetID = %d", TICKCOUNT, pcUnit->GetName(), psPacket->iDamage, psPacket->iDamageState, psPacket->iRange, psPacket->dwTarObjectSerial);
					}

					CopyMemory(&pcUnit->sLatestSkillAttackTrans, psPacket, sizeof(PacketAttackData));
					pcUnit->dwLastReceivedAttackTrans = TICKCOUNT;
					pcUnit->dwLastReceivedSkillAttackTrans = TICKCOUNT;
				}

				//Basic attack
				else if ( (psPacket->iAttackState & 0xFF) == 0x01 )
				{
					if (pcTargetUnitData)
					{
						//DEBUG("%d = PHDamage   BASIC  : Mon %s, Damage: %d, State: %d, Range: %d, Target = %s", TICKCOUNT, pcUnit->GetName(), psPacket->iDamage, psPacket->iDamageState, psPacket->iRange, pcTargetUnitData->GetName());
					}
					else
					{
						//DEBUG("%d = PHDamage   BASIC  : Mon %s, Damage: %d, State: %d, Range: %d, TargetID = %d", TICKCOUNT, pcUnit->GetName(), psPacket->iDamage, psPacket->iDamageState, psPacket->iRange, psPacket->dwTarObjectSerial);
					}

					CopyMemory(&pcUnit->sLatestBasicAttackTrans, psPacket, sizeof(PacketAttackData));
					pcUnit->dwLastReceivedAttackTrans = TICKCOUNT;
					pcUnit->dwLastReceivedBasicAttackTrans = TICKCOUNT;
				}
				else
				{
					DEBUG ("%d = PHDamage                  ERROR!!!!  : Unknown state = %d, %d", TICKCOUNT, psPacket->iDamageState, psPacket->iDamageState & 0xFF);
				}

				pcUnit->cMutex->unlock();

				pcUnitData->iTransSendWait = 0;
				pcUnitData->dwLastActiveTime = TICKCOUNT;
			}
			else
			{
				//DEBUG("%d = PHDamage                  ERROR!!!!  :  Unit data not found!", TICKCOUNT);
			}

			//Invalidate the packet so it won't get procesed in EXE
			psPacket->iHeader = 0;
		}
	}
	else
	{
		//DEBUG("%d = PHDamage                  ERROR!!!!  : UnitData data not found!", TICKCOUNT);
	}
}


BOOL CharacterGame::XorAttackTrans(UnitData * pcUnitData, PacketAttackData * lpTransAttackData)
{
	//return CALLT( 0x422190, (DWORD)pcUnitData );

	int cnt, len;
	DWORD dwCode;

	len = (sizeof(PacketAttackData) / sizeof(DWORD)); // == 13
	DWORD* lpdwBuff = (DWORD*)lpTransAttackData;

	dwCode = GetAttackTrans_XorCode(pcUnitData);
	//dwCoded = CALL_WITH_ARG1(0x00422150, (DWORD)pcUnitData); //will produce same result

	lpdwBuff[0] ^= dwCode;
	lpdwBuff[1] ^= dwCode;

	for (cnt = 2; cnt < len; cnt++)
	{
		lpdwBuff[cnt] ^= dwCode;
		dwCode += dwCode;
	}

	return TRUE;
}

//Damage Encryption Code received
DWORD CharacterGame::GetAttackTrans_XorCode(UnitData * pcUnitData)
{
	return pcUnitData->iID ^ UNITDATA->iID ^ (((DWORD)pcUnitData) >> 8);
}

DWORD CharacterGame::EncodeDamagePacket( PacketAttackData * lpTransAttackData )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD dwChkSum;
	DWORD dwObjSum;
	DWORD *lpDword = (DWORD *)lpTransAttackData;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;

	dwChkSum = GetSpeedSum( UNITDATA->GetName() );
	dwObjSum = UNITDATA->iID;
	dwObjSum *= (UNITDATA->sCharacterData.iClass + 2);

	DWORD dwDamagePacketKey2_0 = ( dwChkSum * dwObjSum / 11 ) + 0x34;
	DWORD dwDamagePacketKey2_1 = dwDamagePacketKey2_0 ^ ( ( dwObjSum >> 7 ) + ( dwObjSum << 16 ) ) + 0x37;

	dwCode = (code^(code<<5)) + size * dwDamagePacketKey2_1;
	cnt2 = (code + dwDamagePacketKey2_1) & 0xBCAFF5;
	cnt3 = cnt2 & 0x5E;

	for ( cnt = 2; cnt < size; cnt++ )
	{
		dwCode = dwCode ^ ( cnt2 << 7 ) ^ lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2 += cnt2 + cnt3;
	}

	return TRUE;
}

DWORD CharacterGame::GetSpeedSum( char *szName )
{
	DWORD cnt;
	DWORD Sum1,Sum2;
	BYTE ch;
	DWORD  dwSum;

	Sum2 = 0;
	dwSum = 0;

	cnt = 0;

	while(1)
	{
		ch = (BYTE)szName[cnt];
		if ( ch==0 ) break;
		if ( ch>='a' && ch<='z' )
		{
			Sum2 += (ch-0x20)*(cnt+1);
			dwSum += (ch-0x20)*(cnt*cnt);
		}
		else
		{
			Sum2 += (ch*(cnt+1));
			dwSum += ch*(cnt*cnt);
		}
		cnt++;
	}

	Sum1 = cnt;

	return (dwSum<<24)|(Sum1<<16)|Sum2;
}

/// <summary>
/// Leaked code: dm_GetDamgeChkSum_S2V
/// </summary>
DWORD CharacterGame::GetDamgeChkSum_S2V( PacketAttackData * lpTransAttackData)
{
	//DWORD chk1 = CALL_WITH_ARG1( 0x408300, (DWORD)lpTransAttackData );


	DWORD	dwChkSum;

	dwChkSum = lpTransAttackData->iHeader;
	dwChkSum += lpTransAttackData->sPosition.iX ^ lpTransAttackData->sPosition.iY ^ lpTransAttackData->sPosition.iZ;
	dwChkSum ^= *(DWORD *)&lpTransAttackData->iAttackState + lpTransAttackData->iRange;
	dwChkSum ^= lpTransAttackData->iDamage << 5;
	dwChkSum ^= lpTransAttackData->dwChkSum;
	dwChkSum ^= ((DWORD*)lpTransAttackData->sRating)[0];
	dwChkSum ^= 0x80516809;



	return dwChkSum;
}

DWORD CharacterGame::GetDamgeChkSum_S2V( PacketSingleTargetSkillData * psPacketAttackDamageEx )
{
	//DWORD chk1 = CALL_WITH_ARG1( 0x408300, (DWORD)lpTransAttackData );

	DWORD	dwChkSum;

	dwChkSum = psPacketAttackDamageEx->iHeader;
	dwChkSum += psPacketAttackDamageEx->sPosition.iX ^ psPacketAttackDamageEx->sPosition.iY ^ psPacketAttackDamageEx->sPosition.iZ;
	dwChkSum += psPacketAttackDamageEx->AttackCount ^ psPacketAttackDamageEx->dwTime;
	dwChkSum ^= psPacketAttackDamageEx->AttackPowerMin;
	dwChkSum ^= psPacketAttackDamageEx->AttackParameter1;
	dwChkSum ^= psPacketAttackDamageEx->sCriticalChance;
	dwChkSum ^= psPacketAttackDamageEx->MotionEventNum;
	dwChkSum += psPacketAttackDamageEx->iSkillCode;
	dwChkSum ^= (0x68098051 + GAME_VERSION);

	return dwChkSum;
}

BOOL CharacterGame::PHTeleport( struct PacketTeleportEvent * psPacket )
{
	BOOL bRet = FALSE;
	switch ( psPacket->iTeleportEventID )
	{
		case 1000:
			MAPGAME->SetMessageBoxMap( MAPID_RicartenTown );
			bRet = TRUE;
			break;
		case 1001:
			MAPGAME->SetMessageBoxMap( MAPID_Atlantis );
			bRet = TRUE;
			break;
		case 1002:
			if ( UNITDATA->sCharacterData.iLevel >= 0 )
				MAPGAME->SetMessageBoxMap( MAPID_BattleTown );
			else
				TITLEBOX( "You must be level 105 to teleport" );
			bRet = TRUE;
			break;
		case 1003:
			if (UNITDATA->sCharacterData.iLevel >= 10)
				MAPGAME->SetMessageBoxMap(MAPID_CastleOfTheLost);
			else
				TITLEBOX("You must be level 10 to travel");
			bRet = TRUE;
			break;
		default:
			break;
	}
	return bRet;
}

void CharacterGame::CancelAttack()
{
	CALL( 0x004529B0 );
}

void CharacterGame::OnUseManaPotion( int iMP )
{

	int iValue = SKILLMANAGERHANDLER->GetSkillIntValue ( Priestess_Meditation_MPPotionBoost, SKILLID_Meditation );
	if (iValue > 0 )
	{
		iMP += (iMP * iValue) / 100;
	}

	iValue = SKILLMANAGERHANDLER->GetSkillIntValue ( Magician_MentalMastery_MPPotionBoost, SKILLID_MentalMastery );
	if ( iValue > 0 )
	{
		iMP += (iMP * iValue) / 100;
	}

	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_InnerPeace );
	if ( iLevelSkill > 0 )
	{
		int iValue = ((int*)0x04B0D950)[iLevelSkill - 1];
		iMP += (iMP * iValue) / 100;
	}

	CALL_WITH_ARG1( 0x00507820, (DWORD)iMP );
}

void CharacterGame::OnUseHPPotion( int iHP )
{
	/*int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SurvivalInstinct );

	if ( iLevelSkill > 0 )
	{
		int iValue = iaFighterT51AddHPPotionBoost[iLevelSkill - 1];
		iHP += (iHP * iValue) / 100;
	}*/

	int iValue = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_BoostHealth_HPBoostPercent, ESkillID::SKILLID_Resilience );
	if ( iValue > 0 )
	{
		iHP += (iHP * iValue) / 100;
	}

	CALL_WITH_ARG1( 0x00507780, (DWORD)iHP );
}

BOOL CharacterGame::CanUseGold( int iGold )
{
	int i = 0;
	__asm
	{
		PUSH EAX;
		MOV EAX, 0x004E5640;
		PUSH iGold;
		CALL EAX;
		MOV i, EAX;
		POP EAX;
	}



	return (i >= 0);
}

void CharacterGame::ResetStats( int iNum )
{
	//Reset All Stats
	if ( iNum == 6 )
	{
		CALLT_WITH_ARG1( pfnResetAllStats, 0x035D0EA0, TRUE );
	}
	else
	{
		CALLT_WITH_ARG1( pfnResetStats, 0x035D0EA0, iNum );
	}
}

void CharacterGame::FocusTargetPet( int iID )
{
	if ( iLastFocusID != iID )
	{
		PacketUpdatePetDataEx sPacket;
		sPacket.iLength = sizeof( PacketUpdatePetDataEx );
		sPacket.iHeader = PKTHDR_UpdatePetDataEx;
		sPacket.iTargetID = iID;

		SENDPACKETG( &sPacket );

		iLastFocusID = iID;
	}
}

void CharacterGame::HandlePacket( PacketUpdatePetDataEx * psPacket )
{
	#define PARTICLEID_TARGETFOCUS		0x2002

	UnitData * pcUnitData = UNITDATABYID( psPacket->iTargetID );

	if( pcUnitData )
	{
		PARTICLEENGINE->Kill( PARTICLEID_TARGETFOCUS );
		CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\Target.luac" );

		if( pc )
		{
			pc->SetID( PARTICLEID_TARGETFOCUS );
			Engine::ObjectOwnerUnit * pcOwner = new Engine::ObjectOwnerUnit( UNITDATATOUNIT( pcUnitData ) );
			pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
			pc->SetOwner( pcOwner );
			PARTICLEENGINE->AddScript( pc );
		}
	}

	if ( psPacket->iTargetID == (-1) )
		iLastFocusID = -1;
}

void CharacterGame::HandlePacket( PacketCharacterDataEx * psPacket )
{
	CopyMemory( &sCharacterDataEx, &psPacket->sData, sizeof(CharacterDataEx) );

	SERVER_IS_SEASONAL = psPacket->sData.bSeasonal;
}

/// <summary>
/// PKTHDR_CharacterData
/// </summary>
void CharacterGame::HandlePacket( PacketUnitInfo * psPacket )
{

}


void CharacterGame::HandlePacket( PacketUpdateIntegrity * psPacket )
{
	UINT uIntegrityDecrease = 0;

	switch( psPacket->eUpdate )
	{
		case INTEGRITYUPDATE_KilledMonster:
			uIntegrityDecrease = 10;
			break;
		case INTEGRITYUPDATE_KilledUser:
			uIntegrityDecrease = 5;
			break;
	}

	//Update equipped Items
	for( int i = 0; i < 15; i++ )
	{
		if( INVENTORYITEMSLOT[i].iItemIndex )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];

			if( psItemData->bValid && psItemData->sItem.sIntegrity.sMax > 0 )
			{
				psItemData->sItem.sIntegrity.sCur -= uIntegrityDecrease;
				if( psItemData->sItem.sIntegrity.sCur <= 0 )
				{
					psItemData->sItem.sIntegrity.sCur = 0;
					psItemData->sItem.bCanNotUse = TRUE;
					//TODO Ensure it only alerts for each spesific item and which item broke.
					CHATBOX->AddMessage("One of your items has broken, meet a BlackSmith to repair them.");
				}
				else if( psItemData->sItem.sIntegrity.sCur > psItemData->sItem.sIntegrity.sMax )
					psItemData->sItem.sIntegrity.sCur = psItemData->sItem.sIntegrity.sMax;
			}
		}
	}
}


/// <summary>
/// Get the total attack rating from equipped items (ignoring attack rating / div)
/// </summary>
int CharacterGame::GetAttackRatingFromEquippedItems ( )
{
	int iAttackRating = 0;

	//Update equipped Items
	for( int i = 0; i < 15; i++ )
	{
		if( INVENTORYITEMSLOT[i].iItemIndex )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];
			if ( psItemData->bValid && psItemData->sItem.sIntegrity.sMax > 0 )
			{
				if ( psItemData->sItem.iAttackRating > 0 )
				{
					iAttackRating += psItemData->sItem.iAttackRating;
				}
			}
		}
	}

	return iAttackRating;
}

BOOL CharacterGame::IsLeftSlotEquippped (enum EItemType eItemType)
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex >= 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType () == eItemType )
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CharacterGame::IsRightSlotEquippped (enum EItemType eItemType)
{
	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex >= 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType () == eItemType )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CharacterGame::SetEXP( INT64 iEXP )
{
	LARGE_INTEGER li;
	li.QuadPart = iEXP;

	ExpXor();

	CALL_WITH_ARG3( 0x004599D0, (DWORD)&UNITDATA->sCharacterData, li.LowPart, li.HighPart );

	ExpXor();
}

INT64 CharacterGame::GetEXP()
{
	INT64 iEXP = ExpXor();
	ExpXor();
	return iEXP;
}

int CharacterGame::GetCanCarryGold()
{
	if ( UNITDATA->sCharacterData.iLevel <= 10 )
		return 200'000;

	BOOL bGoldBag = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GoldBag ) ? TRUE : FALSE;

	int iGoldCarryMax = 0;

	switch ( UNITDATA->sCharacterData.iRank )
	{
		//for level 11+
		case CHARACTERRANK_Rank1:

			//At level 11 it is 2'200'000 - 1'800'000 = 400,000 gold limit
			iGoldCarryMax = (UNITDATA->sCharacterData.iLevel * 200'000) - 1'800'000;
			break;
		case CHARACTERRANK_Rank2:
			iGoldCarryMax = 10'000'000;			//10kk
			break;
		case CHARACTERRANK_Rank3:
			iGoldCarryMax = 50'000'000;			//50kk
			break;
		case CHARACTERRANK_Rank4:
			iGoldCarryMax = 500'000'000;		//500kk
			break;
		case CHARACTERRANK_Rank5:
			iGoldCarryMax = 750'000'000;		//750kk
			if ( bGoldBag )
				iGoldCarryMax = 1000000000;		//1kkk
			break;
	}

	return iGoldCarryMax;
}

BOOL CharacterGame::CanCarryGold( int iGold )
{
	if ( (UNITDATA->sCharacterData.iGold + iGold) < 0 )
		return FALSE;

	if ( (UNITDATA->sCharacterData.iGold + iGold) <= GetCanCarryGold() )
		return TRUE;

	CTITLEBOX->SetText( 13 );

	return FALSE;
}

DWORD CharacterGame::GetCharacterColorByClass( int iClass )
{
	DWORD dwColor = 0;

	switch ( iClass )
	{
		case CHARACTERCLASS_Fighter:
			dwColor = D3DCOLOR_XRGB( 255, 140, 0 );		//Dark Orange
			break;
		case CHARACTERCLASS_Mechanician:
			dwColor = D3DCOLOR_XRGB( 0, 250, 154 );		//Med Spring Green
			break;
		case CHARACTERCLASS_Archer:
			dwColor = D3DCOLOR_XRGB( 255, 69, 0 );		//Orange Red
			break;
		case CHARACTERCLASS_Pikeman:
			dwColor = D3DCOLOR_XRGB( 30, 144, 255 );	//Dodger Blue
			break;
		case CHARACTERCLASS_Atalanta:
			dwColor = D3DCOLOR_XRGB( 218, 165, 32 );	//Goldenrod
			break;
		case CHARACTERCLASS_Knight:
			dwColor = D3DCOLOR_XRGB( 0, 191, 255 );		//Deep Sky Blue
			break;
		case CHARACTERCLASS_Magician:
			dwColor = D3DCOLOR_XRGB( 50, 205, 50 );		//Lime Green
			break;
		case CHARACTERCLASS_Priestess:
			dwColor = D3DCOLOR_XRGB( 238, 0, 238 );		//Magenta 2
			break;
		case CHARACTERCLASS_Assassin:
			dwColor = D3DCOLOR_XRGB( 125, 38, 205 );	//Purple 3
			break;
		case CHARACTERCLASS_Shaman:
			dwColor = D3DCOLOR_XRGB( 205, 0, 0 );		//Red 3
			break;
	}

	return dwColor;
}

void CharacterGame::UpdateCharacterPlayData()
{
	int iFramesSendCount = UNITGAME->GetFramesSendCount();
	int iPing = SOCKETGAME ? SOCKETGAME->GetAveragePingGame() : 400;

	if (TRADEHANDLER->IsWindowTradeOpen())
	{
		if ((TRADEHANDLER->IsTradeSent() == FALSE) && TRADE_ACCEPT && TRADEUNIT_ACCEPT)
		{
			TRADEHANDLER->SendTradeCheckItem(TRADE_UNITID);
			TRADEHANDLER->IsTradeSent(TRUE);
		}
	}

	if (TRADE_MASKTIME)
	{
		if (TRADE_MASKTIME < TICKCOUNT)
			TRADE_MASKTIME = 0;
	}

	if ((int)(CHARACTER_PLAYCOUNTER - CHARACTER_LASTPLAYCOUNTER) > iFramesSendCount)
	{
		UNITGAME->ClearFarUnits();

		CHARACTER_LASTPLAYCOUNTER = CHARACTER_PLAYCOUNTER;
		CHARACTER_LASTSENDPLAYCOUNTER = TICKCOUNT;
	}

	if (SOCKETG)
	{
		DWORD dwTime = TICKCOUNT - CHARACTER_LASTTIMESENDPLAYDATA;

		if (UNITDATA->iID)
		{
			if (((UNITDATA->iPlayBuffCnt % iFramesSendCount) == 0))
			{
				UNITDATA->MakeUnitBufferData((char*)UNITDATA->baUnitInfo, iFramesSendCount, 1);


				PacketPlayData * psPacket = (PacketPlayData *)UNITDATA->baUnitInfo;
				SENDPACKETG(psPacket);
				SOCKET_NETSTANDCOUNT = (psPacket->iPlayBuffCount > 1) ? 0 : (SOCKET_NETSTANDCOUNT + 1);

				SOCKETSEND_DEBUGCOUNT++;

				SOCKET_RECEIVEGAMESERVER = FALSE;
				CHARACTER_LASTTIMESENDPLAYDATA = TICKCOUNT;
			}
		}
	}

	if (GAME_LOADINGTIME)
	{
		if (TICKCOUNT > (GAME_LOADINGTIME + 10000))
		{
			DISCONNECT_TIME = TICKCOUNT;
			GAME_LOADINGTIME = 0;
		}
	}

	if ((CHARACTER_PLAYCOUNTER & 0x1FF) == 0x80)
		CALL(0x00628C00);

	if (CHARACTER_VISIBLECOUNT > 0)
		CHARACTER_VISIBLECOUNT--;
}

