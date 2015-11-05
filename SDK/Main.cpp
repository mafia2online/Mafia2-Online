/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : Main.cpp
*
***************************************************************/

#include "SDK.h"
#include "ScriptFunctions.h"

// This function is called when the module is first loaded
EXPORT bool ModuleInitialise( void )
{
	Log( "Module initialised." );

	// Don't let the server output 'Loaded module <name>.'
	return false;
}

// This function is called when a script is loaded on the server
EXPORT void OnScriptLoad( HSQUIRRELVM vm )
{
	// Register script functions
	RegisterFunction( vm, "getModuleString", ScriptFunctions::sq_myfunction );
}

// This function is called when a script is unloaded from the server
EXPORT void OnScriptUnload( HSQUIRRELVM vm )
{
}

// This function is called every time a server frame is rendered
EXPORT void Pulse( void )
{
}