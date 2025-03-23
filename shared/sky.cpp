#include "stdafx.h"
#include "sky.h"

NAKED void Sky::Reset()
{
	JUMP( pfnReset );
}

NAKED void Sky::Render()
{
	JUMP( pfnRender );
}

NAKED void Sky::Draw( int a, int b, int c, int d, int e, int f )
{
	JUMP( pfnDraw );
}

NAKED void Sky::SetSky( ESkyID iSkyID )
{
	JUMP( pfnSetSky );
}