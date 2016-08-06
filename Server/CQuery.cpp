/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CQuery.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"SharedUtility.h"
#include	"CCore.h"

void CQuery::WorkerThread ( CThread * pCreator )
{
	while ( pCreator->GetUserData< bool > () )
	{
		CQuery * pQuery = CCore::Instance()->GetQuery ();

		if ( pQuery )
		{
			int iSocket = pQuery->GetSocket ();

			static char szBuffer [ 1024 ];
			memset ( szBuffer, 0, sizeof( szBuffer ) );

			sockaddr_in address;
			memset ( &address, 0, sizeof( sockaddr_in ) );
			int iFromLen = sizeof( sockaddr_in );

			int iBytesRead = -1;
			while ( (iBytesRead = recvfrom ( iSocket, szBuffer, sizeof ( szBuffer ), NULL, (sockaddr *)&address, (socklen_t *)&iFromLen )) != -1 )
			{
				char szIpAddress [ 64 ];
				SharedUtility::GetIPFromSocketAddress ( address.sin_family, &address.sin_addr, szIpAddress, sizeof ( szIpAddress ) );

				unsigned short usPort = ntohs ( address.sin_port );
				if (szBuffer[0] == 'M' && szBuffer[1] == '2' && szBuffer[2] == 'O' && szBuffer[3] == 'n' && szBuffer[4] == 'l' && szBuffer[5] == 'i' && szBuffer[6] == 'n' && szBuffer[7] == 'e')
				{
					std::string reply = pQuery->QueryLight ();
					sendto ( iSocket, reply.c_str (), reply.length (), NULL, (sockaddr *)&address, sizeof ( sockaddr_in ) );
				}
			}
		}
		Sleep ( 50 );
	}
}

CQuery::CQuery( unsigned short usPort )
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup ( MAKEWORD( 2, 2 ), &wsaData );
#endif

	sockaddr_in address;
	memset ( &address, 0, sizeof(sockaddr_in) );
	address.sin_family = AF_INET;
	address.sin_port = htons ( usPort );

	if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
		address.sin_addr.s_addr = inet_addr ( CVAR_GET_STRING ( "serverip" ) );

	m_iSocket = socket ( AF_INET, SOCK_DGRAM, 0 );

	// Set the socket non blocking
#ifdef WIN32
	unsigned long sockopt = 1;
	ioctlsocket ( m_iSocket, FIONBIO, &sockopt );
#else
	fcntl ( m_iSocket, F_SETFL, fcntl ( m_iSocket, F_GETFL ) | O_NONBLOCK );
#endif

	if ( bind ( m_iSocket, (sockaddr *)&address, sizeof ( sockaddr_in ) ) == -1 )
		CLogFile::Printf ( "Failed to bind query socket on port %d!", usPort );

	m_workerThread.SetUserData< bool > ( true );
	m_workerThread.Start ( WorkerThread );
}

CQuery::~CQuery( void )
{
	m_workerThread.SetUserData< bool > ( false );
	m_workerThread.Stop ( false, true );

	if( m_iSocket != -1 )
		closesocket ( m_iSocket );

#ifdef WIN32
	WSACleanup ();
#endif
}

std::string CQuery::QueryLight( void )
{
	//
	std::stringstream reply;
	reply << "M2Online";

	// hostname
	reply << ( unsigned char )( strlen( CVAR_GET_STRING( "hostname" ) ) + 1 );
	reply << CVAR_GET_STRING( "hostname" );

	// players
	String strPlayers( "%d", CCore::Instance()->GetPlayerManager()->GetCount() );
	reply << ( unsigned char )( strPlayers.GetLength() + 1 );
	reply << strPlayers.Get();

	// players
	String strMaxPlayers( "%d", CVAR_GET_INTEGER( "maxplayers" ) );
	reply << ( unsigned char )( strMaxPlayers.GetLength() + 1 );
	reply << strMaxPlayers.Get();

	// gamemode
	reply << ( unsigned char )( strlen( CCore::Instance()->GetGameModeText() ) + 1 );
	reply << CCore::Instance()->GetGameModeText();

	// password
	reply << ( unsigned char )( CCore::Instance()->IsPasswordProtected() ? 1 : 0 );

	// Players def
	CNetworkPlayer * pPlayer = NULL;
	char szName[256] = { '\0' };
	char szPing[256] = { '\0' };

	// players
	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		// Is the current player active?
		if( CCore::Instance()->GetPlayerManager()->IsActive( i ) )
		{
			// Get a pointer to the current player
			pPlayer = CCore::Instance()->GetPlayerManager()->Get( i );

			// (TODO: Fix this ugly ifdef)
#ifdef _WIN32
			_snprintf ( szName, 255, "%s", pPlayer->GetNick () );
			_snprintf ( szPing, 255, "%u", pPlayer->GetPing () );
#else
			snprintf ( szName, 255, "%s", pPlayer->GetNick () );
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
