/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Vehicle.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

CM2Vehicle::CM2Vehicle( M2VehicleBaseEntity * pBaseVehicle )
{
	// Set the base vehicle entity
	SetBaseVehicle( pBaseVehicle );

	// Set the vehicle entity
	SetVehicle( pBaseVehicle->m_pVehicle );
}

CM2Vehicle::CM2Vehicle( M2Vehicle * pVehicle )
{
	// Invalidate the base vehicle entity
	SetBaseVehicle( NULL );

	// Set the vehicle entity
	SetVehicle( pVehicle );

	pCore->GetChat()->Outputf( false, "Vehicle: 0x%p, GUID: %d, World: 0x%p", pVehicle, pVehicle->m_dwGUID, pVehicle->m_pWorld );
}

CM2Vehicle::~CM2Vehicle( void )
{
	
}

DWORD CM2Vehicle::GetGUID( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_dwGUID;

	return 0;
}

void CM2Vehicle::SetPosition( CVector3 vecPosition )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pWorld )
	{
		// todo
	}
}

void CM2Vehicle::GetPosition( CVector3 * vecPosition )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pWorld )
		memcpy( vecPosition, &CVector3( m_pVehicle->m_pWorld->m_matWorld._14, m_pVehicle->m_pWorld->m_matWorld._24, m_pVehicle->m_pWorld->m_matWorld._34 ), sizeof(CVector3) );
}

void CM2Vehicle::SetDirection( CVector3 vecDirection )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pWorld )
	{
		// todo
	}
}

void CM2Vehicle::GetDirection( CVector3 * vecDirection )
{
	// Is the vehicle valid?
	if( m_pVehicle && m_pVehicle->m_pWorld )
		memcpy( vecDirection, &CVector3( m_pVehicle->m_pWorld->m_matWorld._12, m_pVehicle->m_pWorld->m_matWorld._22, m_pVehicle->m_pWorld->m_matWorld._32 ), sizeof(CVector3) );
}

void CM2Vehicle::SetMoveSpeed( CVector3 vecMoveSpeed )
{
	// todo
}

void CM2Vehicle::GetMoveSpeed( CVector3 * vecMoveSpeed )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		memcpy( vecMoveSpeed, &m_pVehicle->m_vecMoveSpeed, sizeof(CVector3) );
}

void CM2Vehicle::SetSpeed( float fSpeed )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fSpeed;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetSpeedFloat;
	}
}

float CM2Vehicle::GetSpeed( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return (m_pVehicle->m_fSpeed * 3.599999904632568);

	return 0.0f;
}

bool CM2Vehicle::IsSpeedLimited( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		if( m_pVehicle->m_byteFlags1 & 0x10000 )
			return true;
	}

	return false;
}

void CM2Vehicle::OpenHood( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__OpenHood;
	}
}

void CM2Vehicle::CloseHood( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__CloseHood;
	}
}

void CM2Vehicle::OpenTrunk( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__OpenTrunk;
	}
}

void CM2Vehicle::CloseTrunk( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__CloseTrunk;
	}
}

void CM2Vehicle::SetDirtLevel( float fDirtLevel )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fDirtLevel;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetDirtLevel;
	}
}

float CM2Vehicle::GetDirtLevel( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_fDirtLevel;

	return 0.0f;
}

void CM2Vehicle::SetHandbrake( bool bToggle )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle data
		M2VehicleData * pVehicleData = m_pVehicle->m_pVehicleData;
		int iToggle = (int)bToggle;

		_asm push 0;
		_asm push iToggle;
		_asm mov ecx, pVehicleData;
		_asm call COffsets::FUNC_CVehicle__SetHandbrake;
	}
}

void CM2Vehicle::ToggleEngine( bool bToggle, bool bRevOnStart )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle data
		M2VehicleData * pVehicleData = m_pVehicle->m_pVehicleData;
		int iToggle = (int)bToggle;
		int iRevOnStart = (int)bRevOnStart;

		_asm push iRevOnStart;
		_asm push iToggle;
		_asm mov ecx, pVehicleData;
		_asm call COffsets::FUNC_CVehicle__SetEngineOn;
	}
}

void CM2Vehicle::SetTuningTable( int iTable )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push iTable;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetActualTuningTable;
	}
}

int CM2Vehicle::GetTuningTable( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
		return m_pVehicle->m_iTuningTable;

	return 0;
}

void CM2Vehicle::SetPlateText( const char * szText )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2VehicleData * pVehicleData = m_pVehicle->m_pVehicleData;
		char * szPlate = "";
		strcpy( szPlate, szText );

		_asm push 1;
		_asm push szPlate;
		_asm mov ecx, pVehicleData;
		_asm call COffsets::FUNC_CVehicle__SetSPZText;
	}
}

void CM2Vehicle::SetPainting( const char * szPainting )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle entity
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push szPainting;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetPainting;
	}
}

void CM2Vehicle::DisableExplosions( bool bDisable )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Store the current flags bytes
		BYTE byteFlags5 = m_pVehicle->m_byteFlags5;

		if( bDisable )
		{
			// Disable explosions
			//m_pVehicle->m_byteFlags1 |= sub_100000; (decompile error) - check CVehicleManager__DisableExplosions at 0xD75DC0
			m_pVehicle->m_byteFlags5 = byteFlags5;
		}
		else
		{
			// Enable explosions
			m_pVehicle->m_byteFlags1 &= 0xFEFFFFFFu;
			m_pVehicle->m_byteFlags5 = byteFlags5;
		}
	}
}

void CM2Vehicle::SetBeaconLightOn( bool bToggle )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle data pointer
		M2VehicleData * pVehicleData = m_pVehicle->m_pVehicleData;
		int iToggle = (int)bToggle;

		_asm push iToggle;
		_asm mov ecx, pVehicleData;
		_asm call COffsets::FUNC_CVehicle__SetBeaconLightOn;
	}
}

void CM2Vehicle::SetHornOn( bool bToggle )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle pointer
		M2Vehicle * pVehicle = m_pVehicle;
		DWORD dwFunc = m_pVehicle->m_pVFTable->SetHornOn;
		int iToggle = (int)bToggle;
		
		_asm push 1;
		_asm push iToggle;
		_asm mov ecx, pVehicle;
		_asm call dwFunc;
	}
}

void CM2Vehicle::SetWheelsProtected( bool bProteced )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Store the current flags bytes
		BYTE byteFlags5 = m_pVehicle->m_byteFlags5;

		if( bProteced )
		{
			// Set the wheels protected
			m_pVehicle->m_byteFlags1 |= 0x80000u;
			m_pVehicle->m_byteFlags5 = byteFlags5;
		}
		else
		{
			// Set the wheels unproteced
			m_pVehicle->m_byteFlags1 &= 0xFFF7FFFFu;
			m_pVehicle->m_byteFlags5 = byteFlags5;
		}
	}
}

bool CM2Vehicle::AreWheelsProtected( void )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		if( m_pVehicle->m_byteFlags1 & 0x80000 )
			return true;
	}

	return false;
}

void CM2Vehicle::OpenSeatWindow( M2Enums::eVehicleSeat seat, bool bOpen )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle pointer
		M2Vehicle * pVehicle = m_pVehicle;
		int iOpen = (int)bOpen;
		int iSeat = (int)(seat - 1);

		_asm push 0;
		_asm push iOpen;
		_asm push iSeat;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__OpenSeatWindow;
	}
}

M2VehicleSeat * CM2Vehicle::GetSeat( int iIndex )
{
	M2VehicleSeat * pSeat = NULL;

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle pointer
		M2Vehicle * pVehicle = m_pVehicle;
		DWORD dwFunc = m_pVehicle->m_pVFTable->GetSeatAtIndex;

		_asm push iIndex;
		_asm mov ecx, pVehicle;
		_asm call dwFunc;
		_asm mov pSeat, eax;
	}

	return pSeat;
}

int CM2Vehicle::GetSeat( CM2Entity * pEntity )
{
	// Loop through all seats (todo: find function for it)
	for( int i = 0; i < 4; i++ )
	{
		// Is the current seat occupant this entity?
		if( GetSeat( i )->m_pOccupant == pEntity->GetEntity() )
			return i;
	}

	return INVALID_ENTITY_ID;
}

void CM2Vehicle::Repair( float fDirtLevel )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		// Get the vehicle pointer
		M2Vehicle * pVehicle = m_pVehicle;
		
		_asm push fDirtLevel;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__Repair;
	}
}

void CM2Vehicle::SetEngineDamage( float fDamage )
{
	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;

		_asm push fDamage;
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__SetEngineDamage;
	}
}

float CM2Vehicle::GetEngineDamage() // don't need this - i got it reversed in the class
{
	float fDamage = 0.0f;

	// Is the vehicle valid?
	if( m_pVehicle )
	{
		M2Vehicle * pVehicle = m_pVehicle;
		
		_asm mov ecx, pVehicle;
		_asm call COffsets::FUNC_CVehicle__GetEngineDamage;
		_asm mov fDamage, eax;
	}

	return fDamage;
}