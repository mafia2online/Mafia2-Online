/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CLibrary.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CLogFile.h"
#include	"CLibrary.h"

#ifdef _WIN32
#include	<windows.h>
#else
#include	<dlfcn.h>
#include	<stdio.h>
#endif

CLibrary::CLibrary( void )
{
	// Reset the library instance
	m_pLibrary = NULL;
}

CLibrary::~CLibrary( void )
{
	// Is the library instance valid?
	if( m_pLibrary )
	{
		// Unload the library
		Unload( );
	}
}

bool CLibrary::Load( const char * szPath )
{
	// Is the library instance valid?
	if( m_pLibrary )
		return false;

	// Load the library
#ifdef _WIN32
	m_pLibrary = LoadLibrary( szPath );
#else
	m_pLibrary = dlopen( szPath, RTLD_LAZY );
#endif

	// Did the library fail to load?
	if( !m_pLibrary )
		return false;

	return true;
}

bool CLibrary::Unload( void )
{
	// Is the library instance invalid?
	if( !m_pLibrary )
		return false;

	// Free the library
#ifdef _WIN32
	FreeLibrary( (HMODULE)m_pLibrary );
#else
	dlclose( m_pLibrary );
#endif

	// Invalidate the library pointer
	m_pLibrary = NULL;

	return true;
}

void * CLibrary::GetProcedureAddress( const char * szProcedureName )
{
	// Is the library invalid?
	if( !m_pLibrary )
		return NULL;

	void * pProcedureAddress = NULL;

#ifdef _WIN32
	pProcedureAddress = (void *)GetProcAddress( (HMODULE)m_pLibrary, szProcedureName );
#else
	pProcedureAddress = dlsym( m_pLibrary, szProcedureName );
#endif

	return pProcedureAddress;
}

void * CLibrary::GetProcedureAddress( unsigned int uiOrdinal )
{
	// TODO
	return NULL;
}