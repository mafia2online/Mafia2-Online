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
	if( !pMessage )
		return false;

	if( !CCore::Instance()->GetPlayerManager() || !CCore::Instance()->GetPlayerManager()->GetLocalPlayer() )
		return false;

	CLocalPlayer * pLocalPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();
	CM2Ped * pLocalPed = pLocalPlayer->GetPlayerPed();

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
				pLocalPlayer->OnEnterVehicle();
				break;
			}

		case M2Enums::ON_VEHICLE_ENTER:
			{
				pLocalPlayer->OnEnterVehicleDone();
				break;
			}

		case M2Enums::ON_VEHICLE_EXIT:
			{
				pLocalPlayer->OnLeaveVehicle();
				break;
			}

		case M2Enums::ON_DEATH:
			{
				CNetworkPlayer * pKiller = CCore::Instance()->GetPlayerManager()->GetFromGameGUID( pMessage->M2HumanDeathMessage__dwKillerGUID );
				pLocalPlayer->OnDeath( pKiller );
				break;
			}
		}
		return true;
	}

	return false;
}
