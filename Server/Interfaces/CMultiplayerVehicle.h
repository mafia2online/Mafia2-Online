/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerVehicle.h
*
***************************************************************/

#pragma once

class CMultiplayerPlayer;
class CMultiplayerVehicle
{

public:

	virtual	unsigned int					GetId							( void ) = 0;

	virtual	void							SetModel						( int iModel ) = 0;
	virtual	int								GetModel						( void ) = 0;

	// get position shit

	virtual	void							SetPlateText					( const char * szPlateText ) = 0;
	virtual	const char						* GetPlateText					( void ) = 0;

	virtual	void							Repair							( void ) = 0;
	virtual	void							Explode							( void ) = 0;

	virtual	void							SetDirtLevel					( float fDirtLevel ) = 0;
	virtual	float							GetDirtLevel					( void ) = 0;

	virtual	void							SetEngineState					( bool bState ) = 0;
	virtual	bool							GetEngineState					( void ) = 0;

	virtual	void							SetSirenState					( bool bState ) = 0;
	virtual	bool							GetSirenState					( void ) = 0;

	virtual	void							SetHornState					( bool bState ) = 0;
	virtual	bool							GetHornState					( void ) = 0;

	virtual	void							SetTuningTable					( int iTable ) = 0;
	virtual	int								GetTuningTable					( void ) = 0;

	//virtual	CMultiplayerPlayer				* GetOccupant					( int iSeat ) = 0;

};