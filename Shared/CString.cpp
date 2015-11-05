/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CString.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "CString.h"
#include <stdarg.h>
#include <sstream>

#define		BUFFER_SIZE		8192
#ifdef _WIN32
#define	stricmp _stricmp
#else
#define stricmp strcasecmp
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
inline char* itoa(int value, char* str, int radix);
#endif

const unsigned int String::nPos = (unsigned int)-1;

void String::Initialise()
{
	m_sLimit = nPos;
}

String::String()
{
	Initialise();
}

String::String(const char * szFormat, ...)
{
	Initialise();

	if( szFormat )
	{
		char szString[BUFFER_SIZE];
		va_list vaArgs;
		va_start(vaArgs, szFormat);
		vsnprintf(szString, sizeof(szString), szFormat, vaArgs);
		va_end(vaArgs);
		Set(szString);
	}
}

String::~String()
{

}

String::operator const char * () const
{
	return m_strString.c_str();
}

String& String::operator += (const char * szString)
{
	Append(szString);
	return *this;
}

String& String::operator += (String strString)
{
	Append(strString.Get());
	return *this;
}

String& String::operator += (const unsigned char ucChar)
{
	Append(ucChar);
	return *this;
}

bool String::operator == (const char * szString) const
{
	return (Compare(szString) == 0);
}

bool String::operator == (const String strString ) const
{
	return (Compare(strString.Get()) == 0);
}

String& String::operator = (const unsigned char ucChar)
{
	char szString[2];
	szString[0] = ucChar;
	szString[1] = '\0';
	Set(szString);
	return *this;
}

void String::Set(const char * szString)
{
	// Is the string valid?
	if( szString )
	{
		// Set the string
		m_strString.assign(szString);

		// Ensure we haven't passed the string limit
		LimitTruncate();
	}
}

void String::Set(const char * szString, unsigned int uiLength)
{
	// Is the string valid?
	if( szString )
	{
		// Ensure the length is valid
		unsigned int uiStringLength = strlen(szString);

		if( uiLength > uiStringLength )
			uiLength = uiStringLength;

		// Set the string
		m_strString.assign(szString, uiLength);

		// Ensure we haven't passed the string limit
		LimitTruncate();
	}
}

void String::SetChar(size_t sOffset, unsigned char cChar)
{
	if( sOffset < GetLength() )
	{
		m_strString[sOffset] = cChar;
	}
}

void String::Format(const char * szFormat, ...)
{
	char szString[BUFFER_SIZE];
	va_list vaArgs;
	va_start(vaArgs, szFormat);
	vsprintf(szString, szFormat, vaArgs);
	va_end(vaArgs);
	Set(szString);
}

const char * String::Get() const
{
	return m_strString.c_str();
}

char * String::GetData() const
{
	return (char *)m_strString.data();
}

unsigned char String::GetChar(size_t sOffset) const
{
	if( sOffset < GetLength() )
		return m_strString[sOffset];

	return 0;
}

bool String::LimitTruncate()
{
	if(m_sLimit > nPos && GetLength() > m_sLimit)
	{
		m_strString[m_sLimit] = '\0';
		return true;
	}

	return false;
}

size_t String::GetLength() const
{
	return m_strString.size();
}

void String::clear()
{
	m_strString.clear();
}

size_t String::Find( const char * szString, size_t sPos ) const
{
	return m_strString.find( szString, sPos );
}

size_t String::Find( const unsigned char ucChar, size_t sPos ) const
{
	return m_strString.find( ucChar, sPos );
}

size_t String::Find( String strString, size_t sPos ) const
{
	return m_strString.find( strString.Get(), sPos );
}

size_t String::FindLastIndexOf( const char * szString, size_t sPos ) const
{
	return m_strString.find_last_of( szString, sPos );
}

size_t String::FindLastIndexOf( const unsigned char ucChar, size_t sPos ) const
{
	return m_strString.find_last_of( ucChar, sPos );
}

size_t String::FindLastIndexOf( String strString, size_t sPos ) const
{
	return m_strString.find_last_of( strString.Get(), sPos );
}

void String::Replace( size_t sOffset, const char * szString )
{
	// todo
}

void String::Replace( size_t sOffset, String strString )
{
	// todo
}

void String::Insert(size_t sOffset, const char * szString)
{
	m_strString.insert(sOffset, szString);

	// Ensure we haven't passed the string limit
	LimitTruncate();
}

void String::Append(const char * szString)
{
	// Is the string valid?
	if( szString )
	{
		// Copy the string to the end of our string
		m_strString.append(szString);

		// Ensure we haven't passed the end limit
		LimitTruncate();
	}
}

void String::Append(const char * szString, unsigned int uiLength)
{
	// Is the string valid?
	if( szString )
	{
		// Ensure the length is valid
		unsigned int uiStringLength = strlen(szString);

		if(uiLength > uiStringLength)
			uiLength = uiStringLength;

		// Copy the string to the end of our string
		m_strString.append(szString, uiLength);

		// Ensure we haven't passed the end limit
		LimitTruncate();
	}
}

void String::Append(const unsigned char ucChar)
{
	// Construct the string to append
	char szString[2];
	szString[0] = ucChar;
	szString[1] = '\0';

	// Copy the string to the end of our string
	m_strString.append(szString);

	// Ensure we haven't passed the string limit
	LimitTruncate();
}

void String::AppendF( const char * szFormat, ... )
{
	if( szFormat )
	{
		char szString[BUFFER_SIZE];
		va_list vaArgs;
		va_start(vaArgs, szFormat);
		vsprintf(szString, szFormat, vaArgs);
		va_end(vaArgs);

		// Append the string
		m_strString.append( szString );

		// Ensure we haven't passed the string limit 
		LimitTruncate( );
	}
}

bool String::IsEmpty() const
{
	return m_strString.empty();
}

void String::Erase(size_t sOffset, size_t sCount)
{
	m_strString.erase(sOffset, sCount);
}

void String::Resize( size_t sSize )
{
	m_strString.resize( sSize );
}

int String::ICompare(const char * szString) const
{
	return stricmp(Get(), szString);
}

int String::Compare(const char * szString) const
{
	return strcmp(Get(), szString);
}

int String::Compare(const String strString) const
{
	return strcmp(Get(), strString.Get());
}

int String::ToInteger( ) const
{
	return atoi( Get() );
}

void String::FromInteger(int iValue)
{
	char szBuffer[64];
	Set(itoa(iValue, szBuffer, 10));
}

bool String::ToBoolean( ) const
{
	if( ToInteger() == 1 || !ICompare("true") )
		return true;

	return false;
}

void String::FromBoolean(bool bValue)
{
	if(bValue)
		Set("true");
	else
		Set("false");
}

float String::ToFloat( ) const
{
	return (float)atof(Get());
}

void String::FromFloat(float fValue)
{
	std::ostringstream ss;
	ss << fValue;
	Set(ss.str().c_str());
}

String& String::ToLower()
{
	for(size_t i = 0; i < GetLength(); i++)
		m_strString[i] = tolower(m_strString[i]);

	return *this;
}

String& String::ToUpper(void)
{
	for(size_t i = 0; i < GetLength(); i++)
		m_strString[i] = toupper(m_strString[i]);

	return *this;
}

void String::Truncate( size_t sOffset )
{
	if( sOffset < GetLength() )
		m_strString.resize( sOffset );
}

size_t String::Substitute(const char * szString, const String strSubstitute)
{
	// Reset the find position and the instance count
	unsigned int uiFind = String::nPos;
	unsigned int uiInstanceCount = 0;

	// Loop until we have no more instances of the sequence left in the string
	while((uiFind = Find(szString)) != String::nPos)
	{
		// Erase this instance of the sequence
		Erase(uiFind, strlen(szString));

		// Insert the substitute where the instance of the sequence used to be
		Insert(uiFind, strSubstitute);

		// Increment the instance count
		uiInstanceCount++;
	}

	// Return the instance count
	return uiInstanceCount;
}

size_t String::Substitute(const unsigned char ucChar, const String strSubstitute)
{
	// Construct the string to substitute
	char szString[2];
	szString[0] = ucChar;
	szString[1] = '\0';

	return Substitute(szString, strSubstitute);
}

size_t String::Substitute(const String strString, const String strSubstitute)
{
	return Substitute(strString.Get(), strSubstitute);
}

std::vector< String > String::split( char delimitor )
{
	std::vector< String > tokens;
	int start = 0, end = 0;

	while( (end = m_strString.find( delimitor, start )) != std::string::npos )
	{
		tokens.push_back( m_strString.substr( start, end - start ).c_str() );
		start = end + 1;
	}

	tokens.push_back( m_strString.substr( start ).c_str() );

	return tokens;
}

std::vector< String > String::split( const char * szString )
{
	std::vector< String > tokens;
	int start = 0, end = 0;

	while( (end = m_strString.find( szString, start )) != std::string::npos )
	{
		tokens.push_back( m_strString.substr( start, end - start ).c_str() );
		start = end + strlen(szString);
	}

	tokens.push_back( m_strString.substr( start ).c_str() );

	return tokens;
}