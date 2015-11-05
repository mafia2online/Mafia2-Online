/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkPlayer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define		TIME_BEFORE_ENTER_VEHICLE_WARP				5000

class CBlip;
class CNetworkPlayer
{

	friend class CLocalPlayer;
	friend class CRemotePlayer;

private:

	bool							m_bLocalPlayer;
	EntityId						m_playerId;
	String							m_strNick;
	unsigned int					m_uiColour;
	int								m_iControlStyle;
	unsigned short					m_usPing;
	EntityId						m_iBlipId;

	CM2Ped							* m_pPlayerPed;
	CNetworkVehicle					* m_pVehicle;
	M2ModelMgr						* m_pPlayerModelManager;
	unsigned int					m_uiModelIndex;
	EntityId						m_seat;

	CM2SyncObject					* m_pMoveObject;
	CM2SyncObject					* m_pAimObject;
	CM2SyncObject					* m_pShootObject;
	BYTE							m_bCharacterSlot;

	bool							m_bIsDead;
	DWORD							m_dwDeathTime;
	bool							m_bSpawned;
	DWORD							m_dwWeaponSelect;
	BYTE							m_bMoveStyle;

	bool							m_bAiming;
	bool							m_bShooting;

	CVector3						m_vecLookAt;

	ePlayerState					m_playerState;

	CVector3						m_vecPosition;
	CVector3						m_vecRotation;

	M2Enums::eEntityState			m_lastState;

	CBlip							* m_pAttachedBlip;
	bool							m_bBlipAttached;

	struct
	{
		CNetworkVehicle				* m_pTargetVehicle;
		EntityId					m_targetSeat;
		bool						m_bIsEntering;
		unsigned long				m_ulEnterStartTime;
		CM2SyncObject				* m_pEnterAICommand;
	} m_VehicleEnter;

	struct
	{
		bool						m_bIsExiting;
		unsigned long				m_ulExitStartTime;
		CM2SyncObject				* m_pExitAICommand;
	} m_VehicleExit;

public:

	CNetworkPlayer( bool bLocalPlayer );
	~CNetworkPlayer( void );

	bool							IsLocalPlayer( void ) { return m_bLocalPlayer; }

	void							SetId( EntityId playerId ) { m_playerId = playerId; }
	EntityId						GetId( void ) { return m_playerId; }

	void							SetNick( String strNick ) { m_strNick = strNick; }
	String							GetNick( void ) { return m_strNick; }

	void							SetColour( unsigned int uiColour ) { m_uiColour = uiColour; }
	unsigned int					GetColour( void ) { return m_uiColour; }

	void							SetDead( bool bDead ) { m_bIsDead = bDead; }
	bool							IsDead( void ) { return m_bIsDead; }

	void							SetSpawned( bool bSpawned ) { m_bSpawned = bSpawned; }
	bool							IsSpawned( void ) { return m_bSpawned; }

	void							SetDeathTime( DWORD dwDeathTime ) { m_dwDeathTime = dwDeathTime; }
	DWORD							GetDeathTime( void ) { return m_dwDeathTime; }

	void							SetAiming( bool bAiming ) { m_bAiming = bAiming; }
	bool							IsAiming( void ) { return m_bAiming; }

	void							SetShooting( bool bShooting ) { m_bShooting = bShooting; }
	bool							IsShooting( void ) { return m_bShooting; }

	void							SetPing( unsigned short usPing ) { m_usPing = usPing; }
	unsigned short					GetPing( void ) { return m_usPing; }

	void							SetMoveStyle( BYTE bStyle ) { m_bMoveStyle = bStyle; }
	BYTE							GetMoveStyle( void ) { return m_bMoveStyle; }

	void							Create( void );
	void							Destroy( void );
	void							Respawn( void );

	void							SetModel( unsigned int uiModelIndex, bool bRebuild = true );
	unsigned int					GetModel( void ) { return m_uiModelIndex; }

	void							ProcessVehicle( void );

	void							HandlePlayerDeath( void );

	void							SetPosition( CVector3 vecPosition, BYTE bMoveStyle, CVector3 vecEndDir );
	void							Teleport( CVector3 vecPosition );
	void							GetPosition( CVector3 * vecPosition );

	void							SetRotation( CVector3 vecRotation );
	void							GetRotation( CVector3 * vecRotation );

	void							SetHealth( float fHealth );
	float							GetHealth( void );

	void							SetSelectedWeapon( DWORD dwWeapon, bool bUseAnimation );
	DWORD							GetSelectedWeapon( void );
	bool							HasWeapon( DWORD dwWeapon );
	void							GiveWeapon( DWORD dwWeapon, DWORD dwAmmo );
	void							RemoveWeapon( DWORD dwWeapon, DWORD dwAmmo = 0 );

	void							SetCrouching( bool bCrouching );
	bool							IsCrouching( void );

	CM2Ped							* GetPlayerPed( void ) { return m_pPlayerPed; }

	void							SetCharacterSlot( BYTE bCharacterSlot ) { m_bCharacterSlot = bCharacterSlot; }
	BYTE							GetCharacterSlot( void ) { return m_bCharacterSlot; }

	void							UpdateAim( bool bAiming );
	void							UpdateShot( bool bShooting );

	void							GiveMoney( int iAmount );
	void							RemoveMoney( int iAmount );
	int								GetMoney( void );

	int								GetControlStyle();
	void							SetControlStyle( int iStyle );

	void							SetLookAt( CVector3 vecLookAt );
	void							GetLookAt( CVector3 * vecLookAt ) { memcpy( vecLookAt, &m_vecLookAt, sizeof(CVector3) ); }
	CVector3						GetLookAt( void ) { return m_vecLookAt; }

	void							TerminateSyncObjects( void );

	void							SetState( ePlayerState playerState ) { m_playerState = playerState; }
	ePlayerState					GetState( void ) { return m_playerState; }

	// Entity state
	void							SetEntityState( M2Enums::eEntityState state );
	M2Enums::eEntityState			GetEntityState( bool bLatest = false );

	CVector3						GetLastPosition( void ) { return m_vecPosition; }
	CVector3						GetLastRotation( void ) { return m_vecRotation; }

	// Vehicles
	void							EnterVehicle( CNetworkVehicle * pVehicle, M2Enums::eVehicleSeat seat, bool bEnter = true );
	void							EnterVehicleDone( void );
	void							ExitVehicle( bool bExit = true, bool bQuickly = false );
	bool							IsInVehicle( void ) { return (m_pVehicle != NULL); }
	bool							IsInVehicle ( CNetworkVehicle * pVehicle ) { return (m_pVehicle == pVehicle); }
	bool							InternalIsInVehicle( void );
	bool							InternalIsInVehicle( CM2Vehicle * pVehicle );
	void							SetVehicle( CNetworkVehicle * pVehicle ) { m_pVehicle = pVehicle; }
	CNetworkVehicle					* GetVehicle( void ) { return m_pVehicle; }
	void							SetSeat( EntityId seat ) { m_seat = seat; }
	EntityId						GetSeat( void ) { return m_seat; }
	void							PutInVehicle( CNetworkVehicle * pVehicle, EntityId seat );
	void							RemoveFromVehicle( CNetworkVehicle * pVehicle );

	//
	void							SetEnteringVehicle( CNetworkVehicle * pVehicle, EntityId seat );
	bool							IsEnteringVehicle( void ) { return m_VehicleEnter.m_bIsEntering; }
	bool							IsExitingVehicle( void ) { return m_VehicleExit.m_bIsExiting; }

	//
	CM2SyncObject					* GetMoveObject( void ) { return m_pMoveObject; }
	CM2SyncObject					* GetAimObject( void ) { return m_pAimObject; }
	CM2SyncObject					* GetShootObject( void ) { return m_pShootObject; }

	// Blips
	void							AttachBlip							( CBlip * pBlip );
	void							DetachBlip							( void );
	CBlip							* GetAttachedBlip					( void ) { return m_pAttachedBlip; }

};
