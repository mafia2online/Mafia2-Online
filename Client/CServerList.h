/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerList.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define SERVER_LIST_DATA_BUFFER             16384
#define SERVER_LIST_QUERY_BUFFER            4096
#define SERVER_LIST_MASTER_TIMEOUT          10000
#define SERVER_LIST_QUERIES_PER_PULSE       2
#define SERVER_LIST_BROADCAST_REFRESH       2000

#include <map>

class CServerList;
class CServerListItem
{

public:
	
	ServerBrowserType			m_type;

	// Server info
	String						strHost;
	String						strHostname;
	String						strGamemode;
	unsigned short				usGamePort;
	unsigned short				usPlayers;
	unsigned short				usMaxPlayers;
	unsigned short				usPing;
	bool						bPassworded;
	unsigned long				ulQueryStart;

	// Server rules
	bool						bSummer;

	bool						bSkipped;
	bool						bScanned;
	int							iRow;
	std::map< std::string, unsigned short > m_players;

	// Socket
	int							m_iSocket;

	CServerListItem				( ServerBrowserType type, String strIp, unsigned short uPort )
	{
		m_type = type;
		strHost = strIp;
		usGamePort = uPort;
		usPlayers = 0;
		usMaxPlayers = 0;
		usMaxPlayers = 0;
		usMaxPlayers = 0;
		bPassworded = false;
		bSkipped = false;
		bScanned = false;
		iRow = 0;
		strHostname = String( "%s:%d", strHost.Get(), usGamePort );
		ulQueryStart = 0;

		// Setup the socket
		m_iSocket = socket ( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
		u_long flag = 1;
		ioctlsocket ( m_iSocket, FIONBIO, &flag );
	}

	~CServerListItem			( void )
	{
		// Close the socket
		CloseSocket ();
	}

	void				CloseSocket ( void )
	{
		// Is the socket open?
		if ( m_iSocket != INVALID_SOCKET ) 
		{
			// Close the socket
			closesocket ( m_iSocket );

			// Reset the socket
			m_iSocket = INVALID_SOCKET;
		}
	}

	bool				Query	( void );
	bool				Parse	( const char * szBuffer, unsigned int uiLength );
	String				Pulse	( void );

};

typedef std::list< CServerListItem* >::const_iterator CServerListIterator;

class CServerList
{

private:

	std::list< CServerListItem* >					m_Servers;
	std::list< CServerListItem* >					m_ServerQueries;
	bool											m_bUpdated;
	String											m_strStatus;
	unsigned int									m_uiScanned;
	unsigned int									m_uiSkipped;
	ServerBrowserType								m_type;

	CServerQuery									* m_pServerQuery;

public:

						CServerList					( ServerBrowserType type, CServerQuery * pServerQuery );
						~CServerList				( void );

	CServerListIterator	IteratorBegin				( void ) { return m_Servers.begin(); }
	CServerListIterator IteratorEnd					( void ) { return m_Servers.end(); }

	void				Add							( String strAddress );
	void				Remove						( CServerListItem * pServer ) { m_Servers.remove( pServer ); }
	void				Clear						( void );

	CServerListItem		* GetItem					( const char * szHost, unsigned short usPort );
	String				GetStatus					( void ) { return m_strStatus; }

	void				Pulse						( void );

};