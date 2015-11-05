/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CEvents.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../Client/StdInc.h"
#else
#include	"../Server/StdInc.h"
#endif

extern	CCore			* pCore;

void CSquirrelEventHandler::Call( CSquirrelArguments * pArguments, CSquirrelArgument * pReturn )
{
	// Find the script
#ifndef _CLIENT
	CSquirrel * pScript = pCore->GetScriptingManager()->Get( m_pVM );
#else
	CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( m_pVM );
#endif

	// Is the script valid?
	if( pScript )
	{
		// Call the function
		pScript->Call( m_pFunction, pArguments, pReturn );
	}
}