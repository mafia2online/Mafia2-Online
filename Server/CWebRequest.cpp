/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWebRequest.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/
#include	<thread>
#include "StdInc.h"
#include "CCore.h"

#include "CWebRequest.h"

void CWebRequest::WorkerThread()
{
	while (m_processRequest)
	{
		if ( !IsActive () )
		{
			if ( size () )
			{
				SWebRequest* pCurrent = front ();
				m_activeRequest = pCurrent;
				pCurrent->ulTime = SharedUtility::GetTime();
				SetActive ( true );
				m_pHttpClient->SetHost ( pCurrent->strUrl );
				m_pHttpClient->SetPort ( pCurrent->iPort );

				switch( pCurrent->iType )
				{
				case HTTP_TYPE_GET:
					{
						m_pHttpClient->Get ( pCurrent->strPost );
						break;
					}

				case HTTP_TYPE_POST:
					{
						m_pHttpClient->Post ( false, pCurrent->strPost );
						break;
					}
				}

				if ( pCurrent->iType == HTTP_TYPE_POST )
				{
					Reset ();
				}
			}
		}
		else
		{
			if ( m_pHttpClient->IsBusy () )
			{
				m_pHttpClient->Process ();
				if ( m_pHttpClient->GotData () )
				{
					CSquirrelArguments args;
					args.push( m_activeRequest->iType );
					args.push( m_pHttpClient->GetData()->Get() );
					args.push( (int)(SharedUtility::GetTime() - m_activeRequest->ulTime) );
					CCore::Instance()->GetScriptingManager()->Call( m_activeRequest->pFunction, &args, m_activeRequest->pVM );

					Reset ();
				}
			}
		}

		Sleep ( 100 );
	}
}

bool CWebRequest::RecieveHandler ( const char * szData, unsigned int uiDataSize, void * pUserData )
{
	return true;
}

CWebRequest::CWebRequest()
{
	m_bActive = false;

	m_pHttpClient = new CHttpClient ();
	m_pHttpClient->SetReceiveHandle ( RecieveHandler, this );

	m_thread = std::thread(&CWebRequest::WorkerThread, this);
	m_processRequest = true;
}

CWebRequest::~CWebRequest()
{
	if ( m_thread.joinable () )
	{
		m_processRequest = false;
		m_thread.join();
	}
	clear ();
	SAFE_DELETE ( m_pHttpClient );
}

void CWebRequest::AddToQueue( int iType, String strUrl, int iPort, String strPost, SQObjectPtr pFunction, SQVM * pVM )
{
	SWebRequest * webRequest = new SWebRequest;
	webRequest->iType = iType;
	webRequest->strUrl = SharedUtility::StripForWebRequest( strUrl );
	webRequest->iPort = iPort;
	webRequest->strPost = strPost;
	webRequest->pFunction = pFunction;
	webRequest->pVM = pVM;
	webRequest->ulTime = 0;

	push_back( webRequest );
}

void CWebRequest::Reset ()
{
	if ( m_activeRequest )
		remove ( m_activeRequest );

	SAFE_DELETE ( m_activeRequest );
	m_pHttpClient->Reset ();
	SetActive ( false );
}