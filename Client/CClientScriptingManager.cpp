/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientScriptingManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CString.h"

#include "CEvents.h"
#include "CCommands.h"
#include "Scripting/CScriptingManager.h"
#include "CClientScriptGUIManager.h"

#include "CClientScriptingManager.h"

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
	SClientScript * pClientScript = Find(strName);

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
	SClientScript* pClientScript = Find (strName);
	if (!pClientScript)
		return;

	pClientScript->pScript = m_pScriptingManager->Load( pClientScript->strName, pClientScript->strPath );
}

void CClientScriptingManager::Unload( String strName )
{
	SClientScript* pClientScript = Find (strName);
	if( !pClientScript )
		return;

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

bool CClientScriptingManager::Exists( String strName ) const
{
	return Find(strName) != nullptr;
}

SClientScript* CClientScriptingManager::Find ( String strName ) const
{
	for ( SClientScript* script : m_clientScripts )
	{
		if (script->strName == strName)
			return script;
	}
	return nullptr;
}

void CClientScriptingManager::UnloadAll( void )
{
	// Loop all clientscripts
	for ( SClientScript* script : m_clientScripts )
	{
		// Delete all gui elements linked to this script
		m_pScriptGUIManager->DeleteAll( script->pScript );

		// Unload the script
		m_pScriptingManager->Unload( script->strName );

		// Delete the client script instance
		SAFE_DELETE( script );
	}

	// Clear the scripts list
	m_clientScripts.clear();
}