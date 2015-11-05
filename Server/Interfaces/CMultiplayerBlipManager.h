/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerBlipManager.h
*
***************************************************************/

#pragma once

class CMultiplayerBlipManager
{

public:

	virtual	unsigned int					Add								( float fX, float fY, int iLibrary, int iIcon ) = 0;
	virtual	bool							Remove							( unsigned int uiBlipId ) = 0;
	virtual	bool							IsActive						( unsigned int uiBlipId ) = 0;

	virtual CMultiplayerBlip				* Get							( unsigned int uiBlipId ) = 0;
	virtual	unsigned int					GetCount						( void ) = 0;

};