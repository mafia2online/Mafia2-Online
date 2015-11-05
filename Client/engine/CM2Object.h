#pragma once

class CM2Object // parent of C_Entity, C_Car, C_Door, C_..., etc.
{
public:
	BYTE GetType( void )
	{
		return *( BYTE *)( ( DWORD )( this ) + 0x1C );
	}

	DWORD GetFlags( void )
	{
		return *( DWORD *)( ( DWORD )( this ) + 0x20 );
	}

	__int64 GetGUID( void )
	{
		return *( __int64 *)( ( DWORD )( this ) + 0x28 );
	}
};