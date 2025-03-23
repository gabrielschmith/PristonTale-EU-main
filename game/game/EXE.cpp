#include "stdafx.h"
#include "EXE.h"
#include "QuestWindow.h"

std::vector<EXERenderCacheData*> EXERenderCache::vRender;

extern DropItemView saItemsStage[1024];
extern int bDisableSodDmgReduction;


UI::Image * pcLowHPWarningImage = nullptr;
UI::Image * pcBossFixedPositionImages[4] = { 0 };

extern BOOL bLootLabelViewToggleState;

EMouseIcon eLastMouseCursor = EMouseIcon::EMOUSEICON_None;

void EXERenderCache::FinalRender()
{
}

void EXERenderCache::FinalRenderHandler( EXEMesh * pcThis )
{
	EXERenderCacheData * psRenderData = new EXERenderCacheData;

	CopyMemory( &psRenderData->cMesh, pcThis, sizeof( EXEMesh ) );
	CopyMemory( &psRenderData->cRender, EXERender::GetRender(), sizeof( EXEMesh ) );

	vRender.push_back( psRenderData );
}

void EXERenderCache::RenderCacheData()
{
}

NAKED BOOL AddUnitQueueID( int iID )
{
	JMP( 0x0061E640 );
}


void CloseDropItemView(DropItemView* ps)
{
	ps->bActive = FALSE;

	if (ps->pcMesh)
	{
		CALL_WITH_ARG1(0x0045A5F0, (DWORD)ps->pcMesh);	//AddRemoveDynPattern
		ps->pcMesh = NULL;
	}

	ps->pcModel = NULL;
}


BOOL RenderDropItemOverride(DropItemView * ps, BOOL bAllowBlink, BOOL bForceVisible)
{
	Point3D CameraPosi, CameraAngle;
	Point3D angle;
	//	smPAT3D *pat;
	int r, g, b;
	int scale;

	//Check whether the pattern (if you received this communication model for file untested)
	if ((ps == nullptr || (ps->iItemID.ToInt() == 0 && ps->iServerID == 0)))
		return FALSE;

	//The update removes the item requested timeout
	if ((*(DWORD*)0x00CF4750) > (ps->dwLastUpdate + ITEM_TRANS_LIMIT_TIME)) //ITEM_TRANS_LIMIT_TIME = 8s
	{
		//CHATBOX->AddDebugMessage ( "Item Timed out" );

		CloseDropItemView(ps); //Close();
		return FALSE;
	}


	//Item Auras
	if ( MAP_ID != MAPID_RicartenTown && MAP_ID != MAPID_PillaiTown && MAP_ID != MAPID_BlessCastle )
	{
		//Display Legendary Aura
		if ( ps->eRarity == EItemRarity::LEGENDARY )
		{
			int iAura = UNITGAME->iLegendaryDropParticle;

			if ( PARTICLE->IsRunningItem ( ps->iItemID.ToInt(), iAura ) )
			{
				PARTICLE->UpdateItem ( ps->iItemID.ToInt(), iAura, 70.0f );
			}
			else
			{
				int iID = PARTICLE->Start( iAura,
					ps->sPosition.iX,
					ps->sPosition.iY,
					ps->sPosition.iZ );

				PARTICLE->AddScript( iID, iAura, ps, 70.0f, TRUE );

				SKILLMANAGERHANDLER->SkillPlaySound( 0x4950, ps->sPosition.iX, ps->sPosition.iY, ps->sPosition.iZ ); //0x4950 = Legendary Drop Sound
			}
		}
		//Display Epic Aura
		else if ( ps->eRarity == EItemRarity::EPIC )
		{
			int iAura = UNITGAME->iEpicDropParticle;

			if ( PARTICLE->IsRunningItem ( ps->iItemID.ToInt(), iAura ) )
			{
				PARTICLE->UpdateItem ( ps->iItemID.ToInt(), iAura, 70.0f );
			}
			else
			{
				int iID = PARTICLE->Start( iAura,
					ps->sPosition.iX,
					ps->sPosition.iY,
					ps->sPosition.iZ );

				PARTICLE->AddScript( iID, iAura, ps, 70.0f, TRUE );
			}
		}
		//Display Rare Aura
		else if ( ps->eRarity == EItemRarity::RARE )
		{
			int iAura = UNITGAME->iRareDropParticle;

			if ( PARTICLE->IsRunningItem ( ps->iItemID.ToInt(), iAura ) )
			{
				PARTICLE->UpdateItem ( ps->iItemID.ToInt(), iAura, 70.0f );
			}
			else
			{
				int iID = PARTICLE->Start( iAura,
					ps->sPosition.iX,
					ps->sPosition.iY,
					ps->sPosition.iZ );

				PARTICLE->AddScript( iID, iAura, ps, 70.0f, TRUE );
			}
		}
	}



	CameraPosi.iX = UNITTARGETPOSITION_X;
	CameraPosi.iY = UNITTARGETPOSITION_Y;
	CameraPosi.iZ = UNITTARGETPOSITION_Z;

	CameraAngle.iX = UNITTARGETANGLE_X;
	CameraAngle.iY = UNITTARGETANGLE_Y;
	CameraAngle.iZ = UNITTARGETANGLE_Z;

	memcpy(&angle, &ps->sAngle, sizeof(Point3D));

	ps->sSelfPosition.iX = ps->sPosition.iX;
	ps->sSelfPosition.iY = ps->sPosition.iY + 6 * 256;
	ps->sSelfPosition.iZ = ps->sPosition.iZ;

	int	BlinkFlag;

	BlinkFlag = FALSE;

	if ( ps->iItemID.ToItemBase() == ITEMBASE_Weapon &&
		( ( ps->eItemFlags & EItemFlag::TreasureHuntingMode ) != EItemFlag::TreasureHuntingMode ) )
	{
		angle.iX = ANGLE_90;
	}

	if (ps->iItemID.ToInt() && ((*(DWORD*)0x00CF4750) & 0x80))
		BlinkFlag = TRUE;


	//SOD For certain items, the size variation of the field highlighted
	//Disabled because players have third party mods to show items on ground easier.
	/*if (UNITDATA->iLoadedMapIndex >= 0 && CHARACTERGAME->GetStageID() == MAPID_Bellatra)
	{
		//if (ps->iItemID.ToItemBase() == ITEMBASE_Potion || ps->iItemID.ToItemType() == ITEMTYPE_Bellatra)
		//{
			scale = ((*(DWORD*)0x00CF4750) >> 2) & 0x1FF;

			if ((scale & 0x100) != 0)
				scale = 256 - (scale & 0xFF);

			if (ps->iItemID.ToItemBase() == ITEMBASE_Potion)
				scale *= 3;

			WRITEDWORD(0x34DF498, scale + 256); //smRender.ObjectVertexScale
		//}
	}*/

	if (BlinkFlag && bAllowBlink)
	{
		DWORD iR = READDWORD(0x034DF460);
		DWORD iG = READDWORD(0x034DF464);
		DWORD iB = READDWORD(0x034DF468);

		RGB rarityBlinkColor = rarityColors[ps->eRarity];;

		WRITEDWORD(0x034DF460, iR + rarityBlinkColor.red);
		WRITEDWORD(0x034DF464, iG + rarityBlinkColor.green);
		WRITEDWORD(0x034DF468, iB + rarityBlinkColor.blue);

		ps->pcModel->SetPosition(&ps->sSelfPosition, &angle);
		ps->pcModel->uFrame = 0;
		ps->pcModel->Render(&CameraPosi, &CameraAngle);

		WRITEDWORD(0x034DF460, iR);
		WRITEDWORD(0x034DF464, iG);
		WRITEDWORD(0x034DF468, iB);
	}
	else
	{
		ps->pcModel->SetPosition(&ps->sSelfPosition, &angle);
		ps->pcModel->uFrame = 0;
		ps->pcModel->Render(&CameraPosi, &CameraAngle);
	}

	WRITEDWORD(0x34DF498, 0); //smRender.ObjectVertexScale

	return TRUE;
}

//int scITEM::Draw()
//character.cpp
void RenderDropItem( DropItemView * ps )
{
	//CALLT( pfnRenderDropItemView, (DWORD)ps );



}

DropItemView * FindScItem( int x, int z )
{
	int cnt;

	for( cnt=0; cnt < 1024; cnt++)
	{
		if ( saItemsStage[cnt].bActive &&
			 saItemsStage[cnt].sPosition.iX == x  &&
			 saItemsStage[cnt].sPosition.iZ == z )
		{
			return &saItemsStage[cnt];
		}
	}

	return NULL;
}

DropItemView * FindEmptyScItem( )
{
	int cnt;

	for( cnt=0; cnt < 1024; cnt++)
	{
		if ( !saItemsStage[cnt].bActive )
		{
			return &saItemsStage[cnt];
		}
	}

	return NULL;
}

#define STATE_BAR_WIDTH		100
#define STATE_BAR_HEIGHT	16
#define STATE_BAR_SIZE		92

void RenderCharacterHP( UnitData * pcUnitData, int iX, int iY )
{
	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	if ( pcUnit == NULL )
		return;

	const int iCurrHp = pcUnitData->GetCurrentHealth(); // pcUnit->sHPLong.iMin;
	const int iMaxHp  = pcUnitData->GetMaxHealth(); //pcUnit->sHPLong.iMax;

	//CALLT_WITH_ARG2( 0x00421B30, (DWORD)pcUnitData, iX, iY );

	DWORD dwColor;

	// HP Max is 0? no need show
	if( iCurrHp == 0 )
		return;

	// Treepass screen? no show
	if( iX < 0 || ( iX + STATE_BAR_WIDTH ) >= ( *(int*)0x03A9A30C ) ) //0x03A9A30C = DXSettings::WidthBBDevice
		return;

	// Treepass screen? no show
	if( iY < 0 || ( iY + STATE_BAR_WIDTH ) >= ( *(int*)0x03A9A310 ) ) //0x03A9A310 = DXSettings::HeightBBDevice
		return;


	int iPercent;
	int iPercent2;
	int iPercent3;

	// HP Percent
	iPercent  = ( iCurrHp * STATE_BAR_SIZE ) / iMaxHp;
	iPercent2 = ( iCurrHp * 511 ) / iMaxHp;
	iPercent2 = ( iPercent2 * iPercent2 ) / 512;
	iPercent3 = iPercent2 & 0xFF;


	// Color based to percent HP
	if( iPercent2 < 256 )
		dwColor = D3DCOLOR_RGBA( 255, iPercent3, 0, 255 );
	else
		dwColor = D3DCOLOR_RGBA( 255 - iPercent3, 255, 0, 255 );

	// Calculate Bar Size relative to Resolution
	int iHeight = (RESOLUTION_WIDTH / 4) * 3;
	float fSizeH = 0.0f, fSizeW = 0.0f;

	fSizeW = (float)RESOLUTION_WIDTH * (*(float*)0x007AFD8C);

	if( iHeight == RESOLUTION_HEIGHT )
		fSizeH = ((float)RESOLUTION_WIDTH / 800.0f);
	else
		fSizeH = ((float)RESOLUTION_WIDTH / (float)( ( RESOLUTION_HEIGHT / 3 ) * 4 ));

	// Texture 0 = Bar, 1 = Color
	int * piImage = ( (int*)0x00CDFF54 );

	// Functions
	typedef void( __cdecl *tfnDrawTexImageFloat )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, int Transp );
	typedef void( __cdecl *tfnDrawTexImageFloatColor )( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, DWORD dwColor );
	tfnDrawTexImageFloat fnDrawTexImageFloat = (tfnDrawTexImageFloat)0x00409F40;
	tfnDrawTexImageFloatColor fnDrawTexImageFloatColor = (tfnDrawTexImageFloatColor)0x0040A240;

	fSizeH = ((float)RESOLUTION_WIDTH / 800.0f);
	fSizeW = ((float)RESOLUTION_WIDTH / 800.0f);

	// Draw Bar model
	fnDrawTexImageFloat( piImage[0], (float)iX, (float)iY, 60, 11.2, 128, STATE_BAR_HEIGHT, 0, 0, STATE_BAR_WIDTH, 15, 255 );

	// Have HP? Draw inside Bar model
	if( iCurrHp > 0 )
		fnDrawTexImageFloatColor( piImage[1], (float)iX, (float)iY, (float)(iPercent + 3) * 0.6f, 11.2, 128, STATE_BAR_HEIGHT, 0, 0, (float)iPercent + 3, 15, dwColor );


	return;
}

void RenderCharacterChangingBar( UnitData * pcUnitData, int iX, int iY, int iMin, int iMax )
{
	CALLT_WITH_ARG4( 0x0042B320, (DWORD)pcUnitData, iX, iY, iMin, iMax );
}

void ClearDrops()
{
	DropItemView * psItem = NULL;

	for (int i = 0; i < 1024; i++)
	{
		psItem = saItemsStage + i;

		if (psItem && psItem->bActive)
		{
			CloseDropItemView( psItem );
		}
	}
}

/// <summary>
/// Leaked func: int DrawPat3D( int x, int y, int z, int ax, int ay, int az )
/// </summary>
BOOL Render3D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	int iSkillDisplay = UNITDATA->dwDispAppSkill;

	//Reset
	UNITDATA->dwDispAppSkill = 0;

	//Render Success Counter
	int iRenderedCounter = 0;

	//Instance
	Skill * psaSkill = (Skill*)TIMERSKILLHANDLER->GetPointerSkillTimer();


	//Z Distance from selected target
	int iSelectedZ = 65536 * 16384;

	for ( int i = 0; i < 80; i++ )
	{
		Skill * ps = psaSkill + i;

		if ( ps->iFlag )
		{
			if ( ps->iID )
			{
				switch ( ps->iID )
				{
					case SKILLID_ExtremeShield:
						UNITDATA->dwDispAppSkill |= SKILLMASK_ExtremeShield;
						break;

					case SKILLID_HolyReflection:
						UNITDATA->dwDispAppSkill |= SKILLMASK_HolyReflection;
						break;

					case SKILLID_PhysicalAbsorb:
					case SKILLID_MetalArmor:
						UNITDATA->dwDispAppSkill |= SKILLMASK_PhysicalAbsorb;
						break;

					case SKILLID_Maximize:
					case SKILLID_Automation:
						UNITDATA->dwDispAppSkill |= SKILLMASK_Automation;
						break;
					case SKILLID_Zenith:
						UNITDATA->dwDispAppSkill |= SKILLMASK_CriticalHit;
						break;

					case SKILLID_HolyBody:
						UNITDATA->dwDispAppSkill |= SKILLMASK_PhysicalAbsorb;
						UNITDATA->dwDispAppSkill |= SKILLMASK_HolyBody;
						break;

					case SKILLID_SparkShield:
					case SKILLID_DivineShield:
						UNITDATA->dwDispAppSkill |= SKILLMASK_SparkShield;
						break;

					case SKILLID_GodlyShield:
					case SKILLID_FrostJavelin:
					case SKILLID_SummonMuspell:
						UNITDATA->dwDispAppSkill |= SKILLMASK_GodlyShield;
						break;

					default:
						break;
				}
			}
		}
	}


	//Stop Skill Sounds if it finished...
	{
		if ( (UNITDATA->dwDispAppSkill & SKILLMASK_PhysicalAbsorb) == 0 && (iSkillDisplay & SKILLMASK_PhysicalAbsorb) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_PhysicalAbsorb3 );

		if ( (UNITDATA->dwDispAppSkill & SKILLMASK_Automation) == 0 && (iSkillDisplay & SKILLMASK_Automation) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_Automation3 );

		if ( (UNITDATA->dwDispAppSkill & SKILLMASK_HolyBody) == 0 && (iSkillDisplay & SKILLMASK_HolyBody) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_PhysicalAbsorb3 );
	}

	//Set Camera of Render
	RENDERER->SetCameraPosition( iX, iY, iZ, iAX, iAY, iAZ );


	//Set Dynamic Lights to the map
	if ( STAGE1 )
		SetStageDynamicLight( STAGE1, iX, iY, iZ );
	if ( STAGE2 )
		SetStageDynamicLight( STAGE2, iX, iY, iZ );


	//Set Camera of Character
	CAMERACHARACTER( iX, iY, iZ, iAX, iAY, iAZ );


	int iRestarterTime = RESTARTCOUNTER;
	if ( UNITDATA->bNoDeath )
		iRestarterTime = (int)UNITDATA->bNoDeath;

	//Restart Color
	if ( iRestarterTime > 0 && (iRestarterTime & 0x10) == 0 )
	{
		//Keep it safe...
		int R = RENDERER->r;
		int B = RENDERER->b;

		RENDERER->r += 80;
		RENDERER->b += 80;

		UNITDATA->Render();

		RENDERER->r = R;
		RENDERER->b = B;
	}
	else
	{
		//Update Vanish need to render again??? ugly...
		if ( UNITDATA->iVanishTimeLeft <= 0 && UNITDATA->iBlindSkill <= 0 )
			UNITDATA->Render();
	}

	//Clan Master of BC? Set Crown...
	if ( CASTLEMASTERID && UNITDATA->sCharacterData.iClanID )
		SetClanMaster( CASTLEMASTERID );


	//Reset Target
	UnitData * pcUnitDataSelected = NULL;

	//All Characters Render...
	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		//Character Displayed?
		if ( pc->bActive && pc->DisplayAlpha == 0 )
		{
			//JLM - Rendering Experiment
			//Far from Distance? next Character...
			/*if ((pc->iCharacterType == CHARACTERTYPE_Player || pc->sCharacterData.iType == CHARACTERTYPE_Player) &&
				UNITDATA->sPosition.WithinXZDistance(&pc->sPosition, DISTANCE_XY_16) == FALSE)
			{
				continue;
			}

			if ((CHARACTERGAME->GetStageID() == MAPID_RicartenTown || CHARACTERGAME->GetStageID() == MAPID_BlessCastle) && true || RENDERDISTANCE_ENABLE)
			{

			}*/

			int R = RENDERER->r;
			int G = RENDERER->g;
			int B = RENDERER->b;

			//Update Pet Skill
			if ( UNITDATA->iPetID && pc->GetCurrentHealth() > 0 )
			{
				//Scout Hawk Buff?
				if ( UNITDATA->iPetID == SKILLID_ScoutHawk )
				{
					//Note: Get Attack Rating from Scout Hawk

					//Update Character Color based on Dark
					if ( GetDamageFromAreaSkill( UNITDATA, pc, 0, 0, 0 ) > 0 )
					{
						int iColor = DARKCOLOR_AMOUNT >> 2;

						RENDERER->r += iColor + 40;
						RENDERER->g += iColor + 40;
						RENDERER->b += iColor + 100;
					}
				}
			}

			DWORD dwOutlineEffectColor = 0;

			//Outline Update
			if ( UNITDATA_HOVER && UNITDATA_HOVER == pc )
			{
				RENDERER->r = R + 40;
				RENDERER->g = G + 60;
				RENDERER->b = B + 60;

				//Red
				dwOutlineEffectColor = D3DCOLOR_ARGB( 255, 255, 30, 0 );

				//Green
				if (UNITDATA_HOVER->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
					dwOutlineEffectColor = D3DCOLOR_ARGB(255, 0, 255, 100);

				//White
				if ( UNITDATA_HOVER->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player )
					dwOutlineEffectColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );

				//Compute Distance...
				int a = 0, b = 0, c = 0, d = 0, e = 0;

				Point3D sPositionC = Point3D(iX, iY, iZ);

				UNITDATA_HOVER->sPosition.GetPTDistanceXZY(&sPositionC, a, b, c, d, e);

				int iDistance = (int)sqrt(a);
			}

			if ( pc->bRenderLast == FALSE && pc->iIceOverlay == 0 && pc->RendAlpha == 0 && (pc->iVanishTimeLeft <= 0 && pc->iBlindSkill <= 0) )
			{
				//Athanasia Color
				if ( pc->bNoDeath && ((int)pc->bNoDeath & 0x10) == 0 )
				{
					RENDERER->r += 80;
					RENDERER->b += 80;
				}

				//if ( dwOutlineEffectColor > 0 )
				//{
				//	DX::cSelectGlow1.Begin( dwOutlineEffectColor, TRUE );
				//	pc->Render();
				//	DX::cSelectGlow1.End( TRUE );
				//	DX::cSelectGlow1.Begin( dwOutlineEffectColor, FALSE );
				//	pc->Render();
				//	DX::cSelectGlow1.End( FALSE );
				//}

				if ( (dwOutlineEffectColor > 0) )
				{
					if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, TRUE ) )
					{
						RenderUnitData( pc );
						DX::cSelectGlow1.End( TRUE );

						if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, FALSE ) )
						{
							RenderUnitData( pc );
							DX::cSelectGlow1.End( FALSE );
						}
					}
				}
				RenderUnitData(pc);
				//pc->Render();
			}
			else
			{
				//if ( dwOutlineEffectColor > 0 )
				//{
				//	DX::cSelectGlow1.Begin( dwOutlineEffectColor, TRUE );
				//	CALLT( 0x00420AC0, (DWORD)pc );
				//	DX::cSelectGlow1.End( TRUE );
				//	DX::cSelectGlow1.Begin( dwOutlineEffectColor, FALSE );
				//	CALLT( 0x00420AC0, (DWORD)pc );
				//	DX::cSelectGlow1.End( FALSE );
				//}
				//
				////Fake Render?
				//CALLT( 0x00420AC0, (DWORD)pc );
			}

			//Reset Color and Glow
			RENDERER->r = R;
			RENDERER->g = G;
			RENDERER->b = B;

			//Rendered?
			if ( pc->bRendered )
			{
				//Increment to Handle it later...
				iRenderedCounter++;

				int iZP = pc->sRenderPoint.iZ;

				RECT * psRect2D = &pc->rRenderRectangle;

				//Check Siege War for Defending Clan
				bool bCanSelect = true;
				if ( MAP_ID == MAPID_BlessCastle )
				{

					if ( UNITDATA->sCharacterData.iClanID && BLESSCASTLEHANDLER->GetClanID( UNITDATA->sCharacterData.iClanID ) == BLESSCASTLEHANDLER->GetCastleLeaderClanID() && BLESSCASTLEHANDLER->InSiegeWar() )
					{
						if ( (pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_TOWER_B) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_DOOR) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_R) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_G) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_B) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_CRYSTAL_N) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_A) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_B) ||
							(pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CASTLE_SOLDER_C) )
							bCanSelect = false;
					}
				}
				//In distance and inside 2D Plane and select on mouse?
				if (((pc->sCharacterData.sHP.sCur > 0) || (GM_MODE && (pc->GetAnimation() != ANIMATIONTYPE_Die))) && iZP > (96 << 8) && iZP < iSelectedZ &&
					psRect2D->left < MOUSEHANDLER->GetPosition()->iX && psRect2D->right > MOUSEHANDLER->GetPosition()->iX &&
					psRect2D->top < MOUSEHANDLER->GetPosition()->iY && psRect2D->bottom > MOUSEHANDLER->GetPosition()->iY)
				{
					if ( (pc->dwLastActiveTime + 7000) > TICKCOUNT )
					{
						//Set new target
						iSelectedZ			= iZP;
						pcUnitDataSelected	= pc;
					}
				}
			}
		}
	}

	int iLastRenderedCounter = iRenderedCounter;

	//Press A key?
	if ( ISPRESSEDKEY('A') )
	{
		//Reset target
		UNITDATA_HOVER	= NULL;
		iSelectedZ			= 65536 * 16384;

		pcUnitDataSelected	= NULL;
	}

	int iRenderedItems = 0;

	RECT sRect;
	Point2D sPosition2D;

	DropItemView * psItem = NULL;

	RECT sRectS;
	Point2D sPosition2DS;

	for ( int i = 0; i < 1024; i++ )
	{
		DropItemView * ps = saItemsStage + i;

		if ( ps == NULL )
			continue;

		if ( ps->bActive )
		{
			//GM Debug or is an Item?
			if ( (ps->iItemID.ToInt() || DEBUG_GM ) )
			{
				//Force bVisible to FALSE to fix mouse hover issues
				//when filtered out items are rendered near pickable items
				ps->bVisible = FALSE;

				//It's a gold? So format the number
				if ( ps->iItemID.ToItemID() == ITEMID_Gold && ps->bFormatted == 0x0 )
				{
					ITEMHANDLER->FormatDropItemGold( ps );
				}

				if ((ITEMVIEW_SELECTED == ps) )
				{
					DWORD dwColor = D3DCOLOR_XRGB(255, 255, 0);

					if (DX::cSelectGlow2.Begin(dwColor, TRUE))
					{
						RenderDropItem(ps);
						DX::cSelectGlow2.End(TRUE);

						if (DX::cSelectGlow2.Begin(dwColor, FALSE))
						{
							RenderDropItem(ps);
							DX::cSelectGlow2.End(FALSE);
						}
					}
				}

				//if ( ITEMVIEW_SELECTED == ps )
				//{
				//	DWORD dwColor = D3DCOLOR_XRGB( 255, 255, 0 );
				//	DX::cSelectGlow2.Begin( dwColor, TRUE );
				//	RenderDropItem( ps );
				//	DX::cSelectGlow2.End( TRUE );
				//	DX::cSelectGlow2.Begin( dwColor, FALSE );
				//	RenderDropItem( ps );
				//	DX::cSelectGlow2.End( FALSE );
				//}

				//Render it
				//RenderDropItem(ps);
				RenderDropItemOverride(ps, TRUE, TRUE);

				//Set Camera of Render
				RENDERER->SetCameraPosition( iX, iY, iZ, iAX, iAY, iAZ );

				//Get 2D Plane of item
				int iZP = RENDERER->GetRectangle2D( ps->sPosition.iX, ps->sPosition.iY + (16 << 8), ps->sPosition.iZ, 32 << 8, 32 << 8, &sRect, &sPosition2D );

				//Set to Item
				ps->sRenderPosition.iX = sPosition2D.iX;
				ps->sRenderPosition.iY = sPosition2D.iY;
				ps->sRenderPosition.iZ = iZP;

				//Item on plane?
				if ( iZP > 0 && iZP < iSelectedZ &&
					sRect.left < MOUSEHANDLER->GetPosition()->iX && sRect.right > MOUSEHANDLER->GetPosition()->iX &&
					sRect.top < MOUSEHANDLER->GetPosition()->iY && sRect.bottom > MOUSEHANDLER->GetPosition()->iY )
				{
					//Set it selected...
					iSelectedZ	= iZP;
					psItem		= ps;
					CopyMemory( &sRectS, &sRect, sizeof( RECT ) );
					sPosition2DS = sPosition2D;
				}
			}
		}
		else
		{
			if ( ps->eRarity >= EItemRarity::RARE )
			{
				PARTICLE->DeleteItem ( ps->iItemID.ToInt (), UNITGAME->iLegendaryDropParticle );
				PARTICLE->DeleteItem ( ps->iItemID.ToInt (), UNITGAME->iEpicDropParticle );
				PARTICLE->DeleteItem ( ps->iItemID.ToInt (), UNITGAME->iRareDropParticle );
			}
		}
	}

	BOOL bIgnoreMouse = FALSE;

	//Target selected?
	if ( pcUnitDataSelected )
	{
		//Set It
		UNITDATA_HOVER	= pcUnitDataSelected;

		psItem				= NULL;
	}
	else
	{
		//Otherwise, reset
		UNITDATA_HOVER = NULL;
	}

	//Item selected?
	if ( psItem )
	{
		RECT * psSelectR = (RECT*)0x00CF4208;
		Point2D * psSelect2D = (Point2D*)0x00CF3FBC;

		//Set It
		CopyMemory( psSelectR, &sRectS, sizeof( RECT ) );
		CopyMemory( psSelect2D, &sPosition2DS, sizeof( Point2D ) );

		ITEMVIEW_SELECTED = psItem;
	}
	else
	{
		//Otherwise, reset
		ITEMVIEW_SELECTED = NULL;
	}

	if ( eLastMouseCursor != MOUSE_ICON )
	{
		//invalidate item info box, so that we show repair / sale price correctly
		ITEMINFOBOX->Invalidate();
		eLastMouseCursor = MOUSE_ICON;
	}

	if ( WINDOW_ISOPEN_NPC_SHOP || MOUSEHANDLER->IsNoMove() || MOUSEHANDLER->IsDisabled() || WINDOW_ISOPEN_PERSONALSHOP_OTHERS )
	{
		UNITDATA_HOVER = NULL;
		ITEMVIEW_SELECTED = NULL;

		if ( MOUSE_ICON == EMOUSEICON_Attack || MOUSE_ICON == EMOUSEICON_Talk || MOUSE_ICON == EMOUSEICON_GetItem1 || MOUSE_ICON == EMOUSEICON_GetItem2 )
			MOUSE_ICON = EMOUSEICON_Default;

		return TRUE;
	}

	MOUSE_ICON = EMOUSEICON_None;

	if ( ITEMHANDLER->GetMouseItem()->bValid == FALSE )
	{
		MOUSE_ICON = EMOUSEICON_Default;

		if ( UNITDATA_HOVER )
		{
			if ( UNITDATA_HOVER->sCharacterData.iType == CHARACTERTYPE_Monster )
				MOUSE_ICON = EMOUSEICON_Attack;
			else if ( UNITDATA_HOVER->sCharacterData.iType == CHARACTERTYPE_NPC )
				MOUSE_ICON = EMOUSEICON_Talk;
		}

		if ( ITEMVIEW_SELECTED )
		{
			//if (ITEMHANDLER->CanViewDropItem(ITEMVIEW_SELECTED))
			//{
				if (ISMOUSELEFT_CLICKED)
					MOUSE_ICON = EMOUSEICON_GetItem2;
				else
					MOUSE_ICON = EMOUSEICON_GetItem1;
			//}
		}
	}

	return TRUE;
}

NAKED BOOL Render3DAlpha()
{
	JMP( pfnRender3DAlpha );
}

void WINAPI Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	POLYCOUNT = 0;

	//Render Sight
	if ( RENDERER->iGameFieldView )
	{
		VIEWPOINTSIGHT = 38 * 64 + RENDERSIGHTPOSITION;
		ZDISTANCEMIN = VIEWPOINTSIGHT - (20 * 64 + RENDERSIGHTPOSITION / 4);
	}

	//Y Check?
	if ( DEBUG_USER == FALSE )
	{
		Stage * pcStage = STAGE2 != NULL ? STAGE2 : STAGE1;

		if ( pcStage )
		{
			int iMapY = pcStage->GetHighestPoint( iX, iZ );

			if ( iMapY > iY )
			{
				iY = iMapY;

				iY += (8 << 8);
			}
		}
	}

	//Video Ram
	CALL( 0x00457990 );

	//Update Color...
	RENDERER->r = (-DARKCOLOR_AMOUNT) + COLOR_R;
	RENDERER->g = (-DARKCOLOR_AMOUNT) + COLOR_G;
	RENDERER->b = (-DARKCOLOR_AMOUNT) + COLOR_B;
	RENDERER->a = BRIGHTNESS;

	iY += 32 << 8;

	//Y Distance?
	if ( ANGLE_X <= 40 )
	{
		if ( GAME_DISTANCE <= 100 )
			iY -= (110 - GAME_DISTANCE) << 8;
	}

	ACTIONMODE = FALSE;

	TRACETARGET->iX = iX;
	TRACETARGET->iY = iY;
	TRACETARGET->iZ = iZ;

	//Camera Target Update... auto mode
	if ( ISAUTOCAMERA )
	{
		UpdateMovementCamera();

		iX = TRACECAMERA->iX;
		iY = TRACECAMERA->iY;
		iZ = TRACECAMERA->iZ;
	}

	//Compute Y Player
	int iYBase = UNITDATA->sPosition.iY + (32 << 8);

	if ( ANGLE_X <= 40 )
	{
		if ( GAME_DISTANCE <= 100 )
			iYBase += (100 - GAME_DISTANCE) << 8;
	}

	BOOL bWalking = FALSE;

	//Camera
	EXECameraTrace sCamera;

	if ( UNITDATA && UNITDATA->iLoadedMapIndex >= 0 && FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Distorted )
	{
		//Update Camera
		SetupCamera( &sCamera, &iX, &iY, &iZ );

		iAX = sCamera.sAngle.iX;
		iAY = sCamera.sAngle.iY;

		//Set Camera Position
		RENDERER->SetCameraPosition( iX, iY, iZ, &sCamera.sRotationMatrix );

		ACTIONMODE = TRUE;
	}
	else if ( DEBUG_USER == FALSE )
	{
		//Setup Camera Matrix
		SetupCameraMatrix( &sCamera, iX, iY, iZ, UNITDATA->sPosition.iX, iYBase, UNITDATA->sPosition.iZ );

		iAX = sCamera.sAngle.iX;
		iAY = sCamera.sAngle.iY;

		//Set Camera Position
		RENDERER->SetCameraPosition( iX, iY, iZ, &sCamera.sRotationMatrix );
	}

	RENDERER->ClearLight();

	if ( DARKCOLOR_AMOUNT > 0 )
	{
		if ( UNITDATA->iLoadedMapIndex >= 0 )
		{
			int iLight = DARKCOLOR_AMOUNT + (DARKCOLOR_AMOUNT >> 2);

			//Dungeon Map?
			if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Dungeon )
			{
				DARKLIGHTCOLOR_RANGE = 400 << 8;

				iLight = DARKCOLOR_AMOUNT;
			}
			else
			{
				DARKLIGHTCOLOR_RANGE = 260 << 8;
			}

			if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
			{
				//Nothing...
			}
			else
			{
				//Add Dynamic Light
				RENDERER->AddDynamicLight( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY + (32 << 8), UNITDATA->sPosition.iZ, iLight, iLight, iLight, 0, DARKLIGHTCOLOR_RANGE );
			}
		}
	}

	//Update to render the lights
	SetDynamicLights();

	//Render Sky
	RenderSky( iX, iY, iZ, iAX, iAY, iAZ );

	//Update Fog Render State
	RENDERER->UpdateFogState( TRUE );

	//Apply Lights on next render
	RENDERER->bApplyObjectLight = TRUE;

	WRITEDWORD( 0x04B0D9E0, FALSE );

	Render3D( iX, iY, iZ, iAX, iAY, iAZ );

	//Not Apply lights on next render
	RENDERER->bApplyObjectLight = FALSE;

	POLYCOUNT = RenderStage( iX, iY, iZ, iAX, iAY, iAZ );

	RENDERER->bApplyObjectLight = TRUE;

	Render3DAlpha();

	//Clear Lights
	RENDERER->bApplyObjectLight = FALSE;
	RENDERER->iObjectLightCount = 0;

	RENDERER->UpdateFogState( FALSE );

	RENDERER->ClearLight();
	RENDERER->r = 0;
	RENDERER->g = 0;
	RENDERER->b = 0;
	RENDERER->a = 0;

	//Crown Renderer
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( ISSHOW_INTERFACE )
		{
			for ( UINT i = 0; i < MAX_UNITS; i++ )
			{
				if ( UNITGAME->pcaUnitData[i].bActive && UNITGAME->pcaUnitData[i].bRendered )
					RenderBlessCastleCrown( &UNITGAME->pcaUnitData[i] );
			}

			RenderBlessCastleCrown( UNITDATA );
		}
	}

	RenderModelShadow( iX, iY, iZ, iAX, iAY, iAZ );

	UpdateUITextures();

	RenderEffects( iX, iY, iZ, iAX, iAY, iAZ );

	Render3DEngine( iX, iY, iZ, iAX, iAY, iAZ );

	RenderModel2D( iX, iY, iZ, iAX, iAY, iAZ );

	//Run Mode
	if ( FOLLOWMODE_ACTION == FALSE && ISMOUSELEFT_CLICKED == 0 || UNITDATA->sCharacterData.sSP.sCur > (UNITDATA->sCharacterData.sSP.sMax >> 2) )
	{
		UNITDATA->bRunMode = RUN_UIMODE;

		if ( ACTIONMODE )
			UNITDATA->bRunMode = ACTIONMODE_MOVE;
	}
	else
	{
		if ( UNITDATA->sCharacterData.sSP.sCur == 0 )
			UNITDATA->bRunMode = FALSE;
	}

	RENDERER->CloseCamera();

	if( !pcLowHPWarningImage )
		pcLowHPWarningImage = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\lowhpwarning.png" );

	//Low HP Warning
	if( CHARACTERGAME->GetCurrentHP() > 0 && UNITDATA->psModelAnimation->iType != ANIMATIONTYPE_Die )
	{
		float fDelta = 0.5f - (float)CHARACTERGAME->GetCurrentHP() / (float)UNITDATA->sCharacterData.sHP.sMax;
		if( fDelta > 1.0f )
			fDelta = 1.0f;
		else if( fDelta < 0.0f )
			fDelta = 0.0f;

		UI::ImageRender::Render( pcLowHPWarningImage, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, D3DCOLOR_ARGB( (int)(fDelta * 255), 255, 255, 255 ) );
	}

	if (ISBIGMAP > 0)
		FakeRenderGameState();
	else
		RenderGameState();

	DX::cSelectGlow1.Draw();
	DX::cSelectGlow2.Draw();

	TEXTUREFRAME++;
}

BOOL IsCreatedNewRenderTarget()
{
	return (BOOL)CALL( 0x0046EE40 );
}

NAKED void SetStageDynamicLight( Stage * pcStage, int iX, int iY, int iZ )
{
	JMP( pfnSetDynamicLight );
}

NAKED void SetCharacterCamera( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnSetCameraCharacter );
}

NAKED void SetClanMaster( DWORD dwID )
{
	JMP( pfnSetCastleMaster );
}

NAKED int GetDamageFromAreaSkill( UnitData * pcUnitData, UnitData * pcTarget, int iDestX, int iDestY, int iDestZ )
{
	JMP( pfnGetDamageFromArea );
}

NAKED int SetupCamera( EXECameraTrace * psCamera, int * iX, int * iY, int * iZ )
{
	JMP( pfnSetupCamera );
}

NAKED int SetupCameraMatrix( EXECameraTrace * psCamera, int iX, int iY, int iZ, int iDX, int iDY, int iDZ )
{
	JMP( pfnSetupCameraMatrix );
}

NAKED void RenderSky( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderSky );
}

NAKED int RenderStage( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderStage );
}

NAKED int RenderModelShadow( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderModelShadow );
}

NAKED void UpdateUITextures()
{
	JMP( pfnUpdateUITextures );
}

NAKED void RenderEffects( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderEffects );
}

NAKED void RenderModel2D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderModel2D );
}

void Render3DEngine( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	CALLT_WITH_ARG6( pfnRender3DEngine, 0x03707170, iX, iY, iZ, iAX, iAY, iAZ );
}

NAKED void RenderZoomMap()
{
	JMP( pfnRenderZoomMap );
}

/// <summary>
/// 2012 source: OpenEachPlayer
/// </summary>
NAKED void PrepareUnitDataHUD( UnitData * p )
{
	JMP( 0x004558F0 );
}



void RenderUnitDataHUD( Point2D * sCenterPos, int iCameraDistance, int iType )
{
	UnitData * pcCurrentTarget = (*(UnitData**)0x0CF48D0);

	//Render UnitData
	if ( TARGET_UNITDATA->bActive && pcCurrentTarget && pcCurrentTarget->bActive )
	{
		if ( pcCurrentTarget->sCharacterData.bExclusiveBoss )
		{
			if ( HUDHANDLER->GetTargetUnitData() )
			{
				HUDHANDLER->UpdateTargetInfo( HUDHANDLER->GetTargetUnitData() );
				HUDHANDLER->SetRenderTarget( TRUE );
			}
			else
				HUDHANDLER->SetRenderTarget( FALSE );
		}
		else if ( !pcCurrentTarget->sCharacterData.bExclusiveBoss &&
			       CHARACTERGAME->GetStageID() != MAPID_Bellatra) //don't render in Bellatra SOD
		{
			if (HUDHANDLER->UpdateTargetInfo(pcCurrentTarget))
			{
				HUDHANDLER->SetRenderTarget(TRUE);
			}
		}
	}
	else
	{
		HUDHANDLER->SetRenderTarget(FALSE);
	}

	//Prepare UnitData to Render, if dont have yet
	if ( UNITDATA_FOCUSED )
	{
		if ( pcCurrentTarget != UNITDATA_FOCUSED )
		{
			WRITEDWORD( 0x0CF48D4, FALSE );
			PrepareUnitDataHUD( UNITDATA_FOCUSED );
			WRITEDWORD( 0x00CF48DC, TRUE );
		}
	}
	else if ( UNITDATA_HOVER && pcCurrentTarget != UNITDATA_HOVER )
	{
		PrepareUnitDataHUD( UNITDATA_HOVER );
		WRITEDWORD( 0x00CF48DC, TRUE );
	}
	//Render Character Debug Text
	if ( GM_MODE && *(int*)0x35E11D8 != 0 && DEBUG_GM )
		UNITGAME->RenderCharacterDebugText();
}

NAKED void UpdateMovementCamera()
{
	JMP( pfnUpdateMovementCamera );
}

NAKED void FakeRenderGameState()
{
	JMP( pfnFakeRenderGameState );
}

BOOL RenderGameState()
{
	if( pcBossFixedPositionImages[0] == NULL )
		pcBossFixedPositionImages[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bossbackground.png" );

	if( pcBossFixedPositionImages[1] == NULL )
		pcBossFixedPositionImages[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bosshp.png" );

	if( pcBossFixedPositionImages[2] == NULL )
		pcBossFixedPositionImages[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bosshp_.png" );

	if( pcBossFixedPositionImages[3] == NULL )
		pcBossFixedPositionImages[3] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\type\\background.png" );

	int iOldDarkColor = 0;

	RENDERER->ClearLight();

	iOldDarkColor = DARKCOLOR_AMOUNT;
	DARKCOLOR_AMOUNT = 0;

	UnitData * pcUnitData = NULL, * pcBossUnitData = NULL;

	BOOL bDisplayItemLabel = FALSE;

	if (SETTINGSHANDLER->GetModel()->GetSettings().bItemViewTogglableA)
		bDisplayItemLabel = bLootLabelViewToggleState;
	else
		bDisplayItemLabel = ISPRESSEDKEY('A');


	if ( bDisplayItemLabel )
	{
		for ( int i = 0; i < 1024; i++ )
		{
			DropItemView * ps = saItemsStage + i;

			if ( ps->bActive &&
				 ps->iItemID.ToInt() != 0 &&
				 ps != ITEMVIEW_SELECTED &&
				 (ps->eItemFlags & EItemFlag::TreasureHuntingMode) == EItemFlag::None)
			{
				if (UNITDATA->sPosition.WithinXZYDistance(&ps->sPosition, DISTANCE_XY_20_meters))
				{
					//Draw Box
					MESSAGEBALLOON->ResetMessage();

					ESkinID ItemSkin = SKINID_Normal;

					if ( ps->eRarity != EItemRarity::NONE )
						ItemSkin = ESkinID(static_cast<int>(ps->eRarity) + 3);

					MESSAGEBALLOON->SetSkin(ItemSkin);

					MESSAGEBALLOON->SetMessage(ps->szName);

					ARGBNew rarityColor = rarityColorsGround[ps->eRarity];
					MESSAGEBALLOON->SetColor ( RGBA ( rarityColor.red, rarityColor.green, rarityColor.blue, rarityColor.alpha ) );
					MESSAGEBALLOON->Render(ps->sRenderPosition.iX, ps->sRenderPosition.iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
				}
			}
		}
	}

	int iZoomAmount = 0;

	if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera )
	{
		CAMERA->iCameraZoom = 530;
		CAMERA->iCameraAngle = 500;
		CAMERA->iCameraTurn = 500;
	}
	else if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera != TRUE &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoom &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoomTown != TRUE )
	{
		iZoomAmount = 100 + SETTINGSHANDLER->GetModel ()->GetSettings ().iZoomLevel * 9;

		if ( iZoomAmount < 100 )
			iZoomAmount = 100;
		else if ( iZoomAmount > 1000 )
			iZoomAmount = 1000;

		CAMERA->iCameraZoom = iZoomAmount;
		CAMERA->iCameraAngle = -1;
		CAMERA->iCameraTurn = -1;
	}
	else if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera != TRUE &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoom &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoomTown )
	{
		if ( MAP_ID == MAPID_PillaiTown || MAP_ID == MAPID_RicartenTown )
			iZoomAmount = 100 + SETTINGSHANDLER->GetModel ()->GetSettings ().iZoomTownLevel * 9;
		else
			iZoomAmount = 100 + SETTINGSHANDLER->GetModel ()->GetSettings ().iZoomLevel * 9;

		if ( iZoomAmount < 100 )
			iZoomAmount = 100;
		else if ( iZoomAmount > 1000 )
			iZoomAmount = 1000;

		CAMERA->iCameraZoom = iZoomAmount;
		CAMERA->iCameraAngle = -1;
		CAMERA->iCameraTurn = -1;
	}
	else if ( SETTINGSHANDLER->GetModel ()->GetSettings ().bARPGCamera != TRUE &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoom != TRUE &&
		SETTINGSHANDLER->GetModel ()->GetSettings ().bCustomZoomTown )
	{
		if ( MAP_ID == MAPID_PillaiTown || MAP_ID == MAPID_RicartenTown )
		{
			iZoomAmount = 100 + SETTINGSHANDLER->GetModel ()->GetSettings ().iZoomTownLevel * 9;

			if ( iZoomAmount < 100 )
				iZoomAmount = 100;
			else if ( iZoomAmount > 1000 )
				iZoomAmount = 1000;

			CAMERA->iCameraZoom = iZoomAmount;
			CAMERA->iCameraAngle = -1;
			CAMERA->iCameraTurn = -1;
		}
		else
		{
			CAMERA->iCameraZoom = -1;
			CAMERA->iCameraAngle = -1;
			CAMERA->iCameraTurn = -1;
		}

	}
	else
	{
		CAMERA->iCameraZoom = -1;
		CAMERA->iCameraAngle = -1;
		CAMERA->iCameraTurn = -1;
	}


	if (SETTINGSHANDLER->GetModel()->GetSettings().bShowNameAboveHead)
	{
		UnitData* pcUnitData = UNITDATA;
		Unit* pcUnit = UNITDATATOUNIT(pcUnitData);
		int iY = 8 + ((*(int*)0x00824FA8) >> 6);


		if (!(UNITDATA->szMessage[0]))
		{
			MESSAGEBALLOON->ResetMessage();

			if(SETTINGSHANDLER->GetModel()->GetSettings().bShowFrameAboveHead)
				MESSAGEBALLOON->SetSkin( SKINID_Normal );
			else
				MESSAGEBALLOON->SetSkin( SKINID_Transparent );

			MESSAGEBALLOON->SetColor(D3DCOLOR_XRGB(255, 255, 255));
			MESSAGEBALLOON->AddCharTitleText(pcUnit->szTitle, pcUnit->sTitleRarity);
			MESSAGEBALLOON->SetMessage(pcUnitData->GetName());
			MESSAGEBALLOON->Render(pcUnitData->sRenderPoint.iX, pcUnitData->rRenderRectangle.top + iY - 10, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
		}

	}

	if ( ITEMVIEW_SELECTED && UNITDATA_HOVER == NULL && UNITDATA_FOCUSED == NULL )
	{
		Point2D* psSelect2D = (Point2D*)0x00CF3FBC;

		//X
		(*(int*)0x00A16314) = psSelect2D->iX;
		//Y
		(*(int*)0x00A17534) = psSelect2D->iY;

		//Draw Selected Item.
		MESSAGEBALLOON->ResetMessage();

		ESkinID ItemSkin = SKINID_Normal;

		if ( ITEMVIEW_SELECTED->eRarity != EItemRarity::NONE )
			ItemSkin = ESkinID(static_cast<int>(ITEMVIEW_SELECTED->eRarity) + 3);

		MESSAGEBALLOON->SetSkin(ItemSkin);
		MESSAGEBALLOON->SetMessage(ITEMVIEW_SELECTED->szName);

		ARGBNew rarityColor = rarityColorsGroundHover[ITEMVIEW_SELECTED->eRarity];

		MESSAGEBALLOON->SetColor ( RGBA ( rarityColor.red, rarityColor.green, rarityColor.blue, rarityColor.alpha ) );
		MESSAGEBALLOON->Render(psSelect2D->iX, psSelect2D->iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
	}

	CHARACTERMESSAGE_COUNT = 0;

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		if( pc->bActive && pc->bRendered && pc->sCharacterData.szName[0] )
		{
			//Party Handler
			if( PARTYHANDLER->IsPartyMember( pc->iID ) )
				pc->PartyFlag = 1;
			else if( PARTYHANDLER->IsRaidMember( pc->iID ) )
				pc->PartyFlag = 2;
			else
				pc->PartyFlag = 0;

			//Exclusive Boss
			if( pc->sCharacterData.bExclusiveBoss && !pc->sCharacterData.bGrandFuryBoss )
				pcBossUnitData = pc;

			int iSelected = 0;

			if ( pc->sRenderPoint.iZ < (64 * 32 * 256) )
			{
				Unit * pcUnit = UNITDATATOUNIT ( pc );

				if ( pcUnit && pcUnit->vDamageInfoList.size () > 0 )
				{
					DAMAGEINFOHANDLER->RenderUnitDamageInfo ( pcUnit );
				}

				if ( pc->szMessage[0] )
				{
					if ( pc->dwMessageEndTime < TICKCOUNT )
					{
						pc->szMessage[0] = 0;
					}
					else if ( ( lstrlenA ( pc->szMessage ) == 2 && pc->szMessage[0] == 'e' ) && isdigit ( pc->szMessage[1] ) )
					{
						pc->szMessage[0] = 0;
					}
					else if ( pc->sCharacterData.iType )
					{
						CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
						iSelected++;
					}
				}

				if ( pc->iPersonalShop && iSelected == 0 )
				{
					CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
					iSelected++;
				}
			}

			if ( iSelected == 0 )
			{
				if ( UNITDATA_FOCUSED == pc || UNITDATA_HOVER == pc ||
					(pc->sCharacterData.iType == CHARACTERTYPE_NPC && pc->sRenderPoint.iZ < (64 * 64 * 256)) ||
					(pc->GetCurrentHealth() > 0 && pc->sCharacterData.iType == CHARACTERTYPE_Monster && pc->sCharacterData.iMonsterType == MONSTERTYPE_Summon) ||
					(pc->sCharacterData.iClanID && pc->sCharacterData.iClanID == UNITDATA->sCharacterData.iClanID) ||
					pc->iBellatraCrown || pc->PartyFlag )
				{
					CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
				}
				else
				{
					if ( pc->sCharacterData.iType == CHARACTERTYPE_Player || pc->PkMode_CharState == CHARACTERTYPE_Player )
					{
						Unit * pcUnit1 = UNITDATATOUNIT( pc );
						if ( pcUnit1 )
						{
							if ( pcUnit1->sBellatraSoloCrown )
							{
								CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
							}
						}
					}
				}
			}
		}
	}


	if ( UNIT && UNIT->vDamageInfoList.size () > 0 )
	{
		DAMAGEINFOHANDLER->RenderUnitDamageInfo ( UNIT );
	}

	if ( UNITDATA->szMessage[0] )
	{
		if ( UNITDATA->dwMessageEndTime < TICKCOUNT )
			UNITDATA->szMessage[0] = 0;
		else if( (lstrlenA(UNITDATA->szMessage) == 2 && UNITDATA->szMessage[0] == 'e') && isdigit(UNITDATA->szMessage[1]) )
			UNITDATA->szMessage[0] = 0;
		else
			CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = UNITDATA;
	}
	else
	{
		if ( UNITDATA->iPersonalShop )
		{
			CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = UNITDATA;
		}
	}

	if ( CHARACTERMESSAGE_COUNT > 0 )
	{
		for ( int i = 0; i < CHARACTERMESSAGE_COUNT; i++ )
		{
			for ( int j = i + 1; j < CHARACTERMESSAGE_COUNT; j++ )
			{
				UnitData * pc1 = CHARACTERMESSAGE[i];
				UnitData * pc2 = CHARACTERMESSAGE[j];

				if ( pc1->sRenderPoint.iZ < pc2->sRenderPoint.iZ ||
					pc1 == UNITDATA_FOCUSED || pc1 == UNITDATA_HOVER )
				{
					pcUnitData = pc1;
					CHARACTERMESSAGE[i] = CHARACTERMESSAGE[j];
					CHARACTERMESSAGE[j] = pc1;
				}
			}
		}
	}

	int iY = 8 + ((*(int*)0x00824FA8) >> 6);


	if ( SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnSelf )
	{
		if ( MAP_ID != MAPID_RicartenTown && MAP_ID != MAPID_PillaiTown && MAP_ID != MAPID_Eura && MAP_ID != MAPID_Atlantis )
		{
			//Render this player's HP
			RenderCharacterHP( UNITDATA, UNITDATA->sRenderPoint.iX - 30, UNITDATA->rRenderRectangle.top + iY - 24 );

		}
	}

	for ( int i = 0; i < CHARACTERMESSAGE_COUNT; i++ )
	{
		DWORD dwColor = RGBA( 255, 255, 255, 200 );
		DWORD dwTitleColor = RGBA(255, 225, 150, 255);
		BOOL bShowBar = FALSE;
		BOOL bShowOwner = FALSE;
		BOOL bSelected = FALSE;

		UnitData * pc = CHARACTERMESSAGE[i];

		/*if (!pc->sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_XY_16))
			continue;*/

		if ( pc->PartyFlag == 1 )				//Party
		{
			dwColor = RGBA( 255, 220, 255, 160 );
			bShowBar = TRUE;
		}
		else if( pc->PartyFlag == 2 )			//Raid
		{
			dwColor = RGBA( 255, 40, 240, 230 );
			bShowBar = TRUE;
		}
		else if ( pc->sCharacterData.iType == CHARACTERTYPE_Monster )
		{
			if ( pc->sCharacterData.iMonsterType == MONSTERTYPE_Summon )
			{
				dwColor = RGBA( 255, 220, 255, 160 );

				if ( pc->sCharacterData.iOwnerID == UNITDATA->iID )
				{
					dwColor = RGBA( 255, 0, 162, 232 );
					bShowOwner = TRUE;
					bShowBar = TRUE;
				}
				else
					bShowOwner = TRUE;
			}
			else
				dwColor = RGBA( 255, 255, 210, 210 );
		}
		else if ( pc->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			dwColor = RGBA( 255, 180, 180, 255 );
		}

		if ( pc == UNITDATA_FOCUSED )
		{
			if ( pc->sCharacterData.iType == CHARACTERTYPE_Monster && pc->sCharacterData.iMonsterType != MONSTERTYPE_Summon )
				dwColor = RGBA( 255, 255, 230, 200 );
			else
				dwColor = RGBA( 255, 255, 255, 255 );

			bSelected = TRUE;
		}
		else
		{
			if ( pc == UNITDATA_HOVER && pc->szMessage[0] )
				bSelected = TRUE;
		}

		EXESurfaceOld * psTextureClanIcon = NULL;
		char * pszClanName = NULL;
		DWORD dwClanBit = 0;

		ClanInfo * psClanInfo = (ClanInfo*)0x03AA4608;

		if ( pc->sCharacterData.iClanID )
		{
			if ( pc == UNITDATA )
			{
				psTextureClanIcon = (EXESurfaceOld*)READDWORD( 0x03B18BC4 );
				pszClanName = (char*)0x03B17984;
			}
			else if ( pc->iClanInfoIndex >= 0 )
			{
				psTextureClanIcon = psClanInfo[pc->iClanInfoIndex].psTexture32;
				pszClanName = psClanInfo[pc->iClanInfoIndex].sHeader.szClanName;
			}
		}

		if ( ((pc->szMessage[0] || pc->iPersonalShop) && (pc->sRenderPoint.iZ < (64 * 32 * 256)) || pc == UNITDATA) )
		{
			if ( pszClanName && pc != UNITDATA_FOCUSED && pc != UNITDATA_HOVER )
			{
				if ( UNITDATA->sCharacterData.iClanID == 0 || pc->sCharacterData.iClanID != UNITDATA->sCharacterData.iClanID )
					pszClanName = NULL;
			}

			if ( pc->szMessage[0] )
			{
				//Draw Box...
				MESSAGEBALLOON->ResetMessage();

				if ( psTextureClanIcon )
					MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

				MESSAGEBALLOON->IsCharacterMessage( TRUE );
				MESSAGEBALLOON->SetSkin( SKINID_Normal );

				if( pc->sCharacterData.iType == CHARACTERTYPE_NPC )
				{
					MESSAGEBALLOON->SetMessage( FormatString( "%s: %s", pc->sCharacterData.szName, pc->szMessage ) );
					MESSAGEBALLOON->GetTextMessage()->SetHighlightTextColor( dwColor );
					MESSAGEBALLOON->SetColor ( -1 );

					MESSAGEBALLOON->SetNotifyQuestIconID(QUESTGAMEHANDLER->GetNpcQuestStatus(pc->sCharacterData.iNPCId));
				}
				else
				{
					Unit* pcUnit = UNITDATATOUNIT(pc);
					MESSAGEBALLOON->AddCharTitleText(pcUnit->szTitle, pcUnit->sTitleRarity);
					MESSAGEBALLOON->SetMessage( pc->szMessage );
					MESSAGEBALLOON->SetColor ( dwColor );
				}

				MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
			else
			{
				if ( pc->szPersonalShop[0] )
				{
					MESSAGEBALLOON->ResetMessage();

					if ( psTextureClanIcon )
						MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

					MESSAGEBALLOON->IsCharacterMessage( TRUE );
					MESSAGEBALLOON->SetSkin( SKINID_Blue );
					MESSAGEBALLOON->SetMessage( pc->szPersonalShop );
					MESSAGEBALLOON->SetColor ( dwColor );
					MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
			}
		}
		else if( pc->sRenderPoint.iZ < ( 64 * 32 * 256 ) )
		{
			if ( pc->bVisible )
			{
				if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
				{
					//Nothing...
				}
				else if ( bShowOwner && pc->sCharacterData.iClanID == 0 )
				{
					STRINGFORMATLEN( ((char*)0x00A17420), 240, "(%s)", pc->sCharacterData.Pet.szOwnerName );

					//Draw Box Two Lines
					MESSAGEBALLOON->ResetMessage();

					if ( psTextureClanIcon )
						MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

					MESSAGEBALLOON->IsCharacterMessage( TRUE );
					MESSAGEBALLOON->SetSkin( SKINID_Normal );
					MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );
					MESSAGEBALLOON->SetPetName( ((char*)0x00A17420) );
					MESSAGEBALLOON->SetColor( dwColor );
					MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
				else
				{
					if (pszClanName)
					{
						if (GM_MODE && ISPRESSEDKEY(VK_CONTROL))
							STRINGFORMATLEN(((char*)0x00A17420), 240, "%d/%d", pc->sCharacterData.iClanID, pc->iClanInfoIndex);
						else
							STRINGFORMATLEN(((char*)0x00A17420), 240, "[%s]", pszClanName);

						//Draw Box Two Lines
						MESSAGEBALLOON->ResetMessage();

						if (psTextureClanIcon)
							MESSAGEBALLOON->SetClanIconTexture(psTextureClanIcon);


						MESSAGEBALLOON->IsCharacterMessage(TRUE);
						MESSAGEBALLOON->SetSkin(SKINID_Normal);
						MESSAGEBALLOON->SetMessage(pc->sCharacterData.szName);
						MESSAGEBALLOON->SetBellatraIconID( pc->iBellatraCrown );
						MESSAGEBALLOON->SetClanName( ((char*)0x00A17420) );
						MESSAGEBALLOON->SetColor( dwColor );

						if ( ( pc->sCharacterData.iType == CHARACTERTYPE_Player || pc->PkMode_CharState == CHARACTERTYPE_Player ) )
						{
							Unit* pcUnit = UNITDATATOUNIT(pc);
							if ( pcUnit && pcUnit->sTitleRarity )
							{
								MESSAGEBALLOON->AddCharTitleText (pcUnit->szTitle, pcUnit->sTitleRarity );
							}

							if (MAP_ID != MAPID_Bellatra && MAP_ID != MAPID_BlessCastle && pcUnit->sBellatraSoloCrown > 0)
							{
								MESSAGEBALLOON->SetBellatraSoloIconID(pcUnit->sBellatraSoloCrown);
								MESSAGEBALLOON->SetCharacterClass(pc->sCharacterData.iClass);
							}
						}


						//Only if user want to see
						if ( MESSAGEBOX_SHOWALWAYS || (pc->sCharacterData.iType == CHARACTERTYPE_NPC) || (pc->sCharacterData.iType == CHARACTERTYPE_Monster) )
							MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
					}
					else
					{
						//Draw Box
						MESSAGEBALLOON->ResetMessage();

						if ( UNITDATA->sPosition.WithinXZDistance( &pc->sPosition, DISTANCE_XY_41_meters ) )
						{
							if ( psTextureClanIcon )
								MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

							if ( pc->sCharacterData.iType == CHARACTERTYPE_NPC ) // Check if is NPC
							{


								MESSAGEBALLOON->IsCharacterMessage( TRUE );
									MESSAGEBALLOON->SetSkin( SKINID_Normal );
									MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );
								MESSAGEBALLOON->SetColor( dwColor );
								MESSAGEBALLOON->SetNotifyQuestIconID( QUESTGAMEHANDLER->GetNpcQuestStatus( pc->sCharacterData.iNPCId ) );

							}
							else
							{
								MESSAGEBALLOON->IsCharacterMessage( TRUE );
								MESSAGEBALLOON->SetSkin( SKINID_Normal );
								MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );
								MESSAGEBALLOON->SetBellatraIconID( pc->iBellatraCrown );
								MESSAGEBALLOON->SetColor( dwColor );

							}

							if ( ( pc->sCharacterData.iType == CHARACTERTYPE_Player || pc->PkMode_CharState == CHARACTERTYPE_Player ) )
							{
								Unit * pcUnit = UNITDATATOUNIT( pc );
								if ( pcUnit )
								{
									if ( pcUnit->sTitleRarity )
									{
										MESSAGEBALLOON->AddCharTitleText( pcUnit->szTitle, pcUnit->sTitleRarity );
									}

									if ( MAP_ID != MAPID_Bellatra && MAP_ID != MAPID_BlessCastle && pcUnit->sBellatraSoloCrown > 0 )
									{
										MESSAGEBALLOON->SetBellatraSoloIconID( pcUnit->sBellatraSoloCrown );
										MESSAGEBALLOON->SetCharacterClass( pc->sCharacterData.iClass );
									}
								}
							}

							//Only if user want to see
							if ( MESSAGEBOX_SHOWALWAYS || ( pc->sCharacterData.iType == CHARACTERTYPE_NPC ) || ( pc->sCharacterData.iType == CHARACTERTYPE_Monster ) )
								MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
						}
					}
				}
			}
		}

		if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
		{
			HUDHANDLER->RenderHPCrystalTower( pc->sRenderPoint.iX - 30, pc->rRenderRectangle.top + iY - 16, pc->GetCurrentHealth(), pc->GetMaxHealth() );
		}
		else if ( bShowBar || (SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnTarget && UNITDATA_HOVER == pc && pc->sCharacterData.iType == CHARACTERTYPE_Monster) )
		{
			//Show Bar
			RenderCharacterHP( pc, pc->sRenderPoint.iX - 30, pc->rRenderRectangle.top + iY - 16 );
		}
	}



	if ( UNITDATA->sActiveSkill.iValue )
	{
		switch ( UNITDATA->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Archer:
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
				{
					if ( UNITDATA->psModelAnimation )
					{
						int iFrame = UNITDATA->iFrame - UNITDATA->psModelAnimation->iBeginFrame * 160;
						if ( (DWORD)UNITDATA->psModelAnimation->iaEventFrame[0] < (DWORD)iFrame && (DWORD)UNITDATA->psModelAnimation->iaEventFrame[1] > (DWORD)iFrame )
						{
							int iEventFrame1 = iFrame - UNITDATA->psModelAnimation->iaEventFrame[0];
							int iEventFrame2 = UNITDATA->psModelAnimation->iaEventFrame[1] - UNITDATA->psModelAnimation->iaEventFrame[0];

							//Draw Charging Bar...
							RenderCharacterChangingBar( UNITDATA, UNITDATA->sRenderPoint.iX - 30, UNITDATA->rRenderRectangle.bottom, iEventFrame1, iEventFrame2 );
						}
					}
				}
				break;

			case CHARACTERCLASS_Pikeman:
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
				{
					if ( UNITDATA->psModelAnimation )
					{
						int iFrame = UNITDATA->iFrame - UNITDATA->psModelAnimation->iBeginFrame * 160;
						if ( (DWORD)UNITDATA->psModelAnimation->iaEventFrame[0] < (DWORD)iFrame && (DWORD)UNITDATA->psModelAnimation->iaEventFrame[1] > (DWORD)iFrame )
						{
							int iEventFrame1 = iFrame - UNITDATA->psModelAnimation->iaEventFrame[0];
							int iEventFrame2 = UNITDATA->psModelAnimation->iaEventFrame[1] - UNITDATA->psModelAnimation->iaEventFrame[0];

							//Draw Charging Bar...
							RenderCharacterChangingBar( UNITDATA, UNITDATA->sRenderPoint.iX - 30, UNITDATA->rRenderRectangle.bottom, iEventFrame1, iEventFrame2 );
						}
					}
				}
				break;
		}
	}

	if ( ISSHOW_INTERFACE )
	{
		if( (pcBossUnitData) && pcBossUnitData->GetCurrentHealth() > 0 && pcBossUnitData->psModelAnimation->iType != ANIMATIONTYPE_Die )
		{
			Unit * pcUnit = UNITDATATOUNIT( pcBossUnitData );
			int iTextWidth = FONTHANDLER->GetWidthText(0, pcBossUnitData->sCharacterData.szName, lstrlenA(pcBossUnitData->sCharacterData.szName) );

			if( pcBossFixedPositionImages[0] )
			{
				UI::ImageRender::Render( pcBossFixedPositionImages[0], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 100, pcBossFixedPositionImages[0]->GetWidth(), pcBossFixedPositionImages[0]->GetHeight(), -1 );
				FONTHANDLER->Draw(0,RECT{(RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 105, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 124}, pcBossUnitData->sCharacterData.szName, DT_EXPANDTABS|DT_SINGLELINE|DT_CENTER, D3DCOLOR_XRGB(0,0,0) );
				FONTHANDLER->Draw(0,RECT{(RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 104, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 124}, pcBossUnitData->sCharacterData.szName, DT_EXPANDTABS|DT_SINGLELINE|DT_CENTER, -1 );
			}

			if( pcBossFixedPositionImages[1] )
				UI::ImageRender::Render( pcBossFixedPositionImages[1], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[1]->GetWidth() >> 1), 108, pcBossFixedPositionImages[1]->GetWidth(), pcBossFixedPositionImages[1]->GetHeight(), -1 );

			if( pcBossFixedPositionImages[2] )
				UI::ImageRender::Render( pcBossFixedPositionImages[2], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[2]->GetWidth() >> 1), 129,(pcBossUnitData->GetCurrentHealth() * pcBossFixedPositionImages[2]->GetWidth()) / pcBossUnitData->GetMaxHealth(), pcBossFixedPositionImages[2]->GetHeight(), 0, 0, -1, 1.0f );

			if ( pcBossFixedPositionImages[0] && pcUnit )
			{
				FONTHANDLER->Draw( 0, RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 130, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 150 }, FormatString( "%d/%d", pcUnit->sHPLong.iMin, pcUnit->sHPLong.iMax ), DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, D3DCOLOR_XRGB( 0, 0, 0 ) );
				FONTHANDLER->Draw( 0, RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 129, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 150 }, FormatString( "%d/%d", pcUnit->sHPLong.iMin, pcUnit->sHPLong.iMax ), DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, D3DCOLOR_XRGB( 255, 170, 170 ) );

				if ( pcUnit->sHPLong.iMax > 0 )
				{
					int iPercentHP = (pcUnit->sHPLong.iMin * 100) / pcUnit->sHPLong.iMax;
					FONTHANDLER->Draw( 0, RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 129, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1) - 40, 150 }, FormatString( "%d%%", iPercentHP ), DT_EXPANDTABS | DT_SINGLELINE | DT_RIGHT, D3DCOLOR_XRGB( 255, 170, 170 ) );
				}
			}

			if( iTextWidth )
			{
				UI::ImageRender::Render( pcBossFixedPositionImages[3], (RESOLUTION_WIDTH >> 1) - (iTextWidth >> 1) - 28, 104, pcBossFixedPositionImages[3]->GetWidth(), pcBossFixedPositionImages[3]->GetHeight(), -1 );

				UI::Image * pImageMonsterType = NULL;
				if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(0);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mutant )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(2);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(4);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Normal )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(3);

				if( pImageMonsterType )
					UI::ImageRender::Render( pImageMonsterType, (RESOLUTION_WIDTH >> 1) - (iTextWidth >> 1) - 26, 105, pImageMonsterType->GetWidth(), pImageMonsterType->GetHeight(), -1 );
			}
		}

		Point2D * psPointFix = (Point2D*)0x00A04790;
		psPointFix->iX = RESOLUTION_WIDTH - 800;
		psPointFix->iY = RESOLUTION_HEIGHT - 600;
		WRITEDWORD( 0x00A0478C, 3 );

		Point2D * p = ((Point2D*)0x00A17518);

		CALL_WITH_ARG2( 0x00409CC0, (DWORD)p, 3 );

		//Party, Friends...
		CALL( 0x0057ED70 );

		WRITEDWORD( 0x00A0478C, 0 );
		psPointFix->iX = 0;
		psPointFix->iY = 0;

		CALL_WITH_ARG2( 0x00409CC0, (DWORD)p, 0 );


		//Draw Unit Data Target
		RenderUnitDataHUD(&Point2D((RESOLUTION_WIDTH >> 1) - 113, 55), 150 * 256, 0);

	}

	UnitData * pp = (*(UnitData**)0x00CF48D0);

	if ( (*(BOOL*)0x00CF48D4) && pp )
	{
		CALL( 0x00454D00 );
	}

	//Display Sod Score
	CALL( 0x00460110 );

	Point2D * psPointFix = (Point2D*)0x00A04790;
	psPointFix->iX = RESOLUTION_WIDTH - 800;
	psPointFix->iY = RESOLUTION_HEIGHT - 600;

	if ( ISSHOW_INTERFACE )
		CALL( 0x004CFBF0 ); //Interface...

	psPointFix->iX = 0;
	psPointFix->iY = 0;

	if ( (*(BOOL*)0x03A97614) )
		CALL( 0x004060E0 ); //Bellatra Font Effect...

	//Render Debug Text
	if ( DEBUG_GM && GM_MODE )
		UNITGAME->RenderDebugText();

	if ( DISCONNECT_TIME )
	{
		// UNITGAME->Log( "%d = Disconnected from Server (%d)", TICKCOUNT, DISCONNECT_TYPE );

		switch ( DISCONNECT_TYPE )
		{
			case 0:
				DrawMessageMiddle( "Disconnected from Server" );
				break;
			case 1:
				DrawMessageMiddle( "Disconnected from Server (1)" );
				break;
			case 2:
				DrawMessageMiddle( "Disconnected from Server (2)" );
				break;
			case 3:
				DrawMessageMiddle( "Disconnected from Server (3)" );
				break;
			case 4:
				DrawMessageMiddle( "Disconnected from Server (4)" );
				break;
		}

		if ( GM_MODE )
		{
			if ( GameCore::QuitGame() )
			{
				if ( (DISCONNECT_TIME + 5000) < TICKCOUNT )
					GameCore::QuitGame( TRUE );
			}
		}
	}
	else
	{
		if ( GameCore::QuitGame() )
			DrawMessageMiddle( "Exiting the Game!" );
		else
		{
			DWORD dwCloseWindow = READDWORD( 0x00CF476C );

			if ( dwCloseWindow && dwCloseWindow > TICKCOUNT )
				DrawMessageMiddle( "Close the open windows before exiting the Game!" );
			else
			{
				if ( (*(DWORD*)0x00CF4768) )
				{
					if ( ((*(DWORD*)0x00CF4768) + 5000) > TICKCOUNT )
						DrawMessageMiddle( "You cannot disconnect during combat!" );
					else
						(*(DWORD*)0x00CF4768) = 0;
				}
			}
		}
	}

	DARKCOLOR_AMOUNT = iOldDarkColor;

	return TRUE;
}

NAKED void SetDynamicLights()
{
	JMP( pfnSetDynamicLights );
}

/// <summary>
/// Leaked: SetDynLight
/// ASM: 0x0040C080
/// Dynamic light setup
/// </summary>
NAKED void SetDynamicLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iPower, int iTime )
{
	JMP( SetDynamicLight2 );
}

int __stdcall RenderStageMesh( EXEStageModel * psStageModel )
{
	struct RenderStageModel
	{
		char			cPadding01[0x4B004];

		Point3D			sCamera;
		Point3D			sAngle;

		int				iSum;
	};

	RenderStageModel * pcThis;
	READTHIS( pcThis );

	Point3D s = psStageModel->sPosition;
	Point3D a = psStageModel->sAngle;

	int iRenderedCount = 0;

	DWORD dwLastTime = psStageModel->dwTimeUpdate;

	int iFrame		= psStageModel->pcModel->uFrame;
	int iMaxFrame	= psStageModel->pcModel->uMaxFrame;

	if ( (GetTickCount() - dwLastTime) >= 14 )
	{
		int iDiff	= (GetTickCount() - dwLastTime) / 14;

		iFrame		+= (iDiff * 70);

		psStageModel->dwTimeUpdate = GetTickCount();
	}

	if ( psStageModel->pcModel->pcBoneContainer )
		iMaxFrame = psStageModel->pcModel->pcBoneContainer->uMaxFrame;

	if ( iFrame < 0 || iFrame > iMaxFrame )
		iFrame = 0;

	psStageModel->pcModel->uFrame = iFrame;
	psStageModel->pcModel->SetPosition( &s, &a );
	psStageModel->pcModel->SetFixedPosition();


	//Fix Camera
	WRITEDWORD( 0x02619824, TRUE );

	RENDERER->SetRenderClip( 0x3C );

	iRenderedCount = psStageModel->pcModel->Render( &pcThis->sCamera, &pcThis->sAngle );

	WRITEDWORD( 0x02619824, FALSE );

	psStageModel->iSum = pcThis->iSum;

	return iRenderedCount;
}

void DrawMessageMiddle( const char * pszText )
{
	CTITLEBOX->RenderTextMiddle( pszText );
}

void DrawCharacterMessage( int iX, int iY, char * pszText, int iLineSize, DWORD dwClan, char * pszClanName, DWORD dwColor, BOOL bSelected, int iBellatra )
{
	BOOL bMessage		= FALSE;
	BOOL bCharacterName = FALSE;
	BOOL bClan			= dwClan != NULL ? TRUE : FALSE;

	int iMessageIndex	= 0;

	char szName[32]			= { 0 };

	char szNameFinder[20]	= { 0 };

	STRINGCOPY( szNameFinder, pszText );

	if ( strstr( szNameFinder, ":" ) != NULL )
	{
		bMessage		= TRUE;
		bCharacterName	= TRUE;

		for ( int i = 0; i < STRLEN( szNameFinder ); i++ )
		{
			if ( szNameFinder[i] == ':' )
			{
				iMessageIndex = i + 1;
				STRINGCOPYLEN( szName, i + 1, szNameFinder );
				break;
			}
		}
	}


	if ( pszText )
	{
		auto DrawMessageBox = []( int iX, int iY, int iWidth, int iHeight, bool bUnk = false, BOOL bSelected = FALSE ) -> int
		{
			return CALLT_WITH_ARG6( 0x0066B260, 0x040C2FC0, iX, iY, iWidth, iHeight, bUnk, bSelected );
		};

		int iWidthName = GETWIDTHTEXT( szName );
		int iWidthText = GETWIDTHTEXT( szName ) + GETWIDTHTEXT( &pszText[iMessageIndex] );
		int iHeight = (iWidthText / 200);

		iX -= ((iWidthText >> 1) + 4);
		iY -= 40;

		if ( iHeight == 0 )
			iHeight++;

		//Compute Space
		std::string str = &pszText[iMessageIndex];

		while ( GETWIDTHTEXT( str.c_str() ) < iWidthName )
			str.insert( str.begin(), ' ' );


		if ( (bMessage == TRUE) && (bCharacterName == TRUE) )
			DrawMessageBox( iX, iY, iWidthText, iHeight, (bCharacterName == TRUE), bSelected );
		else
			DrawMessageBox( iX + 2, iY, str.length(), iHeight, (bCharacterName == TRUE), bSelected );


		RECT r;
		r.left		= iX + 10;
		r.top		= iY + 4;
		r.right		= r.left + 200;
		r.bottom	= r.top + 32;

		if ( bCharacterName == TRUE )
			FONTHANDLER->Draw( 0, r, szName, 0, dwColor );

		FONTHANDLER->Draw( 0, r, str, DT_WORDBREAK, dwColor );
	}
}

NAKED BOOL LoadUnitData( UnitData * pcUnitData, char * pszBodyModel, char * pszHeadModel, Point3D * psPosition, Point3D * psAngle )
{
	JMP( 0x00438DD0 );
}

int DrawGameInterface3D()
{
	return 1;
}

NAKED int SetItemTextureColor( UnitTool * psTool )
{
	JMP( 0x00420940 );
}

NAKED void ResetItemTextureColor()
{
	JMP( 0x00420A80 );
}

NAKED void SetShieldStrikeEffect( Point3D * psPosition )
{
	JMP( 0x00531780 );
}

BOOL __cdecl RenderUnitData( UnitData * pcUnitData )
{


	if ( (pcUnitData->bVisible == FALSE) || (pcUnitData->psModelAnimation == NULL) )
		return FALSE;

	if ( pcUnitData->pcBody && pcUnitData->pcBody->iCount <= 0 )
	{
		pcUnitData->Free();

		if ( DEBUG_GM )
			ChatGame::AddChatBoxText( "Model Error" );

		return FALSE;
	}

	if ( pcUnitData->eActionPattern == ACTIONMODE_ClientUnit )
	{
		if ( pcUnitData->iLastPlayDataType )
		{
			if ( (pcUnitData->dwLastActiveTime + 16000) < TICKCOUNT )
			{
				pcUnitData->dwDispAppSkill = 0;
				return TRUE;
			}
		}
		else
		{
			if ( (pcUnitData->dwLastActiveTime + 16000) < TICKCOUNT )
			{
				pcUnitData->dwDispAppSkill = 0;
				return TRUE;
			}
		}
	}




	pcUnitData->bRendered = 0;
	pcUnitData->bFlagShow = TRUE;

	//JLM - Experimental Rendering Code
	//if (!pcUnitData->sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_XY_32))
	//	return TRUE;

	Point3D sDistance;
	sDistance.iX = UNITTARGETPOSITION_X - pcUnitData->sPosition.iX;
	sDistance.iY = UNITTARGETPOSITION_Y - pcUnitData->sPosition.iY;
	sDistance.iZ = UNITTARGETPOSITION_Z - pcUnitData->sPosition.iZ;

	sDistance >>= 8;

	int iDistance	= sDistance.iX * sDistance.iX;
	iDistance	   += sDistance.iY * sDistance.iY;
	iDistance	   += sDistance.iZ * sDistance.iZ;

	Point3D sCameraPosition;
	sCameraPosition.iX = UNITTARGETPOSITION_X;
	sCameraPosition.iY = UNITTARGETPOSITION_Y;
	sCameraPosition.iZ = UNITTARGETPOSITION_Z;

	Point3D sCameraAngle;
	sCameraAngle.iX = UNITTARGETANGLE_X;
	sCameraAngle.iY = UNITTARGETANGLE_Y;
	sCameraAngle.iZ = UNITTARGETANGLE_Z;

	Point3D sAngle = pcUnitData->sAngle;
	sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

	if ( ACTIONMODE )
	{
		if ( sAngle.iY > PTANGLE_180 )
			sAngle.iY = PTANGLE_270;
		else
			sAngle.iY = PTANGLE_90;
	}

	pcUnitData->sPosi = pcUnitData->sPosition;

	EXEModel * pcBodyMeshContainer = pcUnitData->pcBodyMeshContainer;
	EXEModel * pcHeadMeshContainer = pcUnitData->pcHeadMeshContainer;

	pcBodyMeshContainer->SetPosition( &pcUnitData->sPosi, &sAngle );

	if ( pcBodyMeshContainer->pcBoneContainer && ((int)pcBodyMeshContainer->pcBoneContainer->uMaxFrame > pcUnitData->iFrame) )
		pcBodyMeshContainer->uFrame = (UINT)pcUnitData->iFrame;
	else
		pcBodyMeshContainer->uFrame = 80;

	char * pszaModelName[4];

	int iRed, iGreen, iBlue, iAlpha, iERed, iEGreen, iEBlue, iEAlpha;

	iRed	= RENDERER->r;
	iGreen	= RENDERER->g;
	iBlue	= RENDERER->b;
	iAlpha	= RENDERER->a;

	iERed	= pcUnitData->sAddColorEffect[2];
	iEGreen	= pcUnitData->sAddColorEffect[1];
	iEBlue	= pcUnitData->sAddColorEffect[0];
	iEAlpha	= pcUnitData->sAddColorEffect[3];

	if ( pcUnitData->sCharacterData.sMonsterClass == EMonsterClass::Boss )
	{
		int iCount = pcUnitData->iLoopCounter & 0x1F;

		if ( (pcUnitData->iLoopCounter & 0x20) == 0 )
			iCount = 0x1F - iCount;

		iERed	= (iERed * iCount) >> 5;
		iEGreen = (iEGreen * iCount) >> 5;
		iEBlue	= (iEBlue * iCount) >> 5;
	}



	RENDERER->r = iRed + abs(pcUnitData->iFireOverlay) + iERed;
	RENDERER->g = iGreen - (abs( pcUnitData->iFireOverlay) >> 1 ) + iEGreen + abs( pcUnitData->iPoisonOverlay );
	RENDERER->b = iBlue + abs( pcUnitData->iIceOverlay ) + iEBlue;
	RENDERER->a = iAlpha + pcUnitData->DisplayAlpha - ( abs( pcUnitData->iIceOverlay ) >> 1 ) + iEAlpha;

	if ( pcUnitData->bBitterOrdeal )
	{
		RENDERER->r += 100;
		RENDERER->b += 100;
	}

	if ( pcUnitData->bDistortion && (pcUnitData->IsDead() == FALSE) )
	{
		RENDERER->iDistortionMesh = 1;

		RENDERER->r += 80;
		RENDERER->g += 80;
	}

	if ( pcUnitData->iCurseLazy && (pcUnitData->IsDead() == FALSE) )
	{
		RENDERER->iDistortionMesh = 0;

		RENDERER->b += 40;

		EffectGame::InertiaTarget( "CurseLazy2", pcUnitData );
	}

	if ( pcUnitData->sCharacterData.sSize )
		RENDERER->iObjectVertexScale = pcUnitData->sCharacterData.sSize;

	int iDark = 0;

	if ( DARKCOLOR_AMOUNT > 0 && pcUnitData->iLoadedMapIndex >= 0 )
	{
		if ( FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
		{
			iDark = DARKCOLOR_AMOUNT;
			RENDERER->r += iDark;
			RENDERER->g += iDark;
			RENDERER->b += iDark;

			iDark = 0;

			if ( RENDERER->iLightCount > 0 && RENDERER->sObjectLight[0].iRange == DARKLIGHTCOLOR_RANGE )
			{
				RENDERER->sObjectLight[0].iRange = 0;
				iDark = 1;
			}
		}
	}

	if ( pcUnitData->IsDead() )
	{
		if ( pcUnitData->iFrameCounter > 400 )
		{
			RENDERER->r -= 100;
			RENDERER->g -= 100;
			RENDERER->b -= 100;
		}
		else
		{
			if ( pcUnitData->iFrameCounter > 200 )
			{
				int iColor = (pcUnitData->iFrameCounter - 200) >> 1;
				RENDERER->r -= iColor;
				RENDERER->g -= iColor;
				RENDERER->b -= iColor;
			}
		}

		int iMonsterEffectID = pcUnitData->sCharacterData.iMonsterEffectID;

		if ( iMonsterEffectID == MONSTEREFFECTID_FIGON || iMonsterEffectID == MONSTEREFFECTID_STYGIANLORD ||
			iMonsterEffectID == MONSTEREFFECTID_GUARDIAN_SAINT || iMonsterEffectID == MONSTEREFFECTID_S_F_ELEMENTAL ||
			iMonsterEffectID == MONSTEREFFECTID_CHAOSCARA )
		{
			RENDERER->a = iAlpha - (pcUnitData->iFrameCounter << 1);

			if ( RENDERER->a < (-400) )
			{
				RENDERER->r = iRed;
				RENDERER->g = iGreen;
				RENDERER->b = iBlue;
				RENDERER->a = iAlpha;

				if ( iDark )
					RENDERER->sObjectLight[0].iRange = DARKLIGHTCOLOR_RANGE;

				RENDERER->iDistortionMesh = 0;

				RENDERER->iObjectVertexScale = 0;

				return TRUE;
			}

			pcUnitData->RendAlpha = 100;
		}
	}

	if ( pcUnitData->psBody )
	{
		ModelData::MeshQuality * psMeshQuality = &pcUnitData->psBody->sMediumMeshQuality;

		if ( pcUnitData->iTalkFrame )
		{
			if ( (int)pcUnitData->pcTalkBoneContainer->uMaxFrame > pcUnitData->iTalkFrame )
				pcUnitData->pcTalkBoneContainer->uMaxFrame = (UINT)pcUnitData->iTalkFrame;
			else
				pcUnitData->iTalkFrame = 0;
		}

		if ( pcUnitData->iPatDispMode & 1 )
		{

			if ( pcBodyMeshContainer->pcBoneContainer )
				pcBodyMeshContainer->pcBoneContainer->iLastFrame = -1;
			else
				pcBodyMeshContainer->iLastFrame = -1;


			if ( ((psMeshQuality != NULL) && ((DWORD)psMeshQuality != 0x104)) && psMeshQuality->iCount )
			{
				pszaModelName[0] = psMeshQuality->szMeshName1;
				pszaModelName[1] = psMeshQuality->szMeshName2;
				pszaModelName[2] = psMeshQuality->szMeshName3;
				pszaModelName[3] = psMeshQuality->szMeshName4;

				if ( pcUnitData->iTalkFrame && pcUnitData->psTalkModelAnimation->iType )
					pcUnitData->bRendered += pcBodyMeshContainer->PreRender( &sCameraPosition, &sCameraAngle, pszaModelName, psMeshQuality->iCount, pcUnitData->pcTalkBoneContainer, (EXEFrame*)&pcUnitData->iStartFrame );
				else
					pcUnitData->bRendered += pcBodyMeshContainer->PreRender( &sCameraPosition, &sCameraAngle, pszaModelName, psMeshQuality->iCount, (EXEFrame*)&pcUnitData->iStartFrame );
			}
			else
			{
				if ( pcUnitData->iTalkFrame && pcUnitData->psTalkModelAnimation->iType )
					pcUnitData->bRendered += pcBodyMeshContainer->Render( &sCameraPosition, &sCameraAngle, pcUnitData->pcTalkBoneContainer, (EXEFrame*)&pcUnitData->iStartFrame );
				else
					pcUnitData->bRendered += pcBodyMeshContainer->Render( &sCameraPosition, &sCameraAngle, (EXEFrame*)&pcUnitData->iStartFrame );
			}
		}
	}

	if ( pcUnitData->pcHead && pcHeadMeshContainer && (pcUnitData->iPatDispMode & 2) )
	{

		ModelData::MeshQuality * psMeshQuality = &pcUnitData->pcHead->psModelData->sMediumMeshQuality;

		pcHeadMeshContainer->SetPosition( &pcUnitData->sPosi, &sAngle );

		if ( pcHeadMeshContainer->pcBoneContainer && (int)pcHeadMeshContainer->pcBoneContainer->uMaxFrame > pcUnitData->iFrame )
			pcHeadMeshContainer->uFrame = (UINT)pcUnitData->iFrame;

/*
		if ( pcHeadMeshContainer->pcBoneContainer )
			pcHeadMeshContainer->pcBoneContainer->iLastFrame = -1;
		else
			pcHeadMeshContainer->iLastFrame = -1;
*/

		if ( ((psMeshQuality != NULL) && ((DWORD)psMeshQuality != 0x104)) && psMeshQuality->iCount )
		{
			pszaModelName[0] = psMeshQuality->szMeshName1;
			pszaModelName[1] = psMeshQuality->szMeshName2;
			pszaModelName[2] = psMeshQuality->szMeshName3;
			pszaModelName[3] = psMeshQuality->szMeshName4;

			if ( pcUnitData->iTalkFrame && pcUnitData->psTalkModelAnimation->iType )
				pcUnitData->bRendered += pcHeadMeshContainer->PreRender( &sCameraPosition, &sCameraAngle, pszaModelName, psMeshQuality->iCount, pcUnitData->pcTalkBoneContainer, (EXEFrame*)&pcUnitData->iStartFrame );
			else
				pcUnitData->bRendered += pcHeadMeshContainer->PreRender( &sCameraPosition, &sCameraAngle, pszaModelName, psMeshQuality->iCount, (EXEFrame*)&pcUnitData->iStartFrame );
		}
		else
		{
			if ( pcUnitData->iTalkFrame && pcUnitData->psTalkModelAnimation->iType )
				pcUnitData->bRendered += pcHeadMeshContainer->Render( &sCameraPosition, &sCameraAngle, pcUnitData->pcTalkBoneContainer, (EXEFrame*)&pcUnitData->iStartFrame );
			else
				pcUnitData->bRendered += pcHeadMeshContainer->Render( &sCameraPosition, &sCameraAngle, (EXEFrame*)&pcUnitData->iStartFrame );
		}
	}

	if ( pcUnitData->bRendered )
	{
		int iWidthModel = 44 << 8;
		int iHeightModel = pcUnitData->iModelHeight;

		if ( (pcUnitData->sCharacterData.sSize > 0) && (pcUnitData->sCharacterData.sSize != 256) )
		{
			iWidthModel = (iWidthModel * pcUnitData->sCharacterData.sSize) >> 8;
			iHeightModel = (iHeightModel * pcUnitData->sCharacterData.sSize) >> 8;
		}

		if ( iHeightModel < (44 << 8) )
			iHeightModel = (44 << 8);

		RENDERER->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, sCameraAngle.iX, sCameraAngle.iY, sCameraAngle.iZ );

		Point2D sPoint( 0, 0 );

		pcUnitData->sRenderPoint.iZ = RENDERER->GetRectangle2D( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + (iHeightModel >> 1), pcUnitData->sPosition.iZ, iWidthModel, iHeightModel, &pcUnitData->rRenderRectangle, &sPoint );
		pcUnitData->sRenderPoint.iX = sPoint.iX;
		pcUnitData->sRenderPoint.iY = sPoint.iY;

		if ( bUIDebug )
		{
			int iX = pcUnitData->rRenderRectangle.left;
			int iY = pcUnitData->rRenderRectangle.top;
			int iWidth = pcUnitData->rRenderRectangle.right - iX;
			int iHeight = pcUnitData->rRenderRectangle.bottom - iY;

			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY, iWidth, 1 ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY + iHeight - 1, iWidth, 1 ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY, 1, iHeight ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + iWidth - 1, iY, 1, iHeight ), D3DCOLOR_XRGB( 255, 255, 100 ) );
		}
	}

	if ( !pcBodyMeshContainer->pcBoneContainer || pcUnitData->bVisibleEquipment == FALSE )
	{
		RENDERER->r = iRed;
		RENDERER->g = iGreen;
		RENDERER->b = iBlue;

		if ( iDark )
			RENDERER->sLight[0].iRange = DARKLIGHTCOLOR_RANGE;

		return TRUE;
	}

	BOOL bColorIncrease = FALSE;

	if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack && TICKCOUNT & 0x20 )
		bColorIncrease = TRUE;

	if ( bColorIncrease && (pcUnitData->bProjectileMode == FALSE) )
	{
		RENDERER->r += 80;
		RENDERER->g += 80;
		RENDERER->b += 80;
	}

	pcUnitData->bRenderLeftHand = TRUE;
	pcUnitData->bRenderRightHand = TRUE;

	EXEModel * pcModelShield = NULL;

	if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->PkMode_CharState == CHARACTERTYPE_Player )
	{
		//Wing
		if ( pcUnitData->pcaBackBone[0] && pcUnitData->pcaBackBone[0]->pParentMesh )
			CopyMemory( &pcUnitData->sWingMatrix, &pcUnitData->pcaBackBone[0]->pParentMesh->sWorld, sizeof( EXEMatrixI ) );

		if ( pcUnitData->pcaBackBone[2] )
			CopyMemory( &pcUnitData->sWingSkillMatrix, &pcUnitData->pcaBackBone[2]->sWorld, sizeof( EXEMatrixI ) );

		if ( pcUnitData->eToolEquipModeTown == TOOLEQUIPMODETOWN_None || (pcUnitData->iLoadedMapIndex >= 0 && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType != MAPTYPE_Town && pcUnitData->GetAnimation() != ANIMATIONTYPE_Yahoo) )
		{
			if ( pcUnitData->sLeftHandTool.pModel )
			{
				pcModelShield = pcUnitData->sLeftHandTool.pModel;

				if ( ItemID(pcUnitData->sLeftHandTool.eItemID).ToItemType() == ITEMTYPE_Shield )
				{
					if ( (pcUnitData->dwDispAppSkill & SKILLMASK_SparkShield) && SPARKSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Mechanician )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T34 )
						{

						}
						else
							pcModelShield = SPARKSHIELD_MODEL;
					}
					if ( (pcUnitData->dwDispAppSkill & SKILLMASK_SparkShield) && DIVINESHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T32 )
						{

						}
						else
							pcModelShield = DIVINESHIELD_MODEL;
					}
					if ( (pcUnitData->dwDispAppSkill & SKILLMASK_GodlyShield) && GODLYSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
						{

						}
						else
							pcModelShield = GODLYSHIELD_MODEL;
					}
				}

				if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Atalanta && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T11 || pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T23) && pcUnitData->pcShieldBone )
				{
					pcModelShield->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->pcShieldBone );

					RENDERER->r += 100;
					RENDERER->g += 100;
					RENDERER->b += 80;
				}
				else
				{
					if ( pcUnitData->sLeftHandTool.pBone )
					{
						//Dagger? ( PVP and PVE )
						if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Assassin )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Dagger )
							{
								EXEMatrixI sMatrix;
								EXEMatrixIdentityI( sMatrix );
								//EXEMatrixRotateZI( sMatrix, PTANGLE_180 & PTANGLE_Mask );

								pcUnitData->sLeftHandTool.pBone->sWorld = EXEMatrixMultiplyI( sMatrix, pcUnitData->sLeftHandTool.pBone->sWorld );
							}
						}

						pcModelShield->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sLeftHandTool.pBone );

					}
				}

				pcModelShield->SetPosition( &pcUnitData->sPosi, &sAngle );
				pcModelShield->uFrame = (UINT)pcUnitData->iFrame;

				if ( pcUnitData->sLeftHandTool.pBone )
					CopyMemory( &pcUnitData->sLeftHandMatrix, &pcUnitData->sLeftHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );

				if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
				{
					int iColorSpeed = SetItemTextureColor( &pcUnitData->sLeftHandTool );
					int iColorScale = (iColorSpeed * pcUnitData->sLeftHandTool.iBlinkScale) >> 8;

					if ( pcUnitData->sLeftHandTool.iBlinkScale && iColorSpeed )
					{
						EXEMatrixI sMatrix;
						EXEMatrixIdentityI( sMatrix );

						if ( pcUnitData->sLeftHandTool.iBlinkScale < 0 )
							iColorScale = -iColorScale;

						sMatrix.i[0][0] = iColorScale;
						sMatrix.i[1][1] = iColorScale;
						sMatrix.i[2][2] = iColorScale;

						pcModelShield->uFrame = -1;

						if ( pcUnitData->sLeftHandTool.pBone && pcModelShield->paMesh[0] )
						{
							CopyMemory( &pcModelShield->paMesh[0]->sWorld, &pcUnitData->sLeftHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );
							pcModelShield->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelShield->paMesh[0]->sWorld );
						}
					}

					pcModelShield->Render( &sCameraPosition, &sCameraAngle );
					ResetItemTextureColor();


					if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Atalanta && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T11 || pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T23) && pcUnitData->pcShieldBone )
					{
						if ( pcModelShield->paMesh[0] )
						{
							Point3D sShieldPosition;
							sShieldPosition.iX = pcModelShield->paMesh[0]->sWorld.i[3][0] + pcUnitData->sPosition.iX;
							sShieldPosition.iZ = pcModelShield->paMesh[0]->sWorld.i[3][1] + pcUnitData->sPosition.iZ;
							sShieldPosition.iY = pcModelShield->paMesh[0]->sWorld.i[3][2] + pcUnitData->sPosition.iY;
							SetShieldStrikeEffect( &sShieldPosition );
						}
					}
				}
				else
					pcUnitData->bRenderLeftHand = FALSE;

				pcModelShield->SetParentBone( NULL, NULL );
				pcModelShield->iLastFrame = -1;
			}

			if ( pcUnitData->sRightHandTool.pModel && !pcUnitData->bHideRightWeapon )
			{
				if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Javelin && pcUnitData->iProjectileType )
				{

				}
				else
				{
					EXEModel * pcModelWeapon = pcUnitData->sRightHandTool.pModel;
					EXEMesh * pcBoneWeapon = pcUnitData->sRightHandTool.pBone;

					if ( pcBoneWeapon )
					{
						//Shaman & Phantom? ( PVP & PVE )
						if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Phantom )
							{
								if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
								{
									pcBoneWeapon = pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1];

									EXEMatrixI sMatrix;
									EXEMatrixIdentityI( sMatrix );
									EXEMatrixRotateYI( sMatrix, 2250 );
									EXEMatrixTranslate( sMatrix, 0, 0, 10 );

									pcUnitData->sPosi.iY += 1000;

									pcBoneWeapon->sWorld = EXEMatrixMultiplyI( sMatrix, pcBoneWeapon->sWorld );
								}
							}
						}

						pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBoneWeapon );
					}

					pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
					pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

					if ( pcBoneWeapon )
						CopyMemory( &pcUnitData->sRightHandMatrix, &pcBoneWeapon->sWorld, sizeof( EXEMatrixI ) );

					if ( !pcUnitData->sRightHandTool.iBlinkSpeed || !pcUnitData->sRightHandTool.saBlinkColor[3] )
					{
						int iColorSpeed = SetItemTextureColor( &pcUnitData->sRightHandTool );
						int iColorScale = (iColorSpeed * pcUnitData->sRightHandTool.iBlinkScale) >> 8;

						if ( pcUnitData->sRightHandTool.iBlinkScale && iColorSpeed )
						{
							EXEMatrixI sMatrix;
							EXEMatrixIdentityI( sMatrix );

							if ( pcUnitData->sRightHandTool.iBlinkScale < 0 )
								iColorScale = -iColorScale;

							sMatrix.i[0][0] = iColorScale;
							sMatrix.i[1][1] = iColorScale;
							sMatrix.i[2][2] = iColorScale;

							pcModelWeapon->uFrame = -1;

							if ( pcUnitData->sRightHandTool.pBone && pcModelWeapon->paMesh[0] )
							{
								CopyMemory( &pcModelWeapon->paMesh[0]->sWorld, &pcUnitData->sRightHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );
								pcModelWeapon->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->paMesh[0]->sWorld );
							}
						}

						pcModelWeapon->Render( &sCameraPosition, &sCameraAngle );
						ResetItemTextureColor();
					}
					else
						pcUnitData->bRenderRightHand = FALSE;

					pcModelWeapon->SetParentBone( NULL, NULL );
					pcModelWeapon->iLastFrame = -1;
				}
			}
		}
		else
		{
			EXEModel * pcModelWeapon = NULL;

			if ( pcUnitData->eWeaponItemID )
			{
				int iColorScale = 0;

				if ( pcUnitData->sLeftHandTool.eItemID == pcUnitData->eWeaponItemID )
				{
					pcModelWeapon = pcUnitData->sLeftHandTool.pModel;

					if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
					{
						if ( pcModelWeapon )
						{
							int iColorSpeed = SetItemTextureColor( &pcUnitData->sLeftHandTool );

							if ( pcUnitData->sLeftHandTool.iBlinkScale && iColorSpeed )
							{
								iColorScale = (iColorSpeed * pcUnitData->sLeftHandTool.iBlinkScale) >> 8;

								if ( pcUnitData->sLeftHandTool.iBlinkScale < 0 )
									iColorScale = -iColorScale;

								if ( pcUnitData->sLeftHandTool.pBone && pcModelWeapon->paMesh[0] )
									CopyMemory( &pcModelWeapon->paMesh[0]->sWorld, &pcUnitData->sLeftHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );
							}
						}
					}
					else
						pcUnitData->bRenderLeftHand = FALSE;
				}

				if ( pcUnitData->sRightHandTool.eItemID == pcUnitData->eWeaponItemID )
				{
					pcModelWeapon = pcUnitData->sRightHandTool.pModel;

					if ( !pcUnitData->sRightHandTool.iBlinkSpeed || !pcUnitData->sRightHandTool.saBlinkColor[3] )
					{
						if ( pcModelWeapon )
						{
							int iColorSpeed = SetItemTextureColor( &pcUnitData->sRightHandTool );

							if ( pcUnitData->sRightHandTool.iBlinkScale && iColorSpeed )
							{
								iColorScale = (iColorSpeed * pcUnitData->sRightHandTool.iBlinkScale) >> 8;

								if ( pcUnitData->sRightHandTool.iBlinkScale < 0 )
									iColorScale = -iColorScale;

								if ( pcUnitData->sRightHandTool.pBone && pcModelWeapon->paMesh[0] )
									CopyMemory( &pcModelWeapon->paMesh[0]->sWorld, &pcUnitData->sRightHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );
							}
						}
					}
					else
						pcUnitData->bRenderRightHand = FALSE;
				}

				if ( pcModelWeapon )
				{
					if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
					{
						//Shaman & Phantom? ( Town )
						if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Phantom )
							{
								EXEMatrixI sMatrix;
								EXEMatrixIdentityI( sMatrix );
								EXEMatrixRotateYI( sMatrix, 2250 );
								EXEMatrixTranslate( sMatrix, 0, 0, 10 );

								pcUnitData->sPosi.iY += 1000;

								pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->sWorld = EXEMatrixMultiplyI( sMatrix, pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->sWorld );
							}
						}

						pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] );
					}

					pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
					pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

					if ( iColorScale )
					{
						EXEMatrixI sMatrix;

						EXEMatrixIdentityI( sMatrix );
						sMatrix.i[0][0] = iColorScale;
						sMatrix.i[1][1] = iColorScale;
						sMatrix.i[2][2] = iColorScale;
						pcModelWeapon->uFrame = -1;

						if ( pcModelWeapon->paMesh[0] )
							pcModelWeapon->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->paMesh[0]->sWorld );
					}

					if ( pcUnitData->sLeftHandTool.eItemID == pcUnitData->eWeaponItemID )
					{
						if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
							CopyMemory( &pcUnitData->sLeftHandMatrix, &pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->sWorld, sizeof( EXEMatrixI ) );

						if ( pcUnitData->bRenderLeftHand )
						{
							//Assassin & Dagger? ( Town )
							if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Assassin )
							{
								if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Dagger )
								{
									for ( int i = 0; i < 2; i++ )
									{
										EXEMesh * pcBone = pcUnitData->pcaWeaponDaggerBone[i];
										if ( pcBone )
										{
											EXEMatrixI sMatrix;
											EXEMatrixIdentityI( sMatrix );

											if ( pcUnitData->eWeaponItemID == pcUnitData->sLeftHandTool.eItemID )
												EXEMatrixRotateZI( sMatrix, PTANGLE_180 & PTANGLE_Mask );

											pcBone->sWorld = EXEMatrixMultiplyI( sMatrix, pcBone->sWorld );

											pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBone );

											pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
											pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

											if ( pcModelWeapon->paMesh[0] )
												pcModelWeapon->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->paMesh[0]->sWorld );

											pcModelWeapon->uFrame++;

											BOOL bRenderWeapon = FALSE;

											if ( pcUnitData->eWeaponItemID == pcUnitData->sLeftHandTool.eItemID )
											{
												CopyMemory( &pcUnitData->sLeftHandMatrix, &pcBone->sWorld, sizeof( EXEMatrixI ) );
												bRenderWeapon = pcUnitData->bRenderLeftHand;
											}
											else
											{
												CopyMemory( &pcUnitData->sRightHandMatrix, &pcBone->sWorld, sizeof( EXEMatrixI ) );
												bRenderWeapon = pcUnitData->bRenderRightHand;
											}

											if ( bRenderWeapon )
											{
												ResetItemTextureColor();
												pcModelWeapon->Render( &sCameraPosition, &sCameraAngle );
											}

											pcModelWeapon->SetParentBone( NULL, NULL );
											pcModelWeapon->iLastFrame = -1;
										}
									}
								}
							}
							else
							{
								pcModelWeapon->Render( &sCameraPosition, &sCameraAngle );

								pcModelWeapon->SetParentBone( NULL, NULL );
								pcModelWeapon->iLastFrame = -1;
							}

							ResetItemTextureColor();
						}
					}
					else
					{
						if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
							CopyMemory( &pcUnitData->sRightHandMatrix, &pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->sWorld, sizeof( EXEMatrixI ) );

						if ( pcUnitData->bRenderRightHand )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Claw )
							{
								EXEMesh * pcBone = pcUnitData->pcBoneContainer->GetMesh( "Bip01 R Hand" );
								if ( pcBone )
								{
									EXEMatrixI sMatrix;
									EXEMatrixIdentityI( sMatrix );

									pcBone->sWorld = EXEMatrixMultiplyI( sMatrix, pcBone->sWorld );

									pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBone );

									pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
									pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

									if ( pcModelWeapon->paMesh[0] )
										pcModelWeapon->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->paMesh[0]->sWorld );

									pcModelWeapon->uFrame++;

									CopyMemory( &pcUnitData->sRightHandMatrix, &pcBone->sWorld, sizeof( EXEMatrixI ) );
								}
							}

							pcModelWeapon->Render( &sCameraPosition, &sCameraAngle );
							ResetItemTextureColor();
						}
					}

					pcModelWeapon->SetParentBone( NULL, NULL );
					pcModelWeapon->iLastFrame = -1;
				}
			}

			if ( pcUnitData->sLeftHandTool.pModel && pcUnitData->eWeaponItemID != pcUnitData->sLeftHandTool.eItemID )
			{
				pcModelWeapon = pcUnitData->sLeftHandTool.pModel;

				if ( pcUnitData->sLeftHandTool.pBone )
					pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sLeftHandTool.pBone );

				pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
				pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

				if ( pcUnitData->sLeftHandTool.pBone )
					CopyMemory( &pcUnitData->sLeftHandMatrix, &pcUnitData->sLeftHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );

				if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
				{
					int iColorSpeed = SetItemTextureColor( &pcUnitData->sLeftHandTool );

					if ( pcUnitData->sLeftHandTool.iBlinkScale && iColorSpeed )
					{
						int iColorScale = (iColorSpeed * pcUnitData->sLeftHandTool.iBlinkScale) >> 8;

						if ( pcUnitData->sLeftHandTool.iBlinkScale < 0 )
							iColorScale = -iColorScale;

						EXEMatrixI sMatrix;
						EXEMatrixIdentityI( sMatrix );

						sMatrix.i[0][0] = iColorScale;
						sMatrix.i[1][1] = iColorScale;
						sMatrix.i[2][2] = iColorScale;

						pcModelWeapon->uFrame = -1;

						if ( pcUnitData->sLeftHandTool.pBone && pcModelWeapon->paMesh[0] )
						{
							CopyMemory( &pcModelWeapon->paMesh[0]->sWorld, &pcUnitData->sLeftHandTool.pBone->sWorld, sizeof( EXEMatrixI ) );
							pcModelWeapon->paMesh[0]->sWorld = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->paMesh[0]->sWorld );
						}
					}

					pcModelWeapon->Render( &sCameraPosition, &sCameraAngle );
					ResetItemTextureColor();
				}
				else
					pcUnitData->bRenderLeftHand = FALSE;

				pcModelWeapon->SetParentBone( NULL, NULL );
				pcModelWeapon->iLastFrame = -1;
			}
		}

		EXEModel * pcModelProjectile = NULL;

		if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack || pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill) &&
			ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Bow &&
			pcUnitData->sRightHandTool.pModel == NULL && pcUnitData->iProjectileType == FALSE && pcUnitData->bHideRightWeapon == FALSE )
		{
			pcModelProjectile = PROJECTILE_MODEL;
			if ( pcModelProjectile )
			{
				if ( pcUnitData->sRightHandTool.pBone )
					pcModelProjectile->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sRightHandTool.pBone );

				pcModelProjectile->SetPosition( &pcUnitData->sPosi, &sAngle );
				pcModelProjectile->uFrame = (UINT)pcUnitData->iFrame;
				pcModelProjectile->Render( &sCameraPosition, &sCameraAngle );
				pcModelProjectile->SetParentBone( NULL, NULL );
				pcModelProjectile->iLastFrame = -1;
			}
		}
	}

	if ( pcUnitData->iProjectileType )
	{
		EXEModel * pcModelProjectile = NULL;

		CopyMemory( &sAngle, &pcUnitData->sProjectileAngle, sizeof( Point3D ) );
		sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

		if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Bow )
			pcModelProjectile = PROJECTILE_MODEL;
		else
		{
			pcModelProjectile = pcUnitData->sRightHandTool.pModel;

			sAngle.iX = (sAngle.iX - PTANGLE_90) & PTANGLE_Mask;
		}

		if ( pcModelProjectile )
		{
			pcModelProjectile->SetPosition( &pcUnitData->sProjectilePosition, &sAngle );
			pcModelProjectile->uFrame = 0;
			pcModelProjectile->Render( &sCameraPosition, &sCameraAngle );
		}
	}

	RENDERER->r = iRed;
	RENDERER->g = iGreen;
	RENDERER->b = iBlue;
	RENDERER->a = iAlpha;

	if ( iDark )
		RENDERER->sLight[0].iRange = DARKLIGHTCOLOR_RANGE;

	RENDERER->iDistortionMesh = 0;

	RENDERER->iObjectVertexScale = 0;

	return TRUE;
}

// character.cpp line ~ 6451
// int smCHAR::Main()
//	ActionPattern = case 99:
// Notes
// For attacking events
// It can occur on another player, not just self
// So take that into consideration.
BOOL ClientUnitDataUpdate( UnitData * pcUnitData )
{
	//bIsMonster is a NPC monster or other player
	BOOL bIsAMonster = pcUnitData->PkMode_CharState == CHARACTERTYPE_Monster || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster;

	//bIsPlayer is other player
	BOOL bIsAPlayer = pcUnitData->PkMode_CharState == CHARACTERTYPE_Player || pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player;

	//bIsNPC is a NPC (non-monster)
	BOOL bIsANPC = pcUnitData->PkMode_CharState == CHARACTERTYPE_NPC || pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC;

	if (bIsAPlayer && pcUnitData->bAddFrameUnit )
	{
		if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) )
		{
			if ( pcUnitData->iPlayBuffCnt >= pcUnitData->iPlayBuffPosiEnd )
			{
				int iCompareFrame = pcUnitData->iFrame / 160;
				if ( pcUnitData->psModelAnimation && (iCompareFrame >= pcUnitData->psModelAnimation->iEndFrame) )
				{
					pcUnitData->iFrame = (pcUnitData->psModelAnimation->iBeginFrame - pcUnitData->psModelAnimation->iEndFrame) + iCompareFrame;
				}
			}
		}
	}

	//End of buffer sequence?
	if ( pcUnitData->iPlayBuffCnt == pcUnitData->iPlayBuffPosiEnd )
	{
		//If the unit isn't idle, set it to idle
		if ((pcUnitData->GetAnimation() != ANIMATIONTYPE_Idle) &&
			(pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) &&
			(pcUnitData->GetAnimation() != ANIMATIONTYPE_Special) &&
			(pcUnitData->GetAnimation() != ANIMATIONTYPE_Attack) &&
			(pcUnitData->GetAnimation() != ANIMATIONTYPE_Skill))
		{
			pcUnitData->Animate(ANIMATIONTYPE_Idle);
		}

		pcUnitData->iFrame += 80;

		int iCompareFrame = pcUnitData->iFrame / 160;

		if ( pcUnitData->psModelAnimation &&
			(iCompareFrame >= pcUnitData->psModelAnimation->iEndFrame) )
		{
			if (pcUnitData->psModelAnimation->bLoop)
			{
				pcUnitData->ChangeAnimationID(pcUnitData->iAction, pcUnitData->iAnimDisplayMode);
			}
			else
			{
				pcUnitData->iFrame = pcUnitData->psModelAnimation->iEndFrame * 160;
			}

			//If current animation is attack or skill, set to idle
			if ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) ||
				(pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill))
			{
				pcUnitData->Animate(ANIMATIONTYPE_Idle);
			}
		}
	}

	//Got more buffers to play
	else
	{
		//Get the current play sequence from a list of play sequences.
		PlayBuff* psPlayBuff = pcUnitData->saPlayBuff + (pcUnitData->iPlayBuffCnt % _countof(pcUnitData->saPlayBuff));

		int iOldAction = pcUnitData->iAction;

		//set angle and animation id of unit
		pcUnitData->sAngle.iX = 0; //always 0
		pcUnitData->sAngle.iY = psPlayBuff->saAngle[1];
		pcUnitData->sAngle.iZ = 0;  //always 0
		pcUnitData->iAction   = psPlayBuff->saAngle[3];

		if (pcUnitData->iAction == 0)
		{
			pcUnitData->iAction = iOldAction;

			Point3D sDifferencePosition;
			sDifferencePosition.iX = abs(pcUnitData->sPosition.iX - psPlayBuff->sPosition.iX);
			sDifferencePosition.iY = abs(pcUnitData->sPosition.iY - psPlayBuff->sPosition.iY);
			sDifferencePosition.iZ = abs(pcUnitData->sPosition.iZ - psPlayBuff->sPosition.iZ);

			int iDistance = sDifferencePosition.iX + sDifferencePosition.iY + sDifferencePosition.iZ;

			if (iDistance > 8)
			{
				//Unit is not walking or running?
				if ((pcUnitData->GetAnimation() != ANIMATIONTYPE_Walking) &&
					(pcUnitData->GetAnimation() != ANIMATIONTYPE_Running))
				{
					if (iDistance < 512)
					{
						pcUnitData->Animate(ANIMATIONTYPE_Walking);
					}
					else if (!pcUnitData->Animate(ANIMATIONTYPE_Running))
					{
						pcUnitData->Animate(ANIMATIONTYPE_Walking);
					}
				}
			}
			else
			{
				if (pcUnitData->GetAnimation() != ANIMATIONTYPE_Idle)
				{
					pcUnitData->Animate(ANIMATIONTYPE_Idle);
				}
			}
		}

		pcUnitData->sPosition = psPlayBuff->sPosition;
		pcUnitData->iFloorHeight = pcUnitData->sPosition.iY;

		//move sequence has a target
		if (psPlayBuff->lTargetID)
		{
			//If this unit's target is not set or is not same, (re-)set it
			if ((pcUnitData->pcTarget && (pcUnitData->pcTarget->iID != psPlayBuff->lTargetID)) || (pcUnitData->pcTarget == NULL))
			{
				pcUnitData->pcTarget = UNITDATABYID(psPlayBuff->lTargetID);

				//if (pcUnitData->pcTarget)
					//DEBUG("UNIT TARGET IS NOW = %s", pcUnitData->pcTarget->GetName());
			}
		}
		else
		{
			pcUnitData->pcTarget = NULL;
		}

		if (psPlayBuff->iFrame < smPLAYBUFF_FRAME_NONE)
		{
			pcUnitData->iFrame = psPlayBuff->iFrame & CHAR_FRAME_MASK;

			//Select the model of the frame
			pcUnitData->iMotionSelectFrame = psPlayBuff->iFrame >> CHAR_FRAME_SELSHIFT;


			if (pcUnitData->iAnimDisplayMode)
			{
				if (pcUnitData->iMotionSelectFrame)
					pcUnitData->iPatDispMode = 2;		//DISP_MODE_PATSUB - Auxiliary model
				else
					pcUnitData->iPatDispMode = 1;		//DISP_MODE_PATMAIN - Main model
			}

			if (pcUnitData->iAction)
			{
				if ((pcUnitData->iPatDispMode == 2) && pcUnitData->psHead)
				{
					pcUnitData->psModelAnimation = pcUnitData->psHead->saModelAnimation + pcUnitData->iAction;	//&smMotionInfo2->MotionInfo[action];
				}
				else if (pcUnitData->psBody)
				{
					pcUnitData->psModelAnimation = pcUnitData->psBody->saModelAnimation + pcUnitData->iAction;	//&smMotionInfo->MotionInfo[action];
				}

				pcUnitData->iStartFrame = pcUnitData->psModelAnimation->iBeginFrame * 160;
				pcUnitData->iEndFrame = pcUnitData->psModelAnimation->iEndFrame * 160;
			}

			//Current animation is different than previous
			if (pcUnitData->iOldAnimationState != pcUnitData->GetAnimation())
			{
				if ((pcUnitData->GetAnimation() >= ANIMATIONTYPE_Attack) &&
					(pcUnitData->GetAnimation() != ANIMATIONTYPE_Attack) &&
					(pcUnitData->GetAnimation() != ANIMATIONTYPE_Skill))
				{
					//Character sound
					PlayUnitSound(pcUnitData);
				}
				else
				{
					//Occasional monster sounds
					if (bIsAMonster && (pcUnitData->GetAnimation() == ANIMATIONTYPE_Idle))
					{
						if (Dice::RandomI(0, 4) == 0)
							PlayUnitSound(pcUnitData);
					}
				}

				//iAnimationEvent is 0
				if (bIsAPlayer)
				{
					if ((pcUnitData->iMotionEvent == 0) &&
						((pcUnitData->iOldAnimationState == ANIMATIONTYPE_Attack) || (pcUnitData->iOldAnimationState == ANIMATIONTYPE_Skill)))
					{
						if (pcUnitData->psModelAnimation &&
							pcUnitData->psModelAnimation->iaEventFrame[0])
						{
							//DEBUG("%d = CALLT_WITH_ARG1(0x00438EF0, (DWORD)pcUnitData, FALSE);", TICKCOUNT);

							CALLT_WITH_ARG1(0x00438EF0, (DWORD)pcUnitData, FALSE); //Execute skipped events - int smCHAR::EventAttack(int Flag)
						    pcUnitData->dwAttackPlayTime = 0;					   //Except for long-range character attacks
							CALLT(0x00422170, (DWORD)pcUnitData);				   //ResetAttackTrans: AttackTrans.code = GetAttackTrans_XorCode();
						}

						//DWORD dwDamageTimeBefore = pcUnitData->dwLastDamageTime;
						//DEBUG("%d = UpdateAttack(FALSE) B, %d, dwLastDamageTime = %d", TICKCOUNT, pcUnitData->iFrameTimer, pcUnitData->dwLastDamageTime);


						pcUnitData->PlayAttackOverride(FALSE); //Attack applied  - will update pcUnitData->dwLastDamageTime
					}

					pcUnitData->iMotionEvent = 0;
				}

				if (psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_SKIL] >= 0) //0 = default attack
				{
					//DEBUG("%d = SKILLMANAGERHANDLER->BeginSkill A", TICKCOUNT);

					short sSkillCode = psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_SKIL];

					ActiveSkill activeSkill{};
					activeSkill.cLevel = sSkillCode & 0xF;
					activeSkill.cSkillTinyID = ( sSkillCode >> 8 ) & 0xFF;
					activeSkill.sParam = static_cast<USHORT>(psPlayBuff->saAngle[2]); //grab the parameter elsewhere

					//Start skill
					SKILLMANAGERHANDLER->BeginSkill(pcUnitData, activeSkill.iValue, activeSkill.cLevel, pcUnitData->pcTarget, 0, 0, 0);
				}

				if (bIsAMonster == TRUE && bIsAPlayer == FALSE)
				{
					if (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill)
					{
						//DEBUG("%d = SKILLMANAGERHANDLER->BeginSkill B", TICKCOUNT);

						SKILLMANAGERHANDLER->BeginSkill(pcUnitData, 0, 0, pcUnitData->pcTarget, 0, 0, 0);
					}

					if (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack)
					{
						//DEBUG("%d = BASIC ATTACK CALLT(0x00424880, (DWORD)pcUnitData);", TICKCOUNT);

						//DEBUG("%d = BASIC ATTACK A, %d, pcUnitData->dwLastDamageTime = %d, this type = %d, old type = %d",
						//TICKCOUNT, pcUnitData->iFrameTimer, pcUnitData->dwLastDamageTime, pcUnitData->GetAnimation(), pcUnitData->iOldAnimationType);

						CALLT(0x00424880, (DWORD)pcUnitData);	//Monster General Attack Start - int smCHAR::BeginAttack_Monster() - particles
					}
				}
				else if (bIsAPlayer && pcUnitData->iOldAnimationState == ANIMATIONTYPE_Skill)
				{
					//DEBUG("%d = UPDATE SKILL ATTACK, TICKCOUNT");


					//This seems to cause damage overlaps
					pcUnitData->PlayAttackOverride(FALSE);	//Attack applied
				}

				switch (pcUnitData->GetAnimation())
				{
				case ANIMATIONTYPE_Special:
					if (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN)
					{
						SKILLMANAGERHANDLER->StartSkillEffectOld(pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + 0x3000, pcUnitData->sPosition.iZ, 3500);
						SKILLMANAGERHANDLER->SkillPlaySound(0x1000, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ);
						SetDynamicLight(pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 50, 100, 70, 0, 300, 1);
						PlayUnitSound(pcUnitData);
					}
					break;
				case ANIMATIONTYPE_Revive:
					SKILLMANAGERHANDLER->StartSkillEffectOld(pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 500);
					SetDynamicLight(pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 100, 100, 100, 0, 200);
					PlayUnitSound(pcUnitData);
					break;

				case ANIMATIONTYPE_Die:
					pcUnitData->iFrameCounter = 0;
					switch (pcUnitData->sCharacterData.iMonsterEffectID)
					{
					case MONSTEREFFECTID_DEADZONE:
						CALL_WITH_ARG1(0x005B4340, (DWORD)pcUnitData);	//AssaParticle_MonsterDeadZoneDead(this);
						break;
					case MONSTEREFFECTID_HYPERMACHINE:
						CALL_WITH_ARG1(0x005B4380, (DWORD)pcUnitData);	//AssaParticle_MonsterHyperDead(this);
						break;
					case MONSTEREFFECTID_MYSTIC:
						CALL_WITH_ARG1(0x005B0380, (DWORD)pcUnitData);	//ParkAssaParticle_MisticSpinel(this);
						break;
					case MONSTEREFFECTID_DARKMAGE:
						CALL_WITH_ARG1(0x005B1900, (DWORD)pcUnitData);	//ParkAssaParticle_DarkMageDead(this);
						break;
					case MONSTEREFFECTID_SEALCRASHER:
						CALL_WITH_ARG1(0x005B53A0, (DWORD)pcUnitData);
						break;
					default:
						break;
					}

					break;

				default:
					break;
				}
			}
		}
		else
		{
			if (bIsAPlayer)
			{
				if ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running))
				{
					int iMovementSpeed = (((pcUnitData->sCharacterData.iMovementSpeed * 5) + 145) << 1);
					iMovementSpeed = (iMovementSpeed * 80) / 300;

					pcUnitData->iFrame += iMovementSpeed - 8;
				}
				else if ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill))
				{
					pcUnitData->iFrame += SKILLMANAGERHANDLER->GetAttackFrameSpeed(pcUnitData->sCharacterData.iAttackSpeed);
				}
				else
				{
					pcUnitData->iFrame += psPlayBuff->iFrame & 0x0000FFFF;
				}

				if ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running))
					pcUnitData->bAddFrameUnit = TRUE;
				else
					pcUnitData->bAddFrameUnit = FALSE;
			}
			else
			{
				pcUnitData->iFrame += psPlayBuff->iFrame & 0x0000FFFF;
			}
		}

		ActiveSkill lsActiveSkill{};
		lsActiveSkill.cLevel		= psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_SKIL] & 0xF;
		lsActiveSkill.cSkillTinyID  = psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_SKIL] >> 8 & 0xFF;
		lsActiveSkill.sParam		= psPlayBuff->saAngle[2];

		pcUnitData->sActiveSkill.iValue = lsActiveSkill.iValue;

		//DEBUG("%d = EventAttack, AttackEffect = %d, dwDamageLastTime = %d, dwLastDamageTime = %d", TICKCOUNT, pcUnitData->iAttackEffect, pcUnitData->dwDamageLastTime, pcUnitData->dwLastDamageTime);
		//See also  BOOL SkillManager::EventHitMonster( UnitData * pcUnitData )
		//EventAttack will also in turn call PlayAttack or SkillAttack

		CALLT_WITH_ARG1(0x00438EF0, (DWORD)pcUnitData, FALSE);							//0x00438EF0 = EventAttack()


		if ( (pcUnitData->sCharacterData.iType != CHARACTERTYPE_Player) &&
		   ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running)) )
		{
			//Footsteps sound (bargon and giant monster)
			if ( pcUnitData->psModelAnimation->iaEventFrame[0] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[0] > (iCompareFrame - 80)) && (pcUnitData->psModelAnimation->iaEventFrame[0] <= iCompareFrame) )
					PlayUnitSound( pcUnitData );
			}

			if ( pcUnitData->psModelAnimation->iaEventFrame[1] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[1] > (iCompareFrame - 80)) && (pcUnitData->psModelAnimation->iaEventFrame[1] <= iCompareFrame) )
					PlayUnitSound( pcUnitData );
			}
		}

		pcUnitData->iFrameStep = 80;

		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Potion )
		{
			//Potion action
			if ( pcUnitData->psModelAnimation->iaEventFrame[0] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[0] > (iCompareFrame - pcUnitData->iFrameStep)) && (pcUnitData->psModelAnimation->iaEventFrame[0] <= iCompareFrame) )
				{
					int iHeightEffect = pcUnitData->sPosition.iY + pcUnitData->iModelHeight;
					if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
						iHeightEffect = pcUnitData->sPosition.iY + 0x3000;

					switch ( pcUnitData->iPotionStatus )
					{
						case POTIONSTATUS_HP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 600 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_MP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 700 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_SP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 800 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_HPMPSP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 800 );
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 700 );
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 600 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_EtherCore:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + 0x3000, pcUnitData->sPosition.iZ, 3500 );
							SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ );
							break;
						default:
							break;
					}
				}
			}
		}

		int iCompareFrame = 0;


		BOOL bIsMonSelfDestructOnDeath = pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Die &&
			(pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MYSTIC ||
			pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_SEALCRASHER);


		//Monster NPC only
		if (bIsAMonster == TRUE && bIsAPlayer == FALSE &&
			(pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Skill ||
			 pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Attack ||
			 bIsMonSelfDestructOnDeath))
		{
			iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);


			BOOL bContainsEvent0 = pcUnitData->psModelAnimation->iaEventFrame[0] != 0;
			BOOL bContainsEvent1 = pcUnitData->psModelAnimation->iaEventFrame[1] != 0;
			BOOL bContainsEvent2 = pcUnitData->psModelAnimation->iaEventFrame[2] != 0;
			BOOL bContainsEvent3 = pcUnitData->psModelAnimation->iaEventFrame[3] != 0;




			BOOL bEvent0 = FALSE;

			//Babel skill is 165 frames long, the skill hits the player at around 120 frame
			if (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_BABEL &&
				pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Skill)
			{
				bEvent0 = bContainsEvent0 &&
					((120 * 160) > (iCompareFrame - pcUnitData->iFrameStep)) &&
					((120 * 160) <= iCompareFrame);
			}
			else
			{
				bEvent0 = bContainsEvent0 &&
					(pcUnitData->psModelAnimation->iaEventFrame[0] > (iCompareFrame - pcUnitData->iFrameStep)) &&
					(pcUnitData->psModelAnimation->iaEventFrame[0] <= iCompareFrame);
			}


			//no events frame values, and yet the monster is 'attacking' or using a 'skill'? let's use the middle frame number between begin and end
			BOOL bContainsNoEventFrameValues = bContainsEvent0 == FALSE && bContainsEvent1 == FALSE && bContainsEvent2 == FALSE && bContainsEvent3 == FALSE;
			if ( bContainsNoEventFrameValues )
			{
				int targetFrame = ( ( pcUnitData->psModelAnimation->iEndFrame - pcUnitData->psModelAnimation->iBeginFrame ) / 2 ) * 160;

				if ( targetFrame > ( iCompareFrame - pcUnitData->iFrameStep ) &&
					( targetFrame <= iCompareFrame ) )
				{
					bContainsEvent0 = TRUE;
					bEvent0 = TRUE;
				}
			}


			BOOL bEvent1 = bContainsEvent1 &&
				(pcUnitData->psModelAnimation->iaEventFrame[1] > (iCompareFrame - pcUnitData->iFrameStep)) &&
				(pcUnitData->psModelAnimation->iaEventFrame[1] <= iCompareFrame);

			BOOL bEvent2 = bContainsEvent2 &&
				(pcUnitData->psModelAnimation->iaEventFrame[2] > (iCompareFrame - pcUnitData->iFrameStep)) &&
				(pcUnitData->psModelAnimation->iaEventFrame[2] <= iCompareFrame);

			BOOL bEvent3 = bContainsEvent3 &&
				(pcUnitData->psModelAnimation->iaEventFrame[3] > (iCompareFrame - pcUnitData->iFrameStep)) &&
				(pcUnitData->psModelAnimation->iaEventFrame[3] <= iCompareFrame);

			int iEventCount = 0;
			if (bContainsEvent0) iEventCount++;
			if (bContainsEvent1) iEventCount++;
			if (bContainsEvent2) iEventCount++;
			if (bContainsEvent3) iEventCount++;

			int iLastEvent = -1;
			if (bContainsEvent3) iLastEvent = 3;
			else if (bContainsEvent2) iLastEvent = 2;
			else if (bContainsEvent1) iLastEvent = 1;
			else if (bContainsEvent0) iLastEvent = 0;


			if (bEvent0 || bEvent1 || bEvent2 || bEvent3)
			{
				if (bContainsEvent1)
				{
					//DEBUG("bContainsEvent1 = %d", bContainsEvent1);
				}

				if (bEvent0)
				{
					//DEBUG("%d = bEvent0", TICKCOUNT);
				}

				if (bEvent1)
				{
					//DEBUG("%d = bEvent1", TICKCOUNT);
				}

				Unit* pcUnit = UNITDATATOUNIT(pcUnitData);

				int iEventIndex = bEvent0 ? 0 : (bEvent1 ? 1 : (bEvent2 ? 2 : (bEvent3 ? 3 : 0)));

				pcUnit->cMutex->lock();
				if (pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Skill || bIsMonSelfDestructOnDeath)
				{
					//Sometimes frame region is played more than twice. So checks are needed here.
					//Also the monster could be attacking another character, but not me. So need the 2000ms check
					//For last received damage
					BOOL bSkip = ((TICKCOUNT - pcUnit->dwLastSkillAttackReceived) < 500) &&
								 iEventIndex == pcUnit->iLastSkillAttackEvent;

					//Skip if received basic or skill damage is more than 2 seconds old or that the last attack is more than 2 seconds old.
					// It's likely that the monster is attacking another player, or that we haven't received skill damage from the server yet
					BOOL bSkip1 = pcUnit->dwLastReceivedAttackTrans == 0 ||
						((TICKCOUNT - pcUnit->dwLastReceivedAttackTrans) > 5000);

					//If the target of the skill attack is me, force bSkip1 to FALSE (override)
					if (pcUnitData->pcTarget && pcUnitData->pcTarget->iID == UNITDATA->iID)
						bSkip1 = FALSE;

					//Skip event frames that occurs within 80ms from previous. Too fast.
					//Some monsters have this issue - like 'Ice Goblin'
					BOOL bSkip2 = ((TICKCOUNT - pcUnit->dwLastSkillAttackReceived) < 80) &&
								iEventIndex != pcUnit->iLastSkillAttackEvent;

					int maxDistToMon = 400;

					int iDistanceA = 0, iDistanceB = 0, iDistanceC = 0;
					pcUnitData->sPosition.GetPTDistanceXZ(&UNITDATA->sPosition, iDistanceA, iDistanceB, iDistanceC);
					int distToMon = (int)sqrt(iDistanceA);

					int distToSkillPos = distToMon;

					if (pcUnit->sLatestSkillAttackTrans.dwTarObjectSerial != 0)
					{
						UnitData* pcTargetUnitData = UNITDATABYID(pcUnit->sLatestSkillAttackTrans.dwTarObjectSerial);
						Unit* pcUnit = UNITDATATOUNIT(pcUnitData);

						if (pcTargetUnitData && pcUnit)
						{
							int x = (pcUnit->sLatestSkillAttackTrans.sPosition.iX - pcTargetUnitData->sPosition.iX) >> 8;
							int y = (pcUnit->sLatestSkillAttackTrans.sPosition.iY - pcTargetUnitData->sPosition.iY) >> 8;
							int z = (pcUnit->sLatestSkillAttackTrans.sPosition.iZ - pcTargetUnitData->sPosition.iZ) >> 8;

							distToSkillPos = x * x + y * y + z * z;

							//DEBUG("TARGET IS = %s, dist = %d vs %d", pcTargetUnitData->GetName(), dist, 19600);
						}
					}
					else if (pcUnit->sLatestSkillAttackTrans.sPosition.iX != 0 &&
							 pcUnit->sLatestSkillAttackTrans.sPosition.iZ != 0)
					{
						int x = (pcUnit->sLatestSkillAttackTrans.sPosition.iX - UNITDATA->sPosition.iX) >> 8;
						int y = (pcUnit->sLatestSkillAttackTrans.sPosition.iY - UNITDATA->sPosition.iY) >> 8;
						int z = (pcUnit->sLatestSkillAttackTrans.sPosition.iZ - UNITDATA->sPosition.iZ) >> 8;

						distToSkillPos = x * x + y * y + z * z;
					}


					//also check max dist to monster
					// because player can run around the monster and don't get hit
					// otherwise
					BOOL bSkip4 = distToMon > maxDistToMon && distToSkillPos > 19600;


					if (bSkip)
					{
						//DEBUG("bSkip (%d)", TICKCOUNT);
					}

					if (bSkip1)
					{
						//DEBUG("bSkip1 (%d)", (TICKCOUNT - pcUnit->dwLastReceivedAttackTrans));
					}

					if (bSkip2)
					{
						//DEBUG("bSkip2");
					}

					if (bSkip4)
					{
						//DEBUG("bSkip4 (MON = %d vs %d, SKILL = %d vs %d)", distToMon, maxDistToMon, distToSkillPos, 19600);
					}


					if (bSkip == FALSE && bSkip1 == FALSE && bSkip2 == FALSE && bSkip4 == FALSE)
					{

						BOOL bUpdateChecksum = FALSE;

						//It's the last 'attack' event of the animation sequence. Do full damage
						if (iEventIndex == iLastEvent)
						{
							CopyMemory(&pcUnitData->sAttackTrans, &pcUnit->sLatestSkillAttackTrans, sizeof(PacketAttackData));
						}

						//Otherwise do reduced damage (40-66%) as it's part of the attack chain
						else
						{
							CopyMemory(&pcUnitData->sAttackTrans, &pcUnit->sLatestSkillAttackTrans, sizeof(PacketAttackData));

							//Two hit combo
							if (iEventCount == 2)
								pcUnitData->sAttackTrans.iDamage = static_cast<int>(pcUnitData->sAttackTrans.iDamage / Dice::RandomD(1.1, 1.3));

							//Three hit combo
							else if (iEventCount == 3)
								pcUnitData->sAttackTrans.iDamage = static_cast<int>(pcUnitData->sAttackTrans.iDamage / Dice::RandomD(1.2, 1.4));

							//Four hit combo
							else if (iEventCount == 4)
								pcUnitData->sAttackTrans.iDamage = static_cast<int>(pcUnitData->sAttackTrans.iDamage / Dice::RandomD(1.3, 1.5));

							bUpdateChecksum = TRUE;
						}

						if (pcUnitData->bDamageReduction)
						{
							pcUnitData->sAttackTrans.iDamage -= ( pcUnitData->bDamageReduction * pcUnitData->sAttackTrans.iDamage ) / 100;
							bUpdateChecksum = TRUE;
						}

						if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Priestess &&
							 pcUnitData->sCharacterData.iMonsterType == EMonsterType::MONSTERTYPE_Undead)
						{
							int absorb = TIMERSKILLHANDLER->FindSkillTimerIntValue ( SKILLID_SummonMuspell, ESkillArrayPointer::Priestess_SummonMuspell_UndeadAbsorb_New );
							if ( absorb > 0 )
							{
								pcUnitData->sAttackTrans.iDamage -= ( absorb * pcUnitData->sAttackTrans.iDamage ) / 100;
								bUpdateChecksum = TRUE;
							}
						}

						//For Bellatra
						if (CHARACTERGAME->GetStageID() == MAPID_Bellatra &&
							UNITGAME->iBellatraRoomInitialPlayerCount)
						{
							int iAdjustedDamage = AdjustMonsterDamageForBellatra(pcUnitData->sAttackTrans.iDamage);

							if (iAdjustedDamage != pcUnitData->sAttackTrans.iDamage)
							{
								pcUnitData->sAttackTrans.iDamage = iAdjustedDamage;
								bUpdateChecksum = TRUE;
							}
						}

						if ( bUpdateChecksum)
						{
							pcUnitData->sAttackTrans.dwChkSum = pcUnitData->sAttackTrans.iDamage * 2002 + (pcUnitData->sAttackTrans.sPosition.iX * pcUnitData->sAttackTrans.sPosition.iY * pcUnitData->sAttackTrans.sPosition.iZ);
							pcUnitData->sAttackTrans.dwDamageChkSum = CHARACTERGAME->GetDamgeChkSum_S2V(&pcUnitData->sAttackTrans);
						}

						if (pcUnitData->pcTarget)
						{
							//DEBUG("%d = EVENT - SKILL, Mon = %s, Target = %s, Damage = %d, Range = %d, Delta = %d, EventIndex = %d", TICKCOUNT, pcUnitData->GetName(), pcUnitData->pcTarget->GetName(), pcUnitData->sAttackTrans.iDamage, pcUnitData->sAttackTrans.iRange, TICKCOUNT - pcUnit->dwLastSkillAttackReceived, iEventIndex);
						}
						else
						{
							//DEBUG("%d = EVENT - SKILL, Mon = %s, Damage = %d, Range = %d, Delta = %d, EventIndex = %d", TICKCOUNT, pcUnitData->GetName(), pcUnitData->sAttackTrans.iDamage, pcUnitData->sAttackTrans.iRange, TICKCOUNT - pcUnit->dwLastSkillAttackReceived, iEventIndex);
						}

						CHARACTERGAME->XorAttackTrans(pcUnitData, &pcUnitData->sAttackTrans);

						//try with this?
						//CALLT_WITH_ARG1(0x00438EF0, (DWORD)pcUnitData, FALSE);



						pcUnitData->PlayAttackOverride(TRUE);
						pcUnit->dwLastSkillAttackReceived = TICKCOUNT;
						pcUnit->iLastSkillAttackEvent = iEventIndex;
						ZeroMemory(&pcUnitData->sAttackTrans, sizeof(PacketAttackData));
					}
					else
					{
						//DEBUG("%d = SKIPPING EVENT - SKILL. bSkip = %d, bSkip1 = %d, bSkip2 = %d, EventIndex = %d, Delta = %d", TICKCOUNT, bSkip, bSkip1, bSkip2, iEventIndex, TICKCOUNT - pcUnit->dwLastReceivedAttackTrans);
					}
				}
				else if (pcUnitData->psModelAnimation->iType == ANIMATIONTYPE_Attack)
				{
					BOOL bSkip = TICKCOUNT - pcUnit->dwLastBasicAttackReceived < 500 &&
						iEventIndex == pcUnit->iLastBasicAttackEvent;

					//Skip if received basic damage is more than 2 seconds old. It's likely that
					//the monster is attacking another player, or that we haven't received basic damage from the server yet
					BOOL bSkip1 = pcUnit->dwLastReceivedAttackTrans == 0 || ((TICKCOUNT - pcUnit->dwLastReceivedAttackTrans) > 2000);

					//Skip event frames that occurs within 60ms from previous. Too fast.
					//Some monsters have this issue - like 'Ice Goblin'
					//Note, monsters like 'Hobo Golem' does 4 hits in a short time span, but they are usually 150ms
					BOOL bSkip2 = ((TICKCOUNT - pcUnit->dwLastBasicAttackReceived) < 80) &&
						iEventIndex != pcUnit->dwLastBasicAttackReceived;

					//If the target of the basic attack is me, force bSkip1 to FALSE (override)
					if (pcUnitData->pcTarget && pcUnitData->pcTarget->iID == UNITDATA->iID)
						bSkip1 = FALSE;

					int iDistanceA = 0, iDistanceB = 0, iDistanceC = 0;
					pcUnitData->sPosition.GetPTDistanceXZ(&UNITDATA->sPosition, iDistanceA, iDistanceB, iDistanceC);
					int iDistanceToMon = (int)sqrt(iDistanceA);
					int iAttackRange = pcUnitData->sCharacterData.iAttackRange / 256;

					BOOL bSkip3 = iDistanceToMon > static_cast<int>((iAttackRange * 1.5));


					if (bSkip1)
					{
						//DEBUG("bSkip1 (%d)", (TICKCOUNT - pcUnit->dwLastReceivedAttackTrans));
					}

					if (bSkip2)
					{
					//	DEBUG("bSkip2");
					}

					if (bSkip3)
					{
						//DEBUG("SKIPPING: 2 mon = %d vs attack = %d", iDistanceToMon, static_cast<int>(iAttackRange * 1.5));
					}


					if (bSkip == FALSE && bSkip1 == FALSE && bSkip2 == FALSE && bSkip3 == FALSE)
					{
						CopyMemory(&pcUnitData->sAttackTrans, &pcUnit->sLatestBasicAttackTrans, sizeof(PacketAttackData));

						if (pcUnitData->pcTarget)
						{
							//DEBUG("%d = EVENT - BASIC, Mon = %s, Target = %s, Damage = %d, Range = %d, Delta = %d, EventIndex = %d", TICKCOUNT, pcUnitData->GetName(), pcUnitData->pcTarget->GetName(), pcUnitData->sAttackTrans.iDamage, TICKCOUNT - pcUnit->dwLastBasicAttackReceived, iEventIndex);
						}
						else
						{
							//DEBUG("%d = EVENT - BASIC, Mon = %s, Damage = %d, Range = %d, Delta = %d, EventIndex = %d", TICKCOUNT, pcUnitData->GetName(), pcUnitData->sAttackTrans.iDamage, TICKCOUNT - pcUnit->dwLastReceivedAttackTrans, iEventIndex);
						}

						BOOL bUpdateChecksum = FALSE;
						int iAdjustedDamage;

						if (pcUnitData->bDamageReduction)
						{
							pcUnitData->sAttackTrans.iDamage -= ( pcUnitData->bDamageReduction * pcUnitData->sAttackTrans.iDamage ) / 100;
							bUpdateChecksum = TRUE;
						}

						if ( UNITDATA->sCharacterData.iClass == ECharacterClass::CHARACTERCLASS_Priestess &&
							 pcUnitData->sCharacterData.iMonsterType == EMonsterType::MONSTERTYPE_Undead)
						{
							int absorb = TIMERSKILLHANDLER->FindSkillTimerIntValue ( SKILLID_SummonMuspell, ESkillArrayPointer::Priestess_SummonMuspell_UndeadAbsorb_New );
							if ( absorb > 0 )
							{
								pcUnitData->sAttackTrans.iDamage -= ( absorb * pcUnitData->sAttackTrans.iDamage ) / 100;
								bUpdateChecksum = TRUE;
							}
						}

						//For Bellatra
						if (CHARACTERGAME->GetStageID() == MAPID_Bellatra &&
							UNITGAME->iBellatraRoomInitialPlayerCount)
						{
							int iAdjustedDamage = AdjustMonsterDamageForBellatra(pcUnitData->sAttackTrans.iDamage);

							if (iAdjustedDamage != pcUnitData->sAttackTrans.iDamage)
							{
								pcUnitData->sAttackTrans.iDamage = iAdjustedDamage;
								bUpdateChecksum = TRUE;
							}
						}

						if (bUpdateChecksum)
						{
							pcUnitData->sAttackTrans.dwChkSum = pcUnitData->sAttackTrans.iDamage * 2002 + (pcUnitData->sAttackTrans.sPosition.iX * pcUnitData->sAttackTrans.sPosition.iY * pcUnitData->sAttackTrans.sPosition.iZ);
							pcUnitData->sAttackTrans.dwDamageChkSum = CHARACTERGAME->GetDamgeChkSum_S2V( &pcUnitData->sAttackTrans );
						}

						/*if ( pcUnitData->sAttackTrans.dwDamageChkSum != CHARACTERGAME->GetDamgeChkSum_S2V( &pcUnitData->sAttackTrans ) )
						{
							CHATBOX->AddMessage( "Checksum failing 7" );
						}

						CHARACTERGAME->EncodeDamagePacket( &pcUnitData->sAttackTrans );

						//CHARACTERGAME->XorAttackTrans(pcUnitData, &pcUnitData->sAttackTrans);

						CHARACTERGAME->EncodeDamagePacket( &pcUnitData->sAttackTrans );*/

						CHARACTERGAME->XorAttackTrans(pcUnitData, &pcUnitData->sAttackTrans);

						pcUnitData->PlayAttackOverride(TRUE);
						pcUnit->dwLastBasicAttackReceived = TICKCOUNT;
						pcUnit->iLastBasicAttackEvent = iEventIndex;

						ZeroMemory(&pcUnitData->sAttackTrans, sizeof(PacketAttackData));
					}
					else
					{
					//DEBUG("%d = SKIPPING EVENT - BASIC. bSkip = %d, bSkip1 = %d, bSkip2 = %d, EventIndex = %d, Delta %d", TICKCOUNT, bSkip, bSkip1, bSkip2, iEventIndex, TICKCOUNT - pcUnit->dwLastReceivedAttackTrans);
					}
				}

				pcUnit->cMutex->unlock();
			}
		}

		iCompareFrame = pcUnitData->iFrame / 160;

		if ( iCompareFrame < pcUnitData->psModelAnimation->iBeginFrame )
		{
			pcUnitData->iFrame = pcUnitData->psModelAnimation->iBeginFrame * 160;
		}

		if ( iCompareFrame >= pcUnitData->psModelAnimation->iEndFrame )
		{
			if ( pcUnitData->psModelAnimation->bLoop )
				pcUnitData->ChangeAnimationID( pcUnitData->iAction, pcUnitData->iAnimDisplayMode );
			else
				pcUnitData->iFrame = pcUnitData->psModelAnimation->iEndFrame * 160;

			pcUnitData->iAttackEffect = 0;
		}


		//Check the installed item code and apply changes
		if ( bIsAPlayer )
		{
			//Left Hand
			//If the equipped item on the left hand doesn't match, and there is an equipped item
			// in the animation sequence. Then equip the correct item or the unit.
			if ( psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_LEFT] != pcUnitData->saEquippedItems[smPLAYBUFF_ITEM_LEFT] )
			{
				if ( psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_LEFT] >= 0 )
				{
					ITEMHANDLER->EquipItem( pcUnitData, ITEMHANDLER->FindItemTable( psPlayBuff->saEquippedItems[1] )->sBaseItemID.ToItemID(), 2 );
					pcUnitData->sLeftHandTool.iBlinkSpeed = 0;
					pcUnitData->sLeftHandTool.bStarEffect = FALSE;
				}
				else
					ITEMHANDLER->EquipItem( pcUnitData, ITEMID_None, 2 );
			}

			//Right Hand
			//If the equipped item on the right hand doesn't match, and there is an equipped item
			// in the animation sequence. Then equip the correct item or the unit.
			if ( psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_RIGHT] != pcUnitData->saEquippedItems[smPLAYBUFF_ITEM_RIGHT] )
			{
				if ( psPlayBuff->saEquippedItems[smPLAYBUFF_ITEM_RIGHT] >= 0 )
				{
					ITEMHANDLER->EquipItem( pcUnitData, ITEMHANDLER->FindItemTable( psPlayBuff->saEquippedItems[2] )->sBaseItemID.ToItemID(), 1 );
					pcUnitData->sRightHandTool.iBlinkSpeed = 0;
					pcUnitData->sRightHandTool.bStarEffect = FALSE;
				}
				else
					ITEMHANDLER->EquipItem( pcUnitData, ITEMID_None, 1 );
			}

			//Body
			if ( psPlayBuff->saEquippedItems[0] != pcUnitData->saEquippedItems[0] )
			{
				ITEMHANDLER->EquipBody( pcUnitData, psPlayBuff->saEquippedItems[0] );
				pcUnitData->saEquippedItems[0] = psPlayBuff->saEquippedItems[0];
			}
		}

		//Increment the play buffer sequence
		pcUnitData->iPlayBuffCnt++;
		pcUnitData->iMoveCount = 0;


		//Anger change
		//if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MINIG )
		//{
		//	if ( ( ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack ) || ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Flinch ) ) )
		//	{
		//		pcUnitData->iFireOverlay += 2;
		//		if ( pcUnitData->iFireOverlay > 100 )
		//			pcUnitData->iFireOverlay = 100;
		//	}
		//	else
		//	{
		//		pcUnitData->iFireOverlay--;
		//		if ( pcUnitData->iFireOverlay < 0 )
		//			pcUnitData->iFireOverlay = 0;
		//	}
		//}

		if ( pcUnitData->iFireOverlay != 0 )
		{
			if ( ( ( pcUnitData->iFireOverlay > 0 ) && ( pcUnitData->iFireOverlay < 160 ) ) || ( pcUnitData->iFireOverlay < 0 ) )
				pcUnitData->iFireOverlay++; //increment buring until it gets to 160 or 0 if -ve

			if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
				pcUnitData->iFireOverlay = 0;
		}

		if ( pcUnitData->iIceOverlay != 0 )
		{
			if ( ((pcUnitData->iIceOverlay > 0) && (pcUnitData->iIceOverlay < 160)) || (pcUnitData->iIceOverlay < 0) )
				pcUnitData->iIceOverlay++;

			if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
				pcUnitData->iIceOverlay = 0;
		}

		//Poison infection which can be +ve and -ve.. ?
		//with -ve value, it increments until 0 then stops
		//with +ve value, it stays in effect until set to 0
		if ( pcUnitData->iPoisonOverlay != 0 )
		{
			if ( ((pcUnitData->iPoisonOverlay > 0) && (pcUnitData->iPoisonOverlay < 160)) || (pcUnitData->iPoisonOverlay < 0) )
				pcUnitData->iPoisonOverlay++; //increment poison until it gets to 160 or 0 if -ve

			if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
				pcUnitData->iPoisonOverlay = 0;
		}



		//Distortion function
		if ( pcUnitData->bDistortion > 0 )
			pcUnitData->bDistortion--;

		//Map check every ~1s
		if ( ((pcUnitData->iPlayBuffCnt % 64) == 0) && (pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster) )
		{
			pcUnitData->iLoadedMapIndex = -1;

			int iHeight = 0;

			if ( STAGE1 )
			{
				iHeight = STAGE1->GetHighestPoint( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, pcUnitData->pcBodyMeshContainer->uMeshHeight );

				pcUnitData->iLoadedMapIndex = 0;
			}

			if ( (iHeight == (-32767)) && STAGE2 ) //-32767 = CLIP_OUT
				pcUnitData->iLoadedMapIndex = 1;
		}

		if (bIsAPlayer &&
			pcUnitData->dwAttackPlayTime && (pcUnitData->dwAttackPlayTime < PLAYTIME))
		{
			//DEBUG("%d = UPDATE ATTACK, %d, pcUnitData->dwLastDamageTime = %d, Old = %d, New = %d", TICKCOUNT, pcUnitData->dwLastDamageTime, pcUnitData->iOldAnimationType, pcUnitData->GetAnimation());

			pcUnitData->PlayAttackOverride(TRUE);
			pcUnitData->dwAttackPlayTime = 0;
		}

		pcUnitData->iOldAnimationState = pcUnitData->GetAnimation();

		if (pcUnitData->bVisible == FALSE)
		{
			int iDistanceXZY = 0, iDistanceXZ = 0, iDistanceX = 0, iDistanceZ = 0, iDistanceY = 0;
			pcUnitData->sPosition.GetPTDistanceXZY(&UNITDATA->sPosition, iDistanceXZY,  iDistanceXZ, iDistanceX, iDistanceZ, iDistanceY);

			if (iDistanceXZ < DISTANCE_MAX_UNIT_BASIC_VIEW && iDistanceX < DISTANCE_MISC && iDistanceZ < DISTANCE_MISC && iDistanceY < DISTANCE_MISC_Y)
			{
				pcUnitData->bVisible = TRUE;
			}
		}
	}

	if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN) && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster) )
	{
		if ( ((pcUnitData->iLoopCounter & 1) == 0) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) )
		{
			CALL_WITH_ARG6( 0x0048AF40, 0, 0, (Dice::RandomI(0, 48) - 24) << 8, 0, pcUnitData->sAngle.iY, 0 );
			CALL_WITH_ARG7( 0x005266B0, 0x01000001, pcUnitData->sPosition.iX + RADIAN3D_X, pcUnitData->sPosition.iY + (30 << 8), pcUnitData->sPosition.iZ + RADIAN3D_Z, 1000, 1, 0 );
		}

		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
		{
			//Red
			if ( pcUnitData->sAddColorEffect[2] > 0 )
				pcUnitData->sAddColorEffect[2]--;
			//Green
			if ( pcUnitData->sAddColorEffect[1] > 0 )
				pcUnitData->sAddColorEffect[1]--;
			//Blue
			if ( pcUnitData->sAddColorEffect[0] > 0 )
				pcUnitData->sAddColorEffect[0]--;
		}
	}

	if ( pcUnitData->GetMaxHealth() > 0 )
	{
		if ( pcUnitData->sCharacterData.sSize )
		{
			//Christmas Goblin Size, ho ho ho! ^_^
			/*if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_HOBGOBLIN) && (pcUnitData->sCharacterData.iLevel == 60) )
			{
				int iSize = ((pcUnitData->sCharacterData.sHP.sMax - pcUnitData->sCharacterData.sHP.sCur) * 460) / pcUnitData->sCharacterData.sHP.sMax;
				iSize += 120;
				pcUnitData->sCharacterData.sSize = iSize;
			}*/
		}

		//Moriph
		if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MORIF) && (pcUnitData->sCharacterData.iLevel == 60) )
		{
			int iSize = ((pcUnitData->GetMaxHealth() - pcUnitData->GetCurrentHealth()) * 700) / pcUnitData->GetMaxHealth();
			iSize += 300;
			pcUnitData->sCharacterData.sSize = iSize;
		}
	}

	if ( pcUnitData->GetCurrentHealth() != pcUnitData->iCurHPBar )
	{
		if ( pcUnitData->iCurHPBar < pcUnitData->GetCurrentHealth() )
			pcUnitData->iCurHPBar++;
		else
			pcUnitData->iCurHPBar--;
	}


	if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Special )
	{
		if ( (pcUnitData->iFrameCounter > 10) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN) )
		{
			//Red
			if ( pcUnitData->sAddColorEffect[2] > 0 )
				pcUnitData->sAddColorEffect[2] -= 8;
			//Green
			if ( pcUnitData->sAddColorEffect[1] > 0 )
				pcUnitData->sAddColorEffect[1] -= 8;
			//Blue
			if ( pcUnitData->sAddColorEffect[0] > 0 )
				pcUnitData->sAddColorEffect[0] -= 8;
			//Alpha
			if ( pcUnitData->sAddColorEffect[3] > (-256) )
				pcUnitData->sAddColorEffect[3] -= 8;

			pcUnitData->DisplayAlpha = -16;
		}
	}

	if ( pcUnitData->dwEnchantWeaponTimeOut )
	{
		if ( pcUnitData->dwEnchantWeaponTimeOut < TICKCOUNT )
		{
			pcUnitData->dwEnchantWeaponTimeOut	= 0;
			pcUnitData->iEnchantWeaponType		= 0;
		}
	}

	if ( pcUnitData->iVanishTimeLeft > 0 )
		pcUnitData->iVanishTimeLeft--;

	if ( pcUnitData->bNoDeath )
		pcUnitData->bNoDeath--;

	if ( pcUnitData->sCharacterData.iClanID )
	{
		if (pcUnitData->iClanInfoIndex < 0)
		{
			if (pcUnitData->dwClanUpdateTime && ((pcUnitData->dwClanUpdateTime + (60 * 1000)) < TICKCOUNT))
			{
				pcUnitData->iClanInfoIndex = CALL_WITH_ARG1(0x0065B900, pcUnitData->sCharacterData.iClanID);
				pcUnitData->dwClanUpdateTime = TICKCOUNT;
			}
		}
	}

	//pcUnitData->iLoopCounter++;
	//pcUnitData->iFrameCounter++;

	return TRUE;
}

int AdjustMonsterDamageForBellatra(int iDamage)
{
	if (bDisableSodDmgReduction)
	{
		return iDamage;
	}

	int iPlayerAbsorbRating = UNITDATA->sCharacterData.iAbsorbRating;

	int iReducedDmg = iDamage - iPlayerAbsorbRating;

	//if iReducedDmg is -ve then absorb will absorb 100% of the damage or more
	//no further adjustment needed
	if (iReducedDmg <= 0)
		return iDamage;

	float percentReductionTotal = 0.0f;
	int iFlatDmgDecrease = 0;

	//Level 80  will get further 30% damage reduction
	//Level 90  will get further 20% damage reduction
	//Level 100 will get further 10% damage reduction
	//Level 110 will get further 0% damage reduction
	float percentReductionLvlBased = static_cast<float>(110 - UNITDATA->sCharacterData.iLevel) / 100.0f;

	//Solo player gets 30% damage reduction
	//Level 80 solo player gets 80% damage reduction
	if (UNITGAME->iBellatraRoomInitialPlayerCount == 1)
	{
		percentReductionTotal = 0.5f + percentReductionLvlBased;
		iFlatDmgDecrease = 10;
	}

	//Duo players gets 20% damage reduction
	else if (UNITGAME->iBellatraRoomInitialPlayerCount == 2)
	{
		percentReductionTotal = 0.3f + percentReductionLvlBased;
		iFlatDmgDecrease = 6;
	}

	//Tri players get 10% damage reduction
	else if (UNITGAME->iBellatraRoomInitialPlayerCount == 3)
	{
		percentReductionTotal = 0.15f + percentReductionLvlBased;
		iFlatDmgDecrease = 3;
	}

	if (percentReductionTotal > 0.0f)
	{
		int iAdjustedDmg = iDamage - static_cast<int>(iReducedDmg * percentReductionTotal);

		iAdjustedDmg = iAdjustedDmg - iFlatDmgDecrease;

		if (iAdjustedDmg < 0)
			iAdjustedDmg = 1;

		return iAdjustedDmg;
	}

	return iDamage;
}

DWORD dwNPCIgnoreClickTime = 0;

void SendNpcUnitClick( UnitData * pcUnitData, int iState  )
{
	if ( pcUnitData &&
		 pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
	{
		if ( ITEMONMOUSE )
			return;

		if ( dwNPCIgnoreClickTime != 0 &&
			PLAYTIME < dwNPCIgnoreClickTime )
		{
			return;
		}

		dwNPCIgnoreClickTime = PLAYTIME + 2000; //2s between NPC clicks

		PacketNPCClick sPacketNPC{};
		sPacketNPC.iHeader = PKTHDR_NpcUnitClick;
		sPacketNPC.iLength = sizeof( PacketNPCClick );
		sPacketNPC.dwTarObjectSerial = pcUnitData->iID;
		sPacketNPC.iNPCId = pcUnitData->sCharacterData.iNPCId;
		sPacketNPC.iQuestData = iState;

		SENDPACKETG( &sPacketNPC );
		return;
	}
}

BOOL SendSingleDamageUnit( UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillCode, BOOL bChance, SocketData * pcSocketData )
{
	if ( pcUnitData &&
		pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
	{
		SendNpcUnitClick( pcUnitData, iState );
		return FALSE;
	}

	int iOldAttackRating = 0;
	int iAttackChance = 0;
	int iValue = 0;
	int iSkillLevel1Based = 0;

	//CHATBOX->AddDebugMessage ( "SendSingleDamageUnit" );

	PacketSingleTargetSkillData sPacket;

	ZeroMemory( &sPacket, sizeof(PacketSingleTargetSkillData) );

	if ( (UNITDATA->iEndFrame - UNITDATA->iStartFrame) <= (160 * 5) )
		return FALSE;

	DAMAGESINGLE_COUNT++;

	BOOL bTestMap = ( GM_MODE && MAP_ID == MAPID_QuestArena );

	//Disallow attack if the attacker is in the safe zone
	if ( MAP_ID == MAPID_BlessCastle &&
		UNITDATA->sPosition.iZ > CastleBattleZoneAttacker_LineZ)
	{
		return FALSE;
	}

	//Disallow attack if the target is in the safe zone
	if ( pcUnitData && pcUnitData->pcMap &&
		pcUnitData->pcMap->pcBaseMap->iMapID == MAPID_BlessCastle &&
		pcUnitData->sPosition.iZ <= CastleBattleZoneDamage_LineZ )
	{
		return FALSE;
	}

	if ( bChance )
	{

		switch ( pcUnitData->sCharacterData.iType )
		{
			case CHARACTERTYPE_Monster:
			{
				BATTLETIME = PLAYTIME;

				//miss chance is calculated on server side for pvp
				if ( pcUnitData->PkMode_CharState != CHARACTERTYPE_Player )
				{
					if ( ( UNITGAME->PlayerAttackChance( pcUnitData->sCharacterData.iDefenseRating ) > Dice::RandomI( 0, 99 ) ) == FALSE )
					{
						DAMAGEINFOHANDLER->RelayInfoToOthers( pcUnitData->iID, EDamageTextType::Defense ); //show 'Defense' for the others
						DAMAGEINFOHANDLER->SetDefenseInfo( EDamageTextType::Miss, 0, pcUnitData->iID, UNITDATA->iID ); //show 'Miss' for the attacker
						return FALSE;
					}
				}

				break;
			}

			//Iron: not sure what this is for.
			case CHARACTERTYPE_NPC:
				if ( ITEMONMOUSE )
					return FALSE;

				iState		= iResistance;
				iResistance = 0;
				break;

			default:
				break;
		}
	}

	AddUnitQueueID( pcUnitData->sCharacterData.iUniqueMonsterID );

	sPacket.iLength = sizeof(PacketSingleTargetSkillData);
	sPacket.iHeader = PKTHDR_SingleTargetSkillData;

	sPacket.sPosition.iX = pcUnitData->sPosition.iX;
	sPacket.sPosition.iY = pcUnitData->sPosition.iY;
	sPacket.sPosition.iZ = pcUnitData->sPosition.iZ;

	sPacket.iAttackRating = UNITDATA->sCharacterData.iAttackRating;

	//CHATBOX->AddDebugMessage ( FormatString ( "Attack rating: %d", sPacket.iAttackRating ) );

	//Send base attack power (before buffs get applied)
	sPacket.AttackPowerMin = CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin;
	sPacket.AttackPowerMax = CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax;

	//these are usually from force orbs or buffs via ASM - display only damage additions
	//sinTempDamage2[0] and sinTempDamage2[1]
	const int sinTempDamage2_Min = ( *(int *)0x035EB600 );
	const int sinTempDamage2_Max = ( *(int *)0x035EB604 );

	//This is final min / max attack power post buff that is being dislayed on client side (only used for checking purposes)
	//This is final min / max attack power post buff that is being dislayed on client side (only used for checking purposes)
	sPacket.iaParametersEx[0] = sinTempDamage2_Min + PLAYERDATA->iMinDamage;
	sPacket.iaParametersEx[1] = sinTempDamage2_Max + PLAYERDATA->iMaxDamage;

	if ( iDamageMin || iDamageMax )
	{
		sPacket.WeaponAttackPowerMin = iDamageMin;
		sPacket.WeaponAttackPowerMax = iDamageMax;
	}
	else
	{
		if ( iSkillCode == 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];
			if ( pcItemData )
			{
				sPacket.WeaponAttackPowerMin = sPacket.AttackPowerMin - pcItemData->sItem.sDamage.sMin;
				sPacket.WeaponAttackPowerMax = sPacket.AttackPowerMax - pcItemData->sItem.sDamage.sMax;
			}
		}
	}

	sPacket.sCriticalChance = UNITDATA->sCharacterData.iCritical;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
		case CHARACTERCLASS_Mechanician:
		case CHARACTERCLASS_Pikeman:
		case CHARACTERCLASS_Knight:
		case CHARACTERCLASS_Assassin:
			sPacket.sPrimaryStats = UNITDATA->sCharacterData.iStrength;
			break;

		case CHARACTERCLASS_Magician:
		case CHARACTERCLASS_Priestess:
		case CHARACTERCLASS_Shaman:
			sPacket.sPrimaryStats = UNITDATA->sCharacterData.iSpirit;
			break;

		case CHARACTERCLASS_Archer:
		case CHARACTERCLASS_Atalanta:
			sPacket.sPrimaryStats = UNITDATA->sCharacterData.iAgility;
			break;

		default:
			break;
	}

	sPacket.eAttackState = (EAttackState)(iState & 0x0000FFFF);
	sPacket.iAttackProperty = (EAttackProperty)(iState & 0xFFFF0000);
	sPacket.iQuestData = iState; //for monsters / npcs that have quest codes associated to them
	sPacket.iRange = 60 << 8;
	sPacket.MotionEventNum = UNITDATA->iMotionEvent;
	sPacket.AttackCount = DAMAGESINGLE_COUNT;

	sPacket.dwTime = TICKCOUNT;
	sPacket.iSkillCode = iSkillCode;

	sPacket.dwWeaponCode = UNITDATA->eWeaponItemID;

	if ( UNITDATA->iLoadedMapIndex >= 0 && pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
		sPacket.StageId = FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID;
	else
		sPacket.StageId = -1;

	sPacket.dwDestObjectSerial = UNITDATA->iID;
	sPacket.dwTarObjectSerial = pcUnitData->iID;
	pcUnitData->iTryAttackCount++;

	sPacket.dwChkSum = CHARACTERGAME->GetDamgeChkSum_S2V( &sPacket ); //CALL_WITH_ARG1( 0x00408280, (DWORD)&sPacket );

	//CHATBOX->AddDebugMessage( FormatString( "dwChkSum = %d", sPacket.dwChkSum ) );

	//Check Damage
	CALL( 0x00408DF0 );

	SocketData * pcSocketSend = SOCKETG;

	if ( UNITDATA != pcUnitData )
		pcSocketSend = GETSERVER_SOCKET( pcUnitData->iServerCode );

	if ( pcSocketData )
		pcSocketSend = pcSocketData;

	if ( pcSocketSend )
	{
		if ( bTestMap )
		{
			TESTMAPHANDLER->OnSingleTargetSkillHit ( &sPacket, &UNITDATA->sActiveSkill );
		}

		//Encode
		CALL_WITH_ARG1( READDWORD( 0x0A04758 ), (DWORD)&sPacket );

		pcSocketSend->SendPacket( &sPacket, FALSE );

		//Show Bar
		CALL_WITH_ARG2( 0x00408340, (DWORD)pcUnitData, iSkillCode );

		ApplyLightingEnchantWeaponAoEOnTarget( pcUnitData );

		return TRUE;
	}

	return FALSE;
}

BOOL ApplyLightingEnchantWeaponAoEOnTarget(UnitData * pcTarget)
{
	if ( UNITDATA == NULL )
		return FALSE;

	if ( pcTarget == NULL )
		return FALSE;

	if ( UNITDATA->iEnchantWeaponLevel <= 0 && UNITDATA->iEnchantWeaponLevel > 10 )
		return FALSE;

	if ( UNITDATA->iEnchantWeaponType != ( EElementID::Lightning + 1 ) )
		return FALSE;

	//Set Units to Area Damage
	int range = SKILLMANAGERHANDLER->GetSkillIntValue( ESkillArrayPointer::Magician_EnchantWeapon_LightningRange_New, UNITDATA->iEnchantWeaponLevel - 1 );

	//max 10 targets
	SKILLMANAGERHANDLER->SelectRangeChained ( UNITDATA, pcTarget, 10, range, range + 20 );

	if ( UNITS_AREA_COUNT > 0 )
	{
		//Skill Impulsion Effect to Targets...
		CALL_WITH_ARG2( 0x005D5410, (DWORD)UNITS_AREA_TARGET, UNITS_AREA_COUNT );

		SKILLMANAGERHANDLER->SendRangeDamage( pcTarget, pcTarget->sPosition, 0, 0, 0, 0, TINYSKILLID_ENCHANT_WEAPON_LIGHTING );

		return TRUE;
	}

	return FALSE;
}

BOOL SendAreaDamageUnit( int iX, int iY, int iZ, UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID )
{
	PacketMultiTargetSkillData * psPacket = (*(PacketMultiTargetSkillData**)0x00A04760);

	if ( psPacket == NULL )
		return FALSE;

	if ( psPacket->iTargetCount == 0 && pcUnitData == NULL )
	{
		ZeroMemory( psPacket, sizeof(PacketMultiTargetSkillData) );
		SAFE_DELETE( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}

	//Disallow attack if the attacker is in the safe zone
	//if ( MAP_ID == MAPID_BlessCastle &&
	//	 UNITDATA->sPosition.iZ <= CastleBattleZone_LineZ)
	//{
	//	CHATBOX->AddDebugMessage( "Disallow attack. I'm in safe zone.." );
	//	return FALSE;
	//}
	//
	////Disallow attack if the target is in the safe zone
	//if ( pcUnitData && pcUnitData->pcMap &&
	//	pcUnitData->pcMap->pcBaseMap->iMapID == MAPID_BlessCastle &&
	//	pcUnitData->sPosition.iZ <= CastleBattleZone_LineZ)
	//{
	//	CHATBOX->AddDebugMessage( "Disallow attack. Target is in safe zone.." );
	//	return FALSE;
	//}

	//disallow players to target friendly players in Bless Castle / PVP otherwise they could spam AoE at gate
	if ( ( RESTARTCOUNTER != 0 ) ||
		 ( (MAP_ID == MAPID_BlessCastle || MAP_ID == MAPID_BattleTown) && UNITGAME->IsBlockedRangeTarget () == TRUE ) )
	{
		ZeroMemory( psPacket, sizeof(PacketMultiTargetSkillData) );
		SAFE_DELETE( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}

	if ( (UNITDATA->iEndFrame - UNITDATA->iStartFrame) <= (160 * 5) )
	{
		ZeroMemory( psPacket, sizeof( PacketMultiTargetSkillData ) );
		SAFE_DELETE( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}


	DAMAGESINGLE_COUNT++;

	psPacket->iLength = (sizeof(PacketMultiTargetSkillData) - (sizeof( int ) * 128));
	psPacket->iLength += (sizeof( int ) * psPacket->iTargetCount) + 16;
	psPacket->iHeader = PKTHDR_MultiTargetSkillData;
	psPacket->sPosition.iX = iX;
	psPacket->sPosition.iY = iY;
	psPacket->sPosition.iZ = iZ;
	psPacket->iDamageCount = DAMAGESINGLE_COUNT;
	psPacket->dwTime = TICKCOUNT;
	psPacket->iDamageState = iState;

	if ( pcUnitData )
	{
		if ( PRECISIONTARGET_RANGE )
		{
			for ( int i = 0; i < psPacket->iTargetCount; i++ )
			{
				if ( psPacket->iaTargetID[i] == pcUnitData->iID )
				{
					psPacket->iTargetID = pcUnitData->iID;
					break;
				}
			}
		}
		else
			psPacket->iTargetID = pcUnitData->iID;

		pcUnitData->iTryAttackCount++;
	}

	psPacket->sAttackRating = PLAYERDATA->iAttackRating;
	psPacket->iSkillID = iSkillID;
	psPacket->sCriticalChance = PLAYERDATA->iCritical;
	psPacket->iTargetReceiveDamageID = UNITDATA->iID;

	//Send base attack power (before buffs get applied)
	psPacket->sAttackPower.sMin = CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMin;
	psPacket->sAttackPower.sMax = CHARACTERGAME->sCharacterDataEx.iBaseAttackPowerMax;

	//these are usually from force orbs or damage buffs (usually timers via ASM) - display only damage additions
	//sinTempDamage2[0] and sinTempDamage2[1]
	const int sinTempDamage2_Min = ( *(int *)0x035EB600 );
	const int sinTempDamage2_Max = ( *(int *)0x035EB604 );

	//This is final min / max attack power post buff that is being dislayed on client side (only used for checking purposes)
	psPacket->iaParametersEx[0] = sinTempDamage2_Min + PLAYERDATA->iMinDamage;
	psPacket->iaParametersEx[1] = sinTempDamage2_Max + PLAYERDATA->iMaxDamage;

	psPacket->WeaponAttackPower[0] = iDamageMin;
	psPacket->WeaponAttackPower[1] = iDamageMax;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
		case CHARACTERCLASS_Mechanician:
		case CHARACTERCLASS_Pikeman:
		case CHARACTERCLASS_Assassin:
		case CHARACTERCLASS_Knight:
			psPacket->sPrimaryStats = PLAYERDATA->iStrength;
			break;

		case CHARACTERCLASS_Magician:
		case CHARACTERCLASS_Priestess:
		case CHARACTERCLASS_Shaman:
			psPacket->sPrimaryStats = PLAYERDATA->iSpirit;
			break;

		case CHARACTERCLASS_Archer:
		case CHARACTERCLASS_Atalanta:
			psPacket->sPrimaryStats = PLAYERDATA->iAgility;
			break;

		default:
			break;
	}

	psPacket->iRange = 320;
	psPacket->sAttackRange = static_cast<short>(PLAYERDATA->iAttackRange);
	if ( psPacket->sAttackRange > 400 )
		psPacket->sAttackRange = 400;

#ifdef SKILLS_BEHAVIOUR_DEBUG
	CHATBOX->AddDebugMessage ( FormatString("Attack Range = %d, # targets = %d", PLAYERDATA->iAttackRange, psPacket->iTargetCount) );
#endif
	//CHATBOX->AddDebugMessage ( FormatString("Enchant lvl = %d, %d, type = %d", UNITDATA->iEnchantWeaponLevel, READDWORD( (&UNITDATA + 4680)), UNITDATA->iEnchantWeaponType) );

	psPacket->dwWeaponCode = UNITDATA->eWeaponItemID;

	if ( UNITDATA->iLoadedMapIndex >= 0 )
		psPacket->sMapID = FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID;

	psPacket->dwChkSum = CALL( 0x00418DD0 );

	if ( GM_MODE && MAP_ID == MAPID_QuestArena )
	{
		TESTMAPHANDLER->OnMultiTargetSkillHit ( psPacket, &UNITDATA->sActiveSkill );
	}
	//Send
	CALL( 0x00408DF0 );

	SocketData * pcSocketSend = (SocketData*)CALL( 0x0061EB70 );

	if ( pcSocketSend && psPacket->iTargetCount > 0 )
	{
		pcSocketSend->SendPacket( psPacket, FALSE );

		CALL_WITH_ARG1( 0x00408380, psPacket->iSkillID );
	}

	ZeroMemory( psPacket, sizeof(PacketMultiTargetSkillData) );
	SAFE_DELETE( psPacket );
	WRITEDWORD( 0x00A04760, 0 );

	return TRUE;
}

BOOL FreeUnitData( UnitData * pcUnitData )
{
	auto DeleteModelData = []( EXEModelData * ps )->void
	{
		CALL_WITH_ARG1( 0x0045A5F0, (DWORD)ps );
	};

	pcUnitData->bActive = FALSE;

	pcUnitData->iClanInfoIndex = (-1);

	if ( pcUnitData->pcBody )
	{
		DeleteModelData( pcUnitData->pcBody );
		pcUnitData->pcBody = NULL;
	}

	if ( pcUnitData->pcHead )
	{
		DeleteModelData( pcUnitData->pcHead );
		pcUnitData->pcHead = NULL;
	}

	if ( pcUnitData->pcLeftHandTool )
	{
		DeleteModelData( pcUnitData->pcLeftHandTool );
		pcUnitData->pcLeftHandTool = NULL;
	}

	if ( pcUnitData->pcRightHandTool )
	{
		DeleteModelData( pcUnitData->pcRightHandTool );
		pcUnitData->pcRightHandTool = NULL;
	}

	pcUnitData->pcBodyMeshContainer = NULL;
	pcUnitData->pcHeadMeshContainer = NULL;

	pcUnitData->sLeftHandTool.pModel = NULL;
	pcUnitData->sRightHandTool.pModel = NULL;
	pcUnitData->sLeftHandTool.eItemID = ITEMID_None;
	pcUnitData->sRightHandTool.eItemID = ITEMID_None;

	pcUnitData->psBody = NULL;
	pcUnitData->psHead = NULL;

	pcUnitData->dwDispAppSkill = 0;

	if ( pcUnitData->psaDamageUsersData )
	{
		SAFE_DELETE_ARRAY( pcUnitData->psaDamageUsersData );
	}

	if ( pcUnitData->pcaUnitDataTargetList )
	{
		SAFE_DELETE_ARRAY( pcUnitData->pcaUnitDataTargetList );
	}

	if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->PkMode_CharState == CHARACTERTYPE_Player) )
		pcUnitData->psModelAnimation = ITEMHANDLER->GetModelDefault( pcUnitData->sCharacterData.iClass );

	return TRUE;
}

void RenderBlessCastleCrown( UnitData * pcUnitData )
{
	Point3D sCameraPosition;
	sCameraPosition.iX = UNITTARGETPOSITION_X;
	sCameraPosition.iY = UNITTARGETPOSITION_Y;
	sCameraPosition.iZ = UNITTARGETPOSITION_Z;

	Point3D sCameraAngle;
	sCameraAngle.iX = UNITTARGETANGLE_X;
	sCameraAngle.iY = UNITTARGETANGLE_Y;
	sCameraAngle.iZ = UNITTARGETANGLE_Z;

	BOOL bLeader = STRINGCOMPAREI( ((char *)0x03B36C10), pcUnitData->sCharacterData.szName );

	EXEModel * pcBlessCastleCrownModel = BLESSCASTLEHANDLER->GetCrownModelByClanID( pcUnitData->sCharacterData.iClanID );

	if ( pcUnitData->sCharacterData.iClanID && pcBlessCastleCrownModel && pcUnitData->bRendered && (pcUnitData->sRenderPoint.iZ < (12 * 64 * 256)) && (pcUnitData->iVanishTimeLeft <= 0) )
	{
		GetMoveLocation( 0, 0, 0, 0, pcUnitData->sAngle.iY, 0 );

		Point3D sCrownPosition;
		sCrownPosition.iX = pcUnitData->sPosition.iX + RADIAN3D_X;
		sCrownPosition.iY = pcUnitData->sPosition.iY + pcUnitData->iModelHeight + 11 * 256;
		sCrownPosition.iZ = pcUnitData->sPosition.iZ + RADIAN3D_Z;

		int cnt = (TICKCOUNT >> 2) & 0xFF;

		sCrownPosition.iY += (cnt & 0x80) ? (cnt & 0x7F) : (0x80 - (cnt & 0x7F));

		Point3D sCrownAngle;
		sCrownAngle.iX = 0;
		sCrownAngle.iY = (TICKCOUNT >> 2) & 4095;
		sCrownAngle.iZ = 0;

		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		RENDERER->SetTextureManager( EXETEXTUREMANAGER_DEFAULT );

		EXEModel * pcCrownModel = pcBlessCastleCrownModel; //bLeader ? BLESSCASTLE_BLUE_CROWN_MODEL : BLESSCASTLE_GOLDEN_CROWN_MODEL;
		if ( pcCrownModel )
		{
			pcCrownModel->SetPosition( &sCrownPosition, &sCrownAngle );
			pcCrownModel->Render( &sCameraPosition, &sCameraAngle );
		}

		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
}



/*
// limite quest
#define MAX_QUEST 113

struct ListNPC
{
	int			IDQuest;
	char        szNPCName[32];
	char		szNPCProgressName[32];
	char		szNPCCompleteName[32];
	int			szQuestLevel;
	int			szQuestClass;
	//char		szJobRestriction[32] = { 0 };
};
};*/

///QUEST LIST

const ListNPC saListNPC[MAX_QUEST] =
{
	//ID QUEST  | NPC PICK QUEST | NPC END QUEST ONLY QUEST ANNIER | NPC QUEST LEVEL
	// BONUS - Quests
	// Boss Hunter William
	// set quest level
{ 9016  , "Instructor Annie" , "Instructor Annie" , "Boss Hunter, William", 0},
{ 8017	, "Instructor Millie" , "Instructor Millie" ,  "Boss Hunter, William", 0},

{ 7000  , "Rosa" , "Rosa" , "Rosa", 55},
{ 7005  , "Summer Girl" , "Summer Girl" , "Summer Girl", 1},
//{ 7001  , "Rosa" , "Rosa" , "Rosa", 55},
//{ 7002  , "Rosa" , "Rosa" , "Rosa", 55},
//{ 7003  , "Rosa" , "Rosa" , "Rosa", 55},
//{ 7004  , "Rosa" , "Rosa" , "Rosa", 55},

//Challenge quests
{ 6000  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 0},
{ 6002  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 5},
{ 6003  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 10},
{ 6004  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 20},
{ 6005  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 25},
{ 6006  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 25},
{ 6007  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 30},
{ 6008  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 40},
{ 6009  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 40},
{ 6010  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 50},
{ 6011  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 55},
{ 6012  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 55},
{ 6013  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 60},
{ 6014  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 65},
{ 6015  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 70},
{ 6019  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 75},
{ 6016  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 80},
{ 6017  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 90},
{ 6020  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 95},
{ 6018  , "Boss Hunter, William" , "Boss Hunter, William" , "Boss Hunter, William", 100},

//CHARACTERRACE_Tempskron - Quest Chain CHARACTERCLASS_Pikeman || CHARACTERCLASS_Fighter || CHARACTERCLASS_Archer || CHARACTERCLASS_Assassin || CHARACTERCLASS_Mechanician
//Beginner
{ 9008	, "Instructor Annie" , "Instructor Annie" , "Blacksmith Gus" , 1, CHARACTERRACE_Tempskron},
{ 9009	, "Instructor Annie" , "Instructor Annie" , "Miscellaneous Store, Namy" ,1, CHARACTERRACE_Tempskron},
{ 9010	, "Instructor Annie" , "Instructor Annie" , "Warehouse keeper, Arent",1, CHARACTERRACE_Tempskron},
{ 9011	, "Instructor Annie" , "Instructor Annie" , "Inventor of Warpgate", 1, CHARACTERRACE_Tempskron},
{ 9012	, "Instructor Annie" , "Instructor Annie" , "Blacksmith Goden" , 1, CHARACTERRACE_Tempskron},
{ 9013	, "Instructor Annie" , "Instructor Annie" , "Instructor Annie", 1, CHARACTERRACE_Tempskron},
{ 9014	, "Instructor Annie" , "Instructor Annie" , "Instructor Annie" , 1, CHARACTERRACE_Tempskron},
{ 9000	, "Instructor Annie" , "Instructor Annie" , "Instructor Annie" , 1, CHARACTERRACE_Tempskron},
{ 9001	, "Instructor Annie" , "Instructor Annie" , "Instructor Annie" , 1, CHARACTERRACE_Tempskron},
//Novice
{ 9017	, "Instructor Annie" , "Instructor Annie" ,  "Ruinen Kiosk", 15, CHARACTERRACE_Tempskron},
{ 9015	, "Instructor Annie" , "Instructor Annie" ,  "Inventor of Warpgate", 10, CHARACTERRACE_Tempskron},
{ 9002	, "Instructor Annie" , "Instructor Annie" ,  "Skill Master, Verkan", 10, CHARACTERRACE_Tempskron},
{ 9003	, "Instructor Annie" , "Instructor Annie" ,  "Instructor Annie", 10, CHARACTERRACE_Tempskron},
{ 9004	, "Instructor Annie" , "Instructor Annie" ,  "Instructor Annie", 10, CHARACTERRACE_Tempskron},
{ 9005	, "Instructor Annie" , "Instructor Annie" ,  "Instructor Annie" , 10, CHARACTERRACE_Tempskron},
{ 9006	, "Instructor Annie" , "Instructor Annie" ,  "Instructor Annie" , 10, CHARACTERRACE_Tempskron},
{ 9007	, "Instructor Annie" , "Instructor Annie" ,  "Instructor Annie" , 15, CHARACTERRACE_Tempskron},

//Medium
//High
//Expert

//Morion - Quest Chain
//beginner
{ 8009	, "Instructor Millie" , "Instructor Millie" ,  "Blacksmith Bartz", 1, CHARACTERRACE_Morion},
{ 8010	, "Instructor Millie" , "Instructor Millie" ,  "Miscellaneous Store, Phillya" , 1, CHARACTERRACE_Morion},
{ 8011	, "Instructor Millie" , "Instructor Millie" ,  "Warehouse keeper, Lavendar", 1, CHARACTERRACE_Morion},
{ 8012	, "Instructor Millie" , "Instructor Millie" ,  "Inventor of Warpgate", 1, CHARACTERRACE_Morion},
{ 8013	, "Instructor Millie" , "Instructor Millie" ,  "Blacksmith Defrain", 1, CHARACTERRACE_Morion},
{ 8015	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 1, CHARACTERRACE_Morion},
{ 8016	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 1, CHARACTERRACE_Morion},
{ 8018	, "Instructor Millie" , "Instructor Millie" ,  "Ruinen Kiosk", 15, CHARACTERRACE_Morion},
{ 8000	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 1, CHARACTERRACE_Morion},
{ 8001	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 1, CHARACTERRACE_Morion},
//Novice
{ 8002	, "Instructor Millie" , "Instructor Millie" ,  "Witchcraft Academy, Raymon", 10, CHARACTERRACE_Morion},
{ 8003	, "Instructor Millie" , "Instructor Millie" ,  "Inventor of Warpgate", 10, CHARACTERRACE_Morion},
{ 8004	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 10, CHARACTERRACE_Morion},
{ 8005	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 10, CHARACTERRACE_Morion},
{ 8006	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 10, CHARACTERRACE_Morion},
{ 8007	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 10, CHARACTERRACE_Morion},
{ 8008	, "Instructor Millie" , "Instructor Millie" ,  "Instructor Millie", 10, CHARACTERRACE_Morion},

//Phillai
//{ 1990	, "Molly, the granny" , "Molly, the granny" ,  "Albus P. W. B. Dumbledore", 1},
{ 1501	, "Warehouse keeper, Lavendar" , "Warehouse keeper, Lavendar" , "Warehouse keeper, Lavendar", 40},
//RICARTEN
//{ 1991	, "Molly, the granny" , "Molly, the granny" , "Molly, the granny", 1},
//RUINEN
{ 1652	, "Lost Wanderer" , "Blacksmith Ruga" , "Blacksmith Ruga", 15},
{ 3000	, "Ruinen Quest Master" , "Ruinen Quest Master" , "Ruinen Quest Master", 15},
{ 3001	, "Ruinen Quest Master" , "Ruinen Quest Master" , "Ruinen Quest Master", 15},
{ 3002	, "Ruinen Quest Master" , "Ruinen Quest Master" , "Ruinen Quest Master", 15},
{ 3003	, "Ruinen Quest Master" , "Ruinen Quest Master" , "Ruinen Quest Master", 15},
{ 3074	, "Ruinen Quest Master" , "Ruinen Quest Master" , "Ruinen Quest Master", 15}, //group
//CURSED LAND
{ 3004	, "Cursed Land Quest Master" , "Cursed Land Quest Master" , "Cursed Land Quest Master", 25},
{ 3005	, "Cursed Land Quest Master" , "Cursed Land Quest Master" , "Cursed Land Quest Master", 25},
{ 3006	, "Cursed Land Quest Master" , "Cursed Land Quest Master" , "Cursed Land Quest Master", 25},
{ 3007	, "Cursed Land Quest Master" , "Cursed Land Quest Master" , "Cursed Land Quest Master", 25},
{ 3075	, "Cursed Land Quest Master" , "Cursed Land Quest Master" , "Cursed Land Quest Master", 25}, //group

//DUNGEON 1
{ 3011	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 },
{ 3012	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 },
{ 3013	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 },
{ 3019	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 },
{ 3020	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 },
{ 3078	, "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master" , "Dungeon 1 Quest Master", 40 }, //group

//DUNGEON 2
{ 3021	, "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 },
{ 3022	, "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 },
{ 3023	, "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 },
{ 3024  , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 },
{ 3089	, "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 },
{ 3079	, "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master" , "Dungeon 2 Quest Master", 45 }, //group

//DUNGEON 3
{ 3090  , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 },
{ 3091  , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 },
{ 3030  , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 },
{ 3031  , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 },
{ 3032	, "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 },
{ 3080	, "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master" , "Dungeon 3 Quest Master", 60 }, //group

//NAVISKO
{ 3008	, "Navisko Quest Master" , "Navisko Quest Master" , "Navisko Quest Master", 30},
{ 3009	, "Navisko Quest Master" , "Navisko Quest Master" , "Navisko Quest Master", 30},
{ 3010	, "Navisko Quest Master" , "Navisko Quest Master" , "Navisko Quest Master", 30},
{ 3014	, "Navisko Quest Master" , "Navisko Quest Master" , "Navisko Quest Master", 30},
{ 3076	, "Navisko Quest Master" , "Navisko Quest Master" , "Navisko Quest Master", 30 },//group

//OASIS
{ 3015	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },
{ 3016	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },
{ 3017	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },
{ 3018	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },
{ 3088	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },
{ 3077	, "Oasis Quest Master" , "Oasis Quest Master" , "Oasis Quest Master", 35 },//group

//Battlefield of the Ancients
{ 3114	, "Battlefield Quest Master" , "Battlefield Quest Master" , "Battlefield Quest Master", 50 },
{ 3115	, "Battlefield Quest Master" , "Battlefield Quest Master" , "Battlefield Quest Master", 50 },
{ 3116	, "Battlefield Quest Master" , "Battlefield Quest Master" , "Battlefield Quest Master", 50 },
{ 3117	, "Battlefield Quest Master" , "Battlefield Quest Master" , "Battlefield Quest Master", 50 },

//Forbidden Land


//Medium

//High
//Expert

//Lone Wolf
{ 3069	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 15 }, //Ruinen Village
{ 3070	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 25 }, //Cursed Land
{ 3071	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 30 }, //Forgotten Lands
{ 3072	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 35 }, //Oasis
{ 3068	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 40 }, //Ancient Prison 1
{ 3060	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 50 }, //Ancient Prison 2
{ 3061	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 55 }, //Beehive cave
{ 3062	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 60 }, //Ancient Prison 3
{ 3063	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 65 }, //Dark Sanctuary
{ 3064	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 70 }, //Cursed Temple 1
{ 3065	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 75 }, //Cursed Temple 2
{ 3066	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 80 }, //Railroad of Chaos
{ 3067	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 85 }, //Heart of Perum
{ 3073	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 90 }, //Gallubia Valley
{ 3104	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 95 }, //Land of Chaos
{ 3112	, "The Lone Wolf" , "The Lone Wolf" , "The Lone Wolf", 98 }, //Lost Temple


//iron1
{ 3048 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3049 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3050 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3051 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3052 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3053 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 },
{ 3085 , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master" , "Railroad of Chaos Quest Master", 80 }, //group

//iron2
{ 3054 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3055 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3056 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3057 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3058 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3059 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 },
{ 3086 , "Heart of Perum Quest Master" , "Heart of Perum Quest Master" , "Heart of Perum Quest Master", 85 }, //group

//Cursed Temple 1
{ 3038 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 },
{ 3039 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 },
{ 3040 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 },
{ 3041 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 },
{ 3042 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 },
{ 3083 , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master" , "Cursed Temple 1 Quest Master", 80 }, //group

//Cursed Temple 2
{ 3043 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 },
{ 3044 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 },
{ 3045 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 },
{ 3046 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 },
{ 3047 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 },
{ 3084 , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master" , "Cursed Temple 2 Quest Master", 85 }, //group

//Dark Sanc
{ 3033 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },
{ 3034 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },
{ 3035 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },
{ 3036 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },
{ 3037 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },
{ 3082 , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master" , "Dark Sanctuary Quest Master", 65 },

//Beehive Cave
{ 3025 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 },
{ 3026 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 },
{ 3027 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 },
{ 3028 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 },
{ 3029 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 },
{ 3081 , "Cave Guard" , "Cave Guard" , "Cave Guard", 55 }, //group

//Ice 1
{ 3092 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3093 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3094 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3095 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3096 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3097 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 },
{ 3087 , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master" , "Gallubia Valley Quest Master", 90 }, //group


//LoC
{ 3098 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3099 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3100 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3101 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3102 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3103 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 },
{ 3105 , "Land of Chaos Quest Master" , "Land of Chaos Quest Master" , "Land Of Chaos Quest Master", 95 }, //group

//LT
{ 3106 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3107 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3108 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3109 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3110 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3111 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 },
{ 3113 , "Lost Temple Quest Master" , "Lost Temple Quest Master" , "Lost Temple Quest Master", 98 }, //group


};

//CHECK NPC QUEST XXSTR
int QuestNPCCheck(char * NameNPC, int PlayerLVL)
{
	for (int iDQuest = 1500; iDQuest < 7006; iDQuest++)

	if (CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest) && PlayerLVL >= 2)
	{
		if (!CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest))
		{
			for (int i = 0; i < MAX_QUEST; i++)
			{
				if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCProgressName) == 0)
					return 2; // 2 = Grijs ?
				if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 0;
				//}
			}
		}
		// Else Show Completion
		else if (CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest) && PlayerLVL >= 2)
		{
			for (int i = 0; i < MAX_QUEST; i++)
			{
				if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCCompleteName) == 0)
					return 3; // 2 = Geel ?

				if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 0;
				//}
			}
		}
	}

	// If not active Show Quest
	for (int iDQuest = 1500; iDQuest < 7006; iDQuest++)

	if (!CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest) && (!CQUESTGAME->GetQuestInstance()->IsDoneQuest(iDQuest) && PlayerLVL >= 2))
	{
		for (int i = 0; i < MAX_QUEST; i++)
		{
			if (PlayerLVL >= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
				return 1; // return 1 = yellow !
			else if
				(PlayerLVL == saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
				return 1; // return 1 = yellow !
			else if
				(PlayerLVL <= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
				return 4; // return 4 = gray !
		}
	}

	//Morion
	switch (UNITDATA->sCharacterData.iClass)
	{
	case CHARACTERCLASS_Magician:
	case CHARACTERCLASS_Priestess:
	case CHARACTERCLASS_Shaman:
	case CHARACTERCLASS_Knight:
	case CHARACTERCLASS_Atalanta:

	for (int iDQuest = 8000; iDQuest < 8019; iDQuest++)

		if (CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest))
		{
			if (!CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest))
			{
				for (int i = 0; i < MAX_QUEST; i++)
				{
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCProgressName) == 0)
						return 2; // 2 = Grijs ?
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
						return 0;
					//}
				}
			}
			// Else Show Completion
			else if (CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest))
			{
				for (int i = 0; i < MAX_QUEST; i++)
				{
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCCompleteName) == 0)
						return 3; // 2 = Geel ?

					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
						return 0;
					//}
				}
			}
		}

	// If not active Show Quest
	for (int iDQuest = 8000; iDQuest < 8019; iDQuest++)
		if (!CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest) && (!CQUESTGAME->GetQuestInstance()->IsDoneQuest(iDQuest)))
		{
			for (int i = 0; i < MAX_QUEST; i++)
			{
				if (PlayerLVL >= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 1; // return 1 = yellow !
				else if
					(PlayerLVL == saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 1; // return 1 = yellow !
				else if
					(PlayerLVL <= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 4; // return 4 = gray !
			}
		}
	}

	switch (UNITDATA->sCharacterData.iClass)
	{
	case CHARACTERCLASS_Pikeman:
	case CHARACTERCLASS_Archer:
	case CHARACTERCLASS_Fighter:
	case CHARACTERCLASS_Mechanician:
	case CHARACTERCLASS_Assassin:

	for (int iDQuest = 9000; iDQuest < 9018; iDQuest++)

		if (CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest))
		{
			if (!CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest))
			{
				for (int i = 0; i < MAX_QUEST; i++)
				{
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCProgressName) == 0)
						return 2; // 2 = Grijs ?
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
						return 0;
					//}
				}
			}
			// Else Show Completion
			else if (CQUESTGAME->GetQuestInstance()->IsFinishedQuestKills(iDQuest))
			{
				for (int i = 0; i < MAX_QUEST; i++)
				{
					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCCompleteName) == 0)
						return 3; // 2 = Geel ?

					if (saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
						return 0;
					//}
				}
			}
		}

	// If not active Show Quest
	for (int iDQuest = 9000; iDQuest < 9018; iDQuest++)
		if (!CQUESTGAME->GetQuestInstance()->IsActiveQuest(iDQuest) && (!CQUESTGAME->GetQuestInstance()->IsDoneQuest(iDQuest)))
		{
			for (int i = 0; i < MAX_QUEST; i++)
			{
				if (PlayerLVL >= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 1; // return 1 = yellow !
				else if
					(PlayerLVL == saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 1; // return 1 = yellow !
				else if
					(PlayerLVL <= saListNPC[i].szQuestLevel && saListNPC[i].IDQuest == iDQuest && strcmp(NameNPC, saListNPC[i].szNPCName) == 0)
					return 4; // return 4 = gray !
			}
		}
	}
	return 0;
}