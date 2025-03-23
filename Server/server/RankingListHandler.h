#pragma once



class CRankingListHandler
{
public:
					CRankingListHandler();
	virtual			~CRankingListHandler();

	void			SendBellatraSoloRankingData(User * pcUser, int iRankingType);
	void			SendRankingData(User * pcUser, int iRankingType);

};