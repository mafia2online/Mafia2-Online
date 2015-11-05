/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2SyncObject.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "COffsets.h"

enum eTargetType
{
	E_TYPE_MOVE = 0,
	E_TYPE_AIM,
	E_TYPE_SHOOT
};

class C_Vector3
{
public:
	void* m_pVFTable;								// 0000 - 0004
	CVector3 m_vector;								// 0004 - 0008
};

class C_BaseAICommand
{
public:
	void* m_pVFTable;								// 0000 - 0004
	DWORD m_dwFlags;								// 0004 - 0008
	DWORD m_dwState;								// 0008 - 000C
	PAD(C_AICommand, pad0, 0x20);					// 000C - 002C
};

class C_AICommandShoot : public C_BaseAICommand
{
public:
	CVector3 m_vecTarget;							// 002C - 0038
};

class C_AICommandAim : public C_BaseAICommand
{
public:
	C_Vector3 * m_vecTarget;						// 002C - 0038
};

class C_AICommandMove : public C_BaseAICommand
{
public:
	CVector3 m_vecTarget;							// 002C - 0038
	PAD(C_AICommandMove, pad0, 0xB0);				// 0038 - 00E8
	CVector3 m_vecDirectionTarget;					// 00E8 - 00F4
	PAD(C_AICommandMove, pad1, 0x4);				// 00F4 - 00F8
	BYTE m_byteTargetStyle;							// 00F8 - 00F9
};

class C_SyncObjectUserData // size: 0xC
{
public:
	void* m_pVFTable;								// 0004 - 0008
	C_BaseAICommand * m_pAICommand;					// 0004 - 0008
	PAD(C_SyncObjectUserData, pad0, 0x4);			// 0008 - 000C
};

class C_SyncObjectVFTable
{
public:
	DWORD ScalarDeletingDestructor;					// 0000 - 0004
};

class C_SyncObject
{
public:
	C_SyncObjectVFTable* m_pVFTable;				// 0000 - 0004
	DWORD m_dwState;								// 0004 - 0008
	DWORD m_dwFlags;								// 0008 - 000C
	PAD(C_SyncObject, pad0, 0x8);					// 000C - 0014
	C_SyncObjectUserData* m_pUserData;				// 0014 - 0018
};

class CM2SyncObject
{

private:

	C_SyncObject * m_pSyncObject;
	bool						m_bDoneOnDelete;

public:

								CM2SyncObject						( C_SyncObject * pSyncObject );
								~CM2SyncObject						( void );

	void						SetSyncObject						( C_SyncObject * pSyncObject ) { m_pSyncObject = pSyncObject; }
	C_SyncObject				* GetSyncObject						( void ) { return m_pSyncObject; }

	void						Terminate							( void );
	bool						IsTerminated						( void );

	bool						IsSuccess							( void );
	bool						IsFail								( void );
	bool						IsDone								( void );
	void						Reactivate							( void );

	void						SetTarget							( CVector3 vecTarget, int type );
	void						GetTarget							( CVector3 * vecTarget, int type );

	void						SetMoveTargetData					( CVector3 vecEndDirection, BYTE bMoveType );

	void						SetDoneOnDelete						( bool b ) { m_bDoneOnDelete = b; }

};