/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CResourceManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <queue>

enum eScriptType
{
	E_SCRIPT_CLIENT = 0,
	E_SCRIPT_SERVER
};

enum eResourceMarkType
{
	E_RES_MARK_START = 0,
	E_RES_MARK_STOP = 1,
	E_RES_MARK_RESTART = 2
};

struct SScript
{
	eScriptType eType;
	String strName;
};

struct SResource
{
	String strName;
	String strPath;
	std::list< SScript > scripts;
	int iScripts;
	int iFiles;
};

struct SResourceMark
{
	String name;
	eResourceMarkType type;
};

class CResourceManager
{

private:

	std::list< SResource >												m_resourceList;
	std::queue<SResourceMark>											m_resourceMarks;

public:

									CResourceManager					( void );
									~CResourceManager					( void );

	void							LoadAll								( std::list< const char* > resources );
	bool							StartResource						( const char * szResource );
	bool							StopResource						( const char * szResource );
	bool							IsResourceRunning					( const char * szResource );
	void							GetRunningResources					( void );
	bool 							MarkForStarting						( const char * szResource );
	bool 							MarkForStopping						( const char * szResource );
	bool 							MarkForRestarting					( const char * szResource );
	void							Pulse								( void );
};
