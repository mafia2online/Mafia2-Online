/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : CUpdate.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "stdafx.h"
#include "Form1.h"

extern CUpdate * pUpdater;

int ShowMessageBox_( const char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK) )
{
	return MessageBox( NULL, szText, MOD_NAME, uType );
}

bool CUpdate::ReceiveHandler( const char * szData, unsigned int uiSize, void * pUserData )
{
	return true;
}

void CUpdate::HeaderHandler( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData )
{
	// Get the updater singleton
	CUpdate * pUpdater = (CUpdate *)pUserData;

	// Are we waiting for a file?
	if( pUpdater->GetHttpState() == E_HTTP_STATE_WAIT_FILE )
	{
		// Was the request successfull?
		if( uiStatus == 200 )
		{
			// Get the file path
			String strPath( "%s\\mafia2multiplayer.7z", pUpdater->m_strModDirectory.Get() );

			// Open the target file
			pUpdater->m_pUpdateFile = fopen( strPath.Get(), "wb" );

			// Reset
			pUpdater->m_uiUpdateFileSize = uiContentLength;
			pUpdater->m_uiUpdateFileDownloaded = 0;

			// Set the http client file
			pUpdater->GetHttpClient()->SetFile ( pUpdater->m_pUpdateFile );
		}
	}
}

void CUpdate::FileWriteHandler ( unsigned int uiBytesWritten, void * pUserData )
{
	// Get the updater instance
	CUpdate * pUpdater = (CUpdate *)pUserData;

	// Increase the download total
	pUpdater->m_uiTotalBytesWritten += uiBytesWritten;
}

CUpdate::CUpdate ( void )
{
	char szGameDir[MAX_PATH], szModDir[MAX_PATH];

	// Try read the game directory string from the registry
	if ( !SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "GameDir", NULL, szGameDir, sizeof(szGameDir) ) )
		void;
	
	// Try read the install location string from the registry
	if ( !SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "InstallLocation", NULL, szModDir, sizeof(szModDir) ) )
		void;

	// Set the game directories
	m_strGameDirectory.Set ( szGameDir );
	m_strModDirectory.Set ( szModDir );

	// Reset
	m_threadState = E_THREAD_STATE_IDLE;
	m_httpState = E_HTTP_STATE_IDLE;
	m_bProcessedData = false;
	m_bDownloading = false;
	m_bIsUpdateAvailable = false;
	m_pUpdateFile = NULL;
	m_uiUpdateFileSize = 0;
	m_uiUpdateFileDownloaded = 0;
	m_uiTotalBytesWritten = 0;

	// Create the http client instance
	m_pHttpClient = new CHttpClient ();
	m_pHttpClient->ShouldParseHeaders ( true );
	m_pHttpClient->SetHeaderHandler ( HeaderHandler, this );
	m_pHttpClient->SetFileWriteHandler ( FileWriteHandler, this );
	m_pHttpClient->SetHost ( "deploy.m2-multiplayer.com" );
}

CUpdate::~CUpdate ( void )
{
	// Delete the http client instance
	SAFE_DELETE ( m_pHttpClient );
}

void CUpdate::Pulse ( void )
{
	// Loop while we process data
	while ( m_bDownloading )
	{
		// Is the http client busy?
		if ( pUpdater->GetHttpClient()->IsBusy () )
		{
			// Process the http client
			pUpdater->GetHttpClient()->Process ();

			// Did we get all the data?
			if ( pUpdater->GetHttpClient()->GotData () )
			{
				// Get the http client data
				String * strData = pUpdater->GetHttpClient()->GetData ();

				// Check the current http state
				switch ( pUpdater->GetHttpState () )
				{
				case E_HTTP_STATE_WAIT_VERSION:
					{
						// Reset the http state
						pUpdater->SetHttpState ( E_HTTP_STATE_IDLE );

						// Get a string of all the files
						std::vector < String > strFiles = strData->split ( "<br/>" );

						// Process the game files
						CheckFiles ( strFiles );
						
						// Mark as data processed
						m_bProcessedData = true;
						m_bDownloading = false;

						break;
					}

				case E_HTTP_STATE_WAIT_FILE:
					{
						// Do we have an open update file?
						if ( pUpdater->m_pUpdateFile )
						{
							// Close the download file
							fclose( pUpdater->m_pUpdateFile );
							pUpdater->m_pUpdateFile = NULL;

							// Mark as downloaded update
							m_bDownloadedUpdate = true;
						}

						// Reset
						m_bDownloading = false;

						break;
					}
				}
			}
		}

		// Sleep while inactive
		Sleep ( 10 );
	}
}

void CUpdate::CheckForUpdates ( void )
{
	// Get the version file
	pUpdater->GetHttpClient()->Get( "/index.php" );

	// Set the http state
	pUpdater->SetHttpState ( E_HTTP_STATE_WAIT_VERSION );

	// Reset
	m_bIsUpdateAvailable = false;
	m_bDownloadedUpdate = false;
	m_bDownloading = true;
	m_uiTotalBytesWritten = 0;
	m_updateFileQueue.clear ();
	m_strFilesRequested.clear ();

	// Pulse
	pUpdater->Pulse ();

	// Is there and update available?
	if ( m_bIsUpdateAvailable )
	{
		// Ask the user if they want to download the update
		if ( MessageBox ( NULL, "An update is available, would you like to download it now?", "Mafia 2 Multiplayer - Update Available", MB_ICONQUESTION|MB_YESNO ) == IDYES )
		{
			// Build the request URL
			String strRequest ( "/download.php?files=" );

			// Loop over all files we need
			for( std::vector< String >::iterator iter = m_updateFileQueue.begin(); iter != m_updateFileQueue.end(); iter++ )
			{
				// Get the current file
				String strFile ( "%s,", (*iter).Get() );

				// Append the current file to the request URL
				strRequest.Append ( strFile.Get() );

				// Append the current file to the saved request URL
				m_strFilesRequested.Append ( strFile.Get() );
			}

			// Set the http state
			pUpdater->SetHttpState ( E_HTTP_STATE_WAIT_FILE );

			// Download the updated files
			m_pHttpClient->Get ( strRequest );

			// Reset
			m_bDownloading = true;

			// Pulse
			pUpdater->Pulse ();
		}
	}

	// Process any updates we may have
	pUpdater->ProcessUpdates ();
}

bool IsGameFile ( String strFile )
{
	// Try find "edit/" in the string
	if( strFile.Find( "edit/" ) != -1 )
		return true;

	// Try find "pc/" in the string
	if( strFile.Find( "pc/" ) != -1 )
		return true;

	return false;
}

void CUpdate::CheckFiles ( std::vector < String > strFiles )
{
	// Loop over each file
	for( std::vector< String >::iterator iter = strFiles.begin(); iter != strFiles.end(); iter++ )
	{
		// If this file is invalid, ignore it
		if ( (*iter).GetLength() <= 0 )
			continue;

		// Split the string
		std::vector< String > file = (*iter).split( ':' );
		String strFilePath = file.at( 0 );
		String strHash = file.at( 1 );

		// Get the directory for this file
		String strDirectory = (IsGameFile ( strFilePath ) ? m_strGameDirectory : m_strModDirectory);

		// Calculate our file md5
		String ourHash = md5 ( String( "%s\\%s", strDirectory.Get(), strFilePath.Get() ), true );

		// Does our hash match the latest?
		if( !ourHash.Compare( strHash ) )
			continue;

		// Push this file into the update queue
		m_updateFileQueue.push_back( strFilePath );

		// Mark as update needed
		m_bIsUpdateAvailable = true;
	}
}

void CUpdate::ProcessUpdates ( void )
{
	// Do we have some updated data?
	if( SharedUtility::Exists( String( "%s\\mafia2multiplayer.7z", m_strModDirectory.Get() ).Get() ) )
	{
		// Try decompress the zipped file
		if ( system(String("\"\"%s\\data\\7z.exe\" x -r -y \"%s\\mafia2multiplayer.7z\" -o\"%s\\*\"\"", m_strModDirectory.Get(), m_strModDirectory.Get(), m_strModDirectory.Get()).Get()) == 0 )
		{
			// Delete the compressed file
			remove ( String ( "%s\\mafia2multiplayer.7z", m_strModDirectory.Get() ).Get() );

			// Get a list of requested files
			std::vector < String > requestedFile = m_strFilesRequested.split ( "," );

			// Loop over each requested file
			for ( std::vector < String >::iterator iter = requestedFile.begin (); iter != requestedFile.end (); iter ++ )
			{
				// Is the requested file valid?
				if ( (*iter).GetLength () > 0 )
				{
					// Get the current file directory
					String strDirectory = (IsGameFile ( (*iter) ) ? m_strGameDirectory : m_strModDirectory);

					// Get the old directory for this file
					String strOldDirectory ( "%s\\mafia2multiplayer\\%s", m_strModDirectory.Get(), (*iter).Get() );

					// Get the new directory for this file
					String strNewDirectory ( "%s/%s", strDirectory.Get(), (*iter).Get() );

					// Ensure all paths are available
					std::vector < String > str = strNewDirectory.split ( "/" );
					String strCurrentDir = strDirectory;
					for ( std::vector < String >::iterator iter = (str.begin () + 1); iter != str.end (); iter ++ )
					{
						// If the current directory doesn't exist create it
						if ( !SharedUtility::Exists ( strCurrentDir.Get() ) )
							SharedUtility::CreateDirectory ( strCurrentDir.Get() );

						// Append the current directory to the global directory
						strCurrentDir.Append ( String ( "/%s", (*iter).Get() ).Get() );
					}

					// Copy the new file
					if ( SharedUtility::CopyFile ( strOldDirectory.Get(), strNewDirectory.Get() ) )
					{
						// Delete the old file
						remove ( strOldDirectory.Get() );
					}
				}
			}

			// Delete the update directory
			system ( String ( "rmdir \"%s\\mafia2multiplayer\" /s /q", m_strModDirectory.Get() ).Get() );
		}
		else
		{
			//
			ShowMessageBox_ ( "An error occured whilst decompressing the update file.\n\nPlease restart Mafia 2 Multiplayer.", MB_ICONERROR|MB_OK );
		}
	}
}