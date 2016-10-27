/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2ModelManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CIE.h"
#include "COffsets.h"

#include "CModelManager.h"
#include "CM2ModelManager.h"

#include "CLogFile.h"

CM2ModelManager::CM2ModelManager(void)
{
	m_pModelManager = NULL;

	M2ModelManager * pModelManager = (M2ModelManager *)IE::alloc(0x50);
	if (pModelManager)
	{
		__asm
		{
			mov ecx, pModelManager;
			call COffsets::FUNC_CModelMgr__Construct;
		}
		m_pModelManager = pModelManager;
	}
}

CM2ModelManager::~CM2ModelManager(void)
{
	DEBUG_LOG("CM2ModelManager::~CM2ModelManager");
	if (m_pModelManager)
	{
		Free();
		m_pModelManager = NULL;
	}
}

bool CM2ModelManager::Load(const char * szModelDirectory, const char * szModelName)
{
	if (!m_pModelManager)
		return false;

	bool bResult = false;
	M2ModelManager * pModelManager = m_pModelManager;

	CModelManager::SetDir(szModelDirectory);

	__asm
	{
		push szModelName;
		mov ecx, pModelManager;
		call COffsets::FUNC_CModelMgr__Load;
		mov bResult, al;
	}

	strcpy(pModelManager->m_szModelName, szModelName);
	CModelManager::SetDir(SDS_LOAD_DIR_PLAYER);

	return bResult;
}

void CM2ModelManager::Free(void)
{
	if (!m_pModelManager)
		return;

	M2ModelManager * pModelManager = m_pModelManager;

	__asm
	{
		mov ecx, pModelManager;
		call COffsets::FUNC_CModelMgr__Free;
	}
}

bool CM2ModelManager::ChangeModel(const char * szModelDirectory, const char * szModelName, int iHumanColour)
{
	if (!m_pModelManager)
		return false;

	bool bResult = false;

	CModelManager::SetDir(szModelDirectory);

	__asm
	{
		push iHumanColour;
		push szModelName;
		mov ecx, this;
		call COffsets::FUNC_CModelMgr__ChangeModel;
		mov bResult, al;
	}

	CModelManager::SetDir(SDS_LOAD_DIR_PLAYER);
	return bResult;
}

M2Slot * CM2ModelManager::GetSlot(void)
{
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pSlot;
}

M2Entity * CM2ModelManager::GetEntity(void)
{
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pEntity;
}

M2Model * CM2ModelManager::GetModel(void)
{
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pModel;
}

const char * CM2ModelManager::GetModelName(void)
{
	if (!m_pModelManager)
		return NULL;

	return (const char *)m_pModelManager->m_szModelName;
}

int CM2ModelManager::GetModelColour(void)
{
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_iHumanColour;
}