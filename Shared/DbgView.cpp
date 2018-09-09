#include "DbgView.h"
#include <assert.h>

const size_t MSG_MAX_LENGTH = 512;
const size_t TAG_MAX_LENGTH = 128;
const size_t TOTAL_MAX_LENGTH = MSG_MAX_LENGTH + TAG_MAX_LENGTH;

void DbgView::PrintA( const char * pszTag, char * pszFmt, va_list& VAList )
{
	assert(pszTag);
	assert(pszFmt);

	static char szBuffer[TOTAL_MAX_LENGTH + 1];
	memset(szBuffer, 0, sizeof(szBuffer));

	vsnprintf (szBuffer, MSG_MAX_LENGTH, pszFmt, VAList);

	static char szTag[TAG_MAX_LENGTH + 1] = { 0 };
	snprintf(szTag, TAG_MAX_LENGTH, " (%s)\n", pszTag);

	strcat(szBuffer, szTag);

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