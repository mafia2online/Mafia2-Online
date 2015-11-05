/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CMutex.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _WIN32
#include	<windows.h>
#endif
#include	"CMutex.h"
#include	"../SharedUtility.h"

CMutex::CMutex( void )
{
	// Create the mutex
#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	InitializeCriticalSection( &m_criticalSection );
#else
	m_hMutex = CreateMutex( NULL, FALSE, NULL );
#endif
#else
	pthread_mutex_init( &m_mutex, NULL );
#endif

	// Reset the lock count
	m_iLockCount = 0;
}

CMutex::~CMutex( void )
{
	// Delete the mutex
#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	DeleteCriticalSection( &m_criticalSection );
#else
	CloseHandle( m_hMutex );
#endif
#else
	pthread_mutex_destroy( &m_mutex );
#endif
}

void CMutex::Lock( void )
{
	// Lock the mutex
#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	EnterCriticalSection( &m_criticalSection );
#else
	WaitForSingleObject( m_hMutex, INFINITE );
#endif
#else
	pthread_mutex_lock( &m_mutex );
#endif

	// Increase the lock count
	m_iLockCount++;
}

bool CMutex::TryLock( unsigned int uiTimeout )
{
	// Attempt to lock the mutex
	bool bLocked = false;

#if defined(_WIN32) && !defined(USE_CRITICAL_SECTION)
	bLocked = (WaitForSingleObject( m_hMutex, uiTimeout ) == 0);
#else
	if( uiTimeout == 0 )
	{
#ifdef _WIN32
		bLocked = (TryEnterCriticalSection( &m_criticalSection ) != 0);
#else
		bLocked = pthread_mutex_trylock(&m_mutex);
#endif
	}
	else
	{
		unsigned long ulEndTime = (SharedUtility::GetTime() + uiTimeout);
		while( SharedUtility::GetTime() < ulEndTime )
		{
#ifdef _WIN32
			if( TryEnterCriticalSection( &m_criticalSection ) )
#else
			if( pthread_mutex_trylock( &m_mutex ) )
#endif
			{
				bLocked = true;
				break;
			}
		}
	}
#endif

	// Did the mutex lock?
	if( bLocked )
	{
		// Increase the lock count
		m_iLockCount++;
	}

	return bLocked;
}

void CMutex::Unlock( void )
{
	// Decreate the lock count
	m_iLockCount--;

	// Unlock the mutex
#ifdef _WIN32
#ifdef USE_CRITICAL_SECTION
	LeaveCriticalSection(&m_criticalSection);
#else
	ReleaseMutex(m_hMutex);
#endif
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}
