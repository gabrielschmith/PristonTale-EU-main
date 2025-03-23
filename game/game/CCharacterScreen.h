#pragma once

#include "Screen.h"

class CCharacterScreen : public CScreen
{
public:
						CCharacterScreen();
						~CCharacterScreen();

	BOOL				Init();
	void				Shutdown();

	void				Render3D();
	void				Render2D();

	EScreen				Frame();

	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );

	void				SetScreenHalf( int iWidth ) { iScreenHalfWidth = iWidth; }

private:

	char				cPadding001[0x01C4];

	int					iScreenHalfWidth;			//0x1C4

	char				cPadding002[0x036004];

	int					iTexturePosition;

	BOOL				bLoadingTime = FALSE;

}; //Size = 0x361CC

