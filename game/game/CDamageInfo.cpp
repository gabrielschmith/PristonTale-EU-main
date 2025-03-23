#include "stdafx.h"
#include "CDamageInfo.h"
#include "MDamageInfo.h"

bool						bNoShowDebugDamage = false;

void CDamageInfoController::Clear( DamageDataInfo * psData )
{
	if ( psData != NULL )
		ZeroMemory( psData, sizeof( DamageDataInfo ) );
}

CDamageInfoController::CDamageInfoController()
{

}


void CDamageInfoController::Init()
{
	//Font
	pOrangeFont->ReadFontData( "game\\fonts\\font.dat" );
	pOrangeFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font1.png" ) );

	pRedFont->ReadFontData( "game\\fonts\\font.dat" );
	pRedFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font2.png" ) );
		
	pBlueFont->ReadFontData( "game\\fonts\\font3.dat" );
	pBlueFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font3.png" ) );

	pGoldFont->ReadFontData("game\\fonts\\font4.dat");
	pGoldFont->SetFontImage(UI::ImageLoader::LoadImage("game\\fonts\\font4.png"));
	
	pGreenFont->ReadFontData ( "game\\fonts\\font.dat" );
	pGreenFont->SetFontImage ( UI::ImageLoader::LoadImage ( "game\\fonts\\font5a.png" ) );

	pGreyFont->ReadFontData ( "game\\fonts\\font.dat" );
	pGreyFont->SetFontImage ( UI::ImageLoader::LoadImage ( "game\\fonts\\font6.png" ) );

	pYellowFont->ReadFontData ( "game\\fonts\\font.dat" );
	pYellowFont->SetFontImage ( UI::ImageLoader::LoadImage ( "game\\fonts\\font7.png" ) );

	pBitmapFont->ReadFontData( "game\\fonts\\messagebox.dat" );
	pBitmapFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\messagebox.png" ) );

	//pCyanFont->ReadFontData( "game\\fonts\\font3.dat" );
	//pCyanFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font3.png" ) );
}

void CDamageInfoController::Load()
{
	iDefenseInfoPart = PARTICLE->Load( "game\\scripts\\particles\\DefenseInfo.part" );
	iBlockInfoPart = PARTICLE->Load( "game\\scripts\\particles\\BlockInfo.part" );
	iEvadeInfoPart = PARTICLE->Load( "game\\scripts\\particles\\EvadeInfo.part" );
}

void CDamageInfoController::OnHitReceived( EDamageTextType TypeAction, int iValue )
{
	TESTMAPHANDLER->OnHitReceived( TypeAction, iValue );
	DAMAGEINFOHANDLER->SetDefenseInfo( TypeAction, iValue );
}

void CDamageInfoController::SetDefenseInfo( EDamageTextType TypeAction, int Value, ID iTargetIDOverride, ID iAttackerIDOverride )
{
	static DWORD dwTimeDamageSend = 0;

	//DEBUG("%d = DELTA = %d, TypeAction = %d, Value = %d", TICKCOUNT, (TICKCOUNT - dwTimeDamageSend), TypeAction, Value);


	//block / defense / evade
	if ( TypeAction <= EDamageTextType::Evade )
	{
		CALLT_WITH_ARG1( 0x004A7400, 0, (int)TypeAction );
	}

	PacketDebugDamageInfo sPacket;
	sPacket.iLength		= sizeof( PacketDebugDamageInfo );
	sPacket.iHeader		= PKTHDR_DamageDebugInfo;
	sPacket.dwObjectID	= iTargetIDOverride;
	sPacket.dwAttackerID = iAttackerIDOverride;
	sPacket.sTypeAction = TypeAction;
	sPacket.iValue		= Value;

	if ( TypeAction == EDamageTextType::Gold ) //gold
	{
		AddTarget( iAttackerIDOverride, -1, TypeAction, Value );
    }
    else if (TypeAction == EDamageTextType::Healing)
    {
		AddTarget( iAttackerIDOverride, iTargetIDOverride, TypeAction, Value );
    }
	else if ( TypeAction == EDamageTextType::Miss ||
		TypeAction == EDamageTextType::Flinch )
	{
		AddTarget (iAttackerIDOverride,  iTargetIDOverride, TypeAction, 0 );
	}
	else
	{
		if ( ( TICKCOUNT - dwTimeDamageSend ) > 128 )
		{
			RelayInfoToOthers( iTargetIDOverride, TypeAction, Value );
			dwTimeDamageSend = TICKCOUNT;
		}
		if ( TypeAction == EDamageTextType::Normal ||
			 TypeAction == EDamageTextType::Critical )
		{
			AddTarget ( iAttackerIDOverride, iTargetIDOverride, TypeAction, Value );
		}
	}
}

void CDamageInfoController::HandlePacket ( PacketDebugDamageInfoContainer * psPacket )
{
	USHORT uPosition = 0;
	for ( int i = 0; i < psPacket->sAmount; i++ )
	{
		PacketDebugDamageInfo * data = (PacketDebugDamageInfo *)( psPacket->baBuffer + uPosition );
		AddTarget ( data );
		uPosition += sizeof ( PacketDebugDamageInfo );
	}
}

/// <summary>
/// Relay damage info to others, excluding self.
/// </summary>
void CDamageInfoController::RelayInfoToOthers( ID idTargetID, EDamageTextType eType, int iValue)
{
	PacketDebugDamageInfo sPacket;
	sPacket.iLength			= sizeof( PacketDebugDamageInfo );
	sPacket.iHeader			= PKTHDR_DamageDebugInfo;
	sPacket.dwObjectID		= idTargetID;
	sPacket.sTypeAction		= eType;
	sPacket.iValue			= iValue;

	SENDPACKETG ( &sPacket );
}

void CDamageInfoController::AddTarget( PacketDebugDamageInfo * psPacket )
{
	AddTarget( psPacket->dwAttackerID, psPacket->dwObjectID, psPacket->sTypeAction, psPacket->iValue, psPacket->sParameter );
}

void CDamageInfoController::AddTarget( int iAttackerID, int idTargetID, EDamageTextType eType, int iValue, short sParameter)
{

#ifdef DEV_MODE
	if ( GM_MODE && MAP_ID == MAPID_QuestArena && eType != EDamageTextType::Healing)
	{
		TESTMAPHANDLER->AddTargetDamageDealt ( iAttackerID, idTargetID, eType, iValue);
	}
#endif

	if ( bNoShowDebugDamage || CHARACTERGAME->IsStageVillage() /*|| GetForegroundWindow() != DX::Graphic::GetWindowHandler()*/ )
		return;

	UnitData * pcUnitData = NULL;

	if ( idTargetID == 0 || idTargetID == -1 )
	{
		pcUnitData = UNITDATA;
	}
	else
	{
		pcUnitData = UNITDATABYID ( idTargetID );
	}

	if ( pcUnitData )
	{
		Unit * pcUnit = UNITDATATOUNIT ( pcUnitData );

		if (pcUnit)
		{
			DamageInfo* psDamageInfo = new DamageInfo();
			psDamageInfo->iDecreaseY = 0;
			psDamageInfo->dwTime = 0;
			psDamageInfo->iType = eType;
			psDamageInfo->iValue = iValue;
			psDamageInfo->sParameter = sParameter;

			//add to queue
			pcUnit->vDamageInfoList.push_back(psDamageInfo);
		}
	}
}


void CDamageInfoController::RenderUnitDamageInfo ( Unit * pcUnit )
{
	if ( bNoShowDebugDamage == true || (CHARACTERGAME->IsStageVillage () == TRUE && MAP_ID != MAPID_NaviskoTown) )
	{
		//remove all, and clear
		for ( auto& info : pcUnit->vDamageInfoList )
		{
			SAFE_DELETE ( info );
		}
		pcUnit->vDamageInfoList.clear ();
		return;
	}

	int iAlpha = 0;
	int iX = pcUnit->pcUnitData->rRenderRectangle.left + ( ( pcUnit->pcUnitData->rRenderRectangle.right - pcUnit->pcUnitData->rRenderRectangle.left ) >> 1 ) - 20;
	int iY = pcUnit->pcUnitData->rRenderRectangle.top - 64;

	for ( std::vector<DamageInfo *>::iterator it = pcUnit->vDamageInfoList.begin (); it != pcUnit->vDamageInfoList.end (); )
	{
		DamageInfo * info = *it;

		if (info == nullptr)
		{
			it = pcUnit->vDamageInfoList.erase(it);
			continue;
		}

		//time not set? set it.
		if ( info->dwTime == 0 )
		{
			info->dwTime = TICKCOUNT;
		}

		info->iDecreaseY -= GetDecrease ();

		int iTimeDifference = (int)( ( info->dwTime + MAX_DEBUG_TIME ) - TICKCOUNT );

		iAlpha = 0;

		if ( iTimeDifference > 0 )
		{
			if ( ( iTimeDifference >= MAX_DEBUG_TIMEHALF ) )
			{
				iTimeDifference -= MAX_DEBUG_TIMEHALF;
				iTimeDifference = MAX_DEBUG_TIMEHALF - iTimeDifference;
				iAlpha = ( iTimeDifference * 255 ) / MAX_DEBUG_TIMEHALF;
			}
			else
			{
				iAlpha = ( iTimeDifference * 255 ) / MAX_DEBUG_TIMEHALF;
			}
		}

		int value = pcUnit == UNIT ? -info->iValue : info->iValue;
		char szText[32] = { 0 };

		if (info->iType == EDamageTextType::Healing)
        {
            int value = info->iValue;
            STRINGFORMAT(szText, "+%d", value);
		}
		else
			STRINGFORMAT ( szText, "%d", value );

		if ( info->iType == EDamageTextType::Normal )
		{
			if ( pcUnit == UNIT ) //me
			{
				pRedFont->SetPosition ( iX, iY + info->iDecreaseY );
				pRedFont->SetText ( szText );
				pRedFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
				pRedFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
			else //others
			{
				pOrangeFont->SetPosition ( iX, iY + info->iDecreaseY );
				pOrangeFont->SetText ( szText );
				pOrangeFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
				pOrangeFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
		}
		else if ( info->iType == EDamageTextType::Damage )
		{
			pRedFont->SetPosition ( iX, iY + info->iDecreaseY );
			pRedFont->SetText ( szText );
			pRedFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pRedFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::PetAttack )
		{
			pYellowFont->SetPosition ( iX, iY + info->iDecreaseY );
			pYellowFont->SetText ( szText );
			pYellowFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pYellowFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
        }
        else if (info->iType == EDamageTextType::Healing)
        {
            pBlueFont->SetPosition(iX, iY + info->iDecreaseY);
            pBlueFont->SetText(szText);
            pBlueFont->SetColor(D3DCOLOR_RGBA(100, 255, 100, iAlpha));
            pBlueFont->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
        }
		else if ( info->iType == EDamageTextType::Defense )
		{
			pBitmapFont->SetPosition ( iX, iY + info->iDecreaseY );
			pBitmapFont->SetText ( "Defense" );
			pBitmapFont->SetColor ( D3DCOLOR_RGBA ( 90, 203, 244, iAlpha ) );
			pBitmapFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Evade )
		{
			pBitmapFont->SetPosition ( iX, iY + info->iDecreaseY );
			pBitmapFont->SetText ( "Evade" );
			pBitmapFont->SetColor ( D3DCOLOR_RGBA ( 236, 156, 70, iAlpha ) );
			pBitmapFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Block )
		{
			pBitmapFont->SetPosition ( iX, iY + info->iDecreaseY );
			pBitmapFont->SetText ( "Block" );
			pBitmapFont->SetColor ( D3DCOLOR_RGBA ( 243, 216, 76, iAlpha ) );
			pBitmapFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Critical ) //crit
		{
			pBlueFont->SetPosition ( iX, iY + info->iDecreaseY );
			pBlueFont->SetText ( szText );
			pBlueFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pBlueFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Miss ) //missing a target
		{
			pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
			pGreyFont->SetText ( "Miss" );
			pGreyFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Flinch ) //Debug only - Flinch on dmg
		{
			pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
			pGreyFont->SetText ( "Flinch" );
			pGreyFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Stun )
		{
			pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
			pGreyFont->SetText( szText );
			pGreyFont->SetColor ( D3DCOLOR_RGBA ( 221, 173, 222, iAlpha ) );
			pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Freezing )
		{
			pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
			pGreyFont->SetText(  szText );
			pGreyFont->SetColor ( D3DCOLOR_RGBA ( 149, 232, 224, iAlpha ) );
			pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Gold ) //gold
		{
			char szGoldText[32] = { 0 };
			STRINGFORMAT ( szGoldText, "+%dG", info->iValue ); // "G" is the Gold symbol in the font file

			iX -= 10;
			iY += 64;

			pGoldFont->SetPosition ( iX + 70, iY + 30 + info->iDecreaseY );
			pGoldFont->SetText ( szGoldText );
			pGoldFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pGoldFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Poison ) //poison
		{
			pGreenFont->SetPosition ( iX, iY + info->iDecreaseY );

			if ( info->sParameter > 1 )
				pGreenFont->SetText( FormatString( "%d (%d)", value, info->sParameter ) );
			else
				pGreenFont->SetText( szText );

			pGreenFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pGreenFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Burning ) //burning
		{
			pRedFont->SetPosition ( iX, iY + info->iDecreaseY );

			if ( info->sParameter > 1 )
				pRedFont->SetText( FormatString( "%d (%d)", value, info->sParameter ) );
			else
				pRedFont->SetText( szText );

			pRedFont->SetColor ( D3DCOLOR_RGBA ( 255, 255, 255, iAlpha ) );
			pRedFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Distortion ) //burning
		{
			pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
			pGreyFont->SetText(  szText );
			pGreyFont->SetColor ( D3DCOLOR_RGBA ( 212, 245, 66, iAlpha ) );
			pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else if ( info->iType == EDamageTextType::Curse ) //burning
		{
		pGreyFont->SetPosition ( iX, iY + info->iDecreaseY );
		pGreyFont->SetText(  szText );
		pGreyFont->SetColor ( D3DCOLOR_RGBA ( 221, 66, 245, iAlpha ) );
		pGreyFont->Render ( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		
		int minSpacing = -16 + ( pcUnit->vDamageInfoList.size () );
		if ( minSpacing > -1 )
			minSpacing = -1;

		//this info isn't high enough to show the next one, so we won't render the next ones just yet,
		//however if there's many of them still pending, make the requirement shorter.
		if ( info->iDecreaseY >= minSpacing )
		{
			break;
		}

		if ( TICKCOUNT > ( info->dwTime + MAX_DEBUG_TIME ) )
		{
			SAFE_DELETE ( info ); //free pointer
			it = pcUnit->vDamageInfoList.erase ( it );
		}
		else
		{
			it++;
		}
	}	
}

void CDamageInfoController::Render()
{
	if ( ( TICKCOUNT - dwTimeUpdate ) >= 16 )
	{
		iDecreaseY = ( TICKCOUNT - dwTimeUpdate ) / 16;
		dwTimeUpdate = TICKCOUNT;
	}
	else
	{
		iDecreaseY = 0;
	}
}

void CDamageInfoController::HandlePacket(PacketGoldPickup* psPacket)
{
	DAMAGEINFOHANDLER->SetDefenseInfo ( EDamageTextType::Gold, psPacket->iAmount );
}

void CDamageInfoController::HandlePacket(PacketHealing * psPacket)
{
    DAMAGEINFOHANDLER->SetDefenseInfo(EDamageTextType::Healing, psPacket->iHealAmount);
}

void CDamageInfoController::HandlePacket(PacketGrandHealing * psPacket)
{
	if (psPacket->iCasterID == UNITDATA->iID)
	{
        auto l_SkillLevel	= SKILLMANAGERHANDLER->GetLevelSkill(ESkillID::SKILLID_GrandHealing) - 1;
		auto l_Range		= SKILLMANAGERHANDLER->GetSkillIntValue(Priestess_GrandHealing_Range, l_SkillLevel);

        auto vUsers = PARTYHANDLER->GetPartyMembers();
        for (auto & p : vUsers)
        {
            if (p && !p->IsDead() && p->sPosition.GetPTDistanceXZ(&UNITDATA->sPosition) < (l_Range * l_Range))
                DAMAGEINFOHANDLER->SetDefenseInfo(EDamageTextType::Healing, psPacket->iHealAmount, p->iID);
        }

        if (vUsers.size() == 0)
            DAMAGEINFOHANDLER->SetDefenseInfo(EDamageTextType::Healing, psPacket->iHealAmount);
	}
    else
        DAMAGEINFOHANDLER->SetDefenseInfo(EDamageTextType::Healing, psPacket->iHealAmount);
}

void CDamageInfoController::Update( float fTime )
{
	int i = (1000 / 70);

	int iInc = (int)fTime % 15;
	if ( iInc == 0 )
		iInc = 1;

}

CDamageInfoController::~CDamageInfoController()
{
}
