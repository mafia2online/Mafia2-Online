/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CBitStream.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"CString.h"

#define	BUFFER_STACK_ALLOCATION_SIZE			256

// RakNet
#ifdef BITS_TO_BYTES
#undef BITS_TO_BYTES
#endif
#define BITS_TO_BYTES(x)						(((x) + 7) >> 3)

// RakNet
#ifdef BYTES_TO_BITS
#undef BYTES_TO_BITS
#endif
#define BYTES_TO_BITS(x)						((x) << 3)

// Macros
#define	MUL_OF_8(x)								(((x) & 7) << 3)
#define	READ_TEMPLATE(size, out)				return ReadBits((unsigned char *)&out, (size * 8));
#define	READ_COMPRESSED_TEMPLATE(size, out)		return ReadCompressed((unsigned char *)&out, (size * 8), true);
#define	WRITE_TEMPLATE(size, in)				return WriteBits((unsigned char *)&in, (size * 8), true);
#define	WRITE_COMPRESSED_TEMPLATE(size, in)		return WriteCompressed((unsigned char *)&in, (size * 8), true);

class CBitStream
{

private:

	unsigned char			* m_pData;
	unsigned int			m_uiBufferSizeInBits;
	unsigned int			m_uiWriteOffsetInBits;
	unsigned int			m_uiReadOffsetInBits;
	bool					m_bCopyData;
	unsigned char			m_stackData[BUFFER_STACK_ALLOCATION_SIZE];

public:

	CBitStream( void );
	CBitStream( const unsigned int uiSizeInBytes );
	CBitStream( unsigned char * pBuffer, unsigned int uiSizeInBytes, bool bCopyData = false );
	~CBitStream( void );

	void					Reset( void );

	void					Write( const bool &bIn ) { WRITE_TEMPLATE(sizeof(bool), bIn); }
	void					Write( const char &cIn ) { WRITE_TEMPLATE(sizeof(char), cIn); }
	void					Write( const unsigned char &ucIn ) { WRITE_TEMPLATE(sizeof(unsigned char), ucIn); }
	void					Write( const short &sIn ) { WRITE_TEMPLATE(sizeof(short), sIn); }
	void					Write( const unsigned short & usIn ) { WRITE_TEMPLATE(sizeof(unsigned short), usIn); }
	void					Write( const int &iIn ) { WRITE_TEMPLATE(sizeof(int), iIn); }
	void					Write( const unsigned int &uiIn ) { WRITE_TEMPLATE(sizeof(unsigned int), uiIn); }
	void					Write( const long &lIn ) { WRITE_TEMPLATE(sizeof(long), lIn); }
	void					Write( const unsigned long &ulIn ) { WRITE_TEMPLATE(sizeof(unsigned long), ulIn); }
	void					Write( const float &fIn ) { WRITE_TEMPLATE(sizeof(float), fIn); }
	void					Write( const double &dIn ) { WRITE_TEMPLATE(sizeof(double), dIn); }
	void					Write( const CString &strIn );

	void					WriteCompressed( const bool &bIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(bool), bIn); }
	void					WriteCompressed( const char &cIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(char), cIn); }
	void					WriteCompressed( const unsigned char &ucIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(unsigned char), ucIn); }
	void                    WriteCompressed( const short &sIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(short), sIn); }
	void                    WriteCompressed( const unsigned short &usIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(unsigned short), usIn); }
	void                    WriteCompressed( const int &iIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(int), iIn); }
	void                    WriteCompressed( const unsigned int &uiIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(unsigned int), uiIn); }
	void                    WriteCompressed( const long &lIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(long), lIn); }
	void                    WriteCompressed( const unsigned long &ulIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(unsigned long), ulIn); }
	void                    WriteCompressed( const float &fIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(float), fIn); }
	void                    WriteCompressed( const double &dIn ) { WRITE_COMPRESSED_TEMPLATE(sizeof(double), dIn); }

	bool                    Read( bool& bOut ) { READ_TEMPLATE(sizeof(bool), bOut); }
	bool                    Read( char& cOut ) { READ_TEMPLATE(sizeof(char), cOut); }
	bool                    Read( unsigned char& ucOut ) { READ_TEMPLATE(sizeof(unsigned char), ucOut); }
	bool                    Read( short& sOut ) { READ_TEMPLATE(sizeof(short), sOut); }
	bool                    Read( unsigned short& usOut ) { READ_TEMPLATE(sizeof(unsigned short), usOut); }
	bool                    Read( int& iOut ) { READ_TEMPLATE(sizeof(int), iOut); }
	bool                    Read( unsigned int& uiOut ) { READ_TEMPLATE(sizeof(unsigned int), uiOut); }
	bool                    Read( long& lOut ) { READ_TEMPLATE(sizeof(long), lOut); }
	bool                    Read( unsigned long& ulOut ) { READ_TEMPLATE(sizeof(unsigned long), ulOut); }
	bool                    Read( float& fOut ) { READ_TEMPLATE(sizeof(float), fOut); }
	bool                    Read( double& dOut ) { READ_TEMPLATE(sizeof(double), dOut); }
	bool                    Read( CString &strOut );

	bool                    ReadCompressed( bool &bOut ) { READ_COMPRESSED_TEMPLATE(sizeof(bool), bOut); }
	bool                    ReadCompressed( char &cOut ) { READ_COMPRESSED_TEMPLATE(sizeof(char), cOut); }
	bool                    ReadCompressed( unsigned char &ucOut ) { READ_COMPRESSED_TEMPLATE(sizeof(unsigned char), ucOut); }
	bool                    ReadCompressed( short &sOut ) { READ_COMPRESSED_TEMPLATE(sizeof(short), sOut); }
	bool                    ReadCompressed( unsigned short &usOut ) { READ_COMPRESSED_TEMPLATE(sizeof(unsigned short), usOut); }
	bool                    ReadCompressed( int &iOut ) { READ_COMPRESSED_TEMPLATE(sizeof(int), iOut); }
	bool                    ReadCompressed( unsigned int &uiOut ) { READ_COMPRESSED_TEMPLATE(sizeof(unsigned int), uiOut); }
	bool                    ReadCompressed( long &lOut ) { READ_COMPRESSED_TEMPLATE(sizeof(long), lOut); }
	bool                    ReadCompressed( unsigned long &ulOut ) { READ_COMPRESSED_TEMPLATE(sizeof(unsigned long), ulOut); }
	bool                    ReadCompressed( float &fOut ) { READ_COMPRESSED_TEMPLATE(sizeof(float), fOut); }
	bool                    ReadCompressed( double &dOut ) { READ_COMPRESSED_TEMPLATE(sizeof(double), dOut); }

	void					Write( const char * inputByteArray, const unsigned int numberOfBytes );
	void					WriteCompressed( const unsigned char * inByteArray, const unsigned int uiSize, const bool unsignedData );

	bool					Read( char * outputByteArray, const unsigned int numberOfBytes );
	bool					ReadCompressed( unsigned char * inOutByteArray, unsigned in numberOfBitsToRead, bool bAlign = true );


};