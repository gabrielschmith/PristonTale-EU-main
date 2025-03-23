#include "stdafx.h"

#include "itemhandlerstruct.h"

#include "GuideGame.h"

// Login screen input box variables in raw memory location (located in the GAME.exe directly)
enum class ELoginScreenInputBox
{
	USERNAME = (DWORD)0x039033E8,
	PASSWORD = (DWORD)0x039032E8
};