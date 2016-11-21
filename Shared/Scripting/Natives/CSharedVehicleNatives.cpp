/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedVehicleNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "Scripting/CScriptingManager.h"

#include "CSharedVehicleNatives.h"

#include "BaseInc.h"
#include "CCore.h"

#include "CVehicleManager.h"
#include "CNetworkVehicle.h"

#include "CString.h"
#include "CEvents.h"
#include "CCommands.h"
#include "Scripting/CSquirrelCommon.h"

#include "Math/CMaths.h"
#include "Math/CVector3.h"
#include "CColor.h"

void CSharedVehicleNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "setVehiclePosition", SetPosition, 4, "ifff" );
	pScriptingManager->RegisterFunction( "getVehiclePosition", GetPosition, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleRotation", SetRotation, 4, "ifff" );
	pScriptingManager->RegisterFunction( "getVehicleRotation", GetRotation, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleColour", SetColour, 7, "iiiiiii" );
	pScriptingManager->RegisterFunction( "getVehicleColour", GetColour, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehiclePlateText", SetPlateText, 2, "is" );
	pScriptingManager->RegisterFunction( "getVehiclePlateText", GetPlateText, 1, "i" );
	pScriptingManager->RegisterFunction( "repairVehicle", Repair, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleDirtLevel", SetDirtLevel, 2, "if" );
	pScriptingManager->RegisterFunction( "getVehicleDirtLevel", GetDirtLevel, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleEngineState", SetEngineState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleEngineState", GetEngineState, 1, "i" );
	pScriptingManager->RegisterFunction( "explodeVehicle", Explode, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehiclePartOpen", SetPartOpen, 3, "iib" );
	pScriptingManager->RegisterFunction( "isVehiclePartOpen", IsPartOpen, 2, "ii" );
	pScriptingManager->RegisterFunction( "setVehicleSirenState", SetSirenState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleSirenState", GetSirenState, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleBeaconLightState", SetBeaconLightState, 2, "ib");
	pScriptingManager->RegisterFunction( "getVehicleBeaconLightState", GetBeaconLightState, 1, "i");
	pScriptingManager->RegisterFunction( "setVehicleHornState", SetHornState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleHornState", GetHornState, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleWindowOpen", SetWindowOpen, 3, "iib" );
	pScriptingManager->RegisterFunction( "isVehicleWindowOpen", IsWindowOpen, 2, "ii" );
	pScriptingManager->RegisterFunction( "setVehicleTuningTable", SetTuningTable, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicleTuningTable", GetTuningTable, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleWheelTexture", SetWheelTexture, 3, "iii" );
	pScriptingManager->RegisterFunction( "getVehicleWheelTexture", GetWheelTexture, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicleModel", GetModel, 1, "i" );
	//pScriptingManager->RegisterFunction( "setVehicleModel", SetM, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicles", GetVehicles, 0, NULL );
	pScriptingManager->RegisterFunction( "setVehicleSpeed", SetSpeed, 4, "ifff" );
	pScriptingManager->RegisterFunction( "getVehicleSpeed", GetSpeed, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleFuel", SetFuel, 2, "if" );
	pScriptingManager->RegisterFunction( "getVehicleFuel", GetFuel, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleLightState", SetLightState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleLightState", GetLightState, 1, "i" );
	pScriptingManager->RegisterFunction( "setIndicatorLightState", SetIndicatorLightState, 3, "iib");
	pScriptingManager->RegisterFunction( "getIndicatorLightState", GetIndicatorLightState, 2, "ii");
	pScriptingManager->RegisterFunction( "setTaxiLightState", SetTaxiLightState, 2, "ib");
	pScriptingManager->RegisterFunction( "getTaxiLightState", GetTaxiLightState, 1, "i");
	pScriptingManager->RegisterFunction( "getVehicleHandbrake", GetVehicleHandbrake, 1, "i");
	pScriptingManager->RegisterFunction( "setVehicleHandbrake", SetVehicleHandbrake, 2, "ib");

	pScriptingManager->RegisterConstant( "INDICATOR_LEFT", 1);
	pScriptingManager->RegisterConstant( "INDICATOR_RIGHT", 0);
}

SQInteger CSharedVehicleNatives::SetPosition( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -4, &vehicleId );

	// Get the position
	CVector3 vecPosition;
	sq_getfloat( pVM, -1, &vecPosition.fZ );
	sq_getfloat( pVM, -2, &vecPosition.fY );
	sq_getfloat( pVM, -3, &vecPosition.fX );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
#ifdef _CLIENT
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPosition( vecPosition );
#else
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPosition( vecPosition, true );
#endif

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetPosition( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle position
		CVector3 vecPosition;
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetPosition( &vecPosition );

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

// setVehicleRotation ( vehicleid, x, y, z );
SQInteger CSharedVehicleNatives::SetRotation( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -4, &vehicleId );

	// Get the position
	CVector3 vecRotation;
	sq_getfloat( pVM, -1, &vecRotation.fZ );
	sq_getfloat( pVM, -2, &vecRotation.fY );
	sq_getfloat( pVM, -3, &vecRotation.fX );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
#ifdef _CLIENT
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPosition( vecRotation );
#else
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetRotation( vecRotation, true );
#endif

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getVehicleRotation ( vehicleid );
SQInteger CSharedVehicleNatives::GetRotation( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle position
		CVector3 vecRotation;
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetRotation( &vecRotation );

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

// setVehicleColour ( vehicleid, r, g, b, r, g, b );
SQInteger CSharedVehicleNatives::SetColour( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -7, &vehicleId );

	// Get the vehicle secondary colour
	SQInteger r_1, g_1, b_1;
	sq_getinteger( pVM, -6, &r_1 );
	sq_getinteger( pVM, -5, &g_1 );
	sq_getinteger( pVM, -4, &b_1 );

	// Get the vehicle secondary colour
	SQInteger r_2, g_2, b_2;
	sq_getinteger( pVM, -3, &r_2 );
	sq_getinteger( pVM, -2, &g_2 );
	sq_getinteger( pVM, -1, &b_2 );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle colour
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetColour( CColor ( r_1, g_1, b_1, 255 ), CColor ( r_2, g_2, b_2, 255 ) );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetColour( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle colour
		CColor primary, secondary;
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetColour( &primary, &secondary );

		sq_newarray( pVM, 0 );

		sq_pushinteger( pVM, primary.R );
		sq_arrayappend( pVM, -2 );

		sq_pushinteger( pVM, primary.G );
		sq_arrayappend( pVM, -2 );

		sq_pushinteger( pVM, primary.B );
		sq_arrayappend( pVM, -2 );

		sq_pushinteger( pVM, secondary.R );
		sq_arrayappend( pVM, -2 );

		sq_pushinteger( pVM, secondary.G );
		sq_arrayappend( pVM, -2 );

		sq_pushinteger( pVM, secondary.B );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetPlateText( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the vehicle plate text
	const char * szPlateText;
	sq_getstring( pVM, -1, &szPlateText );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPlateText( szPlateText ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetPlateText( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle plate text
		const char * szText = CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetPlateText();

		sq_pushstring( pVM, szText, strlen(szText) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::Repair( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Repair the vehicle
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->Repair();

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetDirtLevel( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the dirt level
	float fDirtLevel;
	sq_getfloat( pVM, -1, &fDirtLevel );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle dirt level
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetDirtLevel( fDirtLevel );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetDirtLevel( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle dirt level
		sq_pushfloat( pVM,  CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetDirtLevel() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetEngineState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the state
	SQBool bState;
	sq_getbool( pVM, -1, &bState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the engine state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetEngineState( bState );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetEngineState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the engine state
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetEngineState() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::Explode( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Explode the vehicle
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->Explode();

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetPartOpen( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -3, &vehicleId );

	// Get the part id
	SQInteger iPart;
	sq_getinteger( pVM, -2, &iPart );

	// Get the state
	SQBool bState;
	sq_getbool( pVM, -1, &bState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Toggle the vehicle part
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPartOpen( iPart, bState );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::IsPartOpen( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the part id
	SQInteger iPart;
	sq_getinteger( pVM, -1, &iPart );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->IsPartOpen( iPart ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetSirenState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the state
	SQBool bState;
	sq_getbool( pVM, -1, &bState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the siren state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetSirenState( bState );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetSirenState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetSirenState() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetBeaconLightState(SQVM * pVM)
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger(pVM, -2, &vehicleId);

	// Get the state
	SQBool bState;
	sq_getbool(pVM, -1, &bState);

	// Is the vehicle active?
	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		// Set the beacon light state
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetBeaconLightState(bState);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::GetBeaconLightState(SQVM * pVM)
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger(pVM, -1, &vehicleId);

	// Is the vehicle active?
	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		sq_pushbool(pVM, CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetBeaconLightState());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::SetHornState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the state
	SQBool bState;
	sq_getbool( pVM, -1, &bState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the horn state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetHornState( bState );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetHornState( SQVM * pVM )
{
	/// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetHornState() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetWindowOpen( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -3, &vehicleId );

	// Get the seat id
	SQInteger iSeat;
	sq_getinteger( pVM, -2, &iSeat );

	// Get the state
	SQBool bState;
	sq_getbool( pVM, -1, &bState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the window state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetWindowOpen( iSeat, bState );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::IsWindowOpen( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the seat id
	SQInteger iSeat;
	sq_getinteger( pVM, -1, &iSeat );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->IsWindowOpen( iSeat ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetTuningTable( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the table id
	SQInteger iTable;
	sq_getinteger( pVM, -1, &iTable );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetTuningTable( Math::Clamp< SQInteger >( 0, iTable, 3 ) );
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetTuningTable( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetTuningTable() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setVehicleWheelTexture( vehicleid, wheelid, textureid );
SQInteger CSharedVehicleNatives::SetWheelTexture( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -3, &vehicleId );

	// Get the wheel id
	SQInteger iWheelIndex;
	sq_getinteger( pVM, -2, &iWheelIndex );

	// Get the texture id
	SQInteger iTexture;
	sq_getinteger( pVM, -1, &iTexture );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetWheelTexture( iWheelIndex, iTexture );
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetWheelTexture( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the wheel id
	SQInteger iWheelIndex;
	sq_getinteger( pVM, -1, &iWheelIndex );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetWheelTexture( iWheelIndex ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::GetModel( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetModel () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getVehicles()
SQInteger CSharedVehicleNatives::GetVehicles( SQVM * pVM )
{
	sq_newarray( pVM, 0 );

	// Loop over every vehicle
	for ( int vehicleId = 0; vehicleId < MAX_VEHICLES; vehicleId++ )
	{
		// Is the current vehicle active?
		if ( CCore::Instance()->GetVehicleManager()->Get( vehicleId ) )
		{
			// Push the vehicle id into the array
			sq_pushinteger( pVM, vehicleId );
			sq_arrayappend( pVM, -2 );
		}
	}

	sq_push( pVM, -1 );
	return 1;
}

// setVehicleSpeed( vehicleid, speed );
SQInteger CSharedVehicleNatives::SetSpeed( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -4, &vehicleId );

	// Get the speed
	CVector3 vecVelocity;
	sq_getfloat( pVM, -1, &vecVelocity.fZ );
	sq_getfloat( pVM, -2, &vecVelocity.fY );
	sq_getfloat( pVM, -3, &vecVelocity.fX );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle speed
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetSpeedVec ( vecVelocity );

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getVehicleSpeed( vehicleid );
SQInteger CSharedVehicleNatives::GetSpeed( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle speed
		CVector3 vecVelocity;
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetSpeedVec ( &vecVelocity );

		sq_newarray( pVM, 0 );

		sq_pushfloat( pVM, vecVelocity.fX );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecVelocity.fY );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecVelocity.fZ );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setVehicleFuel( vehicleid, speed );
SQInteger CSharedVehicleNatives::SetFuel( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the fuel
	SQFloat fFuel;
	sq_getfloat( pVM, -1, &fFuel );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle fuel
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetFuel ( fFuel );

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getVehicleFuel( vehicleid );
SQInteger CSharedVehicleNatives::GetFuel( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle fuel
		sq_pushfloat ( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetFuel () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// setVehicleLightState( vehicleid, enabled );
SQInteger CSharedVehicleNatives::SetLightState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -2, &vehicleId );

	// Get the light state
	SQBool bLightState;
	sq_getbool( pVM, -1, &bLightState );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle light state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetLightState ( bLightState );

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// getVehicleLightState( vehicleid );
SQInteger CSharedVehicleNatives::GetLightState( SQVM * pVM )
{
	// Get the vehicle id
	SQInteger vehicleId;
	sq_getinteger( pVM, -1, &vehicleId );

	// Is the vehicle active?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle light state
		sq_pushbool ( pVM, CCore::Instance()->GetVehicleManager()->Get( vehicleId )->GetLightState () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CSharedVehicleNatives::SetIndicatorLightState(SQVM * pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -3, &vehicleId);

	SQInteger indicator;
	sq_getinteger(pVM, -2, &indicator);

	SQBool bLightState;
	sq_getbool(pVM, -1, &bLightState);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetIndicatorLightState(indicator, bLightState);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::GetIndicatorLightState(SQVM * pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -2, &vehicleId);

	SQInteger indicator;
	sq_getinteger(pVM, -1, &indicator);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		sq_pushbool(pVM, CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetIndicatorLightState(indicator));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::SetTaxiLightState(SQVM * pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -2, &vehicleId);

	SQBool bLightState;
	sq_getbool(pVM, -1, &bLightState);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetTaxiLightState(bLightState);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::GetTaxiLightState(SQVM * pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -1, &vehicleId);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		sq_pushbool(pVM, CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetTaxiLightState());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::SetVehicleHandbrake(SQVM *pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -2, &vehicleId);

	SQBool bHandbrake;
	sq_getbool(pVM, -1, &bHandbrake);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetHandbrake(bHandbrake);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger CSharedVehicleNatives::GetVehicleHandbrake(SQVM *pVM)
{
	SQInteger vehicleId;
	sq_getinteger(pVM, -1, &vehicleId);

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		sq_pushbool(pVM, CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetHandbrake());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}