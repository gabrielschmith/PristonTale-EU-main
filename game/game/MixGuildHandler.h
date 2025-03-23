#pragma once
#include "MixGuildWindow.h"

struct StructFuryArenaBoss;

class CMixGuildHandler
{
private:

	CMixGuildWindow										* pcMixGuildWindow = NULL;


public:
	CMixGuildHandler();
	virtual ~CMixGuildHandler();

	void												Init();

	CMixGuildWindow*									GetWindow() { return pcMixGuildWindow; }

	void												Update();
	void												Render();
};

