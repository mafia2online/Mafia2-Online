/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkVehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

// Taken from /tables/car_colors.tbl
static CColor predefinedColours[] =
{
	CColor ( 154, 154, 154 ),
	CColor ( 143, 137, 124 ),
	CColor ( 112, 104, 89 ),
	CColor ( 79, 72, 65 ),
	CColor ( 120, 111, 68 ),
	CColor ( 157, 143, 110 ),
	CColor ( 145, 114, 33 ),
	CColor ( 121, 113, 31 ),
	CColor ( 98, 26, 21 ),
	CColor ( 66, 0, 0 ),
	CColor ( 97, 46, 10 ),
	CColor ( 29, 4, 0 ),
	CColor ( 70, 128, 95 ),
	CColor ( 27, 76, 65 ),
	CColor ( 57, 84, 37 ),
	CColor ( 15, 32, 24 ),
	CColor ( 80, 80, 80 ),
	CColor ( 83, 104, 80 ),
	CColor ( 73, 75, 33 ),
	CColor ( 1, 17, 13 ),
	CColor ( 47, 95, 106 )
};

CNetworkVehicle::CNetworkVehicle( void )
{
	// Generate a random seed
	srand ( SharedUtility::GetTime () );

	// Reset variables
	m_iModel = -1;
	m_ulLastSyncerCheckTime = 0;
	SetLastSyncer( NULL );
	m_uiTotalOccupants = 0;
	m_ulLastOccupantTime = 0;
	
	// Set the initial respawn time
	SetRespawnTime ( 300000 );

	// Reset occupants
	memset( m_pOccupants, 0, sizeof( m_pOccupants ) );

	// Reset the initial data
	m_lastSyncData.m_bEngineState = false;
	m_lastSyncData.m_bHornState = false;
	m_lastSyncData.m_bSirenState = false;
	m_lastSyncData.m_fDirtLevel = 0.0f;
	m_lastSyncData.m_fEngineDamage = 0.0f;
	m_lastSyncData.m_fFuel = 100.0f;
	m_lastSyncData.m_vecVelocity = CVector3();
	m_lastSyncData.m_fTurnSpeed = 0.0f;
	m_lastSyncData.m_iTuningTable = 0;
	m_lastSyncData.m_bPower = false;
	m_lastSyncData.m_bBrake = false;
	m_lastSyncData.m_bWheelModels[ 0 ] = 0xFF;
	m_lastSyncData.m_bWheelModels[ 1 ] = 0xFF;
	m_lastSyncData.m_bWheelModels[ 2 ] = 0xFF;
	memcpy( &m_lastSyncData.m_primaryColour, &predefinedColours[ rand() % 20 ], sizeof(CColor) );
	memcpy( &m_lastSyncData.m_secondaryColour, &predefinedColours[ rand() % 20 ], sizeof(CColor) );
	strcpy( m_lastSyncData.m_szPlateText, "M2MP01" );
	m_lastSyncData.m_bLightState = false;
}

CNetworkVehicle::~CNetworkVehicle( void )
{
}

void CNetworkVehicle::SetModel( int iModel )
{
	// Store the model id
	m_iModel = iModel;

	// Send it to all clients
	// todo: change vehicle models
}

void CNetworkVehicle::SetSpawnPosition( CVector3 vecSpawnPosition )
{
	// Copy the spawn position
	memcpy( &m_vecSpawnPosition, &vecSpawnPosition, sizeof(CVector3) );
	
	// Set the spawn position
	SetPosition( vecSpawnPosition, false );
}

void CNetworkVehicle::SetSpawnRotation( CVector3 vecSpawnRotation )
{
	// Copy the spawn position
	memcpy( &m_vecSpawnRotation, &vecSpawnRotation, sizeof(CVector3) );
	
	// Set the spawn rotation
	SetRotation( vecSpawnRotation, false );
}

void CNetworkVehicle::AddForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the model id
	pBitStream.Write( m_iModel );

	// Get the last sync data
	InVehicleSync vehicleSync;
	GetSyncData( &vehicleSync );

	// Write the last sync packet
	pBitStream.Write( (char *)&vehicleSync, sizeof(InVehicleSync) );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_NEW_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkVehicle::AddForWorld( void )
{
	// Loop through all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Add this vehicle for this player
			AddForPlayer( i );
		}
	}
}

void CNetworkVehicle::RemoveForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_REMOVE_VEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkVehicle::RemoveForWorld( void )
{
	// Loop through all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Remove this vehicle for this player
			RemoveForPlayer( i );
		}
	}
}

void CNetworkVehicle::SpawnForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the spawn position
	pBitStream.Write( m_vecSpawnPosition );

	// Write the spawn rotation
	pBitStream.Write( m_vecSpawnRotation );

	// Write the total occupants
	pBitStream.Write( m_uiTotalOccupants );

	// Loop over all occupants
	for( int i = 0; i < MAX_SEATS; i++ )
	{
		// Is the current occupant valid?
		if( m_pOccupants[ i ] )
		{
			// Write the occupant id
			pBitStream.WriteCompressed( m_pOccupants[ i ]->GetId() );

			// Write the occupant seat
			pBitStream.Write( i );
		}
	}

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_SPAWNVEHICLE, &pBitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkVehicle::SpawnForWorld( void )
{
	// Loop through all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Spawn this vehicle for this player
			SpawnForPlayer( i );
		}
	}

	// Call the scripting event
	CSquirrelArguments arguments;
	arguments.push( m_vehicleId );
	pCore->GetEvents()->Call ( "onVehicleSpawn", &arguments );
}

void CNetworkVehicle::RespawnForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the spawn position
	pBitStream.Write( m_vecSpawnPosition );

	// Write the spawn rotation
	pBitStream.Write( m_vecSpawnRotation );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_RESPAWNVEHICLE, &pBitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkVehicle::RespawnForWorld( void )
{
	// Loop through all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Respawn this vehicle for this player
			RespawnForPlayer( i );
		}
	}

	// Set the position
	SetPosition( m_vecSpawnPosition, false );

	// Set the rotation
	SetRotation( m_vecSpawnRotation, false );

	// Trigger event?
	//
}

void CNetworkVehicle::Pulse( void )
{
	// Has this vehicle been empty for the respawn time?
	if ( GetRespawnTime() > 0 && (SharedUtility::GetTime() - m_ulLastOccupantTime) > GetRespawnTime() )
	{
		// Reset the last occupant time
		m_ulLastOccupantTime = SharedUtility::GetTime ();

		// Is there no occupants?
		if ( m_uiTotalOccupants == 0 )
		{
			// Respawn the vehicle for the world
			RespawnForWorld ();

			//
			CLogFile::Printf ( "Vehicle %d has respawned! (%d milliseconds of inactivity)", m_vehicleId, GetRespawnTime () );
		}
	}
}

void CNetworkVehicle::SetPosition( CVector3 vecPosition, bool bBroadcast )
{
	// Should we broadcast this?
	if( bBroadcast )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;
	
		// Write the vehicle id
		pBitStream.WriteCompressed( m_vehicleId );

		// Write the position
		pBitStream.Write( vecPosition );

		// Send it to all clients
		pCore->GetNetworkModule()->Call( RPC_SETVEHICLEPOS, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
	}

	// Store the position
	memcpy( &m_lastSyncData.m_vecPosition, &vecPosition, sizeof(CVector3) );
}

void CNetworkVehicle::GetPosition( CVector3 * vecPosition )
{
	// Copy the position
	memcpy( vecPosition, &m_lastSyncData.m_vecPosition, sizeof(CVector3) );
}

void CNetworkVehicle::SetRotation( CVector3 vecRotation, bool bBroadcast )
{
	// Should we broadcast this?
	if( bBroadcast )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;
	
		// Write the vehicle id
		pBitStream.WriteCompressed( m_vehicleId );

		// Write the direction
		pBitStream.Write( vecRotation );

		// Send it to all clients
		pCore->GetNetworkModule()->Call( RPC_SETVEHICLEDIR, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
	}

	// Store the direction
	memcpy( &m_lastSyncData.m_vecRotation, &vecRotation, sizeof(CVector3) );
}

void CNetworkVehicle::GetRotation( CVector3 * vecRotation )
{
	// Copy the position
	memcpy( vecRotation, &m_lastSyncData.m_vecRotation, sizeof(CVector3) );
}

void CNetworkVehicle::SetColour( CColor primary, CColor secondary )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the primary colour
	pBitStream.WriteCompressed( primary.R );
	pBitStream.WriteCompressed( primary.G );
	pBitStream.WriteCompressed( primary.B );

	// Write the secondary colour
	pBitStream.WriteCompressed( secondary.R );
	pBitStream.WriteCompressed( secondary.G );
	pBitStream.WriteCompressed( secondary.B );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLECOLOUR, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Copy the colours into the last sync data
	memcpy ( &m_lastSyncData.m_primaryColour, &primary, sizeof(CColor) );
	memcpy ( &m_lastSyncData.m_secondaryColour, &secondary, sizeof(CColor) );
}

void CNetworkVehicle::GetColour( CColor * primary, CColor * secondary )
{
	// Copy the primary colour
	memcpy( primary, &m_lastSyncData.m_primaryColour, sizeof(CColor) );

	// Copy the secondary colour
	memcpy( secondary, &m_lastSyncData.m_secondaryColour, sizeof(CColor) );
}

void CNetworkVehicle::SetPlateText( const char * szText )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the plate text
	pBitStream.Write( RakNet::RakString( szText ) );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEPLATETEXT, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the plate text in the last sync data
	strcpy ( m_lastSyncData.m_szPlateText, szText );
}

const char * CNetworkVehicle::GetPlateText( void )
{
	return (const char *)m_lastSyncData.m_szPlateText;
}

void CNetworkVehicle::Repair( void )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_REPAIRVEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

void CNetworkVehicle::Explode( void )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_EXPLODEVEHICLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

void CNetworkVehicle::SetDirtLevel( float fDirtLevel )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the dirt level
	pBitStream.Write( fDirtLevel );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEDIRTLEVEL, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the last dirt level
	m_lastSyncData.m_fDirtLevel = fDirtLevel;
}

float CNetworkVehicle::GetDirtLevel( void )
{
	return m_lastSyncData.m_fDirtLevel;
}

void CNetworkVehicle::SetEngineState( bool bState )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the engine state
	bState ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEENGINESTATE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the last engine state
	m_lastSyncData.m_bEngineState = bState;
}

bool CNetworkVehicle::GetEngineState( void )
{
	return m_lastSyncData.m_bEngineState;
}

void CNetworkVehicle::SetPartOpen( int iPart, bool bOpen )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the part id
	pBitStream.WriteCompressed( iPart );

	// Write the part state
	bOpen ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEPARTOPEN, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

bool CNetworkVehicle::IsPartOpen( int iPart )
{
	//if( iPart > 1 )
	//	return false;

	//return m_lastSyncData.m_bPartState[ iPart ];
	return false;
}

void CNetworkVehicle::SetSirenState( bool bState )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the siren state
	bState ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLESIRENSTATE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the last siren state
	m_lastSyncData.m_bSirenState = bState;
}

bool CNetworkVehicle::GetSirenState( void )
{
	return m_lastSyncData.m_bSirenState;
}

void CNetworkVehicle::SetHornState( bool bState )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the horn state
	bState ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEHORNSTATE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the last horn state
	m_lastSyncData.m_bHornState = bState;
}

bool CNetworkVehicle::GetHornState( void )
{
	return m_lastSyncData.m_bHornState;
}

void CNetworkVehicle::SetWindowOpen( int iSeat, bool bOpen )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the seat id
	pBitStream.WriteCompressed( iSeat );

	// Write the open state
	bOpen ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEWINDOWOPEN, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

bool CNetworkVehicle::IsWindowOpen( int iSeat )
{
	//if( iSeat > MAX_SEATS )
	//	return false;

	//return m_lastSyncData.m_bWindowState[ iSeat ];

	return false;
}

void CNetworkVehicle::SetTuningTable( int iTable )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the tuning table
	pBitStream.WriteCompressed( iTable );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLETUNINGTABLE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );

	// Store the last tuning table
	m_lastSyncData.m_iTuningTable = iTable;
}

int CNetworkVehicle::GetTuningTable( void )
{
	return m_lastSyncData.m_iTuningTable;
}

void CNetworkVehicle::SetWheelTexture( int iWheelIndex, int iTexture )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the wheel index
	pBitStream.WriteCompressed ( iWheelIndex );

	// Write the tuning table
	pBitStream.WriteCompressed( iTexture );

	// Send it to all clients
	pCore->GetNetworkModule()->Call( RPC_SETVEHICLEWHEELTEXTURE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

int CNetworkVehicle::GetWheelTexture( int iWheelIndex )
{
	return m_lastSyncData.m_bWheelModels[ Math::Clamp< BYTE > ( 0, iWheelIndex, 3 ) ];
}

void CNetworkVehicle::SetSpeedVec ( CVector3 vecSpeed )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the speed
	pBitStream.Write ( vecSpeed );

	// Send it to all clients
	pCore->GetNetworkModule()->Call ( RPC_SETVEHICLESPEED, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

void CNetworkVehicle::GetSpeedVec ( CVector3 * vecSpeed )
{
	// Copy the last move speed to the vector
	memcpy ( vecSpeed, &m_lastSyncData.m_vecVelocity, sizeof(CVector3) );
}

void CNetworkVehicle::SetFuel ( float fFuel )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the fuel
	pBitStream.Write ( fFuel );

	// Send it to all clients
	pCore->GetNetworkModule()->Call ( RPC_SETVEHICLEFUEL, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

float CNetworkVehicle::GetFuel ( void )
{
	return m_lastSyncData.m_fFuel;
}

void CNetworkVehicle::SetLightState ( bool bLightState )
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;
	
	// Write the vehicle id
	pBitStream.WriteCompressed( m_vehicleId );

	// Write the fuel
	bLightState ? pBitStream.Write1() : pBitStream.Write0();

	// Send it to all clients
	pCore->GetNetworkModule()->Call ( RPC_SETVEHICLELIGHTSTATE, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

bool CNetworkVehicle::GetLightState ()
{
	return m_lastSyncData.m_bLightState;
}

void CNetworkVehicle::HandlePlayerEnter( CNetworkPlayer * pNetworkPlayer, int iSeat )
{
	// Is the player invalid?
	if( !pNetworkPlayer )
		return;

	// Is someone already in that seat?
	if( m_pOccupants[ iSeat ] )
	{
		CLogFile::Printf ( "Someone tried entering vehicle %d, but there's already a driver! (NetPlayer: 0x%p, Seat: %d)", m_vehicleId, pNetworkPlayer, iSeat );
		return;
	}

	// 
	CSquirrelArguments pArguments;
	pArguments.push( pNetworkPlayer->GetId() );
	pArguments.push( m_vehicleId );
	pArguments.push( iSeat );

	// Call the event
	pCore->GetEvents()->Call( "onPlayerVehicleEnter", &pArguments );

	// Set the seat occupant
	SetOccupant( iSeat, pNetworkPlayer );

	// Can't go below zero, but assertion fail told us different!
	if ( (int)m_uiTotalOccupants < 0 )
		m_uiTotalOccupants = 0;

	// Decrease the occupants count
	if ( m_uiTotalOccupants < MAX_SEATS )
		m_uiTotalOccupants++;

	// Is there an issue with the occupant count?
	assert ( m_uiTotalOccupants <= MAX_SEATS );
}

void CNetworkVehicle::HandlePlayerExit( CNetworkPlayer * pNetworkPlayer, int iSeat )
{
	// Is the player invalid?
	if( !pNetworkPlayer )
		return;

	// 
	CSquirrelArguments pArguments;
	pArguments.push( pNetworkPlayer->GetId() );
	pArguments.push( m_vehicleId );
	pArguments.push( iSeat );

	// Call the event
	pCore->GetEvents()->Call( "onPlayerVehicleExit", &pArguments );

	// Is the player the saved driver?
	if( m_pOccupants[ iSeat ] && pNetworkPlayer->GetId() == m_pOccupants[ iSeat ]->GetId() )
	{
		// Was this the driver seat?
		if( iSeat == 0 )
		{
			// Set the last syncer
			SetLastSyncer( pNetworkPlayer );

			// Reset sync data
			m_lastSyncData.m_bPower = false;
			m_lastSyncData.m_bBrake = false;
			m_lastSyncData.m_vecVelocity = CVector3();
			m_lastSyncData.m_bHandbrake = true;

			// Do we have a passenger?
			if ( m_pOccupants[ 1 ] )
			{
				// Construct a new bitstream
				RakNet::BitStream bitStream;

				// Write the vehicle id
				bitStream.WriteCompressed ( m_vehicleId );

				// Write the occupant id
				bitStream.WriteCompressed ( m_pOccupants[ 1 ]->GetId () );

				// Send the packet to all other clients
				pCore->GetNetworkModule()->Call ( RPC_MOVETODRIVER, &bitStream, HIGH_PRIORITY, RELIABLE, m_pOccupants[ 1 ]->GetId (), true );

#ifdef DEBUG
				CLogFile::Printf ( "RPC_MOVETODRIVER sent!" );
#endif
			}
		}
	}
	else
	{
		CLogFile::Printf( "CVehicle::HandlePlayerExit - Skipping some stuff because the saved driver is no longer the real driver!" );
	}

	// Reset the seat occupant
	SetOccupant( iSeat, NULL );

	// Decrease the occupants count
	if ( m_uiTotalOccupants > 0 )
		m_uiTotalOccupants--;

	// Is there an issue with the occupant count?
	assert ( m_uiTotalOccupants <= MAX_SEATS );

	// Set the last occupant time
	if ( m_uiTotalOccupants == 0 )
		m_ulLastOccupantTime = SharedUtility::GetTime ();
}

void CNetworkVehicle::HandlePlayerDisconnect( CNetworkPlayer * pNetworkPlayer )
{
	// Is the player syncing this vehicle?
	if( m_pLastSyncer == pNetworkPlayer )
	{
		// Reset the last syncer
		SetLastSyncer( NULL );

		// Reset sync data
		m_lastSyncData.m_bPower = false;
		m_lastSyncData.m_bBrake = false;
		m_lastSyncData.m_vecVelocity = CVector3();
	}

	// Loop over all vehicle seats
	for( int i = 0; i < MAX_SEATS; i++ )
	{
		// Is the current seat occupant this player?
		if( GetOccupant(i) == pNetworkPlayer )
		{
			// Reset the seat occupant
			SetOccupant( i, NULL );

			// Decrease the occupants count
			if ( m_uiTotalOccupants > 0 )
				m_uiTotalOccupants--;

			// Is there an issue with the occupant count?
			assert ( m_uiTotalOccupants <= MAX_SEATS );

			// Set the last occupant time
			if ( m_uiTotalOccupants == 0 )
				m_ulLastOccupantTime = SharedUtility::GetTime ();

			break;
		}
	}
}

bool CNetworkVehicle::GetClosestPlayer( CNetworkPlayer ** pNetworkPlayer )
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
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Get the current player position
			CVector3 vecCurrentPosition;
			pCore->GetPlayerManager()->Get( i )->GetPosition( &vecCurrentPosition );

			// Get the distance between the current player and the vehicle
			float fDistance = Math::GetDistanceBetweenPoints( vecCurrentPosition, vecPosition );

			// Is this player closer?
			if( fDistance < fCurrentDistance )
			{
				// Set the closest distance
				fCurrentDistance = fDistance;

				// Set the closest player
				pClosestPlayer = pCore->GetPlayerManager()->Get( i );
			}
		}
	}

	// Did we find a player?
	if( pClosestPlayer )
	{
		// Set the player instance
		*pNetworkPlayer = pClosestPlayer;
		return true;
	}

	*pNetworkPlayer = NULL;
	return false;
}

void CNetworkVehicle::ProcessUnoccupiedSync( RakNet::BitStream * pBitStream )
{
	// Read the sync data from the bitstream
	UnoccupiedVehicleSync unoccupiedVehicleSync;
	pBitStream->Read( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );

	// Update the last sync data
	//m_lastSyncData.m_vecPosition = unoccupiedVehicleSync.m_vecPosition;
	//m_lastSyncData.m_quatRotation = unoccupiedVehicleSync.m_quatRotation;

	// Reset the bitstream
	pBitStream->Reset();

	// Do we have a last syncer?
	if( m_pLastSyncer )
	{
		// Write the vehicle id
		pBitStream->WriteCompressed( m_vehicleId );

		// Write the sync data
		pBitStream->Write( (char *)&unoccupiedVehicleSync, sizeof(UnoccupiedVehicleSync) );

		// Send it back to all clients
		pCore->GetNetworkModule()->Call( RPC_UNOCCUPIED_SYNC, pBitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, m_pLastSyncer->GetId(), true );
	}
}