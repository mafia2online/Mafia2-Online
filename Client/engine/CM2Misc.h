/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Misc.h
* Developers : h0pk1nz <>
*
***************************************************************/

#pragma once

class C_TrafficModelUnkData
{
public:
	const char * GetModelFilePath( void )
	{
		return *( const char **)( ( DWORD )( this ) + 0x10 );
	}
};

class C_TrafficCarModelUnkData
{
public:
	const char * GetModelFilePath( void )
	{
		return ( const char *)( *( DWORD *)( ( DWORD )( this ) + 0x10 ) + 0x1D );
	}
};

class C_TrafficModelDescriptor
{
public:
	unsigned char __PAD0001[0x1C];

	int GetType( void )
	{
		return *( int *)( ( DWORD )( this ) + 0x4 );
	}

	M2Model * GetModel( void )
	{
		return *( M2Model **)( ( DWORD )( this ) + 0xC );
	}

	C_TrafficModelUnkData * GetUnkData( void )
	{
		return *( C_TrafficModelUnkData **)( ( DWORD )( this ) + 0x10 );
	}
};

class C_TrafficCarModelDescriptor
{
public:
	unsigned char __PAD0001[0x1C];

	int GetType( void )
	{
		return *( int *)( ( DWORD )( this ) + 0x4 );
	}

	M2Model * GetModel( void )
	{
		return *( M2Model **)( ( DWORD )( this ) + 0xC );
	}

	C_TrafficCarModelUnkData * GetUnkData( void )
	{
		return *( C_TrafficCarModelUnkData **)( ( DWORD )( this ) + 0x14 );
	}
};


class C_TrafficCacheObject
{
public:
	unsigned char GetActorType( void )
	{
		return *( unsigned char *)( ( DWORD )( this ) + 0xC );
	}

	int GetUnkNum( void )
	{
		return *( int *)( ( DWORD )( this ) + 0x4 );
	}

	int GetIndex( void ) /* /tables/Players.tbl */
	{
		return *( int *)( ( DWORD )( this ) + 0x18 );
	}

	int GetFlags( void ) /* /tables/Players.tbl */
	{
		return *( int *)( ( DWORD )( this ) + 0x18 );
	}

	C_TrafficModelDescriptor * GetModelDescriptor( void )
	{
		return *( C_TrafficModelDescriptor **)( ( DWORD )( this ) + 0x24 );
	}
};

class C_TrafficCacheCharacter : public C_TrafficCacheObject
{
public:
	void * GetAssignedEntities( void )
	{
		return ( void *)( ( DWORD )( this ) + 0x28 );
	}

	int GetAssignedEntitySize( void )
	{
		DWORD dwFirstObject = *( DWORD *)( ( DWORD )( this ) + 0x2C );
		DWORD dwLastObject = *( DWORD *)( ( DWORD )( this ) + 0x30 );

		return ( ( dwLastObject - dwFirstObject ) / 8 );
	}

	C_Entity * GetAssignedEntity( int nIndex )
	{
		DWORD dwFirstObject = *( DWORD *)( ( DWORD )( this ) + 0x2C );

		int nMaxIndex = GetAssignedEntitySize( );

		if ( nIndex > nMaxIndex )
			return NULL;

		return *( C_Entity **)( dwFirstObject + ( nIndex * 8 ) );
	}
};

class C_TrafficActorCache
{
public:
	int GetActorCacheSize( void )
	{
		DWORD dwFirstObject = *( DWORD *)( ( DWORD )( this ) + 0x8 );
		DWORD dwLastObject = *( DWORD *)( ( DWORD )( this ) + 0xC );

		return ( ( dwLastObject - dwFirstObject ) / 4 );
	}

	C_TrafficCacheObject * GetActorCache( int nIndex )
	{
		DWORD dwFirstObject = *( DWORD *)( ( DWORD )( this ) + 0x8 );

		int nMaxIndex = GetActorCacheSize( );

		if ( nIndex > nMaxIndex )
			return NULL;

		return *( C_TrafficCacheObject **)( dwFirstObject + ( nIndex * 4 ) );
	}
};

C_TrafficActorCache * GetTrafficActorCache( void );
C_TrafficModelDescriptor * LoadHumanModel( char * szModelFileName );
C_TrafficCacheCharacter * GetCacheObjectByModelName( const char * pszInputModelName );
C_TrafficCacheObject * GetCacheCarObjectByModelName( const char * pszInputModelName );