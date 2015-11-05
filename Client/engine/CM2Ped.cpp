/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Ped.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "../StdInc.h"

extern	CCore			* pCore;

CM2Ped::CM2Ped( M2Ped * pPed ) : CM2Entity( pPed )
{
	DEBUG_TRACE("CM2Ped::CM2Ped");

	// Set the ped
	SetPed( pPed );
}

CM2Ped::~CM2Ped( void )
{
	DEBUG_TRACE("CM2Ped::~CM2Ped");
}

BYTE CM2Ped::GetControlState( void )
{
	DEBUG_TRACE("CM2Ped::GetControlStyle");

	// Is the ped valid?
	if( m_pPed )
		return (BYTE)m_pPed->m_playerControls.m_ePlayerMovementState;

	return 3;
}

DWORD CM2Ped::GetState( void )
{
	DEBUG_TRACE("CM2Ped::GetState");

	// Is the ped valid?
	if( m_pPed )
		return 0; //return m_pPed->m_dwState;

	return 0;
}

void CM2Ped::SetInfiniteAmmo( bool bInfiniteAmmo )
{
	DEBUG_TRACE("CM2Ped::SetInifiniteAmmo");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
		m_pPed->m_pWeaponData->m_bInifiniteAmmo = bInfiniteAmmo;
}

bool CM2Ped::HasInfiniteAmmo( void )
{
	DEBUG_TRACE("CM2Ped::HasInifiniteAmmo");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
		return m_pPed->m_pWeaponData->m_bInifiniteAmmo;

	return false;
}

void CM2Ped::SetSelectedWeapon( DWORD dwWeapon, bool bUseAnimation )
{
	DEBUG_TRACE("CM2Ped::SetSelectedWeapon");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		// Is the weapon invalid?
		if( dwWeapon < 0 || dwWeapon > 21 )
			return;

		void* pEntityData = m_pPed->m_pEntityData;
		
		_asm push 0;
		_asm push bUseAnimation;
		_asm push dwWeapon;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__InventorySelect;
	}
}

DWORD CM2Ped::GetSelectedWeapon( void )
{
	DEBUG_TRACE("CM2Ped::GetSelectedWeapon");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pInventory )
		return m_pPed->m_pInventory->m_dwSelectedWeapon;

	return 0;
}

bool CM2Ped::IsAiming( void )
{
	DEBUG_TRACE("CM2Ped::IsAiming");

	// Is the ped valid?
	if( m_pPed )
		return (m_pPed->m_playerControls.m_bIsAiming);

	return false;
}

bool CM2Ped::IsShooting( void )
{
	DEBUG_TRACE("CM2Ped::IsShooting");

	// Is the ped valid?
	if( m_pPed )
		return false; // todo fix

	return false;
}

void CM2Ped::SetHealth( float fHealth )
{
	DEBUG_TRACE("CM2Ped::SetHealth");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		void* pEntityData = m_pPed->m_pEntityData;

		_asm push fHealth;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__SetHealth;
	}
}

float CM2Ped::GetHealth( void )
{
	DEBUG_TRACE("CM2Ped::GetHealth");

	// Is the ped valid?
	if( m_pPed )
		return m_pPed->m_fHealth;

	return 0.0f;
}

float CM2Ped::GetHealthMax( void )
{
	DEBUG_TRACE("CM2Ped::GetHealthMax");

	// Is the ped valid?
	if( m_pPed )
		return m_pPed->m_fHealthMax;

	return 0.0f;
}

float CM2Ped::GetRealHealth( void )
{
	DEBUG_TRACE("CM2Ped::GetRealHealth");

	float fRealHealth = 0.0f;

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		void* pEntityData = m_pPed->m_pEntityData;

		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__GetRealHealth;
		_asm mov fRealHealth, eax;
	}

	return fRealHealth;
}

void CM2Ped::SetInvulnerable( bool bInvulnerable )
{
	DEBUG_TRACE("CM2Ped::SetInvulnerable");

	// Is the ped valid?
	if( m_pPed )
		m_pPed->m_bInvulnerable = bInvulnerable;
}

bool CM2Ped::IsInvulnerable( void )
{
	DEBUG_TRACE("CM2Ped::IsInvulnerable");

	// Is the ped valid?
	if( m_pPed )
		return m_pPed->m_bInvulnerable;

	return false;
}

void CM2Ped::GiveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	DEBUG_TRACE("CM2Ped::GiveWeapon");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
	{
		// Is the weapon invalid?
		if( dwWeapon <= 0 || dwWeapon > 21 )
			return;

		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm push dwAmmo;
		_asm push dwWeapon;
		_asm mov ecx, pWeaponData;
		_asm call COffsets::FUNC_CHuman__InventoryAddWeapon;
	}
}

void CM2Ped::RemoveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	DEBUG_TRACE("CM2Ped::RemoveWeapon");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
	{
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		// Are we completely removing the weapon?
		if( dwAmmo == 0 )
		{
			_asm push dwWeapon;
			_asm mov ecx, pWeaponData;
			_asm call COffsets::FUNC_CHuman__InventoryRemoveWeapon;
		}
		else
		{
			// todo: remove weapon ammo (maybe just move into it's own function RemoveWeaponAmmo) ?
		}
	}
}

void CM2Ped::RemoveAllWeapons ( void )
{
	// Is the ped valid?
	if ( m_pPed )
	{
		// Loop over each weapon
		for ( int i = 2; i < 22; i++ )
		{
			// Remove the current weapon
			RemoveWeapon ( i );
		}
	}
}

void CM2Ped::ReloadWeapon( void )
{
	DEBUG_TRACE("CM2Ped::ReloadWeapon");

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
	{
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm push -1;						// unknown (something with model reload?)
		_asm push 0;						// weapon inventory index
		_asm mov ecx, pWeaponData;
		_asm call COffsets::FUNC_CHuman__InventoryDoReload;
	}
}

bool CM2Ped::HaveThrowingWeaponInHand( void )
{
	DEBUG_TRACE("CM2Ped::HaveThrowingWeaponInHand");

	bool bHaveThrowingWeaponInHand = false;

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pProjectileInventory )
	{
		void* pProjectileInventory = m_pPed->m_pProjectileInventory;

		_asm mov ecx, pProjectileInventory;
		_asm call COffsets::FUNC_CHuman__HaveThrowingWeaponInHand;
		_asm mov bHaveThrowingWeaponInHand, al;
	}

	return bHaveThrowingWeaponInHand;

	// Is the ped valid?
	//if( m_pPed )
	//	return (((*(DWORD *)m_pPed->m_pProjectileInventory->m_pUnknown + 0x24) >> 21) & 1);

	//return false;
}

bool CM2Ped::HasItem ( DWORD dwItem )
{
	bool bHasItem = false;

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pWeaponData )
	{
		//
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm push dwItem;
		_asm mov ecx, pWeaponData;
		_asm call COffsets::FUNC_CHuman__InventoryHaveItem;
		_asm mov bHasItem, al;
	}

	return bHasItem;
}

C_SyncObject * CM2Ped::GetInOutVehicle( M2Vehicle * pVehicle, int iSeat, bool bEnter, bool bForced )
{
	DEBUG_TRACE("CM2Ped::GetInOutVehicle");

	C_SyncObject * pReturnObj = NULL;

	// Is the ped valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pPed->m_pEntityData;
		int iEnter = (bEnter ? 1 : 2);

		// Adjust the seat for the engine
		if( !bEnter )
			iSeat = 0;
		else
			iSeat -= 1;

		// Is the seat invalid?
		if( iSeat < 0 )
			iSeat = 0;

		// Is this the localped?
		if( m_pPed->m_dwGUID == 9232 )
			bForced = true;

		CLogFile::Printf( "pVehicle: 0x%p, iSeat: %d, Forced: %s", pVehicle, iSeat, (bForced ? "yes" : "no") );

		_asm push 2;
		_asm push 0;
		_asm push bForced;
		_asm push iSeat;
		_asm push iEnter;
		_asm push pVehicle;
		_asm lea ecx, pReturnObj;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__GetInOutCar;

		CLogFile::Printf( "call done!" );
	}

	return pReturnObj;
}

M2Vehicle * CM2Ped::GetCurrentVehicle( void )
{
	DEBUG_TRACE("CM2Ped::GetCurrentVehicle");

	// Is the ped valid?
	if( m_pPed )
		return m_pPed->m_pCurrentVehicle;

	return NULL;
}

void CM2Ped::AddMoney( int iDollars, int iCents )
{
	DEBUG_TRACE("CM2Ped::AddMoney");

	// Is the ped valid?
	if( m_pPed )
	{
		// If we want to disable hud OR sound
		//CPatcher::InstallNopPatch( 0x949CFA , 0x9 ); // stop hud showing
		//CPatcher::InstallNopPatch( 0x949CB6 , 0x15 ); // stop sound playing

		//
		DWORD C_Human__InventoryAddMoney = 0x949C80;
		int iMoney = SharedUtility::int_concat( iDollars, iCents );
		void * pHumanInventory = m_pPed->m_pWeaponData;

		_asm push 0;
		_asm push iMoney;
		_asm mov ecx, pHumanInventory;
		_asm call C_Human__InventoryAddMoney;
	}
}

void CM2Ped::RemoveMoney( int iDollars, int iCents )
{
	DEBUG_TRACE("CM2Ped::RemoveMoney");

	// Is the ped valid?
	if( m_pPed )
	{
		//
		DWORD C_Human__InventoryRemoveMoney = 0x93BE60;
		int iMoney = SharedUtility::int_concat( iDollars, iCents );
		void * pHumanInventory = m_pPed->m_pWeaponData;

		_asm push 0;
		_asm push iMoney;
		_asm mov ecx, pHumanInventory;
		_asm call C_Human__InventoryRemoveMoney;
	}
}

float CM2Ped::GetMoney( void )
{
	DEBUG_TRACE("CM2Ped::GetMoney");

	float fMoney = 0.0f;

	// Is the ped valid?
	if( m_pPed )
	{
		DWORD C_Human__InventoryGetMoney = 0x931B80;
		void * pInventoryData = m_pPed->m_pWeaponData;

		int money;

		_asm mov ecx, pInventoryData;
		_asm call C_Human__InventoryGetMoney;
		_asm mov money, eax;

		CLogFile::Printf ( "CASH: %d (%f)", money, (double)money );
	}

	return fMoney;
}

//
C_SyncObject * CM2Ped::MoveVec( CVector3 vecPosition, M2Enums::eMoveType moveType, CVector3 vecEndDir )
{
	DEBUG_TRACE("CM2Ped::MoveVec");

	// Is the entity valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		//
		C_SyncObject * pSyncObject;
		void * pEntityData = m_pPed->m_pEntityData;
		int iMoveType = (int)moveType;

		_asm push 1;
		_asm lea ecx, vecEndDir;
		_asm push ecx;
		_asm push iMoveType;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__MoveVec;

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::AimAt( CVector3 vecPosition )
{
	DEBUG_TRACE("CM2Ped::AimAt");

	// Is the entity valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		//
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;

		M2Entity * pEnt = NULL;

		// Get the localplayer entity
		if( pCore->GetPlayerManager()->GetLocalPlayer() && pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
			pEnt = pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed();

		_asm push 1;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm push pEnt;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__AimAt;

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::ShootAt( CVector3 vecPosition )
{
	DEBUG_TRACE("CM2Ped::ShootAt");

	// Is the entity valid?
	if( m_pPed && m_pPed->m_pEntityData )
	{
		//
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;
		CVector3 vecUnknown( 0.0f, 0.0f, 0.0f );

		_asm push 1;
		_asm push 50;
		_asm lea ecx, vecUnknown;
		_asm push ecx;
		_asm lea ecx, vecUnknown;
		_asm push ecx;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__ShootAt;

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::LookAt ( CVector3 vecPosition )
{
	DEBUG_TRACE("CM2Ped::LookAt");

	// Is the entity valid?
	if ( m_pPed && m_pPed->m_pEntityData )
	{
		//
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;

		M2Entity * pEnt = NULL;

		// Get the localplayer entity
		if( pCore->GetPlayerManager()->GetLocalPlayer() && pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() )
			pEnt = pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed();

		_asm push 1;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm push pEnt;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__LookAt;

		return pSyncObject;
	}

	return NULL;
}