#pragma once

#include "Window.h"

#include "DXSoundFactory.h"
#include "DXSoundManager.h"

namespace DX
{

typedef std::shared_ptr<class SoundDevice> SoundDevice_ptr;

class SoundDevice : public std::enable_shared_from_this<SoundDevice>
{
public:
							  SoundDevice();
	virtual					 ~SoundDevice();

	BOOL					  Init( Engine::Window_ptr _pWindow );
	void					  Shutdown();

	BOOL					  HasDevice() const;

	Engine::Window_ptr		  GetWindow() { return pWindow; }

	LPDIRECTSOUND8			  Get() { return lpDSoundDevice; }

	SoundFactory_ptr		  GetSoundFactory() { return pSoundFactory; }
	SoundManager_ptr		  GetSoundManager() { return pSoundManager; }

private:
	Engine::Window_ptr		  pWindow;

	LPDIRECTSOUND8			  lpDSoundDevice = NULL;

	SoundFactory_ptr		  pSoundFactory;
	SoundManager_ptr		  pSoundManager;
};

}