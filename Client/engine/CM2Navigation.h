/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Navigation.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class M2Navigation
{
public:
	PAD(M2Navigation, pad0, 0x4);
};

class CM2Navigation
{
	
private:

	M2Navigation * m_pNavigation;

public:

							CM2Navigation					( M2Navigation * pNavigation );
							~CM2Navigation					( void );

	void					SetNavigation					( M2Navigation * pNavigation ) { m_pNavigation = pNavigation; }
	M2Navigation			* GetNavigation					( void ) { return m_pNavigation; }

	int						RegisterIconPos					( Vector2 vecPosition, int iLibrary, int iIcon );
	int						RegisterIconEntity				( M2Entity * pEntity, int iLibrary, int iIcon );

	void					UnregisterIconPos				( int iconId );
	void					UnregisterIconEntity			( M2Entity * pEntity );

};