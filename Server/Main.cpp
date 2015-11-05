/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : Main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CCore					* pCore = NULL;
CThread					inputThread;
CMutex					consoleInputQueueMutex;
std::queue<String>		consoleInputQueue;
unsigned long			m_ulStartTime;

void					Shutdown( void );

#ifdef WIN32
BOOL WINAPI EventHandler( DWORD dwEvent )
{
	switch( dwEvent )
	{
	case CTRL_CLOSE_EVENT: case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT:
		{
			Shutdown();
			break;
		}
	}
	return TRUE;
}
#else
void SignalHandler( int iSignal )
{
    switch( iSignal )
    {
        case 1:
        {
            Shutdown();
            break;
        }
    }
}
#endif

void ConsoleInput( String strInput )
{
	// Is the input empty?
	if( strInput.IsEmpty() )
		return;

	size_t s = strInput.Find( " ", 0 );
	String strCommand = strInput.substr( 0, s++ );
	String strParams = strInput.substr( s, (strInput.GetLength() - s) );

	// Is there no space?
	if( s == 0 )
	{
		// Reset the params string
		strParams.Truncate( 0 );
	}

	if( strCommand == "say" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: say <message>" );
			return;
		}

		// Construct a new bitstream
		RakNet::BitStream pBitStream;
		
		// Write the input
		pBitStream.Write( RakNet::RakString( "Server: %s", strParams.Get() ) );

		// Send it to all clients
		pCore->GetNetworkModule()->Call( RPC_SENDPLAYERMESSAGE, &pBitStream, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, true );

		CLogFile::Printf( "[chat] Server: %s", strParams.Get() );
	}
	else if( strCommand == "exit" || strCommand == "quit" || strCommand == "shutdown" || strCommand == "terminate" )
	{
		// Shutdown the server
		pCore->SetActive( false );
	}
	else if( strCommand == "start" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: start <resource>" );
			return;
		}

		// Load the resource given
		if( pCore->GetResourceManager()->StartResource( strParams ) )
		{
			CLogFile::Printf( "Successfully started resource '%s'.", strParams.Get() );
		}
	}
	else if( strCommand == "stop" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: stop <resource>" );
			return;
		}

		// Stop the resource given
		if( pCore->GetResourceManager()->StopResource( strParams ) )
		{
			CLogFile::Printf( "Successfully stopped resource '%s'.", strParams.Get() );
		}
	}
	else if( strCommand == "restart" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: restart <resource>" );
			return;
		}

		// Stop the given resource
		if( pCore->GetResourceManager()->StopResource( strParams ) )
		{
			// Start the given resource
			pCore->GetResourceManager()->StartResource( strParams );

			CLogFile::Printf( "Successfully restarted resource '%s'.", strParams.Get() );
		}
	}
	else if( strCommand == "resources" )
	{
		CLogFile::Print( "==============================================================" );
		CLogFile::Print( "= List of running resources" );

		// Print a list of running resources
		pCore->GetResourceManager()->GetRunningResources();

		CLogFile::Print( "==============================================================" );
	}
	/*else if( strCommand == "netstats" || strCommand == "network" )
	{
		CLogFile::Print( "==============================================================" );

		// Print the network stats
		CNetworkStats::GetStats ();

		CLogFile::Print( "==============================================================" );
	}*/
	else if( strCommand == "uptime" )
	{
		CLogFile::Print( "==============================================================" );

		// Print the server uptime
		CLogFile::Printf( "= %s.", SharedUtility::GetTimePassedFromTime( m_ulStartTime ).Get() );

		CLogFile::Print( "==============================================================" );
	}
	else
	{
		// Unhandled input
		CSquirrelArguments pArguments;
		pArguments.push( strCommand );
		pArguments.push( strParams );
		pCore->GetEvents()->Call( "onConsoleInput", &pArguments );
	}
}

void InputThread( CThread * pCreator )
{
	char szInputBuffer[512];
	String strInputString;

	// Loop until server shutdown
	while( pCreator->GetUserData<bool>() )
	{
		// Wait for input from the console
		fgets( szInputBuffer, sizeof(szInputBuffer), stdin );

		// Do we have any input?
		if( szInputBuffer[0] != '\n' )
		{
			// Append the input to the input string
			strInputString += szInputBuffer;

			// Get the input string length
			size_t sLength = strInputString.GetLength();

			// is this the last of the input?
			if( strInputString[sLength - 1] == '\n' )
			{
				// Check for CRLF
				if( strInputString[sLength - 2] == '\r' )
					strInputString.Truncate( sLength - 2 );
				else
					strInputString.Truncate( sLength - 1 );

				// Add the input to the console input queue
				consoleInputQueueMutex.Lock( );
				consoleInputQueue.push( strInputString );
				consoleInputQueueMutex.Unlock( );

				// Clear the input string
				strInputString.clear( );
			}
		}

		// Sleep
		Sleep( 10 );
	}
}

int main( int argc, char * argv[] )
{
	// Create the core instance
	pCore = new CCore;

	// Did the core fail to create or initialsie?
	if( !pCore || !pCore->Initialise( argc, argv ) )
	{
		// Terminate the process
#ifdef _WIN32
		TerminateProcess( GetCurrentProcess(), 0 );
#else
		_exit(0);
#endif
	}

	// Set the event handler
#ifdef _WIN32
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)EventHandler, TRUE );
#else
	for( int i = 0; i < 15; i++ )
	{
		signal( i, SignalHandler );
	}
#endif

	// Start the input thread
	inputThread.SetUserData<bool>( true );
	inputThread.Start( InputThread );

	// Set the start time
	m_ulStartTime = SharedUtility::GetTime();

	// Loop while the server is active
	while( pCore->IsActive() )
	{
		// Pulse the core
		pCore->Pulse( );

		// Try and lock the console input queue mutex
		if( consoleInputQueueMutex.TryLock(0) )
		{
			// Process the console input queue
			while( !consoleInputQueue.empty() )
			{
				// Send the console input data
				ConsoleInput( consoleInputQueue.back().GetData() );

				// Clear the queue
				consoleInputQueue.pop( );
			}

			// Unlock the input queue mutex
			consoleInputQueueMutex.Unlock();
		}

		// Sleep
		Sleep( 5 );
	}

	// Shutdown
	Shutdown ();

	return 0;
}

void Shutdown( void )
{
	// Stop the input thread
	inputThread.SetUserData<bool>( false );
	inputThread.Stop( false, true );

	// Log the event
	CLogFile::Printf( "Server shutting down..." );

	// Delete the core
	SAFE_DELETE( pCore );

#ifdef WIN32
	TerminateProcess( GetCurrentProcess(), 0 );
#endif
}
