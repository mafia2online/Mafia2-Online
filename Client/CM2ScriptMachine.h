/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2ScriptMachine.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class M2ScriptMachine
{
public:
	PAD(M2ScriptMachine, pad0, 0x50);				// 0000 - 0050
	lua_State * m_L;								// 0050 - 0054
};

class M2ScriptMachinePool
{
public:
	M2ScriptMachine * m_pScriptMachine[0xC];		// 0000 - 0004
};

class M2ScriptHandler
{
public:
	PAD(M2ScriptHandler, pad0, 0x4);				// 0000 - 0004
	M2ScriptMachinePool * m_pScriptMachinePool;		// 0004 - 0008
};