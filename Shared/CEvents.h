/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CEvents.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<map>
#include	<list>

class CEventHandler
{
public:
	virtual SQVM			* GetScript( void ) const { return 0; }
	virtual SQObjectPtr		GetFunction( void ) const { return (SQInteger)0; }
	virtual void			* GetChunk( void ) const { return 0; };
#ifndef _CLIENT
	//virtual EventHandler_t	GetHandler( void ) const { return 0; };
#endif
	virtual bool			Equals( const CEventHandler * other ) const = 0;
	virtual void			Call( CSquirrelArguments * pArguments, CSquirrelArgument * pReturn ) = 0;
};

class CSquirrelEventHandler : public CEventHandler
{
	SQVM * m_pVM;
	SQObjectPtr m_pFunction;

public:

	CSquirrelEventHandler( SQVM * pVM, SQObjectPtr pFunction )
	{
		m_pVM = pVM;
		m_pFunction = pFunction;
	}

	SQVM * GetScript( void ) const { return m_pVM; }
	SQObjectPtr GetFunction( void ) const { return m_pFunction; }

	bool Equals( const CEventHandler * other ) const
	{
		return (other->GetScript() && other->GetScript() == GetScript() && other->GetFunction()._unVal.pClosure == GetFunction()._unVal.pClosure);
	}

	void Call( CSquirrelArguments * pArguments, CSquirrelArgument * pReturn );
};

/*
#ifndef _CLIENT
class CModuleEventHandler : public CEventHandler
{
	EventHandler_t m_pfnHandler;
public:
	CModuleEventHandler( EventHandler_t pfnHandler )
	{
		m_pfnHandler = pfnHandler;
	}

	bool Equals( const CEventHandler * other ) const
	{
		return (other->GetHandler() && other->GetHandler() == GetHandler());
	}

	void Call( CSquirrelArguments * pArguments, CSquirrelArgument * pReturn )
	{
		m_pfnHandler( pArguments, pReturn );
	}
};
#endif
*/

class CEvents : public std::map< std::string, std::list< CEventHandler *> >
/*#ifndef _CLIENT
	, public CEventsInterface
#endif*/
{

public:

	~CEvents( void )
	{
		clear();
	}

	bool	Add( String strName, CEventHandler * pEventHandler )
	{
		// Is there any events with that name?
		CEvents::iterator iter = find( strName.Get() );

		// Was the event found?
		if( iter != end() )
		{
			// Loop all functions linked to this event
			for( std::list< CEventHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2 )
			{
				// Is the function already added for this event?
				if( pEventHandler->Equals( *iter2 ) )
					return false;
			}
		}
		else
		{
			// Create the event
			std::pair< std::map< std::string, std::list< CEventHandler *> >::iterator , bool > ret;
			ret = insert( std::pair< std::string, std::list< CEventHandler *> >( strName.Get(), std::list< CEventHandler *>( ) ) );

			iter = ret.first;
		}

		// Insert the event handler
		(*iter).second.push_back( pEventHandler );
		return true;
	}

	bool Remove( String strName, CEventHandler * pEventHandler )
	{
		// Is there any events with that name?
		CEvents::iterator iter = find( strName.Get() );

		// Was the event found?
		if( iter != end() )
		{
			// 
			bool bRemoved = false;

			// Loop all event handlers
			for( std::list< CEventHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); )
			{
				// Did we find the function?
				if( pEventHandler->Equals( *iter2 ) )
				{
					// Erase the event handler
					(*iter).second.erase( iter2++ );

					// Mark as removed
					bRemoved = true;

					break;
				}
				else
				{
					// Increase the iterator
					iter2++;
				}
			}

			// Did we remove the event handler?
			if( bRemoved )
			{
				// Is the event list empty?
				if( (*iter).second.size() == 0 )
				{
					// Remove the event
					erase( iter );
				}
			}

			return bRemoved;
		}

		return false;
	}

	CSquirrelArgument	Call( String strName, CSquirrel * pScript = NULL )
	{
		CSquirrelArgument pReturn(1);
		CSquirrelArguments pArguments;
		Call( strName, &pArguments, &pReturn, pScript );
		return pReturn;
	}

	CSquirrelArgument	Call( String strName, CSquirrelArguments * pArguments, CSquirrel * pScript = NULL )
	{
		CSquirrelArgument pReturn(1);
		Call( strName, pArguments, &pReturn, pScript );
		return pReturn;
	}

	void	Call( String strName, CSquirrelArguments * pArguments, CSquirrelArgument * pReturn, CSquirrel * pScript = NULL )
	{
		SQVM * pVM = pScript ? pScript->GetVM() : NULL;

		// Any events with that name?
		CEvents::iterator iter = find(strName.Get());

		if( iter != end() )
		{
			// Loop through all handlers
			for(std::list< CEventHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2)
			{
				// Not for a specific script; or that script is the one we want
				if( !pVM || pVM == (*iter2)->GetScript() )
				{
					(*iter2)->Call( pArguments, pReturn );
				}
			}
		}
	}

	bool	HandleScriptUnload( SQVM * pVM )
	{
		// Loop through all events
		for( CEvents::iterator iter = begin(); iter != end(); )
		{
			// Loop through all event handlers
			for( std::list< CEventHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); )
			{
				if( (*iter2)->GetScript() == pVM )
					(*iter).second.erase( iter2++ );
				else
					iter2++;
			}

			// Remove events without handlers
			if( (*iter).second.size() == 0 )
				erase( iter++ );
			else
				iter++;
		}

		return true;
	}

/*
#ifndef _CLIENT
	bool	AddModuleEvent( const char * szName, EventHandler_t pfnHandler )
	{
		return Add( szName, new CModuleEventHandler( pfnHandler ) );
	}

	bool	RemoveModuleEvent( const char * szName, EventHandler_t pfnHandler )
	{
		return Remove( szName, &CModuleEventHandler( pfnHandler ) );
	}

	void	CallModuleEvent( const char * szName, SquirrelArgumentsInterface * pArguments, SquirrelArgumentInterface * pReturn )
	{
		Call( szName, (CSquirrelArguments *)pArguments, (CSquirrelArgument *)pReturn );
	}
#endif
*/

};
