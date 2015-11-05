/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CMutex.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifdef _WIN32
#include	<windows.h>
#else
#include	<pthread.h>
#endif

#define	USE_CRITICAL_SECTION

class CMutex
{
	
private:

#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	CRITICAL_SECTION 	m_criticalSection;
#else
	HANDLE 				m_hMutex;
#endif
#else
	pthread_mutex_t 	m_mutex;
#endif
	int 				m_iLockCount;

public:

	CMutex( void );
	~CMutex( void );

	void	Lock( void );
	bool	TryLock( unsigned int uiTimeout );
	void	Unlock( void );

};