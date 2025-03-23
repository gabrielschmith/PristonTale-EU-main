#include "DXGraphicEngine.h"
#include "DXGraphicsDevice.h"
#include "DXFont.h"
#include "CSound.h"
#include "DXCamera.h"
#include "DXLogger.h"
#include "DXPostProcess.h"
#include "DXGlow.h"
#include "DXTexture.h"
#include "DXImageSceneRenderer.h"
#include "DXTextureEngine.h"
#include "DXSprite.h"
#include "DXSelectGlow.h"

namespace DX
{
	// DXGraphicEngine
	typedef std::shared_ptr<class DXGraphicEngine>	Graphic_ptr;
	typedef class DXGraphicEngine					Graphic;

	// DXFont
	typedef std::shared_ptr<class DXFont>			Font_ptr;
	typedef class DXFont							Font;

	// DXFont
	typedef std::shared_ptr<class CSound>			Sound_ptr;
	typedef class CSound							Sound;
	
	// DXCamera
	typedef class CDXCamera							Camera;
	typedef std::shared_ptr<class CDXCamera>		Camera_ptr;

	// DXLogger
	typedef class DXLogger							Logger;
	typedef std::shared_ptr<class DXLogger>			Logger_ptr;

	// DXPostProcess
	typedef class DXPostProcess						PostProcess;
	typedef class DXGlow							Glow;

	// DXImageSceneRenderer
	typedef class DXImageSceneRenderer					ImageSceneRenderer;
	typedef std::shared_ptr<class DXImageSceneRenderer>	ImageSceneRenderer_ptr;
};