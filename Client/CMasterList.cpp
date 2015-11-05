/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMasterList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

#define MASTER_SERVER_HOST "master.m2-multiplayer.com"
#define MASTER_SERVER_NONE_URL_PATH "/list.php"
#define MASTER_SERVER_HOSTED_URL_PATH "/list.php?hosted"

extern CCore *pCore;

/*static*/ bool DummyReceiveHandler( const char *, unsigned int, void * )
{
	return true;
}

// TODO: refactor this
void CMasterList::WorkerThread( eRefreshType type )
{
	// Loop until end
	while ( true )
	{
		// Is the http client busy?
		if ( m_pHttpClient->IsBusy() )
		{
			// Process the http client
			m_pHttpClient->Process();

			// Have we done downloading the data?
			if ( m_pHttpClient->GotData() )
			{
				// Get the data
				String * strData = m_pHttpClient->GetData();

				// Do we have any data?
				if( !strData->IsEmpty() )
				{
					OnRefreshSuccess( strData );
				}
				else
				{
					// Raise an error if we got an empty response
					OnRefreshFailed( "Failed to get masterlist from server! (empty response)" );
				}

				// Go to ready state
				m_refreshState = RefreshState_Ready;
			}
		}
		else
		{
			// Should we send the request?
			if ( m_refreshState == RefreshState_UpdateRequired )
			{
				// Create the path string
				String strPath( MASTER_SERVER_NONE_URL_PATH );

				// Are we requesting hosted tab servers?
				if ( type == eRefreshType::E_REFRESH_HOSTED )
					strPath = MASTER_SERVER_HOSTED_URL_PATH;

				// Update the last refresh time
				m_lastRefreshTime = SharedUtility::GetTime();

				// Update the state
				m_refreshState = RefreshState_InProgress;

				// Send the http request
				m_pHttpClient->Get( strPath );

				// Was there an error?
				if ( m_pHttpClient->GetLastError() != HTTP_ERROR_NONE )
				{
					OnRefreshFailed( String( "Failed to get masterlist from server!\nError: %s (%d)", 
						m_pHttpClient->GetLastErrorString().Get(), m_pHttpClient->GetLastError() ).Get() );
				}
			}
		}

		// Sleep
		Sleep( 50 );
	}
}

void CMasterList::OnRefreshFailed( const char *errorMessage )
{
	CLogFile::Printf( "ERROR - %s", errorMessage );

	// Update the messagebox
	m_pMessageBox->SetTitle( "ERROR" );
	m_pMessageBox->SetCaption( errorMessage );
	m_pMessageBox->GetWindow()->BringToFront();

	// Wait 3 seconds
	std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
	
	// Hide the message box
	m_pMessageBox->SetVisible( false );
}

#pragma message( "CMasterList.cpp - Use [slash]n in the masterlist instead of <br /> for lower data usage!" )

void CMasterList::OnRefreshSuccess( String *serverListData )
{
	// Split the servers
	std::vector<String> servers = serverListData->split( "<br />" );

	CLogFile::Printf( "Discovered %d servers. (%s)", 
		servers.size(), SharedUtility::BytesToString(serverListData->GetLength()).Get() );

	// Call our query handler
	if( m_queryHandler )
		m_queryHandler( servers );

	// Hide the message box
	m_pMessageBox->SetVisible( false );
}

CMasterList::CMasterList( QueryHandler_t handler )
	: m_queryHandler( handler )
	, m_refreshState( RefreshState_Ready )
	, m_refreshThread( nullptr )
{
	// Create the http client instance
	m_pHttpClient = new CHttpClient;

	// Set the http recieve handler
	m_pHttpClient->SetReceiveHandle( DummyReceiveHandler, this );

	// Set the http client host
	m_pHttpClient->SetHost( MASTER_SERVER_HOST );

	// Reset
	m_lastRefreshTime = SharedUtility::GetTime();
	
	// Create the messagebox
	m_pMessageBox = pCore->GetGUI()->GetCEGUI()->CreateMessageBox( "", "" );
	m_pMessageBox->SetVisible( false );
}

CMasterList::~CMasterList( void )
{
	// TODO: wait for the worker thread
	SAFE_DELETE( m_refreshThread );

	// Delete the http client instance
	SAFE_DELETE( m_pHttpClient );

	// Delete the messagebox instance
	SAFE_DELETE( m_pMessageBox );
}

bool CMasterList::Refresh( eRefreshType type )
{
	// Have we already sent a request?
	if ( m_refreshState != RefreshState_Ready )
		return false;

	// Make sure they're not spamming!
	//if( (SharedUtility::GetTime() - m_ulLastRefreshTime) > 2000 )
	//	return false;

	// Reset the http client
	m_pHttpClient->Reset();

	// Update the state
	m_refreshState = RefreshState_UpdateRequired;

	// Update the message box
	m_pMessageBox->SetTitle( "Refreshing" );
	m_pMessageBox->SetCaption( "Getting servers from masterlist..." );
	m_pMessageBox->SetVisible( true );
	m_pMessageBox->GetWindow()->SetAlwaysOnTop( true );
	m_pMessageBox->GetWindow()->BringToFront();

	if ( m_refreshThread == nullptr )
	{
		// Create the refresh thread
		m_refreshThread = new std::thread( 
			std::bind( &CMasterList::WorkerThread, this, type ) );

		// Start the refresh thread
		m_refreshThread->detach();
	}

	return true;
}