/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGameFiles.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define		FILE_NOT_FOUND			1

class String;

class CGameFiles
{

private:

	static	String					m_strError;
	static	std::list< String >		m_deleteFiles;

public:

	static	bool					CheckFiles( void );

	static	void					CleanFiles( void );

	static	void					SetLastError( String strError );
	static	String					GetLastError( void );

};
