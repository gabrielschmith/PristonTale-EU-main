#pragma once
class CVirtualLifeHandler
{
public:

private:
	int							iTimerID = 0;
	int							iParticleID = -1;

	void						AddTimer ( int iTime, int iLevel );
	void						UpdateTimer ( int iTime, int iLevel );
public:


	CVirtualLifeHandler ();
	virtual					   ~CVirtualLifeHandler ();

	int							GetTexture () { return iTimerID; }
};

