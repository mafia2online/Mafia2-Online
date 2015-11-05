/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLua.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore							* pCore;
char	* szExecuteName					= "12d42b0b65861af09561f91dcecf582e"; // md5("~/Mafia2Multiplayer#");

unsigned int							CLua::m_uiTotalCalls = 0;
lua_loadbuffer_t						CLua::plua_loadbuffer = 0;
lua_pcall_t								CLua::plua_pcall = 0;
lua_pushcclosure_t						CLua::plua_pushcclosure = 0;
lua_gettop_t							CLua::plua_gettop = 0;
lua_tolstring_t							CLua::plua_tolstring = 0;
lua_tointeger_t							CLua::plua_tointeger = 0;
lua_newthread_t							CLua::plua_newthread = 0;
lua_setfield_t							CLua::plua_setfield = 0;
lua_settop_t							CLua::plua_settop = 0;

DWORD	dwLuaState						= 0;
char	* szLuaCall						= "";
char	* szCallName					= "";

void _declspec( naked ) CLua::HOOK_CLua__loadbuffer( void )
{
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp+8];
	_asm mov dwLuaState, eax;
	_asm mov eax, [ebp+12];
	_asm mov szLuaCall, eax;
	_asm mov eax, [ebp+20];
	_asm mov szCallName, eax;
	_asm pop ebp;
	_asm pushad;

	// todo: lua anti-cheat

	_asm popad;
	_asm push ebp;
	_asm mov ebp, esp;
	_asm sub esp, 8;
	_asm mov edx, COffsets::FUNC_CLua__loadbuffer;
	_asm add edx, 6;
	_asm jmp edx;
}

void CLua::Initialise( void )
{
	// Load the pointers
	plua_pcall				= (lua_pcall_t)COffsets::FUNC_CLua__pcall;
	plua_pushcclosure		= (lua_pushcclosure_t)COffsets::FUNC_CLua__pushcclosure;
	plua_gettop				= (lua_gettop_t)COffsets::FUNC_CLua__gettop;
	plua_tolstring			= (lua_tolstring_t)COffsets::FUNC_CLua__tolstring;
	plua_tointeger			= (lua_tointeger_t)COffsets::FUNC_CLua__tointeger;
	plua_newthread			= (lua_newthread_t)COffsets::FUNC_CLua__newthread;
	plua_setfield			= (lua_setfield_t)COffsets::FUNC_CLua__setfield;
	plua_settop				= (lua_settop_t)COffsets::FUNC_CLua__settop;

	// Hook LoadBuffer
	plua_loadbuffer			= (lua_loadbuffer_t)CPatcher::InstallJmpPatch( COffsets::FUNC_CLua__loadbuffer, (DWORD)HOOK_CLua__loadbuffer );
}

bool CLua::Execute( String strLua )
{
	// Create an int for the result
	int iResult = 0;

	// Get the string length
	int iLength = strLua.GetLength();
	
	// Get the lua string
	char * szLua = (char *)strLua.Get();

	// Get the lua state
	DWORD luaState = (DWORD)IE::GetState();

	// Load the lua code into the buffer
	_asm push szExecuteName;
	_asm push iLength;
	_asm push szLua;
	_asm push luaState;
	_asm call COffsets::FUNC_CLua__loadbuffer;
	_asm add esp, 10h;

	// Call the lua code
	_asm push 0;
	_asm push LUA_MULTRET;
	_asm push 0;
	_asm push luaState;
	_asm call COffsets::FUNC_CLua__pcall;
	_asm mov iResult, eax;
	_asm add esp, 10h;

	// Increase the total lua calls
	m_uiTotalCalls++;

	// Should we reset the buffer?
	if( m_uiTotalCalls >= 10 )
	{
		// Reset the stack
		plua_settop( IE::GetState(), 0 );

		// Reset the total lua calls
		m_uiTotalCalls = 0;
	}

	// Did the lua fail to execute?
	if( iResult > 0 )
		return false;

	return true;
}

bool CLua::Executef( const char * szFormat, ... )
{
	char szString[2048];
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsprintf_s(szString, szFormat, vaArgs);
	va_end(vaArgs);

	return Execute( szString );
}