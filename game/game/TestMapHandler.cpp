#include "stdafx.h"
#include "TestMapHandler.h"


CTestMapHandler::CTestMapHandler ()
{
}


CTestMapHandler::~CTestMapHandler ()
{
}

void CTestMapHandler::Init ()
{
	iFontID = FONTHANDLER->CreateFont ( "Courier New", iFontHeight, 0, FALSE, TRUE, -1 );

	Reset ();
}

void CTestMapHandler::Reset ()
{
	iSkillFirstHitDelayMin = 5000;
	bFirst = TRUE;
	iSkillHitFrameMin = INT_MAX;
	iTargetCount = 0;
	dwIgnoreUntilTick = TICKCOUNT + 2000; // to improve dps
	bIgnoring = TRUE;
	dwStartDamageAllTick = 0;
	dwStartDamageReceivedTick = 0;
	iHitDealtAll = 0;
	iHitsAttempted = 0;
	iHitsAvoided = 0;
	iHitsReceived = 0;
	iTotalDamageAll = 0;
	iDPSAll = 0;
	iDPSAll_NoCrit = 0;
	iCritCounter = 0;
	iMissCounter = 0;
	iBlockCounter = 0;
	iEvadeCounter = 0;
	iDefenseCounter = 0;
	fDPSDurationTotalSec = 0;
	iTotalDamageReceived = 0;
	fDPSReceived = 0;
	dwLastHitReceivedTick = 0;
	dwLastDamageTick = 0;

	mTargetIDs.clear ();
	viDamageDealtAll.clear ();
	viDamageDealtAll_NoCrit.clear ();
}

void CTestMapHandler::Tick1s ()
{
	if ( bPaused )
		return;


	int iTimeSinceLastHitReceived = TICKCOUNT - dwLastHitReceivedTick;
	int iCorrection = 0;
	if ( dwLastHitReceivedTick > 0 && iTimeSinceLastHitReceived > 5000 )
	{
		iCorrection = iTimeSinceLastHitReceived;
		bPaused = TRUE;
	}

	if ( iTotalDamageReceived > 0 )
	{
		fDamagedReceivedDuration = static_cast<float>( ( TICKCOUNT - dwStartDamageReceivedTick - iCorrection ) ) / 1000.0f;
		fDPSReceived = static_cast<float>( iTotalDamageReceived ) / fDamagedReceivedDuration;
	}

	int iTimeSinceLastHit = TICKCOUNT - dwLastDamageTick;
	if ( dwLastDamageTick > 0 && iTimeSinceLastHit > 5000 )
	{
		iCorrection = iTimeSinceLastHit;
		bPaused = TRUE;
	}

	if ( dwStartDamageAllTick > 0 )
	{
		int iTotal = 0;
		for ( auto value : viDamageDealtAll )
		{
			iTotal += value;
		}

		iTotalDamageAll = iTotal;
		fDPSDurationTotalSec = static_cast<float>( ( TICKCOUNT - dwStartDamageAllTick - iCorrection ) ) / 1000.0f;
		iDPSAll = static_cast<int>( round ( static_cast<float>( iTotal ) / fDPSDurationTotalSec ) );

		iTotal = 0;
		for ( auto value : viDamageDealtAll_NoCrit )
		{
			iTotal += value;
		}
		iDPSAll_NoCrit = static_cast<int>( round ( static_cast<float>( iTotal ) / fDPSDurationTotalSec ) );
	}
}

void CTestMapHandler::Render ()
{
	//RENDER DPS INFO
	if ( GM_MODE && MAP_ID == MAPID_QuestArena )
	{
		int iY = 200;
		int iColumn1 = 180;

		if ( bPaused )
		{
			FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "PAUSED" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 209, 158, 17, 255 ) );
		}
		//else if ( bIgnoring ) //ignoring the first n seconds to calc dps better
		//{
		//	FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "WARMING UP" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 209, 158, 17, 255 ) );
		//}
		else
		{
			FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "RUNNING" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 25, 158, 41, 255 ) );
		}

		iY += 20;

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "DUR (SEC)" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		if (fDPSDurationTotalSec > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%.1f", fDPSDurationTotalSec ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else if ( fDamagedReceivedDuration > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%.1f", fDamagedReceivedDuration ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		iY += 20;

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "DPS" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%d", iDPSAll ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

		//FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "DPS (NO CRIT)" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		//FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iDPSAll_NoCrit ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "TOTAL DMG" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%d", iTotalDamageAll ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "CRIT COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		if ( iCritCounter > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%d (%d%%)", iCritCounter, static_cast<int>( ( static_cast<float>( iCritCounter ) / static_cast<float>( iHitDealtAll ) ) * 100.0f ) ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "MISS COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		if ( iMissCounter > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%d (%d%%)", iMissCounter, static_cast<int>( ( static_cast<float>( iMissCounter ) / static_cast<float>( iHitDealtAll ) ) * 100.0f ) ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "HIT COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		if ( iHitDealtAll > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%d", iHitDealtAll ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		iY += 20;

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "DPS RECEIVED" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		if ( iTotalDamageReceived > 0 )
		{
			FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%.0f", fDPSReceived ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "DEFENSE COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		if ( iDefenseCounter > 0 )
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d (%d%%)", iDefenseCounter, static_cast<int>((static_cast<float>(iDefenseCounter) / static_cast<float>(iHitsAttempted)) * 100.0f )), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "BLOCK COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		if ( iBlockCounter > 0 )
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d (%d%%)", iBlockCounter, static_cast<int>((static_cast<float>(iBlockCounter) / static_cast<float>(iHitsAttempted)) * 100.0f )), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "EVADE COUNT" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		if ( iEvadeCounter > 0 )
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d (%d%%)", iEvadeCounter, static_cast<int>((static_cast<float>(iEvadeCounter) / static_cast<float>(iHitsAttempted)) * 100.0f )), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "HITS AVOIDED" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		if ( iDefenseCounter > 0 )
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d (%d%%)", iHitsAvoided, static_cast<int>((static_cast<float>(iHitsAvoided) / static_cast<float>(iHitsAttempted)) * 100.0f )), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}
		else
		{
			FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, "-", DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );
		}


		iY += 20;


		char hexString[16];
		itoa ( GetSkillIDFromTinyID(UNITDATA->sCharacterData.iClass, eCurrentSkillTinyId), hexString, 16 );
		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "SKILL ID" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, eCurrentSkillTinyId == 0 ? FormatString ("-") : FormatString ( "0x%s (%d)", hexString, eCurrentSkillTinyId ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "ANIM TYPE" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iAnimationType ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "ANIM START" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iAnimationBeginFrame ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "ANIM END" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iAnimationEndFrame ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "ANIM LEN" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iAnimationEndFrame - iAnimationBeginFrame ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw ( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString ( "FRAME STEP" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw ( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString ( "%d", iAnimationFrameStep ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA ( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "DELAY" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%dms", bIgnoring ? 0 : iSkillFirstHitDelayLast ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

		FONTHANDLER->Draw( iFontID, RECT{ 5, ( ( iY += iFontHeight + 8 ) ), 0, 0 }, FormatString( "DELAY (MIN)" ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 227, 214, 34, 255 ) );
		FONTHANDLER->Draw( iFontID, RECT{ iColumn1, ( ( iY ) ), 0, 0 }, FormatString( "%dms", bIgnoring ? 0 : iSkillFirstHitDelayMin ), DT_NOCLIP | DT_LEFT | DT_VCENTER, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	}
}

void CTestMapHandler::OnGeneralSkillHit ()
{
	//CHATBOX->AddDebugMessage ( FormatString ( "OnGeneralSkillHit %d vs %d", UNITDATA->iFrame, iSkillHitFrameMin ) );

	if ( UNITDATA->iFrame < iSkillHitFrameMin )
	{
		//CHATBOX->AddDebugMessage ( FormatString ( "frame %d is less than min %d", UNITDATA->iFrame, iSkillHitFrameMin ) );

		iSkillHitFrameMin = UNITDATA->iFrame;
		dwSkillFirstHitLastTick = TICKCOUNT;
	}

	if ( iSkillHitFrameMin == UNITDATA->iFrame )
	{
		iTargetCount = mTargetIDs.size ();
		mTargetIDs.clear ();

		int iDelay = TICKCOUNT - dwSkillFirstHitLastTick;

		//CHATBOX->AddDebugMessage ( FormatString ( "is min frame, delay = %d, frame %d", iDelay, UNITDATA->iFrame ) );

		if ( iDelay > 0 && iDelay < 5000 )
		{
			if ( iDelay < iSkillFirstHitDelayMin )
			{
				iSkillFirstHitDelayMin = iDelay;
			}

			iSkillFirstHitDelayLast = iDelay;
		}

		dwSkillFirstHitLastTick = TICKCOUNT;
	}

	int iAnimType = UNITDATA->psModelAnimation->iType;
	if ( iAnimType == ANIMATIONTYPE_Attack ||
		 iAnimType == ANIMATIONTYPE_Skill )
	{
		iAnimationType = UNITDATA->psModelAnimation->iType;
		iAnimationBeginFrame = UNITDATA->psModelAnimation->iBeginFrame;
		iAnimationEndFrame = UNITDATA->psModelAnimation->iEndFrame;
		iAnimationFrameStep = UNITDATA->iFrameStep;
	}
}

/// <summary>
/// Call this when the skill animation has dealt its first hit
/// </summary>
void CTestMapHandler::OnMultiTargetSkillHit ( PacketMultiTargetSkillData * psOutgoingAOESkillPacket, ActiveSkill * pcActiveSkill )
{
	int thisSkillCode = psOutgoingAOESkillPacket->iSkillID;

	if ( bPaused )
	{
		Reset ();
		bPaused = FALSE;
	}

	bMultiSkillTarget = TRUE;

	eCurrentSkillTinyId			= thisSkillCode;
	iCurrentDamageState		= psOutgoingAOESkillPacket->iDamageState;

	iPlayerCriticalChance	= psOutgoingAOESkillPacket->sCriticalChance;
	iPlayerAttackRating		= psOutgoingAOESkillPacket->sAttackRating;
	iPlayerMinDamage		= psOutgoingAOESkillPacket->sAttackPower.sMin;
	iPlayerMaxDamage		= psOutgoingAOESkillPacket->sAttackPower.sMax;

	sActiveSkillTinyID		= pcActiveSkill->cSkillTinyID;

	OnGeneralSkillHit ();
}


/// <summary>
/// A single damage have been executed onto a Target
/// Note - don't record damage or range data here
/// because the server will make adjustments based on database values
/// Use AddTargetDamageDealt()
/// </summary>
void CTestMapHandler::OnSingleTargetSkillHit ( PacketSingleTargetSkillData * psOutgoingAttackPacket, ActiveSkill * pcActiveSkill)
{
	ESkillID thisSkillID = (ESkillID)psOutgoingAttackPacket->iSkillCode;

	if ( bPaused )
	{
		Reset ();
		bPaused = FALSE;
	}


	bMultiSkillTarget		= FALSE;
	eCurrentSkillTinyId		= psOutgoingAttackPacket->iSkillCode;
	iCurrentDamageState		= (int) psOutgoingAttackPacket->eAttackState;
	iCurrentTarget			= psOutgoingAttackPacket->dwTarObjectSerial;

	iPlayerCriticalChance	= psOutgoingAttackPacket->sCriticalChance;
	iPlayerAttackRating		= psOutgoingAttackPacket->iAttackRating;
	iPlayerMinDamage		= psOutgoingAttackPacket->AttackPowerMin;
	iPlayerMaxDamage		= psOutgoingAttackPacket->AttackPowerMax;

	sActiveSkillTinyID		= pcActiveSkill->cSkillTinyID;

	OnGeneralSkillHit ();
}

void CTestMapHandler::OnHitReceived(EDamageTextType eDamageTextType, int iValue)
{
	iHitsAttempted++;

	if ( bPaused )
	{
		Reset ();
		bPaused = FALSE;
	}

	dwLastHitReceivedTick = TICKCOUNT;

	if ( dwStartDamageReceivedTick == 0 )
	{
		dwStartDamageReceivedTick = TICKCOUNT;
	}

	if ( eDamageTextType == EDamageTextType::Normal ||
		 eDamageTextType == EDamageTextType::Critical )
	{
		iTotalDamageReceived += iValue;
		iHitsReceived++;
	}

	if ( eDamageTextType == EDamageTextType::Defense )
	{
		iDefenseCounter++;
		iHitsAvoided++;
	}

	if ( eDamageTextType == EDamageTextType::Block )
	{
		iBlockCounter++;
		iHitsAvoided++;
	}

	else if ( eDamageTextType == EDamageTextType::Evade )
	{
		iEvadeCounter++;
		iHitsAvoided++;
	}
}

void CTestMapHandler::AddTargetDamageDealt( PacketDebugDamageInfo * packet )
{
	AddTargetDamageDealt( packet->dwAttackerID, packet->dwObjectID, packet->sTypeAction, packet->iValue );
}

void CTestMapHandler::AddTargetDamageDealt ( ID idAttacker, ID idTarget, EDamageTextType eTypeAction, int iValue )
{
	//ignore others
	if ( idAttacker != UNITDATA->iID )
		return;

	dwLastDamageTick = TICKCOUNT;

	//if ( TICKCOUNT < dwIgnoreUntilTick )
	//	return;

	//bIgnoring = FALSE;

	mTargetIDs.insert ( idTarget );

	//DEBUGINFO ( "AddTargetDamageDealt" );

	iCurrentTypeAction = eTypeAction;

	iHitDealtAll++;

	if ( dwStartDamageAllTick == 0 )
	{
		dwStartDamageAllTick = TICKCOUNT;
	}

	if ( iCurrentTypeAction == EDamageTextType::Miss )
	{
		iMissCounter++;
	}



	if ( iValue > 0 )
	{

		if ( iCurrentTypeAction == EDamageTextType::Critical ) //crit
		{
			iCritCounter++;

			int calc = ( iValue * 100 ) / 170;
			viDamageDealtAll_NoCrit.push_back ( calc ); //back to normal
		}
		else
		{
			viDamageDealtAll_NoCrit.push_back( iValue );
		}

		viDamageDealtAll.push_back ( iValue );
	}
}

/// <summary>
/// Delay between start of first attack animation for a skill
/// </summary>
/// <param name="iTargetID"></param>
/// <param name="iDamageDealt"></param>
void CTestMapHandler::AddSkillAttackDelay ( enum ESkillID eSkillID, int iDelay )
{

}