#pragma once
class CCrystalTowerHandler
{
private:
	IMinMax					sTowerHP;

	UnitData				* pcTower = NULL;

	int						iRound = 0;

	DWORD					dwTimeLeft = 0;

	BOOL					bStarted = FALSE;

public:
	CCrystalTowerHandler();
	virtual ~CCrystalTowerHandler();



	void					Update();

	void					Render();

	void					HandlePacket( PacketQuestArenaTier5 * psPacket );

};

