#pragma once

#define NUM_VERTEXMODEL_CACHE		5000
#define NUM_EXEMODEL_CACHE			5000
#define MAX_POLYGON_CACHE			500
#define SIZE_EXEMODEL_CACHE			((MAX_POLYGON_CACHE+1) * 64 * 4)

#define EXEMODELDATA_GAME			((EXEModelData*)0x00A7924C)

class EXEMeshCache
{
private:

	EXEMesh							* pcMesh;

	DX::VertexBuffer_ptr			pcVertexBuffer;
	DX::VertexBuffer_ptr			pcTextureCoordBuffer;
	DX::IndexBuffer_ptr				pcIndexBuffer;
	DX::VertexDeclaration_ptr		pcVertexDeclaration;

	DWORD							dwFVF;

public:

	EXEMesh							* GetMesh() { return pcMesh; }
	void							SetMesh( EXEMesh * pcMesh ) { this->pcMesh = pcMesh; };

	BOOL							Build( EXEModel * pcModel );

	void							Render();
};

class EXEModelCache
{
private:
	EXEModel						* pcModel;

	EXEMeshCache					* pcMeshCache[128];

	int								iMeshCount = 0;

public:

	EXEModel						* GetModel() { return pcModel; }
	void							SetModel( EXEModel * pcModel ) { this->pcModel = pcModel; }

	BOOL							Build();

	void							Render();
};

#define TOTAL_CACHE_VERTICES_DATA		(600*3)
#define TOTAL_CACHE_VERTICES			500

struct EXEVertexCacheData
{
	D3DVALUE				vertices[TOTAL_CACHE_VERTICES_DATA * 64];

	int						iMaterialID;

	float					x, y, z;

	DWORD					dwLastUpdateTime;

	int						iCount;
};


class EXEVertexCache
{
protected:

	int						iIndex = -1;

	static EXEVertexCacheData	saVertices[TOTAL_CACHE_VERTICES];

public:
	EXEVertexCache();
	virtual ~EXEVertexCache();

	static void				Update();
	static D3DVALUE			* GetVertex( D3DVALUE * psData, int iMaterialID, int & iCount );

private:

};

