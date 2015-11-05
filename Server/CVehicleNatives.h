/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CVehicleNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"StdInc.h"

class CVehicleNatives
{

private:

	static	SQInteger		Create( SQVM * pVM );
	static	SQInteger		Destroy( SQVM * pVM );

	static	SQInteger		SetRespawnTime ( SQVM * pVM );
	static	SQInteger		GetRespawnTime ( SQVM * pVM );

	static	SQInteger		RespawnVehicle ( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};