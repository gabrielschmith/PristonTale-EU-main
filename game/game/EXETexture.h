#pragma once

enum EBlendType
{
	BLENDTYPE_None			= 0,
	BLENDTYPE_Alpha			= 0x00000001,
	BLENDTYPE_Color			= 0x00000002,
	BLENDTYPE_Shadow		= 0x00000003,
	BLENDTYPE_Lamp			= 0x00000004,
	BLENDTYPE_AddColor		= 0x00000005,
	BLENDTYPE_InvShadow		= 0x00000006,
};

enum ETextureModifier
{
	TEXTUREMODIFIER_None,
	TEXTUREMODIFIER_FormX,
	TEXTUREMODIFIER_FormY,
	TEXTUREMODIFIER_FormZ,
	TEXTUREMODIFIER_Scroll,
	TEXTUREMODIFIER_Reflex,
	TEXTUREMODIFIER_Scroll2,
	TEXTUREMODIFIER_Scroll3,
	TEXTUREMODIFIER_Scroll4,
	TEXTUREMODIFIER_Scroll5,
	TEXTUREMODIFIER_Scroll6,
	TEXTUREMODIFIER_Scroll7,
	TEXTUREMODIFIER_Scroll8,
	TEXTUREMODIFIER_Scroll9,
	TEXTUREMODIFIER_Scroll10,
	TEXTUREMODIFIER_ScrollSlow1,
	TEXTUREMODIFIER_ScrollSlow2,
	TEXTUREMODIFIER_ScrollSlow3,
	TEXTUREMODIFIER_ScrollSlow4,

};

struct EXETextureHandle
{
	char				szName[64];
	char				szNameA[64];

	LPDIRECT3DTEXTURE9	pcD3DTexture;

	int					iWidth;
	int					iHeight;

	int					iUseCounter;

	int					iTransparency;

	BOOL				bSwap;
};

struct EXETextureMaterial
{
	BOOL				bInUse;						//Whether to use materials
	int					iTextureCounter;			//Texture counter
	struct EXETextureHandle* psTexture[8];			//Texture handle list

	int					iaTextureStageState[8];		//Drawing properties
	int					iaTextureFormState[8];		//Form properties
	int					iReformTexture;				//Check texture reform properties

	BOOL				bAlpha;						//Whether map opity (TRUE, FALSE)

	int					iTextureType;				//Texture type (multimix / animation)
	int					iBlendType;					//Brand Format (SMMAT_BLEND_XXXX)
													
	int					iShade;						//Shading method (no-shade/gro-shade)
	BOOL				bTwoSided;					//Whether to use both sides
	ID					iID;						//Unique number for the material
													
	float				fDiffuseColorR;				//Diffuse color
	float				fDiffuseColorG;				
	float				fDiffuseColorB;				
													
	float				fOpacity;					//transparency
	float				fSelfIllumination;			//Self-luminous
												
	int					iTextureSwap;				//Texture swapping
	int					iMaterialFrame;				//Use frame (for synchronization when using)
	int					iTextureClip;				//Existence of texture clip for swapping (If TRUE, clipping of texture is allowed)

	int					iUseState;					//Usage (use attribute)
	int					iMeshState;					//MESH texture attribute values

	int					iWindMesh;					//Start value of wind mesh deformation

	void			  * psAnimatedTexture[32];		//Animation texture handle list
	int					iAnimatedTextureCounter;	//Animation texture counter
	int					iFrameMask;					//Frame mask for animation
	int					iShiftFrameSpeed;			//Frame increase rate (calculated by shifting the timer)
	int					iAnimationFrame;			//Frame number (frame value if fixed / SMTEX_AUTOANIMATION is automatically)
};

struct EXETextureLink
{
	float				u[3];
	float				v[3];
	DWORD			  * hTexture;
	EXETextureLink	  * hNextTexture;
};

class EXETextureManager
{
private:
	IMPFNC				pfnReadTextures = 0x0048A9E0;
	IMPFNC				pfnConstructor = 0x0048A2D0;
	IMPFNC				pfnGetTextureFromFilePath = 0x00489FD0;

public:
	DWORD				dwHeader;

	EXETextureMaterial* pcaTexture;
	int					iTextureCount;

	int					iReformTexture;
	int					iMaxTexture;

	int					iLastSearchTexture;
	char				szLastSearchTextureName[64];

	void				ReadTextures(int iMip = 0);

	EXETextureManager( int _iMaxTextures );
	EXETextureManager();

	EXETextureMaterial * GetTexture( UINT uIndex ) { return pcaTexture + uIndex; };
	UINT				LoadedTexture( std::string strFile );
	UINT				LoadTexture( std::string strFilePath );

	int					GetTextureMatNum(std::string strFilePath);
	UINT				FreeTextureIndex();

	EXETextureMaterial * GetTextureFromFilePath( const char * pszFilePath );


private:
	void				Constructor( int _iMaxTextures );

};

class EXETexture
{
public:

	IMPFNC				pfnLoadTextureDX9 = 0x0063C300;
	IMPFNC				pfnDeleteTexture = 0x004894B0;

	DWORD				dwCheckSum[5000];
	
	EXETextureHandle	saTexture[5000];

	CRITICAL_SECTION	cCriticalSection;

	int					iLastTextureCount;

	int					iTextureCount;

	static int			LoadTexture( EXETextureHandle * psTexture, bool BMipmap, bool bSwap );


	BOOL				ReadTextures();

	EXETextureHandle	* Add( char * pszTexture, char * pszMask = NULL );

	int					GetTextureIndex( std::string strFile );
	EXETextureHandle	* GetTexture( int iIndex );
	
	BOOL				DeleteSafe( EXETextureHandle * ps );

	BOOL				Delete( EXETextureHandle * ps );

	int					GetCount() { return iTextureCount; }

	static EXETexture	* GetInstance() { return ((EXETexture*)0x03500CF8); }

private:
	static bool			DecryptBMP( char * pBuffer, int iSize );
	static bool			DecryptTGA( char * pBuffer, int iSize );
};