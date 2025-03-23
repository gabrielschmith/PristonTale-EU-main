#include "stdafx.h"
#include "GameCore.h"
#include "picosha2.h"
GameCore										* pGameCoreHandler = NULL;

tfnDrawBMP fnDrawBMP = (tfnDrawBMP)0x00505B20;
tfnGetPlayTime fnGetPlayTime = (tfnGetPlayTime)0x0045F770;

int & iRadian3D_X								 = *(int*)(0x035BF3F8);
int & iRadian3D_Y								 = *(int*)(0x035BF3FC);
int & iRadian3D_Z								 = *(int*)(0x035BF400);


extern BOOL bDebug;

#define smTEXSTATE_FS_SCROLL2	6
#define smTEXSTATE_FS_SCROLL3	7
#define smTEXSTATE_FS_SCROLL4	8
#define smTEXSTATE_FS_SCROLL5	9
#define smTEXSTATE_FS_SCROLL6	10
#define smTEXSTATE_FS_SCROLL7	11
#define smTEXSTATE_FS_SCROLL8	12
#define smTEXSTATE_FS_SCROLL9	13
#define smTEXSTATE_FS_SCROLL10	14


DropItemView saItemsStage[1024];
BOOL  bLootLabelViewToggleState = FALSE;


BOOL bServerIsSeasonal = 0;


struct AgingColorStructure
{
	short								R;
	short								G;
	short								B;
	short								A;
	short								iUnk;	//TexMixCode
	short								Scroll;	//TexScroll
};


// See AgingBlinkColor
// TexMixCode and TexScroll doesn't seem to do anything. Tested with a
// combination of values ranged from 0 to 10, and beyond
// See https://docs.google.com/spreadsheets/d/1GNWePBhgcB1YX1n9pny5WlQk2y5lnsa2jJ8JCBAWMMA/edit#gid=1702125964
static AgingColorStructure sAgingColor[20-3]
{
	//R		//G		//B			//A		//TexMixCode	//TexScroll
	{ 54,	19,		48	,		0,		-1,				0						},	// 4
	{ 89,	74,		5	,		0,		-1,				0						},	// 5
	{ 6,	54,		3	,		0,		-1,				0						},	// 6
	{ 8,	47,		117	,		0,		 0,				smTEXSTATE_FS_SCROLL5	},	// 7
	{ 91,	4,		125	,		0,		 1,				smTEXSTATE_FS_SCROLL6	},	// 8
	{ 143,	61,		7	,		0,		 2,				smTEXSTATE_FS_SCROLL6	},	// 9
	{ 130,	23,		23	,		0,		 3,				smTEXSTATE_FS_SCROLL8	},	// 10
	{ 0,	219,	216	,		0,		 4,				smTEXSTATE_FS_SCROLL8	},	// 11
	{ 12,	212,	4	,		0,		 4,				smTEXSTATE_FS_SCROLL8	},	// 12
	{ 0,	0,		255	,		0,		 4,				smTEXSTATE_FS_SCROLL9	},	// 13
	{ 234,  0,		255	,		0,		 4,				smTEXSTATE_FS_SCROLL9	},	// 14
	{ 30,	161,	255 ,		0,		 4,				smTEXSTATE_FS_SCROLL9	},	// 15
	{ 255,	149,	0,			0,		 4,				smTEXSTATE_FS_SCROLL9	},	// 16
	{ 255,  0,		0	,		0,	     5,				smTEXSTATE_FS_SCROLL10	},	// 17
	{ 144,	3,		252	,		0,	     5,				smTEXSTATE_FS_SCROLL10	},	// 18
	{ 255,	255,	0	,		0,	     5,				smTEXSTATE_FS_SCROLL10	},	// 19
	{ 255,	255,	255	,		0,		 5,				smTEXSTATE_FS_SCROLL10	},	// 20
};

//Mixing color comes from lpItemInfo->ItemAgingNum[0];
//static short	CraftBlinkColor[6][6] =
//TexMixCode = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
//TexScroll texture = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
//Only mix 6,7,8,9 seem to have effects ?
/*static AgingColorStructure sMixingColor[10]
{
	//R			//G			//B			//A			//TexMixCode	//TexScroll
	{ 0,		 0,			 100,		0,			 5,				4				},	// 4 Classic
	{ 0,		 0,			 100,		0,			 5,				5				},	// 5 Radiant
	{ 0,		 0,			 100,		0,			 5,				6				},	// 6 Trans
	{ 0,		 0,			 100,		0,			 5,				7				},	// 7 Murkey
	{ 0,		 0,			 100,		0,			 5,				8				},	// 8 Devine
	{ 0,		 0,			 100,		0,			 5,				9				},	// 9 Celesto

	//{ 13,		 0,			 11,		0,			 0,				9				},	// 10 Mirage
	//{ 1,		 0,			 1,			0,			 12,			9				},	// 11 Inferna
	//{ 2,		 0,			 2,			0,			 13,			9				},	// 12 Enigma
	//{ 3,		 0,			 2,			0,			 14,			9				},	// 13 BellumOredo

};*/

//TexMixCode = 9 = cool blue
//TexMixCode = 8 = orange
static AgingColorStructure sMixingColor[10]
{
{ 13,	 0,	  5,	0,	  5,	8 },		// 5 Classic
{ 13,	 0,	  6,	0,	  6,	8 },		// 6 Trans
{ 13,	 0,	  7,	0,	  7,	9 },		// 7 Murkey
{ 13,	 0,	  9,	0,	  9,	9 },		// 8 Devine
{ 2,	58, 110,	0,	  8,	9 },		// 9 Celesto
//{ 13,	 0,	 10,	0,	  10,	9 },		// 9 Mirage
//{ 13,	 0,	 11,	0,	  0,	9 },		// 10 Inferna
//{ 1,	 0,	 1,		0,	  12,	9 },		// 11 Enigma
//{ 2,	 0,	 2,		0,	  13,	9 },		// 12 Bellum
//{ 3,	 0,	 2,		0,	  14,	9 },		// 13 Oredo
};

UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 0, 0 );

GameCore::GameCore()
{
	pcGraphicEngineHandler				= new DXGraphicEngine();
	pcSystemHandler						= new CSystem();
	pcSkillManagerHandler				= new SkillManager();
	SocketGame::CreateInstance();
	pcChatGameHandler					= new ChatGame();
	pcItemHandler						= new ItemHandler();
	pcSettingsHandler					= new CSettingsHandler();
	pcItemTimerHandler					= new CItemTimerHandler();
	pcFontHandler						= new DX::Font();
	pcLoginControllerHandler			= new CLoginControllerHandler();
	pcCharacterGameHandler				= new CharacterGame();
	pcAntiCheatHandler					= new CAntiCheat();
	pcKeyboardHandler					= new CKeyboard();
	pcMouseHandler						= new CMouse();
	pcSoundHandler						= new CSound();
	pcPacketHandler						= new RecvPacket();
	pcDamageInfoHandler					= new CDamageInfoController();
	pcAgingWindowHandler				= new CAgingWindow();
	pcWarehouseHandler					= new CWarehouse();
	pcItemWindowHandler					= new CItemWindowHandler();
	pcQuestGameHandler					= new QuestGame();
	pcTimerSkillHandler					= new CTimerSkill();
	pcBellatraGameHandler				= new BellatraGame();
	pcMixHandler						= new MixHandler();
	pcDailyQuestHandler					= new DailyQuest();
	pcMessageBoxHandler					= new CMessageBox();
	pcToolTipHandler					= new CToolTip();
	pcBlessCastleHandler				= new CBlessCastleHandler();
	pcParticleOldHandler				= new CParticleOld();
	pcShamanHandler						= new CShamanHandler();
	pcHUDHandler						= new HUDController();
	pcHalloweenHandler					= new CHalloweenHandler();
	pcManufactureHandler				= new CManufactureHandler();
	pcPerfectItemViewHandler			= new CPerfectItemViewHandler();
	pcCoinHandler						= new CCoinHandler();
	pcCompareItemHandler				= new CCompareItemHandler();
	pcItemInfoBox						= std::make_shared<UI::ItemInfoBox>();
	pcSkillInfoBox						= std::make_shared<UI::SkillInfoBox>();
	pcAccountHandler					= new CAccountHandler();
	pcAnimationHandler					= new CAnimationHandler();
	pcMapGame							= new MapGame();
	pcChristmasHandler					= new CChristmasHandler();
	pcSkillEffectHandler				= new CSkillEffectHandler();
	pcForceOrbHandler					= new CForceOrbHandler();
	pcGrandFuryHandler					= new CGrandFury();
	pcMixGuildHandler					= new CMixGuildHandler();// xxstr mix guild
	pcRankListHandler					= new CRankListHandler();//xxstr rank list
	pcUIControllerHandler				= new CUIControllerGeneral();
	pcDamageHandler						= new CDamageHandler();
	pcTestMapHandler					= new CTestMapHandler();
	pcInputBox							= new InputBox();
	pcCQuestGameHandler					= new CQuestGame();
	pcQuestBookHandler					= new QuestBookHandler();
	pcNeverSinkHandler					= new NeverSinkHandler();
	pcTestWindowHandler					= new CWindowTest();
	pcBellatraRewardHandler				= new CBellatraReward();
	pcTestScreenHandler					= new CTestScreen();
	pcTitleBoxHandler					= new TitleBox();
	pcSkillDataHandler					= new CSkillDataHandler();
	pcTradeHandler						= new CTradeHandler();
	pcEventGirlHandler					= new CEventGirlHandler();
	pcCamera							= new CCamera();
	pcMessageBalloon					= std::make_shared<UI::MessageBalloon>(Rectangle2D(0,0,0,0));
	pcCrystalTowerHandler				= new CCrystalTowerHandler();
	pcQuestMapHandler					= new CQuestMapHandler();
	pcStableHandler						= new StableHandler();

	UnitGame::CreateInstance();

	bCreated = TRUE;
	ZeroMemory( &sServerTime, sizeof( SYSTEMTIME ) );
}

Skill * GameCore::FindSkill(uint32_t p_ID)
{
	Skill * l_Array = (Skill*)0x00973458;

	for (int l_I = 0; l_I < MAX_SKILLS; ++l_I)
	{
		if (l_Array[l_I].iID == p_ID)
			return &l_Array[l_I];
	}

	return nullptr;
}


Skill * GameCore::FindSkill2(uint32_t p_ID)
{
	Skill * l_Array = saSkill;

	for (int l_I = 0; l_I < MAX_SKILLS; ++l_I)
	{
		if (l_Array[l_I].iID == p_ID)
			return &l_Array[l_I];
	}

	return nullptr;
}

SkillBase * GameCore::FindSkillBase2(uint32_t p_ID)
{
	SkillBase * l_Array = saSkillBase;

	for (int l_I = 0; l_I < MAX_SKILLS; ++l_I)
	{
		if (l_Array[l_I].eSkillID == p_ID)
			return &l_Array[l_I];
	}

	return nullptr;
}

void GameCore::Hooks()
{
	// Level Cap
	*(int*)(0x04B06F14) = SERVER_LEVEL_MAX;

	//CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&CAntiCheat::DebugSelf, (new SDebugSelf{ GetCurrentProcessId(), GetCurrentThreadId() }), 0, NULL ); // chaos

	CopyMemory( (void*)0x00973458, &saSkill, sizeof(Skill) * MAX_SKILLS );
	CopyMemory( (void*)0x00961B18, &saSkillBase, sizeof(SkillBase) * MAX_SKILLS );

	// Set sockets to 0
	WRITEBYTE( 0x38FED40, 0 );
	WRITEBYTE( 0x38FEE40, 0 );
	WRITEDWORD( 0x04C38C28, 27242 ); //Char

	// Initialization Game
	CALL( 0x00484B40 );
	CALL( 0x00448D30 );

	DWORD dwOld = 0;
	VirtualProtect( (void*)0x00401000, 0x3AC000, PAGE_EXECUTE_READWRITE, &dwOld );

	const int iRenderVertexTotal	= 200000;
	const int iRenderFaceTotal		= 200000;

	//Vertex
	WRITEDWORD( 0x0047E571, 0x40 * iRenderVertexTotal );
	WRITEDWORD( 0x0047E582, (0x40 * iRenderVertexTotal) / 4 );
	//Face
	WRITEDWORD( 0x0047E58A, 0x1C * iRenderFaceTotal );
	WRITEDWORD( 0x0047E5A0, (0x1C * iRenderFaceTotal) / 4 );

	//CopyMemory ( (PVOID)0x996B58, &iaPriestT2BoltsSeasonal, 10 * sizeof ( DWORD ) );

	//Don't think this code works.
	//todo - investigate how to get it working
	//See - CraftBlinkColor (leaked)

	WRITEDWORD( (DWORD)(0x00458F70 + 1), (DWORD)(&sMixingColor[0]) + 2 );
	WRITEDWORD( (DWORD)(0x00458F9C + 1), (DWORD)(&sMixingColor[_countof( sMixingColor )]) + 2 );
	WRITEDWORD( (DWORD)(0x00458FD0 + 3), (DWORD)(&sMixingColor[0]) + 8 );
	WRITEDWORD( (DWORD)(0x00458FDA + 3), (DWORD)(&sMixingColor[0]) + 10 );

	WRITEDWORD( (DWORD)(0x0045934E + 3), (DWORD)(&sMixingColor[0]) + 0 );
	WRITEDWORD( (DWORD)(0x00459359 + 3), (DWORD)(&sMixingColor[0]) + 2 );
	WRITEDWORD( (DWORD)(0x0045936A + 3), (DWORD)(&sMixingColor[0]) + 4 );
	WRITEDWORD( (DWORD)(0x0045937B + 3), (DWORD)(&sMixingColor[0]) + 6 );

	WRITEDWORD( (DWORD)(0x0045917D + 3), (DWORD)(&sMixingColor[0]) + 0 );
	WRITEDWORD( (DWORD)(0x0045918B + 3), (DWORD)(&sMixingColor[0]) + 2 );
	WRITEDWORD( (DWORD)(0x0045919F + 3), (DWORD)(&sMixingColor[0]) + 4 );
	WRITEDWORD( (DWORD)(0x004591B3 + 3), (DWORD)(&sMixingColor[0]) + 6 );

	// Color Aging
	AgingColorStructure * psAgingColor = (AgingColorStructure*)(0x0082B8B0);
	CopyMemory( psAgingColor, &sAgingColor, sizeof( AgingColorStructure )*_countof( sAgingColor ) );

	//Skill
	{
		WRITEDWORD( (DWORD)(0x0044EBF6), &saSkillData[0].pszSkillName );
		WRITEDWORD( (DWORD)(0x0044EC10), &saSkillData[0].pszSkillName );
		WRITEDWORD( (DWORD)(0x0044EC23), &saSkillData[1].pszSkillName );
		WRITEDWORD( (DWORD)(0x004297C6), &saSkillData[0].iSkillCode );
	}

	//Sound Skill
	{
		WRITEDWORD( (DWORD)(0x00543992), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x0054399B), &saSoundData[0].pszSkillSoundFile );

		WRITEDWORD( (DWORD)(0x00543DA2), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x00543DB3), &saSoundData[0].iID );
		WRITEDWORD( (DWORD)(0x00543DBC), &saSoundData[1].pszSkillSoundFile );

		WRITEDWORD( (DWORD)(0x00544871), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x00544883), &saSoundData[0].iID );
		WRITEDWORD( (DWORD)(0x0054488C), &saSoundData[1].pszSkillSoundFile );
	}

	// Items Stage
	{
		WRITEDWORD( 0x0061EBED, (DWORD)(&saItemsStage[0]) );
		WRITEDWORD( 0x0061EC25, (DWORD)(&saItemsStage[0]) );

		WRITEDWORD( 0x00401946, (DWORD)(&saItemsStage[0]) + 12 );
		WRITEDWORD( 0x0061EBBD, (DWORD)(&saItemsStage[0]) + 12 );
		WRITEDWORD( 0x004019AC, (DWORD)(&saItemsStage[1023]) + 12 );
		WRITEDWORD( 0x0061EBDA, (DWORD)(&saItemsStage[1023]) + 12 );

		WRITEDWORD( 0x0061EC03, (DWORD)(&saItemsStage[0]) + 8 );
		WRITEDWORD( 0x0061EC15, (DWORD)(&saItemsStage[1023]) + 8 );

		WRITEDWORD( 0x004182DD, (DWORD)(&saItemsStage[0]) + 168 );
		WRITEDWORD( 0x00453FBF, (DWORD)(&saItemsStage[0]) + 168 );
		WRITEDWORD( 0x0041833D, (DWORD)(&saItemsStage[1023]) + 168 );
		WRITEDWORD( 0x004540D3, (DWORD)(&saItemsStage[1023]) + 168 );

	}

	//Monster and character effect sound
	{
		WRITEDWORD( 0x0054369B, &saSoundEffectData[0] );
		WRITEDWORD( 0x0054378A, &saSoundEffectData[_countof( saSoundEffectData )-1] );
	}
}

GameCore::~GameCore()
{

	SAFE_DELETE( pcGraphicEngineHandler );
	SAFE_DELETE( pcSystemHandler );
	SAFE_DELETE( pcSkillManagerHandler );
	SAFE_DELETE( pcChatGameHandler );
	SAFE_DELETE( pcItemHandler );
	SAFE_DELETE( pcSettingsHandler );
	SAFE_DELETE( pcItemTimerHandler );
	SAFE_DELETE( pcFontHandler );
	SAFE_DELETE( pcLoginControllerHandler );
	SAFE_DELETE( pcCharacterGameHandler );
	SAFE_DELETE( pcAntiCheatHandler );
	SAFE_DELETE( pcKeyboardHandler );
	SAFE_DELETE( pcMouseHandler );
	SAFE_DELETE( pcSoundHandler );
	SAFE_DELETE( pcPacketHandler );
	SAFE_DELETE( pcDamageInfoHandler );
	SAFE_DELETE( pcAgingWindowHandler );
	SAFE_DELETE( pcWarehouseHandler );
	SAFE_DELETE( pcItemWindowHandler );
	SAFE_DELETE( pcQuestGameHandler );
	SAFE_DELETE( pcTimerSkillHandler );
	SAFE_DELETE( pcBellatraGameHandler );
	SAFE_DELETE( pcMixHandler );
	SAFE_DELETE( pcDailyQuestHandler );
	SAFE_DELETE( pcMessageBoxHandler );
	SAFE_DELETE( pcBlessCastleHandler );
	SAFE_DELETE( pcParticleOldHandler );
	SAFE_DELETE( pcShamanHandler );
	SAFE_DELETE( pcHUDHandler );
	SAFE_DELETE( pcHalloweenHandler );
	SAFE_DELETE( pcManufactureHandler );
	SAFE_DELETE( pcPerfectItemViewHandler );

	SAFE_DELETE( pcCoinHandler );
	SAFE_DELETE( pcCompareItemHandler );

	if (pcMessageBalloon)
	{
		pcMessageBalloon.reset();
		pcMessageBalloon = NULL;
	}

	if (pcItemInfoBox)
	{
		pcItemInfoBox.reset();
		pcItemInfoBox = NULL;
	}

	if (pcSkillInfoBox)
	{
		pcSkillInfoBox.reset();
		pcSkillInfoBox = NULL;
	}

	SAFE_DELETE( pcAccountHandler );
	SAFE_DELETE( pcAnimationHandler );
	SAFE_DELETE( pcMapGame );
	SAFE_DELETE( pcChristmasHandler );
	SAFE_DELETE( pcSkillEffectHandler );
	SAFE_DELETE( pcForceOrbHandler );
	SAFE_DELETE( pcGrandFuryHandler );
	SAFE_DELETE( pcMixGuildHandler );//xxstr mix guild
	SAFE_DELETE( pcRankListHandler );
	SAFE_DELETE( pcUIControllerHandler );;
	SAFE_DELETE( pcDamageHandler );
	SAFE_DELETE( pcTestMapHandler );
	SAFE_DELETE( pcInputBox );
	SAFE_DELETE( pcQuestBookHandler );
	SAFE_DELETE( pcNeverSinkHandler );
	SAFE_DELETE( pcCQuestGameHandler );
	SAFE_DELETE( pcTestWindowHandler );
	SAFE_DELETE( pcBellatraRewardHandler );
	SAFE_DELETE( pcTestScreenHandler );
	SAFE_DELETE( pcTitleBoxHandler );
	SAFE_DELETE( pcSkillDataHandler );
	SAFE_DELETE( pcTradeHandler );
	SAFE_DELETE( pcEventGirlHandler );
	SAFE_DELETE( pcStableHandler );



	SAFE_DELETE( pcCrystalTowerHandler );

	UnitGame::DeleteInstance();
}

GameCore * GameCore::GetInstance()
{
	return GAMECORE;
}

void GameCore::Init()
{
	INIT( pcItemTimerHandler );
	INIT( pcSettingsHandler );
	INIT( pcAgingWindowHandler );
	INIT( pcWarehouseHandler );
	INIT( pcDailyQuestHandler );
	INIT( pcMessageBoxHandler );
	INIT( pcHalloweenHandler );
	INIT( pcManageWindow );
	INIT( pcCoinHandler );
	INIT( pcItemInfoBox );
	INIT( pcSkillInfoBox );
	INIT( pcCompareItemHandler );
	INIT( pcAnimationHandler );
	INIT( pcSkillEffectHandler );
	INIT( pcGrandFuryHandler );
	INIT( pcMixGuildHandler );// xxstr mix guild
	INIT( pcRankListHandler );//xxstr rank list
	INIT( pcUIControllerHandler );
	INIT( pcInputBox );
	INIT( pcDamageInfoHandler );
	INIT( pcCQuestGameHandler );
	INIT( pcQuestBookHandler );
	INIT( pcTestWindowHandler );
	INIT( pcBellatraRewardHandler );
	INIT( pcTitleBoxHandler );
	INIT( pcToolTipHandler );
	INIT( pcTradeHandler );
	INIT( pcEventGirlHandler );
	INIT( pcBlessCastleHandler );
	INIT( pcMessageBalloon );
	INIT( pcQuestMapHandler );
	INIT( pcTestMapHandler );
	INIT( pcStableHandler );

//	DX::cSelectGlow1.Init( GRAPHICENGINE->GetRenderer() );

	bInit = TRUE;

	//Load Textures
	CALL( 0x0048AB40 );

	ANIMATIONHANDLER->LoadDynamicAnimation();

	//Unload Textures
	CALL( 0x0048ABE0 );
	WRITEDWORD( 0x03500CE4, 0 );

	CAntiCheat::UpdateHardwareID();

#ifdef _ANIM

	if ( FILEEXIST( "debug.ini" ) )
	{
		INI::CReader cReader( "debug.ini" );

		std::string strSMBFile, strINXFile, strAnimationFileSMB, strAnimationFileASE;

		std::string strClassFlag = cReader.ReadString( "Animation", "Class" );
		int iClassFlag = 0;

		if ( STRINGCOMPAREI( strClassFlag.c_str(), "Fighter" ) )
			iClassFlag = CLASSFLAG_Fighter;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Mech" ) )
			iClassFlag = CLASSFLAG_Mechanician;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Archer" ) )
			iClassFlag = CLASSFLAG_Archer;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Pikeman" ) )
			iClassFlag = CLASSFLAG_Pikeman;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Assassin" ) )
			iClassFlag = CLASSFLAG_Assassin;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Knight" ) )
			iClassFlag = CLASSFLAG_Knight;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Atalanta" ) )
			iClassFlag = CLASSFLAG_Atalanta;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Priestess" ) )
			iClassFlag = CLASSFLAG_Priestess;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Mage" ) )
			iClassFlag = CLASSFLAG_Magician;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Shaman" ) )
			iClassFlag = CLASSFLAG_Shaman;

		int iAnimationType = cReader.ReadInt( "Animation", "AnimationType" );
		int iSkillCode = cReader.ReadInt( "Animation", "SkillCode" );
		BOOL bLoop = cReader.ReadBool( "Animation", "Loop" );
		BOOL bkPTMode = cReader.ReadBool( "Animation", "KPT" );

		strSMBFile = cReader.ReadString( "Animation", "SMBFile" );
		strINXFile = cReader.ReadString( "Animation", "INXFile" );
		strAnimationFileSMB = cReader.ReadString( "Animation", "AnimationFileSMB" );
		strAnimationFileASE = cReader.ReadString( "Animation", "AnimationFileASE" );

		DeleteFileA( strAnimationFileSMB.c_str() );
		LoadEXEModelPhysique( strAnimationFileASE.c_str() );

		ANIMATIONHANDLER->FixSMBINXSubFileMotion( strSMBFile.c_str(), strINXFile.c_str(), strSMBFile.c_str(), strINXFile.c_str() );
		ANIMATIONHANDLER->InjectAnimation( strAnimationFileSMB.c_str(), strSMBFile.c_str(), strINXFile.c_str(), iAnimationType, 3, bLoop, iClassFlag, iSkillCode );

		ANIMATIONHANDLER->PrintInfoINX( strINXFile.c_str() );

		ExitProcess( 0 );

		return;
	}
#endif
}

void GameCore::Shutdown()
{
	SHUTDOWN( pcSettingsHandler );
	SocketGame::GetInstance()->UnLoad();
}

BOOL GameCore::OnKeyPress(CKeyboard * pcKeyboard)
{
	if ( pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown && pcKeyboard->GetKey() == VK_F9 && GM_MODE )
		bDebug = !bDebug;

	if ( pcSkillInfoBox->OnKeyPress ( pcKeyboard ) )
		return TRUE;
	if ( pcSkillManagerHandler->OnKeyPress ( pcKeyboard ) )
		return TRUE;
	if ( pcInputBox->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcMessageBoxHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcSettingsHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcPerfectItemViewHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcDailyQuestHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcGrandFuryHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcMixGuildHandler->GetWindow()->OnKeyPress( pcKeyboard )) //xxstr mix guild
		return TRUE;
	if ( pcRankListHandler->GetWindow()->OnKeyPress( pcKeyboard )) //xxstr rank list
		return TRUE;
	if ( pcCoinHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if( pcCompareItemHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcQuestBookHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if (pcNeverSinkHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if (pcStableHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL GameCore::OnKeyChar(CKeyboard * pcKeyboard)
{
	if ( pcInputBox->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcSettingsHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcDailyQuestHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcCoinHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcMixGuildHandler->GetWindow()->OnKeyChar( pcKeyboard ))//xxstr mix guild
		return TRUE;
	if ( pcRankListHandler->GetWindow()->OnKeyChar( pcKeyboard ))//xxstr rank list
		return TRUE;
	if ( pcGrandFuryHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcTradeHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void GameCore::OnResolutionChanged()
{
	ITEMTIMERHANDLER->OnResolutionChanged();
	pcSettingsHandler->GetWindow()->OnResolutionChanged();
	pcDailyQuestHandler->GetWindow()->OnResolutionChanged();
	pcGrandFuryHandler->GetWindow()->OnResolutionChanged();
	pcQuestBookHandler->OnResolutionChanged();
	pcNeverSinkHandler->OnResolutionChanged();
	pcMixGuildHandler->GetWindow()->OnResolutionChanged();//xxstr mix guild
	pcRankListHandler->GetWindow()->OnResolutionChanged();//xxstr rank list
	pcCQuestGameHandler->GetWindow()->OnResolutionChanged();
	pcBellatraRewardHandler->OnResolutionChanged();
	pcCoinHandler->GetWindow()->OnResolutionChanged();
	pcEventGirlHandler->OnResolutionChanged();
	pcBlessCastleHandler->OnResolutionChanged();
	pcMessageBoxHandler->OnResolutionChanged();
}

BOOL GameCore::OnMouseClick( class CMouse * pcMouse )
{

#ifndef  DEV_MODE
	//if ( WINDOW_ISOPEN_NPC_SHOP || WINDOW_ISOPEN_PERSONALSHOP_ME || WINDOW_ISOPEN_WAREHOUSE || WINDOW_ISOPEN_AGING || WINDOW_ISOPEN_TRADE || WINDOW_ISOPEN_MIXING || WINDOW_ISOPEN_CRAFTING || WINDOW_ISOPEN_SMELTING )
	//{
	//	if ( ITEMHANDLER->GetMouseItemHover() &&
	//		 ITEMHANDLER->GetMouseItemHover()->bValid &&
	//		 ITEMHANDLER->GetMouseItemHover()->sItem.eCraftType == EItemCraftType::ITEMCRAFTTYPE_QuestWeapon)
	//	{
	//		return TRUE; //disable selling / putting quest items into other window
	//	}
	//}
#endif // ! DEV_MODE

	if ( WINDOW_ISOPEN_PERSONALSHOP_OTHERS )
	{
		//Disallow right click on other player's personal shop
		//due to a bug (can take out other player's mana for free)
		//@todo - remove when new perasonal shop code is out
		if ( pcMouse->GetEvent () == EMouseEvent::ClickDownL &&
			 KEYBOARDHANDLER->IsKeyDown(VK_CONTROL))
		{
			return TRUE;
		}
	}

	if ( WINDOW_ISOPEN_PERSONALSHOP_ME )
	{
		//Disallow right click on own shop unless rank is higher than 1
		// as this bypass gold limit
		//@todo - remove when new perasonal shop code is out
		if ( pcMouse->GetEvent () == EMouseEvent::DoubleClickR ||
			 pcMouse->GetEvent () == EMouseEvent::ClickDownR ||
			 pcMouse->GetEvent () == EMouseEvent::ClickUpR )
		{
			if ( UNITDATA->sCharacterData.iRank <= CHARACTERRANK_Rank1 )
			{
				return TRUE;
			}
		}
	}

	if (CGameInventory::GetInstance()->OnMouseClick(pcMouse))
		return TRUE;

	if ( pcInputBox->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcItemTimerHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if( pcBlessCastleHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcMessageBoxHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcSettingsHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcWarehouseHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcDailyQuestHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcGrandFuryHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if (pcMixGuildHandler->GetWindow()->OnMouseClick(pcMouse))//xxstr mix guild
		return TRUE;

	if (pcRankListHandler->GetWindow()->OnMouseClick(pcMouse))//xxstr rank list
		return TRUE;

	if ( pcItemWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCoinHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( SKILLMANAGERHANDLER->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcUIControllerHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcQuestBookHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcNeverSinkHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCQuestGameHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcTradeHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcTestWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcBellatraRewardHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcEventGirlHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcQuestMapHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcStableHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	return FALSE;
}

BOOL GameCore::OnMouseScroll( class CMouse * pcMouse )
{
	if( pcInputBox->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcCoinHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcQuestBookHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcNeverSinkHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcGrandFuryHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcCQuestGameHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if (pcMixGuildHandler->GetWindow()->OnMouseScroll(pcMouse))//xxstr mix guild
		return TRUE;
	if (pcRankListHandler->GetWindow()->OnMouseScroll(pcMouse))//xxstr rank list
		return TRUE;
	if( pcBellatraRewardHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcSettingsHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcTestWindowHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcEventGirlHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcStableHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;

	return FALSE;
}

void GameCore::OnMouseMove( class CMouse * pcMouse )
{
	pcInputBox->OnMouseMove( pcMouse );
	pcItemTimerHandler->OnMouseMove( pcMouse );
	pcMessageBoxHandler->OnMouseMove( pcMouse );
	pcSettingsHandler->GetWindow()->OnMouseMove( pcMouse );
	pcWarehouseHandler->GetWindow()->OnMouseMove( pcMouse );
	pcQuestBookHandler->OnMouseMove( pcMouse );
	pcNeverSinkHandler->OnMouseMove( pcMouse );
	pcDailyQuestHandler->GetWindow()->OnMouseMove( pcMouse );
	pcCoinHandler->GetWindow()->OnMouseMove( pcMouse );
	pcGrandFuryHandler->GetWindow()->OnMouseMove( pcMouse );
	pcMixGuildHandler->GetWindow()->OnMouseMove(pcMouse);
	pcRankListHandler->GetWindow()->OnMouseMove(pcMouse);
	pcCQuestGameHandler->GetWindow()->OnMouseMove( pcMouse );
	pcBellatraRewardHandler->OnMouseMove( pcMouse );
	pcTestWindowHandler->OnMouseMove( pcMouse );
	pcTradeHandler->OnMouseMove( pcMouse );
	pcEventGirlHandler->OnMouseMove( pcMouse );
	pcBlessCastleHandler->OnMouseMove( pcMouse );
	pcStableHandler->GetWindow()->OnMouseMove(pcMouse);
}

extern DWORD				dwLastTickLogin;
extern DWORD				dwLastTickGame;
extern int					iLoginServerPingID;
extern int					iGameServerPingID;

void GameCore::Update( float fTime )
{
	double fdTime = (double)fTime;
	static double fCount_14MS = 0.0f; static double fFPS70 = (1000.0f / 70.0f);
	static double fCount_100MS = 0.0f; static double f100MS = 100.0f;
	static double fCount_1SECO = 0.0f; static double f1SECO = 1000.0f;
	static double fCount_1MINU = 0.0f; static double f1MINU = 60000.0f;

	static int iTimes = 0;

	fCount_14MS += fdTime;
	fCount_100MS += fdTime;
	fCount_1SECO += fdTime;
	fCount_1MINU += fdTime;

	Frame();

	pcTitleBoxHandler->Update( fTime );
	pcItemInfoBox->Update( fTime );

	//Skills Timers
	while ( fCount_14MS >= fFPS70 )
	{
		if ( Game::GetGameMode() == GAMEMODE_InGame )
		{
			CALL( 0x005224E0 );
		}

		pcParticleOldHandler->Update( fTime );

		fCount_14MS -= fFPS70;

		pcSkillEffectHandler->Update();

		pcItemTimerHandler->Loop();
		pcQuestMapHandler->Update( (float)fFPS70 );
	}


	// 100 ms
	while ( fCount_100MS >= f100MS )
	{
		pcShamanHandler->Update( fTime );
		pcSoundHandler->Update();
		GameCore::pcManageWindow->Tick();
		pcHUDHandler->Update( fTime );
		pcSettingsHandler->GetWindow()->Update( fTime );

		CHARACTERGAME->SyncCombatData();

		pcBlessCastleHandler->Update( fTime );

		fCount_100MS -= f100MS;
	}
	// 1000 ms
	while ( fCount_1SECO >= f1SECO )
	{
		pcDamageInfoHandler->Update(fTime);

		pcGrandFuryHandler->Update();

		pcMixGuildHandler->Update();//xxstr mix guild

		pcRankListHandler->Update();//xxstr rank list

		pcCQuestGameHandler->Update();

		pcTitleBoxHandler->Tick();

		pcItemTimerHandler->Tick();

		pcTimerSkillHandler->Tick1s ();

		pcTestMapHandler->Tick1s ();

		pcManufactureHandler->Update();

		pcCrystalTowerHandler->Update();

		MESSAGEBOX->Update();

		CHARACTERGAME->Tick1s();

		CQUESTGAME->Tick1s();

		iTimes++;

		if ( (iTimes >= 2) && (Game::GetGameMode() == GAMEMODE_InGame) )
		{
			CAntiCheat::UnloadLibrary();

			if ( UNITDATA->iStunTimeLeft > 0 )
				pcSkillManagerHandler->StunEffect( UNITDATA, 5 );
		}

		if ( ( iTimes % 2 ) == 0 )
		{
			CManageWindow::UpdateClanIcons ();
		}

		EXEVertexCache::Update();

		pcMessageBalloon->Update( static_cast<float>( f1SECO ) );

		fCount_1SECO -= f1SECO;
	}


	DXGraphicEngine::GetScreenShot();
}


void GameCore::UpdateFrame()
{
	typedef FLOAT( __cdecl *tfnUpdatePT )();
	tfnUpdatePT fnUpdatePT = (tfnUpdatePT)0x004164D0;

	typedef void( __cdecl *tfnFrame )(float fTime);
	tfnFrame fnFrame = (tfnFrame)0x0041B5E0;

	float fTime = fnUpdatePT();

	fnFrame( fTime );

	GameCore::GetInstance()->Update( fTime );
}

void GameCore::Frame()
{
}

void GameCore::Render3D()
{
}

void GameCore::Render2D()
{
	ITEMTIMERHANDLER->Render();
	pcChristmasHandler->Render();
	pcShamanHandler->RenderParticle();
	pcCrystalTowerHandler->Render();
	pcSettingsHandler->GetView()->Render();
	pcAgingWindowHandler->Render();
	pcWarehouseHandler->GetWindow()->Render();
	pcBellatraGameHandler->SetCrown();
	pcDailyQuestHandler->GetWindow()->Render();
	pcCoinHandler->GetWindow()->Render();
	pcGrandFuryHandler->Render();
	pcMixGuildHandler->Render();//xxstr mix guild
	pcRankListHandler->Render();//xxstr rank list
	pcTradeHandler->Render();
	pcCQuestGameHandler->GetWindow()->Render();
	pcQuestBookHandler->Render();
	pcNeverSinkHandler->Render();
	pcTestWindowHandler->Render();
	pcBellatraRewardHandler->Render();
	pcEventGirlHandler->Render();
	pcMessageBoxHandler->Render();
	pcInputBox->Render();
	pcTitleBoxHandler->Render();
	pcPerfectItemViewHandler->Render();
	pcTestMapHandler->Render ();

	if( (*GameCore::pcClanManageWindow->pbOpen) )
				if( GameCore::pcManageWindow->iPage == MANAGEWINDOWPAGE_ClanMembers )
					GameCore::pcManageWindow->RenderClanMemberStatus();

	if( (*GameCore::pcManageWindow->piHorScroll) == 0x07D0 )
		if( GameCore::pcManageWindow->iPage == MANAGEWINDOWPAGE_Friends )
			if( GameCore::pcManageWindow->iSubPage == MANAGEWINDOWSUBPAGE_Friends )
				GameCore::pcManageWindow->RenderFriendStatus();

	pcHalloweenHandler->Render();
	pcCompareItemHandler->Render();
	pcItemInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pcSkillInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pcQuestMapHandler->Render();
	pcUIControllerHandler->RenderMouseItem();
	pcToolTipHandler->Render();
	pcStableHandler->Render();

	if( bDebug )
		UNITGAME->RenderDebugText();
}

void GameCore::OnEffect()
{
	// Effect Manager (??)
	__asm
	{
		PUSH 0x3C6A0EA1;
		MOV EAX, 0x005CCA70;
		CALL EAX;
		MOV ECX, EAX;
		MOV EAX, 0x005CBE70;
		CALL EAX;
	}
	// Sin 3D
	CALLT( 0x00525CC0, 0x03707170 );

	// Effect Manager Main
	CALLT( 0x00568D80, 0x0389AC98 );
	// Phisycs Manager Main
	CALLT( 0x005A8260, 0x038FEB10 );

	// Particles Main
	__asm
	{
		PUSH 0x3C6A0EA1;
		PUSH 0;
		MOV ECX, 0x039032CC;
		MOV EAX, 0x005989B0;
		CALL EAX;
	}

	// Skills T5 Effect
	SKILLMANAGERHANDLER->OnEffect();

	// ???
	CALL( 0x005A9940 );
	// ???
	CALL( 0x005ADDB0 );
	// ???
	CALL( 0x005AF4B0 );
}

void GameCore::DrawEffect( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects == FALSE )
	{
		//Get Instance
		DWORD dwClass = CALL( 0x005CCA70 );

		//Effect Manager Update View
		CALLT_WITH_ARG6( 0x005CBD80, dwClass, iX, iY, iZ, iAX, iAY, iAZ );
		//Effect Manager Draw
		CALLT_WITH_ARG6( 0x00568E30, 0x0389AC98, iX, iY, iZ, iAX, iAY, iAZ );
		//Particle Draw
		CALLT_WITH_ARG6( 0x00598F40, 0x039032CC, iX, iY, iZ, iAX, iAY, iAZ );

		PARTICLEENGINE->Render3D( NULL, Point3D( iX, iY, iZ ), Point3D( iAX, iAY, iAZ ) );
	}

	//Pet Draw
	CALL_WITH_ARG6( 0x005ACBE0, iX, iY, iZ, iAX, iAY, iAZ );
	//PHPet Draw
	CALL_WITH_ARG6( 0x005AE3C0, iX, iY, iZ, iAX, iAY, iAZ );
}


DWORD GameCore::OnHandlerFunctions( DWORD dwCode, DWORD * pdwOptional, DWORD UnkTime1, DWORD UnkTime2 )
{
	DWORD dwRet = 0;
	switch ( dwCode )
	{
		case 0x1:
			TIMERSKILLHANDLER->OnAddChasingHunt( pdwOptional[0], pdwOptional[1] );
			break;

		case 0x2:
			DrawMessageMiddle( (char*)pdwOptional[0] );
			break;

		case 0x3:
			TIMERSKILLHANDLER->OnCharacterKilled();
			break;

		case 0x4:
			MIXHANDLER->Load();
			DAMAGEINFOHANDLER->Load();
			SHAMANHANDLER->LoadParticles();
			HALLOWEENHANDLER->LoadParticle();
			CHRISTMASHANDLER->LoadParticles();
			SKILLMANAGERHANDLER->LoadParticles();
			UNITGAME->LoadParticles ();
			CGameEffectFollow::LoadParticles();
			break;

		case 0x5:
			SHAMANHANDLER->OnRainMakerParticle( (UnitData*)pdwOptional[0], (int)pdwOptional[1] );
			break;

		case 0x6:
			TIMERSKILLHANDLER->DrawTimerBox();
			break;

		case 0x7:
			HALLOWEENHANDLER->OnSetTimeDay( (DWORD*)pdwOptional[0], (DWORD*)pdwOptional[1] );
			break;

		case 0x8:
			MANUFACTUREHANDLER->SetManufactureAreaCheck( ITEMHANDLER->GetMouseItem() );
			break;

		case 0x9:
			TIMERSKILLHANDLER->KillTimer( pdwOptional[0] );
			break;

		case 0xA:
			if ( ((UnitGame*)pdwOptional[0]) != 0 )
				((UnitGame*)pdwOptional[0])->DrawLifeBar( pdwOptional[1], pdwOptional[2] );
			break;

		case 0xB:
			UNITGAME->ClearFarUnits();
			break;

		case 0xC:
			MAPGAME->SetMessageBoxMap( (int)pdwOptional[0], TRUE );
			break;

		case 0xD:
			GameCore::GetInstance()->OnEffect();
			break;

		case 0xE:
			CHATGAMEHANDLER->ChatBuffSort();
			break;

		case 0xF:
			Game::OnSetRGBATimer();
			break;

		case 0x10:
			ANIMATIONHANDLER->SetFalconMove( (FalconDataPattern*)pdwOptional[0] );
			break;

		case 0x11:
			CGameProtect::OnRegen();
			break;

		case 0x12:
			CGameProtect::OnSetRegen();
			break;

		case 0x13:
			MIXHANDLER->OnHandlerCharacterUpdate();
			break;

		case 0x14:
			Game::CreateLoaderThread();
			break;

		case 0x15:
			EXETexture * pc;
			pc = ((EXETexture*)pdwOptional);
			if ( pc )
				pc->ReadTextures();
			break;

		case 0x16:
			TIMERSKILLHANDLER->SwapSkillTimer( (Skill*)pdwOptional );
			break;

		case 0x17:
			UNITGAME->RenderItemMotionBlur( (UnitData*)pdwOptional[0], (UnitTool*)pdwOptional[1] );
			break;

		case 0x18:
			if ( (UnitData*)pdwOptional[0] )
				CGameEffectFollow::SetEffectUnitSpawn( (UnitData*)pdwOptional[0] );
			break;

		case 0x19:
			if ( (UnitData*)pdwOptional[0] )
				SKILLMANAGERHANDLER->EventHitMonster( (UnitData*)pdwOptional[0] );
			break;

		case 0x1A:
			EffectBaseFollow::MainEffect();
			break;

		case 0x1B:
			CGameInventory::GetInstance()->ChangeInventorySide( pdwOptional[0] );
			break;

		case 0x1C:
			CTITLEBOX->SetText( pdwOptional[0] );
			break;

		case 0x1D:
			SKILLMANAGERHANDLER->OnMouseClickSkillPet();
			break;

		case 0x1F:
			SKILLMANAGERHANDLER->UpdateSkillCanBuy();
			break;

		case 0x20:
			SKILLMANAGERHANDLER->OnClickSkill();
			break;

		case 0x21:
			CQUESTGAME->OnQuestT5OpenProgress();
			break;

		case 0x22:
			CQUESTGAME->OnQuestT5Start();
			break;

		case 0x23:
			SKILLMANAGERHANDLER->OnEndSkillTimer( (Skill*)pdwOptional[0] );
			break;

		case 0x24:
			UNITGAME->HandlePacketUnitStatus( (UnitData*)pdwOptional[0], (Packet*)pdwOptional[1] );
			break;

		case 0x25:
			//This controls unit animation of other players and monsters
			ClientUnitDataUpdate( (UnitData*)pdwOptional[0] );
			break;

		case 0x26:
			UNITGAME->HandlePacketUnitBuff( (UnitData*)pdwOptional[0], (PacketBuffData*)pdwOptional[1] );
			break;

		case 0x27:
			CHATGAMEHANDLER->AddChatBoxText( (char*)pdwOptional[0], (EChatColor)pdwOptional[1] );
			break;

		case 0x28:
			dwRet = CGameInventory::GetInstance()->GetInventoryPersonalShopItemIndex( (ItemData*)pdwOptional[0] );
			break;

		default:
			break;
	}

	return dwRet;
}

void GameCore::SendLoginData()
{
	PacketLoginUser sPacket;
	sPacket.iLength		= sizeof( PacketLoginUser );
	sPacket.iHeader		= PKTHDR_LoginUser;
	sPacket.dwUnk[0]	= sPacket.dwUnk[1] = sPacket.dwUnk[2] = 0;
	STRINGCOPY( sPacket.szUserID, (char*)0x039033E8 );

	std::string l_UserName = sPacket.szUserID;
	std::string l_Password = (char*)0x039032E8;

	std::transform(l_UserName.begin(), l_UserName.end(), l_UserName.begin(), ::toupper);

	std::string l_HashStr = l_UserName + ":" + l_Password;
	std::string l_HashHexResult;

	picosha2::hash256_hex_string(l_HashStr, l_HashHexResult);
	std::transform(l_HashHexResult.begin(), l_HashHexResult.end(), l_HashHexResult.begin(), ::toupper);

	STRINGCOPY(sPacket.szPassword, l_HashHexResult.c_str());

	sPacket.uWidthScreen	= GetSystemMetrics( SM_CXSCREEN );
	sPacket.uHeightScreen	= GetSystemMetrics( SM_CYSCREEN );

	sPacket.iSystemOS		= SYSTEM->GetSystemOS();
	sPacket.iVersion		= GAME_VERSION;

	//PC Name
	sPacket.szPCName[0] = 0;
	DWORD dwSize		= 32;
	GetComputerNameA( sPacket.szPCName, &dwSize );

	// Mac Address
	{
		UUID uuid;

		// Ask OS to create UUID
		UuidCreateSequential( &uuid );

		// Bytes 2 through 7 inclusive
		sprintf_s( sPacket.szMacAddr, 20, "%02X-%02X-%02X-%02X-%02X-%02X",
			uuid.Data4[ 2 ], uuid.Data4[ 3 ], uuid.Data4[ 4 ], uuid.Data4[ 5 ], uuid.Data4[ 6 ], uuid.Data4[ 7 ] );
	}

	//HD Serial
	{
		char szHD[4];

		szHD[0] = 'C';
		szHD[1] = ':';
		szHD[2] = '\\';
		szHD[3] = 0;

		GetVolumeInformationA( szHD, NULL, 0, &sPacket.dwSerialHD, NULL, NULL, NULL, 0 );

		//Video Name
		STRINGCOPY( sPacket.szVideoName, GRAPHICENGINE->GetVideoName().c_str() );
	}

	//Hardware ID
	STRINGCOPY( sPacket.szHardwareID, Game::GetHardwareID() );

	SENDPACKETL( &sPacket );
}


void WINAPI GameCore::GameRun()
{
	typedef BOOL( __cdecl *tfnCheckQuitGame )();
	tfnCheckQuitGame fnCheckQuitGame = (tfnCheckQuitGame)pfnCheckExit;

	GameCore::UpdateFrame();

	CALL( pfnRender );
	GameCore::GetInstance()->Render3D();

	if ( fnCheckQuitGame() )
	{
		WRITEDWORD( 0x0A1754C, 1 );
	}
}

NAKED void GameCore::SetGameScreen( int iScreen )
{
	JMP( pfnScreenChangeGame );
}

void GameCore::HandlePacket( SocketData * sd, PacketPing * psPacket )
{
	/*DWORD dwTime = CALL_WITH_ARG1( 0x0061F670, psPacket->dwTick );

	if ( sd == SOCKETG )
	{
		if ( abs( (long)dwTime - (long)READDWORD( 0x00CF4794 ) ) > 10 )
			WRITEDWORD( 0x00CF4794, dwTime + (1200 * 1000) );
	}*/

	if ( sd == SOCKETL )
	{
		if ( SOCKETL->dwPingTime != psPacket->dwTime)
			return;

		SOCKETL->dwPingTime = 0;
	}
	else
	{
		//Doesn't get called

		if (SOCKETG->dwPingTime != psPacket->dwTime)
			return;

		SOCKETG->dwPingTime = 0;
	}

	//dwTime = TICKCOUNT - (sd == SOCKETL ? dwLastTickLogin : dwLastTickGame);

	sd->iPing = TICKCOUNT - psPacket->dwTime;
}

