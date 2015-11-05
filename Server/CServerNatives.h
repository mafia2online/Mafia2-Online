#ifndef C_SERVER_NATIVES_H
#define C_SERVER_NATIVES_H
/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CServerNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"StdInc.h"

class CServerNatives
{

private:

	static	SQInteger		SetGameModeText( SQVM * pVM );
	static	SQInteger		SetMapName( SQVM * pVM );
	static	SQInteger		SetWeather( SQVM * pVM );
	static	SQInteger		SetSummer( SQVM * pVM );
	static	SQInteger		GetNetworkStats( SQVM * pVM );
	static	SQInteger		GetWhoWasList( SQVM * pVM );
	static	SQInteger		ClearWhoWas( SQVM * pVM );
	static	SQInteger		GetWhoWasCount( SQVM * pVM );
	static	SQInteger		Shutdown( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};
#endif
