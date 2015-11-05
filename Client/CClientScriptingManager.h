/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CClientScriptingManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

struct SClientScript
{
	String strName;
	String strPath;
	CSquirrel * pScript;
};

class CClientScriptingManager
{

private:

	CScriptingManager							* m_pScriptingManager;
	CEvents										* m_pEventsManager;
	CCommands									* m_pCommandsManager;
	CClientScriptGUIManager						* m_pScriptGUIManager;
	std::list< SClientScript *>					m_clientScripts;

public:

	CClientScriptingManager( void );
	~CClientScriptingManager( void );

	void										AddScript( String strName, String strPath );
	void										RemoveScript( String strName );
	void										Load( String strName );
	void										Unload( String strName );
	bool										Exists( String strName );

	void										UnloadAll( void );

	CScriptingManager							* GetScriptingManager( void ) { return m_pScriptingManager; }
	CEvents										* GetEvents( void ) { return m_pEventsManager; }
	CCommands									* GetCommands( void ) { return m_pCommandsManager; }
	CClientScriptGUIManager						* GetScriptGUIManager( void ) { return m_pScriptGUIManager; }

};