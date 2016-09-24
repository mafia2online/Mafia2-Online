/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CMasterList.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <thread>

#include "TCPInterface.h"
#include "RakSleep.h"

#include "Threading/CThread.h"

enum eThreadState
{
	E_STATE_ADD,
	E_STATE_UPDATE,
	E_STATE_DELETE
};

class CMasterList
{

private:

	unsigned short					m_usPort;
	unsigned long					m_ulLastUpdateTime;
	eThreadState					m_threadState;
	bool							m_bIsAdded;
	RakNet::TCPInterface			* tcp;

	String							m_sAuthKey;

	std::thread						m_thread;
	bool							m_processQueries;

	void							WorkerThread						();

public:

									CMasterList							( void );
									~CMasterList						( void );

	void							SetServerPort						( unsigned short usPort ) { m_usPort = usPort; }
	unsigned short					GetServerPort						( void ) { return m_usPort; }
	void							SetThreadState						( eThreadState state ) { m_threadState = state; }
	eThreadState					GetThreadState						( void ) { return m_threadState; }
	void							SetLastUpdateTime					( unsigned long ulTime ) { m_ulLastUpdateTime = ulTime; }
	unsigned long					GetLastUpdateTime					( void ) { return m_ulLastUpdateTime; }
	void							SetIsAdded							( bool b ) { m_bIsAdded = b; }
	bool							IsAdded								( void ) { return m_bIsAdded; }

	RakNet::TCPInterface			* GetTCPInterface					( void ) { return tcp; }

	void							SetAuthKey							(String key){ m_sAuthKey = key; }
	String							GetAuthKey							(void){ return m_sAuthKey; }

};
