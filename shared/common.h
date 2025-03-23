#pragma once

#include "dice.h"

enum ETickType
{
	TICKTYPE_None,
	TICKTYPE_100MilliSecond,
	TICKTYPE_500MilliSecond,
	TICKTYPE_1Second,
	TICKTYPE_10Second,
	TICKTYPE_1Minute,
	TICKTYPE_1Hour,
};




#define	STG_ITEM_WAIT_TIME		(1000*60*4) //4 min
#define	STG_ITEM_WAIT_TIME_LOW	(1000*60*2)	//2 min
#define	CLIP_OUT				-32767
#define FLOATNS					8
#define	fONE					256
#define ITEM_SET_DIST			24
#define MONSPAWN_SET_DIST		96



#define KB1				(1ull * 1024ull)
#define KB2				(2ull * 1024ull)
#define KB4				(4ull * 1024ull)
#define KB8				(8ull * 1024ull)
#define KB16			(16ull * 1024ull)
#define KB32			(32ull * 1024ull)
#define KB64			(64ull * 1024ull)
#define KB128			(128ull * 1024ull)
#define KB192			(192ull * 1024ull)
#define KB256			(256ull * 1024ull)
#define KB512			(512ull * 1024ull)
#define MB1				(1ull * KB1 * KB1)
#define MB2				(2ull * KB1 * KB1)
#define MB4				(4ull * KB1 * KB1)
#define MB8				(8ull * KB1 * KB1)
#define GB1				(1ull * KB1 * MB1)
#define GB2				(2ull * KB1 * MB1)
#define GB4				(4ull * KB1 * MB1)
#define GB8				(8ull * KB1 * MB1)

#define ID				DWORD
#define ITEMCHECKSUM	DWORD

#define IMAGE_INACTIVE	0
#define IMAGE_ACTIVE	1
#define IMAGE_GRAY		2

#define RGBA(a,r,g,b)	((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))
#define ACOLOR			unsigned long
#define COLOR			unsigned char
#define COLORI			unsigned int
#define ALPHA_LOW		0x00
#define ALPHA_HIGH		0xFF

#define FILEEXIST		FileExist

#define GetAValue(rgb)	(LOBYTE((rgb)>>24))


constexpr int CastleBattleZoneAttacker_LineZ	= -24059 * fONE;												//The flag line - where attacks can be excuted
constexpr int CastleBattleZoneDamage_LineZ		= -23991 * fONE;												//Where targets can get damaged (To match client's 'Battle' zone text)
constexpr int CastleBattleZone_DoorLineZ		= -25545 * fONE;												//entrance to the castle gate
constexpr int CastleBattleZone_DoorLineX[2]		= { 34407 * fONE, 34695 * fONE };								//entrance to the castle gate
constexpr int CastleBattleZone_BridgeZ			= -25258 * fONE;												//bridge entrance
constexpr int CastleSoulFountain_Area[4]		= { 37040 * fONE, 37158 * fONE, -30286 * fONE, -30102 * fONE };	//Soul Fountain


BOOL	IsNull( const char * pszString );
BOOL	IsNull( int iValue );
void	padTo(std::string& str, const size_t num, const char paddingChar = ' ');
void	padToLeft(std::string& str, const size_t num, const char paddingChar = ' ');

void	ChangeFileExtension( const char * pszFile, const char * pszExt, char * pszBuffer );
void	GetDateTime( char * pszDateTime );
double  RoundOffToNearestSF( double N, double n );
double  RoundOffToNearestDP( double N, int dp );
float   RoundOffToNearestDP( float N, int dp );

std::string GetDayOfWeek( int iDay );

BOOL	FileExist( const std::string& name );

void		SetClipboardText( char * szText );
std::string	GetClipboardText();

int		GetDaysCountMonth( int iMonth, int iYear );
DWORD	SystemTimeToDWORD( SYSTEMTIME * ps );
DWORD	SystemTimeToDWORD( const SYSTEMTIME & ps );
DWORD	GetFileSize( const char * pszFile );



struct CurMax
{
	short	  sCur;
	short	  sMax;

	CurMax() {}
	CurMax( short sCur, short sMax ) : sCur(sCur), sMax(sMax) {}

	bool operator ==(const CurMax &o) { return (sCur == o.sCur) && (sMax == o.sMax); }
	bool operator !=(const CurMax &o) { return !(*this == o); }
};

struct UCurMax
{
	USHORT	  sCur;
	USHORT	  sMax;

	UCurMax() {}
	UCurMax( int sCur, int sMax ) : sCur(sCur), sMax(sMax) {}
};

struct MinMax
{
	short	  sMin;
	short	  sMax;

	MinMax() {}
	MinMax( int sMin, int sMax ) : sMin(sMin), sMax(sMax) {}

	short RandomValue() const { return Dice::RandomS( sMin, sMax ); }

	bool operator ==(const MinMax &o) { return (sMin == o.sMin) && (sMax == o.sMax); }
	bool operator !=(const MinMax &o) { return !(*this == o); }
};

struct IMinMax
{
	int		  iMin;
	int		  iMax;

	IMinMax() {}
	IMinMax( int iMin, int iMax ) : iMin(iMin), iMax(iMax) {}

	int RandomValue() const { return Dice::RandomI( iMin, iMax ); }
};

struct FMinMax
{
	float	  fMin;
	float	  fMax;

	FMinMax() {}
	FMinMax( float fMin, float fMax ) : fMin(fMin), fMax(fMax) {}

	float RandomValue() const { return Dice::RandomF( fMin, fMax ); }
};