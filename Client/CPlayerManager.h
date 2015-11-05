/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPlayerManager
{

private:

	CLocalPlayer				* m_pLocalPlayer;
	CRemotePlayer				* m_pNetworkPlayer[MAX_PLAYERS];

public:

	CPlayerManager( void );
	~CPlayerManager( void );

	bool						Add( EntityId playerId, String strNick, unsigned int uiColour );
	void						Remove( EntityId playerId );
	bool						IsActive( EntityId playerId ) { return (m_pNetworkPlayer[playerId] != NULL); }

	void						Pulse( void );

	CLocalPlayer				* GetLocalPlayer( void ) { return m_pLocalPlayer; }
	CRemotePlayer				* Get( EntityId playerId ) { return m_pNetworkPlayer[playerId]; }

	CRemotePlayer				* GetFromGameGUID( DWORD dwGUID );
	EntityId					GetIdFromGameGUID( DWORD dwGUID );

	int							GetLocalPing( bool bAverage = false );

	void						RespawnAll( void );
	void						HandleLocalDeath( void );

	EntityId					GetCount( void );

	//
	bool						IsHumanInventoryLocalPlayer( DWORD dwHumanInventory );

};