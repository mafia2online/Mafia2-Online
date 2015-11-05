/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkModule.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkModule
{

private:

	RakNet::RakPeerInterface				* m_pRakPeer;
	static RakNet::RPC4						* m_pRPC;

	eNetworkState							m_eNetworkState;

	void									UpdateNetwork( void );

public:

	CNetworkModule( void );
	~CNetworkModule( void );

	bool									Startup( void );

	void									SetNetworkState( eNetworkState netState ) { m_eNetworkState = netState; }
	eNetworkState							GetNetworkState( void ) { return m_eNetworkState; }

	void									Pulse( void );

	void									Call( const char * szIdentifier, RakNet::BitStream * pBitStream, PacketPriority priority, PacketReliability reliability, EntityId playerId, bool bBroadCast );
	int										GetPlayerPing( EntityId playerId );

	RakNet::RakPeerInterface				* GetRakPeer( void ) { return m_pRakPeer; }
	static RakNet::RPC4						* GetRPC( void ) { return m_pRPC; }

};