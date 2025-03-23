#include "stdafx.h"
#include "CLuaParticleOld.h"

CLuaParticleOld::CLuaParticleOld()
{
}

CLuaParticleOld::~CLuaParticleOld()
{
}

NAKED CEffectGroupLua * CLuaParticleOld::Load( const char * pszFileName )
{
	JMP( 0x005C78A0 );
}

NAKED void CEffectUnitFollowLua::Constructor()
{
	JMP( 0x005D31B0 );
}

NAKED void CEffectUnitFollowLua::Init( UnitData * pcUnitData, CEffectGroupLua * pcGroup, float fTime, float fHeight )
{
	JMP( 0x005CA5C0 );
}

NAKED CEffectManagerHandlerLua * CEffectManagerHandlerLua::GetInstance()
{
	JMP( 0x005CCA70 );
}

NAKED void CEffectManagerHandlerLua::AddEffect( CEffectMainBaseLua * pcEffect )
{
	JMP( 0x005CD250 );
}

NAKED void CEffectManagerHandlerLua::Clear()
{
	JMP( 0x005CC310 );
}

NAKED void CEffectMainBaseLua::Constructor()
{
	JMP( 0x005D3150 );
}

NAKED void CEffectMainBaseLua::Init( CEffectGroupLua * pcGroup, int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ )
{
	JMP( 0x005CA4A0 );
}
