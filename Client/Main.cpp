/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : Main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CNetworkModule.h"
#include "RakPeerInterface.h"

#include "CLogFile.h"

CCore*		pCore = NULL;
HMODULE		g_hModule = NULL;

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, void * pReserved )
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		{
			g_hModule = hModule;
			DisableThreadLibraryCalls( hModule );

			pCore = new CCore;
			if( !pCore || !pCore->Initialise() )
				TerminateProcess( GetCurrentProcess(), 0 );
			break;
		}

	case DLL_PROCESS_DETACH:
		{
			DEBUG_LOG ( "DLL_PROCESS_DETACH" );
			if (pCore && CCore::Instance()->GetNetworkModule() && CCore::Instance()->GetNetworkModule()->GetRakPeer())
			{
				DEBUG_LOG ( "Shutting down the network..." );
				CCore::Instance()->GetNetworkModule()->GetRakPeer()->Shutdown( 500 );
			}
			DEBUG_LOG ( "Terminating process" );
			TerminateProcess( GetCurrentProcess(), 0 );

			break;
		}
	}
	return TRUE;
}