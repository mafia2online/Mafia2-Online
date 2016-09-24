/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CBlipRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CBlipRPC.h"

#include "BitStream.h"

#include "CBlipManager.h"

#include "CPlayerManager.h"
#include "CNetworkPlayer.h"
#include "CRemotePlayer.h"
#include "CLocalPlayer.h"

#include "CNetworkVehicle.h"
#include "CVehicleManager.h"

#include "CPed.h"
#include "CPedManager.h"

#include	<RPC4Plugin.h>
#include "Network/NetRPCs.h"

#include "CCore.h"

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
	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Get the player instance
		CNetworkPlayer * pNetworkPlayer = NULL;

		// Is this player the localplayer?
		if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() == playerId)
			pNetworkPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();
		else
			pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

		// Get the blip instance
		CBlip * pBlip = CCore::Instance()->GetBlipManager()->GetServerBlip(blipId);

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
	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Is the vehicle instance valid?
		if ( CCore::Instance()->GetVehicleManager()->IsActive ( vehicleId ) )
		{
			// Attach the blip to the vehicle
			CCore::Instance()->GetBlipManager()->GetServerBlip ( blipId )->AttachToVehicle ( CCore::Instance()->GetVehicleManager()->Get( vehicleId ) );
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
	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		// Is the ped instance valid?
		if ( CCore::Instance()->GetPedManager()->IsActive ( pedId ) )
		{
			// Attach the blip to the ped
			CCore::Instance()->GetBlipManager()->GetServerBlip ( blipId )->AttachToPed ( CCore::Instance()->GetPedManager()->Get( pedId ) );
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