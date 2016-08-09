/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CQuery.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CQuery
{

private:

	CThread													m_workerThread;
	int														m_iSocket;

	static	void				WorkerThread				( CThread * pCreator );

public:

								CQuery						( unsigned short usPort );
								~CQuery						( void );

	std::string					QueryLight					( void );

	int							GetSocket					( void ) { return m_iSocket; }

};