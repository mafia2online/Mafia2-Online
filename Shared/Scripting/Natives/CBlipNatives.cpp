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
#include	"../../../Client/BaseInc.h"
#include	"../../../Client/CCore.h"
#include	"../../../Client/CBlipManager.h"
#include	"../../../Client/CNetworkPlayer.h"
#include	"../../../Client/CRemotePlayer.h"
#include	"../../../Client/CNetworkVehicle.h"
#include	"../../../Client/CPlayerManager.h"
#include	"../../../Client/CVehicleManager.h"
#include	"../../../Client/CPed.h"
#include	"../../../Client/CPedManager.h"
#else
#include	"../../../Client/BaseInc.h"
#include	"../../../Server/CCore.h"
#include	"../../../Server/CBlipManager.h"
#include	"../../../Server/CNetworkPlayer.h"
#include	"../../../Server/CNetworkVehicle.h"
#include	"../../../Server/CPlayerManager.h"
#include	"../../../Server/CVehicleManager.h"
#include	"../../../Server/CPed.h"
#include	"../../../Server/CPedManager.h"
#endif

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
	sq_pushinteger( pVM, CCore::Instance()->GetBlipManager()->AddClientBlip( fX, fY, iLibrary, iIcon ) );
#else
	sq_pushinteger( pVM, CCore::Instance()->GetBlipManager()->Add( fX, fY, iLibrary, iIcon ) );
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
	sq_pushbool( pVM, CCore::Instance()->GetBlipManager()->RemoveClientBlip( blipId ) );
#else
	sq_pushbool( pVM, CCore::Instance()->GetBlipManager()->Remove( blipId ) );
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
	if ( CCore::Instance()->GetBlipManager()->IsClientBlipActive( blipId ) && CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
#else
	if ( CCore::Instance()->GetBlipManager()->IsActive( blipId ) && CCore::Instance()->GetPlayerManager()->IsActive( playerId ) )
#endif
	{
		// Attach the blip to the player
#ifdef _CLIENT
		CCore::Instance()->GetBlipManager()->GetClientBlip( blipId )->AttachToPlayer ( CCore::Instance()->GetPlayerManager()->Get( playerId ) );
#else
		CCore::Instance()->GetBlipManager()->Get( blipId )->AttachToPlayer ( CCore::Instance()->GetPlayerManager()->Get( playerId ) );
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
	if ( CCore::Instance()->GetBlipManager()->IsClientBlipActive( blipId ) && CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
#else
	if ( CCore::Instance()->GetBlipManager()->IsActive( blipId ) && CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
#endif
	{
		// Attach the blip to the vehicle
#ifdef _CLIENT
		CCore::Instance()->GetBlipManager()->GetClientBlip( blipId )->AttachToVehicle ( CCore::Instance()->GetVehicleManager()->Get( vehicleId ) );
#else
		CCore::Instance()->GetBlipManager()->Get( blipId )->AttachToVehicle ( CCore::Instance()->GetVehicleManager()->Get( vehicleId ) );
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
	if ( CCore::Instance()->GetBlipManager()->IsClientBlipActive( blipId ) && CCore::Instance()->GetPedManager()->IsActive( pedId ) )
#else
	if ( CCore::Instance()->GetBlipManager()->IsActive( blipId ) && CCore::Instance()->GetPedManager()->IsActive( pedId ) )
#endif
	{
		// Attach the blip to the ped
#ifdef _CLIENT
		CCore::Instance()->GetBlipManager()->GetClientBlip( blipId )->AttachToPed ( CCore::Instance()->GetPedManager()->Get( pedId ) );
#else
		CCore::Instance()->GetBlipManager()->Get( blipId )->AttachToPed ( CCore::Instance()->GetPedManager()->Get( pedId ) );
#endif

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}