#include "stdafx.h"
#include "CParticleDebug.h"


CParticleDebug::CParticleDebug( CParticleEngine * _pcParticleEngine )
{
	pcParticleEngine	= _pcParticleEngine;

	bDisplay			= FALSE;
}

CParticleDebug::~CParticleDebug()
{
	//DEBUG( "~CParticleDebug()" );

	pcParticleEngine	= NULL;

	bDisplay			= FALSE;
}

void CParticleDebug::Render()
{
	if( bDisplay )
	{

	}
}