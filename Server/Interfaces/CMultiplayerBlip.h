/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerBlip.h
*
***************************************************************/

#pragma once

class CMultiplayerBlip
{

public:

	virtual	unsigned int					GetId							( void ) = 0;
	virtual	int								GetLibrary						( void ) = 0;
	virtual	int								GetIcon							( void ) = 0;

};