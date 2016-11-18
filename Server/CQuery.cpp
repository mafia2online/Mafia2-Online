/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CQuery.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/
#include	<thread>

#include "StdInc.h"
#include "SharedUtility.h"
#include "CCore.h"

#include "CQuery.h"

void CQuery::WorkerThread ()
{
	while (m_processQueries)
	{
		int iSocket = GetSocket ();

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
				std::string reply = QueryLight ();
				sendto ( iSocket, reply.c_str (), reply.length (), NULL, (sockaddr *)&address, sizeof ( sockaddr_in ) );
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

#ifdef WIN32
	unsigned long sockopt = 1;
	ioctlsocket ( m_iSocket, FIONBIO, &sockopt );
#else
	fcntl ( m_iSocket, F_SETFL, fcntl ( m_iSocket, F_GETFL ) | O_NONBLOCK );
#endif

	if ( bind ( m_iSocket, (sockaddr *)&address, sizeof ( sockaddr_in ) ) == -1 )
		CLogFile::Printf ( "Failed to bind query socket on port %d!", usPort );

	m_processQueries = true;
	m_thread = std::thread(&CQuery::WorkerThread, this);
}

CQuery::~CQuery( void )
{
	if (m_thread.joinable()){
		m_processQueries = false;
		m_thread.join();
	}

	if( m_iSocket != -1 )
		closesocket ( m_iSocket );

#ifdef WIN32
	WSACleanup ();
#endif
}

std::string CQuery::QueryLight( void )
{
	std::stringstream reply;
	reply << "M2Online";

	reply << ( unsigned char )( strlen( CVAR_GET_STRING( "hostname" ) ) + 1 );
	reply << CVAR_GET_STRING( "hostname" );

	String strPlayers;
	strPlayers.Format( "%d", CCore::Instance()->GetPlayerManager()->GetCount() );
	reply << ( unsigned char )( strPlayers.GetLength() + 1 );
	reply << strPlayers.Get();

	String strMaxPlayers;
	strMaxPlayers.Format( "%d", CVAR_GET_INTEGER( "maxplayers" ) );
	reply << ( unsigned char )( strMaxPlayers.GetLength() + 1 );
	reply << strMaxPlayers.Get();

	reply << ( unsigned char )( strlen( CCore::Instance()->GetGameModeText() ) + 1 );
	reply << CCore::Instance()->GetGameModeText();

	reply << ( unsigned char )( CCore::Instance()->IsPasswordProtected() ? 1 : 0 );

	CNetworkPlayer * pPlayer = nullptr;
	char szName[256] = { '\0' };
	char szPing[256] = { '\0' };

	for( EntityId i = 0; i < MAX_PLAYERS; i++ )
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive( i ) )
		{
			pPlayer = CCore::Instance()->GetPlayerManager()->Get( i );

#ifdef _WIN32
			_snprintf ( szName, 255, "%s", pPlayer->GetNick () );
			_snprintf ( szPing, 255, "%u", pPlayer->GetPing () );
#else
			snprintf ( szName, 255, "%s", pPlayer->GetNick () );
			snprintf ( szPing, 255, "%u", pPlayer->GetPing () );
#endif

			reply << ( unsigned char )( strlen( szName ) + 1 );
			reply << szName;

			reply << ( unsigned char )( strlen( szPing ) + 1 );
			reply << szPing;
		}
	}

	return reply.str();
}
