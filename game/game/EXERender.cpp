#include "stdafx.h"
#include "EXERender.h"

WORD EXERender::saIndices[3 * 16384];
int EXERender::iIndexIndices = 0;
EXEModelCache * EXERender::pcModelCache;
DWORD EXERender::dwSamplerState = D3DTEXF_ANISOTROPIC;



//D3DVALUE * d3dRenderBuffer = (D3DVALUE*)0x2E45830;
//D3DVALUE d3dRenderBuffer2[16384 * 64 * 64];
D3DVALUE d3dRenderBuffer2[32767 * 64];
D3DVALUE* d3dRenderBuffer = d3dRenderBuffer2;


#define D3DFVF_TLVERTEX0		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX0
#define D3DFVF_TLVERTEX1		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX1
#define D3DFVF_TLVERTEX2		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX2
#define D3DFVF_TLVERTEX3		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX3
#define D3DFVF_TLVERTEX4		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX4
#define D3DFVF_TLVERTEX5		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX5
#define D3DFVF_TLVERTEX6		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX6
#define D3DFVF_TLVERTEX7		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX7
#define D3DFVF_TLVERTEX8		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX8

DWORD D3DFVF_TEXTURE[9] =
{
	D3DFVF_TLVERTEX0,
	D3DFVF_TLVERTEX1,
	D3DFVF_TLVERTEX2,
	D3DFVF_TLVERTEX3,
	D3DFVF_TLVERTEX4,
	D3DFVF_TLVERTEX5,
	D3DFVF_TLVERTEX6,
	D3DFVF_TLVERTEX7,
	D3DFVF_TLVERTEX8
};

DWORD D3DTLVERTEX_SIZE[9] = {
	sizeof( D3DFVF_TLVERTEX0 ),
	sizeof( D3DFVF_TLVERTEX1 ),
	sizeof( D3DFVF_TLVERTEX2 ),
	sizeof( D3DFVF_TLVERTEX3 ),
	sizeof( D3DFVF_TLVERTEX4 ),
	sizeof( D3DFVF_TLVERTEX5 ),
	sizeof( D3DFVF_TLVERTEX6 ),
	sizeof( D3DFVF_TLVERTEX7 ),
	sizeof( D3DFVF_TLVERTEX8 )
};

NAKED int EXERender::Init()
{
	JMP( pfnInit );
}

int EXERender::Render()
{

	int iRenderedCount = 0;

	int iaRenderLater[1024];

	int iRenderLaterCount = 0;

	ZeroMemory( iaRenderLater, _countof( iaRenderLater ) );

	int iCount = iMaterialListCount;

	if ( iCount < 0 )
		iCount = 0;

	if ( iCount > 5000 )
		iCount = 5000;

	if ( bRenderOutline )
	{
//		RenderGlow3D();
	}

	for ( int i = 0; i < iCount; i++ )
	{
		int iMaterialCounter = pdwRenderMaterialList[i];

		if ( iMaterialCounter <= iRenderMaterialCount )
		{
			if ( (psRenderMaterial[iMaterialCounter].iMeshState & 0x2000) == 0 )
				iRenderedCount += Render( iMaterialCounter );
			else
			{
				if ( iRenderLaterCount < 1024 )
					iaRenderLater[iRenderLaterCount++] = iMaterialCounter;
				else
					iRenderedCount += Render( iMaterialCounter );
			}
		}
	}

	for ( int i = iMaterialListRearCount+1; i < iRenderMaterialCount; i++ )
	{
		int iMaterialCounter = pdwRenderMaterialList[i];
		if ( iMaterialCounter <= iRenderMaterialCount )
		{
			
			if ( (psRenderMaterial[iMaterialCounter].iMeshState & 0x2000) == 0 )
				iRenderedCount += Render( iMaterialCounter );
			else
			{
				if ( iRenderLaterCount < 1024 )
					iaRenderLater[iRenderLaterCount++] = iMaterialCounter;
				else
					iRenderedCount += Render( iMaterialCounter );
			}
			
		}
	}

	for ( int i = 0; i < iRenderLaterCount; i++ )
		iRenderedCount += Render( iaRenderLater[i] );

	if ( ZWriteAuto )
		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	return iRenderedCount;
}

int EXERender::Render( int iMaterial )
{
	int iRenderedCount = 0;

	if ( iMaterial < 0 || iMaterial >= iRenderMaterialCount )
		return 0;

	EXETextureMaterial * psTextureMaterial = psRenderMaterial + iMaterial;

	if ( psTextureMaterial == NULL )
		return 0;

	//Textures Counter is 0?
	if ( psTextureMaterial->iTextureCounter == 0 && psTextureMaterial->iAnimatedTextureCounter == 0 )
		return 0;

	if ( iMaterial >= _countof( psaRenderMaterial ) )
		return 0;

	EXERenderMaterial * psMaterial = psaRenderMaterial + iMaterial;

	if ( psMaterial == NULL )
		return 0;


	short sColor[4] = { 0,0,0,0 };

	if ( psTextureMaterial->iUseState & 0x4000 )
		SetColor( sColor, psTextureMaterial );

	if ( psMaterial->iMaterialCounter )
	{
		int iStateVertex = 0;


		int iTextureCount = 0;

		//Set Texture Render State
		if ( GeTextureGameHandle() )
		{
			bRenderAlpha = SetStateRender( psTextureMaterial ); // << responsible for texture pattern ?
			iTextureCount = 1;
		}
		else
		{
			bRenderAlpha = SetStateRender( psTextureMaterial, FALSE );
		}

		//Set Render Buffer
		if ( psTextureMaterial->fSelfIllumination != 0.0f )
		{
			iRenderedCount = HandleVertexLight( d3dRenderBuffer, iMaterial, FALSE );
			iStateVertex = 1;
		}
		else if ( psTextureMaterial->iUseState & 0x4000 )
		{
			iRenderedCount = HandleVertexColor( d3dRenderBuffer, iMaterial, FALSE, sColor );
			iStateVertex = 2;
		}
		else
		{
			iRenderedCount = HandleVertex( d3dRenderBuffer, iMaterial, FALSE );
			iStateVertex = 3;
		}
		//Rendering Alpha?
		if ( bRenderAlpha )
		{
			SetStateRenderAlpha( psTextureMaterial, FALSE );
		}


		int iCounter = psTextureMaterial->iTextureCounter + iTextureCount;
		if ( iCounter < 0 || iCounter >= 9 )
			iCounter = 8;

		EXERender::iIndexIndices = 0;
		AddIndices( iRenderedCount * 3 );


		if( RENDERER->pcModelCache )
			RENDERER->pcModelCache->Render();
		else
			GRAPHICENGINE->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TEXTURE[iCounter], iRenderedCount, d3dRenderBuffer );
	}

	//Rendering Clipped Textures
	if ( psMaterial->iClipMaterialCounter )
	{
		bRenderAlpha = SetStateRender( psTextureMaterial, TRUE );

		//Set Render Buffer
		if ( psTextureMaterial->fSelfIllumination != 0.0f )
			iRenderedCount = HandleVertexLight( d3dRenderBuffer, iMaterial, TRUE );
		else if ( psTextureMaterial->iUseState & 0x4000 )
			iRenderedCount = HandleVertexColor( d3dRenderBuffer, iMaterial, TRUE, sColor );
		else
		{
			iRenderedCount = HandleVertex( d3dRenderBuffer, iMaterial, TRUE );
		}
		//Rendering Alpha?
		if ( bRenderAlpha )
		{
			SetStateRenderAlpha( psTextureMaterial, TRUE );
		}

		int iCounter = psTextureMaterial->iTextureCounter;
		if ( iCounter < 0 || iCounter >= 9 )
			iCounter = 8;

		EXERender::iIndexIndices = 0;
		AddIndices( iRenderedCount * 3 );


		if ( RENDERER->pcModelCache )
			RENDERER->pcModelCache->Render();
		else
			GRAPHICENGINE->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TEXTURE[iCounter], iRenderedCount, d3dRenderBuffer );
	}

	return iRenderedCount;
}

int EXERender::RenderGlow3D()
{
	int iRenderedCount = 0;

	GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_TEXTUREFACTOR, dwOutlineColor );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

	GRAPHICENGINE->GetDevice()->SetTexture( 1, NULL );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	GRAPHICENGINE->GetDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	GRAPHICENGINE->GetDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	GRAPHICENGINE->GetDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	GRAPHICENGINE->GetDevice()->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	GRAPHICENGINE->GetDevice()->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );

	for ( int i = 0; i < iMaterialListCount; i++ )
	{
		int iMaterial = pdwRenderMaterialList[i];

		if ( iRenderMaterialCount > iMaterial )
		{
			EXETextureMaterial * ps = psRenderMaterial + iMaterial;


			if ( ps->bTwoSided )
				GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			else
				GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

			if ( ps->psTexture[0] )
			{
				GRAPHICENGINE->GetDevice()->SetTexture( 0, ps->psTexture[0]->pcD3DTexture );

				if ( ps->psTexture[0]->iTransparency )
				{
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHAREF, iAlphaTestDepth );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
				}
				else
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			}

			switch ( ps->iBlendType )
			{
				case BLENDTYPE_None:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;

				case BLENDTYPE_Alpha:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					break;
				
				case BLENDTYPE_Color:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					break;
				
				case BLENDTYPE_AddColor:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;
				
				case BLENDTYPE_Shadow:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
					break;
				
				case BLENDTYPE_Lamp:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;
				
				case BLENDTYPE_InvShadow:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					break;

				default:
					break;
			}

			iRenderedCount += RenderGlow3D( iMaterial );
		}
	}

	for ( int i = iMaterialListRearCount + 1; i < iRenderMaterialCount; i++ )
	{
		int iMaterial = pdwRenderMaterialList[i];

		if ( iRenderMaterialCount > iMaterial )
		{
			EXETextureMaterial * ps = psRenderMaterial + iMaterial;


			if ( ps->bTwoSided )
				GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			else
				GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

			if ( ps->psTexture[0] )
			{
				GRAPHICENGINE->GetDevice()->SetTexture( 0, ps->psTexture[0]->pcD3DTexture );

				if ( ps->psTexture[0]->iTransparency )
				{
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHAREF, iAlphaTestDepth );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
				}
				else
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			}

			switch ( ps->iBlendType )
			{
				case BLENDTYPE_None:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;

				case BLENDTYPE_Alpha:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					break;

				case BLENDTYPE_Color:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					break;

				case BLENDTYPE_AddColor:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;

				case BLENDTYPE_Shadow:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
					break;

				case BLENDTYPE_Lamp:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					break;

				case BLENDTYPE_InvShadow:
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					break;

				default:
					break;
			}

			iRenderedCount += RenderGlow3D( iMaterial );
		}
	}

	return iRenderedCount;
}

int EXERender::RenderGlow3D( int iMaterial )
{
	//Keep it safe from exception
	if ( iMaterial >= iRenderMaterialCount )
		return 0;

	//Keep it safe from exception...
	if ( psRenderMaterial[iMaterial].iTextureCounter <= 0 && psRenderMaterial[iMaterial].iAnimatedTextureCounter <= 0 )
		return 0;

	EXEDrawVertexMinor * psaRender = (EXEDrawVertexMinor*)pRenderBuffer;
	int iRenderData = 0;


	//Max materials on Model data...
	if ( iMaterial >= 5000 )
		return 0;

	struct ColorGlow
	{
		union
		{
			DWORD		dwColor;
			BYTE		bColor[4];
		};
	};

	ColorGlow sColorData;
	sColorData.dwColor = 0;

	EXERenderFace * psRenderFace	= psaRenderMaterial[iMaterial].psRenderFace;
	int iMaterialCount				= psaRenderMaterial[iMaterial].iMaterialCounter;

	int iRenderCounter = 0;


	for ( int i = 0; i < iMaterialCount; i++ )
	{
		//Display?
		if ( (psRenderFace->dwClipAnd & 0x80) == 0 )
		{
			for ( int j = 0; j < 3; j++ )
			{
				EXERenderVertex * psrv = psRenderFace->psRenderVertex[j];

				EXETextureLink * pstl = (EXETextureLink*)psRenderFace->pcTextureLink;

				//Vertex Opaque color...
				EXEDrawVertexMinor * ps = psaRender + iRenderData;

				ps->x = psrv->xy[0];
				ps->y = psrv->xy[1];
				ps->z = psrv->zbuffer + (psrv->rwh / 2.0f);
			
				ps->rhw = psrv->rwh;

				//Lights on Game?
				if ( psRenderMaterial[iMaterial].fSelfIllumination != 0 )
				{
					//Use color but no alpha...
					ps->d3dColor = (*((DWORD*)psrv->bVertexColors)) | 0x00FFFFFF;
				}
				else
				{
					//Z Distance
					if ( bRenderAlpha && psrv->tz > ((int)READDWORD( 0x0082BF38 ) << 8) )
					{
						sColorData.dwColor = (*(DWORD*)psrv->bVertexColors);

						sColorData.bColor[3] = (sColorData.bColor[3] / 2) + (sColorData.bColor[3] / 3);

						ps->d3dColor = sColorData.dwColor;
					}
					else
						ps->d3dColor = (*(DWORD*)psrv->bVertexColors);
				}

				ps->u = pstl->u[j];
				ps->v = pstl->v[j];

				iRenderCounter++;

				iRenderData++;
			}
		}

		psRenderFace = psRenderFace->psNextRenderFace;
	}

	GRAPHICENGINE->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, iRenderCounter / 3, psaRender );

	return (iRenderCounter / 3);
}

void EXERender::SetGlow3D( BOOL bSelected, int iDistance, float fX, float fY, DWORD dwColor )
{
	bRenderOutline = bSelected;

	if ( bSelected == FALSE )
		return;

	if ( iDistance < 300 )
		fOutlineRatio = 1.0f;
	else if ( iDistance < 1300 )
		fOutlineRatio = 1.0f - (float)(iDistance - 300) / 1000.0f;
	else
		fOutlineRatio = 0;

	fOutlineX = fX;
	fOutlineY = fY;

	dwOutlineColor = dwColor;
}

BOOL EXERender::SetRenderClip( DWORD dwClip )
{
	dwClipAnd = dwClip;
	return TRUE;
}

NAKED BOOL EXERender::AddDynamicLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iRange )
{
	JMP( pfnAddDynamicLight );
}

BOOL EXERender::ClearLight()
{
	iLightCount = 0;
	return TRUE;
}

NAKED BOOL EXERender::AddMaterialRender( int iMaterialID )
{
	JMP( pfnAddMaterialRender );
}


NAKED EXERenderFace * EXERender::AddBillboard( Billboard * psBillboard )
{
	JMP( pfnAddBillboard );
}

NAKED EXERenderFace * EXERender::AddBillboardRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll )
{
	JMP( pfnAddBillboardRotation );
}

NAKED EXERenderFace * EXERender::AddBillboardHorizontal( Billboard * psBillboard )
{
	JMP( pfnAddBillboardHorizontal );
}

NAKED EXERenderFace * EXERender::AddBillboardHorizontalRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll )
{
	JMP( pfnAddBillboardHorizontalRotation );
}

EXERenderFace * EXERender::AddBillboardVertical( Billboard * psBillboard )
{
	return AddBillboardVerticalRotation( psBillboard, 0.0f, 0.0f, 0.0f );
}

NAKED EXERenderFace * EXERender::AddBillboardVerticalRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll )
{
	JMP( pfnAddBillboardVerticalRotation );
}

NAKED EXERenderFace * EXERender::AddFaceRender( EXERenderVertex * v0, EXERenderVertex * v1, EXERenderVertex * v2, int iMaterialID, EXETextureLink * psTexture )
{
	JMP( pfnAddFaceRender2 );
}

NAKED BOOL EXERender::AddTextureLinkRender( EXERenderFace * psFace, DWORD * pdw, float u0, float v0, float u1, float v1, float u2, float v2 )
{
	JMP( pfnAddTextureLinkRender );
}

NAKED EXERenderVertex * EXERender::AddObjectVertex( EXEVertex * psVertex, DWORD dwObjectCode )
{
	JMP( pfnAddVertexObject );
}

NAKED EXERenderFace * EXERender::AddObjectFace( EXEFace * psFace )
{
	JMP( pfnAddFaceObject );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderLeft( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderLeft );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderRight( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderRight );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderTop( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderTop );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderBottom( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderBottom );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderNearZ( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderNearZ );
}

NAKED EXERenderVertex * EXERender::AddVertexRenderFarZ( EXERenderFace * psFace, int iA, int iB, float * fMul )
{
	JMP( pfnAddVertexRenderFarZ );
}

BOOL EXERender::AddFaceRender( EXERenderFace * psFace, int iValue )
{
	if ( psFace == NULL )
		return FALSE;

	psFace->dwClipAnd |= 0x80;

	if ( psFace->psRenderVertex[0] == NULL || psFace->psRenderVertex[1] == NULL || psFace->psRenderVertex[2] == NULL )
		return FALSE;

	int iaClipAnd[3] = { 0,0,0 };

	iaClipAnd[0] = psFace->psRenderVertex[0]->dwClipAnd & iValue;
	iaClipAnd[1] = psFace->psRenderVertex[1]->dwClipAnd & iValue;
	iaClipAnd[2] = psFace->psRenderVertex[2]->dwClipAnd & iValue;

	if ( iaClipAnd[0] && iaClipAnd[1] && iaClipAnd[2] )
		return NULL;

	int iCounter	= 0;
	int iNext		= 0;
	int iLast		= 0;

	for ( iCounter = 0; iCounter < 3; iCounter++ )
	{
		iNext = (iCounter + 1) == 3 ? 0 : (iCounter + 1);
		
		if ( iaClipAnd[iCounter] == 0 && iaClipAnd[iNext] != 0 )
			break;
	}

	iLast = (iCounter - 1) < 0 ? 2 : (iCounter - 1);

	EXERenderVertex * psVertex[2] = { NULL,NULL };

	float fMul[2];
	
	if ( !iaClipAnd[iLast] )
	{
		switch ( iValue )
		{
				//Left
			case 4:
				psVertex[0] = AddVertexRenderLeft( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderLeft( psFace, iLast, iNext, &fMul[1] );
				break;
				//Right
			case 8:
				psVertex[0] = AddVertexRenderRight( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderRight( psFace, iLast, iNext, &fMul[1] );
				break;
				//Top
			case 0x10:
				psVertex[0] = AddVertexRenderTop( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderTop( psFace, iLast, iNext, &fMul[1] );
				break;
				//Bottom
			case 0x20:
				psVertex[0] = AddVertexRenderBottom( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderBottom( psFace, iLast, iNext, &fMul[1] );
				break;
				//Near
			case 1:
				psVertex[0] = AddVertexRenderNearZ( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderNearZ( psFace, iLast, iNext, &fMul[1] );
				break;
				//Far
			case 2:
				psVertex[0] = AddVertexRenderFarZ( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderFarZ( psFace, iLast, iNext, &fMul[1] );
				break;
		}

		if ( psVertex[0] == NULL || psVertex[1] == NULL )
			return FALSE;

		EXERenderFace * psaRender[2] = { NULL,NULL };

		if ( psFace->iMaterialID < iRenderMaterialCount )
		{
			psaRender[0] = AddFaceRender( psFace->psRenderVertex[iCounter], psVertex[0], psFace->psRenderVertex[iLast], psFace->iMaterialID, NULL );
			psaRender[1] = AddFaceRender( psVertex[0], psVertex[1], psFace->psRenderVertex[iLast], psFace->iMaterialID, NULL );
		}

		EXETextureLink * psTextureLink = (EXETextureLink*)psFace->pcTextureLink;
		if ( psTextureLink && psaRender[0] && psaRender[1] )
		{
			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;

			for ( int i = 0; i < 8; i++ )
			{
				u0 = psTextureLink->u[iCounter] + (psTextureLink->u[iNext] - psTextureLink->u[iCounter]) * fMul[0];
				v0 = psTextureLink->v[iCounter] + (psTextureLink->v[iNext] - psTextureLink->v[iCounter]) * fMul[0];
				u1 = psTextureLink->u[iLast] + (psTextureLink->u[iNext] - psTextureLink->u[iLast]) * fMul[1];
				v1 = psTextureLink->v[iLast] + (psTextureLink->v[iNext] - psTextureLink->v[iLast]) * fMul[1];

				AddTextureLinkRender( psaRender[0], psTextureLink->hTexture, psTextureLink->u[iCounter], psTextureLink->v[iCounter], u0, v0, psTextureLink->u[iLast], psTextureLink->v[iLast] );
				AddTextureLinkRender( psaRender[1], psTextureLink->hTexture, u0, v0, u1, v1, psTextureLink->u[iLast], psTextureLink->v[iLast] );

				psTextureLink = psTextureLink->hNextTexture;

				if ( psTextureLink == NULL )
					break;
			}
		}
	}
	else
	{
		switch ( iValue )
		{
				//Left
			case 4:
				psVertex[0] = AddVertexRenderLeft( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderLeft( psFace, iCounter, iLast, &fMul[1] );
				break;
				//Right
			case 8:
				psVertex[0] = AddVertexRenderRight( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderRight( psFace, iCounter, iLast, &fMul[1] );
				break;
				//Top
			case 0x10:
				psVertex[0] = AddVertexRenderTop( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderTop( psFace, iCounter, iLast, &fMul[1] );
				break;
				//Bottom
			case 0x20:
				psVertex[0] = AddVertexRenderBottom( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderBottom( psFace, iCounter, iLast, &fMul[1] );
				break;
				//Near
			case 1:
				psVertex[0] = AddVertexRenderNearZ( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderNearZ( psFace, iCounter, iLast, &fMul[1] );
				break;
				//Far
			case 2:
				psVertex[0] = AddVertexRenderFarZ( psFace, iCounter, iNext, &fMul[0] );
				psVertex[1] = AddVertexRenderFarZ( psFace, iCounter, iLast, &fMul[1] );
				break;
		}

		EXERenderFace * psRender = NULL;

		if ( psFace->iMaterialID < iRenderMaterialCount )
		{
			psRender = AddFaceRender( psFace->psRenderVertex[iCounter], psVertex[0], psVertex[1], psFace->iMaterialID, NULL );
		}

		EXETextureLink * psTextureLink = (EXETextureLink*)psFace->pcTextureLink;
		if ( psTextureLink && psRender )
		{
			float u0 = 0.0f;
			float u1 = 0.0f;
			float v0 = 0.0f;
			float v1 = 0.0f;

			for ( int i = 0; i < 8; i++ )
			{
				u0 = psTextureLink->u[iCounter] + (psTextureLink->u[iNext] - psTextureLink->u[iCounter]) * fMul[0];
				v0 = psTextureLink->v[iCounter] + (psTextureLink->v[iNext] - psTextureLink->v[iCounter]) * fMul[0];
				u1 = psTextureLink->u[iCounter] + (psTextureLink->u[iLast] - psTextureLink->u[iCounter]) * fMul[1];
				v1 = psTextureLink->v[iCounter] + (psTextureLink->v[iLast] - psTextureLink->v[iCounter]) * fMul[1];

				AddTextureLinkRender( psRender, psTextureLink->hTexture, psTextureLink->u[iCounter], psTextureLink->v[iCounter], u0, v0, u1, v1 );

				psTextureLink = psTextureLink->hNextTexture;

				if ( psTextureLink == NULL )
					break;
			}
		}
	}

	return TRUE;
}

NAKED BOOL EXERender::Vertex2DGeometry( int iColor )
{
	JMP( pfnVertex2DGeo );
}

NAKED int EXERender::GetRectangle2D( int iX, int iY, int iZ, int iWidth, int iHeight, RECT * psRectangle2D, Point2D * psPosition )
{
	JMP( pfnGetRectangle2D );
}

//TextureType
#define SMTEX_TYPE_MULTIMIX		0x0000
#define SMTEX_TYPE_ANIMATION	0x0001

#define SMMAT_BLEND_NONE		0x00
#define SMMAT_BLEND_ALPHA		0x01
#define SMMAT_BLEND_COLOR		0x02
#define SMMAT_BLEND_SHADOW		0x03
#define SMMAT_BLEND_LAMP		0x04
#define SMMAT_BLEND_ADDCOLOR	0x05
#define SMMAT_BLEND_INVSHADOW	0x06

NAKED BOOL EXERender::SetStateRender( EXETextureMaterial * lpMaterial)
{
	JMP( pfnSetStateRender );
	/*DWORD cnt;
	int	MapOpacity;
	int MapDualRend;

	MapOpacity = 0;
	MapDualRend = 0;

	if (lpMaterial->iTextureType == SMTEX_TYPE_MULTIMIX)
	{
		//
		for (cnt = 0; cnt < 2; cnt++)
		{
			//
			if (cnt == 0)
				iTextureModifier[cnt] = lpMaterial->iaTextureFormState[cnt];
			else // if( cnt == 1 )
				iTextureModifier[cnt] = READDWORD(0x02E45818);	//s_Item2PassTexFormState

			//
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, cnt);

			if (cnt == 0)
				GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_MODULATE);
			else // if( cnt == 1 )				
				GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_ADD);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

			//
			if (cnt == 0)
			{
				auto text1 = lpMaterial->psTexture[cnt]->pcD3DTexture;
				GRAPHICENGINE->GetDevice()->SetTexture(cnt, text1);
			}
			else // if( cnt == 1 )
			{
				auto text2 = (*(EXETextureHandle**)0x02E4581C)->pcD3DTexture;
				GRAPHICENGINE->GetDevice()->SetTexture(cnt, text2);
			}
			//
			if (cnt == 0)
			{
				if (lpMaterial->psTexture[cnt]->iTransparency)
					MapOpacity += lpMaterial->psTexture[cnt]->iTransparency;
			}
		}

		//
		for (cnt = 2; cnt < 8; cnt++)
		{
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_POINT);

			GRAPHICENGINE->GetDevice()->SetTexture(cnt, 0);
		}
	}
	else
	{
		for (cnt = 0; cnt < 8; cnt++)
		{
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			GRAPHICENGINE->GetDevice()->SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			GRAPHICENGINE->GetDevice()->SetSamplerState(cnt, D3DSAMP_MINFILTER, D3DTEXF_POINT);

			GRAPHICENGINE->GetDevice()->SetTexture(cnt, 0);
		}
	}
	

	GRAPHICENGINE->GetDevice()->SetRenderState(D3DRENDERSTATETYPE::D3DRS_BLENDOPALPHA, D3DBLEND::D3DBLEND_ONE);

	switch (lpMaterial->iBlendType)
	{
	case SMMAT_BLEND_NONE:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		break;
	case SMMAT_BLEND_ALPHA:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		break;
	case SMMAT_BLEND_COLOR:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_ADDCOLOR:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_SHADOW:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		break;

	case SMMAT_BLEND_LAMP:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case SMMAT_BLEND_INVSHADOW:
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		break;

	}

	if (lpMaterial->bTwoSided)
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	if (MapOpacity)
	{
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHAREF, iAlphaTestDepth);
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	else
	{
		GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	if (ZWriteAuto)
	{
		if (lpMaterial->fOpacity > 0.2f)
			GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		else
			GRAPHICENGINE->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	return MapDualRend;*/

}

NAKED BOOL EXERender::SetStateRender( EXETextureMaterial * psTextureMaterial, BOOL bClip )
{
	JMP( pfnSetStateRenderClip );
}

NAKED BOOL EXERender::SetStateRenderAlpha( EXETextureMaterial * psTextureMaterial, BOOL bClip )
{
	JMP( pfnSetStateRenderAlpha );
}

void EXERender::SetColor( short * psColor, EXETextureMaterial * psTextureMaterial )
{
	int iRed		= (int)(psTextureMaterial->fDiffuseColorR * 255.0f);
	int iGreen		= (int)(psTextureMaterial->fDiffuseColorG * 255.0f);
	int iBlue		= (int)(psTextureMaterial->fDiffuseColorB * 255.0f);

	int iTime		= 1 << psTextureMaterial->iWindMesh;
	
	int iMask		= iTime - 1;

	int iTimeValue	= TICKCOUNT & iMask;

	if ( (TICKCOUNT & iTime) == 0 )
		iTimeValue	= iMask - iTimeValue;

	psColor[2]		= (short)((iTimeValue * iRed) >> psTextureMaterial->iWindMesh);
	psColor[1]		= (short)((iTimeValue * iGreen) >> psTextureMaterial->iWindMesh);
	psColor[0]		= (short)((iTimeValue * iBlue) >> psTextureMaterial->iWindMesh);

	psColor[3]		= 0;
}

BOOL EXERender::SetColorVertex( EXERenderVertex * rv, short sColors[4], BYTE Color[4], BYTE Specular[4] )
{
	short BlinkColor[4] = { 0, };
	CopyMemory( BlinkColor, sColors, sizeof( BlinkColor ) );

	int z = rv->tz >> 8;
	if ( z > ZDISTANCEMIN )
	{
		int lev = ((z - ZDISTANCEMIN) << (13 - 6)) >> 8;
		if ( lev >= 256 )
			return FALSE;

		BlinkColor[2] -= ((sColors[2] * lev) >> 8);
		BlinkColor[1] -= ((sColors[1] * lev) >> 8);
		BlinkColor[0] -= ((sColors[0] * lev) >> 8);
		BlinkColor[3] -= ((sColors[2] * lev) >> 8);
	}

	for ( int i = 0; i < 4; i++ )
	{
		BlinkColor[i] += rv->bVertexColors[i];
		if ( BlinkColor[i] > 255 )
		{
			short spe = rv->bSpecular[i] + (BlinkColor[i] - 256);
			Color[i] = 255;
			if ( spe > 255 )
				Specular[i] = 255;
			else
				Specular[i] = (BYTE)spe;
		}
		else
		{
			Color[i] = (BYTE)BlinkColor[i];
			Specular[i] = rv->bSpecular[i];
		}
	}

	return TRUE;
}

void EXERender::AddIndices( int iCount )
{
	if ( EXERender::iIndexIndices >= ((3 * 16384)-3) )
		return;

	int iIndex = EXERender::iIndexIndices;

	for( int i = 0; i < iCount; i++ )
		EXERender::saIndices[EXERender::iIndexIndices++] = iIndex++;
}

NAKED int EXERender::HandleVertexLight( void * pRenderBuffer, int iMaterialID, BOOL bClip )
{
	JMP( pfnHandleVertexLight );
/*
	EXERenderFace * psFace	= NULL;

	int iMaterialCount		= 0;

	int iRenderedCount		= 0;

	int wtime				= (iRenderTime >> 6) & 0xFF;
	float fwtime			= (float)wtime / 256;

	EXEDrawVertex * psaRenderVertex = (EXEDrawVertex*)pRenderBuffer;

	if ( bClip )
	{
		psFace			= psaRenderMaterial[iMaterialID].psClipRenderFace;
		iMaterialCount	= psaRenderMaterial[iMaterialID].iClipMaterialCounter;
	}
	else
	{
		psFace			= psaRenderMaterial[iMaterialID].psRenderFace;
		iMaterialCount	= psaRenderMaterial[iMaterialID].iMaterialCounter;
	}

	for ( int i = 0; i < iMaterialCount; i++ )
	{
		if ( (psFace->dwClipAnd & 0x80) == 0 )
		{
			int iA = 0; int iB = 0; int iC = 0;

			for ( int j = 0; j < 3; j++ )
			{
				EXERenderVertex * ps = psFace->psRenderVertex[j];

				EXEDrawVertex * psVertexDraw = psaRenderVertex + iRenderedCount;

				psVertexDraw->x		= ps->xy[0];
				psVertexDraw->y		= ps->xy[1];
				psVertexDraw->z		= ps->zbuffer;
				psVertexDraw->rhw	= ps->rwh;

				psVertexDraw->d3dColor = ps->dwVertexColor | 0x00FFFFFF;
				psVertexDraw->d3dSpecular = 0;

				EXETextureLink * tl = (EXETextureLink*)psFace->pcTextureLink;

				for ( int k = 0; k < 8; k++ )
				{
					if ( tl == NULL )
					{
						if ( k == 1 && (*(EXETextureHandle**)0x02E4581C) )
							tl = (EXETextureLink*)psFace->pcTextureLink;
						else
							break;
					}

					psVertexDraw->u = tl->u[j];
					psVertexDraw->v = tl->v[j];

					if ( iTextureModifier[k] )
					{
						switch ( iTextureModifier[k] )
						{
							case TEXTUREMODIFIER_None:
							case TEXTUREMODIFIER_FormX:
							case TEXTUREMODIFIER_FormY:
							case TEXTUREMODIFIER_FormZ:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Scroll:
								psVertexDraw->u = tl->u[j] + fwtime;
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Reflex:
								float fEx;
								float fEy;
								float fEz;

								float fLen;

								float fNx;
								float fNy;
								float fNz;

								float fDotNum;
								float fx;
								float fy;

								fEx = (float)ps->tx / 256.0f;
								fEy = (float)ps->ty / 256.0f;
								fEz = (float)ps->tz / 256.0f;

								fLen = (float)sqrt( (fEx*fEx) + (fEy*fEy) + (fEz*fEz) );
								fEx /= fLen;
								fEy /= fLen;
								fEz /= fLen;

								fNx = float( ps->nx - ps->tx ) / 32767.0f;
								fNy = float( ps->ny - ps->ty ) / 32767.0f;
								fNz = float( ps->nz - ps->tz ) / 32767.0f;

								fDotNum = (fEx*fNx) + (fEy*fNy) + (fEz*fNz);
								fx = fEx - (fDotNum * fNx * 2);
								fy = fEy - (fDotNum * fNy * 2);

								psVertexDraw->u = (fx * 0.5f) + 0.5f;
								psVertexDraw->v = (-fy * 0.5f) + 0.5f;
								break;

							case TEXTUREMODIFIER_Scroll2:
							case TEXTUREMODIFIER_Scroll3:
							case TEXTUREMODIFIER_Scroll4:
							case TEXTUREMODIFIER_Scroll5:
							case TEXTUREMODIFIER_Scroll6:
							case TEXTUREMODIFIER_Scroll7:
							case TEXTUREMODIFIER_Scroll8:
							case TEXTUREMODIFIER_Scroll9:
							case TEXTUREMODIFIER_Scroll10:
								psVertexDraw->u = (tl->u[j] + fwtime * (iTextureModifier[k] - (TEXTUREMODIFIER_Scroll2 - 2)));
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_ScrollSlow1:
							case TEXTUREMODIFIER_ScrollSlow2:
							case TEXTUREMODIFIER_ScrollSlow3:
							case TEXTUREMODIFIER_ScrollSlow4:
								int iFactor;
								int wtime2;
								float fwtime2;

								iFactor = TEXTUREMODIFIER_ScrollSlow4 - iTextureModifier[k] + 4;

								wtime2 = (iRenderTime >> 6) & (0xFFFF >> iFactor);
								fwtime2 = (float)wtime2 / (float)(0xFFFF >> iFactor);

								psVertexDraw->u = tl->u[j] + fwtime2;
								psVertexDraw->v = tl->v[j];
								break;

							default:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;
						}
					}

					tl = tl->hNextTexture;
				}

				iRenderedCount++;
			}

			AddIndices( 3 );
		}

		psFace = psFace->psNextRenderFace;
	}
	
	return iRenderedCount / 3;
	*/
}

NAKED int EXERender::HandleVertex( void * pRenderBuffer, int iMaterialID, BOOL bClip )
{
	JMP( pfnHandleVertex );
/*	
	EXERenderFace * psFace = NULL;

	int iMaterialCount = 0;

	int iRenderedCount = 0;

	int wtime = (iRenderTime >> 6) & 0xFF;
	float fwtime = (float)wtime / 256;

	EXEDrawVertex * psaRenderVertex = (EXEDrawVertex*)pRenderBuffer;

	int iVertices = 0;

	if ( bClip )
	{
		psFace = psaRenderMaterial[iMaterialID].psClipRenderFace;
		iMaterialCount = psaRenderMaterial[iMaterialID].iClipMaterialCounter;
	}
	else
	{
		psFace = psaRenderMaterial[iMaterialID].psRenderFace;
		iMaterialCount = psaRenderMaterial[iMaterialID].iMaterialCounter;
	}

	for ( int i = 0; i < iMaterialCount; i++ )
	{
		if ( (psFace->dwClipAnd & 0x80) == 0 )
		{
			for ( int j = 0; j < 3; j++ )
			{
				EXERenderVertex * ps = psFace->psRenderVertex[j];

				EXEDrawVertex * psVertexDraw = psaRenderVertex + iVertices;

				psVertexDraw->x = ps->xy[0];
				psVertexDraw->y = ps->xy[1];
				psVertexDraw->z = ps->zbuffer;
				psVertexDraw->rhw = ps->rwh;

				if ( bRenderAlpha && ps->tz > (ZDISTANCEMIN << 8) )
				{
					BYTE baColor[4];
					BYTE baSpecular[4];
					
					WRITEDWORD( baColor, ps->dwVertexColor );
					WRITEDWORD( baSpecular, ps->dwSpecular );

					baSpecular[3] = baSpecular[3] / 2 + baSpecular[3] / 3;
					baColor[3] = baColor[3] / 2 + baColor[3] / 3;

					psVertexDraw->d3dColor = READDWORD( baColor );
					psVertexDraw->d3dSpecular = READDWORD( baSpecular );
				}
				else
				{
					psVertexDraw->d3dColor = ps->dwVertexColor;
					psVertexDraw->d3dSpecular = ps->dwSpecular;
				}

				EXETextureLink * tl = (EXETextureLink*)psFace->pcTextureLink;

				for ( int k = 0; k < 8; k++ )
				{
					if ( tl == NULL )
					{
						if ( k == 1 && (*(EXETextureHandle**)0x02E4581C) )
							tl = (EXETextureLink*)psFace->pcTextureLink;
						else
							continue;
					}

					psVertexDraw->u = tl->u[j];
					psVertexDraw->v = tl->v[j];

					if ( iTextureModifier[k] )
					{
						switch ( iTextureModifier[k] )
						{
							case TEXTUREMODIFIER_None:
							case TEXTUREMODIFIER_FormX:
							case TEXTUREMODIFIER_FormY:
							case TEXTUREMODIFIER_FormZ:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Scroll:
								psVertexDraw->u = tl->u[j] + fwtime;
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Reflex:
								float fEx;
								float fEy;
								float fEz;

								float fLen;

								float fNx;
								float fNy;
								float fNz;

								float fDotNum;
								float fx;
								float fy;

								fEx = (float)ps->tx / 256.0f;
								fEy = (float)ps->ty / 256.0f;
								fEz = (float)ps->tz / 256.0f;

								fLen = (float)sqrt( (fEx*fEx) + (fEy*fEy) + (fEz*fEz) );
								fEx /= fLen;
								fEy /= fLen;
								fEz /= fLen;

								fNx = float( ps->nx - ps->tx ) / 32767.0f;
								fNy = float( ps->ny - ps->ty ) / 32767.0f;
								fNz = float( ps->nz - ps->tz ) / 32767.0f;

								fDotNum = (fEx*fNx) + (fEy*fNy) + (fEz*fNz);
								fx = fEx - (fDotNum * fNx * 2);
								fy = fEy - (fDotNum * fNy * 2);

								psVertexDraw->u = (fx * 0.5f) + 0.5f;
								psVertexDraw->v = (-fy * 0.5f) + 0.5f;
								break;

							case TEXTUREMODIFIER_Scroll2:
							case TEXTUREMODIFIER_Scroll3:
							case TEXTUREMODIFIER_Scroll4:
							case TEXTUREMODIFIER_Scroll5:
							case TEXTUREMODIFIER_Scroll6:
							case TEXTUREMODIFIER_Scroll7:
							case TEXTUREMODIFIER_Scroll8:
							case TEXTUREMODIFIER_Scroll9:
							case TEXTUREMODIFIER_Scroll10:
								psVertexDraw->u = (tl->u[j] + fwtime * (iTextureModifier[k] - (TEXTUREMODIFIER_Scroll2 - 2)));
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_ScrollSlow1:
							case TEXTUREMODIFIER_ScrollSlow2:
							case TEXTUREMODIFIER_ScrollSlow3:
							case TEXTUREMODIFIER_ScrollSlow4:
								int iFactor;
								int wtime2;
								float fwtime2;

								iFactor = TEXTUREMODIFIER_ScrollSlow4 - iTextureModifier[k] + 4;

								wtime2 = (iRenderTime >> 6) & (0xFFFF >> iFactor);
								fwtime2 = (float)wtime2 / (float)(0xFFFF >> iFactor);

								psVertexDraw->u = tl->u[j] + fwtime2;
								psVertexDraw->v = tl->v[j];
								break;

							default:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;
						}
					}
					
					tl = tl->hNextTexture;
				}
				iVertices++;
			}
			iRenderedCount++;
		}

		psFace = psFace->psNextRenderFace;
	}

	return iRenderedCount;
	*/
}

NAKED int EXERender::HandleVertexColor( void * pRenderBuffer, int iMaterialID, BOOL bClip, short * psColor )
{
	JMP( pfnHandleVertexColor );
	/*
	EXERenderFace * psFace = NULL;

	int iMaterialCount = 0;

	int iRenderedCount = 0;

	int wtime = (iRenderTime >> 6) & 0xFF;
	float fwtime = (float)wtime / 256;

	EXEDrawVertex * psaRenderVertex = (EXEDrawVertex*)pRenderBuffer;

	if ( bClip )
	{
		psFace = psaRenderMaterial[iMaterialID].psClipRenderFace;
		iMaterialCount = psaRenderMaterial[iMaterialID].iClipMaterialCounter;
	}
	else
	{
		psFace = psaRenderMaterial[iMaterialID].psRenderFace;
		iMaterialCount = psaRenderMaterial[iMaterialID].iMaterialCounter;
	}

	for ( int i = 0; i < iMaterialCount; i++ )
	{
		if ( (psFace->dwClipAnd & 0x80) == 0 )
		{
			for ( int j = 0; j < 3; j++ )
			{
				EXERenderVertex * ps = psFace->psRenderVertex[j];

				EXEDrawVertex * psVertexDraw = psaRenderVertex + iRenderedCount;

				psVertexDraw->x = ps->xy[0];
				psVertexDraw->y = ps->xy[1];
				psVertexDraw->z = ps->zbuffer;
				psVertexDraw->rhw = ps->rwh;

				if ( bRenderAlpha && ps->tz > (ZDISTANCEMIN << 8) )
				{
					BYTE baColor[4];
					BYTE baSpecular[4];

					WRITEDWORD( baColor, ps->dwVertexColor );
					WRITEDWORD( baSpecular, ps->dwSpecular );

					baSpecular[3] = baSpecular[3] / 2 + baSpecular[3] / 3;
					baColor[3] = baColor[3] / 2 + baColor[3] / 3;

					psVertexDraw->d3dColor = READDWORD( baColor );
					psVertexDraw->d3dSpecular = READDWORD( baSpecular );
				}
				else
				{
					BYTE baColor[4];
					BYTE baSpecular[4];

					if ( EXERender::SetColorVertex( ps, psColor, baColor, baSpecular ) )
					{
						psVertexDraw->d3dColor		= READDWORD( baColor );
						psVertexDraw->d3dSpecular	= READDWORD( baSpecular );
					}
					else
					{
						psVertexDraw->d3dColor = ps->dwVertexColor;
						psVertexDraw->d3dSpecular = ps->dwSpecular;
					}
				}

				EXETextureLink * tl = (EXETextureLink*)psFace->pcTextureLink;

				for ( int k = 0; k < 8; k++ )
				{
					if ( tl == NULL )
					{
						if ( k == 1 && (*(EXETextureHandle**)0x02E4581C) )
							tl = (EXETextureLink*)psFace->pcTextureLink;
						else
							break;
					}

					psVertexDraw->u = tl->u[j];
					psVertexDraw->v = tl->v[j];

					if ( iTextureModifier[k] )
					{
						switch ( iTextureModifier[k] )
						{
							case TEXTUREMODIFIER_None:
							case TEXTUREMODIFIER_FormX:
							case TEXTUREMODIFIER_FormY:
							case TEXTUREMODIFIER_FormZ:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Scroll:
								psVertexDraw->u = tl->u[j] + fwtime;
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_Reflex:
								psVertexDraw->u = tl->u[j] + ((float)((ps->tx >> 1) / 65536));
								psVertexDraw->v = tl->v[j] + ((float)((ps->ty >> 2) / 65536));
								break;

							case TEXTUREMODIFIER_Scroll2:
							case TEXTUREMODIFIER_Scroll3:
							case TEXTUREMODIFIER_Scroll4:
							case TEXTUREMODIFIER_Scroll5:
							case TEXTUREMODIFIER_Scroll6:
							case TEXTUREMODIFIER_Scroll7:
							case TEXTUREMODIFIER_Scroll8:
							case TEXTUREMODIFIER_Scroll9:
							case TEXTUREMODIFIER_Scroll10:
								psVertexDraw->u = (tl->u[j] + fwtime * (iTextureModifier[k] - (TEXTUREMODIFIER_Scroll2 - 2)));
								psVertexDraw->v = tl->v[j];
								break;

							case TEXTUREMODIFIER_ScrollSlow1:
							case TEXTUREMODIFIER_ScrollSlow2:
							case TEXTUREMODIFIER_ScrollSlow3:
							case TEXTUREMODIFIER_ScrollSlow4:
								int iFactor;
								int wtime2;
								float fwtime2;

								iFactor = TEXTUREMODIFIER_ScrollSlow4 - iTextureModifier[k] + 4;

								wtime2 = (iRenderTime >> 6) & (0xFFFF >> iFactor);
								fwtime2 = (float)wtime2 / (float)(0xFFFF >> iFactor);

								psVertexDraw->u = tl->u[j] + fwtime2;
								psVertexDraw->v = tl->v[j];
								break;

							default:
								psVertexDraw->u = tl->u[j];
								psVertexDraw->v = tl->v[j];
								break;
						}
					}

					tl = tl->hNextTexture;
				}

				iRenderedCount++;
			}

			AddIndices( 3 );
		}

		psFace = psFace->psNextRenderFace;
	}

	return iRenderedCount / 3;
	*/
}


BOOL EXERender::SetTextureManager( EXETextureManager * pcTexture )
{
	psRenderMaterial		= pcTexture->pcaTexture;
	iRenderMaterialCount	= pcTexture->iMaxTexture;
	iMaterialListRearCount	= iRenderMaterialCount - 1;

	return TRUE;
}

NAKED BOOL EXERender::SetCameraPosition( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnSetCameraPosition );
}

NAKED BOOL EXERender::SetCameraPosition( int iX, int iY, int iZ, EXEMatrixE * psMatrix )
{
	JMP( pfnSetCamera2 );
}

BOOL EXERender::GetCameraPosition( Point3D & sPosition, Point3D & sAnglePosition )
{
	sPosition		= sCameraPos;
	sAnglePosition	= sCameraAngle;
	return TRUE;
}

BOOL EXERender::ClipFaceRender()
{
	int iValue = 1;

	//Add Faces to be renderized
	for ( int i = 0; i < 6; i++ )
	{
		//Alive?
		if ( dwClipAnd & iValue )
		{
			//Check Faces...
			for ( int j = 0; j < iFaceCount; j++ )
			{
				EXERenderFace * psRender = psRenderFace + j;

				if ( psRender )
				{
					//Face not Display?
					if ( (psRender->dwClipAnd & 0x80) == 0 && (psRender->dwClipAnd & iValue) )
					{
						//Add Face to Render
						AddFaceRender( psRender, iValue );
					}
				}
			}
		}

		iValue <<= 1;

		if ( iValue == 2 )
		{
			//Next View?
			if ( iGameFieldView == 0 || iGameFieldViewStep == 1 )
			{
				//Check Faces
				for ( int j = 0; j < iFaceCount; j++ )
				{
					EXERenderFace * psRender = psRenderFace + j;

					//Face Alive?
					if ( psRender && psRender->iMaterialID < iRenderMaterialCount )
					{
						EXETextureMaterial * psMaterial = psRenderMaterial + psRender->iMaterialID;

						//Have Material?
						if ( psMaterial )
						{
							EXETextureHandle * psTexture = psMaterial->psTexture[0];

							//Have Texture?
							if ( psTexture && psTexture->szNameA[0] )
							{
								//Face not Displayed?
								if ( (psRender->dwClipAnd & 0x80) == 0 && (psRender->dwClipAnd & iValue) )
								{
									//Add Face to Render
									AddFaceRender( psRender, iValue );
								}
							}
						}
					}
				}
			}

			iValue <<= 1;
			i++;
		}
	}

	//Update Material Faces
	for ( int i = 0; i < iFaceCount; i++ )
	{
		EXERenderFace * psRender = psRenderFace + i;

		//Face Alive?
		if ( psRender )
		{
			//Not Displayed?
			if ( (psRender->dwClipAnd & 0x80) == 0 )
			{
				//Material Fix...
				if ( psRender->iMaterialID < _countof( psaRenderMaterial ) )
				{
					EXERenderMaterial * psMaterial = psaRenderMaterial + psRender->iMaterialID;

					//Have Material?
					if ( psMaterial )
					{
						//Increase Material...
						psMaterial->iMaterialCounter++;

						//Not added to Render?
						if ( psMaterial->psLastRenderFace == NULL )
						{
							//Set Face to Material
							psMaterial->psRenderFace		= psRender;
							psMaterial->psLastRenderFace	= psRender;

							//Add Material to Render by Index
							AddMaterialRender( psRender->iMaterialID );
						}
						else if ( psMaterial->psLastRenderFace )
						{
							//Update Material Render
							psMaterial->psLastRenderFace->psNextRenderFace	= psRender;
							psMaterial->psLastRenderFace					= psRender;
						}
					}
				}


				//Set Display Vertex
				{
					if ( psRender->psRenderVertex[0] )
						psRender->psRenderVertex[0]->dwClipAnd |= 0x100;

					if ( psRender->psRenderVertex[1] )
						psRender->psRenderVertex[1]->dwClipAnd |= 0x100;

					if ( psRender->psRenderVertex[2] )
						psRender->psRenderVertex[2]->dwClipAnd |= 0x100;
				}
			}
		}
	}

	return TRUE;
}

void EXERender::CloseCamera()
{
	iCameraPositionMask = 0;
}

BOOL EXERender::UpdateFogState( BOOL b )
{
	if ( bFogIsRendered && iFogMode )
	{
		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_FOGENABLE, b );

		return TRUE;
	}

	return FALSE;
}

DWORD EXERender::GetTextureAverageColor( EXETextureHandle * psTextureHandle, BOOL bHighColor )
{
	static EXETextureHandle * psLastTextureHandle	= NULL;
	static DWORD dwLastColor						= 0;

	if ( psTextureHandle == NULL )
		return 0;

	if ( psTextureHandle->pcD3DTexture == NULL )
		return 0;

	if ( (psLastTextureHandle == psTextureHandle) && (dwLastColor > 0) )
		return dwLastColor;

	psLastTextureHandle = psTextureHandle;
	dwLastColor			= GRAPHICENGINE->GetTextureAverageColor( psTextureHandle->pcD3DTexture, psTextureHandle->iWidth, psTextureHandle->iHeight, bHighColor );

	return dwLastColor;
}

DWORD EXERender::GetFVF( int iTextureCount )
{
	return D3DFVF_TEXTURE[iTextureCount];
}

int EXERender::GetSizeFVF( int iTextureCount )
{
	return D3DTLVERTEX_SIZE[iTextureCount];
}
