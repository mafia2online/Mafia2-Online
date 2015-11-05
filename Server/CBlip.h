/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBlip.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CBlip : public CMultiplayerBlip
{

private:

	EntityId						m_blipId;
	Vector2							m_vecPosition;
	int								m_iLibrary;
	int								m_iIcon;

	CNetworkPlayer					 * m_pAttachedToPlayer;
	CNetworkVehicle					* m_pAttachedToVehicle;
	CPed							* m_pAttachedToPed;

	eBlipType						m_blipType;

public:

									CBlip						( EntityId blipId, Vector2 vecPosition, int iLibrary, int iIcon );
									~CBlip						( void );

	void							AddForPlayer				( EntityId playerId );
	void							AddForWorld					( void );

	void							RemoveForPlayer				( EntityId playerId );
	void							RemoveForWorld				( void );

	EntityId						GetId						( void ) { return m_blipId; }
	Vector2							GetPosition					( void ) { return m_vecPosition; }
	int								GetLibrary					( void ) { return m_iLibrary; }
	int								GetIcon						( void ) { return m_iIcon; }
	eBlipType						GetType						( void ) { return m_blipType; }

	void							AttachToPlayer				( CNetworkPlayer * pPlayer );
	void							AttachToVehicle				( CNetworkVehicle * pVehicle );
	void							AttachToPed					( CPed * pPed );

};