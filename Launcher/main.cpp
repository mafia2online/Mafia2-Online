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
#define CORE_MODULE	"m2mp_d.dll"
#else
#define	CORE_MODULE	"m2mp.dll"
#endif

CUpdate * pUpdater = NULL;
CGUI * pGUI = NULL;

int ShowMessageBox( const char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK) )
{
	return MessageBox( NULL, szText, MOD_NAME, uType );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	#ifndef _DEBUG
	// Is there a debugger present?
	if ( IsDebuggerPresent () )
	{
		// Exit
		ExitProcess ( -1 );
	}
	#endif

	// Create the gui instance
	pGUI = new CGUI;

#ifndef _DEBUG
	// Create the updater instance
	/*pUpdater = new CUpdate;
	
	// Check for updates
	pUpdater->CheckForUpdates();*/
#endif

	//
	bool bFoundCustomDirectory = false;
	char szInstallDirectory[ MAX_PATH ];

	// Try get the custom directory
	if( !SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "GameDir", NULL, szInstallDirectory, sizeof(szInstallDirectory) ) )
	{
		// Ask them to find their own directory
		if( ShowMessageBox( "Failed to find Mafia II install directory. Do you want to select it now?", (MB_ICONEXCLAMATION | MB_YESNO) ) == IDYES )
		{
			// Construct the browse info
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
					// Free the memory
					pIMalloc->Free( pItemIdList );

					// Release the malloc
					pIMalloc->Release();
				}
			}

			// Did they not find the registry?
			if( !bFoundCustomDirectory )
			{
				ShowMessageBox( "Failed to find Mafia II install directory. Can't launch "MOD_NAME"." );
				return 1;
			}
		}
	}

	// Get the launch path string
	String strLaunchPath( "%s\\pc", szInstallDirectory );

	// Get the full path to Mafia2.exe
	String strApplicationPath( "%s\\Mafia2.exe", strLaunchPath.Get() );

	// Does Mafia2.exe not exist?
	if( !SharedUtility::Exists( strApplicationPath.Get() ) )
	{
		ShowMessageBox( "Failed to find Mafia2.exe. Can't launch "MOD_NAME"." );
		return 1;
	}

	// If we have a custom directory, save it!
	if( bFoundCustomDirectory )
		SharedUtility::WriteRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "GameDir", szInstallDirectory, sizeof(szInstallDirectory) );

	// Get the full path to m2mp.dll
	String strModulePath( "%s\\%s", SharedUtility::GetAppPath(), CORE_MODULE );

	// Does m2mp.dll not exist?
	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		ShowMessageBox( "Failed to find "CORE_MODULE". Can't launch "MOD_NAME"." );
		return 1;
	}

	// Terminate Mafia II process if it's already running?
	if( SharedUtility::IsProcessRunning( "Mafia2.exe" ) )
		SharedUtility::_TerminateProcess( "Mafia2.exe" );

	// Create the startup info struct
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset( &siStartupInfo, 0, sizeof(siStartupInfo) );
	memset( &piProcessInfo, 0, sizeof(piProcessInfo) );
	siStartupInfo.cb = sizeof(siStartupInfo);

	// Create the Mafia II process
	if( !CreateProcess( strApplicationPath.Get(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo ) )
	{
		ShowMessageBox( "Failed to start Mafia2.exe. Can't launch "MOD_NAME"." );
		return 1;
	}

	// Inject m2mp.dll into Mafia2.exe
	int iReturn = SharedUtility::InjectLibraryIntoProcess( piProcessInfo.hProcess, strModulePath.Get() );

	// Did m2mp.dll fail to inject?
	if( iReturn > 0 )
	{
		// Terminate Mafia2.exe
		TerminateProcess( piProcessInfo.hProcess, 0 );

		// Generate the error string
		String strError( "Unknown Error. Can't launch "MOD_NAME"." );

		// Find the cause of the error
		if( iReturn == 1 )
			strError = "Failed to write library path into remote process. Can't launch "MOD_NAME".";
		else if( iReturn == 2 )
			strError = "Failed to create remote thread in remote process. Can't launch "MOD_NAME".";
		else if( iReturn == 2 )
			strError = "Failed to open the remote process. Can't launch "MOD_NAME".";

		// Show the error message
		ShowMessageBox( strError.Get() );
		return 1;
	}

	// Resume Mafia2.exe thread
	ResumeThread( piProcessInfo.hThread );

	return 0;
}