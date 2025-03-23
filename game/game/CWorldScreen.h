#pragma once
#include "Screen.h"

class CWorldScreen : public CScreen
{
public:
						CWorldScreen();
					   ~CWorldScreen();

	BOOL				Init();
	void				Shutdown();

	void				Render3D();
	void				Render2D();

	int					SinMain();
	EScreen				Frame();
	void				Update( float fTime );

	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );
	BOOL				OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL				OnMouseScroll( class CMouse * pcMouse );
	BOOL				OnKeyChar( class CKeyboard * pcKeyboard );

	void				OnResolutionChanged();
private:
	bool				bHUDInitialized = false;

	EXEModel * pcBoneTarget;
};

