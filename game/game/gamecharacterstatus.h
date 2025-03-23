#pragma once

struct CharacterDataLocal
{
	char						cPadding001[0x14];
	
	int							iAttackRatingItems;			//0x0014

	char						cPadding001a[0x18];			//0x0018

	int							iDefenseSkills;				//0x0030
	
	char						cPadding002[0x10];			//0x0034

	float						fAbsorbAdd;					//0x0044

	float						fIncreaseHPAdd;				//0x0048

	char						cPadding002b[0xC];			//0x004C
	
	float						fRegenMP;					//0x0058

	float						fRegenHP;					//0x005C

	int							iAttackSpeedAdd;			//0x0060

	char						cPadding002bb[0x10];		//0x0064

	float						fRegenSP;					//0x0074

	int							iLevelMP;					//0x0078

	char						fPercentSPAdd;				//0x007C

	float						fIncreaseMPAdd;				//0x0080

	int							iWeaponSpeed;				//0x0084

	int							iCriticalItems;				//0x0088

	int							iLevelHP;					//0x008C

	float 						fBlockRatingAdd;			//0x0090

	int							iDefenseAdd;				//0x0094

	int							iCountAbsorbItems;			//0x0098

	char						iSkillWeaponSpeed;			//0x009C

	int							iAttackRange;				//0x00A0

	float						fMovementSpeed;				//0x00A4

	char						cPadding003[0x38];			//0x00A8

	int							iViewSight;					//0x00E0

	int							iLevelAttackRating;			//0x00E4

	float						fPercentHPAdd;				//0x00E8

	float						fBlockRating;				//0x00EC

	int							iCriticalSkills;			//0x00F0

	int							iUnknown;					//0x00F4

	float 						fAbsorbSkills;				//0x00F8

	float						fMovementSpeedAdd;			//0x00FC

	float						fPercentMPAdd;				//0x0100

	float						fIncreaseSPAdd;				//0x0104

	int							iAttackRangeAdd;			//0x0108

	char						cPadding004[0x10];			//0x010C

	char						cPadding003b[0x04];			//0x011C
};

struct ElementalAttackData
{
	IMinMax			sOrganic;
	IMinMax			sFire;
	IMinMax			sIce;
	IMinMax			sLightning;
	IMinMax			sPoison;
};

class CGameCharacterStatus
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004993B0;

public:
	CGameCharacterStatus();
	~CGameCharacterStatus();

	static CGameCharacterStatus	* GetInstance() { return (CGameCharacterStatus*)0x035D0EA0; }

	void						OnMouseLDownClick( int iX, int iY );

#ifdef ITEMFIXES
	void						UpdateCharacterStatus( CharacterDataLocal * psData, ItemFixData eItemFixData );
#else
	void						UpdateCharacterStatus( CharacterDataLocal * psData);
#endif

	void						UpdateCharacterStatusItem( BOOL bCheckHack = FALSE );
};

