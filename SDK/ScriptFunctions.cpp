/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : ScriptFunctions.cpp
*
***************************************************************/

#include	"ScriptFunctions.h"

int ScriptFunctions::sq_myfunction( HSQUIRRELVM vm )
{
	sq_pushstring( vm, "Module says: Hi!", -1 );
	return 1;
}