/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkVehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore		* pCore;

CNetworkVehicle::CNetworkVehicle( void )
{
	DEBUG_TRACE("CNetworkVehicle::CNetworkVehicle");

	// Reset variables
	m_vehicleId = INVALID_ENTITY_ID;
	m_uiModelIndex = -1;
	m_pVehicle = NULL;
	m_pVehicleModelManager = NULL;
	m_bSpawned = false;
	m_bProcessSyncOnSpawn = false;
	m_bSpawnProcessed = false;
	m_pLastSyncer = NULL;
	m_ulSpawnTime = 0;
	m_pAttachedBlip = NULL;
	m_bBlipAttached = false;

	// Reset occupants
	m_pDriver = NULL;
	memset( m_pPassenger, NULL, sizeof(m_pPassenger) );

	// Reset the interpolation
	ResetInterpolation();
}

CNetworkVehicle::~CNetworkVehicle( void )
{
	DEBUG_TRACE("CNetworkVehicle::~CNetworkVehicle");

	// Handle this destroy with the localplayer
	//pCore->GetPlayerManager()->GetLocalPlayer()->StopSyncVehicle( this );

	// Destroy the vehicle
	Destroy ();
}

void CNetworkVehicle::SetModel( unsigned int uiModelIndex, bool bRebuild )
{
	DEBUG_TRACE("CNetworkVehicle::SetModel");

	// Set the model index
	m_uiModelIndex = uiModelIndex;

	// Are we rebulding the model?
	if( bRebuild )
	{
		// todo
	}
}

void CNetworkVehicle::Create( void )
{
	DEBUG_TRACE("CNetworkVehicle::Create");

	// Destroy the vehicle if it's already created
	if( m_pVehicle )
		Destroy ();

	// Get the vehicle model
	String strModel;
	Game::GetVehicleModelFromId( m_uiModelIndex, &strModel );

#ifdef DEBUG
	CLogFile::Printf( "CNetworkVehicle< %d >::Create( %d ) - Building vehicle with model '%s'...", m_vehicleId, m_uiModelIndex, strModel.Get() );
#endif

	// Try load the player model
	m_pVehicleModelManager = CNetworkModelManager::Load( SDS_LOAD_DIR_CARS, strModel.Get() );

	// Did the model fail to load?
	if( !m_pVehicleModelManager )
		return;

	// Create the vehicle instance
	m_pVehicle = new CM2Vehicle ( IE::CreateVehicle ( m_pVehicleModelManager, m_vecSpawnPosition ) );

#ifdef DEBUG
	CLogFile::Printf ( "CNetworkVehicle< %d >::Create( ) - Wrapper Vehicle: 0x%p, Engine vehicle: 0x%p", m_vehicleId, m_pVehicle, (m_pVehicle ? m_pVehicle->GetVehicle () : NULL) );
#endif

	// Get the spawn colour
	GetColour ( &m_primarySpawnColour, &m_secondarySpawnColour );

#ifdef DEBUG
	CLogFile::Printf ( "CNetworkVehicle< %d >::Create( ) - 1", m_vehicleId );
#endif

	// Reset interpolation
	ResetInterpolation();

#ifdef DEBUG
	CLogFile::Printf ( "CNetworkVehicle< %d >::Create( ) - 2", m_vehicleId );
#endif

	// Set the spawned time
	m_ulSpawnTime = SharedUtility::GetTime();
	m_bSpawnProcessed = true;

#ifdef DEBUG
	CLogFile::Printf ( "CNetworkVehicle< %d >::Create( ) - Done", m_vehicleId );
#endif

	// Do we have a blip attached to this vehicle which hasn't been created?
	if ( m_pAttachedBlip && !m_bBlipAttached )
	{
		// Attach the blip to the vehicle
		m_pAttachedBlip->AttachToVehicle ( this );

		// Mark as blip attached
		m_bBlipAttached = true;
	}
}

void CNetworkVehicle::Destroy( void )
{
	DEBUG_TRACE("CNetworkVehicle::Destroy");

	// Is the vehicle instance invalid?
	if( !m_pVehicle )
		return;

	// Loop over all seats in the vehicles
	M2Ped * pSeatPed = NULL;
	CNetworkPlayer * pPlayer = NULL;
	for( int i = 0; i < GetVehicle()->GetTotalSeats(); i++ )
	{
		// Get the current seat occupant
		pSeatPed = m_pVehicle->GetSeatOccupant ( ( i + 1 ) );

		// Is someone in this seat?
		if( pSeatPed )
		{
			// Get the network player
			pPlayer = (pSeatPed == IE::GetGame()->m_pLocalPed ? (CNetworkPlayer *)pCore->GetPlayerManager()->GetLocalPlayer() : (CNetworkPlayer *)pCore->GetPlayerManager()->GetFromGameGUID ( pSeatPed->m_dwGUID ));

			// Is the player instance valid?
			if( pPlayer )
			{
				// Is this the localplayer?
				if ( pPlayer->IsLocalPlayer () )
				{
					// Flag for forceful remove
					((CLocalPlayer *)pPlayer)->SetRemoveForcefully ( true );

					// Flag the localplayer for fast exit vehicle
					((CLocalPlayer *)pPlayer)->FlagForFastExitVehicle ( true );
				}

				// Remove the player from the vehicle
				pPlayer->RemoveFromVehicle ( this );
			}
		}
	}

	// Deactivate the vehicle
	m_pVehicle->Deactivate ();

	// Detach any blips
	DetachBlip ();

	// Free the model
	CNetworkModelManager::Unload ( m_pVehicleModelManager );

	// Delete the vehicle instance
	SAFE_DELETE( m_pVehicle );

	// Mark as not spawned
	SetSpawned ( false );
}

void CNetworkVehicle::Respawn( void )
{
	DEBUG_TRACE("CNetworkVehicle::Respawn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Create a new bitstream
		RakNet::BitStream bitStream;

		// Write the vehicle id
		bitStream.WriteCompressed( m_vehicleId );

		// Send it to the server
		pCore->GetNetworkModule()->Call( RPC_RESPAWNVEHICLE, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, true );
	}
}

void CNetworkVehicle::HandleRespawn( void )
{
	DEBUG_TRACE("CNetworkVehicle::HandleRespawn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Deactivate the vehicle
		m_pVehicle->Deactivate();

		// Activate the vehicle
		m_pVehicle->Activate();

		// Repair the vehicle
		m_pVehicle->Repair();

		// Set the default colour
		m_pVehicle->SetColour( m_primarySpawnColour, m_secondarySpawnColour );

		// Reset the platetext
		m_pVehicle->SetPlateText( "M2MP01" );

		// Reset the speed
		m_pVehicle->SetSpeed( 0.0f );

		// Reset the horn
		m_pVehicle->SetHornOn( false );

		// Reset the siren
		m_pVehicle->SetSirenOn( false );

		// Reset the dirt level
		m_pVehicle->SetDirtLevel( 0.0f );

		// Reset the tuning table
		m_pVehicle->SetTuningTable( 0 );

		// Reset the power
		m_pVehicle->SetPower( false );

		// Reset the brake
		m_pVehicle->SetBrake( false );

		// Reset the wheel protection
		m_pVehicle->SetWheelsProtected( true );

		// Set the position
		SetPosition( m_vecSpawnPosition );

		// Set the rotation
		SetRotation( m_vecSpawnRotation );

		// Reset the lights
		m_pVehicle->SetLightState ( false );

		// Is the client script manager active?
		if( pCore->GetClientScriptingManager() && pCore->GetClientScriptingManager()->GetEvents() )
		{
			// Call the script event
			CSquirrelArguments args;
			args.push( m_vehicleId );
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientVehicleRespawn", &args );
		}
	}
}

void CNetworkVehicle::StoreVehicleSync( InVehicleSync vehicleSync, bool bInterpolate, bool bSpawn )
{
	DEBUG_TRACE("CNetworkVehicle::StoreVehicleSync");

	// Get the localplayer position
	CVector3 vecLocalPos;
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecLocalPos );

	// Is the last sync data invalid?
	if ( !Math::IsValidVector ( vehicleSync.m_vecPosition ) || !Math::IsValidVector ( vehicleSync.m_vecRotation ) )
	{
		// Reset all positions
		memcpy ( &vehicleSync.m_vecPosition, &m_vecSpawnPosition, sizeof(CVector3) );
		memcpy ( &vehicleSync.m_vecRotation, &m_vecSpawnRotation, sizeof(CVector3) );
		memcpy ( &m_vecLastGoodPosition, &m_vecSpawnPosition, sizeof(CVector3) );
		memcpy ( &m_vecLastGoodRotation, &m_vecSpawnRotation, sizeof(CVector3) );
	}

	// Is the vehicle spawning or inrange?
	if( bSpawn || Math::GetDistanceBetweenPoints( vecLocalPos, vehicleSync.m_vecPosition ) < 300.0f )
	{
		// Should we interpolate the position and rotation?
		if( bInterpolate )
		{
			// Set the target position
			SetTargetPosition( vehicleSync.m_vecPosition );

			// Set the target rotation
			SetTargetRotation( vehicleSync.m_vecRotation );
		}
		else
		{
			// Set the position
			SetPosition( vehicleSync.m_vecPosition );

			// Set the rotation
			SetRotation( vehicleSync.m_vecRotation );

			CLogFile::Printf ( "Not using interpolation this frame for vehicle %d.", m_vehicleId );
		}

		// Has the vehicle dirt level changed?
		if( GetDirtLevel() != vehicleSync.m_fDirtLevel )
			SetDirtLevel( vehicleSync.m_fDirtLevel );

		// Has the tuning table changed?
		if( GetTuningTable() != vehicleSync.m_iTuningTable )
			SetTuningTable( vehicleSync.m_iTuningTable );

		// Has the horn state changed?
		if( GetHornState() != vehicleSync.m_bHornState )
			SetHornState( vehicleSync.m_bHornState );

		// Has the siren state changed?
		if( m_pVehicle->IsSirenOn() != vehicleSync.m_bSirenState )
			m_pVehicle->SetSirenOn( vehicleSync.m_bSirenState );

		// Has the fuel changed?
		//if ( m_pVehicle->GetFuel () != vehicleSync.m_fFuel )
		//	m_pVehicle->SetFuel ( vehicleSync.m_fFuel );

		// Has the speed changed?
		CVector3 vecVelocity;
		GetSpeedVec ( &vecVelocity );
		if( Math::IsValidVector ( vehicleSync.m_vecVelocity ) && vecVelocity != vehicleSync.m_vecVelocity )
			SetTargetSpeed ( vehicleSync.m_vecVelocity ); //SetSpeedVec( vehicleSync.m_vecVelocity );

		// Has the turn speed changed?
		if( GetSteer() != vehicleSync.m_fTurnSpeed )
			SetTargetSteer ( vehicleSync.m_fTurnSpeed );

		// Has the engine damage changed?
		//if( m_pVehicle->GetEngineDamage() != vehicleSync.m_fEngineDamage )
		//	m_pVehicle->SetEngineDamage( vehicleSync.m_fEngineDamage );

		// Has the plate text changed?
		if( strcmp( vehicleSync.m_szPlateText, GetPlateText() ) )
			SetPlateText( vehicleSync.m_szPlateText );

		// Get the vehicle colour
		CColor primary, secondary;
		GetColour( &primary, &secondary );

		// Has the primary colour changed?
		if( primary != vehicleSync.m_primaryColour )
			SetColour( vehicleSync.m_primaryColour, secondary );

		// Get the colours again
		GetColour( &primary, &secondary );

		// Has the secondary colour changed?
		if( secondary != vehicleSync.m_secondaryColour )
			SetColour( primary, vehicleSync.m_secondaryColour );

		// Has the power state changed?
		if( m_pVehicle->GetPower() != vehicleSync.m_bPower )
			m_pVehicle->SetPower( vehicleSync.m_bPower );

		// Has the brake state changed?
		if( m_pVehicle->GetBrake() != vehicleSync.m_bBrake )
			m_pVehicle->SetBrake( vehicleSync.m_bBrake );

		// This can cause some crashes
		// Have the front wheels changed?
		/*if ( vehicleSync.m_bWheelModels[ 0 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 0 ) ) != vehicleSync.m_bWheelModels[ 0 ] )
			m_pVehicle->SetWheelTexture ( 0, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 0 ] ).Get () );

		// Have the rear wheels changed?
		if ( vehicleSync.m_bWheelModels[ 1 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 1 ) ) != vehicleSync.m_bWheelModels[ 1 ] )
			m_pVehicle->SetWheelTexture ( 1, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 1 ] ).Get () );

		// Have the rear wheels changed?
		if ( vehicleSync.m_bWheelModels[ 2 ] != 0xFF && Game::GetIdFromVehicleWheelModel ( m_pVehicle->GetWheelTexture ( 2 ) ) != vehicleSync.m_bWheelModels[ 2 ] )
			m_pVehicle->SetWheelTexture ( 2, Game::GetVehicleWheelModelFromId ( vehicleSync.m_bWheelModels[ 2 ] ).Get () );*/

		// Has the handbrake state changed?
		if ( m_pVehicle->IsHandbrakeOn () != vehicleSync.m_bHandbrake )
			m_pVehicle->SetHandbrake ( vehicleSync.m_bHandbrake );

		// Has the light state changed?
		if ( m_pVehicle->GetLightState () != vehicleSync.m_bLightState )
			m_pVehicle->SetLightState ( vehicleSync.m_bLightState );
	}
	else
	{
		// Set the target position
		SetTargetPosition( vehicleSync.m_vecPosition );

		// Set the target rotation
		SetTargetRotation( vehicleSync.m_vecRotation );
	}

	// Store the last sync data
	m_lastSyncData = vehicleSync;
}

void CNetworkVehicle::Pulse( void )
{
	DEBUG_TRACE("CNetworkVehicle::Pulse");

	// Is the localplayer not spawned?
	if( !pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	// Is the vehicle spawned?
	if( IsSpawned() ) 
	{
		// Get the vehicle position and rotation
		CVector3 vecPosition, vecRotation;
		GetPosition( &vecPosition );
		GetRotation( &vecRotation );

		// Is our position or rotation invalid?
		if ( !Math::IsValidVector ( vecPosition ) || !Math::IsValidVector ( vecRotation ) )
		{
			// Deactivate the vehicle
			m_pVehicle->Deactivate ();

			// Warp the vehicle back to their last good saved position and rotation
			SetPosition ( m_vecLastGoodPosition );
			SetRotation ( m_vecLastGoodRotation );

			// Is the last good state invalid?
			if ( !Math::IsValidVector ( m_vecLastGoodPosition ) || !Math::IsValidVector ( m_vecLastGoodRotation ) )
			{
				// Reset
				SetPosition ( m_vecSpawnPosition );
				SetRotation ( m_vecSpawnRotation );
			}

			// Activate the vehicle
			m_pVehicle->Activate ();

			// Return so we don't overwrite the last good position with invalid positions
			return;
		}

		// Store the current vehicle position and rotation
		memcpy ( &m_vecLastGoodPosition, &vecPosition, sizeof(CVector3) );
		memcpy ( &m_vecLastGoodRotation, &vecRotation, sizeof(CVector3) );

		// Process the interpolation
		Interpolate ();

		// Is there no driver?
		if( !m_pDriver )
		{
			// Has the vehicle fallen through the ground?
			if( vecPosition.fZ <= -150.0f )
			{
				// Deactivate the vehicle
				m_pVehicle->Deactivate();

				// Set the position
				SetPosition( m_vecSpawnPosition );

				// Set the rotation
				SetRotation( m_vecSpawnRotation );

				// Activate the vehicle
				m_pVehicle->Activate();

				// Process last sync data
				StoreVehicleSync( m_lastSyncData, false, true );
			}
		}
	}
	else
	{
		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Should we activate the vehicle?
		if( m_bSpawnProcessed && ((ulCurrentTime - m_ulSpawnTime) >= 4000 || ((ulCurrentTime - pCore->GetPlayerManager()->GetLocalPlayer()->GetSpawnTime()) >= 4000)) )
		{
			// Set the spawn not processed
			m_ulSpawnTime = ulCurrentTime;
			m_bSpawnProcessed = false;

			// Activate the vehicle
			m_pVehicle->Activate();

			// Should we process the last sync data?
			if( m_bProcessSyncOnSpawn )
			{
				// Process the last sync data
				StoreVehicleSync( m_lastSyncData, false, true );
			}
			else
			{
				// Set position
				SetPosition( m_vecSpawnPosition );

				// Set the rotation
				SetRotation( m_vecSpawnRotation );
			}

			// Is there a driver set?
			if( m_pDriver )
			{
				// Put the driver in the vehicle
				m_pDriver->PutInVehicle( this, 1 );
			}

			// Loop over all seats
			for( int i = 0; i < (MAX_SEATS - 1); i++ )
			{
				// Is there an occupant?
				if( m_pPassenger[ i ] )
				{
					// Put the passenger in the vehicle
					m_pPassenger[ i ]->PutInVehicle( this, (i + 2) );
				}
			}

			// Mark as spawned
			SetSpawned( true );
		}
	}
}

void CNetworkVehicle::SetPosition( CVector3 vecPosition )
{
	DEBUG_TRACE("CNetworkVehicle::SetPosition");

	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetPosition( vecPosition );
}

void CNetworkVehicle::GetPosition( CVector3 * vecPosition )
{
	DEBUG_TRACE("CNetworkVehicle::GetPosition");

	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->GetPosition( vecPosition );
}

void CNetworkVehicle::SetRotation( CVector3 vecRotation )
{
	DEBUG_TRACE("CNetworkVehicle::SetRotation");

	// Is the vehicle instance valid?
	if( m_pVehicle )
	{
		// Convert the rotation to radians
		vecRotation.ToRadians();
		
		// Set the vehicle rotation
		m_pVehicle->SetRotation( Quaternion( vecRotation ) );
	}
}

void CNetworkVehicle::GetRotation( CVector3 * vecRotation )
{
	DEBUG_TRACE("CNetworkVehicle::GetRotation");

	// Is the vehicle instance valid?
	if( m_pVehicle )
	{
		// Get the vehicle rotation
		Quaternion quatRotation;
		m_pVehicle->GetRotation( &quatRotation );

		// Copy the rotation quaternion to the eular angles vector
		memcpy( vecRotation, &quatRotation.toEularAngles(), sizeof(CVector3) );

		// Convert the rotation to degrees
		vecRotation->FromRadians();
	}
}

void CNetworkVehicle::SetColour( CColor primary, CColor secondary )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetColour( primary, secondary );
}

void CNetworkVehicle::GetColour( CColor * primary, CColor * secondary )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->GetColour( primary, secondary );
}

void CNetworkVehicle::SetPlateText( const char * szPlateText )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetPlateText( szPlateText );
}

const char * CNetworkVehicle::GetPlateText( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		return m_pVehicle->GetPlateText();

	return "M2MP01b";
}

void CNetworkVehicle::Repair( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->Repair();
}

void CNetworkVehicle::Explode( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->Explode();
}

void CNetworkVehicle::SetDirtLevel( float fDirtLevel )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetDirtLevel( fDirtLevel );
}

float CNetworkVehicle::GetDirtLevel( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		return m_pVehicle->GetDirtLevel();

	return 0.0f;
}

void CNetworkVehicle::SetEngineState( bool bState )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetEngineOn( bState );
}

bool CNetworkVehicle::GetEngineState( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		return m_pVehicle->IsEngineOn();

	return false;
}

void CNetworkVehicle::SetPartOpen( int iPart, bool bOpen )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
	{
		if( iPart == 0 )
			bOpen ? m_pVehicle->OpenHood() : m_pVehicle->CloseHood();
		else
			bOpen ? m_pVehicle->OpenTrunk() : m_pVehicle->CloseTrunk();
	}
}

bool CNetworkVehicle::IsPartOpen( int iPart )
{
	// todo
	return false;
}

void CNetworkVehicle::SetSirenState( bool bState )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetSirenOn( bState );
}

bool CNetworkVehicle::GetSirenState( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		return m_pVehicle->IsSirenOn();

	return false;
}

void CNetworkVehicle::SetHornState( bool bState )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		m_pVehicle->SetHornOn( bState );
}

bool CNetworkVehicle::GetHornState( void )
{
	// Is the vehicle instance valid?
	if( m_pVehicle )
		return m_pVehicle->IsHornOn();

	return false;
}

void CNetworkVehicle::SetWindowOpen( int iSeat, bool bOpen )
{
	// todo
}

bool CNetworkVehicle::IsWindowOpen( int iSeat )
{
	// todo
	return false;
}

void CNetworkVehicle::SetTuningTable( int iTable )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		m_pVehicle->SetTuningTable( iTable );
}

int CNetworkVehicle::GetTuningTable( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->GetTuningTable();

	return 0;
}

void CNetworkVehicle::SetWheelTexture( int iWheelIndex, int iTexture )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		m_pVehicle->SetWheelTexture( iWheelIndex, Game::GetVehicleWheelModelFromId( iTexture ) );
}

int CNetworkVehicle::GetWheelTexture( int iWheelIndex )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return Game::GetIdFromVehicleWheelModel( m_pVehicle->GetWheelTexture( iWheelIndex ) );

	return 0xFF;
}

void CNetworkVehicle::SetSteer ( float fSteer )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		m_pVehicle->AddSteer ( (fSteer * (D3DX_PI / 180.0f)) );
}

float CNetworkVehicle::GetSteer ( void )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		return (m_pVehicle->GetSteer() * (180.0f / D3DX_PI));

	return 0.0f;
}

void CNetworkVehicle::SetSpeed ( float fSpeed )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		m_pVehicle->SetSpeed ( fSpeed );
}

float CNetworkVehicle::GetSpeed ( void )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		return m_pVehicle->GetSpeed ();

	return 0.0f;
}

void CNetworkVehicle::SetSpeedVec ( CVector3 vecSpeed )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		m_pVehicle->SetMoveSpeed ( vecSpeed );
}

void CNetworkVehicle::GetSpeedVec ( CVector3 * vecSpeed )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		m_pVehicle->GetMoveSpeed ( vecSpeed );
}

void CNetworkVehicle::SetFuel ( float fFuel )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		m_pVehicle->SetFuel ( fFuel );
}

float CNetworkVehicle::GetFuel ( void )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
		return m_pVehicle->GetFuel ();

	return 0.0f;
}

void CNetworkVehicle::SetLightState ( bool bLightState )
{
	// Set the light state
	if ( m_pVehicle )
		m_pVehicle->SetLightState ( bLightState );
}

bool CNetworkVehicle::GetLightState ( void )
{
	// Get the light state
	if ( m_pVehicle )
		return m_pVehicle->GetLightState ();

	return false;
}

void CNetworkVehicle::HandlePlayerEnter( CNetworkPlayer * pNetworkPlayer, EntityId seatId )
{
	DEBUG_TRACE("CNetworkVehicle::HandlePlayerEnter");

	// Is this player the driver?
	if( seatId == 0 )
	{
		// Set the driver
		m_pDriver = pNetworkPlayer;

		// Reset the last syncer
		SetLastSyncer( NULL );

		// Reset the handbrake
		m_pVehicle->SetHandbrake ( false );
	}
	else
	{
		// Set the passenger
		m_pPassenger[ seatId ] = pNetworkPlayer;
	}
}

void CNetworkVehicle::HandlePlayerExit( CNetworkPlayer * pNetworkPlayer, EntityId seatId, bool bResetInterpolation )
{
	DEBUG_TRACE("CNetworkVehicle::HandlePlayerExit");

	// Is this player the driver?
	if( seatId == 0 )
	{
		// Reset the driver
		m_pDriver = NULL;

		// Set the last syncer
		SetLastSyncer( pNetworkPlayer );

		// Reset the handbrake
		m_pVehicle->SetHandbrake ( true );

		// If this wasn't the localplayer, reset the interpolation
		if( pNetworkPlayer != pCore->GetPlayerManager()->GetLocalPlayer() && bResetInterpolation )
			ResetInterpolation ();
	}
	else
	{
		// Reset the passenger
		m_pPassenger[ seatId ] = NULL;
	}
}

void CNetworkVehicle::ResetInterpolation( void )
{
	DEBUG_TRACE("CNetworkVehicle::ResetInterpolation");

	// Are we spawned?
	if ( IsSpawned() )
	{
		// Do we have a target position?
		if( HasTargetPosition () )
			SetPosition ( m_Interpolation.position.vecTarget );

		// Do we have a target rotation?
		if( HasTargetRotation () )
			SetRotation ( m_Interpolation.rotation.vecTarget );

		// Do we have a target steer?
		if ( HasTargetSteer () )
			SetSteer ( m_Interpolation.steer.fTarget );

		// Do we have a target speed?
		if ( HasTargetSpeed () )
			SetSpeedVec ( m_Interpolation.speed.vecTarget );
	}

	// Remove the target position
	RemoveTargetPosition ();
	m_Interpolation.position.fLastAlpha = 0;
	m_Interpolation.position.ulStartTime = 0;
	m_Interpolation.position.vecError = CVector3();
	m_Interpolation.position.vecTarget = CVector3();

	// Remove the target rotation
	RemoveTargetRotation ();
	m_Interpolation.rotation.fLastAlpha = 0;
	m_Interpolation.rotation.ulStartTime = 0;
	m_Interpolation.rotation.vecError = CVector3();
	m_Interpolation.rotation.vecTarget = CVector3();

	// Remove the target steer
	RemoveTargetSteer ();
	m_Interpolation.steer.fLastAlpha = 0;
	m_Interpolation.steer.ulStartTime = 0;
	m_Interpolation.steer.fError = 0.0f;
	m_Interpolation.steer.fTarget = 0.0f;

	// Remove the target speed
	RemoveTargetSpeed ();
	m_Interpolation.speed.fLastAlpha = 0;
	m_Interpolation.speed.ulStartTime = 0;
	m_Interpolation.speed.vecError = CVector3();
	m_Interpolation.speed.vecTarget = CVector3();
}

void CNetworkVehicle::Interpolate( void )
{
	DEBUG_TRACE("CNetworkVehicle::Interpolate");

	// Do we have a driver and he is not the localplayer?
	if( m_pDriver && m_pDriver != pCore->GetPlayerManager()->GetLocalPlayer () )
	{
		// Update the target position
		UpdateTargetPosition ();

		// Update the target rotation
		UpdateTargetRotation ();

		// Update the target steer
		UpdateTargetSteer ();

		// Update the target speed
		UpdateTargetSpeed ();
	}
}

void CNetworkVehicle::SetTargetPosition( CVector3 vecPosition )
{
	// Is the vehicle spawned?
	if ( IsSpawned () )
	{
		// Update the current target position
		UpdateTargetPosition ();

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Get our current position
		CVector3 vecCurrentPosition;
		GetPosition ( &vecCurrentPosition );

		// Set the target position
		m_Interpolation.position.vecTarget = vecPosition;

		// Set the relative error
		m_Interpolation.position.vecError = (vecPosition - vecCurrentPosition);

		// Apply the error over 400ms
		m_Interpolation.position.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );

		// Calculate the interpolation interval
		m_Interpolation.position.ulStartTime = ulCurrentTime;
		m_Interpolation.position.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);

		// Start the interpolation
		m_Interpolation.position.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetPosition( void )
{
	// Do we have a target position?
	if ( IsSpawned () && HasTargetPosition () )
	{
		// Get our current position
		CVector3 vecCurrentPosition;
		GetPosition ( &vecCurrentPosition );

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Calculate the factor of time spent since the start of the interpolation
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.position.ulStartTime, ulCurrentTime, m_Interpolation.position.ulFinishTime ), 1.5f );

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_Interpolation.position.fLastAlpha);

		// Store the current error portion
		m_Interpolation.position.fLastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.position.vecError );

		// Have we finished compensating the error?
		if ( fAlpha == 1.5f )
			m_Interpolation.position.ulFinishTime = 0;

		// Update our new position
		SetPosition ( (vecCurrentPosition + vecErrorCompensation) );
	}
}

void CNetworkVehicle::SetTargetRotation( CVector3 vecRotation )
{
	// Is the vehicle spawned?
	if( IsSpawned() )
	{
		// Update the current target rotation
		UpdateTargetRotation ();

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Get the current rotation
		CVector3 vecCurrentRotation;
		GetRotation ( &vecCurrentRotation );

		// Set the target rotation
		m_Interpolation.rotation.vecTarget = vecRotation;

		// Set the relative error over 400ms
		m_Interpolation.rotation.vecError = Math::GetOffsetDegrees ( vecCurrentRotation, vecRotation );
		m_Interpolation.rotation.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f ); // 250ms -> 0.40f

		// Calculate the interpolation interval
		m_Interpolation.rotation.ulStartTime = ulCurrentTime;
		m_Interpolation.rotation.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);

		// Start the interpolation
		m_Interpolation.rotation.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetRotation( void )
{
	// Do we have a target direction?
	if( HasTargetRotation() )
	{
		// Get our current rotation
		CVector3 vecCurrentRotation;
		GetRotation ( &vecCurrentRotation );

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Calculate the factor of time spent since the start of the interpolation
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.rotation.ulStartTime, ulCurrentTime, m_Interpolation.rotation.ulFinishTime ), 1.0f );

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_Interpolation.rotation.fLastAlpha);

		// Store the current error portion
		m_Interpolation.rotation.fLastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.rotation.vecError );

		// Have we finished compensating the error?
		if ( fAlpha == 1.0f )
			m_Interpolation.rotation.ulFinishTime = 0;

		// Set the new rotation
		SetRotation ( (vecCurrentRotation + vecErrorCompensation) );
	}
}

void CNetworkVehicle::SetTargetSteer( float fSteer )
{
	// Is the vehicle spawned?
	if( IsSpawned() )
	{
		// Update the target steer
		UpdateTargetSteer ();

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Get the current steer
		float fCurrentSteer = GetSteer ();

		// Set the target steer
		m_Interpolation.steer.fTarget = fSteer;

		// Set the relative error over 400ms
		m_Interpolation.steer.fError = (fSteer - fCurrentSteer);
		m_Interpolation.steer.fError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );

		// Calculate the interpolation interval
		m_Interpolation.steer.ulStartTime = ulCurrentTime;
		m_Interpolation.steer.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);

		// Start the interpolation
		m_Interpolation.steer.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetSteer( void )
{
	// Do we have a target steer?
	if( HasTargetSteer () )
	{
		// Get the current steer
		float fCurrentSteer = GetSteer ();

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Calculate the factor of time spent since the start of the interpolation
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.steer.ulStartTime, ulCurrentTime, m_Interpolation.steer.ulFinishTime ), 1.0f );

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_Interpolation.steer.fLastAlpha);

		// Store the current error portion
		m_Interpolation.steer.fLastAlpha = fAlpha;

		// Apply the error compensation
		float fErrorCompensation = Math::Lerp ( 0.0f, fCurrentAlpha, m_Interpolation.steer.fError );

		// Have we finished compensating the error?
		if ( fAlpha == 1.0f )
			m_Interpolation.steer.ulFinishTime = 0;

		// Set the new steer
		SetSteer ( (fCurrentSteer + fErrorCompensation) );
	}
}

void CNetworkVehicle::SetTargetSpeed( CVector3 vecSpeed )
{
	// Is the vehicle spawned?
	if( IsSpawned() )
	{
		// Update the current target speed
		UpdateTargetSpeed ();

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Get the current speed
		CVector3 vecCurrentSpeed;
		GetSpeedVec ( &vecCurrentSpeed );

		// Set the target rotation
		m_Interpolation.speed.vecTarget = vecSpeed;

		// Set the relative error over 400ms
		m_Interpolation.speed.vecError = (vecSpeed - vecCurrentSpeed);
		m_Interpolation.speed.vecError *= Math::Lerp < const float > ( 0.25f, Math::UnlerpClamped ( 100, NETWORK_TICKRATE, 400 ), 1.0f );

		// Calculate the interpolation interval
		m_Interpolation.speed.ulStartTime = ulCurrentTime;
		m_Interpolation.speed.ulFinishTime = (ulCurrentTime + NETWORK_TICKRATE);

		// Start the interpolation
		m_Interpolation.speed.fLastAlpha = 0.0f;
	}
}

void CNetworkVehicle::UpdateTargetSpeed( void )
{
	// Do we have a target speed?
	if( HasTargetSpeed() )
	{
		// Get the current speed
		CVector3 vecCurrentSpeed;
		GetSpeedVec ( &vecCurrentSpeed );

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime ();

		// Calculate the factor of time spent since the start of the interpolation
		float fAlpha = Math::Clamp ( 0.0f, Math::Unlerp ( m_Interpolation.speed.ulStartTime, ulCurrentTime, m_Interpolation.speed.ulFinishTime ), 1.0f );

		// Get the current error portion to compensate
		float fCurrentAlpha = (fAlpha - m_Interpolation.speed.fLastAlpha);

		// Store the current error portion
		m_Interpolation.speed.fLastAlpha = fAlpha;

		// Apply the error compensation
		CVector3 vecErrorCompensation = Math::Lerp ( CVector3(), fCurrentAlpha, m_Interpolation.speed.vecError );

		// Have we finished compensating the error?
		if ( fAlpha == 1.0f )
			m_Interpolation.speed.ulFinishTime = 0;

		// Set the new speed
		SetSpeedVec ( (vecCurrentSpeed + vecErrorCompensation) );
	}
}

bool CNetworkVehicle::GetClosestPlayer( CNetworkPlayer ** pNetworkPlayer )
{
	// Is the vehicle spawned?
	if( IsSpawned() )
	{
		// Get the current vehicle position
		CVector3 vecPosition;
		GetPosition( &vecPosition );

		//
		CNetworkPlayer * pClosestPlayer = NULL;
		float fCurrentDistance = 100.0f;

		// Loop over each player
		for( EntityId i = 0; i < MAX_PLAYERS; i++ )
		{
			// Is the player connected?
			if( pCore->GetPlayerManager()->IsActive( i ) || i == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
			{
				bool bLocalPlayer = (i == pCore->GetPlayerManager()->GetLocalPlayer()->GetId());

				// Get the current player position
				CVector3 vecCurrentPosition;
				if( bLocalPlayer )
					pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecCurrentPosition );
				else
					pCore->GetPlayerManager()->Get( i )->GetPosition( &vecCurrentPosition );

				// Get the distance between the current player and the vehicle
				float fDistance = Math::GetDistanceBetweenPoints( vecCurrentPosition, vecPosition );

				// Is this player closer?
				if( fDistance < fCurrentDistance )
				{
					// Set the closest distance
					fCurrentDistance = fDistance;

					// Set the closest player
					if( bLocalPlayer )
						pClosestPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
					else
						pClosestPlayer = pCore->GetPlayerManager()->Get( i );
				}
			}
		}

		// Did we find a player?
		if( pClosestPlayer )
		{
			// Set the network player instance
			*pNetworkPlayer = pClosestPlayer;
			return true;
		}
	}

	return false;
}

void CNetworkVehicle::ProcessUnoccupiedSync( RakNet::BitStream * pBitStream )
{
	// Read the sync data from the bitstream
	UnoccupiedVehicleSync unoccupiedVehicleSync;
	pBitStream->Read( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );

	/*
	// Is the vehicle invalid?
	if( !m_pVehicle )
		return;

	// Is the localplayer not spawned?
	if( !pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
		return;

	// Set the target position
	SetPosition( unoccupiedVehicleSync.m_vecPosition ); // todo: ::Interpolate() only works if there's a driver! (check if there's a syncer instead!)

	// Set the target direction
	SetDirection( unoccupiedVehicleSync.m_vecDirection );

	// Set the target rotation
	SetRotation( unoccupiedVehicleSync.m_quatRotation );
	*/
}

void CNetworkVehicle::AttachBlip ( CBlip * pBlip )
{
	// Is the blip valid?
	if ( pBlip )
	{
		// Store the blip pointer
		m_pAttachedBlip = pBlip;

		// Is the blip created?
		m_bBlipAttached = pBlip->IsCreated ();

		CLogFile::Printf ( "CNetworkVehicle::AttachBlip () - 0x%p, %s", m_pAttachedBlip, (m_bBlipAttached ? "true" : "false") );
	}
}

void CNetworkVehicle::DetachBlip ( void )
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

		CLogFile::Printf ( "CNetworkVehicle::DetachBlip ()" );
	}
}