/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CNetworkVehicle.h"

class CVehicleManager
{

private:

	CNetworkVehicle											* m_pNetworkVehicle[MAX_VEHICLES];

public:

									CVehicleManager			( void );
									~CVehicleManager		( void );

	bool							Add						( EntityId vehicleId, int iModel, CVector3 vecPosition, CVector3 vecRotation );
	void							Remove					( EntityId vehicleId );
	bool							IsActive				( EntityId vehicleId ) { assert(vehicleId < MAX_VEHICLES); return (m_pNetworkVehicle[vehicleId] != NULL); }

	void							Pulse					( void );

	CNetworkVehicle					* Get					( EntityId vehicleId ) { assert(vehicleId < MAX_VEHICLES); return m_pNetworkVehicle[vehicleId]; }
	CNetworkVehicle					* GetFromGameGUID		( DWORD dwGuid );

	EntityId						GetCount				( void );

	void							RespawnAll				( void );

};