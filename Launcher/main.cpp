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
	if( !SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", NULL, szInstallDirectory, sizeof(szInstallDirectory) ) )
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
				ShowMessageBox( "Failed to find Mafia II install directory. Can't launch." );
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
		ShowMessageBox( "Failed to find Mafia2.exe. Can't launch." );
		return 1;
	}

	// If we have a custom directory, save it!
	if( bFoundCustomDirectory )
		SharedUtility::WriteRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", szInstallDirectory, sizeof(szInstallDirectory) );

	// Get the full path to m2o.dll
	String strModulePath( "%s\\%s", SharedUtility::GetAppPath(), CORE_MODULE );
	CLogFile::Printf("M2Online : %s", strModulePath.Get());

	// Does m2online.dll not exist?
	if( !SharedUtility::Exists( strModulePath.Get() ) )
	{
		ShowMessageBox( "Failed to find m2online.dll ! Can't launch." );
		return 1;
	}

	String strBassPath("%s\\%s", SharedUtility::GetAppPath(), BASS_MODULE);
	CLogFile::Printf("Bass : %s", strBassPath.Get());

	// Does bass.dll not exist?
	if (!SharedUtility::Exists(strBassPath.Get()))
	{
		ShowMessageBox("Failed to find bass.dll ! Can't launch.");
		return 1;
	}


	// Terminate Mafia II process if it's already running?
	if (SharedUtility::IsProcessRunning("Mafia2.exe")){
		if (SharedUtility::_TerminateProcess("Mafia2.exe") == false)
		{
			ShowMessageBox("Failed to kill Mafia.exe. Cannot launch.");
			return 1;
		}
	}

	// Create the startup info struct
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset( &siStartupInfo, 0, sizeof(siStartupInfo) );
	memset( &piProcessInfo, 0, sizeof(piProcessInfo) );
	siStartupInfo.cb = sizeof(siStartupInfo);

	// Create the Mafia II process
	if( !CreateProcess( strApplicationPath.Get(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo ) )
	{
		ShowMessageBox( "Failed to start Mafia2.exe. Can't launch." );
		return 1;
	}

	// Inject m2o.dll into Mafia2.exe
	int iReturn = SharedUtility::InjectLibraryIntoProcess( piProcessInfo.hProcess, strModulePath.Get() );

	// Inject bass.dll into Mafia2.exe
	iReturn += SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, strBassPath.Get());

	// Did m2o.dll fail to inject?
	if( iReturn > 0)
	{
		// Terminate Mafia2.exe
		TerminateProcess( piProcessInfo.hProcess, 0 );

		// Generate the error string
		String strError( "Unknown Error. Can't launch." );

		// Find the cause of the error
		if( iReturn == 1 )
			strError = "[M2O] : Failed to write library path into remote process. Can't launch.";
		else if( iReturn == 2 )
			strError = "[M2O] : Failed to create remote thread in remote process. Can't launch.";
		else if( iReturn == 2 )
			strError = "[M2O] : Failed to open the remote process. Can't launch.";

		// Show the error message
		ShowMessageBox( strError.Get() );

		// Kill mafia2 process
		if (SharedUtility::IsProcessRunning("Mafia2.exe")){
			SharedUtility::_TerminateProcess("Mafia2.exe");
		}
		return 1;
	}

	// Resume Mafia2.exe thread
	ResumeThread( piProcessInfo.hThread );

	return 0;
}