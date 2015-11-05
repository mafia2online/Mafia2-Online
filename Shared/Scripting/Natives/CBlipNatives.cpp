/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CBlipNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CBlipNatives.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

void CBlipNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "createBlip", Create, 4, "ffii" );
	pScriptingManager->RegisterFunction( "destroyBlip", Destroy, 1, "i" );
	pScriptingManager->RegisterFunction( "attachBlipToPlayer", AttachToPlayer, 2, "ii" );
	pScriptingManager->RegisterFunction( "attachBlipToVehicle", AttachToVehicle, 2, "ii" );
	pScriptingManager->RegisterFunction( "attachBlipToPed", AttachToPed, 2, "ii" );
}

// createBlip ( fX, fY, library, icon );
SQInteger CBlipNatives::Create( SQVM * pVM )
{
	float fX, fY;
	SQInteger iLibrary, iIcon;

	sq_getfloat( pVM, -4, &fX );
	sq_getfloat( pVM, -3, &fY );
	sq_getinteger( pVM, -2, &iLibrary );
	sq_getinteger( pVM, -1, &iIcon );

	// Add the blip into the blip manager
#ifdef _CLIENT
	sq_pushinteger( pVM, pCore->GetBlipManager()->AddClientBlip( fX, fY, iLibrary, iIcon ) );
#else
	sq_pushinteger( pVM, pCore->GetBlipManager()->Add( fX, fY, iLibrary, iIcon ) );
#endif
	return 1;
}

// destroyBlip ( blipId );
SQInteger CBlipNatives::Destroy( SQVM * pVM )
{
	SQInteger blipId;
	sq_getinteger( pVM, -1, &blipId );

	// Remove this blip from the blip manager
#ifdef _CLIENT
	sq_pushbool( pVM, pCore->GetBlipManager()->RemoveClientBlip( blipId ) );
#else
	sq_pushbool( pVM, pCore->GetBlipManager()->Remove( blipId ) );
#endif
	return 1;
}

// attachBlipToPlayer ( blipId, playerId );
SQInteger CBlipNatives::AttachToPlayer ( SQVM * pVM )
{
	SQInteger blipId, playerId;
	sq_getinteger ( pVM, -2, &blipId );
	sq_getinteger ( pVM, -1, &playerId );

#ifdef _CLIENT
	if ( pCore->GetBlipManager()->IsClientBlipActive( blipId ) && pCore->GetPlayerManager()->IsActive( playerId ) )
#else
	if ( pCore->GetBlipManager()->IsActive( blipId ) && pCore->GetPlayerManager()->IsActive( playerId ) )
#endif
	{
		// Attach the blip to the player
#ifdef _CLIENT
		pCore->GetBlipManager()->GetClientBlip( blipId )->AttachToPlayer ( pCore->GetPlayerManager()->Get( playerId ) );
#else
		pCore->GetBlipManager()->Get( blipId )->AttachToPlayer ( pCore->GetPlayerManager()->Get( playerId ) );
#endif

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}

// attachBlipToVehicle ( blipId, vehicleId );
SQInteger CBlipNatives::AttachToVehicle ( SQVM * pVM )
{
	SQInteger blipId, vehicleId;
	sq_getinteger ( pVM, -2, &blipId );
	sq_getinteger ( pVM, -1, &vehicleId );

#ifdef _CLIENT
	if ( pCore->GetBlipManager()->IsClientBlipActive( blipId ) && pCore->GetVehicleManager()->IsActive( vehicleId ) )
#else
	if ( pCore->GetBlipManager()->IsActive( blipId ) && pCore->GetVehicleManager()->IsActive( vehicleId ) )
#endif
	{
		// Attach the blip to the vehicle
#ifdef _CLIENT
		pCore->GetBlipManager()->GetClientBlip( blipId )->AttachToVehicle ( pCore->GetVehicleManager()->Get( vehicleId ) );
#else
		pCore->GetBlipManager()->Get( blipId )->AttachToVehicle ( pCore->GetVehicleManager()->Get( vehicleId ) );
#endif

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}

// attachBlipToPed ( blipId, pedId );
SQInteger CBlipNatives::AttachToPed ( SQVM * pVM )
{
	SQInteger blipId, pedId;
	sq_getinteger ( pVM, -2, &blipId );
	sq_getinteger ( pVM, -1, &pedId );

#ifdef _CLIENT
	if ( pCore->GetBlipManager()->IsClientBlipActive( blipId ) && pCore->GetPedManager()->IsActive( pedId ) )
#else
	if ( pCore->GetBlipManager()->IsActive( blipId ) && pCore->GetPedManager()->IsActive( pedId ) )
#endif
	{
		// Attach the blip to the ped
#ifdef _CLIENT
		pCore->GetBlipManager()->GetClientBlip( blipId )->AttachToPed ( pCore->GetPedManager()->Get( pedId ) );
#else
		pCore->GetBlipManager()->Get( blipId )->AttachToPed ( pCore->GetPedManager()->Get( pedId ) );
#endif

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}