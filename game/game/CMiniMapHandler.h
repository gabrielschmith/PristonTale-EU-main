#pragma once

//Size of Box of Minimap(in pixels)
#define MINIMAP_LARGE_SIZE	384
#define MINIMAP_SMALL_SIZE	128

//Size of Image of Minimap (0~100%)
#define MINIMAP_TEXTURE_SIZE	512
#define MINIMAP_TEXTURE_SCALE	84



struct MiniMapData
{
	struct EXETextureHandle		* psTexture;
	struct EXETextureHandle		* psTextureTitle;

	RECT						sRectangle;

	BaseMap						* pcBaseMap;

	Point2D						sPosition;
};

class CMiniMapHandler
{
public:
							CMiniMapHandler();
	virtual				   ~CMiniMapHandler();

	void					Init();
	void					Render();
	void					Update();

	BOOL					OnMouseClick( class CMouse * pcMouse );
	void					OnMouseMove( class CMouse * pcMouse );
	void					ResetMapTextureDeltas();
	void					OnResolutionChanged();
	void					OnMiniMapSizeChanged();
	void					ResetMiniMapScreenPositions();
	void					ClearSpawnFlagMarkers ();
	void					AddSpawnFlagMarker ( int iX, int iY, int iZ );

	void					HandlePacket( PacketMapIndicators * psPacket );
	void					HandlePacket( PacketMapSpawnMarkers * psPacket );
	void					HandlePacket( PacketBossTimeUpdate * psPacket );
	void					ProcessPacket( PacketBossTimeUpdate * psPacket );

	void					OnChangeMap( int iMapID );

	int						iMiniMapSize = MINIMAP_SMALL_SIZE; //default being small
	BOOL					bLargeMiniMap = FALSE;

	//These determines the positioning of the map texture and items within the mini map
	// which is controlled by user mouse movements.
	int						iMiniMapDeltaX = 0;
	int						iMiniMapDeltaY = 0;
	int						iMiniMapDeltaXPrev = 0;
	int						iMiniMapDeltaYPrev = 0;
	int						iMouseDownInitialX = 0;
	int						iMouseDownInitialY = 0;

	//these determines the minimap position on the screen
	int						iMiniMapSmallScreenPositionX = 0;
	int						iMiniMapSmallScreenPositionY = 0;
	int						iMiniMapLargeScreenPositionX = 0;
	int						iMiniMapLargeScreenPositionY = 0;
	int						iMiniMapScreenPositionX = 0;
	int						iMiniMapScreenPositionY = 0;

private:
	BOOL					bInitialized = FALSE;
	BOOL					bActive = FALSE;

	UI::Image				* pcMaskBackground = NULL;
	UI::Image				* pcMaskBackgroundActive = NULL;
	UI::Image				* pcBorderImage = NULL;
	UI::Image				* pcBorderImageLarge = NULL;
	UI::Image				* pcMapImage = NULL;
	UI::Image				* pcMapArrowImage = NULL;
	UI::Image				* pcMapHellspawnMainImage = NULL;
	UI::Image				* pcMapHellspawnSecondaryImage = NULL;
	UI::Image				* pcMapBossImage = NULL;
	UI::Image				* pcUserPoint = NULL;
	UI::Image				* pcNPCPoint = NULL;
	UI::Image				* pcBlacksmithPoint = NULL;
	UI::Image				* pcMagicsmithPoint = NULL;
	UI::Image				* pcTeleportPoint = NULL;
	UI::Image				* pcWarehousePoint = NULL;
	UI::Image				* pcUpgradePoint = NULL;
	UI::Image				* pcSkillMasterPoint = NULL;
	UI::Image				* pcSupplyPoint = NULL;
	UI::Image				* pcQuestStartPoint = NULL;
	UI::Image				* pcQuestFinishPoint = NULL;
	UI::Image				* pcQuestStartRepeatablePoint = NULL;
	UI::Image				* pcQuestFinishRepeatablePoint = NULL;
	UI::Image				* pcQuestProgressPoint = NULL;
	UI::Image				* pcBossIcon = NULL;
	UI::Image				* pcSpawnIcon = NULL;
	UI::Image				* pcGoldWolverineIcon = NULL;
	UI::Image				* pcSilverWolverineIcon = NULL;
	UI::Image				* pcBronzeWolverineIcon = NULL;
	UI::Image			    * pcImageNextBoss = NULL;


	UI::Image			   * pcImagePlayerIcon = NULL;
	UI::Image			   * pcImageUnknownIcon = NULL;
	UI::Image			   * pcImageEnemyIcon = NULL;
	UI::Image			   * pcImageFriendlyIcon = NULL;
	UI::Image			   * pcaImagesPartyMemberIcon[MAX_PARTY_MEMBERS];
	UI::Image			   * pcImageRaidIcon = NULL;

	DX::Effect_ptr			pEffectMask;
	DX::RenderTarget_ptr	pRenderTargetMask;

	BOOL					bMapTitleSelected;
	BOOL					bMapSelected;
	Point2D					sPointMouse;

	Rectangle2D				sMiniMapBox;
	Point2D					saMinimaps[4];

	int						iFontID = -1;
	int						iMiniMapIndicatorFont = -1;
	DWORD					dwLastRenderTime = 0;

	BOOL					bIsHoverUnit = FALSE;
	Rectangle2D				sHoverUnitBox = Rectangle2D();
	std::string				strHoverUnit;
	UI::Color				cHoverColor = UI::Color();
	UI::Tooltip_ptr			pTooltipUnit;


	BOOL					bFirstMapBossActive = FALSE;
	BOOL					bSecondMapBossActive = FALSE;

	BOOL					bShowBossTime = FALSE;
	std::string				strNextBossName;
	DWORD					dwNextBossTime = 0;
	DWORD					dwTickTime = 0;
	std::vector<Point3D>	vSpawnFlags;
	std::map<int, PacketMapIndicators>	mMapIndicators;



	void					RenderMap( Point3D sUserPosition );
	float					GetMapScale(EMapID mapId);
	void					RenderUser(Point3D sUserPosition, Point3D sUserAngle, int iSize);
	BOOL					RenderNPC( UnitData * pcUnitData, Point3D sUserPosition, int iSize, BOOL bQuestsOnly );
	BOOL					RenderBoss ( UnitData * pcUnitData, Point3D sUserPosition, int iSize );
	BOOL					RenderQuestMonsters (UnitData* pcUnitData, Point3D sUserPosition, int iSize);
	BOOL					RenderPlayer( UnitData * pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize , int mapID);
	BOOL					RenderParty( Point3D sUserPosition, int iUserID, int iSize );
	BOOL					RenderUnknown ( UnitData * pcUnitData, Point3D sUserPosition, int iSize);
	void					RenderSpawnFlagMarker ( Point3D sSpawnFlagPosition, Point3D sUserPosition, int iSize );
	void					RenderMapIndicator( BOOL bIsSecondaryMap, MapIndicator * pMapIndicator, Point3D sUserPosition, int iSize );
	void					RenderMapIndicator( UI::Image * pImage, int iCompactPosX, int iCompactPosZ, int iAngle, Point3D sUserPosition, int iSize, const std::string szDescription = "");
	void					RenderNextMapIndicator( int iNextMapID, int iCompactPosX, int iCompactPosZ, int iAngle, Point3D sUserPosition, int iSize );

	BOOL					RenderMapTime();

	MiniMapData				* GetMiniMapData( int i ) { return &((MiniMapData*)0x02085FB0)[i]; }

	BOOL					IsCurrentUserMap( int iMapID );
	BOOL					IsCurrentUserSecondMap ( int iMapID );
	BOOL					IsCurrentUserMap( MiniMapData  * psMap );

	Rectangle2D				GetMiniMapBox() { return sMiniMapBox; }
	Point2D					GetMiniMapPostion() { return Point2D(sMiniMapBox.iX, sMiniMapBox.iY); }
	void					SetMiniMapBox(Rectangle2D sRect) { sMiniMapBox = sRect; }

	void					BeginViewport();
	void					EndViewport();

	static void				DrawMinimap( MiniMapData  * psMap, int iX, int iY, int iSize );

	void					SetMinimapSize( float fSize );

	Point2D					GetMinimapPositionFromCenter( Point3D sCenterPosition, Point3D sObjectPosition, int iWidthPoint, int iHeightPoint, int iSize );
	Point2D					GetMinimapPositionFromCenter( Point3D sCenterPosition, int iX, int iY, int iWidthPoint, int iHeightPoint, int iSize );

};

