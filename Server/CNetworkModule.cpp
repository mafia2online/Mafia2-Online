/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkModule.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;
RakNet::RPC4			* CNetworkModule::m_pRPC = NULL;

CNetworkModule::CNetworkModule( void )
{
	// Get the RakPeerInterface instance
	m_pRakPeer = RakNet::RakPeerInterface::GetInstance();

	// Get the RPC4 instance
	m_pRPC = RakNet::RPC4::GetInstance();

	// Attact RPC4 to RakPeerInterface
	m_pRakPeer->AttachPlugin( m_pRPC );

	// Register the RPC's
	CNetworkRPC::Register( m_pRPC );

	// Set the network state
	SetNetworkState( NETSTATE_NONE );
}

CNetworkModule::~CNetworkModule( void )
{
	// Shutdown RakNet
	m_pRakPeer->Shutdown( 500 );

	// Unregister the RPC's
	CNetworkRPC::Unregister( m_pRPC );

	// Detach RPC4 from RakPeerInterface
	m_pRakPeer->DetachPlugin( m_pRPC );

	// Destroy the RPC4 instance
	RakNet::RPC4::DestroyInstance( m_pRPC );

	// Destroy the RakPeerInterface instance
	RakNet::RakPeerInterface::DestroyInstance( m_pRakPeer );
}

bool CNetworkModule::Startup( void )
{
	// Create the socket descriptor
	RakNet::SocketDescriptor socketDescriptor( CVAR_GET_INTEGER( "port" ), CVAR_GET_STRING( "serverip" ) );

	// Attempt to startup raknet
	bool bReturn = (m_pRakPeer->Startup( CVAR_GET_INTEGER( "maxplayers" ), &socketDescriptor, 1, THREAD_PRIORITY_NORMAL ) == RakNet::RAKNET_STARTED);

	// Did it start?
	if( bReturn )
	{
		// Set the maximum incoming connections
		m_pRakPeer->SetMaximumIncomingConnections( CVAR_GET_INTEGER( "maxplayers" ) );

		// Set the timeout time
		m_pRakPeer->SetTimeoutTime( 10000, RakNet::UNASSIGNED_SYSTEM_ADDRESS );

		// Get the password string
		String strPassword = CVAR_GET_STRING( "password" );

		// Do we have a password?
		if( strPassword.GetLength() > 0 )
		{
			// Set the server password
			m_pRakPeer->SetIncomingPassword( strPassword.Get(), strPassword.GetLength() );
		}
	}

	return bReturn;
}

void CNetworkModule::Pulse( void )
{
	// Update the network
	UpdateNetwork ();
}

void CNetworkModule::Call( const char * szIdentifier, RakNet::BitStream * pBitStream, PacketPriority priority, PacketReliability reliability, EntityId playerId, bool bBroadCast )
{
	// Pass it to RPC4
	m_pRPC->Call( szIdentifier, pBitStream, priority, reliability, 0, (playerId != INVALID_ENTITY_ID ? m_pRakPeer->GetSystemAddressFromIndex(playerId) : RakNet::UNASSIGNED_SYSTEM_ADDRESS), bBroadCast );
}

int CNetworkModule::GetPlayerPing( EntityId playerId )
{
	return m_pRakPeer->GetLastPing( m_pRakPeer->GetSystemAddressFromIndex( playerId ) );
}

void CNetworkModule::UpdateNetwork( void )
{
	// Create a packet
	RakNet::Packet * pPacket = NULL;

	// Process RakNet
	while( pPacket = m_pRakPeer->Receive() )
	{
		switch( pPacket->data[0] )
		{
		case ID_NEW_INCOMING_CONNECTION:
			{
				CLogFile::Printf( "[network] Incoming connection from %s.", pPacket->systemAddress.ToString( true, ':' ) );
				break;
			}

		case ID_DISCONNECTION_NOTIFICATION:
			{
				// Is the player active in the player manager?
				if( pCore->GetPlayerManager()->IsActive( (EntityId)pPacket->systemAddress.systemIndex ) )
				{
					// Get the network player instance
					CNetworkPlayer * pNetworkPlayer = pCore->GetPlayerManager()->Get( (EntityId)pPacket->systemAddress.systemIndex );

					// Add the player to the who was manager
					CWhoWas::Add( pNetworkPlayer->GetNick(), pNetworkPlayer->GetSerial(), pPacket->systemAddress.ToString( true, ':' ) );

					// Delete the player from the manager
					pCore->GetPlayerManager()->Remove( (EntityId)pPacket->systemAddress.systemIndex, DISCONNECT_QUIT );
				}
				break;
			}

		case ID_CONNECTION_LOST:
			{
				// Is the player active in the player manager?
				if( pCore->GetPlayerManager()->IsActive( (EntityId)pPacket->systemAddress.systemIndex ) )
				{
					// Delete the player from the manager
					pCore->GetPlayerManager()->Remove( (EntityId)pPacket->systemAddress.systemIndex, DISCONNECT_TIMEOUT );
				}
				break;
			}
		}

		// Deallocate the memory used by the packet
		m_pRakPeer->DeallocatePacket( pPacket );
	}
}