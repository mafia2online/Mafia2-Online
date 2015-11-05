/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CMasterList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore			* pCore;

void CMasterList::WorkerThread ( CThread * pCreator )
{
	// Is the thread active?
	while( pCreator->GetUserData < bool > () )
	{
		// Get the masterlist pointer
		CMasterList * pMasterList = pCore->GetMasterList ();

		// Is the masterlist pointer valid and time for an update?
		if( pMasterList && pMasterList->IsAdded() && ((SharedUtility::GetTime() - pMasterList->GetLastUpdateTime()) > 5000) )
		{
			// Get the tcp interface
			RakNet::TCPInterface * tcp = pMasterList->GetTCPInterface ();

			// Connect to the website
			tcp->Connect ( "master.m2-multiplayer.com", 80, true );

			// Wait for the connection
			RakSleep ( 100 );

			//
			RakNet::RakString post;

			// Switch the thread state
			switch( pMasterList->GetThreadState() )
			{
			case E_STATE_ADD:
				{
					// Generate the string for RakNet
					if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
						post = RakNet::RakString::FormatForGET ( RakNet::RakString ( "master.m2-multiplayer.com/query.php?type=add&port=%d&ip=%s", pMasterList->GetServerPort(), CVAR_GET_STRING ( "serverip" ) ), RakNet::RakString ( "User-Agent: Mafia2Multiplayer/1.0" ) );
					else
						post = RakNet::RakString::FormatForGET ( RakNet::RakString ( "master.m2-multiplayer.com/query.php?type=add&port=%d", pMasterList->GetServerPort() ), RakNet::RakString ( "User-Agent: Mafia2Multiplayer/1.0" ) );
					break;
				}

			case E_STATE_UPDATE:
				{
					// Generate the string for RakNet
					if ( strlen ( CVAR_GET_STRING ( "serverip" ) ) > 0 )
						post = RakNet::RakString::FormatForGET ( RakNet::RakString ( "master.m2-multiplayer.com/query.php?type=pong&port=%dplayers=%d&ip=%s", pMasterList->GetServerPort(), pCore->GetPlayerManager()->GetCount(), CVAR_GET_STRING ( "serverip" ) ), RakNet::RakString ( "User-Agent: Mafia2Multiplayer/1.0" ) );
					else
						post = RakNet::RakString::FormatForGET ( RakNet::RakString ( "master.m2-multiplayer.com/query.php?type=pong&port=%d&players=%d", pMasterList->GetServerPort(), pCore->GetPlayerManager()->GetCount() ), RakNet::RakString ( "User-Agent: Mafia2Multiplayer/1.0" ) );
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
					//
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
	m_workerThread.SetUserData< bool > ( true );
	m_workerThread.Start ( WorkerThread );
}

CMasterList::~CMasterList( void )
{
	// Stop the worker thread
	m_workerThread.SetUserData< bool > ( false );
	m_workerThread.Stop ( false, true );

	// Stop the TCP interface
	tcp->Stop ();

	// Delete the TCP interface
	SAFE_DELETE( tcp );
}

// todo: REMOVE
void CMasterList::Pulse ( void )
{
}