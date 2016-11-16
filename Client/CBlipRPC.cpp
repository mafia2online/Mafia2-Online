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

#include <RPC4Plugin.h>
#include "Network/NetRPCs.h"

#include "CCore.h"

bool	CBlipRPC::m_bRegistered = false;

void AttachBlipToPlayer ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	EntityId playerId;
	pBitStream->ReadCompressed ( playerId );


	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		CNetworkPlayer * pNetworkPlayer = NULL;
		if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() == playerId)
			pNetworkPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();
		else
			pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

		CBlip * pBlip = CCore::Instance()->GetBlipManager()->GetServerBlip(blipId);

		if (pBlip && pNetworkPlayer) {
			pBlip->AttachToPlayer(pNetworkPlayer);
		}
	}
}

void AttachBlipToVehicle ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	EntityId vehicleId;
	pBitStream->ReadCompressed ( vehicleId );

	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		if ( CCore::Instance()->GetVehicleManager()->IsActive ( vehicleId ) )
		{
			CCore::Instance()->GetBlipManager()->GetServerBlip ( blipId )->AttachToVehicle ( CCore::Instance()->GetVehicleManager()->Get( vehicleId ) );
		}
	}
}

void AttachBlipToPed ( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	EntityId blipId;
	pBitStream->ReadCompressed ( blipId );

	EntityId pedId;
	pBitStream->ReadCompressed ( pedId );

	if ( CCore::Instance()->GetBlipManager()->IsServerBlipActive ( blipId ) )
	{
		if ( CCore::Instance()->GetPedManager()->IsActive ( pedId ) )
		{
			CCore::Instance()->GetBlipManager()->GetServerBlip ( blipId )->AttachToPed ( CCore::Instance()->GetPedManager()->Get( pedId ) );
		}
	}
}

void CBlipRPC::Register( RakNet::RPC4 * pRPC )
{
	if( m_bRegistered )
		return;

	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOPLAYER, AttachBlipToPlayer );
	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOVEHICLE, AttachBlipToVehicle );
	pRPC->RegisterFunction ( RPC_ATTACHBLIPTOPED, AttachBlipToPed );
}

void CBlipRPC::Unregister( RakNet::RPC4 * pRPC )
{
	if( !m_bRegistered )
		return;

	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOPLAYER );
	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOVEHICLE );
	pRPC->UnregisterFunction ( RPC_ATTACHBLIPTOPED );
}