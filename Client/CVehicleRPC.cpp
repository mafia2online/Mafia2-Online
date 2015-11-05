/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CVehicleRPC.h"

extern	CCore			* pCore;
bool	CVehicleRPC::m_bRegistered = false;

void SetVehiclePosition( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the position
	CVector3 vecPosition;
	pBitStream->Read( vecPosition );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
		pCore->GetVehicleManager()->Get( vehicleId )->SetPosition( vecPosition );
	}
}

void SetVehicleDirection( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the direction
	CVector3 vecRotation;
	pBitStream->Read( vecRotation );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle direction
		pCore->GetVehicleManager()->Get( vehicleId )->SetRotation( vecRotation );
	}
}

void SetVehicleColour( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the primary colour
	int r1, g1, b1;
	pBitStream->ReadCompressed( r1 );
	pBitStream->ReadCompressed( g1 );
	pBitStream->ReadCompressed( b1 );

	// Read the secondary colour
	int r2, g2, b2;
	pBitStream->ReadCompressed( r2 );
	pBitStream->ReadCompressed( g2 );
	pBitStream->ReadCompressed( b2 );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle colour
		pCore->GetVehicleManager()->Get( vehicleId )->SetColour( CColor ( r1, g1, b1, 255 ), CColor ( r2, g2, b2, 255 ) );
	}
}

void SetVehiclePlateText( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the platetext
	RakNet::RakString sPlateText;
	pBitStream->Read( sPlateText );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle plate text
		pCore->GetVehicleManager()->Get( vehicleId )->SetPlateText( sPlateText.C_String() );
	}
}

void RepairVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Repair the vehicle
		pCore->GetVehicleManager()->Get( vehicleId )->Repair();
	}
}

void ExplodeVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Explode the vehicle
		pCore->GetVehicleManager()->Get( vehicleId )->Explode();
	}
}

void SetVehicleDirtLevel( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the dirt level
	float fDirtLevel;
	pBitStream->Read( fDirtLevel );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle dirt level
		pCore->GetVehicleManager()->Get( vehicleId )->SetDirtLevel( fDirtLevel );
	}
}

void SetVehicleEngineState( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the engine state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle engine state
		pCore->GetVehicleManager()->Get( vehicleId )->SetEngineState( bState );
	}
}

void SetVehiclePartOpen( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the seat id
	int iSeat;
	pBitStream->ReadCompressed( iSeat );

	// Read the part state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle part open state
		// todo
	}
}

void SetVehicleSirenState( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the siren state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle siren state
		pCore->GetVehicleManager()->Get( vehicleId )->SetSirenState( bState );
	}
}

void SetVehicleHornState( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the horn state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle horn state
		pCore->GetVehicleManager()->Get( vehicleId )->SetHornState( bState );
	}
}

void SetVehicleWindowOpen( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the seat id
	int iSeat;
	pBitStream->ReadCompressed( iSeat );

	// Read the open state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle window state
		// todo
	}
}

void SetVehicleTuningTable( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the table id
	int iTable;
	pBitStream->ReadCompressed( iTable );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the tuning table
		pCore->GetVehicleManager()->Get( vehicleId )->SetTuningTable( iTable );
	}
}

void SetVehicleWheelTexture( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the wheel id
	int iWheelIndex;
	pBitStream->ReadCompressed( iWheelIndex );

	// Read the texture id
	int iTexture;
	pBitStream->ReadCompressed( iTexture );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the tuning table
		pCore->GetVehicleManager()->Get( vehicleId )->SetWheelTexture( iWheelIndex, iTexture );
	}
}

void SetVehicleSpeed( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the speed
	CVector3 vecSpeed;
	pBitStream->Read ( vecSpeed );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle speed
		pCore->GetVehicleManager()->Get( vehicleId )->SetSpeedVec ( vecSpeed );
	}
}

void SetVehicleFuel( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the fuel
	float fFuel;
	pBitStream->Read ( fFuel );

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicel fuel
		pCore->GetVehicleManager()->Get( vehicleId )->SetFuel ( fFuel );
	}
}

void SetVehicleLightState( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the light state
	bool bLightState = pBitStream->ReadBit ();

	// Does the vehicle exist?
	if( pCore->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicel light state
		pCore->GetVehicleManager()->Get( vehicleId )->SetLightState ( bLightState );
	}
}

void CVehicleRPC::Register( RakNet::RPC4 * pRPC )
{
	// Are we already registered?
	if( m_bRegistered )
		return;

	// Vehicle functions
	pRPC->RegisterFunction( RPC_SETVEHICLEPOS, SetVehiclePosition );
	pRPC->RegisterFunction( RPC_SETVEHICLEDIR, SetVehicleDirection );
	pRPC->RegisterFunction( RPC_SETVEHICLECOLOUR, SetVehicleColour );
	pRPC->RegisterFunction( RPC_SETVEHICLEPLATETEXT, SetVehiclePlateText );
	pRPC->RegisterFunction( RPC_REPAIRVEHICLE, RepairVehicle );
	pRPC->RegisterFunction( RPC_EXPLODEVEHICLE, ExplodeVehicle );
	pRPC->RegisterFunction( RPC_SETVEHICLEDIRTLEVEL, SetVehicleDirtLevel );
	pRPC->RegisterFunction( RPC_SETVEHICLEENGINESTATE, SetVehicleEngineState );
	pRPC->RegisterFunction( RPC_SETVEHICLEPARTOPEN, SetVehiclePartOpen );
	pRPC->RegisterFunction( RPC_SETVEHICLESIRENSTATE, SetVehicleSirenState );
	pRPC->RegisterFunction( RPC_SETVEHICLEHORNSTATE, SetVehicleHornState );
	pRPC->RegisterFunction( RPC_SETVEHICLEWINDOWOPEN, SetVehicleWindowOpen );
	pRPC->RegisterFunction( RPC_SETVEHICLETUNINGTABLE, SetVehicleTuningTable );
	pRPC->RegisterFunction( RPC_SETVEHICLEWHEELTEXTURE, SetVehicleWheelTexture );
	pRPC->RegisterFunction( RPC_SETVEHICLESPEED, SetVehicleSpeed );
	pRPC->RegisterFunction( RPC_SETVEHICLEFUEL, SetVehicleFuel );
	pRPC->RegisterFunction( RPC_SETVEHICLELIGHTSTATE, SetVehicleLightState );
}

void CVehicleRPC::Unregister( RakNet::RPC4 * pRPC )
{
	// Are we not registered?
	if( !m_bRegistered )
		return;

	// Vehicle functions
	pRPC->UnregisterFunction( RPC_SETVEHICLEPOS );
	pRPC->UnregisterFunction( RPC_SETVEHICLEDIR );
	pRPC->UnregisterFunction( RPC_SETVEHICLECOLOUR );
	pRPC->UnregisterFunction( RPC_SETVEHICLEPLATETEXT );
	pRPC->UnregisterFunction( RPC_REPAIRVEHICLE );
	pRPC->UnregisterFunction( RPC_EXPLODEVEHICLE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEDIRTLEVEL );
	pRPC->UnregisterFunction( RPC_SETVEHICLEENGINESTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEPARTOPEN );
	pRPC->UnregisterFunction( RPC_SETVEHICLESIRENSTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEHORNSTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEWINDOWOPEN );
	pRPC->UnregisterFunction( RPC_SETVEHICLETUNINGTABLE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEWHEELTEXTURE );
	pRPC->UnregisterFunction( RPC_SETVEHICLESPEED );
	pRPC->UnregisterFunction( RPC_SETVEHICLEFUEL );
	pRPC->UnregisterFunction( RPC_SETVEHICLELIGHTSTATE );
}