/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2ModelManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include	"../BaseInc.h"

#include	"../CIE.h"
#include	"../COffsets.h"

#include	"../CModelManager.h"
#include	"CM2ModelManager.h"

CM2ModelManager::CM2ModelManager(void)
{
	// Reset
	m_pModelManager = NULL;

	// Allocate the model manager memory
	M2ModelManager * pModelManager = (M2ModelManager *)IE::alloc(0x50);

	// Did the memory allocate succeed?
	if (pModelManager)
	{
		// Construct the model manager
		_asm mov ecx, pModelManager;
		_asm call COffsets::FUNC_CModelMgr__Construct;

		// Store the model manager point
		m_pModelManager = pModelManager;
	}
}

CM2ModelManager::~CM2ModelManager(void)
{
	CLogFile::Printf("CM2ModelManager::~CM2ModelManager");

	// Do we have an allocated model?
	if (m_pModelManager)
	{
		// Free the model manager
		Free();

		// Reset the model manager pointer
		m_pModelManager = NULL;
	}
}

bool CM2ModelManager::Load(const char * szModelDirectory, const char * szModelName)
{
	// Do we not have a valid model manager pointer?
	if (!m_pModelManager)
		return false;

	// Get the model manager pointer
	bool bResult = false;
	M2ModelManager * pModelManager = m_pModelManager;

	// Set the model directory
	CModelManager::SetDir(szModelDirectory);

	// Load the model
	_asm push szModelName;
	_asm mov ecx, pModelManager;
	_asm call COffsets::FUNC_CModelMgr__Load;
	_asm mov bResult, al;

	// Copy the model name to the model manager
	strcpy(pModelManager->m_szModelName, szModelName);

	// Restore the old model load directory
	CModelManager::SetDir(SDS_LOAD_DIR_PLAYER);

	return bResult;
}

void CM2ModelManager::Free(void)
{
	// Do we not have a valid model manager?
	if (!m_pModelManager)
		return;

	// Get the model manager pointer
	M2ModelManager * pModelManager = m_pModelManager;

	// Free the model
	_asm mov ecx, pModelManager;
	_asm call COffsets::FUNC_CModelMgr__Free;
}

bool CM2ModelManager::ChangeModel(const char * szModelDirectory, const char * szModelName, int iHumanColour)
{
	// Do we not have a valid model manager?
	if (!m_pModelManager)
		return false;

	bool bResult = false;

	// Set the model directory
	CModelManager::SetDir(szModelDirectory);

	// Change the model
	_asm push iHumanColour;
	_asm push szModelName;
	_asm mov ecx, this;
	_asm call COffsets::FUNC_CModelMgr__ChangeModel;
	_asm mov bResult, al;

	// Restore the old model load directory
	CModelManager::SetDir(SDS_LOAD_DIR_PLAYER);

	return bResult;
}

M2Slot * CM2ModelManager::GetSlot(void)
{
	// Is the model manager invalid?
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pSlot;
}

M2Entity * CM2ModelManager::GetEntity(void)
{
	// Is the model manager invalid?
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pEntity;
}

M2Model * CM2ModelManager::GetModel(void)
{
	// Is the model manager invalid?
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_pModel;
}

const char * CM2ModelManager::GetModelName(void)
{
	// Is the model manager invalid?
	if (!m_pModelManager)
		return NULL;

	return (const char *)m_pModelManager->m_szModelName;
}

int CM2ModelManager::GetModelColour(void)
{
	// Is the model manager invalid?
	if (!m_pModelManager)
		return NULL;

	return m_pModelManager->m_iHumanColour;
}