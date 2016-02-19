/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPlayerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"Scripting\CScriptingManager.h"
#include	"Scripting\CSquirrelCommon.h"

#include	"Math\CVector3.h"
#include	"CString.h"

#include	"CClientScriptingManager.h"

#include	"engine\CM2Entity.h"
#include	"engine\CM2Ped.h"
#include	"CM2Hud.h"

#include	"CPlayerManager.h"
#include	"CLocalPlayer.h"
#include	"CNetworkPlayer.h"
#include	"CRemotePlayer.h"

#include	"SharedUtility.h"

#include	"CPlayerNatives.h"

void CPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	// Functions
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
	pScriptingManager->RegisterFunction( "isTogglePlayerControls", IsToggleControls, 0, NULL );
	pScriptingManager->RegisterFunction( "isPlayerInVehicle", IsInVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerVehicle", GetVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "setPlayerDrunkLevel", SetDrunkLevel, 1, "i");
	pScriptingManager->RegisterFunction( "getPlayerDrunkLevel", GetDrunkLevel, 0, NULL);
	pScriptingManager->RegisterFunction( "resetPlayerDrunkLevel", ResetDrunkLevel, 0, NULL);
	pScriptingManager->RegisterFunction( "setPlayerWantedLevel", SetWantedLevel, 1, "i");
	pScriptingManager->RegisterFunction( "getPlayerWantedLevel", GetWantedLevel, 0, NULL);
	pScriptingManager->RegisterFunction( "getPlayerMoveState", GetMoveState, 1, "i" );

	// Constants
	pScriptingManager->RegisterConstant("MOVE_STATE_WALK", ePlayerMovementState::E_WALK);
	pScriptingManager->RegisterConstant("MOVE_STATE_JOGGING", ePlayerMovementState::E_JOG);
	pScriptingManager->RegisterConstant("MOVE_STATE_SPRINT", ePlayerMovementState::E_SPRINT);
	pScriptingManager->RegisterConstant("MOVE_STATE_IDLE", ePlayerMovementState::E_IDLE);
	pScriptingManager->RegisterConstant("MOVE_STATE_STOP", ePlayerMovementState::E_STOPPING);
}

// getPlayerMoveState(playerid);
SQInteger CPlayerNatives::GetMoveState(SQVM *pVM)
{
	// Get the playerId
	SQInteger playerId;
	sq_getinteger(pVM, -1, &playerId);

	// Player control structure
	M2PlayerControls controls;

	// Is this not the localPlayer ?
	if (playerId != CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId())
	{
		if (CCore::Instance()->GetPlayerManager()->Get(playerId))
		{
			// Get the controls
			controls = CCore::Instance()->GetPlayerManager()->Get(playerId)->GetPlayerPed()->GetPed()->m_playerControls;

			// Return it
			sq_pushinteger(pVM, controls.m_playerMovementState);
			return (true);
		}
	}
	else {
		// Get the controls
		controls = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed()->m_playerControls;

		// Return it
		sq_pushinteger(pVM, controls.m_playerMovementState);
		return (true);
	}

	// In case of failure
	return (false);
}

// getLocalPlayer();
SQInteger CPlayerNatives::GetLocalPlayer( SQVM * pVM )
{
	sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() );
	return 1;
}

// getPlayerName( playerid );
SQInteger CPlayerNatives::GetName( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this not the localplayer?
	if( playerId != CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( CCore::Instance()->GetPlayerManager()->Get(playerId) )
		{
			// Get the player name
			String strNick = CCore::Instance()->GetPlayerManager()->Get(playerId)->GetNick();

			sq_pushstring( pVM, strNick.Get(), strNick.GetLength() );
			return 1;
		}
	}
	else
	{
		// Get the localplayer nick
		String strNick = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetNick();

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
	if( playerId != CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( CCore::Instance()->GetPlayerManager()->Get(playerId) )
		{
			sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get(playerId)->GetPing() );
			return 1;
		}
	}
	else
	{
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPing() );
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
	if( playerId != CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		// Is the player active?
		if( CCore::Instance()->GetPlayerManager()->Get(playerId) )
		{
			sq_pushinteger( pVM, ((CCore::Instance()->GetPlayerManager()->Get(playerId)->GetColour() >> 8) | 0xFF000000) );
			return 1;
		}
	}
	else
	{
		sq_pushinteger( pVM, ((CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetColour() >> 8) | 0xFF000000) );
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->IsActive(playerId) );
	return 1;
}

// isPlayerOnScreen( playerid );
SQInteger CPlayerNatives::IsOnScreen( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is this the localplayer?
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, true );
		return 1;
	}

	// Is the player connected?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) && CCore::Instance()->GetPlayerManager()->Get(playerId)->GetPlayerPed() )
	{
		sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->Get(playerId)->GetPlayerPed()->IsOnScreen() );
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
	{
		sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() );
		return 1;
	}
	else
	{
		if ( CCore::Instance()->GetPlayerManager()->IsActive(playerId) )
		{
			sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->Get(playerId)->IsSpawned() );
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecPos );
	else
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive(playerId) )
			CCore::Instance()->GetPlayerManager()->Get(playerId)->GetPosition( &vecPos );
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetRotation( &vecRot );
	else
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive(playerId) )
			CCore::Instance()->GetPlayerManager()->Get(playerId)->GetRotation( &vecRot );
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
		fHealth = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetHealth();
	else
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive(playerId) )
			fHealth = CCore::Instance()->GetPlayerManager()->Get(playerId)->GetHealth();
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
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls( bToggle );

	sq_pushbool( pVM, true );
	return 1;
}

// isTogglePlayerControls( bool bToggle );
SQInteger CPlayerNatives::IsToggleControls( SQVM * pVM )
{
	sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->AreControlsLocked() );
	return 1;
}

// isPlayerInVehicle( playerid );
SQInteger CPlayerNatives::IsInVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );
	
	bool bInVehicle = false;

	// Is this the localplayer?
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() )
		bInVehicle = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsInVehicle();
	else
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive(playerId) )
			bInVehicle = CCore::Instance()->GetPlayerManager()->Get(playerId)->IsInVehicle();
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
	if( playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId() && CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsInVehicle() )
		vehicleId = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetVehicle()->GetId();
	else
	{
		if( CCore::Instance()->GetPlayerManager()->IsActive(playerId) && CCore::Instance()->GetPlayerManager()->Get(playerId)->IsInVehicle() )
			vehicleId = CCore::Instance()->GetPlayerManager()->Get(playerId)->GetVehicle()->GetId();
	}

	sq_pushinteger( pVM, vehicleId );
	return 1;
}

SQInteger CPlayerNatives::SetDrunkLevel( SQVM *pVM )
{
	SQInteger drunkLevel;

	// Get the drunk level
	sq_getinteger(pVM, -1, &drunkLevel);

	if (CCore::Instance()->GetHud() != NULL){

		// Set the drunk level
		CCore::Instance()->GetHud()->SetDrunkLevel(drunkLevel);

		sq_pushbool(pVM, true);
		return (1);

	}
	sq_pushbool(pVM, false);
	return (1);
}

SQInteger CPlayerNatives::GetDrunkLevel(SQVM *pVM)
{
	// Return the value
	sq_pushinteger(pVM, CCore::Instance()->GetHud()->GetDrunkLevel());
	return (1);
}

SQInteger CPlayerNatives::ResetDrunkLevel(SQVM *pVM)
{
	// Reset drunk level
	CCore::Instance()->GetHud()->SetDrunkLevel(0);

	// Return the value
	sq_pushbool(pVM, true);
	return (1);
}

SQInteger CPlayerNatives::SetWantedLevel(SQVM *pVM)
{
	SQInteger wantedLevel;
	SQFloat	size;

	// Get the drunk level
	sq_getinteger(pVM, -2, &wantedLevel);
	sq_getfloat(pVM, -1, &size);

	if (CCore::Instance()->GetHud() != NULL){

		// Set the drunk level
		CCore::Instance()->GetHud()->SetWantedLevel(wantedLevel, size);

		sq_pushbool(pVM, true);
		return (1);

	}
	sq_pushbool(pVM, false);
	return (1);
}

SQInteger CPlayerNatives::GetWantedLevel(SQVM *pVM)
{
	// Return the value
	sq_pushinteger(pVM, CCore::Instance()->GetHud()->GetWantedLevel());
	return (1);
}