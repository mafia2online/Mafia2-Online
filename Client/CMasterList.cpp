/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMasterList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"

#include "CGUI.h"
#include "gui_impl/CGUI_Impl.h"
#include "gui_impl/CGUIMessageBox_Impl.h"
#include "gui_impl/CGUIWindow_Impl.h"

#include <atomic>
#include <thread>

#include "SharedUtility.h"

#include "Network/CHttpClient.h"

#include "CMasterList.h"

#include "CLogFile.h"

/*static*/ bool DummyReceiveHandler( const char *, unsigned int, void * )
{
	return true;
}

// TODO: refactor this
void CMasterList::WorkerThread()
{
	// Loop until end
	while ( !m_forceWorkerShutdown )
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
				String strPath( MASTERLIST_NONE_URL_PATH );

				// Are we requesting hosted tab servers?
				if ( m_refreshType == eRefreshType::E_REFRESH_HOSTED )
					strPath = MASTERLIST_HOSTED_URL_PATH;

				// Update the state
				m_refreshState = RefreshState_InProgress;

				// Send the http request
				m_pHttpClient->Get( strPath );

				// Was there an error?
				if ( m_pHttpClient->GetLastError() != HTTP_ERROR_NONE )
				{
					String error;
					error.Format( "Failed to get masterlist from server!\nError: %s (%d)", m_pHttpClient->GetLastErrorString().Get(), m_pHttpClient->GetLastError() );
					OnRefreshFailed( error );
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

	CLogFile::Printf( "Discovered %d servers. (%s)", servers.size(), SharedUtility::BytesToString(serverListData->GetLength()).Get() );

	// Call our query handler
	if( m_queryHandler )
		m_queryHandler( servers );

	// Hide the message box
	m_pMessageBox->SetVisible( false );
}

CMasterList::CMasterList( QueryHandler_t handler )
	: m_queryHandler( handler )
	, m_forceWorkerShutdown(false)
	, m_refreshState( RefreshState_Ready )
	, m_refreshType( E_REFRESH_NONE )

	, m_pMessageBox()
{
	// Create the http client instance
	m_pHttpClient = std::make_unique<CHttpClient>();

	// Set the http recieve handler
	m_pHttpClient->SetReceiveHandle( DummyReceiveHandler, this );

	// Set the http client host
	m_pHttpClient->SetHost( MASTERLIST_HOST );

	// Create the messagebox
	m_pMessageBox = CCore::Instance()->GetGUI()->GetCEGUI()->CreateMessageBox( "", "" );
	m_pMessageBox->SetVisible( false );

	m_refreshThread = std::thread(std::bind( &CMasterList::WorkerThread, this ));
}

CMasterList::~CMasterList( void )
{
	m_forceWorkerShutdown = true;
	m_refreshThread.join();
}

bool CMasterList::Refresh( eRefreshType type )
{
	// Have we already sent a request?
	if ( m_refreshState != RefreshState_Ready )
		return false;

	// Reset the http client
	m_pHttpClient->Reset();

	// Update the state
	m_refreshState = RefreshState_UpdateRequired;
	m_refreshType = type;

	// Update the message box
	m_pMessageBox->SetTitle( "Refreshing" );
	m_pMessageBox->SetCaption( "Getting servers from masterlist..." );
	m_pMessageBox->SetVisible( true );
	m_pMessageBox->GetWindow()->SetAlwaysOnTop( true );
	m_pMessageBox->GetWindow()->BringToFront();

	return true;
}