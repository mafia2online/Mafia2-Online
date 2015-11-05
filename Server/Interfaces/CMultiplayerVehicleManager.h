/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CMultiplayerVehicleManager.h
*
***************************************************************/

#pragma once

class CMultiplayerVehicleManager
{

public:

	virtual	bool							Remove							( unsigned int uiVehicleId ) = 0;
	virtual	bool							IsActive						( unsigned int uiVehicleId ) = 0;

	virtual	CMultiplayerVehicle				* Get							( unsigned int uiVehicleId ) = 0;
	virtual	unsigned int					GetCount						( void ) = 0;

};