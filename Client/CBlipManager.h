/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CBlipManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CBlip.h"

class CBlipManager
{

private:

	CBlip						* m_pServerBlip[MAX_BLIPS];
	CBlip						* m_pClientBlip[MAX_BLIPS];

public:

								CBlipManager					( void );
								~CBlipManager					( void );

	bool						AddServerBlip					( EntityId blipId, float fX, float fY, int iLibrary, int iIcon, eBlipType blipType = eBlipType::BLIP_TYPE_STANDALONE, EntityId entityId = INVALID_ENTITY_ID );
	bool						RemoveServerBlip				( EntityId blipId );
	bool						IsServerBlipActive				( EntityId blipId );

	EntityId					AddClientBlip					( float fX, float fY, int iLibrary, int iIcon );
	bool						RemoveClientBlip				( EntityId blipId );
	bool						IsClientBlipActive				( EntityId blipId );
	EntityId					FindClientBlipSlot				( void );

	void						HandlePlayerDeath				( void );
	void						HandlePlayerRespawn				( void );

	CBlip						* GetServerBlip					( EntityId blipId ) { return m_pServerBlip[blipId]; }
	CBlip						* GetClientBlip					( EntityId blipId ) { return m_pClientBlip[blipId]; }

};