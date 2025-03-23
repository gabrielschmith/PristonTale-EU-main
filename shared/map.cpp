#include "stdafx.h"
#include "map.h"



void BaseMap::SetFileName( char * pszName, char * pszTitle )
{
#if defined(_GAME)
	StringCbPrintfA( szStageFilePath, 64, "field\\%s", pszName );
#else
	StringCbPrintfA( szStageFilePath, 64, "%s", pszName );
#endif
	StringCbPrintfA( szMiniMapNameFilePath, 64, "field\\title\\%st.tga", pszTitle );
	StringCbPrintfA( szMiniMapFilePath, 64, "field\\map\\%s.tga", pszTitle );
	iUnused = 1;
}

void BaseMap::AddBorder( BaseMap * pcMap, int iX, int iZ, int iY )
{
	if( iBorderCount >= MAX_BORDERMAP )
		return;
	saFieldBorder[iBorderCount].iX = iX;
	saFieldBorder[iBorderCount].iY = iY;
	saFieldBorder[iBorderCount].iZ = iZ;
	saFieldBorder[iBorderCount].pcMap = pcMap;
	iBorderCount++;

	if( pcMap->iBorderCount >= MAX_BORDERMAP )
		return;
	pcMap->saFieldBorder[pcMap->iBorderCount].iX = iX;
	pcMap->saFieldBorder[pcMap->iBorderCount].iY = 0;
	pcMap->saFieldBorder[pcMap->iBorderCount].iZ = iZ;
	pcMap->saFieldBorder[pcMap->iBorderCount].pcMap = this;
	pcMap->iBorderCount++;
}

void BaseMap::AddTeleport1( int iX, int iZ, int iY, int iSize, int iHeight, BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iLevel, int iWarpType )
{
	if( iWarpGateCount >= MAX_BORDERMAP )
		return;
	iWarpGateCurrent = iWarpGateCount;
	saWarpGate[iWarpGateCount].iX = iX << 8;
	saWarpGate[iWarpGateCount].iY = iY << 8;
	saWarpGate[iWarpGateCount].iZ = iZ << 8;
	saWarpGate[iWarpGateCount].iHeight = iHeight << 8;
	saWarpGate[iWarpGateCount].iSize = iSize << 8;
	saWarpGate[iWarpGateCurrent].iEffect = iWarpType;
	saWarpGate[iWarpGateCount].iLevel = iLevel;
	saWarpGate[iWarpGateCount].iOutFieldCount = 0;
	iWarpGateCount++;

	Teleport * psWarp = saWarpGate + iWarpGateCurrent;
	int iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap << 8;
	psWarp->iOutFieldCount++;

	if( pcMap == this )
	{
		sWarpGatePosition.iX = iXMap << 8;
		sWarpGatePosition.iY = iYMap << 8;
		sWarpGatePosition.iZ = iZMap << 8;
	}
}

void BaseMap::AddTeleport2( int iX, int iZ, int iY, int iSize, int iHeight, BaseMap * pcMap, int iXMap, int iZMap, int iYMap, int iXMap2, int iZMap2, int iYMap2, int iLevel, int iWarpType )
{
	if( iWarpGateCount >= MAX_BORDERMAP )
		return;

	iWarpGateCurrent = iWarpGateCount;
	saWarpGate[iWarpGateCount].iX = iX << 8;
	saWarpGate[iWarpGateCount].iY = iY << 8;
	saWarpGate[iWarpGateCount].iZ = iZ << 8;
	saWarpGate[iWarpGateCount].iHeight = iHeight << 8;
	saWarpGate[iWarpGateCount].iSize = iSize << 8;
	saWarpGate[iWarpGateCount].iEffect = iWarpType;
	saWarpGate[iWarpGateCount].iLevel = iLevel;
	iWarpGateCount++;

	Teleport * psWarp = saWarpGate + iWarpGateCurrent;
	int iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap << 8;
	psWarp->iOutFieldCount++;

	if( pcMap == this )
	{
		sWarpGatePosition.iX = iXMap << 8;
		sWarpGatePosition.iY = iYMap << 8;
		sWarpGatePosition.iZ = iZMap << 8;
	}

	iOut = psWarp->iOutFieldCount;

	if( psWarp->iOutFieldCount >= MAX_BORDERMAP )
		return;

	psWarp->saOutFieldBorder[iOut].pcMap = pcMap;
	psWarp->saOutFieldBorder[iOut].iX = iXMap2 << 8;
	psWarp->saOutFieldBorder[iOut].iY = iYMap2 << 8;
	psWarp->saOutFieldBorder[iOut].iZ = iZMap2 << 8;
	psWarp->iOutFieldCount++;
}

void BaseMap::AddSound( int iX, int iY, int iZ, int iRound, int iCode )
{
	if( iAmbientCount >= MAX_AMBIENTMAP )
		return;

	saAmbientPosition[iAmbientCount].iX = iX << 8;
	saAmbientPosition[iAmbientCount].iY = iY << 8;
	saAmbientPosition[iAmbientCount].iZ = iZ << 8;
	saAmbientPosition[iAmbientCount].iRound = iRound;
	saAmbientPosition[iAmbientCount].iNum = iCode;
	iAmbientCount++;
}

void BaseMap::AddStaticModel( char * pszName, int iAnimationBip )
{
	if( iObjectCount >= MAX_OBJECTMAP )
		return;

	StringCbPrintfA( szaObjectName[iObjectCount], 64, "%s", pszName );
	dwaObjectBip[iObjectCount] = iAnimationBip;
	iObjectCount++;
}

void BaseMap::AddModelEx( char * pszFormat, ... )
{
	if( iObjectCount >= MAX_OBJECTMAP )
		return;

	va_list vl;

	char szStr[128];

	va_start( vl, pszFormat );
	_vsnprintf_s( szStr, _countof( szStr ), _TRUNCATE, pszFormat, vl );
	va_end( vl );

	StringCbPrintfA( szaObjectName[iObjectCount], 64, "%s", szStr );
	dwaObjectBip[iObjectCount] = 0;
	iObjectCount++;
}

void BaseMap::SetCamera( int iX, int iY, int iZ, int iLeftX, int iRightX )
{
#if defined(_GAME)
	sActionCamera.sPosition.iX	= iX;
	sActionCamera.sPosition.iY	= iY;
	sActionCamera.sPosition.iZ	= iZ;
	sActionCamera.iLeftX		= iLeftX;
	sActionCamera.iRightX		= iRightX;
#endif
}



BOOL Map::DeleteItem( Loot * psLoot )
{
	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		Loot * ps = saLoot + i;

		if ( ps->bInUse && 
			 ps->sPosition == psLoot->sPosition &&
			 ps->sLootItem.sItem.sItemID.ToItemID() == psLoot->sLootItem.sItem.sItemID.ToItemID() )
		{
			ps->bInUse = FALSE;
			return TRUE;
		}
	}

	WARN( "DeleteItem failed. Item '%s' was not found in map", psLoot->sLootItem.sItem.szItemName );

	return FALSE;
}

NAKED UnitData * Map::CreateMonster( CharacterData * psCharacterData, int iCount )
{
	JMP( pfnCreateMonster ); 
}

NAKED void Map::AddUnitData( UnitData * pcUnitData )
{
	JMP( pfnAddUnitData ); //this actually calls BOOL __stdcall AddMonsterSpawn( UnitData * pcUnitData ) in DLL.cpp
}

NAKED int Stage::GetHighestPoint( int iX, int iZ )
{
	JMP( pfnGetHighestPoint );
}

/// <summary>
/// Leaked: smSTAGE3D::GetFloorHeight( int x, int y, int z , int ObjHeight )
/// ASM Address: 0x46A960
/// Find the floor height at the current location
/// </summary>
NAKED int Stage::GetHighestPoint( int iX, int iY, int iZ, int iMeshHeight )
{
	JMP( pfnGetHighestPointEx );
}

/// <summary>
/// Leaked STG_ITEMS *STG_AREA::FindItem(int x, int y, int z)
/// Find matching item based on position
/// </summary>
NAKED Loot * Stage::FindItem ( int iX, int iY, int iZ ) 
{
	JMP ( pfnFindItem );
}

NAKED void Stage::Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ, EXEMatrixE * sRotationMatrix )
{
	JMP( pfnDrawMap );
}

NAKED Stage * Stage::LoadStage( char * pszFileName, int i )
{
	JMP( pfnLoadStage );
}