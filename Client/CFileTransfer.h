/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFileTransfer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Shared/Network/CHttpClient.h"

class CFileTransfer
{

	friend class CFileTransferManager;

private:

	CHttpClient						m_httpClient;
	String							m_strFileName;
	String							m_strFileType;
	CFileChecksum					m_fileChecksum;
	unsigned int					m_uiFileSize;
	unsigned int					m_uiTotalDownloaded;
	String							m_strTargetFile;
	bool							m_bIsScript;

	bool							m_bComplete;
	bool							m_bSucceeded;

	static	void					FileWriteHandler				( unsigned int uiBytesWritten, void * pUserData );
	static	void					HeaderHandler					( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData );

public:

									CFileTransfer					( String strFileName, CFileChecksum fileChecksum, bool bIsScript );
									~CFileTransfer					( void );

	void							Pulse							( String strHost, unsigned short usHttpPort );

	bool							IsComplete						( void ) { return m_bComplete; }
	bool							HasSucceeded					( void ) { return m_bSucceeded; }
	bool							IsScript						( void ) { return m_bIsScript; }

};