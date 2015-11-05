/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGameNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGameNatives
{

private:

	static	SQInteger		SetWeather( SQVM * pVM );
	static	SQInteger		GetWeather( SQVM * pVM );
	static	SQInteger		ToggleHud( SQVM * pVM );
	static	SQInteger		TakeScreenshot( SQVM * pVM );
	static	SQInteger		FadeScreen( SQVM * pVM );
	static	SQInteger		CreateHudTimer( SQVM * pVM );
	static	SQInteger		SetHudTimerRemainingTime( SQVM * pVM );
	static	SQInteger		GetHudTimerRemainingTime( SQVM * pVM );
	static	SQInteger		StartHudTimer( SQVM * pVM );
	static	SQInteger		StopHudTimer( SQVM * pVM );
	static	SQInteger		IsHudTimerRunning( SQVM * pVM );
	static	SQInteger		DestroyHudTimer( SQVM * pVM );
	static	SQInteger		OpenMap ( SQVM * pVM );
	static	SQInteger		IsMapOpen ( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};