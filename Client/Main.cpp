/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : Main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"
#include	"CExceptionHandler.h"

#include	"CNetworkModule.h"
#include	<RakPeerInterface.h>

CCore*		pCore = NULL;
HMODULE		g_hModule = NULL;

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, void * pReserved )
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		{
			
			// Store the hmodule pointer
			g_hModule = hModule;

			// Disable thread library calls
			DisableThreadLibraryCalls( hModule );

			// Install the exception handler
			CExceptionHandler::Install();
			
			// Create the core instance
			pCore = new CCore;

			// Did the core fail to create or initialsie?
			if( !pCore || !pCore->Initialise() )
				TerminateProcess( GetCurrentProcess(), 0 );


			break;
		}

	case DLL_PROCESS_DETACH:
		{
			CLogFile::Print( "DLL_PROCESS_DETACH" );

			// Is the core instance valid?
			if (pCore && CCore::Instance()->GetNetworkModule() && CCore::Instance()->GetNetworkModule()->GetRakPeer())
			{
				CLogFile::Print( "Shutting down the network..." );

				// Shutdown the network
				CCore::Instance()->GetNetworkModule()->GetRakPeer()->Shutdown( 500 );
			}

			CLogFile::Print( "Terminating process" );

			// Terminate the process
			TerminateProcess( GetCurrentProcess(), 0 );

			break;
		}
	}

	return TRUE;
}