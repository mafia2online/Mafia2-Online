/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CModule.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"
#include "CCore.h"
#include "CLogFile.h"

static void * SquirrelInterfaceContainer[] =
{
	(void *)sq_open,
	(void *)sq_newthread,
	(void *)sq_seterrorhandler,
	(void *)sq_close,
	(void *)sq_setforeignptr,
	(void *)sq_getforeignptr,
	(void *)sq_setprintfunc,
	(void *)sq_getprintfunc,
	(void *)sq_geterrorfunc,
	(void *)sq_suspendvm,
	(void *)sq_wakeupvm,
	(void *)sq_getvmstate,
	(void *)sq_compile,
	(void *)sq_compilebuffer,
	(void *)sq_enabledebuginfo,
	(void *)sq_notifyallexceptions,
	(void *)sq_setcompilererrorhandler,
	(void *)sq_push,
	(void *)sq_pop,
	(void *)sq_poptop,
	(void *)sq_remove,
	(void *)sq_gettop,
	(void *)sq_settop,
	(void *)sq_reservestack,
	(void *)sq_cmp,
	(void *)sq_move,
	(void *)sq_newuserdata,
	(void *)sq_newtable,
	(void *)sq_newarray,
	(void *)sq_newclosure,
	(void *)sq_setparamscheck,
	(void *)sq_bindenv,
	(void *)sq_pushstring,
	(void *)sq_pushfloat,
	(void *)sq_pushinteger,
	(void *)sq_pushbool,
	(void *)sq_pushuserpointer,
	(void *)sq_pushnull,
	(void *)sq_gettype,
	(void *)sq_getsize,
	(void *)sq_getbase,
	(void *)sq_instanceof,
	(void *)sq_tostring,
	(void *)sq_tobool,
	(void *)sq_getstring,
	(void *)sq_getinteger,
	(void *)sq_getfloat,
	(void *)sq_getbool,
	(void *)sq_getthread,
	(void *)sq_getuserpointer,
	(void *)sq_getuserdata,
	(void *)sq_settypetag,
	(void *)sq_gettypetag,
	(void *)sq_setreleasehook,
	(void *)sq_getscratchpad,
	(void *)sq_getfunctioninfo,
	(void *)sq_getclosureinfo,
	(void *)sq_setnativeclosurename,
	(void *)sq_setinstanceup,
	(void *)sq_getinstanceup,
	(void *)sq_setclassudsize,
	(void *)sq_newclass,
	(void *)sq_createinstance,
	(void *)sq_setattributes,
	(void *)sq_getattributes,
	(void *)sq_getclass,
	(void *)sq_weakref,
	(void *)sq_getdefaultdelegate,
	(void *)sq_pushroottable,
	(void *)sq_pushregistrytable,
	(void *)sq_pushconsttable,
	(void *)sq_setroottable,
	(void *)sq_setconsttable,
	(void *)sq_newslot,
	(void *)sq_deleteslot,
	(void *)sq_set,
	(void *)sq_get,
	(void *)sq_rawget,
	(void *)sq_rawset,
	(void *)sq_rawdeleteslot,
	(void *)sq_arrayappend,
	(void *)sq_arraypop,
	(void *)sq_arrayresize,
	(void *)sq_arrayreverse,
	(void *)sq_arrayremove,
	(void *)sq_arrayinsert,
	(void *)sq_setdelegate,
	(void *)sq_getdelegate,
	(void *)sq_clone,
	(void *)sq_setfreevariable,
	(void *)sq_next,
	(void *)sq_getweakrefval,
	(void *)sq_clear,
	(void *)sq_call,
	(void *)sq_resume,
	(void *)sq_getlocal,
	(void *)sq_getfreevariable,
	(void *)sq_throwerror,
	(void *)sq_reseterror,
	(void *)sq_getlasterror,
	(void *)sq_getstackobj,
	(void *)sq_pushobject,
	(void *)sq_addref,
	(void *)sq_release,
	(void *)sq_resetobject,
	(void *)sq_objtostring,
	(void *)sq_objtobool,
	(void *)sq_objtointeger,
	(void *)sq_objtofloat,
	(void *)sq_getobjtypetag,
	(void *)sq_collectgarbage,
	(void *)sq_writeclosure,
	(void *)sq_readclosure,
	(void *)sq_malloc,
	(void *)sq_realloc,
	(void *)sq_free,
	(void *)sq_stackinfos,
	(void *)sq_setdebughook,
	(void *)CLogFile::Printf
};

CModule::CModule( String strName )
{
	// Store the module name
	m_strName = strName;

	// Reset library
	m_pLibrary = NULL;

	// Remove any illegal characters from the module name
	SharedUtility::RemoveIllegalCharacters( strName );

	// Get the module path string
	String strModulePath;
	strModulePath.Format( "modules/%s%s", strName.Get(), LIB_EXTENSION );

	// Does the module not exists?
	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (File doesn't exist)", strName.Get(), LIB_EXTENSION );
		return;
	}

	// Create the libray instance
	m_pLibrary = new CLibrary();

	// Is the library instance invalid?
	if( !m_pLibrary )
		return;

	// Did the module fail to load?
	if( !m_pLibrary->Load( strModulePath.Get() ) )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (Failed to load)", strName.Get(), LIB_EXTENSION );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );

		return;
	}

	// Get the module function pointers
	m_moduleFunctions.pfnSetupSquirrel = (SetupSquirrel_t)m_pLibrary->GetProcedureAddress( "SetupSquirrel" );
	m_moduleFunctions.pfnSetupCore = (SetupCore_t)m_pLibrary->GetProcedureAddress( "SetupCore" );
	m_moduleFunctions.pfnInitialiseModule = (InitialiseModule_t)m_pLibrary->GetProcedureAddress( "ModuleInitialise" );
	m_moduleFunctions.pfnScriptLoad = (ScriptLoad_t)m_pLibrary->GetProcedureAddress( "OnScriptLoad" );
	m_moduleFunctions.pfnScriptUnload = (ScriptUnload_t)m_pLibrary->GetProcedureAddress( "OnScriptUnload" );
	m_moduleFunctions.pfnPulse = (Pulse_t)m_pLibrary->GetProcedureAddress( "Pulse" );

	// Are the pointers invalid?
	if( !IsValid() )
	{
		CLogFile::Printf( "Failed to load module '%s%s'. (Unable to get module procedures)", strName.Get(), LIB_EXTENSION );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );

		return;
	}

	// Setup the functions
	m_moduleFunctions.pfnSetupSquirrel( SquirrelInterfaceContainer );

	// Setup the core interface with the module
	m_moduleFunctions.pfnSetupCore( (void *)CCore::Instance() );

	// Initialise the module
	if( m_moduleFunctions.pfnInitialiseModule() )
		CLogFile::Printf( "Loaded module '%s%s'.", strName.Get(), LIB_EXTENSION );
}

CModule::~CModule( void )
{
	// Is the library instance valid?
	if( m_pLibrary )
	{
		// Unload the library
		m_pLibrary->Unload( );

		// Delete the library instance
		SAFE_DELETE( m_pLibrary );
	}
}

bool CModule::IsValid( void )
{
	// Is the library instance invalid?
	if( !m_pLibrary )
		return false;

	// Are the library function pointers invalid?
	if( !m_moduleFunctions.pfnSetupSquirrel || !m_moduleFunctions.pfnInitialiseModule )
		return false;

	return true;
}

void CModule::ScriptLoad( HSQUIRRELVM pVM )
{
	// Is the library instance invalid or script load pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnScriptLoad )
		return;

	m_moduleFunctions.pfnScriptLoad( pVM );
}

void CModule::ScriptUnload( HSQUIRRELVM pVM )
{
	// Is the library instance invalid or script unload pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnScriptUnload )
		return;

	m_moduleFunctions.pfnScriptUnload( pVM );
}

void CModule::Pulse( void )
{
	// Is the library instance invalid or pulse pointer invalid?
	if( !m_pLibrary || !m_moduleFunctions.pfnPulse )
		return;

	m_moduleFunctions.pfnPulse( );
}
