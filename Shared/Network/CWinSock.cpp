/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CWinSock.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _WIN32
#include <winsock2.h>
#include <winsock.h>
typedef int socklen_t;
#else
#include "../Linux.h"
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#endif

#include "../CString.h"
#include "CWinSock.h"

CWinSock::CWinSock( )
{
	// Startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData );
#endif

	// Setup the socket
	m_iSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	// ioctl socket
#ifdef WIN32
	u_long ulArg = 1;
	ioctlsocket( m_iSocket, FIONBIO, &ulArg );
#else
	fcntl( m_iSocket, F_SETFL, fcntl(m_iSocket, F_GETFL) | O_NONBLOCK );
#endif

	// Setup the socket address
	m_socketAddr.sin_family = AF_INET;
	m_socketAddr.sin_addr.s_addr = INADDR_ANY;
	m_socketAddr.sin_port = htons(36213);

	// Bind the socket
	int iResult = bind( m_iSocket, (sockaddr *)&m_socketAddr, sizeof(m_socketAddr) );

	// Did the socket fail to bind?
	if( iResult != 0 )
	{
		// Mark as not intiialised
		m_bSocketInitialised = false;

		return;
	}

	// Create the thread to listen for results
#ifdef WIN32
	m_hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)ListenThread, (void *)this, 0, 0 );
#else
	pthread_create( &m_thread, 0, ListenThread, (void *)this );
#endif
}

CWinSock::~CWinSock( )
{
	// Do we have an active socket?
	if( m_iSocket != -1 )
	{
		// Close the socket
		closesocket( m_iSocket );
	}

	// Terminate the thread
#ifdef WIN32
	TerminateThread( m_hThread, 0 );
#else
	pthread_cancel( m_thread );
#endif

	// Cleanup winsock
#ifdef WIN32
	WSACleanup( );
#endif
}

void CWinSock::Query( const char * szIp, unsigned short usPort, char cCommand )
{
	// Construct the socket target address
	sockaddr_in target;
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = inet_addr( szIp );
	target.sin_port = htons( usPort + 1 );

	// Setup the query type
	char szOutput[5];
	szOutput[0] = '5';
	szOutput[1] = '-';
	szOutput[2] = 'M';
	szOutput[3] = 'P';
	szOutput[4] = cCommand;

	// Send the command over the socket
	int iResult = sendto( m_iSocket, (char *)szOutput, sizeof(szOutput), 0, (const sockaddr *)&target, sizeof(sockaddr_in) );

	// Did the query fail to send?
	if( iResult < 0 )
	{
		return;
	}
}

void CWinSock::Pulse( )
{
	static char szBuffer[2048];
	memset( szBuffer, 0, sizeof(szBuffer) );
	static int iLen = sizeof(m_socketAddr);

	// Receive from the socket
	int iResult = recvfrom( m_iSocket, szBuffer, sizeof(szBuffer), 0, (sockaddr *)&m_socketAddr, (socklen_t *)&iLen );

	// Do we have any data and should we process it ?
	if( iResult > 4 && szBuffer[0] == '5' && szBuffer[1] == '-' && szBuffer[2] == 'M' && szBuffer[3] == 'P' )
	{
		// Move the buffer into a new string
		String strBuffer( "%s", szBuffer );

		// Delete the query identifier from the string
		strBuffer.Erase( 0, 5 );

		// Get the server ip
		char * szIp = new char[256];
		const char * str = inet_ntoa( m_socketAddr.sin_addr );
		strcpy( szIp, str );

		// Get the server port
		unsigned long ulPort = (ntohs( m_socketAddr.sin_port ) - 1);

		// Send this to the query handler
		if( m_pfnQueryHandler )
			m_pfnQueryHandler( szIp, ulPort, szBuffer[4], strBuffer );
	}
}

#ifdef WIN32
void CWinSock::ListenThread( void * pThis )
#else
void * CWinSock::ListenThread( void * pThis )
#endif
{
	CWinSock * m_pThis = (CWinSock *)pThis;

	// Loop until the program closes
	while( true )
	{
		// Pulse winsock
		m_pThis->Pulse( );

		// Prevent high cpu usage
		Sleep( 100 );
	}

#ifndef WIN32
	return (void *)1;
#endif
}