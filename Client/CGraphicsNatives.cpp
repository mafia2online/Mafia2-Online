/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGraphicsNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

void CGraphicsNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getFPS", GetFPS, 0, NULL );
	pScriptingManager->RegisterFunction( "getScreenSize", GetScreenSize, 0, NULL );
	pScriptingManager->RegisterFunction( "getScreenFromWorld", WorldToScreen, 3, "fff" );
	pScriptingManager->RegisterFunction( "getWorldFromScreen", ScreenToWorld, 3, "fff" );
	pScriptingManager->RegisterFunction( "showChat", ShowChat, 1, "b" );
	pScriptingManager->RegisterFunction( "isChatVisible", IsChatVisible, 0, NULL );
	pScriptingManager->RegisterFunction( "isInputVisible", IsInputVisible, 0, NULL );
	pScriptingManager->RegisterFunction( "sendMessage", SendMessage, -1, NULL );
}

// getFPS( );
SQInteger CGraphicsNatives::GetFPS( SQVM * pVM )
{
	sq_pushinteger( pVM, pCore->GetFPSCounter()->GetFPS() );
	return 1;
}

// getScreenSize();
SQInteger CGraphicsNatives::GetScreenSize( SQVM * pVM )
{
	// Get the current viewport
	D3DVIEWPORT9 Viewport;
	pCore->GetGraphics()->GetDevice()->GetViewport( &Viewport );

	CSquirrelArguments args;
	args.push( (int)Viewport.Width );
	args.push( (int)Viewport.Height );

	CSquirrelArgument * pArgument = new CSquirrelArgument(args, true);
	pArgument->push( pVM );

	delete pArgument;
	return 1;
}

// getScreenFromWorld( x, y, z );
SQInteger CGraphicsNatives::WorldToScreen( SQVM * pVM )
{
	// Get the world position
	CVector3 vecWorld;
	sq_getfloat( pVM, -3, &vecWorld.fX );
	sq_getfloat( pVM, -2, &vecWorld.fY );
	sq_getfloat( pVM, -1, &vecWorld.fZ );

	// Convert the world coordinates into screen coordinates
	CVector3 vecScreen;
	pCore->GetGraphics()->WorldToScreen( vecWorld, &vecScreen );

	// Return the screen coordinates
	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecScreen.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecScreen.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecScreen.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}

// getWorldFromScreen( x, y, depth );
SQInteger CGraphicsNatives::ScreenToWorld( SQVM * pVM )
{
	// Get the screen position
	CVector3 vecScreen;
	sq_getfloat( pVM, -3, &vecScreen.fX );
	sq_getfloat( pVM, -2, &vecScreen.fY );
	sq_getfloat( pVM, -1, &vecScreen.fZ );

	// Convert the screen coordinates into world coordinates
	CVector3 vecWorld;
	pCore->GetGraphics()->ScreenToWorld( vecScreen, &vecWorld );

	// Return the world coordinates
	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecWorld.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecWorld.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecWorld.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}

// showChat( bool );
SQInteger CGraphicsNatives::ShowChat( SQVM * pVM )
{
	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );

	pCore->GetChat()->SetVisible( (bool)bToggle );

	sq_pushbool( pVM, true );
	return 1;
}

// isChatVisible( );
SQInteger CGraphicsNatives::IsChatVisible( SQVM * pVM )
{
	sq_pushbool( pVM, (SQBool)pCore->GetChat()->IsVisible() );
	return 1;
}

// isInputVisible( );
SQInteger CGraphicsNatives::IsInputVisible( SQVM * pVM )
{
	sq_pushbool( pVM, (SQBool)pCore->GetChat()->IsInputVisible() );
	return 1;
}

// sendMessage( string[, r = 255, g = 255, b = 255] );
SQInteger CGraphicsNatives::SendMessage( SQVM * pVM )
{
	// Get the top of the stack
	int iTop = (sq_gettop( pVM ) - 1);

	// Invalid params?
	if( iTop < 1 || iTop > 4 )
		CHECK_PARAMS( "sendMessage", 1 );

	//
	const SQChar * szMessage;
	int r = 255, g = 255, b = 255;

	// Do we only have a string?
	if( iTop == 1 )
	{
		CHECK_TYPE( "sendMessage", 1, -1, OT_STRING );
		sq_getstring( pVM, -1, &szMessage );
	}
	else if( iTop == 2 )
	{
		CHECK_TYPE( "sendMessage", 1, -2, OT_STRING );
		CHECK_TYPE( "sendMessage", 2, -1, OT_INTEGER );
		sq_getstring( pVM, -2, &szMessage );
		sq_getinteger( pVM, -1, &r );
	}
	else if( iTop == 3 )
	{
		CHECK_TYPE( "sendMessage", 1, -3, OT_STRING );
		CHECK_TYPE( "sendMessage", 2, -2, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 3, -1, OT_INTEGER );
		sq_getstring( pVM, -3, &szMessage );
		sq_getinteger( pVM, -2, &r );
		sq_getinteger( pVM, -1, &g );
	}
	else if( iTop == 3 )
	{
		CHECK_TYPE( "sendMessage", 1, -4, OT_STRING );
		CHECK_TYPE( "sendMessage", 2, -3, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 3, -2, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 4, -1, OT_INTEGER );
		sq_getstring( pVM, -4, &szMessage );
		sq_getinteger( pVM, -3, &r );
		sq_getinteger( pVM, -2, &g );
		sq_getinteger( pVM, -1, &b );
	}

	if( pCore->GetChat() )
	{
		// Output message to the chat window
		pCore->GetChat()->AddInfoMessage( CColor( r, g, b, 255 ), szMessage );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}