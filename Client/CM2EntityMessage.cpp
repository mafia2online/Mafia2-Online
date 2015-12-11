/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2EntityMessage.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CM2Enums.h"

extern	CCore		* pCore;

bool CM2EntityMessage::HandleEntityEvent( M2EntityMessage * pMessage )
{
	// Is the event invalid?
	if( !pMessage )
		return false;

	// Is the localplayer instance invalid?
	if( !pCore->GetPlayerManager() || !pCore->GetPlayerManager()->GetLocalPlayer() )
		return false;

	// Get the localplayer
	CLocalPlayer * pLocalPlayer = pCore->GetPlayerManager()->GetLocalPlayer();

	// Get the localplayer ped
	CM2Ped * pLocalPed = pLocalPlayer->GetPlayerPed();

	// Is this event for the localplayer?
	if( pMessage->m_dwReceiveGUID == pLocalPed->GetGUID() )
	{
		switch( pMessage->m_dwMessage )
		{
			
		case M2Enums::ON_USE_DOOR:
			{
				pCore->GetChat()->AddDebugMessage( "DOOR_OPEN (0x%p, %d, %d, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID, pMessage->m_dwUnknown1, pMessage->m_dwUnknown2 );
				break;
			}

		case M2Enums::ON_DOOR_KICK:
			{
				pCore->GetChat()->AddDebugMessage( "DOOR_KICK (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID );
				break;
			}
		
		case M2Enums::ON_SHOOT:
			{
				pCore->GetChat()->AddDebugMessage("ON_SHOOT (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				pCore->GetPlayerManager()->GetLocalPlayer()->SetShooting(true);
				break;
			}

		case M2Enums::ON_SHOT_HIT_ENTITY:
			{
				pCore->GetChat()->AddDebugMessage("ON_SHOT_HIT_ENTITY (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				break;
			}

		case M2Enums::ON_DAMAGE:
			{
				pCore->GetChat()->AddDebugMessage("ON_DAMAGE (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				break;
			}

		case M2Enums::ON_AIM_ENTER:
			{
				pCore->GetChat()->AddDebugMessage("ON_AIM_ENTER (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				pCore->GetPlayerManager()->GetLocalPlayer()->SetAiming(true);
				break;
			}

		case M2Enums::ON_AIM_LEAVE:
			{
				pCore->GetChat()->AddDebugMessage("ON_AIM_LEAVE (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				pCore->GetPlayerManager()->GetLocalPlayer()->SetAiming(false);
				break;
			}

		case M2Enums::CAR_BREAK_IN:
			{
				pCore->GetChat()->AddDebugMessage("CAR_BREAK_IN (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
				break;
			}

		case M2Enums::ON_ACTION:
		{
			pCore->GetChat()->AddDebugMessage("ON_ACTION (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID);
			break;
		}

		default:
		{
			pCore->GetChat()->AddDebugMessage("Unhandled entity message %d", pMessage->m_dwMessage);
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
				CNetworkPlayer * pKiller = pCore->GetPlayerManager()->GetFromGameGUID( pMessage->M2HumanDeathMessage__dwKillerGUID );

				// Call our event
				pLocalPlayer->OnDeath( pKiller );
				break;
			}
		}
		return true;
	}

	return false;
}