#pragma once

#define MAX_CHATTEXTLIST			32

#define MAX_SAVECOMMANDSCHAT		10

#define MAX_LISTCHAT				7

#define MAX_EMOTELIST				20

#define TOTAL_HEIGHT				218

typedef std::map<std::string, std::string> FriendlyCommands;

struct ItemLinkData
{
	int								iID;

	std::deque<ItemData*>			vItemData;
};

class CChatBox
{
enum EChatBoxTabID
{
	CHATBOXTABID_All,
	CHATBOXTABID_Clan,
	CHATBOXTABID_Party,
	CHATBOXTABID_Raid,
	CHATBOXTABID_Trade,
	CHATBOXTABID_PM,
};
enum EElementsID
{
	//Windows
	WINDOWID_Main,

	//Lists
	LISTID_ChatAll,
	LISTID_ChatClan,
	LISTID_ChatParty,
	LISTID_ChatRaid,
	LISTID_ChatTrade,
	LISTID_ChatPM,
	LISTID_ChatNotice,

	//Button
	BUTTONID_TabAll,
	BUTTONID_TabClan,
	BUTTONID_TabParty,
	BUTTONID_TabRaid,
	BUTTONID_TabTrade,
	BUTTONID_TabPM,
	BUTTONID_SendMessage,
	BUTTONID_Discord,

	//Images
	IMAGEID_ChatBoxBar,

	//Input Field
	INPUTFIELDID_Message,
};
public:
									CChatBox();
	virtual						   ~CChatBox();

	void							Init();
	void							Render();

	void							AddDebugMessage( std::string strMessage );
	void							AddDebugMessage( const char* format, ...);
	void							AddMessage( std::string strMessage, EChatColor eChatColor = CHATCOLOR_Error, ItemData * pcItemData = NULL, BOOL bSkipDuplicate = FALSE );
	void							SendMessageChat( std::string strMessage );

	void							OnEnableChatBox();
	void							OnEnterKeyPressed( UIEventArgs eArgs );

	BOOL							IsInputFieldActive();
	void							SetInputFieldText( std::string strText );

	ItemData *						GetMouseItemHover();

	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );
	void							OnDiscordButtonClick ( UIEventArgs eArgs );
	void							OnDiscordButton ( UIEventArgs eArgs );
	BOOL							OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL							OnMouseScroll( class CMouse * pcMouse );
	BOOL							OnKeyChar( class CKeyboard * pcKeyboard );

	void							ToggleNotice( BOOL bOpen );

	void							HandlePacket( PacketChatBoxMessage * psPacket );
	void							HandlePacket( PacketItemLinkChat * psPacket );
	void							HandlePacket( PacketPlayerTitleList * psPacket );

	void							ProcessPacket( PacketItemLinkChat * psPacket, BOOL bLoginServer );

	UI::Window_ptr					GetWindow(){ return pWindow1; }

	void							OnResolutionChanged();

private:
	DWORD							GetMessageColorFromType( EChatColor eType );
	void							OnSelectChatBoxTab( int iID );

	void							BuildWindow();

	BOOL							IsEmoteMessage( std::string strMessage, std::string & strReplace, int & iEmoteIndex, EChatColor eChatColor = CHATCOLOR_Normal );

	BOOL							OnEmoteCommand( std::string & strMessage, UnitData * pcUnitData = NULL );

	void							CheckCollideWindow( BOOL bResolutionChanged );

	void							AddRecentPlayer( std::string strMessage );

	void							AddSaveCommand( std::string strMessage );
	std::string						GetSaveCommand( BOOL bUP );

	void							SendItemLink( ItemData * pcItemData );

	void							OnItemLinkHover( ItemData * pcItemData, UI::Window_ptr pWindowHover, UIEventArgs eArgs );

private:
	UI::Window_ptr					pWindow1;

	std::string						lastMessage;

	std::vector<std::string>		vSaveCommand;
	UINT							uSaveCommandWheel	= 0;

	DWORD							dwLastMessageTick = 0;
	int								iTotalMessages = 0;

	BOOL							bFirstDown = TRUE;
	int								iTabID = 0;
	FriendlyCommands				mFriendlyCommands;

	ItemLinkData					saItemLink[MAX_LISTCHAT];
	ItemData						cItemLinkDataWrapper;
	DWORD							dwDelayItemLinkTime = 0;
};

