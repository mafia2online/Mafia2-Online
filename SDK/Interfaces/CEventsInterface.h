/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CEventsInterface.h
*
***************************************************************/

#pragma once

class SquirrelArgumentInterface
{
public:
	virtual SQObjectType					GetType( void ) const = 0;
			bool							IsNull( void ) const { return GetType() == OT_NULL; };
	virtual	int								GetInteger( void ) const = 0;
	virtual bool							GetBool( void ) const = 0;
	virtual float							GetFloat( void ) const = 0;
	virtual const char						* GetString( void ) const = 0;

	virtual	void							SetNull( void ) = 0;
	virtual void							SetInteger( int i ) = 0;
	virtual void							SetBool( bool b ) = 0;
	virtual void							SetFloat( float f ) = 0;
	virtual void							SetString( const char * s ) = 0;
};

class SquirrelArgumentsInterface
{
public:
	virtual	SquirrelArgumentInterface		* Get( unsigned int i ) const = 0;
	virtual unsigned int					GetSize( void ) const = 0;
	virtual SquirrelArgumentInterface		* Add( void ) = 0;
	virtual void							Remove( void ) = 0;
};

typedef void (* EventHandler_t)( SquirrelArgumentsInterface * pArguments, SquirrelArgumentInterface * pReturn );

class CEventsInterface
{
public:
	virtual bool							AddModuleEvent( const char * szName, EventHandler_t pfnHandler ) = 0;
	virtual bool							RemoveModuleEvent( const char * szName, EventHandler_t pfnHandler ) = 0;
	virtual void							CallModuleEvent( const char * szName, SquirrelArgumentsInterface * pArguments, SquirrelArgumentInterface * pReturn ) = 0;
};