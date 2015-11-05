/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CPlayerManagerInterface.h
*
***************************************************************/

#pragma once

#include "CNetworkPlayerInterface.h"

class CPlayerManagerInterface
{
public:
	virtual	bool							IsActive( EntityId playerId ) = 0;
	virtual CNetworkPlayerInterface			* Get( EntityId playerId ) = 0;
	virtual EntityId						GetCount( void ) = 0;
	virtual bool							IsNickInUse( const char * szNick ) = 0;
	virtual CNetworkPlayerInterface			* GetPlayerFromSerial( const char * szSerial ) = 0;
};