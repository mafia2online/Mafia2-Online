/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CBlip.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CBlip
{

private:

	EntityId				m_blipId;
	int						m_internalId;
	float					m_fX;
	float					m_fY;
	int						m_iLibrary;
	int						m_iIcon;
	bool					m_bCreated;
	EntityId				m_attachedTo;
	eBlipType				m_blipType;

public:

							CBlip						( EntityId blipId, float fX, float fY, int iLibrary, int iIcon );
							~CBlip						( void );

	void					Create						( void );
	void					Destroy						( void );
	bool					IsCreated					( void ) { return m_bCreated; }

	void					AttachToPlayer				( CNetworkPlayer * pPlayer );
	void					AttachToVehicle				( CNetworkVehicle * pVehicle );
	void					AttachToPed					( CPed * pPed );

	void					Detach						( bool bRecreate = true );

	bool					IsAttachedToEntity			( void ) { return (m_attachedTo != INVALID_ENTITY_ID); }
	bool					IsAttachedToEntity			( EntityId entityId ) { return (m_attachedTo == entityId); }
	EntityId				GetAttachedEntity			( void ) { return m_attachedTo; }

	int						GetInternalId				( void ) { return m_internalId; }
	eBlipType				GetType						( void ) { return m_blipType; }

	bool					IsAttachedToPlayer			( void ) { return (m_blipType == eBlipType::BLIP_TYPE_PLAYER); }
	bool					IsAttachedToPlayer			( EntityId playerId ) { return (m_blipType == eBlipType::BLIP_TYPE_PLAYER && m_attachedTo == playerId); }
	bool					IsAttachedToVehicle			( void ) { return (m_blipType == eBlipType::BLIP_TYPE_VEHICLE); }
	bool					IsAttachedToVehicle			( EntityId vehicleId ) { return (m_blipType == eBlipType::BLIP_TYPE_VEHICLE && m_attachedTo == vehicleId); }
	bool					IsAttachedToPed				( void ) { return (m_blipType == eBlipType::BLIP_TYPE_PED); }
	bool					IsAttachedToPed				( EntityId pedId ) { return (m_blipType == eBlipType::BLIP_TYPE_PED && m_attachedTo == pedId); }

};