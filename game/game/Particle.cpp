#include "stdafx.h"
#include "Particle.h"


struct stParticle
{
	std::string					strParticleName;
};


// Vector with objects of particle
std::vector<stParticle*> vParticleControl;


CParticleOld::CParticleOld()
{
}

int CParticleOld::load( const char * File )
{
	// Load the particle and return the pointer in struct created
	typedef void( __thiscall *t_LoadParticle ) ( UINT uPtrMemory, const char * File );
	t_LoadParticle LoadParticle = ( t_LoadParticle )0x0059C820;

	// If file not exists, write game.log error
	if ( !FILEEXIST( File ) )
	{

		return -1;
	}

	LoadParticle( 0x039032A4, File );

	stParticle * pPart = new stParticle;
	ZeroMemory( pPart, sizeof ( stParticle ) );
	// Create class Pointer
	vParticleControl.push_back( pPart );

	int iVecPtr = vParticleControl.size() - 1;

	// Get File name without extension
	vParticleControl[ iVecPtr ]->strParticleName = File;
	const size_t last_slash_idx = vParticleControl[ iVecPtr ]->strParticleName.find_last_of( "\\/" );
	if ( std::string::npos != last_slash_idx )
		vParticleControl[ iVecPtr ]->strParticleName.erase( 0, last_slash_idx + 1 );

	// Remove extension if present.
	const size_t period_idx = vParticleControl[ iVecPtr ]->strParticleName.rfind( '.' );
	if ( std::string::npos != period_idx )
		vParticleControl[ iVecPtr ]->strParticleName.erase( period_idx );

	// Return id
	return vParticleControl.size() - 1;
}

int CParticleOld::Load( const char * File )
{
	return this->load( File );
}


int CParticleOld::start( const int ParticleID, int X, int Y, int Z )
{
	// type define fution
	typedef int( __thiscall *t_StartParticle ) ( UINT uPtrParticles, const char * szParticleName, int iX, int iY, int iZ, int v0 );
	t_StartParticle StartParticle = ( t_StartParticle )0x0059BF40;

	// if its more than vector, do not exists
	int iSize = vParticleControl.size( );
	if ( ParticleID >= iSize || ParticleID < 0 )
		return -1;

	// Get Particle Info Pointer and Start it
	return StartParticle( 0x039032CC, vParticleControl[ ParticleID ]->strParticleName.c_str( ), X, Y, Z, 0 );
}

int CParticleOld::Start( const int ParticleID, int X, int Y, int Z )
{
	return this->start( ParticleID, X, Y, Z );
}

/// <summary>
/// 2012 source: g_NewParticleMgr.Start
/// </summary>
int CParticleOld::Start( const char * pszName, int X, int Y, int Z )
{
	typedef int( __thiscall *t_StartParticle ) (UINT uPtrParticles, const char * szParticleName, int iX, int iY, int iZ, int v0);
	t_StartParticle StartParticle = (t_StartParticle)0x0059BF40;

	return StartParticle( 0x039032CC, pszName, X, Y, Z, 0 );
}


void CParticleOld::SetPosition( int PtrIDParticle, D3DVECTOR pos )
{
	// if PtrIDParticle == -1, its not exists
	if ( PtrIDParticle == -1 )
		return;
	
	// Type define function
	typedef void( __thiscall *t_SetPositionParticle ) ( UINT uMemoryPtr, int iPtrIDParticle, D3DVECTOR vpos );
	t_SetPositionParticle SetPositionParticle = ( t_SetPositionParticle )0x005989F0;

	// Set position of particle
	SetPositionParticle( 0x039032CC, PtrIDParticle, pos );
}

void CParticleOld::SetAttachPosition( int PtrIDParticle, D3DVECTOR pos )
{
	// if PtrIDParticle == -1, its not exists
	if ( PtrIDParticle == -1 )
		return;
	
	// Type define function
	typedef void( __thiscall *t_SetPositionParticle ) ( UINT uMemoryPtr, int iPtrIDParticle, D3DVECTOR vpos );
	t_SetPositionParticle SetPositionParticle = ( t_SetPositionParticle )0x00598AE0;

	// Set position of particle
	SetPositionParticle( 0x039032CC, PtrIDParticle, pos );
}

void CParticleOld::SetAngle( int iPtr, Point3D angle )
{
	typedef void( __thiscall *tfnSetAngle )(UINT uMem, int iptr, Point3D angle);
	tfnSetAngle fnSetAngle = (tfnSetAngle)0x00598A50;
	fnSetAngle( 0x039032CC, iPtr, angle );
}

void CParticleOld::Stop( int iStartID )
{
	__asm
	{
		PUSHAD;
		MOV EAX, iStartID;
		PUSH EAX;
		MOV ECX, 0x039032CC;
		MOV EAX, 0x00598B80;
		CALL EAX; 
		MOV EAX, iStartID;
		PUSH EAX;
		MOV ECX, 0x039032CC;
		MOV EAX, 0x00598B40;
		CALL EAX;
		MOV EAX, iStartID;
		PUSH 0;
		PUSH EAX;
		MOV ECX, 0x039032CC;
		MOV EAX, 0x00598BC0;
		CALL EAX;
		POPAD;
	}
}

BOOL CParticleOld::IsLost( ParticleDataOld * ps )
{
	if ( ps && ps->pcUnitData )
	{
		UnitData * pcUnitData = ps->pcUnitData;

		if ( pcUnitData->iID != ps->iID )
			return TRUE;

		if ( (pcUnitData->dwLastActiveTime + 16000) < TICKCOUNT && pcUnitData != UNITDATA )
			return TRUE;

		if ( pcUnitData->psModelAnimation == NULL )
			return TRUE;

		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
			return TRUE;
	}

	return FALSE;
}


BOOL CParticleOld::IsLost( ParticleDataOldItem * ps )
{
	if ( ps && ps->pcDropItemView )
	{
		DropItemView * pcDropItemView = ps->pcDropItemView;

		if ( pcDropItemView->iItemID.ToInt() != ps->iID )
			return TRUE;

		if ( ( *(DWORD *)0x00CF4750 ) > ( pcDropItemView->dwLastUpdate + ITEM_TRANS_LIMIT_TIME ) ) //ITEM_TRANS_LIMIT_TIME = 8s
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CParticleOld::Update( float fTime )
{
	for ( std::vector<ParticleDataOld*>::iterator it = vParticleData.begin(); it != vParticleData.end(); )
	{
		ParticleDataOld * ps = (*it);

		if ( ps )
		{
			if ( ps->fTime <= 0.0f || IsLost( ps ) )
			{
				Stop( ps->iParticleUseID );
				SAFE_DELETE( ps );
				it = vParticleData.erase( it );
				continue;
			}

			if ( ps->pcUnitData )
			{
				D3DVECTOR sVec = D3DVECTOR{ (float)ps->pcUnitData->sPosition.iX, (float)ps->pcUnitData->sPosition.iY, (float)ps->pcUnitData->sPosition.iZ };

				if ( ps->bWeapon )
				{
					Point3D sPosition;
					SKILLMANAGERHANDLER->GetWeaponPositionNew( ps->pcUnitData, sPosition, ps->fScale );
					sVec.x = (float)sPosition.iX;
					sVec.y = (float)sPosition.iY;
					sVec.z = (float)sPosition.iZ;
				}

				if ( ps->bFixed == FALSE )
				{
					if ( IsRunning( ps->iParticleUseID ) )
					{
						SetRendering( ps->iParticleUseID, TRUE );
						SetPosition( ps->iParticleUseID, sVec );
					}
				}
				else
				{
					if ( IsRunning( ps->iParticleUseID ) )
					{
						SetRendering( ps->iParticleUseID, TRUE );
						SetAttachPosition( ps->iParticleUseID, sVec );
					}
				}

				it++;
			}
			else
				it++;

			ps->fTime--;
		}
		else
		{
			//ps is NULL here
			it = vParticleData.erase( it );
		}
	}

	for ( std::vector<ParticleDataOldItem*>::iterator it = vParticleDataItem.begin(); it != vParticleDataItem.end(); )
	{
		ParticleDataOldItem * ps = (*it);

		if ( ps )
		{
			if ( ps->fTime <= 0.0f || IsLost( ps ) )
			{
				Stop( ps->iParticleUseID );
				SAFE_DELETE( ps );
				it = vParticleDataItem.erase( it );
				continue;
			}

			if ( ps->pcDropItemView )
			{
				D3DVECTOR sVec = D3DVECTOR{ (float)ps->pcDropItemView->sSelfPosition.iX, (float)ps->pcDropItemView->sSelfPosition.iY, (float)ps->pcDropItemView->sSelfPosition.iZ };

				if ( ps->bFixed == FALSE )
				{
					if ( IsRunning( ps->iParticleUseID ) )
					{
						SetRendering( ps->iParticleUseID, TRUE );
						SetPosition( ps->iParticleUseID, sVec );
					}
				}
				else
				{
					if ( IsRunning( ps->iParticleUseID ) )
					{
						SetRendering( ps->iParticleUseID, TRUE );
						SetAttachPosition( ps->iParticleUseID, sVec );
					}
				}

				it++;
			}
			else
				it++;

			ps->fTime--;
		}
		else
		{
			//ps is NULL here
			it = vParticleDataItem.erase( it );
		}
	}
}

void CParticleOld::CheckAndStartCastScript( Unit * pcUnitTarget, int iScriptId, BYTE iLevel1Based, BYTE bUnitTimerLevel1Based, BOOL bFixed, BOOL bWeapon, float fScale, int iYOffset)
{	
	if ( iLevel1Based > bUnitTimerLevel1Based )
	{
		if ( !PARTICLE->IsRunning( pcUnitTarget->GetID(), iScriptId ) )
		{
			PARTICLE->StartScript( iScriptId, pcUnitTarget->pcUnitData, 5, FALSE, TRUE, fScale, iYOffset ); //todo - understand why bFixed need to be FALSE and bWeapon need to be T RUE for y offset to work..
		}
	}
}


void CParticleOld::CheckAndStartCastScript( DropItemView * pcDropItemView, int iScriptId, BYTE iLevel1Based, BYTE bUnitTimerLevel1Based, BOOL bFixed, BOOL bWeapon, float fScale, int iYOffset)
{	
	if ( iLevel1Based > bUnitTimerLevel1Based )
	{
		if ( !PARTICLE->IsRunning( pcDropItemView->iItemID.ToInt(), iScriptId ) )
		{
			PARTICLE->StartScript( iScriptId, pcDropItemView, 5, FALSE, TRUE, fScale, iYOffset ); //todo - understand why bFixed need to be FALSE and bWeapon need to be T RUE for y offset to work..
		}
	}
}

void CParticleOld::CheckAndStartKeepScript( Unit * pcUnitTarget, int iScriptId, USHORT iTimeLeftSec, BYTE iLevel1Based, BYTE & bUnitTimerLevel1Based, BOOL bFixed, BOOL bWeapon, float fScale, int iYOffset  )
{
	if ( iTimeLeftSec > 0 && iLevel1Based >= 1 && iLevel1Based <= 10)
	{	
		if ( PARTICLE->IsRunning( pcUnitTarget->GetID(), iScriptId ) )
		{
			PARTICLE->Update( pcUnitTarget->GetID(), iScriptId, iTimeLeftSec );
		}
		else
		{
			PARTICLE->StartScript( iScriptId, pcUnitTarget->pcUnitData, iTimeLeftSec, bFixed, bWeapon, fScale, iYOffset );
		}

		bUnitTimerLevel1Based = iLevel1Based;		
	}
	else
	{
		if ( bUnitTimerLevel1Based > 0 )
		{
			PARTICLE->Delete ( pcUnitTarget->GetID (), iScriptId );	

			bUnitTimerLevel1Based = 0;
		}		
	}
}

void CParticleOld::StartScript( int iScriptID, UnitData * pcUnitData, float fTimeLeft, BOOL bFixed, BOOL bWeapon, float fScale, int iYOffset )
{
	int iID = PARTICLE->Start(iScriptID, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + iYOffset, pcUnitData->sPosition.iZ );
	PARTICLE->AddScript( iID, iScriptID, pcUnitData, fTimeLeft, bFixed, bWeapon, fScale );
}


void CParticleOld::StartScript( int iScriptID, DropItemView * pcDropItemView, float fTimeLeft, BOOL bFixed, BOOL bWeapon, float fScale, int iYOffset )
{
	int iID = PARTICLE->Start(iScriptID, pcDropItemView->sSelfPosition.iX, pcDropItemView->sSelfPosition.iY + iYOffset, pcDropItemView->sSelfPosition.iZ );
	PARTICLE->AddScript( iID, iScriptID, pcDropItemView, fTimeLeft, bFixed, bWeapon, fScale );
}


void CParticleOld::AddScript( int iID, int iParticleID, UnitData * pcUnitData, float fTimeLeft, BOOL bFixed, BOOL bWeapon, float fScale )
{
	ParticleDataOld * ps = new ParticleDataOld;

	ps->iParticleUseID = iID;
	ps->iParticleID = iParticleID;
	ps->pcUnitData	= pcUnitData;
	ps->bFixed		= bFixed;
	ps->bWeapon		= bWeapon;
	ps->fScale		= fScale;
	ps->iID			= pcUnitData->iID;
	ps->fTime		= fTimeLeft * 70.0f;


	vParticleData.push_back( ps );
}

void CParticleOld::AddScript( int iID, int iParticleID, DropItemView * pcDropItemView, float fTimeLeft, BOOL bFixed, BOOL bWeapon, float fScale )
{
	ParticleDataOldItem * ps = new ParticleDataOldItem;

	ps->iParticleUseID	= iID;
	ps->iParticleID		= iParticleID;
	ps->pcDropItemView	= pcDropItemView;
	ps->bFixed			= bFixed;
	ps->bWeapon			= bWeapon;
	ps->fScale			= fScale;
	ps->iID				= pcDropItemView->iItemID.ToInt();
	ps->fTime			= fTimeLeft * 70.0f;

	//Need to make sure this does something
	vParticleDataItem.push_back( ps );
}

void CParticleOld::Update ( int iUnitDataID, int iParticleID, float fTimeLeft)
{
	for ( ParticleDataOld * ps : vParticleData )
	{
		if ( ps->iID == iUnitDataID && ps->iParticleID == iParticleID )
		{
			ps->fTime = fTimeLeft * 70.0f;
		}
	}
}

void CParticleOld::UpdateItem ( int iDropItemViewID, int iParticleID, float fTimeLeft)
{
	for ( ParticleDataOldItem * ps : vParticleDataItem )
	{
		if ( ps->iID == iDropItemViewID && ps->iParticleID == iParticleID )
		{
			ps->fTime = fTimeLeft * 70.0f;
		}
	}
}

BOOL CParticleOld::IsRunning ( int iUnitDataID, int iParticleID )
{
	for ( ParticleDataOld * ps : vParticleData )
	{
		if ( ps->iID == iUnitDataID && ps->iParticleID == iParticleID )
		{
			return TRUE;
		}
	}


	return FALSE;
}

BOOL CParticleOld::IsRunningItem ( int iDropItemViewID, int iParticleID )
{
	for ( ParticleDataOldItem * ps : vParticleDataItem )
	{
		if ( ps->iID == iDropItemViewID && ps->iParticleID == iParticleID )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CParticleOld::Delete( int iUnitDataID, int iParticleID )
{
	for ( std::vector<ParticleDataOld*>::iterator it = vParticleData.begin(); it != vParticleData.end(); )
	{
		ParticleDataOld * ps = (*it);

		if ( ps )
		{
			if ( ps->iID == iUnitDataID && ps->iParticleID == iParticleID )
			{
				Stop( ps->iParticleUseID );
				SAFE_DELETE( ps );
				it = vParticleData.erase( it );
				continue;
			}
		}
		else
		{
			Stop( ps->iParticleUseID );
			SAFE_DELETE( ps );
			it = vParticleData.erase( it );
			continue;
		}

		it++;
	}
	return FALSE;
}

BOOL CParticleOld::DeleteItem( int iUnitDataID, int iParticleID )
{
	for ( std::vector<ParticleDataOldItem*>::iterator it = vParticleDataItem.begin(); it != vParticleDataItem.end(); )
	{
		ParticleDataOldItem * ps = (*it);

		if ( ps )
		{
			if ( ps->iID == iUnitDataID && ps->iParticleID == iParticleID )
			{
				Stop( ps->iParticleUseID );
				SAFE_DELETE( ps );
				it = vParticleDataItem.erase( it );
				continue;
			}
		}
		else
		{
			//Note - ps is NULL Here
			it = vParticleDataItem.erase( it );
			continue;
		}

		it++;
	}

	return FALSE;
}

NAKED void CParticleOld::StartEffectParticleSkill( char * pszParticle, UnitData * pcTarget, int iSkillID )
{
	JMP( pfnSetParticleSkillEffect );
}

void CParticleOld::SetRendering( int iPtr, int iShow )
{
	typedef void( __thiscall *tfnSetRendering )(UINT uMem, int iptr, int is);
	tfnSetRendering fnSetRendering = (tfnSetRendering)0x00598BC0;
	fnSetRendering( 0x039032CC, iPtr, iShow );
}

bool CParticleOld::IsRunning( int iPtr )
{
	return (bool)CALLT_WITH_ARG1( 0x00598AB0, 0x039032CC, iPtr );
}

CParticleOld::~CParticleOld()
{
}