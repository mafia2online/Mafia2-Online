/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CNetworkVehicleInterface.h
*
***************************************************************/

#pragma once

class CNetworkVehicleInterface
{
public:
	virtual void							SetModel( int iModel ) = 0;
	virtual int								GetModel( void ) = 0;
	virtual void							SetSpawnPosition( CVector3 vecPosition ) = 0;
	virtual void							GetSpawnPosition( CVector3 * vecPosition ) = 0;
	virtual void							SetSpawnDirection( CVector3 vecDirection ) = 0;
	virtual void							GetSpawnDirection( CVector3 * vecDirection ) = 0;
	virtual void							SetPosition( CVector3 vecPosition, bool bBroadcast ) = 0;
	virtual void							GetPosition( CVector3 * vecPosition ) = 0;
	virtual void							SetDirection( CVector3 vecDirection, bool bBroadcast ) = 0;
	virtual void							GetDirection( CVector3 * vecDirection ) = 0;
	virtual void							SetColour( unsigned char ucColour1, unsigned char ucColour2, bool bBroadcast ) = 0;
	virtual void							GetColour( unsigned char * ucColour1, unsigned char * ucColour2 ) = 0;
};