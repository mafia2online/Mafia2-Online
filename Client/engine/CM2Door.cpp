/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.cpp
* Developers : h0pk1nz <123@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2Door.h"

#include "COffsets.h"

#include "CLogFile.h"

M2Model * CM2Door::GetModel(void)
{
	return *(M2Model **)((DWORD)(this) + 0x60);
}

bool CM2Door::IsClosed(void)
{
	return (*(DWORD *)((DWORD)(this) + 0x320) == 1);
}

bool CM2Door::IsLocked(void)
{
	return ((*(DWORD *)((DWORD)(this) + 0x2F4) == 0) == 0);
}

void CM2Door::Open( CVector3 * pvPosition )
{
	__asm
	{
		push 0
		push 1
		push pvPosition
		mov ecx, this
		call COffsets::FUNC_CDoor__Open
	}
}

void CM2Door::Kick( CVector3 * pvPosition )
{
	__asm
	{
		push pvPosition
		mov ecx, this
		call COffsets::FUNC_CDoor__Kick
	}
}

void CM2Door::Close( void )
{
	__asm
	{
		mov ecx, this
		call COffsets::FUNC_CDoor__Close
	}
}

void CM2Door::Lock( void )
{
	__asm
	{
		mov ecx, this
		call COffsets::FUNC_CDoor__Lock
	}
}

void CM2Door::Unlock( void )
{
	__asm
	{
		mov ecx, this
		call COffsets::FUNC_CDoor__Unlock
	}
}

void CM2Door::EnableAction( void )
{
	__asm
	{
		mov ecx, this
		call COffsets::FUNC_CDoor__EnableAction
	}
}

void CM2Door::DisableAction( void )
{
	__asm
	{
		mov ecx, this
		call COffsets::FUNC_CDoor__DisableAction
	}
}

void CM2Door::RealLock( void )
{
	if ( !IsClosed( ) )
		this->Close( );

	if ( !IsLocked( ) )
		this->Lock( );

	DisableAction( );
}

void CM2Door::RealUnlock( void )
{
	if ( IsLocked( ) )
		this->Unlock( );

	EnableAction( );
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
