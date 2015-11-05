/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CSquirrelInterface.h
*
***************************************************************/

#pragma once

#include "InterfaceCommon.h"

class CSquirrelInterface
{
public:
	virtual SQVM		* GetVM( void ) = 0;
};