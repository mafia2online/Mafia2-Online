/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Ped.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CPlayerManager.h"
#include "CLocalPlayer.h"

#include "SharedUtility.h"

#include "CM2Entity.h"
#include "CM2Ped.h"

#include "CLogFile.h"

CM2Ped::CM2Ped(M2Ped * pPed) : CM2Entity(pPed)
{
	DEBUG_LOG("CM2Ped::CM2Ped");

	SetPed(pPed);
}

CM2Ped::~CM2Ped(void)
{
	DEBUG_LOG("CM2Ped::~CM2Ped");
}

BYTE CM2Ped::GetControlState(void)
{
	if (m_pPed)
		return (BYTE)m_pPed->m_playerControls.m_playerMovementState;

	return 3;
}

DWORD CM2Ped::GetState(void)
{
	if (m_pPed)
		return 0; //return m_pPed->m_dwState;

	return 0;
}

void CM2Ped::SetInfiniteAmmo(bool bInfiniteAmmo)
{
	DEBUG_LOG("CM2Ped::SetInifiniteAmmo");

	if (m_pPed && m_pPed->m_pWeaponData)
		m_pPed->m_pWeaponData->m_bInifiniteAmmo = bInfiniteAmmo;
}

bool CM2Ped::HasInfiniteAmmo(void)
{
	DEBUG_LOG("CM2Ped::HasInifiniteAmmo");

	if (m_pPed && m_pPed->m_pWeaponData)
		return m_pPed->m_pWeaponData->m_bInifiniteAmmo;

	return false;
}

void CM2Ped::SetSelectedWeapon(DWORD dwWeapon, bool bUseAnimation)
{
	DEBUG_LOG("CM2Ped::SetSelectedWeapon");

	if (m_pPed && m_pPed->m_pHumanScript)
	{
		if (dwWeapon < 0 || dwWeapon > 21)
			return;

		void* pEntityData = m_pPed->m_pHumanScript;

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
	if (m_pPed && m_pPed->m_pInventory)
		return m_pPed->m_pInventory->m_dwSelectedWeapon;

	return 0;
}

int CM2Ped::GetSelectedWeaponBullet(void)
{
	int bullets = 0;

	if (!m_pPed || !m_pPed->m_pWeaponData)
		return (bullets);

	void * pHumanInventory = m_pPed->m_pWeaponData;
	__asm
	{
		mov ecx, pHumanInventory;
		call COffsets::FUNC_CHuman__InventoryTellAmmoInSelectedWeapon;
		mov bullets, eax;
	}

	return (bullets);
}

bool CM2Ped::IsAiming(void)
{
	if (m_pPed)
		return (m_pPed->m_playerControls.m_bIsAiming);

	return false;
}

bool CM2Ped::IsShooting(void)
{
	if (m_pPed)
		return (m_pPed->m_playerControls.m_bIsShooting);

	return false;
}

void CM2Ped::SetHealth(float fHealth)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		void* pEntityData = m_pPed->m_pHumanScript;

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
	if (m_pPed)
		return m_pPed->m_fHealth;

	return 0.0f;
}

float CM2Ped::GetHealthMax(void)
{
	DEBUG_LOG("CM2Ped::GetHealthMax");

	if (m_pPed)
		return m_pPed->m_fHealthMax;

	return 0.0f;
}

float CM2Ped::GetRealHealth(void)
{
	DEBUG_LOG("CM2Ped::GetRealHealth");

	float fRealHealth = 0.0f;

	if (m_pPed && m_pPed->m_pHumanScript)
	{
		void* pEntityData = m_pPed->m_pHumanScript;

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
	DEBUG_LOG("CM2Ped::SetInvulnerable");

	if (m_pPed)
		m_pPed->m_bInvulnerable = bInvulnerable;
}

bool CM2Ped::IsInvulnerable(void)
{
	DEBUG_LOG("CM2Ped::IsInvulnerable");

	if (m_pPed)
		return m_pPed->m_bInvulnerable;

	return false;
}

void CM2Ped::GiveWeapon(DWORD dwWeapon, DWORD dwAmmo)
{
	DEBUG_LOG("CM2Ped::GiveWeapon");

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
	DEBUG_LOG("CM2Ped::RemoveWeapon");

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
	DEBUG_LOG("CM2Ped::ReloadWeapon");

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
	DEBUG_LOG("CM2Ped::HaveThrowingWeaponInHand");

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
	DEBUG_LOG("CM2Ped::GetInOutVehicle");

	C_SyncObject * pReturnObj = NULL;

	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;
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

		DEBUG_LOG ("pVehicle: 0x%p, iSeat: %d, Forced: %s", pVehicle, iSeat, (bForced ? "yes" : "no"));

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
			mov ecx, pHumanScript;
			call COffsets::FUNC_CHuman__GetInOutCar;
		}

		DEBUG_LOG ("call done!");
	}

	return pReturnObj;
}

M2Vehicle * CM2Ped::GetCurrentVehicle(void)
{
	if (m_pPed)
		return m_pPed->m_pCurrentVehicle;

	return NULL;
}

void CM2Ped::AddMoney(int iDollars, int iCents)
{
	DEBUG_LOG("CM2Ped::AddMoney");

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
	DEBUG_LOG("CM2Ped::RemoveMoney");

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
	DEBUG_LOG("CM2Ped::GetMoney");

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

		DEBUG_LOG ("CASH: %d (%f)", money, (double)money);
	}

	return fMoney;
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrMoveV(C_SyncObject **syncObject, const CVector3 &begin, const E_HumanMoveMode moveMode, const CVector3 &target, const bool smoothStop/* = true*/)
{
	_asm jmp COffsets::FUNC_CHuman__MoveVec;
}

C_SyncObject * CM2Ped::MoveVec(const CVector3 &vecPosition, M2Enums::eMoveType moveType, CVector3 vecEndDir)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject = nullptr;
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

		pHumanScript->ScrMoveV(&pSyncObject, vecPosition, static_cast<E_HumanMoveMode>(moveType), vecEndDir, true);
		return pSyncObject;
	}

	return NULL;
}

C_SyncObject * CM2Ped::AimAt(CVector3 vecPosition)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pHumanScript;

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
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pHumanScript;
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
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject;
		void* pEntityData = m_pPed->m_pHumanScript;

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

/*
** Animation system
*/

int _declspec(naked) C_HumanScript::PlayAnim(C_SyncObject **syncObject, const char *const animName, const bool repeat, int, int, float, float, float)
{
	_asm {
		mov eax, 0x0982CC0
		jmp eax
	}
}

C_SyncObject *CM2Ped::PlayAnimation(char *strAnimation, bool repeat)
{
	C_SyncObject *pSyncObject = NULL;
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

		pHumanScript->PlayAnim(&pSyncObject, strAnimation, repeat, 0, 0, 0.0f, 0.30000001f, 0.3000000f);
		++pSyncObject->m_dwState;
	}
	return (pSyncObject);
}

class Allocator
{
public:
	void *Alloc(size_t count);
};

void _declspec(naked) *Allocator::Alloc(size_t count)
{
	_asm {
		mov eax, 0x410150
		jmp eax
	}
}

class UnknownPacket
{
public:
	DWORD		vtable;		// 00-04
	DWORD		unused[5];	// 04-18


	void Initialize(int a1, int a2);
};

void _declspec(naked) UnknownPacket::Initialize(int a1, int a2)
{
	_asm {
		mov eax, 0xF37EC0
		jmp eax
	}
}

void CM2Ped::StopAnimation(C_SyncObject *obj)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		// TODO FIX
		//C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

		//M2EntityData_Unknown_000 *unk000 = pHumanScript->m_pUnknown0;

		//unk000->UnknownMethodVX20();

		//Allocator *allocator = *(Allocator **)(0x1BB5AF4);
		//UnknownPacket *data = (UnknownPacket *)allocator->Alloc(24);
		//data->Initialize(0, 8);
		//data->vtable = 0x18DDFA8;

		//DWORD _data = *(DWORD *)((*(DWORD *)(*(DWORD *)((DWORD)(pHumanScript)) + 0xA8)) + 0xC);
		//DWORD dwFunc = 0x0FA24C0;
		//__asm
		//{
		//	push data
		//	mov ecx, _data
		//	call dwFunc
		//}
	}
}

bool CM2Ped::IsAnimFinished()
{
	bool bReturn = false;

	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

	}
	return bReturn;
}

int _declspec(naked) C_HumanScript::AnimPlayEffect(C_SyncObject **syncObject, const char *const effectName, const bool repeat, int)
{
	__asm
	{
		mov eax, 0x0951700
		jmp eax
	}
}

void _declspec(naked) C_HumanScript::AnimEffectStop()
{
	__asm
	{
		mov eax, 0x0985220
		jmp eax
	}
}

C_SyncObject  *CM2Ped::PlayAnimEffect(const char *effectName, bool bRepeat)
{
	if (!m_pPed || !m_pPed->m_pHumanScript)
		return (NULL);

	C_HumanScript *pHumanScript = m_pPed->m_pHumanScript;

	C_SyncObject *pSyncObject = NULL;
	pHumanScript->AnimPlayEffect(&pSyncObject, effectName, bRepeat, 0);
	++pSyncObject->m_dwState;
	return (pSyncObject);
}



void CM2Ped::AnimEffectStop(C_SyncObject *obj)
{
	if (!m_pPed || !m_pPed->m_pHumanScript)
		return;

	M2Ped *ped = m_pPed;
	C_HumanScript *pHumanScript = m_pPed->m_pHumanScript;
	pHumanScript->AnimEffectStop();
}

/* End of animation system */

void CM2Ped::ModelToHand(int iHand, int iModel)
{
	if (iHand < 1 || iHand > 3) // 1 : right hand & 2 : left hand & 3 : both hand
		return;

	if (iModel < 0 || iModel > 120)
		return;

	if (!m_pPed)
		return;

	C_HumanScript* pHumanScript = m_pPed->m_pHumanScript;

	DWORD func = COffsets::FUNC_CHuman__SetModelToHand;

	int leftModel = 0;
	int rightModel = 0;
	if (iHand == 1)
	{
		leftModel = 0;
		rightModel = iModel;
	}
	else if (iHand == 2)
	{
		leftModel = iModel;
		rightModel = 0;
	}
	else if (iHand == 3)
	{
		leftModel = iModel;
		rightModel = iModel;
	}

	_asm
	{
		push rightModel;
		push leftModel;
		push 0;
		mov ecx, pHumanScript;
		call func;
	}
}

void CM2Ped::ModelToMouth(int iModel)
{
	if (!m_pPed)
		return;

	C_HumanScript* pHumanScript = m_pPed->m_pHumanScript;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetModelToMouth;

	__asm
	{
		push iModel;
		mov ecx, pHumanScript;
		call dwFunc;
	}
}

void CM2Ped::SetAnimStyle(const char *dir, const char *set)
{
	if (strlen(dir) <= 0 || strlen(set) <= 0)
		return;

	if (!m_pPed)
		return;

	C_HumanScript* pHumanScript = m_pPed->m_pHumanScript;

	DWORD func = COffsets::FUNC_CHuman__SetAnimStyle;

	_asm
	{
		push set;
		push dir;
		mov ecx, pHumanScript;
		call func;
	}
}

void CM2Ped::SetPhysState(ePhysState state)
{
	if (!m_pPed)
		return;

	C_HumanScript* pHumanScript = m_pPed->m_pHumanScript;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetPhysState;

	__asm
	{
		push state;
		mov ecx, pHumanScript;
		call dwFunc;
	}
}

ePhysState	CM2Ped::GetPhysState(void)
{
	if (!m_pPed)
		return (E_DYNAMIC);

	C_HumanScript* pHumanScript = m_pPed->m_pHumanScript;

	DWORD dwFunc = 0x090CA8;

	int iRetn = E_DYNAMIC;

	__asm
	{
		mov ecx, pHumanScript;
		call dwFunc;
		mov iRetn, eax;
	}

	return ((ePhysState)iRetn);
}

/* StealthMoving things */

void CM2Ped::SetStealthMove(bool bStealthMove)
{
	if (!m_pPed)
		return;

	C_HumanScript *pHumanScript = m_pPed->m_pHumanScript;

	DWORD dwFunc = COffsets::FUNC_CHuman__SetStealthMove; // C_HumanScript::SetStealthMove(bool)

	__asm
	{
		push bStealthMove;
		mov ecx, pHumanScript;
		call  dwFunc;
	}
}

bool _declspec(naked) M2Ped::IsCrouch() const
{
	_asm {
		mov eax, [ecx]
		mov edx, [eax + 0x198]
		jmp edx
	}
}

bool CM2Ped::IsStealthMoving()
{
	if (!m_pPed)
		return (false);

	return (m_pPed->IsCrouch());
}

/* End of StealthMoving things */
