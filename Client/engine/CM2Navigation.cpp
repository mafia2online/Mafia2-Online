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

CM2Navigation::CM2Navigation( M2Navigation * pNavigation )
{
	// Set the navigation
	SetNavigation( pNavigation );

	CLogFile::Printf ( "0x38: 0x%p", *(DWORD **)(pNavigation + 0x38) );
}

CM2Navigation::~CM2Navigation( void )
{
}

int CM2Navigation::RegisterIconPos( const Vector2& vecPosition, int iLibrary, int iIcon )
{
	int iIconId = 0;

	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm
		{
			push 0;
			push 0;
			push 0;
			push iIcon;
			push iLibrary;
			lea ecx, vecPosition;
			push ecx;
			mov ecx, pNavigation;
			call COffsets::FUNC_CNavigation__RegisterIconPos;
			mov iIconId, eax;
		}
	}

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

	if ( bResult )
		CLogFile::Printf ( "Icon: 0x%p", pIcon );

	return iIconId;
}

int CM2Navigation::RegisterIconEntity( M2Entity * pEntity, int iLibrary, int iIcon )
{
	int iIconId = 0;

	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm
		{
			push 0;
			push 0;
			push 0;
			push iIcon;
			push iLibrary;
			push pEntity;
			mov ecx, pNavigation;
			call COffsets::FUNC_CNavigation__RegisterIconEntity;
			mov iIconId, eax;
		}


		DWORD dwFunc = 0x4965C0; // C_Navigation::GetIconFromEntity
		void * pIcon = NULL;

		_asm
		{
			push pEntity;
			mov ecx, pNavigation;
			call dwFunc;
			mov pIcon, eax;
		}

		CLogFile::Printf ( "Entity icon: 0x%p", pIcon );
	}

	return iIconId;
}

void CM2Navigation::UnregisterIconPos( int iIconId )
{
	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm
		{
			push 1;
			push iIconId;
			mov ecx, pNavigation;
			call COffsets::FUNC_CNavigation__UnregisterIconPos;
		}
	}
}

void CM2Navigation::UnregisterIconEntity( M2Entity * pEntity )
{
	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm
		{
			push 1;
			push pEntity;
			mov ecx, pNavigation;
			call COffsets::FUNC_CNavigation__UnregisterIconEntity;
		}
	}
}
