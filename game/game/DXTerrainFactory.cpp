#include "stdafx.h"
#include "DXTerrainFactory.h"

#include "SMDDXTerrainImporter.h"
#include "ImportFile.h"

namespace DX
{

TerrainFactory::TerrainFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
}

void TerrainFactory::OnLostDevice()
{
}

void TerrainFactory::OnResetDevice()
{
}

Terrain_ptr TerrainFactory::MakeTerrain( std::string strTerrainFile )
{
	//Get File Extension
	auto strFileExtension = Import::File( strTerrainFile ).GetFileExtension();

	//Terrain Importer
	std::shared_ptr<TerrainImporter> pTerrainImporter;

	//Choose Terrain Importer based on File Extension
	if( strFileExtension == "smd" )
	{
		//SMD Terrain Importer
		pTerrainImporter = std::make_shared<SMDTerrainImporter>();
	}

	//Terrain
	Terrain_ptr pTerrain;

	//Importer chosen?
	if( pTerrainImporter )
	{
		//Load File
		if( pTerrainImporter->Load( strTerrainFile ) )
		{
			//Build Terrain
			pTerrain = pTerrainImporter->Build();
		}
	}
	else
	{
		//Error
		WRITEDBG( "[Terrain Factory] No Importer for File Extension: %s", strFileExtension.c_str() );
	}

	//Terrain Loaded successfully?
	if( pTerrain )
	{
		//Do Nothing...
	}
	else
	{
		//Error
		WRITEDBG( "[Terrain Factory] Failed to Load: %s", strTerrainFile.c_str() );
	}

	//Return Terrain
	return pTerrain;
}

}