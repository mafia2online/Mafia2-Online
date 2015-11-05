/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerPlayer.h
*
***************************************************************/

#pragma once

class CMultiplayerVehicle;
class CMultiplayerPlayer
{

public:

	virtual	unsigned int					GetId							( void ) = 0;

	virtual const char						* GetNick						( void ) = 0;

	virtual	void							SetColour						( unsigned int uiColour ) = 0;
	virtual	unsigned int					GetColour						( void ) = 0;

	virtual	const char						* GetSerial						( void ) = 0;

	virtual unsigned short					GetPing							( void ) = 0;

	virtual	bool							IsDead							( void ) = 0;

	virtual	void							SetModel						( unsigned int uiModel ) = 0;
	virtual	unsigned int					GetModel						( void ) = 0;

	virtual	void							SetHealth						( float fHealth ) = 0;
	virtual float							GetHealth						( void ) = 0;

	virtual	void							GiveWeapon						( int iWeapon, int iAmmo ) = 0;
	virtual	void							RemoveWeapon					( int iWeapon, int iAmmo ) = 0;
	virtual int								GetWeapon						( void ) = 0;

	virtual	void							Kick							( void ) = 0;
	
	virtual	CMultiplayerVehicle				* GetVehicle					( void ) = 0;
	virtual	bool							IsInVehicle						( void ) = 0;
	virtual	unsigned int					GetSeat							( void ) = 0;

};