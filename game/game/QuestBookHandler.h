#pragma once

enum EGetQuestTypes
{
	ALL_QUESTS = 1,
	SIDE_QUESTS,
	ZONE_QUESTS,
	SOLO_QUESTS,
	TIER_QUESTS
};

enum class EQuestState : BYTE
{
	Unknown				  = 0,
	Completed			  = 1, //Quest is completed but haven't been handled in yet
	InProgress			  = 2,
	Avaliable			  = 3, //quest is avaliable
	RequiredQuestNotDone  = 4, //required quest not done yet
	OtherNpcQuestTaken    = 5, //when a different quest from same NPC is already underway (only for repeatables)
	LevelReqNotMet        = 6, //level requirements for quest not met yet
	LevelReqNotMetWithin5 = 7, //level requirements for quest not met yet, but is within 5 level of it
	NotReadyYet           = 8, //timed quest, completed but not ready yet
	Done                  = 9, //quest is done (handed in)
	Expired				  = 10, //quest has expired, can be taken again

	Disable				  = 255,
};

struct QuestBookEntry
{
	int				  iQuestID;
	char			  szQuestName[40];
	char			  szGroupName[32];
	short			  sGroupNum;
	short			  sQuestLevelMin;
	short			  sQuestLevelMax;
	short			  sMapID;
	short			  sGiverNpcID;
	short			  sRequiredQuestID;
	int				  sMainQuestID;
	char			  szOtherQuestName[40];

	int				  iASMQuestBit;

	EQuestState		  eQuestState;
	EQuestType	      eQuestType;
	EQuestPartyType   eQuestPartyType;
};

class QuestBookHandler
{
public:
	QuestBookHandler();
	~QuestBookHandler();

	void Init();
	void BuildWindow();
	void GetQuestList();
	bool OnKeyPress(class CKeyboard* pcKeyBoard);
	void Render();

	void OnExpandCollapseClick( int iGroupID, UIEventArgs e );
	void OnShowAvailableCheckboxClicked( UIEventArgs e );
	void OnCloseButtonClick( UIEventArgs e );

	BOOL OnMouseClick(class CMouse* pcMouse);
	BOOL OnMouseScroll(class CMouse* pcMouse);
	void OnMouseMove(class CMouse* pcMouse);

	void OnResolutionChanged();
	void UpdateWindowPosition();

	const std::string GetQuestStateString( EQuestState eQuestStatus );

	void HandlePacket( PacketQuestNamesResponse* psPacket);
	void HandlePacket( PacketQuestDataResponse* psPacket);
	void BuildQuestDataView( PacketQuestDataResponse * psPacket );
	void RebuildCurrentQuestDataView( );

	void SetCurrentViewingQuestID( int iQuestID ) { iCurrentViewingQuestID = iQuestID; };

	void BuildQuestList();
	void ReLoadASMQuests();
	void ReLoadActiveAndFinishedQuest();

	BOOL IsWindowOpen();
	BOOL CanProcessRequest();

	void OnQuestClick(int iID, UIEventArgs e);
	void RequestQuestData( int iQuestID );

private:


	enum EElementID : int
	{
		Window_QuestList,
		Window_QuestInf,

		Button_All,
		Button_Zone,
		Button_Solo,
		Button_Side,

		CheckBox_ShowAll,

		QuestGroup_Expand_ID = 1000,

		QuestGroup_Collapse_ID = 2000,
	};

	UI::Window_ptr questBookWindow;
	UI::Window_ptr pQuestListWindow;
	UI::Window_ptr leftBoxRegion;
	UI::Window_ptr rightBoxRegion;
	UI::Window_ptr rightBoxSubRegion;
	UI::Window_ptr questTextWindow;
	UI::List_ptr questsList;
	UI::List_ptr rightBoxList;

	//std::map <int, QuestBookDataEntry>

	std::vector<QuestBookEntry> vQuestEntries; //all quest entries
	std::map <int, BOOL> mForceShowHideGroup; //force collapse / expand mapping
	std::map <int, PacketQuestDataResponse> mQuestDataResponse;

	int iCurrentViewingQuestID = 0;

	DWORD dwRequestTick;
	DWORD dwResponseTick;
	BOOL bShowAll = FALSE;
	BOOL bQuestNamesReceived = FALSE;
	int iTotalRowHeight = 0;



	const int iQuestBookWindowWidth = 823;
	const int iQuestBookWindowHeight = 698;

	const int iLeftBoxLeftX = 57;
	const int iLeftBoxTopY = 218;
	const int iLeftBoxRightX = 400;
	const int iLeftBoxBottomY = 644;

	const int iRightBoxListPadding = 15;
	const int iQuestListTopYOffset = 30;
	const int iQuestListPadding = 15;
	const int iScrollBarWidth = 10;
	const int iRowPadding = 8;

	const int iQuestListRowHeight = 16;

	const int iCloseButtonLeftX = 766;
	const int iCloseButtonTopY = 112;
	const int iCloseButtonWidth = 39;
	const int iCloseButtonHeight = 39;

	const int iRightBoxLeftX = 423;
	const int iRightBoxTopY = 218;
	const int iRightBoxRightX = 766;
	const int iRightBoxBottomY = 644;

	constexpr int GetLeftBoxHeight() { return iLeftBoxBottomY - iLeftBoxTopY; };
	constexpr int GetLeftBoxWidth() { return iLeftBoxRightX - iLeftBoxLeftX; };

	constexpr int GetRightBoxHeight() { return iRightBoxBottomY - iRightBoxTopY; };
	constexpr int GetRightBoxWidth() { return iRightBoxRightX - iRightBoxLeftX; };

	constexpr int GetQuestListRowWidth() { return GetLeftBoxWidth() - iQuestListPadding - iQuestListPadding - iScrollBarWidth; };
};