/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2SyncObject.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CM2SyncObject::CM2SyncObject( C_SyncObject * pSyncObject )
{
	// Set the sync object
	SetSyncObject( pSyncObject );

	// Mark as done when deleted
	m_bDoneOnDelete = true;
}

CM2SyncObject::~CM2SyncObject( void )
{
	// Is the sync object valid and should we delete this?
	if( m_pSyncObject && m_bDoneOnDelete )
	{
		// Call the sync object destructor
		C_SyncObject * pSyncObject = m_pSyncObject;
		DWORD dwFunc = 0x11DE8C0; // C_SyncObject::Done

		_asm mov ecx, pSyncObject;
		_asm call dwFunc;
	}
}

void CM2SyncObject::Terminate( void )
{
	// Is the object valid?
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		*(DWORD*)((DWORD)m_pSyncObject->m_pUserData->m_pAICommand + 0xD) = 0;
}

bool CM2SyncObject::IsTerminated( void )
{
	// Is the object valid?
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		return (*(DWORD*)((DWORD)m_pSyncObject->m_pUserData->m_pAICommand + 0xD) == 0);

	return false;
}

bool CM2SyncObject::IsSuccess( void )
{
	// Is the object valid?
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		return m_pSyncObject->m_pUserData->m_pAICommand->m_dwState == 1;

	return false;
}

bool CM2SyncObject::IsFail( void )
{
	// Is the object valid?
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		return m_pSyncObject->m_pUserData->m_pAICommand->m_dwState == 2;

	return false;
}

bool CM2SyncObject::IsDone( void )
{
	return (IsSuccess() || IsFail());
}

void CM2SyncObject::Reactivate( void )
{
	// Is the object valid?
	if( m_pSyncObject )
	{
		/*
		// Is the sync object done?
		if( IsDone() )
		{
			// Update the flags
			m_pSyncObject->m_dwFlags |= 1u;

			// Get the sync object pointer
			C_SyncObject * pSyncObject = m_pSyncObject;

			_asm mov ecx, pSyncObject;
			_asm call COffsets::FUNC_CSyncObject__Reactivate;

			CLogFile::Printf( "C_SyncObject reactived!" );
		}
		*/
	}
}

void CM2SyncObject::SetTarget( CVector3 vecTarget, int type )
{
	// Is the sync object valid?
	if( m_pSyncObject )
	{
		// Copy the target into the ai command target
		if( type == E_TYPE_MOVE || type == E_TYPE_SHOOT )
			memcpy( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget, &vecTarget, sizeof(CVector3) );
		else if( type == E_TYPE_AIM )
			memcpy( &((C_AICommandAim *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget->m_vector, &vecTarget, sizeof(CVector3) );

		// Reactivate the sync object if it's done
		//if( IsDone() && type == E_TYPE_MOVE )
		//	Reactivate(); // todo: doesn't work
	}
}

void CM2SyncObject::GetTarget( CVector3 * vecTarget, int type )
{
	// Is the sync object valid?
	if( m_pSyncObject )
	{
		// Copy the ai command target into the target
		if( type == E_TYPE_MOVE || type == E_TYPE_SHOOT )
			memcpy( vecTarget, &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget, sizeof(CVector3) );
		else if( type == E_TYPE_AIM )
			memcpy( vecTarget, &((C_AICommandAim *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget->m_vector, sizeof(CVector3) );
	}
}

void CM2SyncObject::SetMoveTargetData ( CVector3 vecEndDirection, BYTE bMoveType )
{
	// Is the sync object valid?
	if( m_pSyncObject )
	{
		// Copy the end direction target into the AI command
		memcpy ( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecDirectionTarget, &vecEndDirection, sizeof ( CVector3 ) );

		// Copy the move type into the AI command
		memcpy ( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_byteTargetStyle, &bMoveType, sizeof ( BYTE ) );
	}
}