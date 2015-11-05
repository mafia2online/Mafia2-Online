/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMouseHandler.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

/*
void CMouseHandler::HandleMouseEvent( eMouseType mouseEvent, BYTE bKey )
{
	// Are we not connected?
	if( !pCore->GetNetworkModule() || !pCore->GetNetworkModule()->IsConnected() )
		return;

	// Is the localplayer not spawned?
	if( !pCore->GetPlayerManager() || !pCore->GetPlayerManager()->GetLocalPlayer() || !pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	// Get the localplayer pointer
	CLocalPlayer * pLocalPlayer = pCore->GetPlayerManager()->GetLocalPlayer();

	// Was this the up event?
	if( mouseEvent == eMouseEvent::MOUSE_CLICK_UP )
	{
		// Was this the shot button?
		if( bKey == eMouseButton::MOUSE_BUTTON_LEFT )
		{
			// Are we flagged as shooting?
			if( pLocalPlayer->IsShooting() )
			{
				// Flag as not shooting
				pLocalPlayer->SetShooting( false );
			}
		}
	}
}
*/