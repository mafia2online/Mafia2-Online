/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Navigation.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2Navigation.h"

#include "CLogFile.h"

void _declspec(naked) *M2Navigation::GetIconFromEntity(M2Entity *pEntity)
{
	_asm {
		mov eax, 0x4965C0;
		jmp eax;
	}
}

void _declspec(naked) *M2Navigation::GetIconFromId(int iconId)
{
	_asm {
		mov eax, 0x4963E0;
		jmp eax;
	}
}

void _declspec(naked) M2Navigation::SetForegroundColor(int icon, int color)
{
	_asm {
		mov eax, 0x083B740;
		jmp eax;
	}
}

void _declspec(naked) M2Navigation::SetBackgroundColor(int icon, int color)
{
	_asm {
		mov eax, 0x083B6F0;
		jmp eax;
	}
}

void _declspec(naked) M2Navigation::UnregisterIconEntity(M2Entity * pEntity, int unk)
{
	_asm jmp COffsets::FUNC_CNavigation__UnregisterIconEntity;
}

void _declspec(naked) M2Navigation::UnregisterIconPos(int icon, int unk)
{
	_asm jmp COffsets::FUNC_CNavigation__UnregisterIconPos;
}

int _declspec(naked) M2Navigation::RegisterIconEntity(M2Entity *pEntity, int icon, int library, int unk1, int unk2, int unk3)
{
	_asm jmp COffsets::FUNC_CNavigation__RegisterIconEntity;
}

int _declspec(naked) M2Navigation::RegisterIconPos(const Vector2& vecPosition, int lib, int icon, int unk1, int unk2, int unk3)
{
	_asm jmp COffsets::FUNC_CNavigation__RegisterIconPos;
}


CM2Navigation::CM2Navigation(M2Navigation * pNavigation) :
	m_pNavigation(pNavigation)
{
}

CM2Navigation::~CM2Navigation(void)
{
}

int CM2Navigation::RegisterIconPos(const Vector2& vecPosition, int iLibrary, int iIcon)
{
	int iIconId = 0;

	if (m_pNavigation)
	{
		iIconId = m_pNavigation->RegisterIconPos(vecPosition, iLibrary, iIcon, 0, 0, 0);

		void* pIcon = NULL;
		bool bResult = false;
		DWORD C_Navigation__GetIconFromId = 0x4963E0;
		M2Navigation * pNavigation = m_pNavigation;

		_asm
		{
			lea ecx, pIcon;
			push ecx;
			push iIconId;
			mov ecx, pNavigation;
			call C_Navigation__GetIconFromId;
			mov bResult, al;
			add esp, 4h;
		}

		if (bResult) {
			DEBUG_LOG("Icon: 0x%p", pIcon);
		}
	}

	return iIconId;
}

int CM2Navigation::RegisterIconEntity(M2Entity * pEntity, int iLibrary, int iIcon)
{
	int iIconId = 0;

	if (m_pNavigation)
	{
		M2Navigation *pNavigation = m_pNavigation;
		iIconId = m_pNavigation->RegisterIconEntity(pEntity, iLibrary, iIcon, 0, 0, 0);

		DWORD dwFunc = 0x4965C0;
		void * pIcon = NULL;

		_asm
		{
			push pEntity;
			mov ecx, pNavigation;
			call dwFunc;
			mov pIcon, eax;
		}

		DEBUG_LOG("Entity icon: 0x%p", pIcon);
	}

	return iIconId;
}

void CM2Navigation::UnregisterIconPos(int iIconId)
{
	if (m_pNavigation)
	{
		m_pNavigation->UnregisterIconPos(iIconId, 1);
	}
}

void CM2Navigation::UnregisterIconEntity(M2Entity * pEntity)
{
	if (m_pNavigation)
	{
		m_pNavigation->UnregisterIconEntity(pEntity, 1);
	}
}

void CM2Navigation::SetForegroundColor(int iIconId, int iColor)
{
	if (m_pNavigation)
	{
		m_pNavigation->SetForegroundColor(iIconId, iColor);
	}
}

void CM2Navigation::SetBackgroundColor(int iIconId, int iColor)
{
	if (m_pNavigation)
	{
		m_pNavigation->SetBackgroundColor(iIconId, iColor);
	}
}