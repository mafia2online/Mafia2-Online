/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlipManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CBlipManager::CBlipManager( void )
{
	// Reset all blip pointers
	memset( m_pBlip, NULL, sizeof(m_pBlip) );
}

CBlipManager::~CBlipManager( void )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Remove the current blip
		Remove( i );
	}
}

EntityId CBlipManager::Add( float fX, float fY, int iLibrary, int iIcon )
{
	// Find a free blip slot
	EntityId blipId = FindFreeSlot ();

	// Is the blip invalid?
	if( blipId < 0 || blipId > MAX_BLIPS )
		return INVALID_ENTITY_ID;

	// Create the blip instance
	m_pBlip[blipId] = new CBlip( blipId, Vector2( fX, fY ), iLibrary, iIcon );

	// Did the blip fail to create?
	if( !IsActive(blipId) )
		return INVALID_ENTITY_ID;

	// Add this blip for everyone
	m_pBlip[blipId]->AddForWorld ();

	return blipId;
}

bool CBlipManager::Remove( EntityId blipId )
{
	// Is the blip invalid?
	if( !IsActive( blipId ) )
		return false;

	// Remove this blip for everyone
	m_pBlip[blipId]->RemoveForWorld ();

	// Delete the blip instance
	SAFE_DELETE( m_pBlip[blipId] );

	return true;
}

bool CBlipManager::IsActive ( EntityId blipId )
{
	// Is the blip id invalid?
	if ( blipId < 0 || blipId > MAX_BLIPS )
		return false;

	return (m_pBlip[blipId] != NULL);
}

EntityId CBlipManager::FindFreeSlot( void )
{
	//
	EntityId blipId = MAX_BLIPS;

	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current blip not active?
		if( !IsActive(i) )
		{
			// Set the blip id to this blip
			blipId = i;

			break;
		}
	}

	return blipId;
}

void CBlipManager::HandlePlayerJoin( EntityId playerId )
{
	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current blip active?
		if( IsActive(i) )
		{
			// Add the blip for this player
			m_pBlip[i]->AddForPlayer( playerId );
		}
	}
}

EntityId CBlipManager::GetCount ( void )
{
	EntityId count = 0;

	// Loop over all blip instances
	for( EntityId i = 0; i < MAX_BLIPS; i++ )
	{
		// Is the current blip active?
		if( IsActive(i) )
		{
			// Increase the blip count
			count ++;
		}
	}

	return count;
}