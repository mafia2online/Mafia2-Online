/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CUtilNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CUtilNatives.h"
#include	"../../Math/CMaths.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

void CUtilNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getPlayerIdFromName", PlayerIdFromName, 1, "s" );
	//pScriptingManager->RegisterFunction( "getWeaponNameFromId", WeaponNameFromId, 1, "i" );
	//pScriptingManager->RegisterFunction( "getWeaponIdFromName", WeaponIdFromName, 1, "s" );
	pScriptingManager->RegisterFunction( "toRGBA", ToRGBA, 1, "i" );
	pScriptingManager->RegisterFunction( "fromRGB", FromRGB, -1, NULL );
	pScriptingManager->RegisterFunction( "clamp", Clamp, 3, "fff" );
}

SQInteger CUtilNatives::PlayerIdFromName( SQVM * pVM )
{
	const SQChar * szName;
	sq_getstring( pVM, -1, &szName );

	for ( EntityId i = 0; i < MAX_PLAYERS; i ++ )
	{
		if ( pCore->GetPlayerManager()->IsActive ( i ) )
		{
			if ( !strcmp ( szName, pCore->GetPlayerManager()->Get( i )->GetNick() ) )
			{
				sq_pushinteger( pVM, pCore->GetPlayerManager()->Get( i )->GetId () );
				return 1;
			}
		}
	}

	sq_pushinteger( pVM, INVALID_ENTITY_ID );
	return 1;
}

// toRGBA( colour );
SQInteger CUtilNatives::ToRGBA( SQVM * pVM )
{
	SQInteger iColour;
	sq_getinteger( pVM, -1, &iColour );

	CColor m_color( (unsigned long)iColour );
	
	CSquirrelArguments args;
	args.push( m_color.R );
	args.push( m_color.G );
	args.push( m_color.B );
	args.push( m_color.A );

	CSquirrelArgument arguments(args, true);
	arguments.push( pVM );
	return 1;
}

// fromRGB( r, g, b[, a = 255] );
SQInteger CUtilNatives::FromRGB( SQVM * pVM )
{
	// Get the stack top
	int iTop = sq_gettop( pVM ) - 1;

	if( iTop < 3 || iTop > 4 )
	{
		CHECK_PARAMS( "fromRGB", 3 );
	}

	SQInteger r, g, b;
	SQInteger a = 255;
	
	if( iTop > 3 )
	{
		CHECK_TYPE( "fromRGB", 1, -4, OT_INTEGER );
		CHECK_TYPE( "fromRGB", 2, -3, OT_INTEGER );
		CHECK_TYPE( "fromRGB", 3, -2, OT_INTEGER );
		CHECK_TYPE( "fromRGB", 4, -1, OT_INTEGER );

		sq_getinteger( pVM, -4, &r );
		sq_getinteger( pVM, -3, &g );
		sq_getinteger( pVM, -2, &b );
		sq_getinteger( pVM, -1, &a );
	}
	else
	{
		CHECK_TYPE( "fromRGB", 1, -3, OT_INTEGER );
		CHECK_TYPE( "fromRGB", 2, -2, OT_INTEGER );
		CHECK_TYPE( "fromRGB", 3, -1, OT_INTEGER );

		sq_getinteger( pVM, -3, &r );
		sq_getinteger( pVM, -2, &g );
		sq_getinteger( pVM, -1, &b );
	}
	
	CColor m_color( r, g, b, a );
	sq_pushinteger( pVM, m_color.dwHexColor );
	return 1;
}

// clamp( minimum, value, maximum );
SQInteger CUtilNatives::Clamp( SQVM * pVM )
{
	float fMin, fValue, fMax;

	sq_getfloat( pVM, -3, &fMin );
	sq_getfloat( pVM, -2, &fValue );
	sq_getfloat( pVM, -1, &fMax );

	sq_pushfloat( pVM, Math::Clamp< float >( fMin, fValue, fMax )) ;
	return 1;
}