#pragma once

#define DEFAULT_ITEMS_PER_COLUMN	5


#define ITEMTIMER_WIDTH		38
#define ITEMTIMER_HEIGHT	38

#define POS_ITMTMRBASEX				10
#define POS_ITMTMRBASEY				165
#define TMR_SPACEX					40
#define TMR_SPACEY					40


#define ITEM_TIMER(x)				(ITEMTIMERHANDLER->GetItemTimerByType(x))

class ItemTimer
{
public:
	EItemTimerType		iType;
	EItemID				iItemID;
	char				szName[32];
	char				szDescription[64];
	DWORD				dwTimeLeft;
	DWORD				dwTimeTotal;
	UI::Image			* pcImage;
	UI::Image			* pcImageBackground;

	std::vector<int>	vFreezeMaps;
	std::vector<int>	vDisableMaps;

	BOOL				bIsMouseHovering;
	BOOL				bFreeze;
	BOOL				bDisable;
	BOOL				bAlive;
	int					iFade;
	Rectangle2D			sPosition;
	Rectangle2D			sMoveTo;
	int					iColumn;

	static UI::Image	* pcImageBox;

public:
	ItemTimer( EItemTimerType iType, EItemID iItemID, char * pszName, char * pszDescription, DWORD dwTimeLeft, DWORD dwTimeTotal, UI::Image * pcImage, UI::Image * pcImageBackground );
	virtual ~ItemTimer();

	void				RenderTimer();
	void				RenderBox();
	void				Loop();
	void				Tick();
	void				OnMouseMove( Point2D * psPosition );

	BOOL				IsAlive() { return bAlive; }
	void				Die() { bAlive = FALSE; }

	void				AddFreezeMap( int iMapID ) { vFreezeMaps.push_back( iMapID ); }
	void				AddDisableMap ( int iMapID ) { vDisableMaps.push_back( iMapID ); }
	std::vector<int>	GetFreezeMaps() { return vFreezeMaps; }
	std::vector<int>	GetDisabledMaps() { return vDisableMaps; }
	void				CheckFreeze( int iMapID );
	void				CheckDisable( int iMapID );

	BOOL				Moving();

	BOOL				IsViewing() { return bIsMouseHovering; }
};


class CItemTimerHandler
{
private:

	std::vector<ItemTimer*> pcaItemTimer;
	std::vector<ItemTimer*> pcaItemTimerPrefab;

	DWORD				dwaMatForceOrbEffect[14] = { 0 };

	BOOL				CanStack( int iType );

	BOOL				IsHead( int iType );

	void				TimerExists( ItemTimer * pcTimer );
	void				OnClickCancelOK( ItemTimer * pcTimer, UIEventArgs eArgs );

public:
	CItemTimerHandler();
	virtual ~CItemTimerHandler();

	void				Init();

	void				Render();
	void				Loop();
	void				Tick();
	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );

	ItemTimer			* GetPrefab( EItemTimerType iType );

	ItemTimer			* GetItemTimerByType( int iType );

	ItemTimer			* GetHead();

	void				EventTimerStart( ItemTimer * pcTimer );
	void				EventTimerStop( ItemTimer * pcTimer );
	void				RemoveTimer( ItemTimer * pcTimer );

	void				UpdateTimerPositions( );

	void				OnResolutionChanged();

	BOOL				AddTimer( EItemTimerType iType, EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft );

	BOOL				LoadForceOrb( EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft );
	BOOL				SyncForceOrb( EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft );
	BOOL				CancelForceOrb( PacketCommand * packet );


	//Handlers
	void				HandlePacket( PacketNewItemTimer * psPacket );
	void				HandlePacket( PacketItemPremiumData * psPacket, BOOL bSyncOnly );
	void				HandlePacket( PacketOldHeadUse * psPacket );

	//Packet Process
	void				ProcessPacket( PacketOldHeadUse * psPacket );
	void				ProcessPacket( PacketNewItemTimer * psPacket );
	void				ProcessPacket( PacketCancelItemTimer * psPacket );

	void				UpdateOldHead();

};

