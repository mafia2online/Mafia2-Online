/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : CUpdate.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

enum eThreadState
{
	E_THREAD_STATE_IDLE = 0,
	E_THREAD_STATE_GET_VERSION,
	E_THREAD_STATE_GET_FILE,
};

enum eHttpState
{
	E_HTTP_STATE_IDLE = 0,
	E_HTTP_STATE_WAIT_VERSION,
	E_HTTP_STATE_WAIT_FILE,
};

class CUpdate
{

private:

	CHttpClient					* m_pHttpClient;
	eThreadState				m_threadState;
	eHttpState					m_httpState;
	bool						m_bProcessedData;
	bool						m_bDownloadedUpdate;
	bool						m_bDownloading;
	String						m_strFilesRequested;

	bool						m_bIsUpdateAvailable;
	std::vector < String >		m_updateFileQueue;

	static	bool				ReceiveHandler					( const char * szData, unsigned int uiSize, void * pUserData );
	static	void				HeaderHandler					( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData );
	static	void				FileWriteHandler				( unsigned int uiBytesWritten, void * pUserData );

public:

	FILE						* m_pUpdateFile;
	unsigned int				m_uiUpdateFileSize;
	unsigned int				m_uiUpdateFileDownloaded;
	unsigned int				m_uiTotalBytesWritten;
	String						m_strGameDirectory;
	String						m_strModDirectory;

								CUpdate							( void );
								~CUpdate						( void );

	void						Pulse							( void );

	void						CheckForUpdates					( void );
	void						CheckFiles						( std::vector < String > strFiles );

	void						ProcessUpdates					( void );






	CHttpClient					* GetHttpClient					( void ) { return m_pHttpClient; }

	void						SetThreadState					( eThreadState threadState ) { m_threadState = threadState; }
	eThreadState				GetThreadState					( void ) { return m_threadState; }

	void						SetHttpState					( eHttpState httpState ) { m_httpState = httpState; }
	eHttpState					GetHttpState					( void ) { return m_httpState; }

};