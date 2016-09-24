/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CLua.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "lua.h"
#include "CString.h"

// Typedef stuff
typedef int				( __cdecl * lua_loadbuffer_t )		( lua_State * L, char * szBuffer, size_t sSize, char * name );
typedef int				( __cdecl * lua_pcall_t )			( lua_State * L, int iArgs, int iResults, int errFunc );
typedef void			( __cdecl * lua_pushcclosure_t )	( lua_State * L, lua_CFunction gFunction, int i );
typedef int				( __cdecl * lua_gettop_t )			( lua_State * L );
typedef const char *	( __cdecl * lua_tolstring_t )		( lua_State * L, int idx, size_t * len);
typedef lua_Integer		( __cdecl * lua_tointeger_t )		( lua_State * L, int idx );
typedef lua_State *		( __cdecl * lua_newthread_t )		( lua_State * L );
typedef void			( __cdecl * lua_setfield_t )		( lua_State * L, int idx, const char * k );
typedef void			( __cdecl * lua_settop_t )			( lua_State * L, int idx );

typedef void(__cdecl * lua_getfield_t)		(lua_State * L, int idx, const char * k);
typedef int(__cdecl * lua_type_t)			(lua_State * L, int idx);
typedef const char *	(__cdecl * lua_typename_t)		(lua_State * L, int t);
typedef void(__cdecl * lua_pushnil_t)			(lua_State * L);
typedef int(__cdecl * lua_next_t)			(lua_State * L, int idx);
typedef void(__cdecl * lua_pushvalue_t)		(lua_State * L, int idx);
typedef bool(__cdecl * lua_isuserdata_t)		(lua_State * L, int idx);
typedef void *			(__cdecl * lua_touserdata_t)		(lua_State * L, int idx);

class CLua
{

private:

	static unsigned int				m_uiTotalCalls;

	static lua_loadbuffer_t			plua_loadbuffer;
	static lua_pcall_t				plua_pcall;
	static lua_pushcclosure_t		plua_pushcclosure;
	static lua_gettop_t				plua_gettop;
	static lua_tolstring_t			plua_tolstring;
	static lua_tointeger_t			plua_tointeger;
	static lua_newthread_t			plua_newthread;
	static lua_setfield_t			plua_setfield;
	static lua_settop_t				plua_settop;

	static lua_getfield_t			plua_getfield;
	static lua_type_t				plua_type;
	static lua_typename_t			plua_typename;
	static lua_pushnil_t			plua_pushnil;
	static lua_next_t				plua_next;
	static lua_pushvalue_t			plua_pushvalue;
	static lua_isuserdata_t			plua_isuserdata;
	static lua_touserdata_t			plua_touserdata;

	static void						HOOK_CLua__loadbuffer( void );

public:

	static void						Initialise( void );
	static bool						Execute( String strLua );
	static bool						Executef( const char * szFormat, ... );

};