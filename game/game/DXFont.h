#pragma once
#include "stdafx.h"

struct stFontStructure
{
	std::string							fname;
	ID3DXFont*							hFont;
	int									Width;
	int									Height;
	BOOL								italic;
	BOOL								bold;
	UINT								uWidthWhitespace;
};

class DXFont
{
private:
	std::vector<stFontStructure>		vFontResource;
public:
	void								Init();
	int									CreateFont( const char * FontName, int iHeight, int iWidth, BOOL Italic, BOOL Bold, int MemValue );
	int									GetWidthText( int FontObject, const char * Text, INT Size );
	int									GetHeightText( int FontObject, const char * Text, RECT rRect );
	int									GetHeightText( int FontObject, UINT uFormat, const char * Text, RECT rRect );
	int									GetHeightText( int iFontID );
	int									Draw( int FontObject, int x, int y, const std::string & pText, UINT uFormat, D3DCOLOR Color );
	int									Draw( int FontObject, RECT & rRect, LPCTSTR Text, INT Count, UINT uFormat, D3DCOLOR Color );
	int									Draw( int FontObject, RECT & rRect, const std::string & strText, UINT uFormat, D3DCOLOR Color );
	void								DrawBellatraFont( int x, int y, float Size, DWORD Color, const char * Text, ... );
	void								OnResetDevice( );
	void								OnLostDevice();
	void								ReCreateFontDevice( );
	virtual ~DXFont( );
};