/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2SyncObject.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "Math/CVector3.h"

#include "CM2SyncObject.h"

int _declspec(naked) C_SyncObject::SetStatus(int status)
{
	_asm {
		mov eax, 0x11C4C60
		jmp eax
	}
}

unsigned _declspec(naked) C_SyncObject::Done(void)
{
	_asm {
		mov eax, 0x11DE8C0
		jmp eax
	}
}

CM2SyncObject::CM2SyncObject( C_SyncObject * pSyncObject )
{
	SetSyncObject( pSyncObject );

	m_bDoneOnDelete = true;
}

CM2SyncObject::~CM2SyncObject( void )
{
	if( m_pSyncObject && m_bDoneOnDelete ) {
		m_pSyncObject->SetStatus(0);
		m_pSyncObject->Done();
	}
}

void CM2SyncObject::Terminate( void )
{
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		*(DWORD*)((DWORD)m_pSyncObject->m_pUserData->m_pAICommand + 0xD) = 0;
}

bool CM2SyncObject::IsTerminated( void )
{
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		return (*(DWORD*)((DWORD)m_pSyncObject->m_pUserData->m_pAICommand + 0xD) == 0);

	return false;
}

bool CM2SyncObject::IsSuccess( void )
{
	if( m_pSyncObject && m_pSyncObject->m_pUserData && m_pSyncObject->m_pUserData->m_pAICommand )
		return m_pSyncObject->m_pUserData->m_pAICommand->m_dwState == 1;

	return false;
}

bool CM2SyncObject::IsFail( void )
{
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
	if( m_pSyncObject )
	{
		if( type == E_TYPE_MOVE || type == E_TYPE_SHOOT )
			memcpy( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget, &vecTarget, sizeof(CVector3) );
		else if( type == E_TYPE_AIM )
			memcpy( &((C_AICommandAim *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget->m_vector, &vecTarget, sizeof(CVector3) );

		//if( IsDone() && type == E_TYPE_MOVE )
		//	Reactivate(); // todo: doesn't work
	}
}

void CM2SyncObject::GetTarget( CVector3 * vecTarget, int type )
{
	if( m_pSyncObject )
	{
		if( type == E_TYPE_MOVE || type == E_TYPE_SHOOT )
			memcpy( vecTarget, &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget, sizeof(CVector3) );
		else if( type == E_TYPE_AIM )
			memcpy( vecTarget, &((C_AICommandAim *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecTarget->m_vector, sizeof(CVector3) );
	}
}

void CM2SyncObject::SetMoveTargetData ( CVector3 vecEndDirection, BYTE bMoveType )
{
	if( m_pSyncObject )
	{
		memcpy ( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_vecDirectionTarget, &vecEndDirection, sizeof ( CVector3 ) );

		memcpy ( &((C_AICommandMove *)m_pSyncObject->m_pUserData->m_pAICommand)->m_byteTargetStyle, &bMoveType, sizeof ( BYTE ) );
	}
}
