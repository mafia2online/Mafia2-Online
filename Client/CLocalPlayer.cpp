/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLocalPlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"
#include "Math/CMaths.h"
#include "CString.h"

#include "CM2Hud.h"
#include "engine/CM2ModelManager.h"
#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "engine/CM2Vehicle.h"
#include "CM2Camera.h"

#include "CModelManager.h"

#include "CMafia.h"
#include "Game/CGame.h"

#include "CPlayerManager.h"
#include "CVehicleManager.h"

#include "CNetworkPlayer.h"
#include "CNetworkVehicle.h"

#include "CEvents.h"
#include "Scripting/CSquirrelArguments.h"
#include "Scripting/CScriptingManager.h"
#include "CClientScriptingManager.h"

#include "CNetworkModule.h"

#include "CSync.h"
#include "Network/NetRPCs.h"
#include "SharedUtility.h"

#include "PacketPriority.h"

#include "CLocalPlayer.h"

#include "CLogFile.h"

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
	m_oldMoveState = -1;
	m_bRenderNametags = true;
	m_bRenderHealthbar = true;
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
		CCore::Instance()->SetConnectionProblem(true);

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
			CCore::Instance()->GetHud()->FadeOut(0);
			CCore::Instance()->GetGame()->FadeSound(true, 0);

			// Reset position
			Teleport ( CVector3() );
			SetRotation ( CVector3() );

			// Kill the player
			SetHealth ( 0.0f );
		}

		// Return so we don't overwrite the last good position with invalid positions
		return;
	}

	// Store controls
	M2PlayerControls playerControls = m_pPlayerPed->GetPed()->m_playerControls;

	// Is our move state still the same ?
	int newState = playerControls.m_playerMovementState;

	// State changed and not locked ?
	if (m_oldMoveState != newState && CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->AreControlsLocked() == false)
	{
		// Call the event
		if (m_oldMoveState != -1){
			CSquirrelArguments pArguments;

			pArguments.push(CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId());
			pArguments.push(m_oldMoveState);
			pArguments.push(newState);

			CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call("onClientPlayerMoveStateChange", &pArguments);
		}
		// Update stored state
		m_oldMoveState = newState;
	}

	// Save this current position and rotation
	memcpy ( &m_vecLastGoodPosition, &vecCurrentPosition, sizeof ( CVector3 ) );
	memcpy ( &m_vecLastGoodRotation, &vecCurrentRotation, sizeof ( CVector3 ) );

	// Is player flagged as shooting ?
	if ( CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsShooting() )
	{
		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// How much time has passed since the shot
		if ( m_uMarkTimeStartShooting + 200 < ulCurrentTime )
		{
			// Stop shooting
			CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->SetShooting( false );
		}
	}

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
		CCore::Instance()->GetHud()->FadeIn(1000);

		// Fade the sound back in
		CCore::Instance()->GetGame()->FadeSound(false, 1);

		// Activate the player ped
		m_pPlayerPed->Activate ();

		// Restore the camera (I don't understand why it works. Correcting a flying camera after death)
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(true);
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(false);
	}

	// Are we spawned, in a vehicle and typing?
	if (IsSpawned() && IsInVehicle())
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
	if( !CCore::Instance()->GetNetworkModule()->IsConnected() )
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Construct a new bitstream
	RakNet::BitStream bitStream;

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

	// Get the player selected weapon bullet
	onFootSync.m_iSelectedWeaponBullet = GetSelectedWeaponBullet();

	// Get the player control state
    onFootSync.m_bControlState = m_pPlayerPed->GetControlState ();

	// Get the look at position
	CCore::Instance()->GetCamera()->GetLookAt(&onFootSync.m_vecLookAt);

	// Get the aiming state
	onFootSync.m_bAiming = m_pPlayerPed->IsAiming ();

	// Get the shooting state
	onFootSync.m_bShooting = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsShooting();

	// Get the crouching state
	onFootSync.m_bCrouching = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsCrouching();

	// Write the model index
	onFootSync.m_uiModelIndex = GetModel();

	// Write the hand
	onFootSync.m_iHand = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetHandModelHand();

	// Write the handModel
	onFootSync.m_iHandModel = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetHandModelHand();

	// Write the sync structure into the bitstream
	bitStream.Write( (char *)&onFootSync, sizeof(OnFootSync) );

	const RakNet::RakString strAnimStyleName(CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetAnimStyleName().Get());
	bitStream.Write(strAnimStyleName);

	const RakNet::RakString strAnimStyleDirectory(CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetAnimStyleDirectory().Get());
	bitStream.Write(strAnimStyleDirectory);

	// Send the bitstream to the server
	CCore::Instance()->GetNetworkModule()->Call(RPC_PLAYER_SYNC, &bitStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, true);
}

void CLocalPlayer::SendInVehicleSync( void )
{
	// Are we not connected to the network?
	if (!CCore::Instance()->GetNetworkModule()->IsConnected())
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = CCore::Instance()->GetVehicleManager()->GetFromGameGUID(GetPlayerPed()->GetCurrentVehicle()->m_dwGUID);

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

	// Get the vehicle beacon light state
	inVehicleSync.m_bBeaconLightState = pVehicle->GetVehicle()->IsBeaconLightOn();

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
	inVehicleSync.m_szPlateText[ MAX_LICENSE_PLATE_LENGTH ] = '\0';

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

	// Get the indicator state
	inVehicleSync.m_bLeftIndicator = pVehicle->GetVehicle()->IsIndicatorLightsOn(0);
	inVehicleSync.m_bRightIndicator = pVehicle->GetVehicle()->IsIndicatorLightsOn(1);

	// Get the taxi light state
	inVehicleSync.m_bTaxiLight = pVehicle->GetVehicle()->IsTaxiLightOn();

	// Write the sync structure into the bitstream
	pBitStream.Write( (char *)&inVehicleSync, sizeof(InVehicleSync) );

	// Send the bitstream to the server
	CCore::Instance()->GetNetworkModule()->Call(RPC_VEHICLE_SYNC, &pBitStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, true);
}

void CLocalPlayer::SendPassengerSync( void )
{
	// Are we not connected to the network?
	if (!CCore::Instance()->GetNetworkModule()->IsConnected())
		return;

	// Are we dead?
	if( IsDead() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = CCore::Instance()->GetVehicleManager()->GetFromGameGUID( GetPlayerPed()->GetCurrentVehicle()->m_dwGUID );

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

	// Get the player selected weapon bullet
	passengerSync.m_iSelectedWeaponBullet = GetSelectedWeaponBullet();

	// Write the sync structure to the bitstream
	pBitStream.Write( (char *)&passengerSync, sizeof(InPassengerSync) );

	// Send the bitstream to the server
	CCore::Instance()->GetNetworkModule()->Call(RPC_PASSENGER_SYNC, &pBitStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, true);
}

void CLocalPlayer::SendUnoccupiedVehicleSync( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( pNetworkVehicle->GetId() );

	// Construct a new unoccupied vehicle sync structure
	UnoccupiedVehicleSync unoccupiedVehicleSync;

	// Get the vehicle position
	pNetworkVehicle->GetPosition( &unoccupiedVehicleSync.m_vecPosition );

	// Get the vehicle rotation
	pNetworkVehicle->GetRotation( &unoccupiedVehicleSync.m_vecRotation );

	// Write the sync structure to the bitstream
	pBitStream.Write( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );

	// Send the bitstream to the server
	CCore::Instance()->GetNetworkModule()->Call(RPC_UNOCCUPIED_SYNC, &pBitStream, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, true);
}

void CLocalPlayer::Ping ( void )
{
	// Send a packet to the server
	CCore::Instance()->GetNetworkModule()->Call ( RPC_PLAYERPING, NULL, MEDIUM_PRIORITY, RELIABLE_ORDERED, true );
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
	CCore::Instance()->SetConnectionProblem ( false );
}

void CLocalPlayer::OnEnterVehicle( void )
{
	// Get the vehicle instance from the vehicle guid
	CNetworkVehicle * pNetworkVehicle = CCore::Instance()->GetVehicleManager()->GetFromGameGUID( GetPlayerPed()->GetCurrentVehicle()->m_dwGUID );

	// Is the vehicle instance valid?
	if( pNetworkVehicle )
	{
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

		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the vehicle id
		pBitStream.WriteCompressed( pNetworkVehicle->GetId() );

		// Write the seat
		pBitStream.WriteCompressed( seat );

		// Send to the server
		CCore::Instance()->GetNetworkModule()->Call( RPC_ENTER_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );

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
		CCore::Instance()->GetNetworkModule()->Call( RPC_ENTER_VEHICLE_DONE, NULL, HIGH_PRIORITY, RELIABLE_ORDERED, true );

		// Mark as not entering vehicle
		SetEnteringVehicle( NULL, INVALID_ENTITY_ID );
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
		CCore::Instance()->GetNetworkModule()->Call( RPC_EXIT_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );

		// Are we not being removed forcefully?
		if ( !IsBeingRemovedForcefully () )
		{
			// Handle this enter with the network vehicle
			m_pVehicle->HandlePlayerExit( this, seat, false );

			// Are we a passenger?
			if( m_seat > 0 )
			{
				// Restore the player controls
				LockControls( bOldControlState );

				// Restore the camera lock state
				CCore::Instance()->GetCamera()->LockControl( bOldCameraState );

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
	CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYERDAMAGE, NULL, HIGH_PRIORITY, RELIABLE, true );

	return (CCore::Instance()->GetClientScriptingManager()->GetEvents()->Call( "onTakeDamage" ).GetInteger() == 1);
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
	CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYER_SPAWN, NULL, HIGH_PRIORITY, RELIABLE, true );

	// Restore the camera (TODO: Use real game function to reset camera behind player!)
	CCore::Instance()->GetCamera()->LockControl ( false );
	LockControls ( false );

	// Restore the camera control
	CCore::Instance()->GetCamera()->LockControl ( bOldCameraState );

	// Restore the control state
	LockControls ( bOldCTRLState );
}

void CLocalPlayer::OnDeath( CNetworkPlayer * pKiller )
{
	// Are we not marked as dead?
	if( !IsDead() )
	{
		// Get the old camera lock state
		bOldCameraState = CCore::Instance()->GetCamera()->IsLocked ();

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
		CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYER_DEATH, &pBitStream, HIGH_PRIORITY, RELIABLE, true );

		// Fade out the screen
		CCore::Instance()->GetHud()->FadeOut ( 3000 );

		// Fade out the sound
		CCore::Instance()->GetGame()->FadeSound ( true, 3 );

		// Lock the camera control
		CCore::Instance()->GetCamera()->LockControl ( true );

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
		//CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYER_RELOAD_WEAPON, NULL, HIGH_PRIORITY, RELIABLE, true );
	}
}

void CLocalPlayer::OnEnterWater( void )
{
	// Are we not dead and in a vehicle?
	if( !IsDead() && m_pVehicle )
	{
		// Mark the vehicle we exit to respawn
		// TODO(inlife): destroy vehicle ?
	}
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

		DEBUG_LOG( "CLocalPlayer::HandlePassengerKey() - Vehicle: 0x%p, Seat: %d", pVehicle, seat );

		// Did we get a closest vehicle and a seat?
		if( pVehicle && seat )
		{
			// Get the old control state
			bOldControlState = AreControlsLocked();

			// Get the old camera lock state
			bOldCameraState = CCore::Instance()->GetCamera()->IsLocked();

			DEBUG_LOG( "CLocalPlayer::HandlePassengerKey() - bOldControlState: %s, bOldCameraState: %s", (bOldControlState ? "true" : "false"), (bOldCameraState ? "true" : "false") );

			// Lock the player controls
			LockControls( true );

			// Set the player state
			SetState( PLAYERSTATE_PASSENGER );

			DEBUG_LOG( "CLocalPlayer::HandlePassengerKey() - Putting player in passenger seat..." );

			// Get in the vehicle
			PutInVehicle( pVehicle, seat );

			DEBUG_LOG( "CLocalPlayer::HandlePassengerKey() - Done!" );

			// Unlock camera control
			if( !bOldCameraState ) // checking this because after we put in vehicle, it locks control :(
				CCore::Instance()->GetCamera()->LockControl( false );

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
			if( CCore::Instance()->GetVehicleManager()->IsActive( i ) )
			{
				// Get a pointer to the current vehicle
				CNetworkVehicle * pCurrentVehicle = CCore::Instance()->GetVehicleManager()->Get( i );

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
		C_HumanScript * pHumanScript = m_pPlayerPed->GetPed()->m_pHumanScript;

		_asm push bForPlayAnim;
		_asm push bLock;
		_asm mov ecx, pHumanScript;
		_asm call COffsets::FUNC_CHuman__LockControls;
	}
}

bool CLocalPlayer::AreControlsLocked( void )
{
	bool bAreLocked = false;

	// Is the playerped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		C_HumanScript * pHumanScript = m_pPlayerPed->GetPed()->m_pHumanScript;

		_asm mov ecx, pHumanScript;
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
