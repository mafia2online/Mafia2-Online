/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkRPC.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkRPC
{

private:

	static	bool		m_bRegistered;

public:

	static	void		Register( RakNet::RPC4 * pRPC );
	static	void		Unregister( RakNet::RPC4 * pRPC );

};