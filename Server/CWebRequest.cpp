/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWebRequest.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern		CCore			* pCore;

void CWebRequest::WorkerThread ( CThread * pCreator )
{
	// Loop until the thread is closed
	while ( pCreator->GetUserData < bool > () )
	{
		// Get the web request instance
		CWebRequest* pWebRequest = pCore->GetWebRequest ();

		// Is the web request valid?
		if ( pWebRequest )
		{
			// Are we not waiting for any data?
			if ( !pWebRequest->IsActive () )
			{
				// Do we have any waiting requests?
				if ( pWebRequest->size () )
				{
					// Get the current request
					SWebRequest* pCurrent = pWebRequest->front ();

					// Set the active request
					pWebRequest->m_activeRequest = pCurrent;

					// Set the request time
					pCurrent->ulTime = SharedUtility::GetTime();

					// Mark as active
					pWebRequest->SetActive ( true );

					// Set the http host
					pWebRequest->GetHttpClient()->SetHost ( pCurrent->strUrl );

					// Set the http port
					pWebRequest->GetHttpClient()->SetPort ( pCurrent->iPort );

					// Send the http request
					switch( pCurrent->iType )
					{
					case HTTP_TYPE_GET:
						{
							pWebRequest->GetHttpClient()->Get ( String( "%s", pCurrent->strPost.Get () ) );
							break;
						}

					case HTTP_TYPE_POST:
						{
							pWebRequest->GetHttpClient()->Post ( false, String( "%s", pCurrent->strPost.Get () ) );
							break;
						}
					}

					// Was this a post?
					if ( pCurrent->iType == HTTP_TYPE_POST )
					{
						// Reset
						pWebRequest->Reset ();
					}
				}
			}
			else
			{
				// Do we have any data?
				if ( pWebRequest->GetHttpClient()->IsBusy () )
				{
					// Process the http client
					pWebRequest->GetHttpClient()->Process ();

					// Is the request done?
					if ( pWebRequest->GetHttpClient()->GotData () )
					{
						// Call the script function
						CSquirrelArguments args;
						args.push( pWebRequest->m_activeRequest->iType );
						args.push( pWebRequest->GetHttpClient()->GetData()->Get() );
						args.push( (int)(SharedUtility::GetTime() - pWebRequest->m_activeRequest->ulTime) );
						pCore->GetScriptingManager()->Call( pWebRequest->m_activeRequest->pFunction, &args, pWebRequest->m_activeRequest->pVM );

						// Reset
						pWebRequest->Reset ();
					}
				}
			}
		}

		// Sleep
		Sleep ( 100 );
	}
}

bool CWebRequest::RecieveHandler ( const char * szData, unsigned int uiDataSize, void * pUserData )
{
	return true;
}

CWebRequest::CWebRequest( void )
{
	// Mark as not active
	m_bActive = false;

	// Create the http instance
	m_pHttpClient = new CHttpClient ();

	// Set the http receive handler
	m_pHttpClient->SetReceiveHandle ( RecieveHandler, this ); // todo: Http client crashes if there's no receive handler - fix it!

	// Create the worker thread
	m_workerThread.SetUserData < bool > ( true );
	m_workerThread.Start ( WorkerThread );
}

CWebRequest::~CWebRequest( void )
{
	// Is the worker thread running?
	if ( m_workerThread.IsRunning () )
	{
		// Stop the worker thread
		m_workerThread.SetUserData < bool > ( false );
		m_workerThread.Stop ( false, true );
	}

	// Clear the request queue
	clear ();
	
	// Delete the http client
	SAFE_DELETE ( m_pHttpClient );
}

void CWebRequest::AddToQueue( int iType, String strUrl, int iPort, String strPost, SQObjectPtr pFunction, SQVM * pVM )
{
	// Create a web request struct
	SWebRequest * webRequest = new SWebRequest;
	webRequest->iType = iType;
	webRequest->strUrl = SharedUtility::StripForWebRequest( strUrl );
	webRequest->iPort = iPort;
	webRequest->strPost = strPost;
	webRequest->pFunction = pFunction;
	webRequest->pVM = pVM;
	webRequest->ulTime = 0;

	// Add the request to the queue
	push_back( webRequest );
}

void CWebRequest::Reset ( void )
{
	// Remove the current request from the queue
	if ( m_activeRequest )
		remove ( m_activeRequest );

	// Delete the active request memory
	SAFE_DELETE ( m_activeRequest );

	// Reset the http client
	m_pHttpClient->Reset ();

	// Mark as inactive
	SetActive ( false );
}