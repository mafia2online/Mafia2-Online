/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CHashNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CHashNatives
{

private:

	static	SQInteger		md5_string( SQVM * pVM );
	static	SQInteger		md5_file( SQVM * pVM );
	static	SQInteger		crc32( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};