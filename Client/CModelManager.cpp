/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game
* File       : CModelManager.cpp
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "engine/CM2SlotManager.h"

#include "CModelManager.h"

#include "CIE.h"

#include "CLogFile.h"

CModelManager::CModelManager(void)
	: m_modelManagers()
{
}

CModelManager::~CModelManager(void)
{
	// Clear the loaded model list
	m_modelManagers.clear();
}

CM2ModelManager * CModelManager::Load(String strModelDirectory, String strModelName)
{
	return Load(strModelDirectory.Get(), strModelName.Get());
}

CM2ModelManager * CModelManager::Load(const char * szModelDirectory, const char * szModelName)
{
	// Have we loaded too many models?
	if (m_modelManagers.size() >= MODELMGR_MAX)
	{
		CLogFile::Printf("ERROR - Can't load model because model limit reached! (%d/%d)", m_modelManagers.size(), MODELMGR_MAX);
		return NULL;
	}

	// Create the model manager instance
	CM2ModelManager * pModelManager = new CM2ModelManager();

	// Did the model manager fail to create?
	if (!pModelManager)
		return NULL;

	// Did the model fail to load?
	if (!pModelManager->Load(szModelDirectory, szModelName))
	{
		CLogFile::Printf("ERROR - Failed to load model '%s'", szModelName);

		// Delete the model manager instance
		SAFE_DELETE(pModelManager);
		return NULL;
	}

	DEBUG_LOG("CModelManager::Load ( \"%s\" ) - Model loaded successfully!", szModelName);

	// Add the current model manager to the list
	m_modelManagers.push_back(pModelManager);

	return pModelManager;
}

bool CModelManager::Free(const char * szModelName)
{
	// Loop over all model managers
	for (std::list < CM2ModelManager * >::iterator iter = m_modelManagers.begin(); iter != m_modelManagers.end(); iter++)
	{
		// Get the current model manager
		CM2ModelManager * pModelManager = (*iter);

		// Does the current model manager not have a slot linked?
		if (!pModelManager->GetModelManager()->m_pSlot)
			continue;

		// Is the model loaded in this slot?
		if (pModelManager->GetSlot()->GetModelByName(szModelName))
		{
			// Free the current model manager
			m_modelManagers.remove(pModelManager);

			// Delete the model manager instance
			SAFE_DELETE(pModelManager);

			DEBUG_LOG ("CModelManager::Free ( \"%s\" ) - Model unloaded successfully! (%d models loaded)", szModelName, m_modelManagers.size());

			return true;
		}
	}

	return false;
}

bool CModelManager::Free(CM2ModelManager * pModelManager)
{
	// Is the model manager invalid?
	if (!pModelManager || !pModelManager->GetModelManager())
		return false;

	// Try and remove by name
	return Free(pModelManager->GetModelName());
}

void CModelManager::Clear(void)
{
	// Loop over all loaded models
	for (std::list < CM2ModelManager * >::iterator iter = m_modelManagers.begin(); iter != m_modelManagers.end(); iter++)
	{
		// Delete the current model manager instance
		SAFE_DELETE(*iter);
	}

	// Clear the loaded model list
	m_modelManagers.clear();
}

CM2ModelManager * CModelManager::GetModelManagerByName(const char * szModelName)
{
	// Loop over all model managers
	for (std::list < CM2ModelManager * >::iterator iter = m_modelManagers.begin(); iter != m_modelManagers.end(); iter++)
	{
		// Get the current model manager
		CM2ModelManager * pModelManager = (*iter);

		// Does the current model manager not have a slot linked?
		if (!pModelManager->GetModelManager() || !pModelManager->GetModelManager()->m_pSlot)
			continue;

		// Is the model loaded in this slot?
		if (!strcmp(szModelName, pModelManager->GetModelManager()->m_szModelName))
			return pModelManager;
	}

	return NULL;
}

char * CModelManager::GetDir(void)
{
	return (char *)(SDS_LOAD_DIR_ADDR);
}

void CModelManager::SetDir(const char * pszDirectory)
{
	char * pszLoadPath = (char *)(SDS_LOAD_DIR_ADDR);

	DWORD dwOldProtect = 0;
	VirtualProtect(pszLoadPath, 0x20, PAGE_READWRITE, &dwOldProtect);

	// modify path
	sprintf(pszLoadPath, "%s%%s.sds", pszDirectory);

	VirtualProtect(pszLoadPath, 0x20, dwOldProtect, &dwOldProtect);
}