/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlip.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CBlip::CBlip( EntityId blipId, float fX, float fY, int iLibrary, int iIcon )
{
	// Save the variables
	m_blipId = blipId;
	m_internalId = -1;
	m_fX = fX;
	m_fY = fY;
	m_iLibrary = iLibrary;
	m_iIcon = iIcon;
	m_bCreated = false;
	m_attachedTo = INVALID_ENTITY_ID;
	m_blipType = eBlipType::BLIP_TYPE_STANDALONE;
}

CBlip::~CBlip( void )
{
	// Destroy the blip
	Destroy();
}

void CBlip::Create( void )
{
	// Is the blip already created?
	if( m_bCreated )
		return;

	// Create the blip
	m_internalId = pCore->GetGame()->GetNavigation()->RegisterIconPos( Vector2( m_fX, m_fY ), m_iLibrary, m_iIcon );

	// Mark as created
	m_bCreated = true;
}

void CBlip::Destroy( void )
{
	// Is the blip not created?
	if( !m_bCreated )
		return;

	// Is the blip attached to an entity?
	if ( IsAttachedToEntity () )
		Detach ( false );
	else
		pCore->GetGame()->GetNavigation()->UnregisterIconPos( m_internalId );

	// Reset
	m_internalId = -1;
	m_bCreated = false;
	m_attachedTo = INVALID_ENTITY_ID;
	m_blipType = eBlipType::BLIP_TYPE_STANDALONE;
}

void CBlip::AttachToPlayer ( CNetworkPlayer * pPlayer )
{
	// Destroy the blip if it's already created
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	// Is the playerped valid?
	if ( pPlayer->GetPlayerPed () && pPlayer->GetPlayerPed()->GetPed () )
	{
		// Create the blip attached to the player entity
		m_internalId = pCore->GetGame()->GetNavigation()->RegisterIconEntity ( pPlayer->GetPlayerPed()->GetEntity(), m_iLibrary, m_iIcon );

		// Mark as created
		m_bCreated = true;

		// Set the attached entity
		m_attachedTo = pPlayer->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_PLAYER;
	}

	// Attach to the player
	pPlayer->AttachBlip ( this );
}

void CBlip::AttachToVehicle ( CNetworkVehicle * pVehicle )
{
	// Destroy the blip if it's already created
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	// Is the vehicle valid?
	if ( pVehicle->GetVehicle () && pVehicle->GetVehicle()->GetVehicle () )
	{
		// Create the blip attached to the player entity
		m_internalId = pCore->GetGame()->GetNavigation()->RegisterIconEntity ( pVehicle->GetVehicle()->GetEntity(), m_iLibrary, m_iIcon );

		CLogFile::Printf ( "CBlip::AttachToVehicle() - Blip attached! (Internal id: %d, Lib: %d, Icon: %d)", m_internalId, m_iLibrary, m_iIcon );

		// Mark as created
		m_bCreated = true;

		// Set the attached entity
		m_attachedTo = pVehicle->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_VEHICLE;
	}

	// Attach to the vehicle
	pVehicle->AttachBlip ( this );
}

void CBlip::AttachToPed ( CPed * pPed )
{
	// Destroy the blip if it's already created
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	// Is the ped valid?
	if ( pPed->GetPed () && pPed->GetPed()->GetPed () )
	{
		// Create the blip attached to the player entity
		m_internalId = pCore->GetGame()->GetNavigation()->RegisterIconEntity ( pPed->GetPed()->GetEntity(), m_iLibrary, m_iIcon );

		// Mark as created
		m_bCreated = true;

		// Set the attached entity
		m_attachedTo = pPed->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_PED;
	}

	// Attach to the ped
	//pPed->AttachBlip ( this );
}

void CBlip::Detach ( bool bRecreate )
{
	// Is the blip attached to anything?
	if ( IsAttachedToEntity () )
	{
		// Get the attached entity
		EntityId entityId = GetAttachedEntity ();
		M2Entity * pAttachedEntity = NULL;
		
		// Find the attached entity
		if ( m_blipType == eBlipType::BLIP_TYPE_PLAYER )
			pAttachedEntity = pCore->GetPlayerManager()->Get ( entityId )->GetPlayerPed()->GetPed ();
		else if ( m_blipType == eBlipType::BLIP_TYPE_VEHICLE )
			pAttachedEntity = pCore->GetVehicleManager()->Get ( entityId )->GetVehicle()->GetVehicle ();
		else if ( m_blipType == eBlipType::BLIP_TYPE_PED )
			pAttachedEntity = pCore->GetPedManager()->Get ( entityId )->GetPed()->GetPed ();

		// Destroy the blip
		if ( pAttachedEntity )
			pCore->GetGame()->GetNavigation()->UnregisterIconEntity ( pAttachedEntity );

		// Reset
		m_internalId = -1;
		m_bCreated = false;
		m_attachedTo = INVALID_ENTITY_ID;
		m_blipType = eBlipType::BLIP_TYPE_STANDALONE;

		// Should we recreate the blip?
		if ( bRecreate )
			Create ();
	}
}