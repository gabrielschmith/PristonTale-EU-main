#include "stdafx.h"

namespace DiscordAPI
{
	namespace Functions
	{
        static HANDLE s_ThreadID;
        static bool s_ThreadRunning;

		void Initialize()
		{
            s_ThreadRunning = true;
            s_ThreadID      = CAntiDebuggerHandler::CreateHiddenThread( reinterpret_cast<LPTHREAD_START_ROUTINE>(Functions::PluginThread), NULL, 0 );
		}

		void Shutdown()
		{
            s_ThreadRunning = false;

            TerminateThread(s_ThreadID, 0);
            Discord_Shutdown();
		}

		void PluginThread()
		{
			Discord_Initialize(APPLICATION_ID, 0, 0, 0);

            DiscordRichPresence drp;

            drp = { 0 };
            drp.startTimestamp = time(0);

            while (s_ThreadRunning)
            {
			    std::string details, state, class_img, small_img, small_img_text, large_img, large_img_text;


			    // IMAGE LARG CLASS CHAR
			    class_img = "logopt";	  // Name IMG

			    // IMAGE LARG DISCORD
			    drp.largeImageKey = class_img.c_str();

			    // TEXT IMAGE LARG CLASS
                if (MAP_ID >= 0 && MAP_ID < (sizeof(pszaMapsName) / sizeof(pszaMapsName[0])))
                {
                    char large_text[100]{};
                    snprintf(large_text, sizeof(large_text), "Name: %s | Level: %d\0", UNITDATA->GetName (), UNITDATA->sCharacterData.iLevel);
                    large_img_text = large_text;
                }
                else
                    large_img_text = "Playing PristonEU";

			    // TEXT IMAGE LARG DISCORD
			    drp.largeImageText = large_img_text.c_str();	// image large texto

			    // IMAGE SMALL NATION test for oly class
			    small_img = NationsNames[0].nation_name; // img
			    small_img_text = "AS"; // text

			    drp.smallImageKey = small_img.c_str();			// image small nation img
			    drp.smallImageText = small_img_text.c_str();	// image small texto

			    state = large_img_text.c_str();
			    drp.state = state.c_str();

                if (MAP_ID >= 0 && MAP_ID < (sizeof(pszaMapsName) / sizeof(pszaMapsName[0])))
                {
			        details     = "Map: " + std::string(pszaMapsName[MAP_ID]);
			        drp.details = details.c_str();
                }

			    Discord_UpdatePresence(&drp);

			    Sleep(5000);
            }
		}
	}
}