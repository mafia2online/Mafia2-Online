/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CClientNatives
{

private:

	static	SQInteger		TriggerServerEvent					( SQVM * pVM );

	static	SQInteger		BindKey								( SQVM * pVM );
	static	SQInteger		UnbindKey							( SQVM * pVM );
	static	SQInteger		IsKeyBound							( SQVM * pVM );

public:

	static	void			Register							( CScriptingManager * pScriptingManager );

};