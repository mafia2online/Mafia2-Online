/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CRemotePlayer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CNetworkPlayer.h"
#include	"CSync.h"
#include	"Math\CVector3.h"

class CRemotePlayer : public CNetworkPlayer
{

private:

	OnFootSync						* m_pLastOnFootSync;

public:

	CRemotePlayer( void );
	~CRemotePlayer( void );

	void							Pulse( void );

	void							StoreOnFootSync( OnFootSync * onFootSync );
	void							StoreInVehicleSync( EntityId vehicleId, InVehicleSync * inVehicleSync );
	void							StorePassengerSync( InPassengerSync * passengerSync );

	void							GetLastSync( OnFootSync * onFootSync );

	bool							IsPositionOutOfRange ( CVector3 vecPos );

};