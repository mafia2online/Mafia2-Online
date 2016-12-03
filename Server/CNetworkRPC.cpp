/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"
#include "CCore.h"

bool	CNetworkRPC::m_bRegistered = false;
RakNet::BitStream		bsReject;

void InitialData( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the player version
	int iVersion;
	pBitStream->Read( iVersion );

	// Read the player name
	RakNet::RakString strName;
	pBitStream->Read( strName );

	// Read the player serial
	RakNet::RakString strSerial;
	pBitStream->Read( strSerial );

	// Is the network version invalid?
	if( iVersion != NETWORK_VERSION )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( playerId );
		pArguments.push( REJECT_REASON_VERSION );

		bool bOutputDefaultMessage = (CCore::Instance()->GetEvents()->Call( "onPlayerConnectionRejected", &pArguments ).GetInteger() == 1);

		// Reset the bitstream
		bsReject.Reset();

		// Write the reason
		bsReject.Write( (BYTE)REJECT_REASON_VERSION );

		// Write if we should output default messages
		bOutputDefaultMessage ? bsReject.Write1() : bsReject.Write0();

		// Send it back to the client
		CCore::Instance()->GetNetworkModule()->Call( RPC_CONNECTION_REJECTED, &bsReject, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );

		CLogFile::Printf( "[rejected] %s was rejected connection to the server. (Invalid network version - %d, %d)", strName.C_String(), iVersion, NETWORK_VERSION );
		return;
	}

	// Is the nickname already in use?
	if( CCore::Instance()->GetPlayerManager()->IsNickInUse( strName ) )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( playerId );
		pArguments.push( REJECT_REASON_NICKNAME );

		bool bOutputDefaultMessage = (CCore::Instance()->GetEvents()->Call( "onPlayerConnectionRejected", &pArguments ).GetInteger() == 1);

		// Reset the bitstream
		bsReject.Reset( );

		// Write the reason
		bsReject.Write( (BYTE)REJECT_REASON_NICKNAME );

		// Write if we should output default messages
		bOutputDefaultMessage ? bsReject.Write1() : bsReject.Write0();

		// Send it back to the client
		CCore::Instance()->GetNetworkModule()->Call( RPC_CONNECTION_REJECTED, &bsReject, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );

		CLogFile::Printf( "[rejected] %s was rejected connection to the server. (Nickname already in use)", strName.C_String() );
		return;
	}

	// Is the player banned?
	if( CCore::Instance()->GetBanManager()->IsSerialBanned( strSerial ) )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( playerId );
		pArguments.push( REJECT_REASON_BANNED );

		bool bOutputDefaultMessage = (CCore::Instance()->GetEvents()->Call( "onPlayerConnectionRejected", &pArguments ).GetInteger() == 1);

		// Reset the bitstream
		bsReject.Reset( );

		// Write the reason
		bsReject.Write( (BYTE)REJECT_REASON_BANNED );

		// Write if we should output default messages
		bOutputDefaultMessage ? bsReject.Write1() : bsReject.Write0();

		// Send it back to the client
		CCore::Instance()->GetNetworkModule()->Call( RPC_CONNECTION_REJECTED, &bsReject, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );

		CLogFile::Printf( "[rejected] %s was rejected connection to the server. (Banned)", strName.C_String() );
		return;
	}

	// Add the player to the manager
	if( !CCore::Instance()->GetPlayerManager()->Add( playerId, strName.C_String(), pPacket->systemAddress.ToString ( false ), strSerial.C_String() ) )
	{
		CLogFile::Printf ( "WARNING - FAILED TO ADD PLAYER '%s' TO PLAYER MANAGER.", strName.C_String() );
		return;
	}

	// Add everyone else connected for this player
	CCore::Instance()->GetPlayerManager()->HandlePlayerJoin( playerId );

	// Handle this player with the vehicle manager
	CCore::Instance()->GetVehicleManager()->HandlePlayerJoin( playerId );

	// Handle this with the blip manager
	CCore::Instance()->GetBlipManager()->HandlePlayerJoin ( playerId );

	// Handle this player with the client scripting manager
	CCore::Instance()->GetClientScriptingManager()->HandlePlayerJoin( playerId );

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( playerId );

	// Write the player colour
	bitStream.Write( CCore::Instance()->GetPlayerManager()->Get( playerId )->GetColour() );

	// Write the server name
	bitStream.Write( RakNet::RakString( CVAR_GET_STRING( "hostname" ) ) );

	// Write the max player count
	bitStream.Write( CVAR_GET_INTEGER( "maxplayers" ) );

	// Write the http server address
	bitStream.Write( RakNet::RakString( CVAR_GET_STRING( "httpserver" ) ) );

	// Write the file transfer port
	bitStream.Write( (CVAR_GET_INTEGER( "port" ) + 1) ); // todo: REMOVE

	// Write a bit to represent the server season
	CCore::Instance()->IsSummer() ? bitStream.Write1() : bitStream.Write0();

	// Write the weather name
	bitStream.Write( RakNet::RakString ( CCore::Instance()->GetWeather () ) );

	// Send it back to the player
	CCore::Instance()->GetNetworkModule()->Call( RPC_INITIAL_DATA, &bitStream, HIGH_PRIORITY, RELIABLE, playerId, false );
}

void PlayerChat( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read if this is a command
	bool bIsCommand = pBitStream->ReadBit();

	// Read the input
	RakNet::RakString strInput;
	pBitStream->Read( strInput );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get a pointer to the player
		CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

		// Is the pointer valid?
		if( pNetworkPlayer )
		{
			// Is this not a command?
			if( !bIsCommand )
			{
				// Call the event
				CSquirrelArguments pArguments;
				pArguments.push( playerId );
				pArguments.push( strInput );

				//
				bool bSend = (CCore::Instance()->GetEvents()->Call( "onPlayerChat", &pArguments ).GetInteger() == 1);

				// Should we send this bitstream to players?
				if ( bSend )
				{
					CLogFile::Printf( "[chat] %s: %s", pNetworkPlayer->GetNick(), strInput.C_String() );

					// Send the RPC back to other players
					RakNet::BitStream bitStream;
					bitStream.WriteCompressed( playerId );
					bitStream.Write( strInput );
					CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYER_CHAT, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
				}
			}
			else
			{
				// Call the command handler
				CCore::Instance()->GetCommands()->HandleCommand( playerId, strInput.C_String() );
			}
		}
	}
}

void PlayerSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the player sync data
	OnFootSync onFootSync;
	pBitStream->Read( (char *)&onFootSync, sizeof(OnFootSync) );

	RakNet::RakString strAnimStyleName;
	pBitStream->Read(strAnimStyleName);

	RakNet::RakString strAnimStyleDirectory;
	pBitStream->Read(strAnimStyleDirectory);

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Is the player pointer valid?
	if( pNetworkPlayer )
	{
		pNetworkPlayer->SetAnimStyle(strAnimStyleDirectory, strAnimStyleName);

		// Store the sync data
		pNetworkPlayer->StoreOnFootSync( onFootSync );
	}
}

void PlayerDeath( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the killer id
	EntityId killerId;
	pBitStream->ReadCompressed( killerId );

	// Get a pointer to the player
	CNetworkPlayer * pPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the player pointer valid?
	if( pPlayer )
	{
		// Kill the player
		pPlayer->KillForWorld();

		// Is the killer a player?
		if( killerId != INVALID_ENTITY_ID )
		{
			// Find the killer
			CNetworkPlayer * pKiller = CCore::Instance()->GetPlayerManager()->Get(killerId);

			// Is the killer valid?
			if( pKiller )
				CLogFile::Printf( "[death] %s has been killed by %s!", pPlayer->GetNick(), pKiller->GetNick() );
			else
				CLogFile::Printf( "[death] %s has been killed by NOT_A_PLAYER!", pPlayer->GetNick() );
		}
		else
		{
			CLogFile::Printf( "[death] %s has died!", pPlayer->GetNick() );
		}

		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( playerId );
		pArguments.push( killerId );
		CCore::Instance()->GetEvents()->Call( "onPlayerDeath", &pArguments );
	}
}

void PlayerSpawn( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	if( pNetworkPlayer )
	{
		pNetworkPlayer->SpawnForWorld();
		CCore::Instance()->GetPlayerManager()->HandlePlayerSpawn( playerId );

		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		CCore::Instance()->GetEvents()->Call("onPlayerSpawn", &pArguments);

		CLogFile::Printf("[spawn] %s has spawned.", pNetworkPlayer->GetNick());

		CCore::Instance()->GetVehicleManager()->HandlePlayerSpawn( playerId );
	}
}

void PlayerRespawn( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the player pointer valid?
	if( pNetworkPlayer )
	{
	}
}

void PlayerChangeNick( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the new nickname
	RakNet::RakString sNewNick;
	pBitStream->Read( sNewNick );

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the player pointer valid?
	if( pNetworkPlayer )
	{
		// If new kick is not already took
		if( !CCore::Instance()->GetPlayerManager()->IsNickInUse( sNewNick.C_String() ) ){
			// Change the player nick
			pNetworkPlayer->ChangeNick( sNewNick.C_String() );
		}
	}
}

void VehicleSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the sync packet
	InVehicleSync vehicleSync;
	pBitStream->Read( (char *)&vehicleSync, sizeof(InVehicleSync) );

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Get a pointer to the vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle pointer valid?
	if( pNetworkPlayer && pNetworkVehicle )
	{
		// Process the sync data with the player
		pNetworkPlayer->StoreInVehicleSync( vehicleSync );

		// Store the sync data with the vehicle
		pNetworkVehicle->SetSyncData( vehicleSync );
	}
}

void PassengerSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the passenger sync data
	InPassengerSync passengerSync;
	pBitStream->Read( (char *)&passengerSync, sizeof(InPassengerSync) );

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Is the player pointer valid?
	if( pNetworkPlayer )
	{
		// Store the passenger sync
		pNetworkPlayer->StorePassengerSync( passengerSync );
	}
}

void UnoccupiedVehicleSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Get a pointer to the vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Process the unoccupied sync packet with the vehicle
		pNetworkVehicle->ProcessUnoccupiedSync( pBitStream );
	}
}

void VehicleEnter( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the seat
	EntityId seatId;
	pBitStream->ReadCompressed( seatId );

	// Get the player instance
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Is the player instance valid?
	if( pNetworkPlayer )
	{
		// Handle this with the player
		pNetworkPlayer->HandleVehicleEnter( vehicleId, seatId );
	}
}

void VehicleExit( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the seat
	EntityId seatId;
	pBitStream->ReadCompressed( seatId );

	// Read if the player is quickly exiting
	bool bExitFast = pBitStream->ReadBit();

	// Get the player instance
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Is the player instance valid?
	if( pNetworkPlayer )
	{
		// Handle this with the player
		pNetworkPlayer->HandleVehicleExit( vehicleId, seatId, bExitFast );
	}
}

void VehicleRespawn( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Get a pointer to the vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Respawn the vehicle for the world
		pNetworkVehicle->RespawnForWorld();
	}
}

void VehicleEnterDone( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Get a pointer to the player
	CNetworkPlayer * pNetworkPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );

	// Is the vehicle pointer valid?
	if( pNetworkPlayer )
	{
		// Handle this with the player
		pNetworkPlayer->HandleVehicleEnterDone ();
	}
}

void TriggerEvent( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	CSquirrelArguments * pArgs = new CSquirrelArguments( pBitStream );
	CSquirrelArgument * pEventName = (pArgs->size() > 0 ? pArgs->front() : 0);

	//
	if( pEventName && pEventName->GetType() == OT_STRING )
	{
		String strEventName = *pEventName->data.str;
		pArgs->pop_front();
		pArgs->push_front( new CSquirrelArgument( (int)pPacket->systemAddress.systemIndex ) );

		CCore::Instance()->GetEvents()->Call( strEventName, pArgs );
		SAFE_DELETE( pEventName );
	}

	SAFE_DELETE( pArgs );
}

void PlayerPing( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Get the network player instance
	CNetworkPlayer * pPlayer = CCore::Instance()->GetPlayerManager()->Get ( playerId );

	// Is the player instance valid?
	if ( pPlayer )
	{
		// Ping the player
		pPlayer->Ping ();
	}
}

void CNetworkRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Default rpcs
	pRPC->RegisterFunction( RPC_INITIAL_DATA, InitialData );

	// Player rpcs
	pRPC->RegisterFunction( RPC_PLAYER_CHAT, PlayerChat );
	pRPC->RegisterFunction( RPC_PLAYER_SYNC, PlayerSync );
	pRPC->RegisterFunction( RPC_PLAYER_DEATH, PlayerDeath );
	pRPC->RegisterFunction( RPC_PLAYER_SPAWN, PlayerSpawn );
	pRPC->RegisterFunction( RPC_PLAYER_RESPAWN, PlayerRespawn );
	pRPC->RegisterFunction( RPC_PLAYERCHANGENICK, PlayerChangeNick );
	pRPC->RegisterFunction( RPC_PLAYERPING, PlayerPing );

	// Vehicle rpcs
	pRPC->RegisterFunction( RPC_VEHICLE_SYNC, VehicleSync );
	pRPC->RegisterFunction( RPC_PASSENGER_SYNC, PassengerSync );
	pRPC->RegisterFunction( RPC_UNOCCUPIED_SYNC, UnoccupiedVehicleSync );
	pRPC->RegisterFunction( RPC_ENTER_VEHICLE, VehicleEnter );
	pRPC->RegisterFunction( RPC_EXIT_VEHICLE, VehicleExit );
	pRPC->RegisterFunction( RPC_RESPAWNVEHICLE, VehicleRespawn );
	pRPC->RegisterFunction( RPC_ENTER_VEHICLE_DONE, VehicleEnterDone );

	// Events
	pRPC->RegisterFunction( RPC_TRIGGEREVENT, TriggerEvent );
}

void CNetworkRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Default rpcs
	pRPC->UnregisterFunction( RPC_INITIAL_DATA );

	// Player rpcs
	pRPC->UnregisterFunction( RPC_PLAYER_CHAT );
	pRPC->UnregisterFunction( RPC_PLAYER_SYNC );
	pRPC->UnregisterFunction( RPC_PLAYER_DEATH );
	pRPC->UnregisterFunction( RPC_PLAYER_SPAWN );
	pRPC->UnregisterFunction( RPC_PLAYER_RESPAWN );
	pRPC->UnregisterFunction( RPC_PLAYERCHANGENICK );
	pRPC->UnregisterFunction( RPC_PLAYERPING );

	// Vehicle rpcs
	pRPC->UnregisterFunction( RPC_VEHICLE_SYNC );
	pRPC->UnregisterFunction( RPC_PASSENGER_SYNC );
	pRPC->UnregisterFunction( RPC_UNOCCUPIED_SYNC );
	pRPC->UnregisterFunction( RPC_ENTER_VEHICLE );
	pRPC->UnregisterFunction( RPC_EXIT_VEHICLE );
	pRPC->UnregisterFunction( RPC_RESPAWNVEHICLE );
	pRPC->UnregisterFunction( RPC_ENTER_VEHICLE_DONE );

	// Events
	pRPC->UnregisterFunction( RPC_TRIGGEREVENT );
}
