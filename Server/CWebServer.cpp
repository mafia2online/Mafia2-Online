/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWebServer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore		* pCore;
CMutex				webMutex;

static struct mg_serve_http_opts s_http_server_opts;

void CWebServer::ev_handler(struct mg_connection *nc, int ev, void *ev_data)
{
	struct http_message *hm = (struct http_message *) ev_data;
	switch (ev) {
		case MG_EV_HTTP_REQUEST:
		{
			mg_serve_http(nc, hm, s_http_server_opts);  // Serve static content
		}
		break;

		default:
			break;
	}
}

CWebServer::CWebServer( void )
{
	// Do we not have external webserver configred?
	if( strlen( CVAR_GET_STRING( "httpserver" ) ) == 0 )
	{
		/* Start and bind webServer */
		mg_mgr_init(&mgr, NULL);
		nc = mg_bind(&mgr, String("%d", (CVAR_GET_INTEGER("port") + 1)), ev_handler);

		/* Failed to bind ? */
		if (nc == NULL){
			CLogFile::Printf("Cannot bind webServer on port %d", String("%d", (CVAR_GET_INTEGER("port") + 1)));
			exit(1);
		}
		/* Set the options */
		mg_set_protocol_http_websocket(nc);

		/* Set the webRoot */
		s_http_server_opts.document_root = "./webserver";

		/* Authorize the directory listing */
		s_http_server_opts.enable_directory_listing = "yes";

		/* Failed to find the webRoot directory ? */
		if (mg_stat(s_http_server_opts.document_root, &st) != 0){
			CLogFile::Printf("Cannot find the root directory (%s)...", s_http_server_opts.document_root);
			exit(1);
		}
		CLogFile::Printf("Starting web server on port %d", (CVAR_GET_INTEGER("port") + 1));
	}
}

CWebServer::~CWebServer( void )
{
	if (nc != NULL){
		mg_mgr_free(&mgr);
	}
}

void CWebServer::Pulse()
{
	mg_mgr_poll(&mgr, 1000);
}

bool CWebServer::FileCopy( String strDirectory, String strFileName, CFileChecksum * pChecksum, bool bIsScript )
{
	// Get the file location
	String strFile( "%s/%s", strDirectory.Get(), strFileName.Get() );

	//
	String strWebServerFilePath = strFile;

	// Do we not have external webserver configured?
	if( strlen( CVAR_GET_STRING( "httpserver" ) ) == 0 )
	{
		// Does the file not exist?
		if( !SharedUtility::Exists( strFile.Get() ) )
			return false;

		// Get the path to copy to
		strWebServerFilePath = String( "webserver/%s", strFileName.Get() );

		// Does the webserver path not exist?
		if( !SharedUtility::Exists( "webserver" ) )
		{
			// Create the webserver directory
			SharedUtility::CreateDirectory( "webserver" );
		}

		// Is this file a script?
		if( bIsScript )
		{
			// Compile the script
			if( !SharedUtility::CompileScript( strFile.Get(), strWebServerFilePath.Get() ) )
				return false;
		}
		else
		{
			// Copy the file to the webserver
			if( !SharedUtility::CopyFile( strFile.Get(), strWebServerFilePath.Get() ) )
				return false;
		}
	}

	if( pChecksum )
		return pChecksum->Calculate( strWebServerFilePath.Get() );

	return true;
}
