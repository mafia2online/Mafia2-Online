/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlip.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"

#include "CMafia.h"

#include "CPed.h"
#include "CNetworkPlayer.h"
#include "CNetworkVehicle.h"
#include "CVehicleManager.h"
#include "CPlayerManager.h"
#include "CPedManager.h"
#include "CRemotePlayer.h"

#include "engine/CM2Navigation.h"
#include "engine/CM2Ped.h"

#include "CBlip.h"

#include "CLogFile.h"

CBlip::CBlip( EntityId blipId, float fX, float fY, int iLibrary, int iIcon )
{
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
	Destroy();
}

void CBlip::Create( void )
{
	if( m_bCreated )
		return;

	m_internalId = CCore::Instance()->GetGame()->GetNavigation()->RegisterIconPos( Vector2( m_fX, m_fY ), m_iLibrary, m_iIcon );

	m_bCreated = true;
}

void CBlip::Destroy( void )
{
	if( !m_bCreated )
		return;

	if ( IsAttachedToEntity () )
		Detach ( false );
	else
		CCore::Instance()->GetGame()->GetNavigation()->UnregisterIconPos(m_internalId);

	m_internalId = -1;
	m_bCreated = false;
	m_attachedTo = INVALID_ENTITY_ID;
	m_blipType = eBlipType::BLIP_TYPE_STANDALONE;
}

void CBlip::AttachToPlayer ( CNetworkPlayer * pPlayer )
{
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	if ( pPlayer->GetPlayerPed () && pPlayer->GetPlayerPed()->GetPed () )
	{
		m_internalId = CCore::Instance()->GetGame()->GetNavigation()->RegisterIconEntity(pPlayer->GetPlayerPed()->GetEntity(), m_iLibrary, m_iIcon);

		m_bCreated = true;

		m_attachedTo = pPlayer->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_PLAYER;
	}

	pPlayer->AttachBlip ( this );
}

void CBlip::AttachToVehicle ( CNetworkVehicle * pVehicle )
{
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	if ( pVehicle->GetVehicle () && pVehicle->GetVehicle()->GetVehicle () )
	{
		m_internalId = CCore::Instance()->GetGame()->GetNavigation()->RegisterIconEntity(pVehicle->GetVehicle()->GetEntity(), m_iLibrary, m_iIcon);

		DEBUG_LOG ( "CBlip::AttachToVehicle() - Blip attached! (Internal id: %d, Lib: %d, Icon: %d)", m_internalId, m_iLibrary, m_iIcon );

		m_bCreated = true;

		m_attachedTo = pVehicle->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_VEHICLE;
	}

	pVehicle->AttachBlip ( this );
}

void CBlip::AttachToPed ( CPed * pPed )
{
	if ( m_bCreated )
		Destroy (); // todo: Find a way (through engine) to attach a blip to a entity without re-creating it

	if ( pPed->GetPed () && pPed->GetPed()->GetPed () )
	{
		m_internalId = CCore::Instance()->GetGame()->GetNavigation()->RegisterIconEntity(pPed->GetPed()->GetEntity(), m_iLibrary, m_iIcon);

		m_bCreated = true;

		m_attachedTo = pPed->GetId ();
		m_blipType = eBlipType::BLIP_TYPE_PED;
	}

	//pPed->AttachBlip ( this );
}

void CBlip::Detach ( bool bRecreate )
{
	if ( IsAttachedToEntity () )
	{
		EntityId entityId = GetAttachedEntity ();
		M2Entity * pAttachedEntity = NULL;

		if ( m_blipType == eBlipType::BLIP_TYPE_PLAYER )
			pAttachedEntity = CCore::Instance()->GetPlayerManager()->Get(entityId)->GetPlayerPed()->GetPed();
		else if ( m_blipType == eBlipType::BLIP_TYPE_VEHICLE )
			pAttachedEntity = CCore::Instance()->GetVehicleManager()->Get(entityId)->GetVehicle()->GetVehicle();
		else if ( m_blipType == eBlipType::BLIP_TYPE_PED )
			pAttachedEntity = CCore::Instance()->GetPedManager()->Get(entityId)->GetPed()->GetPed();

		if ( pAttachedEntity )
			CCore::Instance()->GetGame()->GetNavigation()->UnregisterIconEntity(pAttachedEntity);

		m_internalId = -1;
		m_bCreated = false;
		m_attachedTo = INVALID_ENTITY_ID;
		m_blipType = eBlipType::BLIP_TYPE_STANDALONE;

		if ( bRecreate )
			Create ();
	}
}