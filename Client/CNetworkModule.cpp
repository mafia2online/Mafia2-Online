/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
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

	// Register the network RPC's
	CNetworkRPC::Register( m_pRPC );

	// Register the scripting RPC's
	CScriptingRPC::Register( m_pRPC );

	// Set the network state
	SetNetworkState( NETSTATE_NONE );

	// Reset the last connection try
	m_uiLastConnectionTry = (unsigned int)SharedUtility::GetTime ();
}

CNetworkModule::~CNetworkModule( void )
{
	// Are we connected?
	if( IsConnected() )
	{
		// Disconnect from the network
		Disconnect();
	}

	// Unregister the network RPC's
	CNetworkRPC::Unregister( m_pRPC );

	// Unregister the scripting RPC's
	CScriptingRPC::Unregister( m_pRPC );

	// Detach RPC4 from RakPeerInterface
	m_pRakPeer->DetachPlugin( m_pRPC );

	// Destroy the RPC4 instance
	RakNet::RPC4::DestroyInstance( m_pRPC );

	// Destroy the RakPeerInterface instance
	RakNet::RakPeerInterface::DestroyInstance( m_pRakPeer );
}

bool CNetworkModule::Startup( void )
{
	return ( m_pRakPeer->Startup( 1, &RakNet::SocketDescriptor(), 1, THREAD_PRIORITY_NORMAL ) == RakNet::RAKNET_STARTED );
}

void CNetworkModule::Shutdown( void )
{
	// Are we connected?
	if( IsConnected() )
		Disconnect( false );

	// Shutdown rakpeer
	m_pRakPeer->Shutdown( 500, 0, HIGH_PRIORITY );
}

eNetworkResponse CNetworkModule::Connect( String strHost, unsigned short usPort, String strPass )
{
	// Are we already connected?
	if( IsConnected() )
	{
		// Disconnect
		Disconnect();
	}

	// Store the connection info
	SetLastConnection( strHost, usPort, strPass );

	// Attempt to connect
	int iConnectionResult = m_pRakPeer->Connect( strHost.Get(), usPort, strPass.Get(), strPass.GetLength() );

	// Set the network state
	SetNetworkState( NETSTATE_CONNECTING );

	// Did we fail to connect?
	if( iConnectionResult != 0 )
	{
		// Set the network state
		SetNetworkState( NETSTATE_NONE );

		// Set the last connection try
		m_uiLastConnectionTry = (unsigned int)SharedUtility::GetTime();
	}

	return (eNetworkResponse)iConnectionResult;
}

void CNetworkModule::Disconnect( bool bRestart )
{
	// Are we not connected?
	if( !IsConnected() )
		return;

	// Close the connection
	m_pRakPeer->CloseConnection( RakNet::UNASSIGNED_SYSTEM_ADDRESS, true );

	// Set the network state
	SetNetworkState( NETSTATE_DISCONNECTED );

	// Shutdown raknet
	Shutdown();

	// Should we restart raknet?
	if( bRestart )
	{
		// Start up raknet again
		Startup();

		// Reset default server info
		pCore->SetServerName( "M2-MP Server" );
		pCore->SetServerMaxPlayers( 0 );

		// Clear the chat
		pCore->GetChat()->Clear ();
		pCore->GetChat()->ClearHistory ();

		// Reset player model
		//pCore->GetPlayerManager()->GetLocalPlayer()->SetModel ( 10 );
	}
}

void CNetworkModule::Pulse( void )
{
	// Is the game not loaded?
	if( !pCore->IsGameLoaded() )
		return;

	// Are we disconnected from the network?
	if( GetNetworkState() == NETSTATE_DISCONNECTED )
		return;

	// Process the network
	UpdateNetwork ();

	// Are we connected?
	if( IsConnected() )
	{
		// Pulse the player manager
		pCore->GetPlayerManager()->Pulse ();

		// Pulse the vehicle manager
		pCore->GetVehicleManager()->Pulse ();
	}
}

void CNetworkModule::Call( const char * szIdentifier, RakNet::BitStream * pBitStream, PacketPriority priority, PacketReliability reliability, bool bBroadCast )
{
	// Are we not connected to a server?
	if( !IsConnected() )
		return;

	// Pass it to RPC4
	m_pRPC->Call( szIdentifier, pBitStream, priority, reliability, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, bBroadCast );
}

void CNetworkModule::UpdateNetwork( void )
{
	// Create a packet
	RakNet::Packet * pPacket = NULL;

	//
	bool bDisconnect = false;

	// Process RakNet
	while( pPacket = m_pRakPeer->Receive() )
	{
		// Is the packet invalid?
		if ( !pPacket )
			continue;

		// Have we connected?
		if( pPacket->data[0] == ID_CONNECTION_REQUEST_ACCEPTED )
		{
			// Call the connection accepted handler
			ConnectionAccepted( pPacket );

			// Process this packet with the server browser
			pCore->GetGUI()->GetServerBrowser()->ProcessNetworkPacket( (DefaultMessageIDTypes)pPacket->data[0] );
		}
		else if( pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST ||
			 pPacket->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS || pPacket->data[0] == ID_INVALID_PASSWORD ||
			 pPacket->data[0] == ID_CONNECTION_BANNED || pPacket->data[0] == ID_CONNECTION_ATTEMPT_FAILED || pPacket->data[0] == ID_ALREADY_CONNECTED )
		{
			// Did we timeout?
			if( pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST )
			{
				// Stop multiplayer
				pCore->StopMultiplayer ();

				// Start multiplayer
				pCore->StartMultiplayer ();
			}

			// Set the network state
			SetNetworkState( NETSTATE_NONE );

			// Process this packet with the server browser
			pCore->GetGUI()->GetServerBrowser()->ProcessNetworkPacket( (DefaultMessageIDTypes)pPacket->data[0] );
		}

		// Deallocate the memory used by the packet
		m_pRakPeer->DeallocatePacket( pPacket );

		// Should we disconnect?
		if ( bDisconnect )
		{
			// Restart raknet
			Disconnect ();

			//
			bDisconnect = false;
		}
	}
}

void CNetworkModule::ConnectionAccepted( RakNet::Packet * pPacket )
{
	// Set the network state
	SetNetworkState( NETSTATE_CONNECTED );

	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the network version
	pBitStream.Write( NETWORK_VERSION );

	// Write the player nickname
	pBitStream.Write( RakNet::RakString( pCore->GetNick().Get() ) );

	// Write the player serial
	pBitStream.Write( RakNet::RakString( SharedUtility::GetSerialHash().Get() ) );
		
	// Send to the server
	Call( RPC_INITIAL_DATA, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );
}