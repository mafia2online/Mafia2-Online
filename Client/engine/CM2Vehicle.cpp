/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Vehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"
#include "CChat.h"

#include "CM2Entity.h"
#include "CM2Vehicle.h"
#include "engine/CM2Ped.h"

#include "CColor.h"

#include "COffsets.h"

#include "CLogFile.h"

const DWORD C_Car__Spawn = 0x447820;
void _declspec(naked) M2Vehicle::Spawn(void)
{
	_asm jmp C_Car__Spawn;
}

void _declspec(naked) M2Vehicle::SetMotorDamage(float health)
{
	_asm jmp COffsets::FUNC_CVehicle__SetEngineDamage;
}

float _declspec(naked) M2Vehicle::GetMotorDamage(void) const
{
	_asm jmp COffsets::FUNC_CVehicle__GetEngineDamage;
}

const DWORD C_Car__GetDamage = 0x09A9A70;
long double _declspec(naked) M2Vehicle::GetDamage(void) const
{
	_asm jmp C_Car__GetDamage;
}

CM2Vehicle::CM2Vehicle( M2Vehicle * pVehicle ) : CM2Entity( pVehicle )
{
	SetVehicle( pVehicle );

	if (!m_pVehicle)
		return;

	ToggleGarageEnter(false);

	Spawn();
}

CM2Vehicle::~CM2Vehicle( void )
{
}

void CM2Vehicle::Spawn ( void )
{
	if ( m_pVehicle )
		m_pVehicle->Spawn();
}

void CM2Vehicle::SetEngineOn( bool bEngine, bool bRevOnStart )
{
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm
		{
			push bRevOnStart;
			push bEngine;
			mov ecx, dwVehicleData;
			call COffsets::FUNC_CVehicle__SetEngineOn;
		}
	}
}

bool CM2Vehicle::IsEngineOn( void )
{
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x870) & 0x18000);
	}

	return false;
}

void CM2Vehicle::OpenHood(void)
{
	if (m_pVehicle)
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__OpenHood;
		}
	}
}

void CM2Vehicle::CloseHood( void )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__CloseHood;
		}
	}
}

void CM2Vehicle::OpenTrunk( void )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__OpenTrunk;
		}
	}
}

void CM2Vehicle::CloseTrunk( void )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__CloseTrunk;
		}
	}
}

void CM2Vehicle::SetEngineDamage( float fEngineDamage )
{
	if( m_pVehicle )
		m_pVehicle->SetMotorDamage(fEngineDamage);
}

float CM2Vehicle::GetEngineDamage( void )
{
	if( m_pVehicle )
		return m_pVehicle->GetMotorDamage();

	return (0.0f);
}

long double CM2Vehicle::GetDamage(void)
{
	if (m_pVehicle)
		return m_pVehicle->GetDamage();

	return (0.0);
}

void CM2Vehicle::SetSpeed( float fSpeed )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push fSpeed;
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__SetSpeedFloat;
		}
	}
}

float CM2Vehicle::GetSpeed( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fSpeed * 3.599999904632568);

	return 0.0f;
}

void CM2Vehicle::SetMoveSpeed ( CVector3 vecMoveSpeed )
{
	if ( m_pVehicle )
	{
		DWORD FUNC_C_Vehicle__SetSpeedVec = 0x120E910;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm
		{
			push 1;
			lea ecx, vecMoveSpeed;
			push ecx;
			mov ecx, dwVehicleData;
			call FUNC_C_Vehicle__SetSpeedVec;
		}
	}
}

void CM2Vehicle::GetMoveSpeed ( CVector3 * vecMoveSpeed )
{
	if ( m_pVehicle )
		memcpy ( vecMoveSpeed, &m_pVehicle->m_vecMoveSpeed, sizeof(CVector3) );
}

void CM2Vehicle::SetDirtLevel( float fDirtLevel )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push fDirtLevel;
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__SetDirtLevel;
		}
	}
}

float CM2Vehicle::GetDirtLevel( void )
{
	if( m_pVehicle )
		return m_pVehicle->m_fDirtLevel;

	return 0.0f;
}

// todo: not working fully (limiter is enabled, but quickly resets)
void CM2Vehicle::SetSpeedLimited( bool bSpeedLimited )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push bSpeedLimited;
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__SetSpeedLimited;
		}

		DEBUG_LOG( "CM2Vehicle::SetSpeedLimited - Setting unknown flags..." );

		// Set unk flags
		BYTE byteUnknown = pVehicle->m_pUnknown->m_pUnknown->m_byteUnknown;
		pVehicle->m_pUnknown->m_pUnknown->m_byteUnknown ^= (byteUnknown ^ 32 * bSpeedLimited) & 0x20;

		DEBUG_LOG( "Unknown flags set!" );
	}
}

bool CM2Vehicle::IsSpeedLimited( void )
{
	if( m_pVehicle )
		if( m_pVehicle->m_byteFlags1 & 0x10000 )
			return true;

	return false;
}

void CM2Vehicle::SetSpeedLimiterSpeed( float fSpeed )
{
	if( m_pVehicle )
	{
		// Update the speed limiter speed
		m_pVehicle->m_fSpeedLimiterSpeed = (fSpeed / 3.599999904632568);

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
	if( m_pVehicle )
		return (m_pVehicle->m_fSpeedLimiterSpeed * 3.599999904632568);

	return 0.0f;
}

void CM2Vehicle::SetTuningTable( int iTuningTable )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push iTuningTable;
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__SetActualTuningTable;
		}
	}
}

int CM2Vehicle::GetTuningTable( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_iTuningTable - 2);

	return 0;
}

int _declspec(naked) C_Vehicle::SetVehicleColor(const C_Vector &primary, const C_Vector &secondary)
{
	_asm {
		mov eax, 0x11EE9A0
		jmp eax
	}
}

int _declspec(naked) C_Vehicle::GetVehicleColor(C_Vector *const primary, C_Vector *const secondary)
{
	_asm {
		mov eax, 0x11EEA00
		jmp eax
	}
}

const float MAGIC_COLOR_MULTIPLIER = 0.00390625f; // 1/255

void CM2Vehicle::SetColour( const CColor &thePrimary, const CColor &theSecondary )
{
	if( m_pVehicle )
	{
		C_Vector primary;
		primary.r = (thePrimary.R * MAGIC_COLOR_MULTIPLIER);
		primary.g = (thePrimary.G * MAGIC_COLOR_MULTIPLIER);
		primary.b = (thePrimary.B * MAGIC_COLOR_MULTIPLIER);

		C_Vector secondary;
		secondary.r = (theSecondary.R * MAGIC_COLOR_MULTIPLIER);
		secondary.g = (theSecondary.G * MAGIC_COLOR_MULTIPLIER);
		secondary.b = (theSecondary.B * MAGIC_COLOR_MULTIPLIER);

		m_pVehicle->m_vehicleData.SetVehicleColor(primary, secondary);
	}
}

void CM2Vehicle::GetColour( CColor * outPrimary, CColor * outSecondary )
{
	if( m_pVehicle )
	{
		C_Vector primary,secondary;
		m_pVehicle->m_vehicleData.GetVehicleColor(&primary, &secondary);

		outPrimary->R = (primary.r / MAGIC_COLOR_MULTIPLIER);
		outPrimary->G = (primary.g / MAGIC_COLOR_MULTIPLIER);
		outPrimary->B = (primary.b / MAGIC_COLOR_MULTIPLIER);

		outSecondary->R = (secondary.r / MAGIC_COLOR_MULTIPLIER);
		outSecondary->G = (secondary.g / MAGIC_COLOR_MULTIPLIER);
		outSecondary->B = (secondary.b / MAGIC_COLOR_MULTIPLIER);
	}
}

void CM2Vehicle::SetFuel( float fFuel )
{
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
	{
		// Get the fuel tank capacity
		//float fCapacity = GetFuelTankCapacity(); // It doesn't always give the correct value

		// Calculate the new fuel
		//fFuel -= fCapacity;

		// Set the fuel amount
		m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuel = fFuel;
	}
}

float CM2Vehicle::GetFuel( void )
{
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
		return m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuel;

	return 0.0;
}

float CM2Vehicle::GetFuelTankCapacity( void )
{
	if( m_pVehicle && m_pVehicle->m_pFuelUnk )
		return m_pVehicle->m_pFuelUnk->m_pUnknown->m_pFuelTank->m_fFuelTankCapacity;

	return 0.0f;
}

void CM2Vehicle::SetHornOn( bool bHornOn )
{
	if( m_pVehicle )
	{
		DWORD dwFunc = m_pVehicle->m_pVFTable->SetHornOn;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push 1;
			push bHornOn;
			mov ecx, pVehicle;
			call dwFunc;
		}
	}
}

bool CM2Vehicle::IsHornOn( void )
{
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x85C) & 1) == 1;
	}

	return false;
}

void CM2Vehicle::SetSirenOn( bool bSirenOn )
{
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		if( bSirenOn )
			*(DWORD *)(dwVehicleData + 0x85C) |= 2u;
		else
			*(DWORD *)(dwVehicleData + 0x85C) &= 0xFFFFFFFDu;
	}
}

void CM2Vehicle::SetBeaconLightOn(bool bOn)
{
	if (m_pVehicle)
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle)+0xA8;

		DWORD C_Vehicle__SetBeaconLightOn = 0x1203DD0;
		_asm
		{
			push bOn;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetBeaconLightOn;
		}
	}
}

bool CM2Vehicle::IsSirenOn( void )
{
	if( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x85C) & 2) == 2;
	}

	return false;
}

bool CM2Vehicle::IsBeaconLightOn(void)
{
	if (m_pVehicle)
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle)  + 0xA8;
		DWORD retn = (*(DWORD *)(dwVehicleData + 0x6F0 - 1) & 0x40);
		return (retn);
	}

	return (false);
}

void CM2Vehicle::SetPlateText( const char * szText )
{
	if( m_pVehicle )
	{
		DWORD dwFunc = m_pVehicle->m_vehicleData.m_pVFTable->SetSPZText;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm
		{
			push 0;
			push szText;
			mov ecx, dwVehicleData;
			call dwFunc;
		}
	}
}

const char * CM2Vehicle::GetPlateText( void )
{
	if( m_pVehicle )
		return m_pVehicle->m_szPlateText;

	return ("Unk");
}

void CM2Vehicle::SetVehiclePart( int iPart, bool bToggle )
{
	if( m_pVehicle )
	{
		DWORD dwFunc = (bToggle ? COffsets::FUNC_CVehicle__SetOnPart : COffsets::FUNC_CVehicle__SetOffPart);
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm
		{
			push iPart;
			mov ecx, dwVehicleData;
			call dwFunc;
		}
	}
}

bool CM2Vehicle::GetVehiclePart( int iPart )
{
	if( m_pVehicle )
	{
		// Get a pointer to the vehicle part
		DWORD dwVehicleParts = (DWORD)m_pVehicle->m_vehicleData.m_pVehicleParts;
		M2VehiclePart * pPart = (M2VehiclePart *)(*(DWORD *)(dwVehicleParts + (iPart * 4)));

		if( pPart )
			return (pPart->m_byteFlags == 4);
	}

	return false;
}

void CM2Vehicle::Repair( int cleanDirt )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push 0;
			push cleanDirt;
			push 1;
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__Repair;
		}
	}
}

void CM2Vehicle::Explode( void )
{
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			mov ecx, pVehicle;
			call COffsets::FUNC_CVehicle__Explode;
		}
	}
}

void CM2Vehicle::AddSteer( float fSteer )
{
	if( m_pVehicle )
	{
		DWORD dwFunc = 0x11FB4E0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		float steer = (fSteer / m_pVehicle->m_fMaxSteerAngle);

		_asm
		{
			push steer;
			mov ecx, dwVehicleData;
			call dwFunc;
		}
	}
}

float CM2Vehicle::GetAddedSteer( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fAddedSteer);

	return (0.0f);
}

float CM2Vehicle::GetSteer( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fSteer);

	return (0.0f);
}

void CM2Vehicle::SetSpentMoney( float fSpentMoney )
{
	if( m_pVehicle )
		m_pVehicle->m_fSpentMoney = fSpentMoney;
}

float CM2Vehicle::GetSpentMoney( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fSpentMoney);

	return (0.0f);
}

void CM2Vehicle::SetPower( bool bPower )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetPower = 0x120E720;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		float fPower = 0.0f;

		if( bPower )
			fPower = 1.0f;

		_asm
		{
			push fPower;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetPower;
		}
	}
}

bool CM2Vehicle::GetPower( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fPower == 1.0f);

	return (false);
}

void CM2Vehicle::SetBrake( bool bBrake )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetBrake = 0x120E7D0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		float fBrake = 0.0f;

		if( bBrake )
			fBrake = 1.0f;

		_asm
		{
			push fBrake;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetBrake;
		}
	}
}

bool CM2Vehicle::GetBrake( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_fBrake == 1.0f);

	return (false);
}

void CM2Vehicle::SetWheelsProtected( bool bProtected )
{
	if( m_pVehicle )
	{
		// Are we protecting the wheels?
		if( bProtected )
		{
			BYTE byteFlags = (m_pVehicle->m_byteFlags5);
			m_pVehicle->m_byteFlags1 |= 0x80000;
			m_pVehicle->m_byteFlags5 = byteFlags;
		}
		else
		{
			BYTE byteFlags = (m_pVehicle->m_byteFlags5);
			m_pVehicle->m_byteFlags1 &= 0xFFF7FFFF;
			m_pVehicle->m_byteFlags5 = byteFlags;
		}
	}
}

bool CM2Vehicle::AreWheelsProtected( void )
{
	if( m_pVehicle )
		return (m_pVehicle->m_byteFlags1 & 0x80000);

	return (false);
}

void CM2Vehicle::FixPosition( bool bFix )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__FixCarPos = 0x480580;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push bFix;
			mov ecx, pVehicle;
			call C_Vehicle__FixCarPos;
		}
	}
}

bool CM2Vehicle::IsPositionFixed( void )
{
	if( m_pVehicle )
		return (*(DWORD *)(m_pVehicle + 0xD98) != NULL);

	return (false);
}

void CM2Vehicle::SetHandbrake ( bool bHandbrake )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetHandbrake = 0x11FB6D0;
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;

		_asm
		{
			push 0;
			push 0;
			mov ecx, dwVehicleData;
			call C_Vehicle__SetHandbrake;
		}
	}
}

bool CM2Vehicle::IsHandbrakeOn ( void )
{
	// todo
	return false;
}

int CM2Vehicle::GetTotalSeats( void )
{
	if( m_pVehicle )
	{
		DWORD dwFunc = m_pVehicle->m_pVFTable->GetTotalSeats;
		M2Vehicle * pVehicle = m_pVehicle;
		int iSeats = 0;

		_asm
		{
			mov ecx, pVehicle;
			call dwFunc;
			mov iSeats, eax;
		}

		return (iSeats);
	}

	return (0);
}

int CM2Vehicle::GetPedOccupiedSeat( M2Ped * pPed )
{
	if( m_pVehicle && m_pVehicle->m_pSeats )
	{
		for( int i = 0; i < GetTotalSeats(); i++ )
		{
			M2VehicleSeat * pSeat = m_pVehicle->m_pSeats->m_pSeat[ i ];

			if( pSeat )
			{
				if( pSeat->m_pOccupant == pPed )
					return (i);
			}
		}
	}

	return (0);
}

M2Ped * CM2Vehicle::GetSeatOccupant( int iSeat )
{
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
			M2VehicleSeat * pSeat = m_pVehicle->m_pSeats->m_pSeat[ iSeat ];

			if( pSeat )
				return (pSeat->m_pOccupant);
		}
	}

	return (NULL);
}

void CM2Vehicle::SetWindowOpen(int iSeat, bool bState)
{
	if (m_pVehicle)
	{
		DWORD callOff = COffsets::FUNC_CVehicle__OpenSeatWindow;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push bState;
			push iSeat;
			mov ecx, pVehicle;
			call callOff;
		}
	}
}

bool CM2Vehicle::IsWindowOpen(int iSeat)
{
	if (m_pVehicle)
	{
		DWORD func = COffsets::FUNC_CVehicle__IsWindowOpen;
		M2Vehicle * pVehicle = m_pVehicle;

		int bRetn = false;

		_asm
		{
			push iSeat;
			mov ecx, pVehicle;
			call func;
			mov bRetn, eax;
		}
		return ((bool)bRetn);
	}

	return (false);
}

void CM2Vehicle::SetWheelTexture ( int iWheelIndex, const char * szTexture )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__SetWheelMatrixAtIndex = 0x9BE450;
		M2Vehicle * pVehicle = m_pVehicle;

		_asm
		{
			push szTexture;
			push iWheelIndex;
			mov ecx, pVehicle;
			call C_Vehicle__SetWheelMatrixAtIndex;
		}
	}
}

const char * CM2Vehicle::GetWheelTexture ( int iWheelIndex )
{
	if( m_pVehicle )
	{
		DWORD C_Vehicle__GetWheelsMaterial = 0x990CF0;
		M2Vehicle * pVehicle = m_pVehicle;
		const char * szMaterial;

		// Check the pointer is valid, because the game doesn't and it can crash!
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		DWORD v4 = *(DWORD *)( *(DWORD *)( dwVehicleData + 0x230 ) + 0xBA8 * iWheelIndex );

		if ( !v4 )
			return "(null)";

		_asm
		{
			push iWheelIndex;
			mov ecx, pVehicle;
			call C_Vehicle__GetWheelsMaterial;
			mov szMaterial, eax;
		}
		return (szMaterial);
	}

	return "(null)";
}

void CM2Vehicle::SetLightState ( bool bLightState )
{
	if ( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;
		DWORD dwVehicleData = (DWORD)(pVehicle) + 0xA8;
		DWORD C_Vehicle__SetLightState = 0x11EED00;
		int iLightState = (int)bLightState;

		// Unlock light state
		*(BYTE *)(dwVehicleData + 0x6F3) = 0;

		// Toggle the lights
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
	if ( m_pVehicle )
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle) + 0xA8;
		return (*(DWORD *)(dwVehicleData + 0x6F0) & 2);
	}

	return (false);
}

void CM2Vehicle::MarkForSale(bool bSale)
{
	if (m_pVehicle)
	{
		DWORD func = 0x0D649E0;
		M2Vehicle * pVehicle = m_pVehicle;
		DWORD dwVehicleData = (DWORD)(pVehicle)+0xA8;

		_asm
		{
			push bSale;
			mov ecx, dwVehicleData;
			call func;
		}
	}
}

bool CM2Vehicle::IsMarkedForSale()
{
	if (m_pVehicle)
	{
		DWORD dwVehicleData = (DWORD)(m_pVehicle)+0xA8;
		return (*(DWORD *)(dwVehicleData + 0xCAC) & 20);
	}
	return (false);
}

void CM2Vehicle::SetPainting(const char *paint)
{
	if (m_pVehicle)
	{
		DWORD dwFunc = 0x0446B70;
		M2Vehicle *pVeh = m_pVehicle;
		DWORD dwVeh = (DWORD)(pVeh) + 0xA8;

		__asm
		{
			push paint;
			mov ecx, dwVeh;
			call dwFunc;
		}
	}
}

void CM2Vehicle::ToggleGarageEnter(bool bToggle)
{
	if (!m_pVehicle)
		return;

	if (bToggle) // Allow to enter in garages
	{
		BYTE byteFlags = m_pVehicle->m_byteFlags5;
		m_pVehicle->m_byteFlags1 &= 0xF7FFFFFFu;
		m_pVehicle->m_byteFlags5 = byteFlags;
	}
	else
	{
		BYTE byteFlags = m_pVehicle->m_byteFlags5;
		m_pVehicle->m_byteFlags1 |= 0x8000000;
		m_pVehicle->m_byteFlags5 = byteFlags;
	}
}

void CM2Vehicle::SetIndicatorLightsOn(int indicator, int toggle)
{
	if (!m_pVehicle)
		return;

	M2Vehicle * pVehicle = m_pVehicle;
	DWORD dwVehicleData = (DWORD)(pVehicle)+0xA8;
	DWORD dwFunc = 0x1203D80;

	__asm
	{
		push indicator;
		push toggle;
		mov ecx, dwVehicleData;
		call dwFunc;
	}
}