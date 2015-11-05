/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

bool ReadString( std::string &strRead, const char * szBuffer, unsigned int &i, unsigned int nLength )
{
    if ( i <= nLength )
    {
		unsigned char len = szBuffer[i];
        if ( i + len <= nLength && len > 0 )
        {
            const char *ptr = &szBuffer[i + 1];
            i += len;
            strRead = std::string ( ptr, len - 1 );
            return true;
        }
        i++;
    }
    return false;
}

bool CServerListItem::Query ( void )
{
	// Prepare the socket
	sockaddr_in addr;
	memset ( &addr, 0, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons ( usGamePort + 1 );
	addr.sin_addr.s_addr = inet_addr ( strHost.Get() );

	// Is the socket invalid?
	if ( m_iSocket == INVALID_SOCKET )
	{
		// Initialise the socket
		m_iSocket = socket ( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
		u_long flag = 1;
		ioctlsocket ( m_iSocket, FIONBIO, &flag );
	}

	// Send the query
	if ( sendto ( m_iSocket, "M2MP", 4, 0, (sockaddr *)&addr, sizeof(addr) ) == 4 )
	{
		// Set the query start time
		ulQueryStart = SharedUtility::GetTime ();
		return true;
	}

	return false;
}

bool CServerListItem::Parse( const char * szBuffer, unsigned int uiLength )
{
	// Check the header
	if( strncmp( szBuffer, "M2MP", 4 ) != 0 )
		return false;

	// Calculate the ping
	usPing = ((SharedUtility::GetTime () - ulQueryStart) / 2);

	// Parse the data
	unsigned int i = 4;

	// Hostname
	std::string sHost;
	if( !ReadString( sHost, szBuffer, i, uiLength ) )
		return false;

	// Set the hostname
	strHostname = sHost.c_str();

	// Player count
	std::string sPlayers;
	if( !ReadString( sPlayers, szBuffer, i, uiLength ) )
		return false;

	//
	usPlayers = atoi( sPlayers.c_str() );

	// Max player count
	std::string sMaxPlayers;
	if( !ReadString( sMaxPlayers, szBuffer, i, uiLength ) )
		return false;

	//
	usMaxPlayers = atoi( sMaxPlayers.c_str() );

	// Gamemode
	std::string sGamemode;
	if( !ReadString( sGamemode, szBuffer, i, uiLength ) )
		return false;

	// Set the gamemode
	strGamemode = sGamemode.c_str();

	// Password
	bPassworded = (szBuffer[i++] == 1);

	// Players
	m_players.clear ();
	while ( i < uiLength )
	{
		// Read the player name and ping
		std::string sPlayer, sPing;
		if ( ReadString ( sPlayer, szBuffer, i, uiLength ) && ReadString ( sPing, szBuffer, i, uiLength ) )
		{
			// Add the player to the players map
			m_players.insert ( m_players.end(), std::pair < std::string, unsigned short > ( sPlayer, (unsigned short)atoi ( sPing.c_str () ) ) );

			//CLogFile::Printf ( "Added player '%s' (Ping: %d) (Server: %s:%d)", sPlayer.c_str(), atoi(sPing.c_str()), strHost.Get(), usGamePort );
		}
	}

	// Add this server to the serverbrowser
	pCore->GetGUI()->GetServerBrowser()->AddServer ( m_type, this );
	
	// Mark as scanned and return
	return bScanned = true;
}

String CServerListItem::Pulse ( void )
{
	// Did we skip this query?
	if ( bSkipped || bScanned )
		return "Done";

	// Has the query not yet been sent?
	if ( ulQueryStart == 0 )
	{
		// Query the server
		Query ();
		return "SentQuery";
	}

	char szBuffer[4096] = {0};

	// Poll the socket
	sockaddr_in addr;
	int addrLen = sizeof ( addr );
	int len = recvfrom ( m_iSocket, szBuffer, 4096, 0, (sockaddr *)&addr, &addrLen );
	if ( len >= 0 )
	{
		// Parse data
		Parse ( szBuffer, len );
		return "ParsedQuery";
	}

	if ( (SharedUtility::GetTime () - ulQueryStart) > 4000 )
	{
		bSkipped = true;
		return "NoReply";
	}

	return "WaitingReply";
}

CServerList::CServerList( ServerBrowserType type, CServerQuery * pServerQuery )
{
	// Reset
	m_type = type;
	m_bUpdated = false;
	m_strStatus = "Idle";
	m_uiScanned = 0;
	m_uiSkipped = 0;

	//
	m_pServerQuery = pServerQuery;
}

CServerList::~CServerList( void )
{
	// Clear the list memory
	Clear ();
}

void CServerList::Add( String strAddress )
{
	// Get the ip / port from the server address
	std::vector< String > server = strAddress.split( ':' );
	String strIp = server.front ();
	String strPort = server.back ();

	// Create a server list item
	CServerListItem * pServer = new CServerListItem ( m_type, strIp, strPort.ToInteger () );

	// Add the server into the server list
	m_Servers.push_back ( pServer );
}

void CServerList::Clear( void )
{
	// Clear the server list
	m_Servers.clear ();

	// Reset
	m_bUpdated = false;
	m_strStatus = "Idle";
	m_uiScanned = 0;
	m_uiSkipped = 0;
}

CServerListItem * CServerList::GetItem ( const char * szHost, unsigned short usPort )
{
	// Loop over all items
	for ( CServerListIterator i = IteratorBegin (); i != IteratorEnd (); i ++ )
	{
		// Is this what we're looking for?
		if ( !(*i)->strHost.Compare ( szHost ) && (*i)->usGamePort == usPort )
			return *i;
	}

	return NULL;
}

void CServerList::Pulse ( void )
{
	// Have we done?
	if ( !m_Servers.size () || m_bUpdated )
		return;

	// Reset
	unsigned int uiQueriesSent = 0;
	unsigned int uiQueriesParsed = 0;
	unsigned int uiNoReplies = 0;
	unsigned int uiDone = 0;

	// Loop over all servers in the list
	for ( std::list < CServerListItem* >::iterator iter = m_Servers.begin (); iter != m_Servers.end (); iter ++ )
	{
		// Pulse the current server in the list
		String strReply = (*iter)->Pulse ();
		
		// Reply
		if ( strReply == "SentQuery" )
			uiQueriesSent ++;
		else if ( strReply == "ParsedQuery" )
			uiQueriesParsed ++;
		else if ( strReply == "NoReply" )
			uiNoReplies ++;
		else if ( strReply == "Done" )
			uiDone ++;

		// Don't send too many queries per frame
		if ( uiQueriesSent >= 2 )
			break;
	}

	// Update totals
	m_uiScanned += uiQueriesParsed;
	m_uiSkipped += uiNoReplies;

	// Have we parsed all servers?
	if ( (m_uiScanned + m_uiSkipped) == m_Servers.size () )
	{
		// Mark as done
		m_bUpdated = true;

		CLogFile::Printf ( "%d queries sent, %d replied, %d dropped.", m_Servers.size (), m_uiScanned, m_uiSkipped );
	}	
}