#pragma once
#include "RankListWindow.h"

struct StructFuryArenaBoss;

class CRankListHandler
{
private:

	CRankListWindow									* pcRankListWindow = NULL;


public:
	CRankListHandler();
	virtual ~CRankListHandler();

	void												Init();

	CRankListWindow										* GetWindow() { return pcRankListWindow; }

	void												Update();
	void												Render();
};