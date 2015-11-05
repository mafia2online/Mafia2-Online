/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSharedVehicleNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSharedVehicleNatives.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern		CCore			* pCore;

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
	pScriptingManager->RegisterFunction( "setVehicleHornState", SetHornState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleHornState", GetHornState, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleWindowOpen", SetWindowOpen, 3, "iib" );
	pScriptingManager->RegisterFunction( "isVehicleWindowOpen", IsWindowOpen, 2, "ii" );
	pScriptingManager->RegisterFunction( "setVehicleTuningTable", SetTuningTable, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicleTuningTable", GetTuningTable, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleWheelTexture", SetWheelTexture, 3, "iii" );
	pScriptingManager->RegisterFunction( "getVehicleWheelTexture", GetWheelTexture, 2, "ii" );
	pScriptingManager->RegisterFunction( "getVehicleModel", GetModel, 1, "i" );
	pScriptingManager->RegisterFunction( "getVehicles", GetVehicles, 0, NULL );
	pScriptingManager->RegisterFunction( "setVehicleSpeed", SetSpeed, 3, "ifff" );
	pScriptingManager->RegisterFunction( "getVehicleSpeed", GetSpeed, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleFuel", SetFuel, 2, "if" );
	pScriptingManager->RegisterFunction( "getVehicleFuel", SetFuel, 1, "i" );
	pScriptingManager->RegisterFunction( "setVehicleLightState", SetLightState, 2, "ib" );
	pScriptingManager->RegisterFunction( "getVehicleLightState", GetLightState, 1, "i" );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
#ifdef _CLIENT
		pCore->GetVehicleManager()->Get( vehicleId )->SetPosition( vecPosition );
#else
		pCore->GetVehicleManager()->Get( vehicleId )->SetPosition( vecPosition, true );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle position
		CVector3 vecPosition;
		pCore->GetVehicleManager()->Get( vehicleId )->GetPosition( &vecPosition );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
#ifdef _CLIENT
		pCore->GetVehicleManager()->Get( vehicleId )->SetPosition( vecRotation );
#else
		pCore->GetVehicleManager()->Get( vehicleId )->SetRotation( vecRotation, true );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle position
		CVector3 vecRotation;
		pCore->GetVehicleManager()->Get( vehicleId )->GetRotation( &vecRotation );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle colour
		pCore->GetVehicleManager()->Get( vehicleId )->SetColour( CColor ( r_1, g_1, b_1, 255 ), CColor ( r_2, g_2, b_2, 255 ) );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle colour
		CColor primary, secondary;
		pCore->GetVehicleManager()->Get( vehicleId )->GetColour( &primary, &secondary );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle plate text
		pCore->GetVehicleManager()->Get( vehicleId )->SetPlateText( szPlateText );

		sq_pushbool( pVM, true );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle plate text
		const char * szText = pCore->GetVehicleManager()->Get( vehicleId )->GetPlateText();

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Repair the vehicle
		pCore->GetVehicleManager()->Get( vehicleId )->Repair();

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle dirt level
		pCore->GetVehicleManager()->Get( vehicleId )->SetDirtLevel( fDirtLevel );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle dirt level
		sq_pushfloat( pVM,  pCore->GetVehicleManager()->Get( vehicleId )->GetDirtLevel() );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the engine state
		pCore->GetVehicleManager()->Get( vehicleId )->SetEngineState( bState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the engine state
		sq_pushbool( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetEngineState() );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Explode the vehicle
		pCore->GetVehicleManager()->Get( vehicleId )->Explode();

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Toggle the vehicle part
		pCore->GetVehicleManager()->Get( vehicleId )->SetPartOpen( iPart, bState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, pCore->GetVehicleManager()->Get( vehicleId )->IsPartOpen( iPart ) );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the siren state
		pCore->GetVehicleManager()->Get( vehicleId )->SetSirenState( bState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetSirenState() );
		return 1;
	}

	sq_pushbool( pVM, false );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the horn state
		pCore->GetVehicleManager()->Get( vehicleId )->SetHornState( bState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetHornState() );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the window state
		pCore->GetVehicleManager()->Get( vehicleId )->SetWindowOpen( iSeat, bState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushbool( pVM, pCore->GetVehicleManager()->Get( vehicleId )->IsWindowOpen( iSeat ) );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		pCore->GetVehicleManager()->Get( vehicleId )->SetTuningTable( Math::Clamp< SQInteger >( 0, iTable, 3 ) );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetTuningTable() );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		pCore->GetVehicleManager()->Get( vehicleId )->SetWheelTexture( iWheelIndex, iTexture );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetWheelTexture( iWheelIndex ) );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		sq_pushinteger( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetModel () );
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
		if ( pCore->GetVehicleManager()->Get( vehicleId ) )
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle speed
		pCore->GetVehicleManager()->Get( vehicleId )->SetSpeedVec ( vecVelocity );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle speed
		CVector3 vecVelocity;
		pCore->GetVehicleManager()->Get( vehicleId )->GetSpeedVec ( &vecVelocity );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle fuel
		pCore->GetVehicleManager()->Get( vehicleId )->SetFuel ( fFuel );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle fuel
		sq_pushfloat ( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetFuel () );
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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle light state
		pCore->GetVehicleManager()->Get( vehicleId )->SetLightState ( bLightState );

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
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Get the vehicle light state
		sq_pushbool ( pVM, pCore->GetVehicleManager()->Get( vehicleId )->GetLightState () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}