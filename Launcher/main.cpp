/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : main.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

#include "CLogFile.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _DEBUG
#define CORE_MODULE	"m2online.dll"
#else
#define	CORE_MODULE	"m2online.dll"
#endif

#define BASS_MODULE "bass.dll"

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

	String strLaunchPath;
	strLaunchPath.Format( "%s\\pc", szInstallDirectory );
	String strApplicationPath;
	strApplicationPath.Format( "%s\\Mafia2.exe", strLaunchPath.Get() );

	if( !SharedUtility::Exists( strApplicationPath.Get() ) )
	{
		ShowMessageBox( "Failed to find Mafia2.exe. Can't launch." );
		return 1;
	}

	if( bFoundCustomDirectory )
		SharedUtility::WriteRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", szInstallDirectory, sizeof(szInstallDirectory) );

	String strModulePath;
	strModulePath.Format( "%s%s", SharedUtility::GetAppPath(), CORE_MODULE );
	CLogFile::Printf("M2Online: %s", strModulePath.Get());

	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		ShowMessageBox( "Failed to find m2online.dll! Can't launch." );
		return 1;
	}

	String strBassPath;
	strBassPath.Format("%s%s", SharedUtility::GetAppPath(), BASS_MODULE);
	CLogFile::Printf("Bass: %s", strBassPath.Get());

	if (!SharedUtility::Exists(strBassPath.Get()))
	{
		ShowMessageBox("Failed to find bass.dll! Can't launch.");
		return 1;
	}


	if (SharedUtility::IsProcessRunning("Mafia2.exe"))
	{
		if (! SharedUtility::_TerminateProcess("Mafia2.exe"))
		{
			ShowMessageBox("Failed to kill Mafia 2 process. Cannot launch.");
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

	const SharedUtility::InjectLibraryResults bassInjectResult = SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, strBassPath.Get());
	const SharedUtility::InjectLibraryResults moduleInjectResult = SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, strModulePath.Get() );

	if ( moduleInjectResult != SharedUtility::INJECT_LIBRARY_RESULT_OK || bassInjectResult != SharedUtility::INJECT_LIBRARY_RESULT_OK )
	{
		String strError;
		strError.Format( "Failed to inject modules into game process.\nCore: %s (%i)\nBass:  %s (%i)", SharedUtility::InjectLibraryResultToString(moduleInjectResult), moduleInjectResult, SharedUtility::InjectLibraryResultToString(bassInjectResult), bassInjectResult);
		ShowMessageBox( strError );

		TerminateProcess( piProcessInfo.hProcess, 0 );
		return 1;
	}

	ResumeThread( piProcessInfo.hThread );

	return 0;
}