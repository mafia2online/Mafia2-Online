/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
#include "tinyxml.h"

#include <ctype.h>

#ifdef TIXML_USE_STL
#include <sstream>
#include <iostream>
#endif


bool _TiXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* TiXmlFOpen( const char* filename, const char* mode )
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		FILE* fp = 0;
		errno_t err = fopen_s( &fp, filename, mode );
		if ( !err && fp )
			return fp;
		return 0;
	#else
		return fopen( filename, mode );
	#endif
}

void _TiXmlBase::EncodeString( const TIXML_STRING& str, TIXML_STRING* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&'
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];

			#if defined(TIXML_SNPRINTF)
				TIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


_TiXmlNode::_TiXmlNode( NodeType _type ) : _TiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


_TiXmlNode::~_TiXmlNode()
{
	_TiXmlNode* node = firstChild;
	_TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void _TiXmlNode::CopyTo( _TiXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData;
}


void _TiXmlNode::Clear()
{
	_TiXmlNode* node = firstChild;
	_TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}


_TiXmlNode* _TiXmlNode::LinkEndChild( _TiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == _TiXmlNode::DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


_TiXmlNode* _TiXmlNode::InsertEndChild( const _TiXmlNode& addThis )
{
	if ( addThis.Type() == _TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}
	_TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


_TiXmlNode* _TiXmlNode::InsertBeforeChild( _TiXmlNode* beforeThis, const _TiXmlNode& addThis )
{
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == _TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	_TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


_TiXmlNode* _TiXmlNode::InsertAfterChild( _TiXmlNode* afterThis, const _TiXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == _TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return 0;
	}

	_TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


_TiXmlNode* _TiXmlNode::ReplaceChild( _TiXmlNode* replaceThis, const _TiXmlNode& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	_TiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool _TiXmlNode::RemoveChild( _TiXmlNode* removeThis )
{
	if ( removeThis->parent != this )
	{
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const _TiXmlNode* _TiXmlNode::FirstChild( const char * _value ) const
{
	const _TiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const _TiXmlNode* _TiXmlNode::LastChild( const char * _value ) const
{
	const _TiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const _TiXmlNode* _TiXmlNode::IterateChildren( const _TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}


const _TiXmlNode* _TiXmlNode::IterateChildren( const char * val, const _TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}


const _TiXmlNode* _TiXmlNode::NextSibling( const char * _value ) const
{
	const _TiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const _TiXmlNode* _TiXmlNode::PreviousSibling( const char * _value ) const
{
	const _TiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


void _TiXmlElement::RemoveAttribute( const char * name )
{
    #ifdef TIXML_USE_STL
	TIXML_STRING str( name );
	_TiXmlAttribute* node = attributeSet.Find( str );
	#else
	_TiXmlAttribute* node = attributeSet.Find( name );
	#endif
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const _TiXmlElement* _TiXmlNode::FirstChildElement() const
{
	const _TiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const _TiXmlElement* _TiXmlNode::FirstChildElement( const char * _value ) const
{
	const _TiXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const _TiXmlElement* _TiXmlNode::NextSiblingElement() const
{
	const _TiXmlNode* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const _TiXmlElement* _TiXmlNode::NextSiblingElement( const char * _value ) const
{
	const _TiXmlNode* node;

	for (	node = NextSibling( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const _TiXmlDocument* _TiXmlNode::GetDocument() const
{
	const _TiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


_TiXmlElement::_TiXmlElement (const char * _value)
	: _TiXmlNode( _TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef TIXML_USE_STL
_TiXmlElement::_TiXmlElement( const std::string& _value )
	: _TiXmlNode( _TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


_TiXmlElement::_TiXmlElement( const _TiXmlElement& copy)
	: _TiXmlNode( _TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );
}


void _TiXmlElement::operator=( const _TiXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
}


_TiXmlElement::~_TiXmlElement()
{
	ClearThis();
}


void _TiXmlElement::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		_TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const char* _TiXmlElement::Attribute( const char* name ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return node->Value();
	return 0;
}


#ifdef TIXML_USE_STL
const std::string* _TiXmlElement::Attribute( const std::string& name ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return &node->ValueStr();
	return 0;
}
#endif


const char* _TiXmlElement::Attribute( const char* name, int* i ) const
{
	const char* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = atoi( s );
		}
		else {
			*i = 0;
		}
	}
	return s;
}


#ifdef TIXML_USE_STL
const std::string* _TiXmlElement::Attribute( const std::string& name, int* i ) const
{
	const std::string* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = atoi( s->c_str() );
		}
		else {
			*i = 0;
		}
	}
	return s;
}
#endif


const char* _TiXmlElement::Attribute( const char* name, double* d ) const
{
	const char* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = atof( s );
		}
		else {
			*d = 0;
		}
	}
	return s;
}


#ifdef TIXML_USE_STL
const std::string* _TiXmlElement::Attribute( const std::string& name, double* d ) const
{
	const std::string* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = atof( s->c_str() );
		}
		else {
			*d = 0;
		}
	}
	return s;
}
#endif


int _TiXmlElement::QueryIntAttribute( const char* name, int* ival ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}


#ifdef TIXML_USE_STL
int _TiXmlElement::QueryIntAttribute( const std::string& name, int* ival ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}
#endif


int _TiXmlElement::QueryDoubleAttribute( const char* name, double* dval ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}


#ifdef TIXML_USE_STL
int _TiXmlElement::QueryDoubleAttribute( const std::string& name, double* dval ) const
{
	const _TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}
#endif


void _TiXmlElement::SetAttribute( const char * name, int val )
{
	char buf[64];
	#if defined(TIXML_SNPRINTF)
		TIXML_SNPRINTF( buf, sizeof(buf), "%d", val );
	#else
		sprintf( buf, "%d", val );
	#endif
	SetAttribute( name, buf );
}


#ifdef TIXML_USE_STL
void _TiXmlElement::SetAttribute( const std::string& name, int val )
{
   std::ostringstream oss;
   oss << val;
   SetAttribute( name, oss.str() );
}
#endif


void _TiXmlElement::SetDoubleAttribute( const char * name, double val )
{
	char buf[256];
	#if defined(TIXML_SNPRINTF)
		TIXML_SNPRINTF( buf, sizeof(buf), "%f", val );
	#else
		sprintf( buf, "%f", val );
	#endif
	SetAttribute( name, buf );
}


void _TiXmlElement::SetAttribute( const char * cname, const char * cvalue )
{
    #ifdef TIXML_USE_STL
	TIXML_STRING _name( cname );
	TIXML_STRING _value( cvalue );
	#else
	const char* _name = cname;
	const char* _value = cvalue;
	#endif

	_TiXmlAttribute* node = attributeSet.Find( _name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	_TiXmlAttribute* attrib = new _TiXmlAttribute( cname, cvalue );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		_TiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0, TIXML_ENCODING_UNKNOWN );
	}
}


#ifdef TIXML_USE_STL
void _TiXmlElement::SetAttribute( const std::string& name, const std::string& _value )
{
	_TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	_TiXmlAttribute* attrib = new _TiXmlAttribute( name, _value );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		_TiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0, TIXML_ENCODING_UNKNOWN );
	}
}
#endif


void _TiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) {
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const _TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	_TiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "</%s>", value.c_str() );
	}
}


void _TiXmlElement::CopyTo( _TiXmlElement* target ) const
{
	// superclass:
	_TiXmlNode::CopyTo( target );

	// Element class:
	// Clone the attributes, then clone the children.
	const _TiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	_TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}

bool _TiXmlElement::Accept( _TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) )
	{
		for ( const _TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


_TiXmlNode* _TiXmlElement::Clone() const
{
	_TiXmlElement* clone = new _TiXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* _TiXmlElement::GetText() const
{
	const _TiXmlNode* child = this->FirstChild();
	if ( child ) {
		const _TiXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


_TiXmlDocument::_TiXmlDocument() : _TiXmlNode( _TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

_TiXmlDocument::_TiXmlDocument( const char * documentName ) : _TiXmlNode( _TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef TIXML_USE_STL
_TiXmlDocument::_TiXmlDocument( const std::string& documentName ) : _TiXmlNode( _TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


_TiXmlDocument::_TiXmlDocument( const _TiXmlDocument& copy ) : _TiXmlNode( _TiXmlNode::DOCUMENT )
{
	copy.CopyTo( this );
}


void _TiXmlDocument::operator=( const _TiXmlDocument& copy )
{
	Clear();
	copy.CopyTo( this );
}


bool _TiXmlDocument::LoadFile( _TiXmlEncoding encoding )
{
	// See STL_STRING_BUG below.
	//StringToBuffer buf( value );

	return LoadFile( Value(), encoding );
}


bool _TiXmlDocument::SaveFile() const
{
	// See STL_STRING_BUG below.
//	StringToBuffer buf( value );
//
//	if ( buf.buffer && SaveFile( buf.buffer ) )
//		return true;
//
//	return false;
	return SaveFile( Value() );
}

bool _TiXmlDocument::LoadFile( const char* _filename, _TiXmlEncoding encoding )
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// Add an extra string to avoid the crash.
	TIXML_STRING filename( _filename );
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen( value.c_str (), "rb" );

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}
}

bool _TiXmlDocument::LoadFile( FILE* file, _TiXmlEncoding encoding )
{
	if ( !file )
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	TIXML_STRING data;
	data.reserve( length );

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external
	// parsed entities (including the document entity) on input, before parsing, by translating
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform.
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		if ( *p == 0xa ) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd ) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa ) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos ) {
		data.append( lastPos, p-lastPos );
	}
	delete [] buf;
	buf = 0;

	Parse( data.c_str(), 0, encoding );

	if (  Error() )
        return false;
    else
		return true;
}


bool _TiXmlDocument::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


bool _TiXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM )
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		fputc( TIXML_UTF_LEAD_0, fp );
		fputc( TIXML_UTF_LEAD_1, fp );
		fputc( TIXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}


void _TiXmlDocument::CopyTo( _TiXmlDocument* target ) const
{
	_TiXmlNode::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	_TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}


_TiXmlNode* _TiXmlDocument::Clone() const
{
	_TiXmlDocument* clone = new _TiXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void _TiXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const _TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}


bool _TiXmlDocument::Accept( _TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const _TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


const _TiXmlAttribute* _TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

/*
_TiXmlAttribute* _TiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const _TiXmlAttribute* _TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

/*
_TiXmlAttribute* _TiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/

void _TiXmlAttribute::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	TIXML_STRING n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == TIXML_STRING::npos) {
		if ( cfile ) {
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if ( cfile ) {
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int _TiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%d", ival ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int _TiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( TIXML_SSCANF( value.c_str(), "%lf", dval ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

void _TiXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(TIXML_SNPRINTF)
		TIXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void _TiXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(TIXML_SNPRINTF)
		TIXML_SNPRINTF( buf, sizeof(buf), "%lf", _value);
	#else
		sprintf (buf, "%lf", _value);
	#endif
	SetValue (buf);
}

int _TiXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}

double  _TiXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}


_TiXmlComment::_TiXmlComment( const _TiXmlComment& copy ) : _TiXmlNode( _TiXmlNode::COMMENT )
{
	copy.CopyTo( this );
}


void _TiXmlComment::operator=( const _TiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}


void _TiXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}


void _TiXmlComment::CopyTo( _TiXmlComment* target ) const
{
	_TiXmlNode::CopyTo( target );
}


bool _TiXmlComment::Accept( _TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


_TiXmlNode* _TiXmlComment::Clone() const
{
	_TiXmlComment* clone = new _TiXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void _TiXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		TIXML_STRING buffer;
		EncodeString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void _TiXmlText::CopyTo( _TiXmlText* target ) const
{
	_TiXmlNode::CopyTo( target );
	target->cdata = cdata;
}


bool _TiXmlText::Accept( _TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


_TiXmlNode* _TiXmlText::Clone() const
{
	_TiXmlText* clone = 0;
	clone = new _TiXmlText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


_TiXmlDeclaration::_TiXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: _TiXmlNode( _TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef TIXML_USE_STL
_TiXmlDeclaration::_TiXmlDeclaration(	const std::string& _version,
									const std::string& _encoding,
									const std::string& _standalone )
	: _TiXmlNode( _TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


_TiXmlDeclaration::_TiXmlDeclaration( const _TiXmlDeclaration& copy )
	: _TiXmlNode( _TiXmlNode::DECLARATION )
{
	copy.CopyTo( this );
}


void _TiXmlDeclaration::operator=( const _TiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}


void _TiXmlDeclaration::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	if ( cfile ) fprintf( cfile, "<?xml " );
	if ( str )	 (*str) += "<?xml ";

	if ( !version.empty() ) {
		if ( cfile ) fprintf (cfile, "version=\"%s\" ", version.c_str ());
		if ( str ) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if ( !encoding.empty() ) {
		if ( cfile ) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
		if ( str ) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if ( !standalone.empty() ) {
		if ( cfile ) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
		if ( str ) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if ( cfile ) fprintf( cfile, "?>" );
	if ( str )	 (*str) += "?>";
}


void _TiXmlDeclaration::CopyTo( _TiXmlDeclaration* target ) const
{
	_TiXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool _TiXmlDeclaration::Accept( _TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


_TiXmlNode* _TiXmlDeclaration::Clone() const
{
	_TiXmlDeclaration* clone = new _TiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}

_TiXmlStylesheetReference::_TiXmlStylesheetReference( const char * _type,
													const char * _href )
	: _TiXmlNode( _TiXmlNode::STYLESHEETREFERENCE )
{
	type = _type;
	href = _href;
}


#ifdef TIXML_USE_STL
_TiXmlStylesheetReference::_TiXmlStylesheetReference(	const std::string& _type,
													const std::string& _href )
	: _TiXmlNode( _TiXmlNode::STYLESHEETREFERENCE )
{
	type = _type;
	href = _href;
}
#endif


_TiXmlStylesheetReference::_TiXmlStylesheetReference( const _TiXmlStylesheetReference& copy )
	: _TiXmlNode( _TiXmlNode::STYLESHEETREFERENCE )
{
	copy.CopyTo( this );
}


void _TiXmlStylesheetReference::operator=( const _TiXmlStylesheetReference& copy )
{
	Clear();
	copy.CopyTo( this );
}


void _TiXmlStylesheetReference::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	if ( cfile ) fprintf( cfile, "<?xml-stylesheet " );
	if ( str )	 (*str) += "<?xml-stylesheet ";

	if ( !type.empty() ) {
		if ( cfile ) fprintf (cfile, "type=\"%s\" ", type.c_str ());
		if ( str ) { (*str) += "type=\""; (*str) += type; (*str) += "\" "; }
	}
	if ( !href.empty() ) {
		if ( cfile ) fprintf (cfile, "href=\"%s\" ", href.c_str ());
		if ( str ) { (*str) += "href=\""; (*str) += href; (*str) += "\" "; }
	}
	if ( cfile ) fprintf (cfile, "?>");
	if ( str )	 (*str) += "?>";
}

void _TiXmlStylesheetReference::CopyTo( _TiXmlStylesheetReference* target ) const
{
	_TiXmlNode::CopyTo( target );

	target->type = type;
	target->href = href;
}

bool _TiXmlStylesheetReference::Accept( _TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}

_TiXmlNode* _TiXmlStylesheetReference::Clone() const
{
	_TiXmlStylesheetReference* clone = new _TiXmlStylesheetReference();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void _TiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void _TiXmlUnknown::CopyTo( _TiXmlUnknown* target ) const
{
	_TiXmlNode::CopyTo( target );
}


bool _TiXmlUnknown::Accept( _TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


_TiXmlNode* _TiXmlUnknown::Clone() const
{
	_TiXmlUnknown* clone = new _TiXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


_TiXmlAttributeSet::_TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


_TiXmlAttributeSet::~_TiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void _TiXmlAttributeSet::Add( _TiXmlAttribute* addMe )
{
    #ifdef TIXML_USE_STL
	assert( !Find( TIXML_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.
	#else
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void _TiXmlAttributeSet::Remove( _TiXmlAttribute* removeMe )
{
	_TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


#ifdef TIXML_USE_STL
const _TiXmlAttribute* _TiXmlAttributeSet::Find( const std::string& name ) const
{
	for( const _TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

/*
_TiXmlAttribute*	_TiXmlAttributeSet::Find( const std::string& name )
{
	for( _TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}
*/
#endif


const _TiXmlAttribute* _TiXmlAttributeSet::Find( const char* name ) const
{
	for( const _TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}

/*
_TiXmlAttribute*	_TiXmlAttributeSet::Find( const char* name )
{
	for( _TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}
*/

#ifdef TIXML_USE_STL
std::istream& operator>> (std::istream & in, _TiXmlNode & base)
{
	TIXML_STRING tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0, TIXML_DEFAULT_ENCODING );
	return in;
}
#endif


#ifdef TIXML_USE_STL
std::ostream& operator<< (std::ostream & out, const _TiXmlNode & base)
{
	_TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}


std::string& operator<< (std::string& out, const _TiXmlNode& base )
{
	_TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}
#endif


_TiXmlHandle _TiXmlHandle::FirstChild() const
{
	if ( node )
	{
		_TiXmlNode* child = node->FirstChild();
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		_TiXmlNode* child = node->FirstChild( value );
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		_TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		_TiXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		_TiXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		_TiXmlNode* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		_TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


_TiXmlHandle _TiXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		_TiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return _TiXmlHandle( child );
	}
	return _TiXmlHandle( 0 );
}


bool _TiXmlPrinter::VisitEnter( const _TiXmlDocument& )
{
	return true;
}

bool _TiXmlPrinter::VisitExit( const _TiXmlDocument& )
{
	return true;
}

bool _TiXmlPrinter::VisitEnter( const _TiXmlElement& element, const _TiXmlAttribute* firstAttribute )
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for( const _TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += " ";
		attrib->Print( 0, 0, &buffer );
	}

	if ( !element.FirstChild() )
	{
		buffer += " />";
		DoLineBreak();
	}
	else
	{
		buffer += ">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;
	return true;
}


bool _TiXmlPrinter::VisitExit( const _TiXmlElement& element )
{
	--depth;
	if ( !element.FirstChild() )
	{
		// nothing.
	}
	else
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}


bool _TiXmlPrinter::Visit( const _TiXmlText& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		TIXML_STRING str;
		_TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
	}
	else
	{
		DoIndent();
		TIXML_STRING str;
		_TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}


bool _TiXmlPrinter::Visit( const _TiXmlDeclaration& declaration )
{
	DoIndent();
	declaration.Print( 0, 0, &buffer );
	DoLineBreak();
	return true;
}


bool _TiXmlPrinter::Visit( const _TiXmlComment& comment )
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}


bool _TiXmlPrinter::Visit( const _TiXmlUnknown& unknown )
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}

bool _TiXmlPrinter::Visit( const _TiXmlStylesheetReference& stylesheet )
{
	    DoIndent();
	    stylesheet.Print( 0, 0, &buffer );
	    DoLineBreak();
	    return true;
}
