/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CVehicleRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "BitStream.h"

#include "Math/CVector3.h"
#include "CColor.h"

#include "CVehicleManager.h"
#include "CNetworkVehicle.h"

#include "RPC4Plugin.h"
#include "Network/NetRPCs.h"

#include "CVehicleRPC.h"

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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle position
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetPosition(vecPosition);
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle direction
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetRotation(vecRotation);
	}
}

void SetVehicleColour( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the primary colour
	unsigned char r1, g1, b1;
	pBitStream->ReadCompressed( r1 );
	pBitStream->ReadCompressed( g1 );
	pBitStream->ReadCompressed( b1 );

	// Read the secondary colour
	unsigned char r2, g2, b2;
	pBitStream->ReadCompressed( r2 );
	pBitStream->ReadCompressed( g2 );
	pBitStream->ReadCompressed( b2 );

	// Does the vehicle exist?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle colour
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetColour(CColor(r1, g1, b1, 255), CColor(r2, g2, b2, 255));
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle plate text
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetPlateText( sPlateText.C_String() );
	}
}

void RepairVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Does the vehicle exist?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// We store the current lightState
		int oldState = CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetLightState();

		// We store the current moveSpeed
		float speed = CCore::Instance()->GetVehicleManager()->Get(vehicleId)->GetSpeed();

		// Repair the vehicle
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->Repair();

		// We restore the light state
		if (oldState){
			CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetLightState(true);
		}

		// We restore the speed
		if (speed > 0.0){
			CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetSpeed(speed);
		}
	}
}

void ExplodeVehicle( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Does the vehicle exist?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Explode the vehicle
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->Explode();
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle dirt level
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetDirtLevel( fDirtLevel );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle engine state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetEngineState( bState );
	}
}

void SetVehiclePartOpen( RakNet::BitStream * pBitStream, RakNet::Packet * pPacket )
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the part id
	int iPart;
	pBitStream->ReadCompressed( iPart );

	// Read the part state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetPartOpen(iPart, bState);
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle siren state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetSirenState( bState );
	}
}

void SetVehicleBeaconLightState(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	// Read the beacon light state
	bool bState = pBitStream->ReadBit();

	// Does the vehicle exist?
	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		// Set the vehicle siren state
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetBeaconLightState(bState);
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle horn state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetHornState( bState );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle window state
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetWindowOpen(iSeat, bState);
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the tuning table
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetTuningTable( iTable );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the tuning table
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetWheelTexture( iWheelIndex, iTexture );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicle speed
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetSpeedVec ( vecSpeed );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicel fuel
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetFuel ( fFuel );
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
	if( CCore::Instance()->GetVehicleManager()->IsActive( vehicleId ) )
	{
		// Set the vehicel light state
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetLightState ( bLightState );
	}
}

void SetVehicleModel(RakNet::BitStream * pBitStream, RakNet::Packet * Packet)
{
	//Read the vehicle id
	EntityId vehicleId;
	pBitStream->ReadCompressed( vehicleId );

	// Read the model
	int bModel;
	pBitStream->ReadCompressed(bModel);

	// Does the vehicle exist?
	if (CCore::Instance()->GetVehicleManager()->IsActive( vehicleId )){
		// Set the vehicle model
		CCore::Instance()->GetVehicleManager()->Get( vehicleId )->SetModel( bModel);
	}
}

void SetVehicleIndicatorLightState(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	int indicator;
	pBitStream->ReadCompressed(indicator);

	bool bLightState = pBitStream->ReadBit();

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetIndicatorLightState(indicator, bLightState);
	}
}

void SetVehicleTaxiLightState(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	bool bLightState = pBitStream->ReadBit();

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetTaxiLightState(bLightState);
	}
}

void SetVehicleHandbrake(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	EntityId vehicleId;
	pBitStream->ReadCompressed(vehicleId);

	bool bHandbrake = pBitStream->ReadBit();

	if (CCore::Instance()->GetVehicleManager()->IsActive(vehicleId))
	{
		CCore::Instance()->GetVehicleManager()->Get(vehicleId)->SetHandbrake(bHandbrake);
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
	pRPC->RegisterFunction( RPC_SETVEHICLEMODEL, SetVehicleModel);
	pRPC->RegisterFunction( RPC_SETVEHICLEDIRTLEVEL, SetVehicleDirtLevel );
	pRPC->RegisterFunction( RPC_SETVEHICLEENGINESTATE, SetVehicleEngineState );
	pRPC->RegisterFunction( RPC_SETVEHICLEPARTOPEN, SetVehiclePartOpen );
	pRPC->RegisterFunction( RPC_SETVEHICLESIRENSTATE, SetVehicleSirenState );
	pRPC->RegisterFunction( RPC_SETVEHICLEBEACONLIGHT, SetVehicleBeaconLightState);
	pRPC->RegisterFunction( RPC_SETVEHICLEHORNSTATE, SetVehicleHornState );
	pRPC->RegisterFunction( RPC_SETVEHICLEWINDOWOPEN, SetVehicleWindowOpen );
	pRPC->RegisterFunction( RPC_SETVEHICLETUNINGTABLE, SetVehicleTuningTable );
	pRPC->RegisterFunction( RPC_SETVEHICLEWHEELTEXTURE, SetVehicleWheelTexture );
	pRPC->RegisterFunction( RPC_SETVEHICLESPEED, SetVehicleSpeed );
	pRPC->RegisterFunction( RPC_SETVEHICLEFUEL, SetVehicleFuel );
	pRPC->RegisterFunction( RPC_SETVEHICLELIGHTSTATE, SetVehicleLightState );
	pRPC->RegisterFunction( RPC_SETVEHICLEINDICATORLIGHTSTATE, SetVehicleIndicatorLightState);
	pRPC->RegisterFunction( RPC_SETVEHICLETAXILIGHTSTATE, SetVehicleTaxiLightState);
	pRPC->RegisterFunction( RPC_SETVEHICLEHANDBRAKE, SetVehicleHandbrake);
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
	pRPC->UnregisterFunction( RPC_SETVEHICLEMODEL);
	pRPC->UnregisterFunction( RPC_SETVEHICLEDIRTLEVEL );
	pRPC->UnregisterFunction( RPC_SETVEHICLEENGINESTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEPARTOPEN );
	pRPC->UnregisterFunction( RPC_SETVEHICLESIRENSTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEBEACONLIGHT );
	pRPC->UnregisterFunction( RPC_SETVEHICLEHORNSTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEWINDOWOPEN );
	pRPC->UnregisterFunction( RPC_SETVEHICLETUNINGTABLE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEWHEELTEXTURE );
	pRPC->UnregisterFunction( RPC_SETVEHICLESPEED );
	pRPC->UnregisterFunction( RPC_SETVEHICLEFUEL );
	pRPC->UnregisterFunction( RPC_SETVEHICLELIGHTSTATE );
	pRPC->UnregisterFunction( RPC_SETVEHICLEINDICATORLIGHTSTATE);
	pRPC->UnregisterFunction( RPC_SETVEHICLETAXILIGHTSTATE);
}