#pragma once

struct ParticleDataOld
{
	int						iID;
	int						iParticleUseID;
	int						iParticleID;

	BOOL					bFixed;
	BOOL					bWeapon;

	float					fScale;

	UnitData				* pcUnitData;

	float					fTime;
};

struct ParticleDataOldItem
{
	int						iID;
	int						iParticleUseID;
	int						iParticleID;

	BOOL					bFixed;
	BOOL					bWeapon;

	float					fScale;

	DropItemView			* pcDropItemView;

	float					fTime;
};


class CParticleOld
{
private:
	IMPFNC						pfnSetParticleSkillEffect = 0x005B6F60;

	std::vector<ParticleDataOld*>		vParticleData;
	std::vector<ParticleDataOldItem*>   vParticleDataItem;

	int							load( const char * File );
	int							start( const int ParticleID, int X, int Y, int Z );
public:
	CParticleOld();
	int							Load( const char * File );
	int							Start( const int ParticleID, int X, int Y, int Z );
	int							Start( const char * pszName, int X, int Y, int Z );
	void 						SetAttachPosition( int PtrIDParticle, D3DVECTOR pos );
	void 						SetPosition( int PtrIDParticle, D3DVECTOR pos );
	void						SetAngle( int iPtr, Point3D angle );

	void						SetRendering( int iPtr, int iShow );

	bool						IsRunning( int iPtr );

	void						Stop( int iStartID );

	BOOL						IsLost( ParticleDataOld * ps );
	BOOL						IsLost( ParticleDataOldItem * ps );

	void						Update( float fTime );
	void						Update ( int iUnitDataID, int iParticleID, float fTimeLeft );
	void						UpdateItem ( int iUnitDataID, int iParticleID, float fTimeLeft );


	void						CheckAndStartCastScript (Unit * pcUnitTarget, int iScriptId, BYTE iLevel1Based, BYTE iUnitTimerLevel1Based, BOOL bFixed = TRUE, BOOL bWeapon = FALSE, float fScale = 0.0f, int iYOffset = 0 );
	void						CheckAndStartCastScript (DropItemView * pcDropItemView, int iScriptId, BYTE iLevel1Based, BYTE iUnitTimerLevel1Based, BOOL bFixed = TRUE, BOOL bWeapon = FALSE, float fScale = 0.0f, int iYOffset = 0 );

	void						CheckAndStartKeepScript (Unit * pcUnitTarget, int iScriptId, USHORT iTimeLeftSec, BYTE iLevel1Based, BYTE & iUnitTimerLevel1Based, BOOL bFixed = TRUE, BOOL bWeapon = FALSE, float fScale = 0.0f, int iYOffset = 0 );

	void						StartScript( int iScriptID, UnitData * pcUnitData = NULL, float fTimeLeft = 1.0f, BOOL bFixed = FALSE, BOOL bWeapon = FALSE, float fScale = 0.0f, int iYOffset = 0 );
	void						StartScript ( int iScriptID, DropItemView * pcDropItemView = NULL, float fTimeLeft = 1.0f, BOOL bFixed = FALSE, BOOL bWeapon = FALSE, float fScale = 0.0f, int iYOffset = 0 );

	void						AddScript( int iID, int iParticleID, UnitData * pcUnitData = NULL, float fTimeLeft = 1.0f, BOOL bFixed = FALSE, BOOL bWeapon = FALSE, float fScale = 0.0f );
	void						AddScript( int iID, int iParticleID, DropItemView * pcDropItemView = NULL, float fTimeLeft = 1.0f, BOOL bFixed = FALSE, BOOL bWeapon = FALSE, float fScale = 0.0f );

	BOOL						IsRunning ( int iUnitDsataID, int iParticleID );
	BOOL						IsRunningItem ( int iDropItemViewID, int iParticleID );

	BOOL						Delete( int iUnitDataID, int iParticleID );
	BOOL						DeleteItem( int iUnitDataID, int iParticleID );

	static void					StartEffectParticleSkill( char * pszParticle, UnitData * pcTarget, int iSkillID = 0 );

	~CParticleOld();
};

typedef std::shared_ptr<CParticleOld> ParticleOld_ptr;
typedef CParticleOld ParticleOld;