/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2SlotManager.cpp
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "../StdInc.h"

const char * CM2SlotManager::GetSlotsNames( void )
{
	return *( const char **)( ( DWORD )( this ) + 0x4 );
}

CM2Slot * CM2SlotManager::GetSlot( int nIndex )
{
	return *( CM2Slot **)( *( PDWORD )( ( DWORD )( this ) + 0x20 ) + ( 4 * nIndex ) );
}

int CM2SlotManager::GetSlotDescSize( void )
{
	DWORD dwFirstObject = *( DWORD *)( *( DWORD *)( ( DWORD )( this ) + 0x18 ) + 0x8 );
	DWORD dwLastObject = *( DWORD *)( *( DWORD *)( ( DWORD )( this ) + 0x18 ) + 0xC );

	return ( ( dwLastObject - dwFirstObject ) / 0x10 );
}

CM2SlotDesc * CM2SlotManager::GetSlotDesc( int nIndex )
{
	DWORD dwFirstObject = *( DWORD *)( *( DWORD *)( ( DWORD )( this ) + 0x18 ) + 0x8 );

	int nMaxIndex = GetSlotDescSize( );

	if ( nIndex > nMaxIndex )
		return NULL;

	return ( CM2SlotDesc *)( dwFirstObject + ( nIndex * 0x10 ) );
}

CM2SlotManager * CM2SlotManager::GetInstance( void )
{
	return *( CM2SlotManager **)( COffsets::VAR_CSlotManager );
}