/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScriptingRPC.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CScriptingRPC
{

private:

	static	bool							m_bRegistered;

public:

	static	void		Register			( RakNet::RPC4 * pRPC );
	static	void		Unregister			( RakNet::RPC4 * pRPC );

};