/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CIE.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#include "CCommon.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "engine/CM2ModelManager.h"
#include "CM2ScriptMachine.h"

class CVector3;

enum E_ObjType : unsigned char
{
	OBJTYPE_Human = 0xE, // 14
	OBJTYPE_Player = 0x10, // 16
	OBJTYPE_Car = 0x12, // 18
	OBJTYPE_Train = 0x13, // 19
	OBJTYPE_TrafficHuman = 0x16, // 22
	OBJTYPE_Item = 0x24, // 36
	OBJTYPE_Door = 0x26, // 28
	OBJTYPE_UNK1 = 0x27, // 39 - Unknow
	OBJTYPE_UNK2 = 0x2D, // 45 - Unknow
	OBJTYPE_UNK3 = 0x2E, // 46 - Unknow
	OBJTYPE_UNK4 = 0x34, // 52 - Function similar to UNK1 but unknow
	OBJTYPE_UNK5 = 0x42, // 66 - Something related to car
	OBJTYPE_UNK6 = 0x47 // 71 - Related to crashobject (C_WrapperLightEntity)
};

class M2Window
{
public:
	PAD(M2Window, pad0, 0xC);					// 0000 - 000C
	bool m_bMinimized;							// 000C - 000D
	bool m_bFocus;								// 000D - 0011
	PAD(M2Window, pad1, 0xA);					// 0011 - 001B
	HMODULE hModule;							// 001B - 001F
	HWND m_hWnd;								// 001F - 0023

	// 0x18 = HMODULE
	// 0x1C = HWND
	// 0x20 = window name

	// Maybe it's just marking as minimized?
	void SetMinimized(bool bMinimized)
	{
		DWORD dwFunc = 0x57BDE0;

		_asm push bMinimized;
		_asm mov ecx, this;
		_asm call dwFunc;
	}

	bool HasFocus(void)
	{
		return m_bFocus;
	}

	static M2Window * GetInstance(void);
};

class M2GameVFTable
{
public:
	PAD(M2GameVFTable, pad0, 0x50);				// 0000 - 0050
	// 0xC = Get class (Holds LoadActorsFromFile)
	// 0x14 = SetupGame (Creates localplayer and other shit)
	// 0x20	= SetupLocalPlayer
	// 0x24 = DestroyLocalPlayer
	DWORD GetEntityFromIndex;					// 0050 - 0054
	DWORD SetEntityAtIndex;						// 0054 - 0058
	DWORD IsLocalPlayerActive;					// 0058 - 005C
};

class M2GameUnknown001
{
public:
	PAD(M2GameUnknown001, pad0, 0xDC);
	void* m_pUnknown;
};

class M2Game
{
public:
	M2GameVFTable * m_pVFTable;					// 0000 - 0004
	DWORD m_dwFlags;						// 0004 - 0008
	DWORD m_dwRunTime;							// 0008 - 000C
	// 0xC = Pointer to unknown class that contains LoadActorsFromFile
	// 0x9 = actors bin directory (char *)
	// 0x10 = shops directory (char *)
	// 0x28 = IsLocalPlayerCreated
	PAD(M2Game, pad1, 0x24);					// 000C - 0030
	M2GameUnknown001* m_pUnkClass001;			// 0030 - 0034
	PAD(M2Game, pad2, 0xE4);					// 000C - 0118
	M2Ped * m_pLocalPed;						// 0118 - 011C
	PAD(M2Game, pad3, 0x1C);					// 011C - 0138
	DWORD m_dwObjectsLoaded;					// 0138 - 013C

	// 0x130 = Entity Pool Size (DWORD)
	// 0x144 = Entity Pool

	static M2Game * GetInstance(void);

	void SetEntityAtIndex(int iIndex, M2Entity * pEntity)
	{
		// Get the function address
		DWORD dwFunc = m_pVFTable->SetEntityAtIndex;

		_asm push iIndex;
		_asm push pEntity;
		_asm mov ecx, this;
		_asm call dwFunc;
	}

	// 1370 total indexs - fuck knows what kind of entitys there is. (Index 0 = LocalPlayer from 0x118)
	M2Entity * GetEntityFromIndex(int iIndex)
	{
		M2Entity * pReturn = NULL;

		// Get the function address
		DWORD dwFunc = m_pVFTable->GetEntityFromIndex;

		_asm push iIndex;
		_asm mov ecx, this;
		_asm call dwFunc;
		_asm mov pReturn, eax;

		return pReturn;
	}
};

class M2ScriptEngine
{
public:
	void * m_pVFTable;							// 0000 - 0004
	M2ScriptHandler * m_pScriptHandler;			// 0004 - 0008

	static M2ScriptEngine * GetInstance(void);
};

class M2WrapperList
{
public:
	static M2WrapperList * GetInstance(void);
};

namespace IE
{
	void				* alloc(int size);
	void				free(void * memory);

	M2Game				* GetGame(void);
	M2Window			* GetWindow(void);
	M2ScriptEngine		* GetScriptEngine(void);
	M2WrapperList		* GetWrapperList(void);

	bool				LoadPointers(void);
	HWND				GetWindowHandle(void);
	lua_State			* GetState(void);

	M2Ped				* CreateEnginePed(M2ModelManager * pModelManager);
	M2Vehicle			* CreateEngineVehicle(M2ModelManager * pModelManager);

	M2Ped				* CreateEnginePlayerPed(M2ModelManager * pModelManager);

	CM2Ped				* CreateWrapperPed(CM2ModelManager * pModelManager);
	CM2Vehicle			* CreateWrapperVehicle(CM2ModelManager * pModelManager);
};