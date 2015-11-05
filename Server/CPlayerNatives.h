#ifndef C_PLAYER_NATIVES_H
#define	C_PLAYER_NATIVES_H
/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPlayerNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"StdInc.h"

class CPlayerNatives
{

private:

	static	SQInteger		IsConnected( SQVM * pVM );
	static	SQInteger		IsSpawned( SQVM * pVM );
	static	SQInteger		GetName( SQVM * pVM );
	static	SQInteger		GetSerial( SQVM * pVM );
	static	SQInteger		SetPosition( SQVM * pVM );
	static	SQInteger		GetPosition( SQVM * pVM );
	static	SQInteger		SetRotation( SQVM * pVM );
	static	SQInteger		GetRotation( SQVM * pVM );
	static	SQInteger		SetHealth( SQVM * pVM );
	static	SQInteger		GetHealth( SQVM * pVM );
	static	SQInteger		GiveWeapon( SQVM * pVM );
	static	SQInteger		GetWeapon( SQVM * pVM );
	static	SQInteger		RemoveWeapon( SQVM * pVM );
	static	SQInteger		OutputMessage( SQVM * pVM );
	static	SQInteger		OutputMessageToAll( SQVM * pVM );
	static	SQInteger		SetColour( SQVM * pVM );
	static	SQInteger		GetColour( SQVM * pVM );
	static	SQInteger		ToggleHud( SQVM * pVM );
	static	SQInteger		SetMoney( SQVM * pVM );
	static	SQInteger		GiveMoney( SQVM * pVM );
	static	SQInteger		TakeMoney( SQVM * pVM );
	static	SQInteger		GetMoney( SQVM * pVM );
	static	SQInteger		ToggleControls( SQVM * pVM );
	static	SQInteger		Kick( SQVM * pVM );
	static  SQInteger		PlaySound( SQVM * pVM );
	static  SQInteger		StopSound( SQVM * pVM );
	static  SQInteger		PlaySoundToAll( SQVM * pVM );
	static  SQInteger		StopSoundToAll( SQVM * pVM );
	static	SQInteger		GetNetworkStats( SQVM * pVM );
	static	SQInteger		TriggerClientEvent( SQVM * pVM );

	static	SQInteger		SetModel				( SQVM * pVM );
	static	SQInteger		GetModel				( SQVM * pVM );
	static	SQInteger		IsInVehicle				( SQVM * pVM );
	static	SQInteger		GetVehicle				( SQVM * pVM );
	static	SQInteger		GetPing					( SQVM * pVM );
	static	SQInteger		GetIP					( SQVM * pVM );
	static	SQInteger		PutInVehicle			( SQVM * pVM );
	static	SQInteger		RemoveFromVehicle		( SQVM * pVM );
	static	SQInteger		RespawnPlayer			( SQVM * pVM );

public:

	static	void			Register				( CScriptingManager * pScriptingManager );

};
#endif
