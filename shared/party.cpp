#include "stdafx.h"
#include "party.h"

char * GetPartyMode( EPartyMode eMode )
{
	char * pszRet = "Normal Mode";

	if( /*eMode == PARTYMODE_EXP )
		pszRet = "EXP Mode";
	else if(*/ eMode == PARTYMODE_Normal )
		pszRet = "Normal Mode";
	else if( eMode == PARTYMODE_Hunt )
		pszRet = "Hunt Mode";

	return pszRet;
}