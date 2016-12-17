/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Navigation.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "Math/CVector3.h"
#include "COffsets.h"

class M2Entity;
class Vector2;

class M2Navigation
{
public:
	PAD(M2Navigation, pad0, 0x4);
	
	void *GetIconFromEntity(M2Entity *pEntity);
	void *GetIconFromId(int iconId);

	int RegisterIconEntity(M2Entity *pEntity, int icon, int library, int unk1, int unk2, int unk3);
	int RegisterIconPos(const Vector2& vecPosition, int lib, int icon, int unk1, int unk2, int unk3);

	void UnregisterIconEntity(M2Entity * pEntity, int unk);
	void UnregisterIconPos(int icon, int unk);

	void SetForegroundColor(int iIconId, int iColor);
	void SetBackgroundColor(int iIconId, int iColor);
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

	int						RegisterIconPos					( const Vector2& vecPosition, int iLibrary, int iIcon );
	int						RegisterIconEntity				( M2Entity * pEntity, int iLibrary, int iIcon );

	void					UnregisterIconPos				( int iconId );
	void					UnregisterIconEntity			( M2Entity * pEntity );

	void					SetForegroundColor				(int iIconId, int iColor);
	void					SetBackgroundColor				(int iIconId, int iColor);

};