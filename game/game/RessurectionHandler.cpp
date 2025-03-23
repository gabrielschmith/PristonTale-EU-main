#include "stdafx.h"
#include "RessurectionHandler.h"

CRessurectionHandler::CRessurectionHandler()
{
}

CRessurectionHandler::~CRessurectionHandler()
{
}

BOOL CRessurectionHandler::OnUseSkill( UnitData * pcUnitData )
{
	static int iaTargets[10]
	{
		3,3,3,4,4,4,5,5,5,6
	};

	int iCount = 0;

	int iMax = iaTargets[UNITDATA->sActiveSkill.cLevel - 1];

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		if ( pc->bActive && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player )
		{

			if ( CHARACTERGAME->GetStageID() == MAPID_BlessCastle )
			{
				if ( UNITDATA->iClanID != 0 && UNITDATA->iClanID != pc->iClanID )
					continue;
			}

			if ( pc->IsDead() )
			{
				int iX = (pcUnitData->sPosition.iX - pc->sPosition.iX) >> 8;
				int iZ = (pcUnitData->sPosition.iZ - pc->sPosition.iZ) >> 8;


				int iDistance = (iX * iX) + (iZ * iZ);

				if ( iDistance < PRIESTRESS_RESSURECTION_RANGE_SQ)
				{
					CALL_WITH_ARG4( 0x00623790, 0x3B, UNITDATA->sActiveSkill.cLevel, pc->iID, 0 );

					iCount++;

					if ( iCount == iMax )
						break;
				}
			}
		}
	}

	return TRUE;
}

BOOL CRessurectionHandler::OnSkillCasted(PacketSkillCast* psPacket)
{
    if (UNITDATA->psModelAnimation && UNITDATA->psModelAnimation->iType == EAnimationType::ANIMATIONTYPE_Die)
	{
		UNITDATA->Animate(EAnimationType::ANIMATIONTYPE_Revive);		//start motion
        SKILLMANAGERHANDLER->StartSkillEffectOld(UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, EFFECT_GAME_START1);
		SKILLMANAGERHANDLER->SetDynLight(UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ, 100, 100, 100, 0, 200);
		PlayUnitSound(UNITDATA);
		
		UNITDATA->SetCurrentHealth(UNITDATA->GetMaxHealth() / 2);
		UNITDATA->ResetEnergyGraph(4);								//Energy graph check reset
		UNITDATA->bWalking = FALSE;
		RESTARTCOUNTER = 350;								//Invincible for 5 seconds
		RESTARTFLAG = 0;

        return TRUE;	///< Skip ASM handling
    }

	return FALSE;
}