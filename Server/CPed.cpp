/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPed.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CPed::CPed ( int iModel, CVector3 vecPosition, CVector3 vecRotation )
{
	// Set the ped model
	SetModel ( iModel, false );
}

CPed::~CPed ( void )
{

}

void CPed::AddForPlayer ( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the ped id
	bitStream.WriteCompressed ( m_pedId );

	// Write the ped model
	bitStream.Write ( m_iModel );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_NEWPED, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CPed::AddForWorld ( void )
{
	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Add the ped for this player
			AddForPlayer( i );
		}
	}
}

void CPed::RemoveForPlayer ( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the ped id
	bitStream.WriteCompressed ( m_pedId );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_REMOVEPED, &bitStream, HIGH_PRIORITY, RELIABLE, playerId, false );
}

void CPed::RemoveForWorld ( void )
{
	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Remove the ped for this player
			RemoveForPlayer( i );
		}
	}
}

void CPed::SetModel ( int iModel, bool bSendToClient )
{
	// Store the model
	m_iModel = iModel;

	// Are we not sending this to the client?
	if ( !bSendToClient )
		return;

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the ped id
	bitStream.WriteCompressed ( m_pedId );

	// Write the new model id
	bitStream.Write ( iModel );

	// Send it to the server
	pCore->GetNetworkModule()->Call( RPC_SETPEDMODEL, &bitStream, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, true );
}