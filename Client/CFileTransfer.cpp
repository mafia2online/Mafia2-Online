/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransfer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

void CFileTransfer::FileWriteHandler ( unsigned int uiBytesWritten, void * pUserData )
{
	// Get the pointer to this
	CFileTransfer * pFileTransfer = (CFileTransfer *) pUserData;

	// Is the pointer valid?
	if ( pFileTransfer )
	{
		// Update the total downloaded
		pFileTransfer->m_uiTotalDownloaded += uiBytesWritten;

		// Update the transfer progress
		pCore->GetGUI()->GetDownloadProgress()->SetProgress ( ((float)pFileTransfer->m_uiTotalDownloaded / (float)pFileTransfer->m_uiFileSize) );
	}
}

void CFileTransfer::HeaderHandler ( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData )
{
	// Get the pointer to this
	CFileTransfer * pFileTransfer = (CFileTransfer *) pUserData;

	// Is the pointer valid?
	if ( pFileTransfer )
	{
		// Are the headers ok?
		if ( uiStatus == 200 )
		{
			// Update the file size
			pFileTransfer->m_uiFileSize = uiContentLength;
		}
	}
}

CFileTransfer::CFileTransfer ( String strFileName, CFileChecksum fileChecksum, bool bIsScript ) :
	m_strFileName ( strFileName ),
	m_fileChecksum ( fileChecksum ),
	m_bIsScript ( bIsScript ),
	m_uiFileSize ( 0 ),
	m_uiTotalDownloaded ( 0 )
{
	// Build the target directory string
	String strTargetFolder = SharedUtility::GetClientScriptFolder ( pCore->GetHost (), pCore->GetPort () );

	// Does the target directory not exists?
	if ( !SharedUtility::Exists ( strTargetFolder.Get () ) )
	{
		// Create the target directory
		SharedUtility::CreateDirectory ( strTargetFolder.Get () );
	}

	// Build the target file string
	m_strTargetFile = String ( "%s%s", strTargetFolder.Get (), strFileName.Get () );

	// Set the file transfer handler
	m_httpClient.SetFileWriteHandler ( FileWriteHandler, this );

	// Let the http client know it should parse headers
	m_httpClient.ShouldParseHeaders ( true );

	// Set the http header handler
	m_httpClient.SetHeaderHandler ( HeaderHandler, this );
}

CFileTransfer::~CFileTransfer ( void )
{

}

void CFileTransfer::Pulse ( String strHost, unsigned short usHttpPort )
{
	// Set our http client information
	m_httpClient.SetHost ( strHost );
	m_httpClient.SetPort ( usHttpPort );

	// Does the file already exist?
	if ( SharedUtility::Exists ( m_strTargetFile.Get () ) )
	{
		// Calculate the hash of our version of the file
		CFileChecksum currentFileChecksum;
		currentFileChecksum.Calculate ( m_strTargetFile );

		// Do we have the latest file?
		if ( currentFileChecksum == m_fileChecksum )
		{
			// Finish
			m_bComplete = true;
			m_bSucceeded = true;
			return;
		}
	}

	// Request the file from the http server
	if ( !m_httpClient.Get ( String ( "/%s", m_strFileName.Get () ) ) )
		CLogFile::Printf ( "Failed to start http download! (%s)", m_httpClient.GetLastErrorString ().Get () );

	// Show the download progress bar
	pCore->GetGUI()->GetDownloadProgress()->SetVisible( true );
	pCore->GetGUI()->GetDownloadProgress()->SetProgress( 0.0f );

	// Set the download window title
	pCore->GetGUI()->GetDownloadProgress()->SetFileName( m_strFileName );

	// Open the target file
	FILE * pFile = fopen ( m_strTargetFile.Get (), "wb" );

	// Did the file fail to open?
	if ( !pFile )
	{
		// Finish
		m_bComplete = true;
		m_bSucceeded = false;
		return;
	}

	// Set the http download file
	m_httpClient.SetFile ( pFile );

	// Process the http client
	while ( m_httpClient.IsBusy () )
		m_httpClient.Process ();

	// Close the download file
	fclose ( pFile );

	// Calculate the checksum for the new file
	CFileChecksum currentFileChecksum;
	currentFileChecksum.Calculate ( m_strTargetFile );

	// Do the files not match?
	if ( currentFileChecksum != m_fileChecksum )
	{
		CLogFile::Printf ( "Failed to download file '%s' (Checksum mismatch)", m_strTargetFile.Get () );

		// Finish
		m_bComplete = true;
		m_bSucceeded = false;
		return;
	}

	// Finish
	m_bComplete = true;
	m_bSucceeded = true;
}