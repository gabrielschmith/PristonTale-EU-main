#include "stdafx.h"
#include "CMiniMapHandler.h"

CMiniMapHandler::CMiniMapHandler()
{
	bMapSelected	= FALSE;
	bMapTitleSelected = FALSE;
	sPointMouse = Point2D();
}

CMiniMapHandler::~CMiniMapHandler()
{
}

void CMiniMapHandler::Init()
{
	//pcMaskBackground = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\mask_minimap.tga"); //not needed anymore. causes issues
	pcMaskBackgroundActive = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\mask_activeminimap.tga");
	pcBorderImageLarge = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\border_large.png");
	pcBorderImage = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\border.png");

	pcMapArrowImage = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\map_arrow.png");
	pcMapHellspawnMainImage = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\hs_main.png");
	pcMapHellspawnSecondaryImage = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\hs_sec.png");
	pcMapBossImage = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\boss_inactive.png");
	pcUserPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\arrow.png");
	pcNPCPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\npc.png");
	pcBlacksmithPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Anvil.png");
	pcMagicsmithPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Magicsmith.png");
	pcWarehousePoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Warehouse.png");
	pcUpgradePoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Aging-Mixing.png");
	pcSkillMasterPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Skill-Master.png");
	pcSupplyPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Supply.png");
	pcQuestStartPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Quest-Start.png");
	pcQuestFinishPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Quest-Finish.png");
	pcQuestStartRepeatablePoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Quest-Start-Repeatable.png");
	pcQuestFinishRepeatablePoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Quest-Finish-Repeatable.png");
	pcQuestProgressPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Quest-Progress.png");
	pcTeleportPoint = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Warp.png");
	pcBossIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\boss.png");
	pcSpawnIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\spawn.png");


	pcGoldWolverineIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Wolverine-Golden.png");
	pcSilverWolverineIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Wolverine-Silver.png");
	pcBronzeWolverineIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\Wolverine-Bronze.png");

	pEffectMask = GetGraphicsDevice()->GetEffectFactory()->MakeEffect("game\\scripts\\shaders\\UI\\Minimap.fx");
	pRenderTargetMask = GetGraphicsDevice()->MakeRenderTarget(1000, MINIMAP_SMALL_SIZE, MINIMAP_SMALL_SIZE);
	pcImageRaidIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\party\\raidmember.png");

	pTooltipUnit = std::make_shared<UI::Tooltip>(Rectangle2D());
	pTooltipUnit->Init();
	pTooltipUnit->SetFont("Arial", 16, -1);
	pTooltipUnit->SetPosition(TOOLTIPPOSITION_Top);
	pTooltipUnit->SetFollowMouse(FALSE);
	pTooltipUnit->SetHover(TRUE);
	pTooltipUnit->Open();

	pcImagePlayerIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\player.png");
	pcImageUnknownIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\unknown.png");
	pcImageEnemyIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\enemy.png");
	pcImageFriendlyIcon = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\friendly.png");
	pcImageNextBoss = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\minimap\\nextboss.png");

	iMiniMapIndicatorFont = FONTHANDLER->CreateFont("Arvo", 12, 0, FALSE, TRUE, -1);
	iFontID = FONTHANDLER->CreateFont("Arial", 14, 0, FALSE, FALSE, 0);

	for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		pcaImagesPartyMemberIcon[i] = UI::ImageLoader::LoadImage("game\\images\\UI\\hud\\party\\member%d.png", i + 1);

	ResetMiniMapScreenPositions();
}

void CMiniMapHandler::ResetMiniMapScreenPositions()
{
	iMiniMapSmallScreenPositionX = (RESOLUTION_WIDTH - MINIMAP_SMALL_SIZE) - 16;
	iMiniMapSmallScreenPositionY = (RESOLUTION_HEIGHT - MINIMAP_SMALL_SIZE) - 50;

	iMiniMapLargeScreenPositionX = (RESOLUTION_WIDTH - MINIMAP_LARGE_SIZE) - 16;
	iMiniMapLargeScreenPositionY = (RESOLUTION_HEIGHT - MINIMAP_LARGE_SIZE) - 50;

	if (bLargeMiniMap)
	{
		iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY;
	}
	else
	{
		iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY;
	}

	SetMiniMapBox(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));

}

void CMiniMapHandler::Render()
{
	if( Game::GetGameMode() == GAMEMODE_InGame && ISSHOW_MINIMAP )
	{
		bIsHoverUnit = FALSE;

		//Render Minimaps
		RenderMap( UNIT->GetPosition() );

		//Render Border (always do - to force border to be shown regardless of settings. It is off by default)

		int iBorderWidth = 144;
		int iBorderHeight = 158;

		if (bLargeMiniMap)
		{
			//256 is the added width / height to the original mini map border
			// to get the larger version
			iBorderWidth += 256;
			iBorderHeight += 256;
		}

		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera != TRUE )
			UI::ImageRender::Render( bLargeMiniMap ? pcBorderImageLarge : pcBorderImage, GetMiniMapPostion().iX - 9, GetMiniMapPostion().iY - 24, iBorderWidth, iBorderHeight, -1 );

		auto miniMapData = UNITDATA->iLoadedMapIndex >= 0 ? GetMiniMapData(UNITDATA->iLoadedMapIndex) : GetMiniMapData(0);
		auto textureTitle = miniMapData->psTextureTitle;

		int miniMapiX = GetMiniMapPostion().iX;
		int miniMapiY = GetMiniMapPostion().iY - textureTitle->iHeight;

		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera != TRUE )
			UI::ImageRender::Render(textureTitle->pcD3DTexture, miniMapiX - 1, miniMapiY, textureTitle->iWidth, textureTitle->iHeight, 0, 0, -1, FALSE);

		if( bShowBossTime && SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer )
		{
			int iHour	= (dwNextBossTime / 3600) % 60;
			int iMinute = (dwNextBossTime / 60) % 60;
			int iSecond = dwNextBossTime % 60;

			UI::ImageRender::Render( pcImageNextBoss, miniMapiX - 5, miniMapiY - 24, 130, 23, -1 );
			FONTHANDLER->Draw( iFontID, miniMapiX + 20, miniMapiY - 20, FormatString( "Next Boss: %02d:%02d:%02d", iHour, iMinute, iSecond ), DT_NOCLIP, D3DCOLOR_XRGB( 255, 180, 180 ) );
		}

		if( bIsHoverUnit )
		{
			pTooltipUnit->SetText( strHoverUnit );
			pTooltipUnit->SetTextColor( cHoverColor );
			pTooltipUnit->Render( sHoverUnitBox.iX + 5, sHoverUnitBox.iY, 0,0, 0, 0 );
		}
	}
}

void CMiniMapHandler::Update()
{
	dwTickTime++;

	//1 Second?
	if ( (dwTickTime % 10) == 0 )
	{
		//Decrease Boss Time
		if ( dwNextBossTime > 0 )
		{
			dwNextBossTime--;
			//Boss Time End? Hide.
			if ( dwNextBossTime == 0 )
			{
				bShowBossTime = FALSE;

				//Get new Boss Time
				ProcessPacket( &PacketBossTimeUpdate( MAP_ID ) );
			}
		}
	}
}

BOOL CMiniMapHandler::OnMouseClick( CMouse * pcMouse )
{
	//If mini map isn't shown
	// don't process the mouse click otherwise
	// player cannot move
	if (!ISSHOW_MINIMAP)
		FALSE;

	BOOL bRet = FALSE;

	// User clicked outside of mini map region
	if (pcMouse->GetEvent() == EMouseEvent::ClickDownL && !bActive)
		ResetMapTextureDeltas();

	if( bMapSelected || bMapTitleSelected )
	{
		if (pcMouse->GetEvent() == EMouseEvent::ClickUpL)
		{
			bMapSelected = FALSE;
			bMapTitleSelected = FALSE;
			iMiniMapDeltaXPrev = iMiniMapDeltaX;
			iMiniMapDeltaYPrev = iMiniMapDeltaY;
		}
	}
	else if (!bMapTitleSelected && !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI &&
		pcMouse->GetEvent() == EMouseEvent::ClickDownL &&
		pcMouse->GetPosition()->Inside(Rectangle2D(GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, iMiniMapSize, 16)))
	{
		bMapTitleSelected = TRUE;
		bRet = TRUE;

		sPointMouse.iX = abs(pcMouse->GetPosition()->iX - GetMiniMapPostion().iX);
		sPointMouse.iY = abs(pcMouse->GetPosition()->iY - GetMiniMapPostion().iY) - 16;
	}
	else if( !bMapSelected && bLargeMiniMap &&
		pcMouse->GetEvent() == EMouseEvent::ClickDownL &&
		pcMouse->GetPosition()->Inside( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY, iMiniMapSize, iMiniMapSize ) ) )
	{
		bMapSelected = TRUE;


		if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera )
		{
			float fRotation = 0.25f*PI;

			iMouseDownInitialX = static_cast<int>( pcMouse->GetPosition()->iX * cos( fRotation ) - pcMouse->GetPosition()->iY * sin( fRotation ) );
			iMouseDownInitialY = static_cast<int>( pcMouse->GetPosition()->iY * cos( fRotation ) + pcMouse->GetPosition()->iX * sin( fRotation ) );
		}
		else
		{
			iMouseDownInitialX = pcMouse->GetPosition()->iX ;
			iMouseDownInitialY = pcMouse->GetPosition()->iY ;
		}

		bRet = TRUE;
	}

	return bRet;
}

void CMiniMapHandler::OnMouseMove( CMouse * pcMouse )
{
	Rectangle2D MapRect = Rectangle2D ( GetMiniMapPostion ().iX, GetMiniMapPostion ().iY - 16, iMiniMapSize, iMiniMapSize + 16 );

	if (pcMouse->GetPosition()->Inside(MapRect))
		bActive = TRUE;
	else
		bActive = FALSE;

	if( bMapSelected && bLargeMiniMap)
	{
		if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera )
		{
			float fRotation = 0.25f*PI;

			iMiniMapDeltaX = static_cast<int>( iMiniMapDeltaXPrev + pcMouse->GetPosition()->iX * cos( fRotation ) - pcMouse->GetPosition()->iY * sin( fRotation ) - iMouseDownInitialX );
			iMiniMapDeltaY = static_cast<int>( iMiniMapDeltaYPrev + pcMouse->GetPosition()->iY * cos( fRotation ) + pcMouse->GetPosition()->iX * sin( fRotation ) - iMouseDownInitialY );
		}
		else
		{
			iMiniMapDeltaX = iMiniMapDeltaXPrev + (pcMouse->GetPosition()->iX - iMouseDownInitialX);
			iMiniMapDeltaY = iMiniMapDeltaYPrev + (pcMouse->GetPosition()->iY - iMouseDownInitialY);
		}
	}
	else if (bMapTitleSelected)
	{
		int iX = pcMouse->GetPosition()->iX - sPointMouse.iX;
		int iY = pcMouse->GetPosition()->iY - sPointMouse.iY;

		if (iX < 0)
			iX = 0;

		if (iY < 0)
			iY = 0;

		if (iY > RESOLUTION_HEIGHT - iMiniMapSize)
			iY = RESOLUTION_HEIGHT - iMiniMapSize;

		if (iX > RESOLUTION_WIDTH - iMiniMapSize)
			iX = RESOLUTION_WIDTH - iMiniMapSize;

		if (bLargeMiniMap)
		{

			iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX = iX;
			iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY = iY;
		}
		else
		{
			iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX = iX;
			iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY = iY;
		}

		SetMiniMapBox(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));
	}
}

void CMiniMapHandler::ResetMapTextureDeltas()
{
	iMiniMapDeltaY = 0;
	iMiniMapDeltaX = 0;
	iMiniMapDeltaXPrev = 0;
	iMiniMapDeltaYPrev = 0;
}


float CMiniMapHandler::GetMapScale(EMapID mapId)
{
	float fMapSizeScale = 15.0f - ((MINIMAP_TEXTURE_SCALE * 10.0f) / 100.0f);

	if (bLargeMiniMap)
		fMapSizeScale = 15.0f;

	switch (mapId)
	{
		case MAPID_DarkSanctuary:
		case MAPID_BeehiveCave:
		case MAPID_MushroomCave:
		case MAPID_AncientDungeonF1:
		case MAPID_AncientDungeonF2:
		case MAPID_AncientDungeonF3:
			return (float)(fMapSizeScale / 1.5);

		case MAPID_PillaiTown:
		case MAPID_CursedTempleF1:
		case MAPID_CursedTempleF2:
		case MAPID_CursedTempleF3:
		case MAPID_AncientWeapon:
			return (float)(fMapSizeScale / 1.3);

		case MAPID_QuestArena:
			return (float)( fMapSizeScale / 3.0 );
	}

	return fMapSizeScale;
}

void CMiniMapHandler::ClearSpawnFlagMarkers ()
{
	vSpawnFlags.clear();
}

//For test map only (map id = 32)
void CMiniMapHandler::AddSpawnFlagMarker (int iX, int iY, int iZ)
{
	vSpawnFlags.push_back ( Point3D ( iX, iY, iZ ) );
}

void CMiniMapHandler::RenderMap( Point3D sUserPosition )
{
	//New Render Target to render the minimap
	if (GetGraphicsDevice()->SetRenderTarget(pRenderTargetMask))
	{
		//Clear Render Target
		GetGraphicsDevice()->Clear(TRUE, FALSE, FALSE);

		int firstMapId = -1;
		int secondMapId = -1;

		if ( IsCurrentUserMap( GetMiniMapData( 0 )->pcBaseMap->iMapID ) )
		{
			firstMapId = GetMiniMapData( 0 )->pcBaseMap->iMapID;
		}
		else if (IsCurrentUserSecondMap( GetMiniMapData( 0 )->pcBaseMap->iMapID ) )
		{
			secondMapId = GetMiniMapData( 0 )->pcBaseMap->iMapID;
		}

		if ( GetMiniMapData( 1 ) && GetMiniMapData( 1 )->pcBaseMap )
		{
			if ( IsCurrentUserMap( GetMiniMapData( 1 )->pcBaseMap->iMapID ) )
			{
				firstMapId = GetMiniMapData( 1 )->pcBaseMap->iMapID;
			}
			else if ( IsCurrentUserSecondMap( GetMiniMapData( 1 )->pcBaseMap->iMapID ) )
			{
				secondMapId = GetMiniMapData( 1 )->pcBaseMap->iMapID;
			}
		}

		if ( firstMapId == -1 )
			return;

		float fMapSizeScale = GetMapScale((EMapID)firstMapId);

		if (bLargeMiniMap)
		{
			SetMinimapSize(iMiniMapSize * 5.0f); //Render mask to the Render Target
		}
		else
		{
			SetMinimapSize(static_cast<float>(iMiniMapSize)); //Render mask to the Render Target
		}

		int iMinimapSize = 256 * 256 * (int)fMapSizeScale;
		int iMinimapSizeDraw = iMinimapSize;
		int iShiftX = 0;
		int iShiftY = 0;

		if (bLargeMiniMap)
		{
			iMinimapSizeDraw *= 5;
			iShiftX = -(256 * 3);
			iShiftY = -(256 * 3);
		}

		//DrawMinimap -> iMinimapSize defines the map size to be drawn
		DrawMinimap(GetMiniMapData(1), iMiniMapDeltaX + iShiftX, iMiniMapDeltaY + iShiftY, iMinimapSizeDraw);
		DrawMinimap(GetMiniMapData(0), iMiniMapDeltaX + iShiftX, iMiniMapDeltaY + iShiftY, iMinimapSizeDraw);



		int iMapID = CHARACTERGAME->GetStageID();
		BOOL bShowNPCs = SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs;

		if ( DEBUG_GM )
		{
			for ( Point3D point : vSpawnFlags )
			{
				RenderSpawnFlagMarker( point, sUserPosition, iMinimapSize );
			}
		}


		bFirstMapBossActive = FALSE;
		bSecondMapBossActive = FALSE;

		for ( int i = 0; i < MAX_UNITS; i++ )
		{
			UnitData * pc = UNITGAME->pcaUnitData + i;

			if ( pc == NULL )
				continue;

			if (pc->bActive &&
				pc->sCharacterData.iType == CHARACTERTYPE_Monster &&
				((pc->sCharacterData.bExclusiveBoss || pc->sCharacterData.sMonsterClass == EMonsterClass::Boss ) ))
			{

				if ( pc->sCharacterData.sMapID == firstMapId )
				{
					bFirstMapBossActive = TRUE;
					continue;
				}
				else if ( pc->sCharacterData.sMapID == secondMapId )
				{
					bSecondMapBossActive = TRUE;
					continue;
				}

				if ( bFirstMapBossActive && bSecondMapBossActive )
					break;
			}
		}


		auto itFirstMap = mMapIndicators.find( firstMapId );
		if ( itFirstMap != mMapIndicators.end() )
		{
			for ( int i = 0; i < itFirstMap->second.iCount; i++ )
			{
				auto indicator = &itFirstMap->second.mapIndicators[i];
				RenderMapIndicator( FALSE, indicator, sUserPosition, iMinimapSize );
			}
		}

		auto itSecondMap = mMapIndicators.find( secondMapId );
		if ( itSecondMap != mMapIndicators.end() )
		{
			for ( int i = 0; i < itSecondMap->second.iCount; i++ )
			{
				auto indicator = &itSecondMap->second.mapIndicators[i];

				if ( indicator->eType == MapIndicator::Type::NextMapArrow )
				{
					BOOL bSkipRender = FALSE;
					//check for any nearby map indicators from first map. If there is
					//don't render
					if ( itFirstMap != mMapIndicators.end() )
					{
						for ( int i = 0; i < itFirstMap->second.iCount; i++ )
						{
							auto indicatorFirst = &itFirstMap->second.mapIndicators[i];

							if ( indicatorFirst->eType != MapIndicator::Type::NextMapArrow )
								continue;

							const int iDeltaX = indicator->iCompactPosX - indicatorFirst->iCompactPosX;
							const int iDeltaZ = indicator->iCompactPosZ - indicatorFirst->iCompactPosZ;
							const int iDeltaXY = ( iDeltaX * iDeltaX ) + ( iDeltaZ * iDeltaZ );

							if ( iDeltaXY < DISTANCE_XY_60_meters )
							{
								bSkipRender = TRUE;
								break;
							}
						}
					}

					if ( bSkipRender )
						break;

					RenderNextMapIndicator( indicator->iValue, indicator->iCompactPosX, indicator->iCompactPosZ, indicator->iAngleY, sUserPosition, iMinimapSize );
				}
				else
				{
					RenderMapIndicator( TRUE, indicator, sUserPosition, iMinimapSize );
				}
			}
		}


		//Units
		for ( int i = 0; i < MAX_UNITS; i++ )
		{
			UnitData * pc = UNITGAME->pcaUnitData + i;

			if (&pc == nullptr)
				continue;

			if (RenderQuestMonsters(pc, sUserPosition, iMinimapSize))
				continue;

			if (RenderBoss(pc, sUserPosition, iMinimapSize))
				continue;

			if (bShowNPCs && RenderNPC(pc, sUserPosition, iMinimapSize, TRUE)) //quest icons first
				continue;

			if (bShowNPCs && RenderNPC(pc, sUserPosition, iMinimapSize, FALSE)) //other icons
				continue;

			if (pc->iLoadedMapIndex >= 0
				&& pc->bActive &&
				(FIELDGAME[pc->iLoadedMapIndex]->iMapID == iMapID ||
				 FIELDGAME[pc->iLoadedMapIndex]->iMapID == MAPID_Bellatra && iMapID	== MAPID_NaviskoTown)) //players in nav can see players in sod
			{
				if ( RenderPlayer( pc, sUserPosition, UNITDATA->sCharacterData.iClanID, iMinimapSize, CHARACTERGAME->GetStageID() ) )
					continue;

				RenderUnknown( pc, sUserPosition, iMinimapSize );
			}
		}

		RenderParty( sUserPosition, UNITDATA->iID, iMinimapSize );

		//Render user Point
		if (UNIT->bInvisibleInMiniMap == FALSE ||
			MAP_ID == MAPID_BlessCastle ||
			MAP_ID == MAPID_Bellatra)
		{
			RenderUser(sUserPosition, UNITDATA->sAngle, iMinimapSize);
		}

		SetMinimapSize(static_cast<float>(iMiniMapSize) );

		//Back Old Render Target
		GetGraphicsDevice()->UnsetRenderTarget();

		//Set Sprite Effect...
		UI::ImageRender::SetEffect( pEffectMask );
		pEffectMask->SetTechnique( "MaskScale" );
		if ( pEffectMask->Begin() >= 0 )
		{
			//Set Texture Mask Sampler
			D3DXHANDLE hTexture = pEffectMask->Get()->GetParameterByName( 0, "MaskTexture" );
			pEffectMask->Get()->SetTexture( hTexture, pcMaskBackgroundActive->GetTexture() );

			//Rotate Minimap in ARPG Mode
			float fRotation = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotation = ( ( ((PTANGLE_360*3)/8) * 360 ) / PTANGLE_360 ) + 180;

			UI::ImageRender::Render( pRenderTargetMask->GetTexture(), GetMiniMapPostion ().iX, GetMiniMapPostion ().iY, pRenderTargetMask->GetWidth (), pRenderTargetMask->GetWidth (), 0, 0, D3DCOLOR_XRGB ( 255, 255, 255 ), 1.0f, 1.0f, fRotation );

			pEffectMask->End();

		}



		UI::ImageRender::SetEffect( NULL );

		if ( bUIDebug )
		{
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY, iMiniMapSize, 1 ), -1 );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY + iMiniMapSize - 1, iMiniMapSize, 1 ), -1 );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX, GetMiniMapPostion().iY, 1, iMiniMapSize ), -1 );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( GetMiniMapPostion().iX + iMiniMapSize - 1, GetMiniMapPostion().iY, 1, iMiniMapSize ), -1 );
		}
	}
}

void CMiniMapHandler::RenderUser(Point3D sUserPosition, Point3D sUserAngle, int iSize)
{
	Point2D sPosition = GetMinimapPositionFromCenter ( sUserPosition, sUserPosition, pcUserPoint->GetWidth () >> 1, pcUserPoint->GetHeight () >> 1, iSize );

	//Angle of User Point
	int iAngle = ( ( sUserAngle.iY * 360 ) / PTANGLE_360 ) + 180;

	UI::ImageRender::Render ( pcUserPoint, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcUserPoint->GetWidth (), pcUserPoint->GetHeight (), 0, 0, -1, 0.7f, (float)iAngle );
}

BOOL CMiniMapHandler::RenderNPC( UnitData * pcUnitData, Point3D sUserPosition, int iSize, BOOL bQuestsOnly )
{
	if (pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC && pcUnitData->bActive)
	{
		UI::Image* img = pcNPCPoint;

		if ( bQuestsOnly )
		{
			const auto questIcon = QUESTGAMEHANDLER->GetNpcQuestStatus( pcUnitData->sCharacterData.iNPCId );
			switch ( questIcon )
			{
				case  EQuestIcon::Avaliable:			img = pcQuestStartPoint; break;
				case  EQuestIcon::AvaliableRepeatable:	img = pcQuestStartRepeatablePoint; break;
				case  EQuestIcon::Complete:				img = pcQuestFinishPoint; break;
				case  EQuestIcon::CompleteRepeatable:	img = pcQuestFinishRepeatablePoint; break;
				case  EQuestIcon::Progress:				img = pcQuestProgressPoint; break;
			}
		}
		else
		{

			switch ( pcUnitData->sCharacterData.iNPCId )
			{
				case NPCID_BLACKSMITH_ANDREW:
				case NPCID_BLACKSMITH_FRAIN:
				case NPCID_BLACKSMITH_UMPH:
				case NPCID_BLACKSMITH_DROL:
				case NPCID_BLACKSMITH_BARTZ:
				case NPCID_BLACKSMITH_DEFRAIN:
				case NPCID_BLACKSMITH_GODEN:
				case NPCID_BLACKSMITH_GUS:
				case NPCID_BLACKSMITH_RUGA:
				case NPCID_BLACKSMITH_SOLO:
				case NPCID_BLACKSMITH_PLAGUEIS:
				case NPCID_BLACKSMITH_JAMES:
				case NPCID_BLACKSMITH_JOHN:
				{
					img = pcBlacksmithPoint;
					break;
				}

				case NPCID_WAREHOUSE_KEEPER:
				case NPCID_WAREHOUSE_KEEPER_BUJA:
				case NPCID_S2_WAREHOUSE_KEEPER_AYA:
				case NPCID_S2_WAREHOUSE_KEEPER_CEDAR:
				case NPCID_WAREHOUSE_KEEPER_ARENT:
				case NPCID_WAREHOUSE_KEEPER_DOJ:
				case NPCID_WAREHOUSE_KEEPER_EURA:
				case NPCID_WAREHOUSE_KEEPER_OMIDOR:
				case NPCID_WAREHOUSE_KEEPER_LAVENDAR:
				{
					img = pcWarehousePoint;
					break;
				}

				case NPCID_MISCELLANEOUS_STORE_ALYX:
				case NPCID_MISCELLANEOUS_STORE_KAIRI:
				case NPCID_MISCELLANEOUS_STORE_MIDNA:
				case NPCID_MISCELLANEOUS_STORE_SHEPARD:
				case NPCID_MISCELLANEOUS_STORE_TIFA:
				case NPCID_MISCELLANEOUS_STORE_YUNA:
				case NPCID_MISCELLANEOUS_STORE_GORDEN:
				case NPCID_MISCELLANEOUS_STORE_LYNN:
				case NPCID_MISCELLANEOUS_STORE_LETTE:
				case NPCID_MISCELLANEOUS_STORE_SAVY:
				case NPCID_MISCELLANEOUS_STORE_EURA:
				case NPCID_MISCELLANEOUS_STORE_ROBIN:
				case NPCID_MISCELLANEOUS_STORE_EVY:
				case NPCID_MISCELLANEOUS_STORE_IVONNE:
				case NPCID_MISCELLANEOUS_STORE_BYANT:
				case NPCID_MISCELLANEOUS_STORE_PHILLYA:
				case NPCID_MISCELLANEOUS_STORE_NAMY:
				case NPCID_MISCELLANEOUS_STORE_REY:
				case NPCID_MISCELLANEOUS_STORE_LEIA:
				case NPCID_MISCELLANEOUS_STORE_CINDY:
				case NPCID_MISCELLANEOUS_STORE_VALERIYA:
				case NPCID_MISCELLANEOUS_STORE_NAALIS:
				case NPCID_MISCELLANEOUS_STORE_NIMPHY:
				case NPCID_MISCELLANEOUS_STORE_NATASHA:
				case NPCID_DUNGEON_KEEPER_1:
				case NPCID_DUNGEON_KEEPER_2:
				{
					img = pcSupplyPoint;
					break;
				}

				case NPCID_MIXING_CRAFTSMAN_MORALD:
				case NPCID_ARCANE_MORIPH:
				{
					img = pcUpgradePoint;
					break;
				}

				case NPCID_SKILL_MASTER_VERKAN:
				case NPCID_WITCHCRAFT_ACADEMY_RAYMON:
				{
					img = pcSkillMasterPoint;
					break;
				}

				case NPCID_WITCHCRAFT_STORE_1:
				case NPCID_WITCHCRAFT_STORE_2:
				case NPCID_MAGIC_STORE_2ND_BRANCH:
				case NPCID_MAGIC_STORE_3RD_BRANCH:
				{
					img = pcMagicsmithPoint;
					break;
				}

				case NPCID_INVENTOR_OF_WARPGATE:
				case NPCID_CLEVER_NEWTER:
				{
					img = pcTeleportPoint;
					break;
				}
			}
		}

		if ( img )
		{
			Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, img->GetWidth() >> 1, img->GetHeight() >> 1, iSize);

			float fRotation = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotation = -0.25f*PI;

			int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
			int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

			int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
			int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

			int iDistFromCenterX = iXLocation - iMapCenterX;
			int iDistFromCenterY = iYLocation - iMapCenterY;

			int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
			int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

			int FinalX = iMapCenterX + iInputRotX + img->GetWidth()/2 - 5;
			int FinalY = iMapCenterY + iInputRotY;

			Rectangle2D sBox = Rectangle2D(FinalX, FinalY, img->GetWidth() + 10, img->GetHeight());

			float fRotationDX = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

			UI::ImageRender::Render(img, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, img->GetWidth(), img->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);


			if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
				MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()) )
			{
				bIsHoverUnit = TRUE;
				sHoverUnitBox = sBox;
				strHoverUnit = pcUnitData->sCharacterData.szName;
				cHoverColor = UI::Color(255, 255, 255, 255);
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderQuestMonsters(UnitData* pcUnitData, Point3D sUserPosition, int iSize)
{
	if (pcUnitData->bActive &&
		pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster &&
		(pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN))
	{
		UI::Image* img = nullptr;

		if (STRINGCOMPARE(pcUnitData->GetName(), "Golden Wolverine"))
		{
			img = pcGoldWolverineIcon;
		}
		else if (STRINGCOMPARE(pcUnitData->GetName(), "Silver Wolverine"))
		{
			img = pcSilverWolverineIcon;
		}
		else if (STRINGCOMPARE(pcUnitData->GetName(), "Bronze Wolverine"))
		{
			img = pcBronzeWolverineIcon;
		}

		if (img)
		{
			Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, img->GetWidth() >> 1, img->GetHeight() >> 1, iSize);

			float fRotation = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotation = -0.25f*PI;

			int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
			int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

			int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
			int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

			int iDistFromCenterX = iXLocation - iMapCenterX;
			int iDistFromCenterY = iYLocation - iMapCenterY;

			int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
			int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

			int FinalX = iMapCenterX + iInputRotX + img->GetWidth()/2 - 5;
			int FinalY = iMapCenterY + iInputRotY;

			Rectangle2D sBox = Rectangle2D(FinalX, FinalY, img->GetWidth() + 10, img->GetHeight());

			float fRotationDX = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

			UI::ImageRender::Render(img, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, img->GetWidth(), img->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);


			if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
				MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
			{
				bIsHoverUnit = TRUE;
				sHoverUnitBox = sBox;
				strHoverUnit = pcUnitData->sCharacterData.szName;
				cHoverColor = UI::Color(255, 255, 255, 255);
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderBoss(UnitData* pcUnitData, Point3D sUserPosition, int iSize)
{
	if ( pcUnitData->bActive == FALSE )
		return FALSE;

	if (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster &&
		((pcUnitData->sCharacterData.bExclusiveBoss || pcUnitData->sCharacterData.sMonsterClass == EMonsterClass::Boss) )
		)
	{
		//If this unit has a owner, then it's a summon
		//not an actual boss. Note
		//monster id is not 0 for pets
		if ( strlen(pcUnitData->sCharacterData.Pet.szOwnerName) > 0 )
			return FALSE;

		Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, pcBossIcon->GetWidth() >> 1, pcBossIcon->GetHeight() >> 1, iSize);

		float fRotation = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotation = -0.25f*PI;

		int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
		int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

		int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
		int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

		int iDistFromCenterX = iXLocation - iMapCenterX;
		int iDistFromCenterY = iYLocation - iMapCenterY;

		int iInputRotX = static_cast<int>(iDistFromCenterX  * cos ( fRotation ) - iDistFromCenterY * sin ( fRotation ));
		int iInputRotY = static_cast<int>(iDistFromCenterY * cos ( fRotation ) + iDistFromCenterX * sin ( fRotation ));

		int FinalX = iMapCenterX + iInputRotX + pcBossIcon->GetWidth()/2 - 5;
		int FinalY = iMapCenterY + iInputRotY;

		Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcBossIcon->GetWidth() + 10, pcBossIcon->GetHeight());

		float fRotationDX = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

		UI::ImageRender::Render(pcBossIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcBossIcon->GetWidth(), pcBossIcon->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);

		if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
			MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
		{
			bIsHoverUnit = TRUE;
			sHoverUnitBox = sBox;
			strHoverUnit = pcUnitData->sCharacterData.szName;
			cHoverColor = UI::Color(255, 255, 255, 255);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderPlayer( UnitData * pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize, int mapID )
{
	if( pcUnitData->PkMode_CharState == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
	{
		Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
		if ( pcUnit )
		{
			//is invisible from mini map and is not in SoD / Bless Castle?
			if ( FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapID != MAPID_Bellatra &&
				 FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapID != MAPID_BlessCastle &&
				 pcUnit->bInvisibleInMiniMap )
			{
				return TRUE; //need to be returned TRUE. see logic one level up
			}
		}

		//Allied Clan
		if( pcUnitData->sCharacterData.iClanID > 0 && pcUnitData->sCharacterData.iClanID == iUserClanID && SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers )
		{
			Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcImageFriendlyIcon->GetWidth() >> 1, pcImageFriendlyIcon->GetHeight() >> 1, iSize );

			float fRotation = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotation = -0.25f*PI;

			float iXLocation = static_cast<float>( sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX );
			float iYLocation = static_cast<float>( sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY );

			float iMapCenterX = GetMiniMapPostion ().iX + iMiniMapSize / 2.0f;
			float iMapCenterY = GetMiniMapPostion ().iY + iMiniMapSize / 2.0f;

			float iDistFromCenterX = iXLocation - iMapCenterX;
			float iDistFromCenterY = iYLocation - iMapCenterY;

			float iInputRotX = iDistFromCenterX  * cos ( fRotation ) - iDistFromCenterY * sin ( fRotation );
			float iInputRotY = iDistFromCenterY * cos ( fRotation ) + iDistFromCenterX * sin ( fRotation );

			int FinalX = static_cast<int>( iMapCenterX + iInputRotX + pcImageFriendlyIcon->GetWidth() / 2 - 5 );
			int FinalY = static_cast<int>( iMapCenterY + iInputRotY );

			Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcImageFriendlyIcon->GetWidth() + 10, pcImageFriendlyIcon->GetHeight());

			float fRotationDX = 0.0f;
			if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
				fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

			UI::ImageRender::Render(pcImageFriendlyIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageFriendlyIcon->GetWidth(), pcImageFriendlyIcon->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);

			if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
				MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
			{
				if (&pcUnitData->sCharacterData)
				{
					std::string strLevel = std::to_string(pcUnitData->sCharacterData.iLevel);
					bool bTwoDigitLevel = strLevel.size() > 1 ? true : false;

					char fullStr[32] = { 0 };
					STRINGFORMAT(fullStr, "%s (%s%c)", pcUnitData->GetName(), bTwoDigitLevel ? strLevel.substr(0, strLevel.size() - 1).c_str() : strLevel.c_str(), bTwoDigitLevel ? 'x' : NULL);

					strHoverUnit = fullStr;
				}
				else
				{
					strHoverUnit = pcUnitData->GetName();
				}

				bIsHoverUnit = TRUE;
				sHoverUnitBox = sBox;
				cHoverColor = UI::Color(255, 175, 230, 175);
			}
		}
		else if( MAP_ID == MAPID_BlessCastle && SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies )
		{
			//Enemy Clan

			Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, pcUnitData->sPosition, pcImageEnemyIcon->GetWidth() >> 1, pcImageEnemyIcon->GetHeight() >> 1, iSize );

			UI::ImageRender::Render( pcImageEnemyIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageEnemyIcon->GetWidth(), pcImageEnemyIcon->GetHeight(), 0, 0, -1 );

		}
		else if (mapID != MAPID_RicartenTown &&
				 mapID != MAPID_PillaiTown &&
				 mapID != MAPID_BlessCastle)
		{
			//Other players
			if (PARTYHANDLER->GetPartyData().iMembersCount > 0)
			{
				for (int i = 0; i < PARTYHANDLER->GetPartyData().iMembersCount; i++)
				{
					PartyMember sMember = PARTYHANDLER->GetPartyData().saMembers[i];

					//Don't render icon if player is in party. A party icon will be rendered instead
					if (strcmp(sMember.szCharacterName, pcUnitData->GetName()) == 0)
						return TRUE; //need to be true here. see logic above
				}
			}


			Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, pcImageEnemyIcon->GetWidth() >> 1, pcImageEnemyIcon->GetHeight() >> 1, iSize);
			Rectangle2D sBox = Rectangle2D(sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX, sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY, pcImagePlayerIcon->GetWidth(), pcImagePlayerIcon->GetHeight());
			UI::ImageRender::Render(pcImagePlayerIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageEnemyIcon->GetWidth(), pcImageEnemyIcon->GetHeight(), 0, 0, -1);

			if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
				MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
			{
				if (&pcUnitData->sCharacterData)
				{
					std::string strLevel = std::to_string(pcUnitData->sCharacterData.iLevel);
					bool bTwoDigitLevel = strLevel.size() > 1 ? true : false;

					char fullStr[32] = { 0 };
					STRINGFORMAT(fullStr, "%s (%s%c)", pcUnitData->GetName(), bTwoDigitLevel ? strLevel.substr(0, strLevel.size() - 1).c_str() : strLevel.c_str(), bTwoDigitLevel ? 'x' : NULL);

					strHoverUnit = fullStr;
				}
				else
				{
					strHoverUnit = pcUnitData->GetName();
				}

				bIsHoverUnit = TRUE;
				sHoverUnitBox = sBox;
 				cHoverColor = UI::Color(255, 175, 230, 175);
			}
		}

		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// For unknown char type.
/// </summary>
BOOL CMiniMapHandler::RenderUnknown( UnitData * pcUnitData, Point3D sUserPosition, int iSize )
{
	if ( pcUnitData->bActive &&
		pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster &&
		( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN ) )
	{
		return FALSE;
	}

	if (pcUnitData->bActive &&
		pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster &&
		( ( pcUnitData->sCharacterData.bExclusiveBoss || pcUnitData->sCharacterData.sMonsterClass == EMonsterClass::Boss ) ))
	{
		return FALSE;
	}

	if ( pcUnitData->bActive && (pcUnitData->PkMode_CharState == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) )
		return FALSE;

	if ( pcUnitData->bActive && pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		return FALSE;

	if ( pcUnitData->bActive && GM_MODE ) //for GM only for now (need to test)
	{
		Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, pcImageUnknownIcon->GetWidth() >> 1, pcImageUnknownIcon->GetHeight() >> 1, iSize);


		float fRotation = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotation = -0.25f*PI;

		int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
		int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

		int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
		int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

		int iDistFromCenterX = iXLocation - iMapCenterX;
		int iDistFromCenterY = iYLocation - iMapCenterY;

		int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
		int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

		int FinalX = iMapCenterX + iInputRotX + pcImageUnknownIcon->GetWidth()/2 - 5;
		int FinalY = iMapCenterY + iInputRotY ;

		Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcImageUnknownIcon->GetWidth() + 10, pcImageUnknownIcon->GetHeight());

		float fRotationDX = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

		UI::ImageRender::Render(pcImageUnknownIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageUnknownIcon->GetWidth(), pcImageUnknownIcon->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);



		if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
			MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
		{
			if (&pcUnitData->sCharacterData)
			{
				std::string strLevel = std::to_string(pcUnitData->sCharacterData.iLevel);
				bool bTwoDigitLevel = strLevel.size() > 1 ? true : false;

				char fullStr[32] = { 0 };
				STRINGFORMAT(fullStr, "%s (%s%c)", pcUnitData->GetName(), bTwoDigitLevel ? strLevel.substr(0, strLevel.size() - 1).c_str() : strLevel.c_str(), bTwoDigitLevel ? 'x' : NULL);

				strHoverUnit = fullStr;
			}
			else
			{
				strHoverUnit = pcUnitData->GetName();
			}

			bIsHoverUnit = TRUE;
			sHoverUnitBox = sBox;
			cHoverColor = UI::Color(255, 175, 230, 175);
		}
	}

	return TRUE;
}

void CMiniMapHandler::RenderSpawnFlagMarker ( Point3D sSpawnFlagPosition, Point3D sUserPosition, int iSize )
{
	if ( pcSpawnIcon )
	{
		Point2D sPosition = GetMinimapPositionFromCenter ( sUserPosition, sSpawnFlagPosition, pcSpawnIcon->GetWidth () >> 1, pcSpawnIcon->GetHeight () >> 1, iSize );

		float fRotation = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotation = -0.25f*PI;

		int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
		int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

		int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
		int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

		int iDistFromCenterX = iXLocation - iMapCenterX;
		int iDistFromCenterY = iYLocation - iMapCenterY;

		int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
		int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

		int FinalX = iMapCenterX + iInputRotX + pcSpawnIcon->GetWidth()/2 - 5;
		int FinalY = iMapCenterY + iInputRotY;

		Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcSpawnIcon->GetWidth() + 10 , pcSpawnIcon->GetHeight());

		float fRotationDX = 0.0f;
		if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
			fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

		UI::ImageRender::Render(pcSpawnIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcSpawnIcon->GetWidth(), pcSpawnIcon->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);

	}
}


void CMiniMapHandler::RenderMapIndicator( BOOL bIsSecondaryMap, MapIndicator * pMapIndicator, Point3D sUserPosition, int iSize )
{
	if ( pMapIndicator->eType == MapIndicator::Type::NextMapArrow )
	{
		RenderNextMapIndicator( pMapIndicator->iValue, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize );
	}
	else if ( pMapIndicator->eType == MapIndicator::Type::HellSpawnMain )
	{
		RenderMapIndicator( pcMapHellspawnMainImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize );
	}
	else if ( pMapIndicator->eType == MapIndicator::Type::HellSpawnSecondary )
	{
		RenderMapIndicator( pcMapHellspawnSecondaryImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize );
	}
	else if ( pMapIndicator->eType == MapIndicator::Type::BossIcon )
	{
		if (bIsSecondaryMap && !bSecondMapBossActive )
			RenderMapIndicator( pcMapBossImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize );

		if (!bIsSecondaryMap && !bFirstMapBossActive )
			RenderMapIndicator( pcMapBossImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize );
	}
}

void CMiniMapHandler::RenderNextMapIndicator ( int iNextMapID, int iCompactPosX, int iCompactPosZ, int iAngleY, Point3D sUserPosition, int iSize )
{
	if ( iNextMapID < 0 || iNextMapID >= NUM_MAPS )
		return;

	if ( pcMapArrowImage == NULL )
		return;

	std::string desc = "";

	if ( iaMapsLevel[iNextMapID] > 1 )
	{
		desc = FormatString( "%s (Lv. %d+)", pszaMapsName[iNextMapID], iaMapsLevel[iNextMapID] );
	}
	else
	{
		desc = pszaMapsName[iNextMapID];
	}

	RenderMapIndicator( pcMapArrowImage, iCompactPosX, iCompactPosZ, iAngleY, sUserPosition, iSize, desc );
}

void CMiniMapHandler::RenderMapIndicator ( UI::Image * pImage, int iCompactPosX, int iCompactPosZ, int iAngleY, Point3D sUserPosition, int iSize, const std::string szDescription )
{
	if ( pImage == NULL )
		return;

	Point2D sPosition = GetMinimapPositionFromCenter ( sUserPosition, iCompactPosX << 8, iCompactPosZ << 8, pImage->GetWidth () >> 1, pImage->GetHeight () >> 1, iSize );

	float fRotation = 0.0f;
	if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
		fRotation = -0.25f*PI;


	//Angle of User Point
	int iAngle = ( ( iAngleY * 360 ) / PTANGLE_360 ) + 180;


	int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
	int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

	int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
	int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

	int iDistFromCenterX = iXLocation - iMapCenterX;
	int iDistFromCenterY = iYLocation - iMapCenterY;

	int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
	int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

	int FinalX = iMapCenterX + iInputRotX + pImage->GetWidth()/2 - 5;
	int FinalY = iMapCenterY + iInputRotY;

	Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pImage->GetWidth() + 10 , pImage->GetHeight());

	float fRotationDX = 0.0f;
	if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
		fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

	UI::ImageRender::Render ( pImage, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pImage->GetWidth (), pImage->GetHeight (), 0, 0, -1, 0.7f, (float)iAngle );

	if ( !szDescription.empty() )
	{
		FONTHANDLER->Draw( iMiniMapIndicatorFont, sPosition.iX + iMiniMapDeltaX + pcMapArrowImage->GetWidth(), sPosition.iY + iMiniMapDeltaY, szDescription.c_str(), DT_NOCLIP, D3DCOLOR_ARGB( 255, 199, 195, 95 ) );
	}
}

BOOL CMiniMapHandler::RenderParty( Point3D sUserPosition, int iUserID, int iSize )
{
	BOOL found = FALSE;

	if( PARTYHANDLER->GetPartyData().iMembersCount > 0 && SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers )
	{
		for( int i = 0; i < PARTYHANDLER->GetPartyData().iMembersCount; i++ )
		{
			PartyMember sMember = PARTYHANDLER->GetPartyData().saMembers[i];
			UI::Image * pImage = pcaImagesPartyMemberIcon[i];

			if( sMember.sData.sLevel > 0 && (sMember.sData.iID != iUserID) && pImage )
			{
				Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, sMember.sData.sPosition, pImage->GetWidth() >> 1, pImage->GetHeight() >> 1, iSize );

				float fRotation = 0.0f;
				if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
					fRotation = -0.25f*PI;

				int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
				int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

				int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
				int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

				int iDistFromCenterX = iXLocation - iMapCenterX;
				int iDistFromCenterY = iYLocation - iMapCenterY;

				int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
				int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

				int FinalX = iMapCenterX + iInputRotX + pImage->GetWidth()/2 - 5;
				int FinalY = iMapCenterY + iInputRotY;

				Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pImage->GetWidth()+ 20, pImage->GetHeight());

				float fRotationDX = 0.0f;
				if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
					fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

				UI::ImageRender::Render(pImage, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pImage->GetWidth(), pImage->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);


				if( MOUSEHANDLER->GetPosition()->Inside( sBox ) &&
					MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
				{
					bIsHoverUnit = TRUE;
					sHoverUnitBox = sBox;
					strHoverUnit = sMember.szCharacterName;
					cHoverColor = UI::Color( 255, 175, 230, 175 );
				}
			}
		}
	}

	if( PARTYHANDLER->GetPartyData().iRaidCount > 0 && SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers )
	{
		for( int i = 0; i < PARTYHANDLER->GetPartyData().iRaidCount; i++ )
		{
			for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
			{
				PartyRaidMember sMember = PARTYHANDLER->GetPartyData().saRaid[i].saMembers[j];

				if( sMember.sData.iID && sMember.szCharacterName[0] != 0 )
				{
					Point2D sPosition = GetMinimapPositionFromCenter( sUserPosition, sMember.sData.sPosition, pcImageRaidIcon->GetWidth() >> 1, pcImageRaidIcon->GetHeight() >> 1, iSize );

					float fRotation = 0.0f;
					if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
						fRotation = -0.25f*PI;

					int iXLocation = sPosition.iX + GetMiniMapPostion ().iX + iMiniMapDeltaX;
					int iYLocation = sPosition.iY + GetMiniMapPostion ().iY + iMiniMapDeltaY;

					int iMapCenterX =  GetMiniMapPostion ().iX + iMiniMapSize / 2 ;
					int iMapCenterY =  GetMiniMapPostion ().iY + iMiniMapSize / 2 ;

					int iDistFromCenterX = iXLocation - iMapCenterX;
					int iDistFromCenterY = iYLocation - iMapCenterY;

					int iInputRotX = static_cast<int>( iDistFromCenterX * cos( fRotation ) - iDistFromCenterY * sin( fRotation ) );
					int iInputRotY = static_cast<int>( iDistFromCenterY * cos( fRotation ) + iDistFromCenterX * sin( fRotation ) );

					int FinalX = iMapCenterX + iInputRotX + pcImageRaidIcon->GetWidth()/2 - 5;
					int FinalY = iMapCenterY + iInputRotY;

					Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcImageRaidIcon->GetWidth() + 10, pcImageRaidIcon->GetHeight());

					float fRotationDX = 0.0f;
					if (SETTINGSHANDLER->GetModel()->GetSettings().bARPGCamera )
						fRotationDX = ( ( ((PTANGLE_360*3)/8) * -360 ) / PTANGLE_360 ) + 180;

					UI::ImageRender::Render(pcImageRaidIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageRaidIcon->GetWidth(), pcImageRaidIcon->GetHeight(), 0, 0, -1, 1.0f ,fRotationDX);

					if( MOUSEHANDLER->GetPosition()->Inside(sBox) &&
						MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
					{
						bIsHoverUnit = TRUE;
						sHoverUnitBox = sBox;
						strHoverUnit = sMember.szCharacterName;
						cHoverColor = UI::Color( 255, 40, 240, 230 );
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CMiniMapHandler::RenderMapTime()
{
	return TRUE;
}

BOOL CMiniMapHandler::IsCurrentUserMap( int iMapID )
{
	if ( UNITDATA->iLoadedMapIndex >= 0 )
	{
		if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID == iMapID )
			return TRUE;
	}

	return FALSE;
}

BOOL CMiniMapHandler::IsCurrentUserSecondMap( int iMapID )
{
	int iSecondMapIndex = (UNITDATA->iLoadedMapIndex == 0) ? 1 : 0;
	if ( FIELDGAME[iSecondMapIndex] )
	{
		return FIELDGAME[iSecondMapIndex]->iMapID == iMapID;
	}

	return FALSE;
}

BOOL CMiniMapHandler::IsCurrentUserMap( MiniMapData * psMap )
{
	if ( UNITDATA->iLoadedMapIndex >= 0 )
	{
		if ( FIELDGAME[UNITDATA->iLoadedMapIndex] == psMap->pcBaseMap )
			return TRUE;
	}

	return FALSE;
}

void CMiniMapHandler::BeginViewport()
{
	RECT r = { GetMiniMapPostion().iX, GetMiniMapPostion().iY, GetMiniMapPostion().iX + iMiniMapSize, GetMiniMapPostion().iY + iMiniMapSize };
	GetGraphicsDevice()->PushScissorRect( r );
}

void CMiniMapHandler::EndViewport()
{
	GetGraphicsDevice()->PopScissorRect();
}

NAKED void CMiniMapHandler::DrawMinimap( MiniMapData * psMap, int iX, int iY, int iSize )
{
	JMP( 0x00460D50 );
}

void CMiniMapHandler::SetMinimapSize( float fSize )
{
	(*(float*)0x04B0DF44) = fSize / 2.0f;
	(*(float*)0x04B0DF48) = fSize - 2.0f;
}

Point2D CMiniMapHandler::GetMinimapPositionFromCenter( Point3D sCenterPosition, Point3D sObjectPositiion, int iWidthPoint, int iHeightPoint, int iSize )
{
	return GetMinimapPositionFromCenter( sCenterPosition, sObjectPositiion.iX, sObjectPositiion.iZ, iWidthPoint, iHeightPoint, iSize );
}

Point2D CMiniMapHandler::GetMinimapPositionFromCenter( Point3D sCenterPosition, int iDetailedX, int iDetailedZ, int iWidthPoint, int iHeightPoint, int iSize )
{
	//Compute position from center to object
	int iX = iDetailedX - sCenterPosition.iX;
	int iZ = iDetailedZ - sCenterPosition.iZ;

	//Divide points to inside of minimap
	iX /= (iSize / (iMiniMapSize / 2));
	iZ /= (iSize / (iMiniMapSize / 2));

	//Negative Z, since it will up...
	iZ = -iZ;


	return Point2D( iX + (iMiniMapSize >> 1) - iWidthPoint, iZ + (iMiniMapSize >> 1) - iHeightPoint );
}

void CMiniMapHandler::OnResolutionChanged()
{
	ResetMiniMapScreenPositions();
}

void CMiniMapHandler::OnMiniMapSizeChanged()
{
	pRenderTargetMask = GetGraphicsDevice()->MakeRenderTarget(1000, iMiniMapSize, iMiniMapSize);

	if (bLargeMiniMap)
	{
		iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY;
	}
	else
	{
		iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY;
	}

	SetMiniMapBox(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));
	ResetMapTextureDeltas();
}

void CMiniMapHandler::HandlePacket ( PacketMapSpawnMarkers * psPacket )
{
	ClearSpawnFlagMarkers();

	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		if ( psPacket->saMarkers[i].iX != 0 )
		{
			AddSpawnFlagMarker ( psPacket->saMarkers[i].iX << 8, psPacket->saMarkers[i].iY << 8, psPacket->saMarkers[i].iZ << 8 );
		}
	}
}

void CMiniMapHandler::HandlePacket ( PacketMapIndicators * psPacket )
{
	auto it = mMapIndicators.find( psPacket->iMapID );

	if ( it == mMapIndicators.end() )
	{
		PacketMapIndicators packetCopy;
		mMapIndicators.insert( { psPacket->iMapID, packetCopy } );
	}

	ZeroMemory( &mMapIndicators[psPacket->iMapID], sizeof( PacketMapIndicators ) );
	CopyMemory( &mMapIndicators[psPacket->iMapID], psPacket, psPacket->iLength );
}

void CMiniMapHandler::HandlePacket( PacketBossTimeUpdate * psPacket )
{
	strNextBossName = psPacket->szBossName;
	dwNextBossTime	= psPacket->dwTime;

	if ( dwNextBossTime )
		bShowBossTime = TRUE;
	else
		bShowBossTime = FALSE;
}

void CMiniMapHandler::ProcessPacket( PacketBossTimeUpdate * psPacket )
{
	psPacket->iLength = sizeof( PacketBossTimeUpdate );
	psPacket->iHeader = PKTHDR_BossTimeUpdate;
	SENDPACKETG( psPacket );
}

void CMiniMapHandler::OnChangeMap( int iMapID )
{
	//std::cout << "OnChangeMap" << std::endl;
	//vSpawnFlags.clear ();

	if ( CHARACTERGAME->GetLastStageID () == MAPID_Invalid )
	{
		ProcessPacket ( &PacketBossTimeUpdate ( iMapID ) );
	}
}