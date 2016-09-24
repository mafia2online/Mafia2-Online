/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2ModelManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#include "BaseInc.h"
#include "CM2Slot.h"
#include "CM2SlotManager.h"

class M2Slot;
class M2Entity;
class M2Model;

class M2ModelManager // size: 0x50
{
public:
	DWORD m_dwVFTable;					// 0000 - 0004
	M2Slot * m_pSlot;					// 0004 - 0008
	int m_iRefCount;					// 0008 - 000C
	PAD(M2ModelManager, pad0, 0x4);		// 000C - 0010
	M2Entity * m_pEntity;				// 0010 - 0014
	M2Model * m_pModel;					// 0014 - 0018
	void * m_pUnknown;					// 0018 - 001C
	PAD(M2ModelManager, pad1, 0x10);	// 001C - 002C
	char m_szModelName[32];				// 002C - 004C
	int m_iHumanColour;					// 004C - 0050
};

class CM2ModelManager
{

private:

	M2ModelManager 								* m_pModelManager;

public:

	CM2ModelManager(void);
	CM2ModelManager(M2ModelManager * pModelManager) { m_pModelManager = pModelManager; }
	~CM2ModelManager(void);

	bool				Load(const char * szModelDirectory, const char * szModelName);
	void				Free(void);
	bool				ChangeModel(const char * szModelDirectory, const char * szModelName, int iHumanColour = -1);

	M2Slot				* GetSlot(void);
	M2Entity			* GetEntity(void);
	M2Model				* GetModel(void);
	const char			* GetModelName(void);
	int					GetModelColour(void);

	M2ModelManager		* GetModelManager(void) { return m_pModelManager; }

};
