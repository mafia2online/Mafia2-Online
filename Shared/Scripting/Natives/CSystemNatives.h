/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSystemNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CSystemNatives
{

private:

	static	SQInteger		Log( SQVM * pVM );
	static	SQInteger		Time( SQVM * pVM );
	static	SQInteger		TickCount( SQVM * pVM );
	static	SQInteger		Date( SQVM * pVM );
	static	SQInteger		PlayerCount( SQVM * pVM );
	static	SQInteger		MaxPlayers( SQVM * pVM );
	static	SQInteger		ServerName( SQVM * pVM );
	static	SQInteger		StrRand( SQVM * pVM );
	static	SQInteger		IntConcat( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};