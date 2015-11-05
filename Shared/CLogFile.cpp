/*****************************************************************************
*
*		PROJECT:		Mafia 2 Multiplayer v0.1
*		FILE:			Utils/CLogFile.cpp
*		DEVELOPERS:		AaronLad
*
*		www.m2-multiplayer.com
*
*****************************************************************************/

#include	"CLogFile.h"
#include	"SharedUtility.h"

#ifdef _WIN32
#include	<windows.h>
#include	<direct.h>
#else
#include	<stdarg.h>
#endif

#include	<errno.h>

FILE 		* CLogFile::m_fLogFile = NULL;
bool 		CLogFile::m_bLogTime = true;
CMutex		CLogFile::m_mutex;

void CLogFile::Open( const char * szLogFile, bool bAppend )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Open the log file
	m_fLogFile = fopen( szLogFile, bAppend ? "a" : "w" );

	// Did the log file open?
	//if( m_fLogFile )
	//	PrintToFile( "Log file started." );

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::Print( const char * szString )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Print the message
	if( m_bLogTime )
		printf( "[%s] %s\n", SharedUtility::GetTimeString(), szString );
	else
		printf( "%s\n", szString );

	// Flush the output buffer
	fflush( stdout );

	// Print the message to the log file
	PrintToFile( szString );

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::Printf( const char * szFormat, ... )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Collect the arguments
	va_list vaArgs;
	char szBuffer[1024];
	va_start(vaArgs, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
	va_end(vaArgs);

	// Print the message
	Print( szBuffer );

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::PrintToFile( const char * szString )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Is the log file open?
	if( m_fLogFile )
	{
		// Log the message
		if( m_bLogTime )
			fprintf( m_fLogFile, "[%s] %s\n", SharedUtility::GetTimeString(), szString );
		else
			fprintf( m_fLogFile, "%s\n", szString );

		// Flush the log file buffer
		fflush( m_fLogFile );
	}

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::PrintfToFile( const char * szFormat, ... )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Collect the arguments
	va_list vaArgs;
	char szBuffer[1024];
	va_start(vaArgs, szFormat);
	vsnprintf(szBuffer, sizeof(szBuffer), szFormat, vaArgs);
	va_end(vaArgs);

	// Print the message to the log file
	PrintToFile( szBuffer );

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::Close( void )
{
	// Lock the mutex
	m_mutex.TryLock( 1 );

	// Is the log file open?
	if( m_fLogFile )
	{
		// Log the file closing message
		//PrintToFile( "Log file closing." );

		// Close the log file
		fclose( m_fLogFile );
		m_fLogFile = NULL;
	}

	// Unlock the mutex
	m_mutex.Unlock( );
}

void CLogFile::TimeStamp( bool bEnable )
{
	m_bLogTime = bEnable;
}