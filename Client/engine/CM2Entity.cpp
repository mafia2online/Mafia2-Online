/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Entity.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "../StdInc.h"

extern CCore		* pCore;

CM2Entity::CM2Entity( M2Entity * pEntity )
{
	// Set the entity
	SetEntity( pEntity );
}

CM2Entity::~CM2Entity( void )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD C_Entity__Delete = 0x1195580;
		M2Entity * pEntity = m_pEntity;

		_asm mov ecx, pEntity;
		_asm call C_Entity__Delete;
	}
}

void CM2Entity::Activate( void )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = FUNC_CEntity__Activate;
		M2Entity * pEntity = m_pEntity;

		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::Deactivate( void )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = FUNC_CEntity__Deactivate;
		M2Entity * pEntity = m_pEntity;

		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

DWORD CM2Entity::GetGUID( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_dwGUID;

	return 0;
}

void CM2Entity::SetPosition( CVector3 vecPosition )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetPosition;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetPosition( CVector3 * vecPosition, bool bCheckVehicle )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->GetPosition;
		M2Entity * pEntity = m_pEntity;

		_asm push vecPosition;
		_asm mov ecx, pEntity;
		_asm call dwFunc;

		// If the ped is in a vehicle, adjust the Z axis
		// TODO: Use the engine function which does exactly this.
		if( bCheckVehicle && ((M2Ped *)m_pEntity)->m_pCurrentVehicle )
			vecPosition->fZ -= 1.0f;
	}
}

void CM2Entity::SetDirection( CVector3 vecDirection )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetDirection;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, vecDirection;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetDirection( CVector3 * vecDirection )
{
	// Is the entity valid?
	if ( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->GetDirection;
		M2Entity * pEntity = m_pEntity;

		_asm push vecDirection;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::SetRotation( Quaternion quatRotation )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetRotation;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, quatRotation;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetRotation( Quaternion * quatRotation )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->GetRotation;
		M2Entity * pEntity = m_pEntity;

		_asm push quatRotation;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

bool CM2Entity::IsOnScreen( void )
{
	bool bIsOnScreen = false;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pModel && m_pEntity->m_pModel->m_pSingleMesh && m_pEntity->m_pModel->m_pSingleMesh->m_pRenderSkinnedVisual )
	{
		// Get the model last render time
		DWORD dwLastModelRenderTime = m_pEntity->m_pModel->m_pSingleMesh->m_pRenderSkinnedVisual->m_dwLastRenderTime;

		// Get the engine core last render time
		void* pEngineCore = *(void **)0x1AAF6C8;
		DWORD dwEngineLastRenderTime = *(DWORD *)((DWORD)pEngineCore + 0x9C);

		// Has the model been rendered recently?
		if( dwLastModelRenderTime >= dwEngineLastRenderTime )
			bIsOnScreen = true;
	}

	return bIsOnScreen;
}

void CM2Entity::ShowModel( bool bShow )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pModel )
	{
		M2Model * pModel = m_pEntity->m_pModel;

		// Toggle the model visibilty
		if( bShow )
			pModel->m_dwFlags |= 1;
		else
			pModel->m_dwFlags &= 0xFFFFFFFE;

		// Refresh model?
		DWORD dwFunc = 0x14AEAC0;
		_asm push 6;
		_asm mov ecx, pModel;
		_asm call dwFunc;
	}
}

bool CM2Entity::IsModelShowing( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pModel )
		return (m_pEntity->m_pModel->m_dwFlags & 1);

	return false;
}