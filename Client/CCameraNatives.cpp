/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCameraNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

void CCameraNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getCameraPosition", GetPosition, 0, NULL );
	pScriptingManager->RegisterFunction( "getCameraRotation", GetRotation, 0, NULL );
}

// getCameraPosition();
SQInteger CCameraNatives::GetPosition( SQVM * pVM )
{
	CVector3 vecPosition;
	pCore->GetCamera()->GetPosition( &vecPosition );

	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecPosition.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecPosition.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecPosition.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}

// getCameraRotation();
SQInteger CCameraNatives::GetRotation( SQVM * pVM )
{
	CVector3 vecRight;
	pCore->GetCamera()->GetRight( &vecRight );

	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vecRight.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecRight.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vecRight.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
	return 1;
}