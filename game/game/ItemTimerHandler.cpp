#include "stdafx.h"
#include "ItemTimerHandler.h"

UI::Image * ItemTimer::pcImageBox;


//See AssaParticle_ShelltomWeapon in leaked code
typedef DWORD ( __cdecl * tfnAssaParticleShelltomWeapon )( UnitData * pcUnitData, int iTime, int iCode );
tfnAssaParticleShelltomWeapon fnAssaParticleShelltomWeapon = (tfnAssaParticleShelltomWeapon)0x005C16B0;




ItemTimer * CItemTimerHandler::GetItemTimerByType( int iType )
{
	for ( ItemTimer * timer : pcaItemTimer )
	{
		if ( ((timer->iType == iType) || (ItemTimerIsSame( timer->iType, iType ) == TRUE)) )
			return timer;
	}

	return NULL;
}

BOOL CItemTimerHandler::CanStack( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixHeal:
		case ITEMTIMERTYPE_PhoenixLightning:
			break;

		default:
			break;
	}

	return bRet;
}


BOOL CItemTimerHandler::IsHead( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadValentine:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_ChristmasGreenHat:
		case ITEMTIMERTYPE_ChristmasRedHat:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_SoccerHatSpeed:
		case ITEMTIMERTYPE_PumpkinHat:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

void CItemTimerHandler::OnClickCancelOK( ItemTimer * pcTimer, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( pcTimer )
		{
			PacketCancelItemTimer s;
			s.iType = pcTimer->iType;
			s.iItemID = pcTimer->iItemID;
			ProcessPacket( &s );

			//Remove and Stop Timer
			RemoveTimer( pcTimer );
		}
	}
}

CItemTimerHandler::CItemTimerHandler()
{
	pcaItemTimerPrefab.clear();
	pcaItemTimer.clear();
}


CItemTimerHandler::~CItemTimerHandler()
{
	for ( ItemTimer * timer : pcaItemTimerPrefab )
	{
		SAFE_DELETE( timer );
	}

	for ( ItemTimer * timer : pcaItemTimer )
	{
		SAFE_DELETE( timer );
	}

	pcaItemTimerPrefab.clear();
	pcaItemTimer.clear();
}

void CItemTimerHandler::Init()
{
	ItemTimer * pcHED = new ItemTimer(ITEMTIMERTYPE_HoneyDamage, ITEMID_None, "Power Honey", "+10% Attack Power", 0, 0, NULL, NULL);
	pcHED->AddDisableMap(MAPID_Bellatra);
	pcHED->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey.png");
	pcHED->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey_.png");
	pcaItemTimerPrefab.push_back(pcHED);

	ItemTimer * pcHEF = new ItemTimer(ITEMTIMERTYPE_HoneyFast, ITEMID_None, "Fast Honey", "+10% Experience gained", 0, 0, NULL, NULL);
	pcHEF->AddFreezeMap( MAPID_BattleTown );
	pcHEF->AddFreezeMap( MAPID_BlessCastle );
	pcHEF->AddFreezeMap( MAPID_Bellatra );
	pcHEF->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey2.png");
	pcHEF->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey_.png");
	pcaItemTimerPrefab.push_back(pcHEF);

	ItemTimer * pcHES = new ItemTimer(ITEMTIMERTYPE_HoneySmart, ITEMID_None, "Smart Honey", "+5 MP Regen and 50MP", 0, 0, NULL, NULL);
	pcHES->AddDisableMap(MAPID_Bellatra);
	pcHES->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey3.png");
	pcHES->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey_.png");
	pcaItemTimerPrefab.push_back(pcHES);

	ItemTimer * pcHEA = new ItemTimer(ITEMTIMERTYPE_HoneyDefense, ITEMID_None, "Defensive Honey", "+10% Absorb", 0, 0, NULL, NULL);
	pcHEA->AddDisableMap(MAPID_Bellatra);
	pcHEA->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey4.png");
	pcHEA->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\honey_.png");
	pcaItemTimerPrefab.push_back(pcHEA);

	ItemTimer * pcEXP = new ItemTimer(ITEMTIMERTYPE_ExpUp, ITEMID_None, "EXP Potion", "30% Additional Experience", 0, 0, NULL, NULL);
	pcEXP->AddFreezeMap( MAPID_BattleTown );
	pcEXP->AddFreezeMap( MAPID_BlessCastle );
	pcEXP->AddFreezeMap( MAPID_Bellatra );
	pcEXP->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EXP.png" );
	pcEXP->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EXP_.png" );
	pcaItemTimerPrefab.push_back(pcEXP);

	ItemTimer * pcTHE = new ItemTimer(ITEMTIMERTYPE_ThirdEye, ITEMID_None, "Third Eye", "One Additional Drop", 0, 0, NULL, NULL);
	pcTHE->AddFreezeMap( MAPID_BattleTown );
	pcTHE->AddFreezeMap( MAPID_BlessCastle );
	pcTHE->AddFreezeMap( MAPID_Bellatra );
	pcTHE->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ThirdEye.png" );
	pcTHE->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ThirdEye_.png" );
	pcaItemTimerPrefab.push_back(pcTHE);

	ItemTimer* pcSS = new ItemTimer(ITEMTIMERTYPE_SixthSense, ITEMID_None, "Sixth Sense", "25% chance for one additional \n drop on every kill", 0, 0, NULL, NULL);
	pcSS->AddFreezeMap(MAPID_BattleTown);
	pcSS->AddFreezeMap(MAPID_BlessCastle);
	pcSS->AddFreezeMap(MAPID_Bellatra);
	pcSS->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SixthSense.png");
	pcSS->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SixthSense_.png");
	pcaItemTimerPrefab.push_back(pcSS);

	// SERVER WIDE BUFFS
	// SHPRegen
	ItemTimer* pcSHPR = new ItemTimer(ITEMTIMERTYPE_SHPRegen, ITEMID_None, "World HP Regen", "Adds 3 HP Regen \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSHPR->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\HPUp.png");
	pcSHPR->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\HPUp_.png");
	pcaItemTimerPrefab.push_back(pcSHPR);
	// SSPRegen
	ItemTimer* pcSSPR = new ItemTimer(ITEMTIMERTYPE_SSPRegen, ITEMID_None, "World SP Regen", "Adds 3 SP Regen \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSSPR->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SPUp.png");
	pcSSPR->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SPUp_.png");
	pcaItemTimerPrefab.push_back(pcSSPR);
	// SMPRegen
	ItemTimer* pcSMPR = new ItemTimer(ITEMTIMERTYPE_SMPRegen, ITEMID_None, "World MP Regen", "Adds 3 MP Regen to all players \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSMPR->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\MPUp.png");
	pcSMPR->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\MPUp_.png");
	pcaItemTimerPrefab.push_back(pcSMPR);
	// SMPDown
	ItemTimer* pcSMPD = new ItemTimer(ITEMTIMERTYPE_SMPDown, ITEMID_None, "World MP Down", "10% reduced MP cost on skills \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSMPD->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\MPDown.png");
	pcSMPD->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\MPDown_.png");
	pcaItemTimerPrefab.push_back(pcSMPD);
	// SSPDown
	ItemTimer* pcSSPD = new ItemTimer(ITEMTIMERTYPE_SSPDown, ITEMID_None, "World SP Down", "10% reduced SP cost on skills \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSSPD->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SPDown.png");
	pcSSPD->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SPDown_.png");
	pcaItemTimerPrefab.push_back(pcSSPD);
	// SBonusDamage
	ItemTimer* pcSBD = new ItemTimer(ITEMTIMERTYPE_SBonusDamage, ITEMID_None, "World Bonus Damage", "5% Increased Damage \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSBD->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\TopPvp.png");
	pcSBD->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\TopPvp_.png");
	pcaItemTimerPrefab.push_back(pcSBD);
	// SBonusAbsorb
	ItemTimer* pcSBA = new ItemTimer(ITEMTIMERTYPE_SBonusAbsorb, ITEMID_None, "World Bonus Absorb", "5% Increased Absorb \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSBA->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\Gravity.png");
	pcSBA->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\Gravity_.png");
	pcaItemTimerPrefab.push_back(pcSBA);
	// SMoveSpeed
	ItemTimer* pcSMS = new ItemTimer(ITEMTIMERTYPE_SMoveSpeed, ITEMID_None, "World Move Speed", "+3 Move Speed \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSMS->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SoccerHat.png");
	pcSMS->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SoccerHat_.png");
	pcaItemTimerPrefab.push_back(pcSMS);
	// SEXPBuff
	ItemTimer* pcSEB = new ItemTimer(ITEMTIMERTYPE_SExpBuff, ITEMID_None, "Server EXP Buff", "5% Increased EXP Gained \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSEB->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\EXP.png");
	pcSEB->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\EXP_.png");
	pcaItemTimerPrefab.push_back(pcSEB);
	// SDropBuff
	ItemTimer* pcSDB = new ItemTimer(ITEMTIMERTYPE_SDropBuff, ITEMID_None, "World Drop Buff", "15% chance for one additional drop \nDOES NOT PAUSE!", 0, 0, NULL, NULL);
	pcSDB->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SixthSense.png");
	pcSDB->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\SixthSense_.png");
	pcaItemTimerPrefab.push_back(pcSDB);
	// END SERVER SIDE BUFFS

	// Resistance Flasks
	// IceResistanceBuff
	ItemTimer* pcIRB = new ItemTimer(ITEMTIMERTYPE_IceResistanceBuff, ITEMID_None, "Ice Resistance", "Raises your Ice resistance. \n+70 Ice Resistance", 0, 0, NULL, NULL);
	pcIRB->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\IceUp.png");
	pcIRB->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\IceUp_.png");
	pcaItemTimerPrefab.push_back(pcIRB);
	// END Resistance flasks

	ItemTimer * pcPH1 = new ItemTimer(ITEMTIMERTYPE_PhoenixFire, ITEMID_None, "Phoenix Pet", "Fire Element", 0, 0, NULL, NULL);
	pcPH1->AddDisableMap( MAPID_BattleTown );
	pcPH1->AddDisableMap( MAPID_BlessCastle );
	pcPH1->AddDisableMap( MAPID_Bellatra );
	pcPH1->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixFire.png" );
	pcPH1->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixFire_.png" );
	pcaItemTimerPrefab.push_back(pcPH1);

	ItemTimer * pcPH2 = new ItemTimer(ITEMTIMERTYPE_PhoenixIce, ITEMID_None, "Phoenix Pet", "Ice Element", 0, 0, NULL, NULL);
	pcPH2->AddDisableMap( MAPID_BattleTown );
	pcPH2->AddDisableMap( MAPID_BlessCastle );
	pcPH2->AddDisableMap( MAPID_Bellatra );
	pcPH2->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixIce.png" );
	pcPH2->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixIce_.png" );
	pcaItemTimerPrefab.push_back(pcPH2);

	ItemTimer * pcPH3 = new ItemTimer(ITEMTIMERTYPE_PhoenixLightning, ITEMID_None, "Phoenix Pet", "Lightning Element", 0, 0, NULL, NULL);
	pcPH3->AddDisableMap( MAPID_BattleTown );
	pcPH3->AddDisableMap( MAPID_BlessCastle );
	pcPH3->AddDisableMap( MAPID_Bellatra );
	pcPH3->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixLightning.png" );
	pcPH3->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixLightning_.png" );
	pcaItemTimerPrefab.push_back(pcPH3);

	ItemTimer * pcPH4 = new ItemTimer(ITEMTIMERTYPE_PhoenixHeal, ITEMID_None, "Phoenix Pet", "Heal Element", 0, 0, NULL, NULL);
	pcPH4->AddDisableMap( MAPID_BattleTown );
	pcPH4->AddDisableMap( MAPID_BlessCastle );
	pcPH4->AddDisableMap( MAPID_Bellatra );
	pcPH4->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixHeal.png" );
	pcPH4->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixHeal_.png" );
	pcaItemTimerPrefab.push_back(pcPH4);

	ItemTimer* pcPH5 = new ItemTimer(ITEMTIMERTYPE_PhoenixFireSmall, ITEMID_None, "Dragon Pet", "Fire Element", 0, 0, NULL, NULL);
	pcPH5->AddDisableMap(MAPID_BattleTown);
	pcPH5->AddDisableMap(MAPID_BlessCastle);
	pcPH5->AddDisableMap(MAPID_Bellatra);
	pcPH5->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixFireSmall.png");
	pcPH5->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixFireSmall_.png");
	pcaItemTimerPrefab.push_back(pcPH5);

	ItemTimer* pcPH6 = new ItemTimer(ITEMTIMERTYPE_PhoenixIceSmall, ITEMID_None, "Dragon Pet", "Ice Element", 0, 0, NULL, NULL);
	pcPH6->AddDisableMap(MAPID_BattleTown);
	pcPH6->AddDisableMap(MAPID_BlessCastle);
	pcPH6->AddDisableMap(MAPID_Bellatra);
	pcPH6->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixIceSmall.png");
	pcPH6->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixIceSmall_.png");
	pcaItemTimerPrefab.push_back(pcPH6);

	ItemTimer* pcPH7 = new ItemTimer(ITEMTIMERTYPE_PhoenixLightningSmall, ITEMID_None, "Dragon Pet", "Lightning Element", 0, 0, NULL, NULL);
	pcPH7->AddDisableMap(MAPID_BattleTown);
	pcPH7->AddDisableMap(MAPID_BlessCastle);
	pcPH7->AddDisableMap(MAPID_Bellatra);
	pcPH7->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixLightningSmall.png");
	pcPH7->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixLightningSmall_.png");
	pcaItemTimerPrefab.push_back(pcPH7);

	ItemTimer* pcPH8 = new ItemTimer(ITEMTIMERTYPE_PhoenixHealingSmall, ITEMID_None, "Dragon Pet", "Healing Element", 0, 0, NULL, NULL);
	pcPH8->AddDisableMap(MAPID_BattleTown);
	pcPH8->AddDisableMap(MAPID_BlessCastle);
	pcPH8->AddDisableMap(MAPID_Bellatra);
	pcPH8->pcImage = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixHealingSmall.png");
	pcPH8->pcImageBackground = UI::ImageLoader::LoadImage("game\\images\\items\\premium\\PhoenixHealingSmall_.png");
	pcaItemTimerPrefab.push_back(pcPH8);

	ItemTimer * pcMPD = new ItemTimer(ITEMTIMERTYPE_MPDown, ITEMID_None, "Mana Down", "Reduces 25% Mana Usage", 0, 0, NULL, NULL);
	pcMPD->AddDisableMap( MAPID_Bellatra );
	pcMPD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPDown.png" );
	pcMPD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPDown_.png" );
	pcaItemTimerPrefab.push_back(pcMPD);

	//SP Down
	ItemTimer * pcSPD = new ItemTimer(ITEMTIMERTYPE_SPDown, ITEMID_None, "Stamina Down", "Reduces 25% Stamina Usage", 0, 0, NULL, NULL);
	pcSPD->AddDisableMap( MAPID_Bellatra );
	pcSPD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SPDown.png" );
	pcSPD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SPDown_.png" );
	pcaItemTimerPrefab.push_back(pcSPD);

	//HP down
	ItemTimer * pcHPU = new ItemTimer(ITEMTIMERTYPE_HPUp, ITEMID_None, "Vampire Teeth", "+5 HP Regen", 0, 0, NULL, NULL);
	pcHPU->AddDisableMap( MAPID_Bellatra );
	pcHPU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HPUp.png" );
	pcHPU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HPUp_.png" );
	pcaItemTimerPrefab.push_back(pcHPU);

	//MP up
	ItemTimer * pcMPU = new ItemTimer(ITEMTIMERTYPE_MPUp, ITEMID_None, "Mana Aura", "+ 5 MP Regen", 0, 0, NULL, NULL);
	pcMPU->AddDisableMap( MAPID_Bellatra );
	pcMPU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPUp.png" );
	pcMPU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPUp_.png" );
	pcaItemTimerPrefab.push_back(pcMPU);


	ItemTimer * pcWGS = new ItemTimer(ITEMTIMERTYPE_WeightStone, ITEMID_None, "Weight Stone", "+250 Additional Weight", 0, 0, NULL, NULL);
	pcWGS->AddFreezeMap( MAPID_Bellatra );
	pcWGS->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Gravity.png" );
	pcWGS->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Gravity_.png" );
	pcaItemTimerPrefab.push_back(pcWGS);

	ItemTimer * pcMTS = new ItemTimer(ITEMTIMERTYPE_MatureStone, ITEMID_None, "Mature Stone", " ", 0, 0, NULL, NULL);
	pcMTS->AddFreezeMap( MAPID_Bellatra );
	pcMTS->pcImage					= NULL;
	pcMTS->pcImageBackground		= NULL;
	pcaItemTimerPrefab.push_back(pcMTS);

	ItemTimer * pcSKS = new ItemTimer(ITEMTIMERTYPE_SkillStone, ITEMID_None, "Skill Stone", " ", 0, 0, NULL, NULL);
	pcSKS->AddFreezeMap( MAPID_Bellatra );
	pcSKS->pcImage					= NULL;
	pcSKS->pcImageBackground		= NULL;
	pcaItemTimerPrefab.push_back(pcSKS);

	ItemTimer * pcCLH = new ItemTimer(ITEMTIMERTYPE_CartolaHat, ITEMID_None, "Magician Hat", "+5 HP Regen +10 Add HP", 0, 0, NULL, NULL);
	pcCLH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MagicianHat.png" );
	pcCLH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MagicianHat.png" );
	pcaItemTimerPrefab.push_back(pcCLH);

	ItemTimer * pcWTH = new ItemTimer(ITEMTIMERTYPE_WitchHat, ITEMID_None, "Witch Hat", "+5 HP Regen +10 Add HP", 0, 0, NULL, NULL);
	pcWTH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\WitchHat.png" );
	pcWTH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\WitchHat_.png" );
	pcaItemTimerPrefab.push_back(pcWTH);

	ItemTimer * pcSHH = new ItemTimer(ITEMTIMERTYPE_SheepHat, ITEMID_None, "Sheep Hat", "It's so cute! +150 Weight", 0, 0, NULL, NULL);
	pcSHH->AddFreezeMap( MAPID_Bellatra );
	pcSHH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SheepHat.png" );
	pcSHH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SheepHat_.png" );
	pcaItemTimerPrefab.push_back(pcSHH);

	ItemTimer * pcGFH = new ItemTimer(ITEMTIMERTYPE_GiraffeHat, ITEMID_None, "Giraffe Hat", "What a long neck! +150 Weight", 0, 0, NULL, NULL);
	pcGFH->AddFreezeMap( MAPID_Bellatra );
	pcGFH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GiraffeHat.png" );
	pcGFH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GiraffeHat_.png" );
	pcaItemTimerPrefab.push_back(pcGFH);

	ItemTimer * pcSCH = new ItemTimer( ITEMTIMERTYPE_SoccerHat, ITEMID_None, "Soccer Hat", "The Soccer Champion! +150 Weight", 0, 0, NULL, NULL );
	pcSCH->AddFreezeMap( MAPID_Bellatra );
	pcSCH->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat.png" );
	pcSCH->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat_.png" );
	pcaItemTimerPrefab.push_back( pcSCH );

	ItemTimer * pcSCH2 = new ItemTimer( ITEMTIMERTYPE_SoccerHatSpeed, ITEMID_None, "Soccer Hat", "The Soccer Champion!\n+2 Movement Speed", 0, 0, NULL, NULL );
	pcSCH2->AddFreezeMap( MAPID_Bellatra );
	pcSCH2->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat.png" );
	pcSCH2->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat_.png" );
	pcaItemTimerPrefab.push_back( pcSCH2 );

	ItemTimer * pcXMG = new ItemTimer(ITEMTIMERTYPE_ChristmasGreenHat, ITEMID_None, "Christmas Green Hat", "+2 Move Speed", 0, 0, NULL, NULL);
	pcXMG->AddFreezeMap( MAPID_Bellatra );
	pcXMG->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat.png" );
	pcXMG->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat_.png" );
	pcaItemTimerPrefab.push_back(pcXMG);

	ItemTimer * pcXMR = new ItemTimer(ITEMTIMERTYPE_ChristmasRedHat, ITEMID_None, "Red Christmas Hat", "+50 MP and STM", 0, 0, NULL, NULL);
	pcXMR->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat.png" );
	pcXMR->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat_.png" );
	pcaItemTimerPrefab.push_back(pcXMR);

	ItemTimer * pcBHN = new ItemTimer(ITEMTIMERTYPE_BigHead, ITEMID_None, "Big Head", " ", 0, 0, NULL, NULL);
	pcBHN->AddFreezeMap( MAPID_Bellatra );
	pcBHN->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHN->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBHN);

	ItemTimer * pcBHH = new ItemTimer(ITEMTIMERTYPE_BigHeadHappiness, ITEMID_None, "Big Head of Happiness", "You look so happy! +150 Weight", 0, 0, NULL, NULL);
	pcBHH->AddFreezeMap( MAPID_Bellatra );
	pcBHH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBHH);

	ItemTimer * pcBHL = new ItemTimer(ITEMTIMERTYPE_BigHeadLove, ITEMID_None, "Big Head of Love", "Awh you're in love! +150 Weight", 0, 0, NULL, NULL);
	pcBHL->AddFreezeMap( MAPID_Bellatra );
	pcBHL->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHL->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBHL);

	ItemTimer * pcBHV = new ItemTimer(ITEMTIMERTYPE_BigHeadValentine, ITEMID_None, "Saint Valentine", "+150 Weight, +100 HP, +5 HP/MP regen", 0, 0, NULL, NULL);
	pcBHV->AddFreezeMap( MAPID_Bellatra );
	pcBHV->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHV->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBHV);

	ItemTimer * pcBSD = new ItemTimer(ITEMTIMERTYPE_BigHeadSadness, ITEMID_None, "Big Head of Sadness", "Awh why so sad? +150 Weight", 0, 0, NULL, NULL);
	pcBSD->AddFreezeMap( MAPID_Bellatra );
	pcBSD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBSD);


	ItemTimer * pcBSY = new ItemTimer(ITEMTIMERTYPE_BigHeadShyness, ITEMID_None, "Big Head of Shyness", "You're blushing! +150 Weight", 0, 0, NULL, NULL);
	pcBSY->AddFreezeMap( MAPID_Bellatra );
	pcBSY->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSY->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBSY);

	ItemTimer * pcBHA = new ItemTimer(ITEMTIMERTYPE_BigHeadAngry, ITEMID_None, "Big Head of Angry", "Raaaghhhh so angry! +150 Weight", 0, 0, NULL, NULL);
	pcBHA->AddFreezeMap( MAPID_Bellatra );
	pcBHA->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHA->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBHA);

	ItemTimer * pcBSU = new ItemTimer(ITEMTIMERTYPE_BigHeadSurprised, ITEMID_None, "Big Head of Surprised", "Ohh really? +150 Weight", 0, 0, NULL, NULL);
	pcBSU->AddFreezeMap( MAPID_Bellatra );
	pcBSU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBSU);

	ItemTimer * pcBSN = new ItemTimer(ITEMTIMERTYPE_BigHeadSensual, ITEMID_None, "Big Head of Sensual", "Awimbawe, awimbawe +150 Weight", 0, 0, NULL, NULL);
	pcBSN->AddFreezeMap( MAPID_Bellatra );
	pcBSN->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSN->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab.push_back(pcBSN);

	ItemTimer * pcBCB = new ItemTimer(ITEMTIMERTYPE_BCBuff, ITEMID_None, "PvP Honor", "+100 HP", 0, 0, NULL, NULL);
	pcBCB->AddDisableMap( MAPID_BattleTown );
	pcBCB->AddDisableMap( MAPID_Bellatra );
	pcBCB->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp.png" );
	pcBCB->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp_.png" );
	pcaItemTimerPrefab.push_back(pcBCB);

	ItemTimer * pcPTS = new ItemTimer(ITEMTIMERTYPE_PetSystem, ITEMID_None, "Battle Companion", "+X Weight", 0, 0, NULL, NULL);
	pcPTS->AddDisableMap( MAPID_BattleTown );
	pcPTS->AddDisableMap( MAPID_Bellatra );
	pcPTS->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp.png" );
	pcPTS->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp_.png" );
	pcaItemTimerPrefab.push_back(pcPTS);

	//Box
	ItemTimer::pcImageBox = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\box.png" );
}

void CItemTimerHandler::Render()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame && ISSHOW_INTERFACE )
	{
		for ( ItemTimer * timer : pcaItemTimer )
			timer->RenderTimer();

		for ( ItemTimer * timer : pcaItemTimer )
			timer->RenderBox();
	}
}

void CItemTimerHandler::Loop()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( ItemTimer * timer : pcaItemTimer )
			timer->Loop();
	}
}

void CItemTimerHandler::Tick()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( ItemTimer * timer : pcaItemTimer )
		{
			timer->Tick();

			BOOL bOldDisable = timer->bDisable;

			timer->CheckDisable( MAP_ID );
			timer->CheckFreeze( MAP_ID );

			if ( bOldDisable != timer->bDisable )
			{
				if ( timer->bDisable )
					EventTimerStop( timer );
				else
					EventTimerStart( timer );
			}

			if ( timer->IsAlive() == FALSE )
			{
				EventTimerStop( timer );
			}
		}

		const auto orig_size = pcaItemTimer.size();

		//Remove all non-alive timers.
		//This approach is used to avoid memcpy error when the vector is changed
		//during iteration (when removing a timer)
		//especially when multiple timers expired at the same time
		auto new_end = std::remove_if( pcaItemTimer.begin(), pcaItemTimer.end(),
			[]( ItemTimer * timer )
			{
				return timer->IsAlive() == FALSE;
			} );

		pcaItemTimer.erase( new_end, pcaItemTimer.end() );

		//Size was changed, therefore elements were erased
		if ( pcaItemTimer.size() != orig_size  )
		{
			UPDATE_CHARACTER_CHECKSUM;
			UpdateTimerPositions ();
		}
	}
}

void CItemTimerHandler::OnMouseMove( CMouse * pcMouse )
{
	Point2D * ps = pcMouse->GetPosition();

	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( ItemTimer * timer : pcaItemTimer )
		{
			if ( timer && timer->IsAlive() )
			{
				timer->OnMouseMove( ps );
			}
		}
	}
}

BOOL CItemTimerHandler::OnMouseClick( CMouse * pcMouse )
{
	BOOL bRet = FALSE;

	if ( (Game::GetGameMode() == GAMEMODE_InGame) && (pcMouse->GetEvent() == EMouseEvent::ClickDownR) )
	{
		for ( ItemTimer * timer : pcaItemTimer )
		{
			if ( timer && timer->IsAlive() && timer->IsViewing() )
			{
				MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
				MESSAGEBOX->SetTitle( "Are you sure?" );
				MESSAGEBOX->SetDescription( FormatString("Do you want to cancel the %s?", timer->szName) );
				MESSAGEBOX->SetEvent( std::bind( &CItemTimerHandler::OnClickCancelOK, this, timer, std::placeholders::_1 ) );
				MESSAGEBOX->Show();
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

ItemTimer * CItemTimerHandler::GetPrefab( EItemTimerType iType )
{
	for ( ItemTimer * timer : pcaItemTimerPrefab )
	{
		if ( timer && timer->iType == iType )
			return timer;
	}

	return NULL;
}

ItemTimer * CItemTimerHandler::GetHead()
{
	for ( ItemTimer * timer : pcaItemTimer )
	{
		if ( timer->bAlive )
		{
			if ( IsHead( timer->iType ) )
				return timer;
		}
	}

	return NULL;
}

void CItemTimerHandler::EventTimerStart( ItemTimer * pcTimer )
{
	switch ( pcTimer->iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			int iID;

			iID = PHOENIXTYPEID_Fire;

			if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixIce )
				iID = PHOENIXTYPEID_Ice;
			else if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixLightning )
				iID = PHOENIXTYPEID_Lightning;
			else if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixHeal )
				iID = PHOENIXTYPEID_Healing;

			CHARACTERGAME->SetPhoenix( TRUE, iID );
			break;

		case ITEMTIMERTYPE_HPUp:
			CHARACTERGAME->bHPUp = TRUE;
			break;

		case ITEMTIMERTYPE_MPUp:
			CHARACTERGAME->bMPUp = TRUE;
			break;

		case ITEMTIMERTYPE_MPDown:
			CHARACTERGAME->bMPDown = TRUE;
			break;

		case ITEMTIMERTYPE_SPDown:
			CHARACTERGAME->bSPDown = TRUE;
			break;

		case ITEMTIMERTYPE_WeightStone:
			CHARACTERGAME->bWeightStone = TRUE;
			break;

		case ITEMTIMERTYPE_BCBuff:
			CHARACTERGAME->bTopPVP = TRUE;
			break;

		case ITEMTIMERTYPE_HoneyDamage:
			CHARACTERGAME->bHoneyDamage = TRUE;
			break;

		case ITEMTIMERTYPE_HoneyFast:
			CHARACTERGAME->bHoneyFast = TRUE;
			break;

		case ITEMTIMERTYPE_HoneySmart:
			CHARACTERGAME->bHoneySmart = TRUE;
			break;

		case ITEMTIMERTYPE_HoneyDefense:
			CHARACTERGAME->bHoneyDefense = TRUE;
			break;

		// SERVER WIDE BUFFS
		case ITEMTIMERTYPE_SHPRegen:
			CHARACTERGAME->bSHPRegen = TRUE;
			break;
		case ITEMTIMERTYPE_SSPRegen:
			CHARACTERGAME->bSSPRegen = TRUE;
			break;
		case ITEMTIMERTYPE_SMPRegen:
			CHARACTERGAME->bSMPRegen = TRUE;
			break;
		case ITEMTIMERTYPE_SMPDown:
			CHARACTERGAME->bSMPDown = TRUE;
			break;
		case ITEMTIMERTYPE_SSPDown:
			CHARACTERGAME->bSSPDown = TRUE;
			break;
		case ITEMTIMERTYPE_SBonusDamage:
			CHARACTERGAME->bSBonusDamage = TRUE;
			break;
		case ITEMTIMERTYPE_SBonusAbsorb:
			CHARACTERGAME->bSBonusAbsorb = TRUE;
			break;
		case ITEMTIMERTYPE_SMoveSpeed:
			CHARACTERGAME->bSMoveSpeed = TRUE;
			break;
		// END SERVER WIDE BUFFS

		case ITEMTIMERTYPE_IceResistanceBuff:
			CHARACTERGAME->bIceResistanceBuff = true;
			break;

		case ITEMTIMERTYPE_PhoenixFireSmall:
			CHARACTERGAME->bPhoenixFireSmall = TRUE;
			break;
		case ITEMTIMERTYPE_PhoenixIceSmall:
			CHARACTERGAME->bPhoenixIceSmall = TRUE;
			break;
		case ITEMTIMERTYPE_PhoenixHealingSmall:
			CHARACTERGAME->bPhoenixHealingSmall = TRUE;
			break;
		case ITEMTIMERTYPE_PhoenixLightningSmall:
			CHARACTERGAME->bPhoenixLightningSmall = TRUE;
			break;

		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_SoccerHatSpeed:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_ChristmasGreenHat:
		case ITEMTIMERTYPE_ChristmasRedHat:
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadValentine:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
			ITEMHANDLER->SetHairModel( (DWORD)pcTimer->iItemID, UNITDATA->sCharacterData.iRank );
			UPDATE_CHARACTER_CHECKSUM;
			break;
	}
}

void CItemTimerHandler::EventTimerStop( ItemTimer * pcTimer )
{
	switch ( pcTimer->iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			CHARACTERGAME->SetPhoenix( FALSE, PHOENIXTYPEID_DeleteMask );
			break;

		case ITEMTIMERTYPE_HPUp:
			CHARACTERGAME->bHPUp = FALSE;
			break;

		case ITEMTIMERTYPE_MPUp:
			CHARACTERGAME->bMPUp = FALSE;
			break;

		case ITEMTIMERTYPE_MPDown:
			CHARACTERGAME->bMPDown = FALSE;
			break;

		case ITEMTIMERTYPE_SPDown:
			CHARACTERGAME->bSPDown = FALSE;
			break;

		case ITEMTIMERTYPE_WeightStone:
			CHARACTERGAME->bWeightStone = FALSE;
			break;

		case ITEMTIMERTYPE_BCBuff:
			CHARACTERGAME->bTopPVP = FALSE;
			break;

		case ITEMTIMERTYPE_HoneyDamage:
			CHARACTERGAME->bHoneyDamage = FALSE;
			break;

		case ITEMTIMERTYPE_HoneyFast:
			CHARACTERGAME->bHoneyFast = FALSE;
			break;

		case ITEMTIMERTYPE_HoneySmart:
			CHARACTERGAME->bHoneySmart = FALSE;
			break;

		case ITEMTIMERTYPE_HoneyDefense:
			CHARACTERGAME->bHoneyDefense = FALSE;
			break;

			// SERVER WIDE BUFFS
		case ITEMTIMERTYPE_SHPRegen:
			CHARACTERGAME->bSHPRegen = FALSE;
			break;
		case ITEMTIMERTYPE_SSPRegen:
			CHARACTERGAME->bSSPRegen = FALSE;
			break;
		case ITEMTIMERTYPE_SMPRegen:
			CHARACTERGAME->bSMPRegen = FALSE;
			break;
		case ITEMTIMERTYPE_SMPDown:
			CHARACTERGAME->bSMPDown = FALSE;
			break;
		case ITEMTIMERTYPE_SSPDown:
			CHARACTERGAME->bSSPDown = FALSE;
			break;
		case ITEMTIMERTYPE_SBonusDamage:
			CHARACTERGAME->bSBonusDamage = FALSE;
			break;
		case ITEMTIMERTYPE_SBonusAbsorb:
			CHARACTERGAME->bSBonusAbsorb = FALSE;
			break;
		case ITEMTIMERTYPE_SMoveSpeed:
			CHARACTERGAME->bSMoveSpeed = FALSE;
			break;
			// END SERVER WIDE BUFFS

		case ITEMTIMERTYPE_IceResistanceBuff:
			CHARACTERGAME->bIceResistanceBuff = FALSE;
			break;

		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_SoccerHatSpeed:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_ChristmasGreenHat:
		case ITEMTIMERTYPE_ChristmasRedHat:
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadValentine:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
			ProcessPacket( &PacketOldHeadUse( FALSE ) );
			break;
	}
}


void CItemTimerHandler::RemoveTimer( ItemTimer * pcTimer )
{
	EventTimerStop( pcTimer );

	for ( std::vector<ItemTimer *>::iterator it = pcaItemTimer.begin(); it != pcaItemTimer.end(); it++ )
	{
		ItemTimer * timer = ( *it );
		if ( timer == pcTimer )
		{
			pcaItemTimer.erase( it );
			break;
		}
	}

	UpdateTimerPositions();
}

void CItemTimerHandler::UpdateTimerPositions()
{
	int index = 0;
	for ( ItemTimer * timer : pcaItemTimer )
	{
		if ( timer->IsAlive() == FALSE )
			continue;

		int iColumn = index / DEFAULT_ITEMS_PER_COLUMN;
		int iRow	= index % DEFAULT_ITEMS_PER_COLUMN;

		timer->sMoveTo.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
		timer->sMoveTo.iY = POS_ITMTMRBASEY + (iRow * TMR_SPACEY);

		index++;
	}
}

void CItemTimerHandler::OnResolutionChanged()
{
	if( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for( ItemTimer * timer : pcaItemTimer )
		{
			if( timer->bAlive )
			{
				timer->sPosition.iX = ( RESOLUTION_WIDTH - POS_ITMTMRBASEX ) - ( timer->iColumn * TMR_SPACEX ) - ITEMTIMER_WIDTH;
				timer->sMoveTo.iX = ( RESOLUTION_WIDTH - POS_ITMTMRBASEX ) - ( timer->iColumn * TMR_SPACEX ) - ITEMTIMER_WIDTH;
			}
		}
	}
}

BOOL CItemTimerHandler::AddTimer( EItemTimerType iType, EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft )
{
	if ( iType == EItemTimerType::ITEMTIMERTYPE_ForceOrb )
	{
		LoadForceOrb( iItemID, dwTimeTotal, dwTimeLeft );
		return TRUE;
	}

	int iNextSlot = pcaItemTimer.size();

	if ( iNextSlot != -1 )
	{
		int iColumn = iNextSlot / DEFAULT_ITEMS_PER_COLUMN;
		int iRow	= iNextSlot % DEFAULT_ITEMS_PER_COLUMN;

		ItemTimer * pcTimer = GetItemTimerByType( iType );

		if ( pcTimer )
		{
			//Can Stack?
			if ( ItemTimerCanStack( pcTimer->iType ) )
			{
				RemoveTimer( pcTimer );

				//Use last Slot
				iNextSlot = pcaItemTimer.size();

				//Update Position
				iColumn		= iNextSlot / DEFAULT_ITEMS_PER_COLUMN;
				iRow		= iNextSlot % DEFAULT_ITEMS_PER_COLUMN;

				//Free Pointer
				pcTimer = NULL;
			}
		}

		if ( ( pcTimer == NULL ) && ( IsHead( iType ) == TRUE ) )
		{
			pcTimer = GetHead();
		}

		if ( pcTimer == NULL )
		{
			ItemTimer * pcPrefab = GetPrefab( iType );

			if ( pcPrefab )
			{
				ItemTimer * pcItemTimer = new ItemTimer( iType, iItemID, pcPrefab->szName, pcPrefab->szDescription, dwTimeLeft, dwTimeTotal, pcPrefab->pcImage, pcPrefab->pcImageBackground );

				std::vector<int> vFreezeMaps = pcPrefab->GetFreezeMaps();
				for ( vector<int>::iterator it = vFreezeMaps.begin(); it != vFreezeMaps.end(); it++ )
					pcItemTimer->AddFreezeMap( *it );

				std::vector<int> vDisableMaps = pcPrefab->GetDisabledMaps();
				for ( vector<int>::iterator it = vDisableMaps.begin(); it != vDisableMaps.end(); it++ )
					pcItemTimer->AddDisableMap( *it );

				pcItemTimer->iColumn = iColumn;
				pcItemTimer->sPosition.iY = POS_ITMTMRBASEY;

				pcItemTimer->sMoveTo.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
				pcItemTimer->sMoveTo.iY = POS_ITMTMRBASEY + (iRow * TMR_SPACEY);

				if ( !pcItemTimer->bDisable )
					EventTimerStart( pcItemTimer );

				pcaItemTimer.push_back( pcItemTimer );

				UPDATE_CHARACTER_CHECKSUM;

				return TRUE;
			}
		}
		else
			TITLEBOX( "You are already using an item of this type!" );
	}

	return FALSE;
}



BOOL CItemTimerHandler::LoadForceOrb( EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft )
{
	DWORD forceIndex0Based = ( ( iItemID & 0xFFFF ) >> 8 ) - 1; //0 = Lucidy force and so on

	if ( forceIndex0Based < 0 || forceIndex0Based > 16 )
	{
		//CHATBOX->AddMessage( FormatString( "Force orb load error %d", forceIndex0Based ) );
		return FALSE;
	}

	if ( !dwaMatForceOrbEffect[forceIndex0Based] )
	{
		std::string path = FormatString ( "image\\Sinimage\\skill\\ForceOrb\\FOS_0%d.tga", forceIndex0Based + 1 );
		dwaMatForceOrbEffect[forceIndex0Based] = TIMERSKILLHANDLER->CreateTextureMaterial ( path.c_str() );
	}

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( SKILLID_ForceOrb );

	if ( psSkill )
		ZeroMemory ( psSkill, sizeof ( Skill ) );

	TIMERSKILLHANDLER->KillTimer ( SKILLID_ForceOrb );

	Skill sSkill;
	ZeroMemory ( &sSkill, sizeof ( sSkill ) );


	sSkill.iFlag = TRUE;
	sSkill.iID = SKILLID_ForceOrb;
	sSkill.iLevel = forceIndex0Based + 1;
	sSkill.iUseTime = dwTimeLeft;
	sSkill.iCheckTime = 0;
	sSkill.MatIcon = dwaMatForceOrbEffect[forceIndex0Based];
	TIMERSKILLHANDLER->AddSkillTimer ( &sSkill );

	UNITDATA->eForceOrbItemID					= iItemID;
	UNITDATA->dwForceOrbTimeOut					= TICKCOUNT + (dwTimeLeft * 1000);
	CHARACTERGAME->dwForceOrbTimeRemaining		= dwTimeLeft;
	CHARACTERGAME->dwForceOrbTotalDuration		= dwTimeTotal;

	fnAssaParticleShelltomWeapon( UNITDATA, 30 * 70, forceIndex0Based );

	return TRUE;
}

BOOL CItemTimerHandler::SyncForceOrb( EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft )
{
	DWORD forceIndex0Based = ( ( iItemID & 0xFFFF ) >> 8 ) - 1; //0 = Lucidy force and so on

	if ( forceIndex0Based < 0 || forceIndex0Based > 16 )
	{
		//CHATBOX->AddMessage( FormatString( "Force orb load error %d", forceIndex0Based ) );
		return FALSE;
	}

	//CHATBOX->AddMessage ( FormatString("Force orb sync %d", psPacket->sForceOrb.dwTimeLeft) );
	Skill * sSkill = TIMERSKILLHANDLER->FindSkillTimer ( SKILLID_ForceOrb );
	if ( sSkill )
	{
		sSkill->iUseTime = dwTimeTotal;
		sSkill->iCheckTime = (dwTimeTotal - dwTimeLeft) * 70;
	}

	UNITDATA->eForceOrbItemID				= iItemID;
	UNITDATA->dwForceOrbTimeOut				= TICKCOUNT + (dwTimeLeft * 1000);
	CHARACTERGAME->dwForceOrbTimeRemaining	= dwTimeLeft;
	CHARACTERGAME->dwForceOrbTotalDuration	= dwTimeTotal;

	fnAssaParticleShelltomWeapon( UNITDATA, 30 * 70, forceIndex0Based );

	return TRUE;
}

BOOL CItemTimerHandler::CancelForceOrb( PacketCommand * packet )
{
	EItemID  iItemID = (EItemID)packet->p1;

	TIMERSKILLHANDLER->KillTimer ( SKILLID_ForceOrb );

	UNITDATA->eForceOrbItemID						= EItemID::ITEMID_None;
	UNITDATA->dwForceOrbTimeOut						= 0;
	CHARACTERGAME->dwForceOrbTimeRemaining			= 0;
	CHARACTERGAME->dwForceOrbTimeRemainingAtCity	= 0;
	CHARACTERGAME->dwForceOrbTotalDuration			= 0;

	DWORD forceIndex0Based = ( ( iItemID & 0xFFFF ) >> 8 ) - 1; //0 = Lucidy force and so on
	if ( forceIndex0Based < 0 || forceIndex0Based > 16 )
	{
		fnAssaParticleShelltomWeapon ( UNITDATA, 0, 0 );
		return FALSE;
	}
	else
	{
		fnAssaParticleShelltomWeapon ( UNITDATA, 0, forceIndex0Based );
	}

	return TRUE;
}


void CItemTimerHandler::HandlePacket( PacketNewItemTimer * psPacket )
{
	AddTimer( psPacket->iType, psPacket->iItemID, psPacket->dwTimeTotal, psPacket->dwTimeLeft );
}

void CItemTimerHandler::HandlePacket( PacketItemPremiumData * psPacket, BOOL bSyncOnly )
{
	for ( int i = 0; i < _countof( psPacket->saItemList ); i++ )
	{
		ItemPremium * p = psPacket->saItemList + i;

		if ( p && p->dwCode )
		{
			if ( bSyncOnly )
			{
				if ( p->eType == EItemTimerType::ITEMTIMERTYPE_ForceOrb )
				{
					SyncForceOrb( (EItemID)p->dwCode, p->dwTimeTotal, p->dwTimeLeft );
					continue;
				}

				ItemTimer * pcTimer = GetItemTimerByType ( (EItemTimerType)p->eType );
				if ( pcTimer &&
					 pcTimer->dwTimeTotal == p->dwTimeTotal)
				{
					pcTimer->dwTimeLeft = p->dwTimeLeft;
					pcTimer->dwTimeTotal = p->dwTimeTotal;
				}
			}
			else
			{
				AddTimer ( p->eType, (EItemID)p->dwCode, p->dwTimeTotal, p->dwTimeLeft );
			}
		}
	}
}

void CItemTimerHandler::HandlePacket( PacketOldHeadUse * psPacket )
{
	UNITDATA->bNoMove = TRUE;

	if ( psPacket->szHeadModel[0] != 0 )
		STRINGCOPY( UNITDATA->sCharacterData.Player.szHeadModel, psPacket->szHeadModel );
	else
	{
		CHARACTERGAME->ResetHead();
		return;
	}

	SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );
	CHECK_CHARACTER_CHECKSUM;
}

void CItemTimerHandler::ProcessPacket( PacketOldHeadUse * psPacket )
{
	psPacket->iLength = sizeof( PacketOldHeadUse );
	psPacket->iHeader = PKTHDR_CharacterHeadOldUse;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::ProcessPacket( PacketNewItemTimer * psPacket )
{
	psPacket->iLength = sizeof( PacketNewItemTimer );
	psPacket->iHeader = PKTHDR_SetItemTimer;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::ProcessPacket( PacketCancelItemTimer * psPacket )
{
	psPacket->iLength = sizeof( PacketCancelItemTimer );
	psPacket->iHeader = PKTHDR_CancelItemTimer;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::UpdateOldHead()
{
	PacketOldHeadUse s;
	STRINGCOPY( s.szHeadModel, UNITDATA->sCharacterData.Player.szHeadModel );
	ProcessPacket( &s );
}

ItemTimer::ItemTimer( EItemTimerType iType, EItemID iItemID, char * pszName, char * pszDescription, DWORD dwTimeLeft, DWORD dwTimeTotal, UI::Image * pcImage, UI::Image * pcImageBackground )
{
	this->iType				= iType;
	this->iItemID			= iItemID;

	STRINGCOPY( szName, pszName );
	STRINGCOPY( szDescription, pszDescription );

	this->dwTimeLeft		= dwTimeLeft;
	this->dwTimeTotal		= dwTimeTotal;

	this->pcImage			= pcImage;
	this->pcImageBackground = pcImageBackground;

	bIsMouseHovering		= FALSE;
	bFreeze					= FALSE;
	bDisable				= FALSE;
	bAlive					= TRUE;
	iFade					= 0;
	sPosition.iX			= RESOLUTION_WIDTH;
	sPosition.iY			= 0;
	sPosition.iWidth		= ITEMTIMER_WIDTH;
	sPosition.iHeight		= ITEMTIMER_HEIGHT;
	sMoveTo.iX				= 0;
	sMoveTo.iY				= 0;
	sMoveTo.iWidth			= ITEMTIMER_WIDTH;
	sMoveTo.iHeight			= ITEMTIMER_HEIGHT;
	iColumn					= 0;

	vFreezeMaps.clear();

	if (   iType == ITEMTIMERTYPE_SHPRegen
		|| iType == ITEMTIMERTYPE_SSPRegen
		|| iType == ITEMTIMERTYPE_SMPRegen
		|| iType == ITEMTIMERTYPE_SMPDown
		|| iType == ITEMTIMERTYPE_SSPDown
		|| iType == ITEMTIMERTYPE_SBonusDamage
		|| iType == ITEMTIMERTYPE_SBonusAbsorb
		|| iType == ITEMTIMERTYPE_SMoveSpeed
		|| iType == ITEMTIMERTYPE_SExpBuff
		|| iType == ITEMTIMERTYPE_SDropBuff)
	{
		//don't freeze these items in towns
		return;
	}

	vFreezeMaps.push_back(MAPID_RicartenTown);
	vFreezeMaps.push_back(MAPID_PillaiTown);
	vFreezeMaps.push_back(MAPID_Eura);
	vFreezeMaps.push_back(MAPID_Atlantis);
	vFreezeMaps.push_back(MAPID_NaviskoTown);
}

ItemTimer::~ItemTimer()
{
	vFreezeMaps.clear();
}

void ItemTimer::RenderTimer()
{
	UI::ImageRender::Render( pcImageBackground, sPosition.iX, sPosition.iY, ITEMTIMER_WIDTH, ITEMTIMER_HEIGHT, D3DCOLOR_ARGB( iFade, 255, 255, 255 ) );

	float fTimePast = (float)(dwTimeTotal - dwTimeLeft);
	int iTop		= int( (fTimePast * ITEMTIMER_HEIGHT) / (float)dwTimeTotal );



	if ( iTop < ITEMTIMER_HEIGHT )
	{
		if ( bFreeze || bDisable  )
		{
			UI::ImageRender::Render( pcImage, sPosition.iX, sPosition.iY + iTop, ITEMTIMER_WIDTH, ITEMTIMER_HEIGHT - iTop, 0, iTop, D3DCOLOR_ARGB( iFade < 100 ? iFade :100, 255, 255, 255 ) );
		}
		else
		{
			UI::ImageRender::Render( pcImage, sPosition.iX, sPosition.iY + iTop, ITEMTIMER_WIDTH, ITEMTIMER_HEIGHT - iTop, 0, iTop, D3DCOLOR_ARGB( iFade, 255, 255, 255 ) );
		}

	}
}

void ItemTimer::RenderBox()
{
	static char szTimeDescription[64] = { 0 };

	if ( bIsMouseHovering )
	{
		UI::ImageRender::Render( ItemTimer::pcImageBox, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth(), sMoveTo.iY, ItemTimer::pcImageBox->GetWidth(), ItemTimer::pcImageBox->GetHeight(), -1 );

		FONTHANDLER->Draw( 0, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth() + 8, sMoveTo.iY + 6, szName, 0, RGBA( 255, 225, 225, 225 ) );

		FONTHANDLER->Draw( 0, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth() + 8, sMoveTo.iY + 22, szDescription, 0, RGBA( 255, 255, 255, 255 ) );

		int iHour	 = (dwTimeLeft / 3600);
		int iMinutes = (dwTimeLeft / 60) % 60;
		int iSeconds = (dwTimeLeft % 60);


		if ( bFreeze || bDisable )
		{
			STRINGFORMAT ( szTimeDescription, "Time Left: %02d:%02d:%02d (Paused)", iHour, iMinutes, iSeconds );
		}
		else
		{
			STRINGFORMAT ( szTimeDescription, "Time Left: %02d:%02d:%02d", iHour, iMinutes, iSeconds );
		}

		FONTHANDLER->Draw ( 0, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth () + 8, sMoveTo.iY + 51, szTimeDescription, 0, RGBA ( 255, 50, 205, 50 ) );
	}
}

void ItemTimer::Loop()
{
	if ( iFade < ALPHA_HIGH )
	{
		iFade += 1;
		if ( iFade > ALPHA_HIGH )
			iFade = ALPHA_HIGH;
	}

	if ( sMoveTo.iX > sPosition.iX )
	{
		sPosition.iX += 1;

		if ( sPosition.iX > sMoveTo.iX )
			sPosition.iX = sMoveTo.iX;
	}
	else if ( sMoveTo.iX < sPosition.iX )
	{
		sPosition.iX -= 1;

		if ( sPosition.iX < sMoveTo.iX )
			sPosition.iX = sMoveTo.iX;
	}

	if ( sMoveTo.iY > sPosition.iY )
	{
		sPosition.iY += 1;

		if ( sPosition.iY > sMoveTo.iY )
			sPosition.iY = sMoveTo.iY;
	}
	else if ( sMoveTo.iY < sPosition.iY )
	{
		sPosition.iY -= 1;
		if ( sPosition.iY < sMoveTo.iY )
			sPosition.iY = sMoveTo.iY;
	}
}

void ItemTimer::Tick()
{
	if ( dwTimeLeft != 0 )
	{
		if ( (bFreeze == FALSE && bDisable == FALSE) && (bAlive == TRUE) )
		{
			if ( (dwTimeLeft--) == 0 )
				Die();
		}
	}
	else
	{
		Die ();
	}
}

void ItemTimer::OnMouseMove( Point2D * psPosition )
{
	if ( !Moving() )
	{
		if ( sMoveTo.Inside( psPosition ) )
			bIsMouseHovering = TRUE;
		else
			bIsMouseHovering = FALSE;
	}
	else
		bIsMouseHovering = FALSE;
}

void ItemTimer::CheckFreeze( int iMapID )
{
	bFreeze = FALSE;

	for (int mapId : vFreezeMaps )
	{
		if ( mapId == iMapID )
		{
			bFreeze = TRUE;
			break;
		}
	}
}

void ItemTimer::CheckDisable( int iMapID )
{
	bDisable = FALSE;

	for (int mapId : vDisableMaps )
	{
		if ( mapId == iMapID )
		{
			bDisable = TRUE;
			break;
		}
	}
}

BOOL ItemTimer::Moving()
{
	if ( sPosition.Equals( &sMoveTo ) )
		return FALSE;

	return TRUE;
}
