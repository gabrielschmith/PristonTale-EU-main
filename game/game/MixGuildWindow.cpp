/*
*	STR MIX GUILD 2019
*	gabrielcarlos91@hotmail.com
*/
#include "stdafx.h"
#include "MixGuildWindow.h"
#define MAXMIXLIST 70

CMixGuildWindow::CMixGuildWindow()
{
}

CMixGuildWindow::~CMixGuildWindow()
{
}

void CMixGuildWindow::HandlePacket(PacketMixGuild* psPacket)
{
	if (!psPacket)
		return;

	if (!psPacketMixGuild)
		psPacketMixGuild = new PacketMixGuild;

	ZeroMemory(psPacketMixGuild, sizeof(PacketMixGuild));

	CopyMemory(psPacketMixGuild, psPacket, sizeof(PacketMixGuild));

	// open window
	if (!pWindow1->IsOpen())
	{

		this->Open(); // open window

		MOUSEHANDLER->BlockMouse();
	}
}

void CMixGuildWindow::OnMouseMove(class CMouse* pcMouse)
{
	pWindow1->OnMouseMove(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}

BOOL CMixGuildWindow::OnMouseClick(class CMouse* pcMouse)
{
	if (pWindow1->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;
	return FALSE;
}

BOOL CMixGuildWindow::OnMouseScroll(class CMouse* pcMouse)
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_MixData);
	if (pWindow1->IsOpen() && pWindowItem)
	{
		if (pWindowItem->OnMouseScroll(pcMouse, pWindow1->GetX(), pWindow1->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
			return TRUE;
	}

	if (pWindow1->OnMouseScroll(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;
	return FALSE;
}

BOOL CMixGuildWindow::OnKeyPress(class CKeyboard* pcKeyboard)
{
	if (pWindow1->OnKeyPress(pcKeyboard))
		return TRUE;
	return FALSE;
}

BOOL CMixGuildWindow::OnKeyChar(class CKeyboard* pcKeyboard)
{
	if (pWindow1->OnKeyChar(pcKeyboard))
		return TRUE;
	return FALSE;
}

int CMixGuildWindow::GetImgSheltom(int IDItem)
{
	switch (IDItem)
	{
	case 1:
		return 101;
	case 2:
		return 102;
	case 3:
		return 103;
	case 4:
		return 104;
	case 5:
		return 105;
	case 6:
		return 106;
	case 7:
		return 107;
	case 8:
		return 108;
	case 9:
		return 109;
	case 10:
		return 110;
	case 11:
		return 111;
	case 12:
		return 112;
	case 13:
		return 113;
	default:
		break;
	}
}

void CMixGuildWindow::BuildWindow1()
{
	pWindow1 = std::make_shared<UI::Window>(Rectangle2D(20, 20, 762, 553));
	pWindow1->SetImage(UI::ImageLoader::LoadImage("game\\images\\mixguide\\window.png"));

	pWindow1->SetPosition((RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1));

	// Window Item Data
	UI::Window_ptr pWindow2 = std::make_shared<UI::Window>(Rectangle2D(20, 20, 750, 443));
	pWindow2->SetID(WINDOWID_MixData);
	pWindow1->AddElement(pWindow2);

	// List Mix
	UI::List_ptr pListText = std::make_shared<UI::List>(Rectangle2D(20, 24, 700, 500));
	pListText->SetID(LISTID_Items);
	pListText->MakeScrollBar(Rectangle2D(690, 65, 6, 445), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png");
	pWindow1->AddElement(pListText);

	// Close Button
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>(Rectangle2D(725, 25, 30, 28));
	pButtonClose->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\mixguide\\closebtn.png", "game\\images\\mixguide\\closebtn_.png"));
	pButtonClose->SetEvent(UI::Event::Build(std::bind(&CMixGuildWindow::OnCloseButtonClick, this, std::placeholders::_1)));
	pWindow1->AddElement(pButtonClose);

	{
		UI::Text_ptr pItemMixGuide = std::make_shared<UI::Text>(Rectangle2D(335, 15, 240, 14));
		pItemMixGuide->SetText("Mix Guide");
		pItemMixGuide->SetFont("Arial", 18, 0, FALSE, TRUE, D3DCOLOR_ARGB(150, 170, 150, 120));
		pWindow1->AddElement(pItemMixGuide);

		UI::Text_ptr pItemMixName = std::make_shared<UI::Text>(Rectangle2D(20, 55, 240, 14));
		pItemMixName->SetText("Mix Name");
		pItemMixName->SetFont("Arial", 16, 0, FALSE, TRUE, D3DCOLOR_ARGB(255, 210, 195, 180));
		pWindow1->AddElement(pItemMixName);

		UI::Text_ptr pItemSheltoms = std::make_shared<UI::Text>(Rectangle2D(228, 55, 240, 14));
		pItemSheltoms->SetText("Sheltoms");
		pItemSheltoms->SetFont("Arial", 16, 0, FALSE, TRUE, D3DCOLOR_ARGB(255, 210, 195, 180));
		pWindow1->AddElement(pItemSheltoms);

		UI::Text_ptr pItemDesc = std::make_shared<UI::Text>(Rectangle2D(505, 55, 240, 14));
		pItemDesc->SetText("Description");
		pItemDesc->SetFont("Arial", 16, 0, FALSE, TRUE, D3DCOLOR_ARGB(255, 210, 195, 180));
		pWindow1->AddElement(pItemDesc);

		// ComboBox
		UI::ComboBox_ptr pComboBox = std::make_shared<UI::ComboBox>(Rectangle2D(620, 55, 110, 26));
		pComboBox->SetDefaultText("Select");
		pComboBox->SetID(COMBOBOX_DESC);
		pComboBox->SetBackgroundImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\combobox\\drop.png", "game\\images\\UI\\combobox\\dropon.png"));
		pComboBox->SetBackgroundSelectedImage(UI::ImageLoader::LoadImage("game\\images\\UI\\combobox\\drop_.png"));
		pComboBox->SetBackgroundActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\combobox\\dropbg.png", "game\\images\\UI\\combobox\\dropdown.png"));
		pComboBox->SetHoverColor(D3DCOLOR_ARGB(150, 170, 150, 120));
		//Options
		{
			pComboBox->AddOption(pComboBox->MakeOption("All", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			/*pComboBox->AddOption(pComboBox->MakeOption("Weapons", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Amors/Robes", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Shields", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Orbs", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			/*pComboBox->AddOption(pComboBox->MakeOption("Orbs", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Shield", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Orb", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Ring", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Amulet", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));
			pComboBox->AddOption(pComboBox->MakeOption("Sheltom", Rectangle2D(0, 0, 110, 16), FALSE, D3DCOLOR_ARGB(255, 210, 195, 180)));*/
		}

		pComboBox->SetChangeEvent(UI::Event::Build(std::bind(&CMixGuildWindow::OnChangeMixSelect, this, std::placeholders::_1)));

		//MIX ITEM
		for (size_t i = 0; i < 70; i++)
		{
			UI::Window_ptr pWindowPlayer = std::make_shared<UI::Window>(Rectangle2D(0, 0, 450, 25));
			pWindowPlayer->SetID(WINDOWID_MIXITEM + i);

			// NAME MIX
			UI::Text_ptr pItemMixName = std::make_shared<UI::Text>(Rectangle2D(0, 65, 240, 14));
			pItemMixName->SetID(TEXTID_MixName + i);
			pItemMixName->SetText("");
			pItemMixName->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemMixName->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemMixName);

			// SHELTONS 01
			UI::Text_ptr pItemSheltoms1 = std::make_shared<UI::Text>(Rectangle2D(120, 65, 240, 14));
			pItemSheltoms1->SetID(TEXTID_SHELTONID01 + i);
			pItemSheltoms1->SetText("");
			pItemSheltoms1->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms1->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms1);

			// SHELTONS IMG 01
			//UI::ImageBox_ptr pItemImageBox1 = std::make_shared<UI::ImageBox>(Rectangle2D(125, 65, 22, 22));
			//pItemImageBox1->SetID(TEXTID_IMGSHELTON01 + i);
			//pItemImageBox1->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo101.bmp"));
			//pItemImageBox1->Hide();
			//pWindowPlayer->AddElement(pItemImageBox1);

			// SHELTONS 02
			UI::Text_ptr pItemSheltoms2 = std::make_shared<UI::Text>(Rectangle2D(145, 65, 240, 14));
			pItemSheltoms2->SetID(TEXTID_SHELTONID02 + i);
			pItemSheltoms2->SetText("");
			pItemSheltoms2->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms2->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms2);

			// SHELTONS IMG 02
			//UI::ImageBox_ptr pItemImageBox2 = std::make_shared<UI::ImageBox>(Rectangle2D(152, 65, 22, 22));
			//pItemImageBox2->SetID(TEXTID_IMGSHELTON02 + i);
			//pItemImageBox2->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo102.bmp"));
			//pItemImageBox2->Hide();
			//pWindowPlayer->AddElement(pItemImageBox2);

			// SHELTONS 03
			UI::Text_ptr pItemSheltoms3 = std::make_shared<UI::Text>(Rectangle2D(170, 65, 240, 14));
			pItemSheltoms3->SetID(TEXTID_SHELTONID03 + i);
			pItemSheltoms3->SetText("");
			pItemSheltoms3->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms3->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms3);

			// SHELTONS IMG 03
			//UI::ImageBox_ptr pItemImageBox3 = std::make_shared<UI::ImageBox>(Rectangle2D(175, 65, 22, 22));
			//pItemImageBox3->SetID(TEXTID_IMGSHELTON03 + i);
			//pItemImageBox3->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo103.bmp"));
			//pItemImageBox3->Hide();
			//pWindowPlayer->AddElement(pItemImageBox3);

			// SHELTONS 04
			UI::Text_ptr pItemSheltoms4 = std::make_shared<UI::Text>(Rectangle2D(195, 65, 240, 14));
			pItemSheltoms4->SetID(TEXTID_SHELTONID04 + i);
			pItemSheltoms4->SetText("");
			pItemSheltoms4->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms4->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms4);

			// SHELTONS IMG 04
			//UI::ImageBox_ptr pItemImageBox4 = std::make_shared<UI::ImageBox>(Rectangle2D(203, 65, 22, 22));
			//pItemImageBox4->SetID(TEXTID_IMGSHELTON04 + i);
			//pItemImageBox4->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo104.bmp"));
			//pItemImageBox4->Hide();
			//pWindowPlayer->AddElement(pItemImageBox4);

			// SHELTONS 05
			UI::Text_ptr pItemSheltoms5 = std::make_shared<UI::Text>(Rectangle2D(222, 65, 240, 14));
			pItemSheltoms5->SetID(TEXTID_SHELTONID05 + i);
			pItemSheltoms5->SetText("");
			pItemSheltoms5->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms5->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms5);

			// SHELTONS IMG 05
			//UI::ImageBox_ptr pItemImageBox5 = std::make_shared<UI::ImageBox>(Rectangle2D(228, 65, 22, 22));
			//pItemImageBox5->SetID(TEXTID_IMGSHELTON05 + i);
			//pItemImageBox5->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo105.bmp"));
			//pItemImageBox5->Hide();
			//pWindowPlayer->AddElement(pItemImageBox5);

			// SHELTONS 06
			UI::Text_ptr pItemSheltoms6 = std::make_shared<UI::Text>(Rectangle2D(247, 65, 240, 14));
			pItemSheltoms6->SetID(TEXTID_SHELTONID06 + i);
			pItemSheltoms6->SetText("");
			pItemSheltoms6->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms6->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms6);

			// SHELTONS IMG 06
			//UI::ImageBox_ptr pItemImageBox6 = std::make_shared<UI::ImageBox>(Rectangle2D(253, 65, 22, 22));
			//pItemImageBox6->SetID(TEXTID_IMGSHELTON06 + i);
			//pItemImageBox6->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo106.bmp"));
			//pItemImageBox6->Hide();
			//pWindowPlayer->AddElement(pItemImageBox6);

			// SHELTONS 07
			UI::Text_ptr pItemSheltoms7 = std::make_shared<UI::Text>(Rectangle2D(273, 65, 240, 14));
			pItemSheltoms7->SetID(TEXTID_SHELTONID07 + i);
			pItemSheltoms7->SetText("");
			pItemSheltoms7->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms7->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms7);

			// SHELTONS IMG 07
			//UI::ImageBox_ptr pItemImageBox7 = std::make_shared<UI::ImageBox>(Rectangle2D(280, 65, 22, 22));
			//pItemImageBox7->SetID(TEXTID_IMGSHELTON07 + i);
			//pItemImageBox7->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo107.bmp"));
			//pItemImageBox7->Hide();
			//pWindowPlayer->AddElement(pItemImageBox7);

			// SHELTONS 08
			UI::Text_ptr pItemSheltoms8 = std::make_shared<UI::Text>(Rectangle2D(300, 65, 240, 14));
			pItemSheltoms8->SetID(TEXTID_SHELTONID08 + i);
			pItemSheltoms8->SetText("");
			pItemSheltoms8->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms8->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms8);

			// SHELTONS IMG 08
			//UI::ImageBox_ptr pItemImageBox8 = std::make_shared<UI::ImageBox>(Rectangle2D(310, 65, 22, 22));
			//pItemImageBox8->SetID(TEXTID_IMGSHELTON08 + i);
			//pItemImageBox8->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo108.bmp"));
			//pItemImageBox8->Hide();
			//pWindowPlayer->AddElement(pItemImageBox8);

			// SHELTONS 09
			UI::Text_ptr pItemSheltoms9 = std::make_shared<UI::Text>(Rectangle2D(330, 65, 240, 14));
			pItemSheltoms9->SetID(TEXTID_SHELTONID09 + i);
			pItemSheltoms9->SetText("");
			pItemSheltoms9->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms9->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms9);

			// SHELTONS IMG 09
			//UI::ImageBox_ptr pItemImageBox9 = std::make_shared<UI::ImageBox>(Rectangle2D(340, 65, 22, 22));
			//pItemImageBox9->SetID(TEXTID_IMGSHELTON09 + i);
			//pItemImageBox9->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo109.bmp"));
			//pItemImageBox9->Hide();
			//pWindowPlayer->AddElement(pItemImageBox9);

			// SHELTONS 10
			UI::Text_ptr pItemSheltoms10 = std::make_shared<UI::Text>(Rectangle2D(370, 65, 240, 14));
			pItemSheltoms10->SetID(TEXTID_SHELTONID10 + i);
			pItemSheltoms10->SetText("");
			pItemSheltoms10->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms10->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms10);

			// SHELTONS IMG 10
			//UI::ImageBox_ptr pItemImageBox10 = std::make_shared<UI::ImageBox>(Rectangle2D(365, 65, 22, 22));
			//pItemImageBox10->SetID(TEXTID_IMGSHELTON10 + i);
			//pItemImageBox10->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo110.bmp"));
			//pItemImageBox10->Hide();
			//pWindowPlayer->AddElement(pItemImageBox10);

			// SHELTONS 11
			UI::Text_ptr pItemSheltoms11 = std::make_shared<UI::Text>(Rectangle2D(395, 65, 240, 14));
			pItemSheltoms11->SetID(TEXTID_SHELTONID11 + i);
			pItemSheltoms11->SetText("");
			pItemSheltoms11->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms11->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms11);

			// SHELTONS IMG 11
			//UI::ImageBox_ptr pItemImageBox11 = std::make_shared<UI::ImageBox>(Rectangle2D(388, 65, 22, 22));
			//pItemImageBox11->SetID(TEXTID_IMGSHELTON11 + i);
			//pItemImageBox11->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo111.bmp"));
			//pItemImageBox11->Hide();
			//pWindowPlayer->AddElement(pItemImageBox11);

			// SHELTONS 12
			UI::Text_ptr pItemSheltoms12 = std::make_shared<UI::Text>(Rectangle2D(425, 65, 240, 14));
			pItemSheltoms12->SetID(TEXTID_SHELTONID12 + i);
			pItemSheltoms12->SetText("");
			pItemSheltoms12->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms12->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms12);

			// SHELTONS IMG 12
			//UI::ImageBox_ptr pItemImageBox12 = std::make_shared<UI::ImageBox>(Rectangle2D(415, 65, 22, 22));
			//pItemImageBox10->SetID(TEXTID_IMGSHELTON12 + i);
			//pItemImageBox12->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo112.bmp"));
			//pItemImageBox12->Hide();
			//pWindowPlayer->AddElement(pItemImageBox12);

			// SHELTONS 13
			UI::Text_ptr pItemSheltoms13 = std::make_shared<UI::Text>(Rectangle2D(455, 65, 240, 14));
			pItemSheltoms13->SetID(TEXTID_SHELTONID13 + i);
			pItemSheltoms13->SetText("");
			pItemSheltoms13->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemSheltoms13->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemSheltoms13);

			// SHELTONS IMG 13
			//UI::ImageBox_ptr pItemImageBox13 = std::make_shared<UI::ImageBox>(Rectangle2D(443, 65, 22, 22));
			//pItemImageBox13->SetID(TEXTID_IMGSHELTON13 + i);
			//pItemImageBox13->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\\Accessory\\itfo113.bmp"));
			//pItemImageBox13->Hide();
			//pWindowPlayer->AddElement(pItemImageBox13);

			//DESC MIX
			UI::Text_ptr pItemDesc = std::make_shared<UI::Text>(Rectangle2D(490, 65, 240, 14));
			pItemDesc->SetID(TEXTID_MixDesc + i);
			pItemDesc->SetText("");
			pItemDesc->SetFont("Arial", 16, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 236, 236, 236));
			pItemDesc->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pItemDesc);

			pListText->AddWindow(pWindowPlayer);
		}


		pWindow1->AddElement(pComboBox);
	}

	pWindow1->Hide();
}

void CMixGuildWindow::ClearList()
{
	auto pListItems = pWindow1->GetElement<UI::List>(LISTID_Items);
	if (pListItems)
	{
		for (size_t i = 0; i < MAXMIXLIST; i++)
		{
			UI::Window_ptr pWindowPlayer = pListItems->GetWindow(WINDOWID_MIXITEM + i);
			if (pWindowPlayer)
			{
				auto MixName = pWindowPlayer->GetElement<UI::Text>(TEXTID_MixName + i);		//mix name
				MixName->SetText("");
				auto MixDesc = pWindowPlayer->GetElement<UI::Text>(TEXTID_MixDesc + i);		//mix desc
				MixDesc->SetText("");

				//clear qnd shelton 
				auto SheltonID01 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID01 + i);
				SheltonID01->SetText("");
				auto SheltonID02 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID02 + i);
				SheltonID02->SetText("");
				auto SheltonID03 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID03 + i);
				SheltonID03->SetText("");
				auto SheltonID04 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID04 + i);
				SheltonID04->SetText("");
				auto SheltonID05 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID05 + i);
				SheltonID05->SetText("");
				auto SheltonID06 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID06 + i);
				SheltonID06->SetText("");
				auto SheltonID07 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID07 + i);
				SheltonID07->SetText("");
				auto SheltonID08 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID08 + i);
				SheltonID08->SetText("");
				auto SheltonID09 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID09 + i);
				SheltonID09->SetText("");
				auto SheltonID10 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID10 + i);
				SheltonID10->SetText("");
				auto SheltonID11 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID11 + i);
				SheltonID11->SetText("");
				auto SheltonID12 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID12 + i);
				SheltonID12->SetText("");
				auto SheltonID13 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID13 + i);
				SheltonID13->SetText("");

				//clear img shelton
				/*auto IMGSheltonID01 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON01 + i);
				IMGSheltonID01->Clear();
				auto IMGSheltonID02 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON02 + i);
				IMGSheltonID02->Clear();
				auto IMGSheltonID03 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON03 + i);
				IMGSheltonID03->Clear();
				auto IMGSheltonID04 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON04 + i);
				IMGSheltonID04->Clear();
				auto IMGSheltonID05 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON05 + i);
				IMGSheltonID05->Clear();
				auto IMGSheltonID06 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON06 + i);
				IMGSheltonID06->Clear();
				auto IMGSheltonID07 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON07 + i);
				IMGSheltonID07->Clear();
				auto IMGSheltonID08 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON08 + i);
				IMGSheltonID08->Clear();
				auto IMGSheltonID09 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON09 + i);
				IMGSheltonID09->Clear();
				auto IMGSheltonID10 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON10 + i);
				IMGSheltonID10->Clear();
				auto IMGSheltonID11 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON11 + i);
				IMGSheltonID11->Clear();
				auto IMGSheltonID12 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON12 + i);
				IMGSheltonID12->Clear();
				auto IMGSheltonID13 = pWindowPlayer->GetElement<UI::ImageBox>(TEXTID_IMGSHELTON13 + i);
				IMGSheltonID13->Clear();*/


			}

		}

		//pListItems->Clear();

	}
}

void CMixGuildWindow::OnResolutionChanged()
{
	pWindow1->SetPosition((RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1));
}

void CMixGuildWindow::OnMixLoad(int MixID)
{
	//ClearList();

	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_MixData);
	auto pMixList = pWindow1->GetElement<UI::List>(LISTID_Items);

	ZeroMemory(sMixList, sizeof(sMixList));

	if (pWindowItem && pMixList && MixID == 1)
	{
		pWindowItem->Clear(); // limpa dados 

		for (int i = 0; i < MAXMIXLIST; i++)
		{

			auto pWindowPlayer = pMixList->GetWindow(WINDOWID_MIXITEM + i);

			if (pWindowPlayer)
			{
				auto MixName = pWindowPlayer->GetElement<UI::Text>(TEXTID_MixName + i);		//mix name			
				auto MixDesc = pWindowPlayer->GetElement<UI::Text>(TEXTID_MixDesc + i);		//mix desc

				//mix shelton qnt
				auto SheltonID01 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID01 + i);
				auto SheltonID02 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID02 + i);
				auto SheltonID03 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID03 + i);
				auto SheltonID04 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID04 + i);
				auto SheltonID05 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID05 + i);
				auto SheltonID06 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID06 + i);
				auto SheltonID07 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID07 + i);
				auto SheltonID08 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID08 + i);
				auto SheltonID09 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID09 + i);
				auto SheltonID10 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID10 + i);
				auto SheltonID11 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID11 + i);
				auto SheltonID12 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID12 + i);
				auto SheltonID13 = pWindowPlayer->GetElement<UI::Text>(TEXTID_SHELTONID13 + i);


				// SHELTONS IMG 01
				UI::ImageBox_ptr pItemImageBox1 = std::make_shared<UI::ImageBox>(Rectangle2D(125, 65, 22, 22));
				pItemImageBox1->SetID(TEXTID_IMGSHELTON01 + i);
				pItemImageBox1->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo101.bmp"));
				pWindowPlayer->AddElement(pItemImageBox1);

				// SHELTONS IMG 02
				UI::ImageBox_ptr pItemImageBox2 = std::make_shared<UI::ImageBox>(Rectangle2D(152, 65, 22, 22));
				pItemImageBox2->SetID(TEXTID_IMGSHELTON02 + i);
				pItemImageBox2->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo102.bmp"));
				pWindowPlayer->AddElement(pItemImageBox2);

				// SHELTONS IMG 03
				UI::ImageBox_ptr pItemImageBox3 = std::make_shared<UI::ImageBox>(Rectangle2D(175, 65, 22, 22));
				pItemImageBox3->SetID(TEXTID_IMGSHELTON03 + i);
				pItemImageBox3->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo103.bmp"));
				pWindowPlayer->AddElement(pItemImageBox3);

				// SHELTONS IMG 04
				UI::ImageBox_ptr pItemImageBox4 = std::make_shared<UI::ImageBox>(Rectangle2D(203, 65, 22, 22));
				pItemImageBox4->SetID(TEXTID_IMGSHELTON04 + i);
				pItemImageBox4->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo104.bmp"));
				pWindowPlayer->AddElement(pItemImageBox4);

				// SHELTONS IMG 05
				UI::ImageBox_ptr pItemImageBox5 = std::make_shared<UI::ImageBox>(Rectangle2D(228, 65, 22, 22));
				pItemImageBox5->SetID(TEXTID_IMGSHELTON05 + i);
				pItemImageBox5->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo105.bmp"));
				pWindowPlayer->AddElement(pItemImageBox5);

				// SHELTONS IMG 06
				UI::ImageBox_ptr pItemImageBox6 = std::make_shared<UI::ImageBox>(Rectangle2D(253, 65, 22, 22));
				pItemImageBox6->SetID(TEXTID_IMGSHELTON06 + i);
				pItemImageBox6->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo106.bmp"));
				pWindowPlayer->AddElement(pItemImageBox6);

				// SHELTONS IMG 07
				UI::ImageBox_ptr pItemImageBox7 = std::make_shared<UI::ImageBox>(Rectangle2D(280, 65, 22, 22));
				pItemImageBox7->SetID(TEXTID_IMGSHELTON07 + i);
				pItemImageBox7->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo107.bmp"));
				pWindowPlayer->AddElement(pItemImageBox7);

				// SHELTONS IMG 08
				UI::ImageBox_ptr pItemImageBox8 = std::make_shared<UI::ImageBox>(Rectangle2D(310, 65, 22, 22));
				pItemImageBox8->SetID(TEXTID_IMGSHELTON08 + i);
				pItemImageBox8->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo108.bmp"));
				pWindowPlayer->AddElement(pItemImageBox8);

				// SHELTONS IMG 09
				UI::ImageBox_ptr pItemImageBox9 = std::make_shared<UI::ImageBox>(Rectangle2D(340, 65, 22, 22));
				pItemImageBox9->SetID(TEXTID_IMGSHELTON09 + i);
				pItemImageBox9->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo109.bmp"));
				pWindowPlayer->AddElement(pItemImageBox9);

				// SHELTONS IMG 10
				UI::ImageBox_ptr pItemImageBox10 = std::make_shared<UI::ImageBox>(Rectangle2D(375, 65, 22, 22));
				pItemImageBox10->SetID(TEXTID_IMGSHELTON10 + i);
				pItemImageBox10->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo110.bmp"));
				pWindowPlayer->AddElement(pItemImageBox10);

				// SHELTONS IMG 11
				UI::ImageBox_ptr pItemImageBox11 = std::make_shared<UI::ImageBox>(Rectangle2D(405, 65, 22, 22));
				pItemImageBox11->SetID(TEXTID_IMGSHELTON11 + i);
				pItemImageBox11->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo111.bmp"));
				pWindowPlayer->AddElement(pItemImageBox11);

				// SHELTONS IMG 12
				UI::ImageBox_ptr pItemImageBox12 = std::make_shared<UI::ImageBox>(Rectangle2D(435, 65, 22, 22));
				pItemImageBox10->SetID(TEXTID_IMGSHELTON12 + i);
				pItemImageBox12->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo112.bmp"));
				pWindowPlayer->AddElement(pItemImageBox12);

				// SHELTONS IMG 13
				UI::ImageBox_ptr pItemImageBox13 = std::make_shared<UI::ImageBox>(Rectangle2D(460, 65, 22, 22));
				pItemImageBox13->SetID(TEXTID_IMGSHELTON13 + i);
				pItemImageBox13->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\Items\\Accessory\\itfo113.bmp"));
				pWindowPlayer->AddElement(pItemImageBox13);

				if (MixName != 0 && MixDesc != 0)
				{
					if (psPacketMixGuild->sMixList[i].iTypeMix == 1)
					{
						MixName->SetText("WEAPON MIX"); // mix tile
					}
					else if (psPacketMixGuild->sMixList[i].iTypeMix == 2)
					{
						MixName->SetText("ARMOS/ROBS MIX"); // mix tile
					}
					else if (psPacketMixGuild->sMixList[i].iTypeMix == 3)
					{
						MixName->SetText("SHIELDS MIX"); // mix tile
					}
					else if (psPacketMixGuild->sMixList[i].iTypeMix == 4)
					{
						MixName->SetText("ORBS MIX"); // mix tile
					}

					//mix qnd shelton
					SheltonID01->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iLucidy));
					SheltonID02->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iSereneo));
					SheltonID03->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iFadeo));
					SheltonID04->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iSparky));
					SheltonID05->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iRaident));
					SheltonID06->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iTransparo));
					SheltonID07->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iMurky));
					SheltonID08->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iDevine));
					SheltonID09->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iCelesto));
					SheltonID10->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iMirage));
					SheltonID11->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iInferna));
					SheltonID12->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iEnigma));
					SheltonID13->SetText(FormatNumber(psPacketMixGuild->sMixList[i].iBellum));

					MixDesc->SetText(psPacketMixGuild->sMixDesc[i].Description);
				}
			}
		}
	}

}

void CMixGuildWindow::OnChangeMixSelect(UIEventArgs e)
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_MixData);

	if (pWindowItem)
	{
		UI::ComboBox_ptr pComboBox = pWindow1->GetElement<UI::ComboBox>(COMBOBOX_DESC);//dados combobox

		if (pComboBox)
		{

			iSelectedMix = pComboBox->GetSelectedID(); // pega id do combobox selecionado

			switch (iSelectedMix)
			{
			default:
				break;
			case 1:
				OnMixLoad(iSelectedMix);
				break;
			case 2:
				OnMixLoad(iSelectedMix);
				break;
			case 3:
				OnMixLoad(iSelectedMix);
				break;
			case 4:
				OnMixLoad(iSelectedMix);
				break;
			case 5:
				OnMixLoad(iSelectedMix);
				break;
			case 6:
				OnMixLoad(iSelectedMix);
				break;
			case 7:
				OnMixLoad(iSelectedMix);
				break;
			case 8:
				OnMixLoad(iSelectedMix);
				break;
			case 9:
				OnMixLoad(iSelectedMix);
				break;
			case 10:
				OnMixLoad(iSelectedMix);
				break;
			}


		}
	}
}

void CMixGuildWindow::Open()
{
	HIDERIGHTNPCBOX;

	pWindow1->Show();

	//MOUSEHANDLER->BlockMouse();
}

void CMixGuildWindow::Close()
{
	if (pWindow1->IsOpen())
	{
		pWindow1->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
	iSelectedMix = 0;
	/*if (pWindow2->IsOpen())
		OnReadMoreButtonClick(UIEventArgs{});*/

	HIDERIGHTNPCBOX;
}

void CMixGuildWindow::Init()
{
	BuildWindow1();
}

void CMixGuildWindow::Render()
{
	pWindow1->Render(0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}
