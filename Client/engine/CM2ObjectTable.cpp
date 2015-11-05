/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.cpp
* Developers : h0pk1nz <123@hotmail.com>
*
***************************************************************/

#include "../StdInc.h"

int CM2ObjectTable::GetSize( void )
{
	return *( int *)( ( DWORD )( this ) + 0xC );
}

int CM2ObjectTable::GetRegisteredSize( void )
{
	return *( int *)( ( DWORD )( this ) + 0x13968 );
}

CM2Object * CM2ObjectTable::GetObject( int nIndex )
{
	return *( CM2Object **)( ( ( DWORD )( this ) + 0x20 ) + ( 4 * nIndex ) );
}

CM2Object * CM2ObjectTable::GetRegisteredObject( int nIndex )
{
	return *( CM2Object **)( ( ( DWORD )( this ) + 0x1397C ) + ( 4 * nIndex ) );
}

CM2ObjectTable * CM2ObjectTable::GetInstance( void )
{
	return *( CM2ObjectTable **)( COffsets::VAR_CObjectTable );
}