/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CStreamer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

extern	CCore				* pCore;

CStreamer::CStreamer( void )
{
	// Reset
	m_ulLastStreamTime = 0;
}

CStreamer::~CStreamer( void )
{
	// Clear all current entities
	clear();
}

void CStreamer::Process( void )
{
	// Are we not connected to a network?
	if( pCore->GetNetworkModule() && !pCore->GetNetworkModule()->IsConnected() )
		return;

	// Get the current time
	unsigned long ulCurrentTime = SharedUtility::GetTime();

	// Has enough time passed?
	if( (ulCurrentTime - m_ulLastStreamTime) > STREAMER_TICKRATE )
	{
		// Store the last streamed time
		m_ulLastStreamTime = ulCurrentTime;

		// Get the localplayer position
		CVector3 vecLocalPos;
		pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecLocalPos );

		// Loop through all streamable entities
		for( iterator iter = begin(); iter != end(); iter++ )
		{
			// Is the current streamable entity invalid?
			if( !(*iter)->GetEntity() )
				continue;

			//
			bool bIsInRange = false;
			float fDistance = 0.0f;

			// Get the current entity position
			CVector3 vecPos;
			(*iter)->GetEntity()->GetPosition( &vecPos );

			// Is the current entity in range?
			bIsInRange = ((vecLocalPos - vecPos).Length() <= (*iter)->GetStreamDistance());

			// Is the current entity in range of the localplayer and not streamed in?
			if( bIsInRange && !(*iter)->IsStreamedIn() )
			{
				// Stream in
				(*iter)->StreamIn();
			}
			// Are they not in range, but still streamed in?
			else if( !bIsInRange && (*iter)->IsStreamedIn() )
			{
				// Stream out
				(*iter)->StreamOut();
			}
		}
	}
}