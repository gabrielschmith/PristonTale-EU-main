#pragma once

#include "CWorld.h"

#define WORLD			(GAMEWORLD->GetWorld())

#define WORLD_BABEL_MAIN	(WORLD->GetWorldID() == WORLDID_Babel)
#define WORLD_SEASONAL		(WORLD->GetWorldID() == WORLDID_Seasonal)
#define WORLD_DEV			(WORLD->GetWorldID() == WORLDID_Dev)

class CGameWorld
{
public:
	CGameWorld();
	virtual				 ~CGameWorld() {}

	BOOL				  Init();
	void				  Shutdown();

	void				  Apply();

	//Setters
	void				  SetWorld( EWorldID iWorldID );
	void				  SetWorld( std::string strWorld );

	//Getters
	CWorld				* GetWorld() { return pcWorld; }

private:
	std::vector<CWorld*>	  vWorlds;

	CWorld				* pcWorld;

};