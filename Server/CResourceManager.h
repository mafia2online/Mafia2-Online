/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CResourceManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

enum eScriptType
{
	E_SCRIPT_CLIENT = 0,
	E_SCRIPT_SERVER
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

class CResourceManager
{

private:

	std::list< SResource >												m_resourceList;

public:

									CResourceManager					( void );
									~CResourceManager					( void );

	void							LoadAll								( std::list< const char* > resources );
	bool							StartResource						( const char * szResource );
	bool							StopResource						( const char * szResource );
	bool							IsResourceRunning					( const char * szResource );
	void							GetRunningResources					( void );

};