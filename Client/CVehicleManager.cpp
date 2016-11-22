/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"

#include "CNetworkVehicle.h"
#include "CNetworkPlayer.h"

#include "CGraphics.h"

#include "CSync.h"

#include "CVehicleManager.h"

#include "CLogFile.h"

CVehicleManager::CVehicleManager( void )
{
	memset( m_pNetworkVehicle, NULL, sizeof(m_pNetworkVehicle) );
}

CVehicleManager::~CVehicleManager( void )
{
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		if( IsActive( i ) )
		{
			SAFE_DELETE( m_pNetworkVehicle[i] );
		}
	}
}

bool CVehicleManager::Add( EntityId vehicleId, int iModel, CVector3 vecPosition, CVector3 vecRotation )
{
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
	if( !IsActive( vehicleId ) )
		return;

	SAFE_DELETE( m_pNetworkVehicle[vehicleId] );
}

void CVehicleManager::Pulse( void )
{
	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		if( IsActive(i) )
			m_pNetworkVehicle[i]->Pulse ();
	}
}

CNetworkVehicle * CVehicleManager::GetFromGameGUID( DWORD dwGuid )
{
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

	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		if( IsActive( i ) )
		{
			count++;
		}
	}

	return count;
}

void CVehicleManager::RespawnAll( void )
{
	DEBUG_LOG("CVehicleManager::RespawnAll");

	for( EntityId i = 0; i < MAX_VEHICLES; i++ )
	{
		if( IsActive ( i ) && m_pNetworkVehicle[ i ]->IsSpawned () )
		{
			InVehicleSync vehicleSync;
			m_pNetworkVehicle[ i ]->GetSyncData ( &vehicleSync );
			m_pNetworkVehicle[ i ]->StoreVehicleSync ( vehicleSync, false, true );
		}
	}
}
