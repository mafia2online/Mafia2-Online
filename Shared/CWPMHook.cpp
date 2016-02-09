/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CWPMHook.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include "BaseInc.h"

#include <Windows.h>

#include	"CString.h"

#include "../Client/CCore.h"

#include "CPatcher.h"

#include "CWPMHook.h"
#else
// todo
#endif

#define MSG_ANTICHEAT_RPM         0x1336
#define MSG_ANTICHEAT_WPM         0x1337

extern	CCore		* pCore;
bool CWPMHook::m_bInstalled = false;

// typedefs
typedef BOOL ( WINAPI * ReadProcessMemory_t ) ( HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead );
typedef BOOL ( WINAPI * WriteProcessMemory_t ) ( HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten );

// Hook functions
ReadProcessMemory_t		pfnReadProcessMemory = NULL;
WriteProcessMemory_t	pfnWriteProcessMemory = NULL;

// ReadProcessMemory hook
BOOL WINAPI HOOK_ReadProcessMemory ( HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead )
{
	pCore->HandleAntiCheatEvent( MSG_ANTICHEAT_RPM, (unsigned int)lpBaseAddress, nSize );
	return pfnReadProcessMemory( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead );
}

// WriteProcessMemory hook
BOOL WINAPI HOOK_WriteProcessMemory ( HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead )
{
	pCore->HandleAntiCheatEvent( MSG_ANTICHEAT_WPM, (unsigned int)lpBaseAddress, nSize );
	return pfnWriteProcessMemory( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead );
}

bool CWPMHook::Install( void )
{
	if( m_bInstalled )
		return false;

	pfnReadProcessMemory = ( ReadProcessMemory_t ) CPatcher::InstallDetourPatch( "Kernel32.dll", "ReadProcessMemory", (DWORD)HOOK_ReadProcessMemory );
	pfnWriteProcessMemory = ( WriteProcessMemory_t ) CPatcher::InstallDetourPatch( "Kernel32.dll", "WriteProcessMemory", (DWORD)HOOK_WriteProcessMemory );

	if( pfnReadProcessMemory && pfnWriteProcessMemory )
		return true;

	return false;
}

bool CWPMHook::Uninstall( void )
{
	if( !m_bInstalled )
		return false;

	bool bResult = false;

	bResult &= (bool)CPatcher::UninstallDetourPatch( pfnReadProcessMemory, (DWORD)HOOK_ReadProcessMemory );
	bResult &= (bool)CPatcher::UninstallDetourPatch( pfnWriteProcessMemory, (DWORD)HOOK_WriteProcessMemory );

	return bResult;
}