#include "stdafx.h"
#include "EXETexture.h"

NAKED void EXETextureManager::ReadTextures(int iMip)
{
	JMP( pfnReadTextures );
}

NAKED void EXETextureManager::Constructor( int _iMaxTextures )
{
	JMP( pfnConstructor );
}


EXETextureManager::EXETextureManager( int _iMaxTextures )
{
	Constructor( _iMaxTextures );
}

EXETextureManager::EXETextureManager()
{
}

UINT EXETextureManager::LoadedTexture( std::string strFile )
{
	for ( int i = 0; i < iTextureCount; i++ )
	{
		EXETextureMaterial * pc = pcaTexture + i;

		if ( pc->bInUse && pc->psTexture )
		{
			if ( STRINGCOMPAREI( pc->psTexture[0]->szName, strFile.c_str() ) )
				return (UINT)i;
		}
	}

	return (-1);
}

UINT EXETextureManager::LoadTexture( string strFilePath )
{
	UINT uIndex = LoadedTexture( strFilePath );

	//Already Loaded?
	if ( uIndex != -1 )
		return uIndex;

	uIndex = FreeTextureIndex();

	//No Free Positions?
	if ( uIndex == -1 )
		return 0;

	EXETextureMaterial * pTextureWrapper = pcaTexture + uIndex;

	pTextureWrapper->iTextureCounter = 1;
	pTextureWrapper->psTexture[0] = EXETexture::GetInstance()->Add( (char*)strFilePath.c_str() );

	if ( pTextureWrapper->psTexture[0] == NULL )
	{
		pTextureWrapper->iTextureCounter = 0;

		return 0;
	}

	EXETexture::LoadTexture( pTextureWrapper->psTexture[0], 0, (pTextureWrapper->psTexture[0]->bSwap == TRUE) );

	pTextureWrapper->bInUse = TRUE;

	return uIndex;
}

UINT EXETextureManager::FreeTextureIndex()
{
	for ( UINT i = 0; i < (UINT)iMaxTexture; i++ )
	{
		EXETextureMaterial * pTextureWrapper = pcaTexture + i;

		if ( pTextureWrapper->bInUse == FALSE )
		{
			iTextureCount++;

			return i;
		}
	}

	return -1;
}

NAKED EXETextureMaterial * EXETextureManager::GetTextureFromFilePath( const char * pszFilePath )
{
	JMP( pfnGetTextureFromFilePath );
}

int EXETextureManager::GetTextureMatNum( std::string strFile )
{
	int cnt2;

	for ( UINT i = 0; i < (UINT)iMaxTexture; i++ )
	{
		EXETextureMaterial * pTextureWrapper = pcaTexture + i;

		if ( pTextureWrapper->bInUse )
		{
			for ( cnt2 = 0; cnt2 < pTextureWrapper->iTextureCounter; cnt2++ )
			{
				if ( STRINGCOMPAREI( pTextureWrapper->psTexture[cnt2]->szName, strFile.c_str() ) )
					return i;
			}
		}
	}

	return 0;
}



int EXETexture::LoadTexture( EXETextureHandle * psTexture, bool bMipmap, bool bSwap )
{
	FILE * pFile = NULL;

	char * pBuffer = NULL;

	fopen_s( &pFile, psTexture->szName, "rb" );

	BOOL bRet = 0;

	if ( pFile )
	{
		fseek( pFile, 0, SEEK_END );
		int iSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		if ( iSize <= 0 )
		{
			fclose( pFile );
			return 0;
		}

		pBuffer = new char[iSize];

		fread( pBuffer, iSize, 1, pFile );

		fclose( pFile );

		//Get MipMap Count
		UINT uMipLevels = 0;

		//MipMap?
		if ( bMipmap == true )
		{
			char szFilePath[MAX_PATH] = { 0 };
			STRINGCOPY( szFilePath, psTexture->szName );

			const char * iDot = strrchr( szFilePath, '.' );

			if ( iDot != NULL )
			{
				char szBaseFile[MAX_PATH], szFileExtension[MAX_PATH];

				int iLenToDot = iDot - szFilePath;

				//Get Base File
				STRINGCOPY( szBaseFile, szFilePath );
				szBaseFile[iLenToDot] = 0;

				//Get File Extension
				STRINGCOPY( szFileExtension, szBaseFile + iLenToDot + 1 );

				//Find MipMap Levels
				for ( int i = 1; i <= 4; i++ )
				{
					char szBuffer[MAX_PATH];
					STRINGFORMAT( szBuffer, "%s_mm%d.%s", szBaseFile, i, szFileExtension );

					uMipLevels += PathFileExistsA( szBuffer ) ? 1 : 0;
				}
			}
		}

		//Get Extension
		std::string strFileExtension = GetFileExtension( psTexture->szName );

		D3DCOLOR d3dcolorkey = D3DCOLOR_ARGB( 0, 0, 0, 0 );

		if ( strFileExtension.compare( "tga" ) == 0 )
		{
			psTexture->iTransparency = TRUE;

			//Decrypt
			if ( DecryptTGA( pBuffer, iSize ) == false )
			{
				delete[] pBuffer;
				return 0;
			}
		}

		if ( strFileExtension.compare( "bmp" ) == 0 )
		{
			psTexture->iTransparency = FALSE;

			if ( bSwap == false )
				d3dcolorkey = D3DCOLOR_ARGB( 255, 0, 0, 0 );

			//Decrypt
			if ( DecryptBMP( pBuffer, iSize ) == false )
			{
				delete[] pBuffer;
				return 0;
			}
		}

		if ( strFileExtension.compare( "jpg" ) == 0 )
		{
			psTexture->iTransparency = FALSE;
		}

		if ( strFileExtension.compare( "png" ) == 0 )
		{
			psTexture->iTransparency = TRUE;
		}

		UINT uWidth		= D3DX_DEFAULT_NONPOW2;
		UINT uHeight	= D3DX_DEFAULT_NONPOW2;

		D3DXIMAGE_INFO ii;
		ZeroMemory( &ii, sizeof( D3DXIMAGE_INFO ) );

		if ( bSwap == true && (*(int*)0x03A9A334) >= 0 )
		{
			if ( FAILED( D3DXGetImageInfoFromFileInMemory( pBuffer, iSize, &ii ) ) )
			{
				delete[] pBuffer;
				return 0;
			}

			uWidth = ii.Width >> (*(int*)0x03A9A334);
			uHeight = ii.Height >> (*(int*)0x03A9A334);
		}

		LPDIRECT3DTEXTURE9 lpT = NULL;

		if ( FAILED( D3DXCreateTextureFromFileInMemoryEx(
			GRAPHICENGINE->GetDevice(),
			pBuffer,
			iSize,
			uWidth,
			uHeight,
			uMipLevels == 0 ? D3DX_FROM_FILE : uMipLevels + 1,
			0,
			D3DFMT_FROM_FILE,
			D3DPOOL_MANAGED,
			D3DX_FILTER_POINT,
			D3DX_DEFAULT,
			d3dcolorkey,
			&ii,
			NULL,
			&lpT ) ) )
		{
			psTexture->pcD3DTexture = lpT;
			delete[] pBuffer;
			return NULL;
		}

		psTexture->pcD3DTexture = lpT;

		psTexture->iWidth = ii.Width;
		psTexture->iHeight = ii.Height;

		delete[] pBuffer;

		bRet = 1;
	}

	return bRet;
}

BOOL EXETexture::ReadTextures()
{
	if ( ISTEXTUREREADY )
	{
		EnterCriticalSection( &cCriticalSection );

		for ( int i = 0; i < 5000; i++ )
		{
			EXETextureHandle * ps = &saTexture[i];

			if ( ps->iUseCounter && ps->pcD3DTexture == NULL )
			{
				//Load Texture
				EXETexture::LoadTexture( ps, 0, (ps->bSwap == TRUE) ? false : true );
			}
		}

		LeaveCriticalSection( &cCriticalSection );
	}

	return TRUE;
}

NAKED EXETextureHandle * EXETexture::Add( char * pszTexture, char * pszMask )
{
	JMP( 0x004892F0 );
}

int EXETexture::GetTextureIndex( std::string strFile )
{
	for ( int i = 0; i < 5000; i++ )
	{
		EXETextureHandle * ps = &saTexture[i];

		if ( ps->pcD3DTexture )
		{
			if ( STRINGCOMPAREI( ps->szName, strFile.c_str() ) )
				return i;
		}
	}

	return (-1);
}

EXETextureHandle * EXETexture::GetTexture( int iIndex )
{
	if ( iIndex >= 0 && iIndex < 5000 )
		return &saTexture[iIndex];

	return NULL;
}

BOOL EXETexture::DeleteSafe( EXETextureHandle * ps )
{
	if ( ps != NULL )
		return Delete( ps );

	return FALSE;
}

NAKED BOOL EXETexture::Delete( EXETextureHandle * ps )
{
	JMP( pfnDeleteTexture );
}

NAKED bool EXETexture::DecryptBMP( char * pBuffer, int iSize )
{
	__asm
	{
		MOV EAX, 0x0063C190;
		JMP EAX;
	}
}

NAKED bool EXETexture::DecryptTGA( char * pBuffer, int iSize )
{
	__asm
	{
		MOV EAX, 0x0063C1E0;
		JMP EAX;
	}
}
