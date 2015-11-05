/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CBlipNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CBlipNatives
{

private:

	static	SQInteger		Create( SQVM * pVM );
	static	SQInteger		Destroy( SQVM * pVM );

	static	SQInteger		AttachToPlayer ( SQVM * pVM );
	static	SQInteger		AttachToVehicle ( SQVM * pVM );
	static	SQInteger		AttachToPed ( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};