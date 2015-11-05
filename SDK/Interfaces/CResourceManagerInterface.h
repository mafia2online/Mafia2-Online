/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CResourceManagerInterface.h
*
***************************************************************/

#pragma once

class CResourceManagerInterface
{
public:
	virtual bool							StartResource( const char * szResource ) = 0;
	virtual bool							StopResource( const char * szResource ) = 0;
	virtual bool							IsResourceRunning( const char * szResource ) = 0;
};