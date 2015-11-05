/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

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

	// Create the network vehicle instance
	m_pNetworkVehicle[ vehicleId ] = new CNetworkVehicle;

	// Was the network vehicle instance created?
	if( m_pNetworkVehicle[ vehicleId ] )
	{
		// Set the vehicle id
		m_pNetworkVehicle[ vehicleId ]->SetId( vehicleId );

		// Set the vehicle model
		m_pNetworkVehicle[ vehicleId ]->SetModel( iModel, false );

		// Set the vehicle spawn position
		m_pNetworkVehicle[ vehicleId ]->SetSpawnPosition( vecPosition );

		// Set the vehicle spawn direction
		m_pNetworkVehicle[ vehicleId ]->SetSpawnRotation( vecRotation );

		return true;
	}

	return false;
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

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive(i) )
		{
			// Pulse this vehicle
			m_pNetworkVehicle[i]->Pulse ();
		}
	}
}

void CVehicleManager::PreProcess( void )
{
	DEBUG_TRACE("CVehicleManager::PreProcess");

	//
	CVector3 vecPosition;

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive(i) )
		{
			// Get the current vehicle position
			m_pNetworkVehicle[i]->GetPosition ( &vecPosition );

			// Get the vehicle screen position
			pCore->GetGraphics()->WorldToScreen ( vecPosition, &m_vecScreenPosition[i] );
		}
	}
}

void CVehicleManager::Process( void )
{
	DEBUG_TRACE("CVehicleManager::Process");

	//
	CVector3 vecPosition, vecRotation;
	CVector3 vecPosInterpTarget, vecPosInterpError;
	float fPosInterpAlpha;
	CVector3 vecRotInterpTarget, vecRotInterpError;
	float fRotInterpAlpha;
	float fSteerTarget, fSteerAlpha, fSteerError;

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive(i) && m_pNetworkVehicle[i]->GetVehicle() && m_pNetworkVehicle[i]->GetVehicle()->IsOnScreen () )
		{
			// Get the vehicle position
			m_pNetworkVehicle[i]->GetPosition ( &vecPosition );
			m_pNetworkVehicle[i]->GetRotation ( &vecRotation );
			vecPosInterpTarget = m_pNetworkVehicle[i]->m_Interpolation.position.vecTarget;
			vecPosInterpError = m_pNetworkVehicle[i]->m_Interpolation.position.vecError;
			fPosInterpAlpha = m_pNetworkVehicle[i]->m_Interpolation.position.fLastAlpha;
			vecRotInterpTarget = m_pNetworkVehicle[i]->m_Interpolation.rotation.vecTarget;
			vecRotInterpError = m_pNetworkVehicle[i]->m_Interpolation.rotation.vecError;
			fRotInterpAlpha = m_pNetworkVehicle[i]->m_Interpolation.rotation.fLastAlpha;
			fSteerTarget = m_pNetworkVehicle[i]->m_Interpolation.steer.fTarget;
			fSteerAlpha = m_pNetworkVehicle[i]->m_Interpolation.steer.fLastAlpha;
			fSteerError = m_pNetworkVehicle[i]->m_Interpolation.steer.fError;

			// Draw
			pCore->GetGraphics()->DrawText ( m_vecScreenPosition[i].fX, m_vecScreenPosition[i].fY, 0xFFFFFFFF, 1.0f, "tahoma-bold", true, (DT_NOCLIP | DT_CENTER), 
				"Position: %f %f %f\n"
				"Rotation: %f, %f, %f\n"
				"Position interpolation - Target: %f, %f, %f - Error: %f, %f, %f - Alpha: %f\n"
				"Rotation interpolation - Target: %f, %f, %f - Error: %f, %f, %f - Alpha: %f\n"
				"Steering interpolation - Target: %f - Error: %f - Alpha: %f",
				vecPosition.fX, vecPosition.fY, vecPosition.fZ,
				vecRotation.fX, vecRotation.fY, vecRotation.fZ,
				vecPosInterpTarget.fX, vecPosInterpTarget.fY, vecPosInterpTarget.fZ,
				vecPosInterpError.fX, vecPosInterpError.fY, vecPosInterpError.fZ,
				fPosInterpAlpha,
				vecRotInterpTarget.fX, vecRotInterpTarget.fY, vecRotInterpTarget.fZ,
				vecRotInterpError.fX, vecRotInterpError.fY, vecRotInterpError.fZ,
				fRotInterpAlpha,
				fSteerTarget, fSteerAlpha, fSteerError
			);
		}
	}
}

void CVehicleManager::ProcessSpawnQueue( void )
{
	DEBUG_TRACE("CVehicleManager::ProcessSpawnQueue");

	// Loop over all vehicles in the spawn queue
	for( std::list< CNetworkVehicle* >::iterator iter = m_spawnQueue.begin(); iter != m_spawnQueue.end(); iter++ )
	{
		// Create the current vehicle
		(*iter)->Create();

		// Is there a driver in the vehicle?
		if( (*iter)->GetDriver() )
		{
			// Put the driver back in the vehicle
			(*iter)->GetDriver()->PutInVehicle( (*iter), 1 );
		}

		// Loop over each passenger
		for( int i = 0; i < (MAX_SEATS - 1); i++ ) // todo: Use GetTotalSeats() instead, not all vehicles have 4 seats.
		{
			// Put the occupants in the vehicle
			if( (*iter)->GetPassenger( i ) )
			{
				// Put the passenger back in the vehicle
				(*iter)->GetPassenger( i )->PutInVehicle( (*iter), (i + 2) );
			}
		}
	}

	// Clear the spawn queue
	m_spawnQueue.clear();
}

CNetworkVehicle * CVehicleManager::GetFromGameGUID( DWORD dwGuid )
{
	DEBUG_TRACE("CVehicleManager::GetFromGameGUID");

	// Loop through all vehicles
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		// Is the current vehicle active?
		if( IsActive( i ) )
		{
			// Is this the vehicle we're looking for?
			if( Get( i )->GetVehicle() && Get( i )->GetVehicle()->GetGUID() == dwGuid )
				return Get( i );
		}
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