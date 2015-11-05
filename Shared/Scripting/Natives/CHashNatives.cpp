/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CHashNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CHashNatives.h"
#include	"../../md5/md5.h"
#include	"../../CRC.h"

void CHashNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "md5", md5_string, 1, "s" );
	pScriptingManager->RegisterFunction( "md5_file", md5_file, 1, "s" );
	pScriptingManager->RegisterFunction( "crc32", crc32, 1, "s" );
}

// md5_string ( const char * szString );
SQInteger CHashNatives::md5_string ( SQVM * pVM )
{
	const SQChar * szString;
	sq_getstring ( pVM, -1, &szString );

	String strHash = md5 ( szString );
	sq_pushstring ( pVM, strHash.Get(), strHash.GetLength() );
	return 1;
}

// md5_file ( const char * szFile );
SQInteger CHashNatives::md5_file ( SQVM * pVM )
{
	const SQChar * szFile;
	sq_getstring ( pVM, -1, &szFile );

	String strHash = md5 ( szFile, true );
	sq_pushstring ( pVM, strHash.Get(), strHash.GetLength() );
	return 1;
}

// crc32 ( const char * szFile );
SQInteger CHashNatives::crc32 ( SQVM * pVM )
{
	const SQChar * szFile;
	sq_getstring ( pVM, -1, &szFile );

	CFileChecksum fileChecksum;
	fileChecksum.Calculate ( szFile );
	sq_pushinteger ( pVM, fileChecksum.GetChecksum () );
	return 1;
}