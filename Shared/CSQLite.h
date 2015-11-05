/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSQLite.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Libraries/sqlite/sqlite3.h"
#include	"CString.h"

class CSQLite
{

private:

	sqlite3			* m_pDB;

public:

	CSQLite( void );
	~CSQLite( void );

	bool			Open( String strFileName );
	bool			Close( void );
	bool			IsOpen( void ) { return (m_pDB != NULL); }
	bool			Query( const char * szQuery );
	sqlite3			* GetDatabase( void ) { return m_pDB; }

};