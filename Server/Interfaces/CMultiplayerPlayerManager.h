/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerPlayerManager.h
*
***************************************************************/

#pragma once

class CMultiplayerPlayerManager
{

public:

	virtual	void							Remove							( unsigned int uiPlayerId, unsigned int uiReason ) = 0;
	virtual	bool							IsActive						( unsigned int uiPlayerId ) = 0;

	virtual CMultiplayerPlayer				* Get							( unsigned int uiPlayerId ) = 0;
	virtual	unsigned int					GetCount						( void ) = 0;

	virtual	bool							IsNickInUse						( const char * szNick ) = 0;

};