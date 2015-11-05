/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CScriptingRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

#include	"CPlayerRPC.h"
#include	"CVehicleRPC.h"
#include	"CBlipRPC.h"

extern	CCore			* pCore;
bool	CScriptingRPC::m_bRegistered = false;

void CScriptingRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Register the player RPC's
	CPlayerRPC::Register( pRPC );

	// Register the vehicle RPC's
	CVehicleRPC::Register( pRPC );

	// Register the blip RPC's
	CBlipRPC::Register ( pRPC );
}

void CScriptingRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Unregister the player RPC's
	CPlayerRPC::Unregister( pRPC );

	// Unregister the vehicle RPC's
	CVehicleRPC::Unregister( pRPC );

	// Unregister the blip RPC's
	CBlipRPC::Unregister ( pRPC );
}