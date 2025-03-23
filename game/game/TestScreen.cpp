#include "stdafx.h"
#include "TestScreen.h"


struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD dwColor; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)


USHORT g_indices[] = { 0, 1, 2, 1, 3, 2 };

LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

float fAngleH = 0.0f;

struct FrameEx : public D3DXFRAME
{
	D3DXMATRIX combinedTransform;
};

void combineTransforms( FrameEx * frame, D3DXMATRIX & P )
{
	// Save some references to economize line space. 
	D3DXMATRIX& L = frame->TransformationMatrix;
	D3DXMATRIX& C = frame->combinedTransform;

	C = L * P;

	FrameEx* sibling = (FrameEx*)frame->pFrameSibling;
	FrameEx* firstChild = (FrameEx*)frame->pFrameFirstChild;

	//Recurse down siblings. 
	if( sibling )
		combineTransforms( sibling, P );

	//Recurse to first child. 
	if( firstChild )
		combineTransforms( firstChild, C );
}

CTestScreen::CTestScreen()
{
	dwTimeL = 0;
}


CTestScreen::~CTestScreen()
{
}

// this is the function that puts the 3D models into video RAM
void init_graphics( void )
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ 3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB( 0, 0, 255 ), },
		{ 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB( 0, 255, 0 ), },
		{ -3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB( 255, 0, 0 ), },
	};

	// create a vertex buffer interface called v_buffer
	GRAPHICENGINE->GetDevice()->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL );

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	v_buffer->Lock( 0, 0, (void**)&pVoid, 0 );
	memcpy( pVoid, vertices, sizeof( vertices ) );
	v_buffer->Unlock();
}

BOOL CTestScreen::InitProg()
{
	sPositionCamera = Point3D( -34594 * 256, 1568 * 256, -7360 * 256 );
	sPointPosition.iX = -35100 * 256;
	sPointPosition.iY = 1400 * 256;
	sPointPosition.iZ = -7560 * 256;
	sAngleCamera	= Point3D( 0,0,0 );
	sPosition		= Point3D( -34800 * 256, 1440 * 256, -7460 * 256 );
	sAngle			= Point3D( 0, 1000, 0 );
	sPositionNew = Point3D( 0, 0, 0 );

	pcStage = Stage::LoadStage( "Field\\Custom\\fo1.smd" );
	pcStage->bState = TRUE;

	pcUnitData = new UnitData;

	pcUnitData->Init();
	pcUnitData->pcStage = pcStage;

	LoadUnitData( pcUnitData, "char\\npc\\sandy\\FPTSandy.ini", "", &sPosition, &sAngle );
	pcUnitData->eActionPattern = ACTIONMODE_ClientSelf;

//	pcUnitData->sCharacterData.iClass = CHARACTERCLASS_Archer;
//	pcUnitData->sCharacterData.sWarpHomeTown = 0;

	pcUnitData->SetPosition( sPosition.iX, pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ ) + 784, sPosition.iZ, 0, sAngle.iY, 0 );

	pcUnitData->bActive = TRUE;
	pcUnitData->bVisible = TRUE;

	//Prepare for Rendering
	RENDERER->Init();

	pcModelCache = new EXEModelCache();
	//pcModelCache->SetModel( EXEModel::ReadModel( "image\\Sinimage\\Items\\DropItem\\itWA101.ASE" ) );
//	pcModelCache->SetModel( EXEModel::ReadModel( "char\\monster\\arcuda\\ARCUDA.ASE" ) );
	pcModelCache->SetModel( pcUnitData->pcBodyMeshContainer );
	pcModelCache->Build();

	RENDERER->Init();

	pcVertexBuffer = GetGraphicsDevice()->CreateStaticVertexBuffer( sizeof( CUSTOMVERTEX ), 6 );
	pcIndexBuffer = GetGraphicsDevice()->CreateIndexBuffer( sizeof( WORD ), 6 );


	pRenderGlow1 = GetGraphicsDevice()->MakeRenderTarget( 11001, 1920, 1017 );
	pRenderGlow2 = GetGraphicsDevice()->MakeRenderTarget( 11002, 1920, 1017 );

	DX::cSelectGlow1.Init( GRAPHICENGINE->GetRenderer() );

	CALL( 0x0048A260 );
	return TRUE;
}

BOOL CTestScreen::InitSlave()
{
	sPositionCamera = Point3D( -35094 * 256, 1440 * 256, -7360 * 256 );
	sPointPosition.iX = -35100 * 256;
	sPointPosition.iY = 1400 * 256;
	sPointPosition.iZ = -7560 * 256;
	sAngleCamera	= Point3D( 0,0,0 );
	sPosition		= Point3D( -35094 * 256, 1440 * 256, -7360 * 256 );
	sAngle			= Point3D( 0, 5655, 0 );

	pcStage = Stage::LoadStage( "Field\\Custom\\fo1.smd" );
	pcStage->bState = TRUE;

	pcUnitData = new UnitData;

	pcUnitData->Init();
	pcUnitData->pcStage = pcStage;

	LoadUnitData( pcUnitData, "char\\tmABCD\\cd016.ini", "char\\tmABCD\\asxmas.inf", &sPosition, &sAngle );
	pcUnitData->eActionPattern = ACTIONMODE_ClientSelf;

	pcUnitData->sCharacterData.iClass = CHARACTERCLASS_Archer;
	pcUnitData->sCharacterData.sWarpHomeTown = 0;

	pcUnitData->SetPosition( sPosition.iX, pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ ), sPosition.iZ, 0, sAngle.iY, 0 );

	pcUnitData->bActive = TRUE;
	pcUnitData->bVisible = TRUE;

	EXEModel * pcBone = EXEModel::ReadBone( "archer_login_01.ASE" );

	int iaTest[4] = { 0,0,0,0 };

	ANIMATIONHANDLER->InjectDynamicAnimation( pcUnitData->pcHead, pcBone, 1000, TRUE, 3, 0, iaTest, 0, 0x150 );
	ANIMATIONHANDLER->InjectDynamicAnimation( pcUnitData->pcBody, pcBone, 1000, TRUE, 3, 0, iaTest, 0, 0x150 );

	int iIndex = pcUnitData->pcBody->psModelData->iNumModelAnimation - 1;

	iBeginFrame = pcUnitData->pcBody->psModelData->saModelAnimation[iIndex].iBeginFrame * 160;
	iEndFrame = pcUnitData->pcBody->psModelData->saModelAnimation[iIndex].iEndFrame * 160;

	pcUnitData->ChangeAnimationID( iIndex );

	CALL( 0x0048A260 );

	/*CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\SkillFighterT5SesmicImpact.lua" );

	pc->SetOwner( new Engine::ObjectOwnerPosition( Point3D( sPosition.iX, sPosition.iY + (20*256), sPosition.iZ + (62*256) ) ) );

	PARTICLEENGINE->AddScript( pc );*/
	return TRUE;
}

BOOL CTestScreen::Init()
{
	//InitSlave();
	InitProg();
	init_graphics();

	return TRUE;
}

void CTestScreen::Shutdown()
{
}


// this is the function used to render a single frame
void render_frame( void )
{
//	GRAPHICENGINE->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

//	GRAPHICENGINE->GetDevice()->BeginScene();

	// select which vertex format we are using
	GRAPHICENGINE->GetDevice()->SetFVF( CUSTOMFVF );

	// SET UP THE PIPELINE

	D3DXMATRIX matRotateY;    // a matrix to store the rotation information

	static float index = 0.0f; index += 0.01f;    // an ever-increasing float value

												  // build a matrix to rotate the model based on the increasing float value
	D3DXMatrixRotationY( &matRotateY, index );

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );

	D3DXMatrixTranslation( &mat, 0.0f, 0.0f, 10.0f );
	D3DXMatrixMultiply( &mat, &matRotateY, &mat );

	/*
	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_WORLD, &mat );

	D3DXMATRIX matView;    // the view transform matrix

	D3DXMatrixLookAtLH( &matView,
		&D3DXVECTOR3( 0.0f, 0.0f, -20.0f ),    // the camera position
		&D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),    // the look-at position
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );    // the up direction

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_VIEW, &matView );    // set the view transform to matView

	D3DXMATRIX matProjection;     // the projection transform matrix

	D3DXMatrixPerspectiveFovLH( &matProjection,
		D3DXToRadian( 45 ),    // the horizontal field of view
		(FLOAT)800 / (FLOAT)600, // aspect ratio
		1.0f,    // the near view-plane
		100.0f );    // the far view-plane

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProjection );    // set the projection

	*/

	TESTSCREEN->SetCamera( Point3D( 0, 0, -20 ), Point3D( 0, 0, 50 ), Point3D( 0, 0, 0 ) );

																 // select the vertex buffer to display
	GRAPHICENGINE->GetDevice()->SetStreamSource( 0, v_buffer, 0, sizeof( CUSTOMVERTEX ) );

	// copy the vertex buffer to the back buffer
	GRAPHICENGINE->GetDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

//	GRAPHICENGINE->GetDevice()->EndScene();

//	GRAPHICENGINE->GetDevice()->Present( NULL, NULL, NULL, NULL );
}


void CTestScreen::SetCamera( Point3D sPosition, Point3D sLookAt )
{
	struct eCAMERA_TRACE
	{
		int x, y, z;
		int tx, ty, tz;

		EXEMatrixE	eRotMatrix;
		int AngX, AngY;
	};


	eCAMERA_TRACE eTrace;

	CALL_WITH_ARG7( 0x0048B540, (DWORD)&eTrace, sPosition.iX, sPosition.iY, sPosition.iZ, sLookAt.iX, sLookAt.iY, sLookAt.iZ );
	CALLT_WITH_ARG4( 0x0047F1A0, (DWORD)RENDERER, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)&eTrace.eRotMatrix );


	RENDERER->SetCameraPosition( sPosition.iX, sPosition.iY, sPosition.iZ, 0, 0, 0 );

	sAngleCamera.iX = eTrace.AngX;
	sAngleCamera.iY = eTrace.AngY;

	(*(int*)0x00A17520) = sPosition.iX;
	(*(int*)0x00A17510) = sPosition.iY;
	(*(int*)0x00A1619C) = sPosition.iZ;

	(*(int*)0x00824FA8) = eTrace.AngX;
	(*(int*)0x00A17630) = eTrace.AngY;

	D3DXVECTOR3 vPosition;
	vPosition.x = (float)sPosition.iX;
	vPosition.y = (float)sPosition.iY;
	vPosition.z = (float)sPosition.iZ;

	D3DXVECTOR3 vLookAt;
	vLookAt.x = (float)sLookAt.iX;
	vLookAt.y = (float)sLookAt.iY;
	vLookAt.z = (float)sLookAt.iZ;

	D3DXMATRIX matView;

	D3DXMatrixLookAtLH( &matView, &vPosition, &vLookAt, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_VIEW, &matView );    // set the view transform to matView

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );

	D3DXMatrixTranslation( &mat, vPosition.x, vPosition.y, 10.0f );


	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_WORLD, &mat );

	D3DXMATRIX matProjection;     // the projection transform matrix

	D3DXMatrixPerspectiveFovLH( &matProjection,
		D3DXToRadian( 45 ),    // the horizontal field of view
		(FLOAT)800 / (FLOAT)600, // aspect ratio
		1.0f,    // the near view-plane
		100000.0f );    // the far view-plane

	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProjection );    // set the projection
}

void CTestScreen::SetCamera( Point3D sPositionCamera, Point3D sPositionObject, Point3D sLookAt )
{
	D3DXVECTOR3 vPositionObject;
	vPositionObject.x = (float)sPositionObject.iX;
	vPositionObject.y = (float)sPositionObject.iY;
	vPositionObject.z = (float)sPositionObject.iZ;

	D3DXVECTOR3 vPosition;
	vPosition.x = (float)sPositionCamera.iX;
	vPosition.y = (float)sPositionCamera.iY;
	vPosition.z = (float)sPositionCamera.iZ;

	D3DXVECTOR3 vLookAt;
	vLookAt.x = (float)sLookAt.iX;
	vLookAt.y = (float)sLookAt.iY;
	vLookAt.z = (float)sLookAt.iZ;

	//Set Camera Position and where look at
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vPosition, &vLookAt, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_VIEW, &matView );

	//Object Position
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixTranslation( &matWorld, vPositionObject.x, vPositionObject.y, vPositionObject.z );

	//Pitch Angle rotation
	D3DXMATRIX matRot;
	D3DXMatrixIdentity( &matRot );
	D3DXMatrixRotationX( &matRot, (float)((int)fAngleH % 360) );
	D3DXMatrixMultiply( &matWorld, &matRot, &matWorld );

	//Set World
	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_WORLD, &matWorld );

	//Set Projection
	D3DXMATRIX matProjection; 
	D3DXMatrixPerspectiveFovLH( &matProjection, D3DXToRadian( 45 ), (FLOAT)800 / (FLOAT)600, 20.0f, 4000.0f * 256.0f );
	GRAPHICENGINE->GetDevice()->SetTransform( D3DTS_PROJECTION, &matProjection ); 
}

void CTestScreen::Render2D()
{
}

EScreen CTestScreen::Frame()
{
	pcUnitData->Main();

	return SCREEN_Test;
}

void CTestScreen::Update( float fTime )
{
	//PARTICLEENGINE->Update( fTime );
}


void CTestScreen::Render3D()
{

	//Sky::GetInstance()->SetSky( 15 );

	SetCamera( sPositionCamera, sPointPosition );

	if ( dwTime <= GetTickCount() )
	{
		dwTime = GetTickCount() + 30;
	}

	

//	pcStage->Render( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, 0 );

	SetCharacterCamera( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, sAngleCamera.iZ );

// RENDERER->pcModelCache = pcModelCache;

		//pcUnitData->Render();

	DrawShadowUnit();


	TESTSCREEN->SetCamera( Point3D( 0, 0, -20 ), sPositionNew, Point3D( 0, 0, 0 ) );

//	pcModelCache->Render();
	

	//render_frame();

	RENDERER->pcModelCache = NULL;


//	PARTICLEENGINE->Render3D( NULL );

// 	Sky::Draw( 0, 0, 0, 0, 0, 0 );

	D3DXMATRIX matp;
	GRAPHICENGINE->GetDevice()->GetTransform( D3DTS_WORLD, &matp );

	int iY = 10;

	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object X:        %d [%d]", sPosition.iX, sPosition.iX >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object Y:        %d [%d]", sPosition.iY, sPosition.iY >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object Z:        %d [%d]", sPosition.iZ, sPosition.iZ >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object AX:       %d [%d]", sAngle.iX, sAngle.iX >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object AY:       %d [%d]", sAngle.iY, sAngle.iY >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object AZ:       %d [%d]", sAngle.iZ, sAngle.iZ >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Object Anim:     %d [%d]", pcUnitData->iFrame, pcUnitData->iFrame / 160 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera X:        %d [%d]", sPositionCamera.iX, sPositionCamera.iX >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera Y:        %d [%d]", sPositionCamera.iY, sPositionCamera.iY >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera Z:        %d [%d]", sPositionCamera.iZ, sPositionCamera.iZ >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera AX:       %d [%d]", sAngleCamera.iX, sAngleCamera.iX >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera AY:       %d [%d]", sAngleCamera.iY, sAngleCamera.iY >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "Camera AZ:       %d [%d]", sAngleCamera.iZ, sAngleCamera.iZ >> 8 ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "World X:       %f", matp.m[3][0] ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "World Y:       %f", matp.m[3][1] ), 0, -1 );
	iY += 18;
	FONTHANDLER->Draw( 0, 10, iY, FormatString( "World Z:       %f", matp.m[3][2] ), 0, -1 );

	CALLT( 0x0047F200, (DWORD)RENDERER );


	(*(UINT*)0x04B0D364) = TRUE;
}

void CTestScreen::OnMouseMove( CMouse * pcMouse )
{
#ifdef TEST_SCREEN

	Point2D * ps = pcMouse->GetPosition();



#endif
}

BOOL CTestScreen::OnMouseClick( CMouse * pcMouse )
{
	return FALSE;
}

BOOL CTestScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
	if( pcKeyboard->GetKey() == 'Q' )
		sPositionCamera.iX += 1000;

	if( pcKeyboard->GetKey() == 'W' )
		sPositionCamera.iY += 1000;

	if( pcKeyboard->GetKey() == 'E' )
		sPositionCamera.iZ += 1000;

	if( pcKeyboard->GetKey() == 'A' )
		sPointPosition.iX += 1000;

	if ( pcKeyboard->GetKey() == 'S' )
	{
		sPointPosition.iY += 1000;
		fAngleH += 10.0f;
	}
	if ( pcKeyboard->GetKey() == 'D' )
	{
		sPointPosition.iZ += 1000;
		sPositionNew.iZ += 256;
	}
	return FALSE;
}

void CTestScreen::DrawShadowUnit()
{
	DX::cSelectGlow1.Begin( D3DCOLOR_XRGB(0, 255, 0), TRUE );
	pcUnitData->Render();
	DX::cSelectGlow1.End( TRUE );
	DX::cSelectGlow1.Begin( D3DCOLOR_XRGB( 0, 255, 0 ), FALSE );
	pcUnitData->Render();
	DX::cSelectGlow1.End( FALSE );

	DX::cSelectGlow1.Draw();
	pcUnitData->Render();
}
