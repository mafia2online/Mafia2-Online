/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CSquirrelArguments.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#ifdef _CLIENT
#include	"../../Client/StdInc.h"
#else
#include	"../../Server/StdInc.h"
#endif

#include	"../../Libraries/squirrel/sqstate.h"
#include	"../../Libraries/squirrel/sqvm.h"
#include	"../../Libraries/squirrel/sqstring.h"

CSquirrelArgument::CSquirrelArgument( CSquirrelArguments array, bool isArray )
{
	type = (isArray ? OT_ARRAY : OT_TABLE);
	data.pArray = new CSquirrelArguments( array );
}

CSquirrelArgument::CSquirrelArgument( SQObject o )
{
	type = o._type;

	switch( type )
	{
	case OT_INTEGER:
		data.i = o._unVal.nInteger;
		break;
	case OT_BOOL:
		data.b = (o._unVal.nInteger != 0);
		break;
	case OT_FLOAT:
		data.f = o._unVal.fFloat;
	case OT_STRING:
		data.str = new String(o._unVal.pString->_val);
	case OT_NATIVECLOSURE:
	case OT_CLOSURE:
		data.sqObject = o;
	case OT_INSTANCE:
		data.pInstance = o._unVal.pInstance;
		break;
	}
}

CSquirrelArgument::CSquirrelArgument( RakNet::BitStream * bitStream )
{
	Deserialise( bitStream );
}

CSquirrelArgument::CSquirrelArgument( const CSquirrelArgument& p )
{
	set( p );
}

CSquirrelArgument::~CSquirrelArgument( )
{
	reset( );
}

void CSquirrelArgument::reset( )
{
	if(type == OT_STRING)
		delete data.str;
	else if(type == OT_ARRAY || type == OT_TABLE)
		delete data.pArray;

	type = OT_NULL;
}

bool CSquirrelArgument::push( SQVM * pVM )
{
	switch(type)
	{
		case OT_NULL:
			sq_pushnull(pVM);
			break;
		case OT_INTEGER:
			sq_pushinteger(pVM, data.i);
			break;
		case OT_BOOL:
			sq_pushbool(pVM, data.b);
			break;
		case OT_FLOAT:
			sq_pushfloat(pVM, data.f);
			break;
		case OT_STRING:
			sq_pushstring(pVM, data.str->Get(), data.str->GetLength());
			break;
		case OT_ARRAY:
			{
				sq_newarray(pVM, 0);

				for(CSquirrelArguments::iterator iter = data.pArray->begin(); iter != data.pArray->end(); iter++)
				{
					(*iter)->push(pVM);
					sq_arrayappend(pVM, -2);
				}
				break;
			}
		case OT_TABLE:
			{
				assert(data.pArray->size() % 2 == 0);
				sq_newtable(pVM);

				for(CSquirrelArguments::iterator iter = data.pArray->begin(); iter != data.pArray->end(); iter++)
				{
					(*iter)->push(pVM);
					++iter;
					(*iter)->push(pVM);
					sq_createslot(pVM, -3);
				}
				break;
			}
			break;
		case OT_CLOSURE:
		case OT_NATIVECLOSURE:
			sq_pushobject(pVM, data.sqObject);
			break;
		case OT_INSTANCE:
			{
				SQObject obj;
				obj._type = OT_INSTANCE;
				obj._unVal.pInstance = data.pInstance;
				sq_pushobject(pVM, obj);
			}
			break;
		case OT_USERPOINTER:
			sq_pushuserpointer( pVM, (SQUserPointer *)data.pUserPointer );
			break;
		default:
			sq_pushnull(pVM);
			assert(0);
			return false;
	}
	return true;
}

bool CSquirrelArgument::pushFromStack( SQVM * pVM, int idx )
{
	SQObjectType _type = sq_gettype(pVM, idx);

	switch(_type)
	{
	case OT_NULL:
		SetNull();
		break;
	case OT_INTEGER:
		{
			SQInteger i;
			sq_getinteger(pVM, idx, &i);
			SetInteger((int)i);
		}
		break;
	case OT_BOOL:
		{
			SQBool b;
			sq_getbool(pVM, idx, &b);
			SetBool(b != 0);
		}
		break;
	case OT_FLOAT:
		{
			float f;
			sq_getfloat(pVM, idx, &f);
			SetFloat(f);
		}
		break;
	case OT_STRING:
		{
			const char* szTemp;
			sq_getstring(pVM, idx, &szTemp);
			SetString(szTemp);
		}
		break;
	case OT_TABLE:
		{
			CSquirrelArguments* pArguments = new CSquirrelArguments();
			sq_push(pVM, idx);
			sq_pushnull(pVM);

			while(SQ_SUCCEEDED(sq_next(pVM,-2)))
			{
				if(!pArguments->pushFromStack(pVM, -2) || !pArguments->pushFromStack(pVM,-1))
				{
					sq_pop(pVM,4);
					delete pArguments;
					return false;
				}
				sq_pop(pVM,2);
			}

			sq_pop(pVM,2);
			SetTable(pArguments);
		}
		break;
	case OT_ARRAY:
		{
			CSquirrelArguments* pArguments = new CSquirrelArguments();
			sq_push(pVM, idx);
			sq_pushnull(pVM);

			while(SQ_SUCCEEDED(sq_next(pVM,-2)))
			{
				if(!pArguments->pushFromStack(pVM,-1))
				{
					sq_pop(pVM,4);
					delete pArguments;
					return false;
				}
				sq_pop(pVM,2);
			}

			sq_pop(pVM,2);
			SetArray(pArguments);
		}
		break;
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
		{
			type = _type;
			data.sqObject = SQObject(stack_get(pVM, idx));
		}
		break;
	default:
		return false;
	}

	return true;
}

void CSquirrelArgument::Serialise( RakNet::BitStream * bitStream )
{
	switch( type )
	{
	case OT_NULL:
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
		bitStream->Write( (unsigned char)NET_SQ_NULL );
		break;

	case OT_INTEGER:
		if( data.i > 0 )
		{
			if( data.i <= (0xFF - NET_SQ_MAX) )
			{
				bitStream->Write( (unsigned char)(data.i + NET_SQ_MAX) );
			}
			else if( data.i < (0x200 - NET_SQ_MAX) )
			{
				bitStream->Write( (unsigned char)NET_SQ_BYTE_POS );
				bitStream->Write( (unsigned char)(data.i - 0x100 + NET_SQ_MAX) );
			}
			else if( data.i < (0x10200 - NET_SQ_MAX) )
			{
				bitStream->Write( (unsigned char)NET_SQ_SHORT_POS );
				bitStream->Write( (unsigned char)(data.i - 0x10200 + NET_SQ_MAX) );
			}
			else
			{
				bitStream->Write( (unsigned char)NET_SQ_INT );
				bitStream->Write( data.i );
			}
		}
		else
		{
			if( data.i > -0xFF )
			{
				bitStream->Write( (unsigned char)NET_SQ_BYTE_NEG );
				bitStream->Write( (unsigned char)(-data.i) );
			}
			else if( data.i >= -0xFFFF )
			{
				bitStream->Write( (unsigned char)NET_SQ_SHORT_NEG );
				bitStream->Write( (unsigned char)(-data.i) );
			}
			else
			{
				bitStream->Write( (unsigned char)NET_SQ_INT );
				bitStream->Write( data.i );
			}
		}
		break;

	case OT_BOOL:
		bitStream->Write( (unsigned char)(data.b ? NET_SQ_BOOL_TRUE : NET_SQ_BOOL_FALSE) );
		break;

	case OT_FLOAT:
		bitStream->Write( (unsigned char)NET_SQ_FLOAT );
		bitStream->Write( data.f );
		break;

	case OT_STRING:
		{
			size_t sSize = data.str->GetLength();
			if( sSize <= 0xFF )
			{
				bitStream->Write( (unsigned char)NET_SQ_STRING_TINY );
				bitStream->Write( (unsigned char)sSize );
			}
			else if( sSize <= 0xFFFF )
			{
				bitStream->Write( (unsigned char)NET_SQ_STRING_SMALL );
				bitStream->Write( (unsigned short)sSize );
			}
			else
			{
				bitStream->Write( (unsigned char)NET_SQ_STRING );
				bitStream->Write( sSize );
			}

			bitStream->Write( data.str->Get(), data.str->GetLength() );
		}
		break;

	case OT_ARRAY:
	case OT_TABLE:
		bitStream->Write( (unsigned char)(type == OT_ARRAY ? NET_SQ_ARRAY : NET_SQ_TABLE) );
		break;

	default:
		assert( 0 && "SquirrelArguments to BitStream: Tried to serialise an unknown data type." );
	}
}

void CSquirrelArgument::Deserialise( RakNet::BitStream * bitStream )
{
	unsigned char type;
	bitStream->Read( type );

	if( type >= NET_SQ_MAX )
	{
		this->type = OT_INTEGER;
		data.i = (type - NET_SQ_MAX);
	}
	else
	{
		switch( type )
		{
		case NET_SQ_NULL:
			this->type = OT_NULL;
			break;

		case NET_SQ_BYTE_POS:
			{
				this->type = OT_INTEGER;
				unsigned char temp;
				bitStream->Read( temp );
				data.i = (0x100 - NET_SQ_MAX + temp);
			}
			break;

		case NET_SQ_BYTE_NEG:
			{
				this->type = OT_INTEGER;
				unsigned char temp;
				bitStream->Read( temp );
				data.i = -(int)temp;
			}
			break;

		case NET_SQ_SHORT_POS:
			{
				this->type = OT_INTEGER;
				unsigned short temp;
				bitStream->Read( temp );
				data.i = (0x200 - NET_SQ_MAX + temp);
			}
			break;

		case NET_SQ_SHORT_NEG:
			{
				this->type = OT_INTEGER;
				unsigned short temp;
				bitStream->Read( temp );
				data.i = -(int)temp;
			}
			break;

		case NET_SQ_INT:
			{
				this->type = OT_INTEGER;
				bitStream->Read( data.i );
			}
			break;

		case NET_SQ_FLOAT:
			{
				this->type = OT_FLOAT;
				bitStream->Read( data.f );
			}
			break;

		case NET_SQ_BOOL_TRUE:
		case NET_SQ_BOOL_FALSE:
			{
				this->type = OT_BOOL;
				data.b = (type == NET_SQ_BOOL_TRUE);
			}
			break;

		case NET_SQ_STRING_TINY:
		case NET_SQ_STRING_SMALL:
		case NET_SQ_STRING:
			{
				this->type = OT_STRING;
				size_t size = 0;

				if( type == NET_SQ_STRING_TINY )
				{
					unsigned char temp;
					bitStream->Read( temp );
					size = temp;
				}
				else if( type == NET_SQ_STRING_SMALL )
				{
					unsigned short temp;
					bitStream->Read( temp );
					size = temp;
				}
				else
					bitStream->Read( size );

				data.str = new String();
				data.str->Resize( size );
				char * szString = data.str->GetData();
				bitStream->Read( szString, size );
				data.str->Truncate( size );
			}
			break;

		case NET_SQ_TABLE:
			this->type = OT_TABLE;
			data.pArray = new CSquirrelArguments( bitStream );
			break;

		case NET_SQ_ARRAY:
			this->type = OT_ARRAY;
			data.pArray = new CSquirrelArguments( bitStream );
			break;

		default:
			assert( 0 && "BitStream to SquirrelArguments: Tried to deserialise an unknown data type." );
		}
	}
}

void CSquirrelArgument::set( const CSquirrelArgument& p )
{
	type = p.type;

	switch(type)
	{
	case OT_NULL:
		break;
	case OT_INTEGER:
		data.i = p.data.i;
		break;
	case OT_BOOL:
		data.b = p.data.b;
		break;
	case OT_FLOAT:
		data.f = p.data.f;
		break;
	case OT_STRING:
		data.str = new String(p.data.str->Get());
		break;
	case OT_ARRAY:
	case OT_TABLE:
		data.pArray = new CSquirrelArguments(*p.data.pArray);
		break;
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
		data.sqObject = p.data.sqObject;
		break;
	case OT_INSTANCE:
		data.pInstance = p.data.pInstance;
		break;
	}
}

CSquirrelArguments::CSquirrelArguments( SQVM * pVM, int idx )
{
	for(int i = idx; i <= sq_gettop(pVM); i++)
		pushFromStack(pVM, i);
}

CSquirrelArguments::CSquirrelArguments( RakNet::BitStream * bitStream )
{
	Deserialise( bitStream );
}

CSquirrelArguments::CSquirrelArguments( const CSquirrelArguments& p )
{
	for(CSquirrelArguments::const_iterator iter = p.begin(); iter != p.end(); ++ iter)
		push_back(new CSquirrelArgument(**iter));
}

CSquirrelArguments::~CSquirrelArguments()
{
	reset();
}

void CSquirrelArguments::reset()
{
	for(iterator iter = begin(); iter != end(); iter++)
		delete *iter;

	clear();
}

void CSquirrelArguments::push_to_vm(SQVM* pVM)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
		(*iter)->push(pVM);
}

void CSquirrelArguments::push()
{
	push_back(new CSquirrelArgument());
}

void CSquirrelArguments::pushObject(SQObject o)
{
	push_back(new CSquirrelArgument(o));
}

void CSquirrelArguments::pushUserPointer( void* pUserPointer )
{
	push_back(new CSquirrelArgument(pUserPointer));
}

void CSquirrelArguments::push(int i)
{
	push_back(new CSquirrelArgument(i));
}

void CSquirrelArguments::push(unsigned int i)
{
	push_back(new CSquirrelArgument(i));
}

void CSquirrelArguments::push(bool b)
{
	push_back(new CSquirrelArgument(b));
}

void CSquirrelArguments::push(float f)
{
	push_back(new CSquirrelArgument(f));
}

void CSquirrelArguments::push(const char* c)
{
	push_back(new CSquirrelArgument(new String(c)));
}

void CSquirrelArguments::push(String str)
{
	push_back(new CSquirrelArgument(new String(str)));
}

void CSquirrelArguments::push(CSquirrelArguments array, bool isArray)
{
	push_back(new CSquirrelArgument(array, isArray));
}

void CSquirrelArguments::push(CSquirrelArguments* pArray, bool isArray)
{
	push_back(new CSquirrelArgument(pArray, isArray));
}

bool CSquirrelArguments::pushFromStack(SQVM* pVM, int idx)
{
	CSquirrelArgument * arg = new CSquirrelArgument();
	bool bValid = arg->pushFromStack(pVM, idx);

	if(bValid)
		push_back(arg);
	else
		delete arg;

	return bValid;
}

CSquirrelArgument CSquirrelArguments::pop()
{
	// Do we have an argument to pop?
	if(size() > 0)
	{
		// Get an argument from the front
		CSquirrelArgument * pArgument = front();
		pop_front();

		// Create a new instance of the argument
		CSquirrelArgument argument(*pArgument);

		// Delete the argument
		delete pArgument;

		// Return the new argument instance
		return argument;
	}

	// Nothing we can do, return a NULL argument
	return CSquirrelArgument();
}

void CSquirrelArguments::Serialise( RakNet::BitStream * bitStream )
{
	size_t size = this->size();

	if( size < 0x80 )
		bitStream->Write( (unsigned char)size );
	else if( size < 0x7f80 )
		bitStream->Write( (unsigned short)(size + 0x7f80) );
	else
	{
		bitStream->Write( (unsigned char)0xFF );
		bitStream->Write( size );
	}

	for( iterator iter = begin(); iter != end(); ++iter )
		(*iter)->Serialise( bitStream );
}

void CSquirrelArguments::Deserialise( RakNet::BitStream * bitStream )
{
	size_t size;
	unsigned char temp;
	bitStream->Read( temp );

	if( temp < 0x80 )
		size = temp;
	else if( temp < 0xFF )
	{
		size = (temp << 8);
		bitStream->Read( temp );
		size += temp;
		size -= 0x7f80;
	}
	else
		bitStream->Read( size );

	for( size_t i = 0; i < size; i++ )
		push_back( new CSquirrelArgument( bitStream ) );
}

/*
#ifndef _CLIENT
SquirrelArgumentInterface * CSquirrelArguments::Get( unsigned int i ) const
{
	if( i >= size() )
		return 0;

	unsigned int j = 0;
	for( const_iterator iter = begin(); iter != end(); ++ iter )
	{
		if( j == i )
			return *iter;

		++ j;
	}

	return 0;
}
#endif
*/