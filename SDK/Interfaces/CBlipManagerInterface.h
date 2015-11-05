/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CBlipManagerInterface.h
*
***************************************************************/

#pragma once

#include	"CBlipInterface.h"

class CBlipManagerInterface
{
public:
	virtual EntityId						Add( float fX, float fY, int iLibrary, int iIcon ) = 0;
	virtual bool							Remove( EntityId blipId ) = 0;
	virtual bool							IsActive( EntityId blipId ) = 0;
	virtual CBlipInterface					* Get( EntityId blipId ) = 0;
};