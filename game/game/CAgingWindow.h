#pragma once

static int iaAgingChanceFail[]
{
	0,  //1
	0,  //2
	0,  //3
	0,  //4
	0,  //5
	4,  //6
	8,  //7
	12, //8
	16, //9
	20, //10
	24, //11
	28, //12
	32, //13
	36, //14
	40, //15
	44, //16
	48, //17
	52, //18
	56, //19
	60, //20
};

static int iaAgingChanceFailStone[]
{
	0,  //1
	0,  //2
	0,  //3
	0,  //4
	0,  //5
	2,  //6
	4,  //7
	6, //8
	8, //9
	10, //10
	12, //11
	14, //12
	16, //13
	18, //14
	20, //15
	22, //16
	24, //17
	26, //18
	28, //19
	30, //20
};

class CAgingWindow
{
private:
	BOOL bFreeAging								= FALSE;
	BOOL bHalfPrice								= FALSE;
	BOOL bNoBreak								= FALSE;
	int iCurrentAgingPrice						= 0;

public:
	CAgingWindow();
	virtual ~CAgingWindow();

	UI::Window_ptr								pWindow = NULL;
	int											iAging = -1;

	enum
	{
		TextID_None,
		TextID_Fail,
		TextID_Annouce,
		TextID_Price,
		ImageID_MoneyBox,
	};

	BOOL										IsHalfPrice() { return bHalfPrice; };
	void										UpdateItemPrice(int iAgingCost);

	void										Open(PacketAgingEvent* psPacket);
	void										Render();

	void										HandlePostPacket ( PacketAgingItem * psPacket );

	void										Init();
};

