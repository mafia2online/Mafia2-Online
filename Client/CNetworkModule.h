/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModule.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

enum eNetworkResponse
{
	NETWORK_RESPONSE_SUCCESS = 0,
	NETWORK_RESPONSE_INVALID_PARAM = 1,
	NETWORK_RESPONSE_CANT_RESOLVE_DOMAIN = 2,
	NETWORK_RESPONSE_ALREADY_CONNECTED = 3,
	NETWORK_RESPONSE_ALREADY_CONNECTING = 4,
	NETWORK_RESPONSE_SECURITY_FAILED = 5,
	NETWORK_RESPONSE_NO_HOST_SET = 6
};

class CNetworkModule
{

private:

	RakNet::RakPeerInterface				* m_pRakPeer;
	static RakNet::RPC4						* m_pRPC;

	eNetworkState							m_eNetworkState;
	unsigned int							m_uiLastConnectionTry;

	String									m_strIp;
	unsigned short							m_usPort;
	String									m_strPass;

	void									UpdateNetwork					( void );
	void									ConnectionAccepted				( RakNet::Packet * pPacket );

public:

	CNetworkModule( void );
	~CNetworkModule( void );

	bool									Startup							( void );
	void									Shutdown						( void );
	eNetworkResponse						Connect							( String strIp, unsigned short usPort, String strPass = "" );
	void									Disconnect						( bool bRestart = true );
	bool									IsConnected						( void ) { return (m_eNetworkState == NETSTATE_CONNECTED || m_eNetworkState == NETSTATE_CONNECTING); }

	void									SetNetworkState					( eNetworkState netState ) { m_eNetworkState = netState; }
	eNetworkState							GetNetworkState					( void ) { return m_eNetworkState; }

	void									SetLastConnection				( String strIp, unsigned short usPort, String strPass ) { m_strIp = strIp; m_usPort = usPort; m_strPass = strPass; }

	void									Pulse							( void );

	void									Call							( const char * szIdentifier, RakNet::BitStream * pBitStream, PacketPriority priority, PacketReliability reliability, bool bBroadCast );

	RakNet::RakPeerInterface				* GetRakPeer					( void ) { return m_pRakPeer; }
	static RakNet::RPC4						* GetRPC						( void ) { return m_pRPC; }

};