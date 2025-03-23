#include "stdafx.h"
#include "DamageHandler.h"


CDamageHandler::CDamageHandler()
{
}


CDamageHandler::~CDamageHandler()
{
}

////JLM - TODO
void CDamageHandler::HandlePacket(PacketDamageQuickDelayed* psPacket)
{
	psPacket->dwClientTimeToExecute = TICKCOUNT * (psPacket->iMillisecondDelay);
}

//JLM - TODO
/*void CDamageHandler::Tick()
{
	//foreach (std::vector)
}*/

void CDamageHandler::HandlePacket( PacketDamageQuick * psPacket )
{
	if ( psPacket->iDamage <= 0 )
		return;

	int iDecrease;

	if ( psPacket->bPercentile == FALSE )
	{
		//Decrease HP
		iDecrease = psPacket->iDamage;
	}
	else   
	{
		if ( psPacket->bBaseCurrentHP )
		{
			int iCurHP = CHARACTERGAME->GetCurrentHP();

			//Based on Current HP
			iDecrease = (iCurHP * psPacket->iDamage) / 100;
		}
		else
		{
			int iMaxHP = CHARACTERGAME->GetCurrentHP();

			//Based on Max HP
			iDecrease = (iMaxHP * psPacket->iDamage) / 100;
		}
	}

	int iNewHP = CHARACTERGAME->GetCurrentHP() - iDecrease;

	if (iNewHP != CHARACTERGAME->GetCurrentHP())
	{
		if ( iNewHP > 0 )
		{
			CHARACTERGAME->SetCurrentHP( iNewHP );
			CHECK_CHARACTER_CHECKSUM;
		}
		else
		{
			//Die
			CHARACTERGAME->Die();
		}

		PacketQuickHealthUpdate l_HealthUpdate;
		l_HealthUpdate.iHeader	= PKTHDR_QuickHealthUpdate;
		l_HealthUpdate.iLength	= sizeof(PacketQuickHealthUpdate);
		l_HealthUpdate.dwID		= UNITDATA->iID;
        l_HealthUpdate.sHP		= CHARACTERGAME->GetCurrentHP();

		SOCKETG->SendPacket(&l_HealthUpdate, true);
	}
}
