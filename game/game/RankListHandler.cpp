#include "stdafx.h"
#include "RankListHandler.h"


CRankListHandler::CRankListHandler()
{
}


CRankListHandler::~CRankListHandler()
{
	SAFE_DELETE(pcRankListWindow);
}

void CRankListHandler::Init()
{
	pcRankListWindow = new CRankListWindow;
	pcRankListWindow->Init();

}

void CRankListHandler::Update()
{

}

void CRankListHandler::Render()
{
	GetWindow()->Render();
}

