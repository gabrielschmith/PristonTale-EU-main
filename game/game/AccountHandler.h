#pragma once
class CAccountHandler
{
public:
	CAccountHandler();
	virtual ~CAccountHandler();

	void												SetCoinAmount( int i ) { iCoinAmount = i; }
	int													GetCoinAmount() { return iCoinAmount; }

protected:
	int													iCoinAmount;
};

