/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CVehicleManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CVehicleManager : public CMultiplayerVehicleManager
{

private:

	CNetworkVehicle													* m_pNetworkVehicle[ MAX_VEHICLES ];

public:

								CVehicleManager						( void );
								~CVehicleManager					( void );

	EntityId					Add									( int iModel, CVector3 vecPosition, CVector3 vecRotation );
	bool						Remove								( EntityId vehicleId );
	bool						IsActive							( EntityId vehicleId );

	void						Pulse								( void );

	CNetworkVehicle				* Get								( EntityId vehicleId ) { return m_pNetworkVehicle[vehicleId]; };
	EntityId					GetCount							( void );
	EntityId					FindFreeId							( void );

	void						HandlePlayerJoin					( EntityId playerId );
	void						HandlePlayerQuit					( EntityId playerId );
	void						HandlePlayerSpawn					( EntityId playerId );

};