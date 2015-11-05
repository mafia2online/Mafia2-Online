/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CPlayerNatives
{

private:

	static	SQInteger		GetLocalPlayer( SQVM * pVM );
	static	SQInteger		GetName( SQVM * pVM );
	static	SQInteger		GetPing( SQVM * pVM );
	static	SQInteger		GetColour( SQVM * pVM );
	static	SQInteger		IsPlayerConnected( SQVM * pVM );
	static	SQInteger		IsOnScreen( SQVM * pVM );
	static	SQInteger		IsSpawned( SQVM * pVM );
	static	SQInteger		GetPosition( SQVM * pVM );
	static	SQInteger		GetRotation( SQVM * pVM );
	static	SQInteger		GetHealth( SQVM * pVM );
	static	SQInteger		ToggleControls( SQVM * pVM );
	static	SQInteger		IsInVehicle( SQVM * pVM );
	static	SQInteger		GetVehicle( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};