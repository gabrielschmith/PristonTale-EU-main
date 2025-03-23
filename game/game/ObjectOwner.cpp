#include "stdafx.h"
#include "ObjectOwner.h"

namespace Engine
{

//ABSTRACT	
ObjectOwner::ObjectOwner()
{
	eType		= OBJECTOWNERTYPE_None;
}

ObjectOwner::ObjectOwner( ObjectOwner & c )
{
	eType		= c.eType;

	cPosition.x	= c.cPosition.x;
	cPosition.y	= c.cPosition.y;
	cPosition.z	= c.cPosition.z;
}

ObjectOwner::~ObjectOwner()
{
}

//UNIT
ObjectOwnerUnit::ObjectOwnerUnit( Unit * _pcUnit ) : ObjectOwner()
{
	eType	= OBJECTOWNERTYPE_Unit;

	lID		= _pcUnit->iID;
	pcUnit	= _pcUnit;
}

ObjectOwnerUnit::ObjectOwnerUnit( ObjectOwnerUnit & c )
{
	eType	= c.eType;

	lID		= c.lID;
	pcUnit	= c.pcUnit;

	for( vector<EAnimationType>::iterator it = c.vAnimationTypeLostConditions.begin(); it != c.vAnimationTypeLostConditions.end(); it++ )
	{
		EAnimationType eAnimationType = *it;

		vAnimationTypeLostConditions.push_back( eAnimationType );
	}
}

ObjectOwnerUnit::~ObjectOwnerUnit()
{
	pcUnit	= NULL;
}

ObjectOwnerUnit * ObjectOwnerUnit::Clone()
{
	ObjectOwnerUnit * pClone = new ObjectOwnerUnit( *this );

	return pClone;
}

void ObjectOwnerUnit::Update()
{
	Point3D & p	= pcUnit->GetPosition();

	cPosition.x	= p.iX;
	cPosition.y	= p.iY;
	cPosition.z	= p.iZ;
}

BOOL ObjectOwnerUnit::Lost( DWORD dwTickcount )
{
	if( pcUnit == NULL )
		return TRUE;

	if( pcUnit->GetID() != lID )
		return TRUE;

	UnitData * pcUnitData = pcUnit->pcUnitData;

	if( pcUnitData->bActive == FALSE )
		return TRUE;

	if( pcUnitData != UNITDATA && (pcUnitData->dwLastActiveTime + 16000) < dwTickcount )
		return TRUE;

	if( pcUnitData->psModelAnimation == NULL )
		return TRUE;
	
	for( vector<EAnimationType>::iterator it = vAnimationTypeLostConditions.begin(); it != vAnimationTypeLostConditions.end(); it++ )
	{
		EAnimationType eAnimationType = *it;

		if( pcUnitData->psModelAnimation->iType == eAnimationType )
			return TRUE;
	}

	return FALSE;
}

ObjectOwnerProjectile::ObjectOwnerProjectile( Unit * _pcUnit ) : ObjectOwner()
{
	eType = OBJECTOWNERTYPE_Projectile;

	lID = _pcUnit->iID;
	pcUnit = _pcUnit;
}

ObjectOwnerProjectile::ObjectOwnerProjectile( ObjectOwnerProjectile & c )
{
	eType = c.eType;

	lID = c.lID;
	pcUnit = c.pcUnit;
}

ObjectOwnerProjectile::~ObjectOwnerProjectile()
{
	pcUnit = NULL;
}

ObjectOwnerProjectile * ObjectOwnerProjectile::Clone()
{
	ObjectOwnerProjectile * pClone = new ObjectOwnerProjectile( *this );

	return pClone;
}

void ObjectOwnerProjectile::Update()
{
	Point3D & p = pcUnit->GetPosition();

	cPosition.x = p.iX;
	cPosition.y = p.iY;
	cPosition.z = p.iZ;
}

BOOL ObjectOwnerProjectile::Lost( DWORD dwTickcount )
{
	if( pcUnit == NULL )
		return TRUE;

	if( pcUnit->GetID() != lID )
		return TRUE;

	if( pcUnit->pcUnitData->bActive == FALSE )
		return TRUE;

	if( pcUnit->pcUnitData->psModelAnimation == NULL )
		return TRUE;

	return FALSE;
}

ObjectOwnerPosition::ObjectOwnerPosition( Point3D s )
{
	cPosition.x = s.iX; cPosition.y = s.iY; cPosition.z = s.iZ; eType = OBJECTOWNERTYPE_None;
}

ObjectOwnerPosition::ObjectOwnerPosition( ObjectOwnerPosition & c )
{
	eType		= c.eType;
	cPosition.x = c.cPosition.x;
	cPosition.y = c.cPosition.y;
	cPosition.z = c.cPosition.z;
}

ObjectOwnerPosition::~ObjectOwnerPosition()
{
}

ObjectOwnerPosition * ObjectOwnerPosition::Clone()
{
	ObjectOwnerPosition * pClone = new ObjectOwnerPosition( *this );

	return pClone;
}

void ObjectOwnerPosition::Update()
{
}

BOOL ObjectOwnerPosition::Lost( DWORD dwTickcount )
{
	return FALSE;
}

}