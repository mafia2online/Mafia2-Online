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

#include	"../../../Shared/CString.h"
#include	"../../../Shared/CEvents.h"
#include	"../../../Shared/CCommands.h"
#include	"../../../Shared/Scripting/CSquirrelCommon.h"

void CSharedPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getPlayerMoney", GetMoney, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayers", GetPlayers, 0, NULL);
}

// getPlayers();
SQInteger CSharedPlayerNatives::GetPlayers(SQVM * pVM)
{
	// Counter
	SQInteger iCount;

	// Create new table
	sq_newtable(pVM);

	// Loop each entity
	for (EntityId id = 0; id < MAX_PLAYERS; id++)
	{
		// Entity active ?
		if (CCore::Instance()->GetPlayerManager()->IsActive(id)){

			// Valid pointer ?
			if (CCore::Instance()->GetPlayerManager()->Get(id)){

				// Push id and name
				sq_pushinteger(pVM, id);
				sq_pushstring(pVM, CCore::Instance()->GetPlayerManager()->Get(id)->GetNick(), -1);

				// Create new table slot
				sq_createslot(pVM, -3);
				iCount++;
			}
		}
	}
	return (1);
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