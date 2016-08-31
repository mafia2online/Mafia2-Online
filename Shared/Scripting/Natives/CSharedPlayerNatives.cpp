/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedPlayerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../../../Shared/CString.h"

#ifdef _CLIENT
#include	"../../../Client/BaseInc.h"
#include	"../../../Client/CCore.h"
#include	"../../../Client/CClientScriptingManager.h"
#include	"../../../Client/CClientScriptGUIManager.h"
#include	"../../../Client/CPlayerManager.h"
#include	"../../../Client/CRemotePlayer.h"
#include	"../../../Client/CNetworkPlayer.h"
#else
#include	"../../../Server/StdInc.h"
#endif

#include	"../../../Shared/CEvents.h"
#include	"../../../Shared/CCommands.h"
#include	"../../../Shared/Scripting/CSquirrelCommon.h"

#include	"CSharedPlayerNatives.h"

void CSharedPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getPlayerMoney", GetMoney, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayers", GetPlayers, 0, NULL);
}

// getPlayers();
SQInteger CSharedPlayerNatives::GetPlayers(SQVM * pVM)
{
	SQInteger iCount = 0;

	sq_newtable(pVM);

	CPlayerManager *pPlayerManager = CCore::Instance()->GetPlayerManager();

	for (EntityId id = 0; id < MAX_PLAYERS; ++id)
	{
		if (!pPlayerManager->IsActive(id))
		{
			continue;
		}

		sq_pushinteger(pVM, id);
		sq_pushstring(pVM, pPlayerManager->Get(id)->GetNick(), -1);

		sq_createslot(pVM, -3);
		iCount++;
	}
	return 1;
}

// getPlayerMoney( playerId );
SQInteger CSharedPlayerNatives::GetMoney( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		sq_pushinteger ( pVM, CCore::Instance()->GetPlayerManager()->Get ( playerId )->GetMoney () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}