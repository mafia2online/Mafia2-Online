/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CEventNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CEventNatives
{

private:

	static	SQInteger		AddEvent( SQVM * pVM );
	static	SQInteger		RemoveEvent( SQVM * pVM );
	static	SQInteger		CallEvent( SQVM * pVM );

	static	SQInteger		AddCommand( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};