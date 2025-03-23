/*
*	STR MIX GUILD 2019
*	gabrielcarlos91@hotmail.com
*/
#pragma once
class CRankListWindow
{
protected:

	UI::Window_ptr											pWindow1 = NULL;

	enum
	{
		WINDOWID_RankListData	= 2,
		WINDOWID_LABEL			= 4,	
		TEXTID_TITLE			= 6,

		LISTID_Main				= 8,
		LISTID_Ranking			= 10,

		LISTID_Tabs				= 20,
		LISTID_ChildTabs		= 30,

		GROUPID_TabButtons		= 40,
		GROUPID_ChildTabButtons	= 50,

		WINDOWID_Player			= 100,
		ITEM_TAB				= 200,
		ITEM_TABWindow			= 300,

		COLUMN_NAME01			= 400,
		COLUMN_NAME02,
		COLUMN_NAME03,
		COLUMN_NAME04,
		COLUMN_NAME05,
		COLUMN_NAME06,

		COLUMN_VALUE01			= 500,				
		COLUMN_VALUE02			= 600,		
		COLUMN_VALUE03			= 700,		
		COLUMN_VALUE04			= 800,		
		COLUMN_VALUE05			= 900,		
		COLUMN_VALUE06			= 1000		

	};
	int														iSelectedRank = 0;
private:

	void													BuildWindow1();

	void													OnCloseButtonClick(UIEventArgs e) { this->Close(); };

	void													OnSoloRankTopLevelButtonClick(int iType);
	void													OnRankTopLevelButtonClick(int iType);
	void													OnRankTopBlessButtonClick();
	void													OnRankTopPvpButtonClick();
	void													OnRankTopSodClanButtonClick();

	BOOL													bRankingIsForSoloSod;
	const int												iRankingListStartX = 200;

public:
	CRankListWindow();
	virtual ~CRankListWindow();

	//DADOS PACOTE TOP LEVEL
	char							szCharacterName[50][32];
	int								iLevel[50];
	int								iClass[50];
	uint64_t						iExperience[50];


	//TOP SOLO RANKING
	char							szSoloCharacterName[30][32];
	int								iSoloLevel[30];
	int								iSoloClass[30];
	int								iSoloKills[30];
	int								iSoloScore[30];

	//DADOS PACOTE TOP BLESS
	char							szNameClan[32];
	char							szClanLeader[32];
	char							iClanBlessIconID[32];

	//DADOS PACOTE TOP PVP
	char							szPVPCharName[10][32];
	int								iPVPKills[10];
	int								iPVPDeaths[10];

	//DADOS PACOTE TOP CLAN SOD
	char							szSODClanName[10][32];
	int								iSODPoints[10];
	int								iSODMemCount[10];

	//DADOS PACOTE TOP SOD PLAYER
	char							szSODPlayer[10][32];
	int								iSODPlayerPoints[10];

	void													OnMouseMove(class CMouse* pcMouse);
	BOOL													OnMouseClick(class CMouse* pcMouse);
	BOOL													OnMouseScroll(class CMouse* pcMouse);

	BOOL													OnKeyPress(class CKeyboard* pcKeyboard);
	BOOL													OnKeyChar(class CKeyboard* pcKeyboard);

	void													HandlePacket(PacketRankingListBlessedCastle* psPacket);
	void													HandlePacket(PacketRankingListByClass* psPacket);
	void													HandlePacket(PacketRankingListClanSodList * psPacket);
	void													HandlePacket(PacketRankingListIndividualSod * psPacket);
	void													HandlePacket(PacketSoloRankingList* psPacket);
	void													HandlePacket(PacketRankingListAllPlayer* psPacket);
	void													HandlePacket(PacketRankingListPVP * psPacket);



	void													OnParentTabButtonClick(char* pcTab, UIEventArgs eArgs);

	void													ClearParentTabList();
	void													ClearItemsList();
	void													ClearLabel();

	void													OnResolutionChanged();

	void													AddTab(char * pcTab, int count);

	void													Open();
	void													Close();

	void													Init();

	void													Render();
};

