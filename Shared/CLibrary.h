/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CLibrary.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CLibrary
{

private:

	void						* m_pLibrary;

public:

	CLibrary( void );
	~CLibrary( void );

	bool						Load( const char * szPath );
	bool						Unload( void );
	void						* GetProcedureAddress( const char * szProcedureName );
	void						* GetProcedureAddress( unsigned int uiOrdinal );

};