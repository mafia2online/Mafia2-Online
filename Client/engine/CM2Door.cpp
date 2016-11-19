/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.cpp
* Developers : h0pk1nz <123@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2BaseEntity.h"
#include "CM2Entity.h"

#include "COffsets.h"

#include "CLogFile.h"

#include "CM2Door.h"

M2Model * CM2Door::GetModel(void)
{
	return *(M2Model **)((DWORD)(this) + 0x60);
}

const bool CM2Door::IsClosed(void) const
{
	return (*(DWORD *)((DWORD)(this) + 0x320) == 1);
}

const bool CM2Door::IsLocked(void) const
{
	return ((*(DWORD *)((DWORD)(this) + 0x2F4) == 0) == 0);
}

void _declspec(naked) M2Door::Open(CVector3 *position, bool sound, int unk)
{
	_asm jmp COffsets::FUNC_CDoor__Open;
}

void _declspec(naked) M2Door::Close()
{
	_asm jmp COffsets::FUNC_CDoor__Close;
}

void _declspec(naked) M2Door::Lock()
{
	_asm jmp COffsets::FUNC_CDoor__Lock;
}

void _declspec(naked) M2Door::Unlock()
{
	_asm jmp COffsets::FUNC_CDoor__Unlock;
}

void _declspec(naked) M2Door::EnableAction()
{
	_asm jmp COffsets::FUNC_CDoor__EnableAction;
}

void _declspec(naked) M2Door::DisableAction()
{
	_asm jmp COffsets::FUNC_CDoor__DisableAction;
}

void _declspec(naked) M2Door::Kick(const CVector3 *vec)
{
	_asm jmp COffsets::FUNC_CDoor__Kick;
}

CM2Door::CM2Door(M2Door *door) : CM2Entity(door)
{
	SetDoor(door);

	if (!m_pDoor)
		return;
}

CM2Door::~CM2Door()
{
}

void CM2Door::Open(CVector3 * pvPosition, bool sound)
{
	if (!m_pDoor)
		return;

	m_pDoor->Open(pvPosition, sound, 0);
}

void CM2Door::Kick( CVector3 * pvPosition )
{
	if (!m_pDoor)
		return;

	m_pDoor->Kick(pvPosition);
}

void CM2Door::Close(void)
{
	if (!m_pDoor)
		return;

	m_pDoor->Close();
}

void CM2Door::Lock( void )
{
	if (!m_pDoor)
		return;

	m_pDoor->Lock();
}

void CM2Door::Unlock( void )
{
	if (!m_pDoor)
		return;

	m_pDoor->Unlock();
}

void CM2Door::EnableAction( void )
{
	if (!m_pDoor)
		return;

	m_pDoor->EnableAction();
}

void CM2Door::DisableAction( void )
{
	if (!m_pDoor)
		return;

	m_pDoor->DisableAction();
}

void CM2Door::RealLock( void )
{
	if (!IsClosed())
		this->Close();

	if (!IsLocked())
		this->Lock();

	DisableAction();
}

void CM2Door::RealUnlock( void )
{
	if (IsLocked())
		this->Unlock();

	EnableAction();
}

__int64 CM2Door::GetHash( void )
{
	CVector3 vDoorPos;
	vDoorPos.fX = *( float *)( *( DWORD *)( ( DWORD )( this ) + 0x60 ) + 0x44 );
	vDoorPos.fY = *( float *)( *( DWORD *)( ( DWORD )( this ) + 0x60 ) + 0x54 );
	vDoorPos.fZ = *( float *)( *( DWORD *)( ( DWORD )( this ) + 0x60 ) + 0x64 );

	char szDoorPos[512] = { 0 };
	sprintf( szDoorPos, "DOOR_%0.0f_%0.0f_%0.0f", vDoorPos.fX, vDoorPos.fY, vDoorPos.fZ );

	unsigned int uiHash = 0;

	__asm
	{
		push 0CBF29CE4h;						//.text:005AB1CB                 push    0CBF29CE4h
		push 84222325h;						//.text:005AB1D0                 push    84222325h
		push szDoorPos;
		call COffsets::FUNC_fnv64;
		mov uiHash, eax;
		add esp, 0Ch;
	}

	return (uiHash);
}
