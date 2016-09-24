/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFPSCounter.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CFPSCounter.h"
#include "CCore.h"
#include "SharedUtility.h"

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
	m_uiCurrentFrames++;
	unsigned long ulCurrentTime = SharedUtility::GetTime( );

	if( (ulCurrentTime - m_ulLastTime) >= 1000 )
	{
		m_uiFramesPerSecond = m_uiCurrentFrames;
		m_uiCurrentFrames = 0;
		m_ulLastTime = ulCurrentTime;
	}
}