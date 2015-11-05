/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CWinSock.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

typedef void ( * pfnQueryHandler )( String strHost, unsigned long ulPort, char cCommand, String strData );

class CWinSock
{

private:

	int						m_iSocket;
	sockaddr_in				m_socketAddr;
	bool					m_bSocketInitialised;
	pfnQueryHandler			m_pfnQueryHandler;

#ifdef _WIN32
	HANDLE					m_hThread;
#else
	pthread_t				m_thread;
#endif

public:

	CWinSock( );
	~CWinSock( );

	void					Query( const char * strIp, unsigned short usPort, char cCommand );
	void					Pulse( );

#ifdef _WIN32
	static	void			ListenThread( void * pThis );
#else
	static	void			* ListenThread( void * pThis );
#endif

	void					SetQueryHandler( pfnQueryHandler handler ) { m_pfnQueryHandler = handler; }

};