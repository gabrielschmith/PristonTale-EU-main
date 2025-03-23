#pragma once


#include "CParticleObject.h"
#include "CParticle.h"

class CParticleDebug;
class CParticleFactory;
class CTextureManager;

class CParticleSystem
{
	friend class CParticleDebug;
	friend class CParticleEmitter;
	friend class CParticleFactory;

public:
						  CParticleSystem();
						  CParticleSystem( CParticle * _pcParticle );
						  CParticleSystem( CParticleSystem & c );
	virtual				 ~CParticleSystem();

	void				  Birth();

	BOOL				  Update( double fTime );

	void				  Render( class EXETextureManager * pcTextureManager );

	void				  Pause() { bPaused = TRUE; }
	void				  Continue() { bPaused = FALSE; }

	void				  Kill( BOOL bForce );

	std::string				  GetName() { return strName; }

	UINT				  GetOwnerID();
	void				  SetOwner( Engine::ObjectOwner * pNewOwner );

	void				  SetID( UINT _uID ) { uID = _uID; }
	UINT				  GetID() { return uID; }

	CParticleEmitter	* GetParticleEmitter() { return pcParticleEmitter; }
	CParticle			* GetParticle() { return pcParticle; }

private:
	void				  Add( CParticle * pcParticle );

	UINT				  ParticlesAlive() { return vActiveParticles.size(); }

private:
	std::string				  strName;

	CParticleEmitter	* pcParticleEmitter;

	CParticleObject		* pcParticleObject;

	CParticle			* pcParticle;

	std::vector<CParticle*>	  vActiveParticles;

	Engine::ObjectOwner	* pOwner;

	BOOL				  bKill;

	BOOL				  bPaused;

	UINT				  uID;

};