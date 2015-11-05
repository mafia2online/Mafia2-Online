/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CModuleManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<list>
#include	"CModule.h"

class CModuleManager
{

private:

	std::list< CModule* >			m_modulesList;
	int								m_iLoadedModules;
	int								m_iFailedModules;

public:

	CModuleManager( void );
	~CModuleManager( void );

	void							LoadAll( std::list< const char* > modulesList );
	CModule							* LoadModule( const char * szName );
	void							ScriptLoad( HSQUIRRELVM pVM );
	void							ScriptUnload( HSQUIRRELVM pVM );
	void							Pulse( void );

};