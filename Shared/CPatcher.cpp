/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CPatcher.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CPatcher.h"
#include	"CLogFile.h"
#include	"../Libraries/Detours/detours.h"

ProtectionInfo CPatcher::Unprotect(DWORD dwAddress, int iSize)
{
	ProtectionInfo pInfo;
	pInfo.dwAddress = dwAddress;
	pInfo.iSize = iSize;
	VirtualProtect((void *)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &pInfo.dwOldProtection);
	return pInfo;
}

void CPatcher::Reprotect(ProtectionInfo pInfo)
{
	DWORD dwProtection;
	VirtualProtect((void *)pInfo.dwAddress, pInfo.iSize, pInfo.dwOldProtection, &dwProtection);
}

void * CPatcher::InstallDetourPatchInternal(DWORD dwAddress, DWORD dwDetourAddress, BYTE byteType, int iSize)
{
	BYTE * pbyteAddr = (BYTE *)dwAddress;

	// Allocate the trampoline memory
	BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);

	// Unprotect the trampoline memory
	Unprotect((DWORD)pbyteTrampoline, (iSize + 5));

	// Unprotect the address memory
	ProtectionInfo pInfo = Unprotect((DWORD)pbyteAddr, (iSize + 5));

	// Copy the overwritten address memory to the trampoline
	memcpy(pbyteTrampoline, pbyteAddr, iSize);

	// Write the type to the trampoline memory
	pbyteTrampoline[iSize] = byteType;
	
	// Write the detour to the trampoline memory
	*(void **)(&pbyteTrampoline[iSize + 1]) = (void *)((pbyteAddr + iSize) - (pbyteTrampoline + iSize) - 5);

	// Write the type to the address memory
	pbyteAddr[0] = byteType;

	// Write the detour to the address memory
	*(void **)(&pbyteAddr[1]) = (void *)((BYTE *)dwDetourAddress - pbyteAddr - 5);

	// Re-protect the address memory
	Reprotect(pInfo);

	return pbyteTrampoline;
}

void CPatcher::UninstallDetourPatchInternal(DWORD dwAddress, void ** pTrampoline, int iSize)
{
	BYTE * pTramp = (BYTE *)*pTrampoline;
	BYTE * pbyteAddr = (BYTE *)dwAddress;

	// Unprotect the address memory
	ProtectionInfo pInfo = Unprotect((DWORD)pbyteAddr, iSize);

	// Copy the trampoline to the address
	memcpy(pbyteAddr, pTramp, iSize);

	// Re-protect the address memory
	Reprotect(pInfo);

	// Free trampoline
	free(pTramp);

	// Set the trampoline to the address
	*pTrampoline = pbyteAddr;
}

void * CPatcher::InstallDetourPatch( char * szLibrary, char * szFunction, DWORD dwFunctionAddress )
{
	return DetourFunction( DetourFindFunction( szLibrary, szFunction ), (BYTE *)dwFunctionAddress );
}

void * CPatcher::InstallDetourPatch( DWORD dwAddress, DWORD dwFunctionAddress )
{
	return DetourFunction( (BYTE *)dwAddress, (BYTE *)dwFunctionAddress );
}

BOOL CPatcher::UninstallDetourPatch(void * pTrampoline, DWORD dwFunctionAddress)
{
	return DetourRemove((BYTE *)pTrampoline, (BYTE *)dwFunctionAddress);
}

void CPatcher::PatchAddress(DWORD dwAddress, BYTE *bPatch, size_t iSize)
{
	DWORD d, ds;

	VirtualProtect( ( void* )dwAddress, iSize, PAGE_EXECUTE_READWRITE, &d );
	memcpy( ( void* )dwAddress, bPatch, iSize );
	VirtualProtect( ( void* )dwAddress, iSize, d, &ds );
}

void CPatcher::PatchAddress( DWORD dwAddress, DWORD dwPatch )
{
	DWORD d, ds;

	VirtualProtect( ( void* )dwAddress, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &d );
	*(DWORD *)(dwAddress) = dwPatch;
	VirtualProtect( ( void* )dwAddress, sizeof(DWORD), d, &ds );
}

void * CPatcher::InstallCallPatch( DWORD dwAddress, DWORD dwCallAddress, int iSize )
{
	return InstallDetourPatchInternal( dwAddress, dwCallAddress, X86_CALL, iSize );
}

void * CPatcher::InstallJmpPatch( DWORD dwAddress, DWORD dwJmpAddress, int iSize )
{
	return InstallDetourPatchInternal( dwAddress, dwJmpAddress, X86_JMP, iSize );
}

void CPatcher::InstallNopPatch( DWORD dwAddress, int iSize )
{
	DWORD dwAddr = dwAddress;

	// Unprotect the address memory
	ProtectionInfo protectionInfo = Unprotect( dwAddr, iSize );

	// Write the no operation to the address memory
	memset( (void *)dwAddr, X86_NOP, iSize );

	// Re-protect the address memory
	Reprotect( protectionInfo );
}

bool CPatcher::bDataCompare( const unsigned char * pData, const unsigned char * bMask, const char * szMask )
{
    for( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if( *szMask == 'x' && *pData != *bMask )
		{
			return false;
		}
	}

	return (*szMask) == 0;
}

unsigned long CPatcher::FindPattern2( DWORD dwAddress, DWORD dwLen, unsigned char * bMask, char * szMask )
{
    for( unsigned long i = 0; i < dwLen; i++ )
	{
		if( bDataCompare( (unsigned char *)( dwAddress + i ), bMask, szMask ) )
		{
			return (unsigned long)( dwAddress + i );
		}
	}

	return 0;
}

unsigned long CPatcher::FindPattern( unsigned char * bMask, char * szMask )
{
	return FindPattern2( (DWORD)GetModuleHandle( NULL ), 0xFFFFFFFF, bMask, szMask );
}

int CPatcher::ToInteger( String strString )
{
	return atoi( strString.Get() );
}

void CPatcher::DumpVFTable( DWORD dwAddress, int iFunctionCount )
{
	CLogFile::Printf( "Dumping Virtual Function Table at 0x%p...", dwAddress );
	for( int i = 0; i < iFunctionCount; i++ )
	{
		CLogFile::Printf( "VFTable Offset: %d, Function: 0x%p (At Address: 0x%p)", (i * 4), *(PDWORD)(dwAddress + (i * 4)), (dwAddress + (i * 4)) );
	}
}