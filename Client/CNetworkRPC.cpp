/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"


#include "RPC4Plugin.h"

#include "CCore.h"

#include "Math/CVector3.h"
#include "CString.h"

#include "BitStream.h"

#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CNetworkVehicle.h"
#include "CLocalPlayer.h"
#include "CRemotePlayer.h"
#include "engine/CM2Ped.h"

#include "CBlipManager.h"

#include "CFileTransferManager.h"

#include "CMafia.h"

#include "CGUI.h"
#include "CServerBrowser.h"

#include "GUI/ChatBox.h"

#include "CSync.h"

#include "SharedUtility.h"

#include "CClientScriptingManager.h"

#include "CRC.h"

#include "CEvents.h"
#include "Scripting/CSquirrelArguments.h"
#include "Network/NetRPCs.h"

#include "CNetworkRPC.h"

#include "CLogFile.h"

bool	CNetworkRPC::m_bRegistered = false;

void InitialData( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the player colour
	unsigned int uiColour;
	pBitStream->Read( uiColour );

	// Read the server name string
	RakNet::RakString strServerName;
	pBitStream->Read( strServerName );

	// Read the max player count
	int iMaxPlayers;
	pBitStream->Read( iMaxPlayers );

	// Read the http server address
	RakNet::RakString strHttpServer;
	pBitStream->Read( strHttpServer );

	// Read the http server port
	int iPort;
	pBitStream->Read( iPort );

	// Set the season
	CCore::Instance()->SetSummer ( pBitStream->ReadBit() );

	DEBUG_LOG ( "Season: %s", (CCore::Instance()->IsSummer () ? "Summer" : "Winter") );

	// Read the weather
	RakNet::RakString strWeather;
	pBitStream->Read ( strWeather );

	// Set the localplayer id
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetId( playerId );

	// Set the localplayer colour
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetColour( uiColour );

	// Set the server name string
	CCore::Instance()->SetServerName( String( strServerName ) );

	// Set the server max player count
	CCore::Instance()->SetServerMaxPlayers( iMaxPlayers );

	// Set the file transfer information
	CCore::Instance()->GetFileTransferManager()->SetServerInformation( strHttpServer.IsEmpty() ? CCore::Instance()->GetHost() : strHttpServer.C_String(), iPort );

	// Print success message
	ChatBox::Instance()->OutputF(ChatBox::INFO_MESSAGE_COLOR, "Successfully connected to %s.", strServerName.C_String());

	// Get the server folder string
	String strServerFolder = SharedUtility::GetAbsolutePath( SharedUtility::GetClientScriptFolder( CCore::Instance()->GetHost(), CCore::Instance()->GetPort() ) );

	// Does the folder not exist?
	if( !SharedUtility::Exists( strServerFolder.Get() ) )
	{
		// Create the folder
		SharedUtility::CreateDirectory( strServerFolder.Get() );
	}

	// Set the weather
	CCore::Instance()->GetGame()->ChangeWeather ( strWeather.C_String (), 1000 );
}

void ConnectionRejected( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	DEBUG_LOG("ConnectionRejected");

	// Read the rejection reason
	BYTE byteReason;
	pBitStream->Read( byteReason );

	// Read if we should output default messages
	bool bOutput = pBitStream->ReadBit();

	// Construct a message
	String strReason( "Server terminated the connection." );

	// Switch the reason
	switch( byteReason )
	{
		case REJECT_REASON_VERSION: strReason.Format( "Your client version doesn't match the one required by the server." ); break;
		case REJECT_REASON_NICKNAME: strReason.Format( "The nickname you're using is already in use." ); break;
		case REJECT_REASON_BANNED: strReason.Format( "You're banned from playing on this server." ); break;
	}

	// Disconnected due to reject ?
	CLogFile::Printf("Disconnect due to reason: %s", strReason.Get());
	CCore::Instance()->GetGUI()->GetServerBrowser()->SetDisconnectReason(true, strReason.Get());
}

void NewPlayer( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the player name
	RakNet::RakString strNick;
	pBitStream->Read( strNick );

	// Read the player colour
	unsigned int uiColour;
	pBitStream->ReadCompressed( uiColour );

	// Read the player model
	unsigned int uiModelIndex;
	pBitStream->ReadCompressed( uiModelIndex );

	// Add the player to the manager
	if( CCore::Instance()->GetPlayerManager()->Add( playerId, String( strNick ), uiColour ) )
	{
		// Set the player model
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SetModel( uiModelIndex, false );
	}
}

void RemovePlayer( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Remove the player from the manager
	CCore::Instance()->GetPlayerManager()->Remove( playerId );
}

void PlayerChat( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the input
	RakNet::RakString strInput;
	pBitStream->Read( strInput );

	CPlayerManager *const playerManager = CCore::Instance()->GetPlayerManager();
	CLocalPlayer *const localPlayer = playerManager->GetLocalPlayer();

	CNetworkPlayer *netPlayer = nullptr;
	if (localPlayer->GetId() == playerId) {
		netPlayer = localPlayer;
	}
	else {
		netPlayer = playerManager->Get(playerId);
	}
	if ( netPlayer )
		ChatBox::Instance()->OutputPlayerMessage(netPlayer, strInput.C_String());
}

void PlayerSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the player ping
	unsigned short usPing;
	pBitStream->ReadCompressed( usPing );

	// Read the player sync
	OnFootSync onFootSync;
	pBitStream->Read( (char *)&onFootSync, sizeof(OnFootSync) );

	RakNet::RakString strAnimStyleName;
	pBitStream->Read(strAnimStyleName);

	RakNet::RakString strAnimStyleDirectory;
	pBitStream->Read(strAnimStyleDirectory);

	CPlayerManager *pPlayerManager = CCore::Instance()->GetPlayerManager();

	// Get a pointer to the player
	CRemotePlayer * pRemotePlayer = pPlayerManager->Get(playerId);
	if(!pRemotePlayer)
		return;

	pRemotePlayer->SetPing( usPing );

	if (!pPlayerManager->GetLocalPlayer()->IsSpawned())
		return;

	if (playerId == pPlayerManager->GetLocalPlayer()->GetId())
		return;

	if (pRemotePlayer->GetAnimStyleName() != strAnimStyleName || pRemotePlayer->GetAnimStyleDirectory() != strAnimStyleDirectory)
		pRemotePlayer->SetAnimStyle(strAnimStyleDirectory, strAnimStyleName);

	pRemotePlayer->StoreOnFootSync( onFootSync );
}

void PlayerDeath( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Get a pointer to the player
	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the player pointer valid?
	if( pRemotePlayer )
	{
		// Handle the death with the network player
		pRemotePlayer->HandlePlayerDeath();
	}
}

void PlayerSpawn( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Get a pointer to the player
	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the player pointer valid?
	if( pRemotePlayer )
	{
		// Is the player not dead?
		if( !pRemotePlayer->IsDead() )
		{
			// Create the player
			pRemotePlayer->Create();
		}
		else
		{
			// Respawn the player
			pRemotePlayer->Respawn();
		}
	}
}

void KickPlayer( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	DEBUG_LOG ( "KickPlayer packet!" );

	// Handle this with the serverbrowser
	CCore::Instance()->GetGUI()->GetServerBrowser()->SetDisconnectReason(true, "YOU'VE BEEN KICKED FROM THE SERVER");

	DEBUG_LOG ( "KickPlayer done!" );
}

void PlayerSyncVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Get the vehicle pointer
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Let the localplayer know they're syncing this vehicle
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->StartSyncVehicle(pNetworkVehicle);
	}
}

void PlayerStopSyncVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Get the vehicle pointer
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Let the localplayer know they're no longer syncing this vehicle
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->StopSyncVehicle(pNetworkVehicle);
	}
}

// Blips
void NewBlip( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the blipid
	EntityId blipId;
	pBitStream->ReadCompressed( blipId );

	// Read the x coordinate
	float fX;
	pBitStream->Read( fX );

	// Read the y coordinate
	float fY;
	pBitStream->Read( fY );

	// Read the library
	int iLibrary;
	pBitStream->ReadCompressed( iLibrary );

	// Read the icon
	int iIcon;
	pBitStream->ReadCompressed( iIcon );

	// Read the blip type
	int blipType;
	pBitStream->ReadCompressed ( blipType );

	// Read the attached entity id
	EntityId entityId;
	pBitStream->ReadCompressed( entityId );

	DEBUG_LOG ( "NewBlip - %d, BlipType: %d, Attached to: %d", blipId, blipType, entityId );

	// Add the blip into the blip manager
	CCore::Instance()->GetBlipManager()->AddServerBlip(blipId, fX, fY, iLibrary, iIcon, (eBlipType)blipType, entityId);
}

void RemoveBlip( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the blipid
	EntityId blipId;
	pBitStream->ReadCompressed( blipId );

	// Remove the blip from the blip manager
	CCore::Instance()->GetBlipManager()->RemoveServerBlip(blipId);
}

// Vehicles
void NewVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	int iModel;
	pBitStream->Read( iModel );

	InVehicleSync vehicleSync;
	pBitStream->Read( (char *)&vehicleSync, sizeof(InVehicleSync) );

	CVehicleManager *pVehicleManager = CCore::Instance()->GetVehicleManager();
	if( pVehicleManager->Add( vehicleId, iModel, vehicleSync.m_vecPosition, vehicleSync.m_vecRotation ) )
	{
		CNetworkVehicle *pVehicle = pVehicleManager->Get(vehicleId);

		pVehicle->SetSyncData(vehicleSync);
		pVehicle->ProcessSyncOnSpawn(true);
	}
}

void RemoveVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicleid
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Remove the vehicle from the manager
	CCore::Instance()->GetVehicleManager()->Remove(vehicleId);
}

void EnterVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read if the enter was successful
	bool bSuccess = pBitStream->ReadBit ();

	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the seat
	EntityId seat;
	pBitStream->ReadCompressed( seat );

	// Get the network vehicle instance
	CNetworkVehicle * pVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the network vehicle instance valid?
	if( pVehicle )
	{
		// Was the enter a success?
		if ( bSuccess )
		{
			// Get the network player instance
			CNetworkPlayer * pPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

			// Is the network player instance valid?
			if( pPlayer )
			{
				// Is the localplayer spawned?
				if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned())
				{
					// Handle this with the player
					pPlayer->EnterVehicle( pVehicle, (M2Enums::eVehicleSeat)(seat + 1) );
				}
				else
				{
					// Set the player vehicle
					pPlayer->SetVehicle( pVehicle );

					// Set the seat
					pPlayer->SetSeat( (seat + 1) );
				}
			}
		}
		// If not success this packet will only return to the localplayer!
		else
		{
			// Get the localplayer instance
			CLocalPlayer * pLocalPlayer = CCore::Instance()->GetPlayerManager()->GetLocalPlayer();

			// Ensure we're in a vehicle internally
			if ( pLocalPlayer->InternalIsInVehicle () )
			{
				// Get the current vehicle we're in from internal id
				CNetworkVehicle * pCurrentVehicle = CCore::Instance()->GetVehicleManager()->GetFromGameGUID(pLocalPlayer->GetPlayerPed()->GetCurrentVehicle()->m_dwGUID);

				// Remove the player from the vehicle
				pLocalPlayer->RemoveFromVehicle ( pCurrentVehicle );

				// Get the network player instance of the real driver (player = driver id)
				CNetworkPlayer * pRealDriver = CCore::Instance()->GetPlayerManager()->Get(playerId);

				// Is the real driver valid?
				if ( pRealDriver )
				{
					// Is the real driver not in a vehicle?
					if ( !pRealDriver->InternalIsInVehicle () )
					{
						// Force the real driver back into the vehicle
						pRealDriver->PutInVehicle ( pCurrentVehicle, 0 );
					}
				}
			}
		}
	}
}

void ExitVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the player id
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read if we're exiting quickly
	bool bQuickly = pBitStream->ReadBit();

	// Get the network player instance
	CNetworkPlayer * pPlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the network player instance valid?
	if( pPlayer )
	{
		// Is the localplayer spawned?
		if( CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		{
			// Handle this with the player
			pPlayer->ExitVehicle( true, bQuickly );
		}
		else
		{
			// Reset the player vehicle
			pPlayer->SetVehicle( NULL );

			// Reset the seat
			pPlayer->SetSeat( INVALID_ENTITY_ID );
		}
	}
}

void VehicleSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the vehicleid
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the player ping
	unsigned short usPing;
	pBitStream->ReadCompressed( usPing );

	// Read the sync data
	InVehicleSync inVehicleSync;
	pBitStream->Read( (char *)&inVehicleSync, sizeof(InVehicleSync) );

	// Get the network player instance
	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the network player instance valid?
	if( pRemotePlayer )
	{
		// Set the player ping
		pRemotePlayer->SetPing( usPing );

		// Store the in vehicle sync
		pRemotePlayer->StoreInVehicleSync( vehicleId, inVehicleSync );
	}
}

void EnterVehicleDone( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Get the network player instance
	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the network player instance valid?
	if( pRemotePlayer )
	{
		// Handle this with the network player
		pRemotePlayer->EnterVehicleDone ();
	}
}

void PassengerSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the playerid
	EntityId playerId;
	pBitStream->ReadCompressed( playerId );

	// Read the player ping
	unsigned short usPing;
	pBitStream->ReadCompressed( usPing );

	// Read the sync data
	InPassengerSync passengerSync;
	pBitStream->Read( (char *)&passengerSync, sizeof(InPassengerSync) );

	// Get the network player instance
	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);

	// Is the network player instance valid?
	if( pRemotePlayer )
	{
		// Set the player ping
		pRemotePlayer->SetPing( usPing );

		// Store the in vehicle sync
		pRemotePlayer->StorePassengerSync( passengerSync );
	}
}

void UnoccupiedVehicleSync( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Get a pointer to the network vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Process the unoccupied sync with this vehicle
		pNetworkVehicle->ProcessUnoccupiedSync( pBitStream );
	}
}

void SpawnVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the spawn position
	CVector3 vecPosition;
	pBitStream->Read( vecPosition );

	// Read teh spawn rotation
	CVector3 vecRotation;
	pBitStream->Read( vecRotation );

	// Get a pointer to the network vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Is the vehicle not already spawned?
		if( !pNetworkVehicle->IsSpawned() )
		{
			// Set the spawn position
			pNetworkVehicle->SetSpawnPosition( vecPosition );

			// Set the spawn rotation
			pNetworkVehicle->SetSpawnRotation( vecRotation );

			// Create the vehicle
			pNetworkVehicle->Create();

			// Read the occupant count
			int iTotalOccupants;
			pBitStream->Read( iTotalOccupants );

			DEBUG_LOG ( "(SpawnVehicle) Total occupants: %d", iTotalOccupants );

			// Loop over the total occupants
			EntityId occupantId = INVALID_ENTITY_ID;
			int iSeat = 0;
			CRemotePlayer * pOccupant = NULL;
			for( int i = 0; i < iTotalOccupants; i++ )
			{
				// Read the current occupant id
				pBitStream->ReadCompressed( occupantId );

				// Read the current occupant seat
				pBitStream->Read( iSeat );

				DEBUG_LOG( "SpawnVehicle - Read occupant %d in seat %d.", occupantId, iSeat );

				// Is the read data valid?
				if( occupantId != INVALID_ENTITY_ID )
				{
					// Get a pointer to the current occupant
					pOccupant = CCore::Instance()->GetPlayerManager()->Get(occupantId);

					// Is the pointer valid?
					if( pOccupant )
					{
						// Set the occupant vehicle
						pOccupant->SetVehicle( pNetworkVehicle );

						// Set the occupant seat
						pOccupant->SetSeat( (iSeat + 1) );

						// Set the vehicle passenger
						pNetworkVehicle->SetOccupant( iSeat, pOccupant );

						DEBUG_LOG ( "Put occupant %d into vehicle %d (%d)", occupantId, pNetworkVehicle->GetId(), (iSeat + 1) );
					}
				}
			}
			DEBUG_LOG ( "SpawnVehicle RPC done!" );
		}
	}
}

void RespawnVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the spawn position
	CVector3 vecPosition;
	pBitStream->Read( vecPosition );

	// Read teh spawn rotation
	CVector3 vecRotation;
	pBitStream->Read( vecRotation );

	// Get a pointer to the network vehicle
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle pointer valid?
	if( pNetworkVehicle )
	{
		// Set the spawn position
		pNetworkVehicle->SetSpawnPosition( CVector3( vecPosition.fX, vecPosition.fY, (vecPosition.fZ - 1.0f) ) );

		// Set the spawn rotation
		pNetworkVehicle->SetSpawnRotation( vecRotation );

		// Handle the respawn with the vehicle
		pNetworkVehicle->HandleRespawn();
	}
}

void MoveToDriver( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed ( vehicleId );

	// Read the occupant id
	EntityId occupantId;
	pBitStream->ReadCompressed ( occupantId );

	// Is the vehicle and player valid?
	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId) && CCore::Instance()->GetPlayerManager()->IsActive(occupantId))
	{
		// Get the occupant
		CNetworkPlayer * pOccupant = CCore::Instance()->GetPlayerManager()->Get(occupantId);

		// Is the player inside a vehicle?
		if ( pOccupant->IsInVehicle () )
		{
			// Remove the player from the current vehicle
			pOccupant->RemoveFromVehicle ( pOccupant->GetVehicle() );
		}

		// Put the player in the driver seat
		CCore::Instance()->GetPlayerManager()->Get(occupantId)->PutInVehicle(CCore::Instance()->GetVehicleManager()->Get(vehicleId), 0);
	}
}

// File RPC's
void NewFile( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the file type
	bool bIsScript;
	pBitStream->Read( bIsScript );

	// Read the file name
	RakNet::RakString strFileName;
	pBitStream->Read( strFileName );

	// Read the file path
	RakNet::RakString strFilePath;
	pBitStream->Read( strFilePath );

	// Read the file checksum
	CFileChecksum fileChecksum;
	pBitStream->Read( (char *)&fileChecksum, sizeof(CFileChecksum) );

	// Add the file to the transfer manager
	CCore::Instance()->GetFileTransferManager()->Add(strFileName.C_String(), strFilePath.C_String(), fileChecksum, bIsScript);
}

void DeleteFile( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the file type
	bool bIsScript;
	pBitStream->Read( bIsScript );

	// Read the file name
	RakNet::RakString _strFileName;
	pBitStream->Read( _strFileName );

	String strFileName( _strFileName );

	// Is this a script and in the client script manager?
	if (bIsScript && CCore::Instance()->GetClientScriptingManager()->Exists(strFileName))
	{
		// Unload the script
		CCore::Instance()->GetClientScriptingManager()->RemoveScript(strFileName);

		// Remove the script
		CCore::Instance()->GetClientScriptingManager()->Unload(strFileName);
	}
}

void TriggerEvent( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	CSquirrelArguments * pArgs = new CSquirrelArguments( pBitStream );
	CSquirrelArgument * pEventName = (pArgs->size() > 0 ? pArgs->front() : 0);

	if( pEventName && pEventName->GetType() == OT_STRING )
	{
		String strEventName = *pEventName->data.str;
		pArgs->pop_front();

		CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call(strEventName, pArgs);
		SAFE_DELETE( pEventName );
	}

	SAFE_DELETE( pArgs );
}

void CNetworkRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Network functions
	pRPC->RegisterFunction( RPC_INITIAL_DATA, InitialData );
	pRPC->RegisterFunction( RPC_CONNECTION_REJECTED, ConnectionRejected );
	pRPC->RegisterFunction( RPC_NEW_PLAYER, NewPlayer );
	pRPC->RegisterFunction( RPC_REMOVE_PLAYER, RemovePlayer );
	pRPC->RegisterFunction( RPC_PLAYER_CHAT, PlayerChat );
	pRPC->RegisterFunction( RPC_PLAYER_SYNC, PlayerSync );
	pRPC->RegisterFunction( RPC_PLAYER_DEATH, PlayerDeath );
	pRPC->RegisterFunction( RPC_PLAYER_SPAWN, PlayerSpawn );
	pRPC->RegisterFunction( RPC_KICKPLAYER, KickPlayer );
	pRPC->RegisterFunction( RPC_PLAYERSYNCVEHICLE, PlayerSyncVehicle );
	pRPC->RegisterFunction( RPC_PLAYERSTOPSYNCVEHICLE, PlayerStopSyncVehicle );
	pRPC->RegisterFunction( RPC_NEWBLIP, NewBlip );
	pRPC->RegisterFunction( RPC_REMOVEBLIP, RemoveBlip );
	pRPC->RegisterFunction( RPC_NEW_VEHICLE, NewVehicle );
	pRPC->RegisterFunction( RPC_REMOVE_VEHICLE, RemoveVehicle );
	pRPC->RegisterFunction( RPC_ENTER_VEHICLE, EnterVehicle );
	pRPC->RegisterFunction( RPC_EXIT_VEHICLE, ExitVehicle );
	pRPC->RegisterFunction( RPC_VEHICLE_SYNC, VehicleSync );
	pRPC->RegisterFunction( RPC_ENTER_VEHICLE_DONE, EnterVehicleDone );
	pRPC->RegisterFunction( RPC_PASSENGER_SYNC, PassengerSync );
	pRPC->RegisterFunction( RPC_UNOCCUPIED_SYNC, UnoccupiedVehicleSync );
	pRPC->RegisterFunction( RPC_SPAWNVEHICLE, SpawnVehicle );
	pRPC->RegisterFunction( RPC_RESPAWNVEHICLE, RespawnVehicle );
	pRPC->RegisterFunction( RPC_MOVETODRIVER, MoveToDriver );
	pRPC->RegisterFunction( RPC_NEWFILE, NewFile );
	pRPC->RegisterFunction( RPC_DELETEFILE, DeleteFile );

	// Scripting
	pRPC->RegisterFunction( RPC_TRIGGEREVENT, TriggerEvent );
}

void CNetworkRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Network functions
	pRPC->UnregisterFunction( RPC_INITIAL_DATA );
	pRPC->UnregisterFunction( RPC_CONNECTION_REJECTED );
	pRPC->UnregisterFunction( RPC_NEW_PLAYER );
	pRPC->UnregisterFunction( RPC_REMOVE_PLAYER );
	pRPC->UnregisterFunction( RPC_PLAYER_CHAT );
	pRPC->UnregisterFunction( RPC_PLAYER_SYNC );
	pRPC->UnregisterFunction( RPC_PLAYER_DEATH );
	pRPC->UnregisterFunction( RPC_PLAYER_SPAWN );
	pRPC->UnregisterFunction( RPC_KICKPLAYER );
	pRPC->UnregisterFunction( RPC_PLAYERSYNCVEHICLE );
	pRPC->UnregisterFunction( RPC_PLAYERSTOPSYNCVEHICLE );
	pRPC->UnregisterFunction( RPC_NEWBLIP );
	pRPC->UnregisterFunction( RPC_REMOVEBLIP );
	pRPC->UnregisterFunction( RPC_NEW_VEHICLE );
	pRPC->UnregisterFunction( RPC_REMOVE_VEHICLE );
	pRPC->UnregisterFunction( RPC_ENTER_VEHICLE );
	pRPC->UnregisterFunction( RPC_EXIT_VEHICLE );
	pRPC->UnregisterFunction( RPC_VEHICLE_SYNC );
	pRPC->UnregisterFunction( RPC_ENTER_VEHICLE_DONE );
	pRPC->UnregisterFunction( RPC_PASSENGER_SYNC );
	pRPC->UnregisterFunction( RPC_UNOCCUPIED_SYNC );
	pRPC->UnregisterFunction( RPC_SPAWNVEHICLE );
	pRPC->UnregisterFunction( RPC_RESPAWNVEHICLE );
	pRPC->UnregisterFunction( RPC_MOVETODRIVER );
	pRPC->UnregisterFunction( RPC_NEWFILE );
	pRPC->UnregisterFunction( RPC_DELETEFILE );

	// Scripting
	pRPC->UnregisterFunction( RPC_TRIGGEREVENT );
}