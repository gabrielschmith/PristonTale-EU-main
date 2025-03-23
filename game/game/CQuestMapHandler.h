#pragma once


class CQuestMapHandler
{
public:
	CQuestMapHandler();
	virtual ~CQuestMapHandler();

	void					Init();

	void					Render();

	void					Update( float fTime );

	BOOL					OnMouseClick( class CMouse * pcMouse );

private:
	UI::ChargeBar_ptr		pChargeBar;

	void					OnCompleteChargeBarQuest( int iQuestID, UIEventArgs eArgs );

	BOOL					IsNearFromFrozenBlessedTree( Point3D sPosition );
};

