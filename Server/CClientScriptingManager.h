/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CClientScriptingManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

struct SClientScript
{
	bool bIsScript;
	String strName;
	String strPath;
	CFileChecksum fileChecksum;
};

class CClientScriptingManager : public std::list< SClientScript >
{

private:

	bool			m_bIsScriptManager;

public:

	CClientScriptingManager( bool bScriptManager );
	~CClientScriptingManager( void );

	bool			Start( String strName, String strDirectory );
	bool			Stop( String strName );
	bool			Restart( String strName );
	bool			Exists( String strName );
	void			HandlePlayerJoin( EntityId playerId );

};
