#pragma once
class CVirtualRegenHandler
{
public:

private:
	int							iTimerID = 0;
	int							iParticleID = -1;
	BOOL						bMyCharacterFlag = FALSE;
	BOOL						bEndFlag = FALSE;

	void						AddTimer ( int iTime, int iLevel );
	void						UpdateTimer ( int iTime, int iLevel );
public:


	CVirtualRegenHandler ();
	virtual					   ~CVirtualRegenHandler ();

	int							GetTexture () { return iTimerID; }

};

