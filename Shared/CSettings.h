/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSettings.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <map>
#include <list>
#include "CCommon.h"
#include "CString.h"
#include "CXML.h"

// CVAR Macros
#define	CVAR_SET					CSettings::Set
#define	CVAR_GET					CSettings::Get
#define	CVAR_GET_STRING				CSettings::GetString
#define	CVAR_GET_BOOL				CSettings::GetBool
#define	CVAR_GET_INTEGER			CSettings::GetInteger
#define	CVAR_GET_UINT				Csettings::GetUnsingedInt
#define	CVAR_GET_FLOAT				CSettings::GetFloat
#define	CVAR_GET_LIST				CSettings::GetList

class CSettings
{

private:

	static	CXML										* m_pXML;
	static	CXMLNode									* m_pRootNode;
	static	bool										m_bOpen;

public:

	static	bool					Load				( const char * szFileName );
	static	bool					Close				( void );
	static	bool					Save				( void );

	static	void					LoadDefaults		( void );
	static	bool					Exists				( const char * szName );

	static	void					ParseCommandLine	( int argc, char ** argv );
	static	void					ParseCommandLine	( char * szCommandLine );

	static	void					Set					( const char * szName, const char * szValue );
	static	void					Set					( const char * szName, bool bValue );
	static	void					Set					( const char * szName, int iValue );
	static	void					Set					( const char * szName, unsigned int uiValue );
	static	void					Set					( const char * szName, float fValue );
	static	void					Set					( const char * szNode, const char * szSubNode, std::list< const char* > list );

	static	void					Get					( const char * szName, const char ** szValue );
	static	void					Get					( const char * szName, bool * bValue );
	static	void					Get					( const char * szName, int * iValue );
	static	void					Get					( const char * szName, unsigned int * uiValue );
	static	void					Get					( const char * szName, float * fValue );
	static	void					Get					( const char * szNode, const char * szSubNode, std::list< const char* > * list );

	static	const char				* GetString			( const char * szName ) { const char * sz; Get( szName, &sz ); return sz; }
	static	bool					GetBool				( const char * szName ) { bool b; Get( szName, &b ); return b; }
	static	int						GetInteger			( const char * szName ) { int i; Get( szName, &i ); return i; }
	static	unsigned int			GetUnsignedInt		( const char * szName ) { unsigned int ui; Get( szName, &ui ); return ui; }
	static	float					GetFloat			( const char * szName ) { float f; Get( szName, &f ); return f; }
	static	std::list< const char* > GetList			( const char * szNode, const char * szSubNode ) { std::list< const char* > list; Get( szNode, szSubNode, &list ); return list; }

};