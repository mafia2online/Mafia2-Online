/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerRPC.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<RPC4Plugin.h>

class CPlayerRPC
{

private:

	static	bool							m_bRegistered;

public:

	static	void		Register			( RakNet::RPC4 * pRPC );
	static	void		Unregister			( RakNet::RPC4 * pRPC );

};