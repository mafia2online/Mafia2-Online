/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CRemotePlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

CRemotePlayer::CRemotePlayer( void ) : CNetworkPlayer( false )
{
	DEBUG_TRACE("CRemotePlayer::CRemotePlayer");

	// Invalidate the on-foot sync data
	m_pLastOnFootSync = NULL;
}

CRemotePlayer::~CRemotePlayer( void )
{
	DEBUG_TRACE("CRemotePlayer::~CRemotePlayer");
}

void CRemotePlayer::Pulse( void )
{
	DEBUG_TRACE("CRemotePlayer::Pulse");

	// Are we spawned?
	if ( IsSpawned() )
	{
		// Is the player entering a vehicle?
		if( IsEnteringVehicle() )
		{
			// Has enough time passed?
			if( (SharedUtility::GetTime() - m_VehicleEnter.m_ulEnterStartTime) >= TIME_BEFORE_ENTER_VEHICLE_WARP )
			{
				// Is the AI commnad valid?
				if( m_VehicleEnter.m_pEnterAICommand )
				{
					// Is the enter command not finished?
					if( !m_VehicleEnter.m_pEnterAICommand->IsDone() )
					{
						CLogFile::Print ( "Warping player into vehicle..." );

						// Delete the AI command
						SAFE_DELETE( m_VehicleEnter.m_pEnterAICommand );

						// Put the player in the vehicle
						PutInVehicle( m_VehicleEnter.m_pTargetVehicle, m_VehicleEnter.m_targetSeat );

						// Mark as not entering vehicle
						SetEnteringVehicle( NULL, INVALID_ENTITY_ID );

						CLogFile::Print ( "Done!" );

#ifdef _DEBUG
						pCore->GetChat()->AddDebugMessage( "CRemotePlayer::Pulse - The enter vehicle AI command took too long. Warped the player into the vehicle." );
#endif
					}
				}
			}
		}
	}
}

void CRemotePlayer::StoreOnFootSync( OnFootSync * onFootSync )
{
	DEBUG_TRACE("CRemotePlayer::StoreOnFootSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Is the player position out of range?
	if ( IsPositionOutOfRange ( onFootSync->m_vecPosition ) )
		return;

	// Is the player in a vehicle and not exiting?
	if( InternalIsInVehicle() && !m_VehicleExit.m_bIsExiting )
	{
		CLogFile::Printf ( "Player %d is in vehicle, but we're getting on-foot sync, removing from vehicle (quickly)", m_playerId );

		// Remove the player from the vehicle, quickly
		RemoveFromVehicle( m_pVehicle );

		CLogFile::Print ( "Done!" );
	}

	// Get the localplayer position
	CVector3 vecLocalPos;
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecLocalPos );

	// Is the player in range of the localplayer?
	if( (vecLocalPos - onFootSync->m_vecPosition).Length() < 200.0f )
	{
		// Set the move style
		SetMoveStyle( onFootSync->m_bControlState );

		// Is there any movement being done?
		if( onFootSync->m_vecPosition != GetLastPosition() )
		{
			// Set the position
			SetPosition ( onFootSync->m_vecPosition, onFootSync->m_bControlState, onFootSync->m_vecDirection );
		}

		// Set the player health
		SetHealth( onFootSync->m_fHealth );

		// Does the player not have this weapon selected?
		if( GetSelectedWeapon() != onFootSync->m_dwSelectedWeapon )
		{
			// Does this weapon need ammo?
			if( onFootSync->m_dwSelectedWeapon > 1 )
			{
				// Does the player not have the current weapon
				if( !HasWeapon( onFootSync->m_dwSelectedWeapon ) )
				{
					// Give the player the weapon
					GiveWeapon( onFootSync->m_dwSelectedWeapon, 250 );
				}
			}

			// Set the selected weapon
			SetSelectedWeapon( onFootSync->m_dwSelectedWeapon, true );

			// Cleanup sync objects
			TerminateSyncObjects ();
		}

		// Set the look at position
		SetLookAt( onFootSync->m_vecLookAt );

		// Update their shot
		UpdateShot( onFootSync->m_bShooting );

		// Are we not shooting?
		if( !onFootSync->m_bShooting )
		{
			// Update their aim
			UpdateAim( onFootSync->m_bAiming );
		}

		// Has the player model changed?
		if( GetModel() != onFootSync->m_uiModelIndex )
			SetModel( onFootSync->m_uiModelIndex );
	}
	else
	{
		// Teleport
		Teleport( onFootSync->m_vecPosition );

		// Cleanup
		TerminateSyncObjects();
	}

	// Store the sync packet
	m_pLastOnFootSync = onFootSync;
}

void CRemotePlayer::StoreInVehicleSync( EntityId vehicleId, InVehicleSync * inVehicleSync )
{
	DEBUG_TRACE("CRemotePlayer::StoreInVehicleSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = pCore->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle valid?
	if( pVehicle && pVehicle->IsSpawned () )
	{
		// Is the player not in a vehicle and not entering?
		if( !InternalIsInVehicle () && !IsEnteringVehicle () )
		{
			CLogFile::Printf ( "CRemotePlayer< %d, 0x%p >::StoreInVehicleSync - Player isn't in vehicle and has not been told to get in. Warping into vehicle %d (Current driver: 0x%p)...", m_playerId, this, pVehicle->GetId (), pVehicle->GetDriver () );

			// Put the player into the vehicle
			PutInVehicle( pVehicle, 1 );

			CLogFile::Printf ( "CRemotePlayer< %d, 0x%p >::StoreInVehicleSync - Done!", m_playerId, this );
		}

		// Store the vehicle sync
		pVehicle->StoreVehicleSync( *inVehicleSync );
	}
}

void CRemotePlayer::StorePassengerSync( InPassengerSync * passengerSync )
{
	DEBUG_TRACE("CRemotePlayer::StorePassengerSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Update our health
	if( GetHealth() != passengerSync->m_fHealth )
		SetHealth ( passengerSync->m_fHealth );
}

bool CRemotePlayer::IsPositionOutOfRange ( CVector3 vecPos )
{
	// Don't accept the position if they're out of the map
	if ( (vecPos.fX > 7000.0f || vecPos.fX < -7000.0f) || (vecPos.fY > 7000.0f || vecPos.fY < -7000.0f) || (vecPos.fZ > 7000.0f || vecPos.fZ < -7000.0f) )
		return true;
	
	return false;
}