#pragma once

#define MAX_MESSAGES			3
#define MAX_DURATION_TITLEBOX	10
#define MAX_DURATION_ANIM		600

#include "X3DEasings.h"


class CToolTip
{
private:
	int							iFontID = -1;
	int							iCountdown = 0;
	int							iWarmUp = 0;
	std::string					pszText = "";
	BOOL						bRender = FALSE;
	int							iX = 0;
	int							iY = 0;

	int							iWidthText = -1;
	int							iHeightText = -1;


	UI::Image					* pBorderImages[6];

public:
	CToolTip();
	virtual					   ~CToolTip();

	void						Render();

	void						SetText( int iX, int iY, const char * pszText );

	void						Tick();
	void						Init();
};

