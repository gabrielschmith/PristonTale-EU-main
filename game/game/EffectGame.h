#pragma once
class EffectGame
{
private:
	IMPFNC							pfnFireMeteorite				= 0x005B9B30;
	IMPFNC							pfnRegenerationFieldCelestial	= 0x005D4850;
	IMPFNC							pfnInertiaTarget				= 0x005B6EC0;
public:
	
	static void						FireMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay, BOOL bSmallMeteor );

	static void						RegenerationFieldCelestial( UnitData * pcUnitData, float fTime );

	static void						InertiaTarget( const char * pszParticleName, UnitData * pcUnitData, int iCount = 5, int iID = SKILLID_Inertia );

};

