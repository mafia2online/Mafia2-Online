/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CHttpNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore			* pCore;

void CHttpNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "webRequest", WebRequest, -1, NULL );
}

// webRequest( int type, string url, string post, function callback[, int port = 80] );
SQInteger CHttpNatives::WebRequest( SQVM * pVM )
{
	// Get the top of the stack
	int iTop = (sq_gettop( pVM ) - 1);

	// Invalid params?
	if( iTop < 4 || iTop > 5 )
		CHECK_PARAMS( "webRequest", 4 );

	//
	SQInteger iType;
	SQInteger iPort = 80;
	const SQChar * szUrl, * szPost;
	SQObjectPtr pFunction;

	// Do we have 4 params?
	if ( iTop == 4 )
	{
		// Check the param types
		CHECK_TYPE( "webRequest", 1, -4, OT_INTEGER );
		CHECK_TYPE( "webRequest", 2, -3, OT_STRING );
		CHECK_TYPE( "webRequest", 3, -2, OT_STRING );
		CHECK_TYPE( "webRequest", 4, -1, OT_CLOSURE );

		// Get the params
		sq_getinteger( pVM, -4, &iType );
		sq_getstring( pVM, -3, &szUrl );
		sq_getstring( pVM, -2, &szPost );
		pFunction = stack_get( pVM, -1 );
	}
	else if ( iTop == 5 )
	{
		// Check the param types
		CHECK_TYPE( "webRequest", 1, -5, OT_INTEGER );
		CHECK_TYPE( "webRequest", 2, -4, OT_STRING );
		CHECK_TYPE( "webRequest", 3, -3, OT_STRING );
		CHECK_TYPE( "webRequest", 4, -2, OT_CLOSURE );
		CHECK_TYPE( "webRequest", 5, -1, OT_INTEGER );

		// Get the params
		sq_getinteger( pVM, -5, &iType );
		sq_getstring( pVM, -4, &szUrl );
		sq_getstring( pVM, -3, &szPost );
		pFunction = stack_get( pVM, -2 );
		sq_getinteger( pVM, -1, &iPort );
	}

	// Add to the web request queue
	pCore->GetWebRequest()->AddToQueue( iType, szUrl, iPort, szPost, pFunction, pVM );

	sq_pushbool( pVM, true );
	return 1;
}
