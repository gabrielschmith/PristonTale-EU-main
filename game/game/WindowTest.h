#pragma once
class CWindowTest
{
public:
	CWindowTest();
	virtual ~CWindowTest();

	void							Init();

	BOOL							OnMouseClick( class CMouse * pcMouse );
	void							OnAllStatsClick( UIEventArgs e );
	void							OnAllSkillsClick( UIEventArgs e );
	void							OnResetButtonClick( UIEventArgs e );
	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	void							Render();

	void							Open();
	void							Close( UIEventArgs e );

	BOOL							IsOpen() { return pWindow->IsOpen(); }

	enum
	{
		CHECKBOXID_Strenght,
		CHECKBOXID_Spirit,
		CHECKBOXID_Talent,
		CHECKBOXID_Agility,
		CHECKBOXID_Health
	};

private:
	UI::Window_ptr pWindow = NULL;
};

