/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSQLiteNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSQLiteNatives.h"
#include	"../../CSQLite.h"
#include	"../../SharedUtility.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

_MEMBER_FUNCTION_IMPL(sqlite, constructor);
_MEMBER_FUNCTION_IMPL(sqlite, query);
_MEMBER_FUNCTION_IMPL(sqlite, last_insert_id);
_MEMBER_FUNCTION_IMPL(sqlite, num_rows);
_MEMBER_FUNCTION_IMPL(sqlite, close);

_BEGIN_CLASS(sqlite)
_MEMBER_FUNCTION(sqlite, constructor, 1, "s")
_MEMBER_FUNCTION(sqlite, query, 1, "s")
_MEMBER_FUNCTION(sqlite, last_insert_id, 0, NULL)
_MEMBER_FUNCTION(sqlite, num_rows, 1, "t")
_MEMBER_FUNCTION(sqlite, close, 0, NULL)
_END_CLASS(sqlite)

void CSQLiteNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->NewClass( &_CLASS_DECL(sqlite) );
}

_MEMBER_FUNCTION_RELEASE_HOOK(sqlite)
{
	// Get the sqlite database instance
	CSQLite * pSQLite = (CSQLite *)pInst;

	// Is the sqlite instance valid?
	if( pSQLite )
	{
		// Close the sqlite database
		pSQLite->Close( );

		// Delete the sqlite database
		SAFE_DELETE( pSQLite );
	}
	return 1;
}

// database( file );
_MEMBER_FUNCTION_IMPL(sqlite, constructor)
{
	// Get the filename
	const char * szFileName;
	sq_getstring( pVM, -1, &szFileName );

	// Is the filename valid?
	if( szFileName )
	{
		// Generate the path string
		String strPath;
#ifdef _CLIENT
		strPath = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM )->GetPath();
		strPath = SharedUtility::GetFileNameForScriptFile( szFileName, strPath.Get(), pCore->GetHost().Get(), pCore->GetPort() );
#else
		strPath = pCore->GetScriptingManager()->Get( pVM )->GetPath();
		strPath = SharedUtility::GetFileNameForScriptFile( szFileName, strPath.Get() );
#endif

		// Construct the sqlite database instance
		CSQLite * pSQLite = new CSQLite;

		// Open the sqlite database
		pSQLite->Open( strPath );

		// Did it fail to set the sqlite instance?
		if( SQ_FAILED( sq_setinstance( pVM, pSQLite ) ) )
		{
			// Delete the database instance
			SAFE_DELETE( pSQLite );

			sq_pushbool( pVM, false );
			return 1;
		}

		// Set the release hook
		//_SET_RELEASE_HOOK(sqlite);

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// database.query( sql );
_MEMBER_FUNCTION_IMPL(sqlite, query)
{
	// Get the query string
	char * query;
	sq_getstring( pVM, -1, (const char **)&query );

	// Is the query string valid?
	if( query )
	{
		// Get the sqlite database instance
		CSQLite * pSQLite = sq_getinstance< CSQLite *>( pVM );

		// Is the sqlite database instance invalid?
		if( !pSQLite )
		{
			sq_pushbool( pVM, false );
			return 1;
		}

		// Get the sqlite3 database pointer
		sqlite3 * database = pSQLite->GetDatabase( );
		sqlite3_stmt * stmt;

		// Prepare sqlite3
		if( sqlite3_prepare( database, query, strlen(query) + 1, &stmt, 0 ) != SQLITE_OK )
		{
			// TODO: Maybe call events?
			// onSQLiteDatabaseQueryFail

			sq_pushbool( pVM, false );
			return 1;
		}

		// Create a new table
		sq_newtable( pVM );
		SQInteger rowCount = 0;
		SQInteger colCount = sqlite3_column_count( stmt );

		//
		while( sqlite3_step( stmt ) == SQLITE_ROW )
		{
			// Increase the row count
			rowCount++;

			//
			sq_pushinteger( pVM, rowCount );
			sq_newtable( pVM );

			for( SQInteger i = 0; i < colCount; i++ )
			{
				SQInteger columnType = sqlite3_column_type( stmt, i );
				const char * columnName = sqlite3_column_name( stmt, i );

				sq_pushstring( pVM, columnName, strlen(columnName) );

				//
				switch( columnType )
				{
				case SQLITE_NULL:
					break;
				case SQLITE_INTEGER:
					sq_pushinteger( pVM, sqlite3_column_int(stmt, i) );
					break;
				case SQLITE_FLOAT:
					sq_pushfloat( pVM, (float)sqlite3_column_double(stmt, i) );
					break;
				case SQLITE_TEXT:
					sq_pushstring( pVM, (const char *)sqlite3_column_text( stmt, i ), -1 );
					break;
				case SQLITE_BLOB:
					break;
				default:
					break;
				}

				// Create a new slot
				sq_createslot( pVM, -3 );
			}

			// Create a new slot
			sq_createslot( pVM, -3 );
		}

		// Finialise the sqlite3 database
		sqlite3_finalize( stmt );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// database.last_insert_id( );
_MEMBER_FUNCTION_IMPL(sqlite, last_insert_id)
{
	// Get the sqlite database instance
	CSQLite * pSQLite = sq_getinstance< CSQLite *>( pVM );

	sq_pushinteger( pVM, (int)sqlite3_last_insert_rowid( pSQLite->GetDatabase() ) );
	return 1;
}

// database.num_rows( result );
_MEMBER_FUNCTION_IMPL(sqlite, num_rows)
{
	return 1;
}

// database.close();
_MEMBER_FUNCTION_IMPL(sqlite, close)
{
	// Get the sqlite database instance
	CSQLite * pSQLite = sq_getinstance< CSQLite *>( pVM );

	// Is the sqlite database instance invalid?
	if( !pSQLite )
	{
		sq_pushbool( pVM, false );
		return 1;
	}

	sq_pushbool( pVM, pSQLite->Close() );
	return 1;
}