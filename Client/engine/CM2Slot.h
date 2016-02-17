/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2ModelManager.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*			   AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../BaseInc.h"

class M2Model;
class M2Slot
{
public:
	PAD(M2Slot, pad0, 0x18);		// 0000 - 0018
	char m_szName[32];				// 0018 - 0038
	PAD(M2Slot, pad1, 0x8);			// 0038 - 0040
	int m_iType;					// 0040 - 0044
	PAD(M2Slot, pad2, 0x10);		// 0044 - 0054
	int m_iRefCount;				// 0054 - 0058
	PAD(M2Slot, pad3, 0x1C);		// 0058 - 0074
	char m_szModelPath[260];		// 0074 - 0178
	PAD(M2Slot, pad4, 0xC);			// 0178 - 0184
	void * m_pUnknown;				// 0184 - 0188
	PAD(M2Slot, pad5, 0x20);		// 0188 - 01A8
	int m_iSlotNumber;				// 01A8 - 01AC

public:
	M2Model * GetModelByName(const char * szModelName)
	{
		M2Model * pModel = NULL;
		DWORD dwFunc = 0x588FD0; // M2Model * C_Slot::GetModelByName ( const char * szModelName );

		_asm push szModelName;
		_asm mov ecx, this;
		_asm call dwFunc;
		_asm mov pModel, eax;

		return pModel;
	}
};

class M2SlotDesc // Size: 0x10
{
public:
	WORD m_wNameOffset;				// 0000 - 0002
	WORD m_wMaxSlots;				// 0002 - 0004
	WORD m_SlotType;				// 0004 - 0006
	PAD(M2SlotDesc, pad0, 0x4);		// 0006 - 0010

public:
	const char * GetName(const char * szName)
	{
		return (const char *)((DWORD)(szName)+m_wNameOffset);
	}
};