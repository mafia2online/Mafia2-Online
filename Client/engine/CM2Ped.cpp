/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Ped.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../BaseInc.h"

#include	"../CCore.h"

#include	"../CPlayerManager.h"
#include	"../CLocalPlayer.h"

#include	"../../Shared/SharedUtility.h"

#include	"CM2Entity.h"
#include	"CM2Ped.h"

CM2Ped::CM2Ped(M2Ped * pPed) : CM2Entity(pPed)
{
	DEBUG_TRACE("CM2Ped::CM2Ped");

	SetPed(pPed);
}

CM2Ped::~CM2Ped(void)
{
	DEBUG_TRACE("CM2Ped::~CM2Ped");
}

BYTE CM2Ped::GetControlState(void)
{
	DEBUG_TRACE("CM2Ped::GetControlStyle");

	if (m_pPed)
		return (BYTE)m_pPed->m_playerControls.m_playerMovementState;

	return 3;
}

DWORD CM2Ped::GetState(void)
{
	DEBUG_TRACE("CM2Ped::GetState");

	if (m_pPed)
		return 0; //return m_pPed->m_dwState;

	return 0;
}

void CM2Ped::SetInfiniteAmmo(bool bInfiniteAmmo)
{
	DEBUG_TRACE("CM2Ped::SetInifiniteAmmo");

	if (m_pPed && m_pPed->m_pWeaponData)
		m_pPed->m_pWeaponData->m_bInifiniteAmmo = bInfiniteAmmo;
}

bool CM2Ped::HasInfiniteAmmo(void)
{
	DEBUG_TRACE("CM2Ped::HasInifiniteAmmo");

	if (m_pPed && m_pPed->m_pWeaponData)
		return m_pPed->m_pWeaponData->m_bInifiniteAmmo;

	return false;
}

void CM2Ped::SetSelectedWeapon(DWORD dwWeapon, bool bUseAnimation)
{
	DEBUG_TRACE("CM2Ped::SetSelectedWeapon");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		if (dwWeapon < 0 || dwWeapon > 21)
			return;

		void* pEntityData = m_pPed->m_pEntityData;

		_asm
		{
			push 0;
			push bUseAnimation;
			push dwWeapon;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__InventorySelect;
		}
	}
}

DWORD CM2Ped::GetSelectedWeapon(void)
{
	DEBUG_TRACE("CM2Ped::GetSelectedWeapon");

	if (m_pPed && m_pPed->m_pInventory)
		return m_pPed->m_pInventory->m_dwSelectedWeapon;

	return 0;
}

bool CM2Ped::IsAiming(void)
{
	DEBUG_TRACE("CM2Ped::IsAiming");

	if (m_pPed)
		return (m_pPed->m_playerControls.m_bIsAiming);

	return false;
}

bool CM2Ped::IsShooting(void)
{
	DEBUG_TRACE("CM2Ped::IsShooting");

	if (m_pPed)
		return (m_pPed->m_playerControls.m_bIsShooting);

	return false;
}

void CM2Ped::SetHealth(float fHealth)
{
	DEBUG_TRACE("CM2Ped::SetHealth");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		void* pEntityData = m_pPed->m_pEntityData;

		_asm
		{
			push fHealth;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__SetHealth;
		}
	}
}

float CM2Ped::GetHealth(void)
{
	DEBUG_TRACE("CM2Ped::GetHealth");

	if (m_pPed)
		return m_pPed->m_fHealth;

	return 0.0f;
}

float CM2Ped::GetHealthMax(void)
{
	DEBUG_TRACE("CM2Ped::GetHealthMax");

	if (m_pPed)
		return m_pPed->m_fHealthMax;

	return 0.0f;
}

float CM2Ped::GetRealHealth(void)
{
	DEBUG_TRACE("CM2Ped::GetRealHealth");

	float fRealHealth = 0.0f;

	if (m_pPed && m_pPed->m_pEntityData)
	{
		void* pEntityData = m_pPed->m_pEntityData;

		_asm
		{
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__GetRealHealth;
			mov fRealHealth, eax;
		}
	}

	return fRealHealth;
}

void CM2Ped::SetInvulnerable(bool bInvulnerable)
{
	DEBUG_TRACE("CM2Ped::SetInvulnerable");

	if (m_pPed)
		m_pPed->m_bInvulnerable = bInvulnerable;
}

bool CM2Ped::IsInvulnerable(void)
{
	DEBUG_TRACE("CM2Ped::IsInvulnerable");

	if (m_pPed)
		return m_pPed->m_bInvulnerable;

	return false;
}

void CM2Ped::GiveWeapon(DWORD dwWeapon, DWORD dwAmmo)
{
	DEBUG_TRACE("CM2Ped::GiveWeapon");

	if (m_pPed && m_pPed->m_pWeaponData)
	{
		if (dwWeapon <= 0 || dwWeapon > 21)
			return;

		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm
		{
			push dwAmmo;
			push dwWeapon;
			mov ecx, pWeaponData;
			call COffsets::FUNC_CHuman__InventoryAddWeapon;
		}
	}
}

void CM2Ped::RemoveWeapon(DWORD dwWeapon, DWORD dwAmmo)
{
	DEBUG_TRACE("CM2Ped::RemoveWeapon");

	if (m_pPed && m_pPed->m_pWeaponData)
	{
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		// Are we completely removing the weapon?
		if (dwAmmo == 0)
		{
			_asm
			{
				push dwWeapon;
				mov ecx, pWeaponData;
				call COffsets::FUNC_CHuman__InventoryRemoveWeapon;
			}
		}
		else
		{
			// todo: remove weapon ammo (maybe just move into it's own function RemoveWeaponAmmo) ?
		}
	}
}

void CM2Ped::RemoveAllWeapons(void)
{
	if (m_pPed)
	{
		for (int i = 2; i < 22; i++)
		{
			RemoveWeapon(i);
		}
	}
}

void CM2Ped::ReloadWeapon(void)
{
	DEBUG_TRACE("CM2Ped::ReloadWeapon");

	if (m_pPed && m_pPed->m_pWeaponData)
	{
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm
		{
			push - 1;						// unknown (something with model reload?)
			push 0;						// weapon inventory index
			mov ecx, pWeaponData;
			call COffsets::FUNC_CHuman__InventoryDoReload;
		}
	}
}

bool CM2Ped::HaveThrowingWeaponInHand(void)
{
	DEBUG_TRACE("CM2Ped::HaveThrowingWeaponInHand");

	bool bHaveThrowingWeaponInHand = false;

	if (m_pPed && m_pPed->m_pProjectileInventory)
	{
		void* pProjectileInventory = m_pPed->m_pProjectileInventory;

		_asm
		{
			mov ecx, pProjectileInventory;
			call COffsets::FUNC_CHuman__HaveThrowingWeaponInHand;
			mov bHaveThrowingWeaponInHand, al;
		}
	}

	return bHaveThrowingWeaponInHand;

	//if( m_pPed )
	//	return (((*(DWORD *)m_pPed->m_pProjectileInventory->m_pUnknown + 0x24) >> 21) & 1);

	//return false;
}

bool CM2Ped::HasItem(DWORD dwItem)
{
	bool bHasItem = false;

	if (m_pPed && m_pPed->m_pWeaponData)
	{
		M2WeaponData * pWeaponData = m_pPed->m_pWeaponData;

		_asm
		{
			push dwItem;
			mov ecx, pWeaponData;
			call COffsets::FUNC_CHuman__InventoryHaveItem;
			mov bHasItem, al;
		}
	}

	return bHasItem;
}

C_SyncObject * CM2Ped::GetInOutVehicle(M2Vehicle * pVehicle, int iSeat, bool bEnter, bool bForced)
{
	DEBUG_TRACE("CM2Ped::GetInOutVehicle");

	C_SyncObject * pReturnObj = NULL;

	if (m_pPed && m_pPed->m_pEntityData)
	{
		M2EntityData * pEntityData = m_pPed->m_pEntityData;
		int iEnter = (bEnter ? 1 : 2);

		// Adjust the seat for the engine
		if (!bEnter)
			iSeat = 0;
		else
			iSeat -= 1;

		if (iSeat < 0)
			iSeat = 0;

		// Is this the localped?
		if (m_pPed->m_dwGUID == 9232)
			bForced = true;

		CLogFile::Printf("pVehicle: 0x%p, iSeat: %d, Forced: %s", pVehicle, iSeat, (bForced ? "yes" : "no"));

		_asm
		{
			push 2;
			push 0;
			push bForced;
			push iSeat;
			push iEnter;
			push pVehicle;
			lea ecx, pReturnObj;
			push ecx;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__GetInOutCar;
		}

		CLogFile::Printf("call done!");
	}

	return pReturnObj;
}

M2Vehicle * CM2Ped::GetCurrentVehicle(void)
{
	DEBUG_TRACE("CM2Ped::GetCurrentVehicle");

	if (m_pPed)
		return m_pPed->m_pCurrentVehicle;

	return NULL;
}

void CM2Ped::AddMoney(int iDollars, int iCents)
{
	DEBUG_TRACE("CM2Ped::AddMoney");

	if (m_pPed)
	{
		// If we want to disable hud OR sound
		//CPatcher::InstallNopPatch( 0x949CFA , 0x9 ); // stop hud showing
		//CPatcher::InstallNopPatch( 0x949CB6 , 0x15 ); // stop sound playing

		DWORD C_Human__InventoryAddMoney = 0x949C80;
		int iMoney = SharedUtility::int_concat(iDollars, iCents);
		void * pHumanInventory = m_pPed->m_pWeaponData;

		_asm
		{
			push 0;
			push iMoney;
			mov ecx, pHumanInventory;
			call C_Human__InventoryAddMoney;
		}
	}
}

void CM2Ped::RemoveMoney(int iDollars, int iCents)
{
	DEBUG_TRACE("CM2Ped::RemoveMoney");

	if (m_pPed)
	{
		DWORD C_Human__InventoryRemoveMoney = 0x93BE60;
		int iMoney = SharedUtility::int_concat(iDollars, iCents);
		void * pHumanInventory = m_pPed->m_pWeaponData;

		_asm
		{
			push 0;
			push iMoney;
			mov ecx, pHumanInventory;
			call C_Human__InventoryRemoveMoney;
		}
	}
}

float CM2Ped::GetMoney(void)
{
	DEBUG_TRACE("CM2Ped::GetMoney");

	float fMoney = 0.0f;

	if (m_pPed)
	{
		DWORD C_Human__InventoryGetMoney = 0x931B80;
		void * pInventoryData = m_pPed->m_pWeaponData;

		int money;

		_asm
		{
			mov ecx, pInventoryData;
			call C_Human__InventoryGetMoney;
			mov money, eax;
		}

		CLogFile::Printf("CASH: %d (%f)", money, (double)money);
	}

	return fMoney;
}

C_SyncObject * CM2Ped::MoveVec(CVector3 vecPosition, M2Enums::eMoveType moveType, CVector3 vecEndDir)
{
	DEBUG_TRACE("CM2Ped::MoveVec");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		C_SyncObject * pSyncObject;
		void * pEntityData = m_pPed->m_pEntityData;
		int iMoveType = (int)moveType;

		_asm
		{
			push 1;
			lea ecx, vecEndDir;
			push ecx;
			push iMoveType;
			lea ecx, vecPosition;
			push ecx;
			lea ecx, pSyncObject;
			push ecx;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__MoveVec;
		}

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::AimAt(CVector3 vecPosition)
{
	DEBUG_TRACE("CM2Ped::AimAt");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;

		M2Entity * pEnt = NULL;

		// Get the localplayer entity
		CLocalPlayer *pLocalPlayer = CLocalPlayer::Instance();
		if (pLocalPlayer && pLocalPlayer->IsSpawned())
			pEnt = pLocalPlayer->GetPlayerPed()->GetPed();

		_asm
		{
			push 1;
			lea ecx, vecPosition;
			push ecx;
			push pEnt;
			lea ecx, pSyncObject;
			push ecx;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__AimAt;
		}

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::ShootAt(CVector3 vecPosition)
{
	DEBUG_TRACE("CM2Ped::ShootAt");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;
		CVector3 vecUnknown(0.0f, 0.0f, 0.0f);

		_asm
		{
			push 1;
			push 50;
			lea ecx, vecUnknown;
			push ecx;
			lea ecx, vecUnknown;
			push ecx;
			lea ecx, vecPosition;
			push ecx;
			lea ecx, pSyncObject;
			push ecx;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__ShootAt;
		}

		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::LookAt(CVector3 vecPosition)
{
	DEBUG_TRACE("CM2Ped::LookAt");

	if (m_pPed && m_pPed->m_pEntityData)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pEntityData;

		M2Entity * pEnt = NULL;

		CLocalPlayer *pLocalPlayer = CLocalPlayer::Instance();
		if (pLocalPlayer && pLocalPlayer->IsSpawned())
			pEnt = pLocalPlayer->GetPlayerPed()->GetPed();
		_asm
		{
			push 1;
			lea ecx, vecPosition;
			push ecx;
			push pEnt;
			lea ecx, pSyncObject;
			push ecx;
			mov ecx, pEntityData;
			call COffsets::FUNC_CHuman__LookAt;
		}

		return pSyncObject;
	}

	return NULL;
}

int _declspec(naked) M2EntityData::PlayAnim(C_SyncObject **syncObject, const char *const animName, const bool repeat, int, int, float, float, float)
{
	_asm {
		mov eax, 0x00982CC0
		jmp eax
	}
}

void CM2Ped::PlayAnimation(char *strAnimation, bool repeat)
{
	if (m_pPed && m_pPed->m_pEntityData)
	{
		M2EntityData * pEntityData = m_pPed->m_pEntityData;

		C_SyncObject *pSyncObject = NULL;
		pEntityData->PlayAnim(&pSyncObject, strAnimation, repeat, 0, 0, 0.0f, 0.30000001f, 0.3000000f);
		++pSyncObject->m_dwState;
	}
}

void CM2Ped::StopAnimation(char *strAnimation)
{
	if (m_pPed && m_pPed->m_pEntityData)
	{
		M2EntityData * pEntityData = m_pPed->m_pEntityData;

		int iAnimation = (int)strAnimation;// trololol
		DWORD dwAddress = 0xD6FD70; // hacky, todo fix

		_asm
		{
			push iAnimation;
			mov ecx, pEntityData;
			call dwAddress;
		}
	}
}

bool CM2Ped::IsAnimFinished(char *strAnimation)
{
	bool bReturn = false;

	if (m_pPed && m_pPed->m_pEntityData)
	{
		M2EntityData * pEntityData = m_pPed->m_pEntityData;

		int iAnimation = (int)strAnimation;// trololol
		DWORD dwAddress = 0xD6FD40; // hacky, todo fix

		_asm
		{
			push iAnimation;
			mov ecx, pEntityData;
			call dwAddress;
			mov bReturn, al;
		}

		return bReturn;
	}
	return bReturn;
}

int _declspec(naked) M2EntityData::AnimPlayEffect(C_SyncObject **syncObject, const char *const effectName, const bool unknow, int)
{
	__asm
	{
		mov eax, 0x0951700
		jmp eax
	}
}

void CM2Ped::PlayAnimEffect(const char *effectName, bool bRepeat)
{
	if (!m_pPed || !m_pPed->m_pEntityData)
		return;

	M2EntityData *pEntityData = m_pPed->m_pEntityData;

	C_SyncObject *pSyncObject = NULL;
	pEntityData->AnimPlayEffect(&pSyncObject, effectName, bRepeat, 0);
	++pSyncObject->m_dwState;
}



void CM2Ped::AnimEffectStop()
{
	if (!m_pPed || !m_pPed->m_pEntityData)
		return;

	DWORD dwFunc = 0x0D65F60;

	__asm
	{
		mov ecx, m_pPed;
		call dwFunc;
	}	
}

void CM2Ped::ModelToHand(int iHand, int iModel)
{
	if (iHand != 1 && iHand != 2) // 1 : right hand & 2 : left hand
		return;

	if (iModel < 0 || iModel > 120)
		return;

	if (!m_pPed)
		return;

	M2EntityData* pEntityData = m_pPed->m_pEntityData;

	DWORD func = COffsets::FUNC_CHuman__SetModelToHand;

	_asm
	{
		push 1; // Hand or unk
		push iModel; // Model
		push iHand; // Unk or hand
		mov ecx, pEntityData;
		call func;
	}
}

void CM2Ped::ModelToMouth(int iModel)
{
	if (!m_pPed)
		return;

	M2EntityData * pEntityData = m_pPed->m_pEntityData;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetModelToMouth;

	__asm
	{
		push iModel;
		mov ecx, pEntityData;
		call dwFunc;
	}
}

void CM2Ped::SetAnimStyle(const char *dir, const char *set)
{
	if (strlen(dir) <= 0 || strlen(set) <= 0)
		return;

	if (!m_pPed)
		return;

	M2EntityData *pEntityData = m_pPed->m_pEntityData;

	DWORD func = COffsets::FUNC_CHuman__SetAnimStyle;

	_asm
	{
		push set;
		push dir;
		mov ecx, pEntityData;
		call func;
	}
}

void CM2Ped::SetPhysState(ePhysState state)
{
	if (!m_pPed)
		return;

	M2EntityData *pEntityData = m_pPed->m_pEntityData;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetPhysState;

	__asm
	{
		push state;
		mov ecx, pEntityData;
		call dwFunc;
	}
}

void CM2Ped::SetStealthMove(bool bStealthMove)
{
	if (!m_pPed)
		return;

	M2EntityData *pEntityData = m_pPed->m_pEntityData;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetStealthMove;

	__asm
	{
		push bStealthMove;
		mov ecx, pEntityData;
		call  dwFunc;
	}
}

bool CM2Ped::IsStealthMoving()
{
	if (m_pPed)
	{
		M2EntityData *pEntityData = m_pPed->m_pEntityData;

		DWORD dwFunc = 0x0D67C30;
		bool bRetn = 0;
		__asm
		{
			mov ecx, pEntityData;
			call dwFunc;
			mov bRetn, al;
		}
		return (bRetn);
	}

	return (false);
}