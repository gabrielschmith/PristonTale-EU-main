#pragma once
#include "MDamageInfo.h"

#define MAX_DEBUG_TARGETS			350
#define MAX_DEBUG_TIME				1000
#define MAX_DEBUG_TIMEHALF			(MAX_DEBUG_TIME >> 1)
#define MAX_DEBUG_TIMEHALFMINOR		(MAX_DEBUG_TIME >> 2)
#define MAX_DEBUG_TIMEHALFMAJOR		(MAX_DEBUG_TIMEHALFMINOR * 3)

struct DamageDataInfo
{
	BOOL					bActive;

	int						iID;
	UnitData				* pcUnitData;


	int						iParticleID;

	int						iType;

	DWORD					dwTime;

	int						iValue;

	int						iX;
	int						iY;
	int						iDecreaseY;

	int						iAlpha;
};

class CDamageInfoController
{
private:
	void						load();
	class CDamageInfoModel		* pcDamageInfoModel = NULL;

	UINT						uNext = 0;

	int							iDecreaseY = 0;

	int							iDefenseInfoPart;
	int							iBlockInfoPart;
	int							iEvadeInfoPart;

	DWORD						dwTimeUpdate = 0;

	//Create Bitmap Font
	UI::Bitmapfont_ptr			pOrangeFont = std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pBlueFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	//UI::Bitmapfont_ptr			pCyanFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) ); for freezing effect - for later
	UI::Bitmapfont_ptr			pRedFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pGreyFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pGoldFont   = std::make_shared<UI::BitmapFont>( Rectangle2D ( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pGreenFont  = std::make_shared<UI::BitmapFont>( Rectangle2D ( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pYellowFont = std::make_shared<UI::BitmapFont>( Rectangle2D ( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pBitmapFont = std::make_shared<UI::BitmapFont>( Rectangle2D ( 0, 0, 0, 0 ) );

	void						Clear( DamageDataInfo * psData );

public:
	CDamageInfoController();
	virtual ~CDamageInfoController();

	class CDamageInfoModel		* GetModel() { return pcDamageInfoModel; };

	int							GetDecrease() { return iDecreaseY; };

	void						Init();

	void						Load();
	void						OnHitReceived( EDamageTextType TypeAction, int Value = 0 );
	void						SetDefenseInfo( EDamageTextType TypeAction, int Value = 0, ID iTargetIDOverride = 0, ID iAttackerIDOverride = 0 );

	void						HandlePacket ( struct PacketDebugDamageInfoContainer * psPacket );
	void						AddTarget( struct PacketDebugDamageInfo * psPacket );
	void						AddTarget( int idAttackerID, int idTargetID, EDamageTextType eType, int iValue = 0, short sParameter = 0 );
	void						RelayInfoToOthers ( ID idTargetID, EDamageTextType eType, int iValue = 0);
	void						RenderUnitDamageInfo ( Unit * pcUnit );
	void						Render();
    void						HandlePacket(PacketGoldPickup * psPacket);
    void						HandlePacket(PacketHealing * psPacket);
    void						HandlePacket(PacketGrandHealing * psPacket);

	void						Update( float fTime );
};

