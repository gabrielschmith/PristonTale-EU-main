#pragma once

#define NEVERSINK_WINDOW_WIDTH 515
#define NEVERSINK_WINDOW_HEIGHT 400

#define BUTTON_WHEEL_WIDTH 42
#define BUTTON_WHEEL_HEIGHT 42

#define QUIT_BUTTON_WIDTH 42
#define QUIT_BUTTON_HEIGHT 42

#define QUIT_BUTTON_X_POSITION 425
#define QUIT_BUTTON_Y_POSITION 70

#define RICARTEN_X_POSITION 305
#define RICARTEN_Y_POSITION 322

#define BLESS_CASTLE_X_POSITION 277
#define BLESS_CASTLE_Y_POSITION 330

#define CASTLE_OF_THE_LOST_X_POSITION 275
#define CASTLE_OF_THE_LOST_Y_POSITION 230

#define LAND_OF_CHAOS_X_POSITION 363
#define LAND_OF_CHAOS_Y_POSITION 344

class NeverSinkHandler
{
public:
	NeverSinkHandler();
	~NeverSinkHandler();

	void BuildWindow();
	bool OnKeyPress(class CKeyboard* pcKeyBoard);
	void Render();

	BOOL OnMouseClick(class CMouse* pcMouse);
	BOOL OnMouseScroll(class CMouse* pcMouse);
	void OnMouseMove(class CMouse* pcMouse);

	void HandlePacket(PacketNeverSinkOpenUI* psPacket);
	void OnResolutionChanged();

private:
	void UpdateWindowPosition();
	BOOL IsWindowOpen();

	void OnLocationClick(UIEventArgs e, const int fieldID);
	void OnQuitButtonClick(UIEventArgs e);

	UI::Window_ptr neverSinkWindow;
};