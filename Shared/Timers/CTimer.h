/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CTimer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Scripting/CSquirrel.h"
#include	"../Scripting/CSquirrelArguments.h"

class CTimer
{

private:

	CSquirrel				* m_pSquirrel;
	SQObjectPtr				m_pFunction;
	int						m_iInterval;
	int						m_iRepeations;
	CSquirrelArguments		* m_pArguments;
	unsigned int			m_uiLastTick;
	bool					m_bIsActive;

public:

	CTimer( CSquirrel * pSquirrel, SQObjectPtr pFunction, int iInterval, int iRepeations, CSquirrelArguments * pArguments );
	~CTimer( void );

	bool					Pulse( void );
	CSquirrel				* GetScript( void ) { return m_pSquirrel; }
	void					Kill( void ) { m_bIsActive = false; }
	bool					IsActive( void ) { return m_bIsActive; }

};