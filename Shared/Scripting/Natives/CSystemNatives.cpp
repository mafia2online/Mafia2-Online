/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSystemNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSystemNatives.h"
#include	<time.h>

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern		CCore			* pCore;

void CSystemNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "log", Log, 1, "s" );
	pScriptingManager->RegisterFunction( "time", Time, 0, NULL );
	pScriptingManager->RegisterFunction( "getTickCount", TickCount, 0, NULL );
	pScriptingManager->RegisterFunction( "date", Date, 0, NULL );
	pScriptingManager->RegisterFunction( "getPlayerCount", PlayerCount, 0, NULL );
	pScriptingManager->RegisterFunction( "getMaxPlayers", MaxPlayers, 0, NULL );
	pScriptingManager->RegisterFunction( "getServerName", ServerName, 0, NULL );
	pScriptingManager->RegisterFunction( "str_rand", StrRand, 1, "i" );
	pScriptingManager->RegisterFunction( "int_concat", IntConcat, 2, "ii" );
}

SQInteger CSystemNatives::Log( SQVM * pVM )
{
	const SQChar * szText;
	sq_getstring( pVM, -1, &szText );

	CLogFile::Printf( szText );

	sq_pushbool( pVM, true );
	return 1;
}

SQInteger CSystemNatives::Time( SQVM * pVM )
{
	time_t t;
	time ( &t );
	sq_pushinteger ( pVM, *((SQInteger *)&t) );
	return 1;
}

SQInteger CSystemNatives::TickCount( SQVM * pVM )
{
	sq_pushinteger( pVM, SharedUtility::GetTime() );
	return 1;
}

SQInteger CSystemNatives::Date( SQVM * pVM )
{
	// Get the current time
	SQInteger iTime = SharedUtility::GetTime ();

	// Get the current time and date
	time_t t = time( NULL );
	const struct tm * tm = localtime( &t );

	// Create a new table
	sq_newtable( pVM );

	sq_pushstring( pVM, "day", strlen("day") );
	sq_pushinteger( pVM, tm->tm_mday );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "month", strlen("month") );
	sq_pushinteger( pVM, (tm->tm_mon + 1) );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "year", strlen("year") );
	sq_pushinteger( pVM, (tm->tm_year + 1900) );
	sq_createslot( pVM, -3 );

	sq_pushstring( pVM, "yearday", strlen("yearday") );
	sq_pushinteger( pVM, tm->tm_yday );
	sq_createslot( pVM, -3 );

	return 1;
}

SQInteger CSystemNatives::PlayerCount( SQVM * pVM )
{
	sq_pushinteger( pVM, pCore->GetPlayerManager()->GetCount() );
	return 1;
}

SQInteger CSystemNatives::MaxPlayers( SQVM * pVM )
{
#ifdef _CLIENT
	sq_pushinteger( pVM, pCore->GetServerMaxPlayers() );
#else
	sq_pushinteger( pVM, CVAR_GET_INTEGER( "maxplayers" ) );
#endif
	return 1;
}

SQInteger CSystemNatives::ServerName( SQVM * pVM )
{
#ifdef _CLIENT
	sq_pushstring( pVM, pCore->GetServerName().Get(), pCore->GetServerName().GetLength() );
#else
	sq_pushstring( pVM, CVAR_GET_STRING( "hostname" ), strlen(CVAR_GET_STRING( "hostname" )) );
#endif
	return 1;
}

SQInteger CSystemNatives::StrRand( SQVM * pVM )
{
	SQInteger iLen;
	sq_getinteger( pVM, -1, &iLen );

	String strRand = SharedUtility::str_rand ( iLen );

	sq_pushstring ( pVM, strRand.Get(), strRand.GetLength() );
	return 1;
}

SQInteger CSystemNatives::IntConcat( SQVM * pVM )
{
	SQInteger iOne, iTwo;
	sq_getinteger( pVM, -2, &iOne );
	sq_getinteger( pVM, -1, &iTwo );

	sq_pushinteger ( pVM, (SQInteger)SharedUtility::int_concat ( iOne, iTwo ) );
	return 1;
}