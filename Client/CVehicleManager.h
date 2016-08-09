/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CNetworkVehicle.h"

class CVehicleManager
{

private:

	CNetworkVehicle											* m_pNetworkVehicle[MAX_VEHICLES];
	std::list< CNetworkVehicle* >							m_spawnQueue;
	CVector3												m_vecScreenPosition[MAX_VEHICLES];

public:

	CVehicleManager(void);
	~CVehicleManager(void);

	bool							Add(EntityId vehicleId, VehicleSpawnProperties spawnProperties);
	void							Remove(EntityId vehicleId);
	bool							IsActive(EntityId vehicleId) { return (m_pNetworkVehicle[vehicleId] != NULL); }

	void							Pulse(void);

	void							PreProcess(void);
	void							Process(void);

	void							ProcessSpawnQueue(void);

	CNetworkVehicle					* Get(EntityId vehicleId) { return m_pNetworkVehicle[vehicleId]; }
	CNetworkVehicle					* GetFromGameGUID(DWORD dwGuid);

	void							AddToSpawnQueue(CNetworkVehicle * pVehicle) { m_spawnQueue.push_back(pVehicle); }
	void							RemoveFromSpawnQueue(CNetworkVehicle * pVehicle) { m_spawnQueue.remove(pVehicle); }

	EntityId						GetCount(void);

	void							RespawnAll(void);

};