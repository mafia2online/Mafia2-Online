/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedPlayerNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CSharedPlayerNatives
{

private:

	static	SQInteger		GetMoney( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};