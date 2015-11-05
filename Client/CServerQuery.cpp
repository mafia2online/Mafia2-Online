/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerQuery.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	<Threading/CThread.h>

extern	CCore			* pCore;

const char * GetIPFromSocketAddress(int af, const void * src, char * dst, int cnt) // TODO: Move this function to the shared library! It's used in Server/CQuery.cpp also!
{
	if(af == AF_INET || af == PF_INET)
	{
		sockaddr_in in;
		memset(&in, 0, sizeof(in));
		in.sin_family = af;
		memcpy(&in.sin_addr, src, sizeof(in_addr));
		getnameinfo((sockaddr *)&in, sizeof(sockaddr_in), dst, cnt, NULL, 0, NI_NUMERICHOST);
		return dst;
	}
	else if(af == AF_INET6 || af == PF_INET6)
	{
		struct sockaddr_in6 in;
		memset(&in, 0, sizeof(in));
		in.sin6_family = af;
		memcpy(&in.sin6_addr, src, sizeof(in.sin6_addr));
		getnameinfo((sockaddr *)&in, sizeof(sockaddr_in6), dst, cnt, NULL, 0, NI_NUMERICHOST);
		return dst;
	}

	return NULL;
}

void CServerQuery::WorkerThread ( CThread * pCreator )
{
	// Loop until we end the thread
	while ( pCreator->GetUserData < bool > () )
	{
		// Get the server query pointer
		CServerQuery * pServerQuery = pCore->GetGUI()->GetServerBrowser()->GetServerQuery ();

		// Is the server query pointer valid?
		if ( pServerQuery )
		{
			// Remove any stale queries
			for ( std::list< CServerListItem* >::iterator iter = pServerQuery->GetQueue().begin (); iter != pServerQuery->GetQueue().end (); )
			{
				// Get the current query
				CServerListItem * pServerQueryItem = *iter;

				// Has the query timed out?
				if ( (SharedUtility::GetTime () - pServerQueryItem->ulQueryStart) > 10000 )
				{
					CLogFile::Printf ( "Deleting stale query." );

					// Erase the query item from the list
					delete pServerQueryItem;
					pServerQuery->Remove ( pServerQueryItem );
				}
				else
					iter++;
			}

			// Prepare the socket address
			sockaddr_in address;
			memset ( &address, 0, sizeof ( sockaddr_in ) );
			int iFromLen = sizeof ( sockaddr_in );

			// Create the temp buffer
			static char szBuffer [ 2048 ];

			// Loop until everything is read from the socket buffer
			int iBytesRead = -1;
			while ( (iBytesRead = recvfrom ( pServerQuery->GetSocket (), szBuffer, sizeof ( szBuffer ), NULL, (sockaddr *)&address, &iFromLen )) != -1 )
			{
				// Is this query for m2mp?
				if ( szBuffer [ 0 ] == 'M' && szBuffer [ 1 ] == '2' && szBuffer [ 2 ] == 'M' && szBuffer [ 3 ] == 'P' )
				{
					// Convert the socket address into an IP
					char szIpAddress [ 64 ];
					GetIPFromSocketAddress ( address.sin_family, &address.sin_addr, szIpAddress, sizeof ( szIpAddress ) );

					// Get the socket address port
					unsigned short usPort = ntohs ( address.sin_port );

					// Get the server query item pointer
					CServerListItem * pServerItem = pServerQuery->GetQueryItem ( szIpAddress, (usPort - 1) );

					// Is the query item invalid?
					if ( !pServerItem )
						continue;

					// Parse the server info
					pServerItem->Parse ( szBuffer, strlen ( szBuffer ) );

					// Remove the item from the query queue
					pServerQuery->Remove ( pServerItem );
				}
			}
		}

		// Sleep
		Sleep ( 50 );
	}
}

CServerQuery::CServerQuery ( void )
{
	// Startup winsock
	WSADATA wsaData;
	WSAStartup ( MAKEWORD( 2, 2 ), &wsaData );

	// Prepare the socket
	m_iSocket = socket ( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	// Set the socket non blocking
	unsigned long sockopt = 1;
	ioctlsocket ( m_iSocket, FIONBIO, &sockopt );
}

CServerQuery::~CServerQuery ( void )
{
	// Reset all queries
	Reset ();

	// Close the socket if it's valid
	if ( m_iSocket != -1 )
		closesocket ( m_iSocket );

	// Cleanup winsock
	WSACleanup ();
}

bool CServerQuery::Query ( CServerListItem * pServerItem )
{
	// Prepare the address
	sockaddr_in address;
	memset ( &address, 0, sizeof ( sockaddr_in ) );
	address.sin_family = AF_INET;
	address.sin_port = htons ( pServerItem->usGamePort + 1 );
	address.sin_addr.s_addr = inet_addr ( pServerItem->strHost.Get () );

#ifdef _DEBUG
	CLogFile::Printf ( "Sending query to %s:%d...", pServerItem->strHost.Get(), pServerItem->usGamePort );
#endif

	// Send the query to the server
	if ( sendto ( m_iSocket, "M2MP", 4, NULL, (sockaddr *)&address, sizeof ( sockaddr_in ) ) == 4 )
	{
		// Set the query start time
		pServerItem->ulQueryStart = SharedUtility::GetTime ();

		// Add the query item to the queue
		m_queryQueue.push_back ( pServerItem );

		CLogFile::Printf ( "CServerQuery::Query () - Queue size: %d", m_queryQueue.size () );

		// Is the worker thread inactive?
		if ( !m_workerThread.IsRunning () )
		{
			// Create the worker thread
			m_workerThread.SetUserData< bool > ( true );
			m_workerThread.Start ( WorkerThread );
		}

		return true;
	}

	return false;
}

void CServerQuery::Remove ( CServerListItem * pServerItem )
{
	// Loop over the query queue
	for ( std::list< CServerListItem* >::iterator iter = m_queryQueue.begin (); iter != m_queryQueue.end (); iter ++ )
	{
		// Is this the item we're looking for?
		if ( pServerItem == *iter )
		{
			// Remove the item
			m_queryQueue.erase ( iter );
			break;
		}
	}

	CLogFile::Printf ( "CServerQuery::Remove () - Queue size: %d", m_queryQueue.size () );

	// Are we out of queue items?
	if ( !m_queryQueue.size () )
	{
		// Reset
		Reset ();
	}
}

void CServerQuery::Reset ( void )
{
	// Clear the query queue
	m_queryQueue.clear ();

	// Is the worker thread active?
	if ( m_workerThread.IsRunning () )
	{
		// Terminate the worker thread
		m_workerThread.SetUserData< bool > ( false );
		m_workerThread.Stop ( false, true );
	}
}

CServerListItem * CServerQuery::GetQueryItem ( const char * szHost, unsigned short usPort )
{
	// Loop over the query queue
	for ( std::list< CServerListItem* >::iterator iter = m_queryQueue.begin (); iter != m_queryQueue.end (); iter ++ )
	{
		// Is this the query we're looking for?
		if ( !(*iter)->strHost.Compare ( szHost ) && (*iter)->usGamePort == usPort )
			return *iter;
	}

	return NULL;
}

void CServerQuery::Process ( void )
{
	// Do we not have any queries?
	if ( !m_queryQueue.size () )
		return;

	/*
	// Remove any stale queries
	for ( std::list< CServerListItem* >::iterator iter = m_queryQueue.begin (); iter != m_queryQueue.end (); iter ++ )
	{
		// Has the query timed out?
		if ( (SharedUtility::GetTime () - (*iter)->ulQueryStart) > 10000 )
		{
			// Erase the query item from the list
			iter = m_queryQueue.erase ( iter );
		}
		else
			iter ++;
	}
	*/
}