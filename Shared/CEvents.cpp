/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CEvents.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#ifdef _CLIENT
#include "CClientScriptingManager.h"
#endif

#include "CString.h"

#include "Scripting/CSquirrel.h"
#include "Scripting/CScriptingManager.h"

#include "CEvents.h"

void CSquirrelEventHandler::Call( CSquirrelArguments * pArguments, CSquirrelArgument * pReturn )
{
	// Find the script
#ifndef _CLIENT
	CSquirrel * pScript = CCore::Instance()->GetScriptingManager()->Get( m_pVM );
#else
	CSquirrel * pScript = CCore::Instance()->GetClientScriptingManager()->GetScriptingManager()->Get( m_pVM );
#endif

	// Is the script valid?
	if( pScript )
	{
		// Call the function
		pScript->Call( m_pFunction, pArguments, pReturn );
	}
}