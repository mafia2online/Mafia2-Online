/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CModule.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore			* pCore;

CModule::CModule( String strName )
{
	// Store the module name
	m_strName = strName;

	// Reset library
	m_pLibrary = NULL;

	// Remove any illegal characters from the module name
	SharedUtility::RemoveIllegalCharacters( strName );

	// Get the module path string
	String strModulePath( "modules\\%s%s", strName.Get(), LIB_EXTENSION );
	
	// Does the module not exists?
	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (File doesn't exist)", strName.Get(), LIB_EXTENSION );
		return;
	}

	// Create the libray instance
	m_pLibrary = new CLibrary();

	// Is the library instance invalid?
	if( !m_pLibrary )
		return;

	// Did the module fail to load?
	if( !m_pLibrary->Load( strModulePath.Get() ) )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (Failed to load)", strName.Get(), LIB_EXTENSION );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );

		return;
	}

	// Get the module function pointers
	m_moduleFunctions.pfnSetupSquirrel = (SetupSquirrel_t)m_pLibrary->GetProcedureAddress( "SetupSquirrel" );
	m_moduleFunctions.pfnSetupCore = (SetupCore_t)m_pLibrary->GetProcedureAddress( "SetupCore" );
	m_moduleFunctions.pfnInitialiseModule = (InitialiseModule_t)m_pLibrary->GetProcedureAddress( "ModuleInitialise" );
	m_moduleFunctions.pfnScriptLoad = (ScriptLoad_t)m_pLibrary->GetProcedureAddress( "OnScriptLoad" );
	m_moduleFunctions.pfnScriptUnload = (ScriptUnload_t)m_pLibrary->GetProcedureAddress( "OnScriptUnload" );
	m_moduleFunctions.pfnPulse = (Pulse_t)m_pLibrary->GetProcedureAddress( "Pulse" );

	// Are the pointers invalid?
	if( !IsValid() )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (Unable to get module procedures)", strName.Get(), LIB_EXTENSION );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );

		return;
	}

	// Setup the functions
	m_moduleFunctions.pfnSetupSquirrel( SquirrelInterfaceContainer );

	// Setup the core interface with the module
	m_moduleFunctions.pfnSetupCore( (void *)pCore );

	// Initialise the module
	if( m_moduleFunctions.pfnInitialiseModule() )
		CLogFile::Printf( "Loaded module '%s%s'.", strName.Get(), LIB_EXTENSION );
}

CModule::~CModule( void )
{
	// Is the library instance valid?
	if( m_pLibrary )
	{
		// Unload the library
		m_pLibrary->Unload( );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );
	}
}

bool CModule::IsValid( void )
{
	// Is the library instance invalid?
	if( !m_pLibrary )
		return false;

	// Are the library function pointers invalid?
	if( !m_moduleFunctions.pfnSetupSquirrel || !m_moduleFunctions.pfnInitialiseModule )
		return false;

	return true;
}

void CModule::ScriptLoad( HSQUIRRELVM pVM )
{
	// Is the library instance invalid or script load pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnScriptLoad )
		return;

	m_moduleFunctions.pfnScriptLoad( pVM );
}

void CModule::ScriptUnload( HSQUIRRELVM pVM )
{
	// Is the library instance invalid or script unload pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnScriptUnload )
		return;

	m_moduleFunctions.pfnScriptUnload( pVM );
}

void CModule::Pulse( void )
{
	// Is the library instance invalid or pulse pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnPulse )
		return;

	m_moduleFunctions.pfnPulse( );
}
