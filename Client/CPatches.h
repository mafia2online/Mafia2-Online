/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPatches.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			 : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

class M2EntityMessage;

// Typedefs
typedef int				( __cdecl * sub_410440 )			( lua_State * a1, signed int a2, const PCHAR a3 );
typedef int				( __cdecl * onGameEvent_t )			( lua_State * a1, void *, const char *, M2EntityMessage * );
typedef int				( __thiscall * ProcessEntities_t )	( void * This, void * pUnk1 );
typedef int				( __cdecl * OnGameProcessStart_t )	( HINSTANCE, int, int, int );

class CPatches
{

private:

	// Hooks
	static	sub_410440				onGameInit;
	static	onGameEvent_t			onGameEvent;
	static	ProcessEntities_t		processEntities;
	static	OnGameProcessStart_t	onGameProcessStart;

	// Game
	static	int						HOOK_CGame__OnGameInit							( lua_State * a1, signed int a2, const PCHAR a3 );
	static	int						HOOK_CGame__OnGameEvent							( lua_State * a1, void * a2, const char * szFunction, M2EntityMessage * pMessage );
	static	int __fastcall			HOOK_CEntMgr__ProcessEntities					( void * This, void * _EDX, void * pUnk1 );
	static	int						HOOK_OnGameProcessStart							( HINSTANCE hInstance, int a2, int a3, int a4 );

	// Human
	static	void					HOOK_CHumanInventory__DoShot					( void );
	static	void					HOOK_CHumanInventory__DoReload					( void );

public:
	
	static	void				Initialise											( void );

};