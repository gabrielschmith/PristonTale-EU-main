#pragma once

struct EXESurfaceOld;

enum ESkinID
{
	SKINID_Undefined = -1,

	SKINID_Normal,
	SKINID_Blue,
	SKINID_Green,
	SKINID_Transparent,
	SKINID_Common,
	SKINID_Uncommon,
	SKINID_Rare,
	SKINID_Epic,
	SKINID_Legendary,
};

namespace UI
{
struct Balloon
{
	class Image						* pImageLeftTop;
	class Image						* pImageMiddleTop;
	class Image						* pImageRightTop;
	class Image						* pImageLeft;
	class Image						* pImageRight;
	class Image						* pImageLeftBottom;
	class Image						* pImageMiddleBottom;
	class Image						* pImageRightBottom;
	class Image						* pImageTailBottom;
	class Image						* pImageCenter;
};

enum EPositionText
{
	POSITIONTEXT_Top,
	POSITIONTEXT_Middle,
	POSITIONTEXT_Bottom,
};

struct ExtraText
{
	Text_ptr						pText;
	EPositionText					ePosition;
};

typedef std::shared_ptr<class MessageBalloon> MessageBallon_ptr;
class MessageBalloon : public Element
{
public:
	//Constructor
									MessageBalloon( Rectangle2D rRectangle );
	virtual						   ~MessageBalloon();

	void							Init();

	void							SetMessage( std::string strMessage );
	void							SetClanName( std::string strText );
	void							SetPetName( std::string strText, DWORD dwColor = -1 );
	void							AddExtraText( int iID, EPositionText ePosition, std::string strText, DWORD dwColor = -1 );
	void							AddCharTitleText(char* strText, int iRarity);
	void							ResetMessage();
	Text_ptr						GetTextMessage(){ return pTextMessage; }

	void							Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void							RenderDropItem( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void							DisplayBox ( BOOL bDisplay );

	void							SetHover( BOOL b ){ bHover = b; }
	void							SetColor( DWORD dwColor );

	void							IsCharacterMessage( BOOL b );
	void							SetSkin( ESkinID e ){ eSkinID = e; }

	void							SetBellatraIconID( int i ) { iBellatraCrown = i; }
	void							SetBellatraSoloIconID( short s ) { sBellatraSoloCrown = s; }
	void							SetCharacterClass( ECharacterClass eClass ) { eCharClass = eClass; }

	void							Update( float fTime );

	void							SetNotifyQuestIconID(EQuestIcon eIcon) { eNotifyQuest = eIcon; }

	void							SetClanIconTexture( EXESurfaceOld * ps );

	void							DrawMessageBalloonBox( int iX, int iY, int iWidth, int iHeight );
	void							DrawMessageBalloonBoxDropItem( int iX, int iY, int iWidth, int iHeight );
protected:
	const UINT						TIME_STRINGHASHBALLON_UPDATE = 15000;

	struct StringBalloonHash
	{
		std::string					strText;

		DWORD						dwTimeOut;

		int							iHighlightWidth;

		StringBalloonHash( const std::string _strText, int _iHighlightWidth, DWORD _dwTime )
		{
			strText = _strText;
			dwTimeOut = _dwTime;
			iHighlightWidth = _iHighlightWidth;
		};

		~StringBalloonHash() {};
	};


	Balloon							saMessagesBalloon[9][2];
	ESkinID							eSkinID = SKINID_Normal;


	BOOL							bHover = FALSE;
	BOOL							bIsMessage = FALSE;
	BOOL							bIsCharacterMessage = FALSE;
	BOOL							bClanName = FALSE;
	BOOL							bPetName = FALSE;
	BOOL							bHasTitle = FALSE;
	Text_ptr						pTextMessage;
	Text_ptr						pTextClanName;
	Text_ptr						pTextPetName;
	Text_ptr						pTextTitle;

	BOOL							bShowClanIcon = FALSE;
	EXESurfaceOld					* psTextureClanIcon;

	ECharacterClass					eCharClass = ECharacterClass::CHARACTERCLASS_None;
	int								iBellatraCrown = -1;
	short							sBellatraSoloCrown = -1;
	class Image						* pcaBellatraCrown[6];
	class Image						* pcaBellatraCrownSoloSword[3];
	class Image						* pcaClassIcon14Pixel[10];

	EQuestIcon						eNotifyQuest = EQuestIcon::None;
	class Image						* pcNotifyQuest[6];

	std::map<std::size_t, StringBalloonHash> vCache;
};
};