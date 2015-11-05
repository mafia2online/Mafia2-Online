/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPedManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CPedManager::CPedManager( void )
{
	// Reset all ped pointers
	memset( m_pPed, NULL, sizeof(m_pPed) );
}

CPedManager::~CPedManager( void )
{
	// Loop over all ped instances
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Remove the current ped
		Remove( i );
	}
}

EntityId CPedManager::Add( int iModel, CVector3 vecPosition, CVector3 vecRotation )
{
	// Find a free ped slot
	EntityId pedId = FindFreeSlot ();

	// Is the ped invalid?
	if( pedId < 0 || pedId > MAX_PEDS )
		return INVALID_ENTITY_ID;

	// Create the ped instance
	m_pPed[pedId] = new CPed( iModel, vecPosition, vecRotation );

	// Did the ped fail to create?
	if( !IsActive(pedId) )
		return INVALID_ENTITY_ID;

	// Set the ped id
	m_pPed[pedId]->SetId ( pedId );

	// Add this ped for everyone
	m_pPed[pedId]->AddForWorld ();

	return pedId;
}

bool CPedManager::Remove( EntityId pedId )
{
	// Is the ped invalid?
	if( !IsActive( pedId ) )
		return false;

	// Remove this ped for everyone
	m_pPed[pedId]->RemoveForWorld ();

	// Delete the ped instance
	SAFE_DELETE( m_pPed[pedId] );

	return true;
}

bool CPedManager::IsActive ( EntityId pedId )
{
	// Is the ped id invalid?
	if ( pedId < 0 || pedId > MAX_BLIPS )
		return false;

	return (m_pPed[pedId] != NULL);
}

EntityId CPedManager::FindFreeSlot( void )
{
	//
	EntityId pedId = MAX_PEDS;

	// Loop over all ped instances
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Is the current ped not active?
		if( !IsActive(i) )
		{
			// Set the ped id to this id
			pedId = i;

			break;
		}
	}

	return pedId;
}

void CPedManager::HandlePlayerJoin( EntityId playerId )
{
	// Loop over all ped instances
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Is the current ped active?
		if( IsActive(i) )
		{
			// Add the ped for this player
			m_pPed[i]->AddForPlayer( playerId );
		}
	}
}

EntityId CPedManager::GetCount ( void )
{
	EntityId count = 0;

	// Loop over all ped instances
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Is the current ped active?
		if( IsActive(i) )
		{
			// Increase the ped count
			count ++;
		}
	}

	return count;
}