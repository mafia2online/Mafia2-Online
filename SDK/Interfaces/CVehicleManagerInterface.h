/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CVehicleManagerInterface.h
*
***************************************************************/

#pragma once

#include	"CNetworkVehicleInterface.h"

class CVehicleManagerInterface
{
public:
	virtual EntityId						Add( int iModel, CVector3 vecPosition, CVector3 vecDirection ) = 0;
	virtual bool							Remove( EntityId vehicleId ) = 0;
	virtual	bool							IsActive( EntityId vehicleId ) = 0;
	virtual CNetworkVehicleInterface		* Get( EntityId vehicleId ) = 0;
	virtual EntityId						GetCount( void ) = 0;
};