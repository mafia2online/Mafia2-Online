/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientScriptingManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore			* pCore;

CClientScriptingManager::CClientScriptingManager( void )
{
	// Create the events manager instance
	m_pEventsManager = new CEvents;

	// Create the commands manager instance
	m_pCommandsManager = new CCommands;

	// Create the scripting manager instnace
	m_pScriptingManager = new CScriptingManager;

	// Create the script gui manager instance
	m_pScriptGUIManager = new CClientScriptGUIManager;
}

CClientScriptingManager::~CClientScriptingManager( void )
{
	// Unload all clientscripts
	UnloadAll();

	// Delete the script gui manager instance
	SAFE_DELETE( m_pScriptGUIManager );

	// Delete the scripting manager instance
	SAFE_DELETE( m_pScriptingManager );

	// Delete the events manager instance
	SAFE_DELETE( m_pEventsManager );
}

void CClientScriptingManager::AddScript( String strName, String strPath )
{
	// Create a new client script instance
	SClientScript * pClientScript = new SClientScript;
	pClientScript->strName = strName;
	pClientScript->strPath = strPath;
	pClientScript->pScript = NULL;

	// Add the clientscript to the client script list
	m_clientScripts.push_back( pClientScript );
}

void CClientScriptingManager::RemoveScript( String strName )
{
	SClientScript * pClientScript = NULL;

	// Loop all clientscripts
	for( std::list< SClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++ )
	{
		//
		pClientScript = (*iter);

		// Is this clientscript the one we're looking for?
		if( pClientScript && pClientScript->strName == strName )
			break;
	}

	// Did we find the script?
	if( pClientScript )
	{
		// Remove the script from the list
		m_clientScripts.remove( pClientScript );

		// Delete the clientscript
		SAFE_DELETE( pClientScript );
	}
}

void CClientScriptingManager::Load( String strName )
{
	// Does the script not exist?
	if( !Exists( strName ) )
		return;

	// Loop all clientscripts
	for( std::list< SClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++ )
	{
		// Is this clientscript the one we're looking for?
		if( *iter && (*iter)->strName == strName )
		{
			// Load the script
			(*iter)->pScript = m_pScriptingManager->Load( (*iter)->strName, (*iter)->strPath );

			return;
		}
	}
}

void CClientScriptingManager::Unload( String strName )
{
	// Does the script not exist?
	if( !Exists( strName ) )
		return;

	//
	SClientScript* pClientScript = NULL;

	// Loop all clientscripts
	for( std::list< SClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++ )
	{
		// Is this clientscript the one we're looking for?
		if( *iter && (*iter)->strName == strName )
			pClientScript = (*iter); break;
	}

	// Did we find the script?
	if( pClientScript )
	{
		// Delete all gui elements linked to this script
		m_pScriptGUIManager->DeleteAll( pClientScript->pScript );

		// Unload the script
		m_pScriptingManager->Unload( strName );

		// Delete the script from the list
		m_clientScripts.remove( pClientScript );

		// Delete the client script instance
		SAFE_DELETE( pClientScript );
	}
}

bool CClientScriptingManager::Exists( String strName )
{
	// Loop all clientscripts
	for( std::list< SClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++ )
	{
		// Is this clientscript the one we're looking for?
		if( *iter && (*iter)->strName == strName )
			return true;
	}

	return false;
}

void CClientScriptingManager::UnloadAll( void )
{
	// Loop all clientscripts
	for( std::list< SClientScript *>::iterator iter = m_clientScripts.begin(); iter != m_clientScripts.end(); iter++ )
	{
		// Delete all gui elements linked to this script
		m_pScriptGUIManager->DeleteAll( (*iter)->pScript );

		// Unload the script
		m_pScriptingManager->Unload( (*iter)->strName );

		// Delete the client script instance
		SAFE_DELETE( *iter );
	}

	// Clear the scripts list
	m_clientScripts.clear();
}