#include "stdafx.h"
#include "CLoginScreen.h"

UI::Image * p = nullptr;
const std::string CLoginScreen::loginVideoAnimationPath = "game\\textures\\misc\\login.asf";
const std::string CLoginScreen::backgroundSelectorImagePath = "game\\images\\login\\bg_selector.png";
const std::string CLoginScreen::backgroundServersSelectorImagePath = "game\\images\\login\\bg_servers.png";

CLoginScreen::CLoginScreen()
{
	CALLT((DWORD)ELoginScreenFunction::LOGIN_SCREEN_CONSTRUCTOR, GetThis());
	loginScreenSelector = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH >> 2) - 120, RESOLUTION_HEIGHT - 120, 200, 75));
	loginScreenSelector->SetImage(UI::ImageLoader::LoadImage(this->backgroundSelectorImagePath.c_str()));

	this->serverSelector = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH / 2) - (395 / 2), (RESOLUTION_HEIGHT / 2), 395, 71));
	this->serverSelector2 = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH / 2)- (395 / 2), (RESOLUTION_HEIGHT / 2), 395, 71));
	//this->serverSelector2->SetImage(UI::ImageLoader::LoadImage(this->backgroundServersSelectorImagePath.c_str()));

	// Seasonal Info Text
	pWindowSeasonalInfo = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH/2)-(570/2), (RESOLUTION_HEIGHT/2), 570, 84));
	pWindowSeasonalInfo->SetImage(UI::ImageLoader::LoadImage("game\\images\\login\\seasonal_overlay.png"));
}

CLoginScreen::~CLoginScreen()
{
	CALLT((DWORD)ELoginScreenFunction::LOGIN_SCREEN_DECONSTRUCTOR, GetThis());
	loginScreenSelector.reset();
	this->serverSelector.reset();
	this->serverSelector2.reset();
}

BOOL CLoginScreen::Init()
{
	CALLT( 0x0059E570, GetThis() );
	CALL( 0x0066D060 );

	EXETexture::GetInstance()->ReadTextures();

	if ( RESOLUTION_WIDTH < 1024 && RESOLUTION_HEIGHT < 768 )
	{
		this->setCompatibleResolution();
	}

	INI::CReader cReader( "game.ini" );
	BOOL bImageBasedLogin = cReader.ReadBool( "Login", "ImageBasedLogin" );

	int iCurrentSelectedWorld = cReader.ReadInt( "Login", "SelectedServerNum" );

	#if defined (SKILLS_TESTING_TEMP) //skills testing 2.0
	{
		iCurrentSelectedWorld = WORLDID_Seasonal;
	}
	#elif defined (DEV_MODE) //dev mode
	{
		iCurrentSelectedWorld = WORLDID_Dev;
	}

	#elif defined (SEASONAL_IS_ACTIVE) //seasonal is active!
	{
		iCurrentSelectedWorld = WORLDID_Seasonal;
	}
	#else //normal - there is only 'Babel'
	{
		iCurrentSelectedWorld = WORLDID_Babel;
	}
	#endif


	this->currentSelectedWorld = iCurrentSelectedWorld;

	BOOL isImageBasedLogin = cReader.ReadBool ( "Login", "ImageBasedLogin" );

	//Video Selector
	Rectangle2D uiPosition(10, 10, 13, 13);

	uiPosition.iY += 20;
	this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::IS_IMAGE_BACKGROUND, isImageBasedLogin, "Image Only", this->loginScreenSelector));

	uiPosition.iY += 20;
	this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::IS_VIDEO_BACKGROUND, !isImageBasedLogin, "Animated Login", this->loginScreenSelector));


#ifdef DEV_MODE
	// Server selector
	uiPosition.iY = 5;
	uiPosition.iX = 165;




	auto babelCheckBox =this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::WORLD_ID_BABEL, FALSE, "Main", this->serverSelector2));

	uiPosition.iY += 20;

#if defined (SKILLS_TESTING_TEMP)
	babelCheckBox->SetEnabled ( FALSE );
	auto seasonalCheckBox = this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::WORLD_ID_SEASONAL, FALSE, "Seasonal", this->serverSelector2));
#else
	auto seasonalCheckBox = this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::WORLD_ID_SEASONAL, FALSE, "Seasonal", this->serverSelector2));
#ifndef SEAONSAL_IS_ACTIVE
	seasonalCheckBox->SetEnabled ( FALSE );
#endif // !SEAONSAL_IS_ACTIVE

#endif



	#ifdef DEV_MODE
	{
		uiPosition.iY += 20;
		this->createCheckbox( uiPosition, checkboxData( (int)CHECKBOX_ID::WORLD_ID_DEV, FALSE, "Developer", this->serverSelector2 ) );
	}
	#endif

	#if defined(SEASONAL_IS_ACTIVE) || defined (SKILLS_TESTING_TEMP)
	{
		this->serverSelector2->GetElement<UI::CheckBox>( (int)CHECKBOX_ID::WORLD_ID_SEASONAL )->SetCheck( TRUE );
	}
	#elif defined (DEV_MODE)
	{
		this->serverSelector2->GetElement<UI::CheckBox>( (int)CHECKBOX_ID::WORLD_ID_DEV )->SetCheck( TRUE );
	}
	#endif

#endif

	GAMEWORLD->SetWorld ( (EWorldID)this->currentSelectedWorld );
	GAMEWORLD->Apply ();

	this->currentSelectedLoginRender = isImageBasedLogin ? (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND : (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND;
	onCheckboxClick(isImageBasedLogin ? (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND : (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND, UIEventArgs{});

	return TRUE;
}

void CLoginScreen::Shutdown()
{
	DXVIDEORENDERER->Shutdown();
}

void CLoginScreen::Render3D()
{
	LOGINHANDLER->RenderServerSelectBox (); //login box
	DXVIDEORENDERER->Render();
	CALLT ( 0x0059E7A0, GetThis () );
}

void CLoginScreen::Render2D()
{
	this->serverSelector->SetPosition((RESOLUTION_WIDTH/2) - 60, (RESOLUTION_HEIGHT/2) + 90);

	if ( GAMESCREEN->GetActiveScreenType () == SCREEN_Login )
	{
		// Position Seasonal Info Text
		pWindowSeasonalInfo->SetPosition((RESOLUTION_WIDTH/2) - (570/2), (RESOLUTION_HEIGHT/2) - 350);

		if ( ( (CLoginScreen *)GAMESCREEN->GetActiveScreen () )->GetCurrentState () != ELoginState::LOGINSTATE_ServerSelect )
		{
			this->serverSelector2->SetPosition ( ( RESOLUTION_WIDTH / 2 ) - ( 190 ), ( RESOLUTION_HEIGHT / 2 ) + 73 );
		}
		else
		{
			this->serverSelector2->SetPosition ( -100, -100 );
		}
	}

	loginScreenSelector->SetPosition ( ( RESOLUTION_WIDTH >> 2 ) - 120, RESOLUTION_HEIGHT - 120 );
	this->serverSelector->Render ( 0, 0, DX::Graphic::GetWidthBackBuffer (), DX::Graphic::GetHeightBackBuffer (), 0, 0 );

	if ( GAMESCREEN->GetActiveScreenType () == SCREEN_Login )
	{
		if ( ( (CLoginScreen *)GAMESCREEN->GetActiveScreen () )->GetCurrentState () != ELoginState::LOGINSTATE_ServerSelect )
		{
			this->serverSelector2->Render ( 0, 0, DX::Graphic::GetWidthBackBuffer (), DX::Graphic::GetHeightBackBuffer (), 0, 0 );

			// Render Seasonal Info Text
			// pWindowSeasonalInfo->Render ( 0, 0, DX::Graphic::GetWidthBackBuffer (), DX::Graphic::GetHeightBackBuffer (), 0, 0 );
		}
	}

	loginScreenSelector->Render ( 0, 0, DX::Graphic::GetWidthBackBuffer (), DX::Graphic::GetHeightBackBuffer (), 0, 0 );

	CALLT( 0x0059D790, GetThis() );
}

EScreen CLoginScreen::Frame()
{
	CALLT(0x0059D860, GetThis());

	DXVIDEORENDERER->Update();

	SOCKETGAME->PingConnections();

	if (NewScreen())
		return SCREEN_Character;

	return SCREEN_Login;
}

void CLoginScreen::OnMouseMove(CMouse * pcMouse)
{
	SETTINGSHANDLER->GetWindow()->OnMouseMove(pcMouse);
	loginScreenSelector->OnMouseMove(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
	this->serverSelector2->OnMouseMove(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}

BOOL CLoginScreen::OnMouseClick(CMouse * pcMouse)
{
	if (loginScreenSelector->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;

	if (this->serverSelector2->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;

	if (pcMouse->GetEvent() == EMouseEvent::ClickDownL)
		bShowNotify = FALSE;

	return FALSE;
}

void CLoginScreen::OnUpdateResolution()
{
	if (!MESSAGEBOX->GetType())
	{
		return;
	}

	SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1024;
	SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
	SETTINGSHANDLER->GetModel()->WriteSettings();

	GRAPHICENGINE->ResolutionChange(FALSE, 1024, 768);
	GRAPHICENGINE->Reset(SETTINGSHANDLER->GetModel()->GetSettings().bWindowed, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight, TRUE);

	DXGraphicEngine::ResizeWindow();
}

void CLoginScreen::onCheckboxClick(int checkboxID, UIEventArgs e)
{
	switch (checkboxID)
	{
		case (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND:
			this->setImageModeBackground();
			break;
		case (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND:
			this->setVideoModeBackground();
			break;
		case (int)CHECKBOX_ID::WORLD_ID_BABEL:
		case (int)CHECKBOX_ID::WORLD_ID_SEASONAL:
		case (int)CHECKBOX_ID::WORLD_ID_DEV:
			this->setCurrentWorld(checkboxID);
			break;
	}
}

void CLoginScreen::setVideoModeBackground()
{
  	if (!DXVIDEORENDERER->Init(this->loginVideoAnimationPath))
	{
		return;
	}

	DXVIDEORENDERER->SetFPS(60);
	DXVIDEORENDERER->Loop(TRUE);
	DXVIDEORENDERER->SetPosition(DXVIDEORENDERER->GetPosition());
	DXVIDEORENDERER->Run();

	this->isLoginVideo = TRUE;
	this->loginScreenSelector->GetElement<UI::CheckBox>((int)CHECKBOX_ID::IS_VIDEO_BACKGROUND)->SetCheck(TRUE);
	this->loginScreenSelector->GetElement<UI::CheckBox>(this->currentSelectedLoginRender)->SetCheck(FALSE);
	this->currentSelectedLoginRender = (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND;

	INI::CReader cReader("game.ini");
	cReader.WriteBool("Login", "ImageBasedLogin", FALSE);
}

void CLoginScreen::setImageModeBackground()
{
	DXVIDEORENDERER->Shutdown();

	this->isLoginVideo = FALSE;
	this->loginScreenSelector->GetElement<UI::CheckBox>((int)CHECKBOX_ID::IS_IMAGE_BACKGROUND)->SetCheck(TRUE);
	this->loginScreenSelector->GetElement<UI::CheckBox>(this->currentSelectedLoginRender)->SetCheck(FALSE);
	this->currentSelectedLoginRender = (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND;

	INI::CReader cReader("game.ini");
	cReader.WriteBool("Login", "ImageBasedLogin", TRUE);
}

void CLoginScreen::setCurrentWorld(int worldID)
{
	if (this->currentSelectedWorld == worldID)
	{
		this->serverSelector2->GetElement<UI::CheckBox>(worldID)->SetCheck(TRUE);
		return;
	}

	GAMEWORLD->SetWorld((EWorldID)worldID);
	GAMEWORLD->Apply();

	this->serverSelector2->GetElement<UI::CheckBox>(this->currentSelectedWorld)->SetCheck(FALSE);
	this->serverSelector2->GetElement<UI::CheckBox>(worldID)->SetCheck(TRUE);

	this->currentSelectedWorld = worldID;

	INI::CReader cReader("game.ini");


#if defined (SEASONAL_IS_ACTIVE)
	cReader.WriteInt("Login", FormatString("SelectedServerNumS%d",  CURRENT_SEASONAL_NUM ), worldID);
#else
	cReader.WriteInt("Login", "SelectedServerNum", worldID);
#endif


}

void CLoginScreen::setCompatibleResolution()
{
	MESSAGEBOX->SetBoxType(CMessageBox::BOXTYPEID_OkCancel);
	MESSAGEBOX->SetTitle("Warning");
	MESSAGEBOX->SetDescription("Deprecated resolution! We will update your resolution.");
	MESSAGEBOX->SetEvent(std::bind(&CLoginScreen::OnUpdateResolution, this));
	MESSAGEBOX->Show();
}

UI::CheckBox_ptr CLoginScreen::createCheckbox(const Rectangle2D& checkBoxRectangle, checkboxData& checkboxInformation)
{
	UI::CheckBox_ptr checkBox = std::make_shared<UI::CheckBox>(checkBoxRectangle);
	checkBox->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	checkBox->SetText(checkboxInformation.text);
	checkBox->SetID(checkboxInformation.id);
	checkBox->SetCheck(checkboxInformation.state);
	checkBox->SetEvent(UI::Event::Build(std::bind(&CLoginScreen::onCheckboxClick, this, checkboxInformation.id, std::placeholders::_1)));
	checkBox->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage("game\\images\\UI\\checkbox_.png"));
	checkboxInformation.windowSelector->AddElement(checkBox);

	return checkBox;
}
