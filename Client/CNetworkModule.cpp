/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModule.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include	<RPC4Plugin.h>
#include "RakPeerInterface.h"
#include "CNetworkRPC.h"
#include "CScriptingRPC.h"

#include "SharedUtility.h"

#include "CPlayerManager.h"
#include "CVehicleManager.h"

#include "CGUI.h"
#include "CServerBrowser.h"

#include "Network/NetRPCs.h"

#include "MessageIdentifiers.h"

#include "CNetworkModule.h"

RakNet::RPC4			* CNetworkModule::m_pRPC = NULL;

CNetworkModule::CNetworkModule( void )
	: m_pRakPeer(NULL)
	, m_eNetworkState(NETSTATE_NONE)
	, m_uiLastConnectionTry((unsigned int)SharedUtility::GetTime ())

	, m_strIp("127.0.0.1")
	, m_usPort(DEFAULT_PORT)
	, m_strPass()

	, m_bRestartAfterDisconnect(false)
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
}

CNetworkModule::~CNetworkModule( void )
{
	// Are we connected?
	if( IsConnected() )
	{
		// Disconnect from the network
		DoDisconnect(false);
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
	if ( IsConnected() )
	{
		// Forcefully disconnect.
		DoDisconnect();
	}

	// Store the connection info
	SetLastConnection( strHost, usPort, strPass );

	// Attempt to connect
	int iConnectionResult = m_pRakPeer->Connect( strHost.Get(), usPort, strPass.Get(), strPass.GetLength() );

	if (iConnectionResult == RakNet::CONNECTION_ATTEMPT_STARTED )
	{
		SetNetworkState( NETSTATE_CONNECTING );
	}
	else
	{
		SetNetworkState( NETSTATE_NONE );
		m_uiLastConnectionTry = (unsigned int)SharedUtility::GetTime();
	}

	return (eNetworkResponse)iConnectionResult;
}

/**
 * Schedule disconnect to be done after all packets are received.
 *
 * Due to the crash problem caused by calling Disconnect when packets are processed (RPC)
 * we need to do "deferred" disconnect as shutting down RakPeer during the packets processing
 * result in double-packet data removal (one in the RPC handler via RakPeer shutdown
 * and one after RPC plugin stops processing the packet).
 *
 * @param bRestart Restart RakNet after doing disconnect? (So we can connect later again?)
 */
void CNetworkModule::Disconnect( bool bRestart )
{
	if (!IsConnected()) {
		return;
	}

	m_bRestartAfterDisconnect = bRestart;
	SetNetworkState (NETSTATE_DISCONNECTING);
}

void CNetworkModule::Pulse( void )
{
	// Is the game not loaded?
	if( !CCore::Instance()->IsGameLoaded() )
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
		CCore::Instance()->GetPlayerManager()->Pulse ();

		// Pulse the vehicle manager
		CCore::Instance()->GetVehicleManager()->Pulse ();
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

void CNetworkModule::DoDisconnect( bool bRestart )
{
	// Delete all the clientscript gui elements
	CCore::Instance()->GetGUI()->DeleteAllClientScriptGUI();

	// Close the connection
	m_pRakPeer->CloseConnection( RakNet::UNASSIGNED_SYSTEM_ADDRESS, true );

	// Set the network state
	SetNetworkState( NETSTATE_DISCONNECTED );

	// Shutdown raknet
	Shutdown();

	if( bRestart )
	{
		// Start up raknet again
		Startup();

		// Reset default server info
		CCore::Instance()->SetServerName( "M2Online Server" );
		CCore::Instance()->SetServerMaxPlayers( 0 );

		// Clear the chat
		ChatBox::Instance()->Clear ();

		// Reset player model
		//pCore->GetPlayerManager()->GetLocalPlayer()->SetModel ( 10 );
	}
}

void CNetworkModule::UpdateNetwork( void )
{
	// Create a packet
	RakNet::Packet * pPacket = NULL;

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
			CCore::Instance()->GetGUI()->GetServerBrowser()->ProcessNetworkPacket( (DefaultMessageIDTypes)pPacket->data[0] );
		}
		else if( pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST ||
			 pPacket->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS || pPacket->data[0] == ID_INVALID_PASSWORD ||
			 pPacket->data[0] == ID_CONNECTION_BANNED || pPacket->data[0] == ID_CONNECTION_ATTEMPT_FAILED || pPacket->data[0] == ID_ALREADY_CONNECTED )
		{
			// Did we timeout?
			if( pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST )
			{
				// Delete all the clientscript gui elements
				CCore::Instance()->GetGUI()->DeleteAllClientScriptGUI();

				// Stop multiplayer
				CCore::Instance()->StopMultiplayer ();

				// Start multiplayer
				CCore::Instance()->StartMultiplayer ();
			}

			// Set the network state
			SetNetworkState( NETSTATE_NONE );

			// Process this packet with the server browser
			CCore::Instance()->GetGUI()->GetServerBrowser()->ProcessNetworkPacket( (DefaultMessageIDTypes)pPacket->data[0] );
		}

		// Deallocate the memory used by the packet
		m_pRakPeer->DeallocatePacket( pPacket );
	}

	if (m_eNetworkState == NETSTATE_DISCONNECTING)
	{
		DoDisconnect(m_bRestartAfterDisconnect);
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
	pBitStream.Write( RakNet::RakString( CCore::Instance()->GetNick().Get() ) );

	// Write the player serial
	pBitStream.Write( RakNet::RakString( SharedUtility::GetSerialHash().Get() ) );

	// Send to the server
	Call( RPC_INITIAL_DATA, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true );
}