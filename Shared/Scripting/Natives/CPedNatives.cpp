/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CPedNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once
#ifdef _CLIENT

#include	"CPedNatives.h"
#include	"../../Math/CMaths.h"
#include	"../CSquirrelCommon.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

void CPedNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "createPed", CreatePed, 7, "iffffff" );
	pScriptingManager->RegisterFunction( "destroyPed", DestroyPed, 1, "i" );
}

SQInteger CPedNatives::CreatePed( SQVM * pVM )
{
	SQInteger iModelIndex;
	CVector3 vecPosition;
	CVector3 vecRotation;

	sq_getinteger( pVM, -7, &iModelIndex );

	sq_getfloat( pVM, -6, &vecPosition.fX );
	sq_getfloat( pVM, -5, &vecPosition.fY );
	sq_getfloat( pVM, -4, &vecPosition.fZ );

	sq_getfloat( pVM, -3, &vecRotation.fX );
	sq_getfloat( pVM, -2, &vecRotation.fY );
	sq_getfloat( pVM, -1, &vecRotation.fZ );

	CLogFile::Printf( "Model: %d, Pos: %f, %f, %f, Rot: %f, %f, %f", iModelIndex, vecPosition.fX, vecPosition.fY, vecPosition.fZ, vecRotation.fX, vecRotation.fY, vecRotation.fZ );

	// Add the ped to the manager
	sq_pushinteger( pVM, pCore->GetPedManager()->Add( iModelIndex, vecPosition, vecRotation ) );
	return 1;
}

SQInteger CPedNatives::DestroyPed( SQVM * pVM )
{
	SQInteger pedId;
	sq_getinteger( pVM, -1, &pedId );

	// Delete the ped from the manager
	sq_pushbool( pVM, pCore->GetPedManager()->Delete( pedId ) );
	return 1;
}
#endif
