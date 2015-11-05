/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CVehicleNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CVehicleNatives.h"

extern		CCore			* pCore;

void CVehicleNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "createVehicle", Create, 7, "iffffff" );
	pScriptingManager->RegisterFunction( "destroyVehicle", Destroy, 1, "i" );

	pScriptingManager->RegisterFunction( "setVehicleRespawnTime", SetRespawnTime, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicleRespawnTime", GetRespawnTime, 1, "i" );

	pScriptingManager->RegisterFunction( "respawnVehicle", RespawnVehicle, 1, "i" );
}

// createVehicle( model, x, y, z, rx, ry, rz );
SQInteger CVehicleNatives::Create( SQVM * pVM )
{
	SQInteger iModel;
	CVector3 vecPosition;
	CVector3 vecDirection;

	sq_getinteger( pVM, -7, &iModel );	

	sq_getfloat( pVM, -6, &vecPosition.fX );
	sq_getfloat( pVM, -5, &vecPosition.fY );
	sq_getfloat( pVM, -4, &vecPosition.fZ );

	sq_getfloat( pVM, -3, &vecDirection.fX );
	sq_getfloat( pVM, -2, &vecDirection.fY );
	sq_getfloat( pVM, -1, &vecDirection.fZ );

	sq_pushinteger( pVM, pCore->GetVehicleManager()->Add( iModel, vecPosition, vecDirection ) );
	return 1;
}

// destroyVehicle( vehicleid );
SQInteger CVehicleNatives::Destroy( SQVM * pVM )
{
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	sq_pushbool( pVM, pCore->GetVehicleManager()->Remove( vehicleId ) );
	return 1;
}

// setVehicleRespawnTime( vehicleid, time );
SQInteger CVehicleNatives::SetRespawnTime ( SQVM * pVM )
{
	SQInteger vehicleId, iTime;
	sq_getinteger ( pVM, -2, &vehicleId );
	sq_getinteger ( pVM, -1, &iTime );

	// Is the vehicle active?
	if ( pCore->GetVehicleManager()->IsActive ( vehicleId ) )
	{
		// Set the vehicle respawn time
		pCore->GetVehicleManager()->Get( vehicleId )->SetRespawnTime ( iTime );

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}

// getVehicleRespawnTime( vehicleid );
SQInteger CVehicleNatives::GetRespawnTime ( SQVM * pVM )
{
	SQInteger vehicleId;
	sq_getinteger ( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if ( pCore->GetVehicleManager()->IsActive ( vehicleId ) )
	{
		// Set the vehicle respawn time
		sq_pushinteger ( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetRespawnTime () );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}

// respawnVehicle( vehicleid );
SQInteger CVehicleNatives::RespawnVehicle ( SQVM * pVM )
{
	SQInteger vehicleId;
	sq_getinteger ( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if ( pCore->GetVehicleManager()->IsActive ( vehicleId ) )
	{
		// Respawn the vehicle
		pCore->GetVehicleManager()->Get( vehicleId )->RespawnForWorld ();

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}