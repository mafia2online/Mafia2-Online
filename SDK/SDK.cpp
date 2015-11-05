/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : SDK.cpp
*
***************************************************************/

#include "SDK.h"

SquirrelInterface_t Squirrel;
CCoreInterface * pCore;

EXPORT void SetupSquirrel( SquirrelInterface_t * SquirrelInterface )
{
	Squirrel = *SquirrelInterface;
}

EXPORT void SetupCore( CCoreInterface * pCoreInterface )
{
	pCore = pCoreInterface;
}

void RegisterFunction( HSQUIRRELVM vm, const char * szFunctionName, SQFUNCTION pfnFunction )
{
	// Push the root table onto the stack
	sq_pushroottable( vm );

	// Push the function name
	sq_pushstring( vm, szFunctionName, -1 );

	// Create a new clossure
	sq_newclosure( vm, pfnFunction, 0 );

	// Create a new slot
	sq_createslot( vm, -3 );

	// Pop the stack
	sq_pop( vm, 1 );
}