#include "stdafx.h"
#include "SkillManager.h"


typedef				bool( __cdecl *t_IsActiveBuff )( UINT BuffID );
t_IsActiveBuff		IsActiveBuffFunc = ( t_IsActiveBuff )0x005B4810;

typedef				void( __cdecl *tfnStartSkillEffect )(int x, int y, int z, int angleX, int angleY, int angleZ, int skillType, int level);
tfnStartSkillEffect	fnStartSkillEffect = (tfnStartSkillEffect)0x00574250;

typedef				void( __cdecl * tfnAddTimeEffect )( DWORD EffectCode, DWORD dwTime, int x, int y, int z );
tfnAddTimeEffect	fnAddTimeEffect = (tfnAddTimeEffect)0x0045D020;

typedef				int( __cdecl *tfnSkillRetryPlayAttack )(UnitData * pcUnitData );
tfnSkillRetryPlayAttack fnSkillRetryPlayAttack = (tfnSkillRetryPlayAttack)0x0040F3F0;

typedef				void( __cdecl *tfnSkillPlaySound )(DWORD dwCode, int iX, int iY, int iZ, int iFlag);
tfnSkillPlaySound	fnSkillPlaySound = (tfnSkillPlaySound)0x00544870;

typedef				void( __cdecl * tfnPlayWeaponSound )( UnitData * pcUnitData );
tfnPlayWeaponSound	fnPlayWeaponSound = (tfnPlayWeaponSound)0x00544170;



extern Skill sTempSkillTimerData[81];

extern unsigned int __cdecl CustomGetAttackFrameSpeed ( int iAttackSpeed, int iAddSpeed );


static DebuffSkill saDebufSkills[]
{
	{ CHARACTERCLASS_Fighter, SKILLPLAYID_T23 },
	{ CHARACTERCLASS_Pikeman, SKILLPLAYID_T21 },
	{ CHARACTERCLASS_Atalanta, SKILLPLAYID_T11 },
	{ CHARACTERCLASS_Magician, SKILLPLAYID_T43 },
	{ CHARACTERCLASS_Priestess, SKILLPLAYID_T41 },
	{ CHARACTERCLASS_Shaman, SKILLPLAYID_T13 },
	{ CHARACTERCLASS_Pikeman, SKILLPLAYID_T51 },
};

#define TEXTCOLOR_DMGVSMONSTERTYPE		D3DCOLOR_XRGB( 212, 39, 232 )
#define TEXTCOLOR_SKILLCHAIN			D3DCOLOR_XRGB( 105, 144, 176 ) //blue
#define TEXTCOLOR_SKILLSTATS			D3DCOLOR_XRGB( 34, 91, 138 ) //dark blue

#define TRIUMPHOFVALHALLA_FLAG2				(*(int*)0x0367E040) //SetT_Of_ValhallaFlag2 - If this flag is set, this player used the skill
#define TRIUMPHOFVALHALLA_FLAG1				(*(int*)0x0367E03C) //SetT_Of_ValhallaFlag - If this flag is set, someone else used the skill
#define TRIUMPHOFVALHALLA_PLAYERLEVEL		(*(int*)0x0367E044) //SetT_Of_ValhallaLV - Level of player who casted the skill

#define HALLOFVALHALLA_SKILLLEVEL			(*(int*)0x0367E04C) //SetT_Of_ValhallaPOINT  - this gets set via self, or via received buff from other player

SkillManager::SkillManager()
{
	ZeroMemory( baPetAggressive, sizeof( BOOL ) * 8 );

	//Set pet default mode to aggressive
	for ( int i = 0; i < _countof ( baPetAggressive ); i++ )
	{
		baPetAggressive[i] = TRUE;
	}
}


SkillManager::~SkillManager()
{
}


BOOL SkillManager::IsActiveSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_Raving:
		case SKILLID_DoubleImpact:
		case SKILLID_TripleImpact:
		case SKILLID_BrutalSwing:
		case SKILLID_RageofZecram:
		case SKILLID_AvengingCrash:
		case SKILLID_BoneCrash:
		case SKILLID_Destroyer:
		case SKILLID_CycloneStrike:
		case SKILLID_SeismicImpact:
		case SKILLID_Whirlwind:

			//Mechanician
		case SKILLID_MechanicBomb:
		case SKILLID_GreatSmash:
		case SKILLID_Spark:
		case SKILLID_ElectricalOverload:
		case SKILLID_GrandSmash:
		case SKILLID_Impulsion:
		case SKILLID_Rupture:
		case SKILLID_MagneticDischarge:

			//Archer
		case SKILLID_WindArrow:
		case SKILLID_PerfectAim:
		case SKILLID_ArrowofRage:
		case SKILLID_Avalanche:
		case SKILLID_ElementalShot:
		case SKILLID_BombShot:
		case SKILLID_Perforation:
		case SKILLID_PhoenixShot:
		case SKILLID_FierceWind:
		case SKILLID_Bombardment:

			//Pikeman
		case SKILLID_PikeWind:
		case SKILLID_PreciseHit:
		case SKILLID_JumpingCrash:
		case SKILLID_Tornado:
		case SKILLID_Expansion:
		case SKILLID_ChainLance:
		case SKILLID_ChargingStrike:
		case SKILLID_ShadowMaster:

			//Atalanta
		case SKILLID_BiggerSpear:
		case SKILLID_WindJavelin:
		case SKILLID_FireJavelin:
		case SKILLID_SplitJavelin:
		case SKILLID_LightningJavelin:
		case SKILLID_StormJavelin:
		case SKILLID_ExtremeRage:
		case SKILLID_Vengeance:
		case SKILLID_ComboJavelin:

			//Knight
		case SKILLID_SwordBlast:
		case SKILLID_DoubleCrash:
		case SKILLID_Brandish:
		case SKILLID_Piercing:
		case SKILLID_GrandCross:
		case SKILLID_SwordofJustice:
		case SKILLID_DivinePiercing:
		case SKILLID_DivineCross:
		case SKILLID_DivineInquisiton:

			//Magician
		case SKILLID_Agony:
		case SKILLID_FireBolt:
		case SKILLID_FireBall:
		case SKILLID_Watornado:
		case SKILLID_DeathRay:
		case SKILLID_Diastrophism:
		case SKILLID_FlameWave:
		case SKILLID_Meteorite:
		case SKILLID_StoneSkin:
		case SKILLID_RedRay:
		case SKILLID_Cataclysm:

			//Priestess
		case SKILLID_Healing:
		case SKILLID_HolyBolt:
		case SKILLID_MultiSpark:
		case SKILLID_DivineLightning:
		case SKILLID_GrandHealing:
		case SKILLID_VigorBall:
		case SKILLID_Ressurection:
		case SKILLID_GlacialSpike:
		case SKILLID_ChainLightning:
		case SKILLID_LightningSurge:
		case SKILLID_HeavenlyLight:

			//Assassin
		case SKILLID_Stinger:
		case SKILLID_RunningHit:
		case SKILLID_VenomThorn:
		case SKILLID_SoreBlade:
		case SKILLID_GustSlash:
		case SKILLID_Cripple:
		case SKILLID_NinjaShadow:
		case SKILLID_ShadowBlink:

			//Shaman
		case SKILLID_DarkBolt:
		case SKILLID_DarkWave:
		case SKILLID_SpiritualFlare:
		case SKILLID_PhantomCall:
		case SKILLID_Scratch:
		case SKILLID_Judgement:
		case SKILLID_MourningPray:
		case SKILLID_PressDeity:
		case SKILLID_PhantomNail:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::IsPassiveSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_MeleeMastery:
		case SKILLID_Recuperation:
		case SKILLID_Resilience:

			//Mechanician
		case SKILLID_MechanicWeaponMastery:

			//Archer
		case SKILLID_ShootingMastery:
		case SKILLID_DionsEye:
		case SKILLID_EvasionMastery:

			//Pikeman
		case SKILLID_PolearmMastery:
		case SKILLID_WeaponBlockMastery:
		case SKILLID_CriticalMastery:

			//Atalanta
		case SKILLID_ThrowingMastery:
		case SKILLID_GoldenApple:

			//Knight
		case SKILLID_PhysicalTraining:
		case SKILLID_SwordMastery:

			//Magician
		case SKILLID_MentalMastery:

			//Priestess
		case SKILLID_Meditation:

			//Assassin
		case SKILLID_DualWieldMastery:
		case SKILLID_DodgeMastery:
		case SKILLID_AssassinMastery:

			//Shaman
		case SKILLID_InnerPeace:
		case SKILLID_OccultLife:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::IsDebuffSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_WarCry:

			//Mechanician
		case SKILLID_ParasitShot:

			//Archer
		case SKILLID_EntalingRoots:

			//Pikeman
		case SKILLID_GroundPike:
		case SKILLID_VenomSpear:
		case SKILLID_AssassinsEye:
		case SKILLID_RingofSpears:

			//Atalanta
		case SKILLID_ShieldStrike:
		case SKILLID_SoulSucker:
		case SKILLID_PlagueJavelin:

		case SKILLID_Distortion:

			//Priestess
		case SKILLID_HolyMind:
		case SKILLID_Extinction:

			//Assassin
		case SKILLID_Wisp:
		case SKILLID_SoulShock:
		case SKILLID_Polluted:

			//Shaman
		case SKILLID_Inertia:
		case SKILLID_SpiritualManacle:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::IsBuffSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_DemonsBane:
		case SKILLID_ConcentrationSA:
		case SKILLID_Berserker:
		case SKILLID_RuthlessofZecram:
		case SKILLID_SurvivalInstinct:

			//Mechanician
		case SKILLID_ExtremeShield:
		case SKILLID_PhysicalAbsorb:
		case SKILLID_ElectricalOverload:
		case SKILLID_Maximize:
		case SKILLID_Automation:
		case SKILLID_MetalArmor:
		case SKILLID_SparkShield:
		case SKILLID_Compulsion:
		case SKILLID_MagneticSphere:
		case SKILLID_HardeningShield:

			//Archer
		case SKILLID_ScoutHawk:
		case SKILLID_Falcon:
		case SKILLID_GoldenFalcon:
		case SKILLID_ForceofNature:
		case SKILLID_LethalSight:

			//Pikeman
		case SKILLID_Vanish:
		case SKILLID_Vague:

			//Atalanta
		case SKILLID_Farina:
		case SKILLID_Windy:
		case SKILLID_TriumphofValhalla:
		case SKILLID_HallofValhalla:
		case SKILLID_FrostJavelin:

			//Knight
		case SKILLID_HolyBody:
		case SKILLID_HolyValor:
		case SKILLID_DrasticSpirit:
		case SKILLID_DivineShield:
		case SKILLID_HolyIncantation:
		case SKILLID_GodlyShield:
		case SKILLID_GodsBlessing:
		case SKILLID_GloriousShield:
		case SKILLID_HolyConviction:

			//Magician
		case SKILLID_Zenith:
		case SKILLID_EnchantWeapon:
		case SKILLID_EnergyShield:
		case SKILLID_SpiritElemental:
		case SKILLID_DancingSword:
		case SKILLID_WizardTrance:

			//Priestess
		case SKILLID_HolyReflection:
		case SKILLID_VirtualLife:
		case SKILLID_VirtualRegen:
		case SKILLID_SummonMuspell:
		case SKILLID_Consecration:

			//Assassin
		case SKILLID_Alas:
		case SKILLID_Inpes:
		case SKILLID_Blind:

			//Shaman
		case SKILLID_ChasingHunt:
		case SKILLID_AdventMigal:
		case SKILLID_Rainmaker:
		case SKILLID_AdventMidranda:
		case SKILLID_Creed:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::IsSummonSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Mechanician
		case SKILLID_MetalGolem:

			//Archer
		case SKILLID_RecallWolverine:

			//Atalanta
		case SKILLID_SummonTiger:

			//Priestess
		case SKILLID_IceElemental:

			//Magician
		case SKILLID_FireElemental:

			//Shaman
		case SKILLID_RecallHestian:
			return TRUE;
			break;
	}
	return FALSE;
}


BOOL SkillManager::ResetSkills( int iNum )
{
	return CALLT_WITH_ARG1( pfnResetSkills, 0x0367E148, iNum );
}

BOOL SkillManager::IsActiveBuff( UINT uBuffID )
{
	return (BOOL)IsActiveBuffFunc( uBuffID );
}

NAKED BOOL SkillManager::ResetSkillTimer( int iSkillID )
{
	JMP( pfnClearSkillBuff );
}

/// <summary>
/// 2012 code: int sinUseManaStamina(sSKILL *lpSkill)
/// </summary>
BOOL SkillManager::UseManaStamina( Skill * psSkill )
{
	return CALL_WITH_ARG1( 0x00520DE0, (DWORD)psSkill );
}

void SkillManager::CancelForceOrb( int iForceOrbID )
{
	if( MESSAGEBOX->GetType() == FALSE )
		return;
	else
	{
		if ( sTempSkillTimerData[iForceOrbID].iID == SKILLID_ForceOrb )
		{
			TIMERSKILLHANDLER->KillTimer( SKILLID_ForceOrb );
			PacketSimple sPacket;
			sPacket.iLength = sizeof( PacketSimple );
			sPacket.iHeader = PKTHDR_CancelForceOrb;
			SENDPACKETG( &sPacket );

			UNITDATA->eForceOrbItemID = ITEMID_None;
			UNITDATA->dwForceOrbTimeOut = 0;

			/*DWORD dwCode;
			dwCode = UNITDATA->eForceOrbItemID;
			__asm
			{
				PUSH dwCode;
				PUSH 0;
				PUSH DWORD PTR DS : [0x00CF481C];
				MOV EAX, 0x005C16B0;
				CALL EAX;
				ADD ESP, 0x0C;
			}
			UNITDATA->eForceOrbItemID = ITEMID_None;
			UNITDATA->dwForceOrbTimeOut = 0;*/
		}
	}
}

BOOL SkillManager::IsDebuffSkill( UnitData * pcUnit, DWORD dwSkillCode )
{
	if ( pcUnit )
	{
		for ( int i = 0; i < _countof( saDebufSkills ); i++ )
		{
			if ( pcUnit->sCharacterData.iClass == saDebufSkills[i].iClass && saDebufSkills[i].SkillCode == dwSkillCode )
				return TRUE;
		}
	}
	return FALSE;
}

int SkillManager::GetResistanceTime( int iTime, int iResistanceID )
{
	return CALL_WITH_ARG2( 0x0041E610, iTime, iResistanceID );
}

void SkillManager::StunEffect( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG3( 0x005377B0, (DWORD)pcUnitData, iTime, 0 );
}
//A function to find the two-dimensional angle between two points (this is also calculated by Lee Seong-su)
int SkillManager::GetRadian2D( int x1, int y1, int x2, int y2 )
{
	int flag = 0;

	if (x1 > x2)
	{
		flag |= 0x0001;
		x1 ^= x2;x2 ^= x1;x1 ^= x2;
	}

	if (y1 > y2)
	{
		flag |= 0x0002;
		y1 ^= y2;y2 ^= y1;y1 ^= y2;
	}

	x2 -= (x1-1); y2 -= (y1-1);

	if (y2 > x2)
	{
		flag |= 0x0004;
		y2 ^= x2;x2 ^= y2;y2 ^= x2;
	}

	y2 <<= 8;
	y2 /= x2;
	x1 = (y2 * PTANGLE_45);
	x1 >>= 8;

	switch (flag)
	{
		case 2:	x1 +=	PTANGLE_90;			break;
		case 3:	x1 =	PTANGLE_270 - x1;	break;
		case 0:	x1 =	PTANGLE_90 - x1;	break;
		case 1:	x1 +=	PTANGLE_270;		break;
		case 6:	x1 =	PTANGLE_180 - x1;	break;
		case 7:	x1 +=	PTANGLE_180;		break;
		case 5:	x1 =	PTANGLE_360 - x1;	break;
	}
	return x1;
}

void SkillManager::GetInfoBoxSize( struct sSKILLBOX * psSkillBox, int iColumn, int iIcon )
{
	typedef void( __stdcall *tfnGetInfoBoxSize )( struct sSKILLBOX * psSkillBox, int iColumn, int iUnk );
	tfnGetInfoBoxSize fnGetInfoBoxSize = (tfnGetInfoBoxSize)0x004EB9C0;
	fnGetInfoBoxSize( psSkillBox, iColumn, iIcon );
}


void SkillManager::AddSkillInfo( int iColumn, const char * pszText, ... )
{
	char szBuffer[256] = { 0 };
	va_list ap;
	va_start( ap, pszText );
	_vsnprintf_s( szBuffer, 256, 255, pszText, ap );
	va_end( ap );

	if ( iColumn == 1 )
	{
		// Left
		lstrcatA( (char*)(0x036762A0), szBuffer );
	}
	else if ( iColumn == 2 )
	{
		// Right
		lstrcatA( (char*)(0x03677628), szBuffer );
	}
}

float SkillManager::GetChainedSkillScale( int iArrayPointer )
{
	auto it = mapSkillChainScale.find(iArrayPointer);

	if ( it != mapSkillChainScale.end() )
		return it->second;
	else
		return 1.0f;
}

/// <summary>
/// Get Skill Level (1-based index)
/// </summary>
int SkillManager::GetLevelSkill( UINT uSkillCode )
{
	for ( int i = 0; i < 20; i++ )
	{
		if ( SKILLSCHARACTER[ i ].iID == uSkillCode )
			return SKILLSCHARACTER[ i ].iLevel;
	}
	return 0;
}

/// <summary>
/// Leaked code: StartSkill
/// Address: 0x574250
/// </summary>
void SkillManager::StartSkillEffect( int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ, int iSkillType, int iLevel )
{
	fnStartSkillEffect( iX, iY, iZ, iAngleX, iAngleY, iAngleZ, iSkillType, iLevel );
}

/// <summary>
/// Leaked code: StartSkill
/// Address: 0x574250
/// </summary>
void SkillManager::StartSkillEffect( Point3D & point3D, int iSkillType, int iLevel )
{
	fnStartSkillEffect( point3D.iX, point3D.iY, point3D.iZ, 0, 0, 0, iSkillType, iLevel );
}

/// <summary>
/// Leaked code: int SetDynLight( int x,int y,int z, int r, int g, int b, int a, int power , int DecPower )
/// Dynamic light setup
/// ASM: 0x0040C080
/// </summary>
void SkillManager::SetDynLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iPower, int iDecPower )
{
	SetDynamicLight( iX, iY, iZ, iR, iG, iB, iA, iPower, iDecPower );
}


/// <summary>
/// Leaked code: int SetDynLight( int x,int y,int z, int r, int g, int b, int a, int power , int DecPower )
/// Dynamic light setup
/// ASM: 0x0040C080
/// </summary>
void SkillManager::SetDynLight( Point3D & point3D, int iR, int iG, int iB, int iA, int iPower, int iDecPower )
{
	SetDynamicLight( point3D.iX, point3D.iY, point3D.iZ, iR, iG, iB, iA, iPower, iDecPower );
}

/// <summary>
/// Leaked code: AddTimeEffect
/// Address: 0x45D020
/// Add timed effect (delaying a skill attack for example)
/// </summary>
void SkillManager::AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z  )
{
	fnAddTimeEffect( EffectCode, dwTime, x, y, z );
}

/// <summary>
/// Leaked: int StartEffect(int x, int y, int z, int r, int g, int b, int a, int type)
/// which calls///
/// int HoEffectMgr::Start(int x, int y, int z, int effectType, int level)
/// </summary>
/// <param name="iX"></param>
/// <param name="iY"></param>
/// <param name="iZ"></param>
/// <param name="iID"></param>
/// <param name="iLevel"></param>
/// <returns></returns>
NAKED void SkillManager::StartSkillEffectOld( int iX, int iY, int iZ, int iID, int iLevel )
{
	JMP( 0x0057CE60 );
}

/// <summary>
/// ASM address: 0x00544870
/// </summary>
void SkillManager::SkillPlaySound( DWORD dwCode, int iX, int iY, int iZ, int iFlag )
{
	fnSkillPlaySound( dwCode, iX, iY, iZ, iFlag );
}

/// <summary>
/// Play skill sound at position
/// </summary>
void SkillManager::SkillPlaySound( DWORD dwCode, Point3D & point, int iFlag )
{
	fnSkillPlaySound( dwCode, point.iX, point.iY, point.iZ, iFlag );
}

/// <summary>
/// ASM address: 0x00544870
/// </summary>
void SkillManager::SkillPlaySound( DWORD dwCode, UnitData * pcUnitData, int iFlag )
{
	if ( pcUnitData == NULL )
		return;

	fnSkillPlaySound( dwCode, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, iFlag );
}

/// <summary>
/// ASM address: 0x00544170
/// 2012 func: int	WeaponPlaySound( smCHAR *lpChar )
/// Play weapon sound
/// </summary>
void SkillManager::PlayWeaponSound( UnitData * pcUnitData )
{
	fnPlayWeaponSound ( pcUnitData );
}

/// <summary>
/// ASM address: 0x0040F3F0
/// Leaked: int RetryPlayAttack( smCHAR *lpNearestChar ) @ SkillSub.cpp
///
/// Attack retry
/// </summary>
/// <param name="pcMouseTarget"></param>
int SkillManager::RetryPlayAttack( UnitData * pcUnitData )
{
	return fnSkillRetryPlayAttack( pcUnitData );
}


/// <summary>
/// Continuous skill attack attempt
/// Leaked: int PlaySkillLoop( int SkillCode , smCHAR *lpChar )
/// </summary>
/// <returns>FALSE to process the function in ASM, otherwise FALSE</returns>
int SkillManager::PlaySkillLoop( ActiveSkill sActiveSkill, UnitData * pcPlayer )
{
	int	cnt, tmp, value;
	UnitData *chrAttackTarget;
	RECT	rect;

	if ( pcPlayer->pcTarget == NULL || sActiveSkill.cLevel <= 0 || sActiveSkill.cLevel > 10 )
		return TRUE; //bypass ASM

	if ( pcPlayer->pcTarget->GetCurrentHealth() <= 0 )
	{
		pcPlayer->iMotionLoopCnt = 0; //stop continuous execution
		return TRUE; //bypass ASM
	}

	//this function should only be used for this player
	if ( pcPlayer != UNITDATA )
		return TRUE;

	CHATBOX->AddDebugMessage( "PlaySkillLoop" );

	switch ( pcPlayer->sCharacterData.iClass )
	{
		case ECharacterClass::CHARACTERCLASS_Pikeman:
		{
			if ( sActiveSkill.cSkillTinyID == PikemanSkill::PreciseHit )
			{
				tmp = pcPlayer->sCharacterData.iAttackRating;

				value = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_PreciseHit_AddAttackRating_New, sActiveSkill.cLevel - 1 );
				pcPlayer->sCharacterData.iAttackRating += ( pcPlayer->sCharacterData.iAttackRating * value ) / 100;

				SKILLMANAGERHANDLER->SendTransDamage( pcPlayer->pcTarget, pcPlayer->sActiveSkill );

				pcPlayer->sCharacterData.iAttackRating = tmp; //restore

				return TRUE; //bypass ASM
			}

			break;
		}

		case ECharacterClass::CHARACTERCLASS_Fighter:
		{
			if ( sActiveSkill.cSkillTinyID == FighterSkill::Raving )
			{
				cnt = SKILLMANAGERHANDLER->SendTransDamage( pcPlayer->pcTarget, pcPlayer->sActiveSkill );

				if ( !cnt ) pcPlayer->iTargetCritical = -1;
				else
				{
					//Life decrease
					pcPlayer->iTargetCritical = 0;

					value = SKILLMANAGERHANDLER->GetSkillIntValue( Fighter_Raving_HPCost_New, ESkillID::SKILLID_Raving );
					CHARACTERGAME->RemoveHPValue( value );
				}

				pcPlayer->iFireOverlay = 150;
				pcPlayer->sAngle.iY = GetRadian2D( pcPlayer->sPosition.iX, pcPlayer->sPosition.iZ, pcPlayer->pcTarget->sPosition.iX, pcPlayer->pcTarget->sPosition.iZ );

				return TRUE; //bypass ASM
			}

			break;
		}

		case ECharacterClass::CHARACTERCLASS_Atalanta:
		{
			if (sActiveSkill.cSkillTinyID == AtalantaSkill::StormJavelin)
			{
				const int lightningJavelinLevel = SKILLMANAGERHANDLER->GetLevelSkill(ESkillID::SKILLID_LightningJavelin);
				const int lightningJavelinRangeBoost = SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Atalanta_LightningJavelin_Radius_New, lightningJavelinLevel - 1);

				rect.left = -60;
				rect.right = 60;
				rect.top = -50;
				rect.bottom = 300 + lightningJavelinRangeBoost;

				SelectRangeBox(pcPlayer->pcTarget, rect, FALSE);
				SendRangeDamage(pcPlayer->pcTarget, sActiveSkill.iValue);

				return TRUE;
			}
			break;
		}
	}


	return FALSE;
}


BOOL SkillManager::CanUseSkill( Skill * psSkill )
{
	if ( psSkill->iLevel <= 0 || psSkill->iLevel > 10 )
		return FALSE;

	if ( UNITDATA == NULL )
		return FALSE;

	if ( UNITDATA->iLoadedMapIndex < 0 || UNITDATA->iLoadedMapIndex >= NUM_MAPS )
		return FALSE;

	const int iMapType = FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType;

	if ( iMapType == MAPTYPE_Town )
		return FALSE;

	if( SKILLDATAHANDLER->IsSkillLocked( psSkill->iID ) )
	{
		TITLEBOX( "Skill not avaliable!" );
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// This function gets called via right click or left click
/// For right click, this only gets played ONLY if OpenPlaySkill returned FALSE !
/// Generally auto-castable skill should be here and not in OpenPlaySkill
/// 2012 func: int PlaySkillAttack( sSKILL *lpSkill , smCHAR *lpChar )
/// </summary>
BOOL SkillManager::PlaySkillAttack( struct Skill * psSkill, UnitData * pcMouseTarget )
{
	if ( CanUseSkill( psSkill ) == FALSE )
	{
		return TRUE;
	}

	if ( pcMouseTarget->IsPlayer() && MAP_ID != MAPID_BlessCastle && psSkill->iID != SKILLID_Healing )
	{
		return TRUE;
	}

	if ( (BOOL)CALL_WITH_ARG1( 0x00520A00, (DWORD)psSkill ) == FALSE )
	{
		//try do basic attack. If that succeeds, return 2 which will return FALSE! (so it actually executes the basic attack dmg)
		if ( ( psSkill == USESKILLS.pLeftSkill && RetryPlayAttack( pcMouseTarget ) ) )
			return 2;

		return TRUE;
	}

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "PlaySkillAttack id = %d, %d", psSkill->iID, psSkill->sSkillInfo.eSkillID ) );
#endif

	//Use Skill (pUseSkill)
	WRITEDWORD( 0x03673B78, (DWORD)psSkill );

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:		return FIGHTERSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Pikeman:		return PIKEMANSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Mechanician:	return MECHANICIANSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Archer:			return ARCHERSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Priestess:		return PRIESTESSSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Knight:			return KNIGHTSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Atalanta:		return ATALANTASKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Magician:		return MAGICIANSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Assassin:		return ASSASSINSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
		case CHARACTERCLASS_Shaman:			return SHAMANSKILLHANDLER->OnPlaySkillAttack ( psSkill, pcMouseTarget );
	}

	return TRUE;
}

/// <summary>
/// The very first step of the skill animation sequence
/// This occurs after mouse click regardless of conditions,
/// but does not neccessary mean that
/// the skill will be executed in the end.
/// To see this for yourself, uncomment out the '#define SKILLS_BEHAVIOUR_DEBUG' CHATBOX to debug it
/// </summary>
int SkillManager::OpenPlaySkill( struct Skill * psSkill )
{
	if ( SELECTED_MOUSE_BUTTON == 1 && UNITDATA_HOVER && UNITDATA_HOVER->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
	{
		SendNpcUnitClick( UNITDATA_HOVER, 0 );
		return TRUE; //don't process in asm
	}

	if ( CanUseSkill( psSkill ) == FALSE )
		return 2;

	if ( (BOOL)CALL_WITH_ARG1( 0x00520A00, (DWORD)psSkill ) == FALSE )
    {
        if (psSkill->iID == SKILLID_DefaultAttack)
            return 2;

		return TRUE;
	}

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "OpenPlaySkill id = %d, %d", psSkill->iID, psSkill->sSkillInfo.eSkillID ) );
#endif

	//Use Skill
	WRITEDWORD( 0x03673B78, (DWORD)psSkill );

	if ( psSkill->iLevel <= 0 || psSkill->iLevel > 10 )
		return TRUE; //don't process in asm

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:		return FIGHTERSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Pikeman:		return PIKEMANSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Mechanician:	return MECHANICIANSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Archer:			return ARCHERSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Priestess:		return PRIESTESSSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Knight:			return KNIGHTSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Atalanta:		return ATALANTASKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Magician:		return MAGICIANSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Assassin:		return ASSASSINSKILLHANDLER->OnOpenPlaySkill ( psSkill );
		case CHARACTERCLASS_Shaman:			return SHAMANSKILLHANDLER->OnOpenPlaySkill ( psSkill );
	}

	return FALSE;
}

BOOL SkillManager::HandleDebuffSkill( UnitData * pcUnitData, int iID, int iValue )
{
	switch ( pcUnitData->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Pikeman:
			switch ( iID )
			{
				case SKILLPLAYID_T51:
				{
					if ( UNITDATA->iStunTimeLeft <= 0 )
						UNITDATA->iStunTimeLeft = iValue;
				} break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;

		case CHARACTERCLASS_Magician:
			switch ( iID )
			{
				case MagicianSkill::Distortion:
				{
					if ( UNITDATA->iSlowTimeLeft <= 0 )
					{
						int iTime = iValue;
						UNITDATA->iSlowTimeLeft = iTime;
						UNITDATA->iPlaySlowSpeed = SLOW_SPEED_BASE - 50;
						UNITDATA->bDistortion = (BOOL)UNITDATA->iSlowTimeLeft;
						UNITDATA->iIceOverlay = 0;
					}
				} break;
			}
			break;

		case CHARACTERCLASS_Shaman:
			switch ( iID )
			{
				case ShamanSkill::Inertia:
				{
					if ( UNITDATA->iSlowTimeLeft <= 0 )
					{
						int iTime = iValue;
						UNITDATA->iSlowTimeLeft = iTime;
						UNITDATA->iPlaySlowSpeed = SLOW_SPEED_BASE - 50;
						UNITDATA->iCurseLazy = (BOOL)UNITDATA->iSlowTimeLeft;
						UNITDATA->iIceOverlay = 0;
					}
				} break;
				case SKILLPLAYID_T22:
				{
					if ( UNITDATA->iStunTimeLeft <= 0 )
						UNITDATA->iStunTimeLeft = iValue;
				} break;
			}
			break;

		default:
			break;
	}

	return TRUE;
}

int SkillManager::GetSkillRange( Skill * psSkill, int iAttackRange, int iAttackMode )
{
	if ( psSkill->iLevel <= 0 )
		return 0;

	int iRange = 0;

	switch ( psSkill->iID )
	{
		case SKILLID_Healing:			iRange = 300;																										break;
		case SKILLID_SwordBlast:		iRange = GetSkillIntValue( (int)ESkillArrayPointer::Knight_SwordBlast_AttackRange, psSkill->iLevel - 1 );			break;
		case SKILLID_Expansion:			iRange = GetSkillIntValue( (int)ESkillArrayPointer::Pikeman_Expansion_Range_New, psSkill->iLevel - 1 );				break;
		case SKILLID_MechanicBomb:		iRange = GetSkillIntValue( (int)ESkillArrayPointer::Mech_MechanicBomb_AttackRange_New, psSkill->iLevel - 1 );		break;
		case SKILLID_ShadowBlink:		iRange = iaASNT51Range[psSkill->iLevel - 1];																		break;
		case SKILLID_EnchantWeapon:		iRange = 140;																										break;
		case SKILLID_DivineLightning:	iRange = GetSkillIntValue( (int)ESkillArrayPointer::Priestess_DivineLightning_ATKRadius_New, psSkill->iLevel - 1 );	break;
		case SKILLID_Diastrophism:		iRange = GetSkillIntValue( (int)ESkillArrayPointer::Magician_Diastrophism_Area, psSkill->iLevel - 1);				break;
		case SKILLID_Tornado:			iRange = GetSkillIntValue( (int)ESkillArrayPointer::Pikeman_Tornado_Range, psSkill->iLevel - 1 );					break;
		case SKILLID_VenomSpear:		iRange = GetSkillIntValue( (int)ESkillArrayPointer::Pikeman_VenomSpear_Range, psSkill->iLevel - 1 );				break;
		case SKILLID_GroundPike:		iRange = GetSkillIntValue( (int)ESkillArrayPointer::Pikeman_GroundPike_Range, psSkill->iLevel - 1 );				break;
		case SKILLID_AssassinsEye:		iRange = 200;																										break;
		case SKILLID_FlameWave:			iRange = 150;																										break;
		case SKILLID_ChargingStrike:	iRange = 80;																										break;

		default:
			break;
	}

	//secondary range (addition)

	switch ( psSkill->iID )
	{
		case SKILLID_ChargingStrike:
		{
			iRange += GetSkillIntValue( ESkillArrayPointer::Pikeman_Expansion_Range_New, ESkillID::SKILLID_Expansion );
			break;
		}
		case SKILLID_Tornado:
		case SKILLID_VenomSpear:
		case SKILLID_GroundPike:
		{
			iRange += GetSkillIntValue( ESkillArrayPointer::Pikeman_PolearmMastery_AddRange_New, ESkillID::SKILLID_PolearmMastery );
			break;
		}
	}


	return iRange << 8;
}

/// <summary>
/// Triggered on skill event. Like on pet spawned
/// this can be for self or others so do a pcMouseTarget == UNITDATA to check
/// </summary>
BOOL SkillManager::EventSkill( UnitData * pcUnitData )
{
	short skillLevel = ( pcUnitData->sActiveSkill.cLevel & 0xF );
	short sParam = pcUnitData->sActiveSkill.sParam;

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "EventSkill id = %d, %d, %d, %d", pcUnitData->sActiveSkill.cSkillTinyID, skillLevel, sParam, pcUnitData->iMotionEvent ) );
#endif

	if ( skillLevel <= 0 || skillLevel > 10 )
		return TRUE; //return true to not process in assembly

	switch ( pcUnitData->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:		return FIGHTERSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Mechanician:	return MECHANICIANSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Archer:			return ARCHERSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Pikeman:		return PIKEMANSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Assassin:		return ASSASSINSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Knight:			return KNIGHTSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Atalanta:		return ATALANTASKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Priestess:		return PRIESTESSSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Magician:		return MAGICIANSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
		case CHARACTERCLASS_Shaman:			return SHAMANSKILLHANDLER->OnEventSkill ( pcUnitData, &pcUnitData->sActiveSkill );
	}
	return FALSE;
}

/// <summary>
/// Same as EventSkill but AFTER it is processed in ASM
/// </summary>
void SkillManager::EventSkillPostASM ( UnitData * pcUnitData )
{
#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "EventSkillPostASM id = %d", pcUnitData->sActiveSkill.cSkillTinyID ) );
#endif

	if ( pcUnitData->sActiveSkill.cSkillTinyID > 0 && pcUnitData->sActiveSkill.cLevel > 0 &&
		 pcUnitData && pcUnitData == UNITDATA )
	{
		ESkillID eSkillID = GetSkillIDFromTinyID ( pcUnitData->sCharacterData.iClass, pcUnitData->sActiveSkill.cSkillTinyID );

		switch ( eSkillID )
		{
		case SKILLID_RecallHestian:
		case SKILLID_MetalGolem:
		case SKILLID_RecallWolverine:
		case SKILLID_FireElemental:
		case SKILLID_IceElemental:
		case SKILLID_SummonTiger:
			SetDefaultPetMode ( (int)eSkillID, SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive );
			break;
		}
	}
}

//for GreedyStart
BOOL SkillManager::BeginSkillMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "DeyStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Yagditha:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("YagdithaStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("YagdithaStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					PARTICLE->StartEffectParticleSkill("YagdithaStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Greedy:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'K') // Little Blue Ground Effect Before Skill
				{
					PARTICLE->StartEffectParticleSkill("GreedyStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Acero:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("AceroStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("AceroStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Nihil:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("NihilStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("NihilStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Chalybs:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("ChalybsStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("ChalybsStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					PARTICLE->StartEffectParticleSkill("ChalybsStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAGIANT:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAARCHER:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_IGNIS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenStart", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;


		// Backend Information of how particles are loaded in full src.
		//case MONSTEREFFECTID_Greedy:
		//	if (pcMouseTarget->pcMouseTarget && pcMouseTarget->psModelAnimation && (pcMouseTarget->psModelAnimation->uaValue[0] == 'J'))
		//	{
		//
		//		int iID = PARTICLE->Start(iParticleID_GreedyStart,
		//			pcMouseTarget->sPosition.iX,
		//			pcMouseTarget->sPosition.iY - (100 << 8) + pcMouseTarget->iModelHeight,
		//			pcMouseTarget->sPosition.iZ);
		//
		//		//CHATBOX->AddMessage ( FormatString ( "1st Start ID = %d", iID ) );
		//		PARTICLE->AddScript(iID, iParticleID_GreedyStart, pcMouseTarget, 70.0f, TRUE);
		//		bRet = TRUE;
		//	}
		//	break;

		default:
			break;
	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->PlayAttackOverride();
	}


	return bRet;
}

BOOL SkillManager::EventSkillMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "DeySkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Yagditha:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("YagdithaSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("YagdithaSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("YagdithaSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_IGNIS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Greedy:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'K') // Circle Ground Effect After GreedyStart (K)
				{
					PARTICLE->StartEffectParticleSkill("GreedySkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Nihil:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'K') // For some reason letter K is needed to enable this script
				{
					PARTICLE->StartEffectParticleSkill("NihilSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Acero:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'J') // For some reason letter J is needed to enable this script
				{
					PARTICLE->StartEffectParticleSkill("AceroSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Chalybs:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("ChalybsSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("ChalybsSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("ChalybsSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAGIANT:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGiantSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_IGNICULUS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_INSEC:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("InsecSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("InsecSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("InsecSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAARCHER:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAGOLEM:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_LIZARDELDER:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_DRAXOS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosSkill1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosSkill1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosSkill2", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

			case MONSTEREFFECTID_FLAMEMAIDEN:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_LAVAARMOR:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_FLAMO:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoSkill", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		//110 map test
		//case MONSTEREFFECTID_Greedy:
		//	if (pcMouseTarget->pcMouseTarget && pcMouseTarget->psModelAnimation && (pcMouseTarget->psModelAnimation->uaValue[0] == 'J'))
		//	{
		//
		//		int iID = PARTICLE->Start(iParticleID_GreedySkill,
		//			pcMouseTarget->sPosition.iX,
		//			pcMouseTarget->sPosition.iY - (100 << 8) + pcMouseTarget->iModelHeight,
		//			pcMouseTarget->sPosition.iZ);
		//
		//		//CHATBOX->AddMessage ( FormatString ( "1st Start ID = %d", iID ) );
		//		PARTICLE->AddScript(iID, iParticleID_GreedySkill, pcMouseTarget, 70.0f, TRUE);
		//		bRet = TRUE;
		//	}
		//	break;
		//End 110 map test


		case MONSTEREFFECTID_IceElemental:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				CGameSkill::FrozenMistSkillHit( pcUnitData, pcUnitData->pcTarget );
				bRet = TRUE;
			}
		default:
			break;
	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->PlayAttackOverride();
	}

	return bRet;
}

BOOL SkillManager::EventHitMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "DeyHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Yagditha:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'H' )
				{
					PARTICLE->StartEffectParticleSkill( "YagdithaHit1", pcUnitData->pcTarget );
					bRet = TRUE;
				}
				else if ( pcUnitData->psModelAnimation->uaValue[0] == 'K' )
				{
					PARTICLE->StartEffectParticleSkill( "YagdithaHit2", pcUnitData->pcTarget );
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_IGNIS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H')
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K')
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Greedy:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'H') // Swing 1
				{
					PARTICLE->StartEffectParticleSkill("GreedyHit1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'K') // Swing 2
				{
					PARTICLE->StartEffectParticleSkill("GreedyHit2", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->psModelAnimation->uaValue[0] == 'J') // Shockwave (bodyslam) - Critical hit
				{
					PARTICLE->StartEffectParticleSkill("GreedySkill2", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Nihil:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
			{
				PARTICLE->StartEffectParticleSkill("NihilHit", pcUnitData->pcTarget);
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Acero:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
			{
				PARTICLE->StartEffectParticleSkill("AceroHit", pcUnitData->pcTarget);
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Chalybs:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
			{
				PARTICLE->StartEffectParticleSkill("ChalybsHit", pcUnitData->pcTarget);
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LAVAGIANT:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
			{
				PARTICLE->StartEffectParticleSkill("LavaGiantHit", pcUnitData->pcTarget);
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_IGNICULUS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("IgniculusHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LIZARDELDER:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LizardElderHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_DRAXOS:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("DraxosHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_INSEC:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("InsecHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("InsecHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("InsecHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAARCHER:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArcherHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_LAVAGOLEM:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaGolemHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_FLAMEMAIDEN:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("FlamemaidenHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}

		case MONSTEREFFECTID_LAVAARMOR:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("LavaArmorHit", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}


		case MONSTEREFFECTID_FLAMO:
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'K'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoHit1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoHit1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
				else if (pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J'))
				{
					PARTICLE->StartEffectParticleSkill("FlamoHit1", pcUnitData->pcTarget);
					bRet = TRUE;
				}
			}



		case MONSTEREFFECTID_IceElemental:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				CGameSkill::FrozenMistHit( pcUnitData, pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		default:
			break;
	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->PlayAttackOverride();
	}
	else
	{
		//DEBUG("%d = SkillManager -> pcMouseTarget->UpdateAttack() bRet == FALSE", TICKCOUNT);
	}

	return bRet;
}

BOOL SkillManager::EndSkill( class UnitData * pcUnitData )
{

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "EndSkill id = %d", UNITDATA->sActiveSkill.cSkillTinyID ) );
#endif

	switch ( pcUnitData->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Magician: return MAGICIANSKILLHANDLER->OnEndSkill ( pcUnitData );
		case CHARACTERCLASS_Fighter : return FIGHTERSKILLHANDLER->OnEndSkill( pcUnitData );

	/* Priestess Skills */
	case CHARACTERCLASS_Archer:
		switch ( pcUnitData->sActiveSkill.iValue & 0xFF )
		{
		case SKILLPLAYID_T52:
			return FIERCEWINDHANDLER->OnAnimationEnd( pcUnitData );
			break;
		case SKILLPLAYID_T53:
			break;
		case SKILLPLAYID_T54:
			break;
		}
		break;
	}

	return FALSE;
}

/// <summary>
/// Leaked code: int BeginSkill(int SkilCode , int Level , smCHAR *lpTarChar , int x, int y, int z );
/// Skill start
/// Calling UnitData->BeginSkill() will also call this
/// </summary>
/// <param name="pcThis">The player</param>
/// <param name="iSkillID">Skill ID (ActiveSkill iValue)</param>
/// <param name="iLevel1Based">Skill level</param>
/// <param name="pcMouseTarget">The target</param>
BOOL SkillManager::BeginSkill( class UnitData * pcThis, int iSkillID, int iLevel1Based, class UnitData * pcUnitTarget, int iX, int iY, int iZ )
{
	if ( pcUnitTarget == NULL )
		pcUnitTarget = pcThis;

	if ( (pcThis->sCharacterData.iType == CHARACTERTYPE_Monster || pcThis->PkMode_CharState == CHARACTERTYPE_Monster) &&
		  pcThis->PkMode_CharState != CHARACTERTYPE_Player)
	{
		if ( SKILLMANAGERHANDLER->BeginSkillMonster( pcThis ) )
			return TRUE;
	}


	if ( iSkillID == 0 )
		return TRUE;

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "BeginSkill id =%d", iSkillID ) );
#endif

	BOOL handled = FALSE;

	if ( pcThis->PkMode_CharState == CHARACTERTYPE_Player || pcThis->sCharacterData.iType == CHARACTERTYPE_Player )
	{
		Point3D point3D;
		point3D.iX = iX;
		point3D.iY = iY;
		point3D.iZ = iZ;

		ActiveSkill activeSkill = (ActiveSkill)iSkillID;

		switch ( pcThis->sCharacterData.iClass )
		{
			case ECharacterClass::CHARACTERCLASS_Fighter:     handled = FIGHTERSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Magician:    handled = MAGICIANSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Assassin:    handled = ASSASSINSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Mechanician: handled = MECHANICIANSKILLHANDLER->OnBeginSkill ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Atalanta:    handled = ATALANTASKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Priestess:   handled = PRIESTESSSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Shaman:      handled = SHAMANSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Archer:      handled = ARCHERSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Knight:      handled = KNIGHTSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
			case ECharacterClass::CHARACTERCLASS_Pikeman:     handled = PIKEMANSKILLHANDLER->OnBeginSkill	  ( pcThis, activeSkill, iLevel1Based, pcUnitTarget, point3D ); break;
		}
	}

	//if resu
	if ( handled == TRUE )
		return TRUE;

	__asm
	{
		PUSH iZ;
		PUSH iY;
		PUSH iX;
		PUSH pcUnitTarget;
		PUSH iLevel1Based;
		PUSH iSkillID;
		MOV ECX, pcThis;
		MOV EAX, 0x00418CD4;
		CALL EAX
	}

	switch( pcThis->sCharacterData.iClass )
	{
		/* Priestess Skills */
		case CHARACTERCLASS_Priestess:
			switch( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:

					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					MIRACULOUSLIGHTHANDLER->OnCastAnimationStart( pcThis );
					break;
			}
			break;
		/* Pikeman Skills */
		case CHARACTERCLASS_Pikeman:
			switch( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					return RINGOFSPEARSHANDLER->OnBeginSkillUse( pcThis, pcUnitTarget, iX, iY, iZ );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
		/* Archer Skills */
		case CHARACTERCLASS_Archer:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:

					break;
				case SKILLPLAYID_T52:
					return FIERCEWINDHANDLER->OnCastMember( pcThis );
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
		/* Fighter Skills */
		case CHARACTERCLASS_Fighter:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:

					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
		/* Knight Skills */
		case CHARACTERCLASS_Knight:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T12:
				case SKILLPLAYID_T21:
					ResetSkillTimer( SKILLID_HolyConviction );
					HOLYCONVICTIONHANDLER->OnCastEnd( UNITDATATOUNIT( pcUnitTarget ) );
					break;
				case SKILLPLAYID_T51:
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
		/* Shaman Skills */
		case CHARACTERCLASS_Shaman:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					return CREEDHANDLER->OnBeginSkillUse( pcThis, pcUnitTarget, iX, iY, iZ );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
	}
	return FALSE;
}

void SkillManager::OnEffect()
{
}

void SkillManager::LoadParticles()
{
	SKILLEFFECTHANDLER->GetLethalSight()->LoadParticle();
	SKILLEFFECTHANDLER->GetFierceWind()->LoadParticle();
	SKILLEFFECTHANDLER->GetSesmicImpact()->LoadParticle();
	SKILLEFFECTHANDLER->GetStoneSkin()->LoadParticle();
	SKILLEFFECTHANDLER->GetHolyConviction()->LoadParticle();
	SKILLEFFECTHANDLER->GetRupture()->LoadParticle();
	SKILLEFFECTHANDLER->GetWizardTrance()->LoadParticle();
	SKILLEFFECTHANDLER->GetRingOfSpears()->LoadParticle();
	SKILLEFFECTHANDLER->GetShadowBlink()->LoadParticle();

	PRIESTESSSKILLHANDLER->LoadParticle();
	FIGHTERSKILLHANDLER->LoadParticle();
	KNIGHTSKILLHANDLER->LoadParticle();
	ASSASSINSKILLHANDLER->LoadParticle();
	MECHANICIANSKILLHANDLER->LoadParticle();
	ATALANTASKILLHANDLER->LoadParticle();
	SHAMANSKILLHANDLER->LoadParticle();
}

int SkillManager::FormatSkillTimerInfo( Skill * pSkill )
{
	return TIMERSKILLHANDLER->OnRenderTimer( pSkill );
}

BOOL SkillManager::ResetForceOrb()
{
	/*Skill * psaSkill = (Skill*)0x036789F8;
	for ( int i = 0; i < 20; i++ )
	{
		if ( (psaSkill[i].iID & 0xFFFF0000) == 0x03060000 )
		{
			ZeroMemory( &psaSkill[i], sizeof( Skill ) );
			return TRUE;
		}
	}*/
	return FALSE;
}

void SkillManager::ReformSkill( Skill * psSkill, int iSkill )
{
	typedef void( __thiscall *tfnReformSkill )(DWORD dwClass, Skill * psSkill, int iSkill);
	tfnReformSkill fnReformSkill = (tfnReformSkill)0x004EC0B0;
	fnReformSkill( 0x0367E148, psSkill, iSkill );
}

//int cSKILL::ReFormCharSkillInfo()
void SkillManager::ReformCharSkillInfo()
{
	CALL ( 0x4EBC40 );
}

//int SaveCheckSkill()
void SkillManager::SaveCheckSkill ()
{
	CALL ( 0x627BC0 );
}

//int ReformSkillInfo()
void SkillManager::ReformSkillInfo ()
{
	CALL ( 0x620C60 );
}

BOOL SkillManager::OnMouseClick( class CMouse * pcMouse )
{
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR && *(int*)(0x0367DE8C) )
	{
		int iID = *(int*)(0x0367DE8C) - 1;
		if ( sTempSkillTimerData[iID].iID == SKILLID_ForceOrb )
		{
			// Show Box to accept the quest
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Are you sure?" );
			MESSAGEBOX->SetDescription( "Do you want to cancel the Force Orb?" );
			MESSAGEBOX->SetEvent( std::bind( &SkillManager::CancelForceOrb, this, *(int*)(0x0367DE8C) - 1 ) );
			MESSAGEBOX->Show();

			return TRUE;
		}
		if ( sTempSkillTimerData[iID].iID == 0x096A0003 || sTempSkillTimerData[iID].iID == 0x096B0003 )
		{
			// Show Box to accept the quest
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Are you sure?" );
			MESSAGEBOX->SetDescription( "Do you want to cancel the Active Quest?" );
			MESSAGEBOX->SetEvent( std::bind( &CQuestGame::HandleCancelQuest, CQUESTGAME, sTempSkillTimerData[iID].iID, sTempSkillTimerData[iID].PlusState[4] ) );
			MESSAGEBOX->Show();

			return TRUE;
		}

		if ( sTempSkillTimerData[iID].iID )
		{
			switch ( sTempSkillTimerData[iID].iID )
			{
				case (DWORD)ETimerID::DrasticSpirit:
				{
					SOCKETG->SendPacketBlank( PKTHDR_CancelDrasticSpirit );
					TIMERSKILLHANDLER->KillTimer ( sTempSkillTimerData[iID].iID );
				}
				break;
			}
		}


#if defined (DEV_MODE)
		//if ( GM_MODE )
		//{
		//	if ( sTempSkillTimerData[iID].iID )
		//	{
		//		//CHATBOX->AddMessage ( "GM> Force Expiry Skill" );
		//		//SKILLMANAGERHANDLER->ResetSkillTimer ( sTempSkillTimerData[iID].iID );
		//		TIMERSKILLHANDLER->ZeroOutTimer ( sTempSkillTimerData[iID].iID );
		//
		//		if ( sTempSkillTimerData[iID].iID == ETimerSkillID::TIMERSKILLID_VirtualLife )
		//		{
		//			VIRTUALLIFEHANDLER->CancelSkillSelf ();
		//		}
		//		else if ( sTempSkillTimerData[iID].iID == ETimerSkillID::TIMERSKILLID_VirtualRegen )
		//		{
		//			VIRTUALREGENHANDLER->CancelSkillSelf ();
		//		}
		//
		//		//SKILLMANAGERHANDLER->ResetSkillCountdown ( (ESkillID)sTempSkillTimerData[iID].iID );
		//		//SKILLMANAGERHANDLER->CancelSkillOnServer ( sTempSkillTimerData[iID].iID, sTempSkillTimerData[iID].iLevel1Based );
		//	}
		//}
#endif //  DEBUG_MMODE

	}
	return FALSE;
}

void SkillManager::ResetSkillCountdown ( enum ESkillID eSkillID )
{
#ifdef DEV_MODE

	if ( GM_MODE )
	{
		for ( int i = 1; i <= 20; i++ )
		{
			auto sSkill = SKILLSCHARACTER + i;

			if ( sSkill->iID == eSkillID )
			{
				UPDATE_CHARACTER_CHECKSUM;

				CheckSkillValidate ( sSkill );
				sSkill->GageLength = 35;
				sSkill->GageLength2 = 35;
				ValidateSkill ( sSkill );

				//This code causes Cheat_ID 52 to be triggered. Not sure why..

				UPDATE_CHARACTER_CHECKSUM;

				break;
			}
		}
	}
#endif
}

BOOL SkillManager::OnMouseClickSkillPet()
{
	int iID = *(int*)(0x0367DE8C) - 1;
	if ( iID >= 0 )
	{
		for ( int i = 0; i < _countof( iaSkillPet ); i++ )
		{
			if ( sTempSkillTimerData[iID].iID == iaSkillPet[i] )
			{
				baPetAggressive[i] = baPetAggressive[i] ? FALSE : TRUE;

				if ( i <= 4 )
				{
					WRITEDWORD ( ( 0x0367E940 + ( i * 4 ) ), baPetAggressive[i] );
				}

				CALL_WITH_ARG4( 0x00628E60, iaSkillPet[i], baPetAggressive[i], 0, 0 ); //this sends a PKTHDR_UpdatePetData command containing
				break;
			}
		}
	}
	return TRUE;
}

BOOL SkillManager::SetDefaultPetMode ( int iSkillID, BOOL bAgressive )
{
	for ( int i = 0; i < _countof ( iaSkillPet ); i++ )
	{
		if ( iaSkillPet[i] == iSkillID )
		{
			baPetAggressive[i] = bAgressive;
			WRITEDWORD ( ( 0x0367E940 + ( i * 4 ) ), bAgressive );
			CALL_WITH_ARG4 ( 0x00628E60, iSkillID, bAgressive, 0, 0 ); //this sends a PKTHDR_UpdatePetData command. See leaked function: SendUpdateSkillToServer

			return TRUE;
		}
	}

	return FALSE;
}


BOOL SkillManager::IsAgressive( int iSkillID )
{
	for ( int i = 0; i < _countof( iaSkillPet ); i++ )
	{
		if ( iaSkillPet[i] == iSkillID )
			return baPetAggressive[i];
	}

	return FALSE;
}

void SkillManager::ResetSkillEffectParticle( DWORD dwSkillID )
{
	struct EffectStruct
	{
		DWORD				dwID;
		char				cPadding[0x1B4];
	};

	EffectStruct * ps = (EffectStruct*)0x0373F8F8;

	for ( int i = 0; i < 1000; i++ )
	{
		if ( ps[i].dwID != 0 )
		{
			if ( ps[i].dwID == dwSkillID )
			{
				ZeroMemory( &ps[i], 0x1B8 );
			}
		}
	}

	struct AssaEffect
	{
		char				cPadding[0xC];
		DWORD				dwID;
		int					iCurTime;
		int					iTotalTime;
	};

	AssaEffect	** pASSA = (AssaEffect**)0x37B80F0;
	for ( int i = 0; i < 5000; i++ )
	{
		AssaEffect * p = pASSA[i];
		if ( p )
		{
			if ( p->dwID == dwSkillID )
			{
				p->iCurTime = p->iTotalTime;
			}
		}
	}
}


void SkillManager::ResetBuff( DWORD dwSkillID, UnitData * pcUnitData )
{
	BOOL bUpdate = FALSE;

	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	if ( SKILLMANAGERHANDLER->IsActiveBuff( dwSkillID ) )
	{
		switch ( dwSkillID )
		{
			case SKILLID_WizardTrance:
				WIZARDTRANCEHANDLER->OnCastEnd( pcUnit );
				bUpdate = TRUE;
				break;
			case SKILLID_LethalSight:
				LETHALSIGHTHANDLER->OnCastEnd( pcUnit );
				bUpdate = TRUE;
				break;

			case SKILLID_Rainmaker:
			case SKILLID_DancingSword:
			case SKILLID_SpiritElemental:
			case SKILLID_Zenith:
			case SKILLID_EnchantWeapon:
			case SKILLID_HolyReflection:
				if ( dwSkillID == SKILLID_EnchantWeapon )
				{
					if ( UNITDATA_HOVER == pcUnitData )
						return;
					if ( UNITDATA == pcUnitData && UNITDATA_HOVER )
						return;

					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponFire );
					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponIce );
					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponLightning );
				}
				ResetSkillEffectParticle( dwSkillID );
				bUpdate = TRUE;
				break;
			case SKILLID_AdventMigal:
			case SKILLID_AdventMidranda:
			case SKILLID_SummonMuspell:
				if ( *(DWORD*)(0x4B07A00) && dwSkillID != SKILLID_AdventMidranda )
				{
					CALLT_WITH_ARG1( 0x005C9590, *(DWORD*)(0x4B07A00), 6 );
					*(DWORD*)(0x4B07A00) = 0;
				}
				if ( *(DWORD*)(0x4B07A04) && dwSkillID == SKILLID_AdventMidranda )
				{
					CALLT_WITH_ARG1( 0x005C9590, *(DWORD*)(0x4B07A04), 6 );
					*(DWORD*)(0x4B07A04) = 0;
				}
				bUpdate = TRUE;
				break;

			default:
				break;
		}

		if ( bUpdate )
		{
			if ( dwSkillID == SKILLID_EnchantWeapon )
			{
				if ( UNITDATA_HOVER == pcUnitData )
					return;
				if ( UNITDATA == pcUnitData && UNITDATA_HOVER )
					return;
			}

			ResetSkillTimer( dwSkillID );
		}
	}

}

/// <summary>
/// ASM address: 0x005742E0
/// Leaked code:
/// </summary>
void SkillManager::StartSkillEffect( Point3D sStartPosition, Point3D sTargetPosition, int iSkillID, int iLevel )
{
	CALL_WITH_ARG8( pfnStartSkillEffect, sStartPosition.iX, sStartPosition.iY, sStartPosition.iZ, sTargetPosition.iX, sTargetPosition.iY, sTargetPosition.iZ, iSkillID, iLevel );
}

NAKED bool SkillManager::GetWeaponPosition( UnitData * pcUnitData, Point3D * psPositionOut, float fU )
{
	JMP( pfnGetWeaponPosition );
}

/// <summary>
/// Finds nearest monster with dist threshold of 300*300
/// Leaked: smCHAR *FindNearMonster( smCHAR *lpCurChar )
/// </summary>
NAKED UnitData * SkillManager::FindNearestMonster( UnitData * lpCurChar)
{
	JMP( pfnFindNearestMonster );
}


/// <summary>
/// Finds nearest monster with a dist threshold
/// Leaked: smCHAR *FindNearMonster( smCHAR *lpCurChar )
/// </summary>
UnitData * SkillManager::FindNearestPlayer( UnitData * lpCurChar, int iRange)
{
	UnitData * lpNearestChar;
	int cnt, distXZ, distY;
	int	nearestDist;

	lpNearestChar = NULL;
	nearestDist = iRange * iRange;

	if ( lpCurChar == NULL )
		return NULL;

	for ( cnt = 0; cnt < MAX_UNITS; cnt++ )
	{
		UnitData * pcUnitData = UNITGAME->pcaUnitData + cnt;

		if ( pcUnitData == NULL || pcUnitData->bActive == FALSE )
			continue;

		//Players only
		if ( pcUnitData->sCharacterData.iType != ECharacterType::CHARACTERTYPE_Player )
			continue;

		//Skip dead units
		if ( pcUnitData->GetCurrentHealth() <= 0 )
			continue;

		//Skip this char
		if ( lpCurChar == pcUnitData )
			continue;

		//Skip summons
		if ( pcUnitData->sCharacterData.iMonsterType == EMonsterType::MONSTERTYPE_Summon )
			continue;

		lpCurChar->sPosition.GetPTDistanceXZ_Y( &pcUnitData->sPosition, distXZ, distY );

		//Is closer?
		if ( distXZ < nearestDist && distY <= 65 )
		{
			lpNearestChar = pcUnitData;
			nearestDist = distXZ;
		}
	}

	return lpNearestChar;
}



/// <summary>
/// Finds nearest monster with a dist threshold
/// Leaked: smCHAR *FindNearMonster( smCHAR *lpCurChar )
/// </summary>
UnitData * SkillManager::FindNearestMonster( UnitData * lpCurChar, int iRange)
{
	UnitData * lpNearestChar;
	int cnt, distXZ, distY;
	int	nearestDist;

	lpNearestChar = NULL;
	nearestDist = iRange * iRange;

	if ( lpCurChar == NULL )
		return NULL;

	if ( lpCurChar != UNITDATA &&
		(lpCurChar->sCharacterData.iClanID == NULL || lpCurChar->sCharacterData.iClanID != UNITDATA->sCharacterData.iClanID))
	{
		lpCurChar->sPosition.GetPTDistanceXZ_Y( &UNITDATA->sPosition, distXZ, distY );

		//Is closer?
		if ( distXZ < nearestDist && distY <= 65 )
		{
			lpNearestChar = UNITDATA;
			nearestDist = distXZ;
		}
	}

	for ( cnt = 0; cnt < MAX_UNITS; cnt++ )
	{
		UnitData * pcUnitData = UNITGAME->pcaUnitData + cnt;

		if ( pcUnitData == NULL || pcUnitData->bActive == FALSE )
			continue;

		//Monsters only
		if ( pcUnitData->sCharacterData.iType != ECharacterType::CHARACTERTYPE_Monster )
			continue;

		//Skip dead units
		if ( pcUnitData->GetCurrentHealth() <= 0 )
			continue;

		//Skip this char
		if ( lpCurChar == pcUnitData )
			continue;

		//Skip summons
		if ( pcUnitData->sCharacterData.iMonsterType == EMonsterType::MONSTERTYPE_Summon )
			continue;

		//Skip units that is in same clan
		if ( pcUnitData->sCharacterData.iClanID && pcUnitData->sCharacterData.iClanID == lpCurChar->sCharacterData.iClanID )
			continue;

		lpCurChar->sPosition.GetPTDistanceXZ_Y( &pcUnitData->sPosition, distXZ, distY );

		//Is closer?
		if ( distXZ < nearestDist && distY <= 65 )
		{
			lpNearestChar = pcUnitData;
			nearestDist = distXZ;
		}
	}

	return lpNearestChar;
}

BOOL SkillManager::GetWeaponPositionNew( UnitData * pcUnitData, Point3D & sPositionOut, float fScaleSize )
{
	EXEMatrixI & sMatrix = pcUnitData->sLeftHandMatrix;

	//Get Weapon Left
	if ( pcUnitData->sLeftHandTool.eItemID != 0 )
	{
		sMatrix			= pcUnitData->sLeftHandMatrix;
		sPositionOut	= Point3D( (int)((float)pcUnitData->sLeftHandTool.iSizeMax * fScaleSize), 0, 0 );
	}
	else
	{
		//Get Weapon Right
		if ( pcUnitData->sRightHandTool.eItemID != 0 )
		{
			sMatrix			= pcUnitData->sRightHandMatrix;
			sPositionOut	= Point3D( (int)((float)pcUnitData->sRightHandTool.iSizeMax * fScaleSize), 0, 0 );
		}
		else
			return FALSE;
	}

	//Radian
	Point3D sRadian;
	sRadian.iX		= sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
	sRadian.iY		= sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
	sRadian.iZ		= sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

	//Get Weapon Position
	sPositionOut.iX	= (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
	sPositionOut.iZ	= (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
	sPositionOut.iY	= (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);

	return TRUE;
}

void SkillManager::HandlePacket( PacketSkillPartyData * psPacket )
{
	switch ( psPacket->iClass )
	{
		case CHARACTERCLASS_Knight:
			//Holy Valor??
			if ( psPacket->iSkillID == 0x2A )
			{
				Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_HolyValor );
				if ( psSkill && psSkill->iUseTime > (70 * 3) )
				{
					ResetSkillEffectParticle( SKILLEFFECTID_HolyValorAction );
					ResetSkillTimer( SKILLID_HolyValor );
					ResetSkillTimer( SKILLID_HolyConviction );
					HOLYCONVICTIONHANDLER->OnCastEnd( UNIT );
				}
			}

			//Holy Conviction?
			if ( psPacket->iSkillID == SKILLPLAYID_T51 )
			{
				HOLYCONVICTIONHANDLER->OnCastEffect( UNIT, psPacket->iLevel, 300.0f );
			}
			break;
	}
}

void SkillManager::CancelSkillOnServer( int iID, int iP1, int iP2, int iP3 )
{
	CALL_WITH_ARG4( 0x00628E30, iID, iP1, iP2, iP3 );
}

void SkillManager::ProcessSkillInParty( int iID, int iLevel, int iArea, int iP1, int iP2, int iP3, int iP4 )
{
	PacketSkillPartyData sPacket;
	ZeroMemory( &sPacket, sizeof(PacketSkillPartyData) );
	sPacket.iHeader = PKTHDR_BuffSkillsParty;
	sPacket.iLength = sizeof(PacketSkillPartyData);
	sPacket.iSkillID = iID;
	sPacket.iParam1 = iP1;
	sPacket.iParam2 = iP2;
	sPacket.iParam3 = iP3;
	sPacket.iClass = iP4;
	sPacket.iCount = 0;

	sPacket.iaID[sPacket.iCount++] = UNITDATA->iID;

	if ( PARTYHANDLER->GetPartyData().iMembersCount > 0 )
	{
		std::vector<UnitData*> vUnitData = PARTYHANDLER->GetPartyMembers();
		for ( auto &p : vUnitData )
		{
			if ( p != UNITDATA )
			{
				if ( p && p->bActive )
				{
					int iX = (UNITDATA->sPosition.iX - p->sPosition.iX) >> 8;
					int iY = (UNITDATA->sPosition.iY - p->sPosition.iY) >> 8;
					int iZ = (UNITDATA->sPosition.iZ - p->sPosition.iZ) >> 8;
					int iDistance = iX*iX + iY*iY + iZ*iZ;

					if ( iDistance < (iArea * iArea) )
					{
						sPacket.iaID[sPacket.iCount++] = p->iID;

						if ( sPacket.iCount == _countof( sPacket.iaID ) )
							break;
					}
				}
			}
		}
	}

	sPacket.iClass = UNITDATA->sCharacterData.iClass;
	sPacket.iLevel = iLevel;

	if( SOCKETG )
		SENDPACKETG( &sPacket, TRUE );
}

/// <summary>
/// void AssaParticle_KeepSkillEffect(char *FileName, smCHAR *pChar, int liveCount, unsigned int dwSkillCode);
/// </summary>
void SkillManager::AssaParticle_KeepSkillEffect( char * FileName, UnitData * pChar, int iDurationSec, ESkillID eSkillID )
{
	//0x005B6EC0 = AssaParticle_KeepSkillEffect
	CALL_WITH_ARG4( 0x005B6EC0, (DWORD)FileName, (DWORD)pChar, iDurationSec, eSkillID );
}

/// <summary>
/// void AssaParticle_KeepSkillEffect(char *FileName, smCHAR *pChar, int liveCount, unsigned int dwSkillCode);
/// </summary>
void SkillManager::AssaParticle_AttackSkillEffect( char * FileName, UnitData * pChar, DWORD dwSkillCode )
{
	//0x005B6F60 = AssaParticle_AttackSkillEffect
	CALL_WITH_ARG3( 0x005B6F60, (DWORD)FileName, (DWORD)pChar, dwSkillCode );
}



/// <summary>
/// smTRANSCODE_PROCESS_SKILL2
/// For buffs
/// Received via RecvBuffSkill on server side
/// </summary>
int SkillManager::SendProcessSkillToServer( DWORD dwSkillCode, int point, int param1, int param2 )
{
	return CALL_WITH_ARG4( 0x00623790, (DWORD)dwSkillCode, point, param1, param2 );
}

/// <summary>
/// ASM address: 0x004097F0
/// Leaked code: dm_SendTransDamage
///
/// Attack opponent character / monster
/// </summary>
/// <param name="pcMouseTarget">The target</param>
/// <param name="iPowParam1">Weapon damage min</param>
/// <param name="iPowParam2">Weapon dmage max</param>
/// <param name="eAttackState">Attack state</param>
/// <param name="iResistance"></param>
/// <param name="iID">Skill id (Active Skill)</param>
/// <param name="bUseAccuracy"></param>
int SkillManager::SendTransDamage( UnitData * pcUnitData, int iPowParam1, int iPowParam2, int eAttackState, int iResistance, int iID, BOOL bUseAccuracy )
{
	return CALL_WITH_ARG7( 0x004097F0, (DWORD)pcUnitData, iPowParam1, iPowParam2, eAttackState, iResistance, iID, bUseAccuracy );
}

/// <summary>
/// A more simple version of SendTransDamage where
/// most of the parameters doesn't get used.
/// </summary>
int SkillManager::SendTransDamage( UnitData * pcUnitData, const ActiveSkill & sActiveSkill, BOOL bUseAccuracy )
{
	return SendTransDamage( pcUnitData, 0, 0, 0, 0, sActiveSkill.iValue, bUseAccuracy );
}

int SkillManager::SendTransDamageSock ( UnitData * pcUnitData, int iPowParam1, int iPowParam2, int eAttackState, int iResistance, int iID, BOOL bUseAccuracy, SocketData * pcSocketData)
{
	return CALL_WITH_ARG8 ( 0x004093A0, (DWORD)pcUnitData, iPowParam1, iPowParam2, eAttackState, iResistance, iID, bUseAccuracy, (DWORD)pcSocketData );
}

/// <summary>
/// SendRangeDamage
/// Leaked code = dm_SendRangeDamage - Transmission after setting range attack
/// </summary>
void SkillManager::SendRangeDamage( UnitData * pcUnitData, Point3D & sPosition, int iPP1, int iPP2, int eAttackState, int iResistance, int iAttackSkillID )
{
	CALL_WITH_ARG9( 0x004098D0, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)pcUnitData, iPP1, iPP2, eAttackState, iResistance, iAttackSkillID );

}

/// <summary>
/// SendRangeDamage
/// Leaked code = dm_SendRangeDamage - Transmission after setting range attack
/// </summary>
/// <param name="pcUnitTarget">Target unit data. Its position will be used</param>
/// <param name="iAttackSkillID">The attack skill code</param>
void SkillManager::SendRangeDamage( UnitData * pcUnitData, int iAttackSkillID )
{
	CALL_WITH_ARG9( 0x004098D0, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, (DWORD)pcUnitData, 0, 0, 0, 0, iAttackSkillID );

}

/// <summary>
/// SelectRangeBox
/// Leaked code: dm_SelectRangeBox
/// Function Address = 0x4085E0
/// </summary>
void SkillManager::SelectRangeBox( UnitData * pcUnitData, RECT & rRect, BOOL bUseAttackRating )
{
	CALL_WITH_ARG3( 0x004085E0, (DWORD)pcUnitData, (DWORD)&rRect, (DWORD)bUseAttackRating );
}

/// <summary>
/// From leaked code: Damage.cpp
/// Initializes the lpTransSkill_AttackData (0x00A04760)
/// </summary>
/// <returns></returns>
BOOL SkillManager::dm_OpenTransRangeDamage ()
{
	return CALL ( 0x00408410 );
}

/// <summary>
/// Based on Chain lighting source from leaked code
/// </summary>
/// <param name="lpCurChar">Current player</param>
/// <param name="lpTragetChar">Target</param>
/// <param name="SelCount">Max target</param>
/// <param name="Size">Range</param>
/// <returns>Num of targets selected</returns>
int SkillManager::SelectRangeChained ( UnitData * lpPlayerUnit, UnitData * lpTargetUnit, int iMaxTargets, int iAttackRadius, int iMaxAttackRange )
{

	int cnt, cnt2, cnt3;
	int dx, dy, dz, dist;
	int cCount;
	UnitData * lpChar;
	UnitData * lpLinkChar;
	int dSize, dMaxSize;
	int minDist;
	UnitData * lpMinChar;

	dm_OpenTransRangeDamage ();

	PacketMultiTargetSkillData * psPacket = (*(PacketMultiTargetSkillData**)0x00A04760); //lpTransSkill_AttackData

	UNITS_AREA_COUNT = 0;
	//if ( lpCurChar!=lpCurPlayer ) return FALSE;

	dSize = iAttackRadius * iAttackRadius;
	dMaxSize = iMaxAttackRange * iMaxAttackRange;

	UNITS_AREA_TARGET[UNITS_AREA_COUNT] = lpTargetUnit; //first char is the target char
	//dwSkill_ChainedSkill_Target[dmSelected_CharCnt] = lpTragetChar->iID;
	psPacket->iaTargetID[psPacket->iTargetCount++] = lpTargetUnit->iID;
	UNITS_AREA_COUNT++;

	lpLinkChar = lpTargetUnit;

	for ( cnt2 = 1; cnt2 < iMaxTargets; cnt2++ )
	{
		minDist = dSize;
		lpMinChar = 0;

		for ( cnt = 0; cnt < MAX_UNITS; cnt++ )
		{
			cCount = cnt;
			UnitData * pcUnitData = UNITGAME->pcaUnitData + cnt;

			if ( pcUnitData &&
				pcUnitData->bActive && pcUnitData->iID &&
				lpLinkChar != pcUnitData &&
				lpPlayerUnit != pcUnitData &&
				pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Monster &&
				pcUnitData->sCharacterData.iMonsterType != EMonsterType::MONSTERTYPE_Summon &&
				pcUnitData->GetCurrentHealth() > 0 )
			{

				lpChar = pcUnitData;

				for ( cnt3 = 0; cnt3 < UNITS_AREA_COUNT; cnt3++ )
				{
					if ( UNITS_AREA_TARGET[cnt3] == lpChar )
					{
						lpChar = NULL;
						break;
					}
				}

				if ( lpChar )
				{
					dx = ( lpChar->sPosition.iX - lpTargetUnit->sPosition.iX ) >> 8;
					dy = ( lpChar->sPosition.iY - lpTargetUnit->sPosition.iY ) >> 8;
					dz = ( lpChar->sPosition.iZ - lpTargetUnit->sPosition.iZ ) >> 8;

					dist = dx * dx + dz * dz;

					//max length check to original unit
					if ( dMaxSize && dist > dMaxSize )
						continue;

					dx = ( lpChar->sPosition.iX - lpLinkChar->sPosition.iX ) >> 8;
					dy = ( lpChar->sPosition.iY - lpLinkChar->sPosition.iY ) >> 8;
					dz = ( lpChar->sPosition.iZ - lpLinkChar->sPosition.iZ ) >> 8;

					dist = dx * dx + dz * dz;

					//is closer than min?
					if ( dist < minDist && abs ( dy ) < 70 )
					{
						lpMinChar = lpChar;
						minDist = dist;
					}
				}
			}
		}

		//found min char?
		if ( lpMinChar )
		{
			psPacket->iaTargetID[psPacket->iTargetCount++] = lpMinChar->iID;
			UNITS_AREA_TARGET[UNITS_AREA_COUNT] = lpMinChar;

			UNITS_AREA_COUNT++;

			lpLinkChar = lpMinChar;
		}
	}

	return UNITS_AREA_COUNT;

}

/// <summary>
/// SelectRangeBox
/// Function Address = 0x0048AF40
/// This will in turn store GeoResults in
/// GeoResult_X @ 0x035BF3F8. use RADIAN3D_X
/// GeoResult_Y @ 0x035BF3FC. use RADIAN3D_Y
/// GeoResult_Z @ 0x035BF400. use RADIAN3D_Z
/// </summary>
void SkillManager::GetMoveLocation( int x, int y, int z, short AngX, short AngY, short AngZ )
{
	CALL_WITH_ARG6( 0x48AF40, x, y, z, AngX, AngY, AngZ );
}

/// <summary>
/// ASM address = 0x004085B0
/// Leaked code: dm_SelectRange
/// Select units in a circle based range
/// </summary>
/// <param name="sPosition"></param>
/// <param name="iRange"></param>
/// <param name="bUseAttackRating"></param>
void SkillManager::SetUnitsRange( Point3D & sPosition, int iRange, BOOL bUseAttackRating )
{
	CALL_WITH_ARG5( 0x004085B0, sPosition.iX, sPosition.iY, sPosition.iZ, iRange, bUseAttackRating );
}

/// <summary>
/// Leaked code: dm_SelectDamageCount
/// Range-type attack range selection
/// </summary>
void SkillManager::SelectDamageCount( UnitData * lpCurChar, int SelCount, int Size )
{
	CALL_WITH_ARG3( 0x00408760, (DWORD)lpCurChar, (DWORD)SelCount, (DWORD)Size );
}

void SkillManager::SetUnitsRangeCount( Point3D & sPosition, int iRange, BOOL bUseAttackRating, int iTargetCount )
{
	CALL_WITH_ARG6( 0x00408450, sPosition.iX, sPosition.iY, sPosition.iZ, iRange, bUseAttackRating, iTargetCount );
}

int SkillManager::GetFrameSpeed( int iAttackSpeed )
{
	int iResult = CALL_WITH_ARG1( 0x0045F800, iAttackSpeed );

	//CHATBOX->AddMessage ( FormatString ( "Frame Speed = %d", iResult ) );

	return iResult;
}

/// <summary>
/// Leak: int GetAttackSpeedMainFrame( int AttackSpeed )
/// ASM: 0x45F7A0
/// </summary>
int SkillManager::GetAttackSpeedMainFrame ( int iAttackSpeed )
{
	int result;
	int iSpeedBuffPercentage = 0;
	int iSpeedBuffFlat = 0;

	if ( iAttackSpeed )
	{
		result = 40 * ( iAttackSpeed + 2 );
	}
	else
	{
		result = 280;
	}


	if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Fighter )
	{
		if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_ConcentrationSA ) )
		{
			iSpeedBuffPercentage += SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_ConcentrationSA_ATKSpdBoost_New, ESkillID::SKILLID_ConcentrationSA );
		}
	}

	if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Mechanician )
	{
		if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_Automation ) )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
			if ( psItemData && ( psItemData->sItem.sItemID.ToItemType () == ITEMTYPE_Bow || psItemData->sItem.sItemID.ToItemType () == ITEMTYPE_Javelin ) )
			{
				iSpeedBuffFlat += SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Mech_Automation_AddATKSpeed, ESkillID::SKILLID_Automation );
			}
		}

		if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_Maximize ) )
		{
			iSpeedBuffPercentage += SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Mech_Maximize_ATKSPDBoost_New, ESkillID::SKILLID_Maximize );
		}

		Skill* electricalOverload = TIMERSKILLHANDLER->FindSkillTimer(ETimerID::ElectricalOverload);
		if (electricalOverload && electricalOverload->iFlag)
		{
			iSpeedBuffPercentage += SKILLMANAGERHANDLER->GetSkillIntValue(ESkillArrayPointer::Mech_ElectricalOverload_AttackSpeed, ESkillID::SKILLID_ElectricalOverload);
		}
	}

	if (UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin)
	{
		iSpeedBuffPercentage += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Assassin_AssassinMastery_AtkSpeedBoost_New, ESkillID::SKILLID_AssassinMastery );
	}


	if ( iSpeedBuffPercentage > 0 )
	{
		result += ( result * iSpeedBuffPercentage ) / 100;
	}

	if ( iSpeedBuffFlat > 0 )
	{
		result += iSpeedBuffFlat * 40;
	}

	return result;
}

/// <summary>
/// int	GetAttackFrameSpeed( int AttackSpeed , int AddSpeed )
/// ASM: 0x45F7D0
/// </summary>
/// <param name="iAttackSpeed"></param>
/// <param name="iAddSpeed"></param>
/// <returns></returns>
int SkillManager::GetAttackFrameSpeed( int iAttackSpeed, int iAddSpeed )
{
	return CustomGetAttackFrameSpeed ( iAttackSpeed, iAddSpeed );
}

/// <summary>
/// Leaked: int	GetAttackFrameSpeed_Half( int AttackSpeed )
/// ASM: 45F800
/// </summary>
/// <param name="iAttackSpeed"></param>
/// <returns></returns>
int SkillManager::GetAttackFrameSpeed_Half ( int iAttackSpeed )
{
	int cnt;
	cnt = GetAttackSpeedMainFrame ( iAttackSpeed );
	return cnt >> 2;
}


void SkillManager::CheckSkillValidate( Skill * psSkill )
{
	CALLT_WITH_ARG1( 0x004EC230, 0x0367E148, (DWORD)psSkill );
}

void SkillManager::ValidateSkill( Skill * psSkill )
{
	CALLT_WITH_ARG1( 0x004EC2C0, 0x0367E148, (DWORD)psSkill );
}

void SkillManager::SkillEffectLearn( UnitData * pcUnitData )
{
	CALL_WITH_ARG1( 0x0040FBC0, (DWORD)pcUnitData );
}

//Happens when the player buys the skill
BOOL SkillManager::OnBuySkill()
{
	if ( CURRENTSKILL )
	{
		//T5?
		if ( (CURRENTSKILL->iID & 0x00FF0000) == 0x00100000 )
		{
			//Locked
			if ( GM_MODE == FALSE )
			{
				if( SKILLDATAHANDLER->IsSkillLocked( CURRENTSKILL->iID ) )
				{
					TITLEBOX( "Skill not avaliable!" );
					return TRUE;
				}
			}

			if ( (CHARACTERGAME->GetFreeSkillPointForTier5() > 0) && ((CURRENTSKILL->sSkillInfo.iSkillLevelRequired + (CURRENTSKILL->iLevel * 2)) <= PLAYERDATA->iLevel) )
			{
				//Skill Price
				int * piaPrice = ((int*)0x009BBEA8);
				int * piaPriceP = ((int*)0x009BBE58);

				int iCost = piaPrice[CURRENTSKILL->sSkillInfo.iSkillIndex] + (piaPriceP[CURRENTSKILL->sSkillInfo.iSkillIndex] * CURRENTSKILL->iLevel);

				if ( CHARACTERGAME->CanUseGold( iCost ) && (UNITDATA->sCharacterData.iGold - iCost) > 0 )
				{
					UNITDATA->sCharacterData.iGold -= iCost;

					UPDATE_CHARACTER_CHECKSUM;

					ITEMHANDLER->SendGoldUse( iCost );

					CheckSkillValidate( CURRENTSKILL );
					CURRENTSKILL->iLevel++;
					ValidateSkill( CURRENTSKILL );

					//Decrease FSP
					CHARACTERGAME->SetFreeSkillPointForTier5( CHARACTERGAME->GetFreeSkillPointForTier5() - 1 );

					UPDATE_CHARACTER_CHECKSUM;

					ITEMHANDLER->ProcessGold();
					PLAYMINISOUND( 18 );

					ITEMHANDLER->UpdateItemCharacterStatus();
					ITEMHANDLER->CheckDamageItems();
					SkillEffectLearn();

					int iTalent = PLAYERDATA->iTalent / 3;

					if ( iTalent > 50 )
						iTalent = 50;

					int iLearn = 10000 - (iTalent * 100);

					if ( iLearn >= CURRENTSKILL->UseSkillCount )
						CURRENTSKILL->UseSkillCount /= 2;
					else
						CURRENTSKILL->UseSkillCount = iLearn / 2;

					SAVE;
					SAVEEX;
				}
				else
					TITLEBOX( "Not enough gold" );
			}
			else
				TITLEBOX( "Not enough Skill Points" );

			return TRUE;
		}
	}

	return FALSE;
}

void SkillManager::UpdateSkillCanBuy()
{
	int iCount = 0;

	switch ( PLAYERDATA->iRank )
	{
		case CHARACTERRANK_Rank1:
			iCount = 5;
			break;

		case CHARACTERRANK_Rank2:
			iCount = 9;
			break;

		case CHARACTERRANK_Rank3:
			iCount = 13;
			break;

		case CHARACTERRANK_Rank4:
			iCount = 17;
			break;

		case CHARACTERRANK_Rank5:
			iCount = 21;
			break;

		default:
			break;
	}

	SKILLS_TIERCOUNT = iCount;

	if ( SKILLS_TIERCOUNT != SKILLS_TIERCOUNT_BACKUP )
	{
		SKILLS_TIERCOUNT_BACKUP = SKILLS_TIERCOUNT;

		ITEMHANDLER->UpdateItemCharacterStatus( FALSE );
	}

	BOOL bCanUpdate = FALSE;

	for ( int i = 0; i < iCount; i++ )
	{
		Skill * ps = SKILLSCHARACTER + i;

		bCanUpdate = FALSE;

		//Tier 1, 2 & 3
		if ( i <= 12 )
		{
			//SP
			if ( READDWORD( 0x03673B70 ) > 0 )
			{
				ps->iFlag = TRUE;
				bCanUpdate	= TRUE;
			}
		}
		//Tier 4
		else if ( (i > 12) && (i <= 16) )
		{
			//EP
			if ( READDWORD( 0x03673B74 ) > 0 )
			{
				ps->iFlag = TRUE;
				bCanUpdate	= TRUE;
			}
		}
		//Tier 5
		else if ( i > 16 )
		{
			//FSP
			if ( CHARACTERGAME->GetFreeSkillPointForTier5() > 0 )
			{
				ps->iFlag = TRUE;
				bCanUpdate	= TRUE;
			}
		}

		ps->bCanUse = FALSE;

		//Can Use Skill
		if ( ps->iFlag && bCanUpdate )
		{
			if ( ps->iLevel <= 10 )
			{
				//Not First Skill?
				if ( i > 0 )
				{
					if ( SKILLSCHARACTER[i-1].iLevel > 0 )
					{
						if ( (ps->sSkillInfo.iSkillLevelRequired + (ps->iLevel * 2)) <= PLAYERDATA->iLevel )
							ps->bCanUse = TRUE;
					}
				}
			}
		}
	}
}

void SkillManager::OnClickSkill()
{
	WRITEDWORD( 0x0367DF94, 0 );
	WRITEDWORD( 0x0367DF98, 0 );

	Skill * psaSkills = ((Skill*)0x03670B68);

	if ( SKILL_INDEXBUTTON && SKILL_SHORTCUT_CLICK == FALSE )
	{
		Skill * ps = &psaSkills[SKILL_INDEXBUTTON];

		if ( ((ps->sSkillInfo.eSkillType == SKILLTYPE_Left) || (ps->sSkillInfo.eSkillType == SKILLTYPE_All)) &&
			WINDOW_ISOPEN_SKILLMASTER == FALSE && ps->iLevel )
		{
			SKILL_LEFTSIDE = ps;
		}
	}

	if ( SKILL_INDEXBUTTON != 1 && WINDOW_ISOPEN_SKILLMASTER )
	{
		BOOL bCanBuy = FALSE;

		if ( SKILL_INDEXBUTTON <= 13 )
		{
			//SP
			if ( READDWORD( 0x03673B70 ) > 0 )
				bCanBuy = TRUE;
		}
		//Tier 4
		else if ( (SKILL_INDEXBUTTON > 13) && (SKILL_INDEXBUTTON <= 17) )
		{
			//EP
			if ( READDWORD( 0x03673B74 ) > 0 )
				bCanBuy = TRUE;
		}
		//Tier 5
		else if ( SKILL_INDEXBUTTON > 17 )
		{
			//FSP
			if ( CHARACTERGAME->GetFreeSkillPointForTier5() > 0 )
				bCanBuy = TRUE;
		}

		//Check if previous skill has level
		if ( bCanBuy && (SKILL_INDEXBUTTON > 1) && (psaSkills[SKILL_INDEXBUTTON - 1].iLevel == 0) )
			bCanBuy = FALSE;

		if ( bCanBuy )
		{
			int iRequiredLevel = (psaSkills[SKILL_INDEXBUTTON].sSkillInfo.iSkillLevelRequired + (psaSkills[SKILL_INDEXBUTTON].iLevel * 2));
			if( iRequiredLevel <= PLAYERDATA->iLevel )
			{
				if ( (psaSkills[SKILL_INDEXBUTTON].iLevel >= 0) && (psaSkills[SKILL_INDEXBUTTON].iLevel < 10) )
				{
					SKILLINFOBOX->SetSkillInfoLevelOffset ( 0 ); //reset to 0 after purchase
					SKILLINFOBOX->Invalidate ();

					CURRENTSKILL = &psaSkills[SKILL_INDEXBUTTON];

					CALLT_WITH_ARG1( 0x004D32D0, 0x0362B210, 0 );

					WRITEDWORD( 0x0367DE90, 0 );
				}
			}
		}
	}
}

void SkillManager::OnEndSkillTimer( Skill * psSkill )
{
	switch ( psSkill->iID )
	{
		case SKILLID_QUEST1:
			if ( (*(short*)0x0362B5B2) > 0 )
			{
				(*(short*)0x0362B5B0) = 0;
				(*(int*)0x0362B5AC)--;
				(*(int*)0x0362B5A8) = 0;
				CALL( 0x0041A280 );
				CTITLEBOX->SetText( 0x37 );
				PLAYMINISOUND( 0x17 );
			}
			break;

		case SKILLID_LethalSight:
			LETHALSIGHTHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_HolyConviction:
			HOLYCONVICTIONHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_WizardTrance:
			WIZARDTRANCEHANDLER->OnCastEnd( UNIT );
			break;

		default:
			break;
	}
}

BOOL SkillManager::OnKeyPress ( class CKeyboard * pcKeyboard )
{
	if ( GM_MODE && WINDOW_ISOPEN_SKILLS )
	{
		int SkillInfoShowFlag = *(int *)0x367DF48;
		int SkillButtonIndex = *(int *)0x367DF88;

		if ( SkillInfoShowFlag && SkillButtonIndex )
		{
			if ( pcKeyboard->GetKey () == VK_UP || pcKeyboard->GetKey () == VK_DOWN )
			{
				if ( pcKeyboard->GetEvent () == EKeyboardEvent::KeyUp )
				{
					if ( pcKeyboard->GetKey () == VK_UP )
					{
						SKILLINFOBOX->SetSkillInfoLevelOffset( 0 );

						if ( KEYBOARDHANDLER->IsKeyDown ( VK_SHIFT ) )
						{
							GMAddSkillPoint ( 10 );
						}
						else
						{
							GMAddSkillPoint ( 1 );
						}
					}
					else if ( pcKeyboard->GetKey () == VK_DOWN )
					{
						SKILLINFOBOX->SetSkillInfoLevelOffset( 0 );

						if ( KEYBOARDHANDLER->IsKeyDown ( VK_SHIFT ) )
						{
							GMAddSkillPoint ( -10 );
						}
						else
						{
							GMAddSkillPoint ( -1 );
						}
					}

				}

				//return True to prevent other key presses (which causes zooming)
				return TRUE;
			}


		}
	}

	return FALSE;
}

void SkillManager::GMAddSkillPoint ( int iPoint )
{
	int SkillInfoShowFlag = *(int *)0x367DF48;
	int SkillButtonIndex = *(int *)0x367DF88;

	if ( SkillInfoShowFlag == 1 )
	{
		BOOL bTier4 = SkillButtonIndex > 13;

		Skill * sCurrentSkill = &SKILLSCHARACTER[SkillButtonIndex - 1];

		/*CHATBOX->AddDebugMessage ( FormatString ( "Tier 4? = %d", bTier4 ) );
		CHATBOX->AddDebugMessage ( FormatString ( "Skill num = %d", SkillButtonIndex ) );
		CHATBOX->AddDebugMessage ( FormatString ( "Skill id = %s", sCurrentSkill->iID ) );
		CHATBOX->AddDebugMessage ( FormatString ( "Level = %d -> %d", sCurrentSkill->iLevel1Based, iNewLevel ) );
		CHATBOX->AddDebugMessage ( FormatString ( "Mastery = %d", sCurrentSkill->Mastery ) );
		CHATBOX->AddDebugMessage ( FormatString ( "bCanUse = %d", sCurrentSkill->bCanUse ) );
		CHATBOX->AddDebugMessage ( FormatString ( "UseSkillCount = %d", sCurrentSkill->UseSkillCount ) );
		CHATBOX->AddDebugMessage ( FormatString ( "UseSkillMasteryGage = %d", sCurrentSkill->UseSkillMasteryGage ) );*/


		if ( iPoint >= 1 && sCurrentSkill->iLevel == 10 )
			return;

		if ( iPoint <= -1 && sCurrentSkill->iLevel == 1 )
			return;

		//positive = increase, negative = decrease, 0 = same
		int iFreeSkillPoints = bTier4 ? USESKILLS.iSkillPointTier4 : USESKILLS.iSkillPoint;

		if ( iPoint >= 1 && iFreeSkillPoints == 0 )
		{
			CHATBOX->AddMessage ( "Not enough skill points" );
			return;
		}

		if ( iPoint > iFreeSkillPoints )
		{
			CHATBOX->AddMessage ( "Not enough skill points" );
			iPoint = iFreeSkillPoints;
		}

		int iNewLevel = sCurrentSkill->iLevel + iPoint;

		if ( iNewLevel > 10 )
			iNewLevel = 10;

		else if (iNewLevel < 1)
			iNewLevel = 1;

		int iDiff = iNewLevel - sCurrentSkill->iLevel;

		//Skill is trained?
		if ( sCurrentSkill->Mastery )
		{
			Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer ( sCurrentSkill->iID );
			if ( psSkill )
			{
				TIMERSKILLHANDLER->ZeroOutTimer ( sCurrentSkill->iID );
			}

			UPDATE_CHARACTER_CHECKSUM;

			SKILLMANAGERHANDLER->CheckSkillValidate ( sCurrentSkill );
			sCurrentSkill->iLevel = iNewLevel;
			sCurrentSkill->UseSkillCount = 10000; //Fully trained

			sCurrentSkill->GageLength = 35;
			sCurrentSkill->GageLength2 = 35;

			SKILLMANAGERHANDLER->ValidateSkill ( sCurrentSkill );

			//Update Free Skill Point
			if ( bTier4 )
			{
				USESKILLS.iSkillPointTier4 -= iDiff;
			}
			else
			{
				USESKILLS.iSkillPoint -= iDiff;
			}

			UPDATE_CHARACTER_CHECKSUM;

			PLAYMINISOUND ( 18 );
			SKILLMANAGERHANDLER->SkillEffectLearn ();

			SAVE;
		}
	}
}

BOOL SkillManager::HandlePacket( PacketUseSkillBase * psPacket, Unit* pcTargetUnit )
{
	if ( pcTargetUnit == NULL || pcTargetUnit->pcUnitData == NULL )
		return FALSE;

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "OnSkillBuff id = %d, lvl = %d", psPacket->iSkillID, psPacket->iSkillLevel1Based ) );
#endif

	//the character class of the caster
	switch ( psPacket->eCasterCharacterClass )
	{
		case ECharacterClass::CHARACTERCLASS_Assassin:	return ASSASSINSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Fighter:	return FIGHTERSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Magician:	return MAGICIANSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Atalanta:	return ATALANTASKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Priestess:	return PRIESTESSSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Shaman:	return SHAMANSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Archer:	return ARCHERSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Mechanician: return MECHANICIANSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;
		case ECharacterClass::CHARACTERCLASS_Knight: return KNIGHTSKILLHANDLER->OnSkillBuff( pcTargetUnit, psPacket ); break;

		//@todo move these to class skill handler, like above
		default:
		{
			switch ( psPacket->iSkillID )
			{
				case SKILLID_LethalSight:
					LETHALSIGHTHANDLER->OnCastEffect( pcTargetUnit, psPacket->iSkillLevel1Based, 300.0f );
					break;

				case SKILLID_HolyConviction:
					HOLYCONVICTIONHANDLER->OnCastEffect( pcTargetUnit, psPacket->iSkillLevel1Based, 300.0f );
					break;

				default:
					break;
			}
		}
	}


	return FALSE;
}


/// <summary>
/// Received a party buff packet containing a list of users in which
/// this party buff will be applied to. This include players
/// outside of party for visual effect.
/// </summary>
BOOL SkillManager::HandlePacket( PacketUseSkillParty * psPacket )
{
	if ( psPacket->iSkillLevel1Based <= 0 || psPacket->iSkillLevel1Based > 10 )
		return FALSE;

	for ( int i = 0; i < psPacket->iUserCount; i++ )
	{
		if ( psPacket->aiUserID[i] == 0 )
			continue;

		Unit * pcUnitTarget = UNITDATATOUNIT( UNITDATABYID( psPacket->aiUserID[i] ) );

		HandlePacket( (PacketUseSkillBase *)psPacket, pcUnitTarget );
	}

	return TRUE;
}

//Iron: this code is to be obsoleted - use BOOL SkillManager::HandlePacket( PacketUseSkillParty * psPacket ) instead
BOOL SkillManager::HandlePacket( PacketUseSkillSingle * psPacket )
{
	//disable processing in ASM
	psPacket->iHeader = 0;

	Unit * pcUnitTarget = UNITDATATOUNIT( UNITDATABYID( psPacket->lTargetID ) );

	if ( pcUnitTarget == NULL || pcUnitTarget->pcUnitData == NULL )
		return FALSE;

	if ( psPacket->iSkillLevel1Based <= 0 || psPacket->iSkillLevel1Based > 10 )
		return FALSE;

	return HandlePacket( (PacketUseSkillBase *)psPacket, pcUnitTarget );
}

void SkillManager::ApplyPassiveSkillToSelf ()
{
	int value;
	UnitData * pcCurrPlayer = UNITDATA;
	Unit * pcUnit = UNIT;
	int iLevelSkill = 0;

	if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Fighter )
	{
		value = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_BoostHealth_HPBoostPercent, ESkillID::SKILLID_Resilience );
		if ( value != 0 )
		{
			UNITDATA->sCharacterData.sHP.sMax += ( UNITDATA->sCharacterData.sHP.sMax * value ) / 100;
		}

		value = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_Recuperation_HPRegen_New, ESkillID::SKILLID_Recuperation );
		if ( value != 0 )
		{
			UNITDATA->sCharacterData.fHPRegen += value;
		}
	}

	//Dodge Mastery (Assassin)
	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin )
	{
		iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_DodgeMastery );
		if ( iLevelSkill )
		{
			pcUnit->iEvadeRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Assassin_DodgeMastery_AddEvasive_New, iLevelSkill - 1 );
		}
	}


	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer )
	{
		iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_EvasionMastery );
		if ( iLevelSkill )
		{
			pcUnit->iEvadeRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Archer_EvasionMastery_IncreaseEvasion_New, iLevelSkill - 1 );
		}
	}


	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Pikeman )
	{
		if ( UNITGAME->IsWeaponEquippedOnSelf( EItemType::ITEMTYPE_Scythe ) )
		{
			iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_WeaponBlockMastery );
			if ( iLevelSkill )
			{
				pcCurrPlayer->sCharacterData.iBlockRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_WeaponBlockMastery_WPBlockBoost_New, iLevelSkill - 1 );
			}
		}
	}
}

void SkillManager::ApplySkillBuffToSelf ()
{
	Skill * psSkill = NULL;
	int boost, level;
	UnitData * pcCurrPlayer = UNITDATA;
	CharacterData * psPlayerData = PLAYERDATA;
	Unit * pcUnit = UNIT;

	//Triumph of Valhalla - max damage boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::TriumphOfValhalla, ESkillArrayPointer::Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New  );
	if ( boost )
	{
		//half for non-ata
		if ( psPlayerData->iClass != ECharacterClass::CHARACTERCLASS_Atalanta )
			boost >>= 1;

		psPlayerData->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}

	//Hall of Valhalla - Block  boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::HallOfValhalla, ESkillArrayPointer::Atalanta_HallOfValhalla_AddBlock_New  );
	if ( boost )
	{
		//half for non-ata
		if ( PLAYERDATA->iClass != ECharacterClass::CHARACTERCLASS_Atalanta )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.iBlockRating += boost;
	}

	//Virtual life - HP boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::VirtualLife, ESkillArrayPointer::Priestess_VirtualLife_HPBoost  );
	if ( boost )
	{
		//half for non-prs
		if ( PLAYERDATA->iClass != ECharacterClass::CHARACTERCLASS_Priestess )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.sHP.sMax += ( pcCurrPlayer->sCharacterData.sHP.sMax * boost ) / 100;
	}

	//Virtual life - MP boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::VirtualLife, ESkillArrayPointer::Priestess_VirtualLife_MPBoost_New  );
	if ( boost )
	{
		//half for non-prs
		if ( PLAYERDATA->iClass != ECharacterClass::CHARACTERCLASS_Priestess )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.sMP.sMax += ( pcCurrPlayer->sCharacterData.sMP.sMax * boost ) / 100;
	}

	//Virtual Regen - HP regen boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::VirtualRegen, ESkillArrayPointer::Priestess_VirtualRegen_HPRegen_New  );
	if ( boost )
	{
		//half for non-prs
		if ( pcCurrPlayer->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Priestess )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.fHPRegen += boost;
	}

	//Virtual Regen - MP regen boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::VirtualRegen, ESkillArrayPointer::Priestess_VirtualRegen_MPRegen_New  );
	if ( boost )
	{
		//half for non-prs
		if ( pcCurrPlayer->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Priestess )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.fMPRegen += boost;
	}

	//Alas - Evasion boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::Alas, ESkillArrayPointer::Assassin_Alas_AddEvasion  );
	if ( boost )
	{
		pcUnit->iEvadeRating += boost;
	}

	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Priestess )
	{
		boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( SKILLID_SummonMuspell, ESkillArrayPointer::Priestess_SummonMuspell_AddEvasion_New );
		if ( boost )
		{
			pcUnit->iEvadeRating += boost;
		}
	}

	//Advent Migal - Damage boost
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::AdventMigal, ESkillArrayPointer::Shaman_AdventMigal_AddATKPow );
	if ( boost )
	{
		//half for non-shaman
		if ( pcCurrPlayer->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Shaman )
			boost >>= 1;

		psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}

	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::ForceofNature, ESkillArrayPointer::Archer_ForceofNature_AddATKPow_New );
	if ( boost )
	{
		//half for non-as
		if ( psPlayerData->iClass != ECharacterClass::CHARACTERCLASS_Archer )
			boost >>= 1;

		psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}

	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ETimerID::ForceofNature, ESkillArrayPointer::Archer_ForceofNature_AddAtkRtg );
	if ( boost )
	{
		//half for non archers
		if ( pcCurrPlayer->sCharacterData.iClass != ECharacterClass::CHARACTERCLASS_Archer )
			boost >>= 1;

		pcCurrPlayer->sCharacterData.iAttackRating += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackRating * boost ) / 100;;
		SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
	}
	//God's Blessing - damage boost percent
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( SKILLID_GodsBlessing, ESkillArrayPointer::Knight_GodsBlessing_AddATKPowPercent_New);
	if ( boost )
	{
		psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}

	//Muspell
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ESkillID::SKILLID_SummonMuspell, ESkillArrayPointer::Priestess_SummonMuspell_DamageBoost_New);
	if ( boost )
	{
		psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}

	//Chasing hunt
	boost = TIMERSKILLHANDLER->FindSkillTimerIntValue ( ESkillID::SKILLID_ChasingHunt, ESkillArrayPointer::Shaman_ChasingHunt_AtkPowBoost_New);
	if ( boost )
	{
		psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
	}




	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer )
	{
		boost = TIMERSKILLHANDLER->FindSkillTimerIntValue( SKILLID_ScoutHawk, ESkillArrayPointer::Archer_ScoutHawk_AddAtkRtg_New );
		if ( boost )
		{
			pcCurrPlayer->sCharacterData.iAttackRating += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackRating * boost ) / 100;
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
		}
	}


	psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Berserker );
	if ( psSkill && psSkill->iFlag && psSkill->iLevel )
	{
		boost = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_Berserker_DecreaseAbs, ESkillID::SKILLID_Berserker );
		if ( boost != 0 )
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += ( CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating * boost ) / 100;
		}

		boost = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_Berserker_AddATKPow_New, ESkillID::SKILLID_Berserker ); //chained

		//chain
		boost += SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Fighter_Raving_DMGBoost, ESkillID::SKILLID_Raving, TRUE ); //chained
		if ( boost != 0 )
		{
			psPlayerData->iMaxDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * boost ) / 100;
			psPlayerData->iMinDamage += ( CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * boost ) / 100;
		}
	}

	psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Windy );
	if ( psSkill && psSkill->iFlag && psSkill->iLevel )
	{
		int iSkillValue = GetSkillIntValue ( ESkillArrayPointer::Atalanta_Windy_AddAttackRating_New, psSkill->iLevel - 1 );

		pcCurrPlayer->sCharacterData.iAttackRating += ((pcCurrPlayer->sCharacterData.iAttackRating * iSkillValue) / 100);
		SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
	}


	//Extreme shield
	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Mechanician )
	{
		if ( SKILLMANAGERHANDLER->IsActiveBuff (ESkillID::SKILLID_ExtremeShield) &&
			UNITGAME->IsShieldEquippedOnSelf() )
		{
			pcCurrPlayer->sCharacterData.iBlockRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Mech_ExtremeShield_AddBlock_New, ESkillID::SKILLID_ExtremeShield );
		}

		if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_PhysicalAbsorb ) ||
			 SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_MetalArmor ))
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += (CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating * SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Mech_PhysicalAbsorption_ABSBoost, ESkillID::SKILLID_PhysicalAbsorb ) ) / 100;
		}

		if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_Compulsion ))
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += (CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating * SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Mech_PhysicalAbsorption_ABSBoost, ESkillID::SKILLID_Compulsion ) ) / 100;
			pcCurrPlayer->sCharacterData.iAbsorbRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Mech_Compulsion_ExtraAbsorb_New, ESkillID::SKILLID_Compulsion );
		}
	}

	//Divine shield
	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Knight )
	{
		//shield required
		if ( UNITGAME->IsShieldEquippedOnSelf() )
		{
			psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_DivineShield );
			if ( psSkill && psSkill->iFlag && psSkill->iLevel )
			{
				pcCurrPlayer->sCharacterData.iBlockRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Knight_DivineShield_AddBlock_New, psSkill->iLevel - 1 );
			}
			else
			{
				psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_GodlyShield );
				if ( psSkill && psSkill->iFlag && psSkill->iLevel )
				{
					//Divine shield chain
					pcCurrPlayer->sCharacterData.iBlockRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Knight_DivineShield_AddBlock_New, ESkillID::SKILLID_DivineShield, TRUE );

					if ( ( UNITDATA->sLeftHandTool.eItemID & 0xFFFF0000 ) == ITEMTYPE_Shield )
					{
						boost = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Knight_GodlyShield_ShieldABSBoost, ESkillID::SKILLID_GodlyShield );
						pcCurrPlayer->sCharacterData.iAbsorbRating += ( *(int *)( 0x04B0B144 ) * boost ) / 100;
					}
				}
			}
		}

		//DrasticSpirit
		psSkill = TIMERSKILLHANDLER->FindSkillTimer( ETimerID::DrasticSpirit );
		if ( psSkill && psSkill->iFlag &&psSkill->iLevel )
		{
			boost = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Knight_DrasticSpirit_AddDefense, ESkillID::SKILLID_DivineShield );
			pcCurrPlayer->sCharacterData.iDefenseRating += (pcCurrPlayer->sCharacterData.iDefenseRating * boost) / 100;

			SETSTATUSCOLOR(CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_Orange);
		}
	}

	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Pikeman )
	{
		psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Vague );
		if ( psSkill && psSkill->iFlag && psSkill->iLevel )
		{
			pcUnit->iEvadeRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Pikeman_Vague_AddEvasion_New, psSkill->iLevel - 1 );
		}
	}

	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Shaman )
	{
		psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Rainmaker );
		if ( psSkill && psSkill->iFlag && psSkill->iLevel )
		{
			pcUnit->iEvadeRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Shaman_Rainmaker_AddEvasion_New, psSkill->iLevel - 1 );
		}
	}

	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Magician )
	{
		if (SKILLMANAGERHANDLER->IsActiveBuff(SKILLID_Zenith))
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += (CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating * SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_Zenith_AddAbsorbPercent, ESkillID::SKILLID_Zenith ) ) / 100;
			pcCurrPlayer->sCharacterData.iAbsorbRating += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_Zenith_AddAbsorb, ESkillID::SKILLID_Zenith );
		}
	}

	// ABS BC
	if (SKILLMANAGERHANDLER->IsActiveBuff(SKILLID_AbsorptionOfGlory))
	{
		pcCurrPlayer->sCharacterData.iAbsorbRating += ( CHARACTERGAME->sCharacterDataEx.iBaseAbsorbRating * 15 ) / 100;
	}

	if (SKILLMANAGERHANDLER->IsActiveBuff(TIMERSKILLID_BellatraSoloGold))
	{
		if (MAP_ID != MAPID_Bellatra &&
			MAP_ID != MAPID_BlessCastle)
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += 5;
		}
	}

	if (SKILLMANAGERHANDLER->IsActiveBuff(TIMERSKILLID_BellatraSoloSilver))
	{
		if (MAP_ID != MAPID_Bellatra &&
			MAP_ID != MAPID_BlessCastle)
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += 3;
		}
	}

	if (SKILLMANAGERHANDLER->IsActiveBuff(TIMERSKILLID_BellatraSoloBronze))
	{
		if (MAP_ID != MAPID_Bellatra &&
			MAP_ID != MAPID_BlessCastle)
		{
			pcCurrPlayer->sCharacterData.iAbsorbRating += 2;
		}
	}

	// Bellatra Gold
	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraGold ) )
	{
		int iMap = *(int*)(*(int*)((*(int*)((*(int*)0x0CF481C) + 0x200) * 4) + 0x0CF4748) + 0x1FD4);
		if ( iMap != MAPID_Bellatra && iMap != MAPID_BlessCastle)
		{
			PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 15) / 100;
			PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 15) / 100;

			pcCurrPlayer->sCharacterData.sHP.sMax += 50;
			pcCurrPlayer->sCharacterData.sSP.sMax += 100;
			pcCurrPlayer->sCharacterData.sMP.sMax += 100;

			(*(DWORD*)(0x03707150)) = 2;
			(*(DWORD*)(0x03707154)) = 3;
			(*(DWORD*)(0x03707158)) = 4;
		}
	}
	// Bellatra Silver
	else if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraSilver ) )
	{
		int iMap = *(int*)(*(int*)((*(int*)((*(int*)0x0CF481C) + 0x200) * 4) + 0x0CF4748) + 0x1FD4);
		if (iMap != MAPID_Bellatra && iMap != MAPID_BlessCastle)
		{
			PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 8) / 100;
			PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 8) / 100;

			pcCurrPlayer->sCharacterData.sHP.sMax += 25;
			pcCurrPlayer->sCharacterData.sSP.sMax += 50;
			pcCurrPlayer->sCharacterData.sMP.sMax += 50;

			(*(DWORD*)(0x03707150)) = 2;
			(*(DWORD*)(0x03707154)) = 3;
			(*(DWORD*)(0x03707158)) = 4;
		}
	}
	// Bellatra Bronze
	else if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraBronze ) )
	{
		int iMap = *(int*)(*(int*)((*(int*)((*(int*)0x0CF481C) + 0x200) * 4) + 0x0CF4748) + 0x1FD4);
		if (iMap != MAPID_Bellatra && iMap != MAPID_BlessCastle)
		{
			PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * 3) / 100;
			PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * 3) / 100;

			pcCurrPlayer->sCharacterData.sSP.sMax += 25;
			pcCurrPlayer->sCharacterData.sMP.sMax += 25;

			(*(DWORD*)(0x03707154)) = 3;
			(*(DWORD*)(0x03707158)) = 4;
		}
	}

	int iSkillValue = SKILLMANAGERHANDLER->GetSkillIntValue ( ESkillArrayPointer::Archer_DionsEye_AddAtkRtg, ESkillID::SKILLID_DionsEye );
	if ( iSkillValue > 0 )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];

		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Bow )
		{
			pcCurrPlayer->sCharacterData.iAttackRating += ( psItemData->sItem.iAttackRating * iSkillValue ) / 100;
		}
	}


	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Fighter )
	{
		iSkillValue = TIMERSKILLHANDLER->FindSkillTimerIntValue( ESkillID::SKILLID_ConcentrationSA, ESkillArrayPointer::Fighter_ConcentrationSA_ATKRtgBoost_New );
		if ( iSkillValue )
		{
			pcCurrPlayer->sCharacterData.iAttackRating += (CHARACTERGAME->sCharacterDataEx.iBaseAttackRating * iSkillValue ) / 100;
		}
	}

	if ( pcCurrPlayer->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin )
	{
		iSkillValue = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Assassin_DualWieldMastery_WPAtkRtgBoost_New, ESkillID::SKILLID_DualWieldMastery );
		if ( iSkillValue > 0 )
		{
			ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if ( psItemData && psItemData->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Dagger )
			{
				pcCurrPlayer->sCharacterData.iAttackRating += ( psItemData->sItem.iAttackRating * iSkillValue ) / 100;
			}
		}
	}

	//int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_LethalSight );
	//if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_LethalSight ) )
	//{
	//	if ( iLevelSkill )
	//	{
	//		int iPercentSkill = iaArcherT51CritRate[iLevelSkill - 1];
	//		pcCurrPlayer->sCharacterData.iCritical += (pcCurrPlayer->sCharacterData.iCritical * iPercentSkill) / 100;
	//		iPercentSkill = iaArcherT51RangeAdd[iLevelSkill - 1];
	//		pcCurrPlayer->sCharacterData.iAttackRange += (pcCurrPlayer->sCharacterData.iAttackRange * iPercentSkill) / 100;
	//	}
	//}
	//
	//iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_WizardTrance );
	//if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_WizardTrance ) )
	//{
	//	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex >= 0 )
	//	{
	//		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
	//		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Orb )
	//		{
	//			if ( iLevelSkill )
	//			{
	//				int iPercentSkill = iaMageT51BlockBoost[iLevelSkill - 1];
	//				pcCurrPlayer->sCharacterData.iBlockRating += (pcCurrPlayer->sCharacterData.iBlockRating * iPercentSkill) / 100;
	//				iPercentSkill = iaMageT51ATKRateRed[iLevelSkill - 1];
	//				pcCurrPlayer->sCharacterData.iAttackRating -= (pcCurrPlayer->sCharacterData.iAttackRating * iPercentSkill) / 100;
	//			}
	//		}
	//	}
	//}

	if ( SKILLMANAGERHANDLER->IsActiveBuff ( SKILLID_EnergyShield ) )
	{
		if ( ( CHARACTERGAME->GetCurrentMP () > 0 ) )
		{
			int decrease = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_EnergyShield_MPDrain_New, ESkillID::SKILLID_EnergyShield );

			int chainLevel1Based = SKILLMANAGERHANDLER->GetLevelSkill( ESkillID::SKILLID_MentalMastery );
			if ( chainLevel1Based )
			{
				//chain drain values is -ve (reduction)
				decrease += SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_MentalMastery_EW_Chain_MPDrain_New, chainLevel1Based - 1 );
			}

			if ( decrease < 0 )
				decrease = 0;

			CGameProtect::SetDecreaseMP ( CGameProtect::GetDecreaseMP () + decrease );

			//CALL_WITH_ARG2( 0x5BD940, (DWORD)UNITDATA, 3 ); //AssaParticle_EnergyShieldDefence( lpCurPlayer, 3 seconds );
		}
		else
		{
			//int StopAssaCodeEffect(smCHAR *pChar, int code, int setTime)

			//CALL_WITH_ARG3( 0x5B4840, (DWORD)UNITDATA, SKILLID_EnergyShield, 0 );

			//StopAssaCodeEffect_5B48A0((int)a1, DRASTIC_ACTION, 0); //For Drastic Spirit

	//		TIMERSKILLHANDLER->KillTimer( SKILLID_EnergyShield );
		}
	}

	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Creed ) )
	{
		Skill * p = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_Creed );
		if ( p && p->iLevel )
		{
			pcCurrPlayer->sCharacterData.sMP.sMax += (pcCurrPlayer->sCharacterData.sMP.sMax * iaShaT51MPBoost[p->iLevel - 1]) / 100;
			pcCurrPlayer->sCharacterData.sSP.sMax += (pcCurrPlayer->sCharacterData.sSP.sMax * iaShaT51STMBoost[p->iLevel - 1]) / 100;
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_MP, CHARSTATUSCOLOR_Blue );
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_SP, CHARSTATUSCOLOR_Green );
		}
	}

	int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_AssassinMastery );
	if ( iLevelSkill )
	{
		ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
		if ( psItemData && psItemData->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Dagger )
		{
			int * iSkillTable = (int*)0x00998454;
			int iPercentSkill = iSkillTable[iLevelSkill];
			pcCurrPlayer->sCharacterData.iCritical += ((int)psItemData->sItem.iCritical * iPercentSkill) / 100;
		}
	}

	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SurvivalInstinct );
	if ( iLevelSkill )
	{
		pcCurrPlayer->sCharacterData.fHPRegen += (float)iaSurvivalInstinctBoostHPRegen[iLevelSkill - 1];

		int iLevelSkill2 = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Resilience );
		if ( iLevelSkill2 )
		{
			int * piaSkill = ((int*)0x00995938);

			pcCurrPlayer->sCharacterData.sHP.sMax += (piaSkill[iLevelSkill2 - 1] * iaFighterT51BoostHealthBoost[iLevelSkill - 1]) / 100;
		}
		SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_HP, CHARSTATUSCOLOR_Pink );
	}


	iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( TIMERSKILLID_ChasingHunt );
	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_ChasingHunt ) )
	{
		if ( iLevelSkill )
		{
			if ( (pcCurrPlayer->sRightHandTool.eItemID & 0xFFFF0000) == 0x01090000 )
			{
				ItemData * psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
				if ( psItemData )
				{
					int * iSkillTable1 = (int*)0x04B0DBA4;
					int * iSkillTable2 = (int*)0x04B0DBCC;



					int iPercentSkill = iSkillTable1[iLevelSkill - 1];
					PLAYERDATA->iMinDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin * iPercentSkill) / 100;

					iPercentSkill = iSkillTable2[iLevelSkill - 1];
					PLAYERDATA->iMaxDamage += (CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax * iPercentSkill) / 100;
				}
			}
		}
	}
}

/// <summary>
/// Apply buff to weapon damage from passive skills
/// </summary>
/// <param name="iSkillDamage"></param>
/// <param name="iWeaponDamage"></param>
void SkillManager::SetWeaponDamageBuffFromSkill (int * iSkillDamage, int * iWeaponDamage)
{
	int iPercent, iFlat;

	if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Fighter )
	{
		iPercent = GetSkillIntValue ( ESkillArrayPointer::Fighter_MeleeMastery_WPAtkPowAddPercent_New, ESkillID::SKILLID_MeleeMastery );
		iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Fighter_MeleeMastery_WPAtkPowAdd_New, ESkillID::SKILLID_MeleeMastery );

		iSkillDamage[0] = ( ( iWeaponDamage[0] * iPercent ) / 100 ) + iFlat;
		iSkillDamage[1] = ( ( iWeaponDamage[1] * iPercent ) / 100 ) + iFlat;
	}
	else if (UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Mechanician)
	{
		ItemData* psItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
		if (psItemData && psItemData->bValid)
		{
			iPercent = GetSkillIntValue(ESkillArrayPointer::Mech_MechanicWeaponMastery_AddWeaponDamagePercent, ESkillID::SKILLID_MechanicWeaponMastery);

			if (!psItemData->IsTwoHandedWeapon())
				iPercent *= 0.5;

			if (psItemData->sItem.sItemID.ToItemType() == EItemType::ITEMTYPE_Hammer)
				iPercent *= 2;

			iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Mech_MechanicWeaponMastery_AddWeaponDamage_New, ESkillID::SKILLID_MechanicWeaponMastery );

			iSkillDamage[0] = ((iWeaponDamage[0] * iPercent) / 100) + iFlat;
			iSkillDamage[1] = ((iWeaponDamage[1] * iPercent) / 100) + iFlat;
		}
	}
	else if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Assassin )
	{
		if ( ItemID( UNITDATA->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Dagger )
		{
			iPercent = GetSkillIntValue( ESkillArrayPointer::Assassin_DualWieldMastery_WPAtkPowBoost, ESkillID::SKILLID_DualWieldMastery );
			iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Assassin_DualWieldMastery_WPAtkPowBoostFlat_New, ESkillID::SKILLID_DualWieldMastery );

			iSkillDamage[0] = ( ( iWeaponDamage[0] * iPercent ) / 100 ) + iFlat;
			iSkillDamage[1] = ( ( iWeaponDamage[1] * iPercent ) / 100 ) + iFlat;
		}
	}
	else if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Atalanta )
	{
		if ( ItemID ( UNITDATA->sRightHandTool.eItemID ).ToItemType () == ITEMTYPE_Javelin )
		{
			iPercent = GetSkillIntValue ( ESkillArrayPointer::Atalanta_ThrowingMastery_DMGBoostPercent_New, ESkillID::SKILLID_ThrowingMastery );
			iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Atalanta_ThrowingMastery_DMGBoost_New, ESkillID::SKILLID_ThrowingMastery );

			iSkillDamage[0] = ( ( iWeaponDamage[0] * iPercent ) / 100 ) + iFlat;
			iSkillDamage[1] = ( ( iWeaponDamage[1] * iPercent ) / 100 ) + iFlat;
		}
	}
	else if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer )
	{
		iPercent = GetSkillIntValue ( ESkillArrayPointer::Archer_ShootingMastery_AddWeaponDamagePercent, ESkillID::SKILLID_ShootingMastery );
		iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Archer_ShootingMastery_AddWeaponDamage, ESkillID::SKILLID_ShootingMastery );

		iSkillDamage[0] = (( iWeaponDamage[0] * iPercent ) / 100) + iFlat;
		iSkillDamage[1] = (( iWeaponDamage[1] * iPercent ) / 100) + iFlat;
	}
	else if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Knight )
	{
		iPercent = GetSkillIntValue ( ESkillArrayPointer::Knight_SwordMastery_AddWeaponDamagePercent, ESkillID::SKILLID_SwordMastery );
		iFlat = 0; // GetSkillIntValue( ESkillArrayPointer::Knight_SwordMastery_AddWeaponDamage, ESkillID::SKILLID_SwordMastery );

		iSkillDamage[0] = (( iWeaponDamage[0] * iPercent ) / 100) + iFlat;
		iSkillDamage[1] = (( iWeaponDamage[1] * iPercent ) / 100) + iFlat;
	}
}

/// <summary>
/// Add a empty line to the skill info box
/// </summary>
void SkillManager::AddSkillBoxEmptyLine ( )
{
	SKILLINFOBOX->AddString ( 0, "\n" );
}

/// <summary>
/// Add a bold colored title with a line break above it
/// </summary>
/// <param name="pszLabel">Label</param>
/// <param name="dwColor">Text color, use TEXTCOLOR_ defs</param>
void SkillManager::AddSkillBoxGroupTitle( const char * pszTitle, D3DCOLOR dwColor, EElementID eElementID )
{
	SKILLINFOBOX->AddString( 0, "\n" );
	auto textBoxPtr = SKILLINFOBOX->AddString( 0, pszTitle, dwColor, TRUE ); //TRUE = bold
	if ( textBoxPtr )
	{
		switch ( eElementID )
		{
			case EElementID::Fire:
			case EElementID::Ice:
			case EElementID::Organic:
			case EElementID::Lightning:
			case EElementID::Poison:
			{
				textBoxPtr->SetPropertyID( ETextPropertyID::Elemental );
				textBoxPtr->SetPropertyValue( eElementID );
			}
			break;
		}
	}

	SKILLINFOBOX->AddString ( 1, "");

	//also skip compare text
	SKILLINFOBOX->AddString ( 2, "" );
}

/// <summary>
/// Special case - add custom skill string with no comparsion value
/// </summary>
/// <param name="pszLabel">Label</param>
/// <param name="pszString">String to add</param>
void SkillManager::AddSkillBoxInfo ( const char * pszLabel, const char * pszString )
{
	SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
	SKILLINFOBOX->AddString ( 1, pszString, D3DCOLOR_XRGB ( 238, 210, 151 ) );

	//also skip compare text
	SKILLINFOBOX->AddString ( 2, "" );
}

/// <summary>
/// Special case - add custom skill string with no comparsion value
/// </summary>
/// <param name="pszLabel">Label</param>
/// <param name="pszString">String to add</param>
/// <param name="dwColor">Color to add</param>
void SkillManager::AddSkillBoxInfo ( const char * pszLabel, const char * pszString, DWORD dwColor )
{
	SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
	SKILLINFOBOX->AddString ( 1, pszString, dwColor );

	//also skip compare text
	SKILLINFOBOX->AddString ( 2, "" );
}

void SkillManager::AddSkillBoxInfoRange ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bShowNextLevel, float fScale )
{
	int value = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased );

	//only scale if not 1.0f
	if ( fScale != 1.0f )
		value = static_cast<int>( round( static_cast<float>( value ) * fScale ) );

	float valueMeters =  static_cast<float>(value) / 27.0f;

	//don't display skill value if it's 0
	if ( value == 0 )
	{
		if ( GM_MODE )
		{
			char hexString[16];
			itoa ( iSkillArrayPointerID, hexString, 16 );

			CHATBOX->AddMessage ( FormatString ( "%s contains 0 value(s) (%s)", pszLabel, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
		}

		return;
	}
	std::string format = FormatString ( pszFormat, value );

	SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
	SKILLINFOBOX->AddString ( 1, FormatString ( "%s [%.1fm]", format.c_str(), valueMeters ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

	//has next level info and is not same?
	if ( iSkillLevelZeroBased < 9 && bShowNextLevel)
	{
		int nextValue = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased + 1 );

		if ( nextValue != value )
		{
			valueMeters = static_cast<float>( nextValue ) / 27.0f;

			format = FormatString ( pszFormat, GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased + 1 ) );
			SKILLINFOBOX->AddString ( 2, FormatString ( "(%s [%.1fm])", format.c_str (), valueMeters ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
		}
		else
		{
			SKILLINFOBOX->AddString ( 2, "" );
		}
	}
	else
	{
		SKILLINFOBOX->AddString ( 2, "" );
	}
}


void SkillManager::AddChainSkillBoxInfoRange( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased )
{
	float chainScale = GetChainedSkillScale( iSkillArrayPointerID );
	AddSkillBoxInfoRange( pszLabel, pszFormat, iSkillArrayPointerID, iSkillLevelZeroBased, FALSE, chainScale );
}

void SkillManager::AddSkillBoxInfoDuration ( int iSkillArrayPointerID, int iSkillLevelZeroBased )
{
	int minutes, seconds, value, nextValue;

	value = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased );

	if ( iSkillArrayPointerID == Archer_ScoutHawk_Rotations )
	{
		value = static_cast<int>((value * 2.4)); //roughly 2.4s per 1 rotation. This calc is done in ASM
	}


	if ( value == 0 )
	{
#ifdef DEV_MODE
		if ( GM_MODE )
		{
			char hexString[16];
			itoa ( iSkillArrayPointerID, hexString, 16 );

			CHATBOX->AddMessage ( FormatString ( "Duration: contains 0 value(s) (%s)", hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
		}
#endif
		return;
	}
	SKILLINFOBOX->AddString ( 0, "Duration: ", D3DCOLOR_XRGB ( 243, 237, 216 ) );

	if ( value > 60 )
	{
		minutes = (int)( value / 60 );
		seconds = value % 60;

		if ( seconds > 0 )
		{
			SKILLINFOBOX->AddString ( 1, FormatString ( "%dm %ds", minutes, seconds ), D3DCOLOR_XRGB ( 238, 210, 151 ) );
		}
		else
		{
			SKILLINFOBOX->AddString ( 1, FormatString ( "%dm", minutes ), D3DCOLOR_XRGB ( 238, 210, 151 ) );
		}
	}
	else
	{
		SKILLINFOBOX->AddString ( 1, FormatString ( "%ds", value ), D3DCOLOR_XRGB ( 238, 210, 151 ) );
	}

	//has next level info?
	if ( iSkillLevelZeroBased < 9 )
	{
		nextValue = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased + 1 );

		if ( nextValue != value )
		{
			if ( nextValue > 60 )
			{
				minutes = (int)( nextValue / 60 );
				seconds = nextValue % 60;

				if ( seconds > 0 )
				{
					SKILLINFOBOX->AddString ( 2, FormatString ( "(%dm %ds)", minutes, seconds ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
				}
				else
				{
					SKILLINFOBOX->AddString ( 2, FormatString ( "(%dm)", minutes ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
				}
			}
			else
			{
				SKILLINFOBOX->AddString ( 2, FormatString ( "(%ds)", nextValue ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
			}
		}
		else
		{
			SKILLINFOBOX->AddString ( 2, "" );
		}
	}
	else
	{
		SKILLINFOBOX->AddString ( 2, "" );
	}
}

void SkillManager::AddSkillElement( EElementID eElement )
{
	SKILLINFOBOX->AddSkillElement( eElement );
}

/// <summary>
/// Add skill box info
/// </summary>
/// <param name="pszLabel">Skill label</param>
/// <param name="pszFormat">Value format</param>
/// <param name="iSkillArrayPointerID">ESkillArrayPointer id</param>
/// <param name="iSkillLevelZeroBased">Zero based skill level (0-9)</param>
/// <param name="bShowNextLevel">Show next level values</param>bShowNextLevel
/// <param name="fScaling">Scaling to apply to the skill value</param>
void SkillManager::AddSkillBoxInfo ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bShowNextLevel, float fScaling )
{
	//format is float type?
	if ( STRINGCOMPARE ( pszFormat, "%.0f" ) || STRINGCOMPARE ( pszFormat, "%.1f" ) || STRINGCOMPARE ( pszFormat, "%.2f" ) ||
		 STRINGCOMPARE ( pszFormat, "%.0f%%" ) || STRINGCOMPARE ( pszFormat, "%.1f%%" ) || STRINGCOMPARE ( pszFormat, "%.2f%%" ) )
	{
		float value = GetSkillFloatValue ( iSkillArrayPointerID, iSkillLevelZeroBased );


		//don't display skill value if it's 0.0f
		if ( value != 0.0f )
		{
			if ( fScaling != 1.0f )
			{
				value = value * fScaling;
			}

			SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
			SKILLINFOBOX->AddString ( 1, FormatString ( pszFormat, value ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			//has next level info?
			if ( iSkillLevelZeroBased < 9 && bShowNextLevel)
			{
				std::string format = FormatString ( pszFormat, GetSkillFloatValue ( iSkillArrayPointerID, iSkillLevelZeroBased + 1 ) );
				SKILLINFOBOX->AddString ( 2, FormatString ( "(%s)", format.c_str () ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
			}
			else
			{
				SKILLINFOBOX->AddString ( 2, "" );
			}
		}
		else
		{
#ifdef DEV_MODE
			if ( GM_MODE )
			{
				char hexString[16];
				itoa ( iSkillArrayPointerID, hexString, 16 );

				CHATBOX->AddMessage ( FormatString ( "%s contains 0 value(s) (%s)", pszLabel, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif

			SKILLINFOBOX->AddString ( 2, "" );
		}
	}
	else
	{
		int value = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased );


		//don't display skill value if it's 0
		if ( value != 0 )
		{
			//has scaling applied?
			if ( fScaling != 1.0f )
			{
				value = static_cast<int>( round( static_cast<float>( value ) * fScaling ) );
			}
			SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
			SKILLINFOBOX->AddString ( 1, FormatString ( pszFormat, value ), D3DCOLOR_XRGB ( 238, 210, 151 ) );

			//has next level info?
			if ( iSkillLevelZeroBased < 9 && bShowNextLevel )
			{
				value = GetSkillIntValue ( iSkillArrayPointerID, iSkillLevelZeroBased + 1 );

				if ( iSkillArrayPointerID == Archer_ScoutHawk_Rotations )
				{
					value = static_cast<int>( round( value * 2.4 ) ); //roughly 2.4s per 1 rotation. This calc is done in ASM
				}

				std::string format = FormatString ( pszFormat, value );
				SKILLINFOBOX->AddString ( 2, FormatString ( "(%s)", format.c_str () ), D3DCOLOR_XRGB ( 92, 163, 99 ), FALSE, EAlign::ALIGN_Left );
			}
			else
			{
				SKILLINFOBOX->AddString ( 2, "" );
			}
		}
		else
		{
#ifdef DEV_MODE
			if ( GM_MODE )
			{
				char hexString[16];
				itoa ( iSkillArrayPointerID, hexString, 16 );

				CHATBOX->AddMessage ( FormatString ( "%s contains 0 value(s) (%s)", pszLabel, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif

			SKILLINFOBOX->AddString ( 2, "" );
		}
	}
}

void SkillManager::AddChainSkillBoxInfo( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerID, int iSkillLevelZeroBased )
{
	float chainScale = GetChainedSkillScale( iSkillArrayPointerID );
	AddSkillBoxInfo( pszLabel, pszFormat, iSkillArrayPointerID, iSkillLevelZeroBased, FALSE, chainScale);
}

void SkillManager::AddSkillBoxInfoDual ( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerIDFirst, int iSkillArrayPointerIDSecond, int iSkillLevelZeroBased, BOOL bShowNextLevel, float fScaling1, float fScaling2 )
{
	int iValue1 = GetSkillIntValue ( iSkillArrayPointerIDFirst, iSkillLevelZeroBased );
	int iValue2 = GetSkillIntValue ( iSkillArrayPointerIDSecond, iSkillLevelZeroBased );

	if ( iValue1 == 0 || iValue2 == 0 )
	{
#ifdef DEV_MODE
		if ( GM_MODE )
		{
			char hexString[16];
			itoa ( iValue1 == 0 ? iSkillArrayPointerIDFirst : iSkillArrayPointerIDSecond, hexString, 16 );
			CHATBOX->AddMessage ( FormatString ( "%s contains 0 value(s) (%s)", pszLabel, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
		}
#endif
	}

	if ( fScaling1 != 1.0f )
	{
		iValue1 = static_cast<int>( round( static_cast<float>( iValue1 ) * fScaling1 ) );
	}

	if ( fScaling2 != 1.0f )
	{
		iValue2 = static_cast<int>( round( static_cast<float>( iValue2 ) * fScaling2 ) );
	}

	SKILLINFOBOX->AddString ( 0, pszLabel, D3DCOLOR_XRGB ( 243, 237, 216 ) );
	SKILLINFOBOX->AddString ( 1, FormatString ( pszFormat,
		iValue1,
		iValue2),
		D3DCOLOR_XRGB ( 238, 210, 151 ) );

	//has next level info?
	if ( iSkillLevelZeroBased < 9 && bShowNextLevel)
	{
		std::string format = FormatString ( pszFormat,
			GetSkillIntValue ( iSkillArrayPointerIDFirst, iSkillLevelZeroBased + 1 ),
			GetSkillIntValue ( iSkillArrayPointerIDSecond, iSkillLevelZeroBased + 1 ) );

		SKILLINFOBOX->AddString ( 2, FormatString ( "(%s)", format.c_str() ), D3DCOLOR_XRGB ( 92, 163, 99 ) );
	}
	else
	{
		SKILLINFOBOX->AddString ( 2, "" );
	}
}

void SkillManager::AddChainSkillBoxInfoDual( const char * pszLabel, const char * pszFormat, int iSkillArrayPointerIDFirst, int iSkillArrayPointerIDSecond, int iSkillLevelZeroBased )
{
	float chainScale1 = GetChainedSkillScale( iSkillArrayPointerIDFirst );
	float chainScale2 = GetChainedSkillScale( iSkillArrayPointerIDSecond );
	AddSkillBoxInfoDual( pszLabel, pszFormat, iSkillArrayPointerIDFirst, iSkillArrayPointerIDSecond, iSkillLevelZeroBased, FALSE, chainScale1, chainScale2 );
}

int * SkillManager::GetNewSkillIntArrayPointer ( int iSkillArrayPointerID )
{
	switch ( iSkillArrayPointerID )
	{

	case Archer_ElementalShot_LightningRange_New:	return ia_As_ElementalShot_LightningRange;
	case Archer_Perforation_AddedCritical_New:		return ia_As_Perforation_AddedCrit;
	case Archer_Perforation_AttackWidth_New:		return ia_As_Perforation_AttackWidth;
	case Archer_ForceofNature_AddedRange_New:		return ia_As_ForceOfNature_AddedRange;
	case Archer_ScoutHawk_AddAtkRtg_New:			return ia_As_ScoutHawk_AddAtkRtg;
	case Archer_PhoenixShot_AttackRange_New:		return ia_As_PhoenixShot_AttackRange;
	case Archer_PhoenixShot_AttackWidth_New:		return ia_As_PhoenixShot_AttackWidth;
	case Archer_PhoenixShot_BurningPercentOfAP_New: return ia_As_PhoenixShot_BurningPercentOfAP;
	case Archer_PhoenixShot_BurningDurationSec_New:	return ia_As_PhoenixShot_BurningDurationSec;
	case Archer_PhoenixShot_BurningChance_New:		return ia_As_PhoenixShot_BurningChance;
	case Archer_PhoenixShot_ChargedDMGBoost_New:	return ia_As_PhoenixShot_ChargedDamageBoost;
	case Archer_EvasionMastery_IncreaseEvasion_New:	return ia_As_EvasionMastery_IncreaseEvasion;
	case Archer_ForceofNature_AddATKPow_New:		return ia_As_ForceofNature_AddATKPow;

	case Mech_ExtremeShield_AddBlock_New:			return ia_Ms_ExtremeShield_AddBlock;
	case Mech_Maximize_ATKSPDBoost_New:				return ia_Ms_Maximize_AddAttackSpeed;
	case Mech_MechanicBomb_AttackRange_New:			return ia_Ms_MechanicBomb_AttackRange;
	case Mech_MechanicBomb_AttackFrameSpeed_New:	return ia_Ms_MechanicBomb_AttackSpeed;
	case Mech_MechanicBomb_BurningChance:			return ia_Ms_MechanicBomb_BurningChance;
	case Mech_MechanicBomb_BurningDamagePercent:	return ia_Ms_MechanicBomb_BurningDamagePercent;
	case Mech_MechanicBomb_BurningDurationSec:		return ia_Ms_MechanicBomb_BurningDurationSec;
	case Mech_MechanicBomb_BurningTickRateMs:		return ia_Ms_MechanicBomb_BurningTickRateMs;
	case Mech_Automation_MPDrain_New:				return ia_Ms_Automation_MPDrain;
	case Mech_Automation_SPDrain_New:				return ia_Ms_Automation_SPDrain;
	case Mech_Compulsion_ExtraAbsorb_New:			return ia_Ms_Complusion_AddAbsFlat;
	case Mech_Compulsion_AddAbsorbPercent_New:		return ia_Ms_Complusion_AddAbsPercent;
	case Mech_MetalGolem_Duration_New:				return ia_Ms_MetalGolem_Duration;
	case Mech_MechanicWeaponMastery_AddWeaponDamage_New: return ia_Ms_WeaponMastery_AddFlat;
	case Mech_ElectricalOverload_AttackSpeed:		return ia_Ms_ElectricalOverload_AttackSpeed;
	case Mech_ElectricalOverload_Duration:			return ia_Ms_ElectricalOverload_Duration;
	case Mech_Spark_AttackFrameSpeed_New:			return ia_Ms_Spark_AttackFrameSpeed;

	case Fighter_RageofZecram_AttackRadius_New:			return ia_fs_RageOfZecram_Radius;
	case Fighter_RageofZecram_BurningDamagePercent_New: return ia_fs_RageofZecram_BurningDamagePercent;
	case Fighter_RageofZecram_BurningChance_New:		return ia_fs_RageofZecram_BurningChance;
	case Fighter_RageofZecram_BurningDurationSec_New:	return ia_fs_RageofZecram_BurningDurationSec;
	case Fighter_BoostHealth_HPBoostPercent:			return ia_fs_BoostHealth_BoostPercent;
	case Fighter_BoostHealth_HPPotionBoost:				return ia_fs_BoostHealth_HPPotionBoost;
	case Fighter_Recuperation_HPRegen_New:				return ia_fs_Recuperation_HPRegen;
	case Fighter_Destroyer_HPCost_New:					return ia_fs_Destroyer_HPCost_Percent;
	case Fighter_AvengingCrash_HPCost_New:				return ia_fs_AvengingCrash_HPCost_Percent;
	case Fighter_BrutalSwing_HPCost_New:				return ia_fs_BrutalSwing_HPCost_Percent;
	case Fighter_BrutalSwing_CriticalDmgBoost_New:		return ia_fs_BrutalSwing_CriticalDmgBoost;
	case Fighter_ConcentrationSA_ATKSpdBoost_New:		return ia_fs_ConcentrationSA_ATKSpdBoost;
	case Fighter_ConcentrationSA_ATKRtgBoost_New:		return ia_fs_ConcentrationSA_AtkRtgBoost;
	case Fighter_Berserker_LifeLeechPercent_New:		return ia_fs_Berserker_LifeLeechPercent;
	case Fighter_Berserker_AddATKPow_New:				return ia_fs_Berserker_AddATKPow;
	case Fighter_Raving_HPCost_New:						return ia_fs_Raving_HPCost;
	case Fighter_WarCry_StunChance_New:					return ia_fs_WarCry_StunChance;
	case Fighter_WarCry_StunAtkPowPerc_New:				return ia_fs_WarCry_StunAtkPowPerc;
	case Fighter_WarCry_HPRestore_New:					return ia_fs_WarCry_HPRestore;
	case Fighter_WarCry_SPRestore_New:					return ia_fs_WarCry_SPRestore;
	case Fighter_WarCry_HPRegen_New:					return ia_fs_WarCry_HPRegen;
	case Fighter_WarCry_SPRegen_New:					return ia_fs_WarCry_SPRegen;
	case Fighter_WarCry_RegenDuration_Sec:				return ia_fs_WarCry_RegenDuration;
	case Fighter_DemonBuff_DamageBoostVsDemon_New:		return ia_fs_DemonBuff_DamageBoostVsDemon;

	case Pikeman_Expansion_Range_New:					return ia_Ps_Expansion_Range;
	case Pikeman_PolearmMastery_AddRange_New:			return ia_Ps_PolearmMastery_AddRange;
	case Pikeman_PreciseHit_ATKPow_New:					return ia_Ps_PreciseHit_AtkPow;
	case Pikeman_PreciseHit_AddAttackRating_New:		return ia_Ps_PreciseHit_AddAttackRating;
	case Pikeman_VenomSpear_PoisonNumStack_New:			return ia_Ps_VenomSpear_PoisonNumStack;
	case Pikeman_VenomSpear_PoisonDurationSec_New:		return ia_Ps_VenomSpear_PoisonDuration;
	case Pikeman_VenomSpear_PoisonDamagePercent_New:	return ia_Ps_VenomSpear_PoisonDamagePercent;
	case Pikeman_VenomSpear_PoisonChance_New:			return ia_Ps_VenomSpear_PoisonChance;
	case Pikeman_GroundPike_SpeedMonDown_New:			return ia_Ps_GroundPike_SpeedMonDown;
	case Pikeman_Vague_AddEvasion_New:					return ia_Ps_Vague_AddEvasion;
	case Pikeman_WeaponBlockMastery_WPBlockBoost_New:	return ia_Ps_WeaponBlockMastery_WPBlockBoost;

	case Atalanta_ShieldStrike_StunDamage:					return ia_Ata_ShieldStrike_StunDamage;
	case Atalanta_ShieldStrike_StunChance:					return ia_Ata_ShieldStrike_StunChance;
	case Atalanta_ShieldStrike_StunDurationSec:				return ia_Ata_ShieldStrike_StunDurationSec;
	case Atalanta_Windy_Range_New:							return ia_Ata_Windy_Range;
	case Atalanta_Windy_AddAttackRating_New:				return ia_Ata_Windy_AttackRate;
	case Atalanta_Farina_DMGBoostPercent_New:				return ia_Ata_Farina_DMGBoostPercent;
	case Atalanta_ThrowingMastery_DMGBoost_New:				return ia_Ata_ThrowingMastery_DMGBoost_New;
	case Atalanta_ThrowingMastery_DMGBoostPercent_New:		return ia_Ata_ThrowingMastery_DMGBoostPercent_New;
	case Atalanta_StormJavelin_AttackRange_New:				return ia_Ata_StormJavelin_AttackRange_New;
	case Atalanta_StormJavelin_AttackWidth_New:				return ia_Ata_StormJavelin_AttackRange_New;
	case Atalanta_LightningJavelin_Radius_New:				return ia_Ata_LightningJavelin_Radius_New;
	case Atalanta_LightningJavelin_StunChance:				return ia_Ata_LightningJavelin_StunChance;
	case Atalanta_LightningJavelin_StunDurationSec:			return ia_Ata_LightningJavelin_StunDurationSec;
	case Atalanta_FireJavelin_BurningChance:				return ia_Ata_FireJavelin_BurningChance;
	case Atalanta_FireJavelin_BurningDamagePercent:			return ia_Ata_FireJavelin_BurningDamagePercent;
	case Atalanta_FireJavelin_BurningDurationSec:			return ia_Ata_FireJavelin_BurningDurationSec;
	case Atalanta_FireJavelin_BurningTickRateMs:			return ia_Ata_FireJavelin_BurningTickRateMs;
	case Atalanta_HallOfValhalla_AddBlock_New:				return ia_Ata_HallOfValhalla_AddBlock;
	case Atalanta_FrostJavelin_FreezeChance_New:			return ia_Ata_FrostJavelin_FreezeChance;
	case Atalanta_FrostJavelin_FreezeDamage_New:			return ia_Ata_FrostJavelin_FreezeDamage;
	case Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New:   return ia_Ata_TriumphOfValhalla_AddMaxDamagePerc;
	case Atalanta_SoulSucker_Range_New:						return ia_Ata_SoulSucker_Range;
	case Atalanta_SoulSucker_LifeAbsorbCap_New:				return ia_Ata_SoulSucker_LifeAbsorbCap;

	case Knight_HolyValor_BuffRadius_New:				return ia_Ks_HolyValor_BuffRadius;
	case Knight_Piercing_PierceRange_New:				return ia_Ks_Piercing_PierceRange;
	case Knight_Piercing_PierceWidth_New:				return ia_Ks_Piercing_PierceWidth;
	case Knight_HolyIncantation_MaxHP_New:				return ia_Ks_HolyIncantation_MaxHP;
	case Knight_HolyIncantation_MaxAbs_New:				return ia_Ks_HolyIncantation_MaxAbs;
	case Knight_HolyIncantation_MaxDMGMin_New:			return ia_Ks_HolyIncantation_MaxDMGMin;
	case Knight_HolyIncantation_MaxDMGMax_New:			return ia_Ks_HolyIncantation_MaxDMGMax;
	case Knight_HolyIncantation_DMGIncrease_New:		return ia_Ks_HolyIncantation_DMGIncrease;
	case Knight_DivineShield_AddBlock_New:				return ia_Ks_DivineShield_AddBlock;
	case Knight_GodsBlessing_AddATKPowPercent_New:		return ia_Ks_GodsBlessing_AddATKPowPercent;
	case Knight_DrasticSpirit_UndeadDmgReduction_New:	return ia_Ks_DrasticSpirit_UndeadDmgReduction;
	case Knight_HolyBody_HealRadius_New:				return ia_Ks_HolyBody_HealRadius;
	case Knight_HolyBody_DmgToHealConversion_New:		return ia_Ks_HolyBody_DmgToHealConversion;


	case Priestess_VirtualLife_MPBoost_New:				return ia_Prs_VirtualLife_MPBoostPercent;
	case Priestess_VirtualLife_Duration:				return ia_Prs_VirtualLife_Duration;
	case Priestess_VirtualRegen_HPRegen_New:			return ia_Prs_VirtualRegen_HPRegen;
	case Priestess_VirtualRegen_MPRegen_New:			return ia_Prs_VirtualRegen_MPRegen;
	case Priestess_VirtualRegen_Duration_New:			return ia_Prs_VirtualRegen_Duration;
	case Priestess_DivineLightning_Damage_New:			return ia_Prs_DivineLightning_Damage;
	case Priestess_VigorBall_Damage_New:				return ia_Prs_VigorBall_Damage;
	case Priestess_ChainLightning_Damage_New:			return ia_Prs_ChainLightning_Damage;
	case Priestess_MultiSpark_NumSparksMin_New:			return ia_Prs_MultiSpark_NumSparksMin;
	case Priestess_SummonMuspell_DamageBoost_New:		return ia_Prs_SummonMuspell_DamageBoost;
	case Priestess_SummonMuspell_UndeadAbsorb_New:		return ia_Prs_SummonMuspell_UndeadAbsorb;
	case Priestess_SummonMuspell_AddEvasion_New:		return ia_Prs_SummonMuspell_AddEvasion;
	case Priestess_HolyBolt_DamageBoost_New:			return ia_Prs_HolyBolt_DamageBoost;
	case Priestess_Extinction_Radius_New:				return ia_Prs_Extinction_Radius;
	case Priestess_GlacialSpike_SpeedMonDown_New:		return ia_Prs_GlacialSpike_SpeedMonDown;
	case Priestess_GlacialSpike_FreezePercentOfAP_New:	return ia_Prs_GlacialSpike_FreezePercentOfAP;

	case Magician_FireBolt_BurningChance:				return ia_Mgs_FireBolt_BurningChance;
	case Magician_FireBolt_BurningDamagePercent:		return ia_Mgs_FireBolt_BurningDamagePercent;
	case Magician_FireBolt_BurningDurationSec:			return ia_Mgs_FireBolt_BurningDurationSec;
	case Magician_FireBall_BurningChance:				return ia_Mgs_FireBall_BurningChance;
	case Magician_FireBall_BurningDamagePercent:		return ia_Mgs_FireBall_BurningDamagePercent;
	case Magician_FireBall_BurningDurationSec:			return ia_Mgs_FireBall_BurningDurationSec;
	case Magician_EnchantWeapon_FireBurningDuration: 	return ia_Mgs_EnchantWeapon_FireBurningDuration;
	case Magician_EnchantWeapon_FireBurningTickRateMs:	return ia_Mgs_EnchantWeapon_FireBurningTickRateMs;
	case Magician_EnchantWeapon_IceFreezeDuration:		return ia_Mgs_EnchantWeapon_IceFreezeDuration;
	case Magician_EnchantWeapon_IceFreezeSpeedReduction: return ia_Mgs_EnchantWeapon_IceFreezeSlow;
	case Magician_EnchantWeapon_LightningStunDuration: 	return ia_Mgs_EnchantWeapon_LightningStunDuration;
	case Magician_EnchantWeapon_LightningRange_New: 	return ia_Mgs_EnchantWeapon_LightningRange;
	case Magician_FlameWave_BurningChance:				return ia_Mgs_FlameWave_BurningChance;
	case Magician_FlameWave_BurningDamagePercent:		return ia_Mgs_FlameWave_BurningDamagePercent;
	case Magician_FlameWave_BurningDurationSec:			return ia_Mgs_FlameWave_BurningDurationSec;
	case Magician_Distortion_Chance:					return ia_Mgs_Distortion_Chance;
	case Magician_Distortion_DamagePercent:				return ia_Mgs_Distortion_DamagePercent;
	case Magician_EnergyShield_MPDrain_New:				return ia_Mgs_EnergyShield_MPDrain_New;
	case Magician_MentalMastery_EW_Chain_MPDrain_New:	return ia_Mgs_MentalMastery_EW_Chain_MPDrain_New;


	case Magician_DancingSword_FireBurningChance_New:			return ia_Mgs_DancingSword_FireBurningChance;
	case Magician_DancingSword_FireBurningDamagePercent_New:	return ia_Mgs_DancingSword_FireBurningDamagePercent;
	case Magician_DancingSword_FireBurningDurationSec_New:		return ia_Mgs_DancingSword_FireBurningDurationSec;
	case Magician_DancingSword_IceFreezeChance_New:				return ia_Mgs_DancingSword_IceFreezeChance;
	case Magician_DancingSword_IceFreezeDuration_New:			return ia_Mgs_DancingSword_IceFreezeDuration;
	case Magician_DancingSword_IceFreezeSpeedReduction_New:		return ia_Mgs_DancingSword_IceFreezeSpeedReduction;

	case Assassin_Polluted_PoisonChance_New:			return ia_Ass_Polluate_PoisonChance;
	case Assassin_Polluted_PoisonNumStack_New:			return ia_Ass_Polluate_PoisonNumStack;

	case Assassin_Inpes_MovementSpeed_New:					return ia_Ass_Inpes_MovementSpeed;
	case Assassin_DodgeMastery_AddEvasive_New:				return ia_Ass_DodgeMastery_AddEvasive;
	case Assassin_DualWieldMastery_WPAtkPowBoostFlat_New:	return ia_Ass_DualWieldMastery_WPAtkPowBoostFlat;
	case Assassin_DualWieldMastery_WPAtkRtgBoost_New:		return ia_Ass_DualWieldMastery_WPAtkRtgBoost;
	case Assassin_AssassinMastery_AtkSpeedBoost_New:		return ia_Ass_AssassinMastery_AtkSpeedBoost;


	case Assassin_Cripple_PoisonPercentOfAP_New:		return ia_Ass_Cripple_PoisonPercentOfAP;
	case Assassin_Cripple_PoisonChance_New:				return ia_Ass_Cripple_PoisonChance;
	case Assassin_Cripple_PoisonDurationSec:			return ia_Ass_Cripple_PoisonDurationSec;
	case Assassin_Cripple_PoisonNumStack_New:			return ia_Ass_Cripple_PoisonNumStack;


	case Assassin_VenomThorn_PoisonDurationSec_New:		return ia_Ass_VenomThorn_PoisonDurationSec;
	case Assassin_VenomThorn_PoisonDamagePercent_New:	return ia_Ass_VenomThorn_PoisonDamagePercent;
	case Assassin_VenomThorn_PoisonChance_New:			return ia_Ass_VenomThorn_PoisonChance;
	case Assassin_VenomThorn_PoisonNumStack_New:		return ia_Ass_VenomThorn_PoisonNumStack;
	case Assassin_VenomThorn_Duration_New:				return ia_Ass_VenomThorn_Duration;

	case Assassin_SoulShock_StunAtkPowPerc_New:			return ia_Ass_SoulShock_StunAtkPowPerc;
	case Assassin_SoulShock_StunChance_New:				return ia_Ass_SoulShock_StunChance;

	case Assassin_Wisp_Curse_PercentOfAP_New:			return ia_Ass_Wisp_Curse_PercentOfAP;
	case Assassin_Wisp_Curse_Chance_New:				return ia_Ass_Wisp_Curse_Chance;
	case Assassin_Wisp_Curse_Duration_New:				return ia_Ass_Wisp_Curse_Duration;
	case Assassin_Wisp_Curse_SpeedReduction_New:		return ia_Ass_Wisp_Curse_SpeedReduction;

	case Shaman_Inertia_Curse_DamagePercent_New:		return ia_Sha_Inertia_Curse_DamagePercent;
	case Shaman_Inertia_Curse_Chance_New:				return ia_Sha_Inertia_Curse_Chance;
	case Shaman_Inertia_Curse_DamageReduction_New:		return ia_Sha_Inertia_Curse_DamageReduction;

	case Shaman_Rainmaker_AddEvasion_New:				return ia_Sha_Rainmaker_AddEvasion;
	case Shaman_Rainmaker_ReduceDamage_New:				return ia_Sha_Rainmaker_ReduceDamage;
	case Shaman_ChasingHunt_AtkPowBoost_New:			return ia_Sha_ChasingHunt_AtkPowBoost;
	case Shaman_Haunt_Radius_New:						return ia_Sha_Haunt_Radius;
	case Shaman_Haunt_LifeAbsorbCap_New:				return ia_Sha_Haunt_LifeStealCap;

	default:
	{

#ifdef DEV_MODE
		if ( GM_MODE )
		{
			char hexString[16];
			itoa ( iSkillArrayPointerID, hexString, 16 );

			CHATBOX->AddMessage ( FormatString ( "GetNewSkillIntArrayPointer> Missing switch case: 0x%s", hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
		}
#endif

		return nullptr;
	}

	}
}

float * SkillManager::GetNewSkillFloatArrayPointer ( int iSkillArrayPointerID )
{


	switch ( iSkillArrayPointerID )
	{

	default:
	{
#ifdef DEV_MODE
		if ( GM_MODE )
		{
			char hexString[16];
			itoa ( iSkillArrayPointerID, hexString, 16 );

			CHATBOX->AddMessage ( FormatString ( "GetNewSkillFloatArrayPointer> Missing switch case: 0x%s", hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
		}
#endif

		return nullptr;
	}

	}
}
/// <summary>
/// Get skill array pointer int value based on skill current level
/// </summary>
/// <param name="iSkillArrayPointerID">Pointer to skill int array</param>
/// <param name="eSkillID">The skill id</param>
int SkillManager::GetSkillIntValue ( enum ESkillArrayPointer eSkillArrayPointerID, enum ESkillID eSkillID, BOOL bChained )
{
	int iValue = 0;

	int iSkillLevel1Based = GetLevelSkill ( (UINT)eSkillID );
	if ( iSkillLevel1Based != 0)
	{
	    iValue = GetSkillIntValue ( (int) eSkillArrayPointerID, iSkillLevel1Based - 1 );

		if ( iValue == 0 )
		{
#ifdef DEV_MODE
			if ( GM_MODE )
			{
				char hexString[16];
				itoa ( eSkillID, hexString, 16 );

				char hexString1[16];
				itoa ( eSkillArrayPointerID, hexString1, 16 );

				CHATBOX->AddMessage ( FormatString ( "Skill %s has 0 value(s) for %s", hexString, hexString1 ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif
		}
		else
		{
			if ( bChained )
			{
				float chainScale = GetChainedSkillScale( eSkillArrayPointerID );
				if ( chainScale != 1.0f )
				{
					iValue = static_cast<int>( round( static_cast<float>( iValue ) * chainScale ) );
				}
			}
		}
	}

	return iValue;
}

/// <summary>
/// Get skill array pointer int value based on skill current level
/// </summary>
/// <param name="iSkillArrayPointerID">Pointer to skill int array</param>
/// <param name="iSkillLevelZeroBased">Skill level (0-based)</param>
int SkillManager::GetSkillIntValue ( int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bChained )
{
	char hexString[16];
	itoa ( iSkillArrayPointerID, hexString, 16 );

	int value = 0;

	if ( iSkillArrayPointerID < 0x00001000 )
	{
		int * pointer = GetNewSkillIntArrayPointer ( iSkillArrayPointerID );

		if ( pointer != nullptr )
		{
			value = pointer[iSkillLevelZeroBased];
		}
	}
	else
	{
		if ( iSkillLevelZeroBased >= 0 && iSkillLevelZeroBased <= 9 )
		{
			value = ((int *)iSkillArrayPointerID)[iSkillLevelZeroBased];
		}
		else
		{
#ifdef DEV_MODE
			if ( GM_MODE )
			{
				CHATBOX->AddMessage ( FormatString ( "GetSkillIntValue> Incorrect skill level %d for 0x%s", iSkillLevelZeroBased, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif
		}
	}


	if ( bChained )
	{
		float chainScale = GetChainedSkillScale( iSkillArrayPointerID );
		if ( chainScale != 1.0f )
		{
			value = static_cast<int>( round( static_cast<float>( value ) * chainScale ) );
		}
	}


	return value;
}

/// <summary>
/// Get skill array pointer float value based on skill current level
/// </summary>
/// <param name="iSkillArrayPointerID">Pointer to skill float array</param>
/// <param name="eSkillID">The skill id</param>
float SkillManager::GetSkillFloatValue ( int iSkillArrayPointerID, enum ESkillID eSkillID, BOOL bChained )
{
	return GetSkillFloatValue ( iSkillArrayPointerID, GetLevelSkill ( (UINT) eSkillID ), bChained );
}

/// <summary>
/// Get skill array pointer float value based on skill current level
/// </summary>
/// <param name="iSkillArrayPointerID">Pointer to skill float array</param>
/// <param name="iSkillLevelZeroBased">Skill level (0-based)</param>
float SkillManager::GetSkillFloatValue ( int iSkillArrayPointerID, int iSkillLevelZeroBased, BOOL bChained )
{
	if ( iSkillArrayPointerID < 0x00001000 )
	{
		switch ( iSkillArrayPointerID )
		{

			default:
			{

#ifdef DEV_MODE
			if ( GM_MODE )
			{
				char hexString[16];
				itoa ( iSkillArrayPointerID, hexString, 16 );

				CHATBOX->AddMessage ( FormatString ( "GetSkillFloatValue> missing switch case for: 0x%s", hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif

			}

			return 0;
		}

	}
	else
	{
		if ( iSkillLevelZeroBased >= 0 && iSkillLevelZeroBased <= 9 )
		{
			float value = ( (float *)iSkillArrayPointerID)[iSkillLevelZeroBased];

			if ( bChained )
			{
				float chainScale = GetChainedSkillScale( iSkillArrayPointerID );
				if ( chainScale != 1.0f )
				{
					value = value * chainScale;
				}
			}

			return value;
		}
		else
		{
#ifdef DEV_MODE
			if ( GM_MODE )
			{
				char hexString[16];
				itoa ( iSkillArrayPointerID, hexString, 16 );

				CHATBOX->AddMessage ( FormatString ( "GetSkillFloatValue> Incorrect skill level %d for: 0x%s", iSkillLevelZeroBased, hexString ), EChatColor::CHATCOLOR_White, NULL, TRUE );
			}
#endif

			return 0;
		}
	}
}


BOOL SkillManager::AddDamageVsMonsterTypeInfo ( Skill * pcSkill )
{
	std::string text = "";

	switch ( pcSkill->iID )
	{
		//Mech
		case SKILLID_Spark:				text = "+50% Damage VS Normal";			break;
		case SKILLID_MechanicBomb:		text = "+50% Damage VS Mutant";	break;
		case SKILLID_Automation:		text = "+150% Damage VS Monsters";			break;
		case SKILLID_GrandSmash:		text = "+50% Damage VS Normal";			break;
		case SKILLID_MagneticSphere:	text = "+30% Damage VS Normal";			break;
		case SKILLID_Impulsion:			text = "+30% Damage VS Normal";			break;
		case SKILLID_SparkShield:		text = "+50% Damage VS Normal";			break;

			//Pikeman
		case SKILLID_JumpingCrash:		text = "+30% Damage VS Demon";				break;

			//Archer
		case SKILLID_WindArrow:			text = "+30% Damage VS Normal";				break;
		case SKILLID_PerfectAim:		text = "+30% Damage VS Demon";				break;
		case SKILLID_BombShot:			text = "Extra Damage VS Demon";				break;

			//Assassin
		case SKILLID_Stinger:			text = "+30% Damage VS Normal & Mutant";	break;

			//Knight
		case SKILLID_GrandCross:		text = "Extra Damage Vs Undead";			break;
		case SKILLID_Piercing:			text = "+30% Damage Vs Mutant";				break;
		case SKILLID_SwordBlast:		text = "+30% Damage VS Normal & Mutant";	break;

			//Priestess
		case SKILLID_DivineLightning:	text = "+50% Damage VS Undead";				break;
		case SKILLID_ChainLightning:	text = "+100% Damage VS Undead";			break;
		case SKILLID_VigorBall:			text = "+50% Damage VS Undead";				break;

			//Atalanta

		case SKILLID_Expansion:			text = "+30% Damage VS Mutant";				break;
		case SKILLID_Farina:			text = "+20% Damage VS Normal";				break;
		case SKILLID_BiggerSpear:		text = "+30% Damage VS Normal";				break;
		case SKILLID_WindJavelin:		text = "+30% Damage VS Demons";				break;
		case SKILLID_FireJavelin:		text = "+30% Damage VS Mutant";				break;
		case SKILLID_LightningJavelin:	text = "+30% Damage VS Undead";				break;
	}

	if ( text != "" )
	{
		SKILLINFOBOX->AddString ( 0, text.c_str (), D3DCOLOR_XRGB ( 222, 0, 14 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddSpecificMonsterTypeInfo ( Skill * pcSkill )
{
	std::string text = "";

	switch ( pcSkill->iID )
	{
	case SKILLID_HolyReflection:		text = "Undead only";			break;
	case SKILLID_Extinction:			text = "Undead only";			break;
	case SKILLID_HolyIncantation:		text = "Undead only";			break;
	}

	if ( text != "" )
	{
		SKILLINFOBOX->AddString ( 0, text.c_str (), D3DCOLOR_XRGB ( 222, 0, 14 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}


BOOL SkillManager::AddSummonInfo ( Skill * pcSkill )
{
	if ( SKILLMANAGERHANDLER->IsSummonSkill ( (ESkillID)pcSkill->iID ) )
	{
		SKILLINFOBOX->AddString ( 0, "Summon Skill", D3DCOLOR_XRGB ( 224, 198, 52 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}


BOOL SkillManager::AddPassiveInfo ( Skill * pcSkill )
{
	if ( SKILLMANAGERHANDLER->IsPassiveSkill ( (ESkillID)pcSkill->iID ) )
	{
		SKILLINFOBOX->AddString ( 0, "Passive Skill", D3DCOLOR_XRGB ( 19, 140, 176 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddDebuffInfo ( Skill * pcSkill )
{
	if ( SKILLMANAGERHANDLER->IsDebuffSkill ( (ESkillID)pcSkill->iID ) )
	{
		SKILLINFOBOX->AddString ( 0, "Debuff", D3DCOLOR_XRGB ( 202, 87, 255 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddBuffInfo ( Skill * pcSkill )
{
	std::string text = "";

	switch ( pcSkill->iID )
	{
	case SKILLID_ExtremeShield:
	case SKILLID_ElectricalOverload:
	case SKILLID_PhysicalAbsorb:
	case SKILLID_Automation:
	case SKILLID_MetalArmor:
	case SKILLID_SparkShield:
	case SKILLID_Compulsion:
	case SKILLID_MagneticSphere:
	case SKILLID_Berserker:
	case SKILLID_Resilience:
	case SKILLID_VenomThorn:
		text = "Self Buff";
		break;

	case SKILLID_VirtualLife:
	case SKILLID_VirtualRegen:
	case SKILLID_Alas:
	case SKILLID_Inpes:
	case SKILLID_HolyValor:
	case SKILLID_AdventMigal:
	case SKILLID_DemonsBane:
	case SKILLID_TriumphofValhalla:
	case SKILLID_HallofValhalla:
	case SKILLID_EnchantWeapon:
	case SKILLID_ForceofNature:
		text = "Party Buff";
		break;

	case SKILLID_HolyBody:
	case SKILLID_GrandHealing:
		text = "Party Heal";
		break;

	}

	if ( text != "" )
	{
		SKILLINFOBOX->AddString ( 0, text.c_str (), D3DCOLOR_XRGB ( 83, 207, 83 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddHintInfo ( Skill * pcSkill )
{
	std::string text = "";

	switch ( pcSkill->iID )
	{
	case SKILLID_GrandSmash:		text = "Attack rating boost restricted to hammer and claw";	break;
	case SKILLID_ShadowBlink:		text = "Needs Blind Active";								break;
	case SKILLID_MetalArmor:		text = "Requires Mech Spec Armor";							break;
	case SKILLID_ChargingStrike:	text = "This skill can be charged with RMB";				break;
	case SKILLID_PhoenixShot:		text = "This skill can be charged with RMB";				break;
	}

	//is for bubble skill icon (iCheckTime will be non-zero)

	switch ( pcSkill->iID )
	{
	case SKILLID_RecallHestian:
	case SKILLID_MetalGolem:
	case SKILLID_RecallWolverine:
	case SKILLID_FireElemental:
	case SKILLID_IceElemental:
	case SKILLID_SummonTiger:
	{
		if ( pcSkill->iCheckTime )
		{
			text = "Double left click on this skill icon to toggle pet mode";					break;
		}
		else
		{
			text = "Pet mode can be toggled. Use skill icon at top left corner when active";	break;
		}
	}
	}

	if ( text != "" )
	{
		SKILLINFOBOX->AddString ( 0, text.c_str (), D3DCOLOR_XRGB ( 191, 191, 191 ) );
		SKILLINFOBOX->AddString ( 1, "" );

		return TRUE;
	}

	return FALSE;
}


BOOL SkillManager::AddSkillInfoToBox_FS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based, value;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//Fighter
		//t1
	case SKILLID_MeleeMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Fighter_MeleeMastery_WPAtkPowAddPercent_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Resilience:
	{
		AddSkillBoxInfoDual ( "Add Health:", "%d + %d%%", Fighter_BoostHealth_HPBoost, Fighter_BoostHealth_HPBoostPercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Potion Boost:", "%d%%", Fighter_BoostHealth_HPPotionBoost, iSkillLevelZeroBased );

		break;
	}


	case SKILLID_Raving:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_Raving_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of Attacks:", "%d", Fighter_Raving_Numofhits, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Cost:", "%d", Fighter_Raving_HPCost_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DoubleImpact:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_DoubleImpact_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_DoubleImpact_AddAtkRtg, iSkillLevelZeroBased );
		break;
	}

	//t2
	case SKILLID_TripleImpact:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_TripleImpact_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of attacks:", "%d", Fighter_TripleImpact_Numofhits, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill( ESkillID::SKILLID_DoubleImpact );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Double Impact Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_DoubleImpact_DMGBoost, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_DoubleImpact_AddAtkRtg, chainLevel1Based - 1);
		}

		break;
	}

	case SKILLID_BrutalSwing:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_BrutalSwing_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Critical Chance:", "%d%%", Fighter_BrutalSwing_AddCritical, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical Damge Boost:", "%d%%", Fighter_BrutalSwing_CriticalDmgBoost_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Cost:", "%d", Fighter_BrutalSwing_HPCost_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_WarCry:
	{
		BOOL bHalf = PLAYERDATA->iClass != ECharacterClass::CHARACTERCLASS_Fighter;

		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Stun Damage (% of AP):", "%d%%", Fighter_WarCry_StunAtkPowPerc_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Chance:", "%d%%", Fighter_WarCry_StunChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Duration:", "%ds", Fighter_WarCry_StunTime, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Fighter_WarCry_Radius, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( FormatString ( "Party Buff" ), TEXTCOLOR_SKILLCHAIN );
		AddSkillBoxInfo ( "HP Restore:", "%d%%", Fighter_WarCry_HPRestore_New, iSkillLevelZeroBased, TRUE, bHalf ? 0.5f : 1.0f );
		AddSkillBoxInfo ( "SP Restore:", "%d%%", Fighter_WarCry_SPRestore_New, iSkillLevelZeroBased, TRUE, bHalf ? 0.5f : 1.0f );
		//AddSkillBoxInfo ( "HP Regen:", "%d", Fighter_WarCry_HPRegen_New, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "SP Regen:", "%d", Fighter_WarCry_SPRegen_New, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Regen Duration:", "%ds", Fighter_WarCry_RegenDuration_Sec, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_RageofZecram:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_RageofZecram_Damage, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Fighter_RageofZecram_AttackRadius_New, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Fire Attack (AoE)", TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Fighter_RageofZecram_BurningDamagePercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Fighter_RageofZecram_BurningChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Fighter_RageofZecram_BurningDurationSec_New, iSkillLevelZeroBased );
		break;
	}

	//t3
	case SKILLID_DemonsBane:
	{
		AddSkillBoxInfo ( "Damage Boost (Demons):", "%d%%", Fighter_DemonBuff_DamageBoostVsDemon_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Fighter ? 1.0f : 0.5f );

		AddSkillBoxInfoDuration ( Fighter_DemonBuff_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_AvengingCrash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_AvengingCrash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_AvengingCrash_AddAtkRtg, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Cost:", "%d", Fighter_AvengingCrash_HPCost_New, iSkillLevelZeroBased );


		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_BrutalSwing );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Brutal Swing Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Critical Chance Boost:", "%d%%", Fighter_BrutalSwing_AddCritical, chainLevel1Based - 1 );
		}


		break;
	}

	case SKILLID_ConcentrationSA:
	{
		AddSkillBoxInfo ( "Attack Speed Boost:", "%d%%", Fighter_ConcentrationSA_ATKSpdBoost_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_ConcentrationSA_ATKRtgBoost_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Fighter_ConcentrationSA_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_BoneCrash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_BoneCrash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Boost (Demons):", "%d%%", Fighter_BoneCrash_BonusDMGDemons, iSkillLevelZeroBased );


		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_DoubleImpact );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Double Impact Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Attack Power Boost:", "%d%%", Fighter_DoubleImpact_DMGBoost, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_DoubleImpact_AddAtkRtg, chainLevel1Based - 1);
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_TripleImpact );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle( FormatString( "Triple Impact Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Attack Power Boost:", "%d%%", Fighter_TripleImpact_DMGBoost, chainLevel1Based - 1 );
		}
		break;
	}

	//t4
	case SKILLID_Destroyer:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_Destroyer_DMGBoost, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Critical Chance Boost:", "%d%%", Fighter_Destroyer_AddCritical, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Cost:", "%d", Fighter_Destroyer_HPCost_New, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_BrutalSwing );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Brutal Swing Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Critical Chance Boost:", "%d%%", Fighter_BrutalSwing_AddCritical, chainLevel1Based - 1 );
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_AvengingCrash );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Avenging Crash Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_AvengingCrash_DMGBoost, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Fighter_AvengingCrash_AddAtkRtg, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_Berserker:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_Berserker_AddATKPow_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Decrease Absorption:", "%d%%", Fighter_Berserker_DecreaseAbs, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage To HP:", "%d%%", Fighter_Berserker_LifeLeechPercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Fighter_Berserker_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Raving);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Raving Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Attack Power Boost:", "%d%%", Fighter_Raving_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_CycloneStrike:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Fighter_CycloneStrike_DMGBoost, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Max Targets:", "%d", Fighter_CycloneStrike_Maxhits, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Fighter_CycloneStrike_Range, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_RageofZecram );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Rage of Zecram Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
			AddChainSkillBoxInfo( "Burn Damage (% of AP):", "%d%%", Fighter_RageofZecram_BurningDamagePercent_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo( "Burn Chance:", "%d%%", Fighter_RageofZecram_BurningChance_New, chainLevel1Based - 1);
			AddChainSkillBoxInfo( "Burn Duration:", "%ds", Fighter_RageofZecram_BurningDurationSec_New, chainLevel1Based - 1 );
		}

		break;
	}


	case SKILLID_Recuperation:
	{
		AddSkillBoxInfo ( "HP Regen:", "+%d", Fighter_Recuperation_HPRegen_New, iSkillLevelZeroBased );
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}



BOOL SkillManager::AddSkillInfoToBox_AS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{

		//Archer
		//t1
	case SKILLID_ScoutHawk:
	{
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Archer_ScoutHawk_AddAtkRtg_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Archer_ScoutHawk_Rotations, iSkillLevelZeroBased ); //note - %d is multiplied by ~2.7 to match ASM duration
		break;
	}

	case SKILLID_ShootingMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Archer_ShootingMastery_AddWeaponDamagePercent, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_WindArrow:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_WindArrow_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Attack Speed:", "%d", Archer_WindArrow_AddATKSpeed, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_PerfectAim:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_PerfectAim_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Archer_PerfectAim_AddAtkRtg, iSkillLevelZeroBased );
		break;
	}

	//t2
	case SKILLID_DionsEye:
	{
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Archer_DionsEye_AddAtkRtg, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Falcon:
	{
		AddSkillBoxInfoDual ( "Falcon Attack Power:", "%d + %d%%", Archer_Falcon_Damage, Archer_Falcon_DamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Archer_Falcon_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_ArrowofRage:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Archer_ArrowofRage_DMGBoost, Archer_ArrowofRage_DMGFlat, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Archer_ArrowofRage_Area, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Avalanche:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_Avalanche_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Attack Speed:", "%d", Archer_Avalanche_AddATKSpeed, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of Arrows:", "%d", Archer_Avalanche_NumberofArrows, iSkillLevelZeroBased );
		break;
	}

	//t3
	case SKILLID_ElementalShot:
	{
		AddSkillElement ( EElementID::Fire );
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfoDual ( "Fire Damage:", "%d%% + %d", Archer_ElementalShot_FireDamage, Archer_ElementalShot_FireDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfoDual ( "Lightning Damage:", "%d%% + %d", Archer_ElementalShot_LightningDamage, Archer_ElementalShot_LightningDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Lightning Range:", "%d", Archer_ElementalShot_LightningRange_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_GoldenFalcon:
	{
		AddSkillBoxInfo ( "Falcon Damage Boost:", "%d%%", Archer_GoldenFalcon_FalconDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Health Regen:", "%.0f", Archer_GoldenFalcon_HPRegen, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Archer_GoldenFalcon_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Falcon );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Falcon Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Base Attack Power:", "%d + %d%%", Archer_Falcon_Damage, Archer_Falcon_DamagePercent, chainLevel1Based - 1);
		}

		break;
	}

	case SKILLID_BombShot:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_BombShot_Damage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Boost (Demons):", "%d%%", Archer_BombShot_DMGDemons, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Archer_BombShot_Area, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Perforation:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_Perforation_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Add Range:", "%d", Archer_Perforation_AddShotRange, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Width:", "%d", Archer_Perforation_AttackWidth_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical Chance Boost:", "%d%%", Archer_Perforation_AddedCritical_New, iSkillLevelZeroBased ); //- AOE Cannot crit
		break;
	}

	//t4
	case SKILLID_RecallWolverine:
	{
		AddSkillBoxInfoDual ( "Attack Power:", "%d - %d", Archer_Wolverine_ATKPowMin, Archer_Wolverine_ATKPowMax, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating:", "%d", Archer_Wolverine_AtkRating, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Defense:", "%d", Archer_Wolverine_Defense, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Health:", "%d", Archer_Wolverine_HP, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Archer_Wolverine_Duration, iSkillLevelZeroBased );

		if ( !SKILLINFOBOX->IsViewingSkillTimer() )
		{
			AddSkillBoxGroupTitle ( "Pet default setting", D3DCOLOR_XRGB ( 238, 210, 151 ) );
			AddSkillBoxInfo ( "Mode:", FormatString ( "%s", SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive ? "Aggressive" : "Passive" ) );
		}
		else
		{
			AddSkillBoxGroupTitle( "Pet setting", D3DCOLOR_XRGB( 238, 210, 151 ) );
			AddSkillBoxInfo( "Mode:", FormatString( "%s", SKILLMANAGERHANDLER->IsAgressive( SKILLID_RecallWolverine ) ? "Aggressive" : "Passive" ) );
		}

		break;
	}

	case SKILLID_EvasionMastery:
	{
		AddSkillBoxInfo ( "Add Evasion", "+%d%%", Archer_EvasionMastery_IncreaseEvasion_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_PhoenixShot:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_PhoenixShot_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Archer_PhoenixShot_AttackRange_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Width:", "%d", Archer_PhoenixShot_AttackWidth_New, iSkillLevelZeroBased );

		AddSkillBoxEmptyLine();
		AddSkillBoxInfo("Burn Damage (% of AP):", "%d%%", Archer_PhoenixShot_BurningPercentOfAP_New, iSkillLevelZeroBased);
		AddSkillBoxInfo("Burn Chance:", "%d%%", Archer_PhoenixShot_BurningChance_New, iSkillLevelZeroBased);
		AddSkillBoxInfo("Burn Duration: ", "%ds", Archer_PhoenixShot_BurningDurationSec_New, iSkillLevelZeroBased);

		AddSkillBoxGroupTitle ( "Charging mode", D3DCOLOR_XRGB ( 238, 210, 151 ) );
		AddSkillBoxInfo ( "Charging Speed:", "%d", Archer_PhoenixShot_ChargingSpeed, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Charged Damage Boost:", "%d%%", Archer_PhoenixShot_ChargedDMGBoost_New, iSkillLevelZeroBased );


		break;
	}

	case SKILLID_ForceofNature:
	{
		//half for non-archers
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Archer_ForceofNature_AddATKPow_New, iSkillLevelZeroBased, TRUE,
			UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer ? 1.0f : 0.5f );

		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Archer_ForceofNature_AddAtkRtg, iSkillLevelZeroBased, TRUE,
			UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Archer ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration ( Archer_ForceofNature_Duration, iSkillLevelZeroBased );
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_MS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{

		//t1
	case SKILLID_ExtremeShield:
	{
		AddSkillBoxInfo ( "Add Block Rating:", "+%d%%", Mech_ExtremeShield_AddBlock_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_ExtremeShield_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_MechanicBomb:
	{
		AddSkillElement ( EElementID::Fire );

		AddSkillBoxInfoDual ( "Damage:", "%d%% + %d", Mech_MechanicBomb_DamagePercent, Mech_MechanicBomb_Damage, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Mech_MechanicBomb_Area, iSkillLevelZeroBased );
		AddSkillBoxInfo("Burn Damage (% of AP):", "%d%%", Mech_MechanicBomb_BurningDamagePercent, iSkillLevelZeroBased);
		AddSkillBoxInfo("Burn Chance:", "%d%%", Mech_MechanicBomb_BurningChance, iSkillLevelZeroBased);
		AddSkillBoxInfo("Burn Duration: ", "%ds", Mech_MechanicBomb_BurningDurationSec, iSkillLevelZeroBased);
		break;
	}

	case SKILLID_ElectricalOverload:
	{
		AddSkillBoxInfo ( "Attack Speed Boost:", "%d%%", Mech_ElectricalOverload_AttackSpeed, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_ElectricalOverload_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_PhysicalAbsorb:
	{
		AddSkillBoxInfo ( "Absorption Boost:", "%d%%", Mech_PhysicalAbsorption_ABSBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%d%%", Mech_PhysicalAbsorption_Duration, iSkillLevelZeroBased );
		break;
	}

	//t2
	case SKILLID_GreatSmash:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Mech_GreatSmash_AddDamagePercent, Mech_GreatSmash_AddDamage, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Maximize:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Mech_Maximize_AddATKPow, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Mech_Maximize_AtkRtgBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Speed Boost:", "%d%%", Mech_Maximize_ATKSPDBoost_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_Maximize_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Automation:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Mech_Automation_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Attack Speed:", "%d", Mech_Automation_AddATKSpeed, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Mana Drain:", "%d", Mech_Automation_MPDrain_New, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Stamina Drain:", "%d", Mech_Automation_SPDrain_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_Automation_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Spark:
	{
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Mech_Spark_DMGBoostPercent, Mech_Spark_DMGBoostFlat, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Mech_Spark_Area, iSkillLevelZeroBased );
		break;
	}

	//t3
	case SKILLID_MetalArmor:
	{
		AddSkillBoxInfo ( "Add Armor Defense:", "%d%%", Mech_MetalArmor_AddDefense, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_MetalArmor_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_PhysicalAbsorb );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Physical Absorption Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Absorption Boost:", "%d%%", Mech_PhysicalAbsorption_ABSBoost, chainLevel1Based - 1);
		}

		break;
	}

	case SKILLID_GrandSmash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Mech_GrandSmash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Mech_GrandSmash_AtkRtgBoost, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_GreatSmash );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Great Smash Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Damage Boost:", "%d%% + %d", Mech_GreatSmash_AddDamagePercent, Mech_GreatSmash_AddDamage, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_MechanicWeaponMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Mech_MechanicWeaponMastery_AddWeaponDamagePercent, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_SparkShield:
	{
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfo ( "Attack Power:", "%d", Mech_SparkShield_SparkDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Defense Boost:", "%d", Mech_SparkShield_AddDefense, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_SparkShield_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Spark );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Spark Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Damage Boost:", "%d%% + %d", Mech_Spark_DMGBoostPercent, Mech_Spark_DMGBoostFlat, chainLevel1Based - 1 );
		}

		break;
	}

	//t4
	case SKILLID_Impulsion:
	{
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Mech_Impulsion_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Targets:", "%d", Mech_Impulsion_MaxVolts, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Mech_Impulsion_Area, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Compulsion:
	{
		AddSkillBoxInfoDual ( "Absorption Boost:", "%d%% + %d", Mech_Compulsion_AddAbsorbPercent_New, Mech_Compulsion_ExtraAbsorb_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Mech_Compulsion_Range, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_Compulsion_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_MagneticSphere:
	{
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfo ( "Crystal Damage Boost:", "%d%%", Mech_MagneticSphere_SpheresDMG, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Delay:", "%d", Mech_MagneticSphere_AtkDelay, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Mech_MagneticSphere_Range, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_MagneticSphere_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_MetalGolem:
	{
		AddSkillBoxInfoDual ( "Attack Power:", "%d - %d", Mech_MetalGolem_ATKPowMin, Mech_MetalGolem_ATKPowMax, iSkillLevelZeroBased ); // Something is causing a crash here
		AddSkillBoxInfo ( "Attack Rating:", "%d", Mech_MetalGolem_AtkRating, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Defense:", "%d", Mech_MetalGolem_Defense, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Health:", "%d", Mech_MetalGolem_HP, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Mech_MetalGolem_Duration_New, iSkillLevelZeroBased );

		if ( !SKILLINFOBOX->IsViewingSkillTimer() )
		{
			AddSkillBoxGroupTitle ( "Pet default setting", D3DCOLOR_XRGB ( 238, 210, 151 ) );
			AddSkillBoxInfo ( "Mode:", FormatString ( "%s", SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive ? "Aggressive" : "Passive" ) );
		}
		else
		{
			AddSkillBoxGroupTitle( "Pet setting", D3DCOLOR_XRGB( 238, 210, 151 ) );
			AddSkillBoxInfo( "Mode:", FormatString( "%s", SKILLMANAGERHANDLER->IsAgressive( SKILLID_MetalGolem ) ? "Aggressive" : "Passive" ) );
		}
		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_PS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{


		//T1
	case SKILLID_PikeWind:
	{
		AddSkillBoxInfoDual ( "Damage:", "%d - %d", Pikeman_PikeWind_ATKPowMin, Pikeman_PikeWind_ATKPowMax, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Pushback Range:", "%d", Pikeman_PikeWind_Pushback, iSkillLevelZeroBased );

		//chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_GroundPike );
		//if ( chainLevel1Based > 0 )
		//{
		//	AddSkillBoxGroupTitle ( FormatString ( "Ground Pike Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Ice );
		//	AddChainSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Pikeman_GroundPike_FreezeDamage, chainLevel1Based - 1 );
		//	AddChainSkillBoxInfo ( "Freeze Chance:", "%d%%", Pikeman_GroundPike_FreezeChance, chainLevel1Based - 1 );
		//	AddChainSkillBoxInfo ( "Freeze Duration:", "%ds", Pikeman_GroundPike_FreezeDuration, chainLevel1Based - 1 );
		//	AddChainSkillBoxInfo ( "Speed Reduction:", "%d%%", Pikeman_GroundPike_SpeedMonDown_New, chainLevel1Based - 1 );
		//}

		break;
	}

	case SKILLID_PolearmMastery:
	{
		AddSkillBoxInfoRange ( "Add Range:", "%d", Pikeman_PolearmMastery_AddRange_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_PreciseHit:
	{
		AddSkillBoxInfo ( "Add Attack Power:", "%d", Pikeman_PreciseHit_ATKPow_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Pikeman_PreciseHit_AddAttackRating_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_JumpingCrash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_JumpingCrash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Pikeman_JumpingCrash_WPAtkRtgBoost, iSkillLevelZeroBased );
		break;
	}

	//T2
	case SKILLID_GroundPike:
	{
		AddSkillElement ( EElementID::Ice );

		AddSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Pikeman_GroundPike_FreezeDamage, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Freeze Chance:", "%d%%", Pikeman_GroundPike_FreezeChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Duration:", "%ds", Pikeman_GroundPike_FreezeDuration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Pikeman_GroundPike_SpeedMonDown_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Pikeman_GroundPike_Range, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Tornado:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_Tornado_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Pikeman_Tornado_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_WeaponBlockMastery:
	{
		AddSkillBoxInfo ( "Add Block Rating:", "+%d%%", Pikeman_WeaponBlockMastery_WPBlockBoost_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Expansion:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_Expansion_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Weapon Size:", "%d%%", Pikeman_Expansion_AddSize, iSkillLevelZeroBased );
		AddSkillBoxInfoRange( "Add Range:", "%d", Pikeman_Expansion_Range_New, iSkillLevelZeroBased );
		break;
	}

	//T3
	case SKILLID_VenomSpear:
	{
		AddSkillElement ( EElementID::Poison );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_VenomSpear_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Poison Damage (% of AP):", "%d%%", Pikeman_VenomSpear_PoisonDamagePercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Poison Chance:", "%d%%", Pikeman_VenomSpear_PoisonChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add stack:", "%d", Pikeman_VenomSpear_PoisonNumStack_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of targets:", "%d", Pikeman_VenomSpear_Numberofspears, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Poison Duration:", "%d seconds", Pikeman_VenomSpear_PoisonDurationSec_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Pikeman_VenomSpear_Range, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Vanish:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_Vanish_AfterDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Monster Vision:", "%d%%", Pikeman_Vanish_MonsterSight, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Pikeman_Vanish_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_CriticalMastery:
	{
		AddSkillBoxInfo ( "Add Critical Chance:", "%d%%", Pikeman_CriticalMastery_WPCritBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_ChainLance:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_ChainLance_DMGBoost, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_PreciseHit );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Precise Hit Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Add Attack Power:", "%d", Pikeman_PreciseHit_ATKPow_New, chainLevel1Based - 1);
			AddChainSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Pikeman_PreciseHit_AddAttackRating_New, chainLevel1Based - 1 );
		}

		break;
	}

	//T4
	case SKILLID_AssassinsEye:
	{
		AddSkillBoxInfo ( "Add Critical Damage:", "%d%%", Pikeman_AssassinsEye_AddCriticalDmg, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Critical Chance:", "%d%%", Pikeman_AssassinsEye_AddCriticalChance, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Pikeman_AssassinsEye_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_ChargingStrike:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_ChargingStrike_DMGBoost, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Charging mode", D3DCOLOR_XRGB ( 238, 210, 151 ) );
		AddSkillBoxInfo ( "Charging Speed:", "%d", Pikeman_ChargingStrike_RaiseSpeed, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Charged Damage Boost:", "%d%%", Pikeman_ChargingStrike_ChargedDMGBst, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Expansion );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Expansion Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_Expansion_DMGBoost, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Weapon Size:", "%d%%", Pikeman_Expansion_AddSize, chainLevel1Based - 1 );
			AddChainSkillBoxInfoRange( "Add Range:", "%d", Pikeman_Expansion_Range_New, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_Vague:
	{
		AddSkillBoxInfo ( "Add Evasion:", "+%d%%", Pikeman_Vague_AddEvasion_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Pikeman_Vague_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_ShadowMaster:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Pikeman_ShadowMaster_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of attacks:", "%d", Pikeman_ShadowMaster_Numofhits, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical chance:", "100%" );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_PreciseHit);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Precise Hit Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Add Attack Power:", "%d", Pikeman_PreciseHit_ATKPow_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Pikeman_PreciseHit_AddAttackRating_New, chainLevel1Based - 1 );
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_ChainLance );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Chain Lance Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo( "Attack Power Boost:", "%d%%", Pikeman_ChainLance_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_ASS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//Assassin
		//t1
	case SKILLID_Stinger:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_Stinger_DMGBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_RunningHit:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_RunningHit_DMGBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DualWieldMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Assassin_DualWieldMastery_WPAtkPowBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Weapon Attack Rating Boost:", "%d%%", Assassin_DualWieldMastery_WPAtkRtgBoost_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Wisp:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfoRange ( "Range:", "%d", Assassin_Wisp_Range, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Curse", TEXTCOLOR_SKILLCHAIN );
		AddSkillBoxInfo ( "Apply Chance:", "%d%%", Assassin_Wisp_Curse_Chance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Assassin_Wisp_Curse_PercentOfAP_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Assassin_Wisp_Curse_Duration_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Reduction:", "%d%%", Assassin_Wisp_Curse_DamageReduction, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Assassin_Wisp_Curse_SpeedReduction_New, iSkillLevelZeroBased );
		//AddSkillBoxInfo("Duration:", "15", iSkillLevelZeroBased); // UNFINISHED Duration not hooked.
		break;
	}

	//t2
	case SKILLID_VenomThorn:
	{
		AddSkillBoxInfo ( "Duration:", "%d", Assassin_VenomThorn_Duration_New, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Poison" , TEXTCOLOR_SKILLCHAIN, EElementID::Poison );
		AddSkillBoxInfo ( "Apply Chance:", "%d%%", Assassin_VenomThorn_PoisonChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Assassin_VenomThorn_PoisonDamagePercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Assassin_VenomThorn_PoisonDurationSec_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Stack:", "%d", Assassin_VenomThorn_PoisonNumStack_New, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_Alas:
	{
		AddSkillBoxInfo ( "Add Evasion:", "+%d%%", Assassin_Alas_AddEvasion, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Assassin_Alas_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_SoulShock:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Stun Damage (% of AP):", "%d%%", Assassin_SoulShock_StunAtkPowPerc_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Chance:", "%d%%",  Assassin_SoulShock_StunChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Duration:", "%ds", Assassin_SoulShock_StunTime, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Assassin_SoulShock_Radius, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DodgeMastery:
	{
		AddSkillBoxInfo ( "Add Evasive:", "+%d%%", Assassin_DodgeMastery_AddEvasive_New, iSkillLevelZeroBased );
		break;
	}

	//t3
	case SKILLID_SoreBlade:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_SoreBlade_DMGBoost, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_RunningHit);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Running Hit Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_RunningHit_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_GustSlash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_GustSlash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Assassin_GustSlash_Area, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Stinger);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Stinger Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_Stinger_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_Inpes:
	{
		AddSkillBoxInfo ( "Movement Speed Boost:", "%d%%", Assassin_Inpes_MovementSpeed_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Assassin_Inpes_Duration, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( FormatString ( "Party Buff" ), TEXTCOLOR_SKILLCHAIN );
		AddSkillBoxInfo ( "Movement Speed Boost:", "%d%%", Assassin_Inpes_MovementSpeed_New, iSkillLevelZeroBased, TRUE, 0.5f );
		break;
	}

	case SKILLID_Blind:
	{
		//AddSkillBoxInfo ( "After Damage:", "%d%%", Assassin_Blind_AfterDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Monster Sight:", "%d", Assassin_Blind_MonsterSight, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Assassin_Blind_Duration, iSkillLevelZeroBased );
		break;
	}

	//t4
	case SKILLID_Cripple:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_Cripple_DMGBoost, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Poison", TEXTCOLOR_SKILLCHAIN, EElementID::Poison);
		AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Assassin_Cripple_PoisonPercentOfAP_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Apply Chance:", "%d%%", Assassin_Cripple_PoisonChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Assassin_Cripple_PoisonDurationSec, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Stack:", "%d", Assassin_Cripple_PoisonNumStack_New, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Wisp);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Wisp Curse Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Organic );
			AddChainSkillBoxInfo ( "Apply Chance:", "%d%%", Assassin_Wisp_Curse_Chance_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Damage (% of AP):", "%d%%", Assassin_Wisp_Curse_PercentOfAP_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Duration:", "%ds", Assassin_Wisp_Curse_Duration_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Damage Reduction:", "%d%%", Assassin_Wisp_Curse_DamageReduction,  chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Speed Reduction:", "%d%%", Assassin_Wisp_Curse_SpeedReduction_New,  chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_AssassinMastery:
	{
		AddSkillBoxInfo ( "Critical Boost", "%d%%", Assassin_AssassinMastery_WPCritBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Speed Boost", "%d%%", Assassin_AssassinMastery_AtkSpeedBoost_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Polluted:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_Polluted_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Assassin_Polluted_Range, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Poison", TEXTCOLOR_SKILLCHAIN, EElementID::Poison );
		AddSkillBoxInfo ( "Apply Chance:", "%d%%", Assassin_Polluted_PoisonChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Assassin_Polluted_PoisonPercentOfAP, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Stack:", "%d", Assassin_Polluted_PoisonNumStack_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Assassin_Polluted_PoisonDurationSec, iSkillLevelZeroBased );
		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_VenomThorn);

		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Venom Thorn Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Poison );
			AddChainSkillBoxInfo ( "Add Poison Damage (% of AP):", "%d%%", Assassin_VenomThorn_PoisonDamagePercent_New, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Add Poison Chance:", "+%d%%", Assassin_VenomThorn_PoisonChance_New, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_NinjaShadow:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_NinjaShadow_DMGBoost, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_RunningHit);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Running Hit Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_RunningHit_DMGBoost, chainLevel1Based - 1 );
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_SoreBlade);
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Sore Blade Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Assassin_SoreBlade_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}
	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_KS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//t1
	case SKILLID_SwordBlast:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d + %d%%", Knight_SwordBlast_DMGBoostFlat, Knight_SwordBlast_DMGBoostPercent, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Knight_SwordBlast_AttackRange, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Width:", "%d", Knight_SwordBlast_AttackWidth_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_HolyBody:
	{
		AddSkillBoxInfo ( "Damage To Heal:", "%d%%", Knight_HolyBody_DmgToHealConversion_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Heal Radius:", "%d", Knight_HolyBody_HealRadius_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_HolyBody_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_PhysicalTraining:
	{
		AddSkillBoxInfo ( "Add Total Stamina:", "%d%%", Knight_PhysicalTraining_AddStamina, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DoubleCrash:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_DoubleCrash_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical Boost:", "%d%%", Knight_DoubleCrash_AddCritical, iSkillLevelZeroBased );
		break;
	}

	//t2
	case SKILLID_HolyValor:
	{
		// half for non-ks
		AddSkillBoxInfo ( "Damage Boost VS Undead:", "%d%%", Knight_HolyValor_DamageBoostVsUndead, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Knight ? 1.0f : 0.5f);

		AddSkillBoxInfoRange ( "Buff Radius:", "%d", Knight_HolyValor_BuffRadius_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_HolyValor_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Brandish:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_Brandish_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Knight_Brandish_Area, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Piercing:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_Piercing_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Pierce Range:", "%d", Knight_Piercing_PierceRange_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Pierce Width:", "%d", Knight_Piercing_PierceWidth_New, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_SwordBlast );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Sword Blast Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Attack Power Boost:", "%d + %d%%", Knight_SwordBlast_DMGBoostFlat, Knight_SwordBlast_DMGBoostPercent, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_DrasticSpirit:
	{
		AddSkillBoxInfo( "Undead Damage Reduction:", "%d%%", Knight_DrasticSpirit_UndeadDmgReduction_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Defense Boost:", "%d%%", Knight_DrasticSpirit_AddDefense, iSkillLevelZeroBased );

		AddSkillBoxInfoDuration ( Knight_DrasticSpirit_Duration, iSkillLevelZeroBased );

		break;
	}

	//t3
	case SKILLID_SwordMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Knight_SwordMastery_AddWeaponDamagePercent, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DivineShield:
	{
		AddSkillBoxInfo ( "Add Block Rating:", "+%d%%", Knight_DivineShield_AddBlock_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Convert Life:", "%d%%", Knight_DivineShield_ConvertLife, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_DivineShield_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_HolyIncantation:
	{
		AddSkillBoxInfo ( "Success Rate:", "%d%%", Knight_HolyIncantation_SuccessRate, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Life Boost:", "%d%%", Knight_HolyIncantation_LifeIncrease, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Max HP:", "%d", Knight_HolyIncantation_MaxHP_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDual ( "Max Attack Power:", "%d - %d", Knight_HolyIncantation_MaxDMGMin_New, Knight_HolyIncantation_MaxDMGMax_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Absorption Boost:", "%d%%", Knight_HolyIncantation_MaxAbs_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_HolyIncantation_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_GrandCross:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_GrandCross_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Boost VS Undead:", "%d%%", Knight_GrandCross_UndeadDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Knight_GrandCross_AtkRtgBoost, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_DoubleCrash );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Double Crash Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_DoubleCrash_DMGBoost, chainLevel1Based - 1);
		}

		break;
	}

	//t4
	case SKILLID_SwordofJustice:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_SwordofJustice_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range", "%d", Knight_SwordofJustice_Area, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Width", "80" );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Brandish );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Brandish Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_Brandish_DMGBoost, chainLevel1Based - 1  );
		}

		break;
	}

	case SKILLID_GodlyShield:
	{
		AddSkillBoxInfo ( "Shield Absorption Boost:", "%d%%", Knight_GodlyShield_ShieldABSBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_GodlyShield_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_DivineShield );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Divine Shield Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Add Block Rating:", "+%d%%", Knight_DivineShield_AddBlock_New, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_GodsBlessing:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_GodsBlessing_AddATKPowPercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Knight_GodsBlessing_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DivinePiercing:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Knight_DivinePiercing_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical Boost:", "%d%%", Knight_DivinePiercing_AddCritical, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of Attacks", "%d", Knight_DivinePiercing_Numberofattacks, iSkillLevelZeroBased );
		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_ATA ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//t1
	case SKILLID_ShieldStrike:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfoDual ( "Add Attack Power:", "%d - %d", Atalanta_ShieldStrike_ATKPowMin, Atalanta_ShieldStrike_ATKPowMax, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Damage:", "%d%%", Atalanta_ShieldStrike_StunDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Chance:", "%d%%", Atalanta_ShieldStrike_StunChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Duration: ", "%ds", Atalanta_ShieldStrike_StunDurationSec, iSkillLevelZeroBased);
		break;
	}

	case SKILLID_Farina:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Atalanta_Farina_DMGBoostPercent_New, Atalanta_Farina_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Attack Speed:", "%d", Atalanta_Farina_AddATKSpeed, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Atalanta_Farina_AddAtkRtg, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_ThrowingMastery:
	{
		AddSkillBoxInfo ( "Add Weapon Damage to AP:", "%d%%", Atalanta_ThrowingMastery_DMGBoostPercent_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_BiggerSpear:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_BiggerSpear_DMGBoost, iSkillLevelZeroBased );
		break;
	}
	//t2
	case SKILLID_Windy:
	{
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Atalanta_Windy_AddAttackRating_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Add Range:", "%d", Atalanta_Windy_Range_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Atalanta_Windy_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_SoulSucker:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Absorb Life:", "%d%%", Atalanta_SoulSucker_LifeAbsorb, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Max HP Absorb:", "%d", Atalanta_SoulSucker_LifeAbsorbCap_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Atalanta_SoulSucker_Range_New, iSkillLevelZeroBased );
		break;
	}


	case SKILLID_WindJavelin:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_WindJavelin_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Atalanta_WindJavelin_AtkRtgBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_FireJavelin:
	{
		AddSkillElement ( EElementID::Fire );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_FireJavelin_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Atalanta_FireJavelin_BurningDamagePercent, iSkillLevelZeroBased);
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Atalanta_FireJavelin_BurningChance, iSkillLevelZeroBased);
		AddSkillBoxInfo ( "Burn Duration: ", "%ds", Atalanta_FireJavelin_BurningDurationSec, iSkillLevelZeroBased);
		break;
	}

	//t3
	case SKILLID_SplitJavelin:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_SplitJavelin_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Atalanta_SplitJavelin_AtkRtgBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of attacks", "%d", Atalanta_SplitJavelin_NumberOfAttacks, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_TriumphofValhalla:
	{
		//half for non-ata
		AddSkillBoxInfo ( "Max Attack Power Boost:", "%d%%", Atalanta_TriumphOfValhalla_AddMaxAtkPowPerc_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Atalanta ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration ( Atalanta_TriumphOfValhalla_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_LightningJavelin:
	{
		AddSkillElement ( EElementID::Lightning );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_LightningJavelin_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Atalanta_LightningJavelin_Radius_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Chance:", "%d%%", Atalanta_LightningJavelin_StunChance, iSkillLevelZeroBased);
		AddSkillBoxInfo ( "Stun Duration: ", "%ds", Atalanta_LightningJavelin_StunDurationSec, iSkillLevelZeroBased);
		break;
	}

	case SKILLID_StormJavelin:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_StormJavelin_DMGBoost, iSkillLevelZeroBased );

		const int windJavelinLevel = GetLevelSkill ( ESkillID::SKILLID_WindJavelin);
		if ( windJavelinLevel > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Wind Javelin Chain (Lv. %d)", windJavelinLevel), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Damage Boost:", "%d%%", ESkillArrayPointer::Atalanta_WindJavelin_DMGBoost, windJavelinLevel - 1 );
		}

		const int lightningJavelinLevel = GetLevelSkill ( ESkillID::SKILLID_LightningJavelin );
		if ( lightningJavelinLevel > 0 )
		{
			AddSkillBoxGroupTitle( FormatString("Lightning Javelin Chain (Lv. %d)", lightningJavelinLevel), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoRange( "Attack Width Boost:", "+ %d", ESkillArrayPointer::Atalanta_LightningJavelin_Radius_New, lightningJavelinLevel - 1 );
		}

		const int fireJavelinLevel = GetLevelSkill( ESkillID::SKILLID_FireJavelin );
		if ( fireJavelinLevel )
		{
			AddSkillBoxGroupTitle(FormatString("FireJavelin Javelin Chain (Lv. %d)", fireJavelinLevel), TEXTCOLOR_SKILLCHAIN);
			AddChainSkillBoxInfo( "Burn Damage (% of AP):", "%d%%", ESkillArrayPointer::Atalanta_FireJavelin_BurningDamagePercent, fireJavelinLevel - 1);
			AddChainSkillBoxInfo( "Burn Chance:", "%d%%", ESkillArrayPointer::Atalanta_FireJavelin_BurningChance, fireJavelinLevel - 1);
			AddChainSkillBoxInfo( "Burn Duration: ", "%ds", ESkillArrayPointer::Atalanta_FireJavelin_BurningDurationSec, fireJavelinLevel - 1);
		}

		break;
	}

	//t4
	case SKILLID_HallofValhalla:
	{
		//half for non-ata
		AddSkillBoxInfo ( "Increase Block:", "+%d%%", Atalanta_HallOfValhalla_AddBlock_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Atalanta ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration ( Atalanta_HallOfValhalla_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_ExtremeRage:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_ExtremeRage_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Atalanta_ExtremeRage_Area, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_SplitJavelin );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Split Javelin Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Damage Boost:", "%d%%", Atalanta_SplitJavelin_DMGBoost, chainLevel1Based - 1 );
		}

		break;
	}

	case SKILLID_FrostJavelin:
	{
		AddSkillElement ( EElementID::Ice );

		AddSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Atalanta_FrostJavelin_FreezeDamage_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Chance:", "%d%%", Atalanta_FrostJavelin_FreezeChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Duration:", "%ds", Atalanta_FrostJavelin_FreezeDuration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Atalanta_FrostJavelin_SpeedMonDown, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Atalanta_FrostJavelin_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_Vengeance:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Atalanta_Vengeance_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Critical:", "%d%%", Atalanta_Vengeance_AddCritical, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Add Range:", "%d", Atalanta_Vengeance_AddRange, iSkillLevelZeroBased );

		int extremeRageLevel = GetLevelSkill(ESkillID::SKILLID_ExtremeRage);
		if (extremeRageLevel > 0)
		{
			AddSkillBoxGroupTitle(FormatString("Extreme Rage Chain (Lv. %d)", extremeRageLevel), TEXTCOLOR_SKILLCHAIN);
			AddChainSkillBoxInfo("Damage Boost:", "%d%%", ESkillArrayPointer::Atalanta_ExtremeRage_DMGBoost, extremeRageLevel - 1 );
		}

		int splitJavelinLevel = GetLevelSkill(ESkillID::SKILLID_SplitJavelin);
		if (splitJavelinLevel > 0)
		{
			AddSkillBoxGroupTitle(FormatString("Split Javelin Chain (Lv. %d)", splitJavelinLevel), TEXTCOLOR_SKILLCHAIN);
			AddChainSkillBoxInfo("Damage Boost:", "%d%%", ESkillArrayPointer::Atalanta_SplitJavelin_DMGBoost, splitJavelinLevel - 1 );
		}
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_MGS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//t1
	case SKILLID_Agony:
	{
		AddSkillBoxInfo ( "Health Lost:", "%d%%", Magician_Agony_HPTaken, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Mana Gained:", "%d%%", Magician_Agony_MPAdded, iSkillLevelZeroBased );
		break;
	}
	case SKILLID_FireBolt:
	{
		AddSkillElement ( EElementID::Fire );

		AddSkillBoxInfo ( "Add Attack Power:", "%d", Magician_FireBolt_DMGBoost, iSkillLevelZeroBased );

		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Magician_FireBolt_BurningDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Magician_FireBolt_BurningChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Magician_FireBolt_BurningDurationSec, iSkillLevelZeroBased );

		//Note - Range is not used by client or server. it's based on character attacking range.
		break;
	}
	case SKILLID_Zenith:
	{
		AddSkillBoxInfoDual ( "Absorption Boost:", "%d + %d%%", Magician_Zenith_AddAbsorb, Magician_Zenith_AddAbsorbPercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Resistance:", "%d", Magician_Zenith_AddElement, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Magician_Agony_MPAdded, iSkillLevelZeroBased );
		break;
	}
	case SKILLID_FireBall:
	{
		AddSkillElement ( EElementID::Fire );
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_FireBall_DMGBoostPercent, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_FireBall_Area, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Magician_FireBall_Range_NotUsed, iSkillLevelZeroBased );

		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Magician_FireBall_BurningDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Magician_FireBall_BurningChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Magician_FireBall_BurningDurationSec, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_FireBolt );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Fire Bolt Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Fire  );
			AddChainSkillBoxInfo ( "Add Attack Power:", "%d%%", Magician_FireBolt_DMGBoost, chainLevel1Based - 1 ); // Damage Boost is based on Firebolts damage why is this a thing?
		}

		break;
	}

	//t2
	case SKILLID_MentalMastery:
	{
		AddSkillBoxInfo ( "Mana Potion Boost:", "%d%%", Magician_MentalMastery_MPPotionBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Mana Total Boost:", "%d%%", Magician_MentalMastery_MPBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Watornado:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_Watornado_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_Watornado_Area, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Magician_Watornado_Range, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_EnchantWeapon:
	{
		AddSkillBoxInfoDuration ( Magician_EnchantWeapon_Duration, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Fire Enchantment (Burning)" , TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Magician_EnchantWeapon_FireBurningDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Magician_EnchantWeapon_FireBurningChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Magician_EnchantWeapon_FireBurningDuration, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Ice Enchantment (Freeze)" , TEXTCOLOR_SKILLCHAIN, EElementID::Ice );
		AddSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Magician_EnchantWeapon_IceFreezeAPPercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Chance:", "%d%%", Magician_EnchantWeapon_IceFreezeChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Duration:", "%ds", Magician_EnchantWeapon_IceFreezeDuration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Magician_EnchantWeapon_IceFreezeSpeedReduction, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Lightning Enchantment (Stun)" , TEXTCOLOR_SKILLCHAIN, EElementID::Lightning );
		AddSkillBoxInfo ( "Stun Damage (% of AP):", "%d%%", Magician_EnchantWeapon_LightningStunAPPercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Chance:", "%d%%", Magician_EnchantWeapon_LightningStunChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Stun Duration:", "%ds", Magician_EnchantWeapon_LightningStunDuration, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Radius:", "%d", Magician_EnchantWeapon_LightningRange_New, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DeathRay:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_DeathRay_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Magician_DeathRay_Range, iSkillLevelZeroBased );
		break;
	}

	//t3
	case SKILLID_EnergyShield:
	{
		AddSkillBoxInfo ( "Damage Reduction:", "%d%%", Magician_EnergyShield_ReduceDamage, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Mana Drain:", "%d", Magician_EnergyShield_MPDrain_New, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Magician_EnergyShield_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_MentalMastery );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Mental Mastery Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );

			AddChainSkillBoxInfo ( "Reduce Mana Drain:", "%d", Magician_MentalMastery_EW_Chain_MPDrain_New, chainLevel1Based - 1 );

			//AddSkillBoxInfoDual ( "Add Fire Damage:", "%d - %d", Magician_EnchantWeapon_FireDamageMin, Magician_EnchantWeapon_FireDamageMax, chainLevel1Based - 1, FALSE  );
			//AddSkillBoxInfoDual ( "Add Ice Damage:", "%d - %d", Magician_EnchantWeapon_IceDamageMin, Magician_EnchantWeapon_IceDamageMax, chainLevel1Based - 1, FALSE );
		}

		break;
	}

	case SKILLID_Diastrophism:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_Diastrophism_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_Diastrophism_Area, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_SpiritElemental:
	{
		AddSkillBoxInfoDual( "Attack Power Boost:", "%d%% - %d%%", Magician_SpiritElemental_DamageBoostMin, Magician_SpiritElemental_DamageBoostMax, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Magician_SpiritElemental_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DancingSword:
	{
		//AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Magician_DancingSword_DamagePercentOfAP, iSkillLevelZeroBased );
		//AddSkillBoxInfo ( "Attack Delay:", "%ds", Magician_DancingSword_AtkDelay, iSkillLevelZeroBased ); //not used
		AddSkillBoxInfoDuration ( Magician_DancingSword_Duration, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( FormatString ( "Fire Dancing Sword" ), TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Magician_DancingSword_FireBurningDamagePercent_New, iSkillLevelZeroBased);
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Magician_DancingSword_FireBurningChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Magician_DancingSword_FireBurningDurationSec_New, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( FormatString ( "Ice Dancing Sword" ), TEXTCOLOR_SKILLCHAIN, EElementID::Ice );
		AddSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Magician_DancingSword_IceFreezeAPPercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Chance:", "%d%%", Magician_DancingSword_IceFreezeChance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Freeze Duration:", "%ds", Magician_DancingSword_IceFreezeDuration_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Magician_DancingSword_IceFreezeSpeedReduction_New, iSkillLevelZeroBased );


		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_DeathRay );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Death Ray Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_DeathRay_DMGBoost, chainLevel1Based - 1  );
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_EnchantWeapon );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Enchant Weapon Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfo ( "Add Burn Damage (% of AP):", "%d%%", Magician_EnchantWeapon_FireBurningDamagePercent, chainLevel1Based - 1);
			AddChainSkillBoxInfo ( "Add Freeze Damage (% of AP):", "%d%%", Magician_EnchantWeapon_IceFreezeAPPercent, chainLevel1Based - 1 );
		}

		break;
	}

	//t4
	case SKILLID_FireElemental:
	{
		AddSkillElement ( EElementID::Fire );

		AddSkillBoxInfoDual ( "Attack Power:", "%d - %d", Magician_FireElemental_ATKPowMin, Magician_FireElemental_ATKPowMax, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d", Magician_FireElemental_AtkRating, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Health:", "%d", Magician_FireElemental_HP, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Magician_FireElemental_Duration, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_SpiritElemental );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Spirital Elemental Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Attack Power Boost:", "%d%% - %d%%", Magician_SpiritElemental_DamageBoostMin, Magician_SpiritElemental_DamageBoostMax, chainLevel1Based - 1 );
		}

		if ( !SKILLINFOBOX->IsViewingSkillTimer() )
		{
			AddSkillBoxGroupTitle ( "Pet default setting", D3DCOLOR_XRGB ( 238, 210, 151 ) );
			AddSkillBoxInfo ( "Mode:", FormatString ( "%s", SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive ? "Aggressive" : "Passive" ) );
		}
		else
		{
			AddSkillBoxGroupTitle ( "Pet setting", D3DCOLOR_XRGB ( 238, 210, 151 ) );
			AddSkillBoxInfo ( "Mode:", FormatString ( "%s", SKILLMANAGERHANDLER->IsAgressive ( SKILLID_FireElemental ) ? "Aggressive" : "Passive" ) );
		}

		break;
	}

	case SKILLID_FlameWave:
	{
		AddSkillElement ( EElementID::Fire );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_FlameWave_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_FlameWave_Area, iSkillLevelZeroBased );

		AddSkillBoxInfo ( "Burn Damage (% of AP):", "%d%%", Magician_FlameWave_BurningDamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Chance:", "%d%%", Magician_FlameWave_BurningChance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Burn Duration:", "%ds", Magician_FlameWave_BurningDurationSec, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_FireBolt );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Fire Bolt Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
			AddChainSkillBoxInfo ( "Add Attack Power:", "%d", Magician_FireBolt_DMGBoost, chainLevel1Based - 1 );
		}

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_FireBall );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle( FormatString( "Fire Ball Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN, EElementID::Fire );
			AddChainSkillBoxInfo( "Attack Power Boost:", "%d%%", Magician_FireBall_DMGBoostPercent, chainLevel1Based - 1  );
		}
		break;
	}

	case SKILLID_Distortion:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Apply chance:", "%d%%", Magician_Distortion_Chance, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage (% of AP):", "%d%%", Magician_Distortion_DamagePercent, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Magician_Distortion_Duration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Reduction:", "%d%%", Magician_Distortion_DamageReduction, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Magician_Distortion_SpeedReduction, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_Distortion_Area, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Magician_Distortion_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Meteorite:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Magician_Meteorite_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Magician_Meteorite_Area, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Distortion );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Distortion Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN,  EElementID::Organic );

			AddChainSkillBoxInfo ( "Apply chance:", "%d%%", Magician_Distortion_Chance, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Damage (% of AP):", "%d%%", Magician_Distortion_DamagePercent, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Duration:", "%ds", Magician_Distortion_Duration, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Damage Reduction:", "%d%%", Magician_Distortion_DamageReduction, chainLevel1Based - 1 );
			AddChainSkillBoxInfo ( "Speed Reduction:", "%d%%", Magician_Distortion_SpeedReduction, chainLevel1Based - 1 );
		}

		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_PRS ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{
		//tier 1
	case SKILLID_Healing:
	{
		AddSkillBoxInfoDual ( "Healing Power:", "%d%% + %d", Priestess_Healing_LifeRecoveryPercent, Priestess_Healing_LifeRecovery, iSkillLevelZeroBased ); // Unfinished, Healing isn't a DB value yet?
		break;
	}

	case SKILLID_HolyBolt:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Priestess_HolyBolt_DamageBoost_New, Priestess_HolyBolt_Damage, iSkillLevelZeroBased ); // Unfinished, Healing isn't a DB value yet?
		break;
	}

	case SKILLID_MultiSpark:
	{
		AddSkillBoxInfoDual ( "Damage per hit: ", "%d%% + %d", Priestess_MultiSpark_DamageBoost, Priestess_MultiSpark_Damage, iSkillLevelZeroBased );
		AddSkillBoxInfoDual ( "Number of sparks: ", "%d - %d", Priestess_MultiSpark_NumSparksMin_New, Priestess_MultiSpark_NumSparksMax, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_HolyMind:
	{
		AddSkillBoxInfo ( "Reduce Damage:", "%d%%", Priestess_HolyMind_ReduceDamage, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Priestess_HolyMind_Duration_New, iSkillLevelZeroBased );
		break;
	}

	//tier 2

	case SKILLID_Meditation:
	{
		AddSkillBoxInfo ( "MP Potion Boost: ", "%d%%", Priestess_Meditation_MPPotionBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add MP Regen: ", "%.2f", Priestess_Meditation_AddMPRegen, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_DivineLightning:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Priestess_DivineLightning_DamageBoost, Priestess_DivineLightning_Damage_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of Bolts:", "%d", Priestess_DivineLightning_NumOfBolts, iSkillLevelZeroBased );
		//AddSkillBoxInfo("Attack Range:", "180", iSkillLevelZeroBased); // Can we add a text line with a flat value? As the attack range always remains 180.
		break;
	}

	case SKILLID_HolyReflection:
	{
		AddSkillBoxInfo ( "Reflect Damage:", "%d%%", Priestess_HolyReflection_ReflectDMG, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Priestess_HolyReflection_Duration, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_GrandHealing:
	{
		AddSkillBoxInfoDual ( "Healing Power:", "%d%% + %d", Priestess_GrandHealing_LifeRecoveryPercent, Priestess_GrandHealing_LifeRecovery, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Priestess_GrandHealing_Range, iSkillLevelZeroBased );

		chainLevel1Based = GetLevelSkill ( ESkillID::SKILLID_Healing );
		if ( chainLevel1Based > 0 )
		{
			AddSkillBoxGroupTitle ( FormatString ( "Healing Chain (Lv. %d)", chainLevel1Based ), TEXTCOLOR_SKILLCHAIN );
			AddChainSkillBoxInfoDual ( "Healing Power:", "%d%% + %d", Priestess_Healing_LifeRecoveryPercent, Priestess_Healing_LifeRecovery, chainLevel1Based - 1 );
		}

		break;
	}

	//tier 3
	case SKILLID_VigorBall:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Priestess_VigorBall_DamageBoost, Priestess_VigorBall_Damage_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating Boost:", "%d%%", Priestess_VigorBall_AtkRtgBoost, iSkillLevelZeroBased );
		break;
	}

	case SKILLID_Ressurection:
	{
		AddSkillBoxInfo ( "Success Rate:", "%d%%", Priestess_Resurrection_SuccessRate, iSkillLevelZeroBased );
        AddSkillBoxInfo("Max Players:", "%d", Priestess_Resurrection_MaxPlayers, iSkillLevelZeroBased);

        SKILLINFOBOX->AddString(0, "Range:", D3DCOLOR_XRGB(243, 237, 216));
        SKILLINFOBOX->AddString(1, FormatString("%d [%.1fm]", PRIESTRESS_RESSURECTION_RANGE, static_cast<float>(PRIESTRESS_RESSURECTION_RANGE) / 27.0f), D3DCOLOR_XRGB(238, 210, 151));

		break;
	}

	case SKILLID_Extinction:
	{
		AddSkillBoxInfo ( "Success Rate:", "%d%%", Priestess_Extinction_SuccessRate, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP Reduction:", "%d%%", Priestess_Extinction_HPReduction, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Priestess_Extinction_Radius_New, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_VirtualLife:
	{
		AddSkillBoxInfo ( "HP Boost:", "%d%%", Priestess_VirtualLife_HPBoost, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Priestess ? 1.0f : 0.5f);
		AddSkillBoxInfo ( "MP Boost:", "%d%%", Priestess_VirtualLife_MPBoost_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Priestess ? 1.0f : 0.5f);
		AddSkillBoxInfoDuration ( Priestess_VirtualLife_Duration, iSkillLevelZeroBased );

		return TRUE;
	}


	//t4
	case SKILLID_GlacialSpike:
	{
		AddSkillElement ( EElementID::Ice );

		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Priestess_GlacialSpike_DamageBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Priestess_GlacialSpike_ATKRange, iSkillLevelZeroBased );

		AddSkillBoxGroupTitle ( "Freeze attack", TEXTCOLOR_SKILLCHAIN, EElementID::Ice );
		AddChainSkillBoxInfo ( "Freeze Damage (% of AP):", "%d%%", Priestess_GlacialSpike_FreezePercentOfAP_New, iSkillLevelZeroBased);
		AddChainSkillBoxInfo ( "Freeze Chance:", "%d%%", Priestess_GlacialSpike_FreezeChance, iSkillLevelZeroBased );
		AddChainSkillBoxInfo ( "Freeze Duration:", "%ds", Priestess_GlacialSpike_FreezeDurationSec, iSkillLevelZeroBased);
		AddChainSkillBoxInfo ( "Speed Reduction:", "%d%%", Priestess_GlacialSpike_SpeedMonDown_New, iSkillLevelZeroBased);

		break;
	}

	case SKILLID_VirtualRegen:
	{
		//half for non-prs
		AddSkillBoxInfo ( "HP Regen:", "%d", Priestess_VirtualRegen_HPRegen_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Priestess ? 1.0f : 0.5f);
		AddSkillBoxInfo ( "MP Regen:", "%d", Priestess_VirtualRegen_MPRegen_New, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Priestess ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration ( Priestess_VirtualRegen_Duration_New, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_ChainLightning:
	{
		AddSkillBoxInfoDual ( "Attack Power Boost:", "%d%% + %d", Priestess_ChainLightning_DamageBoost, Priestess_ChainLightning_Damage_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Number of bolts:", "%d", Priestess_ChainLightning_Numberofbolts, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Priestess_ChainLightning_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_SummonMuspell:
	{
		AddSkillBoxInfo ( "Muspell Damage Boost:", "%d%%", Priestess_SummonMuspell_MuspellDMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Muspell Attack Delay:", "%d", Priestess_SummonMuspell_MuspellAttackDelaySec, iSkillLevelZeroBased );

		AddSkillBoxEmptyLine ();
		AddSkillBoxInfo ( "Undead Absorption Boost", "%d%%", Priestess_SummonMuspell_UndeadAbsorb_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Evasion Boost", "%d%%", Priestess_SummonMuspell_AddEvasion_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Priestess_SummonMuspell_DamageBoost_New, iSkillLevelZeroBased );

		AddSkillBoxEmptyLine ();
		AddSkillBoxInfo ( "Virtual Life Reduction", "%d%%", Priestess_SummonMuspell_VirtualLifeReduction, iSkillLevelZeroBased );

		return TRUE;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_SHA ( int iSkillID, int iSkillLevelZeroBased )
{
	BOOL bAdded = TRUE;
	int chainLevel1Based;
	float chainDmgScale;

	switch ( iSkillID )
	{

		//Shaman
		//Tier1

	case SKILLID_DarkBolt:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_DarkBolt_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Shaman_DarkBolt_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_DarkWave:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_DarkWave_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Shaman_DarkWave_Range, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Shaman_DarkWave_ATKRange, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_Inertia:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Apply chance:", "%d%%", Shaman_Inertia_Curse_Chance_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Percent:", "%d%%", Shaman_Inertia_Curse_DamagePercent_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Duration:", "%ds", Shaman_Inertia_Curse_Duration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Damage Reduction:", "%d%%", Shaman_Inertia_Curse_DamageReduction_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Speed Reduction:", "%d%%", Shaman_Inertia_Curse_SpeedReduction, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Shaman_Inertia_Area, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_InnerPeace:
	{
		AddSkillBoxInfo ( "Mana Potion Boost:", "%d%%", Shaman_InnerPeace_ManaPotionBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Mana Total Boost:", "%d%%", Shaman_InnerPeace_ManaBoost, iSkillLevelZeroBased );

		break;
	}

	//tier 2

	case SKILLID_SpiritualFlare:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_SpiritualFlare_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange( "Attack Range", "%d", Shaman_SpiritualFlare_ATKRange, iSkillLevelZeroBased );
		AddSkillBoxInfoRange( "Radius", "%d", Shaman_SpiritualFlare_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_SpiritualManacle:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Stun Duration:", "%d", Shaman_SpiritualManacle_StunTime, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_ChasingHunt:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_ChasingHunt_AtkPowBoost_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Shaman_ChasingHunt_Range, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Shaman_ChasingHunt_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_AdventMigal:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_AdventMigal_AddATKPow, iSkillLevelZeroBased, TRUE,
			PLAYERDATA->iClass == ECharacterClass::CHARACTERCLASS_Shaman ? 1.0f : 0.5f );

		AddSkillBoxInfoDuration ( Shaman_AdventMigal_Duration, iSkillLevelZeroBased );

		break;
	}

	//tier 3

	case SKILLID_Rainmaker:
	{
		AddSkillBoxInfo ( "Damage Reduction:", "%d%%", Shaman_Rainmaker_ReduceDamage_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Evasion", "+%d%%", Shaman_Rainmaker_AddEvasion_New, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Mana Reduction", "%d%%", Shaman_Rainmaker_ReduceMP, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Shaman_Rainmaker_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_PhantomCall:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_PhantomCall_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius", "%d", Shaman_PhantomCall_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_Haunt:
	{
		AddSkillElement ( EElementID::Organic );

		AddSkillBoxInfo ( "Absorb Life:", "%d%%", Shaman_Haunt_LifeAbsorb, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Max HP Absorb:", "%d", Shaman_Haunt_LifeAbsorbCap_New, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Range:", "%d", Shaman_Haunt_Radius_New, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_Scratch:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_Scratch_DMGBoost, iSkillLevelZeroBased );

		break;
	}

	//tier 4
	case SKILLID_RecallHestian:
	{
		AddSkillBoxInfoDuration ( Shaman_SummonHestian_Duration, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Defense:", "%d", Shaman_SummonHestian_Defense, iSkillLevelZeroBased );
		AddSkillBoxInfoDual ( "Attack Power:", "%d - %d", Shaman_SummonHestian_ATKPowMin, Shaman_SummonHestian_ATKPowMax, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Attack Rating:", "%d", Shaman_SummonHestian_AtkRating, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "HP:", "%d", Shaman_SummonHestian_HP, iSkillLevelZeroBased );

		if (!SKILLINFOBOX->IsViewingSkillTimer() )
		{
			AddSkillBoxGroupTitle ( "Pet default setting", D3DCOLOR_XRGB ( 238, 210, 151 ) );
			AddSkillBoxInfo ( "Mode:", FormatString ( "%s", SETTINGSHANDLER->GetModel()->GetSettings().bDefaultPetModeAggressive ? "Aggressive" : "Passive" ) );
		}
		else
		{
			AddSkillBoxGroupTitle( "Pet setting", D3DCOLOR_XRGB( 238, 210, 151 ) );
			AddSkillBoxInfo( "Mode:", FormatString( "%s", SKILLMANAGERHANDLER->IsAgressive( SKILLID_RecallHestian ) ? "Aggressive" : "Passive" ) );
		}

		break;
	}

	case SKILLID_Judgement:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_Judgement_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfo ( "Add Critical Chance:", "%d%%", Shaman_Judgement_AddCritical, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Attack Range:", "%d", Shaman_Judgement_Range, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_AdventMidranda:
	{
		AddSkillBoxInfo ( "Attack Speed Boost:", "%d%%", Shaman_AdventMidranda_ATKSPDBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoDuration ( Shaman_AdventMidranda_Duration, iSkillLevelZeroBased );

		break;
	}

	case SKILLID_MourningPray:
	{
		AddSkillBoxInfo ( "Attack Power Boost:", "%d%%", Shaman_MourningPray_DMGBoost, iSkillLevelZeroBased );
		AddSkillBoxInfoRange ( "Radius:", "%d", Shaman_MourningPray_ATKRange, iSkillLevelZeroBased );

		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}


BOOL SkillManager::AddSkillInfoToBox (int iSkillID, int iSkillLevelZeroBased )
{
	if ( AddSkillInfoToBox_AS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_FS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_MS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_PS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_ASS ( iSkillID, iSkillLevelZeroBased ) ||

		 AddSkillInfoToBox_ATA ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_KS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_PRS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_MGS ( iSkillID, iSkillLevelZeroBased ) ||
		 AddSkillInfoToBox_SHA ( iSkillID, iSkillLevelZeroBased ) )
	{
		return TRUE;
	}

	return FALSE;
}

void SkillManager::LoadServerCommonSkillInfo ( SkillInfoCommon * data )
{
	Skill * sSkill1 = GAMECORE->FindSkill2( data->iSkillId );
	Skill * sSkill2 = GAMECORE->FindSkill( data->iSkillId );

	SkillBase * sbSkillBase1 = &sSkill1->sSkillInfo;
	SkillBase * sbSkillBase2 = &sSkill2->sSkillInfo;
	SkillBase * sbSkillBase3 = GAMECORE->FindSkillBase2( data->iSkillId );

	sSkill1->iID = data->iSkillId;
	sSkill1->szName[0] = { 0 };
	sSkill1->szFileName[0] = { 0 };
	sSkill1->szIconName[0] = { 0 };

	sbSkillBase1->szName[0] = { 0 };
	sbSkillBase2->szName[0] = { 0 };
	sbSkillBase3->szName[0] = { 0 };

	STRINGCOPY ( sSkill1->szName, data->szSkillName );
	STRINGCOPY ( sSkill2->szName, data->szSkillName );

	STRINGCOPY ( sSkill1->szFileName, data->szSkillFileName );
	STRINGCOPY ( sSkill2->szFileName, data->szSkillFileName );

	STRINGCOPY ( sSkill1->szIconName, data->szSkillIconName );
	STRINGCOPY ( sSkill2->szIconName, data->szSkillIconName );

	if ( data->szSkillIconName[0] )
	{
		//update Mat icon
		int mat = TIMERSKILLHANDLER->CreateTextureMaterial( FormatString( "Image\\SinImage\\skill\\keep\\%s", data->szSkillIconName ) );

		//update mat with the new one (if load successful)
		if ( mat > 0 )
		{
			sSkill1->MatIcon = mat;
			sSkill2->MatIcon = mat;
		}
	}

	STRINGCOPYLEN ( sbSkillBase1->szName, 32, data->szSkillName );
	STRINGCOPYLEN ( sbSkillBase2->szName, 32, data->szSkillName );
	STRINGCOPYLEN ( sbSkillBase3->szName, 32, data->szSkillName );

	STRINGCOPY ( sbSkillBase1->szDescription, data->szSkillDescription );
	STRINGCOPY ( sbSkillBase2->szDescription, data->szSkillDescription );
	STRINGCOPY ( sbSkillBase3->szDescription, data->szSkillDescription );

	sbSkillBase1->eSkillID = (ESkillID)data->iSkillId;
	sbSkillBase2->eSkillID = (ESkillID)data->iSkillId;
	sbSkillBase3->eSkillID = (ESkillID)data->iSkillId;
}

void SkillManager::LoadServerSkillInfo ( SkillInfo * dataEx )
{
	int iSkillIndex0Based = dataEx->iSkillNum - 1;

	SkillBase * sbSkillBase1;

	switch ( UNITGAME->pcUnitData->sCharacterData.iClass )
	{
		case ECharacterClass::CHARACTERCLASS_Mechanician:	sbSkillBase1 = ( (SkillBase *)0x00961B18 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Fighter:		sbSkillBase1 = ( (SkillBase *)0x00963288 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Archer:		sbSkillBase1 = ( (SkillBase *)0x00966168 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Pikeman:		sbSkillBase1 = ( (SkillBase *)0x009649F8 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Atalanta:		sbSkillBase1 = ( (SkillBase *)0x0096A7B8 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Knight:		sbSkillBase1 = ( (SkillBase *)0x00969048 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Magician:		sbSkillBase1 = ( (SkillBase *)0x0096D698 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Priestess:		sbSkillBase1 = ( (SkillBase *)0x0096BF28 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Assassin:		sbSkillBase1 = ( (SkillBase *)0x009678D8 ) + iSkillIndex0Based; break;
		case ECharacterClass::CHARACTERCLASS_Shaman:		sbSkillBase1 = ( (SkillBase *)0x0096EE08 ) + iSkillIndex0Based; break;
		default: return;
	}

	Skill * sSkill = SKILLSCHARACTER + ( iSkillIndex0Based + 1);
	Skill * sSkill1 = GAMECORE->FindSkill2( dataEx->iSkillId );
	Skill * sSkill2 = GAMECORE->FindSkill( dataEx->iSkillId );

	SkillBase * sbSkillBase2 = &sSkill->sSkillInfo;

	sSkill->iID = dataEx->iSkillId;
	sSkill->Position = iSkillIndex0Based + 1 + 1; //Default attack is position 1, first skill is position 2 and so on.
	sSkill->szName[0] = { 0 };
	sSkill->szFileName[0] = { 0 };
	sSkill->szIconName[0] = { 0 };

	sbSkillBase1->szName[0] = { 0 };
	sbSkillBase2->szName[0] = { 0 };

	sbSkillBase1->iSkillIndex = iSkillIndex0Based;
	sbSkillBase2->iSkillIndex = iSkillIndex0Based;

	STRINGCOPY ( sSkill->szName, dataEx->szSkillName );
	STRINGCOPY ( sSkill->szFileName, dataEx->szSkillFileName );
	STRINGCOPY ( sSkill->szIconName, dataEx->szSkillIconName );

	if ( sSkill->szIconName[0] )
	{
		//update Mat icon
		int mat = TIMERSKILLHANDLER->CreateTextureMaterial( FormatString( "Image\\SinImage\\skill\\keep\\%s", sSkill->szIconName ) );

		//update mat with the new one (if load successful)
		if ( mat > 0 )
		{
			sSkill->MatIcon = mat;
		}
	}

	STRINGCOPYLEN ( sbSkillBase1->szName, 32, dataEx->szSkillName );
	STRINGCOPYLEN ( sbSkillBase2->szName, 32, dataEx->szSkillName );

	STRINGCOPY ( sbSkillBase1->szDescription, dataEx->szSkillDescription );
	STRINGCOPY ( sbSkillBase2->szDescription, dataEx->szSkillDescription );

	sbSkillBase1->eSkillID = (ESkillID)dataEx->iSkillId;
	sbSkillBase2->eSkillID = (ESkillID)dataEx->iSkillId;

	sbSkillBase1->eSkillType = (ESkillType)( dataEx->iSkillType << 24);
	sbSkillBase2->eSkillType = (ESkillType)( dataEx->iSkillType << 24);

	sbSkillBase1->iSkillLevelRequired = dataEx->iSkillReqLevel;
	sbSkillBase2->iSkillLevelRequired = dataEx->iSkillReqLevel;

	sbSkillBase1->iSkillMasteryBase = dataEx->sSkillMasteryBase;
	sbSkillBase2->iSkillMasteryBase = dataEx->sSkillMasteryBase;

	sbSkillBase1->iSkillMasteryPerLevel = dataEx->sSkillMasteryPerLevel;
	sbSkillBase2->iSkillMasteryPerLevel = dataEx->sSkillMasteryPerLevel;

	sbSkillBase1->dwFunctionPointer = dataEx->dwFuncPointer;
	sbSkillBase2->dwFunctionPointer = dataEx->dwFuncPointer;

	sbSkillBase1->dwUseManaPointer = dataEx->dwManaPointer;
	sbSkillBase2->dwUseManaPointer = dataEx->dwManaPointer;

    sbSkillBase1->iaSkillUseStamina[0] = dataEx->sSkillUseStaminaBase;
    sbSkillBase2->iaSkillUseStamina[0] = dataEx->sSkillUseStaminaBase;

    sbSkillBase1->iaSkillUseStamina[1] = dataEx->sSkillUseStaminaPerLevel;
    sbSkillBase2->iaSkillUseStamina[1] = dataEx->sSkillUseStaminaPerLevel;

	CopyMemory (&sbSkillBase1->eSkillItemAllowed, &dataEx->iaItemAllowedType, sizeof (int) * 8);
	CopyMemory (&sbSkillBase2->eSkillItemAllowed, &dataEx->iaItemAllowedType, sizeof (int) * 8);

	CopyMemory( sSkill1, sSkill, sizeof( Skill ) );
	CopyMemory( sSkill2, sSkill, sizeof( Skill ) );

	//iaSkillUseStamina
	//iaSkillElements
}

void SkillManager::LoadServerSkillChainData( SkillChainScaleData * data )
{
	auto it = mapSkillChainScale.find( data->dwSkillArrayPointer );

	if ( it == mapSkillChainScale.end() )
		mapSkillChainScale.insert( { data->dwSkillArrayPointer, data->fScale } );
	else
		it->second = data->fScale; //update
}

void SkillManager::LoadServerSkillData ( SkillArrayData * data )
{
	int iSkillType = (data->dwSkillArrayAddressOrIndex & 0xF0000000) >> 28;
	DWORD dwAddressOrID = ( data->dwSkillArrayAddressOrIndex & 0x0FFFFFFF );

	// Int array
	//data->iSkillID & 0xF0000000 >> 28) = skill type
	if ( iSkillType == 1 )
	{
		//new int values
		if ( dwAddressOrID < 0x00001000 )
		{
			int * pointer = GetNewSkillIntArrayPointer ( dwAddressOrID );
			if ( pointer )
			{
				CopyMemory ( (PVOID)pointer, &data->iSkillValues, 10 * sizeof ( int ) );

				DWORD l_BaseGameArrayToPatch = 0;
				if (dwAddressOrID == ESkillArrayPointer::Priestess_VirtualRegen_HPRegen_New)
					l_BaseGameArrayToPatch = 0x00996F40;
                if (dwAddressOrID == ESkillArrayPointer::Priestess_VirtualRegen_MPRegen_New)
					l_BaseGameArrayToPatch = 0x00996F68;

				if (l_BaseGameArrayToPatch != 0)
				{
                    DWORD l_OldFlags;
                    VirtualProtect((void*)l_BaseGameArrayToPatch, 10 * sizeof(int), PAGE_EXECUTE_READWRITE, &l_OldFlags);
                    CopyMemory((PVOID)l_BaseGameArrayToPatch, &data->iSkillValues, 10 * sizeof(int));
                    VirtualProtect((void*)l_BaseGameArrayToPatch, 10 * sizeof(int), l_OldFlags, NULL);
                }
			}
		}
		else
		{
			//Override at address
			CopyMemory ( (PVOID)dwAddressOrID, &data->iSkillValues, 10 * sizeof ( int ) );
		}
	}

	//Float array
	//data->iSkillID & 0xF0000000 >> 28) = skill type
	else if ( iSkillType == 2 )
	{
		//new float values
		if ( dwAddressOrID < 0x00001000 )
		{
			float * pointer = GetNewSkillFloatArrayPointer ( dwAddressOrID );
			if ( pointer )
			{
				CopyMemory ( (PVOID)pointer, &data->fSkillValues, 10 * sizeof ( int ) );
			}
		}
		else
		{
			//Override at address
			CopyMemory ( (PVOID)( dwAddressOrID ), &data->fSkillValues, 10 * sizeof ( float ) );
		}
	}
}

void SkillManager::HandlePacket( PacketSkillBuffStatus * psPacket )
{
	Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iTargetID ) );
	if ( pcUnit )
	{
		PRIESTESSSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		FIGHTERSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		ATALANTASKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		ASSASSINSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		MECHANICIANSKILLHANDLER->OnSkillBuffUpdate(pcUnit, psPacket);
		MAGICIANSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		SHAMANSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		ARCHERSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
		KNIGHTSKILLHANDLER->OnSkillBuffUpdate( pcUnit, psPacket );
	}

	//std::map<ESkillID, SkillBuffStatus *> mapping;
	//for ( int i = 0; i < psPacket->sCount; i++ )
	//{
	//	mapping.insert( { psPacket->saBuff[i].eSkillID, &psPacket->saBuff[i] } );
	//}
	//
	//Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iTargetID ) );
	//if ( pcUnit )
	//{
	//	auto it = mapping.find( ESkillID::SKILLID_VirtualLife );
	//	if ( it != mapping.end() )
	//	{
	//		VIRTUALLIFEHANDLER->OnCastEffect ( pcUnit, psPacket->sVirtualLifeLevel, psPacket->sVirtualLifeTimeLeft, FALSE );
	//	}
	//	else
	//	{
	//		VIRTUALLIFEHANDLER->OnCastEnd ( pcUnit );
	//	}
	//
	//	if ( psStatus. > 0 && psPacket->sVirtualRegenTimeLeft > 0 )
	//	{
	//		VIRTUALREGENHANDLER->OnCastEffect ( pcUnit, psPacket->sVirtualRegenLevel, psPacket->sVirtualLifeLevel, static_cast<float>(psPacket->sVirtualRegenTimeLeft), FALSE );
	//	}
	//	else
	//	{
	//		VIRTUALREGENHANDLER->OnCastEnd ( pcUnit );
	//	}






		//Lethal Sight
		//if ( pcUnit->bLethalSight )
		//{
		//	//Cast End Time
		//	if ( psPacket->sLethalSightTimeLeft == 0 )
		//	{
		//		LETHALSIGHTHANDLER->OnCastEnd( pcUnit );
		//	}
		//}
		//else
		//{
		//	//Add New Cast
		//	if ( psPacket->sLethalSightTimeLeft > 0 )
		//	{
		//		LETHALSIGHTHANDLER->OnCastEffect( pcUnit, psPacket->sLethalSightLevel, (float)psPacket->sLethalSightTimeLeft );
		//	}
		//}
		//
		////Holy Conviction
		//if ( pcUnit->bHolyConviction )
		//{
		//	//Cast End Time
		//	if ( psPacket->sHolyConvictionTimeLeft == 0 )
		//	{
		//		HOLYCONVICTIONHANDLER->OnCastEnd( pcUnit );
		//	}
		//}
		//else
		//{
		//	//Add New Cast
		//	if ( psPacket->sHolyConvictionTimeLeft > 0 )
		//	{
		//		HOLYCONVICTIONHANDLER->OnCastEffect( pcUnit, psPacket->sHolyConvictionLevel, (float)psPacket->sHolyConvictionTimeLeft );
		//	}
		//}
		//
		////Orb Mastery
		//if( pcUnit->bWizardTrance )
		//{
		//	//Cast End Time
		//	if( psPacket->sWizardTranceTimeLeft == 0 )
		//	{
		//		WIZARDTRANCEHANDLER->OnCastEnd( pcUnit );
		//	}
		//}
		//else
		//{
		//	//Add New Cast
		//	if( psPacket->sWizardTranceTimeLeft > 0 )
		//	{
		//		WIZARDTRANCEHANDLER->OnCastEffect( pcUnit, psPacket->sWizardTranceLevel, 300.0f );
		//	}
		//}
}




void SkillManager::HandlePacket( PacketSkillCancel * psPacket )
{
#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "PacketSkillCancel id = %d", psPacket->iPlayID ) );
#endif
	/*if (psPacket->iPlayID == SKILLPLAYID_VIRTUAL_MANA)
	{
		VIRTUALREGENHANDLER->OnCastAnimationStart(psPacket->iUnk, UNITDATA);
	}*/

	if ( (psPacket->iUnk == 0) && (psPacket->iUnk2 == 0) && (psPacket->iUnk3 == 0) )
	{
		switch ( UNITDATA->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Archer:
				switch ( psPacket->iPlayID )
				{
					case SKILLPLAYID_T51:
						LETHALSIGHTHANDLER->OnCastEnd( UNIT );
						break;
				}
				break;

			case CHARACTERCLASS_Shaman:
				switch ( psPacket->iPlayID )
				{
					case SKILLPLAYID_T51:
						CREEDHANDLER->OnCast( psPacket->iUnk );
						break;
				}
				break;
			default:
				break;
		}
	}
}

/// <summary>
/// Overrides RecvProcessSkill
/// </summary>
/// <param name="psPacket"></param>
void SkillManager::HandlePacket( PacketSkillCast * psPacket )
{
	short skillTinyID = psPacket->iSkillCode & 0xFF;
	short skillLevel1Based = psPacket->iSkillCode >> 8 & 0xF;

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString ( "PacketSkillCast id = %d", skillTinyID ) );
#endif

	/*if (psPacket->sActiveSkill.cSkillTinyID == SKILLPLAYID_VIRTUAL_MANA)
	{
		VIRTUALREGENHANDLER->OnCastAnimationStart(psPacket->sActiveSkill.cLevel, UNITDATA);
	}*/

	ESkillID eSkillID = GetSkillIDFromTinyID( (ECharacterClass)psPacket->LParam, static_cast<BYTE>( skillTinyID ) );

	//kill whatever existing buff there is, and override it
	TIMERSKILLHANDLER->KillTimer ( (DWORD)eSkillID );

    bool l_ShouldSkipASMHandling = false;
	switch ( eSkillID )
	{
		case ESkillID::SKILLID_HallofValhalla:
		case ESkillID::SKILLID_TriumphofValhalla:
        case ESkillID::SKILLID_EnchantWeapon:
			l_ShouldSkipASMHandling = true;
			break;
	}

	switch ( psPacket->LParam )
	{
		case CHARACTERCLASS_Atalanta:
		{
			if (eSkillID == ESkillID::SKILLID_Ressurection)
				l_ShouldSkipASMHandling = RESSURECTIONHANDLER->OnSkillCasted(psPacket);
			/*if ( psPacket->sActiveSkill.cSkillTinyID == AtalantaSkill::HallofValhalla )
			{
				CHATBOX->AddDebugMessage ( "hello" );
			}*/
			break;
		}

		//case CHARACTERCLASS_Shaman:
		//	if ( psPacket->sSkillTinyID == SKILLPLAYID_T51 )
		//		CREEDHANDLER->OnCast( psPacket->sActiveSkill.cLevel );
		//	break;
		//
		//default:
		//	break;
	}

	if (l_ShouldSkipASMHandling)
		psPacket->iHeader = 0; //set to 0 to disable processing in assembly
}
