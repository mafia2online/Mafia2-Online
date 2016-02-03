/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CStreamableEntity.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CStreamer.h"
#include "CStreamableEntity.h"

CStreamableEntity::CStreamableEntity( eStreamEntityType type, CM2Entity * pEntity, float fStreamDistance )
{
	// Reset
	m_bIsStreamedIn = false;
	m_fStreamingDistance = fStreamDistance;
	m_pStreamEntity = pEntity;

	// Set the streamable entity type
	SetType( type );

	// Add ourselfs to the streamer
	CStreamer::Instance()->push_back( this );
}

CStreamableEntity::~CStreamableEntity( void )
{
	// Remove ourselfs from the streamer
	CStreamer::Instance()->remove( this );
}

void CStreamableEntity::StreamIn( void )
{
	// Are we already streamed in?
	if( m_bIsStreamedIn )
		return;

	// Mark as streamed in
	m_bIsStreamedIn = true;
}

void CStreamableEntity::StreamOut( void )
{
	// Are we already streamed out?
	if( !m_bIsStreamedIn )
		return;

	// Mark as streamed out
	m_bIsStreamedIn = false;
}