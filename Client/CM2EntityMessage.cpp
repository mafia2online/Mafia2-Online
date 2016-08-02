/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2EntityMessage.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CChat.h"
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
			
		case M2Enums::ON_USE_DOOR:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage( "DOOR_OPEN (0x%p, %d, %d, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID, pMessage->m_dwUnknown1, pMessage->m_dwUnknown2 );
#endif
				break;
			}

		case M2Enums::ON_DOOR_KICK:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage( "DOOR_KICK (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID );
#endif
				break;
			}
		
		case M2Enums::ON_SHOOT:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
#ifdef DEBUG
					CCore::Instance()->GetChat()->AddDebugMessage("ON_SHOOT (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif	
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetTimeStartShooting( SharedUtility::GetTime() );
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetShooting( true );
				}
				break;
			}

		case M2Enums::ON_SHOT_HIT_ENTITY:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage("ON_SHOT_HIT_ENTITY (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
				break;
			}

		case M2Enums::ON_DAMAGE:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage("ON_DAMAGE (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
				break;
			}

		case M2Enums::ON_AIM_ENTER:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
#ifdef DEBUG
					CCore::Instance()->GetChat()->AddDebugMessage("ON_AIM_ENTER (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetAiming(true);
				}
				break;
			}

		case M2Enums::ON_AIM_LEAVE:
			{
				int selectedWeapon = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetSelectedWeapon();
				if (selectedWeapon != 0 && selectedWeapon != 1)
				{
#ifdef DEBUG
					CCore::Instance()->GetChat()->AddDebugMessage("ON_AIM_LEAVE (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
					CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetAiming(false);
				}
				break;
			}

		case M2Enums::CAR_BREAK_IN:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage("CAR_BREAK_IN (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
				break;
			}

		case M2Enums::ON_ACTION:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage("ON_ACTION (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
#endif
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

		default:
			{
#ifdef DEBUG
				CCore::Instance()->GetChat()->AddDebugMessage("Unhandled entity message %d", pMessage->m_dwMessage);
#endif
				break;
			}
		}
		return true;
	}

	return false;
}
