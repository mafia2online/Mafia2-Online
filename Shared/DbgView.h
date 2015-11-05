/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CLogFile.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#include <Windows.h>
#include <iostream>

#define DV_MAX_BUFLEN 512

namespace DbgView
{
	void PrintA( const char * pszTag, char * pszFmt, va_list& VAList );
	void PrintW( const char * pszTag, wchar_t * pszFmt, va_list& VAList );

	void E( char * pszFmt, ... );
	void W( char * pszFmt, ... );
	void I( char * pszFmt, ... );
	void T( char * pszFmt, ... );
	void N( char * pszFmt, ... );
}