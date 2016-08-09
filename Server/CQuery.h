/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CQuery.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<thread>

class CQuery
{

private:
	std::thread					m_thread;
	bool						m_processQueries;
	int														m_iSocket;

	void						WorkerThread				();

public:

								CQuery						( unsigned short usPort );
								~CQuery						( void );

	std::string					QueryLight					( void );

	int							GetSocket					( void ) { return m_iSocket; }

};