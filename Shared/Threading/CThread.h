/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CThread.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifdef _LINUX
#include <pthread.h>
#endif

#include "CMutex.h"

class CThread
{

private:

	typedef void (* ThreadFunction_t)( CThread * pCreator );

#ifdef _WIN32
	HANDLE					m_hThread;
#else
	pthread_t				m_thread;
#endif
	ThreadFunction_t		m_pfnThreadFunction;
	CMutex					m_startedMutex;
	bool					m_bStarted;
	CMutex					m_runningMutex;
	bool					m_bRunning;
	CMutex					m_userDataMutex;
	void					* m_pUserData;

	void					SetStarted( bool bStarted );
	bool					IsStarted( void );
	void					SetRunning( bool bRunning );

public:

	CThread( void );
	~CThread( void );

	void        Start(ThreadFunction_t pfnThreadFunction, bool bWaitForStart = true);
	bool        Stop(bool bWaitForExit = true, bool bTerminate = false);
	bool        IsRunning();

	template <typename DataType>
	void        SetUserData(DataType pUserData)
	{
		// Lock the user data mutex
		m_userDataMutex.Lock();

		// Set the user data
		m_pUserData = (void *)pUserData;

		// Unlock the user data mutex
		m_userDataMutex.Unlock();
	}

	template <typename ReturnType>
	ReturnType  GetUserData()
	{
		// Lock the user data mutex
		m_userDataMutex.Lock();

		// Get the user data
#ifdef _MSC_VER
#pragma warning(disable:4800) // forcing value to bool (performance warning)
#endif
		ReturnType pUserData = (ReturnType)m_pUserData;

		// Unlock the user data mutex
		m_userDataMutex.Unlock();

		return pUserData;
	}

#ifdef _WIN32
	static void StartAddress(CThread * pThis);
#else
	static void * StartAddress(void * pThis);
#endif

};