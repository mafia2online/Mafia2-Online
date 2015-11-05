/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CTimerNatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CTimerNatives.h"
#include	"../../Timers/CTimerManager.h"
#include	"../../Timers/CTimer.h"

#ifdef _CLIENT
#include	"../../../Client/StdInc.h"
#else
#include	"../../../Server/StdInc.h"
#endif

extern	CCore			* pCore;

_MEMBER_FUNCTION_IMPL(timer, constructor);
_MEMBER_FUNCTION_IMPL(timer, IsActive);
_MEMBER_FUNCTION_IMPL(timer, Kill);

_BEGIN_CLASS(timer)
_MEMBER_FUNCTION(timer, constructor, -1, NULL)
_MEMBER_FUNCTION(timer, IsActive, 0, NULL)
_MEMBER_FUNCTION(timer, Kill, 0, NULL)
_END_CLASS(timer)

void CTimerNatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->NewClass( &_CLASS_DECL(timer) );
}

_MEMBER_FUNCTION_RELEASE_HOOK(timer)
{
	// Get the timer instance
	CTimer * pTimer = (CTimer *)pInst;

	// Is the timer not null and inside the timer manager?
	if( pTimer != NULL && pCore->GetTimerManager()->Contains( pTimer ) )
	{
		// Kill the timer
		pTimer->Kill( );
	}

	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, constructor)
{
	// Check the param count
	CHECK_PARAMS_MIN("timer", 3);

	//
	if( sq_gettype( pVM, 2 ) != OT_NATIVECLOSURE )
		CHECK_TYPE("timer", 1, 2, OT_CLOSURE);

	// Check the params
	CHECK_TYPE("timer", 2, 3, OT_INTEGER);
	CHECK_TYPE("timer", 3, 4, OT_INTEGER);

	SQInteger iInterval = 0;
	SQInteger iRepeations = 0;

	// Get the params
	sq_getinteger( pVM, 3, &iInterval );
	sq_getinteger( pVM, 4, &iRepeations );

	// Invalid repeations?
	if( iRepeations < -1 || iRepeations == 0 )
	{
		sq_pushbool( pVM, false );
		return 1;
	}

	// Get the timer end function
	SQObjectPtr pFunction = stack_get( pVM, 2 );

	// Create the arguments
	CSquirrelArguments * pArguments = new CSquirrelArguments( pVM, 5 );

	// Get the current script instance
#ifdef _CLIENT
	CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM );
#else
	CSquirrel * pScript = pCore->GetScriptingManager()->Get( pVM );
#endif

	// Create the timer instance
	CTimer * pTimer = new CTimer( pScript, pFunction, iInterval, iRepeations, pArguments );

	// Did it fail to set the timer instance?
	if( SQ_FAILED( sq_setinstance( pVM, pTimer) ) )
	{
		// Delete the timer
		SAFE_DELETE( pTimer );

		sq_pushbool( pVM, false );
		return 1;
	}

	// Add the timer into the manager
	pCore->GetTimerManager()->push_back( pTimer );

	sq_pushbool( pVM, true );
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, IsActive)
{
	// Get the timer instance
	CTimer * pTimer = sq_getinstance< CTimer *>( pVM );

	// Is the timer invalid?
	if( !pTimer )
	{
		sq_pushbool( pVM, false );
		return 1;
	}

	// Is the timer inside the timer manager?
	if( pCore->GetTimerManager()->Contains( pTimer ) )
	{
		// Is the timer not active?
		if( !pTimer->IsActive() )
		{
			//
			sq_setinstance( pVM, NULL );
			sq_pushbool( pVM, false );
			return 1;
		}

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, Kill)
{
	// Get the timer instance
	CTimer * pTimer = sq_getinstance< CTimer *>( pVM );

	// Is the timer invalid?
	if( !pTimer )
	{
		sq_pushbool( pVM, false );
		return 1;
	}

	// Is the timer inside the timer manager and active?
	if( pCore->GetTimerManager()->Contains( pTimer ) && pTimer->IsActive() )
	{
		// Kill the timer
		pTimer->Kill( );

		sq_setinstance( pVM, NULL );
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}