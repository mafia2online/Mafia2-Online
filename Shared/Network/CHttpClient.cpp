/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CHttpClient.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CHttpClient.h"

// OS Dependent Defines
#ifdef _WIN32
#include <winsock2.h>
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define closesocket close
#include <string.h>
#include <unistd.h>
#endif

#include "../SharedUtility.h"
#include "../CLogFile.h"

// OS Independent Defines
#define MAX_BUFFER				8192
#define DEFAULT_PORT			80
#define DEFAULT_USER_AGENT		"Mafia2Multiplayer/1.0"
#define DEFAULT_REFERER			"http://m2-multiplayer.com"

CHttpClient::CHttpClient()
{
	// If windows startup winsock
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Invalidate the socket handle
	m_iSocket = INVALID_SOCKET;

	// Reset class vars
	ResetVars();
}

CHttpClient::~CHttpClient()
{
	// If we are connected to a host disconnect
	if(m_bConnected)
		Disconnect();

	// If windows cleanup winsock
#ifdef _WIN32
	WSACleanup();
#endif
}

void CHttpClient::ResetVars()
{
	// Set the timeout amount
	m_lReceiveTimeout = 1;

	// Set the connected flag to false
	m_bConnected = false;

	// Reset the host
	m_strHost.clear();

	// Reset the port
	m_usPort = DEFAULT_PORT;

    // Set the status to none
    m_status = HTTP_STATUS_NONE;

    // Reset the header
    m_headerMap.clear();

    // Reset the data
	m_strData.clear();

	// Set the last error to none
	m_lastError = HTTP_ERROR_NONE;

    // Set the default user agent
    m_strUserAgent = DEFAULT_USER_AGENT;

    // Set the default referer
    m_strReferer = DEFAULT_REFERER;

	// Reset the request start
	m_ulRequestStart = 0;

	// Mark as should parse headers
	m_bParseHeaders = true;

	// Reset instances
	m_pfnRecieveHandler = NULL;
	m_pRecieveHandlerUserData = NULL;
	m_pfnHeaderHandler = NULL;
	m_pfnFileWriteHandler = NULL;
	m_pFileWriteUserData = NULL;
	m_pFile = NULL;
}

void CHttpClient::SetReceiveTimeout(long lReceiveTimeout)
{
	// Set the receive timeout amount
	m_lReceiveTimeout = lReceiveTimeout;
}

bool CHttpClient::Connect()
{
	// Prepare the socket
    m_iSocket = socket( AF_INET, SOCK_STREAM, 0 );

    if( m_iSocket == INVALID_SOCKET )
	{
		// Failed to prepare the socket, set the last error
        m_lastError = HTTP_ERROR_SOCKET_PREPARE_FAILED;
        return false;
	}

    // Get the host
	hostent * heHost = gethostbyname( m_strHost.Get() );

    if( heHost == NULL )
    {
		// Failed to get the host, set the last error
        m_lastError = HTTP_ERROR_INVALID_HOST;
        return false;
    }

	// Prepare a socket address
	sockaddr_in sinAddress;
	sinAddress.sin_family = AF_INET;
	sinAddress.sin_port = htons(m_usPort);
	sinAddress.sin_addr = *((in_addr *)heHost->h_addr);
	memset( &sinAddress.sin_zero, 0, (sizeof(char) * 8) );

	// Try to connect
    if(connect(m_iSocket, (sockaddr *)&sinAddress, sizeof(sockaddr)) < 0)
    {
		// Disconnect
        Disconnect();

        // Connection failed, set the last error
        m_lastError = HTTP_ERROR_CONNECTION_FAILED;
        return false;
	}

	// Set the connected flag to true
    m_bConnected = true;
    return true;
}

void CHttpClient::Disconnect()
{
	// Is the socket valid?
	if(m_iSocket != INVALID_SOCKET)
	{
		// Close the socket
		closesocket(m_iSocket);

		// Invalidate the socket
		m_iSocket = INVALID_SOCKET;
	}

	// Set the connected flag to false
	m_bConnected = false;
}

bool CHttpClient::Write(const char * szData, int iLen)
{
	// Try to send
	if( send( m_iSocket, szData, iLen, 0 ) == SOCKET_ERROR )
	{
		// Send failed, set the last error
        m_lastError = HTTP_ERROR_SEND_FAILED;
        return false;
	}

	return true;
}

int CHttpClient::Read(char * szBuffer, int iLen)
{
	// Read from the socket
	return recv( m_iSocket, szBuffer, iLen, 0 );
}

bool CHttpClient::Get(String strPath)
{
	// Try connect to the host
	if( !Connect() )
		return false;

    // Reset the header and data
    m_headerMap.clear();
    m_strData.clear();

    // Prepare the GET command
	String strGet( "GET %s HTTP/1.0\r\n"
						"Host: %s\r\n"
						"User-Agent: %s\r\n"
						"Referer: %s\r\n"
						"Connection: close\r\n"
						"\r\n",
						strPath.Get(), m_strHost.Get(), m_strUserAgent.Get(),
						m_strReferer.Get() );

    // Send the GET command
	if( !Write( strGet.Get(), strGet.GetLength() ) )
        return false;

	// Set the status to get data
	m_status = HTTP_STATUS_GET_DATA;

	// Set the request start
	m_ulRequestStart = SharedUtility::GetTime();
    return true;
}

bool CHttpClient::Post(bool bHasResponse, String strPath, String strData, String strContentType)
{
	// Try connect to the host
	if( !Connect() )
		return false;

	// Reset the header and data
    m_headerMap.clear();
    m_strData.clear();

    // Prepare the POST command
	String strPost(	"POST %s HTTP/1.0\r\n" \
						"Host: %s\r\n" \
                        "User-Agent: %s\r\n\r\n" \
                        "Referer: %s\r\n" \
                        "Content-Type: %s\r\n" \
                        "Content-Length: %d\r\n" \
                        "Connection: close\r\n" \
                        "\r\n" \
                        "%s", 
							strPath.Get(), m_strHost.Get(), m_strUserAgent.Get(), 
							m_strReferer.Get(), strContentType.Get(), strData.GetLength(), 
							strData.Get() );

	// Send the POST command
    if( !Write( strPost.Get(), strPost.GetLength() ) )
		return false;

	// Do we have a response
	if( bHasResponse )
	{
		// Set the status
		m_status = HTTP_STATUS_GET_DATA;

		// Get the request start time
		m_ulRequestStart = SharedUtility::GetTime();
	}
	else
	{
		// Disconnect
		Disconnect();
	}

	return true;  
}

bool CHttpClient::ParseHeaders( String& strBuffer, int& iBufferSize )
{
	// Should we parse headers?
	if( m_bParseHeaders )
	{
		// Copy the headers
		String headers = strBuffer;

		// Get the status code
		String strStatus = headers;
		strStatus.Erase( 0, 9 );
		strStatus.Erase( 12, strStatus.GetLength() );
		unsigned int uiStatus = strStatus.ToInteger();

		// Get the content length
		size_t sOff = headers.Find( "Content-Length:" );

		if( sOff != String::nPos )
		{
			headers.Erase( 0, sOff + 16 );
			sOff = headers.Find( "Connection:" );

			if( sOff != String::nPos )
				headers.Erase( sOff, headers.GetLength() );
		}

		// Do we have a handler set for the headers?
		if( m_pfnHeaderHandler )
			m_pfnHeaderHandler( uiStatus, (unsigned int)headers.ToInteger(), m_pHeaderHandlerUserData );
	}

	// Find the header size
	int iHeaderSize = 0;
	for( int i = 0; i < iBufferSize; i++ )
	{
		if( strBuffer.GetChar(i) == '\r' && strBuffer.GetChar(i+1) == '\n' && strBuffer.GetChar(i+2) == '\r' && strBuffer.GetChar(i+3) == '\n' )
		{
			iHeaderSize = i + 4;
			break;
		}
	}

	// Do we have any headers?
	if( iHeaderSize > 0 )
	{
		// Adjust the buffer size
		iBufferSize -= iHeaderSize;

		// Erase the headers
		strBuffer.Erase( 0, iHeaderSize );
	}

	// Set the header size
	m_headerMap[ "HeaderSize" ] = iHeaderSize;

	return true;
}

void CHttpClient::Process()
{
	// Have we timed out?
	if( m_ulRequestStart > 0 && (SharedUtility::GetTime() - m_ulRequestStart) >= TIMEOUT )
	{
		// Set the status to none
		m_status = HTTP_STATUS_NONE;

		// Set last error
		m_lastError = HTTP_ERROR_REQUEST_TIMEOUT;

		// Reset the request start
		m_ulRequestStart = 0;

		// Disconnect
		Disconnect();

		return;
	}

	// Are we not idle?
	if( m_status != HTTP_STATUS_NONE )
	{
		switch( m_status )
		{
		case HTTP_STATUS_GET_DATA:
			{
				// Prepare the buffer
				char szBuffer[MAX_BUFFER];
				memset(szBuffer, 0, sizeof(szBuffer));

				// Try to read from the socket
				int iBytesRead = Read(szBuffer, sizeof(szBuffer));
				int iSkipBytes = 0;

				// Did we read anything?
				if( iBytesRead > 0 )
				{
					// Create a string from the recieve buffer
					String strBuffer;
					strBuffer.Set ( szBuffer, iBytesRead );

					// Is the header empty?
					if( m_headerMap.empty() )
					{
						// Set the bytes to skip
						iSkipBytes = iBytesRead;

						// Parse the headers
						if( !ParseHeaders( strBuffer, iBytesRead ) )
						{
							// We don't have a header
							m_status = HTTP_STATUS_INVALID;

							// Set the last error
							m_lastError = HTTP_ERROR_NO_HEADER;

							// Reset the request start
							m_ulRequestStart = 0;

							// Disconnect
							Disconnect();

							CLogFile::Print ( "HTTP DATA READ ERROR - No headers." );

							return;
						}

						// Adjust the bytes to skip
						iSkipBytes -= iBytesRead;

						// Do we not have any data?
						if( iBytesRead == 0 )
							return;
					}

					// Skip the header data
					char * szData = (iBytesRead ? (szBuffer + iSkipBytes) : NULL);

					// Do we have a recieve handler?
					if ( m_pfnRecieveHandler != NULL )
					{
						// Call the recieve handler
						if ( m_pfnRecieveHandler ( szData, iBytesRead, m_pRecieveHandlerUserData ) )
							m_strData.Append ( szData, iBytesRead );
					}
					else if ( m_pFile != NULL )
					{
						// Write the response to the file
						fwrite ( szData, 1, iBytesRead, m_pFile );

						// Call the file write handler
						if ( m_pfnFileWriteHandler )
							m_pfnFileWriteHandler ( iBytesRead, m_pFileWriteUserData );
					}
					else
						m_strData.Append ( szData, iBytesRead );
				}
				else
				{
					// We got data
					m_status = HTTP_STATUS_GOT_DATA;

					// Reset the request start
					m_ulRequestStart = 0;

					// Disconnect
					Disconnect();
				}
			}
			break;
		}
	}
}

String CHttpClient::GetLastErrorString()
{
	String strError("Unknown");

	switch(GetLastError())
    {
    case HTTP_ERROR_SOCKET_PREPARE_FAILED:
		strError.Set("Failed to prepare socket");
        break;
    case HTTP_ERROR_INVALID_HOST:
        strError.Set("Invalid host");
        break;
	case HTTP_ERROR_IOCTL_FAILED:
		strError.Set("IoCtl failed");
		break;
    case HTTP_ERROR_CONNECTION_FAILED:
		strError.Set("Connection failed");
        break;
    case HTTP_ERROR_SEND_FAILED:
		strError.Set("Send failed");
		break;
	case HTTP_ERROR_REQUEST_TIMEOUT:
		strError.Set("Request timed out");
		break;
    case HTTP_ERROR_NO_HEADER:
        strError.Set("No header");
		break;
	}

	return strError;
}

void CHttpClient::Reset( void )
{
	// Are we connected?
	if( IsConnected() )
	{
		// Disconnect
		Disconnect( );
	}

	// Set the status to none
	m_status = HTTP_STATUS_NONE;

	// Reset the file
	SetFile ();
}
