/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerQuery.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <Threading/CThread.h>

class CThread;
class CServerListItem;
class CServerQuery
{

private:

	int													m_iSocket;
	std::list< CServerListItem* >						m_queryQueue;
	CThread												m_workerThread;

	static	void			WorkerThread				( CThread * pCreator );
	void					Process						( void );

public:

							CServerQuery				( void );
							~CServerQuery				( void );

	bool					Query						( CServerListItem * pServerItem );
	void					Remove						( CServerListItem * pServerItem );
	void					Reset						( void );

	int						GetSocket					( void ) { return m_iSocket; }
	std::list< CServerListItem* > GetQueue				( void ) { return m_queryQueue; }

	CServerListItem			* GetQueryItem				( const char * szHost, unsigned short usPort );

};