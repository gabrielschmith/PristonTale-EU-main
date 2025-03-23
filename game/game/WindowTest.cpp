#include "stdafx.h"
#include "WindowTest.h"


CWindowTest::CWindowTest()
{
}


CWindowTest::~CWindowTest()
{
}

void CWindowTest::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 560, 450 ) );
	//pWindow->SetCanMove( Rectangle2D( 0, 0, 560, 450 ), TRUE );
	//pWindow->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Open, this, std::placeholders::_1 ) ) );
	//MOUSEHANDLER->BlockMouse();

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
    int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1);
	int iWindowY = (RESOLUTION_HEIGHT >> 1) - (iWindowHeight >> 1);

	//Usando settings window por agora
	pWindow->SetPosition( iWindowX, iWindowY );
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\eventgirl.png" ) );


	//Intro Text
	UI::TextWindow_ptr pTextWindow = std::make_shared<UI::TextWindow>( Rectangle2D( 24, 48, 520, 54 ) );
	pTextWindow->SetText( "Hi there! Here you can reset your Stats or your Skill Points, choose wisely and make sure you are not overwheight when picking up an item." );
	pTextWindow->SetFont( "Arial", 16, 0, TRUE, FALSE, -1 );
	pWindow->AddElement( pTextWindow );

	//All Stats
	UI::Button_ptr pAllStats = std::make_shared<UI::Button>( Rectangle2D( 80, 124, 121, 29 ) );
	pAllStats->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\allstats.png" ) );
	pAllStats->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\allstats.png", "game\\images\\events\\event_girl\\allstats_.png" ) );
	pAllStats->SetEvent( UI::Event::Build( std::bind( &CWindowTest::OnAllStatsClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pAllStats );

	//All Skills
	UI::Button_ptr pAllSkills = std::make_shared<UI::Button>( Rectangle2D( 352, 124, 121, 29 ) );
	pAllSkills->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\allskills.png" ) );
	pAllSkills->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\allskills.png", "game\\images\\events\\event_girl\\allskills_.png" ) );
	pAllSkills->SetEvent( UI::Event::Build( std::bind( &CWindowTest::OnAllSkillsClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pAllSkills );

	UI::TextWindow_ptr pTextWindow1 = std::make_shared<UI::TextWindow>( Rectangle2D( 24, 178, 520, 18 ) );
	pTextWindow1->SetText( "Choose the especific stats to be reseted, each one has a cost of 200.000 gold" );
	pTextWindow1->SetFont( "Arial", 16, 0, TRUE, FALSE, -1 );
	pWindow->AddElement( pTextWindow1 );

	int iY = 200;
	UI::CheckBox_ptr pCheckBoxStrenght = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxStrenght->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxStrenght->SetText( "Strenght" );
	pCheckBoxStrenght->SetID( CHECKBOXID_Strenght );
	pCheckBoxStrenght->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxStrenght );

    iY += 30;
	UI::CheckBox_ptr pCheckBoxSpirit = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxSpirit->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxSpirit->SetText( "Spirit" );
	pCheckBoxSpirit->SetID( CHECKBOXID_Spirit );
	pCheckBoxSpirit->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxSpirit );

	iY += 30;
	UI::CheckBox_ptr pCheckBoxTalent = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxTalent->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTalent->SetText( "Talent" );
	pCheckBoxTalent->SetID( CHECKBOXID_Talent);
	pCheckBoxTalent->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxTalent );

	iY += 30;
	UI::CheckBox_ptr pCheckBoxAgility = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxAgility->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAgility->SetText( "Agility" );
	pCheckBoxAgility->SetID( CHECKBOXID_Agility );
	pCheckBoxAgility->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxAgility );

	iY += 30;
	UI::CheckBox_ptr pCheckBoxHealth = std::make_shared<UI::CheckBox>( Rectangle2D( 26, iY, 13, 13 ) );
	pCheckBoxHealth->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHealth->SetText( "Health" );
	pCheckBoxHealth->SetID( CHECKBOXID_Health);
	pCheckBoxHealth->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindow->AddElement( pCheckBoxHealth );

	//Value Text
	UI::TextWindow_ptr pTextValue = std::make_shared<UI::TextWindow>( Rectangle2D( 24, 410, 180, 50 ) );
	pTextValue->SetText( "Value: %d Gold" );
	pTextValue->SetFont( "Arial", 18, 0, TRUE, FALSE, -1 );
	pWindow->AddElement( pTextValue );


	//Button Reset Stats
	UI::Button_ptr pResetStats = std::make_shared<UI::Button>( Rectangle2D( 160, 400, 121, 29 ) );
	pResetStats->SetImage( UI::ImageLoader::LoadImage( "game\\images\\events\\event_girl\\reset.png" ) );
	pResetStats->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\event_girl\\reset.png", "game\\images\\events\\event_girl\\reset_.png" ) );
	pResetStats->SetEvent( UI::Event::Build( std::bind( &CWindowTest::OnResetButtonClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pResetStats );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 446, 406, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );


	pWindow->Hide();
}


BOOL CWindowTest::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick(pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::OnAllStatsClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset All your Stats?\nPrice: 1,000,000 Gold" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();
}

void CWindowTest::OnAllSkillsClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset All your Skills?\nPrice: 1,000,000 Gold" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();

}

void CWindowTest::OnResetButtonClick( UIEventArgs e )
{
	// Show Box to accept the reset
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Event Girl" );
	MESSAGEBOX->SetDescription( "Do you want to Reset these stats?\nPrice: %d" );
	//MESSAGEBOX->SetEvent( UI::Event::Build( std::bind( &CWindowTest::Close, this, std::placeholders::_1 ) ) );
	MESSAGEBOX->Show();
}
void CWindowTest::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CWindowTest::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CWindowTest::Open()
{
	pWindow->Show();
	MOUSEHANDLER->BlockMouse();
}

void CWindowTest::Close( UIEventArgs e )
{
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();
}
