#pragma once

#include "DXTerrain.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class TerrainFactory> TerrainFactory_ptr;

class TerrainFactory
{
public:
								  TerrainFactory( GraphicsDevice * pGraphicsDevice );

	void						  OnLostDevice();
	void						  OnResetDevice();
	
	Terrain_ptr					  MakeTerrain( std::string strTerrainFile );

private:
	GraphicsDevice				* pGraphicsDevice;
};

}