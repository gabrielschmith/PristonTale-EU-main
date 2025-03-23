#include "stdafx.h"
#include "NeverSinkHandler.h"

NeverSinkHandler::NeverSinkHandler()
{
}

NeverSinkHandler::~NeverSinkHandler()
{
}

void NeverSinkHandler::BuildWindow()
{
	this->neverSinkWindow = std::make_shared<UI::Window>(Rectangle2D(0, 0, NEVERSINK_WINDOW_WIDTH, NEVERSINK_WINDOW_HEIGHT));
	this->neverSinkWindow->SetImage(UI::ImageLoader::LoadImage("game\\images\\UI\\NeverSink\\window.png"));
	this->UpdateWindowPosition();

	UI::Button_ptr quitButton = std::make_shared<UI::Button>(Rectangle2D(QUIT_BUTTON_X_POSITION, QUIT_BUTTON_Y_POSITION, BUTTON_WHEEL_WIDTH, QUIT_BUTTON_HEIGHT));
	quitButton->SetEvent(UI::Event::Build(std::bind(&NeverSinkHandler::OnQuitButtonClick, this, std::placeholders::_1)));
	quitButton->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\NeverSink\\quitButton.png", "game\\images\\UI\\NeverSink\\quitButton.png"));
	this->neverSinkWindow->AddElement(quitButton);

	UI::Button_ptr ricartenButton = std::make_shared<UI::Button>(Rectangle2D(RICARTEN_X_POSITION - (BUTTON_WHEEL_WIDTH / 2), RICARTEN_Y_POSITION - (BUTTON_WHEEL_HEIGHT / 2), BUTTON_WHEEL_WIDTH, BUTTON_WHEEL_HEIGHT));
	ricartenButton->SetEvent(UI::Event::Build(std::bind(&NeverSinkHandler::OnLocationClick, this, std::placeholders::_1, MAPID_RicartenTown)));
	ricartenButton->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\NeverSink\\btn_wheel.png", "game\\images\\UI\\NeverSink\\btn_wheel_.png"));
	this->neverSinkWindow->AddElement(ricartenButton);

	UI::Button_ptr blessCastleButton = std::make_shared<UI::Button>(Rectangle2D(BLESS_CASTLE_X_POSITION - (BUTTON_WHEEL_WIDTH / 2), BLESS_CASTLE_Y_POSITION - (BUTTON_WHEEL_HEIGHT / 2), BUTTON_WHEEL_WIDTH, BUTTON_WHEEL_HEIGHT));
	blessCastleButton->SetEvent(UI::Event::Build(std::bind(&NeverSinkHandler::OnLocationClick, this, std::placeholders::_1, MAPID_BlessCastle)));
	blessCastleButton->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\NeverSink\\btn_wheel.png", "game\\images\\UI\\NeverSink\\btn_wheel_.png"));
	this->neverSinkWindow->AddElement(blessCastleButton);

	UI::Button_ptr castleOfTheLostButton = std::make_shared<UI::Button>(Rectangle2D(CASTLE_OF_THE_LOST_X_POSITION - (BUTTON_WHEEL_WIDTH / 2), CASTLE_OF_THE_LOST_Y_POSITION - (BUTTON_WHEEL_HEIGHT / 2), BUTTON_WHEEL_WIDTH, BUTTON_WHEEL_HEIGHT));
	castleOfTheLostButton->SetEvent(UI::Event::Build(std::bind(&NeverSinkHandler::OnLocationClick, this, std::placeholders::_1, MAPID_CastleOfTheLost)));
	castleOfTheLostButton->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\NeverSink\\btn_wheel.png", "game\\images\\UI\\NeverSink\\btn_wheel_.png"));
	this->neverSinkWindow->AddElement(castleOfTheLostButton);

	UI::Button_ptr landOfChaosButton = std::make_shared<UI::Button>(Rectangle2D(LAND_OF_CHAOS_X_POSITION - (BUTTON_WHEEL_WIDTH / 2), LAND_OF_CHAOS_Y_POSITION - (BUTTON_WHEEL_HEIGHT / 2), BUTTON_WHEEL_WIDTH, BUTTON_WHEEL_HEIGHT));
	landOfChaosButton->SetEvent(UI::Event::Build(std::bind(&NeverSinkHandler::OnLocationClick, this, std::placeholders::_1, MAPID_LandOfChaos)));
	landOfChaosButton->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\NeverSink\\btn_wheel.png", "game\\images\\UI\\NeverSink\\btn_wheel_.png"));
	this->neverSinkWindow->AddElement(landOfChaosButton);

	this->neverSinkWindow->Hide();
}

bool NeverSinkHandler::OnKeyPress(CKeyboard* pcKeyBoard)
{
	if (Game::GetGameMode() != GAMEMODE_InGame || PLAYERDATA == NULL)
	{ 
		return false;
	}

	if (this->IsWindowOpen() && pcKeyBoard->GetKey() == VK_ESCAPE)
	{
		this->neverSinkWindow->Hide();

		return true;
	}

	return false;
}

void NeverSinkHandler::Render()
{
	if (!this->IsWindowOpen())
	{ 
		return;
	}

	this->neverSinkWindow->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

BOOL NeverSinkHandler::OnMouseClick(CMouse* pcMouse)
{
	if (!this->IsWindowOpen())
	{ 
		return FALSE;
	}

	if (this->neverSinkWindow->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL NeverSinkHandler::OnMouseScroll(CMouse* pcMouse)
{
	if (!this->IsWindowOpen())
	{ 
		return FALSE;
	}

	if (this->neverSinkWindow->OnMouseScroll(pcMouse, this->neverSinkWindow->GetX(), this->neverSinkWindow->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
	{
		return TRUE;
	}

	return FALSE;
}

void NeverSinkHandler::OnMouseMove(CMouse* pcMouse)
{
	if (!this->IsWindowOpen())
	{
		return;
	}

	this->neverSinkWindow->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

void NeverSinkHandler::HandlePacket(PacketNeverSinkOpenUI* psPacket)
{
	if (this->neverSinkWindow == NULL)
	{
		this->BuildWindow();
	}

	this->UpdateWindowPosition();
	this->neverSinkWindow->Show();
}

void NeverSinkHandler::OnResolutionChanged()
{
	if (!this->IsWindowOpen())
	{
		return;
	}

	this->UpdateWindowPosition();
}

void NeverSinkHandler::UpdateWindowPosition()
{
	this->neverSinkWindow->SetPosition((RESOLUTION_WIDTH >> 1) - (this->neverSinkWindow->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - ((this->neverSinkWindow->GetHeight()) >> 1) - 50);
}

BOOL NeverSinkHandler::IsWindowOpen()
{
	if (this->neverSinkWindow == NULL)
	{
		return FALSE;
	}

	return this->neverSinkWindow->IsOpen();
}

void NeverSinkHandler::OnLocationClick(UIEventArgs e, const int fieldID)
{
	this->neverSinkWindow->Hide();
	
	PacketNeverSinkTeleportRequest teleportRequest;

	teleportRequest.iLength = sizeof(PacketNeverSinkTeleportRequest);
	teleportRequest.iHeader = PKTHDR_NeverSinkTeleportRequest;
	teleportRequest.fieldID = fieldID;

	SENDPACKETG(&teleportRequest, TRUE);
}

void NeverSinkHandler::OnQuitButtonClick(UIEventArgs e)
{
	this->neverSinkWindow->Hide();
}
