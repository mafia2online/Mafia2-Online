/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCameraNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CCameraNatives
{

private:

	static	SQInteger		GetPosition( SQVM * pVM );
	static	SQInteger		GetRotation( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};

