/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrelCommon.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CSquirrelCommon.h"

/*
void sq_getentity( SQVM * pVM, int idx, EntityId * entity )
{
	SQInteger sqEntity;
	sq_getinteger( pVM, idx, &sqEntity );
	*entity = (EntityId)sqEntity;
}

void sq_getstrstring( SQVM * pVM, int idx, String * strString )
{
	const SQChar * szMessage;
	sq_getstring( pVM, idx, &szMessage );
	strString->Set( szMessage );
}

void sq_pushstrstring( SQVM * pVM, String strString )
{
	sq_pushstring( pVM, strString.Get(), strString.GetLength() );
}

void sq_getvector( SQVM * pVM, int idx, CVector3 * vector )
{
	SQFloat x, y, z;
	sq_getfloat( pVM, idx, &z );
	sq_getfloat( pVM, (idx - 1), &y );
	sq_getfloat( pVM, (idx - 2), &x );

	vector->fX = x;
	vector->fY = y;
	vector->fZ = z;
}

void sq_getvector2( SQVM * pVM, SQInteger idx, Vector2 * vector )
{
	SQFloat x, y;
	sq_getfloat( pVM, idx, &y );
	sq_getfloat( pVM, (idx - 1), &x );

	vector->fX = x;
	vector->fY = y;
}

void sq_getquaternion( SQVM * pVM, SQInteger idx, Quaternion * quat )
{
	SQFloat x, y, z, w;
	sq_getfloat( pVM, idx, &w );
	sq_getfloat( pVM, (idx - 1), &z );
	sq_getfloat( pVM, (idx - 2), &y );
	sq_getfloat( pVM, (idx - 3), &x );

	quat->fX = x;
	quat->fY = y;
	quat->fZ = z;
	quat->fW = w;
}

void sq_pushvector( SQVM * pVM, CVector3 vector )
{
	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vector.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vector.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vector.fZ );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
}

void sq_pushvector2( SQVM * pVM, Vector2 vector )
{
	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, vector.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, vector.fY );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
}

void sq_pushquaternion( SQVM * pVM, Quaternion quat )
{
	sq_newarray( pVM, 0 );

	sq_pushfloat( pVM, quat.fX );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, quat.fY );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, quat.fZ );
	sq_arrayappend( pVM, -2 );

	sq_pushfloat( pVM, quat.fW );
	sq_arrayappend( pVM, -2 );

	sq_push( pVM, -1 );
}

void sq_pusharg( SQVM * pVM, CSquirrelArgument arg )
{
	arg.push( pVM );
}

void sq_getarg( SQVM * pVM, SQInteger idx, CSquirrelArgument * arg )
{
	arg->pushFromStack( pVM, idx );
}*/