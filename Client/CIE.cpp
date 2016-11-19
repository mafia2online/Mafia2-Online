/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CIE.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CModelManager.h"

#include "engine/CM2Misc.h"
#include "engine/CM2Entity.h"
#include "engine/CM2Core.h"
#include "engine/CM2Ped.h"
#include "engine/CM2Vehicle.h"
#include "engine/CM2SlotManager.h"

#include "CM2ScriptMachine.h"

#include "COffsets.h"

#include "CIE.h"

#include "COffsets.h"
#include "CString.h"

#include "Game/CGame.h"

#include "CNetworkModelManager.h"

#include "Math/CVector3.h"

#include "lua.hpp"

typedef void * (__cdecl * CreateObjectByType_t)(int);
CreateObjectByType_t CreateObjectByType = (CreateObjectByType_t)(0x0115F840);

typedef int(__thiscall * CModel__AssignModel_t)(M2Model *, M2Model *);
CModel__AssignModel_t CModel__AssignModel = (CModel__AssignModel_t)(0x014E2970);

typedef void(__thiscall * CModel__SetNameHashByName_t)(M2Model *, char *);
CModel__SetNameHashByName_t CModel__SetNameHashByName = (CModel__SetNameHashByName_t)(0x014AEA40);

typedef void(__thiscall * CModel__SetFlag_t)(M2Model *, int);
CModel__SetFlag_t CModel__SetFlag = (CModel__SetFlag_t)(0x014AEAC0);

typedef void * (__thiscall * CEntity__SetModel_t)(M2Entity *, M2Model *);
CEntity__SetModel_t CEntity__SetModel = (CEntity__SetModel_t)(0x0115CB10);

typedef void * (__thiscall * CEntity__UnkFunc_t)(M2Entity*);
CEntity__UnkFunc_t CEntity__UnkFunc = (CEntity__UnkFunc_t)(0x0098DA20);

typedef void * (__cdecl * GetHumanParamsByTableIndex_t)(int, int);
GetHumanParamsByTableIndex_t GetTrafficHumanParams = (GetHumanParamsByTableIndex_t)(0x00A2FD60);

typedef bool(__thiscall * CEntity__SetHumanParams_t)(M2Entity *, void *);
CEntity__SetHumanParams_t CEntity__SetHumanParams = (CEntity__SetHumanParams_t)(0x0093E6A0);

typedef void * (__thiscall * GetHashedString_t)(void **, char *);
GetHashedString_t GetHashedString = (GetHashedString_t)(0x180F3E0);

typedef M2Model * (__thiscall * LoadModelFromHashName_t)(void * pszModelHashName);
LoadModelFromHashName_t LoadModelFromHashName = (LoadModelFromHashName_t)(0x124F2F0);

typedef void(__thiscall * CObject__Setup_t)(M2Entity *);
CObject__Setup_t CCar__Setup = (CObject__Setup_t)(0x00A04B40);

typedef bool(__thiscall * CObject__SetParams_t)(M2Entity *, void *);
CObject__SetParams_t CObject__SetCarParams = (CObject__SetParams_t)(0x009906B0);

M2Window * M2Window::GetInstance(void)
{
	static M2Window * pWindow;

	if (!pWindow)
		pWindow = *(M2Window **)COffsets::VAR_CWindow;

	return pWindow;
}

M2Game * M2Game::GetInstance(void)
{
	static M2Game * pGame;

	if (!pGame)
		pGame = *(M2Game **)COffsets::VAR_CGame;

	return pGame;
}

M2ScriptEngine * M2ScriptEngine::GetInstance(void)
{
	static M2ScriptEngine * pScriptEngine;

	if (!pScriptEngine)
		pScriptEngine = *(M2ScriptEngine **)COffsets::VAR_CGame__ScriptEngine;

	return pScriptEngine;
}

M2WrapperList * M2WrapperList::GetInstance(void)
{
	static M2WrapperList * pWrapperList;

	if (!pWrapperList)
		pWrapperList = *(M2WrapperList **)COffsets::VAR_CWrapperList;

	return pWrapperList;
}

DWORD CWrappersList__GetEntityByName = 0x0D8E3B0;
M2Entity _declspec(naked) * M2WrapperList::GetEntityByName(const char *name)
{
	_asm jmp CWrappersList__GetEntityByName;
}

DWORD CWrappersList__GetEntityByGUID = 0x0D8E260;
M2Entity _declspec(naked) * M2WrapperList::GetEntityByGUID(unsigned int guid)
{
	_asm jmp CWrappersList__GetEntityByGUID;
}

namespace IE
{
	M2Game * pGame = NULL;
	M2Window * pWindow = NULL;
	M2ScriptEngine * pScriptEngine = NULL;
	M2WrapperList * pWrapperList = NULL;

	M2Game * GetGame(void) { return pGame; }
	M2Window * GetWindow(void) { return pWindow; }
	M2ScriptEngine * GetScriptEngine(void) { return pScriptEngine; }
	M2WrapperList * GetWrapperList(void) { return pWrapperList; }

	void * alloc(int iSize)
	{
		void * memory = NULL;

		DWORD dwFunc = COffsets::FUNC_CGame__Allocate;

		__asm
		{
			push iSize;
			call dwFunc;
			add esp, 4;
			mov memory, eax;
		}

		return memory;
	}

	void free(void * memory)
	{
		DWORD dwFunc = COffsets::FUNC_CGame__Free;

		__asm
		{
			push memory;
			call dwFunc;
			add esp, 4;
		}
		memory = NULL;
	}

	M2Ped * CreateEnginePlayerPed(M2ModelManager * pModelManager)
	{
		// Is the model manager invalid or has no model loaded?
		if (!pModelManager || !pModelManager->m_pModel)
			return NULL;

		// Create the ped player object
		M2Ped * pPed = (M2Ped *)(CreateObjectByType(OBJTYPE_Player));

		// Did the ped fail to create?
		if (!pPed)
			return NULL;

		// Allocate the model memory
		M2Model * pModel = CM2Core::GetInstance()->AllocateModel();

		// Did the model fail to allocate?
		if (!pModel)
		{
			// Delete the ped memory
			SAFE_DELETE(pPed);
			return NULL;
		}

		// Assign the model
		CModel__AssignModel(pModel, pModelManager->m_pModel);

		// Set the model name hash
		CModel__SetNameHashByName(pModel, "m2online_player");

		// Set model flags
		CModel__SetFlag(pModel, 2);

		// Set the entity model
		CEntity__SetModel(pPed, pModel);

		// Set slot SDS number
		pPed->m_iSlotSDS = pModelManager->m_pSlot->m_iSlotNumber;

		// Setup player
		CEntity__UnkFunc(pPed);

		// Set the entity flags
		pPed->m_dwFlags &= 0xFFFFFFBF | 0x10000;

		// Has the entity created successfully?
		if (pPed->m_dwFlags & 0x20)
			return pPed;

		// Delete memory
		free((void *)pModel);
		SAFE_DELETE(pPed);

		return NULL;
	}

	bool LoadPointers(void)
	{
		pGame = M2Game::GetInstance();
		pWindow = M2Window::GetInstance();
		pScriptEngine = M2ScriptEngine::GetInstance();
		pWrapperList = M2WrapperList::GetInstance();

		return (pGame != NULL && pWindow != NULL && pScriptEngine != NULL && pWrapperList != NULL);
	}

	HWND GetWindowHandle(void)
	{
		if (pWindow)
			return pWindow->m_hWnd;

		return NULL;
	}

	bool HasWindowFocus(void)
	{
		if (pWindow)
			return pWindow->m_bFocus;

		return false;
	}

	lua_State * GetState(void)
	{
		if (!pScriptEngine) return NULL;
		if (!pScriptEngine->m_pScriptHandler) return NULL;
		if (!pScriptEngine->m_pScriptHandler->m_pScriptMachinePool) return NULL;
		if (!pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine) return NULL;
		if (!pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine[0]) return NULL;

		return pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine[0]->m_L;
	}

	M2Ped * CreateEnginePed(M2ModelManager * pModelManager)
	{
		// Is the model manager invalid or has no model loaded?
		if (!pModelManager || !pModelManager->m_pModel)
			return NULL;

		// Create the ped human object
		M2Ped * pPed = (M2Ped *)(CreateObjectByType(OBJTYPE_Human));

		// Did the ped fail to create?
		if (!pPed)
			return NULL;

		// Allocate the model memory
		M2Model * pModel = CM2Core::GetInstance()->AllocateModel();

		// Did the model fail to allocate?
		if (!pModel)
		{
			// Delete the ped memory
			SAFE_DELETE(pPed);
			return NULL;
		}

		// Assign the model
		CModel__AssignModel(pModel, pModelManager->m_pModel);

		// Set the model name hash
		CModel__SetNameHashByName(pModel, "m2online_ped");

		// Set the initial position
		*(float *)((DWORD)(pModel)+0x44) = 0.0f;
		*(float *)((DWORD)(pModel)+0x54) = 0.0f;
		*(float *)((DWORD)(pModel)+0x64) = 0.0f;

		// Set model flags
		CModel__SetFlag(pModel, 2);

		// Set the entity model
		CEntity__SetModel(pPed, pModel);

		// Unknown
		CEntity__UnkFunc(pPed);

		// Set the entity flags
		pPed->m_dwFlags &= 0xFFFFF7FF | 0x80000;

		// Has the entity created successfully?
		if (pPed->m_dwFlags & 0x20)
			return pPed;

		// Delete memory
		free((void *)pModel);
		SAFE_DELETE(pPed);

		return NULL;
	}

	CM2Ped * CreateWrapperPed(CM2ModelManager * pModelManager)
	{
		// Create the engine ped
		M2Ped * pEnginePed = CreateEnginePed(pModelManager->GetModelManager());

		// Is the engine ped invalid?
		if (!pEnginePed)
			return NULL;

		// Create the wrapper ped
		CM2Ped * pPed = new CM2Ped(pEnginePed);

		// Return if the ped created
		if (pPed)
			return pPed;

		// Delete the engine ped isntance
		SAFE_DELETE(pEnginePed);
		return NULL;
	}

	M2Vehicle * CreateEngineVehicle(M2ModelManager * pModelManager)
	{
		// Is the model manager invalid or has no model loaded?
		if (!pModelManager || !pModelManager->m_pModel)
			return NULL;

		// Create the vehicle object
		M2Vehicle * pVehicle = (M2Vehicle *)(CreateObjectByType(OBJTYPE_Car));

		// Did the ped fail to create?
		if (!pVehicle)
			return NULL;

		// Allocate the model memory
		M2Model * pModel = CM2Core::GetInstance()->AllocateModel();

		// Did the model fail to allocate?
		if (!pModel)
		{
			// Delete the vehicle memory
			SAFE_DELETE(pVehicle);
			return NULL;
		}

		// Assign new model
		CModel__AssignModel(pModel, pModelManager->m_pModel);

		// Set name hash
		CModel__SetNameHashByName(pModel, "m2online_vehicle");

		// Set the initial position
		*(float *)((DWORD)(pModel)+0x44) = 0.0f;
		*(float *)((DWORD)(pModel)+0x54) = 0.0f;
		*(float *)((DWORD)(pModel)+0x64) = 0.0f;

		// Set flag
		CModel__SetFlag(pModel, 2);

		// Set model
		CEntity__SetModel(pVehicle, pModel);

		// set car params
		if (!CObject__SetCarParams(pVehicle, NULL))
		{
			// Delete the memory
			free((void *)pModel);
			SAFE_DELETE(pVehicle);
			return NULL;
		}

		// Set model slot number
		pVehicle->m_nSlotSDS = pModelManager->m_pSlot->m_iSlotNumber;

		// Setup
		CCar__Setup(pVehicle);

		// Set entity flags
		DWORD dwFlags = pVehicle->m_dwFlags & 0xFFFFFFBF | 0x4800;
		pVehicle->m_dwFlags = dwFlags;

		// Has the entity created successfully?
		if (pVehicle->m_dwFlags & 0x20)
			return pVehicle;

		// Delete memory
		free((void *)pModel);
		SAFE_DELETE(pVehicle);

		return NULL;
	}

	CM2Vehicle * CreateWrapperVehicle(CM2ModelManager * pModelManager)
	{
		// Create the engine vehicle
		M2Vehicle * pEngineVehicle = CreateEngineVehicle(pModelManager->GetModelManager());

		// Is the engine vehicle invalid?
		if (!pEngineVehicle)
			return NULL;

		// Create the wrapper vehicle
		CM2Vehicle * pVehicle = new CM2Vehicle(pEngineVehicle);

		// Return if the vehicle created
		if (pVehicle)
			return pVehicle;

		// Delete the engine vehicle isntance
		SAFE_DELETE(pEngineVehicle);
		return NULL;
	}

	M2Entity * GetEngineEntityByIndex(int i)
	{
		return (M2Game::GetInstance()->GetEntityFromIndex(i));
	}

	M2Entity * GetEngineEntityByName(const char *name)
	{
		return (M2Game::GetInstance()->GetEntityByname(name));
	}

	M2Entity * GetEngineEntityByGUID(unsigned int guid)
	{
		return (M2Game::GetInstance()->GetEntityByGUID(guid));
	}
};