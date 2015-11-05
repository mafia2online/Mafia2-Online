/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CNetworkPlayerInterface.h
*
***************************************************************/

#pragma once

class CNetworkPlayerInterface
{
public:
	virtual	void							SetNick( const char * szNick ) = 0;
	virtual const char						* GetNick( void ) = 0;
	virtual void							SetColour( unsigned int uiColour ) = 0;
	virtual unsigned int					GetColour( void ) = 0;
	virtual const char						* GetSerial( void ) = 0;
	virtual unsigned short					GetPing( void ) = 0;
	virtual bool							IsDead( void ) = 0;
	virtual int								GetScore( void ) = 0;
	virtual void							Kick( void ) = 0;
	virtual bool							IsInVehicle( void ) = 0;
	virtual EntityId						GetSeat( void ) = 0;
};