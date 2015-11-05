/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CUtilNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CUtilNatives
{

private:

	static	SQInteger		PlayerIdFromName( SQVM * pVM );
	//static	SQInteger		WeaponNameFromId( SQVM * pVM );
	//static	SQInteger		WeaponIdFromName( SQVM * pVM );
	static	SQInteger		ToRGBA( SQVM * pVM );
	static	SQInteger		FromRGB( SQVM * pVM );

	static	SQInteger		Clamp( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};