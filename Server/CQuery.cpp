/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CQuery.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

const char * GetIPFromSocketAddress(int af, const void * src, char * dst, int cnt)
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

void CQuery::WorkerThread ( CThread * pCreator )
{
	// Loop while the thread is active
	while ( pCreator->GetUserData< bool > () )
	{
		// Get the query instance
		CQuery * pQuery = pCore->GetQuery ();

		// Is the query instance valid?
		if ( pQuery )
		{
			// Get the socket interface
			int iSocket = pQuery->GetSocket ();

			// Reset buffer
			static char szBuffer [ 1024 ];
			memset ( szBuffer, 0, sizeof( szBuffer ) );

			// Create the address
			sockaddr_in address;
			memset ( &address, 0, sizeof( sockaddr_in ) );
			int iFromLen = sizeof( sockaddr_in );

			// Did we read any data from winsock?
			int iBytesRead = -1;
			while ( (iBytesRead = recvfrom ( iSocket, szBuffer, sizeof ( szBuffer ), NULL, (sockaddr *)&address, (socklen_t *)&iFromLen )) != -1 )
			{
				// Convert the IP address to a string
				char szIpAddress [ 64 ];
				GetIPFromSocketAddress ( address.sin_family, &address.sin_addr, szIpAddress, sizeof ( szIpAddress ) );

				// Get the port
				unsigned short usPort = ntohs ( address.sin_port );

				// Is this packet for us?
				if ( szBuffer[0] == 'M' && szBuffer[1] == '2' && szBuffer[2] == 'M' && szBuffer[3] == 'P' )
				{
					// Generate a reply string
					std::string reply = pQuery->QueryLight ();

					// Send the packet back to the client
					sendto ( iSocket, reply.c_str (), reply.length (), NULL, (sockaddr *)&address, sizeof ( sockaddr_in ) );
				}
			}
		}

		// Sleep
		Sleep ( 50 );
	}
}

CQuery::CQuery( unsigned short usPort )
{
#ifdef WIN32
	// Startup winsock
	WSADATA wsaData;
	WSAStartup ( MAKEWORD( 2, 2 ), &wsaData );
#endif

	// Create a socket address
	sockaddr_in address;
	memset ( &address, 0, sizeof(sockaddr_in) );
	address.sin_family = AF_INET;
	address.sin_port = htons ( usPort );

	// Do we have a host set?
	if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
		address.sin_addr.s_addr = inet_addr ( CVAR_GET_STRING ( "serverip" ) );

	// Create the socket
	m_iSocket = socket ( AF_INET, SOCK_DGRAM, 0 );

	// Set the socket non blocking
#ifdef WIN32
	unsigned long sockopt = 1;
	ioctlsocket ( m_iSocket, FIONBIO, &sockopt );
#else
	fcntl ( m_iSocket, F_SETFL, fcntl ( m_iSocket, F_GETFL ) | O_NONBLOCK );
#endif

	// Bind the socket to the address
	if ( bind ( m_iSocket, (sockaddr *)&address, sizeof ( sockaddr_in ) ) == -1 )
		CLogFile::Printf ( "Failed to bind query socket on port %d!", usPort );

	// Start the worker thread
	m_workerThread.SetUserData< bool > ( true );
	m_workerThread.Start ( WorkerThread );
}

CQuery::~CQuery( void )
{
	// Stop the worker thread
	m_workerThread.SetUserData< bool > ( false );
	m_workerThread.Stop ( false, true );

	// Close the socket
	if( m_iSocket != -1 )
		closesocket ( m_iSocket );

#ifdef WIN32
	// Cleanup winsock
	WSACleanup ();
#endif
}

std::string CQuery::QueryLight( void )
{
	//
	std::stringstream reply;
	reply << "M2MP";

	// hostname
	reply << ( unsigned char )( strlen( CVAR_GET_STRING( "hostname" ) ) + 1 );
	reply << CVAR_GET_STRING( "hostname" );

	// players
	String strPlayers( "%d", pCore->GetPlayerManager()->GetCount() );
	reply << ( unsigned char )( strPlayers.GetLength() + 1 );
	reply << strPlayers.Get();

	// players
	String strMaxPlayers( "%d", CVAR_GET_INTEGER( "maxplayers" ) );
	reply << ( unsigned char )( strMaxPlayers.GetLength() + 1 );
	reply << strMaxPlayers.Get();

	// gamemode
	reply << ( unsigned char )( strlen( pCore->GetGameModeText() ) + 1 );
	reply << pCore->GetGameModeText();

	// password
	reply << ( unsigned char )( pCore->IsPasswordProtected() ? 1 : 0 );

	//
	CNetworkPlayer * pPlayer = NULL;
	char szName[256] = { '\0' };
	char szPing[256] = { '\0' };

	// players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Get a pointer to the current player
			pPlayer = pCore->GetPlayerManager()->Get( i );

			// (TODO: Fix this ugly ifdef)
#ifdef _WIN32
			_snprintf ( szName, 255, "%s", pPlayer->GetNick () );
			_snprintf ( szPing, 255, "%u", pPlayer->GetPing () );
#else
			snprintf ( szTemp, 255, "%s", pPlayer->GetNick () );
			snprintf ( szPing, 255, "%u", pPlayer->GetPing () );
#endif

			// Write the player name 
			reply << ( unsigned char )( strlen( szName ) + 1 );
			reply << szName;

			// Write the player ping
			reply << ( unsigned char )( strlen( szPing ) + 1 );
			reply << szPing;
		}
	}

	return reply.str();
}
