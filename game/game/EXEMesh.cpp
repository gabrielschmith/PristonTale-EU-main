#include "stdafx.h"
#include "EXEMesh.h"

EXEVertex * psWorldVertex = (EXEVertex*)0x2444C20;

NAKED int EXEMesh::BeginRender()
{
	JMP( pfnBeginRender );
}

BOOL EXEMesh::IsInView( int iX, int iY, int iZ, Point3D * psLocalAngle, Point3D * psWorldAngle )
{
	EXEPoint	cCheckVertex;
	int			iaTrig[6] = { 0 };

	//Mask World Position
	int iWorldX = ( -psWorldAngle->iX ) & 4095;
	int iWorldY = ( -psWorldAngle->iY ) & 4095;
	int iWorldZ = ( -psWorldAngle->iZ ) & 4095;

	cCheckVertex.SetFromVertex( &sZeroVertex );
	cCheckVertex.Move( iX, iY, iZ );

	//Trigonometry Settings
	iaTrig[0] = PTSEN[iWorldX] >> 8;
	iaTrig[1] = PTCOS[iWorldX] >> 8;
	iaTrig[2] = PTSEN[iWorldY] >> 8;
	iaTrig[3] = PTCOS[iWorldY] >> 8;
	iaTrig[4] = PTSEN[iWorldZ] >> 8;
	iaTrig[5] = PTCOS[iWorldZ] >> 8;

	cCheckVertex.WorldRotate( iaTrig );

	//Get Position Z 2D
	int iPositionZ = cCheckVertex.sPosi.iZ >> 8;

	int iDist = *(int*)0x82BF34;

	if( ( iPositionZ + iBound ) > 0 && ( iPositionZ < iDist) )
	{
		if( iPositionZ == 0 )
			iPositionZ = 1;

		iPositionZ = abs( iPositionZ );
		int iBounding = ( iBound * (int)READDWORD( 0x82BF0C ) ) / iPositionZ;

		//Check if Rectangle 2D is in View
		if( ( cCheckVertex.sPosi2D.iX + iBounding ) > 0 &&
			( cCheckVertex.sPosi2D.iX - iBounding ) < RESOLUTION_WIDTH &&
			( cCheckVertex.sPosi2D.iY + iBounding ) > 0 &&
			( cCheckVertex.sPosi2D.iY - iBounding ) < RESOLUTION_HEIGHT )
			return TRUE;
	}

	return FALSE;
}

BOOL EXEMesh::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * psFrameInfo )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );



	if( ( !iFrameCount && (iRotateCount > 0 || iPosCount > 0 || iScaleCount > 0 ) ) || ( iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0 ) ) )
	{
		if( iFrameCount )
		{
			//Rotate
			if( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
				GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame, psFrameInfo );
			else
				sMatrixF = EXEMatrixConvertIF( sTmRotate );

			//Scale
			if( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
			{
				EXEMatrixIdentityI( sMatrix );
				GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

				sM = EXEMatrixConvertIF( sMatrix );
				sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
			}

			//Position
			if( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
				GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
			else
			{
				sMatrixF.f[3][0] = float( px ) / 256;
				sMatrixF.f[3][1] = float( py ) / 256;
				sMatrixF.f[3][2] = float( pz ) / 256;
			}
		}
		else
		{
			//Rotate
			if( iRotateCount > 0 && sRot[iRotateCount-1].iFrame > iFrame )
				GetFrameRotate( sMatrixF, sPreviousRotate, sRot, iRotate, iFrame, psFrameInfo );
			else
				sMatrixF = EXEMatrixConvertIF( sTmRotate );

			//Scale
			if( iScaleCount > 0 && sScale[iScaleCount-1].iFrame > iFrame )
			{
				EXEMatrixIdentityI( sMatrix );
				GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], sScale, iFrame );

				sM = EXEMatrixConvertIF( sMatrix );
				sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
			}

			//Position
			if( iPosCount > 0 && sPos[iPosCount-1].iFrame > iFrame )
				GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], sPos, iFrame );
			else
			{
				sMatrixF.f[3][0] = float( px ) / 256;
				sMatrixF.f[3][1] = float( py ) / 256;
				sMatrixF.f[3][2] = float( pz ) / 256;
			}
		}
	}
	else
	{
		//Parent Mesh?
		if( pParentMesh )
		{
			sMatrix = EXEMatrixMultiplyI( sTm, pParentMesh->sTmInvert );
			sMatrixF = EXEMatrixConvertIF( sMatrix );
		}
		else
			sMatrixF = EXEMatrixConvertIF( sTm );
	}

	//Parent Mesh?
	if( pParentMesh )
		sTmResult = EXEMatrixMultiplyF( sMatrixF, pParentMesh->sTmResult );
	else
		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );

	if( iAngleX == 0 && iAngleY == 0 && iAngleZ == 0 )
	{
		EXEMatrixIdentityI( sLocal );
		sWorld = EXEMatrixConvertFI( sTmResult );
	}
	else
	{
		//Identity Matrix
		EXEMatrixI rX, rY, rZ;
		EXEMatrixIdentityI( rX );
		EXEMatrixIdentityI( rY );
		EXEMatrixIdentityI( rZ );

		//Rotate Matrix
		EXEMatrixRotateXI( rX, iAngleX );
		EXEMatrixRotateZI( rY, iAngleY );
		EXEMatrixRotateYI( rZ, iAngleZ );

		//Multiply Matrix
		if( EXERender::GetRender()->iRenderType )
		{
			sLocal = EXEMatrixMultiplyI( rY, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rZ );
		}
		else
		{
			sLocal = EXEMatrixMultiplyI( rZ, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rY );
		}

		sMatrix = EXEMatrixConvertFI( sTmResult );
		sWorld = EXEMatrixMultiplyI( sMatrix, sLocal );
	}

	return TRUE;
}

BOOL EXEMesh::Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEModel * pcTalkAnimation, EXEFrame * psFrameInfo )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );

	EXEMatrixF * psTalkMatrix = NULL;

	if ( pcTalkAnimation )
	{
		for ( UINT i = 0; i < pcTalkAnimation->uMeshCount; i++ )
		{
			if ( pcTalkAnimation->paMesh[i] )
			{
				if ( STRINGCOMPAREI( pcTalkAnimation->paMesh[i]->szName, szName ) )
				{
					psTalkMatrix = &pcTalkAnimation->paMesh[i]->sTmResult;
					break;
				}
			}
		}
	}

	if ( psTalkMatrix == NULL )
	{
		if ( (!iFrameCount && (iRotateCount > 0 || iPosCount > 0 || iScaleCount > 0)) || (iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0)) )
		{
			if ( iFrameCount )
			{
				//Rotate
				if ( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
					GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame, psFrameInfo );
				else
					sMatrixF = EXEMatrixConvertIF( sTmRotate );

				//Scale
				if ( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
				{
					EXEMatrixIdentityI( sMatrix );
					GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

					sM = EXEMatrixConvertIF( sMatrix );
					sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
				}

				//Position
				if ( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
					GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
				else
				{
					sMatrixF.f[3][0] = float( px ) / 256;
					sMatrixF.f[3][1] = float( py ) / 256;
					sMatrixF.f[3][2] = float( pz ) / 256;
				}
			}
			else
			{
				//Rotate
				if ( iRotateCount > 0 && sRot[iRotateCount - 1].iFrame > iFrame )
					GetFrameRotate( sMatrixF, sPreviousRotate, sRot, 0, iFrame, psFrameInfo );
				else
					sMatrixF = EXEMatrixConvertIF( sTmRotate );

				//Scale
				if ( iScaleCount > 0 && sScale[iScaleCount - 1].iFrame > iFrame )
				{
					EXEMatrixIdentityI( sMatrix );
					GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], sScale, iFrame );

					sM = EXEMatrixConvertIF( sMatrix );
					sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
				}

				//Position
				if ( iPosCount > 0 && sPos[iPosCount - 1].iFrame > iFrame )
					GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], sPos, iFrame );
				else
				{
					sMatrixF.f[3][0] = float( px ) / 256;
					sMatrixF.f[3][1] = float( py ) / 256;
					sMatrixF.f[3][2] = float( pz ) / 256;
				}
			}
		}
		else
		{
			//Parent Mesh?
			if ( pParentMesh )
			{
				sMatrix = EXEMatrixMultiplyI( sTm, pParentMesh->sTmInvert );
				sMatrixF = EXEMatrixConvertIF( sMatrix );
			}
			else
				sMatrixF = EXEMatrixConvertIF( sTm );
		}
	}
	else
		CopyMemory( &sMatrixF, psTalkMatrix, sizeof( EXEMatrixF ) );

	//Parent Mesh?
	if ( pParentMesh )
		sTmResult = EXEMatrixMultiplyF( sMatrixF, pParentMesh->sTmResult );
	else
		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );

	if ( iAngleX == 0 && iAngleY == 0 && iAngleZ == 0 )
	{
		EXEMatrixIdentityI( sLocal );
		sWorld = EXEMatrixConvertFI( sTmResult );
	}
	else
	{
		//Identity Matrix
		EXEMatrixI rX, rY, rZ;
		EXEMatrixIdentityI( rX );
		EXEMatrixIdentityI( rY );
		EXEMatrixIdentityI( rZ );

		//Rotate Matrix
		EXEMatrixRotateXI( rX, iAngleX );
		EXEMatrixRotateZI( rY, iAngleY );
		EXEMatrixRotateYI( rZ, iAngleZ );

		//Multiply Matrix
		if ( EXERender::GetRender()->iRenderType )
		{
			sLocal = EXEMatrixMultiplyI( rY, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rZ );
		}
		else
		{
			sLocal = EXEMatrixMultiplyI( rZ, rX );
			sLocal = EXEMatrixMultiplyI( sLocal, rY );
		}

		sMatrix = EXEMatrixConvertFI( sTmResult );
		sWorld = EXEMatrixMultiplyI( sMatrix, sLocal );
	}


	return TRUE;
}

BOOL EXEMesh::AnimateTalk( int iFrame )
{
	EXEMatrixF sMatrixF;
	EXEMatrixI sMatrix;
	EXEMatrixF sM;

	EXEMatrixIdentityF( sMatrixF );

	int iRotate = GetFrameRotate( iFrame );
	int iPosition = GetFramePosition( iFrame );
	int iScale = GetFrameScale( iFrame );

	if ( iFrameCount && (iRotate >= 0 || iPosition >= 0 || iScale >= 0) )
	{
		//Rotate
		if ( iRotateCount > 0 && iRotate >= 0 && iRotate < iRotateCount )
			GetFrameRotate( sMatrixF, &sPreviousRotate[iRotate], &sRot[iRotate], iRotate, iFrame );
		else
			sMatrixF = EXEMatrixConvertIF( sTmRotate );

		//Scale
		if ( iScaleCount > 0 && iScale >= 0 && iScale < iScaleCount )
		{
			EXEMatrixIdentityI( sMatrix );
			GetFrameScale( sMatrix.i[0][0], sMatrix.i[1][1], sMatrix.i[2][2], &sScale[iScale], iFrame );

			sM = EXEMatrixConvertIF( sMatrix );
			sMatrixF = EXEMatrixMultiplyF( sMatrixF, sM );
		}

		//Position
		if ( iPosCount > 0 && iPosition >= 0 && iPosition < iPosCount )
			GetFramePosition( sMatrixF.f[3][0], sMatrixF.f[3][1], sMatrixF.f[3][2], &sPos[iPosition], iFrame );
		else
		{
			sMatrixF.f[3][0] = float( px ) / 256;
			sMatrixF.f[3][1] = float( py ) / 256;
			sMatrixF.f[3][2] = float( pz ) / 256;
		}

		CopyMemory( &sTmResult, &sMatrixF, sizeof( EXEMatrixF ) );
	}
	else
		EXEMatrixIdentityF( sTmResult );

	return TRUE;
}

BOOL EXEMesh::SetPosition( Point3D * psPosition, Point3D * psAngle )
{
	sPosition.iX = psPosition->iX;
	sPosition.iY = psPosition->iY;
	sPosition.iZ = psPosition->iZ;

	sAngle.iX = psAngle->iX;
	sAngle.iY = psAngle->iY;
	sAngle.iZ = psAngle->iZ;

	return TRUE;
}

void EXEMesh::ClearVertex()
{
	for ( int i = 0; i < iVertexCount; i++ )
	{
		pMeshVertex[i].sPosition2 = Point3D( 0, 0, 0 );
	}

	dwHeader &= (~0x80000000);
}

int EXEMesh::GetFrameRotate( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saRotateFrame[i].iPositionCount > 0 && saRotateFrame[i].iStartFrame <= iFrame && saRotateFrame[i].iEndFrame > iFrame )
				return saRotateFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFrameRotate( EXEMatrixF & sMatrix, EXEMatrixF * prevMatrix, EXEAnimationRotation * pRot, int iRotate, int iFrame, EXEFrame * psFrameInfo )
{
	float ax, ay, az, aw;
	float bx, by, bz, bw;
	int s, e, ch, sh;
	float alpha;

	int iCount = 0;

	if ( iRotateCount > 0 )
	{
		if ( pRot[iCount].iFrame > iFrame )
			return iFrame;

		while ( TRUE )
		{
			s = pRot[iCount].iFrame;
			e = pRot[iCount + 1].iFrame;

			if ( s <= iFrame && e > iFrame )
				break;

			iCount++;

			if ( iCount >= 0 && (iCount + iRotate) >= iRotateCount )
				return iFrame;
		}
	}

	ch = e - s;
	sh = iFrame - s;

	alpha = ((float)sh / (float)ch);

	ax = pRot[iCount].fX;
	ay = pRot[iCount].fY;
	az = pRot[iCount].fZ;
	aw = pRot[iCount].fW;

	bx = pRot[iCount + 1].fX;
	by = pRot[iCount + 1].fY;
	bz = pRot[iCount + 1].fZ;
	bw = pRot[iCount + 1].fW;


	if( psFrameInfo && e > (int)psFrameInfo->iEndFrame )
	{
		if ( iRotateCount > 0 && iCount >= 0 && (iCount + iRotate) < iRotateCount )
			CopyMemory( &sMatrix, &prevMatrix[iCount], sizeof(EXEMatrixF) );

		return s;
	}

	Vector4DF Q( 0, 0, 0, 0 );
	Vector4DF v0( 0, 0, 0, 0 );
	Vector4DF v1( bx, by, bz, bw );

	Q = EXEQuaternionSlerp( v0, v1, alpha );

	EXEMatrixFromQuaterion( sMatrix, Q.fX, Q.fY, Q.fZ, Q.fW );

	if ( iRotateCount > 0 && iCount >= 0 && (iCount + iRotate) < iRotateCount )
		sMatrix = EXEMatrixMultiplyF( prevMatrix[iCount], sMatrix );

	return s;
}

int EXEMesh::GetFramePosition( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saPosFrame[i].iPositionCount > 0 && saPosFrame[i].iStartFrame <= iFrame && saPosFrame[i].iEndFrame > iFrame )
				return saPosFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFramePosition( float & x, float & y, float & z, EXEAnimationPosition * pPos, int iFrame )
{
	int s, e, ch, sh;
	int	iCount = 0;
	float alpha;

	if( pPos[iCount].iFrame > iFrame )
		return iFrame;

	while( TRUE )
	{
		s = pPos[iCount].iFrame;
		e = pPos[iCount + 1].iFrame;

		if( s <= iFrame && e> iFrame ) 
			break;

		iCount++;
	}

	ch = e - s;
	sh = iFrame - s;

	alpha = float( sh ) / float( ch );

	x = pPos[iCount].fX + ( ( pPos[iCount + 1].fX - pPos[iCount].fX ) * alpha );
	y = pPos[iCount].fY + ( ( pPos[iCount + 1].fY - pPos[iCount].fY ) * alpha );
	z = pPos[iCount].fZ + ( ( pPos[iCount + 1].fZ - pPos[iCount].fZ ) * alpha );

	return -1;
}

int EXEMesh::GetFrameScale( int iFrame )
{
	if( iFrameCount > 0 )
	{
		for( int i = 0; i < iFrameCount; i++ )
		{
			if( saScaleFrame[i].iPositionCount > 0 && saScaleFrame[i].iStartFrame <= iFrame && saScaleFrame[i].iEndFrame > iFrame )
				return saScaleFrame[i].iPositionID;
		}
	}

	return -1;
}

int EXEMesh::GetFrameScale( int & x, int & y, int & z, EXEAnimationScale * pScale, int iFrame )
{
	int s, e, ch, sh;
	int alpha;

	int iCount = 0;

	if( pScale[iCount].iFrame > iFrame )
		return iFrame;

	while( TRUE )
	{
		s = pScale[iCount].iFrame;
		e = pScale[iCount + 1].iFrame;

		if( s <= iFrame && e > iFrame ) 
			break;

		iCount++;
	}

	ch = e - s;
	sh = iFrame - s;
	alpha = ( sh << 8 ) / ch;

	x = pScale[iCount].iX + ( ( ( pScale[iCount + 1].iX - pScale[iCount].iX ) * alpha ) >> 8 );
	y = pScale[iCount].iY + ( ( ( pScale[iCount + 1].iY - pScale[iCount].iY ) * alpha ) >> 8 );
	z = pScale[iCount].iZ + ( ( ( pScale[iCount + 1].iZ - pScale[iCount].iZ ) * alpha ) >> 8 );

	return s;
}

NAKED BOOL EXEMesh::ReformTexture( int iType, int iMaterialID, int iMaterialCount, Point3D * psPosi, Point3D * psSize )
{
	JMP( pfnReformTexture );
}

BOOL EXEMesh::PreRender( Point3D * psCamera, Point3D * psAngle )
{
	int iX = ( sPosition.iX - psCamera->iX ) >> 8;
	int iY = ( sPosition.iY - psCamera->iY ) >> 8;
	int iZ = ( sPosition.iZ - psCamera->iZ ) >> 8;

	if( IsInView( iX, iY, iZ, &sAngle, psAngle ) )
	{
		if( READDWORD( 0x2619824 ) )
		{
			sCameraPosition.iX = psCamera->iX;
			sCameraPosition.iY = psCamera->iY;
			sCameraPosition.iZ = psCamera->iZ;
		}
		else
		{
			sCameraPosition.iX = psCamera->iX - sPosition.iX;
			sCameraPosition.iY = psCamera->iY - sPosition.iY;
			sCameraPosition.iZ = psCamera->iZ - sPosition.iZ;
		}

		EXERender::GetRender()->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, psAngle->iX, psAngle->iY, psAngle->iZ );

		WorldForm();

		return TRUE;
	}

	return FALSE;
}

int EXEMesh::Render( Point3D * psCamera, Point3D * psAngle )
{
	int iX = ( sPosition.iX - psCamera->iX ) >> 8;
	int iY = ( sPosition.iY - psCamera->iY ) >> 8;
	int iZ = ( sPosition.iZ - psCamera->iZ ) >> 8;

	if( IsInView( iX, iY, iZ, &sAngle, psAngle ) )
	{
		if( *(BOOL*)( 0x2619824 ) )
		{
			sCameraPosition.iX = psCamera->iX;
			sCameraPosition.iY = psCamera->iY;
			sCameraPosition.iZ = psCamera->iZ;
		}
		else
		{
			sCameraPosition.iX = psCamera->iX - sPosition.iX;
			sCameraPosition.iY = psCamera->iY - sPosition.iY;
			sCameraPosition.iZ = psCamera->iZ - sPosition.iZ;
		}

		WorldForm();
		EXERender::GetRender()->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, psAngle->iX, psAngle->iY, psAngle->iZ );

		int i = 0;

		i = FinalRender();

		return i;
	}

	return FALSE;
}

NAKED int EXEMesh::FinalRender()
{
	JMP( pfnFinalRender );
}

BOOL EXEMesh::WorldForm()
{
	EXEVertex * psVertex;
	EXEMatrixI * psMatrix;

	if( pVertexBoneList )
	{
		for( int i = 0; i < iVertexCount; i++ )
		{
			psVertex = &pMeshVertex[i];
			psMatrix = &pVertexBoneList[i]->sWorld;

			//Update Main Position
			int rx = psVertex->sPosition.iX * psMatrix->i[0][0] + psVertex->sPosition.iY * psMatrix->i[1][0] + psVertex->sPosition.iZ * psMatrix->i[2][0];
			int ry = psVertex->sPosition.iX * psMatrix->i[0][1] + psVertex->sPosition.iY * psMatrix->i[1][1] + psVertex->sPosition.iZ * psMatrix->i[2][1];
			int rz = psVertex->sPosition.iX * psMatrix->i[0][2] + psVertex->sPosition.iY * psMatrix->i[1][2] + psVertex->sPosition.iZ * psMatrix->i[2][2];

			psWorldVertex[i].sPosition.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition.iY = ( rz >> 8 ) + psMatrix->i[3][2];

			if( dwHeader & 0x80000000 )
				psMatrix = &pVertexBoneList[i]->sWorld;
			else
				psMatrix = &pVertexBoneList[i]->sLocal;

			//Update Unk Position
			rx = psVertex->sPosition2.iX * psMatrix->i[0][0] + psVertex->sPosition2.iY * psMatrix->i[1][0] + psVertex->sPosition2.iZ * psMatrix->i[2][0];
			ry = psVertex->sPosition2.iX * psMatrix->i[0][1] + psVertex->sPosition2.iY * psMatrix->i[1][1] + psVertex->sPosition2.iZ * psMatrix->i[2][1];
			rz = psVertex->sPosition2.iX * psMatrix->i[0][2] + psVertex->sPosition2.iY * psMatrix->i[1][2] + psVertex->sPosition2.iZ * psMatrix->i[2][2];
		
			psWorldVertex[i].sPosition2.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition2.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition2.iY = ( rz >> 8 ) + psMatrix->i[3][2];
		}
	}
	else
	{
		for( int i = 0; i < iVertexCount; i++ )
		{
			psVertex = &pMeshVertex[i];

			//Update Main Position
			int rx = psVertex->sPosition.iX * sWorld.i[0][0] + psVertex->sPosition.iY * sWorld.i[1][0] + psVertex->sPosition.iZ * sWorld.i[2][0];
			int ry = psVertex->sPosition.iX * sWorld.i[0][1] + psVertex->sPosition.iY * sWorld.i[1][1] + psVertex->sPosition.iZ * sWorld.i[2][1];
			int rz = psVertex->sPosition.iX * sWorld.i[0][2] + psVertex->sPosition.iY * sWorld.i[1][2] + psVertex->sPosition.iZ * sWorld.i[2][2];

			psWorldVertex[i].sPosition.iX = ( rx >> 8 ) + sWorld.i[3][0];
			psWorldVertex[i].sPosition.iZ = ( ry >> 8 ) + sWorld.i[3][1];
			psWorldVertex[i].sPosition.iY = ( rz >> 8 ) + sWorld.i[3][2];

			if( dwHeader & 0x80000000 )
				psMatrix = &sWorld;
			else
				psMatrix = &sLocal;

			//Update Unk Position
			rx = psVertex->sPosition2.iX * psMatrix->i[0][0] + psVertex->sPosition2.iY * psMatrix->i[1][0] + psVertex->sPosition2.iZ * psMatrix->i[2][0];
			ry = psVertex->sPosition2.iX * psMatrix->i[0][1] + psVertex->sPosition2.iY * psMatrix->i[1][1] + psVertex->sPosition2.iZ * psMatrix->i[2][1];
			rz = psVertex->sPosition2.iX * psMatrix->i[0][2] + psVertex->sPosition2.iY * psMatrix->i[1][2] + psVertex->sPosition2.iZ * psMatrix->i[2][2];

			psWorldVertex[i].sPosition2.iX = ( rx >> 8 ) + psMatrix->i[3][0];
			psWorldVertex[i].sPosition2.iZ = ( ry >> 8 ) + psMatrix->i[3][1];
			psWorldVertex[i].sPosition2.iY = ( rz >> 8 ) + psMatrix->i[3][2];
		}
	}

	return TRUE;
}

FILE * EXEMesh::ReadGeometry( FILE * fp, EXEMesh * pcOut, char * pszNode )
{
	static char szLine[256];
	static char szDecode[256];

	static char szBuffer[256];

	int iVertexCount = 0;
	int iFaceCount = 0;
	int iTVertexCount = 0;

	BOOL bLightMap = FALSE;

	while ( !feof( fp ) )
	{
		if ( fgets( szLine, 255, fp ) == NULL )
		{
			fclose( fp );
			return NULL;
		}

		char * p = (char*)GETWORDPT( szDecode, szLine );

		if ( STRINGCOMPARE( szDecode, "*NODE_NAME" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );
			
			STRINGCOPY( pcOut->szName, szBuffer );
			
			if ( pszNode && pszNode[0] != 0 && STRINGCOMPAREI( szBuffer, pszNode ) == FALSE )
			{
				pcOut->iFaceCount = 0;
				pcOut->iVertexCount = 0;

				return fp;
			}
		}

		if ( STRINGCOMPARE( szDecode, "*NODE_PARENT" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			STRINGCOPY( pcOut->szParentName, szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMVERTEX" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iVertexCount = atoi( szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMFACES" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iFaceCount = atoi( szBuffer );
		}

		if ( STRINGCOMPARE( szDecode, "*MESH_NUMTVERTEX" ) )
		{
			p = GETWORDVALUEPT( szBuffer, p );

			iTVertexCount = atoi( szBuffer );

			if ( bLightMap )
			{

			}
		}

	}

	return fp;
}
