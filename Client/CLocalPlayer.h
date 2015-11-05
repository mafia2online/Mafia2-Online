/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLocalPlayer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CLocalPlayer : public CNetworkPlayer
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

	unsigned long					m_ulLastPingTime;

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

	bool							ProcessControls							( unsigned int uMsg, WPARAM wParam );
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

	void							SetRemoveForcefully						( bool bForcefully ) { m_bIsBeingForcefullyRemoved = bForcefully; }
	bool							IsBeingRemovedForcefully				( void ) { return m_bIsBeingForcefullyRemoved; }

};