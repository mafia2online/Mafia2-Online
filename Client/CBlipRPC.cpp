/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CBlipRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CBlipRPC.h"

extern	CCore			* pCore;
bool	CBlipRPC::m_bRegistered = false;

void AttachBlipToPlayer ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the blip id
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed ( playerId );

	// Is the blip instance valid?
	if ( pCore->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Get the player instance
		CNetworkPlayer * pNetworkPlayer = NULL;

		// Is this player the localplayer?
		if ( pCore->GetPlayerManager()->GetLocalPlayer()->GetId() == playerId )
			pNetworkPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
		else
			pNetworkPlayer = pCore->GetPlayerManager()->Get( playerId );

		// Get the blip instance
		CBlip * pBlip = pCore->GetBlipManager()->GetServerBlip( blipId );

		// Attach the blip to the player
		if ( pBlip && pNetworkPlayer )
			pBlip->AttachToPlayer ( pNetworkPlayer );
	}
}

void AttachBlipToVehicle ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the blip id
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed ( vehicleId );

	// Is the blip instance valid?
	if ( pCore->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Is the vehicle instance valid?
		if ( pCore->GetVehicleManager()->IsActive ( vehicleId ) )
		{
			// Attach the blip to the vehicle
			pCore->GetBlipManager()->GetServerBlip ( blipId )->AttachToVehicle ( pCore->GetVehicleManager()->Get( vehicleId ) );
		}
	}
}

void AttachBlipToPed ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the blip id
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	// Read the ped id
	EntityId pedId;
	pBitStream->ReadCompressed ( pedId );

	// Is the blip instance valid?
	if ( pCore->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Is the ped instance valid?
		if ( pCore->GetPedManager()->IsActive ( pedId ) )
		{
			// Attach the blip to the ped
			pCore->GetBlipManager()->GetServerBlip ( blipId )->AttachToPed ( pCore->GetPedManager()->Get( pedId ) );
		}
	}
}

void CBlipRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Blip functions
	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOPLAYER, AttachBlipToPlayer );
	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOVEHICLE, AttachBlipToVehicle );
	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOPED, AttachBlipToPed );
}

void CBlipRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Blip functions
	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOPLAYER );
	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOVEHICLE );
	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOPED );
}