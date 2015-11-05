/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Entity.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

CM2Entity::CM2Entity( void )
{
#ifdef DEBUG
	CLogFile::Print( "CM2Entity::CM2Entity" );
#endif

	// Set the entity
	SetEntity( NULL );

	// Set the base entity
	SetBaseEntity( NULL );

	// Invalidate the pos entity
	m_pPosEntity = NULL;
}

// Calling this constructor should only be used for the LocalPlayer (Doesn't have M2BaseEntity class!)
CM2Entity::CM2Entity( M2Entity * pEntity )
{
#ifdef DEBUG
	CLogFile::Print( "CM2Entity::CM2Entity" );
#endif

	// Set entity
	SetEntity( pEntity );

	// Set the base entity
	SetBaseEntity( NULL );

	// Invalidate the pos entity
	m_pPosEntity = NULL;

#ifndef DEBUG
	pCore->GetChat()->Outputf( false, "CM2Entity::constructor( LocalPlayer, 0x%p, 0x%p, 0x%p )", pEntity, pEntity->m_pEntityData, pEntity->m_pWorld );
#endif

	// Dump VFTable (temp)
	//CPatcher::DumpVFTable( (DWORD)pEntity->m_pVFTable, 110 );
}

CM2Entity::CM2Entity( M2BaseEntity * pBaseEntity, M2BaseEntity * pBasePosEntity )
{
#ifdef DEBUG
	CLogFile::Print( "CM2Entity::CM2Entity" );
#endif

	// Set the entity
	SetEntity( pBaseEntity->m_pEntity );

	// Set the base entity
	SetBaseEntity( pBaseEntity );

	// Invalidate the pos entity
	m_pPosEntity = NULL;

	// Is the base position entity valid?
	if( pBasePosEntity )
	{
		// Create the position entity instance
		m_pPosEntity = new CM2Entity( pBasePosEntity, NULL );
	}

	//CPatcher::DumpVFTable( (DWORD)pBaseEntity->m_pEntity->m_pVFTable, 110 );
	//CPatcher::DumpVFTable( (DWORD)pBaseEntity->m_pVFTable, 17 );
}

CM2Entity::~CM2Entity( void )
{
#ifdef DEBUG
	CLogFile::Print( "CM2Entity::~CM2Entity" );
#endif

	// Deactivate the entity
	Deactivate( );

	// Is the base position entity valid?
	if( m_pPosEntity )
	{
		// Delete the base position entity
		SAFE_DELETE( m_pPosEntity );
	}
}

void CM2Entity::Activate( void )
{
	// Is the entity invalid?
	if( !m_pEntity || !m_pBaseEntity )
		return;

#ifdef DEBUG
	CLogFile::Print( "CM2Entity::Activate" );
#endif

	// Get the function pointer
	M2BaseEntity * pBaseEntity = m_pBaseEntity;
	DWORD dwFunc = pBaseEntity->m_pVFTable->Activate;

	// Activate the entity
	_asm mov ecx, pBaseEntity;
	_asm call dwFunc;
}

void CM2Entity::Deactivate( void )
{
	// Is the entity invalid?
	if( !m_pEntity || !m_pBaseEntity )
		return;

#ifdef DEBUG
	CLogFile::Print( "CM2Entity::Deactivate" );
#endif

	// Get the function pointer
	M2BaseEntity * pBaseEntity = m_pBaseEntity;
	DWORD dwFunc = pBaseEntity->m_pVFTable->Deactivate;

	// Activate the entity
	_asm mov ecx, pBaseEntity;
	_asm call dwFunc;
}

int CM2Entity::GetScriptMachine( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pUnk001 && m_pEntity->m_pUnk001->pUnk002 && m_pEntity->m_pUnk001->pUnk002->m_pScriptEntity )
		return m_pEntity->m_pUnk001->pUnk002->m_pScriptEntity->m_iScriptMachineNumber;
	
	return 0;
}

void CM2Entity::SetSelectedWeapon( DWORD dwWeapon, bool bUseAnimation )
{
	// Is the entity valid?
	if( m_pBaseEntity && m_pEntity )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iUseAnimation = (int)bUseAnimation;

		_asm push 1;
		_asm push iUseAnimation;
		_asm push dwWeapon;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__InventorySelect;
	}
}

DWORD CM2Entity::GetSelectedWeapon( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pInventory )
		return m_pEntity->m_pInventory->m_dwSelectedWeapon;

	return 0;
}

void CM2Entity::SetPosition( CVector3 vecPosition )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pVFTable )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetPosition;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetPosition( CVector3 * vecPosition )
{
	// Is the entity valid?
	//if( m_pEntity && m_pEntity->m_pWorld )
		//memcpy( vecPosition, &CVector3( m_pEntity->m_pWorld->m_matWorld._14, m_pEntity->m_pWorld->m_matWorld._24, m_pEntity->m_pWorld->m_matWorld._34 ), sizeof(CVector3) );

	// Is the entity valid?
	if( m_pEntity )
		memcpy( vecPosition, &CVector3( m_pEntity->m_vecPosition.fX, m_pEntity->m_vecPosition.fY, m_pEntity->m_vecPosition.fZ ), sizeof(CVector3) );
}

void CM2Entity::SetDirection( CVector3 vecDirection )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pVFTable )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetDirection;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, vecDirection;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetDirection( CVector3 * vecDirection )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pWorld )
		memcpy( vecDirection, &CVector3( m_pEntity->m_pWorld->m_matWorld._12, m_pEntity->m_pWorld->m_matWorld._22, m_pEntity->m_pWorld->m_matWorld._32 ), sizeof(CVector3) );
}

void CM2Entity::SetRotation( CVector3 vecRotation )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pVFTable )
	{
		DWORD dwFunc = m_pEntity->m_pVFTable->SetRotation;
		M2Entity * pEntity = m_pEntity;

		_asm lea ecx, vecRotation;
		_asm push ecx;
		_asm mov ecx, pEntity;
		_asm call dwFunc;
	}
}

void CM2Entity::GetRotation( CVector3 * vecRotation )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pWorld )
		memcpy( vecRotation, &CVector3( m_pEntity->m_pWorld->m_matWorld._13, m_pEntity->m_pWorld->m_matWorld._23, m_pEntity->m_pWorld->m_matWorld._33 ), sizeof(CVector3) );
}

void CM2Entity::SetHealth( float fHealth )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		_asm push fHealth;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__SetHealth;
	}
}

float CM2Entity::GetHealth( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_fHealth;

	return 0.0f;
}

float CM2Entity::GetMaxHealth( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_fMaxHealth;

	return 0.0f;
}

float CM2Entity::GetRealHealth( void )
{
	float fRealHealth = 0.0f;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__GetRealHealth;
		_asm mov fRealHealth, eax;
	}

	return fRealHealth;
}

DWORD CM2Entity::GetGUID( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_dwGUID;

	return 0;
}

bool CM2Entity::IsCrouching( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_bCrouching;

	return false;
}

void CM2Entity::ShowHat( bool bShow )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iShow = (int)bShow;

		_asm push iShow;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__ShowHat;
	}
}

void CM2Entity::ToggleStealthKill( bool bToggle )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iToggle = (int)bToggle;

		_asm push iToggle;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__ToggleStealthKill;
	}
}

void CM2Entity::GiveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		// Fail safe
		if( dwWeapon <= 0 )
			return;

		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;
		int iAmmo = (int)dwAmmo;
		int iWeapon = (int)dwWeapon;

		_asm push iAmmo;
		_asm push iWeapon;
		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryAddWeapon;
	}
}

void CM2Entity::EnablePushByPlayer( bool bEnable )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iEnable = (int)bEnable;

		_asm push iEnable;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__EnablePushByPlayer;
	}
}

void CM2Entity::SetReactions( bool bKillFromBehind, bool bBounce, bool bPunch )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iPunch = (int)bPunch;
		int iBounce = (int)bBounce;
		int iKillFromBehind = (int)bKillFromBehind;

		_asm push iPunch;
		_asm push iBounce;
		_asm push iKillFromBehind;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__SetReactions;
	}
}

M2SyncObject * CM2Entity::MoveVec( CVector3 vecPosition, M2Enums::eMoveType moveType, CVector3 vecEndDirection )
{
	M2SyncObject * pSyncObject;

	// Is the entity valid?
	if( m_pBaseEntity && m_pPosEntity )
	{
		M2BaseEntity * pBaseEntity = m_pBaseEntity;
		int iMoveType = moveType;

		_asm lea ecx, vecEndDirection;
		_asm push ecx;
		_asm push iMoveType;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pBaseEntity;
		_asm call COffsets::FUNC_CHuman__MoveVec;
	}

	return pSyncObject;
}

M2SyncObject * CM2Entity::ShootAt( CVector3 vecPosition, float fTime )
{
	M2SyncObject * pSyncObject;

	// Is the base entity and base position entity valid?
	if( m_pBaseEntity && m_pPosEntity )
	{
		M2BaseEntity * pBaseEntity = m_pBaseEntity;
		M2BaseEntity * pBasePosEntity = m_pPosEntity->GetBaseEntity();
		CVector3 vecOffset( 0, 0, 0 );

		// Set the position entity position
		m_pPosEntity->SetPosition( vecPosition );

		// Create the shoot at event
		_asm push fTime;
		_asm lea ecx, vecOffset;
		_asm push ecx;
		_asm push pBasePosEntity;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pBaseEntity;
		_asm call COffsets::FUNC_CHuman__ShootAt;
	}

	return pSyncObject;
}

M2SyncObject * CM2Entity::AimAt( CVector3 vecPosition, bool bToggle )
{
	M2SyncObject * pSyncObject;

	// Is the base entity and base position entity valid?
	if( m_pBaseEntity && m_pPosEntity )
	{
		M2BaseEntity * pBaseEntity = m_pBaseEntity;
		M2BaseEntity * pBasePosEntity = m_pPosEntity->GetBaseEntity();
		CVector3 vecOffset( 0, 0, 0 );
		int iToggle = (int)bToggle;

		// Set the position entity position
		m_pPosEntity->SetPosition( vecPosition );

		// Create the aim at event
		_asm push iToggle;
		_asm lea ecx, vecOffset;
		_asm push ecx;
		_asm push pBasePosEntity;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pBaseEntity;
		_asm call COffsets::FUNC_CHuman__AimAt;
	}

	return pSyncObject;
}

M2SyncObject * CM2Entity::LookAt( CVector3 vecPosition, float fTime )
{
	M2SyncObject * pSyncObject;

	// Is the base entity and base position entity valid?
	if( m_pBaseEntity && m_pPosEntity )
	{
		M2BaseEntity * pBaseEntity = m_pBaseEntity;
		M2BaseEntity * pBasePosEntity = m_pPosEntity->GetBaseEntity();
		CVector3 vecOffset( 0, 0, 0 );

		// Set the position entity position
		m_pPosEntity->SetPosition( vecPosition );

		_asm push fTime;
		_asm lea ecx, vecOffset;
		_asm push ecx;
		_asm push pBasePosEntity;
		_asm lea ecx, pSyncObject;
		_asm push ecx;
		_asm mov ecx, pBaseEntity;
		_asm call COffsets::FUNC_CHuman__LookAt;
	}

	return pSyncObject;
}

void CM2Entity::LockControls( bool bLock, bool bResetAnimation )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iLock = (int)bLock;

		_asm push 0;
		_asm push iLock;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__LockControls;

		// Reset the animation if needed
		if( bResetAnimation )
			AnimationReset( );
	}
}

bool CM2Entity::AreControlsLocked( void )
{
	bool bReturn = false;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__AreControlsLocked;
		_asm mov bReturn, al;
	}

	return bReturn;
}

bool CM2Entity::InventoryHaveItem( DWORD dwItem )
{
	bool bReturn = false;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;

		_asm push dwItem;
		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryHaveItem;
		_asm mov bReturn, al;
	}

	return bReturn;
}

void CM2Entity::SetInvulnerable( bool bInvulnerable )
{
	// Is the entity valid?
	if( m_pEntity )
		m_pEntity->m_bInvulnerable = bInvulnerable;
}

void CM2Entity::GiveMoney( int iAmount )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;

		_asm push 0;
		_asm push iAmount;
		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryAddMoney;
	}
}

void CM2Entity::RemoveMoney( int iAmount )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		// TODO: Fix instead of saving esp value and restoring!
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;
		int iEsp;

		_asm mov iEsp, esp;
		_asm push iAmount;
		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryRemoveMoney;
		_asm mov esp, iEsp;
	}
}

int CM2Entity::GetMoney( void )
{
	int iAmount = 0;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;

		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryGetMoney;
		_asm mov iAmount, eax;
	}

	return iAmount;
}

void CM2Entity::DropHat( bool bDrop )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iDrop = (int)bDrop;

		_asm push iDrop;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__DropHat;
	}
}

void CM2Entity::RemoveWeapon( DWORD dwWeapon, DWORD dwAmmo )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;

		// Should we just remove the weapon?
		if( dwAmmo == 0 )
		{
			_asm push dwWeapon;
			_asm mov ecx, pHumanInventory;
			_asm call COffsets::FUNC_CHuman__InventoryRemoveWeapon;
		}
		else
		{
			// TODO: Find function to remove ammo?
			// or just get the weapon ammo, remove the weapon, and then subtract dwAmmo from the ammo amount and give the player that weapon with the ammo amount
		}
	}
}

void CM2Entity::ReloadWeapon( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
	{
		M2HumanInventory * pHumanInventory = m_pEntity->m_pHumanInventory;

		_asm push -1;
		_asm push 0;
		_asm mov ecx, pHumanInventory;
		_asm call COffsets::FUNC_CHuman__InventoryDoReload;
	}
}

int CM2Entity::GetControlStyle( void )
{
	int iState = 0;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__GetControlStyle;
		_asm mov iState, eax;
	}

	return iState;
}

void CM2Entity::SetControlStyle( int iStyle )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		_asm push 0;
		_asm push iStyle;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__SetControlStyle;
	}
}

void CM2Entity::SetUnlimitedAmmo( bool bToggle )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
		m_pEntity->m_pHumanInventory->m_bInfiniteAmmo = bToggle;
}

bool CM2Entity::HasUnlimitedAmmo( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pHumanInventory )
		return m_pEntity->m_pHumanInventory->m_bInfiniteAmmo;

	return false;
}

bool CM2Entity::HaveThrowingWeaponInHand( void )
{
	bool bHaveThrowingWeapon = false;

	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pThrowingInventory )
	{
		void * pThrowingInventory = m_pEntity->m_pThrowingInventory;

		_asm mov ecx, pThrowingInventory;
		_asm call COffsets::FUNC_CHuman__HaveThrowingWeaponInHand;
		_asm mov bHaveThrowingWeapon, al;
	}
	
	return bHaveThrowingWeapon;
}

bool CM2Entity::IsOnScreen( void )
{
	bool bReturn = false;

	// Is the entity valid?
	if( m_pBaseEntity )
	{
		M2BaseEntity * pBaseEntity = m_pBaseEntity;
			
		_asm mov ecx, pBaseEntity;
		_asm call COffsets::FUNC_CHuman__IsOnScreen;
		_asm mov bReturn, al;
	}

	return bReturn;
}

// ============= CRASH =======================
void CM2Entity::AnimationReset( void )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pUnknown )
	{
		void * pUnknown = m_pEntity->m_pUnknown->m_pUnknown;

		CLogFile::Printf( "pUnknown: 0x%p", pUnknown );

		_asm mov ecx, pUnknown;
		_asm call COffsets::FUNC_CHuman__AnimationReset;
	}
}
// ==========================================

void CM2Entity::SetState( DWORD dwState )
{
}

DWORD CM2Entity::GetState( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_dwState;

	return (DWORD)M2Enums::IDLE;
}

void CM2Entity::PutInVehicle( M2Vehicle * pVehicle, M2Enums::eVehicleSeat seat, bool bEnter, bool bAnimate )
{
	// Is the entity valid?
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;
		int iEnter = (bEnter ? 1 : 2);
		int iAnimate = (int)!bAnimate;
		int iSeat = (int)seat;
		void * pReturn; // todo: M2SyncObject

		// Are we exiting the vehicle?
		if( !bEnter )
			iSeat = 0;
		else
			iSeat -= 1;

		_asm push 2;
		_asm push 0;
		_asm push iAnimate;
		_asm push iSeat;
		_asm push iEnter;
		_asm push pVehicle;
		_asm lea ecx, pReturn;
		_asm push ecx;
		_asm mov ecx, pEntityData;
		_asm call COffsets::FUNC_CHuman__GetInOutCar;
	}
}

M2Vehicle * CM2Entity::GetVehicle( void )
{
	// Is the entity valid?
	if( m_pEntity )
		return m_pEntity->m_pVehicle;

	return NULL;
}

void CM2Entity::CreateMapIcon( char szUserName )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		/*_asm push szUserName;
		_asm push 0;
		_asm push 1;
		_asm push 0;
		_asm push 1;
		_asm mov ecx, COffsets::VAR_CMap;
		_asm call COffsets::FUNC_CMap__CreateEntityIcon;*/
	}
}

// Note: this only works for the localplayer!
void CM2Entity::ChangeModel( int iFolder, String strModel )
{
	// Is the entity valid?
	if( m_pEntity )
	{
		switch( iFolder )
		{
		case 0: // /hchar/
			{
				// Generate the model path
				unsigned char sdsModel[18] = "/sds/hchar/%s.sds";

				// Patch the model folder address
				CPatcher::PatchAddress( 0x18F6FDC, sdsModel, sizeof(sdsModel) );
			}

		case 1: // /player/
			{
				// Generate the model path
				unsigned char sdsModel[19] = "/sds/player/%s.sds";

				// Patch the model folder address
				CPatcher::PatchAddress( 0x18F6FDC, sdsModel, sizeof(sdsModel) );
			}

		case 2: // /traffic/
			{
				// Generate the model path
				unsigned char sdsModel[20] = "/sds/traffic/%s.sds";

				// Patch the model folder address
				CPatcher::PatchAddress( 0x18F6FDC, sdsModel, sizeof(sdsModel) );
			}
		}

		// Change the player model (TODO: Reverse this 0x5BB7E0 - you can pass entities to it, but game defauly uses VAR_LocalPlayer, let's mess with it, it call's 0x5BB590 which looks better to use)
		CLua::Executef( "DelayBuffer:Insert( function(l_11_0) game.game:ChangePlayerModel( \"%s\" ) end, {l_1_0}, 600, 1, false )", strModel.Get() );

		// Was the library not /player/ ?
		//if( iFolder != 1 )
		//{
			// Generate the model path
		//	unsigned char sdsModel[19] = "/sds/player/%s.sds";

			// Restore the model folder address (prevent any crashing)
		//	CPatcher::PatchAddress( 0x18F6FDC, sdsModel, sizeof(sdsModel) );
		//}
	}
}
void CM2Entity::PlayAnimation( char strAnimation )
{
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		int iAnimation = (int)strAnimation;// trololol
		DWORD dwAddress = 0xD7FC50;//0xD80250 // hacky, todo fix

		_asm push iAnimation;
		_asm mov ecx, pEntityData;
		_asm call dwAddress;
	}
}

void CM2Entity::StopAnimation( char strAnimation )
{
	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		int iAnimation = (int)strAnimation;// trololol
		DWORD dwAddress = 0xD6FD70; // hacky, todo fix

		_asm push iAnimation;
		_asm mov ecx, pEntityData;
		_asm call dwAddress;
	}
}

bool CM2Entity::IsAnimFinished( char strAnimation )
{
	bool bReturn = false;

	if( m_pEntity && m_pEntity->m_pEntityData )
	{
		M2EntityData * pEntityData = m_pEntity->m_pEntityData;

		int iAnimation = (int)strAnimation;// trololol
		DWORD dwAddress = 0xD6FD40; // hacky, todo fix

		_asm push iAnimation;
		_asm mov ecx, pEntityData;
		_asm call dwAddress;
		_asm mov bReturn, al;

		return bReturn;
	}
	return bReturn;
}