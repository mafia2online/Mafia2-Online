/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPlayerManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore				* pCore;

CPlayerManager::CPlayerManager( void )
{
	// Reset all player pointers
	memset( m_pNetworkPlayer, NULL, sizeof(m_pNetworkPlayer) );
}

CPlayerManager::~CPlayerManager( void )
{
	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Remove the current player
		Remove( i, DISCONNECT_QUIT );
	}
}

bool CPlayerManager::Add( EntityId playerId, const char * szNick, const char * szIP, const char * szSerial )
{
	// Is the current player already active?
	if( IsActive(playerId) )
		return false;

	// Create the network player instance
	m_pNetworkPlayer[ playerId ] = new CNetworkPlayer;

	// Did the network player instance fail to create?
	if( !m_pNetworkPlayer[ playerId ] )
		return false;

	// Set the player id
	m_pNetworkPlayer[ playerId ]->SetId( playerId );

	// Set the player nickname
	m_pNetworkPlayer[ playerId ]->SetNick( szNick );

	// Set the player IP
	m_pNetworkPlayer[ playerId ]->SetIP( szIP );

	// Set the player serial
	m_pNetworkPlayer[ playerId ]->SetSerial( szSerial );

	// Add this player for everyone
	m_pNetworkPlayer[ playerId ]->AddForWorld();

	// Call scripting event
	CSquirrelArguments pArguments;
	pArguments.push( playerId );
	pArguments.push( szNick );
	pArguments.push( szIP );
	pArguments.push( szSerial );
	pCore->GetEvents()->Call( "onPlayerConnect", &pArguments );

	CLogFile::Printf( "[join] %s has connected to the server.", szNick );

	return true;
}

void CPlayerManager::Remove( EntityId playerId, unsigned int uiReason )
{
	// Is the player not active?
	if( !IsActive(playerId) )
		return;

	// Remove this player for everyone
	m_pNetworkPlayer[ playerId ]->RemoveForWorld();

	// Handle this quit with the vehicle manager
	pCore->GetVehicleManager()->HandlePlayerQuit( playerId );

	// Call scripting event
	CSquirrelArguments pArguments;
	pArguments.push( playerId );
	pArguments.push( SharedUtility::DiconnectReasonToString( uiReason ) );
	pCore->GetEvents()->Call( "onPlayerDisconnect", &pArguments );

	CLogFile::Printf( "[quit] %s has left the server (%s).", m_pNetworkPlayer[ playerId ]->GetNick(), SharedUtility::DiconnectReasonToString( uiReason ).Get() );

	// Delete the player instance
	SAFE_DELETE( m_pNetworkPlayer[ playerId ] );
}

bool CPlayerManager::IsActive( EntityId playerId )
{
	// Is the player id invalid?
	if ( playerId < 0 || playerId > MAX_PLAYERS )
		return false;

	return (m_pNetworkPlayer[ playerId ] != NULL);
}

void CPlayerManager::Pulse( void )
{
	// Loop over all players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive( i ) )
		{
			// Pulse the current player
			m_pNetworkPlayer[ i ]->Pulse();
		}
	}
}

EntityId CPlayerManager::GetCount( void )
{
	EntityId count = 0;

	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive( i ) )
			count++;
	}

	return count;
}

void CPlayerManager::HandlePlayerJoin( EntityId playerId )
{
	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this not the player who's joining?
		if( i != playerId && IsActive( i ) )
		{
			// Add this player for the current player
			m_pNetworkPlayer[ i ]->AddForPlayer( playerId );
		}
	}
}

void CPlayerManager::HandlePlayerSpawn( EntityId playerId )
{
	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this not the player who's spawning?
		if( i != playerId && IsActive( i ) )
		{
			// Add this player for the current player
			m_pNetworkPlayer[ i ]->SpawnForPlayer( playerId );
		}
	}
}

bool CPlayerManager::IsNickInUse( const char * szNick )
{
	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive( i ) )
		{
			// Is this the nick we're looking for?
			if( !strcmp( m_pNetworkPlayer[ i ]->GetNick(), szNick ) )
				return true;
		}
	}

	return false;
}

CNetworkPlayer * CPlayerManager::GetPlayerFromSerial( const char * szSerial )
{
	// Loop over all players
	for( int i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive( i ) )
		{
			// Is this the serial we're looking for?
			if( !strcmp( m_pNetworkPlayer[ i ]->GetSerial(), szSerial ) )
				return m_pNetworkPlayer[ i ];
		}
	}

	return NULL;
}