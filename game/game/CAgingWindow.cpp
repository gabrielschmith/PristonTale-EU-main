#include "stdafx.h"
#include "CAgingWindow.h"


CAgingWindow::CAgingWindow()
{
}


CAgingWindow::~CAgingWindow()
{
}


//Note - this occured AFTER the packet is processed in EXE
void CAgingWindow::HandlePostPacket ( PacketAgingItem * psPacket )
{
	switch ( psPacket->Result )
	{
		case 1: //-1
		case 2: //-2
			break;
		case 3: //+1
		case 4: //+2
		{
			SKILLMANAGERHANDLER->StartSkillEffectOld ( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, 5001 ); //5001 = EFFECTID_AGING
			PLAYMINISOUND ( 25 ); //SIN_SOUND_EAT_POTION2
		}
		break;
		default: break;
	}

	CHARACTERGAME->UpdateWeight ();
	ITEMHANDLER->ValidateInventoryItems ();
	SAVE; //save
}


void CAgingWindow::Open(PacketAgingEvent* psPacket)
{
	bFreeAging = psPacket->bFreeAging;
	bHalfPrice = psPacket->bHalfPrice;
	bNoBreak = psPacket->bNoBreak;

	WINDOW_ISOPEN_AGING = TRUE;
	(*(DWORD*)(0x00CDC4E4)) = bFreeAging || bHalfPrice;

	auto pTextAnnounce = pWindow->GetElement<UI::Text>(TextID_Annouce);
	auto pTextPrice = pWindow->GetElement<UI::Text>(TextID_Price);
	auto pMoneyBox = pWindow->GetElement<UI::ImageBox>(ImageID_MoneyBox);

	if (bFreeAging)
	{
		if (bNoBreak)
		{
			pTextAnnounce->SetText("No aging cost and no item breakage event on!");
		}
		else
		{
			pTextAnnounce->SetText("No aging cost event on!");
		}
	}
	else if (bHalfPrice)
	{
		if (bNoBreak)
		{
			pTextAnnounce->SetText("Half aging cost and no item breakage event on!");
		}
		else
		{
			pTextAnnounce->SetText("Half aging cost event on!");
		}
	}
	else if (bNoBreak)
	{
		pTextAnnounce->SetText("No item breakage event on!");
	}



	if (bHalfPrice)
	{
		pTextPrice->Show();
		pMoneyBox->Show();

		UpdateItemPrice(0);
	}
	else
	{
		pTextPrice->Hide();
		pMoneyBox->Hide();
	}
}

void CAgingWindow::UpdateItemPrice(int iAgingCost)
{
	if (iAgingCost != iCurrentAgingPrice)
	{
		auto pTextPrice = pWindow->GetElement<UI::Text>(TextID_Price);
		if (pTextPrice)
		{
			pTextPrice->SetText(FormatNumber(iAgingCost));
		}

		iCurrentAgingPrice = iAgingCost;
	}
}

void CAgingWindow::Render()
{
	pWindow->SetPosition( 18 - ( 256 + 128 - READDWORD( 0x367E84C ) ), 88 );

	if ( WINDOW_ISOPEN_AGING )
	{
		auto pTextFailChance = pWindow->GetElement<UI::Text>(TextID_Fail);

		// Set Chance
		if ( (*(BOOL*)(0x03686CD4)) )  //sAgingItem_AgingItem_0__Flag
		{
			DWORD dwCode = 0;
			if ( (*(BOOL*)(0x036897EC)) )
				dwCode = (*(DWORD*)(0x003689838));

			iAging = (int)(*(short*)(0x03686EE4));
			char szChanceFail[32] = { 0 };
			STRINGFORMAT( szChanceFail, "Fail Chance: %d%%", dwCode == 0x080B0A00 ? iaAgingChanceFailStone[iAging] : iaAgingChanceFail[iAging] );
			pTextFailChance->SetText( szChanceFail );
		}
		else
		{
			pTextFailChance->SetText( "Fail Chance: 0%" );
			iAging = -1;
		}
	}

	pWindow->Render( 0, 0, GRAPHICENGINE->GetWidthBackBuffer(), GRAPHICENGINE->GetHeightBackBuffer(), 0, 0 );
}

void CAgingWindow::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 18, 88, 320, 208 ) );
	UI::Text_ptr pTextChanceFail = std::make_shared<UI::Text>( Rectangle2D( 20, 176, 156, 24 ) );
	pTextChanceFail->SetID( TextID_Fail );
	pTextChanceFail->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
	pTextChanceFail->SetText( "" );
	pWindow->AddElement( pTextChanceFail );

	UI::Text_ptr pTextAnnounce = std::make_shared<UI::Text>(Rectangle2D(18, 2, 200, 24));
	pTextAnnounce->SetID(TextID_Annouce);
	pTextAnnounce->SetFont("Arial", 14, 0, FALSE, FALSE, -1);
	pTextAnnounce->SetText("");
	pWindow->AddElement(pTextAnnounce);

	UI::ImageBox_ptr moneyBoxImg = std::make_shared<UI::ImageBox>(Rectangle2D(181, 176, 100, 23));
	moneyBoxImg->SetID(ImageID_MoneyBox);
	moneyBoxImg->SetImage(UI::ImageLoader::LoadImage("image\\Sinimage\\shopall\\my_money.bmp"));
	moneyBoxImg->Hide();
	pWindow->AddElement(moneyBoxImg);

	UI::Text_ptr pTextPrice = std::make_shared<UI::Text>(Rectangle2D(204, 180, 80, 20));
	pTextPrice->SetID(TextID_Price);
	pTextPrice->SetFont("Arial", 16, 0, FALSE, FALSE, -1);
	pTextPrice->SetText("");
	pTextPrice->Hide();
	pWindow->AddElement(pTextPrice);
}

