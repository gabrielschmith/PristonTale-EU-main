#include "stdafx.h"
#include "CServer.h"

CServer::CServer()
{
	pcServerManager = new CServerManager();
	pcServerHandler = new CServerHandler();
	pcServerWorld = new CServerWorld();

	bMaintenance = FALSE;
}

CServer::~CServer()
{
	SAFE_DELETE(pcServerWorld);
	SAFE_DELETE(pcServerHandler);
	SAFE_DELETE(pcServerManager);
}

BOOL CServer::Init()
{
	INIT(pcServerManager);
	INIT(pcServerHandler);
	INIT(pcServerWorld);

	SetMaintenance( GSERVER->SQLGetMaintenace() );

	return TRUE;
}

void CServer::Shutdown()
{
	bMaintenance = TRUE;

	SHUTDOWN(pcServerWorld);
	SHUTDOWN(pcServerHandler);
	SHUTDOWN(pcServerManager);
}

void CServer::Tick1s()
{
	pcServerHandler->Update();
}
