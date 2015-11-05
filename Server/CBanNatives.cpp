/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CBanNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CBanNatives.h"

extern		CCore			* pCore;

void CBanNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "banPlayer", BanPlayer, 4, "iiis" );
	pScriptingManager->RegisterFunction( "banSerial", BanSerial, 4, "siis" );
	pScriptingManager->RegisterFunction( "unbanSerial", UnbanSerial, 1, "s" );
	pScriptingManager->RegisterFunction( "isSerialBanned", IsSerialBanned, 1, "s" );
}

SQInteger CBanNatives::BanPlayer( SQVM * pVM )
{
	SQInteger playerId, bannerId;
	SQInteger iSeconds;
	const SQChar * szReason;

	sq_getinteger( pVM, -4, &playerId );
	sq_getinteger( pVM, -3, &bannerId );
	sq_getinteger( pVM, -2, &iSeconds );
	sq_getstring( pVM, -1, &szReason );

	String strBanner( "Console" );

	// Is the player active?
	if( pCore->GetPlayerManager()->Get( playerId ) )
	{
		// Get the player serial
		String strSerial = pCore->GetPlayerManager()->Get( playerId )->GetSerial();

		// Is the banner not the console?
		if( bannerId != ENTITY_ID_CONSOLE )
		{
			// Is the banner active?
			if( pCore->GetPlayerManager()->Get( bannerId ) )
			{
				strBanner = pCore->GetPlayerManager()->Get( bannerId )->GetNick();
			}
		}

		// Add the ban to the ban manager
		sq_pushbool( pVM, pCore->GetBanManager()->Add( strSerial, strBanner, SharedUtility::GetTime(), (unsigned long)iSeconds, szReason ) );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CBanNatives::BanSerial( SQVM * pVM )
{
	const SQChar * szSerial;
	SQInteger bannerId;
	SQInteger iSeconds;
	const SQChar * szReason;

	sq_getstring( pVM, -4, &szSerial );
	sq_getinteger( pVM, -3, &bannerId );
	sq_getinteger( pVM, -2, &iSeconds );
	sq_getstring( pVM, -1, &szReason );

	String strBanner( "Console" );

	// Is the banner not the console?
	if( bannerId != ENTITY_ID_CONSOLE )
	{
		// Is the banner active?
		if( pCore->GetPlayerManager()->Get( bannerId ) )
		{
			strBanner = pCore->GetPlayerManager()->Get( bannerId )->GetNick();
		}
	}

	// Add the ban to the ban manager
	sq_pushbool( pVM, pCore->GetBanManager()->Add( szSerial, strBanner, SharedUtility::GetTime(), (unsigned long)iSeconds, szReason ) );
	return 1;
}

SQInteger CBanNatives::UnbanSerial( SQVM * pVM )
{
	const SQChar * szSerial;
	sq_getstring( pVM, -1, &szSerial );

	// Is the serial banned?
	if( pCore->GetBanManager()->IsSerialBanned( szSerial ) )
	{
		// Unban the server
		pCore->GetBanManager()->Remove( szSerial );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

SQInteger CBanNatives::IsSerialBanned( SQVM * pVM )
{
	const SQChar * szSerial;
	sq_getstring( pVM, -1, &szSerial );

	sq_pushbool( pVM, pCore->GetBanManager()->IsSerialBanned( szSerial ) );
	return 1;
}
