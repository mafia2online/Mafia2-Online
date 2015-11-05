/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game Engine
* File       : CM2Core.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

class CM2Core
{
public:
	M2Model			* AllocateModel	( void );

	static CM2Core	* GetInstance	( void );
};