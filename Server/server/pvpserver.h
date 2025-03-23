#pragma once
class PvPServer
{
private:
	DWORD												dwTick = 0;
	WORD												sLastDay = 0;

	struct PvPClassData
	{
		char							szCharacterName[10][32];
	};

public:
	PvPServer();
	virtual ~PvPServer();

	BOOL												SQLGetTopPvP( PvPClassData & s );
	BOOL												SQLSetTopPvP( PvPClassData & s );

	void												ResetPvPBuffer();

	void												Update();

	BOOL												SQLSelectCharacterPvP( User * pcUser );

	void												ProcessPvPData( User * pcUser, int iID, BOOL bKill );
	void												ProcessPvPKill( User * pcKiller, User * pcVictim, int iType );

	BOOL												OnLoadUser( User * pcUser );
	BOOL												OnUnLoadUser( User * pcUser );

	void												OnKilled( User * pcKiller, User * pcVictim );

	void												ResetTick() { dwTick = 0; sLastDay = 0; }
};

