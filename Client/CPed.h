/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPed.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "engine/CM2ModelManager.h"
#include "CModelManager.h"

class CPed
{

private:

	EntityId						m_pedId;
	unsigned int					m_uiModelIndex;
	String							m_pNick;
	bool							m_pShowNick;

	CM2Ped							* m_pPed;
	CM2ModelManager					* m_pPedModelManager;

public:

									CPed					( unsigned int uiModelIndex, CVector3 vecPosition, CVector3 vecRotation );
									~CPed					( void );

	void							Create					( CVector3 position, CVector3 rotation );
	void							Destroy					( bool del );

	void							SetId					( EntityId pedId ) { m_pedId = pedId; }
	EntityId						GetId					( void ) { return m_pedId; }

	void							SetModel				( unsigned int uiModelIndex );
	unsigned int					GetModel				( void ) { return m_uiModelIndex; }

	void							SetNick					( const char * szNick );
	String							GetNick					( void ) { return m_pNick; }
	void							ShowNick				( bool bShow) { m_pShowNick = bShow; }
	bool							GetShowNick				( void ) { return m_pShowNick; }

	/*
	void							SetPosition				( CVector3 vecPosition );
	void							GetPosition				( CVector3 * vecPosition );

	void							SetRotation				( CVector3 vecRotation );
	void							GetRotation				( CVector3 * vecRotation );
	*/

	CM2Ped							* GetPed				( void ) { return m_pPed; }

};