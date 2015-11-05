/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CExceptionHandler.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../Client/StdInc.h"
#else
#include	"../Server/StdInc.h"
#endif

ExceptionHandlerCallback_t CExceptionHandler::m_pfnCallback = NULL;

#ifdef _CLIENT
long WINAPI CExceptionHandler::ExceptionHandler( _EXCEPTION_POINTERS * ExceptionInfo )
#else
void CExceptionHandler::ExceptionHandler( int iSignal )
#endif
{
	//
	CLogFile::Printf( "EXCEPTION HANDLER CALLED" );

	// Exit the process
#ifndef WIN32
	exit( 0 );
#endif

	// Terminate mafia2
	SharedUtility::_TerminateProcess( "Mafia2.exe" );

	//
	return EXCEPTION_EXECUTE_HANDLER;
}

void CExceptionHandler::Install( void )
{
	// Set the exception handler filter
#ifdef _WIN32
	SetUnhandledExceptionFilter( ExceptionHandler );
#else
	signal( SIGSEGV, ExceptionHandler );
#endif
}