/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _DEBUG
#define CORE_MODULE	"m2online.dll"
#else
#define	CORE_MODULE	"m2online.dll" 
#endif

#define BASS_MODULE "bass.dll"

CUpdate * pUpdater = NULL;
CGUI * pGUI = NULL;

int ShowMessageBox( const char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK) )
{
	return MessageBox( NULL, szText, MOD_NAME, uType );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	#ifndef _DEBUG
	if ( IsDebuggerPresent () )
	{
		ExitProcess ( -1 );
	}
	#endif

	pGUI = new CGUI;
	bool bFoundCustomDirectory = false;
	char szInstallDirectory[ MAX_PATH ];

	if( !SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", NULL, szInstallDirectory, sizeof(szInstallDirectory) ) )
	{
		if( ShowMessageBox( "Failed to find Mafia II install directory. Do you want to select it now?", (MB_ICONEXCLAMATION | MB_YESNO) ) == IDYES )
		{
			BROWSEINFO browseInfo = {0};
			browseInfo.lpszTitle = "Select your Mafia II directory";
			ITEMIDLIST * pItemIdList = SHBrowseForFolder( &browseInfo );

			// Did they finish looking for a folder?
			if( pItemIdList != NULL )
			{
				// Get the name of the selected folder
				if( SHGetPathFromIDList( pItemIdList, szInstallDirectory ) )
					bFoundCustomDirectory = true;

				// Was any memory used?
				IMalloc * pIMalloc = NULL;
				if( SUCCEEDED( SHGetMalloc( &pIMalloc ) ) )
				{
					pIMalloc->Free( pItemIdList );
					pIMalloc->Release();
				}
			}
			if( !bFoundCustomDirectory )
			{
				ShowMessageBox( "Failed to find Mafia II install directory. Can't launch." );
				return 1;
			}
		}
	}

	String strLaunchPath( "%s\\pc", szInstallDirectory );
	String strApplicationPath( "%s\\Mafia2.exe", strLaunchPath.Get() );

	if( !SharedUtility::Exists( strApplicationPath.Get() ) )
	{
		ShowMessageBox( "Failed to find Mafia2.exe. Can't launch." );
		return 1;
	}

	if( bFoundCustomDirectory )
		SharedUtility::WriteRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", szInstallDirectory, sizeof(szInstallDirectory) );

	String strModulePath( "%s\\%s", SharedUtility::GetAppPath(), CORE_MODULE );
	CLogFile::Printf("M2Online : %s", strModulePath.Get());

	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		ShowMessageBox( "Failed to find m2online.dll ! Can't launch." );
		return 1;
	}

	String strBassPath("%s\\%s", SharedUtility::GetAppPath(), BASS_MODULE);
	CLogFile::Printf("Bass : %s", strBassPath.Get());

	if (!SharedUtility::Exists(strBassPath.Get()))
	{
		ShowMessageBox("Failed to find bass.dll ! Can't launch.");
		return 1;
	}


	if (SharedUtility::IsProcessRunning("Mafia2.exe")){
		if (SharedUtility::_TerminateProcess("Mafia2.exe") == false)
		{
			ShowMessageBox("Failed to kill Mafia.exe. Cannot launch.");
			return 1;
		}
	}

	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset( &siStartupInfo, 0, sizeof(siStartupInfo) );
	memset( &piProcessInfo, 0, sizeof(piProcessInfo) );
	siStartupInfo.cb = sizeof(siStartupInfo);

	if( !CreateProcess( strApplicationPath.Get(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo ) )
	{
		ShowMessageBox( "Failed to start Mafia2.exe. Can't launch." );
		return 1;
	}

	int iReturn = SharedUtility::InjectLibraryIntoProcess( piProcessInfo.hProcess, strModulePath.Get() );
	iReturn += SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, strBassPath.Get());

	if( iReturn > 0)
	{
		TerminateProcess( piProcessInfo.hProcess, 0 );

		String strError( "Unknown Error. Can't launch." );
		if( iReturn == 1 )
			strError = "[M2O] : Failed to write library path into remote process. Can't launch.";
		else if( iReturn == 2 )
			strError = "[M2O] : Failed to create remote thread in remote process. Can't launch.";
		else if( iReturn == 2 )
			strError = "[M2O] : Failed to open the remote process. Can't launch.";
		ShowMessageBox( strError.Get() );
		if (SharedUtility::IsProcessRunning("Mafia2.exe")){
			SharedUtility::_TerminateProcess("Mafia2.exe");
		}
		return 1;
	}
	else {
		CLogFile::Printf("iReturn : %d", iReturn);
	}

	ResumeThread( piProcessInfo.hThread );

	return 0;
}