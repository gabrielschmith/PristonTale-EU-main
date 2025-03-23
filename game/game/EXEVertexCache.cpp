#include "stdafx.h"
#include "EXEVertexCache.h"

EXEVertexCacheData EXEVertexCache::saVertices[TOTAL_CACHE_VERTICES];

EXEVertexCache::EXEVertexCache()
{
	for ( int i = 0; i < NUM_VERTEXMODEL_CACHE; i++ )
	{
	//	psaCache[i] = new EXEVertexCacheData;
	//	ClearCache( psaCache[i] );
	}
}


EXEVertexCache::~EXEVertexCache()
{
}

void EXEVertexCache::Update()
{
	for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
	{
		EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

		if ( (TICKCOUNT - ps->dwLastUpdateTime) > 1000 )
		{
			ps->x = 0;
			ps->y = 0;
			ps->z = 0;
			ps->iMaterialID = -1;
		}
	}
}

D3DVALUE * EXEVertexCache::GetVertex( D3DVALUE * psData, int iMaterialID, int & iCount )
{
	float x = psData[0];
	float y = psData[1];
	float z = psData[2];

	if ( iCount == 0 )
	{
		for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
		{
			EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

			if ( (ps->iMaterialID == iMaterialID) )
			{
				ps->dwLastUpdateTime = TICKCOUNT;
				iCount = ps->iCount;
				return ps->vertices;
			}
		}
	}
	else if ( iCount > 0 && iCount < 600 )
	{
		for ( int i = 0; i < TOTAL_CACHE_VERTICES; i++ )
		{
			EXEVertexCacheData * ps = EXEVertexCache::saVertices + i;

			if ( ps->iMaterialID == -1 )
			{
				CopyMemory( ps->vertices, psData, (64 * (iCount * 3)) );
				ps->x = x;
				ps->y = y;
				ps->z = z;
				ps->iMaterialID = iMaterialID;
				ps->dwLastUpdateTime = TICKCOUNT;
				ps->iCount = iCount;
				return ps->vertices;
			}
		}
	}

	return (D3DVALUE*)0x2E45830;
}

BOOL EXEMeshCache::Build( EXEModel * pcModel )
{
	int iNumVertices = GetMesh()->iVertexCount;
	int iNumFaces = GetMesh()->iFaceCount;

	EXEMesh * pcMesh = GetMesh();

	if ( (iNumVertices > 0) && (iNumFaces > 0) )
	{
		int iSize	= RENDERER->GetSizeFVF( pcModel->pcTextureManager->iTextureCount );
		dwFVF		= (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX0); //RENDERER->GetFVF( pcModel->pcTextureManager->iTextureCount );

		pcVertexBuffer			= GetGraphicsDevice()->CreateStaticVertexBuffer( sizeof( EXEDrawVertexModel ), iNumVertices );
		pcTextureCoordBuffer	= GetGraphicsDevice()->CreateStaticVertexBuffer( sizeof( EXEDrawVertexModelCoord ), iNumVertices );

		if ( pcVertexBuffer && pcTextureCoordBuffer )
		{
			EXEDrawVertexModel * psaVertexData				= (EXEDrawVertexModel*)pcVertexBuffer->Lock();
			EXEDrawVertexModelCoord * psaVertexDataCoord	= (EXEDrawVertexModelCoord*)pcTextureCoordBuffer->Lock();
			if ( psaVertexData )
			{
				int iVertices = 0;
				for ( int i = 0; i < iNumVertices; i++ )
				{
					EXERenderVertex * ps = RENDERER->AddObjectVertex( pcMesh->pMeshVertex + i, 0x80000000 );
					EXEVertex * psPosition = pcMesh->pMeshVertex + i;

					psaVertexData[iVertices].x = (float)psPosition->sPosition.iX;
					psaVertexData[iVertices].y = (float)psPosition->sPosition.iY;
					psaVertexData[iVertices].z = (float)psPosition->sPosition.iZ;
					psaVertexData[iVertices].d3dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );// ps->dwVertexColor;
					psaVertexData[iVertices].d3dSpecular = D3DCOLOR_ARGB(255, 0,0,0 );
					iVertices++;
				}

				pcIndexBuffer = GetGraphicsDevice()->CreateIndexBuffer( sizeof( WORD ), iNumFaces * 3 );
				if ( pcIndexBuffer )
				{
					WORD * pwaIndices = (WORD*)pcIndexBuffer->Lock();
					if ( pwaIndices )
					{
						int iIndices = 0;
						for ( int i = 0; i < iNumFaces; i++ )
						{
							EXEFace * psFace				= pcMesh->pMeshFace + i;
							EXERenderFace * psRenderFace	= RENDERER->AddObjectFace( psFace );

							if ( psRenderFace )
							{
								pwaIndices[iIndices++] = psFace->uVertex1;
								pwaIndices[iIndices++] = psFace->uVertex2;
								pwaIndices[iIndices++] = psFace->uVertex3;

								EXETextureLink * ps = (EXETextureLink*)psRenderFace->pcTextureLink;

								psaVertexDataCoord[psFace->uVertex1].u = ps->u[0];
								psaVertexDataCoord[psFace->uVertex1].v = ps->v[0];
								psaVertexDataCoord[psFace->uVertex2].u = ps->u[1];
								psaVertexDataCoord[psFace->uVertex2].v = ps->v[1];
								psaVertexDataCoord[psFace->uVertex3].u = ps->u[2];
								psaVertexDataCoord[psFace->uVertex3].v = ps->v[2];
							}
						}

						DX::VertexElements_ptr pElements = std::make_shared<DX::VertexElements>();
						pElements->AddElement( 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 );
						pElements->AddElement( 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 );
						pElements->AddElement( 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 );
						pElements->AddElement( 1, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 );

						pcVertexDeclaration = GetGraphicsDevice()->CreateVertexDeclaration( pElements );

						pcIndexBuffer->Unlock();
					}
				}

				pcVertexBuffer->Unlock();
			}
		}
	}

	return TRUE;
}

void EXEMeshCache::Render()
{
	if ( FAILED( GRAPHICENGINE->GetDevice()->SetStreamSource( 0, pcVertexBuffer->Get(), 0, pcVertexBuffer->GetElementSize() ) ) )
	{
		return;
	}
	if ( FAILED( GRAPHICENGINE->GetDevice()->SetStreamSource( 1, pcTextureCoordBuffer->Get(), 0, pcTextureCoordBuffer->GetElementSize() ) ) )
	{
		return;
	}
	if ( FAILED( GRAPHICENGINE->GetDevice()->SetIndices( pcIndexBuffer->Get() ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->SetFVF( NULL ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->SetVertexDeclaration( pcVertexDeclaration->Get() ) ) )
	{
		return;
	}

	if ( FAILED( GRAPHICENGINE->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pcVertexBuffer->GetElementCount(), 0, pcIndexBuffer->GetPrimitiveCount() / 3 ) ) )
	{
		return;
	}
}

BOOL EXEModelCache::Build()
{
	int iMeshes = 0;
	for ( UINT i = 0; i < pcModel->uMeshCount; i++ )
	{
		if ( pcModel->paMesh[pcModel->uaMeshID[i]] )
		{
			pcMeshCache[iMeshes] = new EXEMeshCache();
			pcMeshCache[iMeshes]->SetMesh( pcModel->paMesh[pcModel->uaMeshID[i]] );
			pcMeshCache[iMeshes]->Build( pcModel );
			iMeshes++;
		}
	}

	iMeshCount = iMeshes;

	return TRUE;
}

void EXEModelCache::Render()
{
	return;
//	GRAPHICENGINE->GetDevice()->SetTexture( 0, pcModel->pcTextureManager->pcaTexture->psTexture[0]->pcD3DTexture );

	for ( int i = 0; i < iMeshCount; i++ )
	{
		if ( pcMeshCache[i] )
			pcMeshCache[i]->Render();
	}
}
