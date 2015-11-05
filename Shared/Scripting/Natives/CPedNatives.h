/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CPedNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once
#ifdef _CLIENT

#include	"../CScriptingManager.h"

class CPedNatives
{

private:

	static	SQInteger		CreatePed					( SQVM * pVM );
	static	SQInteger		DestroyPed					( SQVM * pVM );

public:

	static	void			Register					( CScriptingManager * pScriptingManager );

};
#endif
