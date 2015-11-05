/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPedManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CPed.h"

class CPedManager
{

private:

	CPed														* m_pPed[MAX_PEDS];

public:

									CPedManager					( void );
									~CPedManager				( void );

	EntityId						Add							( int iModel, CVector3 vecPosition, CVector3 vecRotation );
	bool							Remove						( EntityId pedId );
	bool							IsActive					( EntityId pedId );

	EntityId						FindFreeSlot				( void );
	void							HandlePlayerJoin			( EntityId playerId );

	CPed							* Get						( EntityId pedId ) { return m_pPed[ pedId ]; }
	EntityId						GetCount					( void );

};