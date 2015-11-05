#include "DbgView.h"

void DbgView::PrintA( const char * pszTag, char * pszFmt, va_list& VAList )
{
	static char szBuffer[DV_MAX_BUFLEN];
	ZeroMemory( szBuffer, DV_MAX_BUFLEN );

	_vsnprintf_s( szBuffer, sizeof( szBuffer ) - strlen( szBuffer ), pszFmt, VAList );
	//_vsnprintf( szBuffer + strlen( szBuffer ), sizeof( szBuffer ) - strlen( szBuffer ), pszFmt, VAList );

	sprintf_s( szBuffer, "%s (%s)\n", szBuffer, pszTag );
	//sprintf_s( szBuffer, "%s (%s)\n", szBuffer, IsDebuggerPresent( ) ? "dbg" : pszTag );

	OutputDebugStringA( szBuffer );
}

void DbgView::E( char * pszFmt, ... )
{
	if ( !pszFmt ) return;

	va_list VAList;
	va_start( VAList, pszFmt );

	PrintA( "ERR", pszFmt, VAList );

	va_end( VAList );
}

void DbgView::W( char * pszFmt, ... )
{
	if ( !pszFmt ) return;

	va_list VAList;
	va_start( VAList, pszFmt );

	PrintA( "WARN", pszFmt, VAList );

	va_end( VAList );
}

void DbgView::I( char * pszFmt, ... )
{
	if ( !pszFmt ) return;

	va_list VAList;
	va_start( VAList, pszFmt );

	PrintA( "INFO", pszFmt, VAList );

	va_end( VAList );
}

void DbgView::T( char * pszFmt, ... )
{
	if ( !pszFmt ) return;

	va_list VAList;
	va_start( VAList, pszFmt );

	PrintA( "TEXT", pszFmt, VAList );

	va_end( VAList );
}

void DbgView::N( char * pszFmt, ... )
{
	if ( !pszFmt ) return;

	va_list VAList;
	va_start( VAList, pszFmt );

	PrintA( "NOTE", pszFmt, VAList );

	va_end( VAList );
}