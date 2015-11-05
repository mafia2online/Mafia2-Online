/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CCoreInterface.h
*
***************************************************************/

#pragma once

#include "InterfaceCommon.h"

class CCoreInterface
{
public:
	virtual	CPlayerManagerInterface			* GetPlayerManager( void ) = 0;
	virtual CBlipManagerInterface			* GetBlipManager( void ) = 0;
	virtual CVehicleManagerInterface		* GetVehicleManager( void ) = 0;
	virtual CEventsInterface				* GetEventManager( void ) = 0;
	virtual CBanManagerInterface			* GetBanManager( void ) = 0;
	virtual CResourceManagerInterface		* GetResourceManager( void ) = 0;
};