/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CExceptionHandler.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

typedef void (* ExceptionHandlerCallback_t)( FILE * fFile );

class CExceptionHandler
{

private:

	static ExceptionHandlerCallback_t	m_pfnCallback;

#ifdef _WIN32
	static	long WINAPI					ExceptionHandler( _EXCEPTION_POINTERS * ExceptionInfo );
#else
	static	void						ExceptionHandler( int iSignal );
#endif

public:

	static	void						Install( void );
	static	void						SetCallback( ExceptionHandlerCallback_t pfnCallback ) { m_pfnCallback = pfnCallback; }

};