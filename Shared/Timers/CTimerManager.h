/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CTimerManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CTimer.h"
#include	<list>

class CTimerManager : public std::list< CTimer *>
{

public:

	void			Pulse( void );
	void			HandleScriptUnload( CSquirrel * pScript );
	bool			Contains( CTimer * pTimer );

};