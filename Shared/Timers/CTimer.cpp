/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CTimer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CTimer.h"
#include	"../SharedUtility.h"

CTimer::CTimer(CSquirrel * pSquirrel, SQObjectPtr pFunction, int iInterval, int iRepeations, CSquirrelArguments * pArguments)
{
	m_bIsActive = true;
	m_pSquirrel = pSquirrel;
	m_pFunction = pFunction;
	m_iInterval = iInterval;
	m_iRepeations = iRepeations;
	m_pArguments = pArguments;
	m_uiLastTick = SharedUtility::GetTime();
}

CTimer::~CTimer( void )
{
	// Do we have any arguments?
	if( m_pArguments )
	{
		// Delete the arguments instance
		SAFE_DELETE( m_pArguments );
	}
}

bool CTimer::Pulse( void )
{
	// Is the timer not active?
	if( !IsActive() )
		return false;

	// Max number of itterations
	unsigned int uiCount = 10;

	// Get the current time
	unsigned int uiCurrentTime = SharedUtility::GetTime( );

	// Call the timer function
	while( uiCurrentTime >= m_uiLastTick + m_iInterval && uiCount -- > 0 )
	{
		// Call the function
		m_pSquirrel->Call( m_pFunction, m_pArguments );

		// Update the last tick count
		m_uiLastTick = (m_uiLastTick + m_iInterval);

		// Do we have any more repeations to do?
		if( m_iRepeations > 0 )
		{
			// Last repeation?
			if( --m_iRepeations == 0 )
				return false;
		}
	}

	return true;
}