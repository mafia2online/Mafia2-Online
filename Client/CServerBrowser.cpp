/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerBrowser.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"
#include "CString.h"
#include "Math/CVector3.h"

#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIElement_Impl.h"
#include "gui_impl/CGUIEdit_Impl.h"
#include "gui_impl/CGUIWindow_Impl.h"
#include "gui_impl/CGUIMessageBox_Impl.h"
#include "gui_impl/CGUITabPanel_Impl.h"
#include "gui_impl/CGUILabel_Impl.h"
#include "gui_impl/CGUIGridList_Impl.h"
#include "gui_impl/CGUIStaticImage_Impl.h"
#include "gui_impl/CGUIButton_Impl.h"

#include "CSettings.h"

#include "CGUI.h"
#include "CGUIEvent.h"
#include "CGUICallback.h"
#include "CMainMenu.h"

#include "CServerList.h"
#include "CMasterList.h"
#include "CServerQuery.h"
#include "CServerPassword.h"

#include "CNetworkModule.h"

#include "MessageIdentifiers.h"

#include "SharedUtility.h"

#include "CServerBrowser.h"

bool CServerBrowser::Event_QuickConnectSubmitClick ( CGUIElement_Impl * pElement )
{
	m_strServerIP = m_pQuickConnectAddress->GetText ();
	m_iServerPort = m_pQuickConnectPort->GetText().ToInteger ();
	m_strServerPassword = m_pQuickConnectPassword->GetText ();

	if (m_strServerIP.GetLength() > 0 && m_iServerPort > 0){

		if ( m_iServerPort == 0 )
			m_iServerPort = DEFAULT_PORT;

		StartConnection ();

		m_pQuickConnectWindow->SetVisible ( false );

		CVAR_SET ( "last-server-ip", m_strServerIP.Get () );
		CVAR_SET ( "last-server-port", m_iServerPort );
		CVAR_SET ( "last-server-password", m_strServerPassword.Get () );
		return (true);
	}
	else {
		return (false);
	}
}

bool CServerBrowser::Event_QuickConnectCancelClick ( CGUIElement_Impl * pElement )
{
	m_pQuickConnectAddress->SetText ( "" );
	m_pQuickConnectPort->SetText ( "" );
	m_pQuickConnectPassword->SetText ( "" );
	m_pQuickConnectWindow->SetVisible ( false );
	return true;
}

bool CServerBrowser::Event_QuickConnectInputFocus ( CGUIElement_Impl * pElement )
{
	pElement->SetText ( "" );
	return true;
}

void CServerBrowser::ServerPasswordHandler ( String strPassword, void * pUserData )
{
	CServerBrowser * pServerBrowser = reinterpret_cast<CServerBrowser *>(pUserData);
	if ( pServerBrowser && pServerBrowser->m_pSelectedServer )
	{
		pServerBrowser->m_pServerPassword->SetVisible ( false );
		pServerBrowser->m_strServerIP = pServerBrowser->m_pSelectedServer->strHost;
		pServerBrowser->m_iServerPort = pServerBrowser->m_pSelectedServer->usGamePort;
		pServerBrowser->m_strServerPassword = strPassword;
		pServerBrowser->StartConnection ();
		pServerBrowser->m_pSelectedServer = nullptr;
	}
}

CServerBrowser::CServerBrowser( CGUI_Impl * pGUI )
	: m_pGUI(pGUI)
	, m_pMasterList(nullptr)
	, m_pServerQuery(nullptr)

	, m_ulLastRefreshTime(0)

	, m_pWindow(nullptr)
	, m_pTabs(nullptr)
	, m_pTab()
	, m_pServerGridList()
	, m_pPlayersGridList(nullptr)
	, m_pPlayers(nullptr)
	, m_pPlayersTab(nullptr)
	, m_pRulesTab(nullptr)
	, m_pRefresh(nullptr)
	, m_pConnect(nullptr)
	, m_pFavourite(nullptr)
	, m_pLockedImage(nullptr)
	, m_pStatusLabel(nullptr)

	, m_iTotalServers(0)
	, m_iTotalPlayers(0)
	, m_iAvailableSlots(0)

	, m_pQuickConnectWindow(nullptr)
	, m_pQuickConnectLabel(nullptr)
	, m_pQuickConnectAddress(nullptr)
	, m_pQuickConnectPort(nullptr)
	, m_pQuickConnectPassword(nullptr)
	, m_pQuickConnectSubmit(nullptr)
	, m_pQuickConnectCancel(nullptr)

	, m_pServerList()

	, m_connectionState(CONNECTION_STATE_NONE)
	, m_strServerIP()
	, m_iServerPort(0)
	, m_strServerPassword()
	, m_ulFailedTime(0)
	, m_iSelectedServer()

	, m_pMessageBox(nullptr)

	, m_pServerPassword(nullptr)
	, m_pSelectedServer(nullptr)
{
	for (size_t i = 0; i < MAX_SERVER_LISTS; ++i) {
		m_pTab[i] = nullptr;
		m_pServerGridList[i] = nullptr;
		m_pServerList[i] = nullptr;
		m_iSelectedServer[i] = 0;
	}

	m_pMasterList = new CMasterList( Event_MasterListQueryHandler );
	m_pServerQuery = new CServerQuery;
	m_pServerPassword = new CServerPassword ( pGUI );
	m_pServerPassword->SetSubmitHandler ( ServerPasswordHandler, this );
}

CServerBrowser::~CServerBrowser( void )
{
	SAFE_DELETE ( m_pServerPassword );
	SAFE_DELETE ( m_pServerQuery );
	SAFE_DELETE( m_pMasterList );

	DeleteTab( INTERNET );
	DeleteTab( FAVOURITES );
	DeleteTab( HISTORY );
}

void CServerBrowser::Pulse( void )
{
	if ( m_pServerList [ GetCurrentServerBrowserType () ] )
		m_pServerList [ GetCurrentServerBrowserType () ]->Pulse ();

	if( m_connectionState == CONNECTION_STATE_WAITING_FOR_GAME )
	{
		if( CCore::Instance()->IsGameLoaded() )
		{
			m_connectionState = CONNECTION_STATE_CONNECTING;
			ProcessConnection();
		}
	}
	else if( m_connectionState == CONNECTION_STATE_FAILED )
	{
		if( (SharedUtility::GetTime() - m_ulFailedTime) >= 3000 )
		{
			m_pMessageBox->SetVisible ( false );
			CCore::Instance()->GetGUI()->GetMainMenu()->SetVisible(true);
			SetConnectionState( CONNECTION_STATE_NONE );
			m_ulFailedTime = 0;
		}
	}
}

void CServerBrowser::Button1ClickHandler ( void * pUserData )
{
	CServerBrowser * pServerBrowser = reinterpret_cast<CServerBrowser *>(pUserData);
	if ( pServerBrowser )
		pServerBrowser->GetMessageBox()->SetVisible ( false );
}

void CServerBrowser::SetupUI( float fX, float fY, float fWidth, float fHeight )
{
	m_pTabs = m_pGUI->CreateTabPanel();
	m_pTabs->SetPosition( Vector2( fX, fY ) );
	m_pTabs->SetSize( Vector2( (fWidth - 260), fHeight ) );
	m_pTabs->SetAlwaysOnTop( true );

	m_pPlayers = m_pGUI->CreateTabPanel();
	m_pPlayers->SetPosition( Vector2( ((fX + fWidth) - 250), fY ) );
	m_pPlayers->SetSize( Vector2( 250, fHeight ) );
	m_pPlayers->SetAlwaysOnTop( true );

	m_pStatusLabel = m_pGUI->CreateLabel ( "", m_pGUI->GetDefaultFont( true ).get() );
	m_pStatusLabel->SetSize ( Vector2 ( (fWidth - 260), 30 ) );
	m_pStatusLabel->SetPosition ( Vector2 ( fX, (fY + fHeight + 10) ) );

	m_pPlayersTab = m_pPlayers->CreateTab( "Players" );
	m_pRulesTab = m_pPlayers->CreateTab( "Rules" );

	m_pPlayersGridList = m_pGUI->CreateGridList( m_pPlayersTab.get() );
	m_pPlayersGridList->SetPosition( Vector2( 1, 1 ) );
	m_pPlayersGridList->SetSize( Vector2( 247, (fHeight - 26) ) );
	m_pPlayersGridList->AddColumn ( "Name", 0.60f );
	m_pPlayersGridList->AddColumn ( "Ping", 0.30f );

	m_pLockedImage = m_pGUI->CreateStaticImage();
	m_pLockedImage->SetVisible( false );
	m_pLockedImage->LoadFromFile( "locked.png", SharedUtility::GetAbsolutePath( "data//gui//images" ) );
	m_pLockedImage->SetSize ( Vector2 ( 12, 12 ) );

	CreateTab( INTERNET, "Internet", (fWidth - 262), (fHeight - 26) );
	CreateTab( LAN, "LAN", (fWidth - 262), (fHeight - 26) );

	SetVisible( false );

	m_pMessageBox = m_pGUI->CreateMessageBox ( "", "", "Close" );
	m_pMessageBox->SetVisible ( false );
	m_pMessageBox->GetWindow()->SetAlwaysOnTop ( true );
	m_pMessageBox->SetButtonClickHandler ( 0, Button1ClickHandler, this );

	m_pQuickConnectWindow = m_pGUI->CreateWnd ( "Quick Connect" );
	m_pQuickConnectWindow->SetSize ( Vector2 ( 260, 155 ) );
	m_pQuickConnectWindow->SetPosition ( Vector2 ( (m_pGUI->GetResolution().fX / 2) - 150, (m_pGUI->GetResolution().fY / 2) - 77.5 ) );
	m_pQuickConnectWindow->SetVisible ( false );
	m_pQuickConnectWindow->SetAlwaysOnTop ( true );
	m_pQuickConnectWindow->SetSizingEnabled ( false );
	m_pQuickConnectWindow->SetAlpha ( 1.0f );

	m_pQuickConnectLabel = m_pGUI->CreateLabel ( "Enter the server IP, Port and Password:", NULL, m_pQuickConnectWindow.get() );
	m_pQuickConnectLabel->SetSize ( Vector2 ( 200, 20 ) );
	m_pQuickConnectLabel->SetPosition ( Vector2 ( 20, 25 ) );

	bool bRestoreInformation = (strlen(CVAR_GET_STRING("last-server-ip")) != 0);

	m_pQuickConnectAddress = m_pGUI->CreateEdit ( (bRestoreInformation ? CVAR_GET_STRING ( "last-server-ip" ) : "127.0.0.1"), m_pQuickConnectWindow.get() );
	m_pQuickConnectAddress->SetSize ( Vector2( 160, 24 ) );
	m_pQuickConnectAddress->SetPosition ( Vector2 ( 18, 50 ) );
	m_pQuickConnectAddress->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );

	String port;
	port.Format ( "%d", bRestoreInformation ? CVAR_GET_INTEGER ( "last-server-port" ) : DEFAULT_PORT );
	m_pQuickConnectPort = m_pGUI->CreateEdit ( port, m_pQuickConnectWindow.get() );
	m_pQuickConnectPort->SetSize ( Vector2( 55, 24 ) );
	m_pQuickConnectPort->SetPosition ( Vector2 ( 183, 50 ) );
	m_pQuickConnectPort->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );

	m_pQuickConnectPassword = m_pGUI->CreateEdit ( (bRestoreInformation ? CVAR_GET_STRING ( "last-server-password" ) : ""), m_pQuickConnectWindow.get() );
	m_pQuickConnectPassword->SetSize ( Vector2( 220, 24 ) );
	m_pQuickConnectPassword->SetPosition ( Vector2( 18, 85 ) );
	m_pQuickConnectPassword->SetClickHandler ( GUI_CALLBACK ( &CServerBrowser::Event_QuickConnectInputFocus, this ) );
	m_pQuickConnectPassword->SetMasked ( true );

	m_pQuickConnectSubmit = m_pGUI->CreateButton ( "Connect", m_pQuickConnectWindow.get() );
	m_pQuickConnectSubmit->SetSize ( Vector2 ( 75, 20 ) );
	m_pQuickConnectSubmit->SetPosition ( Vector2 ( 18, 120 ) );
	m_pQuickConnectSubmit->SetClickHandler ( GUI_CALLBACK( &CServerBrowser::Event_QuickConnectSubmitClick, this ) );

	m_pQuickConnectCancel = m_pGUI->CreateButton ( "Cancel", m_pQuickConnectWindow.get() );
	m_pQuickConnectCancel->SetSize ( Vector2 ( 75, 20 ) );
	m_pQuickConnectCancel->SetPosition ( Vector2 ( 100, 120 ) );
	m_pQuickConnectCancel->SetClickHandler ( GUI_CALLBACK( &CServerBrowser::Event_QuickConnectCancelClick, this ) );
}

void CServerBrowser::CreateTab( ServerBrowserType type, const char * szName, float fWidth, float fHeight )
{
	std::shared_ptr<CGUITab_Impl> pTab = m_pTabs->CreateTab( szName );

	m_pTab[ type ] = pTab;

	m_pServerGridList[ type ] = m_pGUI->CreateGridList( pTab.get() );
	m_pServerGridList[ type ]->SetPosition( Vector2( 1, 1 ) );
	m_pServerGridList[ type ]->SetSize( Vector2( fWidth, fHeight ) );
	m_pServerGridList[ type ]->SetClickHandler( GUI_CALLBACK( &CServerBrowser::Event_OnMouseClick, this ) );
	m_pServerGridList[ type ]->SetDoubleClickHandler( GUI_CALLBACK( &CServerBrowser::Event_OnMouseDoubleClick, this));
	m_pServerGridList[ type ]->AddColumn( "", 0.02f );
	m_pServerGridList[ type ]->AddColumn( "Hostname", 0.45f );
	m_pServerGridList[ type ]->AddColumn( "Players", 0.07f );
	m_pServerGridList[ type ]->AddColumn( "Gamemode", 0.26f );
	m_pServerGridList[ type ]->AddColumn( "Ping", 0.04f );
	m_pServerGridList[ type ]->AddColumn( "Host", 0.08f );
	m_pServerGridList[ type ]->AddColumn( "Port", 0.04f );
	m_pServerList[ type ] = new CServerList( type, m_pServerQuery );
}

void CServerBrowser::DeleteTab( ServerBrowserType type )
{
	assert(type < MAX_SERVER_LISTS);

	SAFE_DELETE( m_pServerList[ type ] );

	m_pServerGridList[ type ].reset();
	m_pTab[ type ].reset();
}

void CServerBrowser::AddServer( ServerBrowserType type, CServerListItem * pServer )
{
	int iRow = m_pServerGridList[ type ]->AddRow ();

	pServer->iRow = iRow;
	if( pServer->bPassworded )
		m_pServerGridList[ type ]->SetItemImage( iRow, 1, m_pLockedImage.get() );

	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 2, pServer->strHostname );
	String players;
	players.Format( "%d/%d", pServer->usPlayers, pServer->usMaxPlayers );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 3, players, true );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 4, pServer->strGamemode );
	String ping;
	ping.Format( "%d", pServer->usPing );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 5, ping, true );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 6, pServer->strHost );
	String port;
	port.Format( "%d", pServer->usGamePort );
	iRow = m_pServerGridList[ type ]->SetItemText( iRow, 7, port, true );

	m_iTotalServers++;
	m_iTotalPlayers += pServer->usPlayers;
	m_iAvailableSlots += (pServer->usMaxPlayers - pServer->usPlayers);

	String status;
	status.Format ( "%d players playing on %d servers. (%d slots available)", m_iTotalPlayers, m_iTotalServers, m_iAvailableSlots );
	m_pStatusLabel->SetText ( status );
}

void CServerBrowser::ConnectToSelectedServer( void )
{
	ServerBrowserType type = GetCurrentServerBrowserType();
	m_strServerPassword.clear ();

	if( m_pServerGridList[ type ]->GetSelectedCount() > 0 )
	{
		int iSelectedRow = m_pServerGridList[ type ]->GetSelectedItemRow();

		char* szIp = m_pServerGridList[ type ]->GetItemText( iSelectedRow, 6 );
		int iPort = atoi( m_pServerGridList[ type ]->GetItemText( iSelectedRow, 7 ) );

		m_pSelectedServer = m_pServerList[ type ]->GetItem ( szIp, iPort );
	}
	if ( m_pSelectedServer )
	{
		if ( m_pSelectedServer->bPassworded )
		{
			m_pServerPassword->SetVisible ( true );
		}
		else
		{
			m_strServerIP = m_pSelectedServer->strHost;
			m_iServerPort = m_pSelectedServer->usGamePort;

			StartConnection ();

			m_pSelectedServer = NULL;
		}
	}
}

void CServerBrowser::StartConnection ( void )
{
	if( CCore::Instance()->GetNetworkModule() && CCore::Instance()->GetNetworkModule()->IsConnected() )
	{
		CCore::Instance()->GetNetworkModule()->Disconnect();
		CCore::Instance()->StopMultiplayer();

		SetConnectionState( CONNECTION_STATE_CONNECTING );

		CCore::Instance()->StartMultiplayer();
		ProcessConnection ();
	}

	if( !CCore::Instance()->IsGameLoaded() )
	{
		SetConnectionState( CONNECTION_STATE_WAITING_FOR_GAME );
		SetMessageBox ( "Connecting", "Waiting for game to load..." );
	}
	else
	{
		if (!CCore::Instance()->IsMultiplayerStarted()) {
			CCore::Instance()->StartMultiplayer();
		}

		m_connectionState = CONNECTION_STATE_CONNECTING;
		ProcessConnection ();
	}
}

void CServerBrowser::ProcessConnection( void )
{
	CNetworkModule *pNetworkModule = CCore::Instance()->GetNetworkModule();
	if (pNetworkModule->GetNetworkState() == NETSTATE_CONNECTING) {
		return;
	}

	eNetworkResponse response = pNetworkModule->Connect(m_strServerIP, m_iServerPort, m_strServerPassword);

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

	SetMessageBox ( (response == NETWORK_RESPONSE_SUCCESS ? "Connecting" : "Failed to connect"), strMessage.Get () );

	if( (int)response > 0 )
	{
		SetConnectionState( CONNECTION_STATE_FAILED );

		m_ulFailedTime = SharedUtility::GetTime();
	}
}

bool CServerBrowser::Event_OnMouseClick( CGUIElement_Impl * pElement )
{
	CServerBrowser * pInst = CCore::Instance()->GetGUI()->GetServerBrowser();
	if( pElement == pInst->GetServerGridList( ServerBrowserType::INTERNET ) || pElement == pInst->GetServerGridList( ServerBrowserType::LAN ))
	{
		pInst->OnClick( pElement );
		return true;
	}

	return false;
}

bool CServerBrowser::Event_OnMouseDoubleClick( CGUIElement_Impl * pElement )
{
	CServerBrowser * pInst = CCore::Instance()->GetGUI()->GetServerBrowser();
	if (pElement == pInst->GetServerGridList(ServerBrowserType::INTERNET) || pElement == pInst->GetServerGridList(ServerBrowserType::LAN))
	{
		pInst->OnDoubleClick(pElement);
		return true;
	}

	return false;
}

void CServerBrowser::Event_MasterListQueryHandler( const std::vector<String>& servers )
{
	CServerList * pList = CCore::Instance()->GetGUI()->GetServerBrowser()->GetServerList(CCore::Instance()->GetGUI()->GetServerBrowser()->GetCurrentServerBrowserType());

	for ( auto server : servers )
	{
		if ( server.GetLength() > 1 )
			pList->Add( server );
	}
}

void CServerBrowser::Refresh( void )
{
	ServerBrowserType type = GetCurrentServerBrowserType();

	m_ulLastRefreshTime = SharedUtility::GetTime();
	m_pServerList[ type ]->Clear ();
	m_pServerGridList[ type ]->Clear ();
	m_pPlayersGridList->Clear ();

	CCore::Instance()->GetGUI()->GetMainMenu()->SetConnectButtonVisible(false);

	m_iTotalServers = m_iTotalPlayers = m_iAvailableSlots = 0;
	m_pStatusLabel->SetText ( "" );
	if( type == ServerBrowserType::INTERNET)
	{
		m_pMasterList->Refresh (E_REFRESH_NONE);
	}
	/*else if ( type == ServerBrowserType::LAN )
	{
		//Todo : Refresh from LAN
	}
	else if( type == ServerBrowserType::FAVOURITES )
	{
		// Refresh the favourites
		RefreshFromFile( "data//browser//favourites.xml" );
	}
	else if( type == ServerBrowserType::HISTORY )
	{
		// Refresh the history
		RefreshFromFile( "data//browser//history.xml" );
	}*/
}

void CServerBrowser::OnClick( CGUIElement_Impl * pElement )
{
	ServerBrowserType type = GetCurrentServerBrowserType();
	m_pPlayersGridList->Clear();

	if( m_pServerGridList[ type ]->GetSelectedCount() > 0 )
	{
		CCore::Instance()->GetGUI()->GetMainMenu()->SetConnectButtonVisible(true);

		int iSelectedIndex = m_pServerGridList[ type ]->GetSelectedItemRow();

		CServerList * pServerList = GetServerList( type );
		CServerListIterator iter, begin = pServerList->IteratorBegin(), end = pServerList->IteratorEnd();
		for( iter = begin; iter != end; iter++ )
		{
			CServerListItem * pCurrentServer = *iter;
			if( pCurrentServer && pCurrentServer->iRow == iSelectedIndex )
			{
				for( std::map< std::string, unsigned short >::iterator iter2 = pCurrentServer->m_players.begin(); iter2 != pCurrentServer->m_players.end(); iter2++ )
				{
					int iRow =  m_pPlayersGridList->AddRow();
					iRow = m_pPlayersGridList->SetItemText ( iRow, 1, (*iter2).first.c_str () );
					String ping;
					ping.Format ( "%u", (*iter2).second );
					iRow = m_pPlayersGridList->SetItemText ( iRow, 2, ping, true );
				}
			}
		}
		m_iSelectedServer[ type ] = iSelectedIndex;
	}
	else
	{
		CCore::Instance()->GetGUI()->GetMainMenu()->SetConnectButtonVisible(false);
	}
}

void CServerBrowser::OnDoubleClick( CGUIElement_Impl * pElement )
{
	ServerBrowserType type = GetCurrentServerBrowserType();

	if (m_pServerGridList[type]->GetSelectedCount() > 0)
	{
		int iSelectedIndex = m_pServerGridList[type]->GetSelectedItemRow();

		CServerList * pServerList = GetServerList(type);
		CServerListIterator iter, begin = pServerList->IteratorBegin(), end = pServerList->IteratorEnd();

		for (iter = begin; iter != end; iter++)
		{
			CServerListItem * pCurrentServer = *iter;

			if (pCurrentServer && pCurrentServer->iRow == iSelectedIndex)
			{
				ConnectToSelectedServer();
			}
		}
	}
}

void CServerBrowser::SetVisible( bool bVisible )
{
	m_pTabs->SetVisible( bVisible );
	m_pTabs->BringToFront();
	m_pPlayers->SetVisible( bVisible );
	m_pPlayers->BringToFront();
	m_pStatusLabel->SetVisible ( bVisible );
	if( bVisible )
	{
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
	if( !SharedUtility::Exists( szFile ) )
		return;

	CXML * pFile = new CXML( szFile );
	CXMLNode * pRootNode = pFile->GetRootNode();
	if( pRootNode )
	{
		String strBuffer;
		CXMLNode * pCurrentNode = NULL;
		unsigned int uiChildCount = pRootNode->GetChildCount();

		for( unsigned int i = 0; i < uiChildCount; i++ )
		{
			pCurrentNode = pRootNode->GetNode( i );
			if( !pCurrentNode )
				continue;

			if( strcmp( pCurrentNode->GetName(), "server" ) )
				continue;

			String nodeVal;
			nodeVal.Format ( "%s|", pCurrentNode->GetValue() );
			strBuffer += nodeVal;
		}
		Event_MasterListQueryHandler( strBuffer.split( '|' ) );
	}
	pFile->Save();
	SAFE_DELETE( pFile );
}

CServerList * CServerBrowser::GetServerList( ServerBrowserType type )
{
	return m_pServerList[ type ];
}

ServerBrowserType CServerBrowser::GetCurrentServerBrowserType( void ) const
{
	CGUITab_Impl *pSelectedTab = m_pTabs->GetSelectedTab();
	for (size_t i = 0; i < MAX_SERVER_LISTS; ++i) {
		if (pSelectedTab == m_pTab[i].get()) {
			return static_cast<ServerBrowserType>(i);
		}
	}
	return INTERNET;
}

void CServerBrowser::ProcessNetworkPacket( DefaultMessageIDTypes packet )
{
	if( packet == ID_CONNECTION_REQUEST_ACCEPTED )
	{
		CCore::Instance()->SetHost(m_strServerIP);
		CCore::Instance()->SetPort(m_iServerPort);
		CCore::Instance()->GetGUI()->SetCursorVisible(false);
		ChatBox::Instance()->SetVisible(true);

		m_pMessageBox->SetVisible ( false );
		return;
	}

	// After that, it's called because of disconnection

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
	CServerBrowser::SetDisconnectReason(true, strMessage);
	CCore::Instance()->GetGUI()->GetMainMenu()->SetVisible(true);
}

void CServerBrowser::SetDisconnectReason( bool bDisconnect, const char * szReason, ... )
{
	CCore *pCore = CCore::Instance();

	pCore->GetGUI()->DeleteAllClientScriptGUI();
	if( bDisconnect )
	{
		pCore->StopMultiplayer();
		pCore->StartMultiplayer();
	}

	char szBuffer[ 128 ];
	va_list args;
	va_start( args, szReason );
	vsnprintf( szBuffer, 128, szReason, args );
	va_end( args );

	SetMessageBox ( "Disconnected from the server", szReason );
	SetConnectionState( CONNECTION_STATE_FAILED );

	m_ulFailedTime = SharedUtility::GetTime();
	if( bDisconnect )
		pCore->GetNetworkModule()->Disconnect();
}

void CServerBrowser::SetMessageBox ( const char * szTitle, const char * szCaption )
{
	m_pMessageBox->SetTitle ( szTitle );
	m_pMessageBox->SetCaption ( szCaption );
	m_pMessageBox->SetVisible ( true );
	m_pMessageBox->GetWindow()->BringToFront ();
}

void CServerBrowser::OnScreenSizeChange ( float fX, float fY )
{
	float fWidth = (fX - 80.0f);
	float fHeight = (fY - 240.0f);

	m_pTabs->SetSize ( Vector2 ( (fWidth - 160.0f), fHeight ) );

	Vector2 pos = m_pPlayers->GetPosition();
	m_pPlayers->SetPosition ( Vector2 ( (40.0f + fWidth) - 150.0f, pos.fY ) );
	m_pPlayers->SetSize ( Vector2 ( 150.0f, fHeight ) );

	m_pServerGridList[ ServerBrowserType::INTERNET ]->SetSize( Vector2( (fWidth - 162.0f), (fHeight - 26.0f) ) );
	m_pServerGridList[ ServerBrowserType::LAN ]->SetSize( Vector2( (fWidth - 162.0f), (fHeight - 26.0f) ) );
	m_pPlayersGridList->SetSize( Vector2( 147.0f, (fHeight - 26.0f) ) );

	m_pServerPassword->OnScreenSizeChange ( fX, fY );

	m_pQuickConnectWindow->SetPosition ( Vector2 ( (fX / 2) - 150.0f, (fY / 2) - 95.0f ) );

	m_pStatusLabel->SetPosition ( Vector2 ( (fX - 160.0f), (fHeight + 10.0f) ) );
}