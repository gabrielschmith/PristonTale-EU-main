#include "stdafx.h"
#include "gameskill.h"


CGameSkill::CGameSkill()
{
}


CGameSkill::~CGameSkill()
{
}

NAKED void CGameSkill::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

NAKED void CGameSkill::FrozenMistHit( UnitData * pcUnitData, UnitData * pcTarget )
{
	JMP( 0x005B08C0 );
}

NAKED void CGameSkill::FrozenMistSkillHit( UnitData * pcUnitData, UnitData * pcTarget )
{
	JMP( 0x005B0950 );
}
