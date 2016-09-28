/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrel.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifndef NULL
#define		NULL 0
#endif

#include	<assert.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include "squirrel.h"
#include "sqobject.h"
#undef type
#include "sqstdio.h"
#include "sqstdaux.h"
#include "sqstdblob.h"
#include "sqstdsystem.h"
#include "sqstdmath.h"
#include "sqstdstring.h"
#include "sqstate.h"
#include "sqvm.h"
#include "CSquirrelArguments.h"
#include "Threading/CMutex.h"

#if defined(_WIN32) && defined(RegisterClass)
#	undef RegisterClass
#endif

struct ScriptClassMemberDecl
{
	char * szFunctionName;
	SQFUNCTION sqFunc;
	int iParamCount;
	char * szFunctionTemplate;
};

struct SquirrelClassDecl
{
	char * name;
	char * base;
	ScriptClassMemberDecl * members;
};

struct SquirrelArguments
{
	int iArgCount;
	SQObjectPtr * pArguments;
};

class CSquirrel
{

private:

	SQVM														* m_pVM;
	String														m_strName;
	String														m_strPath;
	CMutex														m_squirrelCallMutex;

	static void							PrintFunction				( SQVM * pVM, const SQChar * szFormat, ... );
	static void							ErrorFunction				( SQVM * pVM, const SQChar * szFormat, ... );
	static void							CompileErrorFunction		( SQVM * pVM, const SQChar * szError, const SQChar * szSource, SQInteger iLine, SQInteger iColumn );

public:

	SQVM								* GetVM						( void ) { return m_pVM; }
	String								GetName						( void ) { return m_strName; }
	String								GetPath						( void ) { return m_strPath; }

	bool								Load						( String strName, String strPath );
	bool								Execute						( void );
	void								Unload						( void );

	bool								RegisterClass				( SquirrelClassDecl * pClassDecl );
	void								RegisterFunction			( String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate );
	void								RegisterConstant			( String strConstantName, CSquirrelArgument value );

	void								Call						( SQObjectPtr pFunction, CSquirrelArguments * pArguments = NULL, CSquirrelArgument * pReturn = NULL );

};
