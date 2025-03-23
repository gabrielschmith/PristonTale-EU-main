#include "stdafx.h"
#include "DX.h"

namespace DX
{

GraphicsDevice_ptr pGraphicsDevice;
SoundDevice_ptr pSoundDevice;
SoundCaptureDevice_ptr pSoundCaptureDevice;

void Init( LPDIRECT3DDEVICE9 lpD3DDevice, Engine::Window_ptr pWindow )
{
	//Graphics
	{
		pGraphicsDevice = std::make_shared<GraphicsDevice>( lpD3DDevice );
	}

	//Sound
	{
		pSoundDevice = std::make_shared<SoundDevice>();
		pSoundDevice->Init( pWindow );
	}

	//Sound Capture
	{
		pSoundCaptureDevice = std::make_shared<SoundCaptureDevice>();
		pSoundCaptureDevice->Init( pWindow );
	}
}

void Shutdown()
{
	//Sound Capture
	if( pSoundCaptureDevice )
	{
		pSoundCaptureDevice->Shutdown();

		pSoundCaptureDevice = nullptr;
	}

	//Sound
	if( pSoundDevice )
	{
		pSoundDevice->Shutdown();

		pSoundDevice = nullptr;
	}

	//Graphics
	if( pGraphicsDevice )
	{
		pGraphicsDevice = nullptr;
	}
}

void OnLostDevice()
{
	pGraphicsDevice->OnLostDevice();
}

void OnResetDevice()
{
	pGraphicsDevice->OnResetDevice();
}

GraphicsDevice_ptr GetGraphicsDevice()
{
	return pGraphicsDevice;
}

SoundDevice_ptr GetSoundDevice()
{
	return pSoundDevice;
}

SoundCaptureDevice_ptr GetSoundCaptureDevice()
{
	return pSoundCaptureDevice;
}

}