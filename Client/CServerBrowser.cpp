/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerBrowser.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

bool CServerBrowser::Event_QuickConnectSubmitClick ( CGUIElement_Impl * pElement )
{
	// Get the input text
	m_strServerIP = m_pQuickConnectAddress->GetText ();
	m_iServerPort = m_pQuickConnectPort->GetText().ToInteger ();
	m_strServerPassword = m_pQuickConnectPassword->GetText ();

	// Reset input text
	m_pQuickConnectAddress->SetText ( "" );
	m_pQuickConnectPort->SetText ( "" );
	m_pQuickConnectPassword->SetText ( "" );

	// If an invalid port is set, use the default?
	if ( m_iServerPort == 0 )
		m_iServerPort = 27015;

	// Start the connection
	StartConnection ();

	// Hide the quick connect window
	m_pQuickConnectWindow->SetVisible ( false );

	// Store the last server information
	CVAR_SET ( "last-server-ip", m_strServerIP.Get () );
	CVAR_SET ( "last-server-port", m_iServerPort );
	CVAR_SET ( "last-server-password", m_strServerPassword.Get () );

	return true;
}

bool CServerBrowser::Event_QuickConnectCancelClick ( CGUIElement_Impl * pElement )
{
	// Reset input text
	m_pQuickConnectAddress->SetText ( "" );
	m_pQuickConnectPort->SetText ( "" );
	m_pQuickConnectPassword->SetText ( "" );

	// Hide the quick connect window
	m_pQuickConnectWindow->SetVisible ( false );
	return true;
}

bool CServerBrowser::Event_QuickConnectInputFocus ( CGUIElement_Impl * pElement )
{
	// Reset the element text
	pElement->SetText ( "" );
	return true;
}

void CServerBrowser::ServerPasswordHandler ( String strPassword, void * pUserData )
{
	// Get the server browser instance
	CServerBrowser * pServerBrowser = (CServerBrowser *)pUserData;

	// Is the server browser instance valid?
	if ( pServerBrowser && pServerBrowser->m_pSelectedServer )
	{
		// Hide the password window
		pServerBrowser->m_pServerPassword->SetVisible ( false );

		// Store the server ip, port and password
		pServerBrowser->m_strServerIP = pServerBrowser->m_pSelectedServer->strHost;
		pServerBrowser->m_iServerPort = pServerBrowser->m_pSelectedServer->usGamePort;
		pServerBrowser->m_strServerPassword = strPassword;

		// Start the connection
		pServerBrowser->StartConnection ();

		// Reset the selected server
		pServerBrowser->m_pSelectedServer = NULL;
	}
}

CServerBrowser::CServerBrowser( CGUI_Impl * pGUI )
{
	// Store the gui pointer
	m_pGUI = pGUI;

	// Reset
	m_ulLastRefreshTime = 0;
	m_strServerPassword.clear ();
	m_iTotalServers = 0;
	m_iTotalPlayers = 0;
	m_iAvailableSlots = 0;

	// Create the masterlist instance
	m_pMasterList = new CMasterList( Event_MasterListQueryHandler );

	// Create the server query instance
	m_pServerQuery = new CServerQuery;

	// Create the server password instance
	m_pServerPassword = new CServerPassword ( pGUI );
	m_pServerPassword->SetSubmitHandler ( ServerPasswordHandler, this );

	// Reset the connection state
	SetConnectionState( CONNECTION_STATE_NONE );
}

CServerBrowser::~CServerBrowser( void )
{
	// Delete the gui items
	SAFE_DELETE( m_pStatusLabel );
	SAFE_DELETE( m_pMessageBox );
	SAFE_DELETE( m_pLockedImage );
	SAFE_DELETE( m_pStatus );
	SAFE_DELETE( m_pPlayers );
	SAFE_DELETE( m_pFavourite );
	SAFE_DELETE( m_pConnect );
	SAFE_DELETE( m_pRefresh );
	SAFE_DELETE( m_pTabs );
	SAFE_DELETE( m_pWindow );

	// Delete the quick connect gui
	SAFE_DELETE ( m_pQuickConnectCancel );
	SAFE_DELETE ( m_pQuickConnectSubmit );
	SAFE_DELETE ( m_pQuickConnectPassword );
	SAFE_DELETE ( m_pQuickConnectPort );
	SAFE_DELETE ( m_pQuickConnectAddress );
	SAFE_DELETE ( m_pQuickConnectLabel );
	SAFE_DELETE ( m_pQuickConnectWindow );

	// Delete the server password instance
	SAFE_DELETE ( m_pServerPassword );
	
	// Delete the server query instance
	SAFE_DELETE ( m_pServerQuery );

	// Delete the masterlist instance
	SAFE_DELETE( m_pMasterList );

	// Delete the tabs
	DeleteTab( INTERNET );
	DeleteTab( HOSTED );
	DeleteTab( FAVOURITES );
	DeleteTab( HISTORY );
}

void CServerBrowser::Pulse( void )
{
	// Pulse the current server list
	if ( m_pServerList [ GetCurrentServerBrowserType () ] )
		m_pServerList [ GetCurrentServerBrowserType () ]->Pulse ();

	// Pulse the connection state
	if( m_connectionState == CONNECTION_STATE_WAITING_FOR_GAME )
	{
		// Is the game loaded now?
		if( pCore->IsGameLoaded() )
		{
			// Move to the next step
			m_connectionState = CONNECTION_STATE_CONNECTING;

			// Connect!
			ProcessConnection();
		}
	}
	else if( m_connectionState == CONNECTION_STATE_FAILED )
	{
		// Has 3 seconds passed?
		if( (SharedUtility::GetTime() - m_ulFailedTime) >= 3000 )
		{
			// Hide the message box
			m_pMessageBox->SetVisible ( false );

			// Show the main menu
			pCore->GetGUI()->GetMainMenu()->SetVisible( true );

			// Reset the connection state
			SetConnectionState( CONNECTION_STATE_NONE );

			// Reset the failed time
			m_ulFailedTime = 0;
		}
	}
}

void CServerBrowser::Button1ClickHandler ( void * pUserData )
{
	// Get the server browser instance
	CServerBrowser * pServerBrowser = (CServerBrowser *)pUserData;

	// Hide the message box
	if ( pServerBrowser )
		pServerBrowser->GetMessageBox()->SetVisible ( false );
}

void CServerBrowser::SetupUI( float fX, float fY, float fWidth, float fHeight )
{
	// Create the gui tab panel
	m_pTabs = m_pGUI->CreateTabPanel();
	m_pTabs->SetPosition( Vector2( fX, fY ) );
	m_pTabs->SetSize( Vector2( (fWidth - 260), fHeight ) );
	m_pTabs->SetAlwaysOnTop( true );

	// Create the players list
	m_pPlayers = m_pGUI->CreateTabPanel();
	m_pPlayers->SetPosition( Vector2( ((fX + fWidth) - 250), fY ) );
	m_pPlayers->SetSize( Vector2( 250, fHeight ) );
	m_pPlayers->SetAlwaysOnTop( true );

	// Create the status label
	m_pStatusLabel = m_pGUI->CreateLabel ( "", m_pGUI->GetDefaultFont( true ) );
	m_pStatusLabel->SetSize ( Vector2 ( (fWidth - 260), 30 ) );
	m_pStatusLabel->SetPosition ( Vector2 ( fX, (fY + fHeight + 10) ) );

	// Create the players list tabs
	m_pPlayersTab = m_pPlayers->CreateTab( "Players" );
	m_pRulesTab = m_pPlayers->CreateTab( "Rules" );

	// Create the players grid list
	m_pPlayersGridList = m_pGUI->CreateGridList( m_pPlayersTab );
	m_pPlayersGridList->SetPosition( Vector2( 1, 1 ) );
	m_pPlayersGridList->SetSize( Vector2( 247, (fHeight - 26) ) );
	m_pPlayersGridList->AddColumn ( "Name", 0.65f );
	m_pPlayersGridList->AddColumn ( "Ping", 0.30f );

	// Create the locked image
	m_pLockedImage = m_pGUI->CreateStaticImage();
	m_pLockedImage->SetVisible( false );
	m_pLockedImage->LoadFromFile( "locked.png", SharedUtility::GetAbsolutePath( "data\\gui\\images" ) );
	m_pLockedImage->SetSize ( Vector2 ( 12, 12 ) );

	// Create the tabs
	CreateTab( INTERNET, "Internet", (fWidth - 262), (fHeight - 26) );
	CreateTab( HOSTED, "Hosted", (fWidth - 262), (fHeight - 26) );

	// Hide the server browser
	SetVisible( false );

	// Create the message box window
	m_pMessageBox = m_pGUI->CreateMessageBox ( "", "", "Close" );
	m_pMessageBox->SetVisible ( false );
	m_pMessageBox->GetWindow()->SetAlwaysOnTop ( true );
	m_pMessageBox->SetButtonClickHandler ( 0, Button1ClickHandler, this );

	// Create the quick connect window
	m_pQuickConnectWindow = m_pGUI->CreateWnd ( "Quick Connect" );
	m_pQuickConnectWindow->SetSize ( Vector2 ( 260, 155 ) );
	m_pQuickConnectWindow->SetPosition ( Vector2 ( (m_pGUI->GetResolution().fX / 2) - 150, (m_pGUI->GetResolution().fY / 2) - 77.5 ) );
	m_pQuickConnectWindow->SetVisible ( false );
	m_pQuickConnectWindow->SetAlwaysOnTop ( true );
	m_pQuickConnectWindow->SetSizingEnabled ( false );
	m_pQuickConnectWindow->SetAlpha ( 1.0f );

	// Create the quick connect label
	m_pQuickConnectLabel = m_pGUI->CreateLabel ( "Enter the server IP, Port and Password:", NULL, m_pQuickConnectWindow );
	m_pQuickConnectLabel->SetSize ( Vector2 ( 200, 20 ) );
	m_pQuickConnectLabel->SetPosition ( Vector2 ( 20, 25 ) );

	// Should we restore server info?
	bool bRestoreInformation = (strlen(CVAR_GET_STRING("last-server-ip")) != 0);

	// Create the quick connect IP input
	m_pQuickConnectAddress = m_pGUI->CreateEdit ( (bRestoreInformation ? CVAR_GET_STRING ( "last-server-ip" ) : "127.0.0.1"), m_pQuickConnectWindow );
	m_pQuickConnectAddress->SetSize ( Vector2( 160, 24 ) );
	m_pQuickConnectAddress->SetPosition ( Vector2 ( 18, 50 ) );
	m_pQuickConnectAddress->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );

	// Create the quick connect port input
	m_pQuickConnectPort = m_pGUI->CreateEdit ( (bRestoreInformation ? String ( "%d", CVAR_GET_INTEGER ( "last-server-port" ) ) : "27015"), m_pQuickConnectWindow );
	m_pQuickConnectPort->SetSize ( Vector2( 55, 24 ) );
	m_pQuickConnectPort->SetPosition ( Vector2 ( 183, 50 ) );
	m_pQuickConnectPort->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );

	// Create the quick connect password input
	m_pQuickConnectPassword = m_pGUI->CreateEdit ( (bRestoreInformation ? CVAR_GET_STRING ( "last-server-password" ) : ""), m_pQuickConnectWindow );
	m_pQuickConnectPassword->SetSize ( Vector2( 220, 24 ) );
	m_pQuickConnectPassword->SetPosition ( Vector2( 18, 85 ) );
	m_pQuickConnectPassword->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );
	m_pQuickConnectPassword->SetMasked ( true );

	// Create the quick connect submit button
	m_pQuickConnectSubmit = m_pGUI->CreateButton ( "Connect", m_pQuickConnectWindow );
	m_pQuickConnectSubmit->SetSize ( Vector2 ( 75, 20 ) );
	m_pQuickConnectSubmit->SetPosition ( Vector2 ( 18, 120 ) );
	m_pQuickConnectSubmit->SetClickHandler ( GUI_CALLBACK( &CServerBrowser::Event_QuickConnectSubmitClick, this ) );

	// Create the quick connect cancel button
	m_pQuickConnectCancel = m_pGUI->CreateButton ( "Cancel", m_pQuickConnectWindow );
	m_pQuickConnectCancel->SetSize ( Vector2 ( 75, 20 ) );
	m_pQuickConnectCancel->SetPosition ( Vector2 ( 100, 120 ) );
	m_pQuickConnectCancel->SetClickHandler ( GUI_CALLBACK( &CServerBrowser::Event_QuickConnectCancelClick, this ) );
}

void CServerBrowser::CreateTab( ServerBrowserType type, const char * szName, float fWidth, float fHeight )
{
	// Create the gui tab
	CGUITab_Impl * pTab = m_pTabs->CreateTab( szName );

	// Set the tab instance
	m_pTab[ type ] = pTab;

	// Create the server grid list
	m_pServerGridList[ type ] = m_pGUI->CreateGridList( pTab );
	m_pServerGridList[ type ]->SetPosition( Vector2( 1, 1 ) );
	m_pServerGridList[ type ]->SetSize( Vector2( fWidth, fHeight ) );
	m_pServerGridList[ type ]->SetClickHandler( GUI_CALLBACK( &CServerBrowser::Event_OnMouseClick, this ) );
	// todo: double click handler

	// Create the server grid list columns
	m_pServerGridList[ type ]->AddColumn( "", 0.02f );
	m_pServerGridList[ type ]->AddColumn( "Hostname", 0.45f );
	m_pServerGridList[ type ]->AddColumn( "Players", 0.13f );
	m_pServerGridList[ type ]->AddColumn( "Gamemode", 0.25f );
	m_pServerGridList[ type ]->AddColumn( "Ping", 0.14f );
	m_pServerGridList[ type ]->AddColumn( "Host", 0.13f );
	m_pServerGridList[ type ]->AddColumn( "Port", 0.5f );

	// Create the server list instance
	m_pServerList[ type ] = new CServerList( type, m_pServerQuery );
}

void CServerBrowser::DeleteTab( ServerBrowserType type )
{
	// Delete the server list instance
	SAFE_DELETE( m_pServerList[ type ] );

	// Delete the server grid list
	SAFE_DELETE( m_pServerGridList[ type ] ) ;

	// Delete the server tab
	SAFE_DELETE( m_pTab[ type ] );
}

void CServerBrowser::AddServer( ServerBrowserType type, CServerListItem * pServer )
{
	// Add a row
	int iRow = m_pServerGridList[ type ]->AddRow ();

	// Store the row
	pServer->iRow = iRow;

	// Set the locked image
	if( pServer->bPassworded )
		m_pServerGridList[ type ]->SetItemImage( iRow, 1, m_pLockedImage );

	// Set the server information
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 2, pServer->strHostname );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 3, String( "%d/%d", pServer->usPlayers, pServer->usMaxPlayers ), true );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 4, pServer->strGamemode );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 5, String( "%d", pServer->usPing ), true );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 6, pServer->strHost );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 7, String( "%d", pServer->usGamePort ), true );

	// Increase totals
	m_iTotalServers++;
	m_iTotalPlayers += pServer->usPlayers;
	m_iAvailableSlots += (pServer->usMaxPlayers - pServer->usPlayers);

	// Update status label
	m_pStatusLabel->SetText ( String ( "%d players playing on %d servers. (%d slots available)", m_iTotalPlayers, m_iTotalServers, m_iAvailableSlots ) );
}

void CServerBrowser::ConnectToSelectedServer( void )
{
	// Get the server list type
	ServerBrowserType type = GetCurrentServerBrowserType();

	// Reset saved password
	m_strServerPassword.clear ();

	// Did we select a server?
	if( m_pServerGridList[ type ]->GetSelectedCount() > 0 )
	{
		// Get the selected row
		int iSelectedRow = m_pServerGridList[ type ]->GetSelectedItemRow();

		// Get the server ip and port
		char* szIp = m_pServerGridList[ type ]->GetItemText( iSelectedRow, 6 );
		int iPort = atoi( m_pServerGridList[ type ]->GetItemText( iSelectedRow, 7 ) );

		// Get the server list item?
		m_pSelectedServer = m_pServerList[ type ]->GetItem ( szIp, iPort );
	}

	// Is the item valid?
	if ( m_pSelectedServer )
	{
		// Is the server password protected?
		if ( m_pSelectedServer->bPassworded )
		{
			// Show the password window
			m_pServerPassword->SetVisible ( true );
		}
		else
		{
			// Store the server ip and port
			m_strServerIP = m_pSelectedServer->strHost;
			m_iServerPort = m_pSelectedServer->usGamePort;

			// Start the connection
			StartConnection ();

			// Reset the selected server
			m_pSelectedServer = NULL;
		}
	}
}

void CServerBrowser::StartConnection ( void )
{
	// Are we already connected to a server?
	if( pCore->GetNetworkModule() && pCore->GetNetworkModule()->IsConnected() )
	{
		// Disconnect from the server
		pCore->GetNetworkModule()->Disconnect();

		// Stop multiplayer
		pCore->StopMultiplayer ();

		// Set the connection state
		SetConnectionState( CONNECTION_STATE_CONNECTING );

		// Start multiplayer
		pCore->StartMultiplayer ();

		// Connect!
		ProcessConnection ();
	}

	// Is the game not yet loaded?
	if( !pCore->IsGameLoaded() )
	{
		// Set the connection state
		SetConnectionState( CONNECTION_STATE_WAITING_FOR_GAME );

		// Show the message box
		SetMessageBox ( "Connecting", "Waiting for game to load..." );
	}
	else
	{
		// Move to the next step
		m_connectionState = CONNECTION_STATE_CONNECTING;

		// Connect!
		ProcessConnection ();
	}
}

void CServerBrowser::ProcessConnection( void )
{
	// Attempt to connect to the server
	eNetworkResponse response = pCore->GetNetworkModule()->Connect( m_strServerIP, m_iServerPort, m_strServerPassword );

	// Get the response string
	String strMessage( "An unknown error occurred." );
	switch( response )
	{
	case NETWORK_RESPONSE_SUCCESS: strMessage.Format( "Joining %s:%d...", m_strServerIP.Get(), m_iServerPort ); break;
	case NETWORK_RESPONSE_INVALID_PARAM: strMessage.Set( "An internal error occurred" ); break;
	case NETWORK_RESPONSE_CANT_RESOLVE_DOMAIN: strMessage.Set( "Can't resolve domain name!" ); break;
	case NETWORK_RESPONSE_ALREADY_CONNECTED: strMessage.Set( "You're already connect to a server." ); break;
	case NETWORK_RESPONSE_ALREADY_CONNECTING: strMessage.Set( "A connection attempt is already in progress!" ); break;
	case NETWORK_RESPONSE_SECURITY_FAILED: strMessage.Set( "Security initialisation failed!" ); break;
	case NETWORK_RESPONSE_NO_HOST_SET: strMessage.Set( "An internal error occurred." ); break;
	}

	// Show the message box
	SetMessageBox ( ((int)response == 0 ? "Connecting" : "Failed to connect"), strMessage.Get () );

	// Did we fail to connect?
	if( (int)response > 0 )
	{
		// Set the connection state to failed
		SetConnectionState( CONNECTION_STATE_FAILED );

		// Set the failed time
		m_ulFailedTime = SharedUtility::GetTime();
	}
}

bool CServerBrowser::Event_OnMouseClick( CGUIElement_Impl * pElement )
{
	// Get the serverbrowser instance
	CServerBrowser * pInst = pCore->GetGUI()->GetServerBrowser();

	// Was the clicked element the server grid list?
	if( pElement == pInst->GetServerGridList( ServerBrowserType::INTERNET ) || pElement == pInst->GetServerGridList( ServerBrowserType::HOSTED ) ||
		pElement == pInst->GetServerGridList( ServerBrowserType::FAVOURITES ) || pElement == pInst->GetServerGridList( ServerBrowserType::HISTORY ) )
	{
		// Handle the mouse click
		pInst->OnClick( pElement );

		return true;
	}

	return false;
}

bool CServerBrowser::Event_OnMouseDoubleClick( CGUIMouseEventArgs args )
{
	// Get the serverbrowser instance
	CServerBrowser * pInst = pCore->GetGUI()->GetServerBrowser();

	// Was the clicked element the server grid list?
	if( args.pWindow == pInst->GetServerGridList( ServerBrowserType::INTERNET ) || args.pWindow == pInst->GetServerGridList( ServerBrowserType::HOSTED ) ||
		args.pWindow == pInst->GetServerGridList( ServerBrowserType::FAVOURITES ) || args.pWindow == pInst->GetServerGridList( ServerBrowserType::HISTORY ) )
	{
		// Handle the mouse double click
		pInst->OnDoubleClick( args.pWindow );

		return true;
	}

	return false;
}

void CServerBrowser::Event_MasterListQueryHandler( const std::vector<String>& servers )
{
	// Get the current server list
	CServerList * pList = pCore->GetGUI()->GetServerBrowser()->GetServerList( pCore->GetGUI()->GetServerBrowser()->GetCurrentServerBrowserType() );

	// Loop over all servers in the vector
	for ( auto server : servers )
	{
		// Add the new server to the list
		if ( server.GetLength() > 1 )
			pList->Add( server );
	}
}

void CServerBrowser::Refresh( void )
{
	// Get the current server type
	ServerBrowserType type = GetCurrentServerBrowserType();

	// Set the last refresh time
	m_ulLastRefreshTime = SharedUtility::GetTime();

	// Clear the current servers memory
	m_pServerList[ type ]->Clear ();

	// Clear the gridlist
	m_pServerGridList[ type ]->Clear ();

	// Clear the player gridlist
	m_pPlayersGridList->Clear ();

	// Hide the connect button
	pCore->GetGUI()->GetMainMenu()->SetConnectButtonVisible( false );

	// Reset totals
	m_iTotalServers = m_iTotalPlayers = m_iAvailableSlots = 0;

	// Reset status label
	m_pStatusLabel->SetText ( "" );

	// Are we refreshing the internet or hosted tab?
	if( type == ServerBrowserType::INTERNET || type == ServerBrowserType::HOSTED )
	{
		// Refresh the master server list
		m_pMasterList->Refresh ( (type == INTERNET ? E_REFRESH_NONE : E_REFRESH_HOSTED) );
	}
	else if ( type == ServerBrowserType::LAN )
	{

	}
	/*else if( type == ServerBrowserType::FAVOURITES )
	{
		// Refresh the favourites
		RefreshFromFile( "data\\browser\\favourites.xml" );
	}
	else if( type == ServerBrowserType::HISTORY )
	{
		// Refresh the history
		RefreshFromFile( "data\\browser\\history.xml" );
	}*/
}

void CServerBrowser::OnClick( CGUIElement_Impl * pElement )
{
	// Get the current serverbrowser type
	ServerBrowserType type = GetCurrentServerBrowserType();

	// Clear the current playerlist
	m_pPlayersGridList->Clear();

	// Have we selected any server?
	if( m_pServerGridList[ type ]->GetSelectedCount() > 0 )
	{
		// Enable the connection button in the main menu
		pCore->GetGUI()->GetMainMenu()->SetConnectButtonVisible( true );

		// Get the selected row index
		int iSelectedIndex = m_pServerGridList[ type ]->GetSelectedItemRow();

		// Find this server from the serverlist
		CServerList * pServerList = GetServerList( type );
		CServerListIterator iter, begin = pServerList->IteratorBegin(), end = pServerList->IteratorEnd();

		// Loop over all servers in the list
		for( iter = begin; iter != end; iter++ )
		{
			// Get the current server
			CServerListItem * pCurrentServer = *iter;

			// Is the current server valid and the one we're looking for?
			if( pCurrentServer && pCurrentServer->iRow == iSelectedIndex )
			{
				// Loop over the players
				for( std::map< std::string, unsigned short >::iterator iter2 = pCurrentServer->m_players.begin(); iter2 != pCurrentServer->m_players.end(); iter2++ )
				{
					// Add a new row in the players grid
					int iRow =  m_pPlayersGridList->AddRow();
					iRow = m_pPlayersGridList->SetItemText ( iRow, 1, (*iter2).first.c_str () );
					iRow = m_pPlayersGridList->SetItemText ( iRow, 2, String ( "%u", (*iter2).second ), true );
				}
			}
		}

		// Store the current selected server
		m_iSelectedServer[ type ] = iSelectedIndex;
	}
	else
	{
		// Disable the connection button
		pCore->GetGUI()->GetMainMenu()->SetConnectButtonVisible( false );
	}
}

void CServerBrowser::OnDoubleClick( CGUIElement_Impl * pElement )
{
}

void CServerBrowser::SetVisible( bool bVisible )
{
	// Toggle the main tab window
	m_pTabs->SetVisible( bVisible );

	// Bring it to the front
	m_pTabs->BringToFront();

	// Toggle the players tab window
	m_pPlayers->SetVisible( bVisible );

	// Bring it to the front
	m_pPlayers->BringToFront();

	// Toggle the status label
	m_pStatusLabel->SetVisible ( bVisible );

	// Are we showing the serverbrowser?
	if( bVisible )
	{
		// Reset each selected index
		for( int i = 0; i < MAX_SERVER_LISTS; i++ )
			m_iSelectedServer[ i ] = -1;
	}
}

bool CServerBrowser::IsVisible( void )
{
	return m_pTabs->IsVisible();
}

void CServerBrowser::RefreshFromFile( const char * szFile )
{
	// Does the file not exist?
	if( !SharedUtility::Exists( szFile ) )
		return;

	// Create the xml file instance
	CXML * pFile = new CXML( szFile );

	// Get the root node
	CXMLNode * pRootNode = pFile->GetRootNode();

	// Is the root node valid?
	if( pRootNode )
	{
		//
		String strBuffer;
		CXMLNode * pCurrentNode = NULL;
		unsigned int uiChildCount = pRootNode->GetChildCount();

		// Loop over each node
		for( unsigned int i = 0; i < uiChildCount; i++ )
		{
			// Get the current node
			pCurrentNode = pRootNode->GetNode( i );

			// Is the current node invalid?
			if( !pCurrentNode )
				continue;

			// Is this an invalid node?
			if( strcmp( pCurrentNode->GetName(), "server" ) )
				continue;

			// Append the node value to our main string
			strBuffer += String( "%s|", pCurrentNode->GetValue() );
		}

		// Process the buffer with the masterlist handler
		Event_MasterListQueryHandler( strBuffer.split( '|' ) );
	}

	// Close the xml file
	pFile->Save();

	// Destroy the xml file instance
	SAFE_DELETE( pFile );
}

CServerList * CServerBrowser::GetServerList( ServerBrowserType type )
{
	return m_pServerList[ type ];
}

ServerBrowserType CServerBrowser::GetCurrentServerBrowserType( void )
{
	// Get the tab
	if( m_pTabs->IsTabSelected( m_pTab[ INTERNET ] ) )
		return INTERNET;
	else if( m_pTabs->IsTabSelected( m_pTab[ HOSTED ] ) )
		return HOSTED;
	else if( m_pTabs->IsTabSelected( m_pTab[ FAVOURITES ] ) )
		return FAVOURITES;
	else if( m_pTabs->IsTabSelected( m_pTab[ HISTORY ] ) )
		return HISTORY;

	return INTERNET;
}

void CServerBrowser::ProcessNetworkPacket( DefaultMessageIDTypes packet )
{
	// Did we connect?
	if( packet == ID_CONNECTION_REQUEST_ACCEPTED )
	{
		// Set the server information
		pCore->SetHost( m_strServerIP );
		pCore->SetPort( m_iServerPort );

		// Hide the mouse cursor
		pCore->GetGUI()->SetCursorVisible( false );

		// Show the chat
		pCore->GetChat()->SetVisible( true );

		// Hide the message box
		m_pMessageBox->SetVisible ( false );
		return;
	}

	// Get the label text
	String strMessage( "You were disconnected from the server." );
	bool bLost = false;
	switch( packet )
	{
	case ID_NO_FREE_INCOMING_CONNECTIONS: strMessage.Set( "The maximum number of players has been reached." ); break;
	case ID_DISCONNECTION_NOTIFICATION: strMessage.Set( "The connection with the server was closed." ); bLost = true; break;
	case ID_INVALID_PASSWORD: strMessage.Set( "The password you entered was invalid." ); break;
	case ID_CONNECTION_BANNED: strMessage.Set( "You're banned from playing on this server." ); break;
	case ID_CONNECTION_LOST: strMessage.Set( "The connection with the server was lost." ); bLost = true; break;
	case ID_CONNECTION_ATTEMPT_FAILED: strMessage.Set( "Failed to connect to the server." ); break;
	case ID_ALREADY_CONNECTED: strMessage.Set( "You're already connected to a server." ); break;
	}

	// Show the message box
	SetMessageBox ( (bLost ? "Connection terminated" : "Failed to connect"), strMessage.Get () );
}

void CServerBrowser::SetDisconnectReason( bool bDisconnect, const char * szReason, ... )
{
	// Should we disconnect from the network?
	if( bDisconnect )
	{
		// Stop multiplayer
		pCore->StopMultiplayer();

		// Start multiplayer
		pCore->StartMultiplayer();
	}

	// Get the arguments
	char szBuffer[ MAX_MESSAGE_LEN ];
	va_list args;
	va_start( args, szReason );
	vsnprintf( szBuffer, MAX_MESSAGE_LEN, szReason, args );
	va_end( args );

	// Show the message box
	SetMessageBox ( "Disconnected from the server", szReason );

	// Set the connection state
	SetConnectionState( CONNECTION_STATE_FAILED );

	// Set the failed time
	m_ulFailedTime = SharedUtility::GetTime();

	// Disconnect from the network
	if( bDisconnect )
		pCore->GetNetworkModule()->Disconnect();
}

void CServerBrowser::SetMessageBox ( const char * szTitle, const char * szCaption )
{
	// Set the message box title
	m_pMessageBox->SetTitle ( szTitle );

	// Set the message box caption
	m_pMessageBox->SetCaption ( szCaption );
	
	// Show the message box
	m_pMessageBox->SetVisible ( true );
	m_pMessageBox->GetWindow()->BringToFront ();
}

void CServerBrowser::OnScreenSizeChange ( float fX, float fY )
{
	// Get the new width and height
	float fWidth = (fX - 80.0f);
	float fHeight = (fY - 240.0f);

	// Adjust the server list size
	m_pTabs->SetSize ( Vector2 ( (fWidth - 160.0f), fHeight ) );

	// Adjust the player list size
	Vector2 pos = m_pPlayers->GetPosition();
	m_pPlayers->SetPosition ( Vector2 ( (40.0f + fWidth) - 150.0f, pos.fY ) );
	m_pPlayers->SetSize ( Vector2 ( 150.0f, fHeight ) );

	// Ajust the server grid list size
	m_pServerGridList[ ServerBrowserType::INTERNET ]->SetSize( Vector2( (fWidth - 162.0f), (fHeight - 26.0f) ) );
	m_pServerGridList[ ServerBrowserType::HOSTED ]->SetSize( Vector2( (fWidth - 162.0f), (fHeight - 26.0f) ) );

	// Adjust the player grid list size
	m_pPlayersGridList->SetSize( Vector2( 147.0f, (fHeight - 26.0f) ) );

	// Notify the password handler
	m_pServerPassword->OnScreenSizeChange ( fX, fY );

	// Adjust the quick connect window
	m_pQuickConnectWindow->SetPosition ( Vector2 ( (fX / 2) - 150.0f, (fY / 2) - 95.0f ) );

	// Ajust the message box position
	//

	// Ajust the status label position
	m_pStatusLabel->SetPosition ( Vector2 ( (fX - 160.0f), (fHeight + 10.0f) ) );
}