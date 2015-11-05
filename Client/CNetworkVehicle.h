/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkVehicle.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkPlayer;
class CBlip;
class CNetworkVehicle
{

	friend class CVehicleManager;

private:

	EntityId						m_vehicleId;
	unsigned int					m_uiModelIndex;
	CVector3						m_vecSpawnPosition;
	CVector3						m_vecSpawnRotation;
	bool							m_bSpawned;
	CColor							m_primarySpawnColour;
	CColor							m_secondarySpawnColour;

	CVector3						m_vecTempRotation;

	CM2Vehicle						* m_pVehicle;
	M2ModelMgr						* m_pVehicleModelManager;

	InVehicleSync					m_lastSyncData;
	bool							m_bProcessSyncOnSpawn;

	CVector3						m_vecLastGoodPosition;
	CVector3						m_vecLastGoodRotation;

	CBlip							* m_pAttachedBlip;
	bool							m_bBlipAttached;

	struct
	{
		struct
		{
			CVector3				vecTarget;
			CVector3				vecError;
			float					fLastAlpha;
			unsigned long			ulStartTime;
			unsigned long			ulFinishTime;
		} position;

		struct
		{
			CVector3				vecTarget;
			CVector3				vecError;
			float					fLastAlpha;
			unsigned long			ulStartTime;
			unsigned long			ulFinishTime;
		} rotation;

		struct
		{
			float					fTarget;
			float					fError;
			float					fLastAlpha;
			unsigned long			ulStartTime;
			unsigned long			ulFinishTime;
		} steer;

				struct
		{
			CVector3				vecTarget;
			CVector3				vecError;
			float					fLastAlpha;
			unsigned long			ulStartTime;
			unsigned long			ulFinishTime;
		} speed;
	} m_Interpolation;

	CNetworkPlayer					* m_pDriver;
	CNetworkPlayer					* m_pPassenger[3];
	CNetworkPlayer					* m_pLastSyncer;

	unsigned long					m_ulSpawnTime;
	bool							m_bSpawnProcessed;

public:

									CNetworkVehicle						( void );
									~CNetworkVehicle					( void );

	void							SetId								( EntityId vehicleId ) { m_vehicleId = vehicleId; }
	EntityId						GetId								( void ) { return m_vehicleId; }

	void							SetModel							( unsigned int uiModelIndex, bool bRebuild = true );
	unsigned int					GetModel							( void ) { return m_uiModelIndex; }

	void							SetSpawned							( bool bSpawned ) { m_bSpawned = bSpawned; }
	bool							IsSpawned							( void ) { return m_bSpawned; }

	void							SetLastSyncer						( CNetworkPlayer * pPlayer ) { m_pLastSyncer = pPlayer; }
	CNetworkPlayer					* GetLastSyncer						( void ) { return m_pLastSyncer; }

	void							SetSyncData							( InVehicleSync vehicleSync ) { memcpy( &m_lastSyncData, &vehicleSync, sizeof(InVehicleSync) ); }
	void							GetSyncData							( InVehicleSync * vehicleSync ) { memcpy( vehicleSync, &m_lastSyncData, sizeof(InVehicleSync) ); }

	void							SetSpawnPosition					( CVector3 vecSpawnPosition ) { memcpy( &m_vecSpawnPosition, &vecSpawnPosition, sizeof(CVector3) ); }
	void							GetSpawnPosition					( CVector3 * vecSpawnPosition ) { memcpy( vecSpawnPosition, &m_vecSpawnPosition, sizeof(CVector3) ); }

	void							SetSpawnRotation					( CVector3 vecSpawnRotation ) { memcpy( &m_vecSpawnRotation, &vecSpawnRotation, sizeof(CVector3) ); }
	void							GetSpawnRotation					( CVector3 * vecSpawnRotation ) { memcpy( vecSpawnRotation, &m_vecSpawnRotation, sizeof(CVector3) ); }

	void							ProcessSyncOnSpawn					( bool bProcess ) { m_bProcessSyncOnSpawn = bProcess; }
	bool							ShouldProcessSyncOnSpawn			( void ) { return m_bProcessSyncOnSpawn; }

	void							Create								( void );
	void							Destroy								( void );

	void							Respawn								( void );
	void							HandleRespawn						( void );

	void							Deserialise							( RakNet::BitStream * pBitStream );
	void							ProcessLastSyncData					( bool bFirstSpawn = false );

	void							StoreVehicleSync					( InVehicleSync vehicleSync, bool bInterpolate = true, bool bSpawn = false );

	void							Pulse								( void );

	void							SetPosition							( CVector3 vecPosition );
	void							GetPosition							( CVector3 * vecPosition );

	void							SetRotation							( CVector3 vecRotation );
	void							GetRotation							( CVector3 * vecRotation );

	void							SetColour							( CColor primary, CColor secondary );
	void							GetColour							( CColor * primary, CColor * secondary );

	void							SetPlateText						( const char * szPlateText );
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

	void							SetSteer							( float fSteer );
	float							GetSteer							( void );

	void							SetSpeed							( float fSpeed );
	float							GetSpeed							( void );

	void							SetSpeedVec							( CVector3 vecSpeed );
	void							GetSpeedVec							( CVector3 * vecSpeed );

	void							SetFuel								( float fFuel );
	float							GetFuel								( void );

	void							SetLightState						( bool bState );
	bool							GetLightState						( void );

	void							HandlePlayerEnter					( CNetworkPlayer * pNetworkPlayer, EntityId seatId );
	void							HandlePlayerExit					( CNetworkPlayer * pNetworkPlayer, EntityId seatId, bool bResetInterpolation = true );

	CM2Vehicle						* GetVehicle						( void ) { return m_pVehicle; }
	CNetworkPlayer					* GetDriver							( void ) { return m_pDriver; }
	CNetworkPlayer					* GetPassenger						( int iIndex ) { return m_pPassenger[ iIndex ]; }

	void							SetOccupant							( int iIndex, CNetworkPlayer * pPlayer ) { if( iIndex == 0 ) { m_pDriver = pPlayer; } else { m_pPassenger[ iIndex ] = pPlayer; } }

	// Interpolation
	void							ResetInterpolation					( void );
	void							Interpolate							( void );

	// Position Interpolation
	void							SetTargetPosition					( CVector3 vecPosition );
	void							RemoveTargetPosition				( void ) { m_Interpolation.position.ulFinishTime = 0; }
	void							UpdateTargetPosition				( void );
	bool							HasTargetPosition					( void ) { return (m_Interpolation.position.ulFinishTime != 0); }

	// Rotation Interpolation
	void							SetTargetRotation					( CVector3 vecRotation );
	void							RemoveTargetRotation				( void ) { m_Interpolation.rotation.ulFinishTime = 0; }
	void							UpdateTargetRotation				( void );
	bool							HasTargetRotation					( void ) { return (m_Interpolation.rotation.ulFinishTime != 0); }

	// Steer Interpolation
	void							SetTargetSteer						( float fSteer );
	void							RemoveTargetSteer					( void ) { m_Interpolation.steer.ulFinishTime = 0; }
	void							UpdateTargetSteer					( void );
	bool							HasTargetSteer						( void ) { return (m_Interpolation.steer.ulFinishTime != 0); }

	// Speed Interpolation
	void							SetTargetSpeed						( CVector3 vecSpeed );
	void							RemoveTargetSpeed					( void ) { m_Interpolation.speed.ulFinishTime = 0; }
	void							UpdateTargetSpeed					( void );
	bool							HasTargetSpeed						( void ) { return (m_Interpolation.speed.ulFinishTime != 0); }

	bool							GetClosestPlayer					( CNetworkPlayer ** pNetworkPlayer );

	void							ProcessUnoccupiedSync				( RakNet::BitStream * pBitStream );

	// Blips
	void							AttachBlip							( CBlip * pBlip );
	void							DetachBlip							( void );
	CBlip							* GetAttachedBlip					( void ) { return m_pAttachedBlip; }

};