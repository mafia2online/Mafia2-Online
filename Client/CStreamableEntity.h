/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CStreamableEntity.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

enum eStreamEntityType
{
	STREAM_TYPE_PLAYER,
	STREAM_TYPE_VEHICLE,
	STREAM_TYPE_PED
};

class CM2Entity;
class CStreamableEntity
{

private:

	bool						m_bIsStreamedIn;
	float						m_fStreamingDistance;
	eStreamEntityType			m_type;
	CM2Entity					* m_pStreamEntity;

public:

								CStreamableEntity						( eStreamEntityType type, CM2Entity * pEntity, float fStreamDistance = 200.0f );
								~CStreamableEntity						( void );

	void						StreamIn								( void );
	void						StreamOut								( void );
	
	void						SetType									( eStreamEntityType type ) { m_type = type; }
	eStreamEntityType			GetType									( void ) { return m_type; }

	bool						IsStreamedIn							( void ) { return m_bIsStreamedIn; }

	void						SetStreamDistance						( float fStreamDistance ) { m_fStreamingDistance = fStreamDistance; }
	float						GetStreamDistance						( void ) { return m_fStreamingDistance; }

	CM2Entity					* GetEntity								( void ) { return m_pStreamEntity; }

};