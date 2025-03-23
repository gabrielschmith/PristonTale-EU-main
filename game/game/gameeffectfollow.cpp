#include "stdafx.h"
#include "gameeffectfollow.h"



NAKED void CASSAChar::Constructor()
{
	JMP( pfnConstructor );
}

CASSAChar::CASSAChar()
{
	Constructor();
}

CASSAChar::~CASSAChar()
{
}


NAKED void CASSAChar::Start( UnitData * pcUnitData, int code, int maxTime, float delay )
{
	JMP( pfnStart );
}

BOOL CASSAChar::StartMacro( UnitData * pcUnitData, int effectCode, int commonCode, int maxTime, float delay )
{
	if ( EffectBaseFollow::IsActiveEffect( pcUnitData, commonCode ) )
	{
		EffectBaseFollow::MaintainEffect( pcUnitData, commonCode );
	}
	else
	{
		int iIndex = EffectBaseFollow::GetFreeIndex();

		CASSAChar * pc = new CASSAChar();
		pc->Start( pcUnitData, effectCode, maxTime, delay );

		EffectBaseFollow::GetData()[iIndex] = pc;

		return TRUE;
	}

	return FALSE;
}

NAKED void CGameEffectFollow::Constructor()
{
	JMP( pfnConstructor );
}

CGameEffectFollow::CGameEffectFollow()
{
	Constructor();
}


CGameEffectFollow::~CGameEffectFollow()
{
}

NAKED void CGameEffectFollow::Start( UnitData * pcUnitData, char * pszMeshName, char * pszParticleName, bool bAttack, int iTime )
{
	JMP( pfnStart );
}


void CGameEffectFollow::SetPositionLength( int i )
{
	iLength = i;
}

void CGameEffectFollow::SetScale( float f )
{
	fScale = f;
}

BOOL CGameEffectFollow::SetEffectUnitSpawn( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Arhdyra:
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 Spine", "ArhdyraBody", MONSTERFOLLOWEFFECT_ArhdyraBody );
			break;

		case MONSTEREFFECTID_Greedy:
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 Spine", "GreedyBody", MONSTERFOLLOWEFFECT_GreedyBody );
			break;

		case MONSTEREFFECTID_Yagditha:
			SETPARTICLEMESHFOLLOW(pcUnitData, "Bip01 Spine", "YagdithaBody", MONSTERFOLLOWEFFECT_YagdithaBody);
			break;

		//case MONSTEREFFECTID_FLAMEMAIDEN:
		//	SETPARTICLEMESHFOLLOW(pcUnitData, "Bip01 Spine", "FlamemaidenBody", MONSTERFOLLOWEFFECT_FlamemaidenBody);
		//	break;

		case MONSTEREFFECTID_Draxos:
			SETPARTICLEMESHFOLLOW(pcUnitData, "Bip01 Spine", "DraxosBody", MONSTERFOLLOWEFFECT_DraxosBody);
			break;

		//case MONSTEREFFECTID_INSEC:
		//	SETPARTICLEMESHFOLLOW(pcUnitData, "Bip01 Insec01", "InsecBody", MONSTERFOLLOWEFFECT_InsecBody);
		//	break;

		//case MONSTEREFFECTID_IGNICULUS:
		//	SETPARTICLEMESHFOLLOW(pcUnitData, "Bip01 Igniculus01", "IgniculusBody", MONSTERFOLLOWEFFECT_IgniculusBody);
		//	break;

		case MONSTEREFFECTID_Faugn:
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 Faugn01", "FaugnBody", MONSTERFOLLOWEFFECT_FaugnBody );
			break;

		case MONSTEREFFECTID_IceElemental:
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 Tail", "IceElementalBody", MONSTERFOLLOWEFFECT_IceElementalBody );
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 L Hand", "IceElementalHand", MONSTERFOLLOWEFFECT_IceElementalLeftHand );
			SETPARTICLEMESHFOLLOW( pcUnitData, "Bip01 R Hand", "IceElementalHand", MONSTERFOLLOWEFFECT_IceElementalRightHand );
			break;

		default:
			break;
	}

	return bRet;
}

void CGameEffectFollow::LoadParticles()
{
	//Abyss
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\ArhdyraBody.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\ArhdyraHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\ArhdyraSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\DeyHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\DeySkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\DeyStart.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\FaugnBody.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\FaugnHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\FaugnSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\GurkobHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\GurkobSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\GurkobStart.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\KoonHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\KoonSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\SathlaHit.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\SathlaSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\SathlaStart.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\YagdithaBody.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\YagdithaHit1.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\YagdithaHit2.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\YagdithaSkill.part" );
	PARTICLE->Load( "game\\scripts\\particles\\abyssmap\\YagdithaStart.part" );
	PARTICLE->Load( "game\\scripts\\particles\\IceElementalBody.part" );
	PARTICLE->Load( "game\\scripts\\particles\\IceElementalHand.part" );

	//Greedy Boss 110 map
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedyBody.part" );
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedyHit1.part" );
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedyHit2.part" );
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedySkill.part");
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedySkill2.part");
	PARTICLE->Load( "game\\scripts\\particles\\ancientweapon\\GreedyStart.part");

	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\NihilHit.part");
	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\NihilSkill.part");

	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\ChalybsStart.part");
	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\ChalybsSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\ChalybsHit.part");

	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\AceroStart.part");
	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\AceroSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\AceroHit.part");

	//113 Flame Monsters
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\IgniculusBody.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\IgniculusHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\IgniculusSkill.part");
											 
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\InsecBody.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\InsecHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\InsecSkill.part");
											  
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaArcherHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaArcherSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaArcherStart.part");
											  
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaArmorHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaArmorSkill.part");

	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaGiantHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaGiantSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaGiantStart.part");

	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaGolemHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\LavaGolemSkill.part");

	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamoHit1.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamoSkill.part");
	
	//Used for Ignis
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamemaidenStart.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamemaidenSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamemaidenHit.part");
	PARTICLE->Load("game\\scripts\\particles\\lavafield\\FlamemaidenBody.part");

	//108 map missing
	PARTICLE->Load("game\\scripts\\particles\\secretlab\\DraxosBody.part");
	PARTICLE->Load("game\\scripts\\particles\\secretlab\\DraxosHit.part");
	PARTICLE->Load("game\\scripts\\particles\\secretlab\\DraxosSkill1.part");
	PARTICLE->Load("game\\scripts\\particles\\secretlab\\DraxosSkill2.part");

	//Ignis and Yagditha
	PARTICLE->Load("game\\scripts\\particles\\abyssmap\\YagdithaHit1.part");
	PARTICLE->Load("game\\scripts\\particles\\abyssmap\\YagdithaHit2.part");
	PARTICLE->Load("game\\scripts\\particles\\abyssmap\\YagdithaSkill.part");
	PARTICLE->Load("game\\scripts\\particles\\abyssmap\\YagdithaStart.part");
	PARTICLE->Load("game\\scripts\\particles\\abyssmap\\YagdithaBody.part");

	//New Lizard Monsters
	PARTICLE->Load("game\\Effect\\Particle\\Script\\LizardElderHit.part");
	PARTICLE->Load("game\\Effect\\Particle\\Script\\LizardElderSkill.part");
}

BOOL CGameEffectFollow::StartMacro( UnitData * pcUnitData, char * pszMeshName, char * pszParticleName, int iID )
{
	if ( EffectBaseFollow::IsActiveEffect( pcUnitData, iID ) == FALSE )
	{
		int iIndex = EffectBaseFollow::GetFreeIndex();

		CGameEffectFollow * pc = new CGameEffectFollow();
		pc->Start( pcUnitData, pszMeshName, pszParticleName );

		EffectBaseFollow::GetData()[iIndex] = pc;
		EffectBaseFollow::GetData()[iIndex]->dwID = iID;

		return TRUE;
	}

	return FALSE;
}

NAKED int EffectBaseFollow::GetFreeIndex()
{
	JMP( pfnGetFreeIndex );
}

NAKED BOOL EffectBaseFollow::IsActiveEffect( UnitData * pcUnitData, DWORD dwID )
{
	JMP( pfnIsActiveEffect );
}

BOOL EffectBaseFollow::StopEffect( UnitData * pcUnitData, DWORD dwID )
{
	BOOL bRet = FALSE;

	for ( int i = 0; i < 5000; i++ )
	{
		EffectBaseFollow * pc = EffectBaseFollow::GetData()[i];
		if ( pc && pc->dwID == dwID && 
			((pc->pcUnitData == pcUnitData) || (pc->pcUnitDataDest == pcUnitData)) )
		{
			pc->iTime = pc->iTimeMax;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL EffectBaseFollow::MaintainEffect( UnitData * pcUnitData, DWORD dwID )
{
	BOOL bRet = FALSE;

	for ( int i = 0; i < 5000; i++ )
	{
		EffectBaseFollow * pc = EffectBaseFollow::GetData()[i];
		if ( pc && pc->dwID == dwID && 
			((pc->pcUnitData == pcUnitData) || (pc->pcUnitDataDest == pcUnitData)) )
		{			
			pc->iTime = 0;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL EffectBaseFollow::DeleteEffect( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	for ( int i = 0; i < 5000; i++ )
	{
		EffectBaseFollow * pc = EffectBaseFollow::GetData()[i];

		if ( pc )
		{
			if ( pc->iTime < pc->iTimeMax )
			{
				if ( (pc->pcUnitData == pcUnitData) || (pc->pcUnitDataDest == pcUnitData) )
				{
					pc->iTime = pc->iTimeMax;

					bRet = TRUE;
				}
			}
		}
	}

	return bRet;
}

void EffectBaseFollow::MainEffect()
{
	for ( int i = 0; i < 5000; i++ )
	{
		EffectBaseFollow * pc = EffectBaseFollow::GetData()[i];

		if ( pc )
		{
			pc->iTime++;

			if ( (pc->iTime >= pc->iTimeMax) && (pc->iTimeMax > 0) )
			{
				SAFE_DELETE( pc );
				EffectBaseFollow::GetData()[i] = NULL;
				continue;
			}

			pc->Main();

			if ( pc->iTime > 0 )
			{
				if ( pc->pcUnitData && (pc->pcUnitData->bActive && pc->pcUnitData->bVisible) && UNITDATA )
				{
					if (pc->pcUnitData->sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_XY_24_meters))
						CALL_WITH_ARG1( 0x005356D0, (DWORD)pc );
				}
			}

			if ( pc->iTime >= pc->iTimeMax )
			{
				SAFE_DELETE( pc );
				EffectBaseFollow::GetData()[i] = NULL;
				continue;
			}
		}
	}
}

NAKED void EffectBaseFollowTrace::AddData( Point3D * psPosition )
{
	JMP( 0x00535620 );
}
