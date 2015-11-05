/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CStreamer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CStreamableEntity.h"

#define	STREAMER_TICKRATE			3000

class CStreamer : public std::list< CStreamableEntity* >
{

private:

	unsigned long				m_ulLastStreamTime;

public:

								CStreamer								( void );
								~CStreamer								( void );

	void						Process									( void );				

};