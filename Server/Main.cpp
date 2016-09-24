/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : Main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	<thread>
#include	<mutex>
#include	<atomic>
#include	<iostream>

#include "StdInc.h"

CCore					* pCore = NULL;
std::mutex				m_inputMutex;
std::atomic<bool>		m_readInput;
std::thread				m_inputThread;
std::queue<std::string>	m_inputQueue;
unsigned long			m_ulStartTime;

void					Shutdown( void );

#ifdef WIN32
BOOL WINAPI EventHandler( DWORD dwEvent )
{
	switch( dwEvent )
	{
	case CTRL_CLOSE_EVENT: case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT:
		{
			pCore->SetActive(false);
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
			pCore->SetActive(false);
            break;
        };

        case SIGINT:
        {
        	CLogFile::Print( "Please use the 'quit' command instead of ctrl+c" );
        	break;
        };
    }
}
#endif

void ConsoleInput( String strInput )
{
	if( strInput.IsEmpty() )
		return;

	size_t s = strInput.Find( " ", 0 );
	String strCommand = strInput.substr( 0, s++ );
	String strParams = strInput.substr( s, (strInput.GetLength() - s) );

	if( s == 0 )
	{
		strParams.Truncate( 0 );
	}

	if( strCommand == "say" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: say <message>" );
			return;
		}
		RakNet::BitStream pBitStream;
		pBitStream.Write( RakNet::RakString( "Server: %s", strParams.Get() ) );
		pCore->GetNetworkModule()->Call( RPC_SENDPLAYERMESSAGE, &pBitStream, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, true );

		CLogFile::Printf( "[chat] Server: %s", strParams.Get() );
	}
	else if( strCommand == "exit" || strCommand == "quit" || strCommand == "shutdown" || strCommand == "terminate" )
	{
		pCore->SetActive( false );
	}
	else if( strCommand == "start" )
	{
		if( strParams.IsEmpty() )
		{
			CLogFile::Print( "USAGE: start <resource>" );
			return;
		}
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
		if( pCore->GetResourceManager()->StopResource( strParams ) )
		{
			pCore->GetResourceManager()->StartResource( strParams );
			CLogFile::Printf( "Successfully restarted resource '%s'.", strParams.Get() );
		}
	}
	else if( strCommand == "resources" )
	{
		CLogFile::Print( "==============================================================" );
		CLogFile::Print( "= List of running resources" );
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
		CLogFile::Printf( "= %s.", SharedUtility::GetTimePassedFromTime( m_ulStartTime ).Get() );
		CLogFile::Print( "==============================================================" );
	}
	else
	{
		CSquirrelArguments pArguments;
		pArguments.push( strCommand );
		pArguments.push( strParams );
		pCore->GetEvents()->Call( "onConsoleInput", &pArguments );
	}
}

void InputThread()
{
	std::string		input;

	while (m_readInput)
	{
		std::getline(std::cin, input);
		m_inputMutex.lock();
		if (input.size() > 0)
			m_inputQueue.push(input);
		m_inputMutex.unlock();

		Sleep(10);
	}
}

int main( int argc, char * argv[] )
{
	pCore = new CCore;

	if( !pCore || !pCore->Initialise( argc, argv ) )
	{
#ifdef _WIN32
		TerminateProcess( GetCurrentProcess(), 0 );
#else
		_exit(42);
#endif
	}

#ifdef _WIN32
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)EventHandler, TRUE );
#else
	for( int i = 0; i < 15; i++ )
	{
		signal( i, SignalHandler );
	}
#endif

	m_readInput = true;
	m_inputThread = std::thread(InputThread);

	m_ulStartTime = SharedUtility::GetTime();

	while (pCore->IsActive())
	{
		pCore->Pulse();

		if (m_inputMutex.try_lock())
		{
			while (!m_inputQueue.empty())
			{
				std::string	input = m_inputQueue.back();
				if (input.empty())
				{
					m_inputMutex.unlock();
					return (0);
				}
				ConsoleInput(String(input.c_str()));
				m_inputQueue.pop();
			}
			m_inputMutex.unlock();
		}

		Sleep(5);
	}
	Shutdown ();
	return 0;
}

void Shutdown( void )
{
	m_readInput = false;
	m_inputThread.join();

	CLogFile::Printf( "Server shutting down..." );

	SAFE_DELETE( pCore );
#ifdef WIN32
	TerminateProcess( GetCurrentProcess(), 0 );
#else
	_exit(42);
#endif
}
