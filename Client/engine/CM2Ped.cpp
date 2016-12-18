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

ePhysState C_HumanScript::GetPhysState()
{
	_asm {
		mov eax, 0x090CA8;
		jmp eax;
	}
}

void _declspec(naked) C_HumanScript::ModelToHand(int unk, int leftModel, int rightModel)
{
	_asm jmp COffsets::FUNC_CHuman__SetModelToHand;
}

void _declspec(naked) C_HumanScript::ModelToMouth(int model)
{
	_asm jmp COffsets::FUNC_CHuman__SetModelToMouth;
}

void _declspec(naked) C_HumanScript::SetAnimStyle(const char *dir, const char *set)
{
	_asm jmp COffsets::FUNC_CHuman__SetAnimStyle;
}

void _declspec(naked) C_HumanScript::SetPhysState(ePhysState state)
{
	_asm jmp  COffsets::FUNC_CHuman__SetPhysState;
}

void _declspec(naked) C_HumanScript::SetStealthMove(bool enable)
{
	_asm jmp COffsets::FUNC_CHuman__SetStealthMove;
}

C_SyncObject _declspec(naked) *C_HumanScript::GetInOutCar(C_SyncObject **syncObject, M2Vehicle *pVehicle, int enter, int seat, bool forced, int unk, int unk2)
{
	_asm jmp COffsets::FUNC_CHuman__GetInOutCar;
}

int _declspec(naked) C_HumanScript::PlayAnim(C_SyncObject **syncObject, const char *const animName, const bool repeat, int, int, float, float, float)
{
	_asm {
		mov eax, 0x0982CC0
		jmp eax
	}
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrAimAt(C_SyncObject **syncObject, M2Entity *ent, CVector3 const &pos, const bool smooth)
{
	_asm jmp COffsets::FUNC_CHuman__AimAt;
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrLookAt(C_SyncObject **syncObject, M2Entity *entity, CVector3 const &dir, const bool smooth)
{
	_asm jmp COffsets::FUNC_CHuman__LookAt;
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrMoveV(C_SyncObject **syncObject, const CVector3 &begin, const E_HumanMoveMode moveMode, const CVector3 &target, const bool smoothStop)
{
	_asm jmp COffsets::FUNC_CHuman__MoveVec;
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrShootAt(C_SyncObject **syncObject, M2Entity *entity, CVector3 const &dir, const bool smooth)
{
	_asm jmp COffsets::FUNC_CHuman__ShootAt;
}

C_SyncObject _declspec(naked) *C_HumanScript::ScrShootAtEffect(C_SyncObject **syncObject, CVector3 const &dir, CVector3 const &unk1, CVector3 const &unk2, int unk, bool smooth)
{
	_asm jmp COffsets::FUNC_CHuman__ShootAtEffect;
}

void _declspec(naked) C_HumanScript::SetSelectedWeapon(DWORD dwWeapon, bool bUseAnimation, int unk)
{
	_asm jmp COffsets::FUNC_CHuman__InventorySelect;
}

float _declspec(naked) C_HumanScript::GetRealHealth()
{
	_asm jmp COffsets::FUNC_CHuman__GetRealHealth;
}

void _declspec(naked) M2WeaponData::AddWeapon(DWORD weapon, DWORD ammo)
{
	_asm jmp COffsets::FUNC_CHuman__InventoryAddWeapon;
}

void _declspec(naked) M2WeaponData::AddMoney(int money, int unk)
{
	_asm {
		mov eax, 0x949C80;
		jmp eax;
	}
}

float _declspec(naked) M2WeaponData::GetMoney()
{
	_asm {
		mov eax, 0x931B80;
		jmp eax;
	}
}

void _declspec(naked) M2WeaponData::RemoveMoney(int money, int unk)
{
	_asm {
		mov eax, 0x93BE60;
		jmp eax;
	}
}

void _declspec(naked) M2WeaponData::RemoveWeapon(DWORD weapon)
{
	_asm jmp COffsets::FUNC_CHuman__InventoryRemoveWeapon;
}

void _declspec(naked) M2WeaponData::ReloadWeapon(int index, int unk)
{
	_asm jmp COffsets::FUNC_CHuman__InventoryDoReload;
}

bool _declspec(naked) M2WeaponData::HasItem(DWORD item)
{
	_asm jmp COffsets::FUNC_CHuman__InventoryHaveItem;
}

bool _declspec(naked) C_HumanWeaponController::HaveThrowingWeaponInHand()
{
	_asm jmp COffsets::FUNC_CHuman__HaveThrowingWeaponInHand
}

CM2Ped::CM2Ped(M2Ped * pPed) : CM2Entity(pPed),
	m_pPed(pPed)
{
}

CM2Ped::~CM2Ped(void)
{
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
	if (m_pPed && m_pPed->m_pWeaponData)
		m_pPed->m_pWeaponData->m_bInifiniteAmmo = bInfiniteAmmo;
}

bool CM2Ped::HasInfiniteAmmo(void)
{
	if (m_pPed && m_pPed->m_pWeaponData)
		return m_pPed->m_pWeaponData->m_bInifiniteAmmo;

	return false;
}

void CM2Ped::SetSelectedWeapon(DWORD dwWeapon, bool bUseAnimation)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		if (dwWeapon < 0 || dwWeapon > 21)
			return;

		m_pPed->m_pHumanScript->SetSelectedWeapon(dwWeapon, bUseAnimation, 0);
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
	if (m_pPed)
		return m_pPed->m_fHealthMax;

	return 0.0f;
}

float CM2Ped::GetRealHealth(void)
{
	float fRealHealth = 0.0f;

	if (m_pPed && m_pPed->m_pHumanScript)
	{
		fRealHealth = m_pPed->m_pHumanScript->GetRealHealth();
	}

	return fRealHealth;
}

void CM2Ped::SetInvulnerable(bool bInvulnerable)
{

	if (m_pPed)
		m_pPed->m_bInvulnerable = bInvulnerable;
}

bool CM2Ped::IsInvulnerable(void)
{
	if (m_pPed)
		return m_pPed->m_bInvulnerable;

	return false;
}

void CM2Ped::GiveWeapon(DWORD dwWeapon, DWORD dwAmmo)
{
	if (m_pPed && m_pPed->m_pWeaponData)
	{
		if (dwWeapon <= 0 || dwWeapon > 21)
			return;

		m_pPed->m_pWeaponData->AddWeapon(dwWeapon, dwAmmo);
	}
}

void CM2Ped::RemoveWeapon(DWORD dwWeapon, DWORD dwAmmo)
{
	if (m_pPed && m_pPed->m_pWeaponData)
	{
		if (dwAmmo == 0)
		{
			m_pPed->m_pWeaponData->RemoveWeapon(dwWeapon);
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
	if (m_pPed && m_pPed->m_pWeaponData)
	{
		m_pPed->m_pWeaponData->ReloadWeapon(0, -1);
	}
}

bool CM2Ped::HaveThrowingWeaponInHand(void)
{
	bool bHaveThrowingWeaponInHand = false;

	if (m_pPed && m_pPed->m_pWeaponController)
	{
		bHaveThrowingWeaponInHand = m_pPed->m_pWeaponController->HaveThrowingWeaponInHand();
	}

	return bHaveThrowingWeaponInHand;
}

bool CM2Ped::HasItem(DWORD dwItem)
{
	bool bHasItem = false;

	if (m_pPed && m_pPed->m_pWeaponData)
	{
		bHasItem = m_pPed->m_pWeaponData->HasItem(dwItem);
	}

	return bHasItem;
}

C_SyncObject * CM2Ped::GetInOutVehicle(M2Vehicle * pVehicle, int iSeat, bool bEnter, bool bForced)
{
	C_SyncObject * pReturnObj = nullptr;

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

		m_pPed->m_pHumanScript->GetInOutCar(&pReturnObj, pVehicle, iEnter, iSeat, bForced, 0, 2);
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
	if (m_pPed)
	{
		// If we want to disable hud OR sound
		//CPatcher::InstallNopPatch( 0x949CFA , 0x9 ); // stop hud showing
		//CPatcher::InstallNopPatch( 0x949CB6 , 0x15 ); // stop sound playing
		m_pPed->m_pWeaponData->AddMoney(SharedUtility::int_concat(iDollars, iCents), 0);
	}
}

void CM2Ped::RemoveMoney(int iDollars, int iCents)
{
	if (m_pPed)
	{
		m_pPed->m_pWeaponData->RemoveMoney(SharedUtility::int_concat(iDollars, iCents), 0);
	}
}

float CM2Ped::GetMoney(void)
{
	float fMoney = 0.0f;

	if (m_pPed)
	{
		fMoney = m_pPed->m_pWeaponData->GetMoney();
	}
	return fMoney;
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

	return nullptr;
}

C_SyncObject * CM2Ped::AimAt(CVector3 vecPosition)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject = nullptr;
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

		M2Entity * pEnt = nullptr;

		CLocalPlayer *pLocalPlayer = CLocalPlayer::Instance();
		if (pLocalPlayer && pLocalPlayer->IsSpawned())
			pEnt = pLocalPlayer->GetPlayerPed()->GetPed();

		pHumanScript->ScrAimAt(&pSyncObject, pEnt, vecPosition, true);

		return pSyncObject;
	}
	return nullptr;
}

C_SyncObject * CM2Ped::ShootAt(CVector3 vecPosition)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject = nullptr;
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;
		CVector3 vecUnknown(0.0f, 0.0f, 0.0f);
		
		pHumanScript->ScrShootAtEffect(&pSyncObject, vecPosition, vecUnknown, vecUnknown, 50, true);

		return pSyncObject;
	}
	return nullptr;
}

C_SyncObject * CM2Ped::LookAt(CVector3 vecPosition)
{
	if (m_pPed && m_pPed->m_pHumanScript)
	{
		C_SyncObject * pSyncObject = nullptr;
		C_HumanScript * pHumanScript = m_pPed->m_pHumanScript;

		M2Entity * pEnt = nullptr;

		CLocalPlayer *pLocalPlayer = CLocalPlayer::Instance();
		if (pLocalPlayer && pLocalPlayer->IsSpawned())
			pEnt = pLocalPlayer->GetPlayerPed()->GetPed();
		
		pHumanScript->ScrLookAt(&pSyncObject, pEnt, vecPosition, true);

		return pSyncObject;
	}
	return nullptr;
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

void CM2Ped::ModelToHand(int iHand, int iModel)
{
	if (iHand < 1 || iHand > 3) // 1 : right hand & 2 : left hand & 3 : both hand
		return;

	if (iModel < 0 || iModel > 120)
		return;

	if (!m_pPed)
		return;

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

	m_pPed->m_pHumanScript->ModelToHand(0, leftModel, rightModel);
}

void CM2Ped::ModelToMouth(int iModel)
{
	if (!m_pPed)
		return;

	m_pPed->m_pHumanScript->ModelToMouth(iModel);
}

void CM2Ped::SetAnimStyle(const char *dir, const char *set)
{
	if (strlen(dir) <= 0 || strlen(set) <= 0)
		return;

	if (!m_pPed)
		return;

	m_pPed->m_pHumanScript->SetAnimStyle(dir, set);
}

void CM2Ped::SetPhysState(ePhysState state)
{
	if (!m_pPed)
		return;

	m_pPed->m_pHumanScript->SetPhysState(state);
}

ePhysState	CM2Ped::GetPhysState(void)
{
	if (!m_pPed)
		return (E_DYNAMIC);

	ePhysState retn = m_pPed->m_pHumanScript->GetPhysState();

	return (retn);
}

void CM2Ped::SetStealthMove(bool bStealthMove)
{
	if (!m_pPed)
		return;

	m_pPed->m_pHumanScript->SetStealthMove(bStealthMove);
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