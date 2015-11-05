/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Navigation.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "../StdInc.h"

CM2Navigation::CM2Navigation( M2Navigation * pNavigation )
{
	// Set the navigation
	SetNavigation( pNavigation );

	CLogFile::Printf ( "0x38: 0x%p", *(DWORD **)(pNavigation + 0x38) );
}

CM2Navigation::~CM2Navigation( void )
{
}

int CM2Navigation::RegisterIconPos( Vector2 vecPosition, int iLibrary, int iIcon )
{
	int iIconId = 0;

	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm push 0;
		_asm push 0;
		_asm push 0;
		_asm push iIcon;
		_asm push iLibrary;
		_asm lea ecx, vecPosition;
		_asm push ecx;
		_asm mov ecx, pNavigation;
		_asm call COffsets::FUNC_CNavigation__RegisterIconPos;
		_asm mov iIconId, eax;
	}

	void* pIcon = NULL;
	bool bResult = false;
	DWORD C_Navigation__GetIconFromId = 0x4963E0;
	M2Navigation * pNavigation = m_pNavigation;

	_asm lea ecx, pIcon;
	_asm push ecx;
	_asm push iIconId;
	_asm mov ecx, pNavigation;
	_asm call C_Navigation__GetIconFromId;
	_asm mov bResult, al;
	_asm add esp, 4h;

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

		_asm push 0;
		_asm push 0;
		_asm push 0;
		_asm push iIcon;
		_asm push iLibrary;
		_asm push pEntity;
		_asm mov ecx, pNavigation;
		_asm call COffsets::FUNC_CNavigation__RegisterIconEntity;
		_asm mov iIconId, eax;


		DWORD dwFunc = 0x4965C0; // C_Navigation::GetIconFromEntity
		void * pIcon = NULL;

		_asm push pEntity;
		_asm mov ecx, pNavigation;
		_asm call dwFunc;
		_asm mov pIcon, eax;

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

		_asm push 1;
		_asm push iIconId;
		_asm mov ecx, pNavigation;
		_asm call COffsets::FUNC_CNavigation__UnregisterIconPos;
	}
}

void CM2Navigation::UnregisterIconEntity( M2Entity * pEntity )
{
	// Is the navigation valid?
	if( m_pNavigation )
	{
		M2Navigation * pNavigation = m_pNavigation;

		_asm push 1;
		_asm push pEntity;
		_asm mov ecx, pNavigation;
		_asm call COffsets::FUNC_CNavigation__UnregisterIconEntity;
	}
}