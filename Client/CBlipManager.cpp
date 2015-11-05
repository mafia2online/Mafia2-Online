/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlipManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore		* pCore;

CBlipManager::CBlipManager( void )
{
	// Reset all server blip pointers
	memset( m_pServerBlip, NULL, sizeof(m_pServerBlip) );

	// Reset all client blip pointers
	memset( m_pClientBlip, NULL, sizeof(m_pClientBlip) );
}

CBlipManager::~CBlipManager( void )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current server blip active?
		if( IsServerBlipActive(i) )
		{
			// Delete the current blip
			SAFE_DELETE( m_pServerBlip[i] );
		}

		// Is the current client blip active?
		if( IsClientBlipActive(i) )
		{
			// Delete the current blip
			SAFE_DELETE( m_pClientBlip[i] );
		}
	}
}

bool CBlipManager::AddServerBlip( EntityId blipId, float fX, float fY, int iLibrary, int iIcon, eBlipType blipType, EntityId entityId )
{
	// Is the blip invalid?
	if( blipId < 0 || blipId >= MAX_BLIPS )
		return false;

	// Create the blip instance
	m_pServerBlip[blipId] = new CBlip( blipId, fX, fY, iLibrary, iIcon );

	// Did the blip fail to create?
	if( !IsServerBlipActive(blipId) )
		return false;

	// Are we attaching the blip to an entity?
	if ( blipType == eBlipType::BLIP_TYPE_PLAYER )
		m_pServerBlip[blipId]->AttachToPlayer ( pCore->GetPlayerManager()->Get(entityId) );
	else if ( blipType == eBlipType::BLIP_TYPE_VEHICLE )
		m_pServerBlip[blipId]->AttachToVehicle ( pCore->GetVehicleManager()->Get(entityId) );
	else if ( blipType == eBlipType::BLIP_TYPE_PED )
		m_pServerBlip[blipId]->AttachToPed ( pCore->GetPedManager()->Get(entityId) );
	else
		m_pServerBlip[blipId]->Create();

	return true;
}

bool CBlipManager::RemoveServerBlip( EntityId blipId )
{
	// Is the blip invalid?
	if( !IsServerBlipActive( blipId ) )
		return false;

	// Destroy the blip
	m_pServerBlip[blipId]->Destroy();

	// Delete the blip instance
	SAFE_DELETE( m_pServerBlip[blipId] );

	return true;
}

bool CBlipManager::IsServerBlipActive( EntityId blipId )
{
	// Is the blipId invalid?
	if( blipId < 0 || blipId == INVALID_ENTITY_ID )
		return false;

	return (m_pServerBlip[blipId] != NULL);
}

EntityId CBlipManager::AddClientBlip( float fX, float fY, int iLibrary, int iIcon )
{
	// Find a client blip slot
	EntityId blipId = FindClientBlipSlot( );
	
	// Is the blip invalid?
	if( blipId < 0 || blipId >= MAX_BLIPS )
		return INVALID_ENTITY_ID;

	// Create the blip instance
	m_pClientBlip[blipId] = new CBlip( blipId, fX, fY, iLibrary, iIcon );

	// Did the blip fail to create?
	if( !IsClientBlipActive(blipId) )
		return INVALID_ENTITY_ID;

	// Create the client blip
	m_pClientBlip[blipId]->Create();

	return blipId;
}

bool CBlipManager::RemoveClientBlip( EntityId blipId )
{
	// Is the blip invalid?
	if( !IsClientBlipActive( blipId ) )
		return false;

	// Destroy the blip
	m_pClientBlip[blipId]->Destroy();

	// Delete the blip instance
	SAFE_DELETE( m_pClientBlip[blipId] );

	return true;
}

bool CBlipManager::IsClientBlipActive( EntityId blipId )
{
	// Is the blipId invalid?
	if( blipId < 0 || blipId >= MAX_BLIPS )
		return false;

	return (m_pClientBlip[blipId] != NULL);
}

EntityId CBlipManager::FindClientBlipSlot( void )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current blip not active?
		if( !IsClientBlipActive(i) )
			return i;
	}

	return INVALID_ENTITY_ID;
}

void CBlipManager::HandlePlayerDeath( void )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current server blip active?
		if( IsServerBlipActive(i) )
		{
			// Destroy the current server blip
			m_pServerBlip[i]->Destroy();
		}

		// Is the current client blip active?
		if( IsClientBlipActive(i) )
		{
			// Destroy the current client blip
			m_pClientBlip[i]->Destroy();
		}
	}
}

void CBlipManager::HandlePlayerRespawn( void )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current server blip active?
		if( IsServerBlipActive(i) )
		{
			// Create the current server blip
			m_pServerBlip[i]->Create();
		}

		// Is the current client blip active?
		if( IsClientBlipActive(i) )
		{
			// Create the current client blip
			m_pClientBlip[i]->Create();
		}
	}
}