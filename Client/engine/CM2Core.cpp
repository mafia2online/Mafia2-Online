/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.cpp
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2Core.h"

#include "engine/CM2Entity.h"

#include "COffsets.h"

M2Model * CM2Core::AllocateModel( void )
{
	__asm
	{
		push 2
		mov ecx, this
		call COffsets::FUNC_CCore__AllocateModel
	}
}

CM2Core * CM2Core::GetInstance( void )
{
	return *( CM2Core **)( COffsets::VAR_CCore );
}
