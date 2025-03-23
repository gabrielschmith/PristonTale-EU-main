#pragma once

#include "CChatBox.h"
#include "CPartyHandler.h"
#include "CInventory.h"
#include "CMiniMapHandler.h"

#define CHATBOX									(HUDHANDLER->GetChatBox())
#define PARTYHANDLER							(HUDHANDLER->GetPartyHandler())
#define INVENTORY								(HUDHANDLER->GetInventory())
#define MINIMAPHANDLER							(HUDHANDLER->GetMiniMapHandler())

class HUDController
{
enum EWindow1
{
	WINDOWID_Main,
	BUTTONID_LockItem,
	IMAGEID_LockItemView,
};

enum ETargetWindow1
{
	IMAGEID_LifeBar,
	IMAGEID_Type,
	IMAGEID_TargetInfo,
	IMAGEID_ClanIcon,

	TEXTID_Name,
	TEXTID_TargetInfo,
	TEXTID_ClanInfo,
	TEXTID_Life,
	TEXTID_Level,
	TEXTID_LifePercent,
	TEXTID_Range,
	TEXTID_MiscInfo,

	IMAGEID_ElementDef_Organic,
	IMAGEID_ElementDef_Earth,
	IMAGEID_ElementDef_Fire,
	IMAGEID_ElementDef_Ice,
	IMAGEID_ElementDef_Lightning,
	IMAGEID_ElementDef_Poison,
	IMAGEID_ElementDef_Water,
	IMAGEID_ElementDef_Wind,

	TEXTID_ElementDef_Organic,
	TEXTID_ElementDef_Earth,
	TEXTID_ElementDef_Fire,
	TEXTID_ElementDef_Ice,
	TEXTID_ElementDef_Lightning,
	TEXTID_ElementDef_Poison,
	TEXTID_ElementDef_Water,
	TEXTID_ElementDef_Wind,

	BUTTONID_More,

	DROPDOWNMENU_More,

	TOOLTIP_TargetType,

	WINDOWID_TargetInfo,
	WINDOWID_CharacterInfo,
};

enum ETargetType
{
	TARGETTYPE_Demon,
	TARGETTYPE_Mechanic,
	TARGETTYPE_Mutant,
	TARGETTYPE_Normal,
	TARGETTYPE_Undead,
	TARGETTYPE_NPC,
	TARGETTYPE_Player,
};

enum ERollDiceRanking
{
	LISTID_RankingLeft,
	LISTID_RankingRight,

	WINDOWID_Player1,
	WINDOWID_Player2,
	WINDOWID_Player3,
	WINDOWID_Player4,
	WINDOWID_Player5,

	TEXTID_CharacterNick1,
	TEXTID_CharacterNick2,
	TEXTID_CharacterNick3,
	TEXTID_CharacterNick4,
	TEXTID_CharacterNick5,

	TEXTID_Number1,
	TEXTID_Number2,
	TEXTID_Number3,
	TEXTID_Number4,
	TEXTID_Number5,
};

protected:
	void										OnSelectTargetMenu();
public:
	//Constructor
												HUDController();
	virtual									   ~HUDController();

	//Handler Functions
	BOOL										OnMouseClick( class CMouse * pcMouse );
	void										OnMouseMove( class CMouse * pcMouse );
	BOOL										OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL										OnMouseScroll( class CMouse * pcMouse );
	BOOL										OnKeyChar( class CKeyboard * pcKeyboard );

	void										OnLockButtonClick( UIEventArgs e );

	void										OnResolutionChanged();

	void										RenderFriendStatus();

	//Initialize Functions
	void										Init();
	void										Update( float fTime );
	void										Render();

	BOOL										IsLockItems();

	void										RenderHPCrystalTower( int iX, int iY, int iMin, int iMax );

	void										UpdateObjectsSettings();

	//Trade Request
	void										OnAcceptTradeRequest( int iTargetID );

	BOOL										IsWindowOldHUDOpen();

	void										ShowMessageToUser(std::string title, std::string message);

	//Target
	int											GetTargetID(){ return iTargetID; }
	BOOL										UpdateTargetInfo( UnitData * pcUnitData );
	void										SetRenderTarget( BOOL b );
	UnitData								  * GetTargetUnitData(){ return pcLastUnitDataTarget; }
	UI::Image								  * GetTargetTypeImage( int iIndex ){ return pcaImageTargetType[iIndex]; }

	//Dice Roll	Ranking
	void										OpenDiceRollRanking(){ if( pWindowRollDiceRanking )pWindowRollDiceRanking->Show(); }
	void										CloseDiceRollRanking(){ if( pWindowRollDiceRanking )pWindowRollDiceRanking->Hide(); }
	void										HandlePacket( PacketRollDiceRanking * psPacket );
	void										ClearRollDiceRanking( bool bResetVetor = false );
	void										UpdateRollDice(){ dwLastUpdateRollDice = GetTickCount(); }

	//Getters
	CChatBox 							      * GetChatBox() { return pcChatBox; };
	CPartyHandler							  * GetPartyHandler(){ return pcPartyHandler; }
	CInventory								  * GetInventory(){ return pcInventory; }
	CMiniMapHandler							  * GetMiniMapHandler(){ return pcMiniMapHandler; }

	UI::Window_ptr								GetTargetWindow(){ return pWindowTarget; }
private:
	//Target
	void										BuildTargetWindow1();

	//Display
	void										BuildDisplayWindow1();
	int											GetShortcutSkill( int iShortcut );

	//Dice Roll Ranking
	void										BuildDiceRollRankingWindow1();

	std::map<int, DWORD>						ClanIconRedownloadCheck;

private:
	int											iFontID = -1;

	UI::Window_ptr								pWindowInventory = NULL;
	UI::Window_ptr								pWindowTarget = NULL;
	UI::Window_ptr								pWindowDisplay = NULL;
	UI::Window_ptr								pWindowRollDiceRanking = NULL;

	BOOL										baFriendStatus[101];
	BOOL										bRenderTarget;
	int											iTargetID;
	UnitData								  * pcLastUnitDataTarget = NULL;
	BOOL										bLoadClan16 = FALSE;
	BOOL										bLoadClan32 = FALSE;

	CChatBox								  * pcChatBox = NULL;
	CPartyHandler							  * pcPartyHandler = NULL;
	CInventory								  * pcInventory = NULL;
	CMiniMapHandler							  * pcMiniMapHandler = NULL;

	UI::Tooltip_ptr								pcTooltipWorldTime;

	UI::Tooltip_ptr								pcTooltipBars[3];

	UI::Image								  * pcaImageCrystalTowerHPBar[2];
	UI::Image								  * pcaImageSkillsIcon[2][20];

	//Target
	UI::Image								  * pcaImageTargetType[7];

	UI::Image								  * pcaImageElementalDef[6];

	//Dice Roll Ranking
	std::vector<RollDiceRankingUser>			vRollDiceRanking;
	DWORD										dwLastUpdateRollDice = -1;
};

