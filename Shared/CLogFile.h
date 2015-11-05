/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CLogFile.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<stdio.h>
#include	"Threading/CMutex.h"

class CLogFile
{
private:
	static FILE		* m_fLogFile;
	static bool		m_bLogTime;
	static CMutex	m_mutex;

public:
	static	void	Open( const char * szLogFile, bool bAppend = false );
	static	void	Print( const char * szString );
	static	void	Printf( const char * szFormat, ... );
	static	void	PrintToFile( const char * szString );
	static	void	PrintfToFile( const char * szFormat, ... );
	static	void	Close( void );
	static	void	TimeStamp( bool bEnable );
};