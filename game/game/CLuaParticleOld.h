#pragma once

class CEffectGroupLua
{
public:
	char						cPadding001[0x2C];
};

class CEffectModelControllerLua
{
public:
	char						cPadding001[0x50];
};

class CEffectMainBaseLua
{
private:
	char						cPadding001[0x18];
	void						Constructor();
public:
	CEffectMainBaseLua() { Constructor(); };

	void						Init( CEffectGroupLua * pcGroup, int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ );
};

class CEffectUnitFollowLua : public CEffectMainBaseLua
{
private:
	bool						bIsMyCharacter;
	CEffectGroupLua				* pcGroup;
	float						fHeight;
	float						fTime;

	int							iParticleID;
	bool						bEndParticle;

	void						Constructor();
public:
	CEffectUnitFollowLua()		{ Constructor(); };

	void						Init( UnitData * pcUnitData, CEffectGroupLua * pcGroup, float fTime, float fHeight = 0.0f );

	void						SetParticleID( int i ) { iParticleID = i; };
};

class CLuaParticleOld
{
private:
	CEffectGroupLua				* pcGroup;
	CEffectModelControllerLua	* pcModel;
	int							iType;
	int							iSpecificType;
	char						cPadding001[4];

public:
	CLuaParticleOld();
	~CLuaParticleOld();

	static CLuaParticleOld		* GetInstance() { return (CLuaParticleOld*)0x03916BB0; }

	CEffectGroupLua				* Load( const char * pszFileName );
};

class CEffectManagerHandlerLua
{
public:
	static CEffectManagerHandlerLua * GetInstance();

	void						AddEffect( CEffectMainBaseLua * pcEffect );

	void						Clear();
};

