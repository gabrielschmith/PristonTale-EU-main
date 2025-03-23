#include "stdafx.h"
#include "item.h"

BOOL ItemData::IsOneHandedWeapon()
{
	if( iItemSlotFlag & ITEMSLOTFLAG_RightHand )
	{
		if( iItemSlotFlag & ITEMSLOTFLAG_LeftHand )
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL ItemData::IsTwoHandedWeapon()
{
	if( ( iItemSlotFlag & ITEMSLOTFLAG_RightHand ) && ( iItemSlotFlag & ITEMSLOTFLAG_LeftHand ) )
		return TRUE;

	return FALSE;
}