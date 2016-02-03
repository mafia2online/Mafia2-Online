/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CStreamer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <list>

#include "ExplicitSingleton.h"

class CStreamableEntity;

static const unsigned STREAMER_TICKRATE = 3000;

class CStreamer : public std::list< CStreamableEntity* >, public ExplicitSingleton<CStreamer>
{

private:

	unsigned long		m_ulLastStreamTime;

public:

						CStreamer				( void );
						~CStreamer				( void );

	void				Process					( void );

};