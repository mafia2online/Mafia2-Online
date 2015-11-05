/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CHttpClient.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CString.h"
#include	<map>

#define DEFAULT_CONTENT_TYPE	"text/plain"
#define	TIMEOUT					30000

#ifndef _WIN32
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

typedef void	(* HeaderHandler_t)( unsigned int uiStatus, unsigned int uiContentLength, void * pUserData );
typedef bool	(* RecieveHandler_t)( const char * szData, unsigned int uiDataSize, void * pUserData );
typedef void	(* FileWriteHandler_t)( unsigned int uiDataSize, void * pUserData );

// Status codes
enum eHttpStatus
{
	HTTP_STATUS_NONE,
	HTTP_STATUS_INVALID,
	HTTP_STATUS_GET_DATA,
	HTTP_STATUS_GOT_DATA
};

// Error codes
enum eHttpError
{
	HTTP_ERROR_NONE,
    HTTP_ERROR_SOCKET_PREPARE_FAILED,
    HTTP_ERROR_INVALID_HOST,
	HTTP_ERROR_IOCTL_FAILED,
    HTTP_ERROR_CONNECTION_FAILED,
    HTTP_ERROR_SEND_FAILED,
	HTTP_ERROR_REQUEST_TIMEOUT,
    HTTP_ERROR_NO_HEADER
};

class CHttpClient
{

private:

	int								m_iSocket;
	long							m_lReceiveTimeout;
	bool							m_bConnected;
	String							m_strHost;
	unsigned short					m_usPort;
	eHttpStatus						m_status;
	std::map< String, String >		m_headerMap;
	String							m_strData;
	eHttpError						m_lastError;
	String							m_strUserAgent;
	String							m_strReferer;
	unsigned long					m_ulRequestStart;
	bool							m_bParseHeaders;

	HeaderHandler_t					m_pfnHeaderHandler;
	void							* m_pHeaderHandlerUserData;

	RecieveHandler_t				m_pfnRecieveHandler;
	void							* m_pRecieveHandlerUserData;

	FILE							* m_pFile;
	FileWriteHandler_t				m_pfnFileWriteHandler;
	void							* m_pFileWriteUserData;

	void							ResetVars					( void );
	bool							Connect						( void );
	void							Disconnect					( void );
	bool							ParseHeaders				( String& strBuffer, int& iBufferSize );
	bool							Write						( const char * szData, int iLen );
    int								Read						( char * szBuffer, int iLen );

public:

									CHttpClient					( void );
									~CHttpClient				( void );

	virtual void					SetReceiveTimeout			( long lReceiveTimeout );
	virtual long					GetReceiveTimeout			( void ) { return m_lReceiveTimeout; }
	virtual bool					IsConnected					( void ) { return m_bConnected; }
    virtual bool					IsInvalid					( void ) { return (m_status == HTTP_STATUS_INVALID); }
    virtual bool					GettingData					( void ) { return (m_status == HTTP_STATUS_GET_DATA); }
    virtual bool					GotData						( void ) { return (m_status == HTTP_STATUS_GOT_DATA); }
	virtual bool					IsBusy						( void ) { return (m_status == HTTP_STATUS_GET_DATA); }
    virtual String					* GetData					( void ) { return &m_strData; }
    virtual eHttpError				GetLastError				( void ) { return m_lastError; }
    virtual void					SetUserAgent				( String strUserAgent ) { m_strUserAgent = strUserAgent; }
	virtual String					GetUserAgent				( void ) { return m_strUserAgent; }
    virtual void					SetReferer					( String strReferer ) { m_strReferer = strReferer; }
    virtual String					GetReferer					( void ) { return m_strReferer; }
    virtual void					SetHost						( String strHost ) { m_strHost = strHost; }
	virtual String					GetHost						( void ) { return m_strHost; }
    virtual void					SetPort						( unsigned short usPort ) { m_usPort = usPort; }
    virtual unsigned short			GetPort						( void ) { return m_usPort; }
    virtual bool					Get							( String strPath );
    virtual bool					Post						( bool bHasResponse, String strPath, String strData = "", String strContentType = DEFAULT_CONTENT_TYPE );
    virtual void					Process						( void );
    virtual String					GetLastErrorString			( void );

	virtual void					SetHeaderHandler			( HeaderHandler_t pfnHeaderHandler, void * pHeaderHandlerUserData ) { m_pfnHeaderHandler = pfnHeaderHandler; m_pHeaderHandlerUserData = pHeaderHandlerUserData; }
	virtual void					SetReceiveHandle			( RecieveHandler_t pfnRecieveHandler, void * pRecieveHandlerUserData ) { m_pfnRecieveHandler = pfnRecieveHandler; m_pRecieveHandlerUserData = pRecieveHandlerUserData; }
	virtual void					Reset						( void );

	virtual void					ShouldParseHeaders			( bool bShould ) { m_bParseHeaders = bShould; }

	virtual	void					SetFile						( FILE * file = NULL ) { m_pFile = file; }
	virtual void					SetFileWriteHandler			( FileWriteHandler_t pfnFileWriteHandler, void * pFileWriteUserData ) { m_pfnFileWriteHandler = pfnFileWriteHandler; m_pFileWriteUserData = pFileWriteUserData; }

};