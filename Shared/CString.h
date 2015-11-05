/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CString.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<string>
#include	<sstream>
#include	<vector>

class String
{

private:

	std::string			m_strString;
	size_t				m_sLimit;

	void				Initialise( );

public:

	static const unsigned int nPos;

	String();
	String(const char * szFormat, ...);
	~String();

	operator const char * () const;
	String& operator += (const char * szString);
	String& operator += (String strString);
	String& operator += (const unsigned char ucChar);
	bool operator == (const char * szString) const;
	bool operator == (const String strString) const;
	String& operator = (const unsigned char ucChar);

	const char			* Get() const;
	char				* GetData() const;
	unsigned char		GetChar(size_t sOffset) const;
	std::string			GetBase() { return m_strString; }

	void				Set(const char * szString);
	void				Set(const char * szString, unsigned int uiLength);
	void				SetChar(size_t sOffset, unsigned char cChar);
	void				Format(const char * szFormat, ...);

	bool				LimitTruncate();

	size_t				GetLength() const;

	void				clear();

	size_t				Find( const char * szString, size_t sPos = 0 ) const;
	size_t				Find( const unsigned char ucChar, size_t sPos = 0 ) const;
	size_t				Find( String strString, size_t sPos = 0 ) const;
	size_t				FindLastIndexOf( const char * szString, size_t sPos = 0 ) const;
	size_t				FindLastIndexOf( const unsigned char ucChar, size_t sPos = 0 ) const;
	size_t				FindLastIndexOf( String strString, size_t sPos = 0 ) const;

	void				Replace( size_t sOffset, const char * szString );
	void				Replace( size_t sOffset, String strString );

	void				Insert(size_t sOffset, const char * szString);

	String				substr( size_t sOffset, size_t sCount )
	{
		return String( m_strString.substr( sOffset, sCount ).c_str() );
	}

	void				Append(const char * szString);
	void				Append(const char * szString, unsigned int uiLength);
	void				Append(const unsigned char ucChar);

	void				AppendF(const char * szFormat, ...);

	bool				IsEmpty() const;

	void				Erase(size_t sOffset, size_t sCount);

	void				Resize( size_t sSize );

	int					ICompare(const char * szString) const;
	int					Compare(const char * szString) const;
	int					Compare(const String strString) const;

	bool				ToBoolean(void) const;
	void				FromBoolean(bool bValue);

	int					ToInteger(void) const;
	void				FromInteger(int iValue);

	float				ToFloat(void) const;
	void				FromFloat(float fValue);

	String&				ToLower(void);
	String&				ToUpper(void);

	void				Truncate( size_t sOffset );

	size_t				Substitute(const char * szString, const String strSubstitute);
	size_t				Substitute(const unsigned char ucChar, const String strSubstitute);
	size_t				Substitute(const String strString, const String strSubstitute);

	std::vector< String >	split( char delimitor );
	std::vector< String >	split( const char * szString );

	static String		DecimalToString( unsigned int uiDecimal )
	{
		String strReturn( "%p", uiDecimal );
		strReturn.Erase( (strReturn.GetLength() - 2), 2 );
		return strReturn;
	}

	static String		DecimalToStringNoErase( unsigned int uiDecimal )
	{
		return String( "%p", uiDecimal );
	}

	static unsigned long	HexToInt( String hex )
	{
		std::stringstream ss( hex.Get() );
		unsigned long ulResult = 0;
		ss >> std::hex >> ulResult;
		return ulResult;
	}

	int convertFromHex()
    {
		int value = 0;

        int a = 0;
		int b = (m_strString.length() - 1);

		for( ; b >= 0; a++, b-- )
        {
			if( m_strString[b] >= '0' && m_strString[b] <= '9' )
			{
				value += (m_strString[b] - '0') * (1 << (a * 4));
            }
            else
            {
				switch( m_strString[b] )
				{
                case 'A':
                case 'a':
                    value += 10 * (1 << (a * 4));
                    break;
                    
                case 'B':
                case 'b':
                    value += 11 * (1 << (a * 4));
                    break;

                case 'C':
                case 'c':
                    value += 12 * (1 << (a * 4));
                    break;
                    
                case 'D':
                case 'd':
                    value += 13 * (1 << (a * 4));
                    break;
                    
                case 'E':
                case 'e':
                    value += 14 * (1 << (a * 4));
                    break;
                    
                case 'F':
                case 'f':
                    value += 15 * (1 << (a * 4));
                    break;
				}
			}
		}
					
		return value;
	}

};