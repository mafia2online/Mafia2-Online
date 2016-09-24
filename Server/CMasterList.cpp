/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CMasterList.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	<thread>

#include "StdInc.h"
#include "CCore.h"

#include "CMasterList.h"

void CMasterList::WorkerThread ()
{
	while(m_processQueries)
	{
		if( IsAdded() && ((SharedUtility::GetTime() - GetLastUpdateTime()) > 5000) )
		{
			RakNet::TCPInterface * tcp = GetTCPInterface ();
			tcp->Connect ( MASTERLIST_HOST, 80, true );
			RakSleep ( 100 );
			RakNet::RakString post;

			switch( GetThreadState() )
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
			tcp->Send ( post.C_String(), post.GetLength(), tcp->HasCompletedConnectionAttempt(), false );
			RakSleep( 1000 );
			RakNet::Packet * pPacket = tcp->Receive ();
			if( pPacket )
			{
				if( GetThreadState() == E_STATE_ADD )
				{
					CLogFile::Print ( "[network] Server successfully posted to the masterlist." );
					SetThreadState ( E_STATE_UPDATE );
				}
				tcp->DeallocatePacket ( pPacket );
			}
			SetLastUpdateTime ( SharedUtility::GetTime () );
		}
		Sleep( 100 );
	}
}

CMasterList::CMasterList()
{
	SetServerPort( CVAR_GET_INTEGER( "port" ) );
	SetAuthKey(CVAR_GET_STRING("serverkey"));
	SetLastUpdateTime ( 0 );
	SetThreadState ( E_STATE_ADD );

	m_bIsAdded = true;
	tcp = RakNet::OP_NEW< RakNet::TCPInterface >( __FILE__, __LINE__ );
	tcp->Start ( 0, 64 );
	if (strlen(CVAR_GET_STRING("serverkey")) > 0){
		m_processQueries = true;
		m_thread = std::thread(&CMasterList::WorkerThread, this);
	}
}

CMasterList::~CMasterList()
{
	if (m_thread.joinable()){
		m_processQueries = false;
		m_thread.join();
	}
	tcp->Stop ();
	SAFE_DELETE( tcp );
}