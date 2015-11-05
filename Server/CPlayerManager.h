/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPlayerManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPlayerManager : public CMultiplayerPlayerManager
{

private:

	CNetworkPlayer													* m_pNetworkPlayer[ MAX_PLAYERS ];

public:

								CPlayerManager						( void );
								~CPlayerManager						( void );

	bool						Add									( EntityId playerId, const char * szNick, const char * szIP, const char * szSerial );
	void						Remove								( EntityId playerId, unsigned int uiReason );
	bool						IsActive							( EntityId playerId );

	void						Pulse								( void );

	CNetworkPlayer				* Get								( EntityId playerId ) { return m_pNetworkPlayer[ playerId ]; }
	EntityId					GetCount							( void );

	void						HandlePlayerJoin					( EntityId playerId );
	void						HandlePlayerSpawn					( EntityId playerId );

	bool						IsNickInUse							( const char * szNick );
	CNetworkPlayer				* GetPlayerFromSerial				( const char * szSerial );

};