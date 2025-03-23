#include "stdafx.h"
#include "EXEUglyRefactor.h"


NAKED bool __cdecl AssaAddFace2D( EXEFace2D * face, int ZPosiFlag = FALSE, int angle = 0 )
{
	JMP( 0x0053F500 );
}

ForceOrbEffectFollow::ForceOrbEffectFollow()
{
	MaterialNum = 0;
	MyCharacterFlag = FALSE;
}

ForceOrbEffectFollow::~ForceOrbEffectFollow()
{
}

NAKED void ForceOrbEffectFollow::Start( UnitData * pCharacter, int timeCount, int shellTomCode )
{
	JMP( 0x005516A0 );
	/*
	pcUnitData = pCharacter;
	if ( pcUnitData == NULL )
		return;

	if ( UNITDATA == pcUnitData )
	{
		MyCharacterFlag = TRUE;
	}

	EndFlag = FALSE;
	iTimeMax = timeCount;

	int * matNum = (int *)CALL_WITH_ARG2( 0x0053F1B0, (DWORD)(char *)"flare.tga", 4 );
	if ( matNum == NULL )
		return;

	if ( shellTomCode == 0 )
	{
		sFace.iRed = 80;
		sFace.iGreen = 20;
		sFace.iBlue = 150;
	}
	else if ( shellTomCode == 1 ) 
	{
		sFace.iRed = 80;
		sFace.iGreen = 20;
		sFace.iBlue = 150;
	}
	else if ( shellTomCode == 2 )
	{
		sFace.iRed = 80;
		sFace.iGreen = 20;
		sFace.iBlue = 150;
	}
	else if ( shellTomCode == 3 )
	{
		sFace.iRed = 80;
		sFace.iGreen = 20;
		sFace.iBlue = 150;
	}
	else if ( shellTomCode == 4 )
	{
		sFace.iRed = 140;
		sFace.iGreen = 90;
		sFace.iBlue = 20;
	}
	else if ( shellTomCode == 5 ) 
	{
		sFace.iRed = 20;
		sFace.iGreen = 100;
		sFace.iBlue = 20;
	}
	else if ( shellTomCode == 6 )
	{
		sFace.iRed = 0;
		sFace.iGreen = 100;
		sFace.iBlue = 200;
	}
	else if ( shellTomCode == 7 )
	{
		sFace.iRed = 190;
		sFace.iGreen = 30;
		sFace.iBlue = 150;
	}
	else if ( shellTomCode == 8 )
	{
		sFace.iRed = 255;
		sFace.iGreen = 40;
		sFace.iBlue = 20;
	}
	else if ( shellTomCode == 9 )
	{
		sFace.iRed = 220;
		sFace.iGreen = 500;
		sFace.iBlue = 10;
	}
	// 박재원 : 빌링 포스 추가(무기 이펙트 조정)
	else if ( shellTomCode == 10 ) //인페르나
	{
		sFace.iRed = 10;
		sFace.iGreen = 255;
		sFace.iBlue = 10;
	}
	// 박재원 : 빌링 포스 추가(무기 이펙트 조정)
	else if ( shellTomCode == 11 ) //이니그마
	{
		sFace.iRed = 255;
		sFace.iGreen = 255;
		sFace.iBlue = 255;
	}

	sFace.iMaterialID = *matNum;
	sFace.iTransparency = 120;
	sFace.iWidth = 15 * fONE;
	sFace.iHeight = 15 * fONE;
	sFace.sTextureRect.iRight = 1;
	sFace.sTextureRect.iBottom = 1;

	Width = 14 * fONE;
	ScaleWidth = 0;
	ScaleFlag = TRUE;
	memset( WeaponPos, 0, sizeof( WeaponPos ) );
	WeaponPosCount = 3;

	GlowWidth = 60 * fONE;
	GlowFlag = FALSE;

	GlowFace.iMaterialID = *matNum;
	GlowFace.iRed = sFace.iRed;
	GlowFace.iGreen = sFace.iGreen;
	GlowFace.iBlue = sFace.iBlue;
	GlowFace.iTransparency = 160;
	GlowFace.iWidth = GlowWidth;
	GlowFace.iHeight = GlowWidth;

	GlowFace.sTextureRect.iLeft = 0;
	GlowFace.sTextureRect.iTop = 0;
	GlowFace.sTextureRect.iRight = 1;
	GlowFace.sTextureRect.iBottom = 1;

	dwID = 1407;
	*/
}

void ForceOrbEffectFollow::Main()
{
	vWeaponsPosition.clear();

	if ( MyCharacterFlag )
	{
		if ( UNITDATA != pcUnitData )
			pcUnitData = UNITDATA;
	}

	if ( iTime >= (iTimeMax - 5) )
		EndFlag = TRUE;
	if ( EndFlag )
	{
		iTime = iTimeMax;
		return;
	}


	if ( ScaleFlag )
		ScaleWidth += 100;
	else
		ScaleWidth -= 100;

	if ( ScaleWidth >= fONE && ScaleFlag == TRUE )
		ScaleFlag = FALSE;
	if ( ScaleWidth <= -fONE && ScaleFlag == FALSE )
		ScaleFlag = TRUE;

	memset( WeaponPos, 0, sizeof( WeaponPos ) );
	int index = 0;
	int StartZPos = 0;
	int WeaponSize = 0;

	GlowFlag = FALSE;

	EXEMatrixI & sMatrixItem = pcUnitData->sRightHandMatrix;
	UnitTool & sToolItem = pcUnitData->sRightHandTool;

	EItemID eCurrentItemID = pcUnitData->sRightHandTool.eItemID;
	EItemID eLeftItemID = pcUnitData->sLeftHandTool.eItemID;

	EItemType eCurrentItemType = ItemID( eCurrentItemID ).ToItemType();

	std::vector<EXEMatrixI> vWeaponsMatrix;

	if ( pcUnitData->sRightHandTool.eItemID != ITEMID_None )
	{
		vWeaponsMatrix.push_back( pcUnitData->sRightHandMatrix );

		WeaponSize = sToolItem.iSizeMax;

		if ( eCurrentItemType == ITEMTYPE_Javelin )
		{
			StartZPos = -(sToolItem.iSizeMax - fONE * 4);
			WeaponSize = int( (float)sToolItem.iSizeMax * 1.5f );
			Width = 12 * fONE;
			WeaponPosCount = WeaponSize / 1000;


			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;
		}
		else if ( eCurrentItemType == ITEMTYPE_Claw )
		{
			StartZPos = 0;
			WeaponSize = sToolItem.iSizeMax * 3;
			Width = 15 * fONE;
			WeaponPosCount = WeaponSize / 1000;

			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iX = sToolItem.iSizeMax + index * 1000;
		}
		else if ( eCurrentItemType == ITEMTYPE_Wand )
		{
			StartZPos = 0;
			WeaponPosCount = WeaponSize / 1000;
			GlowWidth = 35 * fONE;
			GlowFlag = TRUE;

			Width = 12 * fONE;
			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;

			int glowPosIndex = (WeaponSize - int( (float)WeaponSize * 0.2f )) / 1000;
			if ( glowPosIndex >= MAX_WEAPON_FORCE_POS )
				glowPosIndex = MAX_WEAPON_FORCE_POS - 1;
			GlowPosIndex = glowPosIndex;
		}
		else if ( eCurrentItemType == ITEMTYPE_Bow )//Bow shooting
		{
			WeaponPosCount = (sToolItem.iSizeMax + sToolItem.iSizeMax / 2) / 1000;
			if ( (eCurrentItemID & 0xFFFFFF00) == 0x1100 )
				WeaponPosCount = 7;

			int cnt;
			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
			{
				WeaponPos[index].iZ = index * 1000;

				cnt = (PTANGLE_45)&PTANGLE_Mask;
				GetMoveLocation( WeaponPos[index].iX, WeaponPos[index].iY, WeaponPos[index].iZ, 0, cnt, 0 );

				WeaponPos[index].iX = RADIAN3D_X;
				WeaponPos[index].iY = RADIAN3D_Y;
				WeaponPos[index].iZ = RADIAN3D_Z;
			}
		}
		else if ( eCurrentItemType == ITEMTYPE_Hammer )//Hammer
		{
			StartZPos = 0;
			Width = 12 * fONE;
			WeaponPosCount = WeaponSize / 1000;
			GlowWidth = 40 * fONE;
			GlowFlag = TRUE;
			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;

			int glowPosIndex = (WeaponSize - int( (float)WeaponSize * 0.2f )) / 1000;
			if ( glowPosIndex >= MAX_WEAPON_FORCE_POS )
				glowPosIndex = MAX_WEAPON_FORCE_POS - 1;

			GlowPosIndex = glowPosIndex;
		}
		else if ( eCurrentItemType == ITEMTYPE_Axe )//Axe
		{
			StartZPos = 0;
			Width = 12 * fONE;
			WeaponPosCount = WeaponSize / 1000;
			GlowWidth = 45 * fONE;
			GlowFlag = TRUE;
			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;

			int glowPosIndex = (WeaponSize - int( (float)WeaponSize * 0.2f )) / 1000;
			if ( glowPosIndex >= MAX_WEAPON_FORCE_POS )
				glowPosIndex = MAX_WEAPON_FORCE_POS - 1;

			GlowPosIndex = glowPosIndex;

			if ( (eCurrentItemID & 0xFFFFFF00) == 0x0E00 )
			{
				int cnt;
				int sin;
				int cos;

				cnt = (-PTANGLE_45 / 5) & PTANGLE_Mask;

				sin = PTSEN[cnt] >> 8;
				cos = PTCOS[cnt] >> 8;

				for ( int index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				{
					WeaponPos[index].iY = -(WeaponPos[index].iZ * sin) >> 8;
					WeaponPos[index].iZ = (WeaponPos[index].iZ * cos) >> 8;
				}
			}
		}
		else if ( eCurrentItemType == ITEMTYPE_Scythe )
		{
			StartZPos = 0;
			WeaponPosCount = (WeaponSize - 4500) / 1000;

			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;
		}
		else if ( eCurrentItemType == ITEMTYPE_Dagger )
		{
			if ( pcUnitData->eToolEquipModeTown == TOOLEQUIPMODETOWN_None || (pcUnitData->iLoadedMapIndex >= 0 && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType != MAPTYPE_Town && pcUnitData->GetAnimation() != ANIMATIONTYPE_Yahoo) )
				vWeaponsMatrix.push_back( pcUnitData->sLeftHandMatrix );
			else
			{
				vWeaponsMatrix.clear();

				if ( pcUnitData->pcaWeaponDaggerBone[0] )
					vWeaponsMatrix.push_back( pcUnitData->pcaWeaponDaggerBone[0]->sWorld );

				if ( pcUnitData->pcaWeaponDaggerBone[1] )
					vWeaponsMatrix.push_back( pcUnitData->pcaWeaponDaggerBone[1]->sWorld );
			}

			StartZPos = 0;
			WeaponPosCount = WeaponSize / 1000;
			GlowWidth = 35 * fONE;
			GlowFlag = TRUE;

			Width = 12 * fONE;
			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;

			int glowPosIndex = (WeaponSize - int( (float)WeaponSize * 0.2f )) / 1000;
			if ( glowPosIndex >= MAX_WEAPON_FORCE_POS )
				glowPosIndex = MAX_WEAPON_FORCE_POS - 1;
			GlowPosIndex = glowPosIndex;
		}
		else
		{
			StartZPos = 0;
			WeaponSize = sToolItem.iSizeMax;
			WeaponPosCount = WeaponSize / 1000;

			for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
				WeaponPos[index].iZ = StartZPos + index * 1000;
		}
	}
	else if ( eLeftItemID != ITEMID_None )
	{
		vWeaponsMatrix.push_back( pcUnitData->sLeftHandMatrix );

		WeaponPosCount = pcUnitData->sLeftHandTool.iSizeMax / 1000;
		StartZPos = -pcUnitData->sLeftHandTool.iSizeMax / 3;

		for ( index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
		{
			WeaponPos[index].iZ = StartZPos + 1000 * index;
			WeaponPos[index].iX = 600;
		}
	}
	else
	{
		vWeaponsMatrix.clear();
		return;
	}

	int cnt;
	int sin;
	int cos;
	if ( pcUnitData->eWeaponItemID && (eCurrentItemType != ITEMTYPE_Dagger) && (eCurrentItemType != ITEMTYPE_Javelin) )
	{
		cnt = (-PTANGLE_45 / 5) & PTANGLE_Mask;

		sin = PTSEN[cnt] >> 8;
		cos = PTCOS[cnt] >> 8;

		for ( int index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
		{
			WeaponPos[index].iY = -(WeaponPos[index].iZ * sin) >> 8;
			WeaponPos[index].iZ = (WeaponPos[index].iZ * cos) >> 8;
		}
	}

	Point3D angle;
	memcpy( &angle, &pcUnitData->sAngle, sizeof( Point3D ) );
	angle.iY = (-angle.iY + PTANGLE_180) & PTANGLE_Mask;

	int rx, ry, rz;

	for ( UINT i = 0; i < vWeaponsMatrix.size(); i++ )
		vWeaponsPosition.push_back( ForceOrbEffectWeaponPosition( WeaponPos ) );

	for ( UINT i = 0; i < vWeaponsMatrix.size(); i++ )
	{
		auto & sMatrix				= vWeaponsMatrix[i];
		auto & sWeaponForcePosition = vWeaponsPosition[i];

		for ( int index = 0; index < MAX_WEAPON_FORCE_POS; index++ )
		{
			rx = sWeaponForcePosition.saPosition[index].iX * sMatrix._11 +
				sWeaponForcePosition.saPosition[index].iY * sMatrix._21 +
				sWeaponForcePosition.saPosition[index].iZ * sMatrix._31;

			ry = sWeaponForcePosition.saPosition[index].iX * sMatrix._12 +
				sWeaponForcePosition.saPosition[index].iY * sMatrix._22 +
				sWeaponForcePosition.saPosition[index].iZ * sMatrix._32;

			rz = sWeaponForcePosition.saPosition[index].iX * sMatrix._13 +
				sWeaponForcePosition.saPosition[index].iY * sMatrix._23 +
				sWeaponForcePosition.saPosition[index].iZ * sMatrix._33;

			sWeaponForcePosition.saPosition[index].iX = (pcUnitData->sPosition.iX + (rx >> FLOATNS) + sMatrix._41);
			sWeaponForcePosition.saPosition[index].iZ = (pcUnitData->sPosition.iZ + (ry >> FLOATNS) + sMatrix._42);
			sWeaponForcePosition.saPosition[index].iY = (pcUnitData->sPosition.iY + (rz >> FLOATNS) + sMatrix._43);
		}
	}

	sPosition.iX = WeaponPos[0].iX;
	sPosition.iY = WeaponPos[0].iY;
	sPosition.iZ = WeaponPos[0].iZ;

	vWeaponsMatrix.clear();
	
}

void ForceOrbEffectFollow::Draw( Point3D * pPosi, Point3D * pAngle )
{
	if ( pcUnitData->bVisible == FALSE )
		return;

	EXETEXTUREMANAGER_DEFAULT->pcaTexture[sFace.iMaterialID].bTwoSided = TRUE;

	for ( auto & sWeaponForcePosition : vWeaponsPosition )
	{
		for ( int index = 0; index < WeaponPosCount; index++ )
		{
			RENDERER->Init();
			RENDERER->SetTextureManager( EXETEXTUREMANAGER_DEFAULT );

			sFace.sPosition.iX = sWeaponForcePosition.saPosition[index].iX;
			sFace.sPosition.iY = sWeaponForcePosition.saPosition[index].iY;
			sFace.sPosition.iZ = sWeaponForcePosition.saPosition[index].iZ;
			sFace.iWidth = Width + ScaleWidth - index * 5;
			sFace.iHeight = Width + ScaleWidth - index * 5;
			if ( !AssaAddFace2D( &sFace ) )
				continue;

			RENDERER->SetRenderClip( 0x3C );
			RENDERER->ClipFaceRender();
			RENDERER->Vertex2DGeometry( FALSE );
			RENDERER->Render();
		}
	}

	if ( GlowFlag )
	{
		for ( auto & sWeaponForcePosition : vWeaponsPosition )
		{
			RENDERER->Init();
			RENDERER->SetTextureManager( EXETEXTUREMANAGER_DEFAULT );

			GlowFace.sPosition.iX = sWeaponForcePosition.saPosition[GlowPosIndex].iX;
			GlowFace.sPosition.iY = sWeaponForcePosition.saPosition[GlowPosIndex].iY;
			GlowFace.sPosition.iZ = sWeaponForcePosition.saPosition[GlowPosIndex].iZ;
			GlowFace.iWidth = GlowWidth + ScaleWidth;
			GlowFace.iHeight = GlowWidth + ScaleWidth;

			if ( !AssaAddFace2D( &GlowFace ) )
				return;

			RENDERER->SetRenderClip( 0x3C );
			RENDERER->ClipFaceRender();
			RENDERER->Vertex2DGeometry( FALSE );
			RENDERER->Render();
		}
	}
}

void AddForceOrbCharacterEffect( UnitData * pcUnitData, int iTime, int iID )
{
	bool bFlag = false;
	for ( int i = 0; i < MAX_EFFECTFOLLOW; i++ )
	{
		if ( EffectBaseFollow::GetData()[i] )
		{
			if ( EffectBaseFollow::GetData()[i]->dwID == 1407 )
			{
				if ( EffectBaseFollow::GetData()[i]->pcUnitData == pcUnitData )
				{
					bFlag = true;
					EffectBaseFollow::GetData()[i]->iTime = 0;
					EffectBaseFollow::GetData()[i]->iTimeMax = iTime;
				}
			}
		}
	}

	if( bFlag == false )
	{
		int iIndex = EffectBaseFollow::GetFreeIndex();

		ForceOrbEffectFollow * pcEffect = new ForceOrbEffectFollow();
		pcEffect->Start( pcUnitData, iTime, iID );
		EffectBaseFollow::GetData()[iIndex] = pcEffect;
	}
}