/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCameraNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"

#include "CM2Camera.h"

#include "Scripting/CScriptingManager.h"

#include "CCameraNatives.h"

void CCameraNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getCameraPosition", GetPosition, 0, NULL );
	pScriptingManager->RegisterFunction( "getCameraRotation", GetRotation, 0, NULL );
	pScriptingManager->RegisterFunction( "simpleShake", SimpleShake, 3, "fff");
}

// simpleShake(float speed, float strength, float duration);
SQInteger CCameraNatives::SimpleShake(SQVM * pVM)
{
	// Variables
	SQFloat speed;
	SQFloat strength;
	SQFloat duration;

	// Fetch params
	sq_getfloat(pVM, -3, &speed);
	sq_getfloat(pVM, -2, &strength);
	sq_getfloat(pVM, -1, &duration);

	// Apply and return
	CCore::Instance()->GetCamera()->SimpleShake(speed, strength, duration);
	sq_pushbool(pVM, true);
	return (true);
}

// getCameraPosition();
SQInteger CCameraNatives::GetPosition( SQVM * pVM )
{
	CVector3 vecPosition;
	CCore::Instance()->GetCamera()->GetPosition(&vecPosition);

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
	CCore::Instance()->GetCamera()->GetRight(&vecRight);

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