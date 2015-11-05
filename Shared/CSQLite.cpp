/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSQLite.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSQLite.h"

CSQLite::CSQLite( void )
{
	// Invalidate the sqlite database
	m_pDB = NULL;
}

CSQLite::~CSQLite( void )
{
	// Is the sqlite database open?
	if( IsOpen() )
	{
		// Close the sqlite database
		Close( );
	}
}

bool CSQLite::Open( String strFileName )
{
	// Is the sqlite database open?
	if( IsOpen() )
		return false;

	// Try open the sqlite database
	return (sqlite3_open( strFileName.Get(), &m_pDB ) == SQLITE_OK);
}

bool CSQLite::Close( void )
{
	// Is the sqlite database not open?
	if( !IsOpen() )
		return false;

	// Try close the sqlite database
	return (sqlite3_close( m_pDB ) == SQLITE_OK);
}

bool CSQLite::Query( const char * szQuery )
{
	// Is the sqlite database not open or the query is invalid?
	if( !IsOpen() || !szQuery )
		return false;

	return true;
}