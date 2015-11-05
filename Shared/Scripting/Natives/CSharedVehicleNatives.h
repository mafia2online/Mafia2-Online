/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedVehicleNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CSharedVehicleNatives
{

private:

	static	SQInteger		SetPosition( SQVM * pVM );
	static	SQInteger		GetPosition( SQVM * pVM );
	static	SQInteger		SetRotation( SQVM * pVM );
	static	SQInteger		GetRotation( SQVM * pVM );
	static	SQInteger		SetColour( SQVM * pVM );
	static	SQInteger		GetColour( SQVM * pVM );
	static	SQInteger		SetPlateText( SQVM * pVM );
	static	SQInteger		GetPlateText( SQVM * pVM );
	static	SQInteger		Repair( SQVM * pVM );
	static	SQInteger		SetDirtLevel( SQVM * pVM );
	static	SQInteger		GetDirtLevel( SQVM * pVM );
	static	SQInteger		SetEngineState( SQVM * pVM );
	static	SQInteger		GetEngineState( SQVM * pVM );
	static	SQInteger		Explode( SQVM * pVM );
	static	SQInteger		SetPartOpen( SQVM * pVM );
	static	SQInteger		IsPartOpen( SQVM * pVM );
	static	SQInteger		SetSirenState( SQVM * pVM );
	static	SQInteger		GetSirenState( SQVM * pVM );
	static	SQInteger		SetHornState( SQVM * pVM );
	static	SQInteger		GetHornState( SQVM * pVM );
	static	SQInteger		SetWindowOpen( SQVM * pVM );
	static	SQInteger		IsWindowOpen( SQVM * pVM );
	static	SQInteger		SetTuningTable( SQVM * pVM );
	static	SQInteger		GetTuningTable( SQVM * pVM );
	static	SQInteger		SetWheelTexture( SQVM * pVM );
	static	SQInteger		GetWheelTexture( SQVM * pVM );
	static	SQInteger		GetModel( SQVM * pVM );
	static	SQInteger		GetVehicles( SQVM * pVM );
	static	SQInteger		SetSpeed( SQVM * pVM );
	static	SQInteger		GetSpeed( SQVM * pVM );
	static	SQInteger		SetFuel( SQVM * pVM );
	static	SQInteger		GetFuel( SQVM * pVM );
	static	SQInteger		SetLightState ( SQVM * pVM );
	static	SQInteger		GetLightState ( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};