#include "stdafx.h"
#include "CGameWorld.h"


CGameWorld::CGameWorld()
{
	pcWorld = NULL;
}



BOOL CGameWorld::Init()
{
	ULONG ulFPTSeasonal		= inet_addr( IP_FORTRESS_SEASONAL );
	ULONG ulFPTMain			= inet_addr(IP_FORTRESS_MAIN);
	ULONG ulFPTDev			= inet_addr(IP_FORTRESS_DEV);

#ifdef SKILLS_TESTING_TEMP
	vWorlds.push_back( new CWorld( WORLDID_None,	 "None",		0x00000000,					0,	   "",	       "",              "",  0x00000000,    0x00000000 ) ); // edit chaos
	vWorlds.push_back( new CWorld( WORLDID_Seasonal, "Seasonal",	ulFPTSeasonal,		10009, GAME_NAME, "PT EU", "Pristontale.eu", ulFPTMain,		ulFPTMain ) );
#else
	vWorlds.push_back( new CWorld( WORLDID_None,	 "None",		0x00000000,		0,	   "",	"", "", 0x00000000, 0x00000000 ) ); // edit chaos
	vWorlds.push_back( new CWorld( WORLDID_Babel,	 "Babel",	 ulFPTMain,		10009, GAME_NAME, "PT EU", "Pristontale.eu", ulFPTMain,		ulFPTMain ) );
	vWorlds.push_back( new CWorld( WORLDID_Seasonal, "Seasonal", ulFPTSeasonal,	10009, GAME_NAME, "PT EU", "Pristontale.eu", ulFPTSeasonal, ulFPTSeasonal ) ); 
#endif

	vWorlds.push_back( new CWorld( WORLDID_Dev,		"Dev",		ulFPTDev,		10009, GAME_NAME, "PT EU", "Pristontale.eu", ulFPTDev,		ulFPTDev ) );


#if defined (SEASONAL_IS_ACTIVE) || defined (SKILLS_TESTING_TEMP)
	SetWorld( WORLDID_Seasonal );
#elif defined (DEV_MODE)
	SetWorld( WORLDID_Dev );
#else
	SetWorld( WORLDID_Babel );
#endif

	return TRUE;
}

void CGameWorld::Shutdown()
{
	for ( vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++ )
	{
		CWorld * p = *it;

		SAFE_DELETE( p );
	}
	vWorlds.clear();

	pcWorld = NULL;
}

void CGameWorld::Apply()
{
	SOCKETGAME->SetIPPort( GetWorld()->GetIP().c_str(), GetWorld()->GetPort() );
}

void CGameWorld::SetWorld( EWorldID iWorldID )
{
	for ( vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++ )
	{
		CWorld * p = *it;

		if ( p->GetWorldID() == iWorldID )
		{
			pcWorld = p;
			return;
		}
	}

	pcWorld = NULL;
}

void CGameWorld::SetWorld( string strWorld )
{
	for ( vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++ )
	{
		CWorld * p = *it;

		if ( p->GetName().compare( strWorld ) == 0 )
		{
			pcWorld = p;
			return;
		}
	}

	SetWorld( WORLDID_None );
}