/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CHttpNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"StdInc.h"

class CHttpNatives
{

private:

	static	SQInteger		WebRequest( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};