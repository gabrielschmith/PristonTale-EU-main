#include "stdafx.h"
#include "CParticleEvent.h"

CParticleEvent::CParticleEvent( double f ) : fTime(f)
{
}

CParticleEvent::CParticleEvent( CParticleEvent & c )
{
	fTime = c.fTime;
}

CParticleEvent::~CParticleEvent()
{
	//DEBUG( "~CParticleEvent()" );
}

CParticleEventVelocity::CParticleEventVelocity( CParticle::Point v, double f ) : sVelocity(v), CParticleEvent( f )
{
}

CParticleEventVelocity::CParticleEventVelocity( CParticleEventVelocity & c ) : CParticleEvent( c )
{
	sVelocity = c.sVelocity;
}

CParticleEventVelocity::~CParticleEventVelocity()
{
	//DEBUG( "~CParticleEventVelocity()" );
}

CParticleEventVelocity * CParticleEventVelocity::Clone()
{
	CParticleEventVelocity * pcClone = new CParticleEventVelocity( *this );

	return pcClone;
}

void CParticleEventVelocity::Apply( CParticle * pcParticle )
{
	sVelocity.Birth();

	pcParticle->SetVelocity( sVelocity );
}

CParticleEventSize::CParticleEventSize( CParticle::Size s, double f ) : sSize(s), CParticleEvent( f )
{
}

CParticleEventSize::CParticleEventSize( CParticleEventSize & c ) : CParticleEvent( c )
{
	sSize = c.sSize;
}

CParticleEventSize::~CParticleEventSize()
{
	//DEBUG( "~CParticleEventSize()" );
}

CParticleEventSize * CParticleEventSize::Clone()
{
	CParticleEventSize * pcClone = new CParticleEventSize( *this );

	return pcClone;
}

void CParticleEventSize::Apply( CParticle * pcParticle )
{
	sSize.Birth();

	pcParticle->SetSize( sSize );
}