/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLocalPlayer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <list>

#include "ExplicitSingleton.h"

// As we have to include it theoretically in all the places
// we are using local player include it here.
#include "CNetworkPlayer.h"

class CNetworkVehicle;

class CLocalPlayer : public CNetworkPlayer, public ExplicitSingleton<CLocalPlayer>
{

private:

	bool							m_bFirstSpawn;
	std::list< CNetworkVehicle* >	m_syncingVehicles;
	bool							m_bFastExitVehicle;
	unsigned long					m_ulSpawnTime;
	unsigned long					m_ulDeathTime;
	unsigned long					m_ulLastFullSyncTime;
	bool							m_bIsBeingForcefullyRemoved;
	CVector3						m_vecLastGoodPosition;
	CVector3						m_vecLastGoodRotation;

	bool							m_bRenderNametags;
	bool							m_bRenderHealthbar;

	unsigned long					m_ulLastPingTime;

	int								m_oldMoveState;

public:

									CLocalPlayer							( void );
									~CLocalPlayer							( void );

	void							Pulse									( void );
	void							HandleSpawn								( bool bRespawn = true );

	void							SendOnFootSync							( void );
	void							SendInVehicleSync						( void );
	void							SendPassengerSync						( void );
	void							SendUnoccupiedVehicleSync				( CNetworkVehicle * pNetworkVehicle );

	void							Ping									( void );
	void							PingReturn								( RakNet::BitStream * pBitStream );

	void							OnEnterVehicle							( void );
	void							OnEnterVehicleDone						( void );
	void							OnLeaveVehicle							( void );
	void							OnDeath									( CNetworkPlayer * pKiller );
	void							OnReloadWeapon							( void );
	void							OnEnterWater							( void );
	bool							OnTakeDamage							( void );

	void							HandlePassengerKey						( void );
	void							GetClosestVehicle						( CNetworkVehicle ** pNetworkVehicle, EntityId * seatId );

	void							LockControls							( bool bLock, bool bForPlayAnim = false );
	bool							AreControlsLocked						( void );

	void							StartSyncVehicle						( CNetworkVehicle * pNetworkVehicle );
	void							StopSyncVehicle							( CNetworkVehicle * pNetworkVehicle );
	bool							IsSyncingVehicle						( CNetworkVehicle * pNetworkVehicle );

	void							FlagForFastExitVehicle					( bool bFlag ) { m_bFastExitVehicle = bFlag; }
	bool							IsFlaggedForFastExitVehicle				( void ) { return m_bFastExitVehicle; }

	unsigned long					GetSpawnTime							( void ) { return m_ulSpawnTime; }
	unsigned long					GetDeathTime							( void ) { return m_ulDeathTime; }

	bool							IsFullSyncNeeded						( void );

	void							SetRenderNametags						( bool isRendering ) { m_bRenderNametags = isRendering; }
	void							SetRenderHealthbar						( bool isRendering ) { m_bRenderHealthbar = isRendering; }
	bool							IsRenderingNametags						( void ) { return m_bRenderNametags; }
	bool							IsRenderingHealthbar						( void ) { return m_bRenderHealthbar; }

	void							SetRemoveForcefully						( bool bForcefully ) { m_bIsBeingForcefullyRemoved = bForcefully; }
	bool							IsBeingRemovedForcefully				( void ) { return m_bIsBeingForcefullyRemoved; }

};