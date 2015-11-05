/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlip.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CBlip::CBlip( EntityId blipId, Vector2 vecPosition, int iLibrary, int iIcon )
{
	// Save the variables
	m_blipId = blipId;
	m_vecPosition = vecPosition;
	m_iLibrary = iLibrary;
	m_iIcon = iIcon;

	// Reset
	m_pAttachedToPlayer = NULL;
	m_pAttachedToVehicle = NULL;
	m_pAttachedToPed = NULL;
	m_blipType = eBlipType::BLIP_TYPE_STANDALONE;
}

CBlip::~CBlip( void )
{
}

void CBlip::AddForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the icon id
	pBitStream.WriteCompressed( m_blipId );

	// Write the x coordinate
	pBitStream.Write( m_vecPosition.fX );

	// Write the y coordinate
	pBitStream.Write( m_vecPosition.fY );

	// Write the library id
	pBitStream.WriteCompressed( m_iLibrary );

	// Write the icon id
	pBitStream.WriteCompressed( m_iIcon );

	// Write the blip type
	pBitStream.WriteCompressed ( (int)m_blipType );

	// Write the id of the attached entity
	if ( m_pAttachedToPlayer )
		pBitStream.WriteCompressed ( m_pAttachedToPlayer->GetId() );
	else if ( m_pAttachedToVehicle )
		pBitStream.WriteCompressed ( m_pAttachedToVehicle->GetId() );
	else if ( m_pAttachedToPed )
		pBitStream.WriteCompressed ( m_pAttachedToPed->GetId() );
	else
		pBitStream.WriteCompressed ( INVALID_ENTITY_ID );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_NEWBLIP, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CBlip::AddForWorld( void )
{
	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Add the blip for this player
			AddForPlayer( i );
		}
	}
}

void CBlip::RemoveForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the icon id
	pBitStream.WriteCompressed( m_blipId );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_REMOVEBLIP, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false );
}

void CBlip::RemoveForWorld( void )
{
	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Remove the blip for this player
			RemoveForPlayer( i );
		}
	}
}

void CBlip::AttachToPlayer ( CNetworkPlayer * pPlayer )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) && i != pPlayer->GetId() )
		{
			// Reset the bitstream
			bitStream.Reset();

			// Write the blip id
			bitStream.WriteCompressed ( m_blipId );

			// Write the player id
			bitStream.WriteCompressed ( pPlayer->GetId () );

			// Send the bitstream to the player
			pCore->GetNetworkModule()->Call ( RPC_ATTACHBLIPTOPLAYER, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, i, false );
		}
	}

	// Store the attached player
	m_pAttachedToPlayer = pPlayer;
	m_pAttachedToVehicle = NULL;
	m_pAttachedToPed = NULL;

	// Set the blip type
	m_blipType = eBlipType::BLIP_TYPE_PLAYER;
}

void CBlip::AttachToVehicle ( CNetworkVehicle * pVehicle )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Reset the bitstream
			bitStream.Reset();

			// Write the blip id
			bitStream.WriteCompressed ( m_blipId );

			// Write the vehicle id
			bitStream.WriteCompressed ( pVehicle->GetId () );

			// Send the bitstream to the player
			pCore->GetNetworkModule()->Call ( RPC_ATTACHBLIPTOVEHICLE, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, i, false );
		}
	}

	// Store the attached player
	m_pAttachedToPlayer = NULL;
	m_pAttachedToVehicle = pVehicle;
	m_pAttachedToPed = NULL;

	// Set the blip type
	m_blipType = eBlipType::BLIP_TYPE_VEHICLE;
}

void CBlip::AttachToPed ( CPed * pPed )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Reset the bitstream
			bitStream.Reset();

			// Write the blip id
			bitStream.WriteCompressed ( m_blipId );

			// Write the ped id
			bitStream.WriteCompressed ( pPed->GetId () );

			// Send the bitstream to the player
			pCore->GetNetworkModule()->Call ( RPC_ATTACHBLIPTOPED, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, i, false );
		}
	}

	// Store the attached player
	m_pAttachedToPlayer = NULL;
	m_pAttachedToVehicle = NULL;
	m_pAttachedToPed = pPed;

	// Set the blip type
	m_blipType = eBlipType::BLIP_TYPE_PED;
}