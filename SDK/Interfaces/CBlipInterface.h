/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CBlipInterface.h
*
***************************************************************/

#pragma once

class CBlipInterface
{
public:
	virtual float							GetX( void ) = 0;
	virtual float							GetY( void ) = 0;
	virtual int								GetLibrary( void ) = 0;
	virtual int								GetIcon( void ) = 0;
};