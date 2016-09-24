/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedPlayerNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CScriptingManager;

class CSharedPlayerNatives
{

private:

	static	SQInteger		GetMoney( SQVM * pVM );
	static	SQInteger		GetPlayers( SQVM * pVM );
	static	SQInteger		GetVehicles( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};