#include "stdafx.h"
#include "EffectGame.h"


NAKED void EffectGame::FireMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay, BOOL bSmallMeteor )
{
	JMP( pfnFireMeteorite );
}

NAKED void EffectGame::RegenerationFieldCelestial( UnitData * pcUnitData, float fTime )
{
	JMP( pfnRegenerationFieldCelestial );
}

NAKED void EffectGame::InertiaTarget( const char * pszParticleName, UnitData * pcUnitData, int iCount, int iID )
{
	JMP( pfnInertiaTarget );
}
