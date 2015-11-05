/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CVehicleManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore				* pCore;

CVehicleManager::CVehicleManager( void )
{
	// Reset all vehicle pointers
	memset( m_pNetworkVehicle, NULL, sizeof(m_pNetworkVehicle) );
}

CVehicleManager::~CVehicleManager( void )
{
	// Loop over all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Remove the current vehicle
		Remove( i );
	}
}

EntityId CVehicleManager::Add( int iModel, CVector3 vecPosition, CVector3 vecRotation )
{
	// Find a free id to use
	EntityId id = FindFreeId();

	// Didn't we find a valid id?
	if( id == INVALID_ENTITY_ID )
		return INVALID_ENTITY_ID;

	// Create the network vehicle instance
	m_pNetworkVehicle[ id ] = new CNetworkVehicle();

	// Did the network vehicle instance fail to create?
	if( !m_pNetworkVehicle[ id ] )
		return INVALID_ENTITY_ID;

	// Set the vehicle id
	m_pNetworkVehicle[ id ]->SetId( id );

	// Set the vehicle model
	m_pNetworkVehicle[ id ]->SetModel( iModel );

	// Set the vehicle spawn position
	m_pNetworkVehicle[ id ]->SetSpawnPosition( vecPosition );

	// Set the vehicle spawn rotation
	m_pNetworkVehicle[ id ]->SetSpawnRotation( vecRotation );

	// Add the vehicle for everyone
	m_pNetworkVehicle[ id ]->AddForWorld();

	// Spawn the vehicle for everyone
	m_pNetworkVehicle[ id ]->SpawnForWorld();

	return id;
}

bool CVehicleManager::Remove( EntityId vehicleId )
{
	// Is the vehicle not active?
	if( !IsActive(vehicleId) )
		return false;

	// Remove this vehicle for everyone
	m_pNetworkVehicle[ vehicleId ]->RemoveForWorld();

	// Delete the network vehicle instance
	SAFE_DELETE( m_pNetworkVehicle[ vehicleId ] );

	return true;
}

bool CVehicleManager::IsActive( EntityId vehicleId )
{
	// Is the vehicle id invalid?
	if ( vehicleId < 0 || vehicleId > MAX_VEHICLES )
		return false;

	return (m_pNetworkVehicle[vehicleId] != NULL);
}

void CVehicleManager::Pulse( void )
{
	// Loop over all vehicles
	for( int i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( m_pNetworkVehicle[ i ] )
		{
			// Pulse the current vehicle
			m_pNetworkVehicle[ i ]->Pulse();
		}
	}
}

EntityId CVehicleManager::GetCount( void )
{
	EntityId count = 0;

	// Loop over all vehicles
	for( int i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
			count++;
	}

	return count;
}

EntityId CVehicleManager::FindFreeId( void )
{
	// Loop over all vehicles
	for( int i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle not active?
		if( !IsActive( i ) )
			return i;
	}

	return INVALID_ENTITY_ID;
}

void CVehicleManager::HandlePlayerJoin( EntityId playerId )
{
	// Loop over all vehicles
	for( int i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
		{
			// Add the vehicle for this player
			m_pNetworkVehicle[ i ]->AddForPlayer( playerId );
		}
	}
}

void CVehicleManager::HandlePlayerQuit( EntityId playerId )
{
	// Get a pointer to the network player
	CNetworkPlayer * pNetworkPlayer = pCore->GetPlayerManager()->Get( playerId );

	// Invalid player?
	if( !pNetworkPlayer )
		return;

	// Is the player in a vehicle?
	if( pNetworkPlayer->IsInVehicle() )
	{
		// Get the network vehicle pointer
		CNetworkVehicle * pNetworkVehicle = pNetworkPlayer->GetVehicle();

		// Invalid vehicle?
		if( !pNetworkVehicle )
			return;

		// Pass this to the network vehicle
		pNetworkVehicle->HandlePlayerDisconnect( pNetworkPlayer );
	}

	// Handle the disconnect with the player
	pNetworkPlayer->HandlePlayerQuit();
}

void CVehicleManager::HandlePlayerSpawn( EntityId playerId )
{
	// Loop over all vehicles
	for( int i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
		{
			// Add the vehicle for this player
			m_pNetworkVehicle[ i ]->SpawnForPlayer( playerId );
		}
	}
}