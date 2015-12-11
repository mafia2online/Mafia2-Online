/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrel.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../../Client/StdInc.h"
#else
#include	"../../Server/StdInc.h"
#endif

extern	CCore				* pCore;

void CSquirrel::PrintFunction( SQVM * pVM, const SQChar * szFormat, ... )
{
	va_list args;
	char szBuffer[512];
	va_start( args, szFormat );
	vsnprintf( szBuffer, sizeof(szBuffer), szFormat, args );
	va_end( args );
	CLogFile::Printf( szBuffer );
}

void CSquirrel::ErrorFunction( SQVM * pVM, const SQChar * szFormat, ... )
{
	va_list args;
	char szBuffer[512];
	va_start( args, szFormat );
	vsnprintf( szBuffer, sizeof(szBuffer), szFormat, args );
	va_end( args );

	char * tmp = (char *)szBuffer;
	size_t offstart = 0, offend = 0;

	size_t len = strlen( tmp );
	for( size_t i = 0; i < len; ++i )
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offstart;
			break;
		default:
			i = len - 1;
			break;
		}
	}

	tmp += offstart;
	len -= offstart;

	for (size_t i = len - 1; i > 0; --i)
	{
		switch (tmp[i])
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			++offend;
			break;
		default:
			i = 1;
			break;
		}
	}

	tmp[ len - offend ] = '\0';

	// Find the script
#ifndef _CLIENT
	CSquirrel * pScript = pCore->GetScriptingManager()->Get( pVM );
#else
	CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM );
#endif

	CLogFile::Printf( "Script Error: %s", tmp );

	if( pScript )
	{
		CSquirrelArguments pArguments;
		pArguments.push( tmp );

		// Call scriptError event
#ifndef _CLIENT
		pCore->GetEvents()->Call( "onScriptError", &pArguments, pScript );
#else
		pCore->GetClientScriptingManager()->GetEvents()->Call( "onScriptError", &pArguments, pScript );
#endif
	}
}

void CSquirrel::CompileErrorFunction( SQVM * pVM, const SQChar * szError, const SQChar * szSource, SQInteger iLine, SQInteger iColumn )
{
	// Find the script
#ifndef _CLIENT
	CSquirrel * pScript = pCore->GetScriptingManager()->Get( pVM );
#else
	CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM );
#endif

	if( pScript )
	{
		CSquirrelArguments pArguments;
		pArguments.push( (const char *)szError );
		pArguments.push( (const char *)szSource );
		pArguments.push( (int)iLine );
		pArguments.push( (int)iColumn );

		// Call scriptError event
#ifndef _CLIENT
		pCore->GetEvents()->Call( "onScriptCompileError", &pArguments, pScript );
#else
		pCore->GetClientScriptingManager()->GetEvents()->Call( "onScriptCompileError", &pArguments, pScript );
#endif

		CLogFile::Printf( "CompileError: %s", (const char *)szError );
	}
}

bool CSquirrel::Load( String strName, String strPath )
{
	// Does the script not exist?
	if( !SharedUtility::Exists( strPath.Get() ) )
		return false;

	// Set the script name
	m_strName = strName;

	// Set the script path
	m_strPath = strPath;

	// Create a squirrel vm
	m_pVM = sq_open( 1024 );

	// Register the default error handlers
	sqstd_seterrorhandlers( m_pVM );

	// Set the print function and error function
	sq_setprintfunc( m_pVM, PrintFunction, ErrorFunction );

	// Set the compile error handler
	sq_setcompilererrorhandler( m_pVM, CompileErrorFunction );

	// Push the root table onto the stack
	sq_pushroottable( m_pVM );

#ifndef _CLIENT
	// Register the I/O library
	sqstd_register_iolib( m_pVM );
#endif

	// Register the blob library
	sqstd_register_bloblib( m_pVM );

	// Register the math library
	sqstd_register_mathlib( m_pVM );

	// Register the string library
	sqstd_register_stringlib( m_pVM );

	return true;
}

bool CSquirrel::Execute( void )
{
	// Add the script name constant
	RegisterConstant( "SCRIPT_NAME", m_strName.Get() );

	// Add the script path constant
	RegisterConstant( "SCRIPT_PATH", m_strPath.Get() );

	// Load and compile the script
	if( SQ_FAILED( sqstd_dofile( m_pVM, m_strPath.Get(), SQFalse, SQTrue ) ) )
		return false;

	return true;
}

void CSquirrel::Unload( void )
{
	// Is the script vm instance invalid?
	if( !m_pVM )
		return;

	// Pop the root table from the stack
	sq_pop( m_pVM, 1 );

	// Close the squirrel vm
	sq_close( m_pVM );

	// Reset the vm pointer
	m_pVM = NULL;
}

bool CSquirrel::RegisterClass( SquirrelClassDecl * pClassDecl )
{
	// Get the stack top
	int iOldTop = sq_gettop( m_pVM );

	// Push the class name onto the stack
	sq_pushstring( m_pVM, pClassDecl->name, -1 );

	// Do we have a base class?
	if( pClassDecl->base )
	{
		// Push the base class name onto the stack
		sq_pushstring( m_pVM, pClassDecl->base, -1 );

		// Attempt to get the base class
		if( SQ_FAILED(sq_get( m_pVM, -3 )) )
		{
			sq_settop( m_pVM, iOldTop );
			return false;
		}
	}

	// Create the class
	if( SQ_FAILED(sq_newclass( m_pVM, pClassDecl->base ? 1 : 0 )) )
	{
		sq_settop( m_pVM, iOldTop );
		return false;
	}

	// Register the class members
	const ScriptClassMemberDecl * pMembers = pClassDecl->members;

	for( int i = 0; pMembers[i].szFunctionName; i++ )
	{
		// Register the function
		RegisterFunction( pMembers[i].szFunctionName, pMembers[i].sqFunc, pMembers[i].iParamCount, pMembers[i].szFunctionTemplate );
	}

	// Create a new slot
	sq_createslot( m_pVM, -3 );
	return true;
}

void CSquirrel::RegisterFunction( String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate )
{
	// Push the function name onto the stack
	sq_pushstring( m_pVM, strFunctionName.Get(), -1 );

	// Create a new function
	sq_newclosure( m_pVM, pfnFunction, 0 );

	// Set the function parameter template and count
	if( iParameterCount != -1 )
	{
		String strTypeMask;

		if( !strFunctionTemplate.IsEmpty() )
			strTypeMask.Format( ".%s", strFunctionTemplate.Get() );

		sq_setparamscheck( m_pVM, (iParameterCount + 1), strTypeMask.Get() );
	}

	// Create a new slot
	sq_createslot( m_pVM, -3 );
}

void CSquirrel::RegisterConstant( String strConstantName, CSquirrelArgument value )
{
	// Push the constant name onto the stack
	sq_pushstring( m_pVM, strConstantName.Get(), -1 );

	// Push the constant value onto the stack
	value.push( m_pVM );

	// Create a new slot
	sq_createslot( m_pVM, -3 );
}

void CSquirrel::Call( SQObjectPtr pFunction, CSquirrelArguments * pArguments, CSquirrelArgument * pReturn )
{
	// Get the stack top
	int iTop = sq_gettop( m_pVM );

	// Process the parameters if needed
	int iParams = 1;

	if( pArguments )
	{
		pArguments->push_to_vm( m_pVM );
		iParams += pArguments->size();
	}

	// Create the return value pointer
	SQObjectPtr res;

	// Call the function
	if( m_pVM->Call(pFunction, iParams, (m_pVM->_top - iParams), res, true) )
	{
		// Should we set the return value?
		if( pReturn )
		{
			// Set the return value
			pReturn->set( res );
		}
	}

	// Restore the stack top
	sq_settop( m_pVM, iTop );
}
