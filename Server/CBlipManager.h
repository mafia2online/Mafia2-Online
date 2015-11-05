/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlipManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CBlip.h"

class CBlipManager : public CMultiplayerBlipManager
{

private:

	CBlip														* m_pBlip[MAX_BLIPS];

public:

									CBlipManager				( void );
									~CBlipManager				( void );

	EntityId						Add							( float fX, float fY, int iLibrary, int iIcon );
	bool							Remove						( EntityId blipId );
	bool							IsActive					( EntityId blipId );

	EntityId						FindFreeSlot				( void );
	void							HandlePlayerJoin			( EntityId playerId );

	CBlip							* Get						( EntityId blipId ) { return m_pBlip[ blipId ]; }
	EntityId						GetCount					( void );

};