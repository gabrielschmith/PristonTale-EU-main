#include "stdafx.h"

#include "chat.h"

#include "ChatGame.h"

BOOL bShowFps					= FALSE;
BOOL bDebug						= FALSE;
BOOL bUIDebug					= FALSE;
BOOL bDisableSodDmgReduction	= FALSE;
BOOL bDisableMessageBox			= FALSE;

//for manual camera controls
int iCameraZoomOverride = -1;
int iCameraAngleOverride = -1;
int iCameraCircleOverride = -1;


INT64 lExpEarned = 0;

extern bool						bNoShowDebugDamage;

DWORD							dwLastTickLogin = 0;
DWORD							dwLastTickGame = 0;
int								iLoginServerPingID = 0;
int								iGameServerPingID = 0;

extern float fSZHPBar;

ChatGame::ChatGame()
{
}

ChatGame::~ChatGame()
{
}

void ChatGame::SendChat( BOOL bLoginServer, const char * pszChat )
{
	int iChatLen = lstrlenA( pszChat );

	if( ( iChatLen < MAX_CHATLENGTH ) && ( iChatLen > 0 ) )
	{
		PacketChat sPacket;
		sPacket.iLength = sizeof( Packet ) + iChatLen + 1;
		sPacket.iHeader = PKTHDR_ChatGame;
		StringCchCopyA( sPacket.szChat, _countof( sPacket.szChat ), pszChat );

		if( bLoginServer )
			SENDPACKETL( &sPacket, TRUE );
		else
			SENDPACKETG( &sPacket, TRUE );
	}
}

void ChatGame::RestoreCommand()
{
	char szChatCommandParam[256] = { 0 };

	FILE * fp = NULL;
	fopen_s( &fp, "fp.txt", "r" );
	if ( fp )
	{
		int i = 0;
		while ( TRUE )
		{
			if ( fgets( szChatCommandParam, 256, fp ) == NULL )
			{
				CHATGAMEHANDLER->AddChatBoxText( "> Last Line!" );
				break;
			}

			if ( szChatCommandParam[STRLEN( szChatCommandParam ) - 1] == '\n' )
				szChatCommandParam[STRLEN( szChatCommandParam ) - 1] = 0;

			CHATGAMEHANDLER->SendChat( FALSE, szChatCommandParam );

			Sleep( 300 );

			i++;
		}

		fclose( fp );

		CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, "Done![%d]", i );
	}

	ExitThread( 0 );
}

int ChatGame::AnalyzeChat ( char * pszBuff )
{ //0 = not send, 1 = ok...
	char szChatCommand[256] = { 0 };
	char szChatCommandParam1[256] = { 0 };
	char szChatCommandParam2[256] = { 0 };
	char szChatCommandParam3[256] = { 0 };
	char szChatCommandParam4[256] = { 0 };
	char szChatCommandParam5[256] = { 0 };
	char szChatCommandParam6[256] = { 0 };

	int iRetChat = 1;

	/*if (ChatCommand("/t1", pszBuff))
	{
		int iParticleID = PARTICLE->Load("game\\scripts\\particles\\ancientweapon\\GreedySkill.part");

		int iID = PARTICLE->Start(iParticleID,
			UNITDATA->sPosition.iX,
			UNITDATA->sPosition.iY - (100 << 8) + UNITDATA->iModelHeight,
			UNITDATA->sPosition.iZ);

		//CHATBOX->AddMessage ( FormatString ( "1st Start ID = %d", iID ) );
		PARTICLE->AddScript(iID, iParticleID, UNITDATA, 70.0f, TRUE);
	}*/


	if ( ChatCommand ( "/t2", pszBuff ) )
	{
		PARTICLE->StartEffectParticleSkill ( "GreedyStart", UNITDATA );
	}

	if ( ChatCommand ( "/t3", pszBuff ) )
	{
		PARTICLE->StartEffectParticleSkill ( "GreedySkill", UNITDATA );
	}

	if ( ChatCommand ( "/t4", pszBuff ) )
	{
		PARTICLE->StartEffectParticleSkill ( "GreedySkill2", UNITDATA );
	}

	//Connection Commands
	if ( ChatCommand ( "/ping", pszBuff ) )
	{
		STRINGFORMAT ( szChatCommandParam1, "> Ping to Login Server: %d", SOCKETL->iPing );
		AddChatBoxText ( szChatCommandParam1 );
		STRINGFORMAT ( szChatCommandParam1, "> Ping to Game Server: %d", SOCKETG->iPing );
		AddChatBoxText ( szChatCommandParam1 );
	}

	if ( ChatCommand ( "/togglemessageboxdisplay", pszBuff ) )
	{
		bDisableMessageBox = !bDisableMessageBox;
	}

	//Stability Commands
	if ( ChatCommand ( "/fps", pszBuff ) )
	{
		bShowFps = bShowFps ? FALSE : TRUE;
		AddChatBoxTextEx ( CHATCOLOR_Error, "> FPS %s visible on screen", bShowFps ? "is now" : "is no longer" );
	}
	//Stability Commands
	if ( ChatCommand ( "/GoldReal", pszBuff ) )
	{
		ItemData * pcItemData = ITEMHANDLER->GetMouseItem ();

		if ( pcItemData )
		{
			DX::Color cColor ( RENDERER->GetTextureAverageColor ( (EXETextureHandle *)&( (EXESurfaceOld *)pcItemData->pcInventoryTexture )->sTexture, TRUE ) );
			ChatGame::AddChatBoxTextEx ( CHATCOLOR_Error, "> (%d, %d, %d, %d)", cColor.GetRed (), cColor.GetGreen (), cColor.GetBlue (), cColor.GetAlpha () );
		}
	}

	//Quest Commands
	if ( ChatCommand ( "/CancelQuest", pszBuff ) || ChatCommand ( "/CancelQuests", pszBuff ) || ChatCommand ( "/CancelActiveQuest", pszBuff )
		|| ChatCommand ( "/CancelActiveQuests", pszBuff ) )
	{
		QUESTGAMEHANDLER->CancelQuests ();
	}

	//Common Commands
	else if ( ChatCommand ( "/resetheadold", pszBuff ) || ChatCommand ( "/ResetHeadOld", pszBuff ) )
	{
		if ( CHARACTERGAME->IsStageVillage () )
		{
			if ( ITEMTIMERHANDLER->GetHead () == NULL )
			{
				BOOL bDefault = FALSE;

				if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
				{
					if ( STRINGCOMPAREI ( "true", szChatCommandParam1 ) )
					{
						bDefault = TRUE;
					}
				}

				CHARACTERGAME->ResetHead ( bDefault );

				if ( bDefault )
					AddChatBoxText ( "> Head reseted successful! (force default)" );
				else
					AddChatBoxText ( "> Head reseted successful!" );
			}
			else
			{
				AddChatBoxText ( "> Command failed! Time-based head is active. Right click on it to cancel it first." );
			}
		}
		else
		{
			AddChatBoxText ( "> Command failed! Can only be used in a town." );
		}
	}

	else if ( ChatCommand ( "/resethead", pszBuff ) ||
		ChatCommand ( "/resethaircrown", pszBuff ) ||
		ChatCommand ( "/ResetHead", pszBuff ) )
	{
		if ( CHARACTERGAME->IsStageVillage () )
		{
			CHARACTERGAME->ResetHairCrown ();
		}
		else
		{
			AddChatBoxText ( "> Command failed! Can only be used in a town." );
		}
	}
	else if ( ChatCommand ( "/removehaircrown", pszBuff ) )
	{
		if ( CHARACTERGAME->IsStageVillage () )
		{
			CHARACTERGAME->RemoveHairCrown ();
		}
		else
		{
			AddChatBoxText ( "> Command failed! Can only be used in a town." );
		}
	}

	else if ( ChatCommand ( "yahoo", pszBuff ) || ChatCommand ( "Yes!", pszBuff ) )
	{
		WRITEDWORD ( 0x03A976C8, TICKCOUNT + 60000 );

		if ( ( UNITDATA->GetAnimation () < ANIMATIONTYPE_Attack ) || ( UNITDATA->GetAnimation () == ANIMATIONTYPE_Yahoo ) )
		{
			UNITDATA->Animate ( ANIMATIONTYPE_Taunt );
			UNITDATA->iFrame = UNITDATA->iStartFrame;
			UNITDATA->iMotionLoopCnt = 1;
		}

		iRetChat = 0;
	}

	else if ( ChatCommand ( "/minimap_hideme", pszBuff ) )
	{
		if ( MAP_ID == MAPID_BlessCastle ||
			MAP_ID == MAPID_Bellatra )
		{
			AddChatBoxText ( "> You cannot hide yourself from minimap in this map!" );
			return 0;
		}

		AddChatBoxText ( "> Now hidden from mini map!" );
		UNIT->bInvisibleInMiniMap = TRUE;
	}


	else if ( ChatCommand ( "/minimap_showme", pszBuff ) )
	{
		AddChatBoxText ( "> Now shown in mini map!" );
		UNIT->bInvisibleInMiniMap = FALSE;
	}

	else if ( ChatCommand ( "/record_exp_begin", pszBuff ) || ChatCommand ( "/record_exp_start", pszBuff ) )
	{
		if ( GM_MODE )
		{
			lExpEarned = 0;
			AddChatBoxText ( "> EXP recording started!", EChatColor::CHATCOLOR_White );
		}
	}

	else if ( ChatCommand ( "/record_exp_end", pszBuff ) || ChatCommand ( "/record_exp_stop", pszBuff ) )
	{
		if ( GM_MODE )
		{
			AddChatBoxText ( (char *)FormatString ( "> EXP gained: %s", FormatNumber ( lExpEarned ) ), EChatColor::CHATCOLOR_White );
		}
	}

	else if ( ChatCommand ( "/cam_taz", pszBuff ) )
	{
		if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) &&
			GetParameterString ( pszBuff, 2, szChatCommandParam2 ) &&
			GetParameterString ( pszBuff, 3, szChatCommandParam3 ) )
		{
			iCameraCircleOverride = atoi ( szChatCommandParam1 );
			iCameraAngleOverride = atoi ( szChatCommandParam2 );
			iCameraZoomOverride = atoi ( szChatCommandParam3 );
		}
	}
	else if ( ChatCommand ( "/cam_angle", pszBuff ) ) // Limit to 1000 ok
	{
		if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
		{
			iCameraAngleOverride = atoi ( szChatCommandParam1 );

			if ( iCameraAngleOverride > 1000 )
			{
				CHATBOX->AddMessage ( FormatString ( "Cam> Vert angle allowed range: %d-%d", 0, 1000 ) );
				iCameraAngleOverride = 1000;
			}
			else if ( iCameraAngleOverride < 0 )
			{
				CHATBOX->AddMessage ( FormatString ( "Cam> Vert angle allowed range: %d-%d", 0, 1000 ) );
				iCameraAngleOverride = 0;
			}

			CHATBOX->AddMessage ( FormatString ( "Cam> Vert angle is now: %d. Use /cam_reset to reset.", iCameraAngleOverride ) );
		}
		else
		{
			auto sCameraZoom = (CameraZoom *)0x00824FA4;
			CHATBOX->AddMessage ( FormatString ( "Cam> Vert angle current: %d", iCameraAngleOverride != -1 ? iCameraAngleOverride : sCameraZoom->iVerticalZoom ) );
		}
	}



	else if ( ChatCommand ( "/cam_zoom", pszBuff ) )
	{
		if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
		{
			iCameraZoomOverride = atoi ( szChatCommandParam1 );

			if (iCameraZoomOverride < 100)
			{
				iCameraZoomOverride = 100;
				CHATBOX->AddMessage(FormatString("Cam> Zoom allowed range: %d-%d", 100, 1000));
			}

			else if (iCameraZoomOverride > 1000)
			{
				CHATBOX->AddMessage(FormatString("Cam> Zoom allowed range: %d-%d", 100, 1000));
				iCameraZoomOverride = 1000;
			}

			CHATBOX->AddMessage(FormatString("Cam> Zoom is now: %d. Use /cam_reset to reset.", iCameraZoomOverride));
		}
		else
		{
			auto sCameraZoom = (CameraZoom*)0x00824FA4;


			CHATBOX->AddMessage(FormatString("Cam> Zoom current: %d", iCameraZoomOverride != -1 ? iCameraZoomOverride : sCameraZoom->iHorizontalZoom));
		}
	}
	else if ( ChatCommand ( "/cam_turn", pszBuff ) )
	{
		if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
		{
			iCameraCircleOverride = atoi ( szChatCommandParam1 );

			if (iCameraCircleOverride < 0)
			{
				iCameraCircleOverride = 0;
				CHATBOX->AddMessage(FormatString("Cam> Turn angle allowed range: %d-%d", 0, 4000));
			}

			else if (iCameraCircleOverride > 4000)
			{
				CHATBOX->AddMessage(FormatString("Cam> Turn angle allowed range: %d-%d", 0, 4000));
				iCameraCircleOverride = 4000;
			}

			CHATBOX->AddMessage(FormatString("Cam> Turn angle is now: %d. Use /cam_reset to reset.", iCameraCircleOverride));
		}
		else
		{
			int iCameraCircle = *(int*)0x00A17630;

			CHATBOX->AddMessage(FormatString("Cam> Turn angle current: %d", iCameraCircleOverride != -1 ? iCameraCircleOverride : iCameraCircle));
		}
	}
	else if ( ChatCommand ( "/cam_reset", pszBuff ) )
	{
		iCameraZoomOverride = -1;
		iCameraAngleOverride = -1;
		iCameraCircleOverride = -1;

		CHATBOX->AddMessage("Cam> Camera reset successful!", EChatColor::CHATCOLOR_White);
	}
	else if( ChatCommand( "/disable_debug", pszBuff ) )
	{
		bNoShowDebugDamage = bNoShowDebugDamage == true ? false : true;
		AddChatBoxTextEx( CHATCOLOR_Error, "> Debug Damage is %s", bNoShowDebugDamage == true ? "Hidden" : "Shown" );
	}


	//Game Master Commands
	if ( GM_MODE )
	{
		if ( ChatCommand ( "/HideMe", pszBuff ) )
		{
			UNITDATA->bVisible = UNITDATA->bVisible ? FALSE : TRUE;
        }
        else if (ChatCommand("/suicide", pszBuff))
        {
            UNITDATA->SetCurrentHealth(0);
            UNITDATA->Animate(EAnimationType::ANIMATIONTYPE_Die);
            UNITDATA->bWalking = false;
        }
		else if ( ChatCommand ( "/HideMe", pszBuff ) )
		{
			UNITDATA->bVisible = UNITDATA->bVisible ? FALSE : TRUE;
		}
		else if ( ChatCommand ( "/ClearUnits", pszBuff ) )
		{
			int iClear = 0;
			// Set Pointer
			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				// Enemy? Put pointer in array
				if ( UNITGAME->pcaUnitData[i].bActive && UNITGAME->pcaUnitData[i].psModelAnimation && UNITGAME->pcaUnitData[i].sCharacterData.iType == CHARACTERTYPE_Monster && UNITGAME->pcaUnitData[i].psModelAnimation->iType == ANIMATIONTYPE_Die )
				{
					UnitData * p = &UNITGAME->pcaUnitData[i];
					__asm
					{
						PUSH ECX;
						PUSH EAX;
						MOV ECX, p;
						MOV EAX, 0x0041F3C0;
						CALL EAX;
						POP EAX;
						POP ECX;
					}
					iClear++;
				}
			}
			AddChatBoxTextEx ( CHATCOLOR_Error, "> Total Units Clear: %d", iClear );
		}

		else if ( ChatCommand ( "/new_stats_enable", pszBuff ) )
		{
			UNITGAME->bNewStats = TRUE;
			AddChatBoxTextEx(CHATCOLOR_Error, "> New Stats Enabled");
		}
		else if ( ChatCommand ( "/new_stats_disable", pszBuff ) )
		{
			UNITGAME->bNewStats = FALSE;
			AddChatBoxTextEx(CHATCOLOR_Error, "> New Stats Disabled");
		}

		/*else if ( ChatCommand ( "/st_mod", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
			{
				int iMainStatMod = atoi ( szChatCommandParam1 );

				if ( iMainStatMod >= 10 && iMainStatMod <= 1000 )
				{
					UNITGAME->iMainStatModifier = iMainStatMod;
				}
				else
					CHATBOX->AddMessage ( FormatString ( "Stats> Range allowed: %d-%d", 10, 1000 ) );
			}
		}

		else if ( ChatCommand ( "/st_low", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
			{
				int iNewValue = atoi ( szChatCommandParam1 );

				if ( iNewValue >= 5 && iNewValue <= 100 )
				{
					UNITGAME->iLowestEfficiency = iNewValue;
				}
				else
					CHATBOX->AddMessage ( FormatString ( "Stats> Range allowed: %d-%d", 5, 100 ) );
			}
		}

		else if ( ChatCommand ( "/st_max", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
			{
				int iNewValue = atoi ( szChatCommandParam1 );

				if ( iNewValue >= 5 && iNewValue <= 100 )
				{
					UNITGAME->iMaxEfficiency = iNewValue;
				}
				else
					CHATBOX->AddMessage ( FormatString ( "Stats> Range allowed: %d-%d", 5, 100 ) );
			}
		}

		else if ( ChatCommand ( "/st_max_ratio", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
			{
				int iNewValue = atoi ( szChatCommandParam1 );

				if ( iNewValue >= 1 && iNewValue <= 100 )
				{
					UNITGAME->iStatsToMaxEfficiency = iNewValue;
				}
				else
					CHATBOX->AddMessage ( FormatString ( "Stats> Range allowed: %d-%d", 1, 100 ) );
			}
		}

		else if ( ChatCommand ( "/st_lvl", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) )
			{
				int iNewValue = atoi ( szChatCommandParam1 );

				if ( iNewValue >= 0 && iNewValue <= 120 )
				{
					UNITGAME->iLevelOverride = iNewValue;
				}
				else
					CHATBOX->AddMessage ( FormatString ( "Stats> Range allowed: %d-%d", 0, 120 ) );
			}
		}

		else if ( ChatCommand ( "/st_list", pszBuff ) )
		{
			CHATBOX->AddMessage ( FormatString ( "Stats> st_mod: %d", UNITGAME->iMainStatModifier) );
			CHATBOX->AddMessage ( FormatString ( "Stats> st_low: %d", UNITGAME->iLowestEfficiency) );
			CHATBOX->AddMessage ( FormatString ( "Stats> st_max: %d", UNITGAME->iMaxEfficiency) );
			CHATBOX->AddMessage ( FormatString ( "Stats> st_max_ratio: %d", UNITGAME->iStatsToMaxEfficiency) );
			CHATBOX->AddMessage ( FormatString ( "Stats> st_lvl: %d", UNITGAME->iLevelOverride) );
		}*/

		//example: /wrap_cam_taz 31 8619489 9403198 780 176 700
		else if ( ChatCommand ( "/wrap_cam_taz", pszBuff ) )
		{
			if ( GetParameterString ( pszBuff, 1, szChatCommandParam1 ) &&
				 GetParameterString ( pszBuff, 2, szChatCommandParam2 ) &&
				 GetParameterString ( pszBuff, 3, szChatCommandParam3 ) &&
				 GetParameterString ( pszBuff, 4, szChatCommandParam4 ) &&
				 GetParameterString ( pszBuff, 5, szChatCommandParam5 ) &&
				 GetParameterString ( pszBuff, 6, szChatCommandParam6 ) )
			{
				int iMapID = atoi ( szChatCommandParam1 );
				int iMapPosX = atoi ( szChatCommandParam2 );
				int iMapPosZ = atoi ( szChatCommandParam3 );

				if ( iMapID >= 0 && iMapID <= 60 )
				{
					this->SendChat ( FALSE, FormatString ( "/wrap %d %d %d", iMapID, iMapPosX, iMapPosZ ) );

					iCameraCircleOverride = atoi ( szChatCommandParam4 );
					iCameraAngleOverride = atoi ( szChatCommandParam5 );
					iCameraZoomOverride = atoi ( szChatCommandParam6 );
				}
			}
		}

		else if ( ChatCommand ( "/ClearUnits", pszBuff ) )
		{
			int iClear = 0;
			// Set Pointer
			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				// Enemy? Put pointer in array
				if ( UNITGAME->pcaUnitData[i].bActive && UNITGAME->pcaUnitData[i].psModelAnimation && UNITGAME->pcaUnitData[i].sCharacterData.iType == CHARACTERTYPE_Monster && UNITGAME->pcaUnitData[i].psModelAnimation->iType == ANIMATIONTYPE_Die )
				{
					UnitData * p = &UNITGAME->pcaUnitData[i];
					__asm
					{
						PUSH ECX;
						PUSH EAX;
						MOV ECX, p;
						MOV EAX, 0x0041F3C0;
						CALL EAX;
						POP EAX;
						POP ECX;
					}
					iClear++;
				}
			}
			AddChatBoxTextEx ( CHATCOLOR_Error, "> Total Units Clear: %d", iClear );
		}

		if ( ChatCommand( "/UI", pszBuff ) )
		{
			bUIDebug = !bUIDebug;
		}
		if ( ChatCommand( "/SetFSP", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			AddChatBoxTextEx( CHATCOLOR_Error, "> FSP: %d", atoi( szChatCommandParam1 ) );

			CHARACTERGAME->SetFreeSkillPointForTier5( atoi( szChatCommandParam1 ) );
		}
		if ( ChatCommand( "/SetNoDelay", pszBuff ) )
		{
			for ( int i = 0; i < 20; i++ )
			{
				SKILLSCHARACTER[i].sSkillInfo.iSkillMasteryBase = 0;
				SKILLSCHARACTER[i].sSkillInfo.iSkillMasteryPerLevel = 0;
			}
		}

		if ( ChatCommand( "/InputBox", pszBuff ) )
		{
			INPUTBOX->SetTitle( "Unstack Potions" );
			INPUTBOX->SetDescription( "How many potions would you like to take?" );
			INPUTBOX->SetValue( "0" );
			INPUTBOX->SetNumberType( 0, 100, TRUE );
			INPUTBOX->Show();
		}
		else if ( ChatCommand( "/RP", pszBuff ) || ChatCommand( "/ReloadParticles", pszBuff ) )
		{
			SKILLMANAGERHANDLER->LoadParticles();
			CALL( 0x00574740 );
			AddChatBoxTextEx( CHATCOLOR_Error, "> All Particles reloaded!" );
		}
		else if ( ChatCommand( "/HideAll", pszBuff ) )
		{
			for ( int i = 0; i < 1024; i++ )
			{
				UnitData * p = UNITGAME->pcaUnitData + i;
				if ( p->bActive )
				{
					p->bVisibleEquipment = p->bVisibleEquipment == TRUE ? FALSE : TRUE;
				}
			}
		}
		else if ( ChatCommand( "/getRange", pszBuff ) )
		{
			UnitData * pc = UNITDATA_FOCUSED != NULL ? UNITDATA_FOCUSED : UNITDATA_HOVER;
			if ( pc )
			{
				int iDistanceXZY = 0, iDistanceXZ = 0, iDistanceX = 0, iDistanceZ = 0, iDistanceY = 0;
				pc->sPosition.GetPTDistanceXZY(&UNITDATA->sPosition, iDistanceXZY, iDistanceXZ, iDistanceX, iDistanceZ, iDistanceY);

				ChatGame::AddChatBoxTextEx( CHATCOLOR_White, "> Distance: 2D [%d] 3D [%d] Meters [%d]", iDistanceXZ, iDistanceXZY, (int)sqrt(iDistanceXZY) / 27 );
			}
		}
		else if ( ChatCommand("/getDistances", pszBuff) )
		{
			UnitData* pcUnitData = UNITDATA_FOCUSED != NULL ? UNITDATA_FOCUSED : UNITDATA_HOVER;
			if (pcUnitData)
			{
				int iDistanceXZY = 0, iDistanceXZ = 0, iDistanceX = 0, iDistanceZ = 0, iDistanceY = 0;
				pcUnitData->sPosition.GetPTDistanceXZY(&UNITDATA->sPosition, iDistanceXZY, iDistanceXZ, iDistanceX, iDistanceZ, iDistanceY);

				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> Distances to '%s'", pcUnitData->GetName());
				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> iX*iX    = %d",	   iDistanceX);
				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> iY*iY    = %d",	   iDistanceZ);
				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> iZ*iZ    = %d",	   iDistanceY);
				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> 2D (XY)  = %d",	   iDistanceXZ);
				ChatGame::AddChatBoxTextEx(CHATCOLOR_White, "> 3D (XYZ) = %d ",    iDistanceXZY);
			}
		}

		else if ( ChatCommand ( "/SendCamToCB", pszBuff ) )
		{

			CameraZoom & sCameraZoom = (*(CameraZoom*)0x00824FA4);
			int & iCameraCircle = (*(int*)0x00A17630);

			int iZoom = sCameraZoom.iHorizontalZoom;
			int iAngle = sCameraZoom.iVerticalZoom;
			int iTurn = iCameraCircle;

			Point3D Coordinate = UNITDATA->sPosition;

			SetClipboardText ( (char*)FormatString ( "%d\t%d\t%d\t%d\t%d", Coordinate.iX, Coordinate.iZ, iTurn, iAngle, iZoom ) );
		}

		else if ( ChatCommand ( "/SendXYZToCB", pszBuff ) )
		{
			Point3D Coordinate = UNITDATA->sPosition;

			SetClipboardText ( (char*)FormatString ( "%d\t%d\t%d", Coordinate.iX,Coordinate.iY, Coordinate.iZ ) );
		}

		else if ( ChatCommand ( "/RestartMusic", pszBuff ) )
		{
			SOUNDHANDLER->ChangeMusic ( 0 );  //No matter what number I set, it just seems to restart the currently playing song.
		}

		else if ( ChatCommand ( "/StreamModeOn", pszBuff ) )
		{
			this->SendChat ( FALSE, "/hide" );
			UNITDATA->bVisible = FALSE ;  //HideMe
			bDisableMessageBox = TRUE; //Disable All Messages

			AddChatBoxText ( "> Stream Mode Enabled" );
		}

		else if ( ChatCommand ( "/StreamModeOff", pszBuff ) )
		{
			this->SendChat ( FALSE, "/show" );
			UNITDATA->bVisible = TRUE ;  //HideMe
			bDisableMessageBox = FALSE; //Disable All Messages

			AddChatBoxText ( "> Stream Mode Disabled" );
		}

		else if ( ChatCommand( "/WCC", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", szChatCommandParam1 );
		}
		else if ( ChatCommand( "/ST", pszBuff ) || ChatCommand( "/SetTime", pszBuff )  )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			WRITEDWORD( 0x00CF479C, (int)atoi(szChatCommandParam1) );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Time setted to %d!", atoi(szChatCommandParam1) );
		}
		else if ( ChatCommand( "/SetDarkLevel", pszBuff )  )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			WRITEDWORD( 0x00CF47A0, (int)atoi(szChatCommandParam1) );

			if( strlen(szChatCommandParam2) > 0 )
				WRITEDWORD( 0x00CF4774, (int)atoi(szChatCommandParam2) );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Dark setted to %d %d!", *(DWORD*)0x00CF47A0, *(DWORD*)0x00CF4774 );
		}

		else if( ChatCommand( "/color", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			GetParameterString( pszBuff, 3, szChatCommandParam3 );

			int iR = (int)atoi( szChatCommandParam1 );
			int iG = (int)atoi( szChatCommandParam2 );
			int iB = (int)atoi( szChatCommandParam3 );

			WRITEDWORD( 0x00CF4784, iR );
			WRITEDWORD( 0x00CF4788, iG );
			WRITEDWORD( 0x00CF478C, iB );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Enviroment Color %d %d %d!", iR, iG, iB );
		}
		else if ( ChatCommand( "/set_hp", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_hp %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );
		}
		else if( ChatCommand( "/getbody", pszBuff ) )
		{
			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", UNITDATA->sCharacterData.Player.szBodyModel );
		}
		else if( ChatCommand( "/gethead", pszBuff ) )
		{
			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", UNITDATA->sCharacterData.Player.szHeadModel );
		}
		else if( ChatCommand( "/size", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			UNITDATA->sCharacterData.sSize = atoi( szChatCommandParam1 );
			UNITDATA->sCharacterData.cNewLoad++;
			CHECK_CHARACTER_CHECKSUM;
			UNITGAME->UpdateObjectID( UNITDATA->iID );
		}
		else if ( ChatCommand( "/fontf", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			DAMAGEINFOHANDLER->GetModel()->SetScale( (float)atof( szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/AddTextChat", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			AddChatBoxTextEx( (EChatColor)atoi( szChatCommandParam1 ), szChatCommandParam2 );
		}
		else if ( ChatCommand( "/AddTitleBox", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			CTITLEBOX->SetText( szChatCommandParam1, atoi( szChatCommandParam2 ) );
		}
		else if (ChatCommand("/AddMessageBox", pszBuff))
		{
			GetParameterString(pszBuff, 1, szChatCommandParam1);
			GetParameterString(pszBuff, 2, szChatCommandParam2);

			GAMECORE->GetHUDHandler()->ShowMessageToUser(szChatCommandParam1, szChatCommandParam2);
		}
		else if ( ChatCommand( "/AffinityP", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			DWORD_PTR dw = atoi( szChatCommandParam1 );

			SetProcessAffinityMask( GetCurrentProcess(), dw );
		}
		else if( ChatCommand( "/body", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			UNITDATA->bNoMove = TRUE;
			STRINGFORMAT( UNITDATA->sCharacterData.Player.szBodyModel, "char\\tmABCD\\%s.ini", szChatCommandParam1 );
			SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );
			UNITDATA->sCharacterData.cNewLoad++;
			CHECK_CHARACTER_CHECKSUM;
			UNITGAME->UpdateObjectID( UNITDATA->iID );
		}
		else if ( ChatCommand( "/model", pszBuff ) )
		{
			if ( GetParameterString( pszBuff, 1, szChatCommandParam1 ) )
			{
				char * p = szChatCommandParam1;
				__asm
				{
					PUSH EAX;
					PUSH p;
					MOV EAX, 0x004519A0;
					CALL EAX;
					ADD ESP, 0x04;
					POP EAX;
				}
			}
		}
		else if( ChatCommand( "/head", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			UNITDATA->bNoMove = TRUE;
			STRINGFORMAT( UNITDATA->sCharacterData.Player.szHeadModel, "char\\tmABCD\\%s.inf", szChatCommandParam1 );
			SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );
			CHECK_CHARACTER_CHECKSUM;
		}
		else if ( ChatCommand( "/reloadscripts", pszBuff ) )
		{
			CALL( 0x00574740 );
			AddChatBoxText( "> Scripts reloaded!" );
		}
		else if ( ChatCommand( "/resetforce", pszBuff ) )
		{
			SKILLMANAGERHANDLER->ResetForceOrb();
			AddChatBoxText( "> Force Orb has been reseted!" );
		}
		else if ( ChatCommand ( "/testmap_log_pos", pszBuff ) )
		{
			MINIMAPHANDLER->AddSpawnFlagMarker (UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			UNITGAME->Log("TESTMAP {%d, %d, %d},",UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			UNITGAME->FlushLog ();
		}
		else if ( ChatCommand( "/animTT", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			int j = atoi( szChatCommandParam1 );

			for ( int i = 0; i < UNITDATA->psBody->iNumModelAnimation; i++ )
			{
				if ( UNITDATA->psBody->saModelAnimation[i].uaSkill[0] == j )
					ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> 0x%08X - %d %d", &UNITDATA->psBody->saModelAnimation[i], (int)UNITDATA->psBody->saModelAnimation[i].uaSkill[0], i );
			}
		}
		else if ( ChatCommand( "/anim", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			typedef void( __thiscall *t_SetAnim )(UnitData * pcUnit, int iCode, int v);
			t_SetAnim SetAnim = (t_SetAnim)0x0041F930;
			SetAnim( UNITDATA, atoi( szChatCommandParam1 ), 0 );
		}
		else if ( ChatCommand( "/RT", pszBuff ) )
		{
			CALL( 0x0048A260 );
			AddChatBoxText( "> Read Textures!" );
		}
		else if ( ChatCommand( "/field", pszBuff ) || ChatCommand( "/tp", pszBuff ) || ChatCommand( "/tele", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			TELEPORTPLAYER( atoi( szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/TestW", pszBuff ) )
		{

		}
		else if( ChatCommand( "/SetIntegrity", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if( ITEMHANDLER->GetMouseItemHover() )
			{
				ITEMHANDLER->GetMouseItemHover()->sItem.sIntegrity.sCur = atoi( szChatCommandParam1 );
				ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> New Integrity (%d)", szChatCommandParam1 );
			}
		}
		else if( ChatCommand( "/set_target", pszBuff ) )
		{
			if( CURRENT_TARGET != 0 )
				CHARACTERGAME->FocusTargetPet( CURRENT_TARGET->iID );

			iRetChat = 0;
		}
		else if ( ChatCommand( "/set_def", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_def %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );

			iRetChat = 0;
		}
		else if ( ChatCommand( "/set_abs", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_abs %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );

			iRetChat = 0;
		}
		else if ( ChatCommand( "/set_exp", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_exp %d %lld", CURRENT_TARGET->iID, _atoi64( szChatCommandParam1 ) ) );

			iRetChat = 0;
		}
		else if (ChatCommand("/add_map_id", pszBuff))
		{
			if ( GetParameterString( pszBuff, 1, szChatCommandParam1 ) )
			{
				this->SendChat(FALSE, FormatString("/add_map_indicator_script %d %d %d %d %d %d",
					MAP_ID,
					MapIndicator::Type::NextMapArrow,
					atoi(szChatCommandParam1),
					UNITDATA->sPosition.iX >> 8,
					UNITDATA->sPosition.iZ >> 8,
					UNITDATA->sAngle.iY
				));
			}
			else
			{
				AddChatBoxText("> Syntax: /add_map_id <next map id>. See also `/add_map_next`");
			}

			iRetChat = 0;
		}
		else if (ChatCommand("/add_map_next", pszBuff))
		{
			int iNextMapID = -1;
			int iSecondMapIndex = (UNITDATA->iLoadedMapIndex == 0) ? 1 : 0;
			if ( FIELDGAME[iSecondMapIndex] )
				iNextMapID = FIELDGAME[iSecondMapIndex]->iMapID;

			if ( iNextMapID >= 0 && iNextMapID != MAP_ID)
			{
				this->SendChat(FALSE, FormatString("/add_map_indicator_script %d %d %d %d %d %d",
					MAP_ID,
					MapIndicator::Type::NextMapArrow,
					iNextMapID,
					UNITDATA->sPosition.iX >> 8,
					UNITDATA->sPosition.iZ >> 8,
					UNITDATA->sAngle.iY
				));
			}
			else
			{
				AddChatBoxText("> Invalid next map id. Use `add_map_id <next map id>` if needed");
			}

			iRetChat = 0;
		}
		else if (ChatCommand("/add_map_hs_main", pszBuff)) //main hellspawn
		{
			this->SendChat(FALSE, FormatString("/add_map_indicator_script %d %d %d %d %d %d",
				MAP_ID,
				MapIndicator::Type::HellSpawnMain,
				0,
				UNITDATA->sPosition.iX >> 8,
				UNITDATA->sPosition.iZ >> 8,
				UNITDATA->sAngle.iY
			));

			iRetChat = 0;
		}
		else if (ChatCommand("/add_map_hs_sec", pszBuff)) //secondary hellspawn
		{
			this->SendChat(FALSE, FormatString("/add_map_indicator_script %d %d %d %d %d %d",
				MAP_ID,
				MapIndicator::Type::HellSpawnSecondary,
				0,
				UNITDATA->sPosition.iX >> 8,
				UNITDATA->sPosition.iZ >> 8,
				UNITDATA->sAngle.iY
			));

			iRetChat = 0;
		}
		else if (ChatCommand("/add_map_boss", pszBuff)) //main hellspawn
		{
			this->SendChat(FALSE, FormatString("/add_map_indicator_script %d %d %d %d %d %d",
				MAP_ID,
				MapIndicator::Type::BossIcon,
				0,
				UNITDATA->sPosition.iX >> 8,
				UNITDATA->sPosition.iZ >> 8,
				UNITDATA->sAngle.iY
			));

			iRetChat = 0;
		}
		else if (ChatCommand("/add_npc_here", pszBuff))
		{
			GetParameterString(pszBuff, 1, szChatCommandParam1);

			this->SendChat(FALSE, FormatString("/add_npc %d %d %d %d %d",
				atoi(szChatCommandParam1),
				UNITDATA->sPosition.iX >> 8,
				UNITDATA->sPosition.iY >> 8,
				UNITDATA->sPosition.iZ >> 8,
				UNITDATA->sAngle.iY
			));

			iRetChat = 0;
		}

		else if (ChatCommand("/remove_npc", pszBuff))
		{
			if ( CURRENT_TARGET != NULL &&
				 CURRENT_TARGET->sCharacterData.iType == CHARACTERTYPE_NPC)
			{
				this->SendChat(FALSE, FormatString("/remove_npc %d", CURRENT_TARGET->iID));
			}
			else
			{
				AddChatBoxText("> Invalid target");
			}

			iRetChat = 0;
		}
		else if (ChatCommand("/move_npc_here", pszBuff))
		{
			if (CURRENT_TARGET != 0 &&
				CURRENT_TARGET->sCharacterData.iType == CHARACTERTYPE_NPC)
			{
				this->SendChat(FALSE, FormatString("/set_pos %d %d %d %d",
					CURRENT_TARGET->iID,
					UNITDATA->sPosition.iX >> 8,
					UNITDATA->sPosition.iY >> 8,
					UNITDATA->sPosition.iZ >> 8));

				this->SendChat(FALSE, FormatString("/set_ang %d %d",
					CURRENT_TARGET->iID,
					UNITDATA->sAngle.iY));
			}
			else
			{
				AddChatBoxText("> Invalid target");
			}

			iRetChat = 0;
		}
		else if (ChatCommand("/move_npc_here_by_id", pszBuff))
		{
			GetParameterString(pszBuff, 1, szChatCommandParam1);

			this->SendChat(FALSE, FormatString("/set_pos %d %d %d %d",
				atoi(szChatCommandParam1),
				UNITDATA->sPosition.iX >> 8,
				UNITDATA->sPosition.iY >> 8,
				UNITDATA->sPosition.iZ >> 8));

			this->SendChat(FALSE, FormatString("/set_ang %d %d",
				atoi(szChatCommandParam1),
				UNITDATA->sAngle.iY));

			iRetChat = 0;
		}
		else if ( ChatCommand( "/set_pos", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			GetParameterString( pszBuff, 3, szChatCommandParam3 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_pos %d %d %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ),
					atoi( szChatCommandParam3 ) ) );
			else
			{
				UNITDATA->SetPosition( atoi( szChatCommandParam1 ) << 8, atoi( szChatCommandParam2 ) << 8, atoi( szChatCommandParam3 ) << 8 );

				this->SendChat( FALSE, FormatString( "/set_pos %d %d %d", atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ), atoi( szChatCommandParam3 ) ) );
			}

			iRetChat = 0;
		}
		else if ( ChatCommand( "/set_ang", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_ang %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ) ) );

			iRetChat = 0;
		}
		else if ( ChatCommand( "/EXPT", pszBuff ) )
		{
			int i = 1;
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			i = atoi( szChatCommandParam1 );
//			*(INT64*)(0x0082B9E8 + ((i - 1) * 8)) = _atoi64( szChatCommandParam2 );
//			AddChatBoxTextEx( CHATCOLOR_Error, "> [%d] %lld", i, *(INT64*)(0x0082B9E8 + ((i - 1) * 8)) );

		}

		else if ( ChatCommand( "/set_ViewSightSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
			{
				this->SendChat( FALSE, FormatString( "/set_ViewSightSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
				this->SendChat(FALSE, FormatString("/killmon %d", CURRENT_TARGET->iID));
				this->SendChat(FALSE, FormatString("/mon \"%s\"", CURRENT_TARGET->GetName()));
			}
		}
		//sql commands that requires 2 parameters
		else if (ChatCommand("/sql_SkillMinMax", pszBuff) ||
				 ChatCommand("/sql_AttackMinMax", pszBuff))
		{
			GetCommandString(pszBuff, szChatCommand);

			if (CURRENT_TARGET != 0)
			{
				if (GetParameterString(pszBuff, 1, szChatCommandParam1) &&
					GetParameterString(pszBuff, 2, szChatCommandParam2))
				{
					this->SendChat(FALSE, FormatString("%s %d %s %s", szChatCommand, CURRENT_TARGET->iID, szChatCommandParam1, szChatCommandParam2));
				}
				else
				{
					this->SendChat(FALSE, FormatString("%s %d", szChatCommand, CURRENT_TARGET->iID));
				}
			}
			else
			{
				this->AddChatBoxTextEx(CHATCOLOR_Error, "Mouse target required");
			}
		}

		//sql command that requires 1 parameter
		else if ( ChatCommand("/sql_HP",				pszBuff) || //Health points
				  ChatCommand("/sql_Size",				pszBuff) || //Monster size
				  ChatCommand("/sql_Type",				pszBuff) || //Monster Type
				  ChatCommand("/sql_EXP",				pszBuff) || //Experience
				  ChatCommand("/sql_Absorb",			pszBuff) || //Absorb rating
				  ChatCommand("/sql_Block",				pszBuff) || //Block rating
				  ChatCommand("/sql_StunChance",		pszBuff) || //Stun chance (doesn't work?)
				  ChatCommand("/sql_Defense",			pszBuff) || //Defense
				  ChatCommand("/sql_Potion",			pszBuff) || //Potion
				  ChatCommand("/sql_PotionPercent",		pszBuff) || //Potion percentage
				  ChatCommand("/sql_Organic",			pszBuff) || //Organic resistence
				  ChatCommand("/sql_Lightning",			pszBuff) || //Lightning resistence
				  ChatCommand("/sql_Ice",				pszBuff) || //Ice resistence
				  ChatCommand("/sql_Fire",				pszBuff) || //Fire resistance
				  ChatCommand("/sql_Poison",			pszBuff) || //Poison resistence
				  ChatCommand("/sql_Magic",				pszBuff) || //Magic resistence -- not used
				  ChatCommand("/sql_SpawnMin",			pszBuff) || //Spawn min
				  ChatCommand("/sql_SpawnMax",			pszBuff) || //Spawn max
				  ChatCommand("/sql_MoveSpeed",			pszBuff) || //Move speed (doesn't work?)
				  ChatCommand("/sql_AttackSpeed",		pszBuff) || //Attack speed (doesn't work?)
				  ChatCommand("/sql_AttackRange",		pszBuff) || //Attack range
				  ChatCommand("/sql_AttackRating",		pszBuff) || //Attack rating
				  ChatCommand("/sql_PerfectAttackRate", pszBuff) || //Perfect attack rating (doesn't work?)
				  ChatCommand("/sql_SkillType",			pszBuff) || //Skill type
				  ChatCommand("/sql_SkillChance",		pszBuff) || //Skill chance
				  ChatCommand("/sql_ViewSight",			pszBuff) || //View sight
				  ChatCommand("/sql_NumDrops",			pszBuff) || //Number of loot drops
				  ChatCommand("/sql_SkillHitRange",		pszBuff) ||	//iSkillPierceRange // SkillDistance (uses a rect box to determine hit area?)
				  ChatCommand("/sql_SkillHitBoxLeft",	pszBuff) ||	//Skill hit box rect (left)
				  ChatCommand("/sql_SkillHitBoxRight",	pszBuff) ||	//Skill hit box rect (right)
				  ChatCommand("/sql_SkillHitBoxTop",	pszBuff) ||	//Skill hit box rect (top)
				  ChatCommand("/sql_SkillHitBoxBottom",	pszBuff) ||	//Skill hit box rect (bottom)
				  ChatCommand("/sql_Glow",				pszBuff) ||	//Glow Bossflag
				  ChatCommand("/sql_SkillArea",			pszBuff) ||	//iSkillArea // SkillRange
				  ChatCommand("/sql_Level",				pszBuff))	//Monster Level
		{
			GetCommandString(pszBuff, szChatCommand);

			if (CURRENT_TARGET != 0)
			{
				if (GetParameterString(pszBuff, 1, szChatCommandParam1))
				{
					this->SendChat(FALSE, FormatString("%s %d %s", szChatCommand, CURRENT_TARGET->iID, szChatCommandParam1));
				}
				else
				{
					this->SendChat(FALSE, FormatString("%s %d", szChatCommand, CURRENT_TARGET->iID));
				}
			}
			else
			{
				this->AddChatBoxTextEx(CHATCOLOR_Error, "Mouse target required");
			}
		}

		else if ( ChatCommand( "/HUHUGLJJ", pszBuff ) )
		{
			/*
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			FILE * fp = NULL;
			fopen_s( &fp, "restore.txt", "r" );
			if ( fp )
			{
				static int iRestoreLine = 0;
				int i = 0;

				if ( szChatCommandParam1[0] != 0 )
					iRestoreLine = 0;

				if ( iRestoreLine >= 0 )
				{
					while ( TRUE )
					{
						if ( fgets( szChatCommandParam2, 256, fp ) == NULL )
						{
							iRestoreLine = -1;
							CHATGAMEHANDLER->AddChatBoxText( "> Last Line!" );
							break;
						}

						if ( i == iRestoreLine )
						{
							if ( szChatCommandParam2[STRLEN( szChatCommandParam2 ) - 1] == '\n' )
								szChatCommandParam2[STRLEN( szChatCommandParam2 ) - 1] = 0;

							CHATGAMEHANDLER->SendChat( FALSE, szChatCommandParam2 );
							break;
						}

						i++;
					}
				}

				iRestoreLine++;

				fclose( fp );
			}

			CHATGAMEHANDLER->AddChatBoxText( "Done!" );
			*/

			CAntiDebuggerHandler::CreateHiddenThread( (LPTHREAD_START_ROUTINE)ChatGame::RestoreCommand, NULL, 0 );
		}
		else if ( ChatCommand( "/killmon", pszBuff ) )
		{
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/killmon %d", CURRENT_TARGET->iID ) );
		}
		else if ( ChatCommand ( "/test_research1", pszBuff ) )
		{
			CALL_WITH_ARG2 ( 0x4A0DE0, 1, 1 );
		}
		else if ( ChatCommand ( "/test_research2", pszBuff ) )
		{
			CALL_WITH_ARG2 ( 0x4A0DE0, 1, 1 );
		}
		else if (ChatCommand("/test_NextTarget000", pszBuff))
		{
			if (CURRENT_TARGET != 0)
				this->SendChat(FALSE, FormatString("/test_NextTarget000 %d", CURRENT_TARGET->iID));
		}
		else if (ChatCommand("/test_MovetoSpawn1", pszBuff))
		{
			if (CURRENT_TARGET != 0)
				this->SendChat(FALSE, FormatString("/test_MovetoSpawn1 %d", CURRENT_TARGET->iID));
		}
		else if (ChatCommand("/test_MovetoSpawn2", pszBuff))
		{
			if (CURRENT_TARGET != 0)
				this->SendChat(FALSE, FormatString("/test_MovetoSpawn2 %d", CURRENT_TARGET->iID));
		}
		else if (ChatCommand("/test_MovetoSpawn3", pszBuff))
		{
			if (CURRENT_TARGET != 0)
				this->SendChat(FALSE, FormatString("/test_MovetoSpawn3 %d", CURRENT_TARGET->iID));
		}
		else if (ChatCommand("/test_MovetoMe", pszBuff))
		{
			if (GetParameterString(pszBuff, 1, szChatCommandParam1))
			{
				if (CURRENT_TARGET != 0)
					this->SendChat(FALSE, FormatString("/test_MovetoMe %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)));
			}
		}
		else if (ChatCommand("/dump_skill_timers", pszBuff))
		{
			TIMERSKILLHANDLER->PrintAllSkillTimersToChat();
		}

		else if (ChatCommand("/sod_test_handle_sod_start", pszBuff))
		{
			BELLATRAGAMEHANDLER->HandleSoloSodStart();
			this->SendChat(FALSE, "/sod_test_handle_sod_start");
		}
		else if ( ChatCommand( "/LeakMonsterTest", pszBuff ) )
		{
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/LeakMonsterTest %d", CURRENT_TARGET->iID ) );
		}
		else if ( ChatCommand( "/LeakPacket", pszBuff ) )
		{
			EXEMesh * pt = (EXEMesh*)0;
			ChatGame::AddChatBoxText( pt->szName );

		}
		else if ( ChatCommand( "/set_dmg", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_dmg %d %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ) ) );
		}
		else if ( ChatCommand( "/OpenWindowTest", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GRANDFURYHANDLER->GetWindow()->Open( atoi( szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/CloseWindowTest", pszBuff ) )
		{
			GRANDFURYHANDLER->GetWindow()->Close();
		}
		else if ( ChatCommand( "/TestPP", pszBuff ) )
		{
			CParticleScript * pc = PARTICLEFACTORY->LoadScript( "game\\scripts\\particles\\SkillAtalantaR5ChimeraOfLightAura.lua" );

			pc->SetOwner( new Engine::ObjectOwnerPosition( Point3D( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ ) ) );

			PARTICLEENGINE->AddScript( pc );
		}
		else if ( ChatCommand( "/TrainSkills", pszBuff ) )
		{
			typedef int( __thiscall *t_CheckTrainSkills ) ( UINT uSkillsPtr, int iTier );
			t_CheckTrainSkills CheckTrainSkills = ( t_CheckTrainSkills )0x004ED310;
			typedef int( __thiscall *t_TrainSkills ) ( UINT uSkillsPtr, int iTier );
			t_TrainSkills TrainSkills = ( t_TrainSkills )0x004947B0;

			if ( CheckTrainSkills( 0x367E148, 1 ) == 1 )
				TrainSkills( 0x35D0350, 1 );
			if ( CheckTrainSkills( 0x367E148, 2 ) == 1 )
				TrainSkills( 0x35D0350, 2 );
			if ( CheckTrainSkills( 0x367E148, 3 ) == 1 )
				TrainSkills( 0x35D0350, 3 );

			AddChatBoxText( "> All skills trained successful!" );
		}

		else if ( ChatCommand( "/TestRankup", pszBuff ) )
		{
#ifdef DEV_MODE
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			PLAYER_TIER = atoi(szChatCommandParam1);

			typedef void( __thiscall *t_SearchUseSkill ) ( UINT uSkillsPtr );
			t_SearchUseSkill SearchUseSkill = ( t_SearchUseSkill )0x004ED7F0;
			SearchUseSkill( 0x367E148 ); //skill object


			SKILL_INITIALIZATION_FLAG = 1;
			CALL( 0x45CEE0 ); //ChangeJobFace() //render new face
			SAVE;

			typedef JOB_DATA_CODE*( __cdecl *t_GetJobDataCode ) ( DWORD BaseJob	, int JobCnt );
			t_GetJobDataCode GetJobDataCode = ( t_GetJobDataCode )0x0044D0F0;
			auto jobDataCode = GetJobDataCode( PLAYERDATA->iClass, PLAYER_TIER ); //skill object
			char * invJobName = (char *)0x3625694;

			STRINGCOPYLEN( invJobName, 32, jobDataCode->szName );
#endif
		}
		else if ( ChatCommand( "/SkipTier", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			int iRankUp = atoi( szChatCommandParam1 );
			if( iRankUp > 0 )
				*( UINT* )( PLAYER_BACKUP + 0x3A98 ) = iRankUp;
			else
				*( UINT* )( PLAYER_BACKUP + 0x3A98 ) = 3;

			typedef void( __thiscall *t_SearchUseSkill ) ( UINT uSkillsPtr );
			t_SearchUseSkill SearchUseSkill = ( t_SearchUseSkill )0x004ED7F0;
			SearchUseSkill( 0x367E148 );
			AddChatBoxText( "> RankUp Quest Skipped!" );
		}
		else if ( ChatCommand( "/setmonwp", pszBuff ) )
		{
			INVENTORYITEMS[ITEMINDEX_INVENTORY].sItem.sMatureBar.sCur = 0;
			AddChatBoxText( "> Monsters Finished!" );
		}
		else if ( ChatCommand( "/ForceFinishQuest", pszBuff ) )
		{
			if ( GetParameterString( pszBuff, 1, szChatCommandParam1 ) )
			{
#ifdef DEV_MODE
				CQUESTGAME->ForceFinishDebugOnly(atoi(szChatCommandParam1));
#endif
			}
		}
		else if ( ChatCommand( "/DeleteItemTest", pszBuff ) )
		{
			if ( GetParameterString( pszBuff, 1, szChatCommandParam1 ) )
			{
				CGameInventory::GetInstance()->DeleteItem( atoi( szChatCommandParam1 ) );
			}
		}
		else if ( ChatCommand( "/SkipQuests", pszBuff ) )
		{
			CQUESTGAME->SkipOriginalQuests( TRUE );
		}
		else if ( ChatCommand( "/SkipQuest_30", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_30_ForHer( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> For Her Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_55", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_55_TheCave( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> The Cave Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_70", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_70_FriendshipWithMitchell( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Friendship Mitchell Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_80a", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_80_SealedFury( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Sealed Fury Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_80b", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_80_TestOfKingdom( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Test of Kingdom Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_85", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_85_TearsOfKalia( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Tears Of Kalia Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_90a", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_90_EuraVillage( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Eura Village Skipped!" );
		}
		else if ( ChatCommand( "/SkipQuest_90b", pszBuff ) )
		{
			CQUESTGAME->CompleteAsmQuest_90_BitterOrdeal( );
			CQUESTGAME->UpdateQuestBoard();
			CHECK_CHARACTER_CHECKSUM;
			SAVE;

			CHATGAMEHANDLER->AddChatBoxText( "> Bitter Ordeal Skipped!" );
		}
	}

	return iRetChat;
}

void ChatGame::PHPing( PacketPing * psPacket )
{
}

void ChatGame::AddChatBoxText( char * pszText, EChatColor iColor )
{
	CHATBOX->AddMessage( pszText, iColor );
}

void ChatGame::AddChatBoxTextEx( EChatColor iColor, char * pszText, ... )
{
	va_list vl;

	char szText[128];

	va_start( vl, pszText );
	_vsnprintf_s( szText, _countof( szText ), _TRUNCATE, pszText, vl );
	va_end( vl );

	AddChatBoxText( szText, iColor );
}

void ChatGame::HandlePacket ( PacketWrapCamera * psPacket )
{
	CHATBOX->AddMessage ( FormatString ( "Wrap> Map %d at iX:%d iZ:%d", psPacket->iMap, psPacket->iX, psPacket->iZ ) );

	iCameraCircleOverride = psPacket->iTurn;
	iCameraAngleOverride = psPacket->iAngle;
	iCameraZoomOverride = psPacket->iZoom;
}

void ChatGame::ChatBuffSort()
{
}
