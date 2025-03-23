#include "stdafx.h"
#include "DXModelFactory.h"

#include "DXGraphicsDevice.h"
#include "SMDDXModelImporter.h"
#include "ImportFile.h"

namespace DX
{

ModelFactory::ModelFactory( GraphicsDevice * pGraphicsDevice ) : pGraphicsDevice(pGraphicsDevice)
{
	vCache.reserve( 500 );
}

void ModelFactory::OnLostDevice()
{
}

void ModelFactory::OnResetDevice()
{
}

Model_ptr ModelFactory::MakeModel( std::string strModelFile, std::string strBoneFile )
{
	//Find same Model in Cache..
	for( auto pModel : vCache )
	{
		//With Bones?
		if( strBoneFile.length() != 0 )
		{
			//Same Model File?
			if( pModel->GetFile().compare( strModelFile ) == 0 )
			{
				//Has Bone Model?
				if( auto pBoneModel = pModel->GetBoneModel() )
				{
					//Same Bone Model File? Share it
					if( pBoneModel->GetFile().compare( strBoneFile ) == 0 )
						return pModel;
				}
			}
		}
		else
		{
			//Same Model File? Share it
			if( pModel->GetFile().compare( strModelFile ) == 0 )
				return pModel;
		}
	}

	//Get File Extension
	auto strFileExtension = Import::File( strModelFile ).GetFileExtension();

	//Model Importer
	std::shared_ptr<ModelImporter> pModelImporter;

	//Choose Model Importer based on File Extension
	if( strFileExtension == "ase" )
	{
		//Build LowerCase Model File
		std::string strLowerModelFile;
		for( auto c : strModelFile )
			strLowerModelFile.append( 1, tolower( c ) );

		//Replace ASE with SMD
		if( auto it = strLowerModelFile.find( ".ase" ) )
			strModelFile = strLowerModelFile.replace( it, 4, ".smd" );

		//SMD Model Importer
		pModelImporter = std::make_shared<SMDModelImporter>();
	}
	else if( strFileExtension == "smd" )
	{
		//SMD Model Importer
		pModelImporter = std::make_shared<SMDModelImporter>();
	}

	//Model
	Model_ptr pModel;

	//Importer chosen?
	if( pModelImporter )
	{
		//With Bones?
		if( strBoneFile.length() != 0 )
		{
			//Load Bone File
			if( pModelImporter->Load( strBoneFile ) )
			{
				//Build Bone Model
				auto pBoneModel = pModelImporter->Build();

				//Bone Model Loaded?
				if( pBoneModel )
				{
					//Load Model File
					if( pModelImporter->Load( strModelFile ) )
					{
						//Load Model
						pModel = pModelImporter->Build( pBoneModel );
					}
				}
			}
		}
		else
		{
			//Load Model File
			if( pModelImporter->Load( strModelFile ) )
			{
				//Build Model
				pModel = pModelImporter->Build();
			}
		}
	}
	else
	{
		//Error
		WRITEDBG( "[Model Factory] No Importer for File Extension: %s", strFileExtension.c_str() );
	}

	//Model Loaded successfully?
	if( pModel )
	{
		//Remember Model
		vCache.push_back( pModel );
	}
	else
	{
		//Error
		WRITEDBG( "[Model Factory] Failed to Load: %s (%s)", strModelFile.c_str(), strBoneFile.c_str() );
	}

	//Return Model
	return pModel;
}

}