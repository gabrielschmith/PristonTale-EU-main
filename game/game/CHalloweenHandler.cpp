#include "stdafx.h"
#include "CHalloweenHandler.h"


CHalloweenHandler::CHalloweenHandler()
{
}


CHalloweenHandler::~CHalloweenHandler()
{
	SAFE_DELETE( pcModel );
}


void CHalloweenHandler::OnSetTimeDay( DWORD * pdwHour, DWORD * pdwMinute )
{
	int iMapID = CHARACTERGAME->GetStageID();
	if ( Game::GetGameMode() == GAMEMODE_InGame && iMapID != (-1) && (iMapID == 3 || iMapID == 21))
		pcModel->SetDayTime( pdwHour, pdwMinute );
}

void CHalloweenHandler::Init()
{
	bEventIsAlive = FALSE;
	pcModel = new CHalloweenModel();
}

BOOL CHalloweenHandler::IsLantern( DWORD dwCode )
{
	BOOL bRet = FALSE;

	switch ( dwCode )
	{
		case ITEMID_RottenLantern:
		case ITEMID_EvilLantern:
		case ITEMID_InfernalLantern:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

BOOL CHalloweenHandler::IsValidRune( class ItemData * pcItem )
{
	BOOL bRet = TRUE;

	switch ( pcItem->sItem.sItemID.ToItemID() )
	{
		case ITEMID_HopyToy:
		case ITEMID_SpiderPlastic:
		case ITEMID_ToiletPaper:
		case ITEMID_GriffenEgg:
			bRet = TRUE;
			break;
	}

	return bRet;
}

BOOL CHalloweenHandler::IsUsingSuit( class UnitData * pcUnitData )
{
	BOOL bUseSuit = FALSE;

	if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\ca031.ini" ) )
			bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cb031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cc031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cd031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\ce031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cma031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cmb031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cmc031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cmd031.ini" ) )
		bUseSuit = TRUE;
	else if ( STRINGCOMPARE( pcUnitData->sCharacterData.Player.szBodyModel, "char\\tmabcd\\cme031.ini" ) )
		bUseSuit = TRUE;

	return bUseSuit;
}

void CHalloweenHandler::PHSetEventStatus( struct PacketHalloweenEventStatus * psPacket )
{
#ifdef DEV_MODE
	CHATBOX->AddMessage ( FormatString("DEBUG> CHalloweenHandler status: %d", psPacket->bStatus ) );
#endif

	bEventIsAlive			= psPacket->bStatus;
	*(BOOL*)(0x04B07260)	= psPacket->bStatus; // Set status at the game.exe ( use for time addr )
}

void CHalloweenHandler::Render()
{
	if ( !EventIsAlive() )
		return;

	if ( Game::GetGameMode() == GAMEMODE_InGame && iParticlePointer != -1 )
	{
		if ( TICKCOUNT > dwRenderUpdate )
		{
			for ( int i = 0; i < MAX_UNITS + 1; i++ )
			{
				if ( iParticleID[i] )
				{
					PARTICLE->Stop( iParticleID[i] );
				}
			}
			// Clear
			ZeroMemory( iParticleID, sizeof( int ) * 1025 );

			// Player using suit?
			if ( IsUsingSuit( UNITDATA ) )
			{
				iParticleID[1024] = PARTICLE->Start( iParticlePointer, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			}


			// Check Units using suit
			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				UnitData * pcUnitData = UNITGAME->pcaUnitData + i;
				if ( IsUsingSuit( pcUnitData ) && pcUnitData->bActive )
				{
					iParticleID[i] = PARTICLE->Start( iParticlePointer, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
				}
			}


			// Update Time
			dwRenderUpdate = TICKCOUNT + 4000;
		}
		else
		{
			if ( iParticleID[1024] )
			{
				PARTICLE->SetPosition( iParticleID[1024], D3DVECTOR{ (float)UNITDATA->sPosition.iX, (float)UNITDATA->sPosition.iY, (float)UNITDATA->sPosition.iZ } );
				PARTICLE->SetRendering( iParticleID[1024], TRUE );
			}

			for ( int i = 0; i < MAX_UNITS; i++ )
			{
				UnitData * pcUnitData = UNITGAME->pcaUnitData + i;
				if ( iParticleID[i] )
				{
					PARTICLE->SetPosition( iParticleID[i], D3DVECTOR{ (float)pcUnitData->sPosition.iX, (float)pcUnitData->sPosition.iY, (float)pcUnitData->sPosition.iZ } );
					PARTICLE->SetRendering( iParticleID[i], TRUE );
				}
			}
		}
	}
}

void CHalloweenHandler::LoadParticle()
{
	iParticlePointer = PARTICLE->Load( "game\\scripts\\particles\\HalloweenSuit.part" );
}
