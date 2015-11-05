/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPedManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPedManager
{

private:

	CPed							* m_pPed[ MAX_PEDS ];

public:

									CPedManager				( void );
									~CPedManager			( void );

	EntityId						Add						( unsigned int uiModelIndex, CVector3 vecPosition, CVector3 vecRotation );
	bool							Delete					( EntityId pedId );

	CPed							* Get					( EntityId pedId );
	bool							IsActive				( EntityId pedId ) { return (m_pPed[ pedId ] != NULL); }

	EntityId						GetCount				( void );
	EntityId						FindFreeSlot			( void );

	void							Pulse					( void );

};