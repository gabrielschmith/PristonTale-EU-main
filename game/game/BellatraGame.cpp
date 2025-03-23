#include "stdafx.h"
#include "BellatraGame.h"


BellatraGame::BellatraGame()
{

}


BellatraGame::~BellatraGame()
{
}

//Handle solo sod start
// - Remove buff applied from other players
// - this is to make it fair. i.e. players buffing their char via alts prior to sod
void BellatraGame::HandleSoloSodStart()
{
	if ( UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Priestess )
	{
		TIMERSKILLHANDLER->KillTimer( ETimerID::VirtualLife );
		TIMERSKILLHANDLER->KillTimer( ETimerID::VirtualRegen );
	}

	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Archer)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::ForceofNature);
		TIMERSKILLHANDLER->KillTimer(SKILLID_ForceofNature);
	}
	
	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Atalanta)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::HallOfValhalla);
		TIMERSKILLHANDLER->KillTimer(ETimerID::TriumphOfValhalla);
	}

	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Knight)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::HolyValor);
		TIMERSKILLHANDLER->KillTimer(SKILLID_HolyValor);  //@todo - remove when ETimerID is done
	}

	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Magician)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::EnchantWeapon);
		TIMERSKILLHANDLER->KillTimer(SKILLID_EnchantWeapon);  //@todo - remove when ETimerID is done
	}

	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Shaman)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::AdventMigal);
		TIMERSKILLHANDLER->KillTimer(SKILLID_AdventMigal);

		if (*(DWORD*)(0x4B07A00))
		{
			CALLT_WITH_ARG1(0x005C9590, *(DWORD*)(0x4B07A00), 0);
			*(DWORD*)(0x4B07A00) = 0;
		}
		if (*(DWORD*)(0x4B07A04))
		{
			CALLT_WITH_ARG1(0x005C9590, *(DWORD*)(0x4B07A04), 0);
			*(DWORD*)(0x4B07A04) = 0;
		}
	}
	
	if (UNITDATA->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Assassin)
	{
		TIMERSKILLHANDLER->KillTimer(ETimerID::Inpes);
		TIMERSKILLHANDLER->KillTimer(ETimerID::Alas);
	}	
}

void BellatraGame::SetCrown()
{
	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return;

	BOOL bMapIsSodOrBC = CHARACTERGAME->GetStageID() == MAPID_Bellatra || CHARACTERGAME->GetStageID() == MAPID_BlessCastle;

	if (bMapIsSodOrBC == FALSE && (UNITGAME->iBellatraSoloCrownRanking == 1 || UNITGAME->iBellatraSoloCrownRanking == 4)) //4 = 1st place with humor
	{
		Skill* psSkill = TIMERSKILLHANDLER->FindSkillTimer(TIMERSKILLID_BellatraSoloGold);
		if (!psSkill && dwTextureCrownTimerGold)
		{
			Skill sSkill;
			ZeroMemory(&sSkill, sizeof(sSkill));
			STRINGCOPY(sSkill.szName, "Bellatra Solo Crown Gold");
			STRINGCOPY(sSkill.sSkillInfo.szName, "Bellatra Solo Crown Gold");
			STRINGCOPY(sSkill.sSkillInfo.szDescription, "+50% extra gold from Monsters; \n+5 Speed, +5 Absorb");
			sSkill.iID = TIMERSKILLID_BellatraSoloGold;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerGold;
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloSilver);
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloBronze);
			TIMERSKILLHANDLER->AddSkillTimer(&sSkill);
			PLAYMINISOUND(25);
			bKilledSoloTimers = FALSE;
		}
	}
	else if (bMapIsSodOrBC == FALSE && (UNITGAME->iBellatraSoloCrownRanking == 2 || UNITGAME->iBellatraSoloCrownRanking == 5)) //5 = 2nd place with humor
	{
		Skill* psSkill = TIMERSKILLHANDLER->FindSkillTimer(TIMERSKILLID_BellatraSoloSilver);
		if (!psSkill && dwTextureCrownTimerSilver)
		{
			Skill sSkill;
			ZeroMemory(&sSkill, sizeof(sSkill));
			STRINGCOPY(sSkill.szName, "Bellatra Solo Crown Silver");
			STRINGCOPY(sSkill.sSkillInfo.szName, "Bellatra Solo Crown Silver");
			STRINGCOPY(sSkill.sSkillInfo.szDescription, "+30% extra gold from Monsters; \n+3 Speed, +3 Absorb");
			sSkill.iID = TIMERSKILLID_BellatraSoloSilver;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerSilver;
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloGold);
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloBronze);
			TIMERSKILLHANDLER->AddSkillTimer(&sSkill);
			PLAYMINISOUND(25);
			bKilledSoloTimers = FALSE;
		}
	}
	else if (bMapIsSodOrBC == FALSE && (UNITGAME->iBellatraSoloCrownRanking == 3 || UNITGAME->iBellatraSoloCrownRanking == 6)) //6 = 3rd place with humor
	{
		Skill* psSkill = TIMERSKILLHANDLER->FindSkillTimer(TIMERSKILLID_BellatraSoloBronze);
		if (!psSkill && dwTextureCrownTimerBronze)
		{
			Skill sSkill;
			ZeroMemory(&sSkill, sizeof(sSkill));
			STRINGCOPY(sSkill.szName, "Bellatra Solo Crown Bronze");
			STRINGCOPY(sSkill.sSkillInfo.szName, "Bellatra Solo Crown Bronze");
			STRINGCOPY(sSkill.sSkillInfo.szDescription, "+20% extra gold from Monsters; \n+2 Speed, +2 Absorb");
			sSkill.iID = TIMERSKILLID_BellatraSoloBronze;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerBronze;
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloGold);
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloSilver);
			TIMERSKILLHANDLER->AddSkillTimer(&sSkill);
			PLAYMINISOUND(25);
			bKilledSoloTimers = FALSE;
		}
	}
	else
	{
		if (!bKilledSoloTimers)
		{
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloGold);
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloSilver);
			TIMERSKILLHANDLER->KillTimer(TIMERSKILLID_BellatraSoloBronze);

			bKilledSoloTimers = TRUE;
		}
	}

	if ( bMapIsSodOrBC == FALSE && (*(DWORD*)(0x03B18BC8)) == 1 )
	{
		Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( TIMERSKILLID_BellatraGold );
		if ( !psSkill && dwTextureCrownTimerGold )
		{
			Skill sSkill; 
			ZeroMemory( &sSkill, sizeof( sSkill ) );
			STRINGCOPY( sSkill.szName, "Bellatra Crown Gold" );
			STRINGCOPY( sSkill.sSkillInfo.szName, "Bellatra Crown Gold" );
			STRINGCOPY( sSkill.sSkillInfo.szDescription, "+15% damage to Monsters; \n+100STM +100MP +50HP" );
			sSkill.iID = TIMERSKILLID_BellatraGold;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerGold;
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraSilver );
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraBronze );
			TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
			PLAYMINISOUND( 25 );
			bKilledTimers = FALSE;
		}
	}
	else if ( bMapIsSodOrBC == FALSE && (*(DWORD*)(0x03B18BC8)) == 2 )
	{
		Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( TIMERSKILLID_BellatraSilver );
		if ( !psSkill && dwTextureCrownTimerSilver )
		{
			Skill sSkill; 
			ZeroMemory( &sSkill, sizeof( sSkill ) );
			STRINGCOPY( sSkill.szName, "Bellatra Crown Silver" );
			STRINGCOPY( sSkill.sSkillInfo.szName, "Bellatra Crown Silver" );
			STRINGCOPY(sSkill.sSkillInfo.szDescription, "+8% damage to Monsters; \n+50STM +50MP +25HP");
			sSkill.iID = TIMERSKILLID_BellatraSilver;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerSilver;
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraGold );
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraBronze );
			TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
			PLAYMINISOUND( 25 );
			bKilledTimers = FALSE;
		}
	}
	else if ( bMapIsSodOrBC == FALSE && (*(DWORD*)(0x03B18BC8)) == 3 )
	{
		Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( TIMERSKILLID_BellatraBronze );
		if ( !psSkill && dwTextureCrownTimerBronze )
		{
			Skill sSkill; 
			ZeroMemory( &sSkill, sizeof( sSkill ) );
			STRINGCOPY( sSkill.szName, "Bellatra Crown Bronze" );
			STRINGCOPY( sSkill.sSkillInfo.szName, "Bellatra Crown Bronze" );
			STRINGCOPY(sSkill.sSkillInfo.szDescription, "+3% damage to Monsters; \n+25STM +25MP");
			sSkill.iID = TIMERSKILLID_BellatraBronze;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = dwTextureCrownTimerBronze;
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraGold );
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraSilver );
			TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
			PLAYMINISOUND( 25 );
			bKilledTimers = FALSE;
		}
	}
	else
	{
		if ( !bKilledTimers )
		{
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraGold );
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraSilver );
			TIMERSKILLHANDLER->KillTimer( TIMERSKILLID_BellatraBronze );

			bKilledTimers = TRUE;
		}
	}

	if ( !dwTextureCrownTimerGold )
			dwTextureCrownTimerGold = TIMERSKILLHANDLER->CreateTextureMaterial( "game\\images\\skills\\timers\\bellatracrown_1.tga" );
	if ( !dwTextureCrownTimerSilver )
			dwTextureCrownTimerSilver = TIMERSKILLHANDLER->CreateTextureMaterial( "game\\images\\skills\\timers\\bellatracrown_2.tga" );
	if ( !dwTextureCrownTimerBronze )
			dwTextureCrownTimerBronze = TIMERSKILLHANDLER->CreateTextureMaterial( "game\\images\\skills\\timers\\bellatracrown_3.tga" );
}
