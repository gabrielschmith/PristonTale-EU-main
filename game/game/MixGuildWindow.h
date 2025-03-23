/*
*	STR MIX GUILD 2019
*	gabrielcarlos91@hotmail.com
*/
#pragma once
class CMixGuildWindow
{
protected:

	UI::Window_ptr											pWindow1 = NULL;

	enum
	{
		WINDOWID_MixData,
		WINDOWID_MIXITEM,

		LISTID_Main,
		LISTID_Items,

		TEXTID_MixName,
		TEXTID_IMGMix,
		TEXTID_MixDesc,

		TEXTID_SHELTONID01,
		TEXTID_SHELTONID02,
		TEXTID_SHELTONID03,
		TEXTID_SHELTONID04,
		TEXTID_SHELTONID05,
		TEXTID_SHELTONID06,
		TEXTID_SHELTONID07,
		TEXTID_SHELTONID08,
		TEXTID_SHELTONID09,
		TEXTID_SHELTONID10,
		TEXTID_SHELTONID11,
		TEXTID_SHELTONID12,
		TEXTID_SHELTONID13,

		TEXTID_IMGSHELTON01,
		TEXTID_IMGSHELTON02,
		TEXTID_IMGSHELTON03,
		TEXTID_IMGSHELTON04,
		TEXTID_IMGSHELTON05,
		TEXTID_IMGSHELTON06,
		TEXTID_IMGSHELTON07,
		TEXTID_IMGSHELTON08,
		TEXTID_IMGSHELTON09,
		TEXTID_IMGSHELTON10,
		TEXTID_IMGSHELTON11,
		TEXTID_IMGSHELTON12,
		TEXTID_IMGSHELTON13,

		COMBOBOX_DESC,
	};

	int														iSelectedMix = 0;
	struct PacketMixGuild* psPacketMixGuild = NULL;

	ItemListMix												sMixList[MIXLIST_FUNCTION_TOTAL];


private:

	void													BuildWindow1();

	void													OnChangeMixSelect(UIEventArgs e);
	void													OnCloseButtonClick(UIEventArgs e) { this->Close(); };
public:
	CMixGuildWindow();
	virtual ~CMixGuildWindow();

	void													HandlePacket(PacketMixGuild* psPacket);
	void													OnMouseMove(class CMouse* pcMouse);
	BOOL													OnMouseClick(class CMouse* pcMouse);
	BOOL													OnMouseScroll(class CMouse* pcMouse);

	BOOL													OnKeyPress(class CKeyboard* pcKeyboard);
	BOOL													OnKeyChar(class CKeyboard* pcKeyboard);
	int														GetImgSheltom(int IDItem);

	void													ClearList();
	void													OnResolutionChanged();
	void													OnMixLoad(int MixID);
	void													Open();
	void													Close();

	void													Init();

	void													Render();
};

