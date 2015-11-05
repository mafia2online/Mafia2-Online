/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CWPMHook.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include "../Client/StdInc.h"
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
	// Handle this with the core
	pCore->HandleAntiCheatEvent( MSG_ANTICHEAT_RPM, (unsigned int)lpBaseAddress, nSize );

	// Return to the original function
	return pfnReadProcessMemory( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead );
}

// WriteProcessMemory hook
BOOL WINAPI HOOK_WriteProcessMemory ( HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead )
{
	// Handle this with the core
	pCore->HandleAntiCheatEvent( MSG_ANTICHEAT_WPM, (unsigned int)lpBaseAddress, nSize );

	// Return to the original function
	return pfnWriteProcessMemory( hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead );
}

bool CWPMHook::Install( void )
{
	// Are we already installed?
	if( m_bInstalled )
		return false;

	// Hook ReadProcessMemory in Kernel32.dll
	pfnReadProcessMemory = ( ReadProcessMemory_t ) CPatcher::InstallDetourPatch( "Kernel32.dll", "ReadProcessMemory", (DWORD)HOOK_ReadProcessMemory );

	// Hook WriteProcessMemory in Kernel32.dll
	pfnWriteProcessMemory = ( WriteProcessMemory_t ) CPatcher::InstallDetourPatch( "Kernel32.dll", "WriteProcessMemory", (DWORD)HOOK_WriteProcessMemory );

	// Was both hooks installed successfuly?
	if( pfnReadProcessMemory && pfnWriteProcessMemory )
		return true;

	return false;
}

bool CWPMHook::Uninstall( void )
{
	// Are we not already installed?
	if( !m_bInstalled )
		return false;

	bool bResult = false;

	// Uninstall ReadProcessMemory hook
	bResult &= (bool)CPatcher::UninstallDetourPatch( pfnReadProcessMemory, (DWORD)HOOK_ReadProcessMemory );

	// Uninstall WriteProcessMemory hook
	bResult &= (bool)CPatcher::UninstallDetourPatch( pfnWriteProcessMemory, (DWORD)HOOK_WriteProcessMemory );

	return bResult;
}