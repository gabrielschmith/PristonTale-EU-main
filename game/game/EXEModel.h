#pragma once

#include "EXEFrame.h"
#include "EXEMesh.h"
#include "EXETexture.h"
#include "EXEPoint.h"

class EXEModel
{
private:
	IMPFNC					pfnReadModel			= 0x0047D860;
	IMPFNC					pfnReadModelMap			= 0x0047DC00;
	IMPFNC					pfnReadBone				= 0x0047B360;
	IMPFNC					pfnGetMeshFromName		= 0x00476720;
	IMPFNC					pfnUpdateMeshes			= 0x004765D0;
	IMPFNC					pfnSetFixedPosition		= 0x00476830;
	IMPFNC					pfnSaveFile				= 0x00476AB0;



public:
	DWORD					dwHeader;

	EXEMesh				  * paMesh[128];
	UCHAR					uaMeshID[128];

	EXEModel			  * pcBoneContainer;
	EXETextureManager	  * pcTextureManager;

	int						uMaxFrame;
	int						uFrame;			//Frame

	UINT					uMeshWidth;
	UINT					uMeshHeight;

	UINT					uMeshCount;

	char					szPadding2[0x04];

	Point3D					sPosition;
	Point3D					sAngle;
	Point3D					sCameraPosition;

	int						iUnknown1;
	int						iBound;

	EXEFramePosition		saFrame[32];
	int						iFrameCount;

	int						iLastFrame;
	Point3D					iLastAngle;

	BOOL					SetPosition( Point3D * psPosition, Point3D * psAngle );
	BOOL					SetFixedPosition();

	void					ClearVertex();

	void					UpdateMeshes();

	BOOL					AddMesh( EXEMesh * pcMesh );
	void					SetParentBone( EXEModel * pcBone, EXEMesh * pcMesh );

	BOOL					IsInView( int iX, int iY, int iZ, int iBoundingBox, Point3D * psLocalAngle, Point3D * psWorldAngle );

	BOOL					Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * pFrameInfo = NULL );
	BOOL					Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEModel * pcTalkAnimation, EXEFrame * pFrameInfo = NULL );

	BOOL					AnimateTalk( int iFrame );

	int						Render( Point3D * psCamera, Point3D * psAngle, EXEFrame * psFrameInfo = NULL );
	int						Render( Point3D * psCamera, Point3D * psAngle, EXEModel * pcModelTalk, EXEFrame * psFrameInfo = NULL );

	int						PreRender( Point3D * psCamera, Point3D * psAngle, char ** pszMeshList, int iMeshListCount, EXEModel * pcModelTalk, EXEFrame * psFrameInfo = NULL );
	int						PreRender( Point3D * psCamera, Point3D * psAngle, char ** pszMeshList, int iMeshListCount, EXEFrame * psFrameInfo = NULL );

	BOOL					SetAnimationFrame( int iFrame, EXEFrame * psFrameInfo = NULL );
	BOOL					SetAnimationFrame( int iFrame, EXEModel * pcModelTalk, EXEFrame * psFrameInfo = NULL );

	static EXEModel			* ReadModel( const char * pszFileName, const char * pszModelName = NULL );
	static DWORD			ReadModelMap( char * pszFileName, char * pszModelName = NULL );
	static EXEModel			* ReadBone( char * pszFileName );

	static void				SetBone( EXEModel * pcBone ) { (*(EXEModel**)0x02E45814) = pcBone; };
	static EXEModel			* GetBone( EXEModel * pcBone ) { return (*(EXEModel**)0x02E45814); };

	EXEMesh					* GetMesh( char * pszName );

	int						SaveFile( char * pszFile );
};