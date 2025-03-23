#include "stdafx.h"
#include "CConfig.h"

CConfig::CConfig( string _file ) : file(_file)
{
	pcReader	= NULL;
	pcWriter	= NULL;
}

CConfigFileReader * CConfig::GetReader()
{
	if( pcReader == NULL )
	{
		pcReader = new CConfigFileReader( file );
		pcReader->Open();
	}

	return pcReader;
}

CConfigFileWriter * CConfig::GetWriter()
{
	if( pcWriter == NULL )
	{
		pcWriter = new CConfigFileWriter( file );
		pcWriter->Open();
	}

	return pcWriter;
}

void CConfig::CloseReader()
{
	if( pcReader )
	{
		pcReader->Close();
		SAFE_DELETE( pcReader );
	}
}

void CConfig::CloseWriter()
{
	if( pcWriter )
	{
		pcWriter->Close();
		SAFE_DELETE( pcWriter );
	}
}