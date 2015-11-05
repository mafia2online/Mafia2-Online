/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CScriptingManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../../Client/StdInc.h"
#else
#include	"../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

CScriptingManager::CScriptingManager( void )
{
	// Register event natives
	CEventNatives::Register( this );

	// Register system natives
	CSystemNatives::Register( this );

	// Register the util natives
	CUtilNatives::Register( this );

	// Register the XML natives
	CXMLNatives::Register( this );

#ifdef _CLIENT
	// Register client natives
	CClientNatives::Register( this );

	// Register GUI natives
	CGUINatives::Register( this );

	// Register the graphics natives
	CGraphicsNatives::Register( this );

	// Register the game natives
	CGameNatives::Register( this );

	// Register the camera natives
	CCameraNatives::Register( this );
#else
	// Register server natives
	CServerNatives::Register( this );
	
	// Register ban natives
	CBanNatives::Register( this );

	// Register sqlite natives
	CSQLiteNatives::Register( this );

	// Register vehicle natives
	CVehicleNatives::Register( this );

	// Register http natives
	CHttpNatives::Register( this );
#endif

	// Register player natives
	CPlayerNatives::Register( this );

	// Register timer natives
	CTimerNatives::Register( this );

	// Register area natives
	CAreaNatives::Register( this );

	// Register blip natives
	CBlipNatives::Register( this );

#ifdef _CLIENT
	// Register ped natives
	CPedNatives::Register( this );
#endif

	// Register shared player natives
	CSharedPlayerNatives::Register ( this );

	// Register shared vehicle natives
	CSharedVehicleNatives::Register( this );

	// Register hash natives
	CHashNatives::Register( this );

	// Register datastore natives
	//CDatastoreNatives::Register ( this );

	// Register the default constants
	RegisterDefaultConstants();
}

CSquirrel * CScriptingManager::Load( String strName, String strPath )
{
	// Create the script instance
	CSquirrel * pScript = new CSquirrel( );

	// Failed to load the script?
	if( !pScript->Load( strName, strPath ) )	
	{
		// Delete the script
		delete pScript;

		return NULL;
	}

	// Push the script into the list
	m_scripts.push_back( pScript );
	
	// Register the functions
	if( m_functions.size() > 0 )
	{
		std::list< ScriptingFunction *>::iterator iter;
		for( iter = m_functions.begin(); iter != m_functions.end(); iter++ )
		{
			pScript->RegisterFunction( (*iter)->strName, (*iter)->pfnFunction, (*iter)->iParameterCount, (*iter)->strTemplate );
		}
	}

	// Register the classes
	if( m_classes.size() > 0 )
	{
		std::list< SquirrelClassDecl *>::iterator iter;
		for( iter = m_classes.begin(); iter != m_classes.end(); iter++ )
		{
			pScript->RegisterClass( *iter );
		}
	}

	// Register the constants
	if( m_constants.size() > 0 )
	{
		std::list< ScriptingConstant *>::iterator iter;
		for( iter = m_constants.begin(); iter != m_constants.end(); iter++ )
		{
			pScript->RegisterConstant( (*iter)->strName, (*iter)->value );
		}
	}

	// Failed to execute the script?
	if( !pScript->Execute() )
	{
		// Delete the script
		delete pScript;

		// Remove the script
		m_scripts.remove( pScript );

		return NULL;
	}

#ifndef _CLIENT
	// Call the module event
	pCore->GetModuleManager()->ScriptLoad( pScript->GetVM() );

	// Call onScriptInit event
	pCore->GetEvents()->Call( "onScriptInit", pScript );
#else
	// Call onClientScriptInit event
	pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientScriptInit", pScript );
#endif

	return pScript;
}

void CScriptingManager::LoadAll( std::list< String > scripts )
{
#ifndef _CLIENT
	int iLoaded = 0, iFailed = 0;

	// Loop through all the scripts
	for( std::list< String >::iterator iter = scripts.begin(); iter != scripts.end(); iter++ )
	{
		// Get the script path
		String strPath( "scripts/%s", (*iter).Get() );

		// Try and load the script
		if( Load( (*iter).Get(), strPath ) )
		{
			// Increase the total loaded resources
			iLoaded++;
		}
		else
		{
			// Increase the total failed resources
			iFailed++;
		}
	}

	pCore->UpdateResourceTotals( iLoaded, iFailed );
#endif
}

bool CScriptingManager::Unload( String strName )
{
	CSquirrel * pScript = NULL;

	// Find the script
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			if( (*iter)->GetName() == strName )
			{
				pScript = (*iter);
				break;
			}
		}
	}

	// Is the script valid?
	if( pScript )
	{
#ifndef _CLIENT
		// Call the module event
		pCore->GetModuleManager()->ScriptUnload( pScript->GetVM() );

		// Call onScriptExit event
		pCore->GetEvents()->Call( "onScriptExit", pScript );

		// Remove the script events
		pCore->GetEvents()->HandleScriptUnload( pScript->GetVM() );
#else
		// Call onClientScriptExit event
		pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientScriptExit", pScript );

		// Remove the script events
		pCore->GetClientScriptingManager()->GetEvents()->HandleScriptUnload( pScript->GetVM() );
#endif
		
		// Handle the script unload with the timer manager
		pCore->GetTimerManager()->HandleScriptUnload( pScript );

		// Unload the script
		pScript->Unload();

		// Remove the script
		m_scripts.remove( pScript );

		// Delete the script
		SAFE_DELETE( pScript );

		return true;
	}

	return false;
}

void CScriptingManager::UnloadAll( void )
{
	// Are any scripts loaded?
	if( m_scripts.size() > 0 )
	{
		// Loop through all scripts
		for( std::list< CSquirrel* >::iterator iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			// Unload the current script
			(*iter)->Unload();
		}
	}
}

void CScriptingManager::NewClass( SquirrelClassDecl * pClassDecl )
{
	// Push the class into the list
	m_classes.push_back( pClassDecl );

	// Is there any scripts?
	if( m_scripts.size() > 0 )
	{
		std::list< CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
			(*iter)->RegisterClass( pClassDecl );
	}
}

void CScriptingManager::RegisterFunction( String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate )
{
	ScriptingFunction * pFunction = new ScriptingFunction;
	pFunction->strName = strFunctionName;
	pFunction->pfnFunction = pfnFunction;
	pFunction->iParameterCount = iParameterCount;
	pFunction->strTemplate = strFunctionTemplate;

	m_functions.push_back( pFunction );

	// Register the function for every script
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			(*iter)->RegisterFunction( strFunctionName, pfnFunction, iParameterCount, strFunctionTemplate );
		}
	}
}

void CScriptingManager::RegisterConstant( String strConstantName, CSquirrelArgument value )
{
	ScriptingConstant * pConstant = new ScriptingConstant;
	pConstant->strName = strConstantName;
	pConstant->value = value;

	m_constants.push_back( pConstant );

	// Register the constant for every script
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			(*iter)->RegisterConstant( strConstantName, value );
		}
	}
}

void CScriptingManager::RegisterDefaultConstants( void )
{
	RegisterConstant( "INVALID_ENTITY_ID", INVALID_ENTITY_ID );
	RegisterConstant( "ENTITY_ID_CONSOLE", ENTITY_ID_CONSOLE );
	RegisterConstant( "MAX_PLAYERS", MAX_PLAYERS );
	RegisterConstant( "UNBAN_NEVER", UNBAN_NEVER );
	RegisterConstant( "REJECT_REASON_VERSION", REJECT_REASON_VERSION );
	RegisterConstant( "REJECT_REASON_NICKNAME", REJECT_REASON_NICKNAME );
	RegisterConstant( "REJECT_REASON_BANNED", REJECT_REASON_BANNED );
	RegisterConstant( "DISCONNECT_TIMEOUT", DISCONNECT_TIMEOUT );
	RegisterConstant( "DISCONNECT_QUIT", DISCONNECT_QUIT );
	RegisterConstant( "DISCONNECT_KICKED", DISCONNECT_KICKED );
	RegisterConstant( "DISCONNECT_BANNED", DISCONNECT_BANNED );

#ifndef _CLIENT
	RegisterConstant( "HTTP_TYPE_GET", HTTP_TYPE_GET );
	RegisterConstant( "HTTP_TYPE_POST", HTTP_TYPE_POST );
#endif
}

void CScriptingManager::Call( SQObjectPtr pFunction, CSquirrelArguments * pArguments, SQVM * pVM )
{
	// Loop all scripts
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			if( (*iter)->GetVM() == pVM )
			{
				(*iter)->Call( pFunction, pArguments ); // todo: return?
			}
		}
	}
}

CSquirrel * CScriptingManager::Get( String strName )
{
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			if( (*iter)->GetName() == strName )
				return (*iter);
		}
	}

	return NULL;
}

CSquirrel * CScriptingManager::Get( SQVM * pVM )
{
	if( m_scripts.size() > 0 )
	{
		std::list<CSquirrel *>::iterator iter;

		for( iter = m_scripts.begin(); iter != m_scripts.end(); iter++ )
		{
			if( (*iter)->GetVM() == pVM )
				return (*iter);
		}
	}

	return NULL;
}