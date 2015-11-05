/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CEventNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CEventNatives.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif


extern	CCore			* pCore;

void CEventNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "addEventHandler", AddEvent, -1, NULL );
	pScriptingManager->RegisterFunction( "callEvent", CallEvent, -1, NULL );
	pScriptingManager->RegisterFunction( "removeEventHandler", RemoveEvent, -2, "sc" );

	pScriptingManager->RegisterFunction( "addCommandHandler", AddCommand, -1, NULL );
}

// addEventHandler( name, function );
SQInteger CEventNatives::AddEvent( SQVM * pVM )
{
	SQObjectPtr pFunction;

	const SQChar * szEventName;
	sq_getstring( pVM, 2, &szEventName );

	pFunction = stack_get( pVM, 3 );

	// Add the event
#ifndef _CLIENT
	sq_pushbool( pVM, pCore->GetEvents()->Add( szEventName, new CSquirrelEventHandler( pVM, pFunction ) ) );
#else
	sq_pushbool( pVM, pCore->GetClientScriptingManager()->GetEvents()->Add( szEventName, new CSquirrelEventHandler( pVM, pFunction ) ) );
#endif
	return 1;
}

// callEvent( name, ... );
SQInteger CEventNatives::CallEvent( SQVM * pVM )
{
	//
	CHECK_PARAMS_MIN( "callEvent", 2 );
	CHECK_TYPE( "callEvent", 1, 2, OT_STRING );

	// Get the stack top
	int iTop = sq_gettop( pVM );

	// Get the event name
	const SQChar * szEventName;
	sq_getstring( pVM, 2, &szEventName );

	// Collect all the arguments
	CSquirrelArguments pArguments;
	for( int i = 3; i <= iTop; i++ )
	{
		pArguments.pushFromStack( pVM, i );
	}

	// Is there no arguments?
	if( pArguments.size() == 0 )
	{
		sq_pushnull( pVM );
		return 1;
	}

	// Call the event
#ifdef _CLIENT
	CSquirrelArgument retArgs = pCore->GetClientScriptingManager()->GetEvents()->Call( szEventName, &pArguments );
#else
	CSquirrelArgument retArgs = pCore->GetEvents()->Call( szEventName, &pArguments );
#endif

	retArgs.push( pVM );
	return 1;
}

// removeEventHandler( name, function );
SQInteger CEventNatives::RemoveEvent( SQVM * pVM )
{
	SQObjectPtr pFunction;

	// Get the event name
	const SQChar * szEventName;
	sq_getstring( pVM, -2, &szEventName );

	pFunction = stack_get( pVM, -1 );

	// Remove the event
#ifdef _CLIENT
	sq_pushbool( pVM, pCore->GetClientScriptingManager()->GetEvents()->Remove( szEventName, &CSquirrelEventHandler( pVM, pFunction ) ) );
#else
	CSquirrelEventHandler pEventHandler ( pVM, pFunction );
	sq_pushbool( pVM, pCore->GetEvents()->Remove( szEventName, &pEventHandler ) );
#endif
	return 1;
}

// addCommandHandler( name, function );
SQInteger CEventNatives::AddCommand( SQVM * pVM )
{
	SQObjectPtr pFunction;

	// Get the event name
	const SQChar * szCommandName;
	sq_getstring( pVM, 2, &szCommandName );

	pFunction = stack_get( pVM, 3 );

	// Add the event
#ifndef _CLIENT
	sq_pushbool( pVM, pCore->GetCommands()->Add( szCommandName, new CSquirrelCommandHandler( pVM, pFunction ) ) );
#else
	sq_pushbool( pVM, pCore->GetClientScriptingManager()->GetCommands()->Add( szCommandName, new CSquirrelCommandHandler( pVM, pFunction ) ) );
#endif
	return 1;
}
