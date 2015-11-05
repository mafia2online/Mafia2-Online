#ifndef C_BAN_NATIVES_H
#define C_BAN_NATIVES_H
/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBanNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"StdInc.h"

class CBanNatives
{

private:

	static	SQInteger		BanPlayer( SQVM * pVM );
	static	SQInteger		BanSerial( SQVM * pVM );
	static	SQInteger		UnbanSerial( SQVM * pVM );
	static	SQInteger		IsSerialBanned( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};
#endif
