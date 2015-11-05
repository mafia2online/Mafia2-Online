/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPed.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPed
{

private:

	EntityId						m_pedId;
	unsigned int					m_uiModelIndex;
	CM2Ped							* m_pPed;
	M2ModelMgr						* m_pPedModelManager;

public:

									CPed					( unsigned int uiModelIndex, CVector3 vecPosition, CVector3 vecRotation );
									~CPed					( void );

	void							SetId					( EntityId pedId ) { m_pedId = pedId; }
	EntityId						GetId					( void ) { return m_pedId; }

	void							SetModel				( unsigned int uiModelIndex );
	unsigned int					GetModel				( void ) { return m_uiModelIndex; }

	/*
	void							SetPosition				( CVector3 vecPosition );
	void							GetPosition				( CVector3 * vecPosition );

	void							SetRotation				( CVector3 vecRotation );
	void							GetRotation				( CVector3 * vecRotation );
	*/

	CM2Ped							* GetPed				( void ) { return m_pPed; }

};