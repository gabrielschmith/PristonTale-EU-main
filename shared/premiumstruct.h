#include <Windows.h>

#define PREMIUM_ITEM_TOTAL 65

enum EItemTimerType
{
	ITEMTIMERTYPE_None				= 0,
	ITEMTIMERTYPE_ExpUp				= 1,
	ITEMTIMERTYPE_ThirdEye			= 2,
	ITEMTIMERTYPE_Phoenix			= 3,
	ITEMTIMERTYPE_PhoenixFire		= 4,
	ITEMTIMERTYPE_PhoenixIce		= 5,
	ITEMTIMERTYPE_PhoenixLightning  = 6,
	ITEMTIMERTYPE_PhoenixHeal		= 7,
	ITEMTIMERTYPE_MPDown			= 8,
	ITEMTIMERTYPE_SPDown			= 9,
	ITEMTIMERTYPE_HPUp				= 10,
	ITEMTIMERTYPE_MPUp				= 11,
	ITEMTIMERTYPE_BigHead			= 12,
	ITEMTIMERTYPE_WeightStone		= 13,
	ITEMTIMERTYPE_MatureStone		= 14,
	ITEMTIMERTYPE_SkillStone		= 15,
	ITEMTIMERTYPE_CartolaHat		= 16,
	ITEMTIMERTYPE_WitchHat			= 17,
	ITEMTIMERTYPE_SheepHat			= 18,
	ITEMTIMERTYPE_GiraffeHat		= 19,
	ITEMTIMERTYPE_SoccerHat			= 20,
	ITEMTIMERTYPE_ChristmasGreenHat	= 21,
	ITEMTIMERTYPE_ChristmasRedHat	= 22,
	ITEMTIMERTYPE_BigHeadHappiness	= 23,
	ITEMTIMERTYPE_BigHeadLove		= 24,
	ITEMTIMERTYPE_BigHeadSadness    = 25,
	ITEMTIMERTYPE_BigHeadShyness    = 26,
	ITEMTIMERTYPE_BigHeadAngry      = 27,
	ITEMTIMERTYPE_BigHeadSurprised  = 28,
	ITEMTIMERTYPE_BigHeadSensual	= 29,
	ITEMTIMERTYPE_BCBuff			= 30,
	ITEMTIMERTYPE_PumpkinHat		= 31,
	ITEMTIMERTYPE_BigHeadValentine  = 32,
	ITEMTIMERTYPE_EasterSheepHat	= 33,
	ITEMTIMERTYPE_ForceOrb			= 34,
	ITEMTIMERTYPE_HoneyDamage		= 35,
	ITEMTIMERTYPE_HoneyFast			= 36,
	ITEMTIMERTYPE_HoneySmart		= 37,
	ITEMTIMERTYPE_HoneyDefense		= 38,
	ITEMTIMERTYPE_SoccerHatSpeed	= 39,
	ITEMTIMERTYPE_ChelseaSoccer		= 40,
	ITEMTIMERTYPE_ManchesterSoccer	= 41,
	ITEMTIMERTYPE_PhoenixFireSmall	= 42,
	ITEMTIMERTYPE_PhoenixIceSmall	= 43,
	ITEMTIMERTYPE_PhoenixLightningSmall = 44,
	ITEMTIMERTYPE_PhoenixHealingSmall	= 45,
	ITEMTIMERTYPE_SixthSense		= 46,
	ITEMTIMERTYPE_PetSystem = 58,

	//Server Wide Buffs
	ITEMTIMERTYPE_SHPRegen			= 47,
	ITEMTIMERTYPE_SSPRegen			= 48,
	ITEMTIMERTYPE_SMPRegen			= 49,
	ITEMTIMERTYPE_SMPDown			= 50,
	ITEMTIMERTYPE_SSPDown			= 51,
	ITEMTIMERTYPE_SBonusDamage		= 52,
	ITEMTIMERTYPE_SBonusAbsorb		= 53,
	ITEMTIMERTYPE_SMoveSpeed		= 54,
	ITEMTIMERTYPE_SExpBuff			= 55,
	ITEMTIMERTYPE_SDropBuff			= 56,

	//Resistance Flaks
	ITEMTIMERTYPE_IceResistanceBuff = 57,
};

enum
{
	ITEMTIMERTIME_1H							= (60*60),
	ITEMTIMERTIME_3H							= (60*60*3),
	ITEMTIMERTIME_6H							= (60*60*6),
	ITEMTIMERTIME_12H							= (60*60*12),
	ITEMTIMERTIME_24H							= (60*60*24),
	ITEMTIMERTIME_48H							= (60*60*48),
	ITEMTIMERTIME_72H							= (60*60*72),
};


struct PremiumItem
{
	enum EItemTimerType							eType;
	DWORD										dwCode;
	DWORD										dwTimeLeft;
	BOOL										bRepeatUse;
	BOOL										bOnlyServerUse;
	DWORD										dwLevel;
	char *										pszName;
	char *										pszDescription;
	char *										pszImage;
	char *										pszImageBlack;
};



struct ItemPremium
{
	EItemTimerType								eType;			  //Item timer type
	DWORD										dwCode;			  //Item code
	DWORD										dwTimeLeft;		  //Time remaining
	DWORD										dwTimeTotal;	  //Total time for this item
};
struct PacketItemPremiumData
{
	int											iSize;
	int											iHeader;
	int											iCount;

	ItemPremium									saItemList[PREMIUM_ITEM_TOTAL];
};



struct PacketActivePremium
{
	int											iLength;
	int											iHeader;
	ItemPremium									sPremium;
};


static BOOL ItemTimerCanStack( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_BCBuff:
		case ITEMTIMERTYPE_HoneyDamage:
		case ITEMTIMERTYPE_HoneyFast:
		case ITEMTIMERTYPE_HoneySmart:
		case ITEMTIMERTYPE_HoneyDefense:
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
		case ITEMTIMERTYPE_PhoenixFireSmall:
		case ITEMTIMERTYPE_PhoenixIceSmall:
		case ITEMTIMERTYPE_PhoenixLightningSmall:
		case ITEMTIMERTYPE_PhoenixHealingSmall:
			bRet = TRUE;
			break;
	}

	return bRet;
}

static BOOL ItemTimerIsSame( int iType, int iTypeOther )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			switch ( iTypeOther )
			{
				case ITEMTIMERTYPE_PhoenixFire:
				case ITEMTIMERTYPE_PhoenixIce:
				case ITEMTIMERTYPE_PhoenixLightning:
				case ITEMTIMERTYPE_PhoenixHeal:
					bRet = TRUE;
					break;
			}
			break;
	}

	return bRet;
}


