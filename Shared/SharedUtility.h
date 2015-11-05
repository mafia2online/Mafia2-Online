/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : SharedUtility.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include	<windows.h>
#else
#include	<sys/time.h>
#endif

#undef min
#undef max

typedef unsigned long DWORD;

#include	<string>
#include	"CCommon.h"
#include	"CString.h"

namespace SharedUtility
{
	//
    // Some templates
    //
    template < class T >
    T Min ( const T& a, const T& b )
    {
        return a < b ? a : b;
    }

    template < class T >
    T Max ( const T& a, const T& b )
    {
        return a > b ? a : b;
    }
	
	bool				StripPath1( char * szString );
	char *				StripPath2( char * szString );

	const char			* GetAppPath( void );
	const char			* GetExePath( void );

	unsigned long		GetTime( void );

	bool				Exists( const char * szPath );

	const char			* GetTimeString( void );
	double				GetSecondCount( void );

	String				GetAbsolutePath( const char * szFormat, ... );
	String				DiconnectReasonToString( unsigned int uiReason );

	String				GetFileNameForScriptFile( const char * szName, const char * szScriptPath, const char * szHost = "", unsigned long ulPort = 0 );

#ifdef _CLIENT
	DWORD				GetSerial( void );
	String				GetSerialHash( void );
	String				GetClientScriptFolder( String strHost, unsigned long ulPort );
#endif

	int					CreateDirectory( const char * szPath );
	bool				CopyFile( const char * szSource, const char * szDestination );

	void				FatalExit( String strError );

	String				RemoveIllegalCharacters( String& strString );
	String				HideCharacters( String strString );

	String				StripForWebRequest( String strUrl );

	String				GetTimePassedFromTime( unsigned long ulTime );

#ifdef _WIN32
	int					AnsiToUnicode( const char * szAnsiString, size_t sAnsiStringLength, wchar_t * wszUnicodeString, size_t sUnicodeStringLength, DWORD dwFlags = NULL );
	int					UnicodeToAnsi( const wchar_t * wszUnicodeString, size_t sUnicodeStringLength, char * szAnsiString, size_t sAnsiStringLength, DWORD dwFlags = NULL );

	bool				SetClipboardText( const char * szString, size_t sStringSize );
	const char			* GetClipboardText( void );

	int					InjectLibraryIntoProcess( HANDLE hProcess, const char * szLibraryPath );
	int					InjectLibraryIntoProcess( DWORD dwProcessId, const char * szLibraryPath );

	bool				GetProcessIdFromProcessName( char * szProcessName, DWORD * dwProcessId );
	bool				IsProcessRunning( char * szProcessName );
	bool				_TerminateProcess( char * szProcessName );

	bool				ReadRegistryString( HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize );
	bool				WriteRegistryString( HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize );
#endif

#ifndef _LAUNCHER
	bool				CompileScript( const char * szFile, const char * szFileOut );
#endif

	const char			* MonthToString( int iMonth );
	const char			* DateToString();
	String				BytesToString( unsigned int uiBytes );
	String				str_rand ( unsigned int uiLength );
	int					int_concat ( int a, int b );
	bool				IsValidIP ( const char * szIpAddress );
};
