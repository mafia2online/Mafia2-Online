/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CMasterList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CCore.h"

void CMasterList::WorkerThread ( CThread * pCreator )
{
	// Is the thread active?
	while( pCreator->GetUserData < bool > () )
	{
		// Get the masterlist pointer
		CMasterList * pMasterList = CCore::Instance()->GetMasterList ();

		// Is the masterlist pointer valid and time for an update?
		if( pMasterList && pMasterList->IsAdded() && ((SharedUtility::GetTime() - pMasterList->GetLastUpdateTime()) > 5000) )
		{
			// Get the tcp interface
			RakNet::TCPInterface * tcp = pMasterList->GetTCPInterface ();

			// Connect to the website
			tcp->Connect ( MASTERLIST_HOST, 80, true );

			// Wait for the connection
			RakSleep ( 100 );

			//
			RakNet::RakString post;

			// Switch the thread state
			switch( pMasterList->GetThreadState() )
			{
				case E_STATE_ADD:
				{
					post = RakNet::RakString::FormatForPOST(RakNet::RakString(MASTERLIST_HOST"/api/v1/server/update"),
						RakNet::RakString("application/x-www-form-urlencoded"),
						RakNet::RakString("key=%s&players=%d&maxplayers=%d", CVAR_GET_STRING("serverkey"), CCore::Instance()->GetPlayerManager()->GetCount(), CVAR_GET_INTEGER("maxplayers")), 
						RakNet::RakString("User-Agent: Mafia2Online/1.0"));
					break;
				}

				case E_STATE_UPDATE:
				{
					post = RakNet::RakString::FormatForPOST(RakNet::RakString(MASTERLIST_HOST"/api/v1/server/update"),
						RakNet::RakString("application/x-www-form-urlencoded"),
						RakNet::RakString("key=%s&players=%d&maxplayers=%d", CVAR_GET_STRING("serverkey"), CCore::Instance()->GetPlayerManager()->GetCount(), CVAR_GET_INTEGER("maxplayers")), 
						RakNet::RakString("User-Agent: Mafia2Online/1.0"));
					break;
				}
			}
			// Send the data to the website
			tcp->Send ( post.C_String(), post.GetLength(), tcp->HasCompletedConnectionAttempt(), false );

			// Wait for the result
			RakSleep( 1000 );

			// Get the return packet
			RakNet::Packet * pPacket = tcp->Receive ();

			// Is the packet valid?
			if( pPacket )
			{
				// Have we just added the server?
				if( pMasterList->GetThreadState() == E_STATE_ADD )
				{
					CLogFile::Print ( "[network] Server successfully posted to the masterlist." );

					// Set the thread state
					pMasterList->SetThreadState ( E_STATE_UPDATE );
				}

				// Deallocate the packet memory
				tcp->DeallocatePacket ( pPacket );
			}

			// Set the last update time
			pMasterList->SetLastUpdateTime ( SharedUtility::GetTime () );
		}

		// Sleep
		Sleep( 100 );
	}
}

CMasterList::CMasterList( void )
{
	// Set the server port
	SetServerPort( CVAR_GET_INTEGER( "port" ) );

	// Set the auth key
	SetAuthKey(CVAR_GET_STRING("serverkey"));

	// Reset the last update time
	SetLastUpdateTime ( 0 );

	// Set the thread state
	SetThreadState ( E_STATE_ADD );

	// Mark as added
	m_bIsAdded = true;

	// Create the RakNet TCP interface
	tcp = RakNet::OP_NEW< RakNet::TCPInterface >( __FILE__, __LINE__ );

	// Start the TCP interface
	tcp->Start ( 0, 64 );

	// Create the worker thread
	if (strlen(CVAR_GET_STRING("serverkey")) > 0){
		m_workerThread.SetUserData< bool >(true);
		m_workerThread.Start(WorkerThread);
	}
}

CMasterList::~CMasterList( void )
{
	// Stop the worker thread
	if (m_workerThread.IsRunning()){
		m_workerThread.SetUserData< bool > ( false );
		m_workerThread.Stop ( false, true );
	}
	// Stop the TCP interface
	tcp->Stop ();

	// Delete the TCP interface
	SAFE_DELETE( tcp );
}