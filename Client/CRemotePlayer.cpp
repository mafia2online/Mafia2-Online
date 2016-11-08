/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CRemotePlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CQuaternion.h"
#include "Math/CVector3.h"

#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CLocalPlayer.h"
#include "CNetworkPlayer.h"
#include "CNetworkVehicle.h"

#include "CSync.h"

#include "SharedUtility.h"

#include "CRemotePlayer.h"

#include "CLogFile.h"

CRemotePlayer::CRemotePlayer( void )
	: CNetworkPlayer( false )
	, m_onFootSync()
{
	DEBUG_LOG("CRemotePlayer::CRemotePlayer");
}

CRemotePlayer::~CRemotePlayer( void )
{
	DEBUG_LOG("CRemotePlayer::~CRemotePlayer");
}

void CRemotePlayer::Pulse( void )
{
	DEBUG_LOG("CRemotePlayer::Pulse");

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
						DEBUG_LOG ( "Warping player into vehicle..." );

						// Delete the AI command
						SAFE_DELETE( m_VehicleEnter.m_pEnterAICommand );

						// Put the player in the vehicle
						PutInVehicle( m_VehicleEnter.m_pTargetVehicle, m_VehicleEnter.m_targetSeat );

						// Mark as not entering vehicle
						SetEnteringVehicle( NULL, INVALID_ENTITY_ID );

						DEBUG_LOG ( "Done!" );
					}
				}
			}
		}
	}
}

void CRemotePlayer::StoreOnFootSync( const OnFootSync &onFootSync )
{
	DEBUG_LOG("CRemotePlayer::StoreOnFootSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Is the player position out of range?
	if ( IsPositionOutOfRange ( onFootSync.m_vecPosition ) )
		return;

	// Is the player in a vehicle and not exiting?
	if( InternalIsInVehicle() && !m_VehicleExit.m_bIsExiting )
	{
		DEBUG_LOG ( "Player %d is in vehicle, but we're getting on-foot sync, removing from vehicle (quickly)", m_playerId );

		// Remove the player from the vehicle, quickly
		RemoveFromVehicle( m_pVehicle );

		DEBUG_LOG ( "Done!" );
	}

	// Get the localplayer position
	CVector3 vecLocalPos;
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecLocalPos );

	// Is the player in range of the localplayer?
	if( (vecLocalPos - onFootSync.m_vecPosition).Length() < 200.0f )
	{
		// Set the move style
		SetMoveStyle( onFootSync.m_bControlState );

		// Is there any movement being done?
		if( (GetLastPosition() - onFootSync.m_vecPosition).Length() > 0.0001f )
		{
			// Set the position
			SetPosition ( onFootSync.m_vecPosition, onFootSync.m_bControlState, onFootSync.m_vecDirection );
		}

		// Set the player health
		SetHealth( onFootSync.m_fHealth );

		// Does the player not have this weapon selected?
		if( GetSelectedWeapon() != onFootSync.m_dwSelectedWeapon )
		{
			// Does this weapon need ammo?
			if( onFootSync.m_dwSelectedWeapon > 1 )
			{
				// Does the player not have the current weapon
				if( !HasWeapon( onFootSync.m_dwSelectedWeapon ) )
				{
					// Give the player the weapon
					GiveWeapon( onFootSync.m_dwSelectedWeapon, 250 );
				}
			}

			// Set the selected weapon
			SetSelectedWeapon( onFootSync.m_dwSelectedWeapon, true );

			/*if ( onFootSync.m_dwSelectedWeapon ) {
				TerminateSyncObjects ();
			}*/
		}

		// Set the look at position
		SetLookAt( onFootSync.m_vecLookAt );

		// Update their crouch
		if (m_bCrouching != onFootSync.m_bCrouching)
			SetCrouching(onFootSync.m_bCrouching);

		// Update their aim
		UpdateAim(onFootSync.m_bAiming);

		// Update their shot
		UpdateShot(onFootSync.m_bShooting);

		// Has the player model changed?
		if( GetModel() != onFootSync.m_uiModelIndex )
			SetModel( onFootSync.m_uiModelIndex );

		// Update the handModel
		if (GetHandModel() != onFootSync.m_iHandModel || GetHandModelHand() != onFootSync.m_iHand)
			SetHandModel(onFootSync.m_iHand, onFootSync.m_iHandModel);
	}
	else
	{
		// Teleport
		Teleport( onFootSync.m_vecPosition );

		// Cleanup
		TerminateSyncObjects();
	}

	m_onFootSync = onFootSync;
}

void CRemotePlayer::StoreInVehicleSync( EntityId vehicleId, const InVehicleSync &inVehicleSync )
{
	DEBUG_LOG("CRemotePlayer::StoreInVehicleSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Get the vehicle instance
	CNetworkVehicle * pVehicle = CCore::Instance()->GetVehicleManager()->Get(vehicleId);

	// Is the vehicle valid?
	if( pVehicle && pVehicle->IsSpawned () )
	{
		// Is the player not in a vehicle and not entering?
		if( !InternalIsInVehicle () && !IsEnteringVehicle () )
		{
			DEBUG_LOG ( "CRemotePlayer< %d, 0x%p >::StoreInVehicleSync - Player isn't in vehicle and has not been told to get in. Warping into vehicle %d (Current driver: 0x%p)...", m_playerId, this, pVehicle->GetId (), pVehicle->GetDriver () );

			// Put the player into the vehicle
			PutInVehicle( pVehicle, 1 );

			DEBUG_LOG ( "CRemotePlayer< %d, 0x%p >::StoreInVehicleSync - Done!", m_playerId, this );
		}

		// Store the vehicle sync
		pVehicle->StoreVehicleSync( inVehicleSync );
	}
}

void CRemotePlayer::StorePassengerSync( const InPassengerSync &passengerSync )
{
	DEBUG_LOG("CRemotePlayer::StorePassengerSync");

	// Is the playerped instance invalid ?
	if( !m_pPlayerPed )
		return;

	// Is the playerped not spawned?
	if( !IsSpawned() )
		return;

	// Update our health
	if( GetHealth() != passengerSync.m_fHealth )
		SetHealth ( passengerSync.m_fHealth );
}

bool CRemotePlayer::IsPositionOutOfRange ( CVector3 vecPos )
{
	// Don't accept the position if they're out of the map
	if ( (vecPos.fX > 7000.0f || vecPos.fX < -7000.0f) || (vecPos.fY > 7000.0f || vecPos.fY < -7000.0f) || (vecPos.fZ > 7000.0f || vecPos.fZ < -7000.0f) )
		return true;

	return false;
}