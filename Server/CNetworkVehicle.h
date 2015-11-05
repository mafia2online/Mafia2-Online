/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkVehicle.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkPlayer;
class CNetworkVehicle : public CMultiplayerVehicle
{

	friend class CNetworkPlayer;

private:

	EntityId						m_vehicleId;
	int								m_iModel;
	CVector3						m_vecSpawnPosition;
	CVector3						m_vecSpawnRotation;

	unsigned long					m_ulLastSyncerCheckTime;
	CNetworkPlayer					* m_pLastSyncer;

	CNetworkPlayer					* m_pOccupants[ MAX_SEATS ];
	unsigned int					m_uiTotalOccupants;

	InVehicleSync					m_lastSyncData;
	unsigned long					m_ulLastOccupantTime;
	unsigned long					m_ulRespawnTime;

public:

									CNetworkVehicle						( void );
									~CNetworkVehicle					( void );

	void							SetId								( EntityId id ) { m_vehicleId = id; }
	EntityId						GetId								( void ) { return m_vehicleId; }

	void							SetModel							( int iModel );
	int								GetModel							( void ) { return m_iModel; }

	void							SetSpawnPosition					( CVector3 vecSpawnPosition );
	void							GetSpawnPosition					( CVector3 * vecSpawnPosition ) { memcpy( vecSpawnPosition, &m_vecSpawnPosition, sizeof(CVector3) ); }

	void							SetSpawnRotation					( CVector3 vecSpawnRotation );
	void							GetSpawnRotation					( CVector3 * vecSpawnRotation ) { memcpy( vecSpawnRotation, &m_vecSpawnRotation, sizeof(CVector3) ); }

	void							SetSyncData							( InVehicleSync vehicleSync ) { memcpy( &m_lastSyncData, &vehicleSync, sizeof(InVehicleSync) ); }
	void							GetSyncData							( InVehicleSync * vehicleSync ) { memcpy( vehicleSync, &m_lastSyncData, sizeof(InVehicleSync) ); }

	void							AddForPlayer						( EntityId playerId );
	void							AddForWorld							( void );

	void							RemoveForPlayer						( EntityId playerId );
	void							RemoveForWorld						( void );

	void							SpawnForPlayer						( EntityId playerId );
	void							SpawnForWorld						( void );

	void							RespawnForPlayer					( EntityId playerId );
	void							RespawnForWorld						( void );

	void							Pulse								( void );
	void							SendInVehicleSync					( void );

	void							SetPosition							( CVector3 vecPosition, bool bBroadcast = true );
	void							GetPosition							( CVector3 * vecPosition );

	void							SetRotation							( CVector3 vecRotation, bool bBroadcast = true );
	void							GetRotation							( CVector3 * vecRotation );

	void							SetColour							( CColor primary, CColor secondary );
	void							GetColour							( CColor * primary, CColor * secondary );

	void							SetPlateText						( const char * szText );
	const char						* GetPlateText						( void );

	void							Repair								( void );
	void							Explode								( void );

	void							SetDirtLevel						( float fDirtLevel );
	float							GetDirtLevel						( void );

	void							SetEngineState						( bool bState );
	bool							GetEngineState						( void );

	void							SetPartOpen							( int iPart, bool bOpen );
	bool							IsPartOpen							( int iPart );

	void							SetSirenState						( bool bState );
	bool							GetSirenState						( void );
	
	void							SetHornState						( bool bState );
	bool							GetHornState						( void );

	void							SetWindowOpen						( int iSeat, bool bOpen );
	bool							IsWindowOpen						( int iSeat );

	void							SetTuningTable						( int iTable );
	int								GetTuningTable						( void );

	void							SetWheelTexture						( int iWheelIndex, int iTexture );
	int								GetWheelTexture						( int iWheelIndex );

	void							SetSpeedVec							( CVector3 vecSpeed );
	void							GetSpeedVec							( CVector3 * vecSpeed );

	void							SetFuel								( float fFuel );
	float							GetFuel								( void );

	void							SetLightState						( bool bLightState );
	bool							GetLightState						( void );

	void							SetLastSyncer						( CNetworkPlayer * pLastSyncer ) { m_pLastSyncer = pLastSyncer; }
	CNetworkPlayer					* GetLastSyncer						( void ) { return m_pLastSyncer; }

	void							SetOccupant							( int iIndex, CNetworkPlayer * pOccupant ) { m_pOccupants[iIndex] = pOccupant; }
	CNetworkPlayer					* GetOccupant						( int iIndex ) { return m_pOccupants[iIndex]; }					

	void							HandlePlayerEnter					( CNetworkPlayer * pNetworkPlayer, int iSeat );
	void							HandlePlayerExit					( CNetworkPlayer * pNetworkPlayer, int iSeat );

	void							Serialise							( RakNet::BitStream * pBitStream );
	void							Deserialise							( RakNet::BitStream * pBitStream );

	void							HandlePlayerDisconnect				( CNetworkPlayer * pNetworkPlayer );
	bool							GetClosestPlayer					( CNetworkPlayer ** pNetworkPlayer );
	void							ProcessUnoccupiedSync				( RakNet::BitStream * pBitStream );

	void							SetRespawnTime						( unsigned long ulRespawnTime ) { m_ulRespawnTime = ulRespawnTime; }
	unsigned long					GetRespawnTime						( void ) { return m_ulRespawnTime; }

};