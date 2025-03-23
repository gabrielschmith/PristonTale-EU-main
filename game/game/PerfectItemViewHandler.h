#pragma once

class ItemData;

class CPerfectItemViewHandler
{
protected:
	EClassFlag											eClassFlag = EClassFlag::CLASSFLAG_Unknown;
	EClassFlag											eClassFlagReceived = EClassFlag::CLASSFLAG_Unknown;
	int													iItemRarity = -1;
	int													iItemRarityReceived = -1;
	DWORD												dwCode = 0;
	DWORD												dwCodeReceived = 0;
	short												sAgeLevel = 0;
	EMixTypeName										eMixTypeName = EMixTypeName::MIXTYPENAME_None;
	class ItemData										* pcPrefectItemData = NULL;
	int													iDefenseOverride = 0;
	BOOL												bVisible = FALSE;
	BOOL												bReceivedItem = FALSE;
public:
	CPerfectItemViewHandler();
	~CPerfectItemViewHandler();

	BOOL												IsSet() { return bVisible && bReceivedItem; };
	BOOL												IsItemPreviewable(ItemData* pcItemData);
	void												PHItemData( struct PacketItemPerfectView * psPacket );

	BOOL												OnKeyPress( class CKeyboard * pcKeyBoard );

	void												Render();

private:
	void												ProcessPacket( ItemData * pcItemData );

};

