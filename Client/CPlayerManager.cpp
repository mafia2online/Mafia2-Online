/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

CPlayerManager::CPlayerManager( void )
{
	DEBUG_TRACE("CPlayerManager::CPlayerManager");

	// Create the localplayer instance
	m_pLocalPlayer = new CLocalPlayer;

	// Reset all player pointers
	memset( m_pNetworkPlayer, NULL, sizeof(m_pNetworkPlayer) );
}

CPlayerManager::~CPlayerManager( void )
{
	DEBUG_TRACE("CPlayerManager::~CPlayerManager");

	// Delete the localplayer instance
	SAFE_DELETE( m_pLocalPlayer );

	// Loop through all entities
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive(i) )
		{
			// Delete the network player instance
			SAFE_DELETE( m_pNetworkPlayer[i] );
		}
	}
}

bool CPlayerManager::Add( EntityId playerId, String strNick, unsigned int uiColour )
{
	DEBUG_TRACE("CPlayerManager::Add");

	// Is the current player already active?
	if( IsActive(playerId) )
		return false;

	// Is this player the localplayer?
	if( playerId == m_pLocalPlayer->GetId() )
		return false;

	// Create the network player instance
	m_pNetworkPlayer[playerId] = new CRemotePlayer;

	// Was the network player instance created?
	if( m_pNetworkPlayer[playerId] )
	{
		// Set the player id
		m_pNetworkPlayer[playerId]->SetId( playerId );

		// Set the player nick
		m_pNetworkPlayer[playerId]->SetNick( strNick );

		// Set the player colour
		m_pNetworkPlayer[playerId]->SetColour( uiColour );

		// Call the client script event
		CSquirrelArguments pArguments;
		pArguments.push( playerId );
		pArguments.push( strNick );
		pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientPlayerConnect", &pArguments );

		return true;
	}

	return false;
}

void CPlayerManager::Remove( EntityId playerId )
{
	DEBUG_TRACE("CPlayerManager::Remove");

	// Is the player not active?
	if( !IsActive(playerId) )
		return;

	// Call the client script event
	CSquirrelArguments pArguments;
	pArguments.push( playerId );
	pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientPlayerDisconnect", &pArguments );

	// Destroy the network player instance
	SAFE_DELETE( m_pNetworkPlayer[playerId] );
}

void CPlayerManager::Pulse( void )
{
	// Pulse the localplayer instance
	if ( m_pLocalPlayer )
		m_pLocalPlayer->Pulse ();

	// Loop through all entities
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive(i) )
		{
			// Pulse this player
			m_pNetworkPlayer[i]->Pulse ();
		}
	}
}

CRemotePlayer * CPlayerManager::GetFromGameGUID( DWORD dwGUID )
{
	DEBUG_TRACE("CPlayerManager::GetFromGameGUID");

	// Is this the localplayer?
	if( dwGUID == m_pLocalPlayer->GetPlayerPed()->GetGUID() )
		return NULL;

	// Loop through all entities
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( IsActive(i) )
		{
			// Is the current player guid this guid?
			if( Get(i)->GetPlayerPed() && Get(i)->GetPlayerPed()->GetGUID() == dwGUID )
				return m_pNetworkPlayer[i];
		}
	}

	return NULL;
}

EntityId CPlayerManager::GetIdFromGameGUID( DWORD dwGUID )
{
	DEBUG_TRACE("CPlayerManager::GetIdFromGameGUID");

	// Is the guid invalid?
	if( dwGUID == 0 )
		return INVALID_ENTITY_ID;

	// Get the player from the game guid
	CRemotePlayer * pRemotePlayer = GetFromGameGUID( dwGUID );

	// Is the player invalid?
	if( !pRemotePlayer )
		return INVALID_ENTITY_ID;

	return pRemotePlayer->GetId();
}

void CPlayerManager::RespawnAll( void )
{
	DEBUG_TRACE("CPlayerManager::RespawnAll");

	// Loop through all entities
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active and not dead?
		if( IsActive(i) && !Get(i)->IsDead() )
		{
			// Create the ped
			Get(i)->Create();
		}
	}
}

void CPlayerManager::HandleLocalDeath( void )
{
	DEBUG_TRACE("CPlayerManager::HandleLocalDeath");
}

EntityId CPlayerManager::GetCount( void )
{
	EntityId count = 0;

	// Loop through all entities
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is this player active?
		if( IsActive(i) )
		{
			// Increase the player count
			count++;
		}
	}

	return count;
}

bool CPlayerManager::IsHumanInventoryLocalPlayer( DWORD dwHumanInventory )
{
	DEBUG_TRACE("CPlayerManager::IsHumanInventoryLocalPlayer");

	// Is the localplayer not valid?
	if( !m_pLocalPlayer )
		return false;

	// Get the localplayer ped
	CM2Ped * pLocalPed = m_pLocalPlayer->GetPlayerPed();

	// Is the localplayer ped invalid?
	if( !pLocalPed )
		return false;

	return ((DWORD)pLocalPed->GetPed()->m_pWeaponData == dwHumanInventory);
}