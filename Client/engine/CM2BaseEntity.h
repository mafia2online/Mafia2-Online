/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2BaseEntity.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class M2Entity;

class M2BaseEntityVFTable
{
public:
	DWORD Constructor;							// 0000 - 0004
	DWORD GetType;								// 0004 - 0008
	DWORD Deconstructor;						// 0008 - 000C
	PAD(M2BaseEntityVFTable, pad0, 0x8);		// 000C - 0014
	DWORD IsValid;								// 0014 - 0018
	DWORD IsInvalid;							// 0018 - 001C
	PAD(M2BaseEntityVFTable, pad1, 0x4);		// 001C - 0020
	DWORD Activate;								// 0020 - 0024
	DWORD Deactivate;							// 0024 - 0028
	DWORD Restore;								// 0028 - 002C
	DWORD IsActive;								// 002C - 0030
	PAD(M2BaseEntityVFTable, pad2, 0x14);		// 0030 - 0044
};

class M2BaseEntity
{
public:
	M2BaseEntityVFTable * m_pVFTable;			// 0000 - 0004
	DWORD m_dwFlags;							// 0004 - 0008
	M2Entity * m_pEntity;						// 0008 - 000C
};