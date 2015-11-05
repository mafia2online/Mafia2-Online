/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrelArguments.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CString.h"
#include	"../../Libraries/squirrel/squirrel.h"
#include	<list>
#include	<assert.h>

#include	"../../Libraries/RakNet/Source/BitStream.h"

class CSquirrelArguments;
class CSquirrelArgument
#ifndef _CLIENT
	//: public SquirrelArgumentInterface
#endif
{

private:

	SQObjectType  type;

public:

	union {
		int i;
		bool b;
		float f;
		String * str;
		CSquirrelArguments * pArray;
		SQObject sqObject;
		SQInstance * pInstance;
		void* pUserPointer;
	} data;

	enum {
		NET_SQ_INVALID,
		NET_SQ_NULL,
		NET_SQ_BYTE_POS,
		NET_SQ_BYTE_NEG,
		NET_SQ_SHORT_POS,
		NET_SQ_SHORT_NEG,
		NET_SQ_INT,
		NET_SQ_BOOL_TRUE,
		NET_SQ_BOOL_FALSE,
		NET_SQ_FLOAT,
		NET_SQ_STRING_TINY,
		NET_SQ_STRING_SMALL,
		NET_SQ_STRING,
		NET_SQ_TABLE,
		NET_SQ_ARRAY,
		NET_SQ_USERPOINTER,
		NET_SQ_MAX
	};

							CSquirrelArgument							( void ) { type = OT_NULL; }
							CSquirrelArgument							( int i ) { type = OT_INTEGER; data.i = i; }
							CSquirrelArgument							( unsigned int i ) { type = OT_INTEGER; data.i = (int)i; }
							CSquirrelArgument							( bool b ) { type = OT_BOOL; data.b = b; }
							CSquirrelArgument							( float f ) { type = OT_FLOAT; data.f = f; }
							CSquirrelArgument							( String str ) { type = OT_STRING; data.str = new String(str); }
							CSquirrelArgument							( String * str ) { type = OT_STRING; data.str = str; }
							CSquirrelArgument							( CSquirrelArguments array, bool isArray );
							CSquirrelArgument							( CSquirrelArguments * pArray, bool isArray ) { type = (isArray ? OT_ARRAY : OT_TABLE); data.pArray = pArray; }
							CSquirrelArgument							( SQObject o );
							CSquirrelArgument							( void* pUserPointer ) { type = OT_USERPOINTER; data.pUserPointer = pUserPointer; }
							CSquirrelArgument							( SQInstance * pInstance ) { }
							CSquirrelArgument							( RakNet::BitStream * bitStream );
							CSquirrelArgument							( const CSquirrelArgument& p );
							~CSquirrelArgument							( void );

	SQObjectType			GetType										( void ) const { return type; }
	void					reset										( void );

	bool					push										( SQVM * pVM );
	bool					pushFromStack								( SQVM * pVM, int idx );

	void					Serialise									( RakNet::BitStream * bitStream );
	void					Deserialise									( RakNet::BitStream * bitStream );

	void					set											( const CSquirrelArgument& p );
	void					SetNull										( void )				{ reset(); type = OT_NULL; }
	void					SetInteger									( int i )			{ reset(); type = OT_INTEGER; data.i = i; }
	void					SetInteger									( unsigned int i ) { reset(); type = OT_INTEGER; data.i = (int)i; }
	void					SetBool										( bool b )			{ reset(); type = OT_BOOL; data.b = b; }
	void					SetFloat									( float f )			{ reset(); type = OT_FLOAT; data.f = f; }
	void					SetString									( const char * s ) { reset(); type = OT_STRING; data.str = new String(s); }
	void					SetArray									( CSquirrelArguments * pArray ) { reset(); type = OT_ARRAY; data.pArray = pArray; }
	void					SetTable									( CSquirrelArguments * pTable ) { reset(); type = OT_TABLE; data.pArray = pTable; }
	void					SetInstance									( SQInstance * pInstance ) { reset(); type = OT_INSTANCE; data.pInstance = pInstance; }
	void					SetUserPointer								( void* pUserPointer ) { reset(); type = OT_USERPOINTER; data.pUserPointer = pUserPointer; }

	int						GetInteger									( void )	const { return type == OT_INTEGER ? data.i : 0; }
	bool					GetBool										( void )	const { return type == OT_BOOL    ? data.b : false; }
	float					GetFloat									( void )	const { return type == OT_FLOAT   ? data.f : 0.0f; }
	const char				* GetString									( void )	const { return type == OT_STRING  ? data.str->Get() : NULL; }
	CSquirrelArguments		* GetTable									( void )	const { return type == OT_TABLE ? data.pArray : NULL; }
	CSquirrelArguments		* GetArray									( void )	const { return type == OT_ARRAY ? data.pArray : NULL; }
	SQInstance				* GetInstance								( void )	const { return type == OT_INSTANCE ? data.pInstance : NULL; }
	void					* GetUserPointer							( void )	const { return type == OT_USERPOINTER ? data.pUserPointer : NULL; }

};

class CSquirrelArguments : public std::list< CSquirrelArgument *>
#ifndef _CLIENT
	//, public SquirrelArgumentsInterface
#endif
{

public:

							CSquirrelArguments							( void ) { };
							CSquirrelArguments							( SQVM * pVM, int idx );
							CSquirrelArguments							( RakNet::BitStream * bitStream );
							CSquirrelArguments							( const CSquirrelArguments& p );
							~CSquirrelArguments							( void );

	void					reset										( void );

	void					push_to_vm									( SQVM* pVM );

	void					push										( void );
	void					pushObject									( SQObject o );
	void					pushUserPointer								( void* pUserPointer );
	void					push										( int i );
	void					push										( unsigned int i );
	void					push										( bool b );
	void					push										( float f );
	void					push										( const char* c );
	void					push										( String str );
	void				    push										( CSquirrelArguments array, bool isArray );
	void					push										( CSquirrelArguments * pArray, bool isArray );
	bool				    pushFromStack								( SQVM* pVM, int idx );

	CSquirrelArgument 		pop											( void );

	void					Serialise									( RakNet::BitStream * bitStream );
	void					Deserialise									( RakNet::BitStream * bitStream );

#ifndef _CLIENT
	//SquirrelArgumentInterface * Get										( unsigned int i ) const;
	unsigned int			GetSize										( void ) const { return size(); }
	//SquirrelArgumentInterface * Add										( void ) { push(); return back(); }
	void					Remove										( void ) { pop_back(); }
#endif

};