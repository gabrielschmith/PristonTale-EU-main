#pragma once
class BellatraGame
{
protected:
	const DWORD dwSodCrownGold					= 0x096A0001;
	const DWORD dwSodCrownSilver				= 0x096A0002;
	const DWORD dwSodCrownBronze				= 0x096A0003;

	DWORD										dwTextureCrownTimerGold = 0;
	DWORD										dwTextureCrownTimerSilver = 0;
	DWORD										dwTextureCrownTimerBronze = 0;

	BOOL										bKilledTimers = FALSE;
	BOOL										bKilledSoloTimers = FALSE;
public:
	BellatraGame();
	virtual ~BellatraGame();

	void										HandleSoloSodStart();
	void										SetCrown();

	int											iBellatraCrownGoldClanId = 0;
	int											iBellatraCrownSilverClanId = 0;
	int											iBellatraCrownBronzeClanId = 0;
};

