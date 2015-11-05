/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CBitStream.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"CBitStream.h"

CBitStream::CBitStream( void )
{
	m_pData = m_stackData;
	m_uiBufferSizeInBits = (BUFFER_STACK_ALLOCATION_SIZE * 8);
	m_uiWriteOffsetInBits = 0;
	m_uiReadOffsetInBits = 0;
	m_bCopyData = true;
}

CBitStream::CBitStream( const unsigned int uiSizeInBytes )
{
	if( uiSizeInBytes <= BUFFER_STACK_ALLOCATION_SIZE )
	{
		m_pData = (unsigned char *)m_stackData;
		m_uiBufferSizeInBits = (BUFFER_STACK_ALLOCATION_SIZE * 8);
	}
	else
	{
		m_pData = (unsigned char *)malloc(uiSizeInBytes);
		m_uiBufferSizeInBits = (uiSizeInBytes << 3);
	}

	m_uiWriteOffsetInBits = 0;
	m_uiReadOffsetInBits = 0;
	m_bCopyData = true;
}

CBitStream::CBitStream( unsigned char * pBuffer, unsigned int uiSizeInBytes, bool bCopyData )
{
	if( bCopyData )
	{
		if( uiSizeInBytes > 0 )
		{
			if( uiSizeInBytes < BUFFER_STACK_ALLOCATION_SIZE )
			{
				m_pData = (unsigned char *)m_stackData;
				m_uiBufferSizeInBits = (BUFFER_STACK_ALLOCATION_SIZE << 3);
			}
			else
			{
				m_pData = (unsigned char *)malloc(uiSizeInBytes);
			}

			memcpy(m_pData, pBuffer, uiSizeInBytes);
		}
		else
		{
			m_pData = NULL;
		}
	}
	else
	{
		m_pData = (unsigned char *)pBuffer;
	}
	
	m_uiWriteOffsetInBits = (uiSizeInBytes << 3);
	m_uiReadOffsetInBits = 0;
	m_bCopyData = bCopyData;
	m_uiBufferSizeInBits = (uiSizeInBytes << 3);
}

CBitStream::~CBitStream( void )
{
	if( m_bCopyData && (m_uiBufferSizeInBits > (BUFFER_STACK_ALLOCATION_SIZE << 3)) )
		free(m_pData);
}