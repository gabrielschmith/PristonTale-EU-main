#pragma once

#include <CStable.h>
class CStableWindow
{
public:
	std::string										GetRarityNameByID(StablePetRarity sStablePetRarit);
	void											OnMouseMove(class CMouse* pcMouse);
	BOOL											OnMouseClick(class CMouse* pcMouse);
	BOOL											OnMouseScroll(class CMouse* pcMouse);

	BOOL											OnKeyPress(class CKeyboard* pcKeyboard);
	BOOL											OnKeyChar(class CKeyboard* pcKeyboard);

	void											OnResolutionChanged();

	void											Update(float fTime);
	void											Render();
	void											Init();

	void											AddPet();

	void											SendActivePet(int e);

	void											BuildWindow();
	void											LoadPets(PacketStablePetTab* sPacketStablePetTab);
	void											OnClickPetButton(int iPetID);
	void											RequestPetData();
private:

	UI::Window_ptr									pWindow1 = NULL;

	enum
	{
		// Windows
		WINDOWID_Main = 20,
		WINDOWID_Pet = 21,
		LISTID_Pet = 22
	};
};

