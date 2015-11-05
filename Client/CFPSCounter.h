/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFPSCounter.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CFPSCounter
{

private:

	unsigned long		m_ulLastTime;
	unsigned int		m_uiCurrentFrames;
	unsigned int		m_uiFramesPerSecond;

public:

	CFPSCounter( void );
	~CFPSCounter( void );

	void		Pulse( void );

	unsigned int		GetFPS( void ) { return m_uiFramesPerSecond; }

};