/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransfer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CGUI.h"
#include "CDownloadProgress.h"

#include "CMainMenu.h"
#include "CServerBrowser.h"

#include "CRC.h"

#include "CFileTransfer.h"

#include "CLogFile.h"

void CFileTransfer::FileWriteHandler ( unsigned int uiBytesWritten, void * pUserData )
{
	CFileTransfer * pFileTransfer = (CFileTransfer *) pUserData;
	if ( pFileTransfer )
	{
		pFileTransfer->m_uiTotalDownloaded += uiBytesWritten;
		CCore::Instance()->GetGUI()->GetDownloadProgress()->SetProgress(((float)pFileTransfer->m_uiTotalDownloaded / (float)pFileTransfer->m_uiFileSize));
	}
}

void CFileTransfer::HeaderHandler ( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData )
{
	CFileTransfer * pFileTransfer = (CFileTransfer *) pUserData;
	if ( pFileTransfer )
	{
		if ( uiStatus == 200 )
		{
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
	String strTargetFolder = SharedUtility::GetClientScriptFolder(CCore::Instance()->GetHost(), CCore::Instance()->GetPort());
	if ( !SharedUtility::Exists ( strTargetFolder.Get () ) )
	{
		SharedUtility::CreateDirectory ( strTargetFolder.Get () );
	}

	m_strTargetFile.Format("%s%s", strTargetFolder.Get(), strFileName.Get());

	m_httpClient.SetFileWriteHandler ( FileWriteHandler, this );
	m_httpClient.ShouldParseHeaders ( true );
	m_httpClient.SetHeaderHandler ( HeaderHandler, this );

	CCore::Instance()->GetGUI()->GetMainMenu()->SetVisible(false);
	CCore::Instance()->GetGUI()->GetServerBrowser()->SetVisible(false);
}

CFileTransfer::~CFileTransfer ( void )
{

}

void CFileTransfer::Pulse ( String strHost, unsigned short usHttpPort )
{
	m_httpClient.SetHost ( strHost );
	m_httpClient.SetPort ( usHttpPort );

	if ( SharedUtility::Exists ( m_strTargetFile.Get () ) )
	{
		CFileChecksum currentFileChecksum;
		currentFileChecksum.Calculate ( m_strTargetFile );
		if ( currentFileChecksum == m_fileChecksum )
		{
			m_bComplete = true;
			m_bSucceeded = true;
			return;
		}
	}

	String filePath;
	filePath.Format("/%s", m_strFileName.Get ());
	if ( !m_httpClient.Get ( filePath ) )
		CLogFile::Printf ( "Failed to start http download! (%s)", m_httpClient.GetLastErrorString ().Get () );

	CCore::Instance()->GetGUI()->GetDownloadProgress()->SetVisible(true);
	CCore::Instance()->GetGUI()->GetDownloadProgress()->SetProgress(0.0f);
	CCore::Instance()->GetGUI()->GetDownloadProgress()->SetFileName(m_strFileName);

	FILE * pFile = fopen ( m_strTargetFile.Get (), "wb" );
	if ( !pFile )
	{
		// Finish
		m_bComplete = true;
		m_bSucceeded = false;
		return;
	}

	m_httpClient.SetFile ( pFile );
	while ( m_httpClient.IsBusy () )
		m_httpClient.Process ();

	fclose ( pFile );

	CFileChecksum currentFileChecksum;
	currentFileChecksum.Calculate ( m_strTargetFile );
	if ( currentFileChecksum != m_fileChecksum )
	{
		CLogFile::Printf ( "Failed to download file '%s' (Checksum mismatch)", m_strTargetFile.Get () );
		m_bComplete = true;
		m_bSucceeded = false;
		return;
	}
	m_bComplete = true;
	m_bSucceeded = true;
}