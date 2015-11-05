/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CCommands.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<map>
#include	<list>

class CCommandHandler
{
public:
	virtual SQVM			* GetScript( void ) const { return 0; }
	virtual SQObjectPtr		GetFunction( void ) const { return (SQInteger)0; }
	virtual void			* GetChunk( void ) const { return 0; };
	virtual bool			Equals( const CCommandHandler * other ) const = 0;
	virtual void			Call( CSquirrelArguments * pArguments ) = 0;
};

class CSquirrelCommandHandler : public CCommandHandler
{

	SQVM * m_pVM;
	SQObjectPtr m_pFunction;

public:

	CSquirrelCommandHandler( SQVM * pVM, SQObjectPtr pFunction )
	{
		m_pVM = pVM;
		m_pFunction = pFunction;
	}

	SQVM * GetScript( void ) const { return m_pVM; }
	SQObjectPtr GetFunction( void ) const { return m_pFunction; }

	bool Equals( const CCommandHandler * other ) const
	{
		return (other->GetScript() && other->GetScript() == GetScript() && other->GetFunction()._unVal.pClosure == GetFunction()._unVal.pClosure);
	}

	void Call( CSquirrelArguments * pArguments );

};

class CCommands : public std::map< std::string, std::list< CCommandHandler *> >
{

public:

	~CCommands( void )
	{
		// Clear all the command handlers
		clear();
	}

	bool	Add( String strCommand, CCommandHandler * pCommandHandler )
	{
		// Is there any commands with that name?
		CCommands::iterator iter = find( strCommand.Get() );

		// Was the command found?
		if( iter != end() )
		{
			// Loop all functions linked to this command
			for( std::list< CCommandHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2 )
			{
				// Is this function already added for this command?
				if( pCommandHandler->Equals( *iter2 ) )
					return false;
			}
		}
		else
		{
			// Create the command
			std::pair< std::map< std::string, std::list< CCommandHandler *> >::iterator , bool > ret;
			ret = insert( std::pair< std::string, std::list< CCommandHandler *> >( strCommand.Get(), std::list< CCommandHandler *>( ) ) );

			iter = ret.first;
		}

		// Insert the command handler
		(*iter).second.push_back( pCommandHandler );
		return true;
	}

	bool	HandleCommand( EntityId playerId, String strInput, CSquirrel * pScript = NULL )
	{
		// Was the input empty?
		if( strInput.IsEmpty() )
			return false;

		// Get the script vm
		SQVM * pVM = pScript ? pScript->GetVM() : NULL;

		// Get the command and params
		size_t sOffset = strInput.Find( " ", 0 );
		String strCommand = strInput.substr( 0, sOffset++ );
		String strParams = strInput.substr( sOffset, strInput.GetLength() );

		// Create a list of params
		CSquirrelArguments pArguments;

		// Push the player id
		pArguments.push( playerId );

		// No params?
		if( sOffset == 0 )
		{
			// Reset the params
			strParams.Truncate( 0 );
		}
		else
		{
			//
			char * szTempArguments = new char[ strParams.GetLength () + 1 ];
			strcpy ( szTempArguments, strParams.Get () );
			char * arg = strtok ( szTempArguments, " " );

			// Loop over all spaces
			while ( arg )
			{
				pArguments.push ( arg );
				arg = strtok ( NULL, " " );
			}

			// Cleanup
			delete [] szTempArguments;
		}

		// Is there any commands with that name?
		CCommands::iterator iter = find( strCommand.Get() );

		// Was the command found?
		if( iter != end() )
		{
			// Loop through all handlers
			for( std::list< CCommandHandler *>::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2 )
			{
				if( !pVM || pVM == (*iter2)->GetScript() )
				{
					(*iter2)->Call( &pArguments );
				}
			}
		}

		return true;
	}

};