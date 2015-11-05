/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CModuleManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CModuleManager::CModuleManager( void )
{
	// Reset
	m_iLoadedModules = 0;
	m_iFailedModules = 0;
}

CModuleManager::~CModuleManager( void )
{
	// Loop all modules
	for( std::list< CModule* >::iterator iter = m_modulesList.begin(); iter != m_modulesList.end(); ++iter )
	{
		// Is the current module active?
		if( *iter )
		{
			// Delete the module
			delete *iter;
		}
	}
}

void CModuleManager::LoadAll( std::list< const char* > moduleList )
{
	// Is there not any modules to load?
	if( moduleList.size() == 0 )
		return;

	// Loop the modules list
	for( std::list< const char* >::iterator iter = moduleList.begin(); iter != moduleList.end(); iter++ )
	{
		// Try Load the module
		if( LoadModule( *iter ) )
			m_iLoadedModules++;
		else
			m_iFailedModules++;
	}

	CLogFile::Printf( "Successfully loaded %d modules. (%d failed)\n", m_iLoadedModules, m_iFailedModules );
}

CModule * CModuleManager::LoadModule( const char * szName )
{
	// Create the module instance
	CModule * pModule = new CModule( szName );

	// Did the module fail?
	if( !pModule->IsValid() )
	{
		SAFE_DELETE( pModule );
		return NULL;
	}

	// Add the module into the module list
	m_modulesList.push_back( pModule );

	return pModule;
}

void CModuleManager::ScriptLoad( HSQUIRRELVM pVM )
{
	// Loop all modules
	for( std::list< CModule* >::iterator iter = m_modulesList.begin(); iter != m_modulesList.end(); ++iter )
	{
		// Is the current module active?
		if( *iter )
		{
			// Call the script load for this module
			(*iter)->ScriptLoad( pVM );
		}
	}
}

void CModuleManager::ScriptUnload( HSQUIRRELVM pVM )
{
	// Loop all modules
	for( std::list< CModule* >::iterator iter = m_modulesList.begin(); iter != m_modulesList.end(); ++iter )
	{
		// Is the current module active?
		if( *iter )
		{
			// Call the script unload for this module
			(*iter)->ScriptUnload( pVM );
		}
	}
}

void CModuleManager::Pulse( void )
{
	// Loop all modules
	for( std::list< CModule* >::iterator iter = m_modulesList.begin(); iter != m_modulesList.end(); ++iter )
	{
		// Is the current module active?
		if( *iter )
		{
			// Pulse for this module
			(*iter)->Pulse( );
		}
	}
}