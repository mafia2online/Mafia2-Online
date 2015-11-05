/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CPatcher.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"SharedUtility.h"
#include	"CString.h"

#define	X86_NOP		0x90
#define	X86_RETN	0xC3
#define	X86_CALL	0xE8
#define	X86_JMP		0xE9

typedef unsigned char BYTE;
typedef int BOOL;

struct ProtectionInfo
{
	DWORD dwAddress;
	DWORD dwOldProtection;
	int iSize;
};

class CPatcher
{

public:

	static	ProtectionInfo		Unprotect( DWORD dwAddress, int iSize );
	static	void				Reprotect( ProtectionInfo protectionInfo );

	static	void				* InstallDetourPatchInternal( DWORD dwAddress, DWORD dwDetourAddress, BYTE byteType, int iSize = 5 );
	static	void				UninstallDetourPatchInternal( DWORD dwAddress, void ** pTrampoline, int iSize = 5 );

	static	void				* InstallDetourPatch( char * szLibrary, char * szFunction, DWORD dwFunctionAddress );
	static	void				* InstallDetourPatch( DWORD dwAddress, DWORD dwFunctionAddress );
	static	BOOL				UninstallDetourPatch( void * pTrampoline, DWORD dwFunctionAddress );

	static	void				PatchAddress( DWORD dwAddress, BYTE * bPatch, size_t iSize );
	static	void				PatchAddress( DWORD dwAddress, DWORD dwPatch );

	static	void				* InstallCallPatch( DWORD dwAddress, DWORD dwCallAddress, int iSize = 5 );
	static	void				* InstallJmpPatch( DWORD dwAddress, DWORD dwJmpAddress, int iSize = 5 );
	static	void				InstallNopPatch( DWORD dwAddress, int iSize = 1 );

	static	bool				bDataCompare( const unsigned char * pData, const unsigned char * bMask, const char * szMask );
	static	unsigned long		FindPattern2( DWORD dwAddress, DWORD dwLen, unsigned char * bMask, char * szMask );
	static	unsigned long		FindPattern( unsigned char * bMask, char * szMask );

	static	int					ToInteger( String strString );

	static	void				DumpVFTable( DWORD dwAddress, int iFunctionCount );

};