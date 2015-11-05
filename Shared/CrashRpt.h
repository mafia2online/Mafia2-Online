/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CrashRpt.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#include <CrashRpt1401.h>

namespace CrashRpt
{
	bool AddProp					( char * pszName, char * pszValue );
	bool AddPropf					( char * pszName, char * pszFmt, ... );

	bool AddFile					( char * pszDesc, char * pszDestFile, char * pszFile );					
	bool AddFilef					( char * pszDesc, char * pszDestFile, char * pszFmt, ... );	

	bool InstallMain				( char * pszName, char * pszVersion );
	bool UninstallMain				( void );

	bool InstallToCurrentThread		( void );
	bool UninstallFromCurrentThread	( void );

	bool TestEmulateCrash			( int nType );
	bool TestGenerateReport			( BOOL bManual );
}