#include "stdafx.h"
#include "CStableWindow.h"

void CStableWindow::Update(float fTime)
{
}

void CStableWindow::Render()
{

	pWindow1->Render(0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}

void CStableWindow::Init()
{
	BuildWindow();
}

void CStableWindow::AddPet()
{
	// Buttons on Child Tab
	UI::Button_ptr pButtonPet1 = std::make_shared<UI::Button>(Rectangle2D(198, 58, 12, 12));
	pButtonPet1->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Stable\\H_Summon.png", "game\\images\\Stable\\Summon.png"));
	pButtonPet1->SetEvent(UI::Event::Build(std::bind(&CStableWindow::SendActivePet, this, 1)));
	pWindow1->AddElement(pButtonPet1);
}

void CStableWindow::SendActivePet(int e)
{
}

void CStableWindow::BuildWindow()
{
	pWindow1 = std::make_shared<UI::Window>(Rectangle2D(0, 0, 254, 439));

	//Base Rectangle
	int iWindowWidth = pWindow1->GetWidth();
	int iWindowHeight = pWindow1->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

	pWindow1->SetPosition(iWindowX, iWindowY);
	pWindow1->SetImage(UI::ImageLoader::LoadImage("game\\images\\Stable\\Base.png"));


	// List
	UI::List_ptr pListPets = std::make_shared<UI::List>(Rectangle2D(35, 150, 180, 210));
	pListPets->SetID(LISTID_Pet);
	pListPets->MakeScrollBar(Rectangle2D(180, 0, 6, 200), "game\\images\\Stable\\scrollbar.png", "game\\images\\Stable\\ScrollIcon.png");
	pWindow1->AddElement(pListPets);


	//button Pets Change name
	UI::Button_ptr pButtonPetChangeName = std::make_shared<UI::Button>(Rectangle2D(180, 120, 10, 10));
	pButtonPetChangeName->SetCanHold(TRUE);
	pButtonPetChangeName->SetSelect(TRUE);
	pButtonPetChangeName->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Stable\\EditIcon.png", "game\\images\\Stable\\EditIcon.png"));
	pWindow1->AddElement(pButtonPetChangeName);


	//button Pets title
	UI::Button_ptr pButtonPetActive = std::make_shared<UI::Button>(Rectangle2D(90, 70, 73, 21));
	pButtonPetActive->SetCanHold(TRUE);
	pButtonPetActive->SetSelect(TRUE);
	pButtonPetActive->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Stable\\Pets.png", "game\\images\\Stable\\H_Pets.png"));
	pWindow1->AddElement(pButtonPetActive);

	pWindow1->SetCanMove(TRUE);
	pWindow1->Hide();
}

void CStableWindow::OnClickPetButton(int iPetID)
{
	STABLEHANDLER->OnClickPetButton(iPetID);
	auto pButton = pWindow1->GetElement<UI::List>(iPetID);
	
}

void CStableWindow::RequestPetData()
{
	PacketStablePetTab PetTab;
	ZeroMemory(&PetTab,sizeof(PetTab));

	PetTab.iHeader = PKTHDR_SendStablePet;
	PetTab.iLength = sizeof(PetTab);
	SENDPACKETG(&PetTab, 0);
}

void CStableWindow::LoadPets(PacketStablePetTab* sPacketStablePetTab)
{
	auto pListPets= pWindow1->GetElement<UI::List>(LISTID_Pet);
	pListPets->Clear();

	pListPets->SetPaddingSeparator(6);

	// Name PET of Owner
	UI::Text_ptr pPetName = std::make_shared<UI::Text>(Rectangle2D(90, 110, 235, 16));
	pPetName->SetText(sPacketStablePetTab->sCStablePetTab.cPetNameOwner);
	pPetName->SetColor(D3DCOLOR_ARGB(255, 194, 165, 110));
	pWindow1->AddElement(pPetName);

	if (pListPets)
	{
		
		int i = 0;
		for (i = 0; i < MAX_PET_STABLE - 1; i++)
		{
			UI::Image* pPetImage = UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Premium\\itbi320.bmp");
			if (pPetImage)
			{
				int iImageWidth = pPetImage->GetWidth();
				int iImageHeight = pPetImage->GetHeight();

				int iImagePaddingX = 44 - (iImageWidth >> 1);
				int iImagePaddingY = 44 - (iImageHeight >> 1);

				// Create Window of Item
				UI::Window_ptr pWindowPet = std::make_shared<UI::Window>(Rectangle2D(0, 0, 180, 70));
				pWindowPet->SetID(sPacketStablePetTab->sCStablePetTab.sCStablePetTab[i].sPetId);

				//buton active/desactive pet
				UI::Button_ptr pButtonPetActive = std::make_shared<UI::Button>(Rectangle2D(150, 25, 17, 16));
				pButtonPetActive->SetID(i);
				pButtonPetActive->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Stable\\Summon.png", "game\\images\\Stable\\H_Summon.png"));
				pButtonPetActive->SetEvent(UI::Event::Build(std::bind(&CStableWindow::OnClickPetButton, this, pButtonPetActive->GetID())));
				if (sPacketStablePetTab->sCStablePetTab.sCStablePetTab[i].bActivePet)
					pButtonPetActive->SetSelect(TRUE);
				pWindowPet->AddElement(pButtonPetActive);

				// Image
				UI::ImageBox_ptr pItemImageBox = std::make_shared<UI::ImageBox>(Rectangle2D(iImagePaddingX - 5, iImagePaddingY - 5, iImageWidth, iImageHeight));
				pItemImageBox->SetImage(pPetImage);
				pWindowPet->AddElement(pItemImageBox);

				// Name
				UI::Text_ptr pPetName = std::make_shared<UI::Text>(Rectangle2D(65, 18, 235, 16));
				pPetName->SetText(sPacketStablePetTab->sCStablePetTab.sCStablePetTab[i].cPetName);
				pPetName->SetColor(D3DCOLOR_ARGB(255, 194, 165, 110));
				pWindowPet->AddElement(pPetName);

				UI::Text_ptr pPetRarity = std::make_shared<UI::Text>(Rectangle2D(65, 35, 235, 16));
				pPetRarity->SetText(GetRarityNameByID(sPacketStablePetTab->sCStablePetTab.sCStablePetTab[i].sStablePetRarity));
				pPetRarity->SetColor(D3DCOLOR_ARGB(255, 194, 165, 110));
				pWindowPet->AddElement(pPetRarity);


				pListPets->AddWindow(pWindowPet);
			}
		}
	}
	if (!pWindow1->IsOpen())
		pWindow1->Show();
	
}



std::string CStableWindow::GetRarityNameByID(StablePetRarity sStablePetRarit)
{
	if (sStablePetRarit == PETCOMMON)
		return "Common";
	else if (sStablePetRarit == PETUNCOMMON)
		return "Uncommon";
	else if (sStablePetRarit == PETRARE)
		return "Rare";
	else if (sStablePetRarit == PETEPIC)
		return "Epic";
	else if (sStablePetRarit == PETLEGENDARY)
		return "Legendary";

	return "Common";
}
void CStableWindow::OnMouseMove(CMouse* pcMouse)
{
	pWindow1->OnMouseMove(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}

BOOL CStableWindow::OnMouseClick(CMouse* pcMouse)
{
	if (pWindow1->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;

	return FALSE;
}

BOOL CStableWindow::OnMouseScroll(CMouse* pcMouse)
{
	if (pWindow1->OnMouseScroll(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;

	return FALSE;
}

BOOL CStableWindow::OnKeyPress(CKeyboard* pcKeyBoard)
{
	if (pcKeyBoard->GetKey() == 'K')
	{
		if (!pWindow1->IsOpen() && GameCore::GetInstance()->sServerTime.wDay && pcKeyBoard->GetEvent() == EKeyboardEvent::KeyDown)
		{
			RequestPetData();
			return TRUE;
		}
		else if ((pcKeyBoard->IsKeyDown('K') || pcKeyBoard->GetKey() == VK_ESCAPE))
		{
			pWindow1->Hide();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStableWindow::OnKeyChar(CKeyboard* pcKeyBoard)
{
	return 0;
}

void CStableWindow::OnResolutionChanged()
{
}
