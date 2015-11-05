/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2SlotManager.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

class CM2Slot
{
public:
	char * GetName( void )
	{
		return ( char *)( ( DWORD )( this ) + 0x18 );
	}

	char * GetSDS( void )
	{
		return ( char *)( ( DWORD )( this ) + 0x74 );
	}

	void * GetUnk( void )
	{
		return ( void *)( ( DWORD )( this ) + 0x184 );
	}

	int GetNum( void )
	{
		return *( int *)( ( DWORD )( this ) + 0x1A8 );
	}

	int GetType( void )
	{
		return *( int *)( ( DWORD )( this ) + 0x40 );
	}

	int GetRefCount( void ) // in use?
	{
		return *( int *)( ( DWORD )( this ) + 0x54 );
	}

	void * GetLinkedSDSLoadingObj( void )
	{
		DWORD dwArray = *( DWORD *)( ( DWORD )( this ) + 0xC );

		return ( void *)( *( DWORD *)( dwArray ) );
	}

	M2Model * GetModelByName( const char * pszModelName )
	{
		DWORD dwCall = 0x00588FD0;

		__asm
		{
			push pszModelName
			mov ecx, this
			call dwCall
		}
	}
};

class CM2SlotDesc // size: 0x10
{
public:
	WORD wNameOffset;
	WORD wMaxSlots;

	WORD wSlotType;
	WORD wUnk2;

	WORD w1;
	WORD w2;
	WORD w3;
	WORD w4;

	const char * GetName( const char * pszNames )
	{
		return ( const char *)( ( DWORD )( pszNames ) + wNameOffset ); 
	}
};

class CM2SlotManager
{
public:
	const char *			GetSlotsNames		( void );

	CM2Slot *				GetSlot				( int nIndex );

	int						GetSlotDescSize		( void );
	CM2SlotDesc *			GetSlotDesc			( int nIndex );

	static CM2SlotManager * GetInstance			( void );
};