#include "stdafx.h"
#include "SettingsHandler.h"



CSettingsHandler::CSettingsHandler()
{
	pcSettingsModel		= new CSettingsModel(); 
	pcSettingsView		= new CSettingsView(); 
}

CSettingsHandler::~CSettingsHandler()
{
	SAFE_DELETE( pcSettingsModel );
	SAFE_DELETE( pcSettingsView );
	SAFE_DELETE( pcSettingsWindow );
}

CSettingsModel * CSettingsHandler::GetModel()
{
	return pcSettingsModel;
}

CSettingsView * CSettingsHandler::GetView()
{
	return pcSettingsView;
}

CSettingsWindow * CSettingsHandler::GetWindow()
{
	return pcSettingsWindow;
}

void CSettingsHandler::Init()
{
	
	pcSettingsWindow	= new CSettingsWindow(); 
	INIT( pcSettingsWindow );
}

void CSettingsHandler::Shutdown()
{

}
