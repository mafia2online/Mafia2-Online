/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CPlayerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "CPlayerNatives.h"
#include "CCore.h"

void CPlayerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "isPlayerConnected", IsConnected, 1, "i" );
	pScriptingManager->RegisterFunction( "isPlayerSpawned", IsSpawned, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerName", GetName, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerSerial", GetSerial, 1, "i" );
	pScriptingManager->RegisterFunction( "setPlayerPosition", SetPosition, 4, "ifff" );
	pScriptingManager->RegisterFunction( "getPlayerPosition", GetPosition, 1, "i" );
	pScriptingManager->RegisterFunction( "sendPlayerMessage", OutputMessage, -1, NULL );
	pScriptingManager->RegisterFunction( "sendPlayerMessageToAll", OutputMessageToAll, -1, NULL );
	pScriptingManager->RegisterFunction( "setPlayerRotation", SetRotation, 4, "ifff" );
	pScriptingManager->RegisterFunction( "getPlayerRotation", GetRotation, 1, "i" );
	pScriptingManager->RegisterFunction( "setPlayerHealth", SetHealth, 2, "if" );
	pScriptingManager->RegisterFunction( "getPlayerHealth", GetHealth, 1, "i" );
	pScriptingManager->RegisterFunction( "givePlayerWeapon", GiveWeapon, 3, "iii" );
	pScriptingManager->RegisterFunction( "getPlayerWeapon", GetWeapon, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerWeaponBullet", GetWeaponBullet, 1, "i");
	pScriptingManager->RegisterFunction( "removePlayerWeapon", RemoveWeapon, -1, NULL );
	pScriptingManager->RegisterFunction( "setPlayerColour", SetColour, 2, "ii" );
	pScriptingManager->RegisterFunction( "getPlayerColour", GetColour, 1, "i" );
	pScriptingManager->RegisterFunction( "togglePlayerHud", ToggleHud, 2, "ib" );
	pScriptingManager->RegisterFunction( "setPlayerMoney", SetMoney, 2, "ii" );
	pScriptingManager->RegisterFunction( "givePlayerMoney", GiveMoney, 2, "ii" );
	pScriptingManager->RegisterFunction( "takePlayerMoney", TakeMoney, 2, "ii" );
	pScriptingManager->RegisterFunction( "togglePlayerControls", ToggleControls, 2, "ib" );
	pScriptingManager->RegisterFunction( "kickPlayer", Kick, 1, "i" );
	pScriptingManager->RegisterFunction( "playSoundForPlayer", PlaySound, 2, "is" );
	pScriptingManager->RegisterFunction( "playSoundForAll", PlaySoundToAll, 1, "s" );
	pScriptingManager->RegisterFunction( "stopSoundForPlayer", StopSound, 1, "i" );
	pScriptingManager->RegisterFunction( "stopSoundForAll", StopSoundToAll, 0, NULL );
	pScriptingManager->RegisterFunction( "getPlayerNetworkStats", GetNetworkStats, 1, "i" );
	pScriptingManager->RegisterFunction( "triggerClientEvent", TriggerClientEvent, -1, NULL );
	pScriptingManager->RegisterFunction( "setPlayerModel", SetModel, 2, "ii" );
	pScriptingManager->RegisterFunction( "getPlayerModel", GetModel, 1, "i" );
	pScriptingManager->RegisterFunction( "isPlayerInVehicle", IsInVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerVehicle", GetVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerPing", GetPing, 1, "i" );
	pScriptingManager->RegisterFunction( "getPlayerIP", GetIP, 1, "i" );
	pScriptingManager->RegisterFunction( "putPlayerInVehicle", PutInVehicle, 3, "iii" );
	pScriptingManager->RegisterFunction( "removePlayerFromVehicle", RemoveFromVehicle, 1, "i" );
	pScriptingManager->RegisterFunction( "respawnPlayer", RespawnPlayer, 1, "i" );
	pScriptingManager->RegisterFunction( "setPlayerAnimStyle", SetAnimStyle, 3, "iss");
	pScriptingManager->RegisterFunction( "setPlayerHandModel", SetHandModel, 3, "iii");

	pScriptingManager->RegisterConstant( "HAND_LEFT", 1);
	pScriptingManager->RegisterConstant( "HAND_RIGHT", 2);
}

// isPlayerConnected( playerid );
SQInteger CPlayerNatives::IsConnected( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->IsActive(playerId) );
	return 1;
}

// isPlayerSpawned( playerId );
SQInteger CPlayerNatives::IsSpawned( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		sq_pushbool( pVM, !CCore::Instance()->GetPlayerManager()->Get(playerId)->IsDead() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerName( playerId );
SQInteger CPlayerNatives::GetName( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the player nick
		const char * szNick = CCore::Instance()->GetPlayerManager()->Get( playerId )->GetNick();

		sq_pushstring( pVM, szNick, strlen( szNick ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerSerial( playerId );
SQInteger CPlayerNatives::GetSerial( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the player serial
		const char * szSerial = CCore::Instance()->GetPlayerManager()->Get( playerId )->GetSerial();

		sq_pushstring( pVM, szSerial, strlen( szSerial ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerPosition( playerId, x, y, z );
SQInteger CPlayerNatives::SetPosition( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -4, &playerId );

	CVector3 vecPosition;
	sq_getfloat( pVM, -1, &vecPosition.fZ );
	sq_getfloat( pVM, -2, &vecPosition.fY );
	sq_getfloat( pVM, -3, &vecPosition.fX );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Set the player position
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SetPosition ( vecPosition );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerPosition( playerId );
SQInteger CPlayerNatives::GetPosition( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the players position
		CVector3 vecPosition;
		CCore::Instance()->GetPlayerManager()->Get( playerId )->GetPosition ( &vecPosition );

		sq_newarray( pVM, 0 );

		sq_pushfloat( pVM, vecPosition.fX );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecPosition.fY );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecPosition.fZ );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// sendPlayerMessage( playerId, message[, r = 255, g = 255, b = 255] );
SQInteger CPlayerNatives::OutputMessage( SQVM * pVM )
{
	// Get the top of the stack
	int iTop = (sq_gettop( pVM ) - 1);

	// Invalid params?
	if( iTop < 1 || iTop > 5 )
		CHECK_PARAMS( "sendMessage", 2 );

	//
	SQInteger playerId;
	const SQChar * szMessage;
	SQInteger r = 255, g = 255, b = 255;

	// Do we only have a string?
	if( iTop == 2 )
	{
		CHECK_TYPE( "sendMessage", 1, -2, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 2, -1, OT_STRING );
		sq_getinteger( pVM, -2, &playerId );
		sq_getstring( pVM, -1, &szMessage );
	}
	else if( iTop == 3 )
	{
		CHECK_TYPE( "sendMessage", 1, -3, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 2, -2, OT_STRING );
		CHECK_TYPE( "sendMessage", 3, -1, OT_INTEGER );
		sq_getinteger( pVM, -3, &playerId );
		sq_getstring( pVM, -2, &szMessage );
		sq_getinteger( pVM, -1, &r );
	}
	else if( iTop == 4 )
	{
		CHECK_TYPE( "sendMessage", 1, -4, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 2, -3, OT_STRING );
		CHECK_TYPE( "sendMessage", 3, -2, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 4, -1, OT_INTEGER );
		sq_getinteger( pVM, -4, &playerId );
		sq_getstring( pVM, -3, &szMessage );
		sq_getinteger( pVM, -2, &r );
		sq_getinteger( pVM, -1, &g );
	}
	else if( iTop == 5 )
	{
		CHECK_TYPE( "sendMessage", 1, -5, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 2, -4, OT_STRING );
		CHECK_TYPE( "sendMessage", 3, -3, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 4, -2, OT_INTEGER );
		CHECK_TYPE( "sendMessage", 5, -1, OT_INTEGER );
		sq_getinteger( pVM, -5, &playerId );
		sq_getstring( pVM, -4, &szMessage );
		sq_getinteger( pVM, -3, &r );
		sq_getinteger( pVM, -2, &g );
		sq_getinteger( pVM, -1, &b );
	}

	// Is the player valid?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream bitStream;

		// Write the string
		bitStream.Write( RakNet::RakString( szMessage ) );

		// Write the red value
		bitStream.WriteCompressed( r );

		// Write the green value
		bitStream.WriteCompressed( g );

		// Write the blue value
		bitStream.WriteCompressed( b );

		// Send it to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_SENDPLAYERMESSAGE, &bitStream, IMMEDIATE_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// sendMessageToAll( message[, r = 255, g = 255, b = 255] );
SQInteger CPlayerNatives::OutputMessageToAll( SQVM * pVM )
{
	// Get the top of the stack
	int iTop = (sq_gettop( pVM ) - 1);

	// Invalid params?
	if( iTop < 1 || iTop > 4 )
		CHECK_PARAMS( "sendMessage", 1 );

	//
	const SQChar * szMessage;
	SQInteger r = 255, g = 255, b = 255;

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
	else if( iTop == 4 )
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

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->GetCount() > 0 )
	{
		// Construct a new bitstream
		RakNet::BitStream bitStream;

		// Write the string
		bitStream.Write( RakNet::RakString( szMessage ) );

		// Write the red value
		bitStream.WriteCompressed( r );

		// Write the green value
		bitStream.WriteCompressed( g );

		// Write the blue value
		bitStream.WriteCompressed( b );

		// Send it to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_SENDPLAYERMESSAGE, &bitStream, IMMEDIATE_PRIORITY, RELIABLE, INVALID_ENTITY_ID, true );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerRotation( playerId, x, y, z );
SQInteger CPlayerNatives::SetRotation( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -4, &playerId );

	CVector3 vecRotation;
	sq_getfloat( pVM, -3, &vecRotation.fZ );
	sq_getfloat( pVM, -2, &vecRotation.fY );
	sq_getfloat( pVM, -1, &vecRotation.fX );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Set the player rotation
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SetRotation ( vecRotation );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerRotation( playerId );
SQInteger CPlayerNatives::GetRotation( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the players position
		CVector3 vecRotation;
		CCore::Instance()->GetPlayerManager()->Get( playerId )->GetRotation ( &vecRotation );

		sq_newarray( pVM, 0 );

		sq_pushfloat( pVM, vecRotation.fX );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecRotation.fY );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecRotation.fZ );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerHealth( playerId, health );
SQInteger CPlayerNatives::SetHealth( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQFloat fHealth;
	sq_getfloat( pVM, -1, &fHealth );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Set the player health
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SetHealth ( fHealth );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerHealth( playerId );
SQInteger CPlayerNatives::GetHealth( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the player health
		sq_pushfloat( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetHealth() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// givePlayerWeapon( playerId, weapon, ammo );
SQInteger CPlayerNatives::GiveWeapon( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -3, &playerId );

	SQInteger dwWeapon;
	sq_getinteger( pVM, -2, &dwWeapon );

	SQInteger dwAmmo;
	sq_getinteger( pVM, -1, &dwAmmo );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Give the player the weapon
		CCore::Instance()->GetPlayerManager()->Get( playerId )->GiveWeapon ( dwWeapon, dwAmmo );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerWeapon( playerId );
SQInteger CPlayerNatives::GetWeapon( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetWeapon() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerWeaponBullet(playerId);
SQInteger CPlayerNatives::GetWeaponBullet(SQVM * pVM)
{
	SQInteger playerId;
	sq_getinteger(pVM, -1, &playerId);

	if (CCore::Instance()->GetPlayerManager()->IsActive(playerId))
	{
		sq_pushinteger(pVM, CCore::Instance()->GetPlayerManager()->Get(playerId)->GetWeaponBullet());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// removePlayerWeapon( playerId, weapon[, ammo = 0] );
SQInteger CPlayerNatives::RemoveWeapon( SQVM * pVM )
{
	int iTop = sq_gettop( pVM ) - 1;

	if( iTop < 2 || iTop > 3 )
	{
		CHECK_PARAMS( "removePlayerWeapon", 2 );
	}

	SQInteger playerId;
	SQInteger iWeapon = 0;
	SQInteger iAmmo = 0;

	if( iTop > 2 )
	{
		CHECK_TYPE( "removePlayerWeapon", 1, -3, OT_INTEGER );
		CHECK_TYPE( "removePlayerWeapon", 2, -2, OT_INTEGER );
		CHECK_TYPE( "removePlayerWeapon", 3, -1, OT_INTEGER );

		sq_getinteger( pVM, -3, &playerId );
		sq_getinteger( pVM, -2, &iWeapon );
		sq_getinteger( pVM, -1, &iAmmo );
	}
	else
	{
		CHECK_TYPE( "removePlayerWeapon", 2, -2, OT_INTEGER );
		CHECK_TYPE( "removePlayerWeapon", 3, -1, OT_INTEGER );

		sq_getinteger( pVM, -2, &playerId );
		sq_getinteger( pVM, -1, &iWeapon );
	}

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Remvoe the weapon
		CCore::Instance()->GetPlayerManager()->Get( playerId )->RemoveWeapon ( iWeapon, iAmmo );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerColour( playerId, rgba );
SQInteger CPlayerNatives::SetColour( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQInteger colour;
	sq_getinteger( pVM, -1, &colour );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Set the player colour
		CCore::Instance()->GetPlayerManager()->Get(playerId)->SetColour( colour );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerColour( playerId );
SQInteger CPlayerNatives::GetColour( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the player colour
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get(playerId)->GetColour() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// togglePlayerHud( playerId, bool );
SQInteger CPlayerNatives::ToggleHud( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the toggle
		pBitStream.Write( bToggle );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_TOGGLEPLAYERHUD, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerMoney( playerId, amount );
SQInteger CPlayerNatives::SetMoney( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQInteger iMoney;
	sq_getinteger( pVM, -1, &iMoney );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the money
		pBitStream.Write( iMoney );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_SETPLAYERMONEY, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// givePlayerMoney( playerId, amount );
SQInteger CPlayerNatives::GiveMoney( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQInteger iMoney;
	sq_getinteger( pVM, -1, &iMoney );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		CCore::Instance()->GetPlayerManager()->Get ( playerId )->GiveMoney ( iMoney );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// takePlayerMoney( playerId, amount );
SQInteger CPlayerNatives::TakeMoney( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQInteger iMoney;
	sq_getinteger( pVM, -1, &iMoney );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		CCore::Instance()->GetPlayerManager()->Get ( playerId )->TakeMoney ( iMoney );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// togglePlayerControls( playerId, toggle );
SQInteger CPlayerNatives::ToggleControls( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the toggle
		pBitStream.Write( (bool)bToggle );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_TOGGLEPLAYERCONTROLS, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

/*
// redirectPlayer( playerId, host, port[, password = ""] );
SQInteger CPlayerNatives::Redirect( SQVM * pVM )
{
	// Get the stack top
	int iTop = sq_gettop( pVM ) - 1;

	if( iTop < 3 || iTop > 4 )
	{
		CHECK_PARAMS( "redirectPlayer", 3 );
	}

	EntityId playerId;
	String strHost;
	int iPort;
	String strPassword( "" );

	if( iTop > 3 )
	{
		CHECK_TYPE( "redirectPlayer", 1, -4, OT_INTEGER );
		CHECK_TYPE( "redirectPlayer", 2, -3, OT_STRING );
		CHECK_TYPE( "redirectPlayer", 3, -2, OT_INTEGER );
		CHECK_TYPE( "redirectPlayer", 4, -1, OT_STRING );

		sq_getentity( pVM, -4, &playerId );
		sq_getstrstring( pVM, -3, &strHost );
		sq_getinteger( pVM, -2, &iPort );
		sq_getstrstring( pVM, -1, &strPassword );
	}
	else
	{
		CHECK_TYPE( "redirectPlayer", 1, -3, OT_INTEGER );
		CHECK_TYPE( "redirectPlayer", 2, -2, OT_STRING );
		CHECK_TYPE( "redirectPlayer", 3, -1, OT_INTEGER );

		sq_getentity( pVM, -3, &playerId );
		sq_getstrstring( pVM, -2, &strHost );
		sq_getinteger( pVM, -1, &iPort );
	}

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the host string
		pBitStream.Write( RakNet::RakString( strHost.Get() ) );

		// Write the port
		pBitStream.Write( iPort );

		// Write the password
		pBitStream.Write( RakNet::RakString( strPassword.Get() ) );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_REDIRECTPLAYER, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}
*/

// kickPlayer( playerid );
SQInteger CPlayerNatives::Kick( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Kick the player
		CCore::Instance()->GetPlayerManager()->Get(playerId)->Kick( );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// playPlayerSound( playerId, music );
SQInteger CPlayerNatives::PlaySound( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	const SQChar  * szSound;
	sq_getstring( pVM, -1, &szSound );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the string
		pBitStream.Write( RakNet::RakString( szSound ) );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYSOUND, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// playStopSound( playerId );
SQInteger CPlayerNatives::StopSound( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_STOPSOUND, NULL, HIGH_PRIORITY, RELIABLE, playerId, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// playSoundToAll();
SQInteger CPlayerNatives::PlaySoundToAll( SQVM * pVM )
{
	const SQChar * szSound;
	sq_getstring( pVM, -1, &szSound );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->GetCount() > 0 )
	{
		// Construct a new bitstream
		RakNet::BitStream pBitStream;

		// Write the string
		pBitStream.Write( RakNet::RakString( szSound ) );

		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_PLAYSOUND, &pBitStream, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// stopSoundForAll();
SQInteger CPlayerNatives::StopSoundToAll( SQVM * pVM )
{
	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->GetCount() > 0 )
	{
		// Send to the player
		CCore::Instance()->GetNetworkModule()->Call( RPC_STOPSOUND, NULL, HIGH_PRIORITY, RELIABLE, INVALID_ENTITY_ID, false );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerNetworkStats( playerId );
SQInteger CPlayerNatives::GetNetworkStats( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		// Get the player network stats
		CNetStats netStats;
		CNetworkStats::GetStats( &netStats, playerId );

		// Create a new table
		sq_newtable( pVM );

		sq_pushstring( pVM, "BytesPerSecondReceived", strlen("BytesPerSecondReceived") );
		sq_pushinteger( pVM, netStats.bpsRecv );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "BytesPerSecondSent", strlen("BytesPerSecondSent") );
		sq_pushinteger( pVM, netStats.bpsSent );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "TotalBytesReceived", strlen("TotalBytesReceived") );
		sq_pushinteger( pVM, netStats.totalRecv );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "TotalBytesSent", strlen("TotalBytesSent") );
		sq_pushinteger( pVM, netStats.totalSent );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "CurrentPacketLoss", strlen("CurrentPacketLoss") );
		sq_pushfloat( pVM, netStats.fPacketLossLastSecond );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "TotalPacketLoss", strlen("TotalPacketLoss") );
		sq_pushfloat( pVM, netStats.fTotalPacketLoss );
		sq_createslot( pVM, -3 );

		sq_pushstring( pVM, "ConnectionTime", strlen("ConnectionTime") );
		sq_pushinteger( pVM, netStats.connectionTime );
		sq_createslot( pVM, -3 );

		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// triggerClientEvent( playerid, eventname, ... );
SQInteger CPlayerNatives::TriggerClientEvent( SQVM * pVM )
{
	CHECK_PARAMS_MIN( "triggerClientEvent", 2 );
	CHECK_TYPE( "triggerClientEvent", 1, 2, OT_INTEGER );
	CHECK_TYPE( "triggerClientEvent", 2, 3, OT_STRING );

	SQInteger playerId;
	sq_getinteger( pVM, 2, &playerId );
	CSquirrelArguments args( pVM, 3 );

	RakNet::BitStream bsSend;
	args.Serialise( &bsSend );
	CCore::Instance()->GetNetworkModule()->Call( RPC_TRIGGEREVENT, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );

	sq_pushbool( pVM, true );
	return 1;
}

// setPlayerModel( playerid, model );
SQInteger CPlayerNatives::SetModel( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -2, &playerId );

	SQInteger iModel;
	sq_getinteger( pVM, -1, &iModel );

	// Is the player valid?
	if( CCore::Instance()->GetPlayerManager()->Get( playerId ) )
	{
		// Set the model
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SetModel( iModel );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerModel( playerid );
SQInteger CPlayerNatives::GetModel( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player valid?
	if( CCore::Instance()->GetPlayerManager()->Get( playerId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetModel() );
		return 1;
	}

	sq_pushinteger( pVM, INVALID_ENTITY_ID );
	return 1;
}

// isPlayerInVehicle( playerid );
SQInteger CPlayerNatives::IsInVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	if( CCore::Instance()->GetPlayerManager()->Get( playerId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->IsInVehicle() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerVehicle( playerid );
SQInteger CPlayerNatives::GetVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	if( CCore::Instance()->GetPlayerManager()->Get( playerId ) && CCore::Instance()->GetPlayerManager()->Get( playerId )->IsInVehicle() )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetVehicle()->GetId() );
		return 1;
	}

	sq_pushinteger( pVM, INVALID_ENTITY_ID );
	return 1;
}

// getPlayerPing( playerid );
SQInteger CPlayerNatives::GetPing( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	if( CCore::Instance()->GetPlayerManager()->Get( playerId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetPing() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getPlayerIP( playerid );
SQInteger CPlayerNatives::GetIP( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		sq_pushstring( pVM, CCore::Instance()->GetPlayerManager()->Get( playerId )->GetIP(), strlen(CCore::Instance()->GetPlayerManager()->Get( playerId )->GetIP()) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// putPlayerInVehicle( playerid, vehicleid, seat );
SQInteger CPlayerNatives::PutInVehicle( SQVM * pVM )
{
	SQInteger playerId, vehicleId, seatId;
	sq_getinteger( pVM, -3, &playerId );
	sq_getinteger( pVM, -2, &vehicleId );
	sq_getinteger( pVM, -1, &seatId );

	// Is the player and vehicle active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) && CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		CNetworkPlayer * pPlayer = CCore::Instance()->GetPlayerManager()->Get( playerId );
		CNetworkVehicle * pVehicle = CCore::Instance()->GetVehicleManager()->Get( vehicleId );

		sq_pushbool ( pVM, pPlayer->PutInVehicle ( pVehicle, seatId ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// removePlayerFromVehicle ( playerid );
SQInteger CPlayerNatives::RemoveFromVehicle( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		CCore::Instance()->GetPlayerManager()->Get( playerId )->RemoveFromVehicle ();
		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// respawnPlayer ( playerid );
SQInteger CPlayerNatives::RespawnPlayer ( SQVM * pVM )
{
	SQInteger playerId;
	sq_getinteger( pVM, -1, &playerId );

	// Is the player active?
	if( CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
	{
		CCore::Instance()->GetPlayerManager()->Get( playerId )->SpawnForWorld (); // test
		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setPlayerAnimStyle(playerId, directory, style);
SQInteger	CPlayerNatives::SetAnimStyle(SQVM * pVM)
{
	SQInteger playerId;
	sq_getinteger(pVM, -3, &playerId);

	const SQChar * directory;
	sq_getstring(pVM, -2, &directory);

	const SQChar * style;
	sq_getstring(pVM, -1, &style);

	// Is the player active ?
	if (CCore::Instance()->GetPlayerManager()->IsActive(playerId))
	{
		// Send anim style
		CCore::Instance()->GetPlayerManager()->Get(playerId)->SetAnimStyle(directory, style);
		sq_pushbool(pVM, true);
		return (1);
	}
	sq_pushbool(pVM, false);
	return (1);
}

// setPlayerHandModel(playerid, hand, model);
SQInteger	CPlayerNatives::SetHandModel(SQVM * pVM)
{
	SQInteger playerId;
	sq_getinteger(pVM, -3, &playerId);

	SQInteger iHand;
	sq_getinteger(pVM, -2, &iHand);

	SQInteger iModel;
	sq_getinteger(pVM, -1, &iModel);

	// Is the player active?
	if (CCore::Instance()->GetPlayerManager()->IsActive(playerId))
	{
		// Send hand model
		CCore::Instance()->GetPlayerManager()->Get(playerId)->SetHandModel(iHand, iModel);
		sq_pushbool(pVM, true);
		return (1);
	}
	sq_pushbool(pVM, false);
	return (1);
}