#pragma once

#define	MAX_WEAPON_FORCE_POS	10
class ForceOrbEffectFollow : public EffectBaseFollow
{
public:
	ForceOrbEffectFollow();
	~ForceOrbEffectFollow();

	void	Start( UnitData * pCharacter, int timeCount, int shellTomCode );
	void	Main() override;
	void	Draw( Point3D * psPosition, Point3D * psAngle ) override;

private:
	struct ForceOrbEffectWeaponPosition
	{
		Point3D				saPosition[MAX_WEAPON_FORCE_POS];

		ForceOrbEffectWeaponPosition( const Point3D _saPosition[MAX_WEAPON_FORCE_POS] )
		{
			CopyMemory( saPosition, _saPosition, sizeof( Point3D ) * _countof( saPosition ) );
		};

		~ForceOrbEffectWeaponPosition() {};
	};

	BOOL	EndFlag;
	BOOL	MyCharacterFlag;
	//int		TimeCount;
	int		MaterialNum;

	int		Width;
	BOOL	ScaleFlag;
	int		ScaleWidth;

	int		WeaponPosCount;
	Point3D	WeaponPos[MAX_WEAPON_FORCE_POS];

	int			GlowWidth;
	int			GlowPosIndex;
	BOOL		GlowFlag;
	EXEFace2D	GlowFace;

	std::vector<ForceOrbEffectWeaponPosition> vWeaponsPosition;
};

void __cdecl AddForceOrbCharacterEffect( UnitData * pcUnitData, int iTime, int iID );
