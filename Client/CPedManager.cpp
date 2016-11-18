/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPedManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"
#include "CString.h"

#include "CPed.h"

#include "CPedManager.h"

#include "CLogFile.h"

CPedManager::CPedManager( void )
{
	// Reset all ped pointers
	memset( m_pPed, NULL, sizeof(m_pPed) );
}

CPedManager::~CPedManager( void )
{
	// Loop over each ped
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Delete the current ped
		Delete( i );
	}
}

EntityId CPedManager::Add( unsigned int iModelIndex, CVector3 vecPosition, CVector3 vecRotation )
{
	// Generate a new id
	EntityId pedId = FindFreeSlot();

	// Can't find a slot?
	if( pedId == INVALID_ENTITY_ID )
		return INVALID_ENTITY_ID;

	DEBUG_LOG( "Creating ped with id %d...", pedId );

	// Create a new ped
	m_pPed[ pedId ] = new CPed( iModelIndex, vecPosition, vecRotation );

	DEBUG_LOG( "Ped created! 0x%p", m_pPed[ pedId ] );

	// Is the ped invalid?
	if( !m_pPed[ pedId ] )
		return INVALID_ENTITY_ID;

	// Set the id
	m_pPed[ pedId ]->SetId( pedId );

	// Set the nick
	String nick;
	nick.Format("Ped %d", pedId);
	m_pPed[pedId]->SetNick(nick);
	m_pPed[pedId]->ShowNick(false);
	return pedId;
}

bool CPedManager::Delete( EntityId pedId )
{
	// Is the ped inactive?
	if( !IsActive( pedId ) )
		return false;

	// Delete the ped instance
	SAFE_DELETE( m_pPed[ pedId ] );

	return true;
}

CPed * CPedManager::Get( EntityId pedId )
{
	// Is the current ped inactive?
	if( !IsActive( pedId ) )
		return NULL;

	return m_pPed[ pedId ];
}

EntityId CPedManager::GetCount( void )
{
	//
	EntityId count = 0;

	// Loop over all peds
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Is the current ped active?
		if( IsActive( i ) )
			count++;
	}

	return count;
}

EntityId CPedManager::FindFreeSlot( void )
{
	// Loop over all peds
	for( EntityId i = 0; i < MAX_PEDS; i++ )
	{
		// Is the current ped inactive?
		if( !IsActive( i ) )
			return i;
	}

	return INVALID_ENTITY_ID;
}

void CPedManager::Pulse( void )
{
	//Todo : delete this func
}