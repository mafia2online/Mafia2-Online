/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2EntityMessage.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CM2EntityMessage.h"
#include "CM2Enums.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"

#include "CNetworkPlayer.h"
#include "CRemotePlayer.h"
#include "CLocalPlayer.h"
#include "CPlayerManager.h"

#include "SharedUtility.h"

bool CM2EntityMessage::HandleEntityEvent( M2EntityMessage * pMessage )
{
	// Is the event invalid?
	if( !pMessage )
		return false;

	// Is the localplayer instance invalid?
	if( !CCore::Instance()->GetPlayerManager() || !CCore::Instance()->GetPlayerManager()->GetLocalPlayer() )
		return false;

	// Get the localplayer
	CLocalPlayer * pLocalPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();

	// Get the localplayer ped
	CM2Ped * pLocalPed = pLocalPlayer->GetPlayerPed();

	// Is this event for the localplayer?
	if( pMessage->m_dwReceiveGUID == pLocalPed->GetGUID() )
	{
		switch( pMessage->m_dwMessage )
		{

		case M2Enums::ON_SHOOT:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetTimeStartShooting( SharedUtility::GetTime() );
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetShooting( true );
				}
				break;
			}

		case M2Enums::ON_AIM_ENTER:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetAiming(true);
				}
				break;
			}

		case M2Enums::ON_AIM_LEAVE:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetAiming(false);
				}
				break;
			}

		case M2Enums::ON_VEHICLE_START_ENTER:
			{
				// Call our event
				pLocalPlayer->OnEnterVehicle();
				break;
			}

		case M2Enums::ON_VEHICLE_ENTER:
			{
				// Call our event
				pLocalPlayer->OnEnterVehicleDone();
				break;
			}

		case M2Enums::ON_VEHICLE_EXIT:
			{
				// Call our event
				pLocalPlayer->OnLeaveVehicle();
				break;
			}

		case M2Enums::ON_DEATH:
			{
				// Find the killer from the ped guid
				CNetworkPlayer * pKiller = CCore::Instance()->GetPlayerManager()->GetFromGameGUID( pMessage->M2HumanDeathMessage__dwKillerGUID );

				// Call our event
				pLocalPlayer->OnDeath( pKiller );
				break;
			}
		}
		return true;
	}

	return false;
}
