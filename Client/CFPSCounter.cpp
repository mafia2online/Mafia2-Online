/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFPSCounter.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CFPSCounter::CFPSCounter( void )
{
	m_ulLastTime = 0;
	m_uiCurrentFrames = 0;
	m_uiFramesPerSecond = 0;
}

CFPSCounter::~CFPSCounter( void )
{

}

void CFPSCounter::Pulse( void )
{
	// Increase the current frame count
	m_uiCurrentFrames++;

	// Get the current time
	unsigned long ulCurrentTime = SharedUtility::GetTime( );

	// Has 1 second passed?
	if( (ulCurrentTime - m_ulLastTime) >= 1000 )
	{
		// Set the total FPS count
		m_uiFramesPerSecond = m_uiCurrentFrames;

		// Reset the current frame count
		m_uiCurrentFrames = 0;

		// Set the last time
		m_ulLastTime = ulCurrentTime;
	}
}