/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPed.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPed
{

private:

	EntityId						m_pedId;
	int								m_iModel;

public:

									CPed						( int iModel, CVector3 vecPosition, CVector3 vecRotation );
									~CPed						( void );

	void							SetId						( EntityId pedId ) { m_pedId = pedId; }
	EntityId						GetId						( void ) { return m_pedId; }

	void							AddForPlayer				( EntityId playerId );
	void							AddForWorld					( void );

	void							RemoveForPlayer				( EntityId playerId );
	void							RemoveForWorld				( void );

	void							SetModel					( int iModel, bool bSendToClient = true );
	int								GetModel					( void ) { return m_iModel; }

};