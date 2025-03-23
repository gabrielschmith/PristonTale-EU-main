#pragma once

namespace UI
{
typedef std::shared_ptr<class ItemInfoBox> ItemInfoBox_ptr;
class ItemInfoBox : public Element
{
public:
										ItemInfoBox();
	virtual								~ItemInfoBox();

	void								Clear() {}

	void								AddString( int iSide, const std::string & strString, DWORD dwColor = -1, BOOL bBold = FALSE, BOOL bItalic = FALSE );
	void								AddCompareString( const std::string & strString, DWORD dwColor = -1 );
	void								AppendLineText( std::string str, int iSide = -1, int iLineCount = -1 );
	void								SetLineColor( DWORD dwColor, int iSide = -1, int iLineCount = -1 );
	int									iBaseMaxWidth = -1;

	void								Init();

	void								Invalidate();
	void								ClearCompareItem();
	void								SetCompareItem( const ItemData * pcCompareItemData, BOOL bEquippedItem = FALSE );

	BOOL								IsViewingPerfectItem(){ return bIsViewingPerfectItem; }
	void								SetViewingPerfectItem( BOOL b ){ bIsViewingPerfectItem = b; }
	void								SetPerfectItemDefenseOverride ( int iValue ) { iDefenseOverride = iValue; };

	BOOL								IsBeginnerSet( ItemData * pcItemData );
	BOOL								IsSameItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice );

	BOOL								FormatItemInfo( ItemData * pcItemData );
	char*								GetItemTypeName ( EItemType eItemTYpe );
	char*								GetItemRarityName ( EItemRarity eItemRarity );
	char*								GetHanded(EItemSlotFlag eItemSlot);
	static DWORD						GetItemPlayTime( Item * psItem );
	static BOOL							IsExpiredItemTime( Item * psItemInfo );
	BOOL								FormatItemTimeInfo( ItemData * pcItemData );
	BOOL								PrepareShowItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice, BOOL bShowChecksums, BOOL isCompare = FALSE, int iBaseWidth = -1 );

	void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void								Update( float fTime );

	void								DrawItemInfoBox( int iX, int iY, int iWidth, int iHeight );

protected:
	//Images Box
	class Image							* pImageGradient;
	class Image							* pImagePKey;

	class Image							* pImageMaskLeftTop;
	class Image							* pImageMaskRightTop;
	class Image							* pImageMaskLeftBottom;
	class Image							* pImageMaskRightBottom;

	std::vector<class Image*>			vSheltomImages;

	class Image							* pImageBorderLeftTop;
	class Image							* pImageBorderRightTop;
	class Image							* pImageBorderTop;
	class Image							* pImageBorderLeftBottom;
	class Image							* pImageBorderRightBottom;
	class Image							* pImageBorderBottom;
	class Image							* pImageBorderLeft;
	class Image							* pImageBorderRight;

	class Image							* pImageBorderNoneLeftTop;
	class Image							* pImageBorderNoneRightTop;
	class Image							* pImageBorderNoneTop;
	class Image							* pImageBorderNoneRightBottom;
	class Image							* pImageBorderNoneLeftBottom;
	class Image							* pImageBorderNoneBottom;
	class Image							* pImageBorderNoneLeft;
	class Image							* pImageBorderNoneRight;

	class Image							* pImageBorderNoneDivider;
	class Image							* pImageBorderNoneInfo;
	class Image							* pImageBorderNoneInfoLeft;
	class Image							* pImageBorderNoneInfoRight;

	class Image							* pImageBorderNoneNameFrameLeft;
	class Image							* pImageBorderNoneNameFrameCenter;
	class Image							* pImageBorderNoneNameFrameRight;
	class Image							* pImageBorderNoneNameFrameBackground;

	class Image							* pImageBorderCommonLeftTop;
	class Image							* pImageBorderCommonRightTop;
	class Image							* pImageBorderCommonTop;
	class Image							* pImageBorderCommonRightBottom;
	class Image							* pImageBorderCommonLeftBottom;
	class Image							* pImageBorderCommonBottom;
	class Image							* pImageBorderCommonLeft;
	class Image							* pImageBorderCommonRight;

	class Image							* pImageBorderCommonDivider;
	class Image							* pImageBorderCommonInfo;
	class Image							* pImageBorderCommonInfoLeft;
	class Image							* pImageBorderCommonInfoRight;

	class Image							* pImageBorderCommonNameFrameLeft;
	class Image							* pImageBorderCommonNameFrameCenter;
	class Image							* pImageBorderCommonNameFrameRight;
	class Image							* pImageBorderCommonNameFrameBackground;

	class Image							* pImageBorderUncommonLeftTop;
	class Image							* pImageBorderUncommonRightTop;
	class Image							* pImageBorderUncommonTop;
	class Image							* pImageBorderUncommonRightBottom;
	class Image							* pImageBorderUncommonLeftBottom;
	class Image							* pImageBorderUncommonBottom;
	class Image							* pImageBorderUncommonLeft;
	class Image							* pImageBorderUncommonRight;

	class Image							* pImageBorderUncommonDivider;
	class Image							* pImageBorderUncommonCorner;
	class Image							* pImageBorderUncommonBottomCorner;
	class Image							* pImageBorderUncommonInfo;
	class Image							* pImageBorderUncommonInfoLeft;
	class Image							* pImageBorderUncommonInfoRight;

	class Image							* pImageBorderUncommonNameFrameLeft;
	class Image							* pImageBorderUncommonNameFrameCenter;
	class Image							* pImageBorderUncommonNameFrameRight;
	class Image							* pImageBorderUncommonNameFrameBackground;

	class Image							* pImageBorderRareLeftTop;
	class Image							* pImageBorderRareRightTop;
	class Image							* pImageBorderRareTop;
	class Image							* pImageBorderRareRightBottom;
	class Image							* pImageBorderRareLeftBottom;
	class Image							* pImageBorderRareBottom;
	class Image							* pImageBorderRareLeft;
	class Image							* pImageBorderRareRight;

	class Image							* pImageBorderRareDivider;
	class Image							* pImageBorderRareCorner;
	class Image							* pImageBorderRareBottomCorner;
	class Image							* pImageBorderRareInfo;
	class Image							* pImageBorderRareInfoLeft;
	class Image							* pImageBorderRareInfoRight;

	class Image							* pImageBorderRareNameFrameLeft;
	class Image							* pImageBorderRareNameFrameCenter;
	class Image							* pImageBorderRareNameFrameRight;
	class Image							* pImageBorderRareNameFrameBackground;

	class Image							* pImageBorderEpicLeftTop;
	class Image							* pImageBorderEpicRightTop;
	class Image							* pImageBorderEpicTop;
	class Image							* pImageBorderEpicRightBottom;
	class Image							* pImageBorderEpicLeftBottom;
	class Image							* pImageBorderEpicBottom;
	class Image							* pImageBorderEpicLeft;
	class Image							* pImageBorderEpicRight;

	class Image							* pImageBorderEpicDivider;
	class Image							* pImageBorderEpicCorner;
	class Image							* pImageBorderEpicBottomCorner;
	class Image							* pImageBorderEpicInfo;
	class Image							* pImageBorderEpicInfoLeft;
	class Image							* pImageBorderEpicInfoRight;

	class Image							* pImageBorderEpicNameFrameLeft;
	class Image							* pImageBorderEpicNameFrameCenter;
	class Image							* pImageBorderEpicNameFrameRight;
	class Image							* pImageBorderEpicNameFrameBackground;

	class Image							* pImageBorderLegendaryLeftTop;
	class Image							* pImageBorderLegendaryRightTop;
	class Image							* pImageBorderLegendaryTop;
	class Image							* pImageBorderLegendaryRightBottom;
	class Image							* pImageBorderLegendaryLeftBottom;
	class Image							* pImageBorderLegendaryBottom;
	class Image							* pImageBorderLegendaryLeft;
	class Image							* pImageBorderLegendaryRight;

	class Image							* pImageBorderLegendaryDivider;
	class Image							* pImageBorderLegendaryCorner;
	class Image							* pImageBorderLegendaryBottomCorner;
	class Image							* pImageBorderLegendaryInfo;
	class Image							* pImageBorderLegendaryInfoLeft;
	class Image							* pImageBorderLegendaryInfoRight;

	class Image							* pImageBorderLegendaryNameFrameLeft;
	class Image							* pImageBorderLegendaryNameFrameCenter;
	class Image							* pImageBorderLegendaryNameFrameRight;
	class Image							* pImageBorderLegendaryNameFrameBackground;

	//Aging
	class Image							* pImageAgingBar;
	class Image							* pImageAgingBarGage;
	class Image							* pImageAgingBarNum;

	//Socket
	class Image							* pImageSocketBackground;
	class Image							* paSocket[6];
	class Image							* paSocketGems[15];

	DX::RenderTarget_ptr				pRenderTargetMask = NULL;
	DX::Effect_ptr						pEffectMask;

	int									iDefenseOverride;
	int									iSocketGem[2];
	int									iSocketCount = 0;

	BOOL								isCompare = FALSE;
	int									iSpecLine = 0;
	int									iStatLine = 0;
#ifdef ITEMFIXES
	int									iFixLine = 0;
#endif

	BOOL								bFadeToColor = TRUE;
	float								fProgress = 0.0f;

	std::vector<Text_ptr>				vLeftText, vRightText, vCompareText;
	std::vector<int>					vRowCompare;
	Text_ptr							pTextAging, paSocketText[2][2];
	Text_ptr							paSheltomCountText[7];

	BOOL								bCompareItem = FALSE;
	BOOL								bIsEquippedItem = FALSE;
	BOOL								bCantUseItem = FALSE;
	BOOL								bIsPerfectItem = FALSE;
	BOOL								bHasLargeName = FALSE;
	BOOL								bDisplayHelp = FALSE;

	std::string							sItemName;
#ifdef ITEMFIXES
	char*								sPrefixName = "";
	char*								sSuffixName = "";
	BOOL								bHasPrefix = FALSE;
	BOOL								bHasSuffix = FALSE;
#endif

	BOOL								bIsViewingPerfectItem = FALSE;

	BOOL								bShowingBuyPrice = FALSE;
	BOOL								bShowingSellPrice = FALSE;

	int									iItemInfoBoxWidth = -1;

	ItemData							cItemWrapper;
	ItemData							cItemWrapperImage;
	ItemData							cItemCompare;

	UI::Color							cBackgroundColor1;
	UI::Color							cBackgroundColor2;

	static const DWORD					pdwItemInfoData = 0x03625788;

private:

	int									iPerivousItemID = 0;
	int									iPerivousItemImage = 0;
	int									iPerivousItemWidth = 0;
	int									iPerivousItemHeight = 0;

	std::map<int, std::vector<int>>		vAgeSheltoms;

	std::map<int, std::vector<int>>		vMixSheltoms;
	int									iSheltomYPosExtra;
};
}
