#include "stdafx.h"
#include "Camera.h"

#define MAX_CAMERACIRCLE 4096

enum ECameraMode
{
	CAMERAMODE_Free = 0,
	CAMERAMODE_Automatic = 1,
	CAMERAMODE_Lock = 2
};



CameraZoom & sCameraZoom = (*(CameraZoom*)0x00824FA4);
ECameraMode & iCameraMode = (*(ECameraMode*)0x00824F7C);
int & iCameraCircle = (*(int*)0x00A17630);

extern int	iCameraZoomOverride;
extern int	iCameraHorzZoomSetOverride;
extern int	iCameraVertZoomSetOverride;
extern int	iCameraAngleOverride;
extern int	iCameraCircleOverride;



CCamera::CCamera()
{
	D3DXMatrixIdentity( &matView );
	D3DXMatrixIdentity( &matOrientation );

	vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vLookAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	fYaw = 0.0f;
	fYawVelocity = 0.0f;

	fPitch = 0.0f;
	fPitchVelocity = 0.0f;
}


CCamera::~CCamera()
{
}

void CCamera::Update( double fTime )
{
	UnitData * p = UNITDATA;

	if ( iCameraMode == CAMERAMODE_Lock )
	{
		sCameraZoom.iVerticalZoomSet = 384;
		sCameraZoom.iVerticalZoom = 384;
		sCameraZoom.iHorizontalZoom = 400;
		sCameraZoom.iHorizontalZoomSet = 400;
	}


	if ( iCameraAngleOverride != -1 )
	{
		sCameraZoom.iVerticalZoom = iCameraAngleOverride;
		sCameraZoom.iVerticalZoomSet = iCameraAngleOverride;
	}
	else if ( iCameraAngle != -1 )
	{
		sCameraZoom.iVerticalZoom = iCameraAngle;
		sCameraZoom.iVerticalZoomSet = iCameraAngle;
	}

	//command overrides everything, setting takes percident if that doesnt exist
	if ( iCameraZoomOverride != -1 )
	{
		sCameraZoom.iHorizontalZoom = iCameraZoomOverride;
		sCameraZoom.iHorizontalZoomSet = iCameraZoomOverride;
	}
	else if ( iCameraZoom != -1 )
	{
		sCameraZoom.iHorizontalZoom = iCameraZoom;
		sCameraZoom.iHorizontalZoomSet = iCameraZoom;
	}

	if ( iCameraCircleOverride != -1 )
		iCameraCircle = iCameraCircleOverride;		
	else if ( iCameraTurn != -1 )
		iCameraCircle = iCameraTurn;

	
	iCameraCircle &= 0x0FFF; //4095
	sCameraZoom.iHorizontalZoom &= 0x0FFF;

	CALL_WITH_ARG6( 0x0048AF40, 0, 0, -(sCameraZoom.iHorizontalZoom << 8), iCameraAngleOverride == -1 ? sCameraZoom.iVerticalZoom & 0x0FFF : iCameraAngleOverride, iCameraCircle, 0 );

	Point3D sCamPos;
	sCamPos.iX = p->sPosition.iX + iRadian3D_X;
	sCamPos.iY = p->sPosition.iY + iRadian3D_Y;
	sCamPos.iZ = p->sPosition.iZ + iRadian3D_Z;


	sCameraPosition.iX = sCamPos.iX;
	sCameraPosition.iY = sCamPos.iY;
	sCameraPosition.iZ = sCamPos.iZ;
	sCameraAngle.iX = sCameraZoom.iVerticalZoom;
	sCameraAngle.iY = iCameraCircle;
	sCameraAngle.iZ = 0;

	if ( (STAGE1) != NULL )
	{
		int i;
		if ( (i = STAGE1->GetHighestPoint( sCameraPosition.iX, sCameraPosition.iZ )) > sCameraPosition.iY )
			sCameraPosition.iY = i + 2048;
	}
	if ( (STAGE2) != NULL )
	{
		int i;
		if ( (i = STAGE2->GetHighestPoint( sCameraPosition.iX, sCameraPosition.iZ )) > sCameraPosition.iY )
			sCameraPosition.iY = i + 2048;
	}

	sCameraPosition.iY += 0x2000;

	D3DXMatrixLookAtLH( &matView, &vPosition, &vLookAt, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

Point3D sCameraPosition;
Point3D sCameraAngle;