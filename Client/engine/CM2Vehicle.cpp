/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Vehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "../StdInc.h"

extern	CCore		* pCore;

CM2Vehicle::CM2Vehicle( M2Vehicle * pVehicle ) : CM2Entity( pVehicle )
{
	DEBUG_TRACE("CM2Vehicle::CM2Vehicle");

	// Set the vehicle
	SetVehicle( pVehicle );

#ifdef _DEBUG
	CLogFile::Printf( "CM2Vehicle::CM2Vehicle( 0x%p )", pVehicle );
#endif

	// Prohibit the vehicle from entering garages (house garage)
	// todo: make this a function!
	BYTE byteFlags = pVehicle->m_byteFlags5;
	pVehicle->m_byteFlags1 |= 0x8000000;			// (&= 0xF7FFFFFFu;  to restore)
	pVehicle->m_byteFlags5 = byteFlags;

	// Unlock entry points
	byteFlags = pVehicle->m_byteFlags5;
	pVehicle->m_byteFlags1 &= 0xFFFFFDFF;
	pVehicle->m_byteFlags5 = byteFlags;

	// Unlock player entry points (Doesn't work!)
	DWORD dwFunc = 0x9BC150; // FUNC_CVehicle__UnlockPlayerEntryPoints
	_asm mov ecx, pVehicle;
	_asm call dwFunc;

	// Unlock
	dwFunc = 0x120DBB0;
	DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
	_asm push -1;
	_asm push 1;
	_asm mov ecx, dwVehicleData;
	_asm call dwFunc;

	dwFunc = 0x120E340;
	_asm push 0;
	_asm push 80000000h;
	_asm mov ecx, dwVehicleData;
	_asm call dwFunc;
}

CM2Vehicle::~CM2Vehicle( void )
{
	DEBUG_TRACE("CM2Vehicle::~CM2Vehicle");
}

void CM2Vehicle::SetEngineOn( bool bEngine, bool bRevOnStart )
{
	DEBUG_TRACE("CM2Vehicle::SetEngineOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm push bRevOnStart;
		_asm push bEngine;
		_asm mov ecx, dwVehicleData;
		_asm call COffsets::FUNC_CVehicle__SetEngineOn;
	}
}

bool CM2Vehicle::IsEngineOn( void )
{
	DEBUG_TRACE("CM2Vehicle::IsEngineOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x870) & 0x18000);
	}
	
	return false;
}

void CM2Vehicle::OpenHood( void )
{
	DEBUG_TRACE("CM2Vehicle::OpenHood");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__OpenHood;
	}
}

void CM2Vehicle::CloseHood( void )
{
	DEBUG_TRACE("CM2Vehicle::CloseHood");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__CloseHood;
	}
}

void CM2Vehicle::OpenTrunk( void )
{
	DEBUG_TRACE("CM2Vehicle::OpenTrunk");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__OpenTrunk;
	}
}

void CM2Vehicle::CloseTrunk( void )
{
	DEBUG_TRACE("CM2Vehicle::CloseTrunk");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__CloseTrunk;
	}
}

void CM2Vehicle::SetEngineDamage( float fEngineDamage )
{
	DEBUG_TRACE("CM2Vehicle::SetEngineDamage");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fEngineDamage;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetEngineDamage;
	}
}

float CM2Vehicle::GetEngineDamage( void )
{
	DEBUG_TRACE("CM2Vehicle::GetEngineDamage");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		float fEngineDamage = 0.0f;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__GetEngineDamage;
		_asm mov fEngineDamage, eax;

		return fEngineDamage;
	}

	return 0.0f;
}

void CM2Vehicle::SetSpeed( float fSpeed )
{
	DEBUG_TRACE("CM2Vehicle::SetSpeed");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fSpeed;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetSpeedFloat;
	}
}

float CM2Vehicle::GetSpeed( void )
{
	DEBUG_TRACE("CM2Vehicle::GetSpeed");

	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_fSpeed * 3.599999904632568);

	return 0.0f;
}

void CM2Vehicle::SetMoveSpeed ( CVector3 vecMoveSpeed )
{
	DEBUG_TRACE("CM2Vehicle::SetMoveSpeed");

	// Is the vehicle valid?
	if ( m_pVehicle )
	{
		DWORD FUNC_C_Vehicle__SetSpeedVec = 0x120E910;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm push 1;
		_asm lea ecx, vecMoveSpeed;
		_asm push ecx;
		_asm mov ecx, dwVehicleData;
		_asm call FUNC_C_Vehicle__SetSpeedVec;
	}
}

void CM2Vehicle::GetMoveSpeed ( CVector3 * vecMoveSpeed )
{
	DEBUG_TRACE("CM2Vehicle::GetMoveSpeed");

	// Is the vehicle valid?
	if ( m_pVehicle )
		memcpy ( vecMoveSpeed, &m_pVehicle->m_vecMoveSpeed, sizeof(CVector3) );
}

void CM2Vehicle::SetDirtLevel( float fDirtLevel )
{
	DEBUG_TRACE("CM2Vehicle::SetDirtLevel");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fDirtLevel;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetDirtLevel;
	}
}

float CM2Vehicle::GetDirtLevel( void )
{
	DEBUG_TRACE("CM2Vehicle::GetDirtLevel");

	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_fDirtLevel;

	return 0.0f;
}

// todo: not working fully (limiter is enabled, but quickly resets)
void CM2Vehicle::SetSpeedLimited( bool bSpeedLimited )
{
	DEBUG_TRACE("CM2Vehicle::SetSpeedLimited");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push bSpeedLimited;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetSpeedLimited;

		CLogFile::Printf( "CM2Vehicle::SetSpeedLimited - Setting unknown flags..." );

		// Set unk flags
		BYTE byteUnknown = pVehicle->m_pUnknown->m_pUnknown->m_byteUnknown;
		pVehicle->m_pUnknown->m_pUnknown->m_byteUnknown ^= (byteUnknown ^ 32 * bSpeedLimited) & 0x20;

		CLogFile::Printf( "Unknown flags set!" );
	}
}

bool CM2Vehicle::IsSpeedLimited( void )
{
	DEBUG_TRACE("CM2Vehicle::IsSpeedLimited");

	// Is the vehicle valid?
	if( m_pVehicle )
		if( m_pVehicle->m_byteFlags1 & 0x10000 )
			return true;

	return false;
}

void CM2Vehicle::SetSpeedLimiterSpeed( float fSpeed )
{
	DEBUG_TRACE("CM2Vehicle::SetSpeedLimiterSpeed");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Update the speed limiter speed
		m_pVehicle->m_fSpeedLimiterSpeed = (fSpeed / 3.599999904632568);

		// Is the speed limited?
		if( IsSpeedLimited() )
		{
			// Update the speed limiter gui
			*(float *)(m_pVehicle + 0x5B8) = m_pVehicle->m_fSpeedLimiterSpeed;
		}

		// Update unk flags
		m_pVehicle->m_pUnknown->m_pUnknown->m_byteUnknown |= 0x20u;
	}
}

float CM2Vehicle::GetSpeedLimiterSpeed( void )
{
	DEBUG_TRACE("CM2Vehicle::GetSpeedLimiterSpeed");

	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_fSpeedLimiterSpeed * 3.599999904632568);

	return 0.0f;
}

void CM2Vehicle::SetTuningTable( int iTuningTable )
{
	DEBUG_TRACE("CM2Vehicle::SetTuningTable");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push iTuningTable;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetActualTuningTable;
	}
}

int CM2Vehicle::GetTuningTable( void )
{
	DEBUG_TRACE("CM2Vehicle::GetTuningTable");

	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_iTuningTable - 2);

	return 0;
}

void CM2Vehicle::SetColour( CColor primary, CColor secondary )
{
	DEBUG_TRACE("CM2Vehicle::SetColour");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Copy the primary colour
		m_pVehicle->m_fPrimaryRed = (primary.R * 0.00390625);
		m_pVehicle->m_fPrimaryGreen = (primary.G * 0.00390625);
		m_pVehicle->m_fPrimaryBlue = (primary.B * 0.00390625);

		// Refresh the primary colour
		m_pVehicle->m_dwColourRefreshFlags |= 1u;

		// Copy the secondary colour
		m_pVehicle->m_fSecondaryRed = (secondary.R * 0.00390625);
		m_pVehicle->m_fSecondaryGreen = (secondary.G * 0.00390625);
		m_pVehicle->m_fSecondaryBlue = (secondary.B * 0.00390625);

		// Refresh the secondary colour
		m_pVehicle->m_dwColourRefreshFlags |= 2u;
	}
}

void CM2Vehicle::GetColour( CColor * primary, CColor * secondary )
{
	DEBUG_TRACE("CM2Vehicle::GetColour");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Copy the primary colour
		primary->R = (m_pVehicle->m_fPrimaryRed / 0.00390625);
		primary->G = (m_pVehicle->m_fPrimaryGreen / 0.00390625);
		primary->B = (m_pVehicle->m_fPrimaryBlue / 0.00390625);

		// Copy the secondary colour
		secondary->R = (m_pVehicle->m_fSecondaryRed / 0.00390625);
		secondary->G = (m_pVehicle->m_fSecondaryGreen / 0.00390625);
		secondary->B = (m_pVehicle->m_fSecondaryBlue / 0.00390625);
	}
}

void CM2Vehicle::SetFuel( float fFuel )
{
	DEBUG_TRACE("CM2Vehicle::SetFuel");

	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
	{
		// Get the fuel tank capacity
		float fCapacity = GetFuelTankCapacity();

		// Calculate the new fuel
		fFuel -= fCapacity;

		// Set the fuel amount
		m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuel = fFuel;
	}
}

float CM2Vehicle::GetFuel( void )
{
	DEBUG_TRACE("CM2Vehicle::GetFuel");

	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
		return m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuel;

	return 0.0;
}

float CM2Vehicle::GetFuelTankCapacity( void )
{
	DEBUG_TRACE("CM2Vehicle::GetFuelTankCapacity");

	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
		return m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuelTankCapacity;

	return 0.0f;
}

void CM2Vehicle::SetHornOn( bool bHornOn )
{
	DEBUG_TRACE("CM2Vehicle::SetHornOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		DWORD dwFunc = m_pVehicle->m_pVFTable->SetHornOn;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push 1;
		_asm push bHornOn;
		_asm mov ecx, pVehicle;
		_asm call dwFunc;
	}
}

bool CM2Vehicle::IsHornOn( void )
{
	DEBUG_TRACE("CM2Vehicle::IsHornOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x85C) & 1) == 1;
	}

	return false;
}

void CM2Vehicle::SetSirenOn( bool bSirenOn )
{
	DEBUG_TRACE("CM2Vehicle::SetSirenOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		// Switch the siren state
		if( bSirenOn )
			*(DWORD *)(dwVehicleData + 0x85C) |= 2u;
		else
			*(DWORD *)(dwVehicleData + 0x85C) &= 0xFFFFFFFDu;

		// Toggle the vehicle beacon light
		DWORD C_Vehicle__SetBeaconLightOn = 0x1203DD0;
		_asm push bSirenOn;
		_asm mov ecx, dwVehicleData;
		_asm call C_Vehicle__SetBeaconLightOn;
	}
}

bool CM2Vehicle::IsSirenOn( void )
{
	DEBUG_TRACE("CM2Vehicle::IsSirenOn");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x85C) & 2) == 2;
	}

	return false;
}

void CM2Vehicle::SetPlateText( const char * szText )
{
	DEBUG_TRACE("CM2Vehicle::SetPlateText");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwFunc = m_pVehicle->m_vehicleData.m_pVFTable->SetSPZText;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm push 0;
		_asm push szText;
		_asm mov ecx, dwVehicleData;
		_asm call dwFunc;
	}
}

const char * CM2Vehicle::GetPlateText( void )
{
	DEBUG_TRACE("CM2Vehicle::GetPlateText");

	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_szPlateText;

	return "M2MP1B";
}

void CM2Vehicle::SetVehiclePart( int iPart, bool bToggle )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwFunc = (bToggle ? COffsets::FUNC_CVehicle__SetOnPart : COffsets::FUNC_CVehicle__SetOffPart);
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		//
		_asm push iPart;
		_asm mov ecx, dwVehicleData;
		_asm call dwFunc;
	}
}

bool CM2Vehicle::GetVehiclePart( int iPart )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get a pointer to the vehicle part
		DWORD dwVehicleParts = (DWORD)m_pVehicle->m_vehicleData.m_pVehicleParts;
		M2VehiclePart * pPart = (M2VehiclePart *)(*(DWORD *)(dwVehicleParts + (iPart * 4)));

		// Is the part valid?
		if( pPart )
			return (pPart->m_byteFlags == 4);
	}

	return false;
}

void CM2Vehicle::Repair( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push 0;
		_asm push 0; clean dirt
		_asm push 1;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__Repair;
	}
}

void CM2Vehicle::Explode( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__Explode;
	}
}

void CM2Vehicle::AddSteer( float fSteer )
{
	DEBUG_TRACE("CM2Vehicle::AddSteer");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwFunc = 0x11FB4E0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		float steer = (fSteer / m_pVehicle->m_fMaxSteerAngle);

		_asm push steer;
		_asm mov ecx, dwVehicleData;
		_asm call dwFunc;
	}
}

float CM2Vehicle::GetAddedSteer( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_fAddedSteer;
	
	return 0.0f;
}

float CM2Vehicle::GetSteer( void )
{
	DEBUG_TRACE("CM2Vehicle::GetSteer");

	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_fSteer;

	return 0.0f;
}

void CM2Vehicle::SetSpentMoney( float fSpentMoney )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		m_pVehicle->m_fSpentMoney = fSpentMoney;
}

float CM2Vehicle::GetSpentMoney( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_fSpentMoney;

	return 0.0f;
}

void CM2Vehicle::SetPower( bool bPower )
{
	DEBUG_TRACE("CM2Vehicle::SetPower");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetPower = 0x120E720;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		float fPower = 0.0f;

		if( bPower )
			fPower = 1.0f;

		_asm push fPower;
		_asm mov ecx, dwVehicleData;
		_asm call C_Vehicle__SetPower;
	}
}

bool CM2Vehicle::GetPower( void )
{
	DEBUG_TRACE("CM2Vehicle::GetPower");

	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_fPower == 1.0f);

	return false;
}

void CM2Vehicle::SetBrake( bool bBrake )
{
	DEBUG_TRACE("CM2Vehicle::SetBrake");

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetBrake = 0x120E7D0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		float fBrake = 0.0f;

		if( bBrake )
			fBrake = 1.0f;

		_asm push fBrake;
		_asm mov ecx, dwVehicleData;
		_asm call C_Vehicle__SetBrake;
	}
}

bool CM2Vehicle::GetBrake( void )
{
	DEBUG_TRACE("CM2Vehicle::GetBrake");

	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_fBrake == 1.0f);

	return false;
}

void CM2Vehicle::SetWheelsProtected( bool bProtected )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Are we protecting the wheels?
		if( bProtected )
		{
			// Protect the wheels
			BYTE byteFlags = (m_pVehicle->m_byteFlags5);
			m_pVehicle->m_byteFlags1 |= 0x80000;
			m_pVehicle->m_byteFlags5 = byteFlags;
		}
		else
		{
			// Unprotect the wheels
			BYTE byteFlags = (m_pVehicle->m_byteFlags5);
			m_pVehicle->m_byteFlags1 &= 0xFFF7FFFF;
			m_pVehicle->m_byteFlags5 = byteFlags;
		}
	}
}

bool CM2Vehicle::AreWheelsProtected( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_byteFlags1 & 0x80000);

	return false;
}

void CM2Vehicle::FixPosition( bool bFix )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD C_Vehicle__FixCarPos = 0x480580;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push bFix;
		_asm mov ecx, pVehicle;
		_asm call C_Vehicle__FixCarPos;
	}
}

bool CM2Vehicle::IsPositionFixed( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		CLogFile::Printf( "CM2Vehicle::IsPositionFixed() - 0x%p", *(DWORD *)(m_pVehicle + 0xD98) );
		return (*(DWORD *)(m_pVehicle + 0xD98) != NULL);
	}

	return false;
}

void CM2Vehicle::SetHandbrake ( bool bHandbrake )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Release the vehicle handbrake
		DWORD C_Vehicle__SetHandbrake = 0x11FB6D0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		_asm push 0;
		_asm push 0;
		_asm mov ecx, dwVehicleData;
		_asm call C_Vehicle__SetHandbrake;
	}
}

bool CM2Vehicle::IsHandbrakeOn ( void )
{
	// todo
	return false;
}

int CM2Vehicle::GetTotalSeats( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD dwFunc = m_pVehicle->m_pVFTable->GetTotalSeats;
		M2Vehicle * pVehicle = m_pVehicle;
		int iSeats = 0;

		_asm mov ecx, pVehicle;
		_asm call dwFunc;
		_asm mov iSeats, eax;

		return iSeats;
	}

	return 0;
}

int CM2Vehicle::GetPedOccupiedSeat( M2Ped * pPed )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pSeats )
	{
		// Loop over all seats
		for( int i = 0; i < GetTotalSeats(); i++ )
		{
			// Get the vehicle seat instance
			M2VehicleSeat * pSeat = m_pVehicle->m_pSeats->m_pSeat[ i ];

			// Is the current seat valid?
			if( pSeat )
			{
				// Is the ped in this seat?
				if( pSeat->m_pOccupant == pPed )
					return i;
			}
		}
	}

	return 0;
}

M2Ped * CM2Vehicle::GetSeatOccupant( int iSeat )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pSeats )
	{
		// Fix the seat index
		iSeat--;

		// Did we fuck the seat index?
		if( iSeat < 0 )
			iSeat = 0;

		// Is the seat valid? (NOTE: internal seats are 0-3, GetTotalSeats returns 1-4)
		if( iSeat >= 0 && iSeat < GetTotalSeats() )
		{
			// Get the vehicle seat instance
			M2VehicleSeat * pSeat = m_pVehicle->m_pSeats->m_pSeat[ iSeat ];

			// Is the seat instance valid?
			if( pSeat )
				return pSeat->m_pOccupant;
		}
	}

	return NULL;
}

void CM2Vehicle::SetWheelTexture ( int iWheelIndex, const char * szTexture )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD C_Vehicle__SetWheelMatrixAtIndex = 0x9BE450;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push szTexture;
		_asm push iWheelIndex;
		_asm mov ecx, pVehicle;
		_asm call C_Vehicle__SetWheelMatrixAtIndex;
	}
}

const char * CM2Vehicle::GetWheelTexture ( int iWheelIndex )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		//
		DWORD C_Vehicle__GetWheelsMaterial = 0x990CF0;
		M2Vehicle * pVehicle = m_pVehicle;
		const char * szMaterial;

		// Check the pointer is valid, because the game doesn't and it can crash!
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		DWORD v4 = *(DWORD *)( *(DWORD *)( dwVehicleData + 0x230 ) + 0xBA8 * iWheelIndex );

#ifdef DEBUG
		CLogFile::Printf ( "v4: 0x%p", v4 );
#endif

		if ( !v4 )
			return "(null)";

		_asm push iWheelIndex;
		_asm mov ecx, pVehicle;
		_asm call C_Vehicle__GetWheelsMaterial;
		_asm mov szMaterial, eax;
		
		return szMaterial;
	}

	return "(null)";
}

void CM2Vehicle::SetLightState ( bool bLightState )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
	{
		// Toggle the lights
		M2Vehicle * pVehicle = m_pVehicle;
		DWORD dwVehicleData = (DWORD)(pVehicle) + 0xA8;
		DWORD C_Vehicle__SetLightState = 0x11EED00;
		int iLightState = (int)bLightState;

		// Unlock light state
		*(BYTE *)(dwVehicleData + 0x6F3) = 0;

		_asm {
			push 0;										front lights
			push iLightState;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetLightState;

			push 1;										license plate light
			push iLightState;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetLightState;
		}
	}
}

bool CM2Vehicle::GetLightState ( void )
{
	// Is the vehicle valid?
	if ( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x6F0) & 2);
	}

	return false;
}