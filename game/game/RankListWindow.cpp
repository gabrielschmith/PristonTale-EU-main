
#include "stdafx.h"
#include "RankListWindow.h"

// tabs menu
char	szTabsRank[11][32] = 
{
	"topplayers",
	"topfighters",
	"topmechanicians",
	"toparchers",
	"toppikemen",
	"topatalantas",
	"topknights",
	"topmagicians",
	"toppriestesses",
	"topassassins",
	"topshamans",

	//add later when done properly
	/*"topbless",
	"toppvp",
	"sodclans",
	"sodplayers"*/
};

void CRankListWindow::OnRankTopLevelButtonClick(int type)
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	auto pRankingList = pWindow1->GetElement<UI::List>(LISTID_Items);

	const int iColumn1PosX = 10;
	const int iColumn2PosX = 45; //img
	const int iColumn3PosX = 75;
	const int iColumn4PosX = 195;
	const int iColumn5PosX = 270;


	if (pWindowItem && pRankingList)
	{
		//titulo
		auto pRankListTitle = pWindow1->GetElement<UI::Text>(TEXTID_TITLE);

		switch (type)
		{
			case 0: pRankListTitle->SetText("TOP PLAYERS"); break;
			case 1: pRankListTitle->SetText("TOP FIGHTERS"); break;
			case 2: pRankListTitle->SetText("TOP MECHANICIANS"); break;
			case 3: pRankListTitle->SetText("TOP ARCHERS"); break;
			case 4: pRankListTitle->SetText("TOP PIKEMEN"); break;
			case 5: pRankListTitle->SetText("TOP ATALANTAS"); break;
			case 6: pRankListTitle->SetText("TOP KNIGHTS"); break;
			case 7: pRankListTitle->SetText("TOP MAGICIANS"); break;
			case 8: pRankListTitle->SetText("TOP PRIESTESSES"); break;
			case 9: pRankListTitle->SetText("TOP ASSASSINS"); break;
			case 10: pRankListTitle->SetText("TOP SHAMANS"); break;
		}

		//LABEL
		auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
		auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
		auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
		auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);

		pLabel01->SetText("#");
		pLabel02->SetText("Name");
		pLabel03->SetText("Class");
		pLabel04->SetText("Level (XP)");

		pLabel01->SetPosition( iRankingListStartX + iColumn1PosX, pLabel01->GetY());
		pLabel02->SetPosition( iRankingListStartX + iColumn3PosX, pLabel02->GetY());
		pLabel03->SetPosition( iRankingListStartX + iColumn4PosX, pLabel03->GetY());
		pLabel04->SetPosition( iRankingListStartX + iColumn5PosX, pLabel04->GetY());


		auto pListItems = pWindow1->GetElement<UI::List>(LISTID_Items);

		for (size_t i = 0; i < 50; i++)
		{
			UI::Window_ptr pWindowPlayer = std::make_shared<UI::Window>(Rectangle2D(0, 5, 450, 28));
			pWindowPlayer->SetID(WINDOWID_Player + i);
			pListItems->AddWindow(pWindowPlayer);

			//img top 1
			if (i == 0)
			{
				UI::ImageBox_ptr pImgRank1 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_1.png"));
				pWindowPlayer->AddElement(pImgRank1);
			}
			//img top 2
			else if (i == 1)
			{
				UI::ImageBox_ptr pImgRank2 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank2->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_2.png"));
				pWindowPlayer->AddElement(pImgRank2);
			}
			//img top 3
			else if (i == 2)
			{
				UI::ImageBox_ptr pImgRank3 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank3->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_3.png"));
				pWindowPlayer->AddElement(pImgRank3);
			}

			// numeros 4,5,6,7,8,9,10
			if (i >= 3)
			{
				UI::Text_ptr pTextID = std::make_shared<UI::Text>(Rectangle2D(iColumn1PosX, 8, 32, 32));
				pTextID->SetText(std::to_string(i + 1));
				pTextID->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
				pTextID->SetNoClip(TRUE);
				pWindowPlayer->AddElement(pTextID);
			}

			//IMG CLASS
			UI::ImageBox_ptr pImgClass = std::make_shared<UI::ImageBox>(Rectangle2D(iColumn2PosX, 5, 32, 32));
			pWindowPlayer->AddElement(pImgClass);


			//CHAR NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(iColumn3PosX, 7, 32, 32));
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);

			

			//CLASS code
			UI::Text_ptr pClassCodeText = std::make_shared<UI::Text>(Rectangle2D(iColumn4PosX, 7, 32, 32));
			pClassCodeText->SetText("");
			pClassCodeText->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pClassCodeText->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pClassCodeText);


			//LEVEL
			UI::Text_ptr pTextNumber = std::make_shared<UI::Text>(Rectangle2D(iColumn5PosX, 7, 32, 32));
			pTextNumber->SetText("");
			pTextNumber->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextNumber->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextNumber);



			//Characters Ranking
			if (iExperience[i] != 0)
			{
				//NAME
				pTextCharacterNick->SetText(szCharacterName[i]);

				//pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\Arqueira.png"));
				// IMG CLASS
				switch (iClass[i])
				{
				default:
					break;
				case 1: //FS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_fs.png"));
					pClassCodeText->SetText("Fighter");
					break;
				case 2: //MS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ms.png"));
					pClassCodeText->SetText("Mechanician");
					break;
				case 3: //AS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_as.png"));
					pClassCodeText->SetText("Archer");
					break;
				case 4: //PS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ps.png"));
					pClassCodeText->SetText("Pikeman");
					break;
				case 5: //ATA
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ata.png"));
					pClassCodeText->SetText("Atalanta");
					break;
				case 6: //KS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ks.png"));
					pClassCodeText->SetText("Knight");
					break;
				case 7: //MG
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_mg.png"));
					pClassCodeText->SetText("Magician");
					break;
				case 8: //PRS
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_prs.png"));
					pClassCodeText->SetText("Priestess");
					break;
				case 9: //ASSA
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ass.png"));
					pClassCodeText->SetText("Assassin");
					break;
				case 10: //XAMA
					pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_sh.png"));
					pClassCodeText->SetText("Shaman");
					break;
				}

				uint64_t iCurrentLevelStartExp = s_TotalExpPerLevel[iLevel[i] - 1];
				uint64_t iNextLevelStartExp = s_TotalExpPerLevel[iLevel[i]];
				uint64_t iCurrentExp = iExperience[i];

				int percent = static_cast<int>( round ( ( iCurrentExp - iCurrentLevelStartExp ) / ( ( iNextLevelStartExp - iCurrentLevelStartExp ) * 1.0f ) * 100.0f ) );
						
				if ( percent < 0 )
					percent = 0;

				if ( percent > 100 )
					percent = 100;

				pTextNumber->SetTextEx("%d (%d%%)", iLevel[i], percent); // level char					
			}
		}

	}
}


void CRankListWindow::OnSoloRankTopLevelButtonClick(int type)
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	auto pRankingList = pWindow1->GetElement<UI::List>(LISTID_Items);

	const int iColumn1PosX = 10;
	const int iColumn2PosX = 45;
	const int iColumn3PosX = 72;
	const int iColumn4PosX = 185;
	const int iColumn5PosX = 230;
	const int iColumn6PosX = 270;


	if (pWindowItem && pRankingList)
	{
		//titulo
		auto pRankListTitle = pWindow1->GetElement<UI::Text>(TEXTID_TITLE);

		switch (type)
		{
		case 0:  pRankListTitle->SetText("BELLATRA TOP SOLO PLAYERS"); break;
		case 1:  pRankListTitle->SetText("BELLATRA TOP SOLO FIGHTERS"); break;
		case 2:  pRankListTitle->SetText("BELLATRA TOP SOLO MECHANICIANS"); break;
		case 3:  pRankListTitle->SetText("BELLATRA TOP SOLO ARCHERS"); break;
		case 4:  pRankListTitle->SetText("BELLATRA TOP SOLO PIKEMEN"); break;
		case 5:  pRankListTitle->SetText("BELLATRA TOP SOLO ATALANTAS"); break;
		case 6:  pRankListTitle->SetText("BELLATRA TOP SOLO KNIGHTS"); break;
		case 7:  pRankListTitle->SetText("BELLATRA TOP SOLO MAGICIANS"); break;
		case 8:  pRankListTitle->SetText("BELLATRA TOP SOLO PRIESTESSES"); break;
		case 9:  pRankListTitle->SetText("BELLATRA TOP SOLO ASSASSINS"); break;
		case 10: pRankListTitle->SetText("BELLATRA TOP SOLO SHAMANS"); break;
		}

		//LABEL
		auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
		pLabel01->SetText("#");
		auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
		pLabel02->SetText("Name");
		auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
		pLabel03->SetText("Level");
		auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);
		pLabel04->SetText("Kills");
		auto pLabel05 = pWindow1->GetElement<UI::Text>(COLUMN_NAME05);
		pLabel05->SetText("Score");

		pLabel01->SetPosition(iRankingListStartX + iColumn1PosX, pLabel01->GetY());
		pLabel02->SetPosition(iRankingListStartX + iColumn3PosX, pLabel02->GetY());
		pLabel03->SetPosition(iRankingListStartX + iColumn4PosX, pLabel03->GetY());
		pLabel04->SetPosition(iRankingListStartX + iColumn5PosX, pLabel04->GetY());
		pLabel05->SetPosition(iRankingListStartX + iColumn6PosX, pLabel05->GetY());

		auto pListItems = pWindow1->GetElement<UI::List>(LISTID_Items);

		for (size_t i = 0; i < 30; i++)
		{
			if (iSoloScore[i] == 0)
				continue;

			UI::Window_ptr pWindowPlayer = std::make_shared<UI::Window>(Rectangle2D(0, 5, 450, 28));
			pWindowPlayer->SetID(WINDOWID_Player + i);
			pListItems->AddWindow(pWindowPlayer);
			
			//img top 1
			if (i == 0)
			{
				UI::ImageBox_ptr pImgRank1 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_1.png"));
				pWindowPlayer->AddElement(pImgRank1);
			}
				
			//img top 2
			else if (i == 1)
			{
				UI::ImageBox_ptr pImgRank2 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank2->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_2.png"));
				pWindowPlayer->AddElement(pImgRank2);
			}
				
			//img top 3
			else if (i == 2)
			{
				UI::ImageBox_ptr pImgRank3 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank3->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topplayer\\level_3.png"));
				pWindowPlayer->AddElement(pImgRank3);
			}

			// numeros 4,5,6,7,8,9,10
			if (i >= 3)
			{
				UI::Text_ptr pTextID = std::make_shared<UI::Text>(Rectangle2D(iColumn1PosX, 8, 32, 32));
				pTextID->SetText(std::to_string(i + 1));
				pTextID->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
				pTextID->SetNoClip(TRUE);
				pWindowPlayer->AddElement(pTextID);
			}

			//IMG CLASS
			UI::ImageBox_ptr pImgClass = std::make_shared<UI::ImageBox>(Rectangle2D(iColumn2PosX, 5, 32, 32));
			pImgClass->SetID(COLUMN_VALUE02 + i);
			pWindowPlayer->AddElement(pImgClass);


			//CHAR NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(iColumn3PosX, 7, 32, 32));
			pTextCharacterNick->SetID(COLUMN_VALUE03 + i);
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);



			//LEVEL
			UI::Text_ptr pLevelText = std::make_shared<UI::Text>(Rectangle2D(iColumn4PosX, 7, 32, 32));
			pLevelText->SetID(COLUMN_VALUE04 + i);
			pLevelText->SetText("");
			pLevelText->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pLevelText->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pLevelText);


			//KILLS
			UI::Text_ptr pTextKills = std::make_shared<UI::Text>(Rectangle2D(iColumn5PosX, 7, 32, 32));
			pTextKills->SetID(COLUMN_VALUE05 + i);
			pTextKills->SetText("");
			pTextKills->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextKills->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextKills);


			//SCORE
			UI::Text_ptr pTextScore = std::make_shared<UI::Text>(Rectangle2D(iColumn6PosX, 7, 32, 32));
			pTextScore->SetID(COLUMN_VALUE06 + i);
			pTextScore->SetText("");
			pTextScore->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextScore->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextScore);

			//NAME
			pTextCharacterNick->SetText(szSoloCharacterName[i]);

			// IMG CLASS
			switch (iSoloClass[i])
			{
				case 1:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_fs.png"));	break;	//FS
				case 2:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ms.png"));	break;	//MS
				case 3:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_as.png"));	break;	//AS
				case 4:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ps.png"));	break;	//pS
				case 5:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ata.png"));	break;	//ATA
				case 6:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ks.png"));	break;	//KS
				case 7:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_mg.png"));	break;	//MG
				case 8:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_prs.png"));	break;	//PRS
				case 9:  pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_ass.png"));	break;	//ASSA
				case 10: pImgClass->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\class\\class_sh.png"));	break;	//SHAMAN

				default:
					break;
			}


			pLevelText->SetTextEx("%d", iSoloLevel[i]);
			pTextKills->SetTextEx("%d", iSoloKills[i]);
			pTextScore->SetTextEx("%s", FormatNumber(iSoloScore[i])); // level char
			
		}
	}
}


void CRankListWindow::OnRankTopBlessButtonClick()
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	auto pRankingList = pWindow1->GetElement<UI::List>(LISTID_Items);

	if (pWindowItem && pRankingList)
	{
		//titulo
		auto pRankListTitle = pWindow1->GetElement<UI::Text>(TEXTID_TITLE);
		pRankListTitle->SetText("TOP BLESS CASTLE");
		pRankListTitle->SetPosition(300, 55);

		//LABEL
		auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
		pLabel01->SetText("#");
		auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
		pLabel02->SetText("Clan");
		auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
		pLabel03->SetText("IMG");
		auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);
		pLabel04->SetText("Clan Leader");


		for (size_t i = 0; i < 1; i++)
		{
			auto pWindowPlayer = pRankingList->GetWindow(WINDOWID_Player + i);

			//img top 1
			if (i == 0)
			{
				UI::ImageBox_ptr pImgRank1 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\topbless\\bless.png"));
				pWindowPlayer->AddElement(pImgRank1);
			}

			//CLAN NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(80, 5, 32, 32));
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);

			//IMG CLAN
			UI::ImageBox_ptr pImgClass = std::make_shared<UI::ImageBox>(Rectangle2D(180, 5, 32, 32));
			pWindowPlayer->AddElement(pImgClass);

			//CLAN LEADER
			UI::Text_ptr pTextNumber = std::make_shared<UI::Text>(Rectangle2D(260, 5, 32, 32));
			pTextNumber->SetText("");
			pTextNumber->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextNumber->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextNumber);			
			
			if (pWindowPlayer)
			{
				char szClanIDIMG[MAX_PATH];
				char szClanID[32];
				STRINGFORMAT(szClanID, "10_%s", iClanBlessIconID);
				STRINGFORMAT(szClanIDIMG, "save\\clanDATA\\BETA\\%s_16.bmp", iClanBlessIconID);

				if (szNameClan != 0 && szClanLeader != 0)
				{
					pTextCharacterNick->SetText(szNameClan); // clan name
					pImgClass->SetImage(UI::ImageLoader::LoadImage(szClanIDIMG)); // clan img
					pTextNumber->SetText(szClanLeader); // clan leader
				}
				
			}
		}

	}
}

void CRankListWindow::OnRankTopPvpButtonClick()
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	auto pRankingList = pWindow1->GetElement<UI::List>(LISTID_Items);

	if (pWindowItem && pRankingList)
	{
		//titulo
		auto pRankListTitle = pWindow1->GetElement<UI::Text>(TEXTID_TITLE);
		pRankListTitle->SetText("TOP PVP");
		pRankListTitle->SetPosition(320, 55);

		//LABEL
		auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
		pLabel01->SetText("#");
		auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
		pLabel02->SetText("Player");
		auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
		pLabel03->SetText("Kills");
		auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);
		pLabel04->SetText("Deaths");


		for (size_t i = 0; i < 10; i++)
		{
			auto pWindowPlayer = pRankingList->GetWindow(WINDOWID_Player + i);

			//img top 1
			if (i == 0)
			{
				UI::ImageBox_ptr pImgRank1 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 42, 30));
				pImgRank1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\toppvp\\pvp1.png"));
				pWindowPlayer->AddElement(pImgRank1);
			}
			//img top 2
			else if (i == 1)
			{
				UI::ImageBox_ptr pImgRank2 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 42, 30));
				pImgRank2->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\toppvp\\pvp2.png"));
				pWindowPlayer->AddElement(pImgRank2);
			}
			//img top 3
			else if (i == 2)
			{
				UI::ImageBox_ptr pImgRank3 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 42, 30));
				pImgRank3->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\toppvp\\pvp3.png"));
				pWindowPlayer->AddElement(pImgRank3);
			}

			// numeros 4,5,6,7,8,9,10
			if (i >= 3)
			{
				UI::Text_ptr pTextID = std::make_shared<UI::Text>(Rectangle2D(15, 5, 32, 32));
				pTextID->SetText(std::to_string(i + 1));
				pTextID->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
				pTextID->SetNoClip(TRUE);
				pWindowPlayer->AddElement(pTextID);
			}

			// CHAR NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(80, 5, 32, 32));
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);

			// CHAR KILL
			UI::Text_ptr pTextCharkills = std::make_shared<UI::Text>(Rectangle2D(180, 5, 32, 32));
			pTextCharkills->SetText("");
			pTextCharkills->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharkills->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharkills);

			// CHAR MORTES
			UI::Text_ptr pTextCharDead = std::make_shared<UI::Text>(Rectangle2D(260, 5, 32, 32));
			pTextCharDead->SetText("");
			pTextCharDead->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharDead->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharDead);

			if (pWindowPlayer)
			{
				if (szPVPCharName[i] != 0)
				{
					pTextCharacterNick->SetText(szPVPCharName[i]); // char name
					pTextCharkills->SetTextEx("%d", iPVPKills[i]); // char kill
					pTextCharDead->SetTextEx("%d", iPVPDeaths[i]); // char dead
				}

			}
		}

	}
}

void CRankListWindow::OnRankTopSodClanButtonClick()
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	auto pRankingList = pWindow1->GetElement<UI::List>(LISTID_Items);


	if (pWindowItem && pRankingList)
	{
		//titulo
		auto pRankListTitle = pWindow1->GetElement<UI::Text>(TEXTID_TITLE);
		pRankListTitle->SetText("TOP SOD CLAN");
		pRankListTitle->SetPosition(320, 55);

		//LABEL
		auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
		pLabel01->SetText("#");
		auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
		pLabel02->SetText("Clan");
		auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
		pLabel03->SetText("Mem");
		auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);
		pLabel04->SetText("Point");


		for (size_t i = 0; i < 10; i++)
		{
			auto pWindowPlayer = pRankingList->GetWindow(WINDOWID_Player + i);

			//img top 1
			if (i == 0)
			{
				UI::ImageBox_ptr pImgRank1 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\sodclan\\sod1.png"));
				pWindowPlayer->AddElement(pImgRank1);
			}
			//img top 2
			else if (i == 1)
			{
				UI::ImageBox_ptr pImgRank2 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank2->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\sodclan\\sod2.png"));
				pWindowPlayer->AddElement(pImgRank2);
			}
			//img top 3
			else if (i == 2)
			{
				UI::ImageBox_ptr pImgRank3 = std::make_shared<UI::ImageBox>(Rectangle2D(0, 1, 32, 32));
				pImgRank3->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\sodclan\\sod3.png"));
				pWindowPlayer->AddElement(pImgRank3);
			}

			// numeros 4,5,6,7,8,9,10
			if (i >= 3)
			{
				UI::Text_ptr pTextID = std::make_shared<UI::Text>(Rectangle2D(15, 5, 32, 32));
				pTextID->SetText(std::to_string(i + 1));
				pTextID->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
				pTextID->SetNoClip(TRUE);
				pWindowPlayer->AddElement(pTextID);
			}

			//CLAN NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(80, 5, 32, 32));
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);

			//CLAN MEMBER COUNT
			UI::Text_ptr pTextClanMem = std::make_shared<UI::Text>(Rectangle2D(180, 5, 32, 32));
			pTextClanMem->SetText("");
			pTextClanMem->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextClanMem->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextClanMem);

			//CLAN POINTS
			UI::Text_ptr pTextPoints = std::make_shared<UI::Text>(Rectangle2D(260, 5, 32, 32));
			pTextPoints->SetText("");
			pTextPoints->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextPoints->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextPoints);

			if (pWindowPlayer)
			{
				if (szSODClanName[i] != 0)
				{
					pTextCharacterNick->SetText(szSODClanName[i]); // clan nome
					pTextClanMem->SetTextEx("%d", iSODMemCount[i]); // clan member count
					pTextPoints->SetTextEx("%d", iSODPoints[i]); // clan points					
				}

			}
		}

	}
}


CRankListWindow::CRankListWindow()
{
	//DADOS TOP LEVEL
	ZeroMemory(szCharacterName,		sizeof(szCharacterName));
	ZeroMemory(iClass,				sizeof(iClass));
	ZeroMemory(iLevel,				sizeof(iLevel));
	ZeroMemory(iExperience,			sizeof(iExperience));

	ZeroMemory(szSoloCharacterName, sizeof(szSoloCharacterName));
	ZeroMemory(iSoloClass,			sizeof(iSoloClass));
	ZeroMemory(iSoloLevel,			sizeof(iSoloLevel));
	ZeroMemory(iSoloKills,			sizeof(iSoloKills));
	ZeroMemory(iSoloScore,			sizeof(iSoloScore));

	//DADOS TOP BLESS
	ZeroMemory(szNameClan, sizeof(szNameClan));
	ZeroMemory(szClanLeader, sizeof(szClanLeader));
	ZeroMemory(iClanBlessIconID, sizeof(iClanBlessIconID));

	//DADOS TOP PVP
	ZeroMemory(szPVPCharName, sizeof(szPVPCharName));
	ZeroMemory(iPVPKills, sizeof(iPVPKills));
	ZeroMemory(iPVPDeaths, sizeof(iPVPDeaths));

	//DADOS TOP CLAN SOD
	ZeroMemory(szSODClanName, sizeof(szSODClanName));
	ZeroMemory(iSODPoints, sizeof(iSODPoints));
	ZeroMemory(iSODMemCount, sizeof(iSODMemCount));

	//DADOS TOP SOD PLAYER
	ZeroMemory(szSODPlayer, sizeof(szSODPlayer));
	ZeroMemory(iSODPlayerPoints, sizeof(iSODPlayerPoints));

}

CRankListWindow::~CRankListWindow()
{
}

void CRankListWindow::OnMouseMove(class CMouse* pcMouse)
{
	pWindow1->OnMouseMove(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}

BOOL CRankListWindow::OnMouseClick(class CMouse* pcMouse)
{
	if (pWindow1->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;
	return FALSE;
}

BOOL CRankListWindow::OnMouseScroll(class CMouse* pcMouse)
{
	UI::Window_ptr pWindowItem = pWindow1->GetElement<UI::Window>(WINDOWID_RankListData);
	if (pWindow1->IsOpen() && pWindowItem)
	{
		if (pWindowItem->OnMouseScroll(pcMouse, pWindow1->GetX(), pWindow1->GetY(), DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
			return TRUE;
	}

	if (pWindow1->OnMouseScroll(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0))
		return TRUE;
	return FALSE;
}

BOOL CRankListWindow::OnKeyPress(class CKeyboard* pcKeyboard)
{
	if (pWindow1->OnKeyPress(pcKeyboard))
		return TRUE;
	return FALSE;
}

BOOL CRankListWindow::OnKeyChar(class CKeyboard* pcKeyboard)
{
	if (pWindow1->OnKeyChar(pcKeyboard))
		return TRUE;
	return FALSE;
}

void CRankListWindow::HandlePacket(PacketRankingListBlessedCastle* psPacket)
{
	//DADOS TOP BLESS
	CopyMemory(szNameClan, psPacket->szNameClan, sizeof(szNameClan));
	CopyMemory(szClanLeader, psPacket->szClanLeader, sizeof(szClanLeader));
	char szClanID[32];
	STRINGFORMAT(szClanID, "%06d", psPacket->iClanBlessIconID);
	CopyMemory(iClanBlessIconID, szClanID, sizeof(iClanBlessIconID));
}

void CRankListWindow::HandlePacket(PacketRankingListByClass* psPacket)
{

}

void CRankListWindow::HandlePacket(PacketRankingListClanSodList* psPacket)
{

	//DADOS TOP CLAN SOD
	CopyMemory(szSODClanName, psPacket->szClanSodName, sizeof(szSODClanName));
	CopyMemory(iSODPoints, psPacket->iClanSodPoints, sizeof(iSODPoints));
	CopyMemory(iSODMemCount, psPacket->iClanSodMemberCount, sizeof(iSODMemCount));


}

void CRankListWindow::HandlePacket(PacketRankingListIndividualSod* psPacket)
{
	//DADOS TOP SOD PLAYER
	CopyMemory(szSODPlayer, psPacket->szSODPlayer, sizeof(szSODPlayer));
	CopyMemory(iSODPlayerPoints, psPacket->iSODPlayerPoints, sizeof(iSODPlayerPoints));

}

void CRankListWindow::HandlePacket(PacketSoloRankingList* psPacket)
{
	if (!psPacket)
		return;

	bRankingIsForSoloSod = TRUE;

	CopyMemory(szSoloCharacterName, psPacket->szCharacterName, sizeof(szSoloCharacterName));
	CopyMemory(iSoloClass, psPacket->iClass, sizeof(iSoloClass));
	CopyMemory(iSoloLevel, psPacket->iLevel, sizeof(iSoloLevel));
	CopyMemory(iSoloKills, psPacket->iKills, sizeof(iSoloKills));
	CopyMemory(iSoloScore, psPacket->iScore, sizeof(iSoloScore));

	// open window
	if (!pWindow1->IsOpen())
	{
		// Clear Tabs
		ClearParentTabList();
		// Clear ItemsList
		ClearItemsList();

		// add tabs
		for (size_t i = 0; i < 11; i++)
		{
			AddTab(szTabsRank[i], i);
		}

		this->Open(); // open window

		MOUSEHANDLER->BlockMouse();
	}

	OnSoloRankTopLevelButtonClick(psPacket->iRankingType);
}

void CRankListWindow::HandlePacket(PacketRankingListAllPlayer* psPacket)
{
	if (!psPacket)
		return;

	bRankingIsForSoloSod = FALSE;

	//DADOS TOP LEVEL
	CopyMemory(szCharacterName, psPacket->szCharacterName, sizeof(szCharacterName));
	CopyMemory(iClass, psPacket->iClass, sizeof(iClass));
	CopyMemory(iLevel, psPacket->iLevel, sizeof(iLevel));
	CopyMemory(iExperience, psPacket->iExperience, sizeof(iExperience));

	// open window
	if (!pWindow1->IsOpen())
	{
		// Clear Tabs
		ClearParentTabList();
		// Clear ItemsList
		ClearItemsList();

		// add tabs
		for (size_t i = 0; i < 11; i++)
		{
			AddTab(szTabsRank[i], i);
		}

		this->Open(); // open window

		MOUSEHANDLER->BlockMouse();


	}
	
	OnRankTopLevelButtonClick(psPacket->iRankingType);
	
}

void CRankListWindow::HandlePacket(PacketRankingListPVP* psPacket)
{
	//DADOS TOP PVP
	CopyMemory(szPVPCharName, psPacket->szPVPCharName, sizeof(szPVPCharName));
	CopyMemory(iPVPKills, psPacket->iPVPKills, sizeof(iPVPKills));
	CopyMemory(iPVPDeaths, psPacket->iPVPDeaths, sizeof(iPVPDeaths));

}

/*
void CRankListWindow::ResetWindowAndOpen()
{	
	
}*/

void CRankListWindow::OnParentTabButtonClick(char* pcTab, UIEventArgs eArgs)
{
	if (pcTab)
	{
		ClearItemsList();
		ClearLabel();

		int iRankingType = 0;

		//topplayers
		if (szTabsRank[0] == pcTab)
		{
			iRankingType = 0;
		}

		//topfighter
		else if (szTabsRank[1] == pcTab)
		{
			iRankingType = 1;
		}

		//topmechanician
		else if (szTabsRank[2] == pcTab)
		{
			iRankingType = 2;
		}

		//toparcher
		else if (szTabsRank[3] == pcTab)
		{
			iRankingType = 3;
		}

		//toppikeman
		else if (szTabsRank[4] == pcTab)
		{
			iRankingType = 4;
		}

		//topatalanta
		else if (szTabsRank[5] == pcTab)
		{
			iRankingType = 5;
		}

		//topknight
		else if (szTabsRank[6] == pcTab)
		{
			iRankingType = 6;
		}

		//topmagician
		else if (szTabsRank[7] == pcTab)
		{
			iRankingType = 7;
		}

		//toppriestess
		else if (szTabsRank[8] == pcTab)
		{
			iRankingType = 8;
		}

		//topshaman
		else if (szTabsRank[9] == pcTab)
		{
			iRankingType = 9;
		}

		//topbless
		else if (szTabsRank[10] == pcTab)
		{		
			iRankingType = 10;
		}

		//toppvp
		else if (szTabsRank[11] == pcTab)
		{
			iRankingType = 11;
		}

		//sodclans
		else if (szTabsRank[12] == pcTab)
		{
			iRankingType = 12;
		}

		//sodplayers
		else if (szTabsRank[13] == pcTab)
		{
			iRankingType = 13;
		}	

		if (bRankingIsForSoloSod)
		{
			PacketRankingListRequest sPacket;
			sPacket.iLength = sizeof(PacketRankingListRequest);
			sPacket.iHeader = PKTHDR_SoloRankingListRequest;
			sPacket.iRankingType = iRankingType;
			SENDPACKETL(&sPacket);
		}
		else
		{
			PacketRankingListRequest sPacket;
			sPacket.iLength = sizeof(PacketRankingListRequest);
			sPacket.iHeader = PKTHDR_RankingListRequest;
			sPacket.iRankingType = iRankingType;
			SENDPACKETL(&sPacket);
		}

		
	}
}

void CRankListWindow::ClearParentTabList()
{
	auto pList = pWindow1->GetElement<UI::List>(LISTID_Tabs);

	if (pList)
		pList->Clear();
}

void CRankListWindow::ClearItemsList()
{
	auto pListItems = pWindow1->GetElement<UI::List>(LISTID_Items);
	if (pListItems)
	{
		for (size_t i = 0; i < 50; i++)
		{
			UI::Window_ptr pWindowItm = pListItems->GetWindow(WINDOWID_Player + i);
			if (pWindowItm)
				pWindowItm->Clear();
			
		}		
	}
	pListItems->Clear();
	pListItems->SetScrollPercentage(0.0f);
}

void CRankListWindow::ClearLabel()
{
	auto pLabelItems = pWindow1->GetElement<UI::Window>(WINDOWID_LABEL);

	if (pLabelItems)
	{
		pLabelItems->Clear();
	}

	//LABEL
	auto pLabel01 = pWindow1->GetElement<UI::Text>(COLUMN_NAME01);
	pLabel01->SetText("");
	auto pLabel02 = pWindow1->GetElement<UI::Text>(COLUMN_NAME02);
	pLabel02->SetText("");
	auto pLabel03 = pWindow1->GetElement<UI::Text>(COLUMN_NAME03);
	pLabel03->SetText("");
	auto pLabel04 = pWindow1->GetElement<UI::Text>(COLUMN_NAME04);
	pLabel04->SetText("");
	auto pLabel05 = pWindow1->GetElement<UI::Text>(COLUMN_NAME05);
	pLabel05->SetText("");
}



void CRankListWindow::BuildWindow1()
{


	pWindow1 = std::make_shared<UI::Window>(Rectangle2D(20, 20, 560, 450));
	pWindow1->SetImage(UI::ImageLoader::LoadImage("game\\images\\ranklist\\window.png"));

	pWindow1->SetPosition((RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1));

	// Window Item Data
	UI::Window_ptr pWindow2 = std::make_shared<UI::Window>(Rectangle2D(200, 97, 328, 326));
	pWindow2->SetID(WINDOWID_RankListData);
	pWindow1->AddElement(pWindow2);

	// List
	UI::List_ptr pListItems = std::make_shared<UI::List>(Rectangle2D(200, 99, 330, 326));
	pListItems->SetID(LISTID_Items);
	pListItems->SetPaddingSeparator(8);
	pListItems->MakeScrollBar(Rectangle2D(326, 4, 6, 320), "game\\images\\ranklist\\scroll.png", "game\\images\\ranklist\\scroller.png");

	pWindow1->AddElement(pListItems);

	// List Child Tab
	UI::List_ptr pListChildTabs = std::make_shared<UI::List>(Rectangle2D(214, 57, 306, 28));
	pListChildTabs->SetID(LISTID_ChildTabs);
	pListChildTabs->SetHorizontal(TRUE);
	pListChildTabs->SetPaddingSeparator(3);
	pWindow1->AddElement(pListChildTabs);

	//TEXTO TITULO
	UI::Text_ptr pRankListTitle = std::make_shared<UI::Text>(Rectangle2D(190, 55, 350, 14));
	pRankListTitle->SetID(TEXTID_TITLE);
	pRankListTitle->SetText("");
	pRankListTitle->SetHorizontalAlign(EAlign::ALIGN_Center);
	pRankListTitle->SetFont("Arial", 18, 0, FALSE, TRUE, D3DCOLOR_ARGB(150, 170, 150, 120));
	pRankListTitle->SetNoClip(TRUE);
	pWindow1->AddElement(pRankListTitle);

	// Close Button
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>(Rectangle2D(55, 400, 93, 23));
	pButtonClose->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\ranklist\\btnclose.png", "game\\images\\ranklist\\btnclose_.png"));
	pButtonClose->SetEvent(UI::Event::Build(std::bind(&CRankListWindow::OnCloseButtonClick, this, std::placeholders::_1)));
	pWindow1->AddElement(pButtonClose);

	

	{
		//Labels Ranking
		UI::Text_ptr pColumnTitle1 = std::make_shared<UI::Text>(Rectangle2D(iRankingListStartX + 5, 90, 20, 15));
		pColumnTitle1->SetID(COLUMN_NAME01);
		pColumnTitle1->SetText("");
		pColumnTitle1->SetFont("Arial", 14, 0, FALSE, TRUE, -1);
		pColumnTitle1->SetNoClip(TRUE);
		pColumnTitle1->SetHorizontalAlign(ALIGN_Center);
		pWindow1->AddElement(pColumnTitle1);

		UI::Text_ptr pColumnTitle2 = std::make_shared<UI::Text>(Rectangle2D(iRankingListStartX + 80, 90, 0, 0));
		pColumnTitle2->SetID(COLUMN_NAME02);
		pColumnTitle2->SetText("");
		pColumnTitle2->SetFont("Arial", 14, 0, FALSE, TRUE, -1);
		pColumnTitle2->SetNoClip(TRUE);
		pWindow1->AddElement(pColumnTitle2);

		UI::Text_ptr pColumnTitle3 = std::make_shared<UI::Text>(Rectangle2D(iRankingListStartX + 195, 90, 0, 0));
		pColumnTitle3->SetID(COLUMN_NAME03);
		pColumnTitle3->SetText("");
		pColumnTitle3->SetFont("Arial", 14, 0, FALSE, TRUE, -1);
		pColumnTitle3->SetNoClip(TRUE);
		pWindow1->AddElement(pColumnTitle3);

		UI::Text_ptr pColumnTitle4 = std::make_shared<UI::Text>(Rectangle2D(iRankingListStartX + 269, 90, 0, 0));
		pColumnTitle4->SetID(COLUMN_NAME04);
		pColumnTitle4->SetText("");
		pColumnTitle4->SetFont("Arial", 14, 0, FALSE, TRUE, -1);
		pColumnTitle4->SetNoClip(TRUE);
		pWindow1->AddElement(pColumnTitle4);

		UI::Text_ptr pColumnTitle5 = std::make_shared<UI::Text>(Rectangle2D(iRankingListStartX + 270, 90, 0, 0));
		pColumnTitle5->SetID(COLUMN_NAME05);
		pColumnTitle5->SetText("");
		pColumnTitle5->SetFont("Arial", 14, 0, FALSE, TRUE, -1);
		pColumnTitle5->SetNoClip(TRUE);
		pWindow1->AddElement(pColumnTitle5);


		//Ranking Player
		for (size_t i = 0; i < 50; i++)
		{
			
			/*
			//CHAR NAME
			UI::Text_ptr pTextCharacterNick = std::make_shared<UI::Text>(Rectangle2D(80, 5, 32, 32));
			pTextCharacterNick->SetID(TEXTID_CharacterNick + i);
			pTextCharacterNick->SetText("");
			pTextCharacterNick->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextCharacterNick->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextCharacterNick);

			//IMG CLASS
			UI::ImageBox_ptr pImgClass = std::make_shared<UI::ImageBox>(Rectangle2D(180, 5, 32, 32));
			pImgClass->SetID(TEXTID_Class + i);
			pWindowPlayer->AddElement(pImgClass);

			//LEVEL
			UI::Text_ptr pTextNumber = std::make_shared<UI::Text>(Rectangle2D(260, 5, 32, 32));
			pTextNumber->SetID(TEXTID_Level + i);
			pTextNumber->SetText("");
			pTextNumber->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextNumber->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextNumber);

			//SCORE
			UI::Text_ptr pTextNumber = std::make_shared<UI::Text>(Rectangle2D(260, 5, 32, 32));
			pTextNumber->SetID(TEXTID_Level + i);
			pTextNumber->SetText("");
			pTextNumber->SetFont("Arial", 14, 0, FALSE, TRUE, D3DCOLOR_XRGB(192, 144, 97));
			pTextNumber->SetNoClip(TRUE);
			pWindowPlayer->AddElement(pTextNumber);

			*/

			
		}
	}

	// List Tab
	UI::List_ptr pListTabs = std::make_shared<UI::List>(Rectangle2D(23, 55, 161, 340));
	pListTabs->SetID(LISTID_Tabs);
	pListTabs->SetPaddingSeparator(1);
	pWindow1->AddElement(pListTabs);

	// Groups
	{
		// Tabs
		UI::Group_ptr pGroupTab = std::make_shared<UI::Group>();
		pGroupTab->SetID(GROUPID_TabButtons);
		pWindow1->AddElement(pGroupTab);

		// Child Tabs
		UI::Group_ptr pGroupChildTab = std::make_shared<UI::Group>();
		pGroupChildTab->SetID(GROUPID_ChildTabButtons);
		pWindow1->AddElement(pGroupChildTab);
	}

	pWindow1->Hide();
}

void CRankListWindow::OnResolutionChanged()
{
	pWindow1->SetPosition((RESOLUTION_WIDTH >> 1) - (pWindow1->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1));
}

void CRankListWindow::AddTab(char* pcTab, int count)
{
	auto pList = pWindow1->GetElement<UI::List>(LISTID_Tabs);
	auto pGroup = pWindow1->GetElement<UI::Group>(GROUPID_TabButtons);

	if (pList && pGroup)
	{
		char szImageTab1[MAX_PATH], szImageTab2[MAX_PATH];
		STRINGFORMAT(szImageTab1, "game\\images\\ranklist\\tabs\\%s.png", pcTab);
		STRINGFORMAT(szImageTab2, "game\\images\\ranklist\\tabs\\%s_.png", pcTab);

		auto imagePairs = UI::ImageLoader::LoadActiveImage(szImageTab1, szImageTab2);

		// New Window for button
		UI::Window_ptr pWindowTab = std::make_shared<UI::Window>(Rectangle2D(0, 0, 159, 29));
		pWindowTab->SetID(ITEM_TABWindow + count);

		// Button Click
		UI::Button_ptr pButtonTab = std::make_shared<UI::Button>(Rectangle2D(0, 0, 159, 29));
		pButtonTab->SetID(ITEM_TAB + count);
		pButtonTab->SetGroup(pGroup);
		pButtonTab->SetEvent(UI::Event::Build(std::bind(&CRankListWindow::OnParentTabButtonClick, this, pcTab, std::placeholders::_1)));
		pButtonTab->SetActiveImage(imagePairs);
		pButtonTab->SetSelect(TRUE);
		pWindowTab->AddElement(pButtonTab);

		pList->AddWindow(pWindowTab);
	}
}

void CRankListWindow::Open()
{
	HIDERIGHTNPCBOX;

	pWindow1->Show();

	//MOUSEHANDLER->BlockMouse();
}

void CRankListWindow::Close()
{
	if (pWindow1->IsOpen())
	{
		pWindow1->Hide();
		MOUSEHANDLER->UnblockMouse();
	}

	iSelectedRank = 0;

	/*if (pWindow2->IsOpen())
		OnReadMoreButtonClick(UIEventArgs{});*/

	HIDERIGHTNPCBOX;
}

void CRankListWindow::Init()
{
	BuildWindow1();
}

void CRankListWindow::Render()
{
	pWindow1->Render(0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0);
}
