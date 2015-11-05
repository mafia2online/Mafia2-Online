/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CTimerManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CTimerManager.h"

void CTimerManager::Pulse( void )
{
	// Loop over all timers
	for( iterator iter = begin(); iter != end(); )
	{
		// Attempt to pulse the timer
		if( !(*iter)->Pulse() )
		{
			delete *iter;
			iter = erase( iter );
		}
		else
			++iter;
	}
}

void CTimerManager::HandleScriptUnload( CSquirrel * pScript )
{
	// Loop over all scripts
	for( iterator iter = begin(); iter != end(); )
	{
		// Does this timer script equal the script?
		if( (*iter)->GetScript() == pScript )
		{
			delete *iter;
			iter = erase( iter );
		}
		else
			++iter;
	}
}

bool CTimerManager::Contains( CTimer * pTimer )
{
	// Loop over all scripts
	for( iterator iter = begin(); iter != end(); iter++ )
	{
		// Does this timer equal the timer?
		if( (*iter) == pTimer )
			return true;
	}

	return false;
}