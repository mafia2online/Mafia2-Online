/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGameNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

void CGameNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "setWeather", SetWeather, 1, "s" );
	pScriptingManager->RegisterFunction( "getWeather", GetWeather, 0, NULL );
	pScriptingManager->RegisterFunction( "toggleHud", ToggleHud, 1, "b" );
	pScriptingManager->RegisterFunction( "takeScreenshot", TakeScreenshot, 0, NULL );
	pScriptingManager->RegisterFunction( "fadeScreen", FadeScreen, -1, NULL );
	pScriptingManager->RegisterFunction( "createHudTimer", CreateHudTimer, -1, NULL );
	pScriptingManager->RegisterFunction( "setHudTimerRemainingTime", SetHudTimerRemainingTime, 1, "f" );
	pScriptingManager->RegisterFunction( "getHudTimerReminaingTime", GetHudTimerRemainingTime, 0, NULL );
	pScriptingManager->RegisterFunction( "startHudTimer", StartHudTimer, 0, NULL );
	pScriptingManager->RegisterFunction( "stopHudTimer", StopHudTimer, 0, NULL );
	pScriptingManager->RegisterFunction( "isHudTimerRunning", IsHudTimerRunning, 0, NULL );
	pScriptingManager->RegisterFunction( "destroyHudTimer", DestroyHudTimer, 0, NULL );
	pScriptingManager->RegisterFunction( "openMap", OpenMap, 0, NULL );
	pScriptingManager->RegisterFunction( "isMapOpen", IsMapOpen, 0, NULL );
}

// setWeather( string weather );
SQInteger CGameNatives::SetWeather( SQVM * pVM )
{
	const SQChar * szWeather;
	sq_getstring( pVM, -1, &szWeather );

	// Set the weather
	pCore->GetGame()->ChangeWeather( szWeather );

	sq_pushbool( pVM, true );
	return 1;
}

// getWeather();
SQInteger CGameNatives::GetWeather( SQVM * pVM )
{
	sq_pushstring( pVM, pCore->GetGame()->GetWeather().Get(), pCore->GetGame()->GetWeather().GetLength() );
	return 1;
}

// toggleHud( bool toggle );
SQInteger CGameNatives::ToggleHud( SQVM * pVM )
{
	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );

	pCore->GetHud()->Show( (bool)bToggle );

	sq_pushbool( pVM, true );
	return 1;
}

// takeScreenshot( );
SQInteger CGameNatives::TakeScreenshot( SQVM * pVM )
{
	// Flag as take screenshot for next frame
	pCore->TakeScreenshot();

	sq_pushbool( pVM, true );
	return 1;
}

// fadeScreen( float fTime[, bool bFadeIn = true] );
SQInteger CGameNatives::FadeScreen( SQVM * pVM )
{
	// Get the stack top
	int iTop = sq_gettop( pVM ) - 1;

	if( iTop < 1 || iTop > 2 )
		CHECK_PARAMS( "fadeScreen", 1 );

	float fTime;
	SQBool bFadeIn = true;

	if( iTop == 2 )
	{
		CHECK_TYPE( "fadeScreen", 1, -2, OT_FLOAT );
		CHECK_TYPE( "fadeScreen", 2, -1, OT_BOOL );

		sq_getfloat( pVM, -2, &fTime );
		sq_getbool( pVM, -1, &bFadeIn );
	}
	else
	{
		CHECK_TYPE( "fadeScreen", 1, -1, OT_FLOAT );

		sq_getfloat( pVM, -1, &fTime );
	}

	// Fade the screen
	if( bFadeIn )
		pCore->GetHud()->FadeIn( fTime );
	else
		pCore->GetHud()->FadeOut( fTime );

	sq_pushbool( pVM, true );
	return 1;
}

// createHudTimer( float time[, bool show = true, bool start = true] );
SQInteger CGameNatives::CreateHudTimer( SQVM * pVM )
{
	// Get the stack top
	int iTop = sq_gettop( pVM ) - 1;

	if( iTop < 1 || iTop > 3 )
		CHECK_PARAMS( "createHudTimer", 1 );

	float fTime;
	SQBool bShow = true, bStart = true;

	if( iTop == 3 )
	{
		CHECK_TYPE( "createHudTimer", 1, -3, OT_FLOAT );
		CHECK_TYPE( "createHudTimer", 2, -2, OT_BOOL );
		CHECK_TYPE( "createHudTimer", 3, -1, OT_BOOL );

		sq_getfloat( pVM, -3, &fTime );
		sq_getbool( pVM, -2, &bShow );
		sq_getbool( pVM, -1, &bStart );
	}
	else if( iTop == 2 )
	{
		CHECK_TYPE( "createHudTimer", 1, -2, OT_FLOAT );
		CHECK_TYPE( "createHudTimer", 2, -1, OT_BOOL );

		sq_getfloat( pVM, -2, &fTime );
		sq_getbool( pVM, -1, &bShow );
	}
	else
	{
		CHECK_TYPE( "createHudTimer", 1, -1, OT_FLOAT );

		sq_getfloat( pVM, -1, &fTime );
	}

	// Show the hud timer if needed
	pCore->GetHud()->GetHudTimer()->Toggle( bShow );

	// Set the hud timer time remaining
	pCore->GetHud()->GetHudTimer()->SetTime( fTime );

	// Start the timer if needed
	if( bStart )
		pCore->GetHud()->GetHudTimer()->Start( );

	sq_pushbool( pVM, true );
	return 1;
}

// setHudTimerRemainingTime( float fTime );
SQInteger CGameNatives::SetHudTimerRemainingTime( SQVM * pVM )
{
	float fTime;
	sq_getfloat( pVM, -1, &fTime );

	// Set the hud timer time remaining
	pCore->GetHud()->GetHudTimer()->SetTime( fTime );

	sq_pushbool( pVM, true );
	return 1;
}

// getHudTimerRemainingTime( );
SQInteger CGameNatives::GetHudTimerRemainingTime( SQVM * pVM )
{
	sq_pushfloat( pVM, pCore->GetHud()->GetHudTimer()->GetRemainingTime() );
	return 1;
}

// startHudTimer( )
SQInteger CGameNatives::StartHudTimer( SQVM * pVM )
{
	// Start the hud timer
	pCore->GetHud()->GetHudTimer()->Start();

	sq_pushbool( pVM, true );
	return 1;
}

// stopHudTimer( )
SQInteger CGameNatives::StopHudTimer( SQVM * pVM )
{
	// Stop the hud timer
	pCore->GetHud()->GetHudTimer()->Stop( );

	sq_pushbool( pVM, true );
	return 1;
}

// isHudTimerRunning( );
SQInteger CGameNatives::IsHudTimerRunning( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetHud()->GetHudTimer()->IsRunning() );
	return 1;
}

// destroyHudTimer( )
SQInteger CGameNatives::DestroyHudTimer( SQVM * pVM )
{
	// Hide the hud timer
	pCore->GetHud()->GetHudTimer()->Toggle( false );

	// Stop the hud timer
	pCore->GetHud()->GetHudTimer()->Stop();

	// Reset the remaining time
	pCore->GetHud()->GetHudTimer()->SetTime( 0.0 );

	sq_pushbool( pVM, true );
	return 1;
}

// openMap( )
SQInteger CGameNatives::OpenMap( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetGame()->OpenMap ( true ) );
	return 1;
}

// isMapOpen( )
SQInteger CGameNatives::IsMapOpen( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetGame()->IsMapOpen () );
	return 1;
}