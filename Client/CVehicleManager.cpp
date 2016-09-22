/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"Math\CVector3.h"

#include	"CNetworkVehicle.h"
#include	"CNetworkPlayer.h"

#include	"CGraphics.h"

#include	"CSync.h"

#include	"CVehicleManager.h"

CVehicleManager::CVehicleManager( void )
{
	// Reset all vehicle pointers
	memset( m_pNetworkVehicle, NULL, sizeof(m_pNetworkVehicle) );
}

CVehicleManager::~CVehicleManager( void )
{
	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
		{
			// Delete the network vehicle instance
			SAFE_DELETE( m_pNetworkVehicle[i] );
		}
	}
}

bool CVehicleManager::Add( EntityId vehicleId, int iModel, CVector3 vecPosition, CVector3 vecRotation )
{
	// Is the vehicle already active?
	if( IsActive( vehicleId ) )
		return false;

	m_pNetworkVehicle[ vehicleId ] = new CNetworkVehicle;
	if (!m_pNetworkVehicle[ vehicleId ])
		return false;

	m_pNetworkVehicle[ vehicleId ]->SetId( vehicleId );
	m_pNetworkVehicle[ vehicleId ]->SetModel( iModel, false );

	m_pNetworkVehicle[ vehicleId ]->SetSpawnPosition( vecPosition );
	m_pNetworkVehicle[ vehicleId ]->SetSpawnRotation( vecRotation );
	return true;
}

void CVehicleManager::Remove( EntityId vehicleId )
{
	// Is the vehicle not active?
	if( !IsActive( vehicleId ) )
		return;

	// Destroy the network vehicle instance
	SAFE_DELETE( m_pNetworkVehicle[vehicleId] );
}

void CVehicleManager::Pulse( void )
{
	DEBUG_TRACE("CVehicleManager::Pulse");
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		if( IsActive(i) )
			m_pNetworkVehicle[i]->Pulse ();
	}
}

CNetworkVehicle * CVehicleManager::GetFromGameGUID( DWORD dwGuid )
{
	DEBUG_TRACE("CVehicleManager::GetFromGameGUID");

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		CNetworkVehicle *pVehicle = m_pNetworkVehicle[i];
		if(pVehicle)
			if( pVehicle->GetVehicle()->GetGUID() == dwGuid )
				return pVehicle;
	}

	return  NULL;
}

EntityId CVehicleManager::GetCount( void )
{
	EntityId count = 0;

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
		{
			// Increase the vehicle count
			count++;
		}
	}

	return count;
}

void CVehicleManager::RespawnAll( void )
{
	DEBUG_TRACE("CVehicleManager::RespawnAll");
	CLogFile::Printf ( "CVehicleManager::RespawnAll" );

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active and spawned?
		if( IsActive ( i ) && m_pNetworkVehicle[ i ]->IsSpawned () )
		{
			// Get the last sync data
			InVehicleSync vehicleSync;
			m_pNetworkVehicle[ i ]->GetSyncData ( &vehicleSync );

			// Process the last sync data
			m_pNetworkVehicle[ i ]->StoreVehicleSync ( vehicleSync, false, true );
		}
	}
}