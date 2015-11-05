/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

void CPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getLocalPlayer", GetLocalPlayer, 0, NULL );
	pScriptingManager->RegisterFunction( "getPlayerName", GetName, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerPing", GetPing, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerColour", GetColour, 1, "i" );
	pScriptingManager->RegisterFunction( "isPlayerConnected", IsPlayerConnected, 1, "i" );
	pScriptingManager->RegisterFunction( "isPlayerOnScreen", IsOnScreen, 1, "i" );
	pScriptingManager->RegisterFunction( "isPlayerSpawned", IsSpawned, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerPosition", GetPosition, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerRotation", GetRotation, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerHealth", GetHealth, 1, "i" );
	pScriptingManager->RegisterFunction( "togglePlayerControls", ToggleControls, 1, "b" );
	pScriptingManager->RegisterFunction( "isPlayerInVehicle", IsInVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerVehicle", GetVehicle, 1, "i" );
}

// getLocalPlayer();
SQInteger CPlayerNatives::GetLocalPlayer( SQVM * pVM )
{
	sq_pushinteger( pVM, pCore->GetPlayerManager()->GetLocalPlayer()->GetId() );
	return 1;
}

// getPlayerName( playerid );
SQInteger CPlayerNatives::GetName( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this not the localplayer?
	if( playerId != pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( pCore->GetPlayerManager()->Get(playerId) )
		{
			// Get the player name
			String strNick = pCore->GetPlayerManager()->Get(playerId)->GetNick();

			sq_pushstring( pVM, strNick.Get(), strNick.GetLength() );
			return 1;
		}
	}
	else
	{
		// Get the localplayer nick
		String strNick = pCore->GetPlayerManager()->GetLocalPlayer()->GetNick();

		sq_pushstring( pVM, strNick.Get(), strNick.GetLength() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerPing( playerid );
SQInteger CPlayerNatives::GetPing( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this not the localplayer?
	if( playerId != pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( pCore->GetPlayerManager()->Get(playerId) )
		{
			sq_pushinteger( pVM, pCore->GetPlayerManager()->Get(playerId)->GetPing() );
			return 1;
		}
	}
	else
	{
		sq_pushinteger( pVM, pCore->GetPlayerManager()->GetLocalPlayer()->GetPing() );
		return 1;
	}

	sq_pushinteger( pVM, 99999 );
	return 1;
}

// getPlayerColour( playerid );
SQInteger CPlayerNatives::GetColour( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this not the localplayer?
	if( playerId != pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( pCore->GetPlayerManager()->Get(playerId) )
		{
			sq_pushinteger( pVM, ((pCore->GetPlayerManager()->Get(playerId)->GetColour() >> 8) | 0xFF000000) );
			return 1;
		}
	}
	else
	{
		sq_pushinteger( pVM, ((pCore->GetPlayerManager()->GetLocalPlayer()->GetColour() >> 8) | 0xFF000000) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// isPlayerConnected( playerid );
SQInteger CPlayerNatives::IsPlayerConnected( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, pCore->GetPlayerManager()->IsActive(playerId) );
	return 1;
}

// isPlayerOnScreen( playerid );
SQInteger CPlayerNatives::IsOnScreen( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, true );
		return 1;
	}

	// Is the player connected?
	if( pCore->GetPlayerManager()->IsActive( playerId ) && pCore->GetPlayerManager()->Get(playerId)->GetPlayerPed() )
	{
		sq_pushbool( pVM, pCore->GetPlayerManager()->Get(playerId)->GetPlayerPed()->IsOnScreen() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// isPlayerSpawned( playerid );
SQInteger CPlayerNatives::IsSpawned( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() );
		return 1;
	}
	else
	{
		if ( pCore->GetPlayerManager()->IsActive(playerId) )
		{
			sq_pushbool( pVM, pCore->GetPlayerManager()->Get(playerId)->IsSpawned() );
			return 1;
		}
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerPosition( playerid );
SQInteger CPlayerNatives::GetPosition( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	CVector3 vecPos;

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
		pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecPos );
	else
	{
		if( pCore->GetPlayerManager()->IsActive(playerId) )
			pCore->GetPlayerManager()->Get(playerId)->GetPosition( &vecPos );
	}

	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecPos.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecPos.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecPos.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}

// getPlayerRotation( playerid );
SQInteger CPlayerNatives::GetRotation( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	CVector3 vecRot;

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
		pCore->GetPlayerManager()->GetLocalPlayer()->GetRotation( &vecRot );
	else
	{
		if( pCore->GetPlayerManager()->IsActive(playerId) )
			pCore->GetPlayerManager()->Get(playerId)->GetRotation( &vecRot );
	}

	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecRot.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecRot.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecRot.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}

// getPlayerHealth( playerid );
SQInteger CPlayerNatives::GetHealth( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );
	
	float fHealth = 0.0f;

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
		fHealth = pCore->GetPlayerManager()->GetLocalPlayer()->GetHealth();
	else
	{
		if( pCore->GetPlayerManager()->IsActive(playerId) )
			fHealth = pCore->GetPlayerManager()->Get(playerId)->GetHealth();
	}

	sq_pushfloat( pVM, fHealth );
	return 1;
}

// togglePlayerControls( bool bToggle );
SQInteger CPlayerNatives::ToggleControls( SQVM * pVM )
{
	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );
	
	// Is this the localplayer?
	pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( bToggle );

	sq_pushbool( pVM, true );
	return 1;
}

// isPlayerInVehicle( playerid );
SQInteger CPlayerNatives::IsInVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );
	
	bool bInVehicle = false;

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() )
		bInVehicle = pCore->GetPlayerManager()->GetLocalPlayer()->IsInVehicle();
	else
	{
		if( pCore->GetPlayerManager()->IsActive(playerId) )
			bInVehicle = pCore->GetPlayerManager()->Get(playerId)->IsInVehicle();
	}

	sq_pushbool( pVM, bInVehicle );
	return 1;
}

// getPlayerVehicle( playerid );
SQInteger CPlayerNatives::GetVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );
	
	EntityId vehicleId = INVALID_ENTITY_ID;

	// Is this the localplayer?
	if( playerId == pCore->GetPlayerManager()->GetLocalPlayer()->GetId() && pCore->GetPlayerManager()->GetLocalPlayer()->IsInVehicle() )
		vehicleId = pCore->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetId();
	else
	{
		if( pCore->GetPlayerManager()->IsActive(playerId) && pCore->GetPlayerManager()->Get(playerId)->IsInVehicle() )
			vehicleId = pCore->GetPlayerManager()->Get(playerId)->GetVehicle()->GetId();
	}

	sq_pushinteger( pVM, vehicleId );
	return 1;
}