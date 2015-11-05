/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CScriptingManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CString.h"
#include	<assert.h>
#include	<list>
#include	"CSquirrel.h"
#include	"CSquirrelArguments.h"

#if defined(_WIN32) && defined(RegisterClass)
#undef RegisterClass
#endif

struct ScriptingFunction
{
	String				strName;
	SQFUNCTION			pfnFunction;
	int					iParameterCount;
	String				strTemplate;
};

struct ScriptingConstant
{
	String				strName;
	CSquirrelArgument	value;
};

class CScriptingManager
{

private:

	std::list< CSquirrel *>				m_scripts;
	std::list< ScriptingFunction *>		m_functions;
	std::list< ScriptingConstant *>		m_constants;
	std::list< SquirrelClassDecl *>		m_classes;

public:

										CScriptingManager			( void );

	CSquirrel							* Load						( String strName, String strPath );
	void								LoadAll						( std::list< String > scripts );
	bool								Unload						( String strName );
	void								UnloadAll					( void );

	void								NewClass					( SquirrelClassDecl * pClassDecl );
	void								RegisterFunction			( String strFunctionName, SQFUNCTION pfnFunction, int iParameterCount, String strFunctionTemplate );
	void								RegisterConstant			( String strConstantName, CSquirrelArgument value );
	void								RegisterDefaultConstants	( void );

	void								Call						( SQObjectPtr, CSquirrelArguments * pArguments, SQVM * pVM );

	CSquirrel							* Get						( String strName );
	CSquirrel							* Get						( SQVM * pVM );

};