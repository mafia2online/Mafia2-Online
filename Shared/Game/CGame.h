/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CGame.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CString.h"

namespace Game
{

	unsigned int		GetIdFromPlayerModel			( String strModel );
	void				GetPlayerModelFromId			( unsigned int uiModel, String * strModel, String * strDirectroy );

	unsigned int		GetIdFromVehicleModel			( String strModel );
	void				GetVehicleModelFromId			( unsigned int uiModel, String * strModel );

	unsigned int		GetIdFromVehicleWheelModel		( String strModel );
	String				GetVehicleWheelModelFromId		( unsigned int uiModel );

};