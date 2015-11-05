#ifndef C_SQUIRREL_COMMON_H
#define C_SQUIRREL_COMMON_H
/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrelCommon.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifdef _CLIENT
#include	"../../Client/StdInc.h"
#else
#include	"../../Server/StdInc.h"
#endif

#include	"../Scripting/CSquirrel.h"

#ifndef NULL
#define NULL 0
#endif

template< typename T >
SQRESULT sq_setinstance( SQVM * pVM, T pInstance, SQInteger idx = 1 )
{
	return sq_setinstanceup( pVM, idx, (SQUserPointer *)pInstance );
}

template< typename T >
T sq_getinstance( SQVM * pVM, SQInteger idx = 1 )
{
	SQUserPointer pInstance = NULL;

	if( SQ_FAILED( sq_getinstanceup( pVM, idx, &pInstance, NULL ) ) )
		pInstance = NULL;

	return (T)pInstance;
}

template< typename T >
void sq_pushpointer( SQVM * pVM, T pInstance )
{
	sq_pushuserpointer( pVM, (SQUserPointer *)pInstance );
}

template< typename T >
T sq_getpointer( SQVM * pVM, SQInteger idx = 1 )
{
	SQUserPointer pInstance = NULL;
	sq_getuserpointer( pVM, idx, &pInstance );

	return (T)pInstance;
}

/*
void		sq_getentity( SQVM * pVM, SQInteger idx, EntityId * entity );
void		sq_getstrstring( SQVM * pVM, SQInteger idx, String * strString );
void		sq_pushstrstring( SQVM * pVM, String strString );
void		sq_getvector( SQVM * pVM, SQInteger idx, CVector3 * vector );
void		sq_getvector2( SQVM * pVM, SQInteger idx, Vector2 * vector );
void		sq_getquaternion( SQVM * pVM, SQInteger idx, Quaternion * quat );
void		sq_pushvector( SQVM * pVM, CVector3 vector );
void		sq_pushvector2( SQVM * pVM, Vector2 vector );
void		sq_pushquaternion( SQVM * pVM, Quaternion quat );
void		sq_pusharg( SQVM * pVM, CSquirrelArgument arg );
void		sq_getarg( SQVM * pVM, SQInteger idx, CSquirrelArgument * arg );
*/

#define _SET_RELEASE_HOOK(classname) \
	sq_setreleasehook(pVM, 1, __##classname##_releasehook);

#define _MEMBER_FUNCTION_IMPL(classname, name) \
	SQInteger __##classname##_##name(SQVM * pVM)

#define _MEMBER_FUNCTION_RELEASE_HOOK(classname) \
	SQInteger __##classname##_releasehook(SQUserPointer pInst, SQInteger size)

#define _BEGIN_CLASS(classname)  \
	SQInteger __##classname##__typeof(SQVM * pVM) \
	{ \
		sq_pushstring(pVM, #classname, -1); \
		return 1; \
	} \
	struct ScriptClassMemberDecl __##classname##_members[] = \
	{ \
		{ "_typeof", __##classname##__typeof },

#define _MEMBER_FUNCTION(classname, name, parametercount, functiontemplate) \
		{ #name, __##classname##_##name, parametercount, functiontemplate },

#define _END_CLASS(classname) \
		{ NULL, NULL } \
	}; \
	struct SquirrelClassDecl __##classname##_decl = \
		{  #classname, NULL, __##classname##_members }; 

#define _END_CLASS_BASE(classname, baseclassname) \
		{ NULL, NULL } \
	}; \
	struct SquirrelClassDecl __##classname##_decl = \
{  #classname, #baseclassname, __##classname##_members }; 

#define _CLASS_DECL(classname) \
	__##classname##_decl

//
#define SQUIRREL_FUNCTION(name) SQInteger sq_##name(SQVM * pVM)

#define CHECK_PARAMS(szName, iParams) \
	{ \
		int iTop = sq_gettop( pVM ) - 1; \
		if( iTop != iParams ) \
		{ \
			sq_pushbool( pVM, false ); \
			return 1; \
		} \
	} \

#define CHECK_PARAMS_MIN(szName, iMinParams) \
	{ \
		int iTop = sq_gettop( pVM ) - 1; \
		if( iTop < iMinParams ) \
		{ \
			sq_pushbool( pVM, false ); \
			return 1; \
		} \
	} \

#define CHECK_PARAMS_MIN_MAX(szName, iMinParams, iMaxParams) \
	{ \
		int iTop = sq_gettop( pVM ) - 1; \
		if( iTop < iMinParams || iTop > iMaxParams ) \
		{ \
			sq_pushbool( pVM, false ); \
			return 1; \
		} \
	} \

#define CHECK_TYPE(szNativeName, iParam, iIndex, iTypeRequired) \
	{ \
		int iType = sq_gettype( pVM, iIndex ); \
		if( iType != iTypeRequired ) \
		{ \
			if( !(iTypeRequired == OT_BOOL && iType == OT_INTEGER) && !(iTypeRequired == OT_FLOAT && iType == OT_INTEGER) ) \
			{ \
				sq_pushbool( pVM, false ); \
				return 1; \
			} \
		} \
	}
#endif
