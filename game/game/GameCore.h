#pragma once

#include "ItemHandler.h"
#include "SkillManager.h"
#include "UnitGame.h"
#include "ChatGame.h"
#include "SettingsHandler.h"
#include "UI.h"
#include "UIControllerGeneral.h"
#include "ItemTimerHandler.h"
#include "DXFont.h"
#include "CLoginHandler.h"
#include "DXGraphicEngine.h"
#include "CharacterGame.h"
#include "AntiCheat.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "CAgingWindow.h"
#include "CWarehouse.h"
#include "ItemWindowHandler.h"
#include "QuestGame.h"
#include "TimerSkill.h"
#include "CDamageInfo.h"
#include "BellatraGame.h"
#include "MixHandler.h"
#include "DailyQuest.h"
#include "MessageBox.h"
#include "ToolTip.h"
#include "SocketGame.h"
#include "BlessCastleHandler.h"
#include "Particle.h"
#include "ShamanHandler.h"
#include "HUDController.h"
#include "CHalloweenHandler.h"
#include "ManufactureHandler.h"
#include "ManageWindow.h"
#include "PerfectItemViewHandler.h"
#include "TextWindow.h"
#include "CoinHandler.h"
#include "CSkillEffectHandler.h"
#include "CompareItemHandler.h"
#include "UIItemInfoBox.h"
#include "AccountHandler.h"
#include "AnimationHandler.h"
#include "MapGame.h"
#include "ChristmasHandler.h"
#include "ForceOrbHandler.h"
#include "GrandFury.h"
#include "EffectGame.h"
#include "DamageHandler.h"
#include "TestMapHandler.h"
#include "GameProtect.h"
#include "InputBox.h"
#include "EasterEventHandler.h"
#include "CQuestGame.h"
#include "WindowTest.h"
#include "BellatraReward.h"
#include "TestScreen.h"
#include "TitleBox.h"
#include "SkillDataHandler.h"
#include "TradeHandler.h"
#include "EventGirlHandler.h"
#include "gameeffectfollow.h"
#include "RecvPacket.h"
#include "Camera.h"
#include "gameinventory.h"
#include "gameskill.h"
#include "CrystalTowerHandler.h"
#include "CQuestMapHandler.h"
#include "MixGuildHandler.h"
#include "RankListHandler.h"
#include "QuestBookHandler.h"
#include "NeverSinkHandler.h"
#include "StableHandler.h"

typedef void( __cdecl *tfnDrawBMP )(int iXWindow, int iYWindow, struct EXESurfaceOld * psSurface, int iX, int iY, int iWidth, int iHeight, int iColor);
extern tfnDrawBMP fnDrawBMP;

typedef DWORD( __cdecl *tfnGetPlayTime )();
extern tfnGetPlayTime fnGetPlayTime;

extern class GameCore						* pGameCoreHandler;

extern int & iRadian3D_X;
extern int & iRadian3D_Y;
extern int & iRadian3D_Z;

extern BOOL bServerIsSeasonal;

#define SERVER_IS_SEASONAL					bServerIsSeasonal
#define DRAWBMP								fnDrawBMP

#define GETPLAYTIME							fnGetPlayTime() //not same as PLAYTIME or TICKCOUNT

#define RADIAN3D_X							iRadian3D_X
#define RADIAN3D_Y							iRadian3D_Y
#define RADIAN3D_Z							iRadian3D_Z

#define GAME_LOADINGTIME					(*(DWORD*)0x03A9767C)

#define WINDOW_WAREHOUSEMOVE				(*(int*)0x0367E844)
#define WINDOW_YINVENTORYMOVE				(*(int*)0x0367E834)
#define WINDOW_TRADEMOVE					(*(int*)0x0367E840)
#define WINDOW_SKILLMOVE					(*(int*)0x0367E83C)
#define WINDOW_CHARACTERINFOMOVE			(*(int*)0x0367E830)

#define WINDOW_ISOPEN_NPC_SHOP				(*(BOOL*)0x03653E40)
#define WINDOW_ISOPEN_PERSONALSHOP_ME		(*(BOOL*)0x0363C110) //my personal shop
#define WINDOW_ISOPEN_PERSONALSHOP_OTHERS	(*(BOOL*)0x03642090) //other players
#define WINDOW_ISOPEN_MIXING				(*(BOOL*)0x03680334)
#define WINDOW_ISOPEN_CRAFTING				(*(BOOL*)0x0369DD10)
#define WINDOW_ISOPEN_AGING				    (*(BOOL*)0x036834F0)
#define WINDOW_ISOPEN_TRADE				    (*(BOOL*)0x036EFD98)
#define WINDOW_ISOPEN_SMELTING				(*(BOOL*)0x03680348)
#define WINDOW_ISOPEN_RESETMIX				(*(BOOL*)0x0369E508)
#define WINDOW_ISOPEN_CLANMANAGER			(*(BOOL*)0x03B1D470)
#define WINDOW_ISOPEN_SKILLMASTER			(*(BOOL*)0x0367DE9C)
#define WINDOW_ISOPEN_BELLATRA_RANKING		(*(BOOL*)0x0362BF8C)
#define WINDOW_ISOPEN_WAREHOUSE				(*(BOOL*)0x036932FC)
#define WINDOW_ISOPEN_INVENTORY				(*(BOOL*)0x035EBB20)
#define WINDOW_ISOPEN_CHARACTERINFO			(*(BOOL*)0x035D0EA0)
#define WINDOW_ISOPEN_SKILLS				(*(BOOL*)0x0367E148)

#define SKILLMANAGERHANDLER					(GAMECORE->GetSkillManagerHandler())
#define CHATGAMEHANDLER						(GAMECORE->GetChatGameHandler())
#define ITEMHANDLER							(GAMECORE->GetItemHandler())
#define SETTINGSHANDLER						(GAMECORE->GetSettingsHandler())
#define UIHANDLER							(GAMECORE->GetUIControllerHandler())
#define ITEMTIMERHANDLER					(GAMECORE->GetItemTimerHandler())
#define FONTHANDLER							(GAMECORE->GetFontHandler())
#define LOGINHANDLER						(GAMECORE->GetLoginControllerHandler())
#define CHARACTERGAME						(GAMECORE->GetCharacterGameHandler())
#define ANTICHEATHANDLER					(GAMECORE->GetAntiCheatHandler())
#define	INVENTORYITEMS						(GAMECORE->pItemsInventory)
#define	TEMPINVENTORYITEMS					(GAMECORE->pTempItemsInventory)
#define	MOUSEHANDLER						(GAMECORE->GetMouseHandler())
#define	KEYBOARDHANDLER						(GAMECORE->GetKeyboardHandler())
#define	AGINGWINDOWHANDLER					(GAMECORE->GetAgingWindowHandler())
#define	WAREHOUSEHANDLER					(GAMECORE->GetWarehouseHandler())
#define	INVENTORYITEMSLOT					(GAMECORE->GetInventoryItemSlot())
#define	ITEMWINDOWHANDLER					(GAMECORE->GetItemWindowHandler())
#define	QUESTGAMEHANDLER					(GAMECORE->GetQuestGameHandler())
#define	TIMERSKILLHANDLER					(GAMECORE->GetTimerSkillHandler())
#define	SOUNDHANDLER						(GAMECORE->GetSoundHandler())
#define	PACKETHANDLER						(GAMECORE->GetPacketHandler())
#define	DAMAGEINFOHANDLER					(GAMECORE->GetDamageInfoHandler())
#define	BELLATRAGAMEHANDLER					(GAMECORE->GetBellatraGameHandler())
#define	MIXHANDLER							(GAMECORE->GetMixHandler())
#define	DAILYQUESTHANDLER					(GAMECORE->GetDailyQuestHandler())
#define	MESSAGEBOX							(GAMECORE->GetMessageBoxHandler())
#define	CTOOLTIP							(GAMECORE->GetToolTipHandler())
#define	BLESSCASTLEHANDLER					(GAMECORE->GetBlessCastleHandler())
#define	PARTICLE							(GAMECORE->GetParticleOld())
#define	SHAMANHANDLER						(GAMECORE->GetShamanHandler())
#define	HUDHANDLER							(GAMECORE->GetHUDHandler())
#define	HALLOWEENHANDLER					(GAMECORE->GetHalloweenHandler())
#define	MANUFACTUREHANDLER					(GAMECORE->GetManufactureHandler())
#define	MANAGEWINDOW						(GAMECORE->pcManageWindow)
#define	PERFECTVIEWHANDLER					(GAMECORE->GetPerfectItemViewHandler())
#define	TEXTWINDOW							(CTextWindow::GetInstance())
#define	COINHANDLER							(GAMECORE->GetCoinHandler())
#define SKILLEFFECTHANDLER					(GAMECORE->GetSkillEffectHandler())
#define COMPAREITEMHANDLER					(GAMECORE->GetCompareItemHandler())
#define ITEMINFOBOX							(GAMECORE->GetItemInfoBox())
#define SKILLINFOBOX						(GAMECORE->GetSkillInfoBox())
#define ACCOUNTHANDLER						(GAMECORE->GetAccountHandler())
#define ANIMATIONHANDLER					(GAMECORE->GetAnimationHandler())
#define MAPGAME								(GAMECORE->GetMapGame())
#define CHRISTMASHANDLER					(GAMECORE->GetChristmasHandler())
#define FORCEORBHANDLER						(GAMECORE->GetForceOrbHandler())
#define GRANDFURYHANDLER					(GAMECORE->GetGrandFuryHandler())
#define DAMAGEHANDLER						(GAMECORE->GetDamageHandler())
#define INPUTBOX							(GAMECORE->GetInputBox())
#define EASTEREVENTHANDLER					(GAMECORE->GetEasterEventHandler())
#define CQUESTGAME							(GAMECORE->GetCQuestGameHandler())
#define QUESTBOOKHANDLER					(GAMECORE->GetQuestBookHandler())
#define NEVERSINKHANDLER					(GAMECORE->GetNeverSinkHandler())
#define TESTWINDOW							(GAMECORE->GetTestWindowHandler())
#define BELLATRAREWARD						(GAMECORE->GetBellatraRewardHandler())
#define TESTSCREEN							(GAMECORE->GetTestScreenHandler())
#define CTITLEBOX							(GAMECORE->GetTitleBoxHandler())
#define SKILLDATAHANDLER					(GAMECORE->GetSkillDataHandler())
#define SYSTEM								(GAMECORE->GetSystemHandler())
#define TRADEHANDLER						(GAMECORE->GetTradeHandler())
#define EVENTGIRLHANDLER					(GAMECORE->GetEventGirlHandler())
#define CAMERA								(GAMECORE->GetCamera())
#define MESSAGEBALLOON						(GAMECORE->GetMessageBalloon())
#define CRYSTALTOWERHANDLER					(GAMECORE->GetCrystalTowerHandler())
#define QUESTMAPHANDLER						(GAMECORE->GetQuestMapHandler())
#define MIXGUILDHANDLER						(GAMECORE->GetMixGuildHandler())
#define RANKLISTHANDLER						(GAMECORE->GetRankListHandler())//xxstr rank list
#define TESTMAPHANDLER						(GAMECORE->GetTestMapHandler())
#define STABLEHANDLER						(GAMECORE->GetStableHandler())


#define GETWIDTHTEXT(str)					(FONTHANDLER->GetWidthText(0, str, STRLEN(str)))

class GameCore
{
private:
	static const DWORD							pfnComputeDeltaTime = 0x004164D0;
	static const DWORD							pfnUpdatePT			= 0x0041B5E0;
	static const DWORD							pfnRender			= 0x0041C280;
	static const DWORD							pfnUpdateFPS		= 0x004165B0;
	static const DWORD							pfnCheckExit		= 0x00416410;
	static const DWORD							pfnScreenChangeGame = 0x00417D70;
	BOOL										bCreated			= FALSE;
	BOOL										bInit				= FALSE;

public:
												GameCore();
	virtual									   ~GameCore();

	static void									Hooks();

	void										Init();
	static GameCore							  * GetInstance();

	SYSTEMTIME									sServerTime;

	void										Update( float fTime );

	static void WINAPI							UpdateFrame();

	void										Frame();
	void										Render3D();
	void										Render2D();

	void										OnEffect();

	Skill *										FindSkill ( uint32_t p_ID );
	Skill *										FindSkill2 ( uint32_t p_ID );
	SkillBase *									FindSkillBase2 ( uint32_t p_ID );

	void										DrawEffect( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

	void										OnResolutionChanged();

	BOOL										OnMouseClick( class CMouse * pcMouse );
	BOOL										OnMouseScroll( class CMouse * pcMouse );
	void										OnMouseMove( class CMouse * pcMouse );

	BOOL										OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL										OnKeyChar( class CKeyboard * pcKeyboard );

	void										Shutdown();

	BOOL										IsCreated() { return bCreated; };
	BOOL										IsInit() { return bInit; };

	static DWORD __cdecl						OnHandlerFunctions( DWORD dwCode, DWORD * pdwOptional, DWORD UnkTime1, DWORD UnkTime2);

	static void	WINAPI							SendLoginData();
	static void WINAPI							GameRun();

	static void									SetGameScreen( int iScreen );

	void										HandlePacket( SocketData * sd, PacketPing * psPacket );

	static BOOL									QuitGame() { return (*(BOOL*)0x00A17554); }
	static void									QuitGame(BOOL b) { (*(BOOL*)0x00A17554) = b; }
	static BOOL									QuitAndSave() { return (*(BOOL*)0x00A17558); }
	static void									QuitAndSave(BOOL b) { (*(BOOL*)0x00A17558) = b; }
	static BOOL									IsSaved() { return (CALL( 0x00620CA0 ) == 1); }
	static BOOL									IsWalking() { return (*(BOOL*)0x035E11D0); }

public:
	SkillManager 										* pcSkillManagerHandler;
	ChatGame 											* pcChatGameHandler;
	ItemHandler 										* pcItemHandler;
	CSettingsHandler 									* pcSettingsHandler;
	CUIControllerGeneral 								* pcUIControllerHandler;
	CItemTimerHandler									* pcItemTimerHandler;
	DX::Font 											* pcFontHandler;
	CLoginControllerHandler 							* pcLoginControllerHandler;
	DX::Graphic 										* pcGraphicEngineHandler;
	ItemData											* pItemsInventory = (ItemData*)0x035EBB24;
	ItemData											* pTempItemsInventory = (ItemData*)0x035FEEF4;
	ItemInventoryData									* pInventoryCharacterItem = (ItemInventoryData*)0x0082D4F0;
	CharacterGame 										* pcCharacterGameHandler;
	CAntiCheat											* pcAntiCheatHandler;
	CMouse												* pcMouseHandler = NULL;
	CKeyboard											* pcKeyboardHandler = NULL;
	CAgingWindow										* pcAgingWindowHandler = NULL;
	CWarehouse											* pcWarehouseHandler = NULL;
	CItemWindowHandler									* pcItemWindowHandler = NULL;
	QuestGame											* pcQuestGameHandler = NULL;
	CSound												* pcSoundHandler = NULL;
	CTimerSkill											* pcTimerSkillHandler = NULL;
	RecvPacket											* pcPacketHandler = NULL;
	CDamageInfoController								* pcDamageInfoHandler = NULL;
	BellatraGame										* pcBellatraGameHandler = NULL;
	MixHandler											* pcMixHandler = NULL;
	DailyQuest											* pcDailyQuestHandler = NULL;
	CMessageBox											* pcMessageBoxHandler = NULL;
	CToolTip											* pcToolTipHandler = NULL;
	CBlessCastleHandler									* pcBlessCastleHandler = NULL;
	CParticleOld										* pcParticleOldHandler = NULL;
	CShamanHandler										* pcShamanHandler = NULL;
	HUDController										* pcHUDHandler = NULL;
	CHalloweenHandler									* pcHalloweenHandler = NULL;
	CManufactureHandler									* pcManufactureHandler = NULL;
	CManageWindow										* pcManageWindow = (CManageWindow*)(0x03904600);
	ClanManageWindow									* pcClanManageWindow = (ClanManageWindow*)(0x03AC9F20);
	CPerfectItemViewHandler								* pcPerfectItemViewHandler = NULL;
	CCoinHandler										* pcCoinHandler = NULL;
	CSkillEffectHandler									* pcSkillEffectHandler = NULL;
	CCompareItemHandler									* pcCompareItemHandler = NULL;
	UI::ItemInfoBox_ptr									  pcItemInfoBox;
	UI::SkillInfoBox_ptr								  pcSkillInfoBox;
	CAccountHandler										* pcAccountHandler = NULL;
	CAnimationHandler									* pcAnimationHandler = NULL;
	MapGame												* pcMapGame = NULL;
	CChristmasHandler									* pcChristmasHandler = NULL;
	CForceOrbHandler									* pcForceOrbHandler = NULL;
	CGrandFury											* pcGrandFuryHandler = NULL;
	CDamageHandler										* pcDamageHandler = NULL;
	CTestMapHandler										* pcTestMapHandler = NULL;
	InputBox											* pcInputBox = NULL;
	CEasterEventHandler									* pcEasterEventHandler = NULL;
	CQuestGame											* pcCQuestGameHandler = NULL;
	QuestBookHandler									* pcQuestBookHandler = NULL;
	NeverSinkHandler									* pcNeverSinkHandler = NULL;
	CWindowTest											* pcTestWindowHandler = NULL;
	CBellatraReward										* pcBellatraRewardHandler = NULL;
	CTestScreen											* pcTestScreenHandler = NULL;
	TitleBox											* pcTitleBoxHandler = NULL;
	CSkillDataHandler									* pcSkillDataHandler = NULL;
	CSystem												* pcSystemHandler = NULL;
	CTradeHandler										* pcTradeHandler = NULL;
	CEventGirlHandler									* pcEventGirlHandler = NULL;
	CCamera												* pcCamera = NULL;
	UI::MessageBallon_ptr								  pcMessageBalloon = NULL;
	CCrystalTowerHandler								* pcCrystalTowerHandler = NULL;
	CQuestMapHandler									* pcQuestMapHandler = NULL;
	CMixGuildHandler									* pcMixGuildHandler = NULL;//xxstr mix guild
	CRankListHandler									* pcRankListHandler = NULL;//xxstr rank list
	StableHandler										* pcStableHandler;

	SkillManager 										* GetSkillManagerHandler() { return pcSkillManagerHandler; };
	ChatGame 											* GetChatGameHandler() { return pcChatGameHandler; };
	ItemHandler 										* GetItemHandler() { return pcItemHandler; };
	CSettingsHandler 									* GetSettingsHandler() { return pcSettingsHandler; };
	CUIControllerGeneral 								* GetUIControllerHandler() { return pcUIControllerHandler; };
	CItemTimerHandler		 							* GetItemTimerHandler() { return pcItemTimerHandler; };
	DX::Font 											* GetFontHandler() { return pcFontHandler; };
	CLoginControllerHandler 							* GetLoginControllerHandler() { return pcLoginControllerHandler; };
	DX::Graphic 										* GetGraphicEngineHandler() { return pcGraphicEngineHandler; };
	CharacterGame 										* GetCharacterGameHandler() { return pcCharacterGameHandler; };
	CAntiCheat											* GetAntiCheatHandler() { return pcAntiCheatHandler; };
	CMouse												* GetMouseHandler() { return pcMouseHandler; };
	CKeyboard											* GetKeyboardHandler() { return pcKeyboardHandler; };
	CAgingWindow										* GetAgingWindowHandler() { return pcAgingWindowHandler; };
	CWarehouse											* GetWarehouseHandler() { return pcWarehouseHandler; };
	CItemWindowHandler									* GetItemWindowHandler() { return pcItemWindowHandler; };
	QuestGame											* GetQuestGameHandler() { return pcQuestGameHandler; };
	CTimerSkill											* GetTimerSkillHandler() { return pcTimerSkillHandler; };
	CSound												* GetSoundHandler() { return pcSoundHandler; };
	RecvPacket											* GetPacketHandler() { return pcPacketHandler; };
	CDamageInfoController								* GetDamageInfoHandler() { return pcDamageInfoHandler; };
	BellatraGame										* GetBellatraGameHandler() { return pcBellatraGameHandler; };
	MixHandler											* GetMixHandler() { return pcMixHandler; };
	DailyQuest											* GetDailyQuestHandler() { return pcDailyQuestHandler; };
	ItemInventoryData									* GetInventoryItemSlot() { return pInventoryCharacterItem; };
	CMessageBox											* GetMessageBoxHandler() { return pcMessageBoxHandler; };
	CToolTip											* GetToolTipHandler() { return pcToolTipHandler; };
	CBlessCastleHandler									* GetBlessCastleHandler() { return pcBlessCastleHandler; };
	CParticleOld										* GetParticleOld() { return pcParticleOldHandler; };
	CShamanHandler										* GetShamanHandler() { return pcShamanHandler; };
	HUDController										* GetHUDHandler() { return pcHUDHandler; };
	CHalloweenHandler									* GetHalloweenHandler() { return pcHalloweenHandler; };
	CManufactureHandler									* GetManufactureHandler() { return pcManufactureHandler; };
	CPerfectItemViewHandler								* GetPerfectItemViewHandler() { return pcPerfectItemViewHandler; };
	CCoinHandler										* GetCoinHandler() { return pcCoinHandler; };
	CSkillEffectHandler									* GetSkillEffectHandler() { return pcSkillEffectHandler; };
	CCompareItemHandler									* GetCompareItemHandler() { return pcCompareItemHandler; };
	UI::ItemInfoBox_ptr									  GetItemInfoBox() { return pcItemInfoBox; };
	UI::SkillInfoBox_ptr								  GetSkillInfoBox() { return pcSkillInfoBox; }
	CAccountHandler										* GetAccountHandler() { return pcAccountHandler; };
	CAnimationHandler									* GetAnimationHandler() { return pcAnimationHandler; }
	MapGame												* GetMapGame() { return pcMapGame; }
	CChristmasHandler									* GetChristmasHandler() { return pcChristmasHandler; }
	CForceOrbHandler									* GetForceOrbHandler() { return pcForceOrbHandler; }
	CGrandFury											* GetGrandFuryHandler() { return pcGrandFuryHandler; }
	CDamageHandler										* GetDamageHandler() { return pcDamageHandler; }
	CTestMapHandler										* GetTestMapHandler() { return pcTestMapHandler; }
	InputBox											* GetInputBox() { return pcInputBox; }
	CEasterEventHandler									* GetEasterEventHandler() { return pcEasterEventHandler; }
	CQuestGame											* GetCQuestGameHandler() { return pcCQuestGameHandler; }
	QuestBookHandler									* GetQuestBookHandler() { return pcQuestBookHandler; }
	NeverSinkHandler									* GetNeverSinkHandler() { return pcNeverSinkHandler; }
	CWindowTest											* GetTestWindowHandler() { return pcTestWindowHandler; }
	CBellatraReward										* GetBellatraRewardHandler() { return pcBellatraRewardHandler; }
	CTestScreen											* GetTestScreenHandler() { return pcTestScreenHandler; }
	TitleBox											* GetTitleBoxHandler() { return pcTitleBoxHandler; }
	CSkillDataHandler									* GetSkillDataHandler() { return pcSkillDataHandler; }
	CSystem												* GetSystemHandler() { return pcSystemHandler; }
	CTradeHandler										* GetTradeHandler() { return pcTradeHandler; }
	CEventGirlHandler									* GetEventGirlHandler() { return pcEventGirlHandler; }
	CCamera												* GetCamera() { return pcCamera; }
	UI::MessageBallon_ptr								  GetMessageBalloon() { return pcMessageBalloon; }
	CCrystalTowerHandler								* GetCrystalTowerHandler() { return pcCrystalTowerHandler; }
	CQuestMapHandler									* GetQuestMapHandler() { return pcQuestMapHandler; }
	CMixGuildHandler									* GetMixGuildHandler() { return pcMixGuildHandler; }//xxstr mix guild
	CRankListHandler									* GetRankListHandler() { return pcRankListHandler; }//xxstr rank list
	StableHandler										* GetStableHandler() { return pcStableHandler; }//xxstr rank list
};