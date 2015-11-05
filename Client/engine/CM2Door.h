/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.h
* Developers : h0pk1nz <haha_fuck_u@hotmail.com>
*
***************************************************************/

#pragma once

class CM2Door
{
public:
	bool IsClosed( void )
	{
		return ( *( DWORD *)( ( DWORD )( this ) + 0x320 ) == 1 );
	}

	bool IsLocked( void )
	{
		return ( ( *( DWORD *)( ( DWORD )( this ) + 0x2F4 ) == 0 ) == 0 );
	}
	
	M2Model * GetModel( void )
	{
		return *( M2Model **)( ( DWORD )( this ) + 0x60 );
	}

	void		Open			( CVector3 * pvPosition );
	void		Kick			( CVector3 * pvPosition );
	void		Close			( void );

	void		Lock			( void );
	void		Unlock			( void );

	void		EnableAction	( void );
	void		DisableAction	( void );

	void		RealLock		( void );
	void		RealUnlock		( void );

	__int64		GetHash			( void );
};