/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkPlayer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkVehicle;
class CNetworkPlayer : public CMultiplayerPlayer
{

private:

	EntityId													m_playerId;
	String														m_strNick;
	unsigned int												m_uiColour;
	String														m_strSerial;
	String														m_strIP;
	bool														m_bIsDead;
	ePlayerState												m_State;

	OnFootSync													m_onFootSync;
	InVehicleSync												m_inVehicleSync;
	InPassengerSync												m_passengerSync;

	bool														m_bAiming;
	bool														m_bShooting;

	CNetworkVehicle												* m_pVehicle;
	EntityId													m_iSeat;

	std::list< CNetworkVehicle* >								m_syncingVehicles;
	unsigned long												m_ulLastPingTime;

public:

									CNetworkPlayer				( void );
									~CNetworkPlayer				( void );

	void							SetId						( EntityId playerId );
	EntityId						GetId						( void ) { return m_playerId; }

	void							SetNick						( const char * szNick ) { m_strNick.Set( szNick ); }
	void							ChangeNick					( const char * szNick );
	const char						* GetNick					( void ) { return m_strNick.Get(); }

	void							SetColour					( unsigned int uiColour );
	unsigned int					GetColour					( void ) { return m_uiColour; }

	void							SetSerial					( const char * szSerial ) { m_strSerial.Set( szSerial ); }
	const char						* GetSerial					( void ) { return m_strSerial.Get(); }

	void							SetIP						( const char * szIP ) { m_strIP.Set( szIP ); }
	const char						* GetIP						( void ) { return m_strIP.Get(); }

	unsigned short					GetPing						( void );

	void							SetDead						( bool bDead ) { m_bIsDead = bDead; }
	bool							IsDead						( void ) { return m_bIsDead; }

	void							SetState					( ePlayerState state ) { m_State = state; }
	ePlayerState					GetState					( void ) { return m_State; }

	void							SetModel					( unsigned int iModel );
	unsigned int					GetModel					( void );

	void							SetHealth					( float fHealth );
	float							GetHealth					( void );

	void							GiveWeapon					( int iWeapon, int iAmmo );
	void							RemoveWeapon				( int iWeapon, int iAmmo );
	int								GetWeapon					( void );

	void							SetPosition					( CVector3 vecPosition );
	void							GetPosition					( CVector3 * vecPosition );

	void							SetRotation					( CVector3 vecRotation );
	void							GetRotation					( CVector3 * vecRotation );

	void							AddForPlayer				( EntityId playerId );
	void							AddForWorld					( void );

	void							RemoveForPlayer				( EntityId playerId );
	void							RemoveForWorld				( void );

	void							KillForPlayer				( EntityId playerId );
	void							KillForWorld				( void );

	void							SpawnForPlayer				( EntityId playerId );
	void							SpawnForWorld				( void );

	void							StoreOnFootSync				( OnFootSync * onFootSync );
	void							StoreInVehicleSync			( InVehicleSync * inVehicleSync );
	void							StorePassengerSync			( InPassengerSync * passengerSync );

	void							SendOnFootSync				( void );
	void							SendInVehicleSync			( void );
	void							SendPassengerSync			( void );

	void							Pulse						( void );

	void							Kick						( void );
	void							Ping						( void );

	void							SetVehicle					( CNetworkVehicle * pVehicle ) { m_pVehicle = pVehicle; }
	CNetworkVehicle					* GetVehicle				( void ) { return m_pVehicle; }
	bool							IsInVehicle					( void ) { return (m_pVehicle != NULL); }

	void							SetSeat						( EntityId iSeat ) { m_iSeat = iSeat; }
	EntityId						GetSeat						( void ) { return m_iSeat; }

	void							HandleVehicleEnter			( EntityId vehicleId, EntityId iSeat );
	void							HandleVehicleExit			( EntityId vehicleId, EntityId iSeat, bool bQuickly = false );
	void							HandleVehicleEnterDone		( void );

	void							StartSyncVehicle			( CNetworkVehicle * pNetworkVehicle );
	void							StopSyncVehicle				( CNetworkVehicle * pNetworkVehicle );
	bool							IsSyncingVehicle			( CNetworkVehicle * pNetworkVehicle );

	void							HandlePlayerQuit			( void );

	bool							PutInVehicle				( CNetworkVehicle * pVehicle, EntityId seatId );
	void							RemoveFromVehicle			( void );

	void							GiveMoney					( int iMoney );
	void							TakeMoney					( int iMoney );
	int								GetMoney					( void );

};