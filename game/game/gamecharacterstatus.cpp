#include "stdafx.h"
#include "gamecharacterstatus.h"


CGameCharacterStatus::CGameCharacterStatus()
{
}


CGameCharacterStatus::~CGameCharacterStatus()
{
}

void CGameCharacterStatus::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

#ifdef ITEMFIXES
void CGameCharacterStatus::UpdateCharacterStatus( CharacterDataLocal * psData, ItemFixData eItemFixData )
#else
void CGameCharacterStatus::UpdateCharacterStatus( CharacterDataLocal * psData)
#endif
{
	int iESP14 = psData->iAttackRatingItems;
	psData->iAttackRatingItems = CHARACTERGAME->GetAttackRatingFromEquippedItems (); // READDWORD( 0x04B0DA60 ); //attack rating of items

	CharacterData * playerData = PLAYERDATA;

	//Consts
	const DWORD				pdwInventory = 0x035EBB20;
	const DWORD				pdwViewDamagePercent = pdwInventory + 0x39C24;
	const DWORD				pdwViewDamageNumber = pdwInventory + 0x39C28;

	//Visual Damage
	if ( READDWORD( pdwViewDamagePercent ) != 0 )
	{
		//Percent Damage
		(*(int*)0x035EB600) += ((playerData->iMinDamage + 1) * READDWORD( pdwViewDamagePercent )) / 100;
		(*(int*)0x035EB604) += ((playerData->iMaxDamage + 1) * READDWORD( pdwViewDamagePercent )) / 100;
	}
	else if ( READDWORD( pdwViewDamageNumber ) != 0 )
	{
		//Static value on Damage
		(*(int*)0x035EB600) += READDWORD( pdwViewDamageNumber );
		(*(int*)0x035EB604) += READDWORD( pdwViewDamageNumber );
	}

	float fLevel	= (float)playerData->iLevel;
	float fStrength = (float)playerData->iStrength;
	float fSpirit	= (float)playerData->iSpirit;
	float fTalent	= (float)playerData->iTalent;
	float fAgility	= (float)playerData->iAgility;
	float fHealth	= (float)playerData->iHealth;


	//Attack Rating
#ifdef ITEMFIXES
	int iAttackRating  = (psData->iAttackRatingItems + (int)((fAgility * 3.0f) + (fLevel * 2.0f) + (fTalent * 1.5f))) + psData->iLevelAttackRating + eItemFixData.iATKRating;
#else
	int iAttackRating  = (psData->iAttackRatingItems + (int)((fAgility * 3.0f) + (fLevel * 2.0f) + (fTalent * 1.5f))) + psData->iLevelAttackRating;
#endif


	playerData->iAttackRating = iAttackRating;
	CHARACTERGAME->sCharacterDataEx.iBaseAttackRating = iAttackRating;


	//Critical with 100% cap
#ifdef ITEMFIXES
	playerData->iCritical = psData->iCriticalItems + psData->iCriticalSkills + (int)(fTalent / 50.0f) + (int)eItemFixData.fCritical;
#else
	playerData->iCritical = psData->iCriticalItems + psData->iCriticalSkills + (int)(fTalent / 50.0f);
#endif
	if ( playerData->iCritical > 100 )
		playerData->iCritical = 100;

	//Defense Formula
#ifdef ITEMFIXES
	playerData->iDefenseRating = (int)((fAgility/2) + (fTalent / 4.0f) + fLevel * 1.4f + psData->iDefenseSkills) + psData->iDefenseAdd + eItemFixData.iDefense;
#else
	playerData->iDefenseRating = (int)((fAgility/2) + (fTalent / 4.0f) + fLevel * 1.4f + psData->iDefenseSkills) + psData->iDefenseAdd;
#endif
	float fDefense = (float)playerData->iDefenseRating;


	CHARACTERGAME->sCharacterDataEx.iBaseDefenseRating = playerData->iDefenseRating;

	//Absorb formula with items that have formula div 2 ( 0.5 abs )
	psData->iCountAbsorbItems /= 2;
#ifdef ITEMFIXES
	playerData->iAbsorbRating = (int)(((fDefense / 100.0f) + (fLevel / 10.0f) + psData->fAbsorbSkills + psData->fAbsorbAdd) + ((fStrength + fTalent) / 40) + psData->iCountAbsorbItems + 1) + eItemFixData.fAbsorb;
#else
	playerData->iAbsorbRating = (int)(((fDefense / 100.0f) + (fLevel / 10.0f) + psData->fAbsorbSkills + psData->fAbsorbAdd) + ((fStrength + fTalent) / 40) + psData->iCountAbsorbItems + 1);
#endif


	CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating = playerData->iAbsorbRating;

	//Attack Range for Rangers
#ifdef ITEMFIXES
	playerData->iAttackRange = psData->iAttackRange + psData->iAttackRangeAdd + eItemFixData.iRange;
#else
	playerData->iAttackRange = psData->iAttackRange + psData->iAttackRangeAdd;
#endif

	//Sight default is 180
	playerData->iSight = psData->iViewSight + 180;
	playerData->iSight = psData->iViewSight + 180;

	//Block Rating with 50% cap
#ifdef ITEMFIXES
	playerData->iBlockRating = (int)(round( psData->fBlockRating + psData->fBlockRatingAdd + eItemFixData.fBlock ));
#else
	playerData->iBlockRating = (int)(round( psData->fBlockRating + psData->fBlockRatingAdd));
#endif
	if ( playerData->iBlockRating > 50 )
		playerData->iBlockRating = 50;

	int iWeightAdd = 0;

	//Gravity Stone?
	if ( READDWORD( 0x035D0C50 ) > 0 )
	{
		iWeightAdd += READDWORD( 0x035D0C54 );
	}

	//Weight Formula based on status...
	playerData->sWeight.sMax = (short)((fStrength * 2.0f) + (fHealth + 1.5f) + (fLevel * 3.0f) + 60 +
		playerData->iSpirit + playerData->iTalent + playerData->iAgility + iWeightAdd);

	float fWeightCur = playerData->sWeight.sCur;
	float fWeightMax = playerData->sWeight.sMax;

	//Move Speed
#ifdef ITEMFIXES
	playerData->iMovementSpeed = (int)( (fAgility / 100) + (fStrength / 240) + (fLevel / 100) - (fWeightCur / fWeightMax) + (psData->fMovementSpeed + psData->fMovementSpeedAdd) + 1 + eItemFixData.fRunSpeed);
#else
	playerData->iMovementSpeed = (int)( (fAgility / 100) + (fStrength / 240) + (fLevel / 100) - (fWeightCur / fWeightMax) + (psData->fMovementSpeed + psData->fMovementSpeedAdd) + 1);
#endif

	//HP Formula based on Class
	switch ( playerData->iClass )
	{
		case CHARACTERCLASS_Fighter:
			playerData->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.9f) + (fStrength * 0.5f) + (fSpirit * 0.1f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Mechanician:
			playerData->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 2.7f) + (fStrength * 0.5f) + (fSpirit * 0.1f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Archer:
			playerData->sHP.sMax = (short)((fLevel * 1.2f) + (fHealth * 2.2f) + (fStrength * 0.2f) + (fSpirit * 0.1f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Pikeman:
			playerData->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 2.8f) + (fStrength * 0.6f) + (fSpirit * 0.1f) + (fTalent * 0.1f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Atalanta:
			playerData->sHP.sMax = (short)((fLevel * 1.2f) + (fHealth * 2.6f) + (fStrength * 0.3f) + (fSpirit * 0.2f) + (fTalent * 0.1f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Knight:
			playerData->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 2.7f) + (fStrength * 0.6f) + (fSpirit * 0.2f) + (fTalent * 0.3f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Magician:
			playerData->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 2.0f) + (fStrength * 0.1f) + (fSpirit * 0.2f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Priestess:
			playerData->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 2.3f) + (fStrength * 0.1f) + (fSpirit * 0.3f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Assassin:
			playerData->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 2.0f) + (fStrength * 0.4f) + (fSpirit * 0.1f) + (fTalent * 0.1f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Shaman:
			playerData->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 2.3f) + (fStrength * 0.1f) + (fSpirit * 0.2f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		default:
			break;
	}

	//All classes receive Add HP by spec and skills...
	playerData->sHP.sMax += (short)((float)psData->iLevelHP + psData->fIncreaseHPAdd);

#ifdef ITEMFIXES
	//Add From Affixes
	playerData->sHP.sMax += eItemFixData.iAddHP;
	playerData->sMP.sMax += eItemFixData.iAddMP;
	playerData->fHPRegen += eItemFixData.fRegenerationHP;
	playerData->fMPRegen += eItemFixData.fRegenerationMP;

	//Needs testing to ensure these variables don't change positon in the array, seems super random atm
	playerData->sElementalDef[0] += eItemFixData.iOrganic;
	playerData->sElementalDef[2] += eItemFixData.iFire;
	playerData->sElementalDef[3] += eItemFixData.iFrost;
	playerData->sElementalDef[4] += eItemFixData.iLightning;
	playerData->sElementalDef[5] += eItemFixData.iPoison;
#endif

	//Items Premium of HP
	if ( READDWORD( 0x035D0C80 ) > 0 )
	{
		playerData->sHP.sMax += (short)READDWORD( 0x035D0C84 );
	}

	if (playerData->iLevel >= 90)
	{
		//Completed the level 90 Eura Village quest?
		//If so, add +40 hp
		if ((PLAYER_QUESTLOG & EAsmQuestBit::Level_90) != 0)
		{
			playerData->sHP.sMax += 40;
		}
	}


	//Quest 40
	//if ( READDWORD( 0x0362B6A4 ) == 0 )
	//{
	//	if ( ITEMHANDLER->GetItemAmountByCode( (EItemID)0, EItemCraftType::ITEMCRAFTTYPE_QuestWeapon ) == 0 )
	//	{
	//		if ( playerData->iRank >= ECharacterRank::CHARACTERRANK_Rank3 && playerData->iLevel >= 40 )
	//			playerData->sHP.sMax += 15;
	//	}
	//}


	//HP for Virtual Life
	//(*(short*)0x04B0BA48) = playerData->sHP.sMax;
	//playerData->sHP.sMax += (short)(*(int*)0x0367E0E4);

	//MP Formula based on type of characters ( Meele, Range, Magic )
	switch ( playerData->iMPType )
	{
		//Mage & Prs
		case 1:
			playerData->sMP.sMax = (short)((fLevel * 1.5f) + (fSpirit * 3.8f));
			playerData->sMP.sMax += (short)(psData->fIncreaseMPAdd);
			break;
		//Knight & Atalanta
		case 2:
			playerData->sMP.sMax = (short)((fLevel * 0.9f) + (fSpirit * 2.7f));
			playerData->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;
		//Fs, MS...
		case 3:
			playerData->sMP.sMax = (short)((fLevel * 0.6f) + (fSpirit * 2.2f));
			playerData->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;

		default:
			break;
	}



	//SP Formula is static...
	playerData->sSP.sMax = (short)((fHealth * 1.4f) + (fStrength * 0.5f) + (fTalent * 0.5f) + (fLevel * 2.3f) + 80 + fSpirit);
	playerData->sSP.sMax += (short)(psData->fIncreaseSPAdd);

	//Regen ( its not a formula )
	playerData->fHPRegen = psData->fRegenHP + psData->fPercentHPAdd;
	playerData->fMPRegen = psData->fRegenMP + psData->fPercentMPAdd;
	playerData->fSPRegen = psData->fRegenSP + psData->fPercentSPAdd;

	//Attack Speed Formula
	playerData->iAttackSpeed = psData->iWeaponSpeed + psData->iAttackSpeedAdd + psData->iSkillWeaponSpeed;

	psData->iAttackRatingItems = iESP14;
}

void CGameCharacterStatus::UpdateCharacterStatusItem( BOOL bCheckHack )
{
	ElementalAttackData * psElementalAttack = (ElementalAttackData*)0x035E5BB8;
	ZeroMemory( psElementalAttack, sizeof( ElementalAttackData ) );

	ItemData cItemWrapper;

	ZeroMemory( &cItemWrapper, sizeof( ItemData ) );
}
