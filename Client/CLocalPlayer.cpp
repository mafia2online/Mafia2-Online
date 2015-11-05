/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLocalPlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;
bool bOldControlState = false;
bool bRespawnVehicle = false;
bool bOldCameraState = false;
bool bOldCTRLState = false;

CLocalPlayer::CLocalPlayer( void ) : CNetworkPlayer( true )
{
	// Reset
	m_bFirstSpawn = true;
	m_bFastExitVehicle = false;
	m_bIsBeingForcefullyRemoved = false;
	m_ulSpawnTime = 0;
	m_ulLastFullSyncTime = 0;
	m_ulLastPingTime = 0;
}

CLocalPlayer::~CLocalPlayer( void )
{
	// Clear the syncing vehicles list
	m_syncingVehicles.clear ();
}

void CLocalPlayer::Pulse( void )
{
	// Are we experiencing connection trouble?
	if ( IsSpawned() && (SharedUtility::GetTime() - m_ulLastPingTime) > 6000 )
		pCore->SetConnectionProblem ( true );

	// Get our current position and rotation
	CVector3 vecCurrentPosition, vecCurrentRotation;
	GetPosition( &vecCurrentPosition );
	GetRotation( &vecCurrentRotation );

	// Is our position or rotation invalid?
	if ( !Math::IsValidVector ( vecCurrentPosition ) || !Math::IsValidVector ( vecCurrentRotation ) )
	{
		CLogFile::Printf ( "ERROR - Player position or rotation was invalid. Warped back to the last good saved state." );

		// Warp the player back to their last good saved position and rotation
		Teleport ( m_vecLastGoodPosition );
		SetRotation ( m_vecLastGoodRotation );

		// Is the last good state invalid?
		if ( !Math::IsValidVector ( m_vecLastGoodPosition ) || !Math::IsValidVector ( m_vecLastGoodRotation ) )
		{
			// Fade hud and sound quickly
			pCore->GetHud()->FadeOut ( 0 );
			pCore->GetGame()->FadeSound ( true, 0 );

			// Reset position
			Teleport ( CVector3() );
			SetRotation ( CVector3() );

			// Kill the player
			SetHealth ( 0.0f );
		}

		// Return so we don't overwrite the last good position with invalid positions
		return;
	}

	// Save this current position and rotation
	memcpy ( &m_vecLastGoodPosition, &vecCurrentPosition, sizeof ( CVector3 ) );
	memcpy ( &m_vecLastGoodRotation, &vecCurrentRotation, sizeof ( CVector3 ) );

	// Do we need a full sync?
	if( IsFullSyncNeeded () )
	{
		// Are we not in a vehicle?
		if( !InternalIsInVehicle() )
		{
			// Send onfoot sync data
			SendOnFootSync();
		}
		else
		{
			// Are we the vehicle driver?
			if( GetState() == PLAYERSTATE_DRIVER )
			{
				// Send invehicle sync data
				SendInVehicleSync();
			}

			// Send passenger sync data
			SendPassengerSync();
		}
	}

	// Are we spawned and not dead?
	if( IsSpawned() && ((SharedUtility::GetTime() - m_ulSpawnTime) > 5000) && !IsDead() )
	{
		// Are we falling below the map?
		if( vecCurrentPosition.fZ <= -150.0f )
		{
			// Kill ourself
			SetHealth( 0.0f );
		}
	}

	// Have we been dead for 3 seconds?
	if ( (SharedUtility::GetTime() - m_ulDeathTime) > 4000 && IsDead() )
	{
		// Deactivate the player ped
		m_pPlayerPed->Deactivate ();

		// Respawn
		HandleSpawn ( true );

		// Fade the hud back in
		pCore->GetHud()->FadeIn ( 1000 );

		// Fade the sound back in
		pCore->GetGame()->FadeSound ( false, 1 );

		// Activate the player ped
		m_pPlayerPed->Activate ();
	}

	// Are we spawned, in a vehicle and typing?
	if( IsSpawned() && IsInVehicle() && pCore->GetChat()->IsInputVisible() )
	{
		// Reset the vehicle steering (prevent car turning to sides)
		if( m_pVehicle->GetVehicle() )
			m_pVehicle->GetVehicle()->AddSteer( 0.0f );
	}

	// Loop over all vehicles we're syncing
	//for( std::list< CNetworkVehicle* >::iterator iter = m_syncingVehicles.begin(); iter != m_syncingVehicles.end(); iter++ )
	//{
		// Send the vehicle sync for this vehicle
	//	SendUnoccupiedVehicleSync( *iter );
	//}
}

void CLocalPlayer::SendOnFootSync( void )
{
	// Are we not connected to the network?
	if( !pCore->GetNetworkModule()->IsConnected() )
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Construct a new foot sync data structure
	OnFootSync onFootSync;

	// Get the player position
	GetPosition ( &onFootSync.m_vecPosition );

	// Get the player rotation
	GetRotation ( &onFootSync.m_vecRotation );

	// Get the player direction
	m_pPlayerPed->GetDirection ( &onFootSync.m_vecDirection );

	// Get the player health
	onFootSync.m_fHealth = GetHealth ();

	// Get the player selected weapon
	onFootSync.m_dwSelectedWeapon = GetSelectedWeapon ();

	// Get the player control state
    onFootSync.m_bControlState = m_pPlayerPed->GetControlState ();

	// Get the look at position
	pCore->GetCamera()->GetLookAt ( &onFootSync.m_vecLookAt );

	// Get the aim state
	onFootSync.m_bAiming = m_pPlayerPed->IsAiming ();

	// Get the shooting state
	onFootSync.m_bShooting = m_pPlayerPed->IsShooting ();

	// Write the model index
	onFootSync.m_uiModelIndex = Game::GetIdFromPlayerModel ( m_pPlayerModelManager->GetModelName() );

	// Write the sync structure into the bitstream
	pBitStream.Write( (char *)&onFootSync, sizeof(OnFootSync) );

	// Send the bitstream to the server
	pCore->GetNetworkModule()->Call( RPC_PLAYER_SYNC, &pBitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, true );
}

void CLocalPlayer::SendInVehicleSync( void )
{
	// Are we not connected to the network?
	if( !pCore->GetNetworkModule()->IsConnected() )
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = pCore->GetVehicleManager()->GetFromGameGUID( GetPlayerPed()->GetCurrentVehicle()->m_dwGUID );

	// Did we fail to find the vehicle?
	if( !pVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( pVehicle->GetId() );

	// Construct a new in vehicle sync data structure
	InVehicleSync inVehicleSync;

	// Get the vehicle position
	pVehicle->GetPosition( &inVehicleSync.m_vecPosition );

	// Get the vehicle rotation
	pVehicle->GetRotation( &inVehicleSync.m_vecRotation );

	// Get the vehicle dirt level
	inVehicleSync.m_fDirtLevel = pVehicle->GetVehicle()->GetDirtLevel();

	// Get the vehicle tuning table
	inVehicleSync.m_iTuningTable = pVehicle->GetVehicle()->GetTuningTable();

	// Get the vehicle horn state
	inVehicleSync.m_bHornState = pVehicle->GetVehicle()->IsHornOn();

	// Get the vehicle siren state
	inVehicleSync.m_bSirenState = pVehicle->GetVehicle()->IsSirenOn();

	// Get the vehicle fuel
	inVehicleSync.m_fFuel = pVehicle->GetVehicle()->GetFuel();

	// Get the vehicle speed
	pVehicle->GetSpeedVec( &inVehicleSync.m_vecVelocity );

	// Get the turn speed
	inVehicleSync.m_fTurnSpeed = pVehicle->GetSteer();

	// Get the engine damage
	inVehicleSync.m_fEngineDamage = pVehicle->GetVehicle()->GetEngineDamage();

	// Get the vehicle plate text
	strcpy( inVehicleSync.m_szPlateText, pVehicle->GetPlateText() );
	inVehicleSync.m_szPlateText[ 6 ] = '\0';

	// Get the vehicle colour
	pVehicle->GetColour( &inVehicleSync.m_primaryColour, &inVehicleSync.m_secondaryColour );

	// Get the power state
	inVehicleSync.m_bPower = pVehicle->GetVehicle()->GetPower();

	// Get the brake state
	inVehicleSync.m_bBrake = pVehicle->GetVehicle()->GetBrake();

	// Get the vehicle wheel models (DISABLED FOR TESTING)
	for ( int i = 0; i < 3; i++ )
		inVehicleSync.m_bWheelModels[ i ] = 0xFF; //Game::GetIdFromVehicleWheelModel ( pVehicle->GetVehicle()->GetWheelTexture ( i ) );

	// Get the handbrake state
	inVehicleSync.m_bHandbrake = pVehicle->GetVehicle()->IsHandbrakeOn ();

	// Get the light state
	inVehicleSync.m_bLightState = pVehicle->GetVehicle()->GetLightState ();

	// Write the sync structure into the bitstream
	pBitStream.Write( (char *)&inVehicleSync, sizeof(InVehicleSync) );

	// Send the bitstream to the server
	pCore->GetNetworkModule()->Call( RPC_VEHICLE_SYNC, &pBitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, true );
}

void CLocalPlayer::SendPassengerSync( void )
{
	// Are we not connected to the network?
	if( !pCore->GetNetworkModule()->IsConnected() )
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = pCore->GetVehicleManager()->GetFromGameGUID( GetPlayerPed()->GetCurrentVehicle()->m_dwGUID );

	// Did we fail to find the vehicle?
	if( !pVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Construct a new passenger sync data structure
	InPassengerSync passengerSync;

	// Get the player health
	passengerSync.m_fHealth = GetHealth ();

	// Get the player selected weapon
	passengerSync.m_dwSelectedWeapon = GetSelectedWeapon ();

	// Write the sync structure to the bitstream
	pBitStream.Write( (char *)&passengerSync, sizeof(InPassengerSync) );

	// Send the bitstream to the server
	pCore->GetNetworkModule()->Call( RPC_PASSENGER_SYNC, &pBitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, true );
}

void CLocalPlayer::SendUnoccupiedVehicleSync( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Construct a new bitstream
	//RakNet::BitStream pBitStream;

	// Write the vehicle id
	//pBitStream.WriteCompressed( pNetworkVehicle->GetId() );

	// Construct a new unoccupied vehicle sync structure
	//UnoccupiedVehicleSync unoccupiedVehicleSync;

	// Get the vehicle position
	//pNetworkVehicle->GetPosition( &unoccupiedVehicleSync.m_vecPosition );

	// Get the vehicle rotation
	//pNetworkVehicle->GetRotation( &unoccupiedVehicleSync.m_quatRotation );

	// Write the sync structure to the bitstream
	//pBitStream.Write( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );
	
	// Send the bitstream to the server
	//pCore->GetNetworkModule()->Call( RPC_UNOCCUPIED_SYNC, &pBitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, true );
}

void CLocalPlayer::Ping ( void )
{
	// Send a packet to the server
	pCore->GetNetworkModule()->Call ( RPC_PLAYERPING, NULL, MEDIUM_PRIORITY, RELIABLE_ORDERED, true );
}

void CLocalPlayer::PingReturn ( RakNet::BitStream * pBitStream )
{
	// Read the player ping
	unsigned short usPing;
	pBitStream->ReadCompressed ( usPing );

	// Set the last ping time
	m_ulLastPingTime = SharedUtility::GetTime ();

	// Set our ping
	SetPing ( usPing );

	// Mark as no connection problem
	pCore->SetConnectionProblem ( false );
}

void CLocalPlayer::OnEnterVehicle( void )
{
	// Get the vehicle instance from the vehicle guid
	CNetworkVehicle * pNetworkVehicle = pCore->GetVehicleManager()->GetFromGameGUID( GetPlayerPed()->GetCurrentVehicle()->m_dwGUID );

	// Is the vehicle instance valid?
	if( pNetworkVehicle )
	{
#ifdef _DEBUG
		M2Vehicle * pVehicle = pNetworkVehicle->GetVehicle()->GetVehicle();
		DWORD dwVehicleData = (DWORD)(pVehicle) + 0xA8;

		pCore->GetChat()->AddDebugMessage ( "Vehicle: 0x%p, VehicleData: 0x%p", pVehicle, dwVehicleData );
#endif

		// Set the initial seat as the driver
		EntityId seat = 0;

		// Is the player in the passenger seat?
		//if( GetState() == PLAYERSTATE_PASSENGER )
		//	seat = pNetworkVehicle->GetVehicle()->GetPedOccupiedSeat( GetPlayerPed()->GetPed() );

		// Does the vehicle already have a driver?
		if ( pNetworkVehicle->GetDriver () )
			seat = pNetworkVehicle->GetVehicle()->GetPedOccupiedSeat( GetPlayerPed()->GetPed() );

		// Are we getting in as the driver?
		if( seat == 0 )
			SetState( PLAYERSTATE_DRIVER );
		else
			SetState( PLAYERSTATE_PASSENGER );

#ifdef _DEBUG
		pCore->GetChat()->AddDebugMessage( "Seat: %d, Driver: 0x%p, State: %d", seat, pNetworkVehicle->GetDriver (), GetState () );
#endif

		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the vehicle id
		pBitStream.WriteCompressed( pNetworkVehicle->GetId() );

		// Write the seat
		pBitStream.WriteCompressed( seat );

		// Send to the server
		pCore->GetNetworkModule()->Call( RPC_ENTER_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );

#ifdef _DEBUG
		pCore->GetChat()->AddDebugMessage( "CLocalPlayer::OnEnterVehicle( %d, %d )", pNetworkVehicle->GetId(), seat );
#endif

		// Handle this enter with the network vehicle
		pNetworkVehicle->HandlePlayerEnter( this, seat );

		// Set the vehicle
		SetVehicle( pNetworkVehicle );

		// Set the seat
		SetSeat( seat );

		// Set the target data
		SetEnteringVehicle( pNetworkVehicle, seat );
	}
}

void CLocalPlayer::OnEnterVehicleDone( void )
{
	// Are we marked as entering a vehicle?
	if( IsEnteringVehicle() )
	{
		// Send RPC to the server
		pCore->GetNetworkModule()->Call( RPC_ENTER_VEHICLE_DONE, NULL, HIGH_PRIORITY, RELIABLE_ORDERED, true );

		// Mark as not entering vehicle
		SetEnteringVehicle( NULL, INVALID_ENTITY_ID );

#ifdef _DEBUG
		//pCore->GetChat()->AddDebugMessage( "CLocalPlayer::OnEnterVehicleDone( %d, %d )", GetVehicle()->GetId(), GetSeat() );
#endif
	}
}

void CLocalPlayer::OnLeaveVehicle( void )
{
	// Set the player state
	SetState( PLAYERSTATE_ONFOOT );

	// Is the vehicle instance valid?
	if( m_pVehicle )
	{
		// Get the seat of this player
		EntityId seat = m_seat;

		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the vehicle id
		pBitStream.WriteCompressed( m_pVehicle->GetId() );

		// Write the seat
		pBitStream.WriteCompressed( seat );

		// Write if we're exiting quickly!
		m_bFastExitVehicle ? pBitStream.Write1() : pBitStream.Write0();

		// Send to the server
		pCore->GetNetworkModule()->Call( RPC_EXIT_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );

#ifdef DEBUG
		pCore->GetChat()->AddDebugMessage( "CLocalPlayer::OnExitVehicle( %d, %d ) - Forcefully: %s", m_pVehicle->GetId(), seat, (IsBeingRemovedForcefully () ? "Yes" : "No") );
#endif

		// Are we not being removed forcefully?
		if ( !IsBeingRemovedForcefully () )
		{
			// Handle this enter with the network vehicle
			m_pVehicle->HandlePlayerExit( this, seat, false );

			// Do we need to respawn this vehicle? (Set to true when vehicle enters water)
			if( bRespawnVehicle && m_seat == 0 )
			{
				// Respawn the vehicle
				m_pVehicle->Respawn();

				// 
				bRespawnVehicle = false;
			}

			// Are we a passenger?
			if( m_seat > 0 )
			{
				// Restore the player controls
				LockControls( bOldControlState );

				// Restore the camera lock state
				pCore->GetCamera()->LockControl( bOldCameraState );

				//
				*(DWORD *)(m_pPlayerPed->GetPed() + 0x310) = 6;
			}
		}

		// Reset the vehicle
		SetVehicle( NULL );

		// Reset the seat
		SetSeat( INVALID_ENTITY_ID );

		// Reset the target data
		SetEnteringVehicle( NULL, INVALID_ENTITY_ID );

		// Reset the fast vehicle exit flag
		FlagForFastExitVehicle ( false );

		// Reset the forceful exit flag
		SetRemoveForcefully ( false );
	}
}

bool CLocalPlayer::OnTakeDamage ( void )
{
	// Are we not spawned?
	if ( !IsSpawned () )
		return false;

	// Send RPC to server
	pCore->GetNetworkModule()->Call( RPC_PLAYERDAMAGE, NULL, HIGH_PRIORITY, RELIABLE, true );

	return (pCore->GetClientScriptingManager()->GetEvents()->Call( "onTakeDamage" ).GetInteger() == 1);
}

void CLocalPlayer::HandleSpawn( bool bRespawn )
{
	// Are we respawning and not dead?
	if( bRespawn && !IsDead() )
		return;

	// Have we already just spawned?
	if( (SharedUtility::GetTime() - m_ulSpawnTime) < 500 )
		return;

	// Set the spawn time
	m_ulSpawnTime = SharedUtility::GetTime();

	// Reset the death time
	m_ulDeathTime = 0;

	// Set the state
	SetState( ePlayerState::PLAYERSTATE_ONFOOT );

	// Mark as not dead
	SetDead( false );

	// Mark as spawned
	SetSpawned( true );

	// Are we in a vehicle?
	if( IsInVehicle() )
	{
		// Handle this enter with the network vehicle
		m_pVehicle->HandlePlayerExit( this, m_seat, false );

		// Reset the vehicle
		SetVehicle( NULL );

		// Reset the seat
		SetSeat( INVALID_ENTITY_ID );
	}

	// Reset the player position and rotation
	Teleport ( CVector3() );
	SetRotation ( CVector3() );

	// Send RPC to server
	pCore->GetNetworkModule()->Call( RPC_PLAYER_SPAWN, NULL, HIGH_PRIORITY, RELIABLE, true );

	// Restore the camera (TODO: Use real game function to reset camera behind player!)
	pCore->GetCamera()->LockControl ( false );
	LockControls ( false );

	// Restore the camera control
	pCore->GetCamera()->LockControl ( bOldCameraState );

	// Restore the control state
	LockControls ( bOldCTRLState );
}

void CLocalPlayer::OnDeath( CNetworkPlayer * pKiller )
{
	// Are we not marked as dead?
	if( !IsDead() )
	{
		// Get the old camera lock state
		bOldCameraState = pCore->GetCamera()->IsLocked ();

		// Get the old control state
		bOldCTRLState = AreControlsLocked ();

		// Mark as dead
		SetDead( true );

		// Set the death time
		m_ulDeathTime = SharedUtility::GetTime();

		// Mark as not spawned
		SetSpawned( false );

		// Mark as not aiming
		SetAiming( false );

		// Mark as not shooting
		SetShooting( false );

		// Set the death time
		SetDeathTime( SharedUtility::GetTime() );

		// Remove all the weapons
		m_pPlayerPed->RemoveAllWeapons ();

		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the killerid
		pBitStream.WriteCompressed( (pKiller ? pKiller->GetId() : INVALID_ENTITY_ID) );

		// Send RPC to server
		pCore->GetNetworkModule()->Call( RPC_PLAYER_DEATH, &pBitStream, HIGH_PRIORITY, RELIABLE, true );

		// Fade out the screen
		pCore->GetHud()->FadeOut ( 3000 );

		// Fade out the sound
		pCore->GetGame()->FadeSound ( true, 3 );

		// Lock the camera control
		pCore->GetCamera()->LockControl ( true );

		// Lock the player controls
		LockControls ( true );
	}
}

void CLocalPlayer::OnReloadWeapon( void )
{
	// Are we not dead?
	if( !IsDead() )
	{
		// Send RPC to server
		//pCore->GetNetworkModule()->Call( RPC_PLAYER_RELOAD_WEAPON, NULL, HIGH_PRIORITY, RELIABLE, true );

#ifdef DEBUG
		pCore->GetChat()->AddDebugMessage( "CLocalPlayer::OnReloadWeapon()" );
#endif
	}
}

void CLocalPlayer::OnEnterWater( void )
{
	// Are we not dead and in a vehicle?
	if( !IsDead() && m_pVehicle )
	{
		// Mark the vehicle we exit to respawn
		bRespawnVehicle = true;
	}
}

bool CLocalPlayer::ProcessControls( unsigned int uMsg, WPARAM wParam )
{
	// Are we spawned and not dead?
	if ( IsSpawned () && !IsDead () )
	{
		// Was the M key released?
		if ( uMsg == WM_KEYDOWN && (DWORD)wParam == 0x4D )
		{
			// Open the map
			pCore->GetGame()->OpenMap ( !pCore->GetGame()->IsMapOpen () );
			return true;
		}
	}

	/*
	// Are we spawned and not dead?
	if( IsSpawned() && !IsDead() )
	{
		// Was the F key released?
		if( uMsg == WM_KEYUP && (DWORD)wParam == 0x46 )
		{
			//
			CNetworkVehicle * pVehicle = NULL;
			EntityId seat = 0;

			// Get the closest vehicle in range
			GetClosestVehicle( &pVehicle, &seat );

			// If there's already a driver in this vehicle
			if( pVehicle && pVehicle->GetDriver() )
			{
				LockControls( true );
				LockControls( false );
				return true;
			}
		}
	}
	*/

	/*
	// Are we spawned and not dead?
	if( IsSpawned() && !IsDead() )
	{
		// Was the F key released?
		if( uMsg == WM_KEYUP && (DWORD)wParam == 0x46 )
		{
			// Are we in a vehicle, but not driving?
			if( m_pVehicle && m_seat > 0 )
			{
				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Handling passenger exit..." );

				// Remove from the vehicle
				RemoveFromVehicle( m_pVehicle );

				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Removed from vehicle!" );

				// Restore the player controls
				LockControls( bOldControlState );

				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Old control state restored! (%s)", (bOldControlState ? "true" : "false") );

				// Restore the camera lock state
				pCore->GetCamera()->LockControl( bOldCameraState );

				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Old camera state resoted! (%s)", (bOldCameraState ? "true" : "false") );

				//
				*(DWORD *)(m_pPlayerPed->GetPed() + 0x310) = 6;

				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Reset 0x310" );

				return true;
			}

			//
			CNetworkVehicle * pVehicle = NULL;
			EntityId seat = 0;

			// Get the closest vehicle in range
			GetClosestVehicle( &pVehicle, &seat );

			// If there's already a driver in this vehicle
			if( pVehicle && pVehicle->GetDriver() )
			{
				CLogFile::Printf( "CLocalPlayer::ProcessControls() - Player pressed F and there's already a driver, processing passenger key..." );
				// Process the passenger key
				HandlePassengerKey();
				CLogFile::Printf( "CLocalPlayer::ProcessControls() - F key processed." );
				return true;
			}
		}
	}
	*/

	return false;
}

void CLocalPlayer::HandlePassengerKey( void )
{
	// Are we spawned and not dead and not already in a vehicle?
	if( IsSpawned() && !IsDead() && !m_pVehicle )
	{
		//
		CNetworkVehicle * pVehicle = NULL;
		EntityId seat = 0;

		// Get the closest vehicle in range
		GetClosestVehicle( &pVehicle, &seat );

		CLogFile::Printf( "CLocalPlayer::HandlePassengerKey() - Vehicle: 0x%p, Seat: %d", pVehicle, seat );

		// Did we get a closest vehicle and a seat?
		if( pVehicle && seat )
		{
			// Get the old control state
			bOldControlState = AreControlsLocked();

			// Get the old camera lock state
			bOldCameraState = pCore->GetCamera()->IsLocked();

			CLogFile::Printf( "CLocalPlayer::HandlePassengerKey() - bOldControlState: %s, bOldCameraState: %s", (bOldControlState ? "true" : "false"), (bOldCameraState ? "true" : "false") );

			// Lock the player controls
			LockControls( true );

			// Set the player state
			SetState( PLAYERSTATE_PASSENGER );

			CLogFile::Printf( "CLocalPlayer::HandlePassengerKey() - Putting player in passenger seat..." );

			// Get in the vehicle
			PutInVehicle( pVehicle, seat );

			CLogFile::Printf( "CLocalPlayer::HandlePassengerKey() - Done!" );

			// Unlock camera control
			if( !bOldCameraState ) // checking this because after we put in vehicle, it locks control :(
				pCore->GetCamera()->LockControl( false );

			// Reset the controls
			//*(DWORD *)(m_pPlayerPed->GetPed() + 0x310) = 6;
			//LockControls( bState );
		}
	}
}

void CLocalPlayer::GetClosestVehicle( CNetworkVehicle ** pNetworkVehicle, EntityId * seatId )
{
	// Are we spawned and not dead and not already in a vehicle?
	if( IsSpawned() && !IsDead() && !m_pPlayerPed->GetCurrentVehicle() )
	{
		//
		float fCurrentDistance = 2.68f;
		CVector3 vecVehiclePos;
		CNetworkVehicle * pClosestVehicle = NULL;

		// Get our position
		CVector3 vecCurrentPosition;
		GetPosition( &vecCurrentPosition );

		// Loop through all network vehicles
		for( int i = 0; i < MAX_VEHICLES; i++ )
		{
			// Is the current vehicle active?
			if( pCore->GetVehicleManager()->IsActive( i ) )
			{
				// Get a pointer to the current vehicle
				CNetworkVehicle * pCurrentVehicle = pCore->GetVehicleManager()->Get( i );

				// Get the vehicle position
				pCurrentVehicle->GetPosition( &vecVehiclePos );

				// Get the distance between the current vehicle and us
				float fDistance = Math::GetDistanceBetweenPoints( vecCurrentPosition, vecVehiclePos );

				// Is this distance less than the current distance?
				if( fDistance < fCurrentDistance )
				{
					// Set the current distance
					fCurrentDistance = fDistance;

					// Set the closest vehicle
					pClosestVehicle = pCurrentVehicle;
				}
			}
		}

		// Isn't there a vehicle in range?
		if( !pClosestVehicle )
			return;

		// Loop through all the passenger seats
		BYTE byteCurrentSeat = 0;
		for( BYTE i = 2; i < 5; i++ )
		{
			// Is there no one in this seat?
			if( !pClosestVehicle->GetVehicle()->GetSeatOccupant( i ) )
			{
				byteCurrentSeat = i;
				break;
			}
		}

		// Didn't we find a seat?
		if( !byteCurrentSeat )
			return;

		// Set the seat
		*seatId = byteCurrentSeat;

		// Set the vehicle
		*pNetworkVehicle = pClosestVehicle;
	}
}

void CLocalPlayer::LockControls( bool bLock, bool bForPlayAnim )
{
	// Is the playerped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		M2EntityData * pEntityData = m_pPlayerPed->GetPed()->m_pEntityData;

		_asm push bForPlayAnim;
		_asm push bLock;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__LockControls;
	}
}

bool CLocalPlayer::AreControlsLocked( void )
{
	bool bAreLocked = false;

	// Is the playerped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		M2EntityData * pEntityData = m_pPlayerPed->GetPed()->m_pEntityData;

		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__AreControlsLocked;
		_asm mov bAreLocked, al;
	}

	return bAreLocked;
}

void CLocalPlayer::StartSyncVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Are we already syncing this vehicle?
	if( IsSyncingVehicle( pNetworkVehicle ) )
		return;

	// Add this vehicle to the syncer list
	m_syncingVehicles.push_back( pNetworkVehicle );

	// Set the vehicle last syncer
	pNetworkVehicle->SetLastSyncer( this );

#ifdef _DEBUG
	pCore->GetChat()->AddDebugMessage( "CLocalPlayer::StartSyncVehicle - Server commanded us to start syncing vehicle %d", pNetworkVehicle->GetId() );
#endif
}

void CLocalPlayer::StopSyncVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Are we not syncing this vehicle?
	if( !IsSyncingVehicle( pNetworkVehicle ) )
		return;

	// Remove this vehicle from our syncing
	m_syncingVehicles.remove( pNetworkVehicle );

#ifdef _DEBUG
	pCore->GetChat()->AddDebugMessage( "CLocalPlayer::StopSyncVehicle - Server commanded us to stop syncing vehicle %d", pNetworkVehicle->GetId() );
#endif
}

bool CLocalPlayer::IsSyncingVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Loop over every vehicle we're syncing
	for( std::list< CNetworkVehicle* >::iterator iter = m_syncingVehicles.begin(); iter != m_syncingVehicles.end(); iter++ )
	{
		// Is this the vehicle we're looking for?
		if( (*iter) == pNetworkVehicle )
			return true;
	}

	return false;
}

bool CLocalPlayer::IsFullSyncNeeded ( void )
{
	// Get the current time
	unsigned long ulCurrentTime = SharedUtility::GetTime();

	// Has enough time passed since our last sync time?
	if ( ulCurrentTime >= (m_ulLastFullSyncTime + NETWORK_TICKRATE) )
	{
		// Update the last full sync time
		m_ulLastFullSyncTime = ulCurrentTime;
		return true;
	}
	
	return false;
}