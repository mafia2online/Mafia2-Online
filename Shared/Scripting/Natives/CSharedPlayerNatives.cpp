/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedPlayerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSharedPlayerNatives.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern		CCore			* pCore;

void CSharedPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getPlayerMoney", GetMoney, 1, "i" );
}

// getPlayerMoney( playerId );
SQInteger CSharedPlayerNatives::GetMoney( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( pCore->GetPlayerManager()->IsActive( playerId ) )
	{
		sq_pushinteger ( pVM, pCore->GetPlayerManager()->Get ( playerId )->GetMoney () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}