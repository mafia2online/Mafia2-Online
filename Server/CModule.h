/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CModule.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CLibrary.h"
#include "squirrel.h"

typedef void (* SetupSquirrel_t)( void * );
typedef void (* SetupCore_t)( void * );
typedef bool (* InitialiseModule_t)( void );
typedef void (* ScriptLoad_t)( HSQUIRRELVM );
typedef void (* ScriptUnload_t)( HSQUIRRELVM );
typedef void (* Pulse_t)( void );

struct SModuleFunctions
{
	SetupSquirrel_t pfnSetupSquirrel;
	SetupCore_t pfnSetupCore;
	InitialiseModule_t pfnInitialiseModule;
	ScriptLoad_t pfnScriptLoad;
	ScriptUnload_t pfnScriptUnload;
	Pulse_t pfnPulse;
};

class CModule
{

private:

	CLibrary						* m_pLibrary;
	SModuleFunctions				m_moduleFunctions;
	String							m_strName;

public:

	CModule( String strName );
	~CModule( void );

	bool							IsValid( void );
	void							ScriptLoad( HSQUIRRELVM pVM );
	void							ScriptUnload( HSQUIRRELVM pVM );
	void							Pulse( void );

};