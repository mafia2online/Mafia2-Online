/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Entity.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "COffsets.h"

#include "Math/CVector3.h"
#include "Math/CQuaternion.h"

#include "engine/CM2Ped.h"

#include "CM2Entity.h"

void _declspec(naked) M2Entity::Activate()
{
	_asm jmp COffsets::FUNC_CEntity__Activate;
}

void _declspec(naked) M2Entity::Deactivate()
{
	_asm jmp COffsets::FUNC_CEntity__Deactivate;
}

void _declspec(naked) M2Entity::Delete()
{
	_asm {
		mov eax, 0x1195580;
		jmp eax;
	}
}

CM2Entity::CM2Entity(M2Entity * pEntity):
	m_pEntity(pEntity)
{
}

CM2Entity::~CM2Entity(void)
{
	if (!m_pEntity)
		return;

	m_pEntity->Delete();
}

void CM2Entity::Activate(void)
{
	if (!m_pEntity)
		return;

	m_pEntity->Activate();
}

void CM2Entity::Deactivate(void)
{
	if (!m_pEntity)
		return;

	m_pEntity->Deactivate();
}

DWORD CM2Entity::GetGUID(void)
{
	if (m_pEntity)
		return m_pEntity->m_dwGUID;

	return 0;
}

void CM2Entity::SetPosition(CVector3 vecPosition)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->SetPosition;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		lea ecx, vecPosition;
		push ecx;
		mov ecx, pEntity;
		call dwFunc;
	}
}

void CM2Entity::GetPosition(CVector3 * vecPosition, bool bCheckVehicle)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->GetPosition;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		push vecPosition;
		mov ecx, pEntity;
		call dwFunc;
	}

	// If the ped is in a vehicle, adjust the Z axis
	// TODO: Use the engine function which does exactly this.
	if (bCheckVehicle && ((M2Ped *)m_pEntity)->m_pCurrentVehicle)
		vecPosition->fZ -= 1.0f;
}

void CM2Entity::SetDirection(CVector3 vecDirection)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->SetDirection;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		lea ecx, vecDirection;
		push ecx;
		mov ecx, pEntity;
		call dwFunc;
	}
}

void CM2Entity::GetDirection(CVector3 * vecDirection)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->GetDirection;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		push vecDirection;
		mov ecx, pEntity;
		call dwFunc;
	}
}

void CM2Entity::SetRotation(Quaternion quatRotation)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->SetRotation;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		lea ecx, quatRotation;
		push ecx;
		mov ecx, pEntity;
		call dwFunc;
	}
}

void CM2Entity::GetRotation(Quaternion * quatRotation)
{
	if (!m_pEntity)
		return;

	DWORD dwFunc = m_pEntity->m_pVFTable->GetRotation;
	M2Entity * pEntity = m_pEntity;

	__asm
	{
		push quatRotation;
		mov ecx, pEntity;
		call dwFunc;
	}
}

bool CM2Entity::IsOnScreen(void)
{
	bool bIsOnScreen = false;

	if (!m_pEntity || !m_pEntity->m_pModel || !m_pEntity->m_pModel->m_pSingleMesh || !m_pEntity->m_pModel->m_pSingleMesh->m_pRenderSkinnedVisual)
		return bIsOnScreen;

	DWORD dwLastModelRenderTime = m_pEntity->m_pModel->m_pSingleMesh->m_pRenderSkinnedVisual->m_dwLastRenderTime;

	void* pEngineCore = *(void **)0x1AAF6C8;
	DWORD dwEngineLastRenderTime = *(DWORD *)((DWORD)pEngineCore + 0x9C);

	if (dwLastModelRenderTime >= dwEngineLastRenderTime)
		bIsOnScreen = true;

	return bIsOnScreen;
}

void CM2Entity::ShowModel(bool bShow)
{
	if (!m_pEntity || !m_pEntity->m_pModel)
		return;

	M2Model * pModel = m_pEntity->m_pModel;

	// Toggle the model visibilty
	if (bShow)
		pModel->m_dwFlags |= 1;
	else
		pModel->m_dwFlags &= 0xFFFFFFFE;

	// Refresh model?
	DWORD dwFunc = 0x14AEAC0;
	_asm push 6;
	_asm mov ecx, pModel;
	_asm call dwFunc;
}

bool CM2Entity::IsModelShowing(void)
{
	if (m_pEntity && m_pEntity->m_pModel)
		return (m_pEntity->m_pModel->m_dwFlags & 1);

	return false;
}