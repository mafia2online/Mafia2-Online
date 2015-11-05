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

// Enums:
//	852007 - CAR_BREAK_IN
//	852055 - DOOR_OPEN
//	852054 - DOOR_KICK

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

#ifdef _DEBUG
	//	pCore->GetChat()->AddDebugMessage( "CM2EntityMessage::HandleEntityEvent( %d ) - Sender: %d (LocalPlayer: %s), Receiver: %d (LocalPlayer: %s)", pMessage->m_dwMessage, pMessage->m_dwSenderGUID, (pMessage->m_dwSenderGUID == pLocalPed->GetGUID() ? "Yes" : "No"), pMessage->m_dwReceiveGUID, (pMessage->m_dwReceiveGUID == pLocalPed->GetGUID() ? "Yes" : "No") );
#endif

	// Is this event for the localplayer?
	if( pMessage->m_dwReceiveGUID == pLocalPed->GetGUID() )
	{
		switch( pMessage->m_dwMessage )
		{
			/*
		case 852055:
			{
				pCore->GetChat()->AddDebugMessage( "DOOR_OPEN (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID );
				break;
			}

		case 852054:
			{
				pCore->GetChat()->AddDebugMessage( "DOOR_KICK (0x%p, %d, %d)", pMessage, pMessage->m_dwSenderGUID, pMessage->m_dwReceiveGUID );
				break;
			}
			*/

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
				// Find the killer from the ped guid
				CNetworkPlayer * pKiller = pCore->GetPlayerManager()->GetFromGameGUID( pMessage->M2HumanDeathMessage__dwKillerGUID );

				// Call the event
				pLocalPlayer->OnDeath( pKiller );
				break;
			}
		}
		return true;
	}

	return false;
}