/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Scripting/CScriptingManager.h"
#include "Scripting/CSquirrelCommon.h"

#include "Network/NetRPCs.h"

#include "PacketPriority.h"

#include "CKeyBinds.h"

#include "CNetworkModule.h"

#include "CClientNatives.h"

#include "CLogFile.h"

void CClientNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "triggerServerEvent", TriggerServerEvent, -1, NULL );

	// Key binds
	pScriptingManager->RegisterFunction( "bindKey", BindKey, 3, "ssc" );
	pScriptingManager->RegisterFunction( "unbindKey", UnbindKey, 2, "ss" );
	pScriptingManager->RegisterFunction( "isKeyBound", IsKeyBound, 2, "ss" );
}

// triggerServerEvent( eventname, ... );
SQInteger CClientNatives::TriggerServerEvent( SQVM * pVM )
{
	CHECK_PARAMS_MIN( "triggerServerEvent", 1 );
	CHECK_TYPE( "triggerServerEvent", 1, 2, OT_STRING );

	CSquirrelArguments args( pVM, 2 );

	RakNet::BitStream bsSend;
	args.Serialise( &bsSend );
	CCore::Instance()->GetNetworkModule()->Call( RPC_TRIGGEREVENT, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, true );

	sq_pushbool( pVM, true );
	return 1;
}

// bindKey( key, state, func );
SQInteger CClientNatives::BindKey( SQVM * pVM )
{
	// Get the key and state
	const SQChar *szKey, *szState;
	sq_getstring( pVM, -3, &szKey );
	sq_getstring( pVM, -2, &szState );

	// Get the function pointer
	SQObjectPtr pFunction = stack_get( pVM, -1 );

	// Is the key not already bound?
	if( !CCore::Instance()->GetKeyBinds()->IsKeyBound( szKey, szState ) )
	{
		// Bind the key
		sq_pushbool( pVM, CCore::Instance()->GetKeyBinds()->BindKey( szKey, szState, pVM, pFunction ) );
		return 1;
	}
	else {
		CLogFile::Printf("[Error] : Key %s already bound", szKey);
	}

	sq_pushbool( pVM, false );
	return 1;
}

// unbindKey( key, state );
SQInteger CClientNatives::UnbindKey( SQVM * pVM )
{
	// Get the key and state
	const SQChar *szKey, *szState;
	sq_getstring( pVM, -2, &szKey );
	sq_getstring( pVM, -1, &szState );

	// Is the key bound?
	if( CCore::Instance()->GetKeyBinds()->IsKeyBound( szKey, szState ) )
	{
		// Unbind the key
		sq_pushbool( pVM, CCore::Instance()->GetKeyBinds()->UnbindKey( szState, szState ) );
		return 1;
	}
	else {
		CLogFile::Printf("[Error] : Key %s don't bound", szKey);
	}

	sq_pushbool( pVM, false );
	return 1;
}

// isKeyBound( key, state );
SQInteger CClientNatives::IsKeyBound( SQVM * pVM )
{
	// Get the key and state
	const SQChar *szKey, *szState;
	sq_getstring( pVM, -2, &szKey );
	sq_getstring( pVM, -1, &szState );

	// Is the key bound?
	sq_pushbool( pVM, CCore::Instance()->GetKeyBinds()->IsKeyBound( szKey, szState ) );
	return 1;
}