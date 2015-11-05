/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkPlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

CNetworkPlayer::CNetworkPlayer( bool bLocalPlayer )
{
	DEBUG_TRACE("CNetworkPlayer::CNetworkPlayer");

	// Reset variables
	m_bLocalPlayer = bLocalPlayer;
	m_playerId = INVALID_ENTITY_ID;
	m_pPlayerPed = NULL;
	m_pPlayerModelManager = NULL;
	m_pMoveObject = NULL;
	m_pAimObject = NULL;
	m_pShootObject = NULL;
	m_bCharacterSlot = -1;
	m_bIsDead = false;
	m_bSpawned = false;
	m_dwDeathTime = 0;
	m_bAiming = false;
	m_bShooting = false;
	m_iControlStyle = 0;
	m_usPing = 0;
	m_dwWeaponSelect = NULL;
	m_playerState = PLAYERSTATE_UNKNOWN;
	m_lastState = M2Enums::eEntityState::IDLE;
	m_bMoveStyle = 3;
	m_pVehicle = NULL;
	m_seat = INVALID_ENTITY_ID;
	m_uiModelIndex = -1;
	m_pAttachedBlip = NULL;
	m_bBlipAttached = false;
	
	// Reset enter vehicle stuff
	m_VehicleEnter.m_pEnterAICommand = NULL;
	SetEnteringVehicle( NULL, INVALID_ENTITY_ID );
	m_VehicleEnter.m_bIsEntering = false;

	// Reset exiting vehicle stuff
	m_VehicleExit.m_pExitAICommand = NULL;
	m_VehicleExit.m_ulExitStartTime = 0;
	m_VehicleExit.m_bIsExiting = false;

	// Is this the localplayer?
	if( IsLocalPlayer() )
	{
		// Get the player ped instance
		m_pPlayerPed = new CM2Ped( IE::GetGame()->m_pLocalPed );

		// Set the localplayer model manager instance
		m_pPlayerModelManager = *(M2ModelMgr **)( 0x1AACDAC );

		// Update the model index
		m_uiModelIndex = Game::GetIdFromPlayerModel ( m_pPlayerModelManager->GetModelName () );
	}
}

CNetworkPlayer::~CNetworkPlayer( void )
{
	DEBUG_TRACE("CNetworkPlayer::~CNetworkPlayer");

	// Is this not the localplayer?
	if ( !IsLocalPlayer() )
	{
		// Destroy the playerped
		Destroy();
	}
}

void CNetworkPlayer::Create( void )
{
	DEBUG_TRACE("CNetworkPlayer::Create");

	// Is the ped already created?
	if( m_pPlayerPed )
		Destroy();

	// Get the model name and directory
	String strModel, strDirectory;
	Game::GetPlayerModelFromId( m_uiModelIndex, &strModel, &strDirectory );

#ifdef DEBUG
	CLogFile::Printf( "CNetworkPlayer< %d >::Create( %d ) - Building ped with model '%s' (%s)...", m_playerId, m_uiModelIndex, strModel.Get(), strDirectory.Get() );
#endif

	// Try load the player model
	m_pPlayerModelManager = CNetworkModelManager::Load( strDirectory.Get(), strModel.Get() );

	// Did the model fail to load?
	if( !m_pPlayerModelManager )
		return;

	// Create the playerped instance
	m_pPlayerPed = new CM2Ped( IE::CreatePed( m_pPlayerModelManager, CVector3() ) );

	// Did the playerped instance fail to create?
	if( !m_pPlayerPed )
		return;

	// Activate the entity
	m_pPlayerPed->Activate();

	// Set the ped invulnerable
	m_pPlayerPed->SetInvulnerable( true );

	// Give the ped infinite ammo
	m_pPlayerPed->SetInfiniteAmmo( true );

	// Set the initial health
	m_pPlayerPed->SetHealth( 720.0f );

	// Set the max health
	// todo

	// Create the player map icon
	m_iBlipId = pCore->GetGame()->GetNavigation()->RegisterIconEntity( m_pPlayerPed->GetEntity(), 0, 1 );

	// Mark as not dead
	SetDead( false );

	// Mark as spawned
	SetSpawned( true );

	// Set the initial state
	m_playerState = ePlayerState::PLAYERSTATE_ONFOOT;

	// Is the player in a vehicle?
	if( m_pVehicle && m_pVehicle->IsSpawned () )
	{
		// Put the player back into the vehicle
		PutInVehicle( m_pVehicle, m_seat );
		//EnterVehicle ( m_pVehicle, (M2Enums::eVehicleSeat)m_seat );

		// Reset vehicle interpolation
		//m_pVehicle->ResetInterpolation ();
	}

	// Do we have a blip attached to this player which hasn't been created?
	if ( m_pAttachedBlip && !m_bBlipAttached )
		m_pAttachedBlip->AttachToPlayer ( this );
}

void CNetworkPlayer::Destroy( void )
{
	DEBUG_TRACE("CNetworkPlayer::Destroy");

	// Is the playerped not created?
	if( !m_pPlayerPed )
		return;

	// Destroy the player icon
	pCore->GetGame()->GetNavigation()->UnregisterIconEntity( m_pPlayerPed->GetEntity() );

	// Remove the player from the vehicle
	if( m_pVehicle )
		m_pVehicle->HandlePlayerExit( this, (m_seat - 1), false );

	// Deactivate the entity
	m_pPlayerPed->Deactivate();

	// Detach any blips
	DetachBlip ();

	// Free the model
	CNetworkModelManager::Unload( m_pPlayerModelManager );

	// Delete the playerped instance
	SAFE_DELETE( m_pPlayerPed );

	// Mark as not dead
	SetDead( false );

	// Mark as not spawned
	SetSpawned( false );

	// Set the initial state
	m_playerState = ePlayerState::PLAYERSTATE_UNKNOWN;
}

void CNetworkPlayer::Respawn( void )
{
	DEBUG_TRACE("CNetworkPlayer::Respawn");

	// Is the localplayer not spawned?
	if( !pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	// Is the player inactive?
	if( !m_pPlayerPed )
		return;

	// Deactivate the player ped
	m_pPlayerPed->Deactivate();

	// Activate the player ped
	m_pPlayerPed->Activate();

	// Restore invulnerability
	m_pPlayerPed->SetInvulnerable( true );

	// Set the player health
	m_pPlayerPed->SetHealth( 720.0f );

	// Mark as not dead
	SetDead( false );

	// Mark as spawned
	SetSpawned( true );

	// Reset the playerstate
	m_playerState = ePlayerState::PLAYERSTATE_ONFOOT;
}

void CNetworkPlayer::SetModel( unsigned int uiModelIndex, bool bRebuild )
{
	DEBUG_TRACE("CNetworkPlayer::SetModel");

	// Store the model index
	m_uiModelIndex = uiModelIndex;

	// Is the playerped valid?
	if( m_pPlayerPed && IsSpawned() && bRebuild )
	{
		// Get the model name and directory
		String strModel, strDirectory;
		Game::GetPlayerModelFromId ( uiModelIndex, &strModel, &strDirectory );

		// Get the player current vehicle
		CNetworkVehicle * pCurrentVehicle = GetVehicle ();
		EntityId iSeat = GetSeat ();

		// Remove from vehicle if we're in one
		if( pCurrentVehicle )
			RemoveFromVehicle ( pCurrentVehicle );

		// Is this the localplayer?
		if( IsLocalPlayer() )
		{
			// Change the localplayer model
			m_pPlayerModelManager->ChangeModel ( strDirectory.Get(), strModel.Get() );
		}
		else
		{
			// Get position and rotation
			CVector3 vecPosition, vecRotation;
			GetPosition ( &vecPosition );
			GetRotation ( &vecRotation );

			// Create the player ped
			Create ();

			// Restore position and rotation
			Teleport ( vecPosition );
			SetRotation ( vecRotation );
		}

		// Put the player back in the vehicle
		if ( pCurrentVehicle )
			PutInVehicle ( pCurrentVehicle, iSeat );
	}
}

void CNetworkPlayer::ProcessVehicle( void )
{
	DEBUG_TRACE("CNetworkPlayer::ProcessVehicle");

	// Are we in a vehicle?
	if( m_pVehicle )
	{
		// Put the player in the vehicle
		PutInVehicle( m_pVehicle, m_seat );
	}
}

void CNetworkPlayer::HandlePlayerDeath( void )
{
	DEBUG_TRACE("CNetworkPlayer::HandlePlayerDeath");

	// Is the localplayer not spawned?
	if( !pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	// Is the player invalid?
	if( !IsSpawned() || !m_pPlayerPed )
		return;

	// Reset invulnerability
	m_pPlayerPed->SetInvulnerable( false );

	// Set the player health
	m_pPlayerPed->SetHealth( 0.0f );

	// Mark as dead
	SetDead( true );

	// Mark as not spawned
	SetSpawned( false );

	// Are we in a vehicle as driver?
	if( IsInVehicle() )
	{
		// Reset the vehicle interpolation
		m_pVehicle->ResetInterpolation();
	}

	// Set the playerstate
	m_playerState = ePlayerState::PLAYERSTATE_DEAD;
}

void CNetworkPlayer::SetPosition( CVector3 vecPosition, BYTE bMoveStyle, CVector3 vecEndDir )
{
	DEBUG_TRACE("CNetworkPlayer::SetPosition");

	// Store the position
	m_vecPosition = vecPosition;

	// Is this the localplayer?
	if( m_bLocalPlayer )
		return Teleport( vecPosition );

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Get the current ped position
		CVector3 vecCurrentPosition;
		GetPosition( &vecCurrentPosition );

		// Teleport to the position if it's too far away
		if( (vecPosition - vecCurrentPosition).Length() > 20.0f )
			return Teleport( vecPosition );

		// Find the move style from the control flags
		M2Enums::eMoveType moveType = M2Enums::eMoveType::E_SPRINT;

		// Get the engine move style
		if( bMoveStyle == 0 )
			moveType = M2Enums::eMoveType::E_WALK;
		else if( bMoveStyle == 1 )
			moveType = M2Enums::eMoveType::E_JOG;
		else if( bMoveStyle == 2 )
			moveType = M2Enums::eMoveType::E_SPRINT;

		// Is the old move object still working?
		/*if ( m_pMoveObject && !m_pMoveObject->IsDone () )
		{
			// Update the target position
			m_pMoveObject->SetTarget ( vecPosition, E_TYPE_MOVE );

			// Update the move object data
			m_pMoveObject->SetMoveTargetData ( vecEndDir, (BYTE)moveType ); // doesn't work due to memory protection!

			return;
		}*/

		// Delete the old move object
		SAFE_DELETE( m_pMoveObject );

		// Move the playerped
		m_pMoveObject = new CM2SyncObject( m_pPlayerPed->MoveVec( vecPosition, moveType, vecEndDir ) );
	}
}

void CNetworkPlayer::Teleport( CVector3 vecPosition )
{
	DEBUG_TRACE("CNetworkPlayer::Teleport");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
		m_pPlayerPed->SetPosition( vecPosition );
}

void CNetworkPlayer::GetPosition( CVector3 * vecPosition )
{
	DEBUG_TRACE("CNetworkPlayer::GetPosition");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
		m_pPlayerPed->GetPosition( vecPosition, true );
}

void CNetworkPlayer::SetRotation( CVector3 vecRotation )
{
	DEBUG_TRACE("CNetworkPlayer::SetRotation");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Convert the rotation to radians
		vecRotation.ToRadians();
		
		// Set the player rotation
		m_pPlayerPed->SetRotation( Quaternion( vecRotation ) );
	}
}

void CNetworkPlayer::GetRotation( CVector3 * vecRotation )
{
	DEBUG_TRACE("CNetworkPlayer::GetRotation");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Get the player rotation
		Quaternion quatRotation;
		m_pPlayerPed->GetRotation( &quatRotation );

		// Copy the rotation eular angles
		memcpy( vecRotation, &quatRotation.toEularAngles(), sizeof(CVector3) );

		// Convert the rotation to degrees
		vecRotation->FromRadians();
	}
}

void CNetworkPlayer::SetHealth( float fHealth )
{
	DEBUG_TRACE("CNetworkPlayer::SetHealth");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Set the ped health
		m_pPlayerPed->SetHealth( fHealth );

		// Are we killing the ped?
		if( fHealth <= 0.0f )
		{
			// Mark as dead
			SetDead( true );

			// Mark as not spawned
			SetSpawned( false );
		}
	}
}

float CNetworkPlayer::GetHealth( void )
{
	DEBUG_TRACE("CNetworkPlayer::GetHealth");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
		return m_pPlayerPed->GetHealth();

	return 0.0f;
}

void CNetworkPlayer::SetSelectedWeapon( DWORD dwWeapon, bool bUseAnimation )
{
	DEBUG_TRACE("CNetworkPlayer::SetSelectedWeapon");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Is the weapon id over 6?
		if( dwWeapon > 6 )
			dwWeapon += 10;
			
		// Check if our current weapon is old
		if( dwWeapon != m_dwWeaponSelect )
		{
			// Save the current weapon
			m_dwWeaponSelect = dwWeapon;

			// Set the weapon
			m_pPlayerPed->SetSelectedWeapon( dwWeapon, bUseAnimation );
		}
	}
}

DWORD CNetworkPlayer::GetSelectedWeapon( void )
{
	DEBUG_TRACE("CNetworkPlayer::GetSelectedWeapon");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
		return m_pPlayerPed->GetSelectedWeapon();

	return 0;
}

bool CNetworkPlayer::HasWeapon( DWORD dwWeapon )
{
	// Is the player ped valid?
	if( m_pPlayerPed )
		return m_pPlayerPed->HasItem( dwWeapon );

	return false;
}

void CNetworkPlayer::GiveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	DEBUG_TRACE("CNetworkPlayer::GiveWeapon");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Give the ped the weapon
		m_pPlayerPed->GiveWeapon( dwWeapon, dwAmmo );
	}
}

void CNetworkPlayer::RemoveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	DEBUG_TRACE("CNetworkPlayer::RemoveWeapon");

	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
		m_pPlayerPed->RemoveWeapon( dwWeapon, dwAmmo );
}

void CNetworkPlayer::SetCrouching( bool bCrouching )
{
	// todo
}

bool CNetworkPlayer::IsCrouching( void )
{
	// Is the player ped valid?
	//if( m_pPlayerPed )
	//	return m_pPlayerPed->IsCrouching();

	return false;
}

void CNetworkPlayer::UpdateAim( bool bAiming )
{
	DEBUG_TRACE("CNetworkPlayer::UpdateAim");

	// Is the playerped not valid?
	if( !m_pPlayerPed )
		return;

	// Should we start aiming?
	if( bAiming && !IsAiming() )
	{
		// Terminate the old aim object if it's valid
		if ( m_pAimObject )
			m_pAimObject->Terminate ();

		// Delete the old aim object
		SAFE_DELETE( m_pAimObject );

		// Create the initial aim sync object if needed
		m_pAimObject = new CM2SyncObject( m_pPlayerPed->AimAt( GetLookAt() ) );

		// Mark as aiming
		SetAiming( true );
	}
	// Should we stop aiming?
	else if( !bAiming && IsAiming() )
	{
		// Terminate the old aim object if it's valid
		if ( m_pAimObject )
			m_pAimObject->Terminate ();

		// Delete the old aim sync object
		SAFE_DELETE( m_pAimObject );

		// Mark as not aiming
		SetAiming( false );
	}
	// Should we update the aim position
	else if( bAiming && IsAiming() )
	{
		// Is the aim sync object valid?
		if( m_pAimObject && !m_pAimObject->IsDone() )
		{
			// Update the aim position
			m_pAimObject->SetTarget( GetLookAt(), eTargetType::E_TYPE_AIM );
		}
		else if( m_pAimObject && m_pAimObject->IsDone() )
		{
			// Mark as not aiming to rebuild the object next pulse
			SetAiming( false );
		}
	}
}

void CNetworkPlayer::UpdateShot( bool bShooting )
{
	DEBUG_TRACE("CNetworkPlayer::UpdateShot");

	// Is the playerped not valid?
	if( !m_pPlayerPed )
		return;

	// Should we start shooting?
	if( bShooting && !IsShooting() )
	{
		// Terminate the old shot object if it's valid
		if ( m_pShootObject )
			m_pShootObject->Terminate ();

		// Delete the old shoot object
		SAFE_DELETE( m_pShootObject );

		// Terminate the old aim object if it's valid
		if ( m_pAimObject )
			m_pAimObject->Terminate ();

		// Delete the old aim object
		SAFE_DELETE( m_pAimObject );

		// Create the initial shoot sync object if needed
		m_pShootObject = new CM2SyncObject( m_pPlayerPed->ShootAt( GetLookAt() ) );

		// Mark as shooting
		SetShooting( true );
	}
	// Should we stop shooting?
	else if( !bShooting && IsShooting() )
	{
		// Terminate the old shot object if it's valid
		if ( m_pShootObject )
			m_pShootObject->Terminate ();

		// Delete the old aim sync object
		SAFE_DELETE( m_pShootObject );

		// Mark as not shooting
		SetShooting( false );

		// Are we aiming?
		if( IsAiming() )
		{
			// Mark as not aiming
			SetAiming( false );

			// Update the aim now!
			UpdateAim( true );
		}
	}
	// Should we update the aim position
	else if( bShooting && IsShooting() )
	{
		// Is the aim sync object valid?
		if( m_pShootObject && !m_pShootObject->IsDone() )
		{
			// Update the aim position
			m_pShootObject->SetTarget( GetLookAt(), eTargetType::E_TYPE_SHOOT );
		}
		else if( m_pShootObject && m_pShootObject->IsDone() )
		{
			// Mark as not shooting
			SetShooting( false );
		}
	}
}

void CNetworkPlayer::GiveMoney( int iAmount )
{
	// Is the player ped valid?
	//if( m_pPlayerPed )
	//	m_pPlayerPed->GiveMoney ( iAmount );
}

void CNetworkPlayer::RemoveMoney( int iAmount )
{
	// Is the player ped valid?
	//if( m_pPlayerPed )
	//	m_pPlayerPed->RemoveMoney ( iAmount );
}

int CNetworkPlayer::GetMoney( void )
{
	// Is the player ped valid?
	//if( m_pPlayerPed )
	//	return m_pPlayerPed->GetMoney ();

	return 0;
}

void CNetworkPlayer::SetLookAt( CVector3 vecLookAt )
{
	// Store the vector
	memcpy( &m_vecLookAt, &vecLookAt, sizeof(CVector3) );
}

void CNetworkPlayer::TerminateSyncObjects( void )
{
	DEBUG_TRACE("CNetworkPlayer::TerminateSyncObjects");

	// Terminate the move object if it's valid
	if ( m_pMoveObject )
		m_pMoveObject->Terminate ();

	// Delete it
	SAFE_DELETE( m_pMoveObject );

	// Terminate the aim object if it's valid
	if ( m_pAimObject )
		m_pAimObject->Terminate ();

	// Delete it
	SAFE_DELETE( m_pAimObject );

	// Mark as not aiming
	SetAiming( false );

	// Terminate the shoot object if it's valid
	if ( m_pShootObject )
		m_pShootObject->Terminate ();

	// Delete it
	SAFE_DELETE( m_pShootObject );

	// Mark as not shooting
	SetShooting( false );
}

void CNetworkPlayer::SetEntityState( M2Enums::eEntityState state )
{
	// Is the player ped valid?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Has the player changed state?
		if( state != m_lastState )
		{
			// Store the new state
			m_lastState = state;
		}
	}
}

M2Enums::eEntityState CNetworkPlayer::GetEntityState( bool bLatest )
{
	// Is the player ped valid and we're getting the latest state?
	if( m_pPlayerPed && bLatest )
		return (M2Enums::eEntityState)m_pPlayerPed->GetState();

	return m_lastState;
}

void CNetworkPlayer::EnterVehicle( CNetworkVehicle * pVehicle, M2Enums::eVehicleSeat seat, bool bEnter )
{
	DEBUG_TRACE("CNetworkPlayer::EnterVehicle");

	// Are we already entering?
	if ( IsEnteringVehicle() )
	{
		CLogFile::Printf ( "WARNING: This player (%s) is already entering a vehicle, skipping!", GetNick().Get () );
		return;
	}

	// Is the player ped and network vehicle active?
	if( m_pPlayerPed && pVehicle && IsSpawned() )
	{
		// Clean up any sync objects
		TerminateSyncObjects ();

		// Should we put the player in the vehicle?
		if( bEnter )
		{
			// Remove the ped from the vehicle if we're already in one
			if( InternalIsInVehicle() )
				m_pPlayerPed->GetInOutVehicle( m_pPlayerPed->GetCurrentVehicle(), m_seat, false, true );

			// Get our current position
			CVector3 vecPosition;
			GetPosition( &vecPosition );

			// Get the vehicle position
			CVector3 vecVehiclePos;
			pVehicle->GetPosition( &vecVehiclePos );

			//
			bool bForced = false;

			// Is the vehicle too far away?
			if( Math::GetDistanceBetweenPoints( vecPosition, vecVehiclePos ) > 4.0f ) // 2.68 = max distance
				bForced = true;

			CLogFile::Printf ( "EnterVehicle - Forced: %s", (bForced ? "Yes" : "No") );

			// Is the vehicle invalid?
			if ( !pVehicle->GetVehicle() || !pVehicle->GetVehicle()->GetVehicle() )
			{
				CLogFile::Printf ( "EnterVehicle - Invalid game vehicle, skipping to prevent crash!" );
				return;
			}

			// Enter the vehicle
			m_VehicleEnter.m_pEnterAICommand = new CM2SyncObject( m_pPlayerPed->GetInOutVehicle( pVehicle->GetVehicle()->GetVehicle(), seat, true, bForced ) );

			CLogFile::Printf ( "Done!" );

			// Don't finish this sync object when we delete it
			m_VehicleEnter.m_pEnterAICommand->SetDoneOnDelete ( false );

			// Handle this enter with the network vehicle
			pVehicle->HandlePlayerEnter( this, (seat - 1) );
		}

		// Set the vehicle
		SetVehicle( pVehicle );

		// Set the seat
		SetSeat( seat );

		// Reset the target data
		SetEnteringVehicle( pVehicle, seat );

		// Set the player state
		m_playerState = (seat == 1 ? ePlayerState::PLAYERSTATE_DRIVER : ePlayerState::PLAYERSTATE_PASSENGER);
	}
}

void CNetworkPlayer::EnterVehicleDone( void )
{
	DEBUG_TRACE("CNetworkPlayer::EnterVehicleDone");

	// Is the playerped valid and entering a vehicle?
	if( m_pPlayerPed && IsEnteringVehicle() )
	{
		// Reset the target data
		SetEnteringVehicle( NULL, INVALID_ENTITY_ID );
		m_VehicleEnter.m_bIsEntering = false;

		// Reset
		m_VehicleExit.m_bIsExiting = false;
		m_VehicleExit.m_ulExitStartTime = 0;
		SAFE_DELETE( m_VehicleExit.m_pExitAICommand );
	}
}

void CNetworkPlayer::ExitVehicle( bool bExit, bool bQuickly )
{
	DEBUG_TRACE("CNetworkPlayer::ExitVehicle");

	// Are we not in a vehicle?
	if( !m_pVehicle )
		return;

	// Are we already exiting?
	if ( IsExitingVehicle() )
	{
		CLogFile::Printf ( "WARNING: This player (%s) is already exiting a vehicle, skipping!", GetNick().Get () );
		return;
	}

	// Is the player ped active?
	if( m_pPlayerPed && IsSpawned() )
	{
		// Should we remove the player from the vehicle?
		if( bExit )
		{
			// Are we in a vehicle?
			if( InternalIsInVehicle() )
			{
#ifdef DEBUG
				CLogFile::Printf( "CPlayer::ExitVehicle - Quickly? %s", (bQuickly ? "true" : "false") );
#endif

				// Reset the target data
				SetEnteringVehicle( NULL, INVALID_ENTITY_ID );
				m_VehicleEnter.m_bIsEntering = false;

				// Remove from vehicle
				C_SyncObject * pSyncObject = m_pPlayerPed->GetInOutVehicle( m_pPlayerPed->GetCurrentVehicle(), m_seat, bQuickly );

#ifdef DEBUG
				CLogFile::Printf ( "Exit vehicle sync object: 0x%p", pSyncObject );
#endif

				// Are we not quickly exiting?
				if( !bQuickly )
				{
					m_VehicleExit.m_pExitAICommand = new CM2SyncObject( pSyncObject );
					m_VehicleExit.m_bIsExiting = true;
					m_VehicleExit.m_ulExitStartTime = SharedUtility::GetTime ();
				}
			}

#ifdef DEBUG
			CLogFile::Print ( "Handling with vehicle..." );
#endif

			// Handle this enter with the network vehicle
			m_pVehicle->HandlePlayerExit( this, (m_seat - 1) );

#ifdef DEBUG
			CLogFile::Print ( "Done!" );
#endif
		}

		// Reset the vehicle
		SetVehicle( NULL );

		// Reset the seat
		SetSeat( INVALID_ENTITY_ID );

		// Reset the playerstate
		m_playerState = ePlayerState::PLAYERSTATE_ONFOOT;
	}
}

bool CNetworkPlayer::InternalIsInVehicle( void )
{
	DEBUG_TRACE("CNetworkPlayer::InternalIsInVehicle");

	// Is the player ped active?
	if( m_pPlayerPed && IsSpawned() )
		return (m_pPlayerPed->GetCurrentVehicle() != NULL);

	return false;
}

bool CNetworkPlayer::InternalIsInVehicle ( CM2Vehicle * pVehicle )
{
	// Is the player ped active?
	if( m_pPlayerPed && IsSpawned() )
		return (m_pPlayerPed->GetCurrentVehicle() == pVehicle->GetVehicle());

	return false;
}

void CNetworkPlayer::PutInVehicle( CNetworkVehicle * pVehicle, EntityId seatId )
{
	DEBUG_TRACE("CNetworkPlayer::PutInVehicle");

	// Is the player spawned and the vehicle valid?
	if( m_pPlayerPed && pVehicle && IsSpawned() && pVehicle->IsSpawned() )
	{
		bool bOldControlState;

		// Is this the localplayer?
		if ( IsLocalPlayer() )
		{
			// Get the old control state
			bOldControlState = ((CLocalPlayer *)this)->AreControlsLocked();

			// Lock the player controls
			((CLocalPlayer *)this)->LockControls( true );
		}

		// Does the target vehicle already have a driver?
		if ( pVehicle->GetDriver() )
		{
			// Todo: some checks to check the last sync time from the driver. If he's timed out of not even connected anymore, reset the driver and put this player in the vehicle
			CLogFile::Printf ( "WARNING: Got tasked to put a player (%d) in vehicle (%d), but there's already a driver (%d) in that vehicle!", m_playerId, pVehicle->GetId(), pVehicle->GetDriver()->GetId() );
			CLogFile::Printf ( "Internal driver ped pointer: 0x%p, Internal driver ped vehicle pointer: 0x%p", pVehicle->GetDriver()->GetPlayerPed()->GetPed(), pVehicle->GetDriver()->GetPlayerPed()->GetPed()->m_pCurrentVehicle );

			// Is the driver still in the vehicle?
			if ( pVehicle->GetDriver()->InternalIsInVehicle ( pVehicle->GetVehicle() ) )
				return;

			// Clean the network vehicle data
			pVehicle->HandlePlayerExit ( pVehicle->GetDriver (), 0 );

			CLogFile::Printf ( "We still have a driver set, but he's no longer inside the vehicle. Cleaned network vehicle driver pointer and continuing code.." );
		}

		// Terminate any sync objects if this is not the localplayer
		if ( !IsLocalPlayer() )
			TerminateSyncObjects ();

		// Put the player in the vehicle
		m_pPlayerPed->GetInOutVehicle( pVehicle->GetVehicle()->GetVehicle(), seatId, true, true );

		// Handle with the vehicle
		pVehicle->HandlePlayerEnter ( this, (seatId - 1) );

		// Set the vehicle
		SetVehicle( pVehicle );

		// Set the seat
		SetSeat( seatId );

		// Reset the target data
		SetEnteringVehicle( NULL, INVALID_ENTITY_ID );

		// Set the player state
		m_playerState = (seatId == 1 ? ePlayerState::PLAYERSTATE_DRIVER : ePlayerState::PLAYERSTATE_PASSENGER);

		// Is this the localplayer?
		if ( IsLocalPlayer() )
		{
			// Lock the player controls
			((CLocalPlayer *)this)->LockControls( bOldControlState );
		}
	}
}

void CNetworkPlayer::RemoveFromVehicle( CNetworkVehicle * pVehicle )
{
	DEBUG_TRACE("CNetworkPlayer::RemoveFromVehicle");

	// Is the player and vehicle valid?
	if( m_pPlayerPed && pVehicle && IsSpawned() )
	{
		// Remove the player from the vehicle
		m_pPlayerPed->GetInOutVehicle( pVehicle->GetVehicle()->GetVehicle(), m_seat, false, true );

		// Handle with the vehicle
		pVehicle->HandlePlayerExit ( this, m_seat );

		// Reset
		SetVehicle ( NULL );
		SetSeat ( INVALID_ENTITY_ID );
		m_playerState = ePlayerState::PLAYERSTATE_ONFOOT;
	}

	// Is the player exiting a vehicle?
	if( m_VehicleExit.m_bIsExiting )
	{
		// Reset
		m_VehicleExit.m_bIsExiting = false;
		m_VehicleExit.m_ulExitStartTime = 0;
		m_VehicleExit.m_pExitAICommand = NULL;
	}
}

void CNetworkPlayer::SetEnteringVehicle( CNetworkVehicle * pNetworkVehicle, EntityId seat )
{
	DEBUG_TRACE("CNetworkPlayer::SetEnteringVehicle");

	// Are we spawned?
	if( IsSpawned() )
	{
		// Is the target vehicle valid?
		if( pNetworkVehicle )
		{
			// Set the target vehicle
			m_VehicleEnter.m_pTargetVehicle = pNetworkVehicle;

			// Set the target seat
			m_VehicleEnter.m_targetSeat = seat;

			// Mark as entering
			m_VehicleEnter.m_bIsEntering = true;

			// Set the enter start time
			m_VehicleEnter.m_ulEnterStartTime = SharedUtility::GetTime();
		}
		else
		{
			// Reset
			m_VehicleEnter.m_pTargetVehicle = NULL;
			m_VehicleEnter.m_targetSeat = INVALID_ENTITY_ID;
			m_VehicleEnter.m_ulEnterStartTime = 0;
			m_VehicleEnter.m_bIsEntering = false;
			SAFE_DELETE( m_VehicleEnter.m_pEnterAICommand );
		}
	}
}

void CNetworkPlayer::AttachBlip ( CBlip * pBlip )
{
	// Is the blip valid?
	if ( pBlip )
	{
		// Store the blip pointer
		m_pAttachedBlip = pBlip;

		// Is the blip created?
		m_bBlipAttached = pBlip->IsCreated ();
	}
}

void CNetworkPlayer::DetachBlip ( void )
{
	// Do we have an attached blip?
	if ( m_pAttachedBlip )
	{
		// Detach the blip
		m_pAttachedBlip->Detach ();

		// Reset the attached blip pointer
		m_pAttachedBlip = NULL;

		// Mark as blip not attached
		m_bBlipAttached = false;
	}
}