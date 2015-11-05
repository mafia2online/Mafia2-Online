/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CAreaNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CAreaNatives.h"
#include	"../../Math/CMaths.h"
#include	"../CSquirrelCommon.h"


void CAreaNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "getDistanceBetweenPoints3D", DistanceBetweenPoints3D, 6, "ffffff" );
	pScriptingManager->RegisterFunction( "getDistanceBetweenPoints2D", DistanceBetweenPoints2D, 4, "ffff" );
	pScriptingManager->RegisterFunction( "isPointInArea3D", PointInArea3D, 9, "fffffffff" );
	pScriptingManager->RegisterFunction( "isPointInArea2D", PointInArea2D, 6, "ffffff" );
	pScriptingManager->RegisterFunction( "isPointInCircle3D", PointInCircle3D, 7, "fffffff" );
	pScriptingManager->RegisterFunction( "isPointInCircle2D", PointInCircle2D, 5, "fffff" );
	pScriptingManager->RegisterFunction( "isPointInTube", PointInTube, 8, "ffffffff" );
}

// getDistanceBetweenPoints3D( x, y, z, xx, yy, zz );
SQInteger CAreaNatives::DistanceBetweenPoints3D( SQVM * pVM )
{
	CVector3 vecPosition1, vecPosition2;

	sq_getfloat( pVM, -1, &vecPosition2.fZ );
	sq_getfloat( pVM, -2, &vecPosition2.fY );
	sq_getfloat( pVM, -3, &vecPosition2.fX );

	sq_getfloat( pVM, -4, &vecPosition1.fZ );
	sq_getfloat( pVM, -5, &vecPosition1.fY );
	sq_getfloat( pVM, -6, &vecPosition1.fX );

	sq_pushfloat( pVM, Math::GetDistanceBetweenPoints( vecPosition1, vecPosition2 ) );
	return 1;
}

// getDistanceBetweenPoitns2D( x, y, xx, yy );
SQInteger CAreaNatives::DistanceBetweenPoints2D( SQVM * pVM )
{
	float x, y, xx, yy;
	sq_getfloat( pVM, -4, &x );
	sq_getfloat( pVM, -3, &y );
	sq_getfloat( pVM, -2, &xx );
	sq_getfloat( pVM, -1, &yy );

	sq_pushfloat( pVM, Math::GetDistanceBetweenPoints( x, y, xx, yy ) );
	return 1;
}

// isPointInArea3D( pointX, pointY, pointZ, x, y, z, xx, yy, zz );
SQInteger CAreaNatives::PointInArea3D( SQVM * pVM )
{
	CVector3 vecArea, vecArea2, vecPoint;

	sq_getfloat( pVM, -1, &vecArea2.fZ );
	sq_getfloat( pVM, -2, &vecArea2.fY );
	sq_getfloat( pVM, -3, &vecArea2.fX );

	sq_getfloat( pVM, -4, &vecArea.fZ );
	sq_getfloat( pVM, -5, &vecArea.fY );
	sq_getfloat( pVM, -6, &vecArea.fX );

	sq_getfloat( pVM, -7, &vecPoint.fZ );
	sq_getfloat( pVM, -8, &vecPoint.fY );
	sq_getfloat( pVM, -9, &vecPoint.fX );

	sq_pushbool( pVM, Math::IsPointInArea( vecArea, vecArea2, vecPoint ) );
	return 1;
}

// isPointInArea2D( pointX, pointY, x, y, xx, yy );
SQInteger CAreaNatives::PointInArea2D( SQVM * pVM )
{
	float pointX, pointY;
	sq_getfloat( pVM, -6, &pointX );
	sq_getfloat( pVM, -5, &pointY );

	float x, y;
	sq_getfloat( pVM, -4, &x );
	sq_getfloat( pVM, -3, &y );

	float xx, yy;
	sq_getfloat( pVM, -2, &xx );
	sq_getfloat( pVM, -1, &yy );

	sq_pushbool( pVM, Math::IsPointInArea( x, y, xx, yy, pointX, pointY ) );
	return 1;
}

// isPointInCircle3D( pointX, pointY, pointZ, x, y, z, radius );
SQInteger CAreaNatives::PointInCircle3D( SQVM * pVM )
{
	CVector3 vecPoint, vecCircle;

	sq_getfloat( pVM, -2, &vecCircle.fZ );
	sq_getfloat( pVM, -3, &vecCircle.fY );
	sq_getfloat( pVM, -4, &vecCircle.fX );

	sq_getfloat( pVM, -5, &vecPoint.fZ );
	sq_getfloat( pVM, -6, &vecPoint.fY );
	sq_getfloat( pVM, -7, &vecPoint.fX );

	float fRadius;
	sq_getfloat( pVM, -1, &fRadius );

	sq_pushbool( pVM, Math::IsPointInBall( vecCircle, fRadius, vecPoint ) );
	return 1;
}

// isPointInCircle2D( pointX, pointY, x, y, radius );
SQInteger CAreaNatives::PointInCircle2D( SQVM * pVM )
{
	float pointX, pointY;
	sq_getfloat( pVM, -5, &pointX );
	sq_getfloat( pVM, -4, &pointY );

	float x, y;
	sq_getfloat( pVM, -3, &x );
	sq_getfloat( pVM, -2, &y );

	float fRadius;
	sq_getfloat( pVM, -1, &fRadius );

	sq_pushbool( pVM, Math::IsPointInBall( x, y, fRadius, pointX, pointY ) );
	return 1;
}

// isPointInTube( pointX, pointY, pointZ, tubeX, tubeY, tubeZ, tubeHeight, tubeRadius );
SQInteger CAreaNatives::PointInTube( SQVM * pVM )
{
	CVector3 vecPoint, vecTube;

	sq_getfloat( pVM, -3, &vecTube.fZ );
	sq_getfloat( pVM, -4, &vecTube.fY );
	sq_getfloat( pVM, -5, &vecTube.fX );

	sq_getfloat( pVM, -6, &vecPoint.fZ );
	sq_getfloat( pVM, -7, &vecPoint.fY );
	sq_getfloat( pVM, -8, &vecPoint.fX );

	float fTubeHeight;
	sq_getfloat( pVM, -2, &fTubeHeight );

	float fTubeRadius;
	sq_getfloat( pVM, -1, &fTubeRadius );

	sq_pushbool( pVM, Math::IsPointInTube( vecTube, fTubeHeight, fTubeRadius, vecPoint ) );
	return 1;
}
