#include "stdafx.h"
#include "common.h"

BOOL IsNull( const char * pszString )
{
	if( STRLEN( pszString ) <= 0 )
		return TRUE;

	if( STRINGCOMPARE( pszString, "NULL" ) )
		return TRUE;

	return FALSE;
}

BOOL IsNull( int iValue )
{
	if( !iValue )
		return TRUE;

	return FALSE;
}

//https://stackoverflow.com/questions/667183/padding-stl-strings-in-c
void padTo(std::string& str, const size_t num, const char paddingChar)
{
	if (num > str.size())
		str.insert(0, num - str.size(), paddingChar);
}

void padToLeft ( std::string & str, const size_t num, const char paddingChar )
{
	if ( num > str.size () )
		str.insert ( str.size (), num - str.size (), paddingChar );
}


//https://www.geeksforgeeks.org/round-off-number-given-number-significant-digits/
double RoundOffToNearestSF(double N, double n)
{
	if (n < 1)
		n = 1;

	int h;
	double b, c, d, e, i, j, m, f;
	b = N;
	c = floor(N);

	// Counting the no. of digits to the left of decimal point
	// in the given no.
	for (i = 0; b >= 1; ++i)
		b = b / 10;

	d = n - i;
	b = N;
	b = b * pow(10, d);
	e = b + 0.5;
	if ((float)e == (float)ceil(b)) {
		f = (ceil(b));
		h = static_cast<int>(f - 2);
		if (h % 2 != 0) {
			e = e - 1;
		}
	}
	j = floor(e);
	m = pow(10, d);
	j = j / m;

	return j;
}

double RoundOffToNearestDP(double N, int dp)
{
	double multiplier = pow(10, dp);
	double value = std::round(N * multiplier) / multiplier;
	return value;
}

float RoundOffToNearestDP(float N, int dp)
{
	double multiplier = pow(10, dp);
	float value = std::round(N * multiplier) / multiplier;
	return value;
}

void ChangeFileExtension( const char * pszFile, const char * pszExt, char * pszBuffer )
{
	int cnt, len;

	STRINGCOPYLEN( pszBuffer, STRLEN( pszFile ), pszFile );
	len = STRLEN( pszBuffer );

	for( cnt = len - 1; cnt >= 0; cnt-- )
	{
		if( pszBuffer[cnt] == '.' )
		{
			STRINGCOPYLEN( &pszBuffer[cnt + 1], STRLEN( pszExt ), pszExt );
			return;
		}
	}

	pszBuffer[len] = '.';
	STRINGCOPYLEN( pszBuffer + len + 1, STRLEN( pszExt ), pszExt );
}

void GetDateTime( char * pszDateTime )
{
	char szDate[9] = { 0 };
	char szTime[9] = { 0 };
	_strdate_s( szDate, 9 );
	_strtime_s( szTime, 9 );

	STRINGFORMATLEN( pszDateTime, 20, "%s %s", szDate, szTime );
}

std::string GetDayOfWeek( int iDay )
{
	switch( iDay )
	{
		case 0:
			return "Sunday";
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
	}

	return "";
}

BOOL FileExist( const std::string& name )
{
	FILE * fp = NULL;
	fopen_s( &fp, name.c_str(), "rb" );
	if( fp )
	{
		fclose( fp );
		return TRUE;
	}
	return FALSE;
}

void SetClipboardText( char * szText )
{
	HGLOBAL x;
	char *y;
	x = GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, 0x64 );
	y = ( char* )GlobalLock( x );
	strcpy_s( y, 0x64, szText );
	GlobalUnlock( x );
	OpenClipboard( NULL );
	EmptyClipboard();
	SetClipboardData( CF_TEXT, x );
	CloseClipboard();
}

std::string GetClipboardText()
{
	// Try opening the clipboard
	if( !OpenClipboard( nullptr ) )
	{
		return "";
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData( CF_TEXT );
	if( hData == nullptr )
	{
		return "";
	}

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>( GlobalLock( hData ) );
	if( pszText == nullptr )
	{
		return "";
	}

	// Save text in a string class instance
	std::string text( pszText );

	// Release the lock
	GlobalUnlock( hData );

	// Release the clipboard
	CloseClipboard();

	return text;
}

int GetDaysCountMonth( int iMonth, int iYear )
{
	if ( (iMonth == 4) || (iMonth == 6) || (iMonth == 9) || (iMonth == 11) )
		return 30;
	else if ( iMonth == 2 )
	{
		if ( ((iYear % 400) == 0) || (((iYear % 4) == 0) && ((iYear % 100) != 0)) )
			return 29;
		else
			return 28;
	}

	return 31;
}

DWORD SystemTimeToDWORD( SYSTEMTIME * ps )
{
	if( ps == NULL )
		return NULL;

	DWORD dw;
	int iDM = GetDaysCountMonth( ps->wMonth, ps->wYear );

	dw = ps->wSecond + (ps->wMinute * 60) + (ps->wHour * 60 * 60) + (ps->wDay * 60 * 60 * 24) + (ps->wMonth * 60 * 60 * 24 * iDM);

	return dw;
}

DWORD SystemTimeToDWORD( const SYSTEMTIME & ps )
{
	DWORD dw;
	int iDM = GetDaysCountMonth( ps.wMonth, ps.wYear );

	dw = ps.wSecond + (ps.wMinute * 60) + (ps.wHour * 60 * 60) + (ps.wDay * 60 * 60 * 24) + (ps.wMonth * 60 * 60 * 24 * iDM);

	return dw;
}

DWORD GetFileSize( const char * pszFile )
{
	DWORD dwSize = 0;
	FILE * fp = NULL;
	fopen_s( &fp, pszFile, "rb" );
	if( fp )
	{
		fseek( fp, 0, SEEK_END );
		dwSize = ftell( fp );
		fclose( fp );
	}
	return dwSize;
}