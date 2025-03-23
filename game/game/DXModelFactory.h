#pragma once

#include "DXModel.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class ModelFactory> ModelFactory_ptr;

class ModelFactory
{
public:
								  ModelFactory( GraphicsDevice * pGraphicsDevice );

	void						  OnLostDevice();
	void						  OnResetDevice();
	
	Model_ptr					  MakeModel( std::string strModelFile, std::string strBoneFile = "" );

private:
	GraphicsDevice				* pGraphicsDevice;

	std::vector<Model_ptr>		  vCache;
};

}