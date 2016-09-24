/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWebRequest.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<thread>
#include "Network/CHttpClient.h"

struct SWebRequest
{
	int iType;
	String strUrl;
	int iPort;
	String strPost;
	SQObjectPtr pFunction;
	SQVM * pVM;
	unsigned long ulTime;
};

class CWebRequest : public std::list< SWebRequest* >
{

private:

	CHttpClient						* m_pHttpClient;
	bool							m_bActive;
	SWebRequest						* m_activeRequest;

	std::thread						m_thread;
	bool							m_processRequest;

	void							WorkerThread				();
	static	bool					RecieveHandler				( const char * szData, unsigned int uiDataSize, void * pUserData );

public:

									CWebRequest					( void );
									~CWebRequest				( void );

	void							AddToQueue					( int iType, String strUrl, int iPort, String strPost, SQObjectPtr pFunction, SQVM * pVM );

	void							SetActive					( bool bActive ) { m_bActive = bActive; }
	bool							IsActive					( void ) { return m_bActive; }

	void							Reset						( void );

	CHttpClient						* GetHttpClient				( void ) { return m_pHttpClient; }

};
